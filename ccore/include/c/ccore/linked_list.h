#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "allocator.h"
#include "iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LinkedList LinkedList;

LinkedList *NewLinkedList(Layout layout, Allocator *allocator);
void DeleteLinkedList(LinkedList *list);

Allocator *LinkedListGetAllocator(const LinkedList *list);

void *LinkedListAt(const LinkedList *list, size_t i);
void LinkedListInsert(LinkedList *list, size_t i, void *value);
void *LinkedListRemove(LinkedList *list, size_t i, void *rvalue);

size_t LinkedListSize(const LinkedList *list);
bool LinkedListIsEmpty(const LinkedList *list);
void LinkedListClear(LinkedList *list);

Iterator NewLinkedListIterator(LinkedList *list);

#ifdef __cplusplus
}
#endif

#endif // ARRAY_LIST_H
