#include "linked_list.h"

extern "C" {
static void *stack_back(const void *list) {
	return LinkedListBack(static_cast<const LinkedList *>(list));
}

static void array_list_push_back(void *list, void *value) {
	return LinkedListPushBack(static_cast<LinkedList *>(list), value);
}

static void *array_list_pop_back(void *list, void *rvalue) {
	return LinkedListPopBack(static_cast<LinkedList *>(list), rvalue);
}

static size_t array_list_size(const void *list) {
	return LinkedListSize(static_cast<const LinkedList *>(list));
}

static bool array_list_is_empty(const void *list) {
	return LinkedListIsEmpty(static_cast<const LinkedList *>(list));
}

static StackVTable linked_list_stack_vtable = {
    .back = stack_back,
    .push_back = array_list_push_back,
    .pop_back = array_list_pop_back,
    .size = array_list_size,
    .is_empty = array_list_is_empty,
};

Stack *NewStackFromLinkedList(LinkedList *list) {
	return NewStack(list, &linked_list_stack_vtable);
}
}
