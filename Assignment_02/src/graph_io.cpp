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

AdjList read_bf_adjlist(const string& filename, int& source)
{
    ifstream input(filename);
    if (!input)
    {
        throw runtime_error("Error: could not open input file '" + filename + "'");
    }

    AdjList adj;
    if (!(input >> adj.vertices >> adj.edges) || adj.vertices <= 0)
    {
        throw runtime_error("Error: invalid input file '" + filename + "' (expected 'V E' on line 1)");
    }

    adj.degree.assign(static_cast<size_t>(adj.vertices), 0);
    adj.neighbours.resize(static_cast<size_t>(adj.vertices));

    for (int i = 0; i < adj.vertices; ++i)
    {
        int u;
        int degree;

        if (!(input >> u >> degree))
        {
            throw runtime_error("Error: malformed adjacency line " + to_string(i) + " in '" + filename + "'");
        }

        if (u < 0 || u >= adj.vertices || degree < 0)
        {
            throw runtime_error("Error: invalid vertex/degree in '" + filename + "'");
        }

        adj.degree[static_cast<size_t>(u)] = degree;
        auto& neighbours = adj.neighbours[static_cast<size_t>(u)];
        neighbours.resize(static_cast<size_t>(degree));

        for (int j = 0; j < degree; ++j)
        {
            if (!(input >> neighbours[static_cast<size_t>(j)].vertex
                        >> neighbours[static_cast<size_t>(j)].weight))
            {
                throw runtime_error("Error: malformed edge for vertex " +
                                         to_string(u) + " in '" + filename + "'");
            }
        }
    }

    string tag;
    if (!(input >> tag >> source) || tag != "SOURCE")
    {
        throw runtime_error("Error: expected 'SOURCE s' at end of '" + filename + "'");
    }

    if (source < 0 || source >= adj.vertices)
    {
        throw runtime_error("Error: source vertex " + to_string(source) +
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

DenseGraph read_fw_matrix(const string& filename)
{
    ifstream input(filename);
    if (!input)
    {
        throw runtime_error("Error: could not open input file '" + filename + "'");
    }

    DenseGraph g;
    if (!(input >> g.vertices) || g.vertices <= 0)
    {
        throw runtime_error("Error: invalid input file '" + filename + "' (expected 'V' on line 1)");
    }

    g.dist.assign(static_cast<size_t>(g.vertices),
                  vector<int>(static_cast<size_t>(g.vertices)));

    for (int i = 0; i < g.vertices; ++i)
    {
        for (int j = 0; j < g.vertices; ++j)
        {
            string token;
            if (!(input >> token))
            {
                throw runtime_error("Error: malformed matrix row " +
                                         to_string(i) + " in '" + filename + "'");
            }

            if (token == "INF")
            {
                g.dist[static_cast<size_t>(i)][static_cast<size_t>(j)] = INF_WEIGHT;
            }
            else
            {
                try
                {
                    g.dist[static_cast<size_t>(i)][static_cast<size_t>(j)] = stoi(token);
                }
                catch (const exception&)
                {
                    throw runtime_error("Error: invalid matrix value '" + token +
                                             "' in '" + filename + "'");
                }
            }
        }
    }

    for (int i = 0; i < g.vertices; ++i)
    {
        if (g.dist[static_cast<size_t>(i)][static_cast<size_t>(i)] != 0)
        {
            cerr << "Warning: diagonal entry (" << i << "," << i << ") in '"
                      << filename << "' is "
                      << g.dist[static_cast<size_t>(i)][static_cast<size_t>(i)]
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
