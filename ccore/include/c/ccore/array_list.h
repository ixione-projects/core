#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ArrayList ArrayList;

ArrayList *NewArrayList(Allocator allocator);
void DeleteArrayList(ArrayList *list);

const void *ArrayListAt(const ArrayList *list, size_t i);
void ArrayListInsert(ArrayList *list, size_t i, void *value);
void *ArrayListRemove(ArrayList *list, size_t i);

size_t ArrayListSize(const ArrayList *list);
bool ArrayListIsEmpty(const ArrayList *list);

#define ArrayListBack(list) ArrayListAt(list, ArrayListSize(list) - 1)
#define ArrayListPushBack(list, value) ArrayListInsert(list, ArrayListSize(list), value)
#define ArrayListPopBack(list) ArrayListRemove(list, ArrayListSize(list) - 1)

#ifdef __cplusplus
}
#endif

#endif // ARRAY_LIST_H
