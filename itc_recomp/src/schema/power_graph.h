// The views of docs/functions/power-graph.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/power-graph.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Line::RecordFlat.r4
struct LineSprite : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // the identity at the reset
  uint32_t world_matrix_address() const { return address + 0; }
  float end0_x() const { return guest::LoadFloat(base, address + 80); }
  void set_end0_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t end0_x_address() const { return address + 80; }
  float end0_y() const { return guest::LoadFloat(base, address + 84); }
  void set_end0_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t end0_y_address() const { return address + 84; }
  float end0_z() const { return guest::LoadFloat(base, address + 88); }
  void set_end0_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t end0_z_address() const { return address + 88; }
  float end1_x() const { return guest::LoadFloat(base, address + 96); }
  void set_end1_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t end1_x_address() const { return address + 96; }
  float end1_y() const { return guest::LoadFloat(base, address + 100); }
  void set_end1_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t end1_y_address() const { return address + 100; }
  float end1_z() const { return guest::LoadFloat(base, address + 104); }
  void set_end1_z(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t end1_z_address() const { return address + 104; }
  // 0 to 255
  uint32_t end0_red() const { return guest::LoadU32(base, address + 112); }
  void set_end0_red(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t end0_red_address() const { return address + 112; }
  // end0_red's low byte, as Line::RecordFlat reads it
  uint8_t end0_red_byte() const { return guest::LoadU8(base, address + 115); }
  void set_end0_red_byte(uint8_t v) const { guest::StoreU8(base, address + 115, v); }
  uint32_t end0_red_byte_address() const { return address + 115; }
  uint32_t end0_green() const { return guest::LoadU32(base, address + 116); }
  void set_end0_green(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t end0_green_address() const { return address + 116; }
  uint8_t end0_green_byte() const { return guest::LoadU8(base, address + 119); }
  void set_end0_green_byte(uint8_t v) const { guest::StoreU8(base, address + 119, v); }
  uint32_t end0_green_byte_address() const { return address + 119; }
  uint32_t end0_blue() const { return guest::LoadU32(base, address + 120); }
  void set_end0_blue(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t end0_blue_address() const { return address + 120; }
  uint8_t end0_blue_byte() const { return guest::LoadU8(base, address + 123); }
  void set_end0_blue_byte(uint8_t v) const { guest::StoreU8(base, address + 123, v); }
  uint32_t end0_blue_byte_address() const { return address + 123; }
  // shifted into the vertex color's top byte
  uint32_t end0_alpha() const { return guest::LoadU32(base, address + 124); }
  void set_end0_alpha(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t end0_alpha_address() const { return address + 124; }
  uint32_t end1_red() const { return guest::LoadU32(base, address + 128); }
  void set_end1_red(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t end1_red_address() const { return address + 128; }
  uint8_t end1_red_byte() const { return guest::LoadU8(base, address + 131); }
  void set_end1_red_byte(uint8_t v) const { guest::StoreU8(base, address + 131, v); }
  uint32_t end1_red_byte_address() const { return address + 131; }
  uint32_t end1_green() const { return guest::LoadU32(base, address + 132); }
  void set_end1_green(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t end1_green_address() const { return address + 132; }
  uint8_t end1_green_byte() const { return guest::LoadU8(base, address + 135); }
  void set_end1_green_byte(uint8_t v) const { guest::StoreU8(base, address + 135, v); }
  uint32_t end1_green_byte_address() const { return address + 135; }
  uint32_t end1_blue() const { return guest::LoadU32(base, address + 136); }
  void set_end1_blue(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t end1_blue_address() const { return address + 136; }
  uint8_t end1_blue_byte() const { return guest::LoadU8(base, address + 139); }
  void set_end1_blue_byte(uint8_t v) const { guest::StoreU8(base, address + 139, v); }
  uint32_t end1_blue_byte_address() const { return address + 139; }
  uint32_t end1_alpha() const { return guest::LoadU32(base, address + 140); }
  void set_end1_alpha(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t end1_alpha_address() const { return address + 140; }
};

// embedded; measure: sub_821B8F60.r3
struct SpecSheetMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9608;
  // the StateMachine's state
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
};

// The image words docs/functions/power-graph.md's code uses.
struct PowerGraphGlobals {
  // the line recorder's render queue, stored by every Line::RecordFlat
  static uint32_t line_queue(uint8_t* base) { return guest::LoadU32(base, 0x826D83D8u); }
  static void set_line_queue(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83D8u, v); }
  static constexpr uint32_t line_queue_address() { return 0x826D83D8u; }
  // the line recorder's command list, stored by every Line::RecordFlat
  static uint32_t line_list(uint8_t* base) { return guest::LoadU32(base, 0x826D83DCu); }
  static void set_line_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83DCu, v); }
  static constexpr uint32_t line_list_address() { return 0x826D83DCu; }
  // Primitive::RecordTransform's command list
  static uint32_t transform_list(uint8_t* base) { return guest::LoadU32(base, 0x826D83D4u); }
  static void set_transform_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83D4u, v); }
  static constexpr uint32_t transform_list_address() { return 0x826D83D4u; }
  // the front end's car holder; its +0 word is PowerGraph::Draw's r3
  static uint32_t stand_holder(uint8_t* base) { return guest::LoadU32(base, 0x826D83E0u); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
  // the car list's graph drawn while set (power-graph-screen.md)
  static uint32_t graph_shown(uint8_t* base) { return guest::LoadU32(base, 0x82815EC0u); }
  static void set_graph_shown(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815EC0u, v); }
  static constexpr uint32_t graph_shown_address() { return 0x82815EC0u; }
};

}  // namespace schema
