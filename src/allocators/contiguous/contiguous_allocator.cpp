#include "contiguous_allocator.h"
#include <iomanip>
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

        

        