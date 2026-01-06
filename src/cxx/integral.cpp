#include "integral.hpp"

extern "C" {
int8_t MinimumI8(const int8_t *array, size_t size) {
    return Minimum<int8_t>({array, size});
}

int16_t MinimumI16(const int16_t *array, size_t size) {
    return Minimum<int16_t>({array, size});
}

int32_t MinimumI32(const int32_t *array, size_t size) {
    return Minimum<int32_t>({array, size});
}
int64_t MinimumI64(const int64_t *array, size_t size) {
    return Minimum<int64_t>({array, size});
}

uint8_t MinimumU8(const uint8_t *array, size_t size) {
    return Minimum<uint8_t>({array, size});
}

uint16_t MinimumU16(const uint16_t *array, size_t size) {
    return Minimum<uint16_t>({array, size});
}

uint32_t MinimumU32(const uint32_t *array, size_t size) {
    return Minimum<uint32_t>({array, size});
}
uint64_t MinimumU64(const uint64_t *array, size_t size) {
    return Minimum<uint64_t>({array, size});
}
}
