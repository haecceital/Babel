#pragma once

#include <cstddef>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdexcept>


template<typename T, typename Compare>
class BinarySearchTree {
private:
    static constexpr size_t null_idx = -1;

    struct Node {
        T val;
        size_t left = null_idx;
        size_t right = null_idx;
    };

    std::vector<Node> tree_;
    std::vector<size_t> free_slots_;
    size_t root_ = null_idx;
    Compare compare_;

    size_t alloc_node() {
        if (!free_slots_.empty()) {
            size_t idx = free_slots_.back();
            free_slots_.pop_back();
            return idx;
        }

        tree_.emplace_back({});
        return tree_.size() - 1;
    }

    void free_node(size_t idx) { free_slots_.emplace_back(idx); }

    size_t build(size_t l, size_t r) {
        if (l > r || r == null_idx) return null_idx;

        size_t mid = l + (r - l) / 2;

        size_t left_child = build(l, mid - 1);
        size_t right_child = build(mid + 1, r);

        tree_[mid].left = left_child;
        tree_[mid].right = right_child;

        return mid;
    }

    size_t find_closest(T val, bool return_parent) {
        if (root_ == null_idx) return null_idx;

        size_t idx = root_, parent_idx = null_idx;
        while (1) {
            if (compare_(val, tree_[idx].val)){
                if (tree_[idx].left == null_idx) break;
                parent_idx = idx;
                idx = tree_[idx].left;
            } else if (compare_(tree_[idx].val, val)) {
                if (tree_[idx].right == null_idx) break;
                parent_idx = idx;
                idx = tree_[idx].right;
            } else break;
        }

        return return_parent ? parent_idx : idx;
    }

public:
    template <typename InputIt>
    BinarySearchTree(InputIt first, InputIt last, Compare compare = Compare()) : compare_(compare) {
        size_t size = std::distance(first, last);
        if (size == 0) return;

        tree_.resize(size);
        size_t idx = 0;
        for (auto it = first; it != last; ++it) {
            tree_[idx++].val = *it;
        }

        std::sort(tree_.begin(), tree_.end(), [&](const Node& a, const Node& b) {
            return compare(a.val, b.val);
        });

        root_ = build(0, size - 1);
    }

    bool contain(T val) {
        size_t idx = find_closest(val, false);
        if (idx == null_idx) return false;

        return (val == tree_[idx].val) ? true : false;
    }

    void insert(T val) {
        size_t idx = find_closest(val, false);

        if (idx != null_idx && tree_[idx].val == val) return;

        size_t new_idx = alloc_node();
        tree_[new_idx].val = val;

        if (idx == null_idx) root_ = new_idx;
        else if (compare_(val, tree_[idx].val)) tree_[idx].left = new_idx;
        else tree_[idx].right = new_idx;
    }

    void remove(T val) {
        size_t idx = find_closest(val, true);

        if (idx == null_idx) {
            if (root_ != null_idx && tree_[root_].val == val && tree_.size() == 1) {
                free_node(root_);
                root_ = null_idx;
            }

            return;
        }

        bool left = !compare_(val, tree_[ tree_[idx].left ].val) && !compare_(tree_[ tree_[idx].left ].val, val);
        bool right = !compare_(val, tree_[ tree_[idx].right ].val) && !compare_(tree_[ tree_[idx].right ].val, val);

        if (!left && !right) return;

        size_t min, max;
        if (left) {

        } else {

        }

    }

    T find_max() {
        if (root_ == null_idx) throw std::out_of_range("tree is empty");

        size_t idx = root_;
        while (tree_[idx].right != null_idx) idx = tree_[idx].right;

        return tree_[idx].val
    }

    T find_min() {
        if (root_ == null_idx) throw std::out_of_range("tree is empty");

        size_t idx = root_;
        while (tree_[idx].left != null_idx) idx = tree_[idx].left;

        return tree_[idx].val
    }
};
