#include <cstdlib>
#include <cstring>
#include <vector>

#include "container_allocator.hxx"

#include "array_list.h"
#include "core.h"

#include <stdio.h>

#define array_list_at(list, i) ((list)->data() + (i) * (list)->value_type_size())
#define array_list_size(list) ((list)->size() / (list)->value_type_size())

struct vector_container : std::vector<char, container_allocator<char>> {
    size_t s = 0;

    vector_container(container_allocator<char> allocator) : std::vector<char, container_allocator<char>>(allocator) {}

    size_t value_type_size() const {
        return this->get_allocator().inner.value_type_size;
    }
};

extern "C" {
ArrayList *NewArrayList(Allocator allocator) {
    return reinterpret_cast<ArrayList *>(new vector_container(container_allocator<char>{allocator}));
}

void DeleteArrayList(ArrayList *list) {
    delete reinterpret_cast<vector_container *>(list);
}

const void *ArrayListAt(const ArrayList *list, size_t i) {
    auto l = reinterpret_cast<const vector_container *>(list);
    assert(i < array_list_size(l), "`i = %d`", i);

    return array_list_at(l, i);
}

void ArrayListInsert(ArrayList *list, size_t i, void *value) {
    auto l = reinterpret_cast<vector_container *>(list);
    if (i >= l->capacity() / l->value_type_size()) {
        l->reserve((i ? (i >> 1) + i : 10) * l->value_type_size());
    }
    l->resize(l->size() + l->value_type_size());
    std::memcpy(array_list_at(l, i), value, l->value_type_size());
}

void *ArrayListRemove(ArrayList *list, size_t i) {
    auto l = reinterpret_cast<vector_container *>(list);
    assert(i < array_list_size(l), "`i = %d`", i);

    auto result = std::malloc(l->value_type_size());
    auto value = array_list_at(l, i);
    std::memcpy(result, value, l->value_type_size());

    l->resize(l->size() - l->value_type_size());
    std::memmove(value, value + l->value_type_size(), (array_list_size(l) - i) * l->value_type_size());
    return result;
}

size_t ArrayListSize(const ArrayList *list) {
    auto l = reinterpret_cast<const vector_container *>(list);
    return array_list_size(l);
}

bool ArrayListIsEmpty(const ArrayList *list) {
    auto l = reinterpret_cast<const vector_container *>(list);
    return array_list_size(l) == 0;
}
}
