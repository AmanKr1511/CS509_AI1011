#include "bellman_ford.hpp"

#include <iomanip>
#include <iostream>

BellmanFordResult bellman_ford(const CSRGraph& csr, int source)
{
    BellmanFordResult res;
    res.vertices = csr.vertices;
    res.source = source;
    res.dist.assign(static_cast<size_t>(csr.vertices), INF_WEIGHT);
    res.dist[static_cast<size_t>(source)] = 0;

    for (int iter = 0; iter < csr.vertices - 1; ++iter)
    {
        bool changed = false;

        for (int u = 0; u < csr.vertices; ++u)
        {
            if (res.dist[static_cast<size_t>(u)] == INF_WEIGHT)
            {
                continue;
            }

            for (int e = csr.row_ptr[static_cast<size_t>(u)];
                 e < csr.row_ptr[static_cast<size_t>(u + 1)];
                 ++e)
            {
                const int v = csr.col_index[static_cast<size_t>(e)];
                const int w = csr.values[static_cast<size_t>(e)];

                if (res.dist[static_cast<size_t>(u)] + w <
                    res.dist[static_cast<size_t>(v)])
                {
                    res.dist[static_cast<size_t>(v)] =
                        res.dist[static_cast<size_t>(u)] + w;
                    changed = true;
                }
            }
        }

        if (!changed)
        {
            break;
        }
    }

    for (int u = 0; u < csr.vertices && !res.negative_cycle; ++u)
    {
        if (res.dist[static_cast<size_t>(u)] == INF_WEIGHT)
        {
            continue;
        }

        for (int e = csr.row_ptr[static_cast<size_t>(u)];
             e < csr.row_ptr[static_cast<size_t>(u + 1)];
             ++e)
        {
            const int v = csr.col_index[static_cast<size_t>(e)];
            const int w = csr.values[static_cast<size_t>(e)];

            if (res.dist[static_cast<size_t>(u)] + w <
                res.dist[static_cast<size_t>(v)])
            {
                res.negative_cycle = true;
                break;
            }
        }
    }

    return res;
}

void print_bellman_ford_result(const BellmanFordResult& res, double exec_time_ms)
{
    cout << "Algorithm: Bellman-Ford\n";
    cout << "Source: " << res.source << '\n';

    if (res.negative_cycle)
    {
        cout << "Negative cycle: true\n";
    }
    else
    {
        cout << "Vertex Distance\n";

        for (int i = 0; i < res.vertices; ++i)
        {
            cout << left << setw(6) << i;
            if (res.dist[static_cast<size_t>(i)] == INF_WEIGHT)
            {
                cout << " INF\n";
            }
            else
            {
                cout << ' ' << res.dist[static_cast<size_t>(i)] << '\n';
            }
        }

        cout << "Negative cycle: none\n";
    }

    cout.setf(ios::fixed);
    cout.precision(3);
    cout << "Execution time: " << exec_time_ms << " ms\n";
}

void free_bellman_ford_result(BellmanFordResult& res)
{
    res.dist.clear();
    res.vertices = 0;
}
