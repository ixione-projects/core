#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#  if defined(__has_builtin) && __has_builtin(__builtin_FILE)
#	define AssertFile __builtin_FILE()
#	define AssertLine __builtin_LINE()
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

#if defined(__GNUC__) || defined(__clang__)
#  define likely(x) (__builtin_expect(((x) != false), true))
#  define unlikely(x) (__builtin_expect(((x) != false), false))
#else
#  define likely(x) (x)
#  define unlikely(x) (x)
#endif

// TODO: fail should also unwind and print stack information - rename to unwind
void fail(const char *func, const char *file, unsigned int line, const char *expr, const char *fmt, ...);

#define panic(fmt, ...) fail(AssertFunc, AssertFile, AssertLine, NULL, fmt, ##__VA_ARGS__)
#define assert(expr, fmt, ...) ((unlikely(expr)) ? void(0) : fail(AssertFunc, AssertFile, AssertLine, #expr, fmt, ##__VA_ARGS__))

#define min(l, r) ((l) < (r) ? (l) : (r))
#define max(l, r) ((l) > (r) ? (l) : (r))

#ifdef __cplusplus
}
#endif

#endif // CORE_H
