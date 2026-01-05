#include "cache.h"
#include <iostream>

using namespace std;

Cache::Cache(size_t csize, size_t bsize, size_t assoc)
    : cache_size(csize),
      block_size(bsize),
      associativity(assoc) {

    size_t num_blocks = cache_size / block_size;
    num_sets = num_blocks / associativity; // here associativity will be 2 for 2-way set associative

    sets.resize(num_sets);

    for (auto& set : sets) {
        set.lines.resize(associativity);
    }
}

bool Cache::access(size_t address) {
    size_t block_number = address / block_size;
    size_t set_index = block_number % num_sets;
    size_t tag = block_number / num_sets;

    CacheSet& set = sets[set_index];

    // Check for HIT
    for (auto& line : set.lines) {
        if (line.valid && line.tag == tag) {
            hits++;
            return true;
        }
    }

    // MISS
    misses++;

    // Look for empty line
    for (int i = 0; i < associativity; i++) {
        if (!set.lines[i].valid) {
            set.lines[i].valid = true;
            set.lines[i].tag = tag;
            set.fifo.push(i);
            return false;
        }
    }

    // FIFO eviction
    int victim = set.fifo.front();
    set.fifo.pop();

    set.lines[victim].tag = tag;
    set.lines[victim].valid = true;

    set.fifo.push(victim);

    return false;
}

void Cache::dump() const {
    cout << "Cache State:\n";
    for (size_t i = 0; i < num_sets; i++) {
        cout << "Set " << i << ": ";
        for (const auto& line : sets[i].lines) {
            if (line.valid)
                cout << "[T" << line.tag << "] ";
            else
                cout << "[ ] ";
        }
        cout << "\n";
    }
    cout << "-------------------\n";
}

void Cache::stats() const {
    size_t total = hits + misses;
    double hit_rate = total == 0 ? 0.0 : (double)hits / total * 100.0;

    cout << "Cache Stats:\n";
    cout << "Hits   : " << hits << "\n";
    cout << "Misses : " << misses << "\n";
    cout << "Hit Rate: " << hit_rate << " %\n";
    cout << "-------------------\n";
}
