// The views of docs/functions/minimap.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/minimap.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Triangle
struct MinimapTriangle : View {
  using View::View;
  static constexpr uint32_t kSize = 176;
  // its world matrix (rows of four floats); an identity from Triangle::Init, then the marker's matrix copied in
  uint32_t world_address() const { return address + 0; }
  // 1.0 from Triangle::Init
  float world_row0_w() const { return guest::LoadFloat(base, address + 12); }
  void set_world_row0_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t world_row0_w_address() const { return address + 12; }
  // 1.0 from Triangle::Init
  float world_row1_w() const { return guest::LoadFloat(base, address + 28); }
  void set_world_row1_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t world_row1_w_address() const { return address + 28; }
  // 1.0 from Triangle::Init
  float world_row2_w() const { return guest::LoadFloat(base, address + 44); }
  void set_world_row2_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t world_row2_w_address() const { return address + 44; }
  // 1.0 from Triangle::Init
  float world_row3_w() const { return guest::LoadFloat(base, address + 60); }
  void set_world_row3_w(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t world_row3_w_address() const { return address + 60; }
  // 0 flat (Triangle::RecordFlat), 1 sub_8217C4C0 (never seen in play), above 1 none
  uint32_t kind() const { return guest::LoadU32(base, address + 64); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t kind_address() const { return address + 64; }
  // 0 from Triangle::Init; not read by RecordFlat (probably a render mode, as the sprite list's +68)
  uint32_t mode_a() const { return guest::LoadU32(base, address + 68); }
  void set_mode_a(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t mode_a_address() const { return address + 68; }
  // 2 from Triangle::Init; not read by RecordFlat (probably a render mode, as the sprite list's +72)
  uint32_t mode_b() const { return guest::LoadU32(base, address + 72); }
  void set_mode_b(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t mode_b_address() const { return address + 72; }
  // three vertices of four floats (x, y, z, w)
  uint32_t vertices_address() const { return address + 80; }
  float vertex0_x() const { return guest::LoadFloat(base, address + 80); }
  void set_vertex0_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t vertex0_x_address() const { return address + 80; }
  float vertex0_y() const { return guest::LoadFloat(base, address + 84); }
  void set_vertex0_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t vertex0_y_address() const { return address + 84; }
  float vertex0_z() const { return guest::LoadFloat(base, address + 88); }
  void set_vertex0_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t vertex0_z_address() const { return address + 88; }
  // 1.0 from Triangle::Init
  float vertex0_w() const { return guest::LoadFloat(base, address + 92); }
  void set_vertex0_w(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t vertex0_w_address() const { return address + 92; }
  float vertex1_x() const { return guest::LoadFloat(base, address + 96); }
  void set_vertex1_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t vertex1_x_address() const { return address + 96; }
  float vertex1_y() const { return guest::LoadFloat(base, address + 100); }
  void set_vertex1_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t vertex1_y_address() const { return address + 100; }
  float vertex1_z() const { return guest::LoadFloat(base, address + 104); }
  void set_vertex1_z(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t vertex1_z_address() const { return address + 104; }
  // 1.0 from Triangle::Init
  float vertex1_w() const { return guest::LoadFloat(base, address + 108); }
  void set_vertex1_w(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t vertex1_w_address() const { return address + 108; }
  float vertex2_x() const { return guest::LoadFloat(base, address + 112); }
  void set_vertex2_x(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t vertex2_x_address() const { return address + 112; }
  float vertex2_y() const { return guest::LoadFloat(base, address + 116); }
  void set_vertex2_y(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t vertex2_y_address() const { return address + 116; }
  float vertex2_z() const { return guest::LoadFloat(base, address + 120); }
  void set_vertex2_z(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t vertex2_z_address() const { return address + 120; }
  // 1.0 from Triangle::Init
  float vertex2_w() const { return guest::LoadFloat(base, address + 124); }
  void set_vertex2_w(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t vertex2_w_address() const { return address + 124; }
  // the three vertices' colors, four words each (red, green, blue, alpha)
  uint32_t colors_address() const { return address + 128; }
  uint32_t color0_red() const { return guest::LoadU32(base, address + 128); }
  void set_color0_red(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t color0_red_address() const { return address + 128; }
  // the red word's low byte, which the recorders read
  uint8_t color0_red_byte() const { return guest::LoadU8(base, address + 131); }
  void set_color0_red_byte(uint8_t v) const { guest::StoreU8(base, address + 131, v); }
  uint32_t color0_red_byte_address() const { return address + 131; }
  uint32_t color0_green() const { return guest::LoadU32(base, address + 132); }
  void set_color0_green(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t color0_green_address() const { return address + 132; }
  uint8_t color0_green_byte() const { return guest::LoadU8(base, address + 135); }
  void set_color0_green_byte(uint8_t v) const { guest::StoreU8(base, address + 135, v); }
  uint32_t color0_green_byte_address() const { return address + 135; }
  uint32_t color0_blue() const { return guest::LoadU32(base, address + 136); }
  void set_color0_blue(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t color0_blue_address() const { return address + 136; }
  uint8_t color0_blue_byte() const { return guest::LoadU8(base, address + 139); }
  void set_color0_blue_byte(uint8_t v) const { guest::StoreU8(base, address + 139, v); }
  uint32_t color0_blue_byte_address() const { return address + 139; }
  // shifted into the ARGB's top byte
  uint32_t color0_alpha() const { return guest::LoadU32(base, address + 140); }
  void set_color0_alpha(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t color0_alpha_address() const { return address + 140; }
  uint32_t color1_red() const { return guest::LoadU32(base, address + 144); }
  void set_color1_red(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t color1_red_address() const { return address + 144; }
  uint8_t color1_red_byte() const { return guest::LoadU8(base, address + 147); }
  void set_color1_red_byte(uint8_t v) const { guest::StoreU8(base, address + 147, v); }
  uint32_t color1_red_byte_address() const { return address + 147; }
  uint32_t color1_green() const { return guest::LoadU32(base, address + 148); }
  void set_color1_green(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t color1_green_address() const { return address + 148; }
  uint8_t color1_green_byte() const { return guest::LoadU8(base, address + 151); }
  void set_color1_green_byte(uint8_t v) const { guest::StoreU8(base, address + 151, v); }
  uint32_t color1_green_byte_address() const { return address + 151; }
  uint32_t color1_blue() const { return guest::LoadU32(base, address + 152); }
  void set_color1_blue(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t color1_blue_address() const { return address + 152; }
  uint8_t color1_blue_byte() const { return guest::LoadU8(base, address + 155); }
  void set_color1_blue_byte(uint8_t v) const { guest::StoreU8(base, address + 155, v); }
  uint32_t color1_blue_byte_address() const { return address + 155; }
  uint32_t color1_alpha() const { return guest::LoadU32(base, address + 156); }
  void set_color1_alpha(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t color1_alpha_address() const { return address + 156; }
  uint32_t color2_red() const { return guest::LoadU32(base, address + 160); }
  void set_color2_red(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t color2_red_address() const { return address + 160; }
  uint8_t color2_red_byte() const { return guest::LoadU8(base, address + 163); }
  void set_color2_red_byte(uint8_t v) const { guest::StoreU8(base, address + 163, v); }
  uint32_t color2_red_byte_address() const { return address + 163; }
  uint32_t color2_green() const { return guest::LoadU32(base, address + 164); }
  void set_color2_green(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t color2_green_address() const { return address + 164; }
  uint8_t color2_green_byte() const { return guest::LoadU8(base, address + 167); }
  void set_color2_green_byte(uint8_t v) const { guest::StoreU8(base, address + 167, v); }
  uint32_t color2_green_byte_address() const { return address + 167; }
  uint32_t color2_blue() const { return guest::LoadU32(base, address + 168); }
  void set_color2_blue(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t color2_blue_address() const { return address + 168; }
  uint8_t color2_blue_byte() const { return guest::LoadU8(base, address + 171); }
  void set_color2_blue_byte(uint8_t v) const { guest::StoreU8(base, address + 171, v); }
  uint32_t color2_blue_byte_address() const { return address + 171; }
  uint32_t color2_alpha() const { return guest::LoadU32(base, address + 172); }
  void set_color2_alpha(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t color2_alpha_address() const { return address + 172; }
};

// embedded; measure: List80, List80::Copy.r4
struct MinimapList80 : View {
  using View::View;
  static constexpr uint32_t kSize = 5456;
  // 80 nodes of two halfwords: the next node (-1 none) and the node's data slot
  uint32_t nodes_address() const { return address + 0; }
  // measure fault: List80::Copy's r3 is its out matrix (row 0's w, 1.0)
  float fault_out_row0_w() const { return guest::LoadFloat(base, address + 12); }
  void set_fault_out_row0_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t fault_out_row0_w_address() const { return address + 12; }
  // measure fault: List80::Copy's out matrix (row 1's w, 1.0)
  float fault_out_row1_w() const { return guest::LoadFloat(base, address + 28); }
  void set_fault_out_row1_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t fault_out_row1_w_address() const { return address + 28; }
  // measure fault: List80::Copy's out matrix (row 2's w, 1.0)
  float fault_out_row2_w() const { return guest::LoadFloat(base, address + 44); }
  void set_fault_out_row2_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t fault_out_row2_w_address() const { return address + 44; }
  // measure fault: List80::Copy's out matrix (row 3's w, 1.0)
  float fault_out_row3_w() const { return guest::LoadFloat(base, address + 60); }
  void set_fault_out_row3_w(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t fault_out_row3_w_address() const { return address + 60; }
  // the first free node (taken by List80::Link)
  uint16_t free_head() const { return guest::LoadU16(base, address + 320); }
  void set_free_head(uint16_t v) const { guest::StoreU16(base, address + 320, v); }
  uint32_t free_head_address() const { return address + 320; }
  // 0xFFFF from Minimap::Draw
  uint16_t unknown_322() const { return guest::LoadU16(base, address + 322); }
  void set_unknown_322(uint16_t v) const { guest::StoreU16(base, address + 322, v); }
  uint32_t unknown_322_address() const { return address + 322; }
  // the first node (-1 none, signed)
  uint16_t head() const { return guest::LoadU16(base, address + 324); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 324, v); }
  uint32_t head_address() const { return address + 324; }
  // 0xFFFF from Minimap::Draw
  uint16_t unknown_326() const { return guest::LoadU16(base, address + 326); }
  void set_unknown_326(uint16_t v) const { guest::StoreU16(base, address + 326, v); }
  uint32_t unknown_326_address() const { return address + 326; }
  uint16_t count() const { return guest::LoadU16(base, address + 328); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 328, v); }
  uint32_t count_address() const { return address + 328; }
  // 80 slots of 64-byte matrices
  uint32_t data_address() const { return address + 336; }
};

// embedded; measure: Minimap, Race::FindPlayerEntry.ret
struct MinimapRacerEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // 0x4 and 0x8 the two human cars (probably), 0x1 a racer the minimap shows unless 0x20, 0x10 shown without a rival type; also 0x2 traffic, 0x40 has a vehicle (race-world.md)
  uint32_t flags() const { return guest::LoadU32(base, address + 0); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t flags_address() const { return address + 0; }
  // measure fault: Minimap::Scaling's r3 is its out matrix (the x scale)
  float fault_scaling_m00() const { return guest::LoadFloat(base, address + 0); }
  void set_fault_scaling_m00(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t fault_scaling_m00_address() const { return address + 0; }
  // the entry's number (race-world.md)
  uint32_t number() const { return guest::LoadU32(base, address + 4); }
  void set_number(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t number_address() const { return address + 4; }
  // section and slice read as one word (Minimap::Draw's copy of the player's)
  uint32_t position() const { return guest::LoadU32(base, address + 8); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t position_address() const { return address + 8; }
  // the route section it's on (-1 none, signed)
  uint16_t section() const { return guest::LoadU16(base, address + 8); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t section_address() const { return address + 8; }
  // the section's slice it's on (-1 none, signed)
  uint16_t slice() const { return guest::LoadU16(base, address + 10); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t slice_address() const { return address + 10; }
  // ticks gathered toward the next slice while it has no vehicle (race-world.md)
  float slice_ticks() const { return guest::LoadFloat(base, address + 12); }
  void set_slice_ticks(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t slice_ticks_address() const { return address + 12; }
  // the traffic's, mod 42 (race-world.md)
  uint32_t traffic_number() const { return guest::LoadU32(base, address + 16); }
  void set_traffic_number(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t traffic_number_address() const { return address + 16; }
  // -1 none (race-world.md)
  uint32_t vehicle_index() const { return guest::LoadU32(base, address + 20); }
  void set_vehicle_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t vehicle_index_address() const { return address + 20; }
  // measure fault: Minimap::Scaling's out matrix (the y scale)
  float fault_scaling_m11() const { return guest::LoadFloat(base, address + 20); }
  void set_fault_scaling_m11(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t fault_scaling_m11_address() const { return address + 20; }
  // its racer record's index in the race data's +48 (3,936 bytes each)
  uint32_t record() const { return guest::LoadU32(base, address + 24); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t record_address() const { return address + 24; }
  // measure fault: Minimap::Scaling's out matrix (the z scale)
  float fault_scaling_m22() const { return guest::LoadFloat(base, address + 40); }
  void set_fault_scaling_m22(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t fault_scaling_m22_address() const { return address + 40; }
};

// embedded; measure: Minimap::Scaling.r4
struct MinimapScale : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // 1.0 where Minimap::Draw makes one; not read
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// embedded; measure: Minimap::DrawCarMarker.r4, Minimap::DrawPointMarker.r4
struct MinimapColor : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t red() const { return guest::LoadU32(base, address + 0); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t red_address() const { return address + 0; }
  uint32_t green() const { return guest::LoadU32(base, address + 4); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t green_address() const { return address + 4; }
  uint32_t blue() const { return guest::LoadU32(base, address + 8); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t blue_address() const { return address + 8; }
  uint32_t alpha() const { return guest::LoadU32(base, address + 12); }
  void set_alpha(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t alpha_address() const { return address + 12; }
};

// embedded; measure: SetList::Point.r4
struct MinimapPoint : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // x and y as one doubleword (the original's std)
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  // z and w as one doubleword (the original's std)
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // 1.0
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// The image words docs/functions/minimap.md's code uses.
struct MinimapGlobals {
  // the car marker's 7 triangles: 3 vertices of 16 bytes (x, 0.0, z, 1.0) each
  static constexpr uint32_t car_marker_table_address() { return 0x826C5520u; }
  // bit 0: the car marker's table made
  static uint32_t car_marker_guard(uint8_t* base) { return guest::LoadU32(base, 0x826C5670u); }
  static void set_car_marker_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5670u, v); }
  static constexpr uint32_t car_marker_guard_address() { return 0x826C5670u; }
  // the point marker's 22 triangles, as the car marker's
  static constexpr uint32_t point_marker_table_address() { return 0x826C5680u; }
  // bit 0: the point marker's table made
  static uint32_t point_marker_guard(uint8_t* base) { return guest::LoadU32(base, 0x826C5AA0u); }
  static void set_point_marker_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5AA0u, v); }
  static constexpr uint32_t point_marker_guard_address() { return 0x826C5AA0u; }
  // Minimap::Draw's calls, 0 to 4
  static uint32_t blink_frames(uint8_t* base) { return guest::LoadU32(base, 0x826C5AA4u); }
  static void set_blink_frames(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5AA4u, v); }
  static constexpr uint32_t blink_frames_address() { return 0x826C5AA4u; }
  // 0 to 5, stepped each wrap of blink_frames; added to a car marker's type to pick its pool
  static uint32_t blink_phase(uint8_t* base) { return guest::LoadU32(base, 0x826C5AA8u); }
  static void set_blink_phase(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5AA8u, v); }
  static constexpr uint32_t blink_phase_address() { return 0x826C5AA8u; }
  // the five marker types' colors (MinimapColor)
  static constexpr uint32_t type_colors_address() { return 0x826C5AB0u; }
  // bit 0: type_colors made
  static uint32_t type_colors_guard(uint8_t* base) { return guest::LoadU32(base, 0x826C5B00u); }
  static void set_type_colors_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B00u, v); }
  static constexpr uint32_t type_colors_guard_address() { return 0x826C5B00u; }
  // the course's route (its +12 the sections)
  static uint32_t route(uint8_t* base) { return guest::LoadU32(base, 0x826C5B10u); }
  static void set_route(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B10u, v); }
  static constexpr uint32_t route_address() { return 0x826C5B10u; }
  // the race data (+20 flags, +48 the racer records, +0x59620 the stages, +0xA3AF0 the mode)
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // in mode 6, picks which human car's entry (flags 0x4 or 0x8) gets type 1
  static uint32_t human_flag(uint8_t* base) { return guest::LoadU32(base, 0x826C7824u); }
  static void set_human_flag(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7824u, v); }
  static constexpr uint32_t human_flag_address() { return 0x826C7824u; }
  // the racers' manager (+4 its 28-byte MinimapRacerEntry's, +2368 their count)
  static uint32_t racers_manager(uint8_t* base) { return guest::LoadU32(base, 0x826D82F4u); }
  static void set_racers_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
  // the render queue the triangles go to (the color rectangles')
  static uint32_t triangle_queue(uint8_t* base) { return guest::LoadU32(base, 0x826D83D0u); }
  static void set_triangle_queue(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83D0u, v); }
  static constexpr uint32_t triangle_queue_address() { return 0x826D83D0u; }
  // the command list the triangles are recorded on
  static uint32_t triangle_list(uint8_t* base) { return guest::LoadU32(base, 0x826D83D4u); }
  static void set_triangle_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83D4u, v); }
  static constexpr uint32_t triangle_list_address() { return 0x826D83D4u; }
  // the pass's view, copied by Minimap::Draw
  static constexpr uint32_t view_copy_address() { return 0x8265ECA0u; }
  // group 1's sort key (the pass description's +28)
  static uint32_t draw_key_b(uint8_t* base) { return guest::LoadU32(base, 0x8296B900u); }
  static void set_draw_key_b(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B900u, v); }
  static constexpr uint32_t draw_key_b_address() { return 0x8296B900u; }
  // group 0's sort key (the pass description's +24)
  static uint32_t draw_key_a(uint8_t* base) { return guest::LoadU32(base, 0x8296B90Cu); }
  static void set_draw_key_a(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B90Cu, v); }
  static constexpr uint32_t draw_key_a_address() { return 0x8296B90Cu; }
  // 1.0 (image)
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0 (image)
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // pi / 2 (image)
  static float quarter_turn(uint8_t* base) { return guest::LoadFloat(base, 0x82001FB4u); }
  static void set_quarter_turn(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FB4u, v); }
  static constexpr uint32_t quarter_turn_address() { return 0x82001FB4u; }
  // the map's draws' near range (image)
  static float range_near(uint8_t* base) { return guest::LoadFloat(base, 0x82002814u); }
  static void set_range_near(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002814u, v); }
  static constexpr uint32_t range_near_address() { return 0x82002814u; }
  // the markers' x and z scale (image)
  static float marker_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_marker_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t marker_scale_address() { return 0x82002A0Cu; }
  // the map's draws' far range (image)
  static float range_far(uint8_t* base) { return guest::LoadFloat(base, 0x82006804u); }
  static void set_range_far(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006804u, v); }
  static constexpr uint32_t range_far_address() { return 0x82006804u; }
  // -1000.0: points below aren't drawn (image)
  static float lowest(uint8_t* base) { return guest::LoadFloat(base, 0x82006808u); }
  static void set_lowest(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006808u, v); }
  static constexpr uint32_t lowest_address() { return 0x82006808u; }
};

}  // namespace schema
