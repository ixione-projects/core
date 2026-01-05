#ifndef LIST_HPP
#define LIST_HPP

#include <limits>
#include <span>

#include "definitions.h"

template <typename T>
T Minimum(std::span<const T> array)
    requires std::integral<T>
{
    if (array.empty()) {
        Panic("'size' must be > 0");
    }

    auto min = std::numeric_limits<int>::max();
    for (const auto &item : array) {
        if (item < min) {
            min = item;
        }
    }

    return min;
}

#endif // LIST_HPP
