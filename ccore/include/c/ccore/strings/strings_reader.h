#ifndef STRINGS_READER_H
#define STRINGS_READER_H

#include "reader.h"
#include "stringss.h"

#ifdef __cplusplus
extern "C" {
#endif

Reader *NewStringReader(String *ref_s);

#ifdef __cplusplus
}
#endif

#endif // STRINGS_READER_H
