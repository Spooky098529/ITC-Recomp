// The views of docs/functions/std-string.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/std-string.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: String, std::string, sub_82091398.r3, sub_82091720.r3, sub_82091720.r4, sub_82091808.r3, sub_820A6E60.r3, sub_820A6F60.r3
struct StdString : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the text in place while the capacity is under 16
  uint32_t text_address() const { return address + 4; }
  // the text's heap block (capacity + 1 bytes) while the capacity is 16 or more
  uint32_t block() const { return guest::LoadU32(address + 4); }
  void set_block(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t block_address() const { return address + 4; }
  uint32_t length() const { return guest::LoadU32(address + 20); }
  void set_length(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t length_address() const { return address + 20; }
  // 15 while the text is in place
  uint32_t capacity() const { return guest::LoadU32(address + 24); }
  void set_capacity(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t capacity_address() const { return address + 24; }
};

// embedded; measure: sub_82091968.r3
struct StdStringLogicError : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  static constexpr uint32_t kVtable = 0x82000CD8;
  // 0x82000CD8
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82000CF4, an image word (probably the exception's default text)
  uint32_t what() const { return guest::LoadU32(address + 4); }
  void set_what(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t what_address() const { return address + 4; }
  // a StdString
  uint32_t message_address() const { return address + 8; }
};

// embedded; measure: WildcardSearch.r3, WildcardSearch.r4, String::ReplaceAll.r5, sub_825931D0.r4
struct StdStringCString : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  // any length, to its 0 byte (the search reads up to 3 bytes from a position)
  uint32_t chars_address() const { return address + 0; }
};

// embedded; measure: WildcardSearch.r5, WildcardSearch.r6
struct StdStringMatchWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the match's offset (from the text searched) or its length
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

}  // namespace schema
