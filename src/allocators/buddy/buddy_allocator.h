#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <vector>
#include <list>
#include <unordered_map>
#include <cstddef>
using namespace std;

class BuddyAllocator {
private:
    size_t total_memory;
    size_t min_block_size;
    int max_order;

   vector<list<size_t>> freeLists;
    unordered_map<size_t, size_t> allocated; // addr → size
   
     int sizeToOrder(size_t size) const;
    size_t orderToSize(int order) const;
public:
    BuddyAllocator(size_t memory_size, size_t min_block);

    int allocate(size_t size);
    bool deallocate(size_t address);

    void dump() const;
    void stats() const;
};

#endif
