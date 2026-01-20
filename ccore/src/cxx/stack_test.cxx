#include <gtest/gtest.h>

#include "stack.h"

#define MakeStack(size) NewStackFromArrayList(NewArrayList(GeneralAllocator(size)))

TEST(Stack, Push) {
    auto $1 = MakeStack(sizeof(char *));
    const char *name = "Alice";
    StackPushBack($1, &name);
    name = "Bob";
    StackPushBack($1, &name);
    ASSERT_EQ(2, StackSize($1));

    auto $2 = MakeStack(sizeof(char *));
    name = "Charlie";
    StackPushBack($2, &name);
    name = "Diana";
    StackPushBack($2, &name);
    ASSERT_STREQ("Diana", *(const char **)StackPopBack($2));
    name = "Ethan";
    StackPushBack($2, &name);
    ASSERT_EQ(2, StackSize($2));

    auto $3 = MakeStack(sizeof(char *));
    ASSERT_EQ(0, StackSize($3));

    auto $4 = MakeStack(sizeof(char *));
    name = "Frank";
    StackPushBack($4, &name);
    ASSERT_STREQ("Frank", *(const char **)StackPopBack($4));
    name = "Grace";
    StackPushBack($4, &name);
    name = "Henry";
    StackPushBack($4, &name);
    name = "Ivy";
    StackPushBack($4, &name);
    ASSERT_STREQ("Ivy", *(const char **)StackPopBack($4));
    ASSERT_EQ(2, StackSize($4));

    auto $5 = MakeStack(sizeof(char *));
    name = "Jack";
    StackPushBack($5, &name);
    ASSERT_STREQ("Jack", *(const char **)StackPopBack($5));
    name = "Kelly";
    StackPushBack($5, &name);
    ASSERT_STREQ("Kelly", *(const char **)StackPopBack($5));
    ASSERT_EQ(0, StackSize($5));
}
