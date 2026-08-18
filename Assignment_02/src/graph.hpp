#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <climits>
#include <cstddef>
#include <string>
#include <vector>

using namespace std;

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
    vector<int> degree;
    vector<vector<AdjEdge>> neighbours;
};

struct CSRGraph
{
    int vertices{};
    int nnz{};
    vector<int> row_ptr;
    vector<int> col_index;
    vector<int> values;
};

struct DenseGraph
{
    int vertices{};
    vector<vector<int>> dist;
};

AdjList read_bf_adjlist(const string& filename, int& source);
void free_adjlist(AdjList& adj);

CSRGraph adjlist_to_csr(const AdjList& adj);
void print_csr_graph(const CSRGraph& csr);
void free_csr_graph(CSRGraph& csr);

DenseGraph read_fw_matrix(const string& filename);
void free_dense_graph(DenseGraph& g);

double get_time_ms();

#endif
