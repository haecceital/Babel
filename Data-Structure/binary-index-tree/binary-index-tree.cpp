#pragma once

#include <cstddef>
#include <vector>
#include <iterator>

#define lowbit(x) ((x) & -(x))


template<typename T, typename Merge, typename UnMerge>
class BinaryIndexTree {
private:
    size_t size_;
    std::vector<T> tree_;
    Merge merge_;
    UnMerge unmerge_;

    void build(const T *data) {
        for(size_t i = 0; i < size_; ++i) tree_[i] = data[i];

        for(size_t i = 0; i < size_; ++i) {
            size_t idx = i + 1;
            size_t parent_idx = idx + lowbit(idx);
            
            if (parent_idx <= size_) {
                tree_[parent_idx - 1] = merge_(tree_[parent_idx - 1], tree_[i]);
            }
        }
    }

    T query_prefix(size_t idx) {
        T prefix = tree_[idx];
        idx -= lowbit(idx);

        while(idx) {
            prefix = merge_(prefix, tree_[idx]);
            idx -= lowbit(idx);
        }

        return prefix;
    }

public:
    template <typename InputIt>
    BinaryIndexTree(InputIt first, InputIt last, Merge merge = Merge(), UnMerge unmerge = UnMerge()) : merge_(merge), unmerge_(unmerge) {
        size_ = std::distance(first, last);
        if (size_ == 0) return;

        tree_.resize(size_);

        const T *data_ptr = &*first;

        build(data_ptr);
    }

    T query(size_t start, size_t end) {
        T right_prefix = query_prefix(end);

        if (start == 0) return right_prefix;
        start--;
        
        T left_prefix = query_prefix(start);

        return unmerge_(right_prefix, left_prefix);        
    }
};
