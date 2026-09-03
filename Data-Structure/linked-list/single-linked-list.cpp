#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>


template<typename T>
class SingleLinkedList {
private:
    static constexpr size_t null_idx = -1;

    struct Node {
        T val;
        int next;
    };

    std::vector<Node> mem_pool_;
    std::vector<size_t> free_slots_;
    size_t head_;
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

    size_t get_target_idx(size_t idx) {
        if (idx >= size()) throw std::out_of_range("SingleLinkedList index out of range");
    	
        size_t target_idx = head_;
        for(size_t i = 0; i < idx; ++i) {
            target_idx = mem_pool_[target_idx].next;
        }

        return target_idx;
    }

public:
    SingleLinkedList() : head_(null_idx), size_(0) {}

    size_t size() const { return size_; }

    void insert(size_t idx, T val) {
        if (idx == 0) {
	        Node node = { val, head_ };
            size_t new_idx = alloc_node();

	        mem_pool_[new_idx] = node;
	        head_ = new_idx;
	        ++size_;
	        return;
	    }
	    size_t left_idx = get_target_idx(idx - 1);
        size_t right_idx = mem_pool_[left_idx].next;
        
        size_t new_idx = alloc_node();

        Node node = { val, right_idx };
        mem_pool_[new_idx] = node;

        mem_pool_[left_idx].next = new_idx;

        ++size_;
    }

    void remove(size_t idx) {
        if (idx == 0) {
            size_t old_head_ = head_;
            head_ = mem_pool_[head_].next;
            free_node(old_head_);
            --size_;
            return;
        }
        
        size_t left_idx = get_target_idx(idx - 1);

        size_t target_idx = mem_pool_[left_idx].next;
        mem_pool_[left_idx].next = mem_pool_[target_idx].next;

        free_node(target_idx);
		--size_;
    }

    void clear() {
        free_slots_.clear();
        mem_pool_.clear();
        head_ = null_idx;
        size_ = 0;
    }

    const T& front() const { return mem_pool_[head_].val; }
    T& front() { return mem_pool_[head_].val; }

    const T& operator[](size_t idx) const { return mem_pool_[get_target_idx(idx)].val; }
    T& operator[](size_t idx) { return mem_pool_[get_target_idx(idx)].val; }
};


#ifdef BUILD_MAIN

#include <iostream>

int main() {
    SingleLinkedList<int> sll;
    
    sll.insert(0, 2);
    sll.insert(0, 1);
    
    sll.remove(0);
    
    std::cout << sll[0] << std::endl;
    
    return 0;
}

#endif