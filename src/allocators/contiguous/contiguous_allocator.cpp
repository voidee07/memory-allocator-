#ifndef CONTIGUOUS_ALLOCATOR_H  // only complies the ehader file once
#define CONTIGUOUS_ALLOCATOR_H

#include <cstddef>
#include <iostream>

enum class AllocationStrategy {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

struct Block {
    size_t start;      // start address
    size_t size;       // block size
    bool free;         // free or allocated
    int id;            // allocation id (-1 if free)

    Block* prev;
    Block* next;

    Block(size_t s, size_t sz)
        : start(s), size(sz), free(true), id(-1),
          prev(nullptr), next(nullptr) {}
};

class ContiguousAllocator {
private:
    Block* head;                 // start of block list
    size_t total_memory;
    AllocationStrategy strategy;
    int next_id;

public:
    ContiguousAllocator(size_t memory_size);

    void setStrategy(AllocationStrategy s);

    int allocate(size_t size);   // malloc
    bool deallocate(int id);     // free

    void dump() const;
    void stats() const;

    ~ContiguousAllocator();
};

#endif
