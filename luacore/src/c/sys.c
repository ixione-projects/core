#include <errno.h>
#include <lauxlib.h>
#include <limits.h>
#include <lua.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __cplusplus
#  define LUA_EXPORT extern "C"
#else
#  define LUA_EXPORT extern
#endif

static void *xmalloc(lua_State *L, size_t size) {
    char *ret = (char *)malloc(size);
    if (ret == NULL) {
        lua_pushstring(L, "malloc: failed");
        lua_error(L); // returns
    }
    return ret;
}

static void *xrealloc(lua_State *L, void *ptr, size_t size) {
    char *ret = (char *)realloc(ptr, size);
    if (ret == NULL) {
        lua_pushstring(L, "realloc: failed");
        lua_error(L); // returns
    }
    return ret;
}

static int luacore_sys_getcwd(lua_State *L) {
    size_t size = PATH_MAX;
    size += 2; // see xgetcwd
    char *cwd = xmalloc(L, size);
    char *ret = NULL;
    while ((ret = getcwd(cwd, size)) == NULL && errno == ERANGE) {
        size += 32;
        cwd = xrealloc(L, cwd, size);
        errno = 0;
    }

    if (ret == NULL) {
        lua_pushnil(L);
        lua_pushstring(L, strerror(errno));
    } else {
        lua_pushstring(L, ret);
        lua_pushnil(L);
    }

    free(ret);
    return 2;
}

static int luacore_sys_exists(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    if (access(name, F_OK) == 0) {
        lua_pushboolean(L, true);
        lua_pushnil(L);
    } else {
        lua_pushboolean(L, false);
        lua_pushstring(L, strerror(errno));
    }
    return 2;
}

static const luaL_Reg sys[] = {{"GetCwd", luacore_sys_getcwd},
                               {"Exists", luacore_sys_exists}};

LUA_EXPORT int luaopen_luacore_sys(lua_State *L) {
    const size_t nrec = sizeof(sys) / sizeof(sys[0]);
    lua_createtable(L, 0, nrec);
    for (size_t i = 0; i < nrec; ++i) {
        lua_pushcfunction(L, sys[i].func);
        lua_setfield(L, -2, sys[i].name);
    }
    return 1;
}
