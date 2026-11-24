// A view of one of the game's objects in guest memory (docs/e5-native.md,
// step 2): `base` and the object's guest address, nothing else. The views
// of the schema's types (itc_recomp/schema/*.toml, written into this folder
// by tools/schema_gen.py) derive from it and name the object's fields; their
// accessors read and write guest memory as guest::Load* and guest::Store* do,
// so code switched from offsets to views changes nothing the game sees. Once
// code names a field by its view, where the field lives can change in one
// place (steps 3 and 4). A host type's view (`storage = "host"`) holds a
// handle (memory/handles.h) in `address` instead, and reads the object's
// members in host memory.
#pragma once

#include "guest.h"
#include "memory/handles.h"
#include "memory/host_copy.h"

#include <algorithm>
#include <bit>
#include <cstdint>
#include <cstring>
#include <new>

namespace schema {

struct View {
  uint8_t* base = nullptr;
  uint32_t address = 0;

  View() = default;
  View(uint8_t* base_, uint32_t address_) : base(base_), address(address_) {}

  // Whether it views an object (a null pointer field gives address 0).
  explicit operator bool() const { return address != 0; }
};

// A host type's bytes fields keep their contents in guest order (big
// endian), as guest memory held them: lifted code reading inside one by
// offset reads them through these (docs/loops/e5-loop.md, step 3).
namespace host {
inline uint8_t LoadU8(const uint8_t* p) { return p[0]; }
inline uint16_t LoadU16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }
inline uint32_t LoadU32(const uint8_t* p) {
  return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3];
}
inline uint64_t LoadU64(const uint8_t* p) { return uint64_t(LoadU32(p)) << 32 | LoadU32(p + 4); }
inline void StoreU8(uint8_t* p, uint8_t v) { p[0] = v; }
inline void StoreU16(uint8_t* p, uint16_t v) { p[0] = uint8_t(v >> 8); p[1] = uint8_t(v); }
inline void StoreU32(uint8_t* p, uint32_t v) {
  p[0] = uint8_t(v >> 24); p[1] = uint8_t(v >> 16); p[2] = uint8_t(v >> 8); p[3] = uint8_t(v);
}
inline void StoreU64(uint8_t* p, uint64_t v) { StoreU32(p, uint32_t(v >> 32)); StoreU32(p + 4, uint32_t(v)); }
}  // namespace host

}  // namespace schema
