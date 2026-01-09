#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#  if defined(__has_builtin) && __has_builtin(__builtin_FILE)
#    define AssertFile __builtin_FILE()
#    define AssertLine __builtin_LINE()
#  endif
#endif

#ifndef AssertFile
#  define AssertFile __FILE__
#endif
#ifndef AssertLine
#  define AssertLine __LINE__
#endif

#if defined(__GNUC__) || defined(__clang__)
#  define AssertFunc __extension__ __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#  define AssertFunc __FUNCSIG__
#else
#  define AssertFunc __func__
#endif

#define Panic(msg)                                                             \
    do {                                                                       \
        fprintf(stderr, "Panic: '%s'\n\tat %s(%s:%d)\n", msg, AssertFunc,      \
                AssertFile, AssertLine);                                       \
        abort();                                                               \
    } while (true)

#ifdef __cplusplus
}
#endif

#endif // DEFINITIONS_H
