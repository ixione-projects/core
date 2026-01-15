#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "defines.h"

static int luacore_sys_getcwd(lua_State *L) {
    size_t cwdsize = 1024;
    char *cwdbuf = xmalloc(L, cwdsize);
    char *retbuf = NULL;
    while ((retbuf = getcwd(cwdbuf, cwdsize)) == NULL && errno == ERANGE) {
        cwdsize += 32;
        cwdbuf = xrealloc(L, cwdbuf, cwdsize);
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

static int luacore_sys_access(lua_State *L) {
    const char *file = luaL_checkstring(L, 1);

    struct stat st;
    if (stat(file, &st) != 0) {
        lua_pushboolean(L, false);
        lua_pushstring(L, strerror(errno));
        return 2;
    }

    uid_t ruid = getuid();
    gid_t rgid = getgid();

    bool can_read;
    bool can_write;
    bool can_exec;

    if (ruid == 0) {
        can_read = true;
        can_write = true;
        can_exec = (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0;
    } else if (ruid == st.st_uid) {
        can_read = (st.st_mode & S_IRUSR) != 0;
        can_write = (st.st_mode & S_IWUSR) != 0;
        can_exec = (st.st_mode & S_IXUSR) != 0;
    } else if (rgid == st.st_gid) { // TODO: getgroups
        can_read = (st.st_mode & S_IRGRP) != 0;
        can_write = (st.st_mode & S_IWGRP) != 0;
        can_exec = (st.st_mode & S_IXGRP) != 0;
    } else {
        can_read = (st.st_mode & S_IROTH) != 0;
        can_write = (st.st_mode & S_IWOTH) != 0;
        can_exec = (st.st_mode & S_IXOTH) != 0;
    }

    bool result = true;
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "Read");
        if (!lua_isnil(L, -1)) {
            result &= lua_toboolean(L, -1) ? can_read : !can_read;
        }
        lua_pop(L, 1);

        lua_getfield(L, 2, "Write");
        if (!lua_isnil(L, -1)) {
            result &= lua_toboolean(L, -1) ? can_write : !can_write;
        }
        lua_pop(L, 1);

        lua_getfield(L, 2, "Exec");
        if (!lua_isnil(L, -1)) {
            result &= lua_toboolean(L, -1) ? can_exec : !can_exec;
        }
        lua_pop(L, 1);
    }
    lua_pushboolean(L, result);
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
