#pragma once

#include <cstddef>
#include "../deque/deque.hpp" // /Data-Structure/deque/deque.cpp


template <typename T>
class queue {
private:
    deque<T> container_;
    
public:
    size_t size() const { return container_.size(); }
    
    bool empty() const { return container_.empty(); }
    
    const T& front() const { return container_.front(); }
    
    T& front() { return container_.front(); }
    
    void push(T val) { container_.push_back(val); }
    
    void pop() { container_.pop_front(); }
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