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

struct Layout;  // memory/host_copy.h

constexpr uint32_t kHandlesStart = 0x60000000;
constexpr uint32_t kHandlesEnd = 0x7F000000;
constexpr uint32_t kHandleGranule = 0x1000;

// Called by MapGuest: the range made inaccessible, the fault handler set.
bool ProtectHandles(uint8_t* base);

// A handle for `object` (`size` bytes, of the schema type `type`, a string
// that outlives it). Logs and aborts when the range is full.
// `layout` (its scalar fields, for copies between it and guest memory:
// memory/host_copy.h) may be null.
uint32_t MakeHandle(void* object, uint32_t size, const char* type,
                    const Layout* layout = nullptr);

// Frees `handle` (its object isn't deleted). 0 is ignored.
void ReleaseHandle(uint32_t handle);

// The object `handle` stands for, or for a value inside its span (handle +
// offset) the byte at that offset of the object (host structs are laid out
// at the guest offsets: an embedded object or interface): nullptr for 0,
// and for a value no live handle covers (logged once with the value).
void* HandleObject(uint32_t handle);

// What a value in the handles' range names: false when no live object's
// span covers it.
struct HandleInfo {
  void* object;          // the object's start
  uint32_t offset;       // the value's offset into it
  uint32_t size;
  const char* type;
  const Layout* layout;  // may be null
};
bool DescribeHandle(uint32_t value, HandleInfo* info);

inline bool IsHandle(uint32_t value) {
  return value >= kHandlesStart && value < kHandlesEnd;
}

}  // namespace memory
