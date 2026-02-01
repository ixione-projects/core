#ifndef READER_H
#define READER_H

#include <sys/types.h>

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Reader {
	ssize_t (*read)(struct Reader *self, byte *bytes, size_t n);
} Reader;

#ifdef __cplusplus
}
#endif

#endif // READER_H
