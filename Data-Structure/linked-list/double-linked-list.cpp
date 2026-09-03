#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>


template<typename T>
class DoubleLinkedList {
private:
    static constexpr size_t null_idx = -1;

    struct Node {
        T val;
        int prev;
        int next;
    };

    std::vector<Node> mem_pool_;
    std::vector<size_t> free_slots_;
    size_t head_;
    size_t tail_;
    size_t size_;

    size_t alloc_node() {
        if (!free_slots_.empty()) {
            size_t idx = free_slots_.back();
            free_slots_.pop_back();
            return idx;
        }
        mem_pool_.emplace_back({});
        return mem_pool_.size() - 1;
    }

    void free_node(size_t idx) { free_slots_.emplace_back(idx); }

    size_t get_target_index(size_t idx) {
        if (idx >= size()) throw std::out_of_range("DoubleLinkedList index out of range");
    	
        size_t target_idx;
        if (idx <= size() / 2) {
            target_idx = head_;
            for (size_t i = 0; i < idx; ++i) {
                target_idx = mem_pool_[target_idx].next;
            }
        } else {
            target_idx = tail_;
            for (size_t i = 0; i < size() - 1 - idx; ++i) {
                target_idx = mem_pool_[target_idx].prev;
            }
        }

        return target_idx;
    }

public:
    DoubleLinkedList() : head_(null_idx), tail_(null_idx), size_(0) {}

    size_t size() const { return size_; }

    void insert(size_t idx, T val) {
        size_t new_idx = alloc_node();

        if (idx == size()) {
            Node node = {val, tail_, null_idx};
            mem_pool_[new_idx] = node;

            if (tail_ != null_idx) mem_pool_[tail_].next = new_idx;
            else head_ = new_idx;

            tail_ = new_idx;
            return;
        }
        size_t target_idx = get_target_index(idx);

        size_t left_idx = mem_pool_[target_idx].prev;
        size_t right_idx = mem_pool_[target_idx].next;

        if (left_idx != null_idx) mem_pool_[left_idx].next = new_idx;
        else head_ = new_idx;

        mem_pool_[right_idx].prev = new_idx;

        Node node = {val, left_idx, right_idx};
        mem_pool_[target_idx] = node;

        ++size_;
    }

    void remove(size_t idx) {
        size_t target_idx = get_target_index(idx);

        size_t left_idx = mem_pool_[target_idx].prev;
        size_t right_idx = mem_pool_[target_idx].next;
        
        if (left_idx != null_idx) mem_pool_[left_idx].next = right_idx;
        else head_ = right_idx;

        if (right_idx != null_idx) mem_pool_[right_idx].prev = left_idx;
        else tail_ = left_idx;

        free_node(target_idx);
        --size_;
    }

    void clear() {
        free_slots_.clear();
        mem_pool_.clear();
        head_ = null_idx;
        tail_ = null_idx;
        size_ = 0;
    }

    const T& front() const { return mem_pool_[head_].val; }
    T& front() { return mem_pool_[head_].val; }

    const T& operator[](size_t idx) const { return mem_pool_[get_target_index(idx)].val; }
    T& operator[](size_t idx) { return mem_pool_[get_target_index(idx)].val; }
};


#ifdef BUILD_MAIN

#include <iostream>

int main() {
    DoubleLinkedList<int> dll;
    
    dll.insert(0, 2);
    dll.insert(0, 1);
    
    dll.remove(0);
    
    std::cout << dll[0] << std::endl;
    
    return 0;
}

#endif