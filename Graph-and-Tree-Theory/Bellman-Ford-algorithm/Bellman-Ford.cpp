#pragma once

#include <cstddef>
#include <vector>
#include <optional>


// u <-> v
struct Edge { size_t u, v; long long weight; };

// n is the number of nodes
std::optional<long long> BellmanFord(size_t n, size_t start, size_t end, std::vector<Edge> edges) {
    constexpr long long INF = 1e18;
    std::vector<long long> dist(n, INF);
    dist[start] = 0;

    bool updated;
    for(size_t i = 0; i < n - 1; ++i) {
        updated = false;
        for(const Edge &e : edges) {
            if (dist[e.u] != INF && dist[e.u] + e.weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.weight;
                updated = true;
            }
        }
        if (!updated) break;
    }

    for(const Edge &e : edges) {
        if (dist[e.u] != INF && dist[e.u] + e.weight < dist[e.v]) {
            return std::nullopt;
        }
    }

    return (dist[end] == INF) ? std::nullopt : std::make_optional(dist[end]);
}