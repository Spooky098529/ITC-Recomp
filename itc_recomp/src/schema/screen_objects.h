// The views of docs/functions/screen-objects.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/screen-objects.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ScreenObject
struct ScreenObject : View {
  using View::View;
  static constexpr uint32_t kSize = 128000;
  // a float2 position, a color, a float2 texture coordinate (20 bytes a vertex)
  uint32_t vertex_declaration() const { return guest::LoadU32(base, address + 0); }
  void set_vertex_declaration(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vertex_declaration_address() const { return address + 0; }
  // 1024 wide, A8R8G8B8
  uint32_t texture() const { return guest::LoadU32(base, address + 4); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t texture_address() const { return address + 4; }
  uint32_t width() const { return guest::LoadU32(base, address + 8); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t width_address() const { return address + 8; }
  uint32_t height() const { return guest::LoadU32(base, address + 12); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t height_address() const { return address + 12; }
  uint32_t x() const { return guest::LoadU32(base, address + 16); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t x_address() const { return address + 16; }
  uint32_t y() const { return guest::LoadU32(base, address + 20); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t y_address() const { return address + 20; }
  // set by ScreenObject::Start (never seen in play)
  uint8_t running() const { return guest::LoadU8(base, address + 24); }
  void set_running(uint8_t v) const { guest::StoreU8(base, address + 24, v); }
  uint32_t running_address() const { return address + 24; }
  // ScreenObject::ToggleHidden
  uint8_t hidden() const { return guest::LoadU8(base, address + 25); }
  void set_hidden(uint8_t v) const { guest::StoreU8(base, address + 25, v); }
  uint32_t hidden_address() const { return address + 25; }
  uint8_t stopping() const { return guest::LoadU8(base, address + 26); }
  void set_stopping(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t stopping_address() const { return address + 26; }
  // set by each SetSpeed, cleared by each Record
  uint8_t speed_set() const { return guest::LoadU8(base, address + 27); }
  void set_speed_set(uint8_t v) const { guest::StoreU8(base, address + 27, v); }
  uint32_t speed_set_address() const { return address + 27; }
  // the +88 object's slot 13, saved by Start (probably to restore it)
  float glow_saved_0() const { return guest::LoadFloat(base, address + 28); }
  void set_glow_saved_0(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t glow_saved_0_address() const { return address + 28; }
  // the +88 object's slot 21, saved by Start
  float glow_saved_1() const { return guest::LoadFloat(base, address + 32); }
  void set_glow_saved_1(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t glow_saved_1_address() const { return address + 32; }
  // the +88 object's slot 22, saved by Start
  float glow_saved_2() const { return guest::LoadFloat(base, address + 36); }
  void set_glow_saved_2(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t glow_saved_2_address() const { return address + 36; }
  // 0.25: Record sets it on the +88 object (slot 2)
  float glow_setting_0() const { return guest::LoadFloat(base, address + 40); }
  void set_glow_setting_0(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t glow_setting_0_address() const { return address + 40; }
  // 0.035: Record's slot 10
  float glow_setting_1() const { return guest::LoadFloat(base, address + 44); }
  void set_glow_setting_1(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t glow_setting_1_address() const { return address + 44; }
  // 0.6: Record's slot 11
  float glow_setting_2() const { return guest::LoadFloat(base, address + 48); }
  void set_glow_setting_2(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t glow_setting_2_address() const { return address + 48; }
  // the +92 object's settings Start reads (its slot 9 with +52 to +60)
  float second_saved_0() const { return guest::LoadFloat(base, address + 52); }
  void set_second_saved_0(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t second_saved_0_address() const { return address + 52; }
  float second_saved_1() const { return guest::LoadFloat(base, address + 56); }
  void set_second_saved_1(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t second_saved_1_address() const { return address + 56; }
  float second_saved_2() const { return guest::LoadFloat(base, address + 60); }
  void set_second_saved_2(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t second_saved_2_address() const { return address + 60; }
  // 0.95: Record sets +64 to +72 on the +92 object (slot 4)
  float second_setting_0() const { return guest::LoadFloat(base, address + 64); }
  void set_second_setting_0(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t second_setting_0_address() const { return address + 64; }
  // 0.95
  float second_setting_1() const { return guest::LoadFloat(base, address + 68); }
  void set_second_setting_1(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t second_setting_1_address() const { return address + 68; }
  // 1.05
  float second_setting_2() const { return guest::LoadFloat(base, address + 72); }
  void set_second_setting_2(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t second_setting_2_address() const { return address + 72; }
  // the speed in km/h / 300, in [0, 1] (SetSpeed)
  float strength() const { return guest::LoadFloat(base, address + 76); }
  void set_strength(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t strength_address() const { return address + 76; }
  // 0x826BAC00 + 592 x it
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 80); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t camera_pass_address() const { return address + 80; }
  // 599 or 601: probably the chunk's sort key
  uint32_t key() const { return guest::LoadU32(base, address + 84); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t key_address() const { return address + 84; }
  // probably its glow; 0 from the constructor, who sets it not found
  template <typename T = ScreenObjectGlowEffect>
  T glow() const { return T(base, guest::LoadU32(base, address + 88)); }
  void set_glow(const View& v) const { guest::StoreU32(base, address + 88, v.address); }
  void set_glow(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t glow_address() const { return address + 88; }
  // 0 from the constructor, who sets it not found
  template <typename T = ScreenObjectSecondEffect>
  T second_effect() const { return T(base, guest::LoadU32(base, address + 92)); }
  void set_second_effect(const View& v) const { guest::StoreU32(base, address + 92, v.address); }
  void set_second_effect(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t second_effect_address() const { return address + 92; }
  // the fan's 20-byte vertices (720 triangles)
  uint32_t fan_vertices_address() const { return address + 96; }
  uint32_t fan_vertex_count() const { return guest::LoadU32(base, address + 60096); }
  void set_fan_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 60096, v); }
  uint32_t fan_vertex_count_address() const { return address + 60096; }
  // each step's two texture offsets, 16 bytes a step (probably for the draw)
  uint32_t fan_offsets_address() const { return address + 60100; }
  // drawn again every second point
  uint32_t noise_toggle() const { return guest::LoadU32(base, address + 76100); }
  void set_noise_toggle(uint32_t v) const { guest::StoreU32(base, address + 76100, v); }
  uint32_t noise_toggle_address() const { return address + 76100; }
  float noise() const { return guest::LoadFloat(base, address + 76104); }
  void set_noise(float v) const { guest::StoreFloat(base, address + 76104, v); }
  uint32_t noise_address() const { return address + 76104; }
  // Init: 0 (with +76112 to +76120 probably the fade)
  uint8_t fade_flag() const { return guest::LoadU8(base, address + 76108); }
  void set_fade_flag(uint8_t v) const { guest::StoreU8(base, address + 76108, v); }
  uint32_t fade_flag_address() const { return address + 76108; }
  // Init: 0
  uint32_t fade_word() const { return guest::LoadU32(base, address + 76112); }
  void set_fade_word(uint32_t v) const { guest::StoreU32(base, address + 76112, v); }
  uint32_t fade_word_address() const { return address + 76112; }
  // Init: 1.0
  float fade_0() const { return guest::LoadFloat(base, address + 76116); }
  void set_fade_0(float v) const { guest::StoreFloat(base, address + 76116, v); }
  uint32_t fade_0_address() const { return address + 76116; }
  // Init: 1.0
  float fade_1() const { return guest::LoadFloat(base, address + 76120); }
  void set_fade_1(float v) const { guest::StoreFloat(base, address + 76120, v); }
  uint32_t fade_1_address() const { return address + 76120; }
  // a ScreenObjectUvMap (std::map<double, UvColumn>)
  uint32_t uv_map_address() const { return address + 76124; }
  template <typename T = ScreenObjectUvMapNode>
  T uv_map_head() const { return T(base, guest::LoadU32(base, address + 76128)); }
  void set_uv_map_head(const View& v) const { guest::StoreU32(base, address + 76128, v.address); }
  void set_uv_map_head(uint32_t v) const { guest::StoreU32(base, address + 76128, v); }
  uint32_t uv_map_head_address() const { return address + 76128; }
  uint32_t uv_map_size() const { return guest::LoadU32(base, address + 76132); }
  void set_uv_map_size(uint32_t v) const { guest::StoreU32(base, address + 76132, v); }
  uint32_t uv_map_size_address() const { return address + 76132; }
  // the rings' 20-byte vertices (1,440 written)
  uint32_t ring_vertices_address() const { return address + 76136; }
  uint32_t ring_vertex_count() const { return guest::LoadU32(base, address + 127976); }
  void set_ring_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 127976, v); }
  uint32_t ring_vertex_count_address() const { return address + 127976; }
  // Init sets +127980 to +127996 (0, 0.5); not read further
  float end_0() const { return guest::LoadFloat(base, address + 127980); }
  void set_end_0(float v) const { guest::StoreFloat(base, address + 127980, v); }
  uint32_t end_0_address() const { return address + 127980; }
  uint32_t end_word() const { return guest::LoadU32(base, address + 127984); }
  void set_end_word(uint32_t v) const { guest::StoreU32(base, address + 127984, v); }
  uint32_t end_word_address() const { return address + 127984; }
  uint8_t end_flag() const { return guest::LoadU8(base, address + 127988); }
  void set_end_flag(uint8_t v) const { guest::StoreU8(base, address + 127988, v); }
  uint32_t end_flag_address() const { return address + 127988; }
  float end_1() const { return guest::LoadFloat(base, address + 127996); }
  void set_end_1(float v) const { guest::StoreFloat(base, address + 127996, v); }
  uint32_t end_1_address() const { return address + 127996; }
};

// embedded; measure: ScreenObject.+88
struct ScreenObjectGlowEffect : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = ScreenObjectGlowEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: ScreenObject.+88.+0
struct ScreenObjectGlowEffectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  // Record sets +40
  uint32_t slot2() const { return guest::LoadU32(base, address + 8); }
  void set_slot2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot2_address() const { return address + 8; }
  // Record sets +44
  uint32_t slot10() const { return guest::LoadU32(base, address + 40); }
  void set_slot10(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t slot10_address() const { return address + 40; }
  // Record sets +48
  uint32_t slot11() const { return guest::LoadU32(base, address + 44); }
  void set_slot11(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t slot11_address() const { return address + 44; }
  // Start saves it into +28
  uint32_t slot13() const { return guest::LoadU32(base, address + 52); }
  void set_slot13(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t slot13_address() const { return address + 52; }
  // Start saves it into +32
  uint32_t slot21() const { return guest::LoadU32(base, address + 84); }
  void set_slot21(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t slot21_address() const { return address + 84; }
  // Start saves it into +36
  uint32_t slot22() const { return guest::LoadU32(base, address + 88); }
  void set_slot22(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t slot22_address() const { return address + 88; }
};

// embedded; measure: ScreenObject.+92
struct ScreenObjectSecondEffect : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = ScreenObjectSecondEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: ScreenObject.+92.+0
struct ScreenObjectSecondEffectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // Record sets +64 to +72
  uint32_t slot4() const { return guest::LoadU32(base, address + 16); }
  void set_slot4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot4_address() const { return address + 16; }
  // Start reads +52 to +60
  uint32_t slot9() const { return guest::LoadU32(base, address + 36); }
  void set_slot9(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t slot9_address() const { return address + 36; }
};

// embedded; measure: UvMap
struct ScreenObjectUvMap : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the head node: its parent the root, its left and right the leftmost and rightmost nodes
  template <typename T = ScreenObjectUvMapNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
  // measure fault: a node's left (UvMap::ConstructNode's r3)
  uint32_t fault_node_left() const { return guest::LoadU32(base, address + 0); }
  void set_fault_node_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_node_left_address() const { return address + 0; }
  // measure fault: Erase's and InsertUnique's result out (an iterator, stored as one doubleword)
  uint64_t fault_iterator() const { return guest::LoadU64(base, address + 0); }
  void set_fault_iterator(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t fault_iterator_address() const { return address + 0; }
  // measure fault: a byte met at +8
  uint8_t fault_node_right() const { return guest::LoadU8(base, address + 8); }
  void set_fault_node_right(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t fault_node_right_address() const { return address + 8; }
  // measure fault: a node's key
  double fault_node_key() const { return std::bit_cast<double>(guest::LoadU64(base, address + 16)); }
  void set_fault_node_key(double v) const { guest::StoreU64(base, address + 16, std::bit_cast<uint64_t>(v)); }
  uint32_t fault_node_key_address() const { return address + 16; }
  // measure fault: a node's color
  uint8_t fault_node_color() const { return guest::LoadU8(base, address + 40); }
  void set_fault_node_color(uint8_t v) const { guest::StoreU8(base, address + 40, v); }
  uint32_t fault_node_color_address() const { return address + 40; }
  // measure fault: a node's isnil
  uint8_t fault_node_is_nil() const { return guest::LoadU8(base, address + 41); }
  void set_fault_node_is_nil(uint8_t v) const { guest::StoreU8(base, address + 41, v); }
  uint32_t fault_node_is_nil_address() const { return address + 41; }
};

// allocated; measure: UvMap.+4, UvMap.+4.+0, UvMap.+4.+8, UvMap.+4.+8.+0, UvMap.+8, VideoOutput.+276, VideoOutput.+276.+4, new<?>@UvMap::AllocateNodes.+4, new<?>@UvMap::AllocateNodes.+4.+0, new<?>@UvMap::AllocateNodes.+4.+0.+8, new<?>@UvMap::AllocateNodes.+4.+8, new<?>@UvMap::AllocateNodes.+4.+8.+0, UvMap::EraseSubtree.r4, UvMap::EraseSubtree.r4.+0
struct ScreenObjectUvMapNode : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  template <typename T = ScreenObjectUvMapNode>
  T left() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_left(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t left_address() const { return address + 0; }
  template <typename T = ScreenObjectUvMapNode>
  T parent() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_parent(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_parent(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t parent_address() const { return address + 4; }
  template <typename T = ScreenObjectUvMapNode>
  T right() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_right(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_right(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t right_address() const { return address + 8; }
  // x rounded to 0.001
  double key() const { return std::bit_cast<double>(guest::LoadU64(base, address + 16)); }
  void set_key(double v) const { guest::StoreU64(base, address + 16, std::bit_cast<uint64_t>(v)); }
  uint32_t key_address() const { return address + 16; }
  // the key's high word
  uint32_t key_high() const { return guest::LoadU32(base, address + 16); }
  void set_key_high(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t key_high_address() const { return address + 16; }
  // the key's low word
  uint32_t key_low() const { return guest::LoadU32(base, address + 20); }
  void set_key_low(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t key_low_address() const { return address + 20; }
  // a ScreenObjectUvColumn
  uint32_t column_address() const { return address + 24; }
  template <typename T = ScreenObjectUvColumnNode>
  T column_head() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_column_head(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_column_head(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t column_head_address() const { return address + 28; }
  uint32_t column_size() const { return guest::LoadU32(base, address + 32); }
  void set_column_size(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t column_size_address() const { return address + 32; }
  // 0 red, 1 black
  uint8_t color() const { return guest::LoadU8(base, address + 40); }
  void set_color(uint8_t v) const { guest::StoreU8(base, address + 40, v); }
  uint32_t color_address() const { return address + 40; }
  // 1 only for the head
  uint8_t is_nil() const { return guest::LoadU8(base, address + 41); }
  void set_is_nil(uint8_t v) const { guest::StoreU8(base, address + 41, v); }
  uint32_t is_nil_address() const { return address + 41; }
};

// embedded; measure: UvMap::InsertUnique.r5, UvMap::ConstructNode.r7, UvMap::ConstructNode.r7.+4
struct ScreenObjectUvMapValue : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  double key() const { return std::bit_cast<double>(guest::LoadU64(base, address + 0)); }
  void set_key(double v) const { guest::StoreU64(base, address + 0, std::bit_cast<uint64_t>(v)); }
  uint32_t key_address() const { return address + 0; }
  uint32_t key_high() const { return guest::LoadU32(base, address + 0); }
  void set_key_high(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_high_address() const { return address + 0; }
  uint32_t key_low() const { return guest::LoadU32(base, address + 4); }
  void set_key_low(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t key_low_address() const { return address + 4; }
  // a ScreenObjectUvColumn
  uint32_t column_address() const { return address + 8; }
  template <typename T = ScreenObjectUvColumnNode>
  T column_head() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_column_head(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_column_head(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t column_head_address() const { return address + 12; }
  uint32_t column_size() const { return guest::LoadU32(base, address + 16); }
  void set_column_size(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t column_size_address() const { return address + 16; }
  // measure fault: a column's +8 met here
  uint32_t fault_column_head() const { return guest::LoadU32(base, address + 8); }
  void set_fault_column_head(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fault_column_head_address() const { return address + 8; }
};

// embedded; measure: UvColumn, UvColumn::Find.r4, UvColumn::InsertUnique.r4
struct ScreenObjectUvColumn : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = ScreenObjectUvColumnNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
  // measure fault: a node's left met as the column
  uint32_t fault_node_left() const { return guest::LoadU32(base, address + 0); }
  void set_fault_node_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_node_left_address() const { return address + 0; }
  // measure fault: Erase's and InsertUnique's result out (an iterator, stored as one doubleword)
  uint64_t fault_iterator() const { return guest::LoadU64(base, address + 0); }
  void set_fault_iterator(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t fault_iterator_address() const { return address + 0; }
  // measure fault: a byte met at +8
  uint8_t fault_size_byte() const { return guest::LoadU8(base, address + 8); }
  void set_fault_size_byte(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t fault_size_byte_address() const { return address + 8; }
};

// allocated; measure: new<?>@UvColumn::AllocateNodes, UvColumn.+4.+8, UvColumn.+4.+8.+4, UvColumn.+4.+8.+4.+8, UvColumn::RotateRight.r4.+0, UvColumn::EraseSubtree.r4, UvColumn::EraseSubtree.r4.+0, UvColumn::CopySubtree.r4, UvColumn::Insert.r4.+4.+8, UvColumn::Insert.r4.+4.+8.+4, UvColumn::Insert.r4.+4.+8.+4.+8, UvColumn::InsertUnique.r4.+4, UvColumn::InsertUnique.r4.+4.+0, UvColumn::InsertUnique.r4.+4.+0.+8, sub_8212EEF8.r3.+4, sub_8212EEF8.r3.+4.+0, sub_8212EEF8.r3.+4.+8
struct ScreenObjectUvColumnNode : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  template <typename T = ScreenObjectUvColumnNode>
  T left() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_left(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t left_address() const { return address + 0; }
  template <typename T = ScreenObjectUvColumnNode>
  T parent() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_parent(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_parent(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t parent_address() const { return address + 4; }
  template <typename T = ScreenObjectUvColumnNode>
  T right() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_right(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_right(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t right_address() const { return address + 8; }
  // y rounded to 0.001
  double key() const { return std::bit_cast<double>(guest::LoadU64(base, address + 16)); }
  void set_key(double v) const { guest::StoreU64(base, address + 16, std::bit_cast<uint64_t>(v)); }
  uint32_t key_address() const { return address + 16; }
  // the key's high word
  uint32_t key_high() const { return guest::LoadU32(base, address + 16); }
  void set_key_high(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t key_high_address() const { return address + 16; }
  // the key's low word
  uint32_t key_low() const { return guest::LoadU32(base, address + 20); }
  void set_key_low(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t key_low_address() const { return address + 20; }
  // the float u's bits
  uint32_t u() const { return guest::LoadU32(base, address + 24); }
  void set_u(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t u_address() const { return address + 24; }
  // the float v's bits
  uint32_t v() const { return guest::LoadU32(base, address + 28); }
  void set_v(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t v_address() const { return address + 28; }
  // 0 red, 1 black
  uint8_t color() const { return guest::LoadU8(base, address + 32); }
  void set_color(uint8_t v) const { guest::StoreU8(base, address + 32, v); }
  uint32_t color_address() const { return address + 32; }
  // 1 only for the head
  uint8_t is_nil() const { return guest::LoadU8(base, address + 33); }
  void set_is_nil(uint8_t v) const { guest::StoreU8(base, address + 33, v); }
  uint32_t is_nil_address() const { return address + 33; }
};

// embedded; measure: UvColumn::InsertUnique.r5, UvColumn::NewNode.r7
struct ScreenObjectUvColumnValue : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  double key() const { return std::bit_cast<double>(guest::LoadU64(base, address + 0)); }
  void set_key(double v) const { guest::StoreU64(base, address + 0, std::bit_cast<uint64_t>(v)); }
  uint32_t key_address() const { return address + 0; }
  uint32_t key_high() const { return guest::LoadU32(base, address + 0); }
  void set_key_high(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_high_address() const { return address + 0; }
  uint32_t key_low() const { return guest::LoadU32(base, address + 4); }
  void set_key_low(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t key_low_address() const { return address + 4; }
  // the float u's bits
  uint32_t u() const { return guest::LoadU32(base, address + 8); }
  void set_u(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t u_address() const { return address + 8; }
  // the float v's bits
  uint32_t v() const { return guest::LoadU32(base, address + 12); }
  void set_v(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t v_address() const { return address + 12; }
};

// embedded; measure: UvColumn::LowerBoundNode.r4, UvMap::LowerBoundNode.r4, UvColumn::Find.r5, UvMap::Find.r5
struct ScreenObjectUvKey : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  double value() const { return std::bit_cast<double>(guest::LoadU64(base, address + 0)); }
  void set_value(double v) const { guest::StoreU64(base, address + 0, std::bit_cast<uint64_t>(v)); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: UvColumn::Insert.ret, UvMap::Insert.ret, sub_8212EEF8.r3
struct ScreenObjectUvIterator : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t tree() const { return guest::LoadU32(base, address + 0); }
  void set_tree(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tree_address() const { return address + 0; }
  uint32_t node() const { return guest::LoadU32(base, address + 4); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_address() const { return address + 4; }
};

// file; measure: sub_82091200.r3
struct StdThrownObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = StdThrownObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: sub_82091200.r3.+0
struct StdThrownObjectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t slot2() const { return guest::LoadU32(base, address + 8); }
  void set_slot2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot2_address() const { return address + 8; }
};

// embedded; measure: sub_820919B8.r3, sub_820915F8.r3
struct StdLogicError : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // probably
  uint32_t what() const { return guest::LoadU32(base, address + 4); }
  void set_what(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t what_address() const { return address + 4; }
};

// embedded; measure: sub_820A9330.r3, sub_820A96D0.r3.+4
struct StdWordVector : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t first() const { return guest::LoadU32(base, address + 4); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  uint32_t last() const { return guest::LoadU32(base, address + 8); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
};

// embedded; measure: sub_820A9330.ret
struct StdWordVectorElement : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: sub_820A96D0.r3
struct StdWordVectorOwner : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = StdWordVector>
  T vector() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_vector(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_vector(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vector_address() const { return address + 4; }
};

// The image words docs/functions/screen-objects.md's code uses.
struct ScreenObjectsGlobals {
  // 1000.0 (AddRingPoint's key rounding)
  static double thousand(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x82008808u)); }
  static void set_thousand(uint8_t* base, double v) { guest::StoreU64(base, 0x82008808u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t thousand_address() { return 0x82008808u; }
  // 0.5 (AddRingPoint's key rounding)
  static double half_double(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x820022B8u)); }
  static void set_half_double(uint8_t* base, double v) { guest::StoreU64(base, 0x820022B8u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t half_double_address() { return 0x820022B8u; }
  // 0.001 (AddRingPoint's key rounding)
  static double thousandth(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x82002B98u)); }
  static void set_thousandth(uint8_t* base, double v) { guest::StoreU64(base, 0x82002B98u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t thousandth_address() { return 0x82002B98u; }
  // 0.5 (BuildRings: the center)
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // pi / 180 (BuildRings)
  static float degree(uint8_t* base) { return guest::LoadFloat(base, 0x82002948u); }
  static void set_degree(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002948u, v); }
  static constexpr uint32_t degree_address() { return 0x82002948u; }
  // 0.035 (BuildRings: the radius step)
  static float ring_step(uint8_t* base) { return guest::LoadFloat(base, 0x82008810u); }
  static void set_ring_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008810u, v); }
  static constexpr uint32_t ring_step_address() { return 0x82008810u; }
  // 1.0 (SetSpeed's clamp)
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0 (SetSpeed's clamp)
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // the video output's way in (its slot 7, then that object's slot 3; app-start-up.toml's scene_target_app)
  static uint32_t video_app(uint8_t* base) { return guest::LoadU32(base, 0x8266D088u); }
  static void set_video_app(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8266D088u, v); }
  static constexpr uint32_t video_app_address() { return 0x8266D088u; }
};

}  // namespace schema
