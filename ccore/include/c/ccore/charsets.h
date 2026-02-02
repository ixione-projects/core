#ifndef CHARSET_H
#define CHARSET_H

#include <stdint.h>

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	CharsetUTF8,
	CharsetUTF16,
	CharsetUTF32,
	CharsetLatin1,
} CharsetKind;

typedef struct {
	int (*encode)(uint32_t codepoint, byte *out);
	uint32_t (*decode)(const byte *bytes, unsigned int *length);
} Charset;

#ifdef __cplusplus
}
#endif

#endif // CHARSET_H
