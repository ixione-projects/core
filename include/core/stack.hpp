#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <optional>

#include "stack.h"

static size_t InitialStackCapacity = 10;

template <typename T> struct Stack {
  private:
    StackType t;
    std::size_t c;
    T *vs;
    std::size_t p = 0;

  public:
    Stack() : Stack(InitialStackCapacity) {};
    explicit Stack(std::size_t c) : c(c), vs(new T[c]{}) {};

    Stack(const Stack &) = delete;
    Stack &operator=(const Stack &) = delete;

    ~Stack() { delete[] vs; }

    void Push(T v);
    std::optional<T> Pop();
    T *Peek() const;

    std::size_t Size() const { return this->p; }
    bool IsEmpty() const { return this->Size() == 0; }

  private:
    void grow();
};

template <typename T> void Stack<T>::Push(T v) {
    if (this->p >= this->c) {
        this->grow();
    }
    this->vs[this->p++] = v;
}

template <typename T> std::optional<T> Stack<T>::Pop() {
    if (this->IsEmpty()) {
        return std::nullopt;
    }
    this->p -= 1;
    return this->vs[this->p];
}

template <typename T> T *Stack<T>::Peek() const {
    if (this->IsEmpty()) {
        return nullptr;
    }
    return &this->vs[this->p - 1];
}

template <typename T> void Stack<T>::grow() {
    delete[] this->vs;
    this->c = (this->c >> 1) + this->c;
    this->vs = new T[this->c]{};
}

#endif // STACK_HPP
