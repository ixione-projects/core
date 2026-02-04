#ifndef INTERNAL_H
#define INTERNAL_H

#include <stddef.h>

#include "ccore/encodings.h"
#include "ccore/reader.h"

// clang-format off
#define ForEachJsonToken(op)    \
	op(JsonTokenArrayBegin)     \
	op(JsonTokenArrayEnd)       \
	op(JsonTokenObjectBegin)    \
	op(JsonTokenObjectEnd)      \
	op(JsonTokenNameSeparator)  \
	op(JsonTokenValueSeparator) \
	op(JsonTokenString)         \
	op(JsonTokenNumber)         \
	op(JsonTokenFalse)          \
	op(JsonTokenNull)           \
	op(JsonTokenTrue)           \
	op(JsonTokenError)          \
	op(JsonTokenEOF)
// clang-format on

typedef enum {
#define Identifier(tt) tt,
	ForEachJsonToken(Identifier)
#undef Identifier
} JsonTokenKind;

inline const char *json_token_to_string(JsonTokenKind tt) {
#define Stringify(tt) \
case tt:              \
	return #tt;
	switch (tt) {
		ForEachJsonToken(Stringify);
	default:
		return NULL;
	}

#undef Stringify
}

typedef struct {
	Reader *reader;
	Encoding *encoding;

	size_t bufsize;
	byte *buf;
	size_t buflen;

	size_t start;
	size_t current;

	size_t position;
	bool eof;
} Parser;

Parser *json_new_parser(Reader *reader);
Parser *json_new_parser_with_encoding(Reader *reader, Encoding *encoding);

JsonTokenKind json_next_token(Parser *parser);

#endif // INTERNAL_H
