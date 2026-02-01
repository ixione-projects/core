#include <stdlib.h>

#include "charsets.h"
#include "internal.h"
#include "unicode.h"

#define EmitBasicToken(codepoint, token) \
	case codepoint:                      \
		result = JSONToken##token;       \
		parser->current += length;       \
		break

static inline uint32_t json_next_codepoint(Parser *parser, unsigned int *length) {
	return parser->charset->decode(parser->buf + parser->current, length);
}

static bool json_is_whitespace(uint32_t ch) {
	return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

static inline void json_skip_whitespace(Parser *parser) {
	uint32_t ch = UnicodeReplacement;
	unsigned int length = 0;
	while (json_is_whitespace((ch = json_next_codepoint(parser, &length)))) {
		if (unlikely(ch == UnicodeReplacement)) {
			// TODO: error
		}
		parser->current += length;
	}
}

JSONTokenKind json_next_token(Parser *parser) {
	parser->start = parser->current;
	if (parser->start == parser->buflen) {
		parser->start = parser->current = 0;
		size_t bufsize = parser->bufsize;
		if (bufsize == 0) {
			parser->bufsize = 8192;
			parser->buf = (byte *)malloc(parser->bufsize);
		}

		ssize_t n = parser->reader->read(parser->reader, parser->buf, parser->bufsize);
		if (n == 0) {
			free(parser->buf);
			parser->bufsize = 0;
			parser->buf = NULL;
			return JSONTokenEOF;
		}

		if (unlikely(n < 0)) {
			// TODO: error
		} else {
			if ((size_t)n == parser->bufsize) {
				parser->bufsize = (parser->bufsize << 1);
				parser->buf = (byte *)realloc(parser->buf, parser->bufsize);
			} else if ((size_t)n < parser->bufsize / 2) {
				parser->bufsize = parser->bufsize >> 1;
				parser->buf = (byte *)realloc(parser->buf, parser->bufsize);
			}
			parser->buflen = (size_t)n;
		}
	}

	json_skip_whitespace(parser);

	JSONTokenKind result = JSONTokenError;

	uint32_t ch = UnicodeReplacement;
	unsigned int length = 0;
	switch (ch = json_next_codepoint(parser, &length)) {
		EmitBasicToken('[', ArrayBegin);
		EmitBasicToken('{', ObjectBegin);
		EmitBasicToken(']', ArrayEnd);
		EmitBasicToken('}', ObjectEnd);
		EmitBasicToken(':', NameSeparator);
		EmitBasicToken(',', ValueSeparator);
	default:
		if (unlikely(ch == UnicodeReplacement)) {
			// TODO: error
		}
		break;
	}

	json_skip_whitespace(parser);

	if (unlikely(result == JSONTokenError)) {
		// TODO: error
	}

	return result;
}
