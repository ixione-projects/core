#include <stack>

#include "stack.h"

struct StackContainer {
	using value_type = void *;
	using reference = value_type;
	using const_reference = const value_type;
	using size_type = size_t;

	void *backend;
	StackVTable *vtable;

	StackContainer(void *backend, StackVTable *vtable) : backend(backend), vtable(vtable) {}

	const_reference back() const {
		return vtable->back(backend);
	}

	void push_back(const value_type &value) {
		vtable->push_back(backend, value);
	}

	reference pop_back(value_type &rvalue) {
		return vtable->pop_back(backend, rvalue);
	}

	size_type size() const {
		return vtable->size(backend);
	}
	bool empty() const { return vtable->is_empty(backend); }
};

struct StackInner : std::stack<void *, StackContainer> {
	StackInner(void *backend, StackVTable *vtable) : std::stack<void *, StackContainer>(StackContainer{backend, vtable}) {}

	reference pop(value_type &rvalue) {
		return c.pop_back(rvalue);
	}
};

extern "C" {
Stack *NewStack(void *backend, StackVTable *vtable) {
	return reinterpret_cast<Stack *>(new StackInner{backend, vtable});
}

void DeleteStack(Stack *stack) {
	delete reinterpret_cast<StackInner *>(stack);
}

void *StackBack(const Stack *stack) {
	auto s = reinterpret_cast<const StackInner *>(stack);
	if (s->empty()) {
		return nullptr;
	}
	return s->top();
}

void StackPushBack(Stack *stack, void *value) {
	reinterpret_cast<StackInner *>(stack)->push(value);
}

void *StackPopBack(Stack *stack, void *rvalue) {
	auto s = reinterpret_cast<StackInner *>(stack);
	if (s->empty()) {
		return nullptr;
	}
	return s->pop(rvalue);
}

size_t StackSize(const Stack *stack) {
	return reinterpret_cast<const StackInner *>(stack)->size();
}

bool StackIsEmpty(const Stack *stack) {
	return reinterpret_cast<const StackInner *>(stack)->empty();
}
}
