#ifndef CONTAINER_ALLOCATOR
#define CONTAINER_ALLOCATOR

#include <cstddef>

#include "allocator.h"

template <typename T>
struct container_allocator {
    using value_type = T;

    Allocator inner;

    explicit container_allocator(Allocator inner) : inner(inner) {}
    template <typename U>
    container_allocator(const container_allocator<U> &other) : inner(other.inner) {}

    container_allocator() = delete;

    T *allocate(std::size_t size) {
        return (T *)this->inner.allocate(size * inner.value_type_size);
    }

    void deallocate(T *ptr, std::size_t _) {
        this->inner.deallocate(ptr);
    }
};

#endif // CONTAINER_ALLOCATOR
