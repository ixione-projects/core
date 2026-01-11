#ifndef INTEGERAL_HPP
#define INTEGERAL_HPP

#include <limits>
#include <span>

#include "core.h"
#include "integral.h"

template <typename T>
T Minimum(std::span<const T> array)
    requires std::integral<T>
{
    if (array.empty()) {
        Panic("'size' must be > 0");
    }

    auto min = std::numeric_limits<T>::max();
    for (const auto &item : array) {
        if (item < min) {
            min = item;
        }
    }

    return static_cast<T>(min);
}

#endif // INTEGERAL_HPP
