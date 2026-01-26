#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void *(*back)(const void *stack);
	void (*push_back)(void *stack, void *value);
	void *(*pop_back)(void *stack, void *rvalue);
	size_t (*size)(const void *stack);
	bool (*is_empty)(const void *stack);
} StackVTable;

typedef struct Stack Stack;

Stack *NewStack(void *backend, StackVTable *vtable);
void DeleteStack(Stack *stack);

void *StackBack(const Stack *stack);
void StackPushBack(Stack *stack, void *value);
void *StackPopBack(Stack *stack, void *rvalue);

size_t StackSize(const Stack *stack);
bool StackIsEmpty(const Stack *stack);

#ifdef __cplusplus
}
#endif

#endif // STACK_H
