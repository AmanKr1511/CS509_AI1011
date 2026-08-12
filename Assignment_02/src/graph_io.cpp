#define _POSIX_C_SOURCE 199309L

#include "graph.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <time.h>

AdjList read_bf_adjlist(const std::string& filename, int& source)
{
    std::ifstream input(filename);
    if (!input)
    {
        throw std::runtime_error("Error: could not open input file '" + filename + "'");
    }

    AdjList adj;
    if (!(input >> adj.vertices >> adj.edges) || adj.vertices <= 0)
    {
        throw std::runtime_error("Error: invalid input file '" + filename + "' (expected 'V E' on line 1)");
    }

    adj.degree.assign(static_cast<std::size_t>(adj.vertices), 0);
    adj.neighbours.resize(static_cast<std::size_t>(adj.vertices));

    for (int i = 0; i < adj.vertices; ++i)
    {
        int u;
        int degree;

        if (!(input >> u >> degree))
        {
            throw std::runtime_error("Error: malformed adjacency line " + std::to_string(i) + " in '" + filename + "'");
        }

        if (u < 0 || u >= adj.vertices || degree < 0)
        {
            throw std::runtime_error("Error: invalid vertex/degree in '" + filename + "'");
        }

        adj.degree[static_cast<std::size_t>(u)] = degree;
        auto& neighbours = adj.neighbours[static_cast<std::size_t>(u)];
        neighbours.resize(static_cast<std::size_t>(degree));

        for (int j = 0; j < degree; ++j)
        {
            if (!(input >> neighbours[static_cast<std::size_t>(j)].vertex
                        >> neighbours[static_cast<std::size_t>(j)].weight))
            {
                throw std::runtime_error("Error: malformed edge for vertex " +
                                         std::to_string(u) + " in '" + filename + "'");
            }
        }
    }

    std::string tag;
    if (!(input >> tag >> source) || tag != "SOURCE")
    {
        throw std::runtime_error("Error: expected 'SOURCE s' at end of '" + filename + "'");
    }

    if (source < 0 || source >= adj.vertices)
    {
        throw std::runtime_error("Error: source vertex " + std::to_string(source) +
                                 " out of range in '" + filename + "'");
    }

    return adj;
}

void free_adjlist(AdjList& adj)
{
    adj.neighbours.clear();
    adj.degree.clear();
    adj.vertices = 0;
    adj.edges = 0;
}

DenseGraph read_fw_matrix(const std::string& filename)
{
    std::ifstream input(filename);
    if (!input)
    {
        throw std::runtime_error("Error: could not open input file '" + filename + "'");
    }

    DenseGraph g;
    if (!(input >> g.vertices) || g.vertices <= 0)
    {
        throw std::runtime_error("Error: invalid input file '" + filename + "' (expected 'V' on line 1)");
    }

    g.dist.assign(static_cast<std::size_t>(g.vertices),
                  std::vector<int>(static_cast<std::size_t>(g.vertices)));

    for (int i = 0; i < g.vertices; ++i)
    {
        for (int j = 0; j < g.vertices; ++j)
        {
            std::string token;
            if (!(input >> token))
            {
                throw std::runtime_error("Error: malformed matrix row " +
                                         std::to_string(i) + " in '" + filename + "'");
            }

            if (token == "INF")
            {
                g.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] = INF_WEIGHT;
            }
            else
            {
                try
                {
                    g.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] = std::stoi(token);
                }
                catch (const std::exception&)
                {
                    throw std::runtime_error("Error: invalid matrix value '" + token +
                                             "' in '" + filename + "'");
                }
            }
        }
    }

    for (int i = 0; i < g.vertices; ++i)
    {
        if (g.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(i)] != 0)
        {
            std::cerr << "Warning: diagonal entry (" << i << "," << i << ") in '"
                      << filename << "' is "
                      << g.dist[static_cast<std::size_t>(i)][static_cast<std::size_t>(i)]
                      << ", expected 0\n";
        }
    }

    return g;
}

void free_dense_graph(DenseGraph& g)
{
    g.dist.clear();
    g.vertices = 0;
}

double get_time_ms()
{
    timespec ts{};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<double>(ts.tv_sec) * 1000.0 +
           static_cast<double>(ts.tv_nsec) / 1000000.0;
}
