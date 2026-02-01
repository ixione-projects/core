#include <stdlib.h>
#include <string.h>

#include "strings/strings_reader.h"

struct StringReader {
	Reader interface;
	String *ref_s;
	size_t offset;
};

static ssize_t string_read(struct Reader *self, byte *bytes, size_t n) {
	struct StringReader *sr = (struct StringReader *)self;
	size_t len = sr->ref_s->size;
	// TODO: assert offset <= ref_s->length;
	if (sr->offset == len) {
		return 0;
	}
	size_t nread = min(n, len - sr->offset);
	memcpy(bytes, sr->ref_s->value + sr->offset, nread);
	sr->offset += nread;
	return (ssize_t)nread;
}

Reader *NewStringReader(String *ref_s) {
	struct StringReader *sr = (struct StringReader *)malloc(sizeof(struct StringReader));
	sr->interface = (Reader){
	    .read = string_read,
	};
	sr->ref_s = ref_s;
	sr->offset = 0;
	return &sr->interface;
}
