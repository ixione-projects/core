#include <cstddef>
#include <optional>
#include <stdexcept>

#include "stack.h"

struct stack_push_error : public std::runtime_error {
    stack_push_error(const std::string &message)
        : std::runtime_error("Stack::Push: " + message) {}
};

struct Stack {
  private:
    StackType t;
    std::size_t c;
    StackValue *vs;
    std::size_t p = 0;

  public:
    explicit Stack(StackType t) : Stack(t, InitialStackCapacity) {};
    Stack(StackType t, std::size_t c) : t(t), c(c), vs(new StackValue[c]{}) {};

    Stack(const Stack &) = delete;
    Stack &operator=(const Stack &) = delete;

    ~Stack() { delete[] vs; }

    void Push(StackValue v) {
        if (v.type != this->t) {
            throw stack_push_error("InvalidType");
        }

        if (this->p >= this->c) {
            this->grow();
        }
        this->vs[this->p++] = v;
    }

    std::optional<StackValue> Pop() {
        if (this->IsEmpty()) {
            return std::nullopt;
        }
        this->p -= 1;
        return this->vs[this->p];
    }

    StackValue *Peek() const {
        if (this->IsEmpty()) {
            return nullptr;
        }
        return &this->vs[this->p - 1];
    }

    std::size_t Size() const { return this->p; }

    bool IsEmpty() const { return this->Size() == 0; }

  private:
    void grow() {
        delete[] this->vs;
        this->c = (this->c >> 1) + this->c;
        this->vs = new StackValue[this->c]{};
    }
};

extern "C" {
Stack_t *NewStack(StackType type) {
    return reinterpret_cast<Stack_t *>(new Stack(type));
}

Stack_t *NewStackWithCapacity(StackType type, size_t c) {
    return reinterpret_cast<Stack_t *>(new Stack(type, c));
}

void DeleteStack(Stack_t *s) { delete reinterpret_cast<Stack *>(s); }

int StackPush(Stack_t *s, StackValue value) {
    try {
        reinterpret_cast<Stack *>(s)->Push(value);
        return 0;
    } catch (const stack_push_error &e) {
        return -1;
    }
}

StackValue StackPop(Stack_t *s) {
    auto maybe = reinterpret_cast<Stack *>(s)->Pop();
    if (maybe.has_value()) {
        return maybe.value();
    } else {
        return NULL_VALUE;
    }
}

StackValue *StackPeek(Stack_t *s) {
    return reinterpret_cast<Stack *>(s)->Peek();
}

size_t StackSize(const Stack_t *s) {
    return reinterpret_cast<const Stack *>(s)->Size();
}

bool StackIsEmpty(const Stack_t *s) {
    return reinterpret_cast<const Stack *>(s)->IsEmpty();
}
}
