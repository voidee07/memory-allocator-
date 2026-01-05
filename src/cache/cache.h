#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <bits/stdc++.h>
#include <queue>
#include <cstddef>
using namespace std;

struct CacheLine {
    bool valid = false;
    size_t tag = 0;
};

struct CacheSet {
    vector<CacheLine> lines;
    queue<int> fifo;   // indices of lines
};

class Cache {
private:
    size_t cache_size;
    size_t block_size;
    size_t associativity;
    size_t num_sets;

    vector<CacheSet> sets;

    size_t hits = 0;
    size_t misses = 0;

public:
    Cache(size_t cache_size,
          size_t block_size,
          size_t associativity);

    bool access(size_t address);

    void dump() const;
    void stats() const;
};

#endif
