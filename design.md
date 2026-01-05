
---


# Design Document – Memory Management & Cache Simulator

## 1. Objectives
The goal of this project is to understand and simulate:
- Memory allocation strategies used by operating systems
- Fragmentation behavior
- Cache hierarchy behavior and replacement policies

The simulator models *policy decisions*, not real hardware execution.

---

## 2. Memory Allocation Design

### 2.1 Contiguous Allocator
A single contiguous memory region is simulated. Memory is divided into blocks that are
tracked using a linked-list–like structure.

#### Allocation Strategies
- **First Fit**: Select the first block large enough.
- **Best Fit**: Select the smallest block that satisfies the request.
- **Worst Fit**: Select the largest available block.

#### Deallocation
- Freed blocks are immediately coalesced with adjacent free blocks.
- External fragmentation is reduced through coalescing.

#### Fragmentation Metrics
- **Internal fragmentation**: unused memory inside allocated blocks.
- **External fragmentation**: free memory that cannot satisfy requests.

---

### 2.2 Buddy Allocator
Memory is managed in blocks of size 2^k.

#### Key Properties
- Allocation size is rounded up to nearest power of two.
- Each free block has a unique “buddy” identified using XOR.
- Recursive splitting during allocation.
- Recursive merging during deallocation.

This allocator significantly reduces external fragmentation at the cost of internal
fragmentation.

---

## 3. Cache Simulator Design

### 3.1 Cache Organization
- Two-level cache hierarchy: L1 and L2
- Set-associative mapping
- FIFO replacement policy

Each cache consists of:
- Multiple sets
- Each set containing a fixed number of cache lines (associativity)

---

### 3.2 Address Mapping
For an address:
- Block number = address / block_size
- Set index = block_number % num_sets
- Tag = block_number / num_sets

A cache line is uniquely identified by the pair (set index, tag).

---

### 3.3 Cache Access Flow
1. Check L1 cache
2. On L1 miss, check L2
3. On L2 hit, promote block to L1
4. On miss in both, fetch from memory and insert into both caches

---

### 3.4 Replacement Policy
FIFO replacement is implemented per set:
- Each set maintains a queue of insertion order
- Oldest entry is evicted upon overflow

---

## 4. CLI Design
The simulator is controlled through a CLI supporting:
- Initialization of allocators and cache
- Allocation and deallocation
- Memory and cache inspection
- Statistics reporting

---

## 5. Limitations
- No virtual memory or paging
- No write-back or dirty bits
- Cache stores metadata only, not real data
- Single-threaded simulation

---

## 6. Conclusion
This project demonstrates core OS concepts including memory management, fragmentation,
and cache behavior, providing hands-on insight into low-level system design trade-offs.
