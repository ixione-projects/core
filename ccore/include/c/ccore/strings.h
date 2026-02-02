#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	size_t size;
	const char *value;
} String;

String *NewString(const char *value, size_t size);

#ifdef __cplusplus
}
#endif

#endif // STRINGS_H
