#include "mst.hpp"

#include <functional>
#include <queue>
#include <tuple>

MSTResult prim_mst(const CSRGraph& csr, int start_vertex)
{
    MSTResult result;
    result.vertices = csr.vertices;

    if (csr.vertices == 0)
    {
        result.connected = true;
        return result;
    }

    if (start_vertex < 0 || start_vertex >= csr.vertices)
    {
        start_vertex = 0;
    }

    using QueueEntry = tuple<int, int, int>; // weight, vertex, parent
    priority_queue<QueueEntry,
                        vector<QueueEntry>,
                        greater<QueueEntry>> pq;

    vector<bool> in_tree(static_cast<size_t>(csr.vertices), false);
    pq.emplace(0, start_vertex, -1);

    while (!pq.empty())
    {
        const auto [weight, u, parent] = pq.top();
        pq.pop();

        if (in_tree[static_cast<size_t>(u)])
        {
            continue;
        }

        in_tree[static_cast<size_t>(u)] = true;

        if (parent != -1)
        {
            result.edges.push_back({parent, u, weight});
            result.total_weight += weight;
        }

        for (int e = csr.row_ptr[static_cast<size_t>(u)];
             e < csr.row_ptr[static_cast<size_t>(u + 1)];
             ++e)
        {
            const int v = csr.col_index[static_cast<size_t>(e)];
            const int w = csr.values[static_cast<size_t>(e)];

            if (!in_tree[static_cast<size_t>(v)])
            {
                pq.emplace(w, v, u);
            }
        }
    }

    result.connected = static_cast<int>(result.edges.size()) == csr.vertices - 1;
    return result;
}
