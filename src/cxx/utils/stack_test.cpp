#include <gtest/gtest.h>

#include "stack.h"

TEST(StackTest, TestStackPush) {
    Stack_t *s = NewStack();

    StackPush(s, StackValue{StackI32, {.i32 = 1}});
    StackPush(s, StackValue{StackI32, {.i32 = 2}});

    ASSERT_EQ(2, StackSize(s));
    DeleteStack(s);

    s = NewStack();

    StackPush(s, StackValue{StackI32, {.i32 = 3}});
    StackPush(s, StackValue{StackI32, {.i32 = 4}});
    StackPush(s, StackValue{StackI32, {.i32 = 5}});

    ASSERT_EQ(3, StackSize(s));
    DeleteStack(s);

    s = NewStack();

    ASSERT_EQ(0, StackSize(s));
    DeleteStack(s);

    s = NewStack();

    StackPush(s, StackValue{StackI32, {.i32 = 6}});
    StackPush(s, StackValue{StackI32, {.i32 = 7}});
    StackPush(s, StackValue{StackI32, {.i32 = 8}});
    StackPush(s, StackValue{StackI32, {.i32 = 9}});

    ASSERT_EQ(4, StackSize(s));
    DeleteStack(s);

    s = NewStack();

    StackPush(s, StackValue{StackI32, {.i32 = 10}});
    StackPush(s, StackValue{StackI32, {.i32 = 11}});

    ASSERT_EQ(2, StackSize(s));
    DeleteStack(s);
}
