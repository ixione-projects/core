#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

#include "allocator.h"
#include "core.h"

static void *general_allocate(size_t size, size_t _) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		panic("malloc: unable to allocate %zu bytes: %m", size);
	}
	return ptr;
}

static void general_deallocate(void *ptr) {
	free(ptr);
}

Allocator GeneralAllocator = {
    .allocate = general_allocate,
    .deallocate = general_deallocate,
};

static void *aligned_allocate(size_t size, size_t align) {
	void *ptr = malloc(size + align + sizeof(void *));
	if (ptr == NULL) {
		panic("malloc: unable to allocate %zu bytes: %m", size);
	}

	uintptr_t raw = (uintptr_t)ptr + sizeof(void *);
	uintptr_t aligned = (raw + (align - 1)) & ~(align - 1);

	((void **)aligned)[-1] = ptr;
	return (void *)aligned;
}

static void aligned_deallocate(void *ptr) {
	free(((void **)ptr)[-1]);
}

Allocator AlignedAllocator = {
    .allocate = aligned_allocate,
    .deallocate = aligned_deallocate,
};
