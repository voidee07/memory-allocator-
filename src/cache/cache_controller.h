#ifndef CACHE_CONTROLLER_H
#define CACHE_CONTROLLER_H

#include "cache.h"

class CacheController {
private:
    Cache l1;
    Cache l2;

public:
    CacheController(
        size_t l1_size,
        size_t l2_size,
        size_t block_size,
        size_t associativity
    );

    void access(size_t address);

    void dump() const;
    void stats() const;
};

#endif
