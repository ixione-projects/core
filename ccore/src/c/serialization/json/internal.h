#ifndef INTERNAL_H
#define INTERNAL_H

#include <stddef.h>

#include "ccore/charsets.h"
#include "ccore/reader.h"

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
#define ForEachJSONToken(op)    \
	op(JSONTokenArrayBegin)     \
	op(JSONTokenArrayEnd)       \
	op(JSONTokenObjectBegin)    \
	op(JSONTokenObjectEnd)      \
	op(JSONTokenNameSeparator)  \
	op(JSONTokenValueSeparator) \
	op(JSONTokenString)         \
	op(JSONTokenNumber)         \
	op(JSONTokenFalse)          \
	op(JSONTokenNull)           \
	op(JSONTokenTrue)           \
	op(JSONTokenError)          \
	op(JSONTokenEOF)
// clang-format on

typedef enum {
#define Identifier(tt) tt,
	ForEachJSONToken(Identifier)
#undef Identifier
} JSONTokenKind;

inline const char *json_token_to_string(JSONTokenKind tt) {
#define Stringify(tt) \
case tt:              \
	return #tt;
	switch (tt) {
		ForEachJSONToken(Stringify);
	default:
		return NULL;
	}

#undef Stringify
}

typedef struct {
	Reader *reader;
	size_t bufsize;
	byte *buf;
	size_t buflen;
	Charset *charset;

	size_t start;
	size_t current;

	size_t position;
	bool eof;
} Parser;

Parser *json_new_parser(Reader *reader);
Parser *json_new_parser_with_charset(Reader *reader, Charset *charset);

JSONTokenKind json_next_token(Parser *parser);

#ifdef __cplusplus
}
#endif

#endif // INTERNAL_H
