// The views of docs/functions/build-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/build-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: BuildList::Insert.r4
struct BuildScreenInsertValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // a build's address (the slot's pointer)
  uint32_t build() const { return guest::LoadU32(base, address + 0); }
  void set_build(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t build_address() const { return address + 0; }
};

// The image words docs/functions/build-screen.md's code uses.
struct BuildScreenGlobals {
  // the transition machine: pages 0 to 3 the levels (groups, builds, the stand, the details), 4 leaving; 21 transitions
  template <typename T = PageObject>
  static T machine(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D843Cu)); }
  static void set_machine(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D843Cu, v.address); }
  static void set_machine(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D843Cu, v); }
  static constexpr uint32_t machine_address() { return 0x826D843Cu; }
  // the level's menu list (72 bytes, a "menu list")
  template <typename T = MenuList>
  static T list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8440u)); }
  static void set_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8440u, v.address); }
  static void set_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8440u, v); }
  static constexpr uint32_t list_address() { return 0x826D8440u; }
  // the groups' menu list, stepped by left and right on levels 1 and 2
  template <typename T = MenuList>
  static T group_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8444u)); }
  static void set_group_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8444u, v.address); }
  static void set_group_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8444u, v); }
  static constexpr uint32_t group_list_address() { return 0x826D8444u; }
  // the screen data (type2-screens.md's type)
  template <typename T = BuildScreenData>
  static T screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8448u)); }
  static void set_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8448u, v.address); }
  static void set_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8448u, v); }
  static constexpr uint32_t screen_address() { return 0x826D8448u; }
  // the stand scene's holder: three 4-byte blocks, each holding a garage stage at +0
  template <typename T = StandHolder>
  static T stand_holder(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83E0u, v.address); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
  // the screen fades' fade-out counts word
  static uint32_t fade_out_counts(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_out_counts(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_out_counts_address() { return 0x826A50E4u; }
  // the screen fades' fade-in counts word
  static uint32_t fade_in_counts(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_in_counts(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_in_counts_address() { return 0x826A50DCu; }
};

}  // namespace schema
