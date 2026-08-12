#ifndef FLOYD_WARSHALL_HPP
#define FLOYD_WARSHALL_HPP

#include "graph.hpp"

struct FloydWarshallResult
{
    int vertices{};
    std::vector<std::vector<int>> dist;
    bool negative_cycle{false};
};

FloydWarshallResult floyd_warshall(const DenseGraph& g);
void print_floyd_warshall_result(const FloydWarshallResult& res, double exec_time_ms);
void free_floyd_warshall_result(FloydWarshallResult& res);

#endif
