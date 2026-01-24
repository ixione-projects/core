#ifndef CONTAINER_ALLOCATOR
#define CONTAINER_ALLOCATOR

#include <cstddef>

#include "allocator.h"

template <typename T>
struct ContainerAllocator {
	using value_type = T;

	Allocator inner;

	explicit ContainerAllocator(Allocator inner) : inner(inner) {}
	template <typename U>
	ContainerAllocator(const ContainerAllocator<U> &other) : inner(other.inner) {}

	ContainerAllocator() = delete;

	T *allocate(std::size_t size) {
		return (T *)this->inner.allocate(size * inner.value_type_size, inner.value_type_align);
	}

	void deallocate(T *ptr, std::size_t _) {
		this->inner.deallocate(ptr);
	}
};

#endif // CONTAINER_ALLOCATOR
