#include <gtest/gtest.h>

#include "stack.hxx"

TEST(StackTest, TestStackPush) {
    Stack $1 = Stack<int32_t>{};
    $1.Push(1);
    $1.Push(2);
    ASSERT_EQ(2, $1.Size());

    Stack $2 = Stack<int32_t>{};
    $2.Push(3);
    $2.Push(4);
    $2.Push(5);
    ASSERT_EQ(3, $2.Size());

    Stack $3 = Stack<int32_t>{};
    ASSERT_EQ(0, $3.Size());

    Stack $4 = Stack<int32_t>{};
    $4.Push(6);
    $4.Push(7);
    $4.Push(8);
    $4.Push(9);
    ASSERT_EQ(4, $4.Size());

    Stack $5 = Stack<int32_t>{};
    $5.Push(10);
    $5.Push(11);
    ASSERT_EQ(2, $5.Size());
}
