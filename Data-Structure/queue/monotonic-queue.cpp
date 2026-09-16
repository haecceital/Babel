#pragma once

#include <cstddef>
#include <vector>
#include <functional>
#include "../deque/deque.hpp" // /Data-Structure/deque/deque.cpp


template <typename T, typename Compare = std::greater<T>>
class MonotonicQueue {
private:
    deque<T> container_;
    Compare comp_;
    
public:
    explicit MonotonicQueue(Compare compare = Compare()) : comp_(compare) {}
    
    size_t size() const { return container_.size(); }
    
    bool empty() const { return container_.empty(); }
    
    const T& front() const { return container_.front(); }
    
    T& front() { return container_.front(); }
    
    std::vector<T> push(T val) {
        std::vector<T> popped;
        while (!container_.empty() && comp_(container_.back(), val)) {
            popped.push_back(container_.back());
            container_.pop_back();
        }
        container_.push_back(val);
        return popped;
    }
    
    void pop() { container_.pop_front(); }
};