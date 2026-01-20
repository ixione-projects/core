#include <gtest/gtest.h>

#include "array_list.h"

TEST(ArrayList, At) {
    int32_t x = 0;
    auto $1 = NewArrayList(GeneralAllocator(sizeof(int32_t)));

    x = 1;
    ArrayListPushBack($1, &x);
    x = 2;
    ArrayListPushBack($1, &x);
    ASSERT_EQ(2, ArrayListSize($1));
}

TEST(ArrayList, Insert) {
}

TEST(ArrayList, Remove) {
}
