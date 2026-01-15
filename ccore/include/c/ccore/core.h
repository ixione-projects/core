#ifndef CORE_H
#define CORE_H

#include <stdarg.h>
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

#define Panic(fmt, ...)                                                        \
    do {                                                                       \
        char *msg = (char *)malloc(sizeof(char));                              \
        if (msg == NULL) {                                                     \
            fprintf(stderr, "Panic:\n\tat %s(%s:%d)\n", AssertFunc,            \
                    AssertFile, AssertLine);                                   \
            abort();                                                           \
        }                                                                      \
                                                                               \
        msg[0] = '\0';                                                         \
        size_t length = (size_t)snprintf(NULL, 0, fmt, ##__VA_ARGS__);         \
        if (length >= 0) {                                                     \
            msg = (char *)realloc(msg, length + 1);                            \
            if (msg != NULL) {                                                 \
                snprintf(msg, length + 1, fmt, ##__VA_ARGS__);                 \
            }                                                                  \
        }                                                                      \
        fprintf(stderr, "Panic: '%s'\n\tat %s(%s:%d)\n", msg, AssertFunc,      \
                AssertFile, AssertLine);                                       \
        abort();                                                               \
    } while (true)

#ifdef __cplusplus
}
#endif

#endif // CORE_H
