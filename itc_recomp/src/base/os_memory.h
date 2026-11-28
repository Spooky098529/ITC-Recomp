// Host memory from the operating system, for the game's blocks (the heap,
// the image, the physical memory: memory/mapping.cpp) and for giving their
// pages back: mmap and madvise on Linux, VirtualAlloc on Windows.

#pragma once

#include <cstdint>
#include <vector>

namespace base {

// A block of `bytes`, read-write and zeroed, its pages backed only once
// touched. Null if it can't be had.
uint8_t* ReserveZeroed(uint64_t bytes);

// [start, start + bytes) (whole pages, inside a ReserveZeroed block) given
// back to the system: read again, they're zeros.
void DropPages(void* start, uint64_t bytes);

// The page size.
uint64_t PageSize();

// For each page of [start, start + bytes): whether it's backed now (a
// diagnostic). False if it can't be told.
bool ResidentPages(const void* start, uint64_t bytes, std::vector<unsigned char>& resident);

}  // namespace base
