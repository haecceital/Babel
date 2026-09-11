#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>
#include <climits>
#include <optional>
#include "../../Data-Structure/heap/heap.hpp" // Data-Structure/heap/heap.cpp


struct Edge { size_t u, v; long long weight; };

struct HalfEdge { size_t node; long long weight; };

struct Compare {
    bool operator()(const HalfEdge& a, const HalfEdge& b) const {
        return a.weight > b.weight;
    }
};

// n is the numbers of node
std::optional<long long> Dijkstra(size_t n, size_t start, size_t end, std::vector<Edge> edges) {
    std::vector<std::vector<HalfEdge>> adjacency_list(n);
    for(const Edge &e : edges) {
        adjacency_list[e.u].push_back({ e.v, e.weight });
        adjacency_list[e.v].push_back({ e.u, e.weight });
    }

    heap<HalfEdge, Compare> pq;
    std::vector<bool> visited(n, false);
    std::vector<long long> dist(n, LLONG_MAX);

    dist[start] = 0;
    pq.push({ start, 0 });
    while(!pq.empty()) {
        HalfEdge cur = pq.top(); pq.pop();
        if (cur.node == end) return cur.weight;

        if (visited[cur.node]) continue;
        visited[cur.node] = true;

        for (const HalfEdge &e : adjacency_list[cur.node]) {
            long long new_dist = dist[cur.node] + e.weight;
            if (new_dist < dist[e.node]) {
                dist[e.node] = new_dist;
                pq.push({ e.node, new_dist });
            }
        }
    }

    return std::nullopt;
}