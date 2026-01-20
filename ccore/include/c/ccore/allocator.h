#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Allocator {
    size_t value_type_size;
    void *(*allocate)(size_t size);
    void (*deallocate)(void *ptr);
} Allocator;

Allocator GeneralAllocator(size_t size);

#ifdef __cplusplus
}
#endif

#endif // ALLOCATOR_H
