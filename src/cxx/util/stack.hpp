#ifndef STACK_H
#define STACK_H

#include <cstddef>

#define INITIAL_STACK_CAPACITY 10

template <typename T> class Stack {
  public:
    Stack() : Stack<T>(INITIAL_STACK_CAPACITY) {};
    Stack(int c) : c(c), es(new T(c)) {};

    ~Stack() { delete es; }

    void push(T e) {
        if (this->p >= this->c) {
            this->grow();
        }
        this->es[this->p++] = e;
    }

    T peek() {
        if (this->p == 0) {
            return (T)NULL;
        }
        return this->es[this->p - 1];
    }

    T pop() {
        if (this->p == 0) {
            return (T)NULL;
        }
        this->p -= 1;
        return this->es[this->p];
    }

    int size() { return this->p; }

  private:
    int c;
    T *es;
    int p = 0;

    void grow() {
        delete this->es;
        this->es = new T((this->c >> 1) + this->c);
    }
};

#endif // STACK_H
