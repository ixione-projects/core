#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"

void fail(const char *func, const char *file, unsigned int line, const char *expr, const char *fmt, ...) {
    char *msg = (char *)malloc(sizeof(char));
    if (msg == NULL) {
        if (expr == NULL) {
            fprintf(stderr, "Panicked:\n\tat %s(%s:%d)\n", AssertFunc, AssertFile, AssertLine);
        } else {
            fprintf(stderr, "Assertion `%s` failed:\n\tat %s(%s:%d)\n", expr, AssertFunc, AssertFile, AssertLine);
        }
        abort();
    }
    msg[0] = '\0';

    va_list args;
    va_start(args, fmt);

    int length = vsnprintf(NULL, 0, fmt, args);
    if (length >= 0) {
        msg = (char *)realloc(msg, (size_t)length + 1);
        if (msg != NULL) {
            vsnprintf(msg, (size_t)length + 1, fmt, args);
        }
    }
    va_end(args);

    if (expr == NULL) {
        fprintf(stderr, "Panicked: '%s'\n\tat %s(%s:%d)\n", msg, func, file, line);
    } else {
        fprintf(stderr, "Assertion `%s` failed: '%s'\n\tat %s(%s:%d)\n", expr, msg, func, file, line);
    }
    abort();
}
