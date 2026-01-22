#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"

static int luacore_sys_getcwd(lua_State *L) {
	size_t cwdsize = 1024;
	char *cwdbuf = l_malloc(L, cwdsize);
	char *retbuf = NULL;
	while ((retbuf = getcwd(cwdbuf, cwdsize)) == NULL && errno == ERANGE) {
		cwdsize += 32;
		cwdbuf = l_realloc(L, cwdbuf, cwdsize);
		errno = 0;
	}

	int n = 0;
	if (retbuf == NULL) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		n = 2;
	} else {
		lua_pushstring(L, retbuf);
		n = 1;
	}

	free(cwdbuf);
	return n;
}

static int8_t l_checkmode(const char *arg) {
	int8_t mode = 0;
	while (*arg) {
		int bit;
		switch (*(arg++)) {
		case 'r':
			bit = 4;
			break;
		case 'w':
			bit = 2;
			break;
		case 'x':
			bit = 1;
			break;
		default:
			return 0;
		}
		if (mode & bit) {
			return 0;
		}
		mode |= bit;
	}
	return mode;
}

static int luacore_sys_access(lua_State *L) {
	const char *file = luaL_checkstring(L, 1);

	int8_t mode = F_OK;
	if (lua_gettop(L) > 1) {
		luaL_argcheck(L, (bool)(mode = l_checkmode(luaL_checkstring(L, 2))), 2, "invalid mode");
	}

	if (access(file, mode) != 0) {
		lua_pushboolean(L, false);
		lua_pushstring(L, strerror(errno));
		return 2;
	}
	lua_pushboolean(L, true);
	return 1;
}

static const luaL_Reg sys[] = {{"GetCwd", luacore_sys_getcwd},
							   {"Access", luacore_sys_access}};

LUA_EXPORT int luaopen_luacore_sys(lua_State *L) {
	const size_t nrec = sizeof(sys) / sizeof(sys[0]);
	lua_createtable(L, 0, nrec);
	for (size_t i = 0; i < nrec; ++i) {
		lua_pushcfunction(L, sys[i].func);
		lua_setfield(L, -2, sys[i].name);
	}
	return 1;
}
