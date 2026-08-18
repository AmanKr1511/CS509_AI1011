#ifndef BELLMAN_FORD_HPP
#define BELLMAN_FORD_HPP

#include "graph.hpp"

struct BellmanFordResult
{
    int vertices{};
    int source{};
    vector<int> dist;
    bool negative_cycle{false};
};

BellmanFordResult bellman_ford(const CSRGraph& csr, int source);
void print_bellman_ford_result(const BellmanFordResult& res, double exec_time_ms);
void free_bellman_ford_result(BellmanFordResult& res);

#endif