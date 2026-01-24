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
		size_t cursor = 0;
		Inner *enclosing;

		char *last = nullptr;

		Iter(Inner *l) : enclosing(l) {}

		bool has_next() const {
			return cursor != length(enclosing);
		}

		void *next() {
			assert(cursor < length(enclosing), "`curosr = %d`", cursor);

			return last = element(enclosing, cursor++);
		}

		void erase() {
			assert(cursor >= 0, "`cursor = %d`", cursor);
			assert(last != nullptr, "");

			enclosing->resize(enclosing->size() - enclosing->value_type_size());
			cursor -= 1;
			auto n = (length(enclosing) - cursor) * enclosing->value_type_size();
			std::memmove(last, last + enclosing->value_type_size(), n);
			last = nullptr;
		}
	};
};

extern "C" {
static bool array_list_has_next(const void *it) {
	return static_cast<const Inner::Iter *>(it)->has_next();
}

static void *array_list_next(void *it) {
	return static_cast<Inner::Iter *>(it)->next();
}

static void array_list_erase(void *it) {
	static_cast<Inner::Iter *>(it)->erase();
}

static IteratorVTable array_list_iterator_vtable = {
    .has_next = array_list_has_next,
    .next = array_list_next,
    .erase = array_list_erase,
};

ArrayList *NewArrayList(Allocator allocator) {
	return reinterpret_cast<ArrayList *>(new Inner(ContainerAllocator<char>{allocator}));
}

void DeleteArrayList(ArrayList *list) {
	delete reinterpret_cast<Inner *>(list);
}

Allocator ArrayListGetAllocator(const ArrayList *list) {
	return reinterpret_cast<const Inner *>(list)->get_allocator().inner;
}

void *ArrayListAt(const ArrayList *list, size_t i) {
	auto l = reinterpret_cast<const Inner *>(list);
	assert(i >= 0 && i < length(l), "`i = %d`", i);

	return const_cast<char *>(element(l, i));
}

void ArrayListInsert(ArrayList *list, size_t i, void *value) {
	auto l = reinterpret_cast<Inner *>(list);
	assert(i >= 0 && i <= length(l), "`i = %d`", i);

	if (i == l->capacity() / l->value_type_size()) {
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
	auto n = (length(l) - i) * l->value_type_size();
	std::memmove(value, value + l->value_type_size(), n);

	return result;
}

size_t ArrayListSize(const ArrayList *list) {
	return length(reinterpret_cast<const Inner *>(list));
}

bool ArrayListIsEmpty(const ArrayList *list) {
	return length(reinterpret_cast<const Inner *>(list)) == 0;
}

void ArrayListReserve(ArrayList *list, size_t capacity) {
	auto l = reinterpret_cast<Inner *>(list);
	l->reserve(capacity * l->value_type_size());
}

void ArrayListClear(ArrayList *list) {
	reinterpret_cast<Inner *>(list)->clear();
}

Iterator NewArrayListIterator(ArrayList *list) {
	return (Iterator){
	    .handle = new Inner::Iter{reinterpret_cast<Inner *>(list)},
	    .vtable = &array_list_iterator_vtable,
	};
}
}
