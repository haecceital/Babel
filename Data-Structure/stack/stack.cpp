#pragma once

#include <cstddef>
#include "../deque/deque.hpp" // /Data-Structure/deque/deque.cpp


template <typename T>
class stack {
private:
    deque<T> container_;
    
public:
    size_t size() const { return container_.size(); }
    
    bool empty() const { return container_.empty(); }
    
    const T& top() const { return container_.back(); }
    
    T& top() { return container_.back(); }
    
    void push(T val) { container_.push_back(val); }
    
    void pop() { container_.pop_back(); }
};


#ifdef BUILD_MAIN

#include <iostream>

int main() {
    deque dq<int>;

    dq.push_back(1);

    std::cout << dq.back() << endl;

    return 0;
}

#endif