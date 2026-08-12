#include "graph.hpp"
#include "bellman_ford.hpp"
#include "floyd_warshall.hpp"

#include <cstring>
#include <exception>
#include <iostream>
#include <string>

constexpr const char* TEST_DIR = "tests/";

static std::string make_test_path(const std::string& filename)
{
    if (filename.rfind(TEST_DIR, 0) == 0)
    {
        return filename;
    }

    return std::string(TEST_DIR) + filename;
}

static void print_usage(const char* prog)
{
    std::cerr << "Usage: " << prog << " <bf|fw> <input_file>\n";
    std::cerr << "       " << prog << "                    (interactive menu)\n";
}

static void run_bellman_ford(const std::string& filename)
{
    const std::string filepath = make_test_path(filename);

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

static void run_floyd_warshall(const std::string& filename)
{
    const std::string filepath = make_test_path(filename);

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
        std::cout << "\n======================================\n";
        std::cout << "        CS509 - Assignment 2\n";
        std::cout << "======================================\n";
        std::cout << "  1. Bellman-Ford\n";
        std::cout << "  2. Floyd-Warshall\n";
        std::cout << "  3. Exit\n";
        std::cout << "Select an option: ";

        int choice = 0;
        if (!(std::cin >> choice))
        {
            std::cerr << "Error: invalid input.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 3)
        {
            std::cout << "Exiting ....\n";
            break;
        }

        std::string filename;
        switch (choice)
        {
            case 1:
                std::cout << "Enter input file name (from tests/) : ";
                if (!(std::cin >> filename))
                {
                    std::cerr << "Error: invalid file name.\n";
                    return;
                }
                run_bellman_ford(filename);
                break;

            case 2:
                std::cout << "Enter input file name (from tests/) : ";
                if (!(std::cin >> filename))
                {
                    std::cerr << "Error: invalid file name.\n";
                    return;
                }
                run_floyd_warshall(filename);
                break;

            default:
                std::cerr << "Error: invalid selection '" << choice
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

        if (std::strcmp(argv[1], "bf") == 0)
        {
            run_bellman_ford(argv[2]);
        }
        else if (std::strcmp(argv[1], "fw") == 0)
        {
            run_floyd_warshall(argv[2]);
        }
        else
        {
            std::cerr << "Error: unknown algorithm '" << argv[1]
                      << "' (expected 'bf' or 'fw')\n";
            print_usage(argv[0]);
            return 1;
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return 1;
    }

    return 0;
}
