#ifndef CORE_H
#define CORE_H

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

void fail(const char *func, const char *file, unsigned int line, const char *expr, const char *fmt, ...);

#define panic(fmt, ...) fail(AssertFunc, AssertFile, AssertLine, NULL, fmt, ##__VA_ARGS__)
#define assert(expr, fmt, ...) ((expr) ? void(0) : fail(AssertFunc, AssertFile, AssertLine, #expr, fmt, ##__VA_ARGS__))

#ifdef __cplusplus
}
#endif

#endif // CORE_H
