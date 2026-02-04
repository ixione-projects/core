#include <stdlib.h>

#include "encodings.h"
#include "internal.h"
#include "unicode.h"

static inline uint32_t json_next_codepoint(Parser *parser, unsigned int *length) {
	return parser->encoding->decode(parser->buf + parser->current, length);
}

static bool json_is_whitespace(uint32_t ch) {
	return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

static inline void json_skip_whitespace(Parser *parser) {
	uint32_t ch = UnicodeError;
	unsigned int length = 0;
	while (json_is_whitespace((ch = json_next_codepoint(parser, &length)))) {
		if (unlikely(ch == UnicodeError)) {
			// TODO: error
		}
		parser->current += length;
		parser->position += length;
	}
}

JsonTokenKind json_next_token(Parser *parser) {
#define EmitBasicToken(codepoint, token) \
	case codepoint:                      \
		result = JsonToken##token;       \
		parser->current += length;       \
		parser->position += length;      \
		break

	if (parser->eof) {
		return JsonTokenEOF;
	}

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
			parser->eof = true;
			return JsonTokenEOF;
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

	JsonTokenKind result = JsonTokenError;

	uint32_t ch = UnicodeError;
	unsigned int length = 0;
	switch (ch = json_next_codepoint(parser, &length)) {
	case '"':
		// TODO: parser string
		break;
		EmitBasicToken(',', ValueSeparator);
		EmitBasicToken(':', NameSeparator);
		EmitBasicToken('[', ArrayBegin);
		EmitBasicToken(']', ArrayEnd);
		EmitBasicToken('{', ObjectBegin);
		EmitBasicToken('}', ObjectEnd);
	default:
		if (unlikely(ch == UnicodeError)) {
			// TODO: error
		}
		break;
	}

	json_skip_whitespace(parser);

	if (unlikely(result == JsonTokenError)) {
		// TODO: error
	}

	return result;

#undef EmitBasicToken
}
