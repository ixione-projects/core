#include <cstring>
#include <vector>

#include "array_list.h"
#include "core.h"

#define length(list) ((list)->size() / (list)->value_type_size())
#define element(list, i) ((list)->data() + (i) * (list)->value_type_size())

template <typename T>
    requires(sizeof(T) == 1)
struct ArrayListAllocator {
	using value_type = T;

	Layout layout;
	Allocator *inner;

	explicit ArrayListAllocator(Layout layout, Allocator *inner) : layout(layout), inner(inner) {}
	template <typename U>
	ArrayListAllocator(const ArrayListAllocator<U> &other) : layout(other.layout), inner(other.inner) {}

	ArrayListAllocator() = delete;

	T *allocate(std::size_t n) {
		return (T *)inner->allocate(n, layout.align);
	}

	void deallocate(T *ptr, std::size_t _) {
		inner->deallocate(ptr);
	}
};

struct ArrayListInner : std::vector<std::byte, ArrayListAllocator<std::byte>> {
	ArrayListInner(ArrayListAllocator<std::byte> allocator) : std::vector<std::byte, ArrayListAllocator<std::byte>>(allocator) {}

	size_t value_type_size() const {
		return get_allocator().layout.size;
	}

	size_t value_type_align() const {
		return get_allocator().layout.align;
	}

	struct Iter {
		ArrayListInner *enclosing;
		size_t cursor = 0;

		Iter(ArrayListInner *enclosing) : enclosing(enclosing) {}

		bool has_next() const {
			return cursor != length(enclosing);
		}

		bool has_previous() const {
			return cursor != 0;
		}

		void *next() {
			assert(cursor < length(enclosing), "`curosr = %d`", cursor);

			return element(enclosing, cursor++);
		}

		void *previous() {
			assert(cursor >= 0, "`curosr = %d`", cursor);

			return element(enclosing, --cursor);
		}

		void erase() {
			assert(cursor >= 0, "`curosr = %d`", cursor);

			auto element = element(enclosing, --cursor);
			auto s = enclosing->value_type_size();
			auto n = (length(enclosing) - cursor - 1) * s;
			std::memmove(element, element + s, n);
			enclosing->resize(enclosing->size() - s);
		}
	};
};

extern "C" {
static bool array_list_has_next(const void *it) {
	return static_cast<const ArrayListInner::Iter *>(it)->has_next();
}

static bool array_list_has_previous(const void *it) {
	return static_cast<const ArrayListInner::Iter *>(it)->has_previous();
}

static void *array_list_next(void *it) {
	return static_cast<ArrayListInner::Iter *>(it)->next();
}

static void *array_list_previous(void *it) {
	return static_cast<ArrayListInner::Iter *>(it)->previous();
}

static void array_list_erase(void *it) {
	static_cast<ArrayListInner::Iter *>(it)->erase();
}

static IteratorVTable array_list_iterator_vtable = {
    .has_next = array_list_has_next,
    .has_previous = array_list_has_previous,
    .next = array_list_next,
    .previous = array_list_previous,
    .erase = array_list_erase,
};

ArrayList *NewArrayList(Layout layout, Allocator *allocator) {
	return reinterpret_cast<ArrayList *>(new ArrayListInner{ArrayListAllocator<std::byte>{layout, allocator}});
}

void DeleteArrayList(ArrayList *list) {
	delete reinterpret_cast<ArrayListInner *>(list);
}

Allocator *ArrayListGetAllocator(const ArrayList *list) {
	return reinterpret_cast<const ArrayListInner *>(list)->get_allocator().inner;
}

void *ArrayListAt(const ArrayList *list, size_t i) {
	auto l = reinterpret_cast<const ArrayListInner *>(list);
	assert(i >= 0 && i < length(l), "`i = %d`", i);

	return const_cast<std::byte *>(element(l, i));
}

void ArrayListInsert(ArrayList *list, size_t i, void *value) {
	auto l = reinterpret_cast<ArrayListInner *>(list);
	assert(i >= 0 && i <= length(l), "`i = %d`", i);

	auto s = l->value_type_size();
	if (i == l->capacity() / s) {
		l->reserve((i ? (i >> 1) + i : 10) * s);
	}

	auto element = element(l, i);
	auto n = (length(l) - i) * s;
	l->resize(l->size() + s);
	std::memmove(element + s, element, n);
	std::memcpy(element, value, s);
}

void *ArrayListRemove(ArrayList *list, size_t i, void *rvalue) {
	auto l = reinterpret_cast<ArrayListInner *>(list);
	assert(i >= 0 && i < length(l), "`i = %d`", i);

	auto s = l->value_type_size();
	auto value = element(l, i);
	std::memcpy(rvalue, value, s);

	auto n = (length(l) - i - 1) * s;
	std::memmove(value, value + s, n);
	l->resize(l->size() - s);

	return rvalue;
}

size_t ArrayListSize(const ArrayList *list) {
	return length(reinterpret_cast<const ArrayListInner *>(list));
}

bool ArrayListIsEmpty(const ArrayList *list) {
	return length(reinterpret_cast<const ArrayListInner *>(list)) == 0;
}

void ArrayListReserve(ArrayList *list, size_t capacity) {
	auto l = reinterpret_cast<ArrayListInner *>(list);
	l->reserve(capacity * l->value_type_size());
}

void ArrayListClear(ArrayList *list) {
	reinterpret_cast<ArrayListInner *>(list)->clear();
}

Iterator *NewArrayListIterator(ArrayList *list) {
	return new Iterator{
	    .handle = new ArrayListInner::Iter{reinterpret_cast<ArrayListInner *>(list)},
	    .vtable = &array_list_iterator_vtable,
	};
}

void DeleteArrayListIterator(Iterator *it) {
	delete static_cast<ArrayListInner::Iter *>(it->handle);
}
}
