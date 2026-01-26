#include <cstring>
#include <list>
#include <memory>

#include "core.h"
#include "linked_list.h"

#include "iterator_internal.hxx"

#define length(list) ((list)->size())

struct LinkedListPayload {
	std::byte payload[]; // (FAM)

	struct Delete {
		Allocator *allocator;

		void operator()(LinkedListPayload *node) const {
			allocator->deallocate(node);
		}
	};
};

using LinkedListNode = std::unique_ptr<LinkedListPayload, LinkedListPayload::Delete>;

template <typename T>
struct LinkedListAllocator {
	using value_type = T;

	Layout layout;
	Allocator *inner;

	explicit LinkedListAllocator(Layout layout, Allocator *inner) : layout(layout), inner(inner) {}
	template <typename U>
	LinkedListAllocator(const LinkedListAllocator<U> &other) : layout(other.layout), inner(other.inner) {}

	LinkedListAllocator() = delete;

	T *allocate(std::size_t n) {
		return (T *)inner->allocate(n * sizeof(T), layout.align);
	}

	void deallocate(T *ptr, std::size_t _) {
		inner->deallocate(ptr);
	}
};

struct LinkedListInner : std::list<LinkedListNode, LinkedListAllocator<LinkedListNode>> {
	LinkedListInner(LinkedListAllocator<LinkedListNode> allocator) : std::list<LinkedListNode, LinkedListAllocator<LinkedListNode>>(allocator) {}

	size_t value_type_size() const {
		return get_allocator().layout.size;
	}

	size_t value_type_align() const {
		return get_allocator().layout.align;
	}

	struct Iter : IteratorContainer {
		LinkedListInner *enclosing;
		LinkedListInner::iterator cursor;

		Iter(LinkedListInner *enclosing) : enclosing(enclosing), cursor(enclosing->begin()) {}

		bool has_next() const override {
			return cursor != enclosing->end();
		}

		bool has_previous() const override {
			return cursor != enclosing->begin();
		}

		void *next() override {
			assert(cursor != enclosing->end(), "");

			return &(*cursor++)->payload;
		}

		void *previous() override {
			assert(cursor != enclosing->begin(), "");

			return &(*--cursor)->payload;
		}

		void erase() override {
			assert(cursor != enclosing->begin(), "");

			enclosing->erase(--cursor);
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

inline LinkedListInner::const_iterator element(const LinkedListInner *list, size_t i) {
	auto element = list->begin();
	for (size_t index{}; index < i; ++index) {
		++element;
	}
	return element;
}

static LinkedListPayload *make_payload(LinkedListInner *l, void *value) {
	auto allocator = l->get_allocator();
	auto s = l->value_type_size();
	auto mem = allocator.inner->allocate(sizeof(LinkedListPayload) + s, alignof(LinkedListPayload));
	auto payload = new (mem) LinkedListPayload{};
	std::memcpy(payload->payload, value, s);
	return payload;
}

extern "C" {
LinkedList *NewLinkedList(Layout layout, Allocator *allocator) {
	return reinterpret_cast<LinkedList *>(new LinkedListInner{LinkedListAllocator<LinkedListNode>{layout, allocator}});
}

void DeleteLinkedList(LinkedList *list) {
	delete reinterpret_cast<LinkedListInner *>(list);
}

Allocator *LinkedListGetAllocator(const LinkedList *list) {
	return reinterpret_cast<const LinkedListInner *>(list)->get_allocator().inner;
}

void *LinkedListAt(const LinkedList *list, size_t i) {
	auto l = reinterpret_cast<const LinkedListInner *>(list);
	assert(i >= 0 && i < length(l), "`i = %d`", i);

	return &(*element(l, i))->payload;
}

void LinkedListInsert(LinkedList *list, size_t i, void *value) {
	auto l = reinterpret_cast<LinkedListInner *>(list);
	assert(i >= 0 && i <= length(l), "`i = %d`", i);

	l->insert(element(l, i), LinkedListNode{make_payload(l, value), LinkedListPayload::Delete{l->get_allocator().inner}});
}

void *LinkedListRemove(LinkedList *list, size_t i, void *rvalue) {
	auto l = reinterpret_cast<LinkedListInner *>(list);
	assert(i >= 0 && i < length(l), "`i = %d`", i);

	auto value = element(l, i);
	std::memcpy(rvalue, (*value)->payload, l->value_type_size());
	l->erase(value);

	return rvalue;
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

void *ArrayListFront(LinkedList *list) {
	return &reinterpret_cast<LinkedListInner *>(list)->front()->payload;
}

void *ArrayListBack(LinkedList *list) {
	return &reinterpret_cast<LinkedListInner *>(list)->back()->payload;
}

void ArrayListPushFront(LinkedList *list, void *value) {
	auto l = reinterpret_cast<LinkedListInner *>(list);
	l->push_front(LinkedListNode{make_payload(l, value), LinkedListPayload::Delete{l->get_allocator().inner}});
}

void ArrayListPushBack(LinkedList *list, void *value) {
	auto l = reinterpret_cast<LinkedListInner *>(list);
	l->push_back(LinkedListNode{make_payload(l, value), LinkedListPayload::Delete{l->get_allocator().inner}});
}

void *ArrayListPopFront(LinkedList *list, void *rvalue) {
	auto l = reinterpret_cast<LinkedListInner *>(list);
	std::memcpy(rvalue, l->front()->payload, l->value_type_size());
	l->pop_front();
	return rvalue;
}

void *ArrayListPopBack(LinkedList *list, void *rvalue) {
	auto l = reinterpret_cast<LinkedListInner *>(list);
	std::memcpy(rvalue, l->back()->payload, l->value_type_size());
	l->pop_back();
	return rvalue;
}

Iterator *NewLinkedListIterator(LinkedList *list) {
	return reinterpret_cast<Iterator *>(new LinkedListInner::Iter{reinterpret_cast<LinkedListInner *>(list)});
}
}
