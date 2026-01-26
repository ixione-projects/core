#ifndef ITERATOR_INTERNAL_H
#define ITERATOR_INTERNAL_H

struct IteratorContainer {
	virtual ~IteratorContainer() = default;

	virtual bool has_next() const = 0;
	virtual bool has_previous() const = 0;
	virtual void *next() = 0;
	virtual void *previous() = 0;
	virtual void erase() = 0;
};

#endif // ITERATOR_INTERNAL_H
