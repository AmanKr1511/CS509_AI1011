#include "floyd_warshall.hpp"

#include <iostream>

FloydWarshallResult floyd_warshall(const DenseGraph& g)
{
    FloydWarshallResult res;
    res.vertices = g.vertices;
    res.dist = g.dist;

    for (int k = 0; k < g.vertices; ++k)
    {
        for (int i = 0; i < g.vertices; ++i)
        {
            if (res.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(k)] == INF_WEIGHT)
            {
                continue;
            }

            for (int j = 0; j < g.vertices; ++j)
            {
                if (res.dist[static_cast<std::size_t>(k)][static_cast<std::size_t>(j)] == INF_WEIGHT)
                {
                    continue;
                }

                const int through_k =
                    res.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(k)] +
                    res.dist[static_cast<std::size_t>(k)][static_cast<std::size_t>(j)];

                if (through_k < res.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)])
                {
                    res.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] = through_k;
                }
            }
        }
    }

    for (int i = 0; i < g.vertices; ++i)
    {
        if (res.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(i)] < 0)
        {
            res.negative_cycle = true;
            break;
        }
    }

    return res;
}

void print_floyd_warshall_result(const FloydWarshallResult& res, double exec_time_ms)
{
    std::cout << "Algorithm: Floyd-Warshall\n";

    if (res.negative_cycle)
    {
        std::cout << "Negative cycle: true\n";
    }
    else
    {
        std::cout << "Distance matrix:\n";

        for (int i = 0; i < res.vertices; ++i)
        {
            for (int j = 0; j < res.vertices; ++j)
            {
                const int value = res.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)];
                if (value == INF_WEIGHT)
                {
                    std::cout << "INF ";
                }
                else
                {
                    std::cout << value << ' ';
                }
            }
            std::cout << '\n';
        }

        std::cout << "Negative cycle: none\n";
    }

    std::cout.setf(std::ios::fixed);
    std::cout.precision(3);
    std::cout << "Execution time: " << exec_time_ms << " ms\n";
}

void free_floyd_warshall_result(FloydWarshallResult& res)
{
    res.dist.clear();
    res.vertices = 0;
}
