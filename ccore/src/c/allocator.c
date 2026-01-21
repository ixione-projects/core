#include <stdlib.h>

#include "allocator.h"
#include "core.h"

static void *general_allocate(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        panic("malloc: unable to allocate %zu bytes: %m", size);
    }
    return ptr;
}

static void general_deallocate(void *ptr) {
    free(ptr);
}

Allocator GeneralAllocator(size_t size) {
    return (Allocator){
        .value_type_size = size,
        .allocate = general_allocate,
        .deallocate = general_deallocate,
    };
}
