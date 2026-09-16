#pragma once

#include <cstddef>
#include <vector>
#include <functional>
#include "../stack.hpp" // /Data-Structure/stack/stack.cpp


template<typename T, typename Compare = std::greater<T>>
class MonotonicStack {
private:
    stack<T> container_;
    Compare comp_;
    
public:
    explicit MonotonicStack(Compare comp = Compare()) : comp_(comp) {}

    size_t size() const { return container_.size(); }
    
    bool empty() const { return container_.empty(); }
    
    const T& top() const { return container_.top(); }
    
    T& top() { return container_.top(); }
    
    std::vector<T> push(T val) {
        std::vector<T> popped;
        while (!container_.empty() && comp_(container_.top(), val)) {
            popped.push_back(container_.top());
            container_.pop();
        }
        container_.push(val);
        return popped;
    }
    
    void pop() { container_.pop(); }
};