#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	size_t size;
	size_t align;
} Layout;

typedef struct Allocator {
	void *(*allocate)(size_t size, size_t align);
	void (*deallocate)(void *ptr);
} Allocator;

extern Allocator GeneralAllocator;
extern Allocator AlignedAllocator;

#ifdef __cplusplus
}
#endif

#endif // ALLOCATOR_H
