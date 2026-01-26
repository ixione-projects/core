#include <gtest/gtest.h>

#include "array_list.h"
#include "stack.h"

TEST(StackTest, TestPushBack) {
	auto backend = NewArrayList({.size = sizeof(char *), .align = alignof(char *)}, &GeneralAllocator);

	auto $1 = NewStackFromArrayList(backend);
	const char *name = "Alice";
	StackPushBack($1, &name);
	name = "Bob";
	StackPushBack($1, &name);
	ASSERT_EQ(2, StackSize($1));
	DeleteStack($1);

	ArrayListClear(backend);
	auto $2 = NewStackFromArrayList(backend);
	name = "Charlie";
	StackPushBack($2, &name);
	name = "Diana";
	StackPushBack($2, &name);
	name = "Ethan";
	StackPushBack($2, &name);
	ASSERT_EQ(3, StackSize($2));
	DeleteStack($2);

	ArrayListClear(backend);
	auto $3 = NewStackFromArrayList(backend);
	ASSERT_EQ(true, StackIsEmpty($3));
	DeleteStack($3);

	ArrayListClear(backend);
	auto $4 = NewStackFromArrayList(backend);
	name = "Frank";
	StackPushBack($4, &name);
	name = "Grace";
	StackPushBack($4, &name);
	name = "Henry";
	StackPushBack($4, &name);
	name = "Ivy";
	StackPushBack($4, &name);
	ASSERT_EQ(4, StackSize($4));
	DeleteStack($4);

	ArrayListClear(backend);
	auto $5 = NewStackFromArrayList(backend);
	name = "Jack";
	StackPushBack($5, &name);
	name = "Kelly";
	StackPushBack($5, &name);
	ASSERT_EQ(2, StackSize($5));
	DeleteStack($5);

	DeleteArrayList(backend);
}
TEST(StackTest, TestBackAndPopBack) {
	auto backend = NewArrayList({.size = sizeof(char *), .align = alignof(char *)}, &GeneralAllocator);

	auto $1 = NewStackFromArrayList(backend);
	char **rvalue = new (char *);
	const char *name = "Alice";
	StackPushBack($1, &name);
	name = "Bob";
	StackPushBack($1, &name);
	ASSERT_EQ(2, StackSize($1));
	ASSERT_STREQ("Bob", *(char **)StackBack($1));
	ASSERT_STREQ("Bob", *(char **)StackPopBack($1, rvalue));
	ASSERT_EQ(1, StackSize($1));
	DeleteStack($1);

	ArrayListClear(backend);
	auto $2 = NewStackFromArrayList(backend);
	ASSERT_EQ(nullptr, StackBack($2));
	DeleteStack($2);

	ArrayListClear(backend);
	auto $3 = NewStackFromArrayList(backend);
	name = "Charlie";
	StackPushBack($3, &name);
	name = "David";
	StackPushBack($3, &name);
	ASSERT_STREQ("David", *(char **)StackPopBack($3, rvalue));
	ASSERT_STREQ("Charlie", *(char **)StackPopBack($3, rvalue));
	ASSERT_EQ(nullptr, StackPopBack($3, rvalue));
	DeleteStack($3);

	ArrayListClear(backend);
	auto $4 = NewStackFromArrayList(backend);
	name = "Eve";
	StackPushBack($4, &name);
	ASSERT_STREQ("Eve", *(char **)StackBack($4));
	name = "Frank";
	StackPushBack($4, &name);
	ASSERT_EQ(2, StackSize($4));
	ASSERT_STREQ("Frank", *(char **)StackPopBack($4, rvalue));
	ASSERT_STREQ("Eve", *(char **)StackPopBack($4, rvalue));
	ASSERT_EQ(nullptr, StackPopBack($4, rvalue));
	DeleteStack($4);

	delete (rvalue);
	DeleteArrayList(backend);
}
