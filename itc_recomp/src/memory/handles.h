// Handles (docs/e5-native.md, "A bridge while both shapes exist"; step 3):
// a 32-bit value standing for an object that lives in host memory, where
// code still in guest shape holds it (a guest object's field, a register
// passed to code not yet converted). The schema's views of a host type
// (`storage = "host"`, tools/schema_gen.py) take a handle where a view of a
// guest type takes a guest address, so the value flows through guest memory
// and registers unchanged.
//
// Handles come from a guest range nothing allocates in (0x60000000 to
// 0x7EFFFFFF), made inaccessible when guest memory is mapped: a load or
// store the tools couldn't type, reaching a moved object through its
// handle, faults there, and the fault handler logs the object's type and
// the offset into it, and stops the program. Each object's handles span
// its size (rounded up to 4 KB), so `handle + offset` names the object too.

#pragma once

#include <cstdint>

namespace memory {

constexpr uint32_t kHandlesStart = 0x60000000;
constexpr uint32_t kHandlesEnd = 0x7F000000;
constexpr uint32_t kHandleGranule = 0x1000;

// Called by MapGuest: the range made inaccessible, the fault handler set.
bool ProtectHandles(uint8_t* base);

// A handle for `object` (`size` bytes, of the schema type `type`, a string
// that outlives it). Logs and aborts when the range is full.
uint32_t MakeHandle(void* object, uint32_t size, const char* type);

// Frees `handle` (its object isn't deleted). 0 is ignored.
void ReleaseHandle(uint32_t handle);

// The object `handle` stands for: nullptr for 0, and for a value that
// isn't a live handle's start (logged once with the value).
void* HandleObject(uint32_t handle);

inline bool IsHandle(uint32_t value) {
  return value >= kHandlesStart && value < kHandlesEnd;
}

}  // namespace memory
