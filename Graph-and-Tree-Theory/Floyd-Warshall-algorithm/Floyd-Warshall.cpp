#pragma once

#include <cstddef>
#include <vector>
#include <optional>


// u <-> v
struct Edge { size_t u, v; long long weight; };

// n is the number of nodes
std::optional<std::vector<std::vector<long long>>> FloydWarshall(size_t n, std::vector<Edge> edges) {
    constexpr long long INF = 1e18;
    std::vector<std::vector<long long>> dist(n, std::vector<long long>(n, INF))
    for(size_t i = 0; i < n; ++i) dist[i][i] = 0;
    for (const Edge &e : edges) {
        dist[e.u][e.v] = std::min(dist[e.u][e.v], e.weight);
        dist[e.v][e.u] = std::min(dist[e.v][e.u], e.weight);
    }

    for(size_t k = 0; k < n; ++k) {
        for(size_t i = 0; i < n; ++i) {
            if (dist[i][k] == INF) continue
            for(size_t j = 0; j < n; ++j) {
                if (dist[k][j] == INF) continue;
                dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    for(size_t i = 0; i < n; ++i) {
        if (dist[i][i] < 0) return std::nullopt;
    }

    return std::make_optional(dist);
}