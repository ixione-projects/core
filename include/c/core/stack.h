#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Stack_t Stack_t;

typedef enum { StackI32, StackPtr } StackType;

typedef struct {
    StackType type;
    union {
        int32_t i32;
        void *ptr;
    } as;
} StackValue;

#define NULL_VALUE ((StackValue){StackPtr, {.ptr = NULL}})

#define IsI32(value) ((value).type == StackI32)
#define IsPtr(value) ((value).type == StackPtr)

#define AsI32(value) ((value).as.i32)
#define AsPtr(value) ((value).as.ptr)

Stack_t *NewStack();
Stack_t *NewStackWithCapacity(size_t c);
void DeleteStack(Stack_t *s);

void StackPush(Stack_t *s, StackValue value);
StackValue StackPop(Stack_t *s);
StackValue *StackPeek(Stack_t *s);

size_t StackSize(const Stack_t *s);
bool StackIsEmpty(const Stack_t *s);

#ifdef __cplusplus
}
#endif

#endif // STACK_H
