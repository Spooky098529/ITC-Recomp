// The memory the game uses (E2 step 4, docs/rexglue-removal.md "E2: the
// plan"; since E5 step 9, docs/e5-native.md): the heap, the image and the
// physical memory as blocks of host memory of their own, the guest's
// addresses for them translated by guest.h (memory::g_heap, g_image,
// g_physical). Linux only for now.

#pragma once

#include <cstdint>

namespace memory {

// Maps the blocks, once (later calls return the first one's result).
// Logs and returns false when it can't.
bool MapGuest();

// What the code still passes as `base` (until step 10): a 4 GB range with
// nothing mapped, so a raw base + address faults and is named
// (memory/handles.cpp). nullptr before MapGuest.
uint8_t* Base();

}  // namespace memory
