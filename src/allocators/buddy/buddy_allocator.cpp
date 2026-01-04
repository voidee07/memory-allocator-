#include "buddy_allocator.h"
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;


BuddyAllocator::BuddyAllocator(size_t memory_size, size_t min_block)
    : total_memory(memory_size),
      min_block_size(min_block) {

    max_order = log2(total_memory);
    int min_order = log2(min_block_size);

    freeLists.resize(max_order + 1);

    // Initially all memory is one big free block
    freeLists[max_order].push_back(0);
}

int BuddyAllocator::sizeToOrder(size_t size) const {
    return std::log2(size);
}

size_t BuddyAllocator::orderToSize(int order) const {
    return (size_t)1 << order;
}
int BuddyAllocator::allocate(size_t size) {
    if (size == 0)
        return -1;

    // Round up to next power of two
    size_t actual = min_block_size;
    while (actual < size)
        actual <<= 1;

    int req_order = sizeToOrder(actual);

    int order = req_order;
    while (order <= max_order && freeLists[order].empty())
        order++;

    if (order > max_order)
        return -1;

    // Take a block
    size_t addr = freeLists[order].front();
    freeLists[order].pop_front();

    // Split until desired size
    while (order > req_order) {
        order--;
        size_t buddy = addr + orderToSize(order);
        freeLists[order].push_back(buddy);
    }

    allocated[addr] = {size, orderToSize(req_order)};
    return (int)addr;
}
//--- deallocation---
bool BuddyAllocator::deallocate(size_t address) {
    auto it = allocated.find(address);
    if (it == allocated.end())
        return false;

    size_t size = it->second.second;// the allocated block size
    allocated.erase(it);

    int order = sizeToOrder(size);
    size_t addr = address;

    while (order < max_order) {
        size_t buddy = addr ^ orderToSize(order);// xor of adress and size gives the buddy address

        auto& list = freeLists[order];
        auto buddy_it = find(list.begin(), list.end(), buddy);

        if (buddy_it == list.end())
            break;

        // Merge
        list.erase(buddy_it);
        addr = min(addr, buddy);
        order++;
    }

    freeLists[order].push_back(addr);
    return true;
}

//--- dump and stats ---//
void BuddyAllocator::dump() const {
    std::cout << "Buddy Allocator State:\n";
    for (int i = 0; i <= max_order; i++) {
        size_t size = orderToSize(i);
        std::cout << "Order " << i << " (size " << size << "): ";
        for (size_t addr : freeLists[i])
            std::cout << addr << " ";
        std::cout << "\n";
    }
    std::cout << "-----------------------------\n";
}
void BuddyAllocator::stats() const {
   
    
    size_t requested=0;
    size_t allocated_mem=0;

    for (auto& p : allocated) {
        requested += p.second.first;
        allocated_mem += p.second.second;
        
    }

    size_t internal_frag = allocated_mem - requested;

    cout << "Buddy Allocator Stats:\n";
    cout << "Total Memory : " << total_memory << "\n";
    cout << "Used Memory  : " << allocated_mem << "\n";
    cout << "Free Memory  : " << (total_memory - allocated_mem) << "\n";
   double internal_frag_percent =
    allocated_mem == 0 ? 0.0 :
    (double)internal_frag / (double)allocated_mem * 100.0;

cout << "Internal Fragmentation: "
     << internal_frag << " bytes ("
     << internal_frag_percent << " %)\n";

    cout << "-----------------------------\n";
}


