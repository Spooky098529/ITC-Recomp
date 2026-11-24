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
// memory/host_copy.h) may be null. `element`: for an array allocated at
// once (schema's NewArray), the size of one element, whose layout repeats
// every `element` bytes; 0 for one object.
uint32_t MakeHandle(void* object, uint32_t size, const char* type,
                    const Layout* layout = nullptr, uint32_t element = 0);

// A blob (E5 step 5): `size` bytes of a file's data or another buffer the
// game uses as raw bytes, in PC memory behind a handle, zero-filled, kept in
// the guest's byte order (no field layout). Everything reaching it behaves
// as on guest memory: loads and stores by address (guest.h), raw pointers
// (guest::Pointer gives the blob's bytes), copies and frees.
uint32_t NewBlob(uint32_t size, const char* type);

// The host bytes at `value` when it lies inside a live blob (`left`, when
// given, the bytes from there to the blob's end); else nullptr.
uint8_t* BlobPointer(uint32_t value, uint32_t* left = nullptr);

// Whether `layout` is the blobs' (no fields; raw accesses aren't logged).
bool IsBlobLayout(const Layout* layout);

// Frees `handle` (its object isn't deleted). 0 is ignored.
void ReleaseHandle(uint32_t handle);

// The object `handle` stands for, or for a value inside its span (handle +
// offset) the byte at that offset of the object (host structs are laid out
// at the guest offsets: an embedded object or interface): nullptr for 0,
// and for a value no live handle covers (logged once with the value).
void* HandleObject(uint32_t handle);

// The object a view of the type whose layout is `layout` stands for, when
// `value` is a handle to an object of that type (one allocated with that
// layout, or an element of such an array): `object + offset`. nullptr for
// anything else: a guest address, or an object inside a host owner of
// another type (an embedded object, a record kept in the owner's bytes),
// whose bytes the owner's layout orders (memory::OwnerLoad).
void* HandleOwn(uint32_t value, const Layout* layout);

// The game's frees (HeapFree, VirtualFree, XPhysicalFree, free and operator
// delete, all through memory/heap.h) given a handle: the host object deleted
// and its handle released, true; false for a value that isn't a live
// handle's start (logged). A shared deleting destructor frees host and
// guest objects alike, so the frees, not their call sites, tell them apart.
bool FreeHandle(uint32_t handle);

// What a value in the handles' range names: false when no live object's
// span covers it.
struct HandleInfo {
  void* object;          // the object's start
  uint32_t offset;       // the value's offset into it
  uint32_t size;
  const char* type;
  const Layout* layout;  // may be null; an array's: one element's
  uint32_t element;      // an array's element size, else the object's size
};
bool DescribeHandle(uint32_t value, HandleInfo* info);

inline bool IsHandle(uint32_t value) {
  return value >= kHandlesStart && value < kHandlesEnd;
}

}  // namespace memory
