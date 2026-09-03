#pragma once

#include <cstddef>
#include <vector>
#include <iterator>
#include <cmath>


template <typename T, typename Merge> 
class SparseTable {
private:
    size_t size_;
    size_t max_j_;
    std::vector<std::vector<T>> st_;
    Merge merge_;


    void build(const T* data) {
        for(size_t i = 0; i < size_; ++i) st_[0][i] = data[i];

        size_t gap = 1;
        for(size_t j = 1; j < max_j_; ++j) {
            for(size_t i = 0; i + (gap * 2) < size_; ++i) {
                st_[j][i] = merge_(st_[j - 1][i], st_[j - 1][i + gap]);
            }
            gap *= 2;
        }
    }

public:
    template<typename InputIt>
    SparseTable(InputIt first, InputIt last, Merge merge = Merge()) : merge_(merge) {
        size_ = std::distance(first, last);
        if (size_ == 0) return;
        
        max_j_ = std::log2(size_) + 1;
        st_.assign(max_j_, std::vector<T>(size_));

        const T *data_ptr = &*first;

        build(data_ptr);
    }

    T query(size_t start, size_t end) {
        size_t idx = std::log2(end - start + 1);
        return merge_(st_[idx][start], st_[idx][end - (1 << idx) + 1]);
    }
};
