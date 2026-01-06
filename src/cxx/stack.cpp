#include "stack.hpp"

extern "C" {
Stack_t *NewStack() {
    return reinterpret_cast<Stack_t *>(new Stack<StackValue>());
}

Stack_t *NewStackWithCapacity(size_t c) {
    return reinterpret_cast<Stack_t *>(new Stack<StackValue>(c));
}

void DeleteStack(Stack_t *s) {
    delete reinterpret_cast<Stack<StackValue> *>(s);
}

void StackPush(Stack_t *s, StackValue value) {
    reinterpret_cast<Stack<StackValue> *>(s)->Push(value);
}

StackValue StackPop(Stack_t *s) {
    auto maybe = reinterpret_cast<Stack<StackValue> *>(s)->Pop();
    if (maybe.has_value()) {
        return maybe.value();
    } else {
        return NULL_VALUE;
    }
}

StackValue *StackPeek(Stack_t *s) {
    return reinterpret_cast<Stack<StackValue> *>(s)->Peek();
}

size_t StackSize(const Stack_t *s) {
    return reinterpret_cast<const Stack<StackValue> *>(s)->Size();
}

bool StackIsEmpty(const Stack_t *s) {
    return reinterpret_cast<const Stack<StackValue> *>(s)->IsEmpty();
}
}
