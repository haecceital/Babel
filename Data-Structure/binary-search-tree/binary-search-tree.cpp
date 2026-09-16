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
        size_t count = 0;
        size_t left = null_idx;
        size_t right = null_idx;
    };

    std::vector<Node> tree_;
    std::vector<size_t> free_slots_;
    Compare compare_;
    size_t root_ = null_idx;

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

    size_t find_closest(T val, bool return_parent = false) {
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

    bool equal(T a, T b) { return !compare_(a, b) && !compare_(b, a); }

public:
    template <typename InputIt>
    BinarySearchTree(InputIt first, InputIt last, Compare compare = Compare()) : compare_(compare) {
        std::vector<T> tmp(first, last);
        if (tmp.empty()) return;

        tree_.resize(tmp.size());

        size_t write_idx = 0;
        tree_[0].val = tmp[0];
        tree_[0].count = 1;
        for (size_t read_idx = 1; read_idx < tmp.size(); ++read_idx) {
            if (equal(tmp[read_idx], tree_[write_idx].val);) {
                tree_[write_idx].count++;
            } else {
                write_idx++;
                tree_[write_idx].val = tmp[read_idx];
                tree_[write_idx].count = 1;
            }
        }

        size_t unique_size = write_idx + 1;
        tree_.resize(unique_size);

        root_ = build(0, unique_size - 1);
    }

    bool contain(T val) {
        size_t idx = find_closest(val);
        if (idx == null_idx) return false;

        return (val == tree_[idx].val) ? true : false;
    }

    void insert(T val) {
        size_t idx = find_closest(val);

        if (idx != null_idx && tree_[idx].val == val) return;

        size_t new_idx = alloc_node();
        tree_[new_idx].val = val;

        if (idx == null_idx) root_ = new_idx;
        else if (compare_(val, tree_[idx].val)) tree_[idx].left = new_idx;
        else tree_[idx].right = new_idx;
    }

    void remove(T val, size_t cnt = 1) {
        if (!contain(val)) return;

        size_t parent_idx = find_closest(val, true);
        size_t idx = null_idx;
        bool is_left_child = false;

        if (parent_idx == null_idx) idx = root_;
        else if (tree_[parent_idx].left != null_idx && equal(val, tree_[tree_[parent_idx].left].val)) {
            idx = tree_[parent_idx].left;
            is_left_child = true;
        } else {
            idx = tree_[parent_idx].right;
            is_left_child = false;
        }

        if (tree_[idx].count > cnt) {
            tree_[idx].count -= cnt;
            return;
        }

        size_t child_cnt = (tree_[idx].left != null_idx) + (tree_[idx].right != null_idx);

        auto attach_to_parent = [&](size_t new_child) {
            if (parent_idx == null_idx) root_ = new_child;
            else if (is_left_child)  tree_[parent_idx].left = new_child;
            else tree_[parent_idx].right = new_child;
        };

        if (child_cnt == 0) {
            attach_to_parent(null_idx);
            free_node(idx);
        } else if (child_cnt == 1) {
            size_t only_child = (tree_[idx].left != null_idx) ? tree_[idx].left : tree_[idx].right;
            attach_to_parent(only_child);
            free_node(idx);
        } else {
            size_t succ_parent = idx;
            size_t succ = tree_[idx].right;

            while (tree_[succ].left != null_idx) {
                succ_parent = succ;
                succ = tree_[succ].left;
            }

            tree_[idx].val = tree_[succ].val;
            tree_[idx].count = tree_[succ].count;

            size_t succ_child = tree_[succ].right;
            if (succ_parent == idx) {
                tree_[succ_parent].right = succ_child;
            } else {
                tree_[succ_parent].left = succ_child;
            }

            free_node(succ);
        }
    }

    T find_max() {
        if (root_ == null_idx) throw std::out_of_range("tree is empty");

        size_t idx = root_;
        while (tree_[idx].right != null_idx) idx = tree_[idx].right;

        return tree_[idx].val;
    }

    T find_min() {
        if (root_ == null_idx) throw std::out_of_range("tree is empty");

        size_t idx = root_;
        while (tree_[idx].left != null_idx) idx = tree_[idx].left;

        return tree_[idx].val;
    }
};
