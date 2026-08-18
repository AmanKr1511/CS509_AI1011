#include "graph.hpp"

#include <iostream>

CSRGraph adjlist_to_csr(const AdjList& adj)
{
    CSRGraph csr;
    csr.vertices = adj.vertices;

    int count = 0;
    for (int degree : adj.degree)
    {
        count += degree;
    }
    csr.nnz = count;

    if (count != adj.edges)
    {
        cerr << "Warning: declared edge count (" << adj.edges
                  << ") does not match sum of vertex degrees (" << count << ")\n";
    }

    csr.row_ptr.resize(static_cast<size_t>(adj.vertices + 1));
    csr.col_index.resize(static_cast<size_t>(csr.nnz));
    csr.values.resize(static_cast<size_t>(csr.nnz));

    count = 0;
    for (int i = 0; i < adj.vertices; ++i)
    {
        csr.row_ptr[static_cast<size_t>(i)] = count;

        for (const AdjEdge& edge : adj.neighbours[static_cast<size_t>(i)])
        {
            csr.col_index[static_cast<size_t>(count)] = edge.vertex;
            csr.values[static_cast<size_t>(count)] = edge.weight;
            ++count;
        }
    }

    csr.row_ptr[static_cast<size_t>(adj.vertices)] = count;
    return csr;
}

void print_csr_graph(const CSRGraph& csr)
{
    cout << "\nCSR Graph Representation\n";
    cout << "-----------------------------\n";
    cout << "Vertices: " << csr.vertices << "   Edges (nnz): " << csr.nnz << "\n";

    cout << "\nValues:\n";
    for (int value : csr.values)
    {
        cout << value << ' ';
    }
    cout << '\n';

    cout << "\nColumn Index:\n";
    for (int index : csr.col_index)
    {
        cout << index << ' ';
    }
    cout << '\n';

    cout << "\nRow Pointer:\n";
    for (int value : csr.row_ptr)
    {
        cout << value << ' ';
    }
    cout << '\n';
}

void free_csr_graph(CSRGraph& csr)
{
    csr.values.clear();
    csr.col_index.clear();
    csr.row_ptr.clear();
    csr.vertices = 0;
    csr.nnz = 0;
}
