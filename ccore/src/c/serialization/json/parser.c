#include <stdlib.h>

#include "encodings/latin1.h"
#include "internal.h"

Parser *json_new_parser(Reader *reader) {
	return json_new_parser_with_encoding(reader, &Latin1);
}

Parser *json_new_parser_with_charset(Reader *reader, Encoding *encoding) {
	// TODO: assert reader and charset are non-null;
	Parser *p = (Parser *)malloc(sizeof(Parser));
	p->reader = reader;
	p->encoding = encoding;
	p->bufsize = p->buflen = 0;
	p->buf = NULL;
	p->start = p->current = 0;
	p->position = 0;
	p->eof = false;
	return p;
}

void json_parse() {
}
