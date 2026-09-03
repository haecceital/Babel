#pragma once

#include <cstddef>
#include <vector>
#include <iterator>

template <
    typename T,
    typename Merge,
    typename ApplyLazy,
    typename LazyType = T
>
class SegmentTree {
private:
    size_t size_;
    std::vector<T> tree_;
    std::vector<LazyType> lazy_;
    Merge merge_;
    ApplyLazy apply_lazy_;
    T id_t_;
    LazyType id_lazy_;

    void build(size_t l, size_t r, size_t cur, const T *data) {
        if (l == r) {
            tree_[cur] = data[l];
            return;
        }

        size_t mid = l + (r - l) / 2;

        build(l, mid, cur * 2 + 1, data);
        build(mid + 1, r, cur * 2 + 2, data);

        tree_[cur] = merge(tree_[cur * 2 + 1], tree_[cur * 2 + 2]);
    }

    T query_(size_t start, size_t end, size_t l, size_t r, size_t p) {
        if (r < start || l > end) return id_t_;

        if (l >= start && r <= end) return tree_[p];

        push_down(l, r, p);

        size_t mid = l + (r - l) / 2;

        return merge_(
            query_(start, end, l, mid, p * 2 + 1),
            query_(start, end, mid + 1, r, p * 2 + 2)
        );
    }

    void update_(size_t start, size_t end, size_t l, size_t r, size_t p, T new_val) {
        push_down(l, r, p);

        if (r < start || l > end) return;

        if (l >= start && r <= end) {
            lazy_[p] = new_val;

            tree_[p] = apply_lazy_(tree_[p], new_val, r - l + 1);
            return;
        }

        size_t mid = l + (r - l) / 2;

        update_(start, end, l, mid, p * 2 + 1, new_val);
        update_(start, end, mid + 1, r, p * 2 + 2, new_val);
    
        tree_[p] = merge_(tree_[p * 2 + 1], tree_[p * 2 + 2]);
    }
    
    void push_down(size_t l, size_t r, size_t p) {
        if (lazy_[p] == id_lazy_) return;

        size_t mid = l + (r - l) / 2;

        lazy_[p * 2 + 1] = merge_(lazy_[p * 2 + 1], lazy_[p]);
        tree_[p * 2 + 1] = apply_lazy_(tree_[p * 2 + 1], mid - l + 1, p);

        lazy_[p * 2 + 2] = merge_(lazy_[p * 2 + 2], lazy_[p]);
        tree_[p * 2 + 2] = apply_lazy_(tree_[p * 2 + 2], r - mid, p);

        lazy_[p] = id_lazy_;
    }
    
public:
    template <typename InputIt>
    SegmentTree(InputIt first, InputIt last, Merge merge = Merge(), ApplyLazy apply_lazy = ApplyLazy(), T id_t = T(), LazyType id_lazy = LazyType())
        : merge_(merge), apply_lazy_(apply_lazy), id_t_(id_t), id_lazy_(id_lazy) {

        size_ = std::distance(first, last);
        if (size_ == 0) return;

        tree_.resize(size_ * 4);
        lazy_.resize(size_ * 4, id_lazy_);

        const T *data_ptr = &*first;

        build(0, size_ - 1, 0, data_ptr);
    }

    T query(size_t start, size_t end) {
        if (size_ == 0 || start > end || start >= size_) return id_t_;

        if (end >= size_) end = size_ - 1;

        return query_(start, end, 0, size_ - 1, 0);
    }

    void update(size_t start, size_t end, T new_val) {
        if (size_ == 0 || start > end || start >= size_) return;

        if (end >= size_) end = size_ - 1;

        return update_(start, end, 0, size_ - 1, 0);
    }
};
