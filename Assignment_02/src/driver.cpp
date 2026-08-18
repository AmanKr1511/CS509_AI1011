#include "graph.hpp"
#include "bellman_ford.hpp"
#include "floyd_warshall.hpp"

#include <cstring>
#include <exception>
#include <iostream>
#include <string>

constexpr const char* TEST_DIR = "tests/";

static string make_test_path(const string& filename)
{
    if (filename.rfind(TEST_DIR, 0) == 0)
    {
        return filename;
    }

    return string(TEST_DIR) + filename;
}

static void print_usage(const char* prog)
{
    cerr << "Usage: " << prog << " <bf|fw> <input_file>\n";
    cerr << "       " << prog << "                    (interactive menu)\n";
}

static void run_bellman_ford(const string& filename)
{
    const string filepath = make_test_path(filename);

    int source = 0;
    AdjList adj = read_bf_adjlist(filepath, source);
    CSRGraph csr = adjlist_to_csr(adj);

    const double t0 = get_time_ms();
    BellmanFordResult res = bellman_ford(csr, source);
    const double t1 = get_time_ms();

    print_bellman_ford_result(res, t1 - t0);

    free_bellman_ford_result(res);
    free_csr_graph(csr);
    free_adjlist(adj);
}

static void run_floyd_warshall(const string& filename)
{
    const string filepath = make_test_path(filename);

    DenseGraph g = read_fw_matrix(filepath);

    const double t0 = get_time_ms();
    FloydWarshallResult res = floyd_warshall(g);
    const double t1 = get_time_ms();

    print_floyd_warshall_result(res, t1 - t0);

    free_floyd_warshall_result(res);
    free_dense_graph(g);
}

static void interactive_menu()
{
    while (true)
    {
        cout << "\n======================================\n";
        cout << "        CS509 - Assignment 2\n";
        cout << "======================================\n";
        cout << "  1. Bellman-Ford\n";
        cout << "  2. Floyd-Warshall\n";
        cout << "  3. Exit\n";
        cout << "Select an option: ";

        int choice = 0;
        if (!(cin >> choice))
        {
            cerr << "Error: invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 3)
        {
            cout << "Exiting ....\n";
            break;
        }

        string filename;
        switch (choice)
        {
            case 1:
                cout << "Enter input file name (from tests/) : ";
                if (!(cin >> filename))
                {
                    cerr << "Error: invalid file name.\n";
                    return;
                }
                run_bellman_ford(filename);
                break;

            case 2:
                cout << "Enter input file name (from tests/) : ";
                if (!(cin >> filename))
                {
                    cerr << "Error: invalid file name.\n";
                    return;
                }
                run_floyd_warshall(filename);
                break;

            default:
                cerr << "Error: invalid selection '" << choice
                          << "' (expected 1, 2, or 3)\n";
                break;
        }
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

        if (strcmp(argv[1], "bf") == 0)
        {
            run_bellman_ford(argv[2]);
        }
        else if (strcmp(argv[1], "fw") == 0)
        {
            run_floyd_warshall(argv[2]);
        }
        else
        {
            cerr << "Error: unknown algorithm '" << argv[1]
                      << "' (expected 'bf' or 'fw')\n";
            print_usage(argv[0]);
            return 1;
        }
    }
    catch (const exception& ex)
    {
        cerr << ex.what() << '\n';
        return 1;
    }

    return 0;
}
