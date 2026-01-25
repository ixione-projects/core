#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "allocator.h"
#include "iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ArrayList ArrayList;

ArrayList *NewArrayList(Layout layout, Allocator *allocator);
void DeleteArrayList(ArrayList *list);

Allocator *ArrayListGetAllocator(const ArrayList *list);

void *ArrayListAt(const ArrayList *list, size_t i);
void ArrayListInsert(ArrayList *list, size_t i, void *value);
void *ArrayListRemove(ArrayList *list, size_t i, void *rvalue);

size_t ArrayListSize(const ArrayList *list);
bool ArrayListIsEmpty(const ArrayList *list);
void ArrayListReserve(ArrayList *list, size_t capacity);
void ArrayListClear(ArrayList *list);

#define ArrayListFront(list) ArrayListAt(list, 0)
#define ArrayListBack(list) ArrayListAt(list, ArrayListSize(list) - 1)
#define ArrayListPushFront(list, value) ArrayListInsert(list, 0, value)
#define ArrayListPushBack(list, value) ArrayListInsert(list, ArrayListSize(list), value)
#define ArrayListPopFront(list, rvalue) ArrayListRemove(list, 0, rvalue)
#define ArrayListPopBack(list, rvalue) ArrayListRemove(list, ArrayListSize(list) - 1, rvalue)

Iterator NewArrayListIterator(ArrayList *list);

#ifdef __cplusplus
}
#endif

#endif // ARRAY_LIST_H
