#include "graph.hpp"
#include "mst.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

constexpr const char* TEST_DIR = "tests/";

typedef MSTResult (*MSTAlgorithm)(const CSRGraph&);

MSTResult run_prim_default(const CSRGraph& csr)
{
    return prim_mst(csr, 0);
}

string make_test_path(const string& filename)
{
    if (filename.rfind(TEST_DIR, 0) == 0)
    {
        return filename;
    }
    return string(TEST_DIR) + filename;
}

AdjList read_mst_adjlist(const string& filename)
{
    ifstream input(filename);
    if (!input)
    {
        throw runtime_error("Error: could not open input file '" + filename + "'");
    }

    AdjList adj;
    if (!(input >> adj.vertices >> adj.edges) || adj.vertices <= 0 || adj.edges < 0)
    {
        throw runtime_error("Error: invalid input file '" + filename + "' (expected 'V E' on line 1)");
    }

    adj.degree.assign(static_cast<size_t>(adj.vertices), 0);
    adj.neighbours.resize(static_cast<size_t>(adj.vertices));

    for (int i = 0; i < adj.vertices; ++i)
    {
        int u = -1;
        int degree = -1;

        if (!(input >> u >> degree) || u < 0 || u >= adj.vertices || degree < 0)
        {
            throw runtime_error("Error: malformed adjacency line " +
                                     to_string(i) + " in '" + filename + "'");
        }

        auto& neighbours = adj.neighbours[static_cast<size_t>(u)];
        neighbours.resize(static_cast<size_t>(degree));
        adj.degree[static_cast<size_t>(u)] = degree;

        for (int j = 0; j < degree; ++j)
        {
            int v = -1;
            int w = 0;
            if (!(input >> v >> w) || v < 0 || v >= adj.vertices)
            {
                throw runtime_error("Error: invalid edge for vertex " +
                                         to_string(u) + " in '" + filename + "'");
            }
            neighbours[static_cast<size_t>(j)] = {v, w};
        }
    }

    return adj;
}

void validate_mst_graph(const AdjList& adj, const string& filename)
{
    if (adj.vertices > 1 && adj.edges == 0)
    {
        throw runtime_error("Error: MST input '" + filename + "' has no edges");
    }

    long long degree_sum = 0;
    for (int d : adj.degree)
    {
        degree_sum += d;
    }

    if (degree_sum != 2LL * adj.edges)
    {
        throw runtime_error("Error: MST input '" + filename +
                                 "' is inconsistent: sum of degrees must equal 2E");
    }

    for (int u = 0; u < adj.vertices; ++u)
    {
        for (const AdjEdge& edge : adj.neighbours[static_cast<size_t>(u)])
        {
            bool reverse_found = false;
            for (const AdjEdge& reverse :
                 adj.neighbours[static_cast<size_t>(edge.vertex)])
            {
                if (reverse.vertex == u && reverse.weight == edge.weight)
                {
                    reverse_found = true;
                    break;
                }
            }

            if (!reverse_found)
            {
                throw runtime_error("Error: MST input '" + filename +
                                         "' is not an undirected weighted graph");
            }
        }
    }
}

double run_algorithm(const char* name,
                            MSTAlgorithm algorithm,
                            const CSRGraph& csr,
                            MSTResult& result)
{
    const double t0 = get_time_ms();
    result = algorithm(csr);
    const double t1 = get_time_ms();

    cout << "\nAlgorithm: " << name << "'s MST\n";
    if (!result.connected)
    {
        cout << "Status: Graph is disconnected; MST does not exist.\n";
    }
    else
    {
        cout << "MST edges:\n";
        for (const MSTEdge& edge : result.edges)
        {
            cout << edge.u << ' ' << edge.v << ' ' << edge.weight << '\n';
        }
        cout << "Total MST weight: " << result.total_weight << '\n';
    }

    const double elapsed = t1 - t0;
    cout.setf(ios::fixed);
    cout.precision(3);
    cout << "Execution time: " << elapsed << " ms\n";
    return elapsed;
}

void run_mst(const string& filename, int choice)
{
    const string filepath = make_test_path(filename);
    AdjList adj = read_mst_adjlist(filepath);
    validate_mst_graph(adj, filepath);

    CSRGraph csr = adjlist_to_csr(adj);

    MSTResult kruskal_result;
    MSTResult prim_result;
    double kruskal_time = 0.0;
    double prim_time = 0.0;

    if (choice == 1 || choice == 3)
    {
        kruskal_time = run_algorithm("Kruskal", kruskal_mst, csr, kruskal_result);
    }

    if (choice == 2 || choice == 3)
    {
        prim_time = run_algorithm("Prim", run_prim_default, csr, prim_result);
    }

    if (choice == 3 && kruskal_result.connected && prim_result.connected)
    {
        cout << "\nMST weight comparison: "
                  << (kruskal_result.total_weight == prim_result.total_weight
                          ? "PASS (same total MST weight)"
                          : "FAIL (different total MST weights)")
                  << '\n';
        (void)kruskal_time;
        (void)prim_time;
    }

    free_csr_graph(csr);
    free_adjlist(adj);
}

void print_usage(const char* prog)
{
    cerr << "Usage: " << prog << " <kruskal|prim|both> <input_file>\n";
    cerr << "       " << prog << "                    (interactive menu)\n";
}

void interactive_menu()
{
    while (true)
    {
        cout << "\n======================================\n"
                  << "        CS509 - Assignment 3\n"
                  << "======================================\n"
                  << "  1. Kruskal's MST\n"
                  << "  2. Prim's MST\n"
                  << "  3. Run both (compare)\n"
                  << "  4. Exit\n"
                  << "Select an option: ";

        int choice = 0;
        if (!(cin >> choice))
        {
            cerr << "Error: invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 4)
        {
            cout << "Exiting ....\n";
            return;
        }

        if (choice < 1 || choice > 3)
        {
            cerr << "Error: invalid selection.\n";
            continue;
        }

        string filename;
        cout << "Enter MST input file name (from tests/) : ";
        if (!(cin >> filename))
        {
            cerr << "Error: invalid file name.\n";
            return;
        }

        run_mst(filename, choice);
    }
}

int main(int argc, char** argv)
{
    try
    {
        if (argc == 1)
        {
            interactive_menu();
            return 0;
        }

        if (argc != 3)
        {
            print_usage(argv[0]);
            return 1;
        }

        int choice = 0;
        const string algorithm = argv[1];

        if (algorithm == "kruskal")
        {
            choice = 1;
        }
        else if (algorithm == "prim")
        {
            choice = 2;
        }
        else if (algorithm == "both")
        {
            choice = 3;
        }
        else
        {
            cerr << "Error: unknown algorithm '" << algorithm
                      << "' (expected 'kruskal', 'prim', or 'both')\n";
            print_usage(argv[0]);
            return 1;
        }

        run_mst(argv[2], choice);
    }
    catch (const exception& ex)
    {
        cerr << ex.what() << '\n';
        return 1;
    }

    return 0;
}
