#pragma once

#include <cstddef>
#include <vector>
#include "../../Data-Structure/queue/queue.hpp" // Data-Structure/queue/queue.cpp


// u -> v, directed
struct Edge { size_t u, v; };

// n is the numbers of node
std::vector<size_t> KahnsAlgorithm(size_t n, std::vector<Edge> edges) {
    std::vector<size_t> in_degree(n), answer;
    std::vector<std::vector<size_t>> adjacency_list(n);
    queue<size_t> q;
    
    for(const Edge &e : edges) {
        in_degree[e.v]++;
        adjacency_list[e.u].push_back(e.v);
    }

    for(size_t i = 0; i < n; ++i) if (in_degree[i] == 0) q.push(i);

    while(!q.empty()) {
        size_t node = q.front(); q.pop();
        answer.push_back(node);

        for(const size_t &i : adjacency_list[node]) {
            in_degree[i]--;
            if (in_degree[i] == 0) q.push(i);
        }
    }

    return answer;
}