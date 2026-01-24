#include <cstdio>
#include <gtest/gtest.h>

#include "array_list.h"

TEST(ArrayListTest, Iterator) {
	auto list = NewArrayList(GeneralAllocator(sizeof(int)));
	for (int i{}; i < 10; ++i) {
		ArrayListPushBack(list, &i);
	}

	auto iter = NewArrayListIterator(list);
	int i{};
	while (IteratorHasNext(iter)) {
		ASSERT_EQ(i++, *(int *)IteratorNext(iter));
	}
}
