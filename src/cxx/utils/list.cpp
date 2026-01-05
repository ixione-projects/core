#include "list.hpp"

extern "C" {
int32_t MinimumI32(const int32_t *array, size_t size) {
    return Minimum<int32_t>({array, size});
}
}
