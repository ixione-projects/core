#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	bool (*has_next)(const void *it);
	void *(*next)(void *it);
} IteratorVTable;

typedef struct {
	void *handle;
	IteratorVTable *vtable;
} Iterator;

inline bool IteratorHasNext(const Iterator it) {
	return it.vtable->has_next(it.handle);
}

inline void *IteratorNext(Iterator it) {
	return it.vtable->next(it.handle);
}

#ifdef __cplusplus
}
#endif

#endif // ITERATOR_H
