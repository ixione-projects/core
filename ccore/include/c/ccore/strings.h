#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

#include "core.h"

typedef struct {
	size_t size;
	const char *value;
} String;

export String *NewString(const char *value, size_t size);

#endif // STRINGS_H
