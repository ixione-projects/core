#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "allocator.h"
#include "iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ArrayList ArrayList;

ArrayList *NewArrayList(Allocator allocator);
void DeleteArrayList(ArrayList *list);

void *ArrayListAt(const ArrayList *list, size_t i);
void ArrayListInsert(ArrayList *list, size_t i, void *value);
void *ArrayListRemove(ArrayList *list, size_t i);

size_t ArrayListSize(const ArrayList *list);
bool ArrayListIsEmpty(const ArrayList *list);

#define ArrayListFront(list) ArrayListAt(list, 0)
#define ArrayListBack(list) ArrayListAt(list, ArrayListSize(list) - 1)
#define ArrayListPushFront(list, value) ArrayListInsert(list, 0, value)
#define ArrayListPushBack(list, value) ArrayListInsert(list, ArrayListSize(list), value)
#define ArrayListPopFront(list) ArrayListRemove(list, 0)
#define ArrayListPopBack(list) ArrayListRemove(list, ArrayListSize(list) - 1)

Iterator NewArrayListIterator(ArrayList *list);

#ifdef __cplusplus
}
#endif

#endif // ARRAY_LIST_H
