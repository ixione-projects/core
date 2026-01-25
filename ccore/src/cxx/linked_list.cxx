#include <cstring>
#include <list>
#include <memory>

#include "core.h"
#include "linked_list.h"

#define length(list) ((list)->size())

struct Payload {
	std::size_t size;
	std::byte payload[]; // (FAM)

	struct Delete {
		void operator()(Payload *node) const {
			::operator delete(node);
		}
	};
};

using Node = std::unique_ptr<Payload, Payload::Delete>;

template <typename T>
struct LinkedListAllocator {
	using value_type = T;

	Layout layout;
	Allocator *inner;

	explicit LinkedListAllocator(Layout layout, Allocator *inner) : layout(layout), inner(inner) {}
	template <typename U>
	LinkedListAllocator(const LinkedListAllocator<U> &other) : layout(other.layout), inner(other.inner) {}

	LinkedListAllocator() = delete;

	T *allocate(std::size_t size) {
		return (T *)this->inner->allocate(size, layout.align);
	}

	void deallocate(T *ptr, std::size_t _) {
		this->inner->deallocate(ptr);
	}
};

struct LinkedListInner : std::list<Node, LinkedListAllocator<Node>> {
	LinkedListInner(LinkedListAllocator<Node> allocator) : std::list<Node, LinkedListAllocator<Node>>(allocator) {}

	size_t value_type_size() const {
		return get_allocator().layout.size;
	}

	size_t value_type_align() const {
		return get_allocator().layout.align;
	}

	struct Iter {
		LinkedListInner *enclosing;
		LinkedListInner::iterator cursor;

		Iter(LinkedListInner *enclosing) : enclosing(enclosing), cursor(enclosing->begin()) {}

		bool has_next() const {
			return cursor != enclosing->end();
		}

		bool has_previous() const {
			return cursor != enclosing->begin();
		}

		void *next() {
			assert(cursor != enclosing->end(), "");

			return &(*cursor++)->payload;
		}

		void *previous() {
			assert(cursor != enclosing->begin(), "");

			return &(*--cursor)->payload;
		}

		void erase() {
			assert(cursor != enclosing->begin(), "");

			auto save_cursor = cursor++;
			enclosing->erase(--cursor);
			cursor = save_cursor;
		}
	};
};

inline LinkedListInner::iterator element(LinkedListInner *list, size_t i) {
	auto element = list->begin();
	for (size_t index{}; index < i; ++index) {
		++element;
	}
	return element;
}

extern "C" {
static bool linked_list_has_next(const void *it) {
	return static_cast<const LinkedListInner::Iter *>(it)->has_next();
}

static bool linked_list_has_previous(const void *it) {
	return static_cast<const LinkedListInner::Iter *>(it)->has_previous();
}

static void *linked_list_next(void *it) {
	return static_cast<LinkedListInner::Iter *>(it)->next();
}

static void *linked_list_previous(void *it) {
	return static_cast<LinkedListInner::Iter *>(it)->previous();
}

static void linked_list_erase(void *it) {
	static_cast<LinkedListInner::Iter *>(it)->erase();
}

static IteratorVTable linked_list_iterator_vtable = {
    .has_next = linked_list_has_next,
    .has_previous = linked_list_has_previous,
    .next = linked_list_next,
    .previous = linked_list_previous,
    .erase = linked_list_erase,
};

LinkedList *NewLinkedList(Layout layout, Allocator *allocator) {
	return reinterpret_cast<LinkedList *>(new LinkedListInner{LinkedListAllocator<Node>{layout, allocator}});
}

void DeleteLinkedList(LinkedList *list) {
	delete reinterpret_cast<LinkedListInner *>(list);
}

Allocator *LinkedListGetAllocator(const LinkedList *list) {
	return reinterpret_cast<const LinkedListInner *>(list)->get_allocator().inner;
}

void *LinkedListAt(const LinkedList *list, size_t i) {
}

void LinkedListInsert(LinkedList *list, size_t i, void *value) {
	auto l = reinterpret_cast<LinkedListInner *>(list);
	assert(i >= 0 && i <= length(l), "`i = %d`", i);

	// TODO: move this into the allocator?
	auto mem = ::operator new(sizeof(Payload) + l->value_type_size());
	auto payload = new (mem) Payload{l->value_type_size()};
	std::memcpy(payload->payload, value, l->value_type_size());
	l->insert(element(l, i), Node{payload});
}

void *LinkedListRemove(LinkedList *list, size_t i, void *rvalue) {
}

size_t LinkedListSize(const LinkedList *list) {
	return length(reinterpret_cast<const LinkedListInner *>(list));
}

bool LinkedListIsEmpty(const LinkedList *list) {
	return length(reinterpret_cast<const LinkedListInner *>(list)) == 0;
}

void LinkedListClear(LinkedList *list) {
	reinterpret_cast<LinkedListInner *>(list)->clear();
}

Iterator NewLinkedListIterator(LinkedList *list) {
	return {
	    .handle = new LinkedListInner::Iter{reinterpret_cast<LinkedListInner *>(list)},
	    .vtable = &linked_list_iterator_vtable,
	};
}
}
