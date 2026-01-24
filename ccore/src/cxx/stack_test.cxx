#include <gtest/gtest.h>

#include "stack.h"

#define MakeStack(size) NewStackFromArrayList(NewArrayList(GeneralAllocator(size)))

TEST(StackTest, Implmentation) {
	auto $1 = MakeStack(sizeof(char *));
	const char *name = "Alice";
	StackPushBack($1, &name);
	name = "Bob";
	StackPushBack($1, &name);
	ASSERT_EQ(2, StackSize($1));
	ASSERT_STREQ("Bob", *(char **)StackBack($1));
	ASSERT_STREQ("Bob", *(char **)StackPopBack($1));
	ASSERT_EQ(1, StackSize($1));

	auto $2 = MakeStack(sizeof(char *));
	ASSERT_EQ(nullptr, StackBack($2));

	auto $3 = MakeStack(sizeof(char *));
	name = "Charlie";
	StackPushBack($3, &name);
	name = "David";
	StackPushBack($3, &name);
	ASSERT_STREQ("David", *(char **)StackPopBack($3));
	ASSERT_STREQ("Charlie", *(char **)StackPopBack($3));
	ASSERT_EQ(nullptr, StackPopBack($3));

	auto $4 = MakeStack(sizeof(char *));
	name = "Eve";
	StackPushBack($4, &name);
	ASSERT_STREQ("Eve", *(char **)StackBack($4));
	name = "Frank";
	StackPushBack($4, &name);
	ASSERT_EQ(2, StackSize($4));
	ASSERT_STREQ("Frank", *(char **)StackPopBack($4));
	ASSERT_STREQ("Eve", *(char **)StackPopBack($4));
	ASSERT_EQ(nullptr, StackPopBack($4));
}
