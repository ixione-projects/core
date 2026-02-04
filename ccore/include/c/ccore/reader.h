#ifndef READER_H
#define READER_H

#include <sys/types.h>

#include "core.h"

typedef struct Reader {
	ssize_t (*read)(struct Reader *self, byte *bytes, size_t n);
} Reader;

#endif // READER_H
