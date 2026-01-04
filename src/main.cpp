#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "allocators/contiguous/contiguous_allocator.h"
#include "allocators/buddy/buddy_allocator.h"

using namespace std;
enum class Mode {
    NONE,
    CONTIGUOUS,
    BUDDY
};
Mode currentMode = Mode::NONE;

ContiguousAllocator* contig = nullptr;
BuddyAllocator* buddy = nullptr;


vector<string> tokenize(const string& line) {
    stringstream ss(line);
    vector<string> tokens;
    string token;
    while (ss >> token)
        tokens.push_back(token);
    return tokens;
}

int main() {
    Mode currentMode = Mode::NONE;
    ContiguousAllocator* contig = nullptr;
    BuddyAllocator* buddy = nullptr;

    cout << "Memory Simulator CLI\n";

    while (true) {
        cout << "> ";
        string line;
        getline(cin, line);

        if (line.empty())
            continue;

        auto tokens = tokenize(line);
        string cmd = tokens[0];

        // EXIT
        if (cmd == "exit") {
            break;
        }

        // INIT
        else if (cmd == "init") {
            if (tokens.size() < 3) {
                cout << "Invalid init command\n";
                continue;
            }

            // Cleanup previous allocator
            delete contig; contig = nullptr;
            delete buddy;  buddy  = nullptr;

            if (tokens[1] == "contiguous") {
                size_t size = stoul(tokens[2]);
                contig = new ContiguousAllocator(size);
                currentMode = Mode::CONTIGUOUS;
                cout << "Initialized contiguous allocator\n";
            }
            else if (tokens[1] == "buddy") {
                if (tokens.size() < 4) {
                    cout << "Invalid buddy init\n";
                    continue;
                }
                size_t size = stoul(tokens[2]);
                size_t minBlock = stoul(tokens[3]);
                buddy = new BuddyAllocator(size, minBlock);
                currentMode = Mode::BUDDY;
                cout << "Initialized buddy allocator\n";
            }
            else {
                cout << "Unknown allocator type\n";
            }
        }

        // SET STRATEGY (CONTIGUOUS ONLY)
        else if (cmd == "set") {
            if (currentMode != Mode::CONTIGUOUS || tokens.size() < 2) {
                cout << "Set valid only for contiguous allocator\n";
                continue;
            }

            if (tokens[1] == "first_fit")
                contig->setStrategy(AllocationStrategy::FIRST_FIT);
            else if (tokens[1] == "best_fit")
                contig->setStrategy(AllocationStrategy::BEST_FIT);
            else if (tokens[1] == "worst_fit")
                contig->setStrategy(AllocationStrategy::WORST_FIT);
            else
                cout << "Unknown strategy\n";
        }

        // MALLOC
        else if (cmd == "malloc") {
            if (tokens.size() < 2) {
                cout << "Invalid malloc\n";
                continue;
            }

            size_t size = stoul(tokens[1]);

            if (currentMode == Mode::CONTIGUOUS) {
                int id = contig->allocate(size);
                cout << "Allocated id: " << id << "\n";
            }
            else if (currentMode == Mode::BUDDY) {
                int addr = buddy->allocate(size);
                cout << "Allocated addr: " << addr << "\n";
            }
            else {
                cout << "No allocator initialized\n";
            }
        }

        // FREE
        else if (cmd == "free") {
            if (tokens.size() < 2) {
                cout << "Invalid free\n";
                continue;
            }

            int val = stoi(tokens[1]);

            if (currentMode == Mode::CONTIGUOUS)
                contig->deallocate(val);
            else if (currentMode == Mode::BUDDY)
                buddy->deallocate(val);
            else
                cout << "No allocator initialized\n";
        }

        // DUMP
        else if (cmd == "dump") {
            if (currentMode == Mode::CONTIGUOUS)
                contig->dump();
            else if (currentMode == Mode::BUDDY)
                buddy->dump();
            else
                cout << "No allocator initialized\n";
        }

        // STATS
        else if (cmd == "stats") {
            if (currentMode == Mode::CONTIGUOUS)
                contig->stats();
            else if (currentMode == Mode::BUDDY)
                buddy->stats();
            else
                cout << "No allocator initialized\n";
        }

        else {
            cout << "Unknown command\n";
        }
    }

    delete contig;
    delete buddy;
    return 0;
}

