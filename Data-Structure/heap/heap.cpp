#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>


template<typename T, typename Compare>
class heap {
private:
    std::vector<T> mem_pool_;
    Compare comp_;

    size_t parent(size_t i) { return (i - 1) >> 1; }
    size_t left_child(size_t i) { return (i << 1) + 1; }
    size_t right_child(size_t i) { return (i << 1) + 2; }

    void sink(size_t idx) {
        size_t n = mem_pool_.size();

        while (left_child(idx) < n) {
            size_t result_idx = idx;
            size_t left = left_child(idx);
            size_t right = right_child(idx);

            if (comp_(mem_pool_[result_idx], mem_pool_[left])) result_idx = left;
            if (right < n && comp_(mem_pool_[result_idx], mem_pool_[right])) result_idx = right;
            if (result_idx == idx) break;

            std::swap(mem_pool_[idx], mem_pool_[result_idx]);
            
            idx = result_idx; 
        }
    }

    void swim(size_t idx) {
        while (idx > 0 && comp_(mem_pool_[parent(idx)], mem_pool_[idx])) {
            std::swap(mem_pool_[idx], mem_pool_[parent(idx)]);
            idx = parent(idx);
        }
    }

public:
    explicit heap(Compare compare = Compare()) : comp_(compare) {}

    void push(T val) {
        mem_pool_.push_back(val);
        swim(mem_pool_.size() - 1);
    }

    void pop() {
        if (empty()) return;

        mem_pool_[0] = mem_pool_.back();
        mem_pool_.pop_back();
        sink(0);
    }

    T& top() {
        if (empty()) throw std::out_of_range("heap is empty");
        
        return mem_pool_[0];
    }
    
    size_t size() const { return mem_pool_.size(); }
    bool empty() const { return mem_pool_.empty(); }
};