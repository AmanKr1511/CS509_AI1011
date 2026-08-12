#include "bellman_ford.hpp"

#include <iomanip>
#include <iostream>

BellmanFordResult bellman_ford(const CSRGraph& csr, int source)
{
    BellmanFordResult res;
    res.vertices = csr.vertices;
    res.source = source;
    res.dist.assign(static_cast<std::size_t>(csr.vertices), INF_WEIGHT);
    res.dist[static_cast<std::size_t>(source)] = 0;

    for (int iter = 0; iter < csr.vertices - 1; ++iter)
    {
        bool changed = false;

        for (int u = 0; u < csr.vertices; ++u)
        {
            if (res.dist[static_cast<std::size_t>(u)] == INF_WEIGHT)
            {
                continue;
            }

            for (int e = csr.row_ptr[static_cast<std::size_t>(u)];
                 e < csr.row_ptr[static_cast<std::size_t>(u + 1)];
                 ++e)
            {
                const int v = csr.col_index[static_cast<std::size_t>(e)];
                const int w = csr.values[static_cast<std::size_t>(e)];

                if (res.dist[static_cast<std::size_t>(u)] + w <
                    res.dist[static_cast<std::size_t>(v)])
                {
                    res.dist[static_cast<std::size_t>(v)] =
                        res.dist[static_cast<std::size_t>(u)] + w;
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
        if (res.dist[static_cast<std::size_t>(u)] == INF_WEIGHT)
        {
            continue;
        }

        for (int e = csr.row_ptr[static_cast<std::size_t>(u)];
             e < csr.row_ptr[static_cast<std::size_t>(u + 1)];
             ++e)
        {
            const int v = csr.col_index[static_cast<std::size_t>(e)];
            const int w = csr.values[static_cast<std::size_t>(e)];

            if (res.dist[static_cast<std::size_t>(u)] + w <
                res.dist[static_cast<std::size_t>(v)])
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
    std::cout << "Algorithm: Bellman-Ford\n";
    std::cout << "Source: " << res.source << '\n';

    if (res.negative_cycle)
    {
        std::cout << "Negative cycle: true\n";
    }
    else
    {
        std::cout << "Vertex Distance\n";

        for (int i = 0; i < res.vertices; ++i)
        {
            std::cout << std::left << std::setw(6) << i;
            if (res.dist[static_cast<std::size_t>(i)] == INF_WEIGHT)
            {
                std::cout << " INF\n";
            }
            else
            {
                std::cout << ' ' << res.dist[static_cast<std::size_t>(i)] << '\n';
            }
        }

        std::cout << "Negative cycle: none\n";
    }

    std::cout.setf(std::ios::fixed);
    std::cout.precision(3);
    std::cout << "Execution time: " << exec_time_ms << " ms\n";
}

void free_bellman_ford_result(BellmanFordResult& res)
{
    res.dist.clear();
    res.vertices = 0;
}
