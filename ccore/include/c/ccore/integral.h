#ifndef INTEGERAL_H
#define INTEGERAL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int8_t MinimumI8(const int8_t *array, size_t size);
int16_t MinimumI16(const int16_t *array, size_t size);
int32_t MinimumI32(const int32_t *array, size_t size);
int64_t MinimumI64(const int64_t *array, size_t size);

uint8_t MinimumU8(const uint8_t *array, size_t size);
uint16_t MinimumU16(const uint16_t *array, size_t size);
uint32_t MinimumU32(const uint32_t *array, size_t size);
uint64_t MinimumU64(const uint64_t *array, size_t size);

#ifdef __cplusplus
}
#endif

#endif // INTEGERAL_H
