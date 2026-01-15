#ifndef DEFINE_H
#define DEFINE_H

#include <lauxlib.h>

#ifdef __cplusplus
#  define LUA_EXPORT extern "C"
#else
#  define LUA_EXPORT extern
#endif

void *xmalloc(lua_State *L, size_t size);

void *xrealloc(lua_State *L, void *ptr, size_t size);

#endif // DEFINE_H
