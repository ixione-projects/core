#include "encodings/latin1.h"

static int encode(uint32_t codepoint, byte *out) {
	out[0] = (byte)codepoint;
	return 1;
}

static uint32_t decode(const byte *bytes, unsigned int *length) {
	*length = 1;
	return (uint32_t)bytes[0];
}

Encoding Latin1 = {
    .encode = encode,
    .decode = decode,
};
