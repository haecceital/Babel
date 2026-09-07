#pragma once

#include <cstddef>
#include <vector>


class DisjointSetUnion {
private:
    struct Node {
        size_t parent;
        size_t rank;
    };
    std::vector<Node> tree_;

public:
    size_t push() {
        tree_.emplaceback(Node { idx, 0 })

        return tree_.size() - 1;
    }

    size_t find(size_t a) {
        if (a == tree_[a].parent) return a;

        return tree_[a].parent = find(tree_[a].parent);
    }

    bool query(size_t a, size_t b) { return find(a) == find(b); }

    void unite(size_t a, size_t b) {
        size_t rootA = find(a);
        size_t rootB = find(b);

        if (tree_[rootA].rank < tree_[rootB].rank) tree_[rootA].parent = rootB;
        else if (tree_[rootA].rank > tree_[rootB].rank) tree_[rootB].parent = rootA;
        else {
            tree_[rootB].parent = rootA;
            tree_[rootA].rank++;
        }
    }
}