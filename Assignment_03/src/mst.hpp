#ifndef MST_HPP
#define MST_HPP

#include "graph.hpp"
#include <utility>
#include <vector>

using namespace std;

struct MSTEdge
{
    int u{};
    int v{};
    int weight{};
};

struct MSTResult
{
    int vertices{};
    vector<MSTEdge> edges;
    long long total_weight{};
    bool connected{false};
};

MSTResult kruskal_mst(const CSRGraph& csr);
MSTResult prim_mst(const CSRGraph& csr, int start_vertex = 0);

void print_mst_result(const char* algorithm,
                      const MSTResult& result,
                      double execution_time_ms);

#endif