#ifndef ENCODINGS_H
#define ENCODINGS_H

#include <stdint.h>

#include "core.h"

typedef enum {
	EncodingUtf8,
	EncodingUtf16,
	EncodingUtf32,
	EncodingLatin1,
} EncodingKind;

typedef struct {
	int (*encode)(uint32_t codepoint, byte *out);
	uint32_t (*decode)(const byte *bytes, unsigned int *length);
} Encoding;

#endif // ENCODINGS_H
