#include <cstdlib>
#include <cstring>
#include <vector>

#include "array_list.h"
#include "core.h"

#include "container_allocator.hxx"

#define element(list, i) ((list)->data() + (i) * (list)->value_type_size())
#define length(list) ((list)->size() / (list)->value_type_size())

struct Inner : std::vector<char, ContainerAllocator<char>> {
	Inner(ContainerAllocator<char> allocator) : std::vector<char, ContainerAllocator<char>>(allocator) {}

	constexpr size_t value_type_size() const {
		return this->get_allocator().inner.value_type_size;
	}

	struct Iter {
		Inner::iterator begin, end;
		difference_type value_type_size;

		Iter(Inner *l) : begin(l->begin()), end(l->end()), value_type_size((difference_type)l->value_type_size()) {}

		bool has_next() const {
			return begin != end;
		}

		void *next() {
			auto temp = begin;
			begin += value_type_size;
			return &*temp;
		}
	};
};

extern "C" {
static bool array_list_has_next(const void *it) {
	return ((const Inner::Iter *)it)->has_next();
}

static void *array_list_next(void *it) {
	return ((Inner::Iter *)it)->next();
}

static IteratorVTable array_list_iterator_vtable = {
    .has_next = array_list_has_next,
    .next = array_list_next,
};

ArrayList *NewArrayList(Allocator allocator) {
	return reinterpret_cast<ArrayList *>(new Inner(ContainerAllocator<char>{allocator}));
}

void DeleteArrayList(ArrayList *list) {
	delete reinterpret_cast<Inner *>(list);
}

void *ArrayListAt(const ArrayList *list, size_t i) {
	auto l = reinterpret_cast<const Inner *>(list);
	assert(i >= 0 && i < length(l), "`i = %d`", i);

	return const_cast<char *>(element(l, i));
}

void ArrayListInsert(ArrayList *list, size_t i, void *value) {
	auto l = reinterpret_cast<Inner *>(list);
	if (i >= l->capacity() / l->value_type_size()) {
		l->reserve((i ? (i >> 1) + i : 10) * l->value_type_size());
	}
	l->resize(l->size() + l->value_type_size());
	std::memcpy(element(l, i), value, l->value_type_size());
}

void *ArrayListRemove(ArrayList *list, size_t i) {
	auto l = reinterpret_cast<Inner *>(list);
	assert(i >= 0 && i < length(l), "`i = %d`", i);

	auto result = l->get_allocator().allocate(l->value_type_size());
	auto value = element(l, i);
	std::memcpy(result, value, l->value_type_size());

	l->resize(l->size() - l->value_type_size());
	std::memmove(value, value + l->value_type_size(), (length(l) - i) * l->value_type_size());
	return result;
}

size_t ArrayListSize(const ArrayList *list) {
	auto l = reinterpret_cast<const Inner *>(list);
	return length(l);
}

bool ArrayListIsEmpty(const ArrayList *list) {
	auto l = reinterpret_cast<const Inner *>(list);
	return length(l) == 0;
}

Iterator NewArrayListIterator(ArrayList *list) {
	return (Iterator){
	    .handle = new Inner::Iter{reinterpret_cast<Inner *>(list)},
	    .vtable = &array_list_iterator_vtable,
	};
}
}
