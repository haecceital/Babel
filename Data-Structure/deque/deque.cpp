#pragma once

#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <stdexcept>


template <typename T>
class deque {
private:
    static constexpr size_t BLOCK_SIZE = (sizeof(T) < 512) ? (512 / sizeof(T)) : 1;

    T** map_;
    size_t map_size_;
    size_t start_block_;
    size_t start_idx_;
    size_t start_idx_;
    size_t end_idx_;

    T get_val(size_t i) {
        if (i >= size()) throw std::out_of_range("deque index out of range");

        size_t flat_idx = start_block_ * BLOCK_SIZE + start_idx_ + i;
        size_t block = flat_idx / BLOCK_SIZE;
        size_t idx = flat_idx % BLOCK_SIZE;
        
        return map_[block][idx];
    }
    
public:
    deque() {
        map_size_ = 8;

        map_ = (T**)calloc(map_size_, sizeof(T*));

        start_block_ = map_size_ >> 1;
        start_idx_ = map_size_ >> 1;
    
        start_idx_ = 0;
        end_idx_ = 0;
    }

    ~deque() {
        while(!empty()) {
            pop_back(); 
        }

        for (size_t i = 0; i < map_size_; ++i) {
            if (map_[i] != nullptr) {
                ::operator delete(map_[i]);
            }
        }

        free(map_);
    }

    size_t size() const {
        size_t flat_start = start_block_ * BLOCK_SIZE + start_idx_;
        size_t flat_end   = start_idx_ * BLOCK_SIZE + end_idx_;
    
        return flat_end - flat_start;
    }
    
    bool empty() const { return (size() == 0) ? true : false; }
    
    const T& front() const { return map_[start_block_][start_idx_]; }
    T& front() { return map_[start_block_][start_idx_]; }
    
    const T& back() const {
        size_t flat_last = start_idx_ * BLOCK_SIZE + end_idx_ - 1;
        size_t block = flat_last / BLOCK_SIZE;
        size_t idx = flat_last % BLOCK_SIZE;
        
        return map_[block][idx];
    }
    
    T& back() {
        size_t flat_last = start_idx_ * BLOCK_SIZE + end_idx_ - 1;
        size_t block = flat_last / BLOCK_SIZE;
        size_t idx = flat_last % BLOCK_SIZE;
        
        return map_[block][idx];
    }
    
    void push_back(T val) {
        if (start_idx_ == map_size_) {
            T** new_map = (T**)calloc(map_size_ << 1, sizeof(T*));
            
            memcpy(new_map + (map_size_ >> 1), map_, map_size_ * sizeof(T*));
            size_t offset = map_size_ >> 1;
            map_size_ <<= 1;
            
            start_block_ += offset;
            start_idx_ += offset;
            
            free(map_);
            map_ = new_map;
        }
        
        if (map_[start_idx_] == nullptr) {
            map_[start_idx_] = static_cast<T*>(::operator new(BLOCK_SIZE * sizeof(T)));
        }
        
        new (&map_[start_idx_][end_idx_]) T(val);
        size_t flat_last = start_idx_ * BLOCK_SIZE + end_idx_ + 1;
        start_idx_ = flat_last / BLOCK_SIZE;
        end_idx_ = flat_last % BLOCK_SIZE;
    }
    
    void push_front(T val) {
        if (start_block_ == 0) {
            T** new_map = (T**)calloc(map_size_ << 1, sizeof(T*));
            
            memcpy(new_map + (map_size_ >> 1), map_, map_size_ * sizeof(T*));
            size_t offset = map_size_ >> 1;
            map_size_ <<= 1;
            
            start_block_ += offset;
            start_idx_ += offset;
            
            free(map_);
            map_ = new_map;
        }
        
        size_t flat_first = start_block_ * BLOCK_SIZE + start_idx_ - 1;
        start_block_ = flat_first / BLOCK_SIZE;
        start_idx_ = flat_first % BLOCK_SIZE;
        
        if (map_[start_block_] == nullptr) {
            map_[start_block_] = static_cast<T*>(::operator new(BLOCK_SIZE * sizeof(T)));
        }
        
        new (&map_[start_block_][start_idx_]) T(val);
    }

    void pop_back() {
        if (empty()) return;

        map_[start_idx_][end_idx_].~T();
        
        size_t flat_last = start_idx_ * BLOCK_SIZE + end_idx_ - 1;
        size_t old_block = start_idx_;
        
        start_idx_ = flat_last / BLOCK_SIZE;
        end_idx_ = flat_last % BLOCK_SIZE;

        if (start_idx_ < old_block) {
            ::operator delete(map_[old_block]);
            map_[old_block] = nullptr;
        }
    }

    void pop_front() {
        if (empty()) return;

        map_[start_block_][start_idx_].~T();
        
        size_t flat_first = start_block_ * BLOCK_SIZE + start_idx_ + 1;
        size_t old_block = start_block_;
        
        start_block_ = flat_first / BLOCK_SIZE;
        start_idx_ = flat_first % BLOCK_SIZE;

        if (start_block_ > old_block) {
            ::operator delete(map_[old_block]);
            map_[old_block] = nullptr;
        }
    }

    const T& operator[](size_t i) const { return get_val(i); }
    T& operator[](size_t i) { return get_val(i); }
};


#ifdef BUILD_MAIN

#include <iostream>

int main() {
    deque<int> dq;

    dq.push_back(1);

    std::cout << dq.back() << std::endl;

    return 0;
}

#endif