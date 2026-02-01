#include <stdlib.h>
#include <string.h>

#include "stringss.h"

String *NewString(const char *value, size_t size) {
	String *s = (String *)malloc(sizeof(String));
	s->size = size;
	char *sv = (char *)malloc(s->size);
	memcpy(sv, value, s->size);
	s->value = sv;
	return s;
}
