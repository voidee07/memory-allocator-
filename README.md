# memory-allocator-

# Memory Management & Cache Simulator

## Overview
This project is a C++ command-line simulator that models key operating system memory
management mechanisms and cache behavior. It implements contiguous memory allocation
strategies, a buddy allocator, and a two-level cache hierarchy (L1 and L2) with FIFO
replacement.

The simulator is interactive and allows users to experiment with allocation strategies,
fragmentation behavior, and cache hit/miss patterns under different workloads.

---

## Features

### Memory Allocation
- Contiguous memory allocator
  - First Fit
  - Best Fit
  - Worst Fit
- Buddy allocator (power-of-two allocation)
- Block splitting and coalescing
- Internal and external fragmentation analysis
- Allocation success/failure tracking

### Cache Simulation
- Two-level cache hierarchy (L1 and L2)
- Set-associative cache (configurable associativity)
- FIFO replacement policy
- Cache hit/miss statistics

### Interface
- Interactive CLI
- Deterministic command execution
- Dump and statistics commands for debugging and analysis

---

## Build Instructions

### Requirements
- g++ (C++17 or later)
- Windows PowerShell / Linux terminal

### Build
```sh
g++ -std=c++17 src/main.cpp \
    src/allocators/contiguous/contiguous_allocator.cpp \
    src/allocators/buddy/buddy_allocator.cpp \
    src/cache/cache.cpp \
    src/cache/cache_controller.cpp \
    -o memsim

### Run
.\memsim
