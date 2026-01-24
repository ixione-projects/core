#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ccore/core.h>

#include "defines.h"

static int luacore_canonicalize(lua_State *L) {
	size_t filelen = 0;
	const char *input = luaL_checklstring(L, 1, &filelen);

	size_t filesize = 1024;
	char *file = (char *)l_malloc(L, max(filesize, filelen + 1));
	memcpy(file, input, filelen);
	file[filelen] = '\0';

	if (file[0] != '/') {
		size_t size = 1024;
		char *cwdbuf = (char *)l_malloc(L, size);
		char *retbuf = NULL;
		while ((retbuf = getcwd(cwdbuf, size)) == NULL && errno == ERANGE) {
			size += 32;
			cwdbuf = (char *)l_realloc(L, cwdbuf, size);
			errno = 0;
		}

		if (retbuf == NULL) {
			free(file);
			lua_pushnil(L);
			lua_pushstring(L, strerror(errno));
			return 2;
		}

		size_t retbuflen = strlen(retbuf);
		if (size < filelen + retbuflen) {
			retbuf = (char *)l_realloc(L, retbuf, filelen + retbuflen + 2);
		}
		retbuf[retbuflen] = '/';
		memcpy(retbuf + retbuflen + 1, file, filelen);
		free(file);
		filelen += retbuflen + 1;
		retbuf[filelen] = '\0';
		file = retbuf;
	}

	size_t pathbufsize = 1024;
	char *pathbuf = (char *)l_malloc(L, pathbufsize);
	size_t pathlen = 0;
	size_t retbufsize = 1024;
	char *retbuf = (char *)l_malloc(L, retbufsize);

	char *start = file;
	if (filelen <= 0) {
		free(file);
		free(pathbuf);
		free(retbuf);
		lua_pushstring(L, "internal error: length of 'file' must be > 0");
		lua_error(L); // return
		return 0;
	}
	char *curr = file + 1;
	while (*start != '\0') {
		while (*curr != '/' && *curr != '\0') {
			curr += 1;
		}

		if (curr - start <= 0) {
			free(file);
			free(pathbuf);
			free(retbuf);
			lua_pushstring(L, "internal error: length of file 'component' must be > 0");
			lua_error(L); // return
			return 0;
		}

		size_t componentlen = (size_t)(curr - start);
		bool is_dot = componentlen == 1 || (componentlen == 2 && start[1] == '.');
		bool is_dot_dot = componentlen == 3 && start[1] == '.' && start[2] == '.';
		if ((is_dot || is_dot_dot) && pathlen > 0) {
			if (is_dot_dot && pathlen > 1) {
				pathlen -= 1;
				while (pathlen > 0 && pathbuf[pathlen] != '/') {
					pathlen -= 1;
				}
				pathbuf[pathlen] = '\0';
			}
			start = curr;
			curr += 1;
			continue;
		}

		if (is_dot || is_dot_dot) {
			componentlen = 1;
		}

		if (pathbufsize <= pathlen + componentlen) {
			pathbufsize += pathlen + componentlen + 1;
			pathbuf = (char *)l_realloc(L, pathbuf, pathbufsize);
		}

		if (pathlen > 1) {
			memcpy(pathbuf + pathlen, start, componentlen);
			pathlen += componentlen;
			pathbuf[pathlen] = '\0';
		} else {
			memcpy(pathbuf, start, componentlen);
			pathlen = componentlen;
			pathbuf[pathlen] = '\0';
		}

		// TODO: keep track of traversed paths
		ssize_t n = readlink(pathbuf, retbuf, retbufsize);
		while (n >= (ssize_t)retbufsize) {
			retbufsize = (retbufsize << 1) + retbufsize;
			if (retbufsize >= LONG_MAX) {
				free(file);
				free(pathbuf);
				free(retbuf);
				lua_pushstring(L, "internal error: 'retbuf' size must be <= LONG_MAX");
				lua_error(L); // return
				return 0;
			}
			retbuf = (char *)l_realloc(L, retbuf, retbufsize);
			n = readlink(pathbuf, retbuf, retbufsize);
		}

		if (errno && errno != EINVAL) {
			free(file);
			free(pathbuf);
			free(retbuf);
			lua_pushnil(L);
			lua_pushstring(L, strerror(errno));
			return 2;
		}

		if (n >= 0L) {
			size_t postfixlen = filelen - pathlen;
			if (retbuf[0] == '/') {
				pathlen = 0;
				if ((size_t)n != pathlen) {
					filelen = (size_t)n + postfixlen;
					if (filesize <= filelen) {
						filesize += filelen + 1;
						file = (char *)l_realloc(L, file, filesize);
					}
					memmove(file + n, curr, postfixlen);
					file[filelen] = '\0';
				}
				memcpy(file, retbuf, (size_t)n);
				curr = file;
			} else {
				pathlen -= componentlen;
				if ((size_t)n != componentlen) {
					filelen = pathlen + (size_t)n + postfixlen + 1;
					if (filesize <= filelen) {
						filesize += filelen + 1;
						file = (char *)l_realloc(L, file, filesize);
					}
					memmove(file + pathlen + n, curr, postfixlen);
					file[filelen] = '\0';
				}
				file[pathlen] = '/';
				memcpy(file + pathlen + 1, retbuf, (size_t)n);
				curr = file + pathlen;
			}
		}

		struct stat st;
		if (stat(pathbuf, &st) != 0) {
			free(file);
			free(pathbuf);
			free(retbuf);
			lua_pushnil(L);
			lua_pushstring(L, strerror(errno));
			return 2;
		}

		start = curr;
		curr += 1;
	}
	lua_pushstring(L, pathbuf);

	free(file);
	free(retbuf);
	free(pathbuf);
	return 1;
}

LUA_EXPORT int luaopen_luacore_canonicalize(lua_State *L) {
	lua_pushcfunction(L, luacore_canonicalize);
	return 1;
}
