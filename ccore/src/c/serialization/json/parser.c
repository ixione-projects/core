#include <stdlib.h>

#include "charsets/latin1.h"
#include "internal.h"

Parser *json_new_parser(Reader *reader) {
	return json_new_parser_with_charset(reader, &Latin1Charset);
}

Parser *json_new_parser_with_charset(Reader *reader, Charset *charset) {
	// TODO: assert reader and charset are non-null;
	Parser *p = (Parser *)malloc(sizeof(Parser));
	p->reader = reader;
	p->charset = charset;
	p->charset = charset;
	p->bufsize = p->buflen = 0;
	p->buf = NULL;
	p->start = p->current = 0;
	p->position = 0;
	p->eof = false;
	return p;
}

void json_parse() {
}
