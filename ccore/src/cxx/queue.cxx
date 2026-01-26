#include <queue>

#include "queue.h"

struct QueueContainer {
	using value_type = void *;
	using reference = value_type;
	using const_reference = const value_type;
	using size_type = size_t;

	void *backend;
	QueueVTable *vtable;

	QueueContainer(void *backend, QueueVTable *vtable) : backend(backend), vtable(vtable) {}

	const_reference front() const {
		return vtable->front(backend);
	}

	const_reference back() const {
		return vtable->back(backend);
	}

	void push_back(const value_type &value) {
		vtable->push_front(backend, value);
	}

	reference pop_front(value_type &rvalue) {
		return vtable->pop_back(backend, rvalue);
	}

	size_type size() const {
		return vtable->size(backend);
	}
	bool empty() const { return vtable->is_empty(backend); }
};

struct QueueInner : std::queue<void *, QueueContainer> {
	QueueInner(void *backend, QueueVTable *vtable) : std::queue<void *, QueueContainer>(QueueContainer{backend, vtable}) {}

	reference pop(value_type &rvalue) {
		return c.pop_front(rvalue);
	}
};

extern "C" {
Queue *NewQueue(void *backend, QueueVTable *vtable) {
	return reinterpret_cast<Queue *>(new QueueInner{backend, vtable});
}

void DeleteQueue(Queue *stack) {
	delete reinterpret_cast<QueueInner *>(stack);
}

void *QueueFront(const Queue *stack) {
	auto s = reinterpret_cast<const QueueInner *>(stack);
	if (s->empty()) {
		return nullptr;
	}
	return s->front();
}

void *QueueBack(const Queue *stack) {
	auto s = reinterpret_cast<const QueueInner *>(stack);
	if (s->empty()) {
		return nullptr;
	}
	return s->back();
}

void QueuePushFront(Queue *stack, void *value) {
	reinterpret_cast<QueueInner *>(stack)->push(value);
}

void *QueuePopBack(Queue *stack, void *rvalue) {
	auto s = reinterpret_cast<QueueInner *>(stack);
	if (s->empty()) {
		return nullptr;
	}
	return s->pop(rvalue);
}

size_t QueueSize(const Queue *stack) {
	return reinterpret_cast<const QueueInner *>(stack)->size();
}

bool QueueIsEmpty(const Queue *stack) {
	return reinterpret_cast<const QueueInner *>(stack)->empty();
}
}
