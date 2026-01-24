#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Allocator {
	size_t value_type_size;
	size_t value_type_align;
	void *(*allocate)(size_t size, size_t align);
	void (*deallocate)(void *ptr);
} Allocator;

Allocator GeneralAllocator(size_t size);
Allocator AlignedAllocator(size_t size, size_t align);

#ifdef __cplusplus
}
#endif

#endif // ALLOCATOR_H
