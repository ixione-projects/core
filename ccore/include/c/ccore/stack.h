#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

#include "array_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Stack Stack;
typedef struct {
    const void *(*back)(const void *stack);
    void (*push_back)(void *stack, void *value);
    void *(*pop_back)(void *stack);
    size_t (*size)(const void *stack);
    bool (*is_empty)(const void *stack);

} StackVTable;

Stack *NewStack(void *backend, StackVTable vtable);
void DeleteStack(Stack *stack);

const void *StackBack(const Stack *stack);
void StackPushBack(Stack *stack, void *value);
void *StackPopBack(Stack *stack);

size_t StackSize(const Stack *stack);
bool StackIsEmpty(const Stack *stack);

Stack *NewStackFromArrayList(ArrayList *list);

#ifdef __cplusplus
}
#endif

#endif // STACK_H
