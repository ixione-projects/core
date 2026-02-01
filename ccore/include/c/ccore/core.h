#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#  define export extern "C"
#else
#  define export extern
#endif

#define byte unsigned char

#define min(l, r) ((l) < (r) ? (l) : (r))
#define max(l, r) ((l) > (r) ? (l) : (r))

#if defined(__has_builtin) && __has_builtin(__builtin_expect)
#  define likely(x) (__builtin_expect(((x) != false), true))
#  define unlikely(x) (__builtin_expect(((x) != false), false))
#else
#  define likely(x) (x)
#  define unlikely(x) (x)
#endif

#if defined(__has_builtin)
#  if __has_builtin(__builtin_FILE)
#	define AssertFile __builtin_FILE()
#  endif
#  if __has_builtin(__builtin_LINE)
#	define AssertLine __builtin_LINE()
#  endif
#endif

#if !defined(AssertFile) || !defined(AssertLine)
#  define AssertFile __FILE__
#  define AssertLine __LINE__
#endif

#if defined(__GNUC__) || defined(__clang__)
#  define AssertFunc __extension__ __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#  define AssertFunc __FUNCSIG__
#else
#  define AssertFunc __func__
#endif

// TODO: fail should also unwind and print stack information - rename to unwind
void fail(const char *func, const char *file, unsigned int line, const char *expr, const char *fmt, ...);

#define Panic(fmt, ...) fail(AssertFunc, AssertFile, AssertLine, NULL, fmt, ##__VA_ARGS__)
#define Assert(expr, fmt, ...) ((unlikely(expr)) ? void(0) : fail(AssertFunc, AssertFile, AssertLine, #expr, fmt, ##__VA_ARGS__))

#ifdef __cplusplus
}
#endif

#endif // CORE_H
