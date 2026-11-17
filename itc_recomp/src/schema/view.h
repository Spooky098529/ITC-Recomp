// A view of one of the game's objects in guest memory (docs/e5-native.md,
// step 2): `base` and the object's guest address, nothing else. The views
// of the schema's types (itc_recomp/schema/*.toml, written into this folder
// by tools/schema_gen.py) derive from it and name the object's fields; their
// accessors read and write guest memory as guest::Load* and guest::Store* do,
// so code switched from offsets to views changes nothing the game sees. Once
// code names a field by its view, where the field lives can change in one
// place (steps 3 and 4).
#pragma once

#include "guest.h"

#include <bit>
#include <cstdint>

namespace schema {

struct View {
  uint8_t* base = nullptr;
  uint32_t address = 0;

  View() = default;
  View(uint8_t* base_, uint32_t address_) : base(base_), address(address_) {}

  // Whether it views an object (a null pointer field gives address 0).
  explicit operator bool() const { return address != 0; }
};

}  // namespace schema
