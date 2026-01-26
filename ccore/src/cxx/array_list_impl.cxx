#include "array_list.h"

extern "C" {
static void *stack_back(const void *list) {
	return ArrayListBack(static_cast<const ArrayList *>(list));
}

static void stack_push_back(void *list, void *value) {
	return ArrayListPushBack(static_cast<ArrayList *>(list), value);
}

static void *stack_pop_back(void *list, void *rvalue) {
	return ArrayListPopBack(static_cast<ArrayList *>(list), rvalue);
}

static size_t stack_size(const void *list) {
	return ArrayListSize(static_cast<const ArrayList *>(list));
}

static bool stack_is_empty(const void *list) {
	return ArrayListIsEmpty(static_cast<const ArrayList *>(list));
}

static StackVTable array_list_stack_vtable = {
    .back = stack_back,
    .push_back = stack_push_back,
    .pop_back = stack_pop_back,
    .size = stack_size,
    .is_empty = stack_is_empty,
};

Stack *NewStackFromArrayList(ArrayList *list) {
	return NewStack(list, &array_list_stack_vtable);
}
}
