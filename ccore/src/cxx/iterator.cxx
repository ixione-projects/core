#include "iterator.h"
#include "iterator_internal.hxx"

extern "C" {
void DeleteIterator(Iterator *stack) {
	delete reinterpret_cast<IteratorContainer *>(stack);
}

bool IteratorHasNext(const Iterator *it) {
	return reinterpret_cast<const IteratorContainer *>(it)->has_next();
}

bool IteratorHasPrevious(const Iterator *it) {
	return reinterpret_cast<const IteratorContainer *>(it)->has_previous();
}

void *IteratorNext(Iterator *it) {
	return reinterpret_cast<IteratorContainer *>(it)->next();
}

void *IteratorPrevious(Iterator *it) {
	return reinterpret_cast<IteratorContainer *>(it)->previous();
}

void IteratorErase(Iterator *it) {
	reinterpret_cast<IteratorContainer *>(it)->erase();
}
}
