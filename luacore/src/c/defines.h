#ifndef DEFINE_H
#define DEFINE_H

#include <lauxlib.h>

#ifdef __cplusplus
#  define LUA_EXPORT extern "C"
#else
#  define LUA_EXPORT extern
#endif

void *l_malloc(lua_State *L, size_t size);

void *l_realloc(lua_State *L, void *ptr, size_t size);

#endif // DEFINE_H
