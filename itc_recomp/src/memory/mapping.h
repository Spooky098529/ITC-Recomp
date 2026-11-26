// The memory the game uses (E2 step 4, docs/rexglue-removal.md "E2: the
// plan"; since E5 steps 9 and 10, docs/e5-native.md): the heap, the image
// and the physical memory as blocks of host memory of their own, the
// guest's addresses for them translated by guest.h (memory::g_heap,
// g_image, g_physical). Linux only for now.

#pragma once

#include <cstdint>
#include <string>

namespace memory {

// Maps the blocks, once (later calls return the first one's result).
// Logs and returns false when it can't.
bool MapGuest();

// The resident memory by block, for the log (--log_memory_use,
// docs/pc-renderer.md step 0): each block's touched pages (mincore), and
// the process's resident set from /proc/self/status (anonymous, file and
// shared). "heap 212.3 MB; image 9.1 MB; physical 301.0 MB; process 1830.2
// MB (anon 1402.1, file 120.4, shmem 307.7); driver (whole card1) VRAM
// 3100.2 MB, GTT 1420.0 MB" (the driver part on amdgpu only).
std::string DescribeResident();

// Starts a thread giving freed memory back to the system every 10 s
// (malloc_trim; --trim_memory).
void StartTrimming();

}  // namespace memory
