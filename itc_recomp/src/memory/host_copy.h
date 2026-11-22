// Copies and fills over host objects (docs/loops/e5-loop.md, step 4): the
// game's memcpy, memmove and memset take guest addresses, which may now be
// handles (memory/handles.h) naming host objects. Host structs keep the
// guest offsets (tools/schema_gen.py) with their scalar fields in host byte
// order, so a copy between two host objects is a copy of bytes, and a copy
// between a host object and guest memory swaps each scalar field the range
// covers (the type's layout, written by schema_gen.py; bytes fields keep
// guest order and are copied as they are). A fill writes one byte value
// everywhere, which reads the same in either byte order.
#pragma once

#include <cstdint>

namespace memory {

// A scalar field of a host type wider than a byte: its offset and width.
struct LayoutField {
  uint32_t offset;
  uint32_t width;
};

// A host type's scalar fields (embedded objects' fields included), by offset.
struct Layout {
  const LayoutField* fields;
  uint32_t count;
};

// When either range is a handle's: the copy done (as memmove: overlapping
// host ranges come out right) and true; else false, nothing done (a copy
// in guest memory, the caller's).
bool CopyWithHandles(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size);

// When the destination is a handle's: the fill done and true; else false.
bool FillWithHandles(uint8_t* base, uint32_t destination, uint8_t value, uint32_t size);

// A field over other fields of a host object (a union, a word read as two
// halves, a byte of a word): its value as the guest's bytes would give it.
// `object` is the host struct's start, `layout` its scalar members; the
// covered members are put in guest order in a scratch copy, the field read
// (or written, then the members put back) big-endian there.
uint64_t LoadOverlay(const uint8_t* object, const Layout& layout, uint32_t offset, uint32_t width);
void StoreOverlay(uint8_t* object, const Layout& layout, uint32_t offset, uint32_t width,
                  uint64_t value);

}  // namespace memory
