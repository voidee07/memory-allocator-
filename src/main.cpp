#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "allocators/contiguous/contiguous_allocator.h"
#include "allocators/buddy/buddy_allocator.h"
#include "cache/cache_controller.h"


using namespace std;
enum class Mode {
    NONE,
    CONTIGUOUS,
    BUDDY,
    CACHE
};
Mode currentMode = Mode::NONE;

ContiguousAllocator* contig = nullptr;
BuddyAllocator* buddy = nullptr;
CacheController* cacheCtrl = nullptr;


vector<string> tokenize(const string& line) {
    stringstream ss(line);
    vector<string> tokens;
    string token;
    while (ss >> token)
        tokens.push_back(token);
    return tokens;
}

int main() {
    size_t alloc_requests = 0;
size_t alloc_success  = 0;

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
           else if (tokens[1] == "cache") {
    if (tokens.size() < 6) {
        cout << "Usage: init cache <L1_size> <L2_size> <block_size> <assoc>\n";
        continue;
    }

    delete cacheCtrl;
    cacheCtrl = new CacheController(
        stoul(tokens[2]),
        stoul(tokens[3]),
        stoul(tokens[4]),
        stoul(tokens[5])
    );

    currentMode = Mode::CACHE;
    cout << "Initialized cache (L1 + L2)\n";
}
else {
                cout << "Unknown allocator type\n";
        }}

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
            alloc_requests++;

            if (currentMode == Mode::CONTIGUOUS) {
                int id = contig->allocate(size);
                if (id != -1)
                {alloc_success++;
                cout << "Allocated id: " << id << "\n";
            }
                else cout << "Allocation failed\n";
            }
            else if (currentMode == Mode::BUDDY) {
                int addr = buddy->allocate(size);
                if (addr != -1)
                {alloc_success++;
                cout << "Allocated addr: " << addr << "\n";
            } else {
                    cout << "Allocation failed\n";
                }
            }
            else {
                cout << "No allocator initialized\n";
            }
        }

        else if (cmd == "access") {
    if (currentMode != Mode::CACHE || tokens.size() < 2) {
        cout << "Access valid only in cache mode\n";
        continue;
    }

    size_t addr = stoul(tokens[1]);
    cacheCtrl->access(addr);
}

else if (cmd == "cache_dump") {
    if (currentMode == Mode::CACHE)
        cacheCtrl->dump();
    else
        cout << "Cache not initialized\n";
}

else if (cmd == "cache_stats") {
    if (currentMode == Mode::CACHE)
        cacheCtrl->stats();
    else
        cout << "Cache not initialized\n";
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


        double success_rate =
        alloc_requests == 0 ? 0.0 :
        (double)alloc_success / alloc_requests * 100.0;

    cout << "Allocation Requests : " << alloc_requests << "\n";
    cout << "Allocation Success  : " << alloc_success << "\n";
    cout << "Success Rate        : " << success_rate << " %\n";
        }

        else {
            cout << "Unknown command\n";
        }

        
    }

    delete contig;
    delete buddy;
    delete cacheCtrl;

    return 0;
}

