#include "contiguous_allocator.h"
#include <iomanip>
#include <iostream>
using namespace std;
ContiguousAllocator::ContiguousAllocator(size_t memory_size)
    : head(nullptr),
      total_memory(memory_size),
      strategy(AllocationStrategy::FIRST_FIT),
      next_id(1)
{
    // create one big free block
    head = new Block(0, memory_size);
}

void ContiguousAllocator::setStrategy(AllocationStrategy s) {
    strategy = s;
}


ContiguousAllocator::~ContiguousAllocator() {
    Block* curr = head;
    while (curr) {
        Block* next = curr->next;
        delete curr;
        curr = next;
    }
}

// the three allocation strategies only differ in selection of the free block the allocation and status keeping are same hence we do not make three different functions

int ContiguousAllocator::allocate(size_t size){
    if (size == 0 || size > total_memory) {
        return -1; // invalid size
    }
    Block* curr =head;
    Block* chosen = nullptr;

    while(curr){
        if (curr->free && curr->size >= size) {
            // choose block based on strategy

            if (!chosen) {
                chosen = curr;

                // first fit: stop immediately
                if (strategy == AllocationStrategy::FIRST_FIT)
                    break;
            }
            else {
                if (strategy == AllocationStrategy::BEST_FIT &&
                    curr->size < chosen->size) {
                    chosen = curr;
                }
                else if (strategy == AllocationStrategy::WORST_FIT &&
                         curr->size > chosen->size) {
                    chosen = curr;
                }
            }}
        curr = curr->next;}

        if(!chosen){
        return -1; // no suitable block found
        }
        // allocate from chosen block(same for all three strategies)

            // Case 1: Exact fit
            if (chosen->size == size) {
                chosen->free = false;
                chosen->id = next_id++;
                return curr->id;
            }

            // Case 2: Split block
            Block* newBlock = new Block(
                chosen->start + size,
                chosen->size - size
            );

            newBlock->next = chosen->next;
            if (newBlock->next)
                newBlock->next->prev = newBlock;

            newBlock->prev = chosen;
            chosen->next = newBlock;
            chosen->size = size;
            chosen->free = false;
            chosen->id = next_id++;

            return chosen->id;
        }



        //-- deallocation and merging of free blocks --//
        bool ContiguousAllocator::deallocate(int id) {
    Block* curr = head;

    // 1. Find the block by ID
    while (curr) {
        if (!curr->free && curr->id == id)
            break;
        curr = curr->next;
    }

    if (!curr)
        return false;   // invalid id

    // 2. Mark block as free
    curr->free = true;
    curr->id = -1;

    // 3. Coalesce with previous block if free
    if (curr->prev && curr->prev->free) {
        Block* prev = curr->prev;

        prev->size += curr->size;
        prev->next = curr->next;

        if (curr->next)
            curr->next->prev = prev;

        delete curr;
        curr = prev;
    }

    // 4. Coalesce with next block if free
    if (curr->next && curr->next->free) {
        Block* next = curr->next;

        curr->size += next->size;
        curr->next = next->next;

        if (next->next)
            next->next->prev = curr;

        delete next;
    }

    return true;
}


        void ContiguousAllocator::dump() const {
    Block* curr = head;
    cout << "Memory State:\n";
    while (curr) {
        cout << "["
                  << curr->start << " - "
                  << curr->start + curr->size - 1 << "] "
                  << (curr->free ? "FREE" : "USED");

        if (!curr->free)
            cout << " (id=" << curr->id << ")";

        cout << "\n";
        curr = curr->next;
    }
    cout << "----------------------\n";
}

void ContiguousAllocator::stats() const {
    size_t used_memory = 0;
    size_t free_memory = 0;
    size_t largest_free = 0;
    int free_blocks = 0;

    Block* curr = head;

    while (curr) {
        if (curr->free) {
            free_memory += curr->size;
            free_blocks++;
            if (curr->size > largest_free)
                largest_free = curr->size;
        } else {
            used_memory += curr->size;
        }
        curr = curr->next;
    }

    double external_fragmentation = 0.0;
    if (free_memory > 0) {
        external_fragmentation =
            (1.0 - (double)largest_free / (double)free_memory) * 100.0;
    }

    cout << "Memory Statistics:\n";
    cout << "Total Memory      : " << total_memory << "\n";
    cout << "Used Memory       : " << used_memory << "\n";
    cout << "Free Memory       : " << free_memory << "\n";
    cout << "Free Blocks       : " << free_blocks << "\n";
    cout << "Largest Free Block: " << largest_free << "\n";
    cout << "External Fragmentation: "
              << external_fragmentation << " %\n";
    cout << "Internal Fragmentation: 0 %\n";
    cout << "-----------------------------\n";
}



        