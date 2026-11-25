// The memory the game uses (E2 step 4, docs/rexglue-removal.md "E2: the
// plan"; since E5 steps 9 and 10, docs/e5-native.md): the heap, the image
// and the physical memory as blocks of host memory of their own, the
// guest's addresses for them translated by guest.h (memory::g_heap,
// g_image, g_physical). Linux only for now.

#pragma once

#include <cstdint>

namespace memory {

// Maps the blocks, once (later calls return the first one's result).
// Logs and returns false when it can't.
bool MapGuest();

}  // namespace memory
