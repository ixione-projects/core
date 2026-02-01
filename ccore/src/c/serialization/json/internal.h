#ifndef INTERNAL_H
#define INTERNAL_H

#include "charsets.h"
#include "reader.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	/* Structural Characters */
	JSONTokenArrayBegin,
	JSONTokenArrayEnd,
	JSONTokenObjectBegin,
	JSONTokenObjectEnd,
	JSONTokenNameSeparator,
	JSONTokenValueSeparator,

	JSONTokenString,
	JSONTokenNumber,

	/* Literal Names */
	JSONTokenFalse,
	JSONTokenNull,
	JSONTokenTrue,

	JSONTokenError,
	JSONTokenEOF,
} JSONTokenKind;

typedef struct {
	Reader *reader;
	size_t bufsize;
	byte *buf;
	size_t buflen;
	Charset *charset;

	size_t start;
	size_t current;
} Parser;

inline const char *json_token_to_string(JSONTokenKind tt) {
	switch (tt) {
	case JSONTokenArrayBegin:
		return "JSONTokenArrayBegin";
	case JSONTokenArrayEnd:
		return "JSONTokenArrayEnd";
	case JSONTokenObjectBegin:
		return "JSONTokenObjectBegin";
	case JSONTokenObjectEnd:
		return "JSONTokenObjectEnd";
	case JSONTokenNameSeparator:
		return "JSONTokenNameSeparator";
	case JSONTokenValueSeparator:
		return "JSONTokenValueSeparator";
	case JSONTokenString:
		return "JSONTokenString";
	case JSONTokenNumber:
		return "JSONTokenNumber";
	case JSONTokenFalse:
		return "JSONTokenFalse";
	case JSONTokenNull:
		return "JSONTokenNull";
	case JSONTokenTrue:
		return "JSONTokenTrue";
	case JSONTokenError:
		return "JSONTokenError";
	case JSONTokenEOF:
		return "JSONTokenEOF";
	}
}

Parser *json_new_parser(Reader *reader, Charset *charset);

JSONTokenKind json_next_token(Parser *parser);

#ifdef __cplusplus
}
#endif

#endif // INTERNAL_H
