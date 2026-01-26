#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Iterator Iterator;

void DeleteIterator(Iterator *stack);

bool IteratorHasNext(const Iterator *it);
bool IteratorHasPrevious(const Iterator *it);
void *IteratorNext(Iterator *it);
void *IteratorPrevious(Iterator *it);
void IteratorErase(Iterator *it);

#ifdef __cplusplus
}
#endif

#endif // ITERATOR_H
