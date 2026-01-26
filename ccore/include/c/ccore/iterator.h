#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	bool (*has_next)(const void *it);
	bool (*has_previous)(const void *it);
	void *(*next)(void *it);
	void *(*previous)(void *it);
	void (*erase)(void *it);
} IteratorVTable;

// TODO: make opaque handle
typedef struct {
	void *handle;
	IteratorVTable *vtable;
} Iterator;

inline bool IteratorHasNext(const Iterator *it) {
	return it->vtable->has_next(it->handle);
}

inline bool IteratorHasPrevious(const Iterator *it) {
	return it->vtable->has_previous(it->handle);
}

inline void *IteratorNext(Iterator *it) {
	return it->vtable->next(it->handle);
}

inline void *IteratorPrevious(Iterator *it) {
	return it->vtable->previous(it->handle);
}

inline void IteratorErase(Iterator *it) {
	it->vtable->erase(it->handle);
}

#ifdef __cplusplus
}
#endif

#endif // ITERATOR_H
