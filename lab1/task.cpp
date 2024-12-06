#include <iostream>
#include <cstring>

class Stack {
    private:
        int *container;
        size_t size;
        size_t capacity;
    public:
        Stack();
        ~Stack();
        void Push(int el);
        int Pop();
        size_t Size() const;
};

Stack::Stack() : size(0), capacity(0), container(nullptr) {}

Stack::~Stack() {
    size = 0;
    capacity = 0;
    delete[] container;
    container = nullptr;
}

void Stack::Push(int el) {
    if (size == capacity) {
        capacity = capacity * 2 + 1;
        int *newCont = new int[capacity];
        memcpy(newCont, container, sizeof(int) * size);
        delete[] container;
        newCont[size] = el;
        container = newCont;
    } else {
        container[size] = el; 
    }
    ++size;
}

int Stack::Pop() {
    --size;
    return container[size];
}

size_t Stack::Size() const{
    return size;
}

int *fib(int n) {
    int *res = new int[n];
    res[0] = 0; res[1] = 1;
    for (int i = 2; i < n; ++i) {
        res[i] = res[i - 1] + res[i - 2];
    }
    return res;
}

bool find(int *arr, int el) {
    for (int i = 0; i < 17; ++i) {
        if (arr[i] - 1 == el || arr[i] + 1 == el) {
            return true;
        }
    }
    return false;
}

void solve(Stack &s) {
    int *fibo = fib(16);
    Stack tmp;
    int el;
    while (s.Size() > 0) {
        el = s.Pop();
        if (!find(fibo, el)) {
            tmp.Push(el);
        }
    }
    while (tmp.Size() > 0) {
        s.Push(tmp.Pop());
    }
}