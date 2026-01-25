#include <gtest/gtest.h>

#include "iterator.h"
#include "linked_list.h"

TEST(LinkedListTest, TestInsert) {
	auto list = NewLinkedList({.size = sizeof(char *), .align = alignof(char *)}, &GeneralAllocator);

	const char *name = "Llewelyn Moss";
	LinkedListInsert(list, LinkedListSize(list), &name);
	name = "Anton Chigurh";
	LinkedListInsert(list, LinkedListSize(list), &name);
	name = "Carson Wells";
	LinkedListInsert(list, LinkedListSize(list), &name);
	name = "Ed Tom Bell";
	LinkedListInsert(list, LinkedListSize(list), &name);
	name = "Carla Jean Moss";
	LinkedListInsert(list, LinkedListSize(list), &name);
	name = "Wendell";
	LinkedListInsert(list, LinkedListSize(list), &name);
	auto it = NewLinkedListIterator(list);
	while (IteratorHasNext(it)) {
		printf("%s -> ", *(char **)IteratorNext(it));
	}
	printf("\n");

	DeleteLinkedList(list);
}
