#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <climits>
#include <cstddef>
#include <string>
#include <vector>

constexpr int INF_WEIGHT = INT_MAX / 2;

struct AdjEdge
{
    int vertex{};
    int weight{};
};

struct AdjList
{
    int vertices{};
    int edges{};
    std::vector<int> degree;
    std::vector<std::vector<AdjEdge>> neighbours;
};

struct CSRGraph
{
    int vertices{};
    int nnz{};
    std::vector<int> row_ptr;
    std::vector<int> col_index;
    std::vector<int> values;
};

struct DenseGraph
{
    int vertices{};
    std::vector<std::vector<int>> dist;
};

AdjList read_bf_adjlist(const std::string& filename, int& source);
void free_adjlist(AdjList& adj);

CSRGraph adjlist_to_csr(const AdjList& adj);
void print_csr_graph(const CSRGraph& csr);
void free_csr_graph(CSRGraph& csr);

DenseGraph read_fw_matrix(const std::string& filename);
void free_dense_graph(DenseGraph& g);

double get_time_ms();

#endif
