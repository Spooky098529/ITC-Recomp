// The views of docs/functions/screen-constructors.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/screen-constructors.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: NodeBase
struct NodeBase : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr uint32_t kVtable = 0x82010B5C;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 in the constructor; the classes above it keep their group or movie there
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// image; measure: -
struct Type7ListRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // 0, 1, or 2 and more
  int32_t count() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_count(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t count_address() const { return address + 0; }
  // a bit number (in the mask from a count of 1 up)
  uint32_t bit_a() const { return guest::LoadU32(base, address + 12); }
  void set_bit_a(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t bit_a_address() const { return address + 12; }
  // a bit number (in the mask from a count of 2 up)
  uint32_t bit_b() const { return guest::LoadU32(base, address + 16); }
  void set_bit_b(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t bit_b_address() const { return address + 16; }
};

// The image words docs/functions/screen-constructors.md's code uses.
struct ScreenConstructorsGlobals {
  // 15 Type7ListRecords
  static constexpr uint32_t type7_list_records_address() { return 0x826370A0u; }
};

}  // namespace schema
