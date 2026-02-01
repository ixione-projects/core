#include <stdlib.h>

#include "internal.h"

Parser *json_new_parser(Reader *reader, Charset *charset) {
	Parser *p = (Parser *)malloc(sizeof(Parser));
	p->reader = reader;
	p->charset = charset;
	p->bufsize = p->buflen = 0;
	p->buf = NULL;
	p->start = p->current = 0;
	return p;
}

void json_parse() {
}
