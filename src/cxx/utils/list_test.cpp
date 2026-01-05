#include <array>
#include <cstdint>
#include <gtest/gtest.h>

#include "list.hpp"

TEST(MinimumTest, TestMinimum) {
    std::array<int32_t, 9> $1 = {7, 4, 3, 100, 2343243, 343434, 1, 2, 32};
    ASSERT_EQ(1, Minimum<int32_t>({$1}));

    // std::array<int32_t, 3> $2 = {12, 12, 12};
    // ASSERT_EQ(12, Minimum<int32_t>({$2}));
    //
    // std::array<int32_t, 5> $3 = {10, 200, 3000, 5000, 4};
    // ASSERT_EQ(4, Minimum<int32_t>({$3}));
    //
    // std::array<int32_t, 1> $4 = {1};
    // ASSERT_EQ(1, Minimum<int32_t>({$4}));
    //
    // std::array<int32_t, 5> $5 = {1, 2, 3, 4, 5};
    // ASSERT_EQ(1, Minimum<int32_t>({$5}));
    //
    // std::array<int32_t, 5> $6 = {5, 4, 3, 2, 1};
    // ASSERT_EQ(1, Minimum<int32_t>({$6}));
    //
    // std::array<int32_t, 5> $7 = {100, 200, 300, 400, 500};
    // ASSERT_EQ(100, Minimum<int32_t>({$7}));
    //
    // std::array<int32_t, 5> $8 = {500, 400, 300, 200, 100};
    // ASSERT_EQ(100, Minimum<int32_t>({$8}));

    // std::array<int32_t, 0> $9 = {};
    // ASSERT_EQ(100, Minimum<int32_t>({$9}));
}
