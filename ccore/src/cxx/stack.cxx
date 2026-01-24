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
		return this->vtable->back(this->backend);
	}

	const_reference back() const {
		return this->vtable->back(this->backend);
	}

	void push_back(const value_type &value) {
		this->vtable->push_back(this->backend, value);
	}

	void pop_back() {
		this->vtable->pop_back(this->backend);
	}

	size_t size() const { return this->vtable->size(this->backend); }
	bool empty() const { return this->vtable->is_empty(this->backend); }
};

struct Inner : std::stack<void *, StackContainer> {
	Inner(void *backend, StackVTable *vtable) : std::stack<void *, StackContainer>(StackContainer{backend, vtable}) {}
};

// array_list

extern "C" {
static void *array_list_back(const void *list) {
	const ArrayList *l = (const ArrayList *)list;
	return ArrayListBack(l);
}

static void array_list_push_back(void *list, void *value) {
	ArrayList *l = (ArrayList *)list;
	return ArrayListPushBack(l, value);
}

static void *array_list_pop_back(void *list) {
	ArrayList *l = (ArrayList *)list;
	return ArrayListPopBack(l);
}

static size_t array_list_size(const void *list) {
	return ArrayListSize((const ArrayList *)list);
}

static bool array_list_is_empty(const void *list) {
	return ArrayListIsEmpty((const ArrayList *)list);
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
	auto result = s->top();
	s->pop();
	return result;
}

size_t StackSize(const Stack *stack) {
	return reinterpret_cast<const Inner *>(stack)->size();
}

bool StackIsEmpty(const Stack *stack) {
	return reinterpret_cast<const Inner *>(stack)->empty();
}
}
