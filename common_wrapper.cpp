#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    std::vector<fs::path> a;

    for (auto& x : fs::directory_iterator("."))
        if (x.is_directory() &&
            x.path().filename().string().find("Assignment_") == 0)
            a.push_back(x.path());

    std::sort(a.begin(), a.end());

    int n = 0;

    if (argc > 1)
    {
        for (int i = 0; i < (int)a.size(); ++i)
            if (a[i].filename() == argv[1])
                n = i + 1;
    }
    else
    {
        std::cout << "\n1. Assignment_01\n"
                  << "2. Assignment_02\n"
                  << "0. Exit\n"
                  << "Choose assignment: ";
        std::cin >> n;
    }

    if (n <= 0 || n > (int)a.size())
        return 0;

    fs::path bin = a[n - 1] / "bin";
    fs::create_directories(bin);

    std::string cmd;

    /*
     * Assignment 01 stays C.
     * Its .c files are compiled as C object files.
     */
    if (n == 1)
        cmd = "gcc -std=c11 -Wall -Wextra ";

    /*
     * Assignment 02+ are C++.
     * Previous assignment sources are reused.
     */
    else
        cmd = "g++ -std=c++17 -Wall -Wextra ";

    for (int i = 0; i < n; ++i)
    {
        fs::path src = a[i] / "src";

        for (auto& f : fs::directory_iterator(src))
        {
            auto ext = f.path().extension().string();
            auto name = f.path().filename().string();

            if ((ext != ".c" && ext != ".cpp") ||
                (i < n - 1 &&
                 (name == "driver.c" || name == "driver.cpp")))
                continue;

            cmd += "\"" + f.path().string() + "\" ";
        }

        cmd += "-I\"" + src.string() + "\" ";
    }

    cmd += "-o \"" + (bin / "driver").string() + "\"";

    std::cout << "\nBuilding...\n";

    if (std::system(cmd.c_str()))
        return 1;

    std::cout << "Build successful.\n\n";

    std::string run =
        "cd \"" + a[n - 1].string() +
        "\" && \"bin/driver\"";

    for (int i = 2; i < argc; ++i)
        run += " \"" + std::string(argv[i]) + "\"";

    return std::system(run.c_str());
}