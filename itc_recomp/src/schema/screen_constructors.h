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

// embedded; measure: Type6ListScreen.+9672
struct ListNodeTable : View {
  using View::View;
  static constexpr uint32_t kSize = 460;
  static constexpr uint32_t kVtable = 0x82010FF8;
  // 0x82010FF0 NodeTable's, 0x82010FF8 PairedNodeTable's
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the UI movie (25 for the type 6 list)
  uint32_t movie() const { return guest::LoadU32(base, address + 4); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t movie_address() const { return address + 4; }
  uint32_t node_set_a() const { return guest::LoadU32(base, address + 8); }
  void set_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_set_a_address() const { return address + 8; }
  uint32_t node_set_b() const { return guest::LoadU32(base, address + 12); }
  void set_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t node_set_b_address() const { return address + 12; }
  // 27 items' named animations
  uint32_t item_anims_address() const { return address + 16; }
  // eight more (the third pair's)
  uint32_t more_anims_address() const { return address + 124; }
  // 27 items' nodes
  uint32_t item_nodes_address() const { return address + 156; }
  // eight nodes
  uint32_t nodes_address() const { return address + 264; }
  // four children's indexes
  uint32_t children_address() const { return address + 296; }
  // 27 items' masks
  uint32_t item_masks_address() const { return address + 312; }
  uint32_t node_base_a() const { return guest::LoadU32(base, address + 420); }
  void set_node_base_a(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t node_base_a_address() const { return address + 420; }
  uint32_t node_base_b() const { return guest::LoadU32(base, address + 424); }
  void set_node_base_b(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t node_base_b_address() const { return address + 424; }
  // four pairs (now, before)
  uint32_t pairs_address() const { return address + 428; }
};

// embedded; measure: Type7ListScreen.+9672
struct ListColorNodeTable : View {
  using View::View;
  static constexpr uint32_t kSize = 584;
  static constexpr uint32_t kVtable = 0x82011000;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the UI movie (the list screen's +9680)
  uint32_t movie() const { return guest::LoadU32(base, address + 4); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t movie_address() const { return address + 4; }
  uint32_t node_set_a() const { return guest::LoadU32(base, address + 8); }
  void set_node_set_a(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_set_a_address() const { return address + 8; }
  uint32_t node_set_b() const { return guest::LoadU32(base, address + 12); }
  void set_node_set_b(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t node_set_b_address() const { return address + 12; }
  // 27 items' named animations
  uint32_t item_anims_address() const { return address + 16; }
  // eight more (the third pair's)
  uint32_t more_anims_address() const { return address + 124; }
  // 27 items' nodes
  uint32_t item_nodes_address() const { return address + 156; }
  // eight nodes
  uint32_t nodes_address() const { return address + 264; }
  // four children's indexes
  uint32_t children_address() const { return address + 296; }
  // 27 items' masks (type 7: from the records at 0x826370A0)
  uint32_t item_masks_address() const { return address + 312; }
  uint32_t node_base_a() const { return guest::LoadU32(base, address + 420); }
  void set_node_base_a(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t node_base_a_address() const { return address + 420; }
  uint32_t node_base_b() const { return guest::LoadU32(base, address + 424); }
  void set_node_base_b(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t node_base_b_address() const { return address + 424; }
  // four pairs (now, before)
  uint32_t pairs_address() const { return address + 428; }
  // two more pairs: mark bits
  uint32_t marks_address() const { return address + 460; }
  uint32_t color_child() const { return guest::LoadU32(base, address + 476); }
  void set_color_child(uint32_t v) const { guest::StoreU32(base, address + 476, v); }
  uint32_t color_child_address() const { return address + 476; }
  // a 3-column node grid
  uint32_t grid_a_address() const { return address + 480; }
  // another
  uint32_t grid_b_address() const { return address + 516; }
  // four words (255 in the constructor)
  uint32_t color_a_address() const { return address + 552; }
  // four words (205, 235, 67, 255)
  uint32_t color_b_address() const { return address + 568; }
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
