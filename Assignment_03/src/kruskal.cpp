#include "mst.hpp"

#include <algorithm>
#include <numeric>

class DisjointSet
{
public:
    explicit DisjointSet(int n) : parent_(static_cast<size_t>(n)), rank_(static_cast<size_t>(n), 0)
    {
        iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int x)
    {
        if (parent_[static_cast<size_t>(x)] != x)
        {
            parent_[static_cast<size_t>(x)] = find(parent_[static_cast<size_t>(x)]);
        }
        return parent_[static_cast<size_t>(x)];
    }

    bool unite(int a, int b)
    {
        a = find(a);
        b = find(b);

        if (a == b)
        {
            return false;
        }

        if (rank_[static_cast<size_t>(a)] < rank_[static_cast<size_t>(b)])
        {
            swap(a, b);
        }

        parent_[static_cast<size_t>(b)] = a;
        if (rank_[static_cast<size_t>(a)] == rank_[static_cast<size_t>(b)])
        {
            ++rank_[static_cast<size_t>(a)];
        }

        return true;
    }

private:
    vector<int> parent_;
    vector<int> rank_;
};

MSTResult kruskal_mst(const CSRGraph& csr)
{
    MSTResult result;
    result.vertices = csr.vertices;

    vector<MSTEdge> edges;
    edges.reserve(static_cast<size_t>(csr.nnz / 2));

    for (int u = 0; u < csr.vertices; ++u)
    {
        for (int e = csr.row_ptr[static_cast<size_t>(u)];
             e < csr.row_ptr[static_cast<size_t>(u + 1)];
             ++e)
        {
            const int v = csr.col_index[static_cast<size_t>(e)];
            const int w = csr.values[static_cast<size_t>(e)];

            if (u < v)
            {
                edges.push_back({u, v, w});
            }
        }
    }

    sort(edges.begin(), edges.end(), [](const MSTEdge& a, const MSTEdge& b) {
        if (a.weight != b.weight)
        {
            return a.weight < b.weight;
        }
        if (a.u != b.u)
        {
            return a.u < b.u;
        }
        return a.v < b.v;
    });

    DisjointSet dsu(csr.vertices);

    for (const MSTEdge& edge : edges)
    {
        if (dsu.unite(edge.u, edge.v))
        {
            result.edges.push_back(edge);
            result.total_weight += edge.weight;

            if (static_cast<int>(result.edges.size()) == csr.vertices - 1)
            {
                break;
            }
        }
    }

    result.connected = (csr.vertices <= 1) ||
                       static_cast<int>(result.edges.size()) == csr.vertices - 1;
    return result;
}
