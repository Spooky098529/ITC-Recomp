// Guest memory, mapped by us (E2 step 4, docs/rexglue-removal.md "E2: the
// plan"): the guest's 4 GB of addresses and 512 MB of physical memory, laid
// out as the SDK's views laid them out (rexglue-sdk src/system/xmemory.cpp,
// map_info), so guest addresses, the physical aliases and the renderer's
// flat physical range (base + 4 GB, native/state.h) read the same memory
// as before. Linux only for now.

#pragma once

#include <cstdint>

namespace memory {

// Maps guest memory, once (later calls return the first one's result).
// Logs and returns false when it can't.
bool MapGuest();

// The host address of guest address 0 (nullptr before MapGuest).
uint8_t* Base();

}  // namespace memory
