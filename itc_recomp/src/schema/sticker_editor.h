// The views of docs/functions/sticker-editor.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/sticker-editor.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// image; measure: -
struct StickerGroup : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  int32_t first_id() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_first_id(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t first_id_address() const { return address + 0; }
  int32_t last_id() const { return int32_t(guest::LoadU32(base, address + 4)); }
  void set_last_id(int32_t v) const { guest::StoreU32(base, address + 4, uint32_t(v)); }
  uint32_t last_id_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// image; measure: -
struct StickerLayout : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the area's origin on the movie (plus 816 or 824 by the node)
  int32_t x() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_x(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t x_address() const { return address + 0; }
  // plus 284 or 252 by the node
  int32_t y() const { return int32_t(guest::LoadU32(base, address + 4)); }
  void set_y(int32_t v) const { guest::StoreU32(base, address + 4, uint32_t(v)); }
  uint32_t y_address() const { return address + 4; }
  // times the sticker's scale: the node's size
  float size() const { return guest::LoadFloat(base, address + 8); }
  void set_size(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
  // times the sticker's x and y
  float position_scale() const { return guest::LoadFloat(base, address + 12); }
  void set_position_scale(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t position_scale_address() const { return address + 12; }
};

// image; measure: -
struct StickerMoveBounds : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  int32_t x_min() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_x_min(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t x_min_address() const { return address + 0; }
  int32_t x_max() const { return int32_t(guest::LoadU32(base, address + 4)); }
  void set_x_max(int32_t v) const { guest::StoreU32(base, address + 4, uint32_t(v)); }
  uint32_t x_max_address() const { return address + 4; }
  int32_t y_min() const { return int32_t(guest::LoadU32(base, address + 8)); }
  void set_y_min(int32_t v) const { guest::StoreU32(base, address + 8, uint32_t(v)); }
  uint32_t y_min_address() const { return address + 8; }
  int32_t y_max() const { return int32_t(guest::LoadU32(base, address + 12)); }
  void set_y_max(int32_t v) const { guest::StoreU32(base, address + 12, uint32_t(v)); }
  uint32_t y_max_address() const { return address + 12; }
};

// embedded; measure: -
struct StickerRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // times the layout's position scale, plus its origin
  int32_t x() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_x(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t x_address() const { return address + 0; }
  int32_t y() const { return int32_t(guest::LoadU32(base, address + 4)); }
  void set_y(int32_t v) const { guest::StoreU32(base, address + 4, uint32_t(v)); }
  uint32_t y_address() const { return address + 4; }
  // degrees (0.0 to 359.0): the node's rotation
  float angle() const { return guest::LoadFloat(base, address + 8); }
  void set_angle(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t angle_address() const { return address + 8; }
  // times the layout's size: the node's scale
  float scale() const { return guest::LoadFloat(base, address + 12); }
  void set_scale(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t scale_address() const { return address + 12; }
  // copied into the node's flip_x
  uint32_t flip() const { return guest::LoadU32(base, address + 16); }
  void set_flip(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t flip_address() const { return address + 16; }
  // 0 at the reset
  uint32_t word() const { return guest::LoadU32(base, address + 20); }
  void set_word(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t word_address() const { return address + 20; }
};

// The image words docs/functions/sticker-editor.md's code uses.
struct StickerEditorGlobals {
  // six StickerGroup records
  static constexpr uint32_t groups_address() { return 0x8200B5E8u; }
  // 61 words: 1 when sticker i is drawn on its slot's second node (read for any i below 61, negative too)
  static constexpr uint32_t second_node_address() { return 0x8200B630u; }
  // StickerLayout records, 16 bytes a car id (the garage car's +224) and area (8 a car id); the table's length isn't known
  static constexpr uint32_t layouts_address() { return 0x8200B728u; }
  // eight StickerMoveBounds, one an area
  static constexpr uint32_t move_bounds_address() { return 0x8200C2A8u; }
  // the first node's extra size factor
  static float first_node_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8200C548u); }
  static void set_first_node_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200C548u, v); }
  static constexpr uint32_t first_node_scale_address() { return 0x8200C548u; }
  // 359.0: the rotation wraps between 0.0 and it
  static float angle_max(uint8_t* base) { return guest::LoadFloat(base, 0x8200C54Cu); }
  static void set_angle_max(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200C54Cu, v); }
  static constexpr uint32_t angle_max_address() { return 0x8200C54Cu; }
  // the scale's step
  static float scale_step(uint8_t* base) { return guest::LoadFloat(base, 0x8200222Cu); }
  static void set_scale_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200222Cu, v); }
  static constexpr uint32_t scale_step_address() { return 0x8200222Cu; }
  // the scale's lower clamp
  static float scale_min(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_scale_min(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t scale_min_address() { return 0x82001FBCu; }
  // the scale's upper clamp
  static float scale_max(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_scale_max(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t scale_max_address() { return 0x820021E8u; }
  // 0.0: the reset angle
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 1.0: the reset scale and the rotation's step
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // the stand scene's holder (garage-screen.toml's stand_holder): +0 the garage stage's holder
  static View stand_holder(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
};

}  // namespace schema
