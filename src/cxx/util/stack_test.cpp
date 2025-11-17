#include <gtest/gtest.h>

#include "stack.hpp"

TEST(StackTest, TestPushAndPop) {
    Stack<int> s{};

    for (auto i = 0; i < 3; i++) {
        s.push(i);
    }

    ASSERT_EQ(3, s.size());

    for (auto i = 3; i > 0; i--) {
        ASSERT_EQ(i - 1, s.pop());
    }
}

TEST(StackTest, TestGrow) {
    Stack<int> s{};

    for (auto i = 0; i < 10; i++) {
        s.push(i);
    }

    ASSERT_EQ(10, s.size());

    for (auto i = 10; i > 0; i--) {
        ASSERT_EQ(i - 1, s.pop());
    }
}
