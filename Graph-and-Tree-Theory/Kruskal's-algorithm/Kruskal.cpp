#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>
#include "../../Data-Structure/disjoint-set-union/disjoint-set-union.hpp" // Data-Structure/disjoint-set-union/disjoint-set-union.cpp


// u <-> v
struct Edge { size_t u, v; long long weight; };

struct MSTResult {
    long long weight;
    std::vector<Edge> edges;
};

// n is the number of node
MSTResult Kruskal(size_t n, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) -> bool {
        return a.weight < b.weight;
    });

    DisjointSetUnion dsu(n);
    MSTResult result = { 0, {} };

    for(const Edge &e : edges) {
        if (!dsu.query(e.u, e.v)) {
            dsu.unite(e.u, e.v);
            result.weight += e.weight;
            result.edges.push_back(e);
        }
    }

    return result;
}