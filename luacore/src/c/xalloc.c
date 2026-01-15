#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

void *xmalloc(lua_State *L, size_t size) {
    char *ret = (char *)malloc(size);
    if (ret == NULL) {
        lua_pushstring(L, strerror(errno));
        lua_error(L); // returns
    }
    return ret;
}

void *xrealloc(lua_State *L, void *ptr, size_t size) {
    char *ret = (char *)realloc(ptr, size);
    if (ret == NULL) {
        lua_pushstring(L, strerror(errno));
        lua_error(L); // returns
    }
    return ret;
}
