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

	reference back() {
		return vtable->back(backend);
	}

	const_reference back() const {
		return vtable->back(backend);
	}

	void push_back(const value_type &value) {
		vtable->push_back(backend, value);
	}

	reference pop_back() {
		return vtable->pop_back(backend);
	}

	size_type size() const {
		return vtable->size(backend);
	}
	bool empty() const { return vtable->is_empty(backend); }
};

struct Inner : std::stack<void *, StackContainer> {
	Inner(void *backend, StackVTable *vtable) : std::stack<void *, StackContainer>(StackContainer{backend, vtable}) {}

	reference pop() {
		return c.pop_back();
	}
};

// array_list

extern "C" {
static void *array_list_back(const void *list) {
	return ArrayListBack(static_cast<const ArrayList *>(list));
}

static void array_list_push_back(void *list, void *value) {
	return ArrayListPushBack(static_cast<ArrayList *>(list), value);
}

static void *array_list_pop_back(void *list) {
	return ArrayListPopBack(static_cast<ArrayList *>(list));
}

static size_t array_list_size(const void *list) {
	return ArrayListSize(static_cast<const ArrayList *>(list));
}

static bool array_list_is_empty(const void *list) {
	return ArrayListIsEmpty(static_cast<const ArrayList *>(list));
}

static StackVTable array_list_stack_vtable = {
    .back = array_list_back,
    .push_back = array_list_push_back,
    .pop_back = array_list_pop_back,
    .size = array_list_size,
    .is_empty = array_list_is_empty,
};

// array_list

Stack *NewStack(void *backend, StackVTable *vtable) {
	return reinterpret_cast<Stack *>(new Inner{backend, vtable});
}

Stack *NewStackFromArrayList(ArrayList *list) {
	return NewStack(list, &array_list_stack_vtable);
}

void DeleteStack(Stack *stack) {
	delete reinterpret_cast<Inner *>(stack);
}

void *StackBack(const Stack *stack) {
	auto s = reinterpret_cast<const Inner *>(stack);
	if (s->empty()) {
		return nullptr;
	}
	return s->top();
}

void StackPushBack(Stack *stack, void *value) {
	reinterpret_cast<Inner *>(stack)->push(value);
}

void *StackPopBack(Stack *stack) {
	auto s = reinterpret_cast<Inner *>(stack);
	if (s->empty()) {
		return nullptr;
	}
	return s->pop();
}

size_t StackSize(const Stack *stack) {
	return reinterpret_cast<const Inner *>(stack)->size();
}

bool StackIsEmpty(const Stack *stack) {
	return reinterpret_cast<const Inner *>(stack)->empty();
}
}
