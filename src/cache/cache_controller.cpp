#include "cache_controller.h"
#include <iostream>

using namespace std;

CacheController::CacheController(
    size_t l1_size,
    size_t l2_size,
    size_t block_size,
    size_t associativity
)
    : l1(l1_size, block_size, associativity),
      l2(l2_size, block_size, associativity) {}

void CacheController::access(size_t address) {
    // Try L1
    if (l1.access(address)) {
        return;
    }

    // L1 miss → try L2
    if (l2.access(address)) {
        // Bring block into L1
        l1.access(address);
        return;
    }

    // Miss in both → bring into both
    l2.access(address);
    l1.access(address);
}

void CacheController::dump() const {
    cout << "\n===== L1 Cache =====\n";
    l1.dump();
    cout << "\n===== L2 Cache =====\n";
    l2.dump();
}

void CacheController::stats() const {
    cout << "\n===== L1 Stats =====\n";
    l1.stats();
    cout << "\n===== L2 Stats =====\n";
    l2.stats();
}
