#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

namespace fs = filesystem;

int main(int argc, char* argv[])
{
    vector<fs::path> assignments;

    for (const auto& entry : fs::directory_iterator("."))
    {
        if (entry.is_directory() &&
            entry.path().filename().string().find("Assignment_") == 0)
        {
            assignments.push_back(entry.path());
        }
    }

    sort(assignments.begin(), assignments.end());

    int selected = 0;

    if (argc > 1)
    {
        for (int i = 0; i < static_cast<int>(assignments.size()); ++i)
        {
            if (assignments[static_cast<size_t>(i)].filename() == argv[1])
            {
                selected = i + 1;
                break;
            }
        }
    }
    else
    {
        cout << "\n";

        for (int i = 0; i < static_cast<int>(assignments.size()); ++i)
        {
            cout << (i + 1) << ". "
                      << assignments[static_cast<size_t>(i)]
                             .filename().string()
                      << "\n";
        }

        cout << "0. Exit\nChoose assignment: ";
        cin >> selected;
    }

    if (selected <= 0 ||
        selected > static_cast<int>(assignments.size()))
    {
        return 0;
    }

    fs::path bin =
        assignments[static_cast<size_t>(selected - 1)] / "bin";

    fs::create_directories(bin);

    string cmd;

    // Select compiler
    switch (selected)
    {
        case 1:
            // Assignment 01 is C
            cmd = "gcc -std=c11 -Wall -Wextra ";
            break;

        default:
            // Assignment 02+ are C++
            cmd = "g++ -std=c++17 -Wall -Wextra ";
            break;
    }

    // Compile all source files from previous assignments,
    // excluding their drivers, then add the selected
    // assignment's sources including its driver.
    for (int i = 0; i < selected; ++i)
    {
        const fs::path src =
            assignments[static_cast<size_t>(i)] / "src";

        if (!fs::exists(src))
        {
            continue;
        }

        for (const auto& entry : fs::directory_iterator(src))
        {
            const string ext =
                entry.path().extension().string();

            const string name =
                entry.path().filename().string();

            if ((ext != ".c" && ext != ".cpp") ||
                (i < selected - 1 &&
                 (name == "driver.c" || name == "driver.cpp")))
            {
                continue;
            }

            cmd += "\"" + entry.path().string() + "\" ";
        }

        cmd += "-I\"" + src.string() + "\" ";
    }

    cmd += "-o \"" + (bin / "driver").string() + "\"";

    cout << "\nBuilding "
              << assignments[static_cast<size_t>(selected - 1)]
                     .filename().string()
              << "...\n";

    if (system(cmd.c_str()) != 0)
    {
        cerr << "Build failed.\n";
        return 1;
    }

    cout << "Build successful.\n\n";

    string run =
        "cd \"" +
        assignments[static_cast<size_t>(selected - 1)].string() +
        "\" && \"bin/driver\"";

    for (int i = 2; i < argc; ++i)
    {
        run += " \"" + string(argv[i]) + "\"";
    }

    return system(run.c_str());
}