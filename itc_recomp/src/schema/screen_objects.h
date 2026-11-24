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

// allocated; measure: UvMap.+4, UvMap.+4.+0, UvMap.+4.+8, UvMap.+4.+8.+0, new<?>@UvMap::AllocateNodes.+4, new<?>@UvMap::AllocateNodes.+4.+0, new<?>@UvMap::AllocateNodes.+4.+0.+8, new<?>@UvMap::AllocateNodes.+4.+8, new<?>@UvMap::AllocateNodes.+4.+8.+0, UvMap::EraseSubtree.r4, UvMap::EraseSubtree.r4.+0, UvMap.+4.+0.+8
#pragma pack(push, 1)
struct ScreenObjectUvMapNodeData {
  uint32_t left = 0;  // +0
  uint32_t parent = 0;  // +4
  uint32_t right = 0;  // +8
  uint8_t pad_12[4] = {};
  double key = 0;  // +16
  uint8_t column[12] = {};  // +24, 1 x ScreenObjectUvColumn
  uint8_t pad_36[4] = {};
  uint8_t color = 0;  // +40
  uint8_t is_nil = 0;  // +41
  uint8_t pad_42[6] = {};
};
#pragma pack(pop)
static_assert(sizeof(ScreenObjectUvMapNodeData) == 48);
inline constexpr memory::LayoutField kScreenObjectUvMapNodeFields[] = {{0, 4}, {4, 4}, {8, 4}, {16, 8}, {24, 4}, {28, 4}, {32, 4}};
inline constexpr memory::Layout kScreenObjectUvMapNodeLayout = {kScreenObjectUvMapNodeFields, 7};

struct ScreenObjectUvMapNode : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ScreenObjectUvMapNodeData* data() const { return static_cast<ScreenObjectUvMapNodeData*>(memory::HandleObject(address)); }
  static ScreenObjectUvMapNode New(uint8_t* base) {
    return ScreenObjectUvMapNode(base, memory::MakeHandle(new ScreenObjectUvMapNodeData(), sizeof(ScreenObjectUvMapNodeData), "ScreenObjectUvMapNode", &kScreenObjectUvMapNodeLayout));
  }
  template <uint32_t Stride = kSize>
  static ScreenObjectUvMapNode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ScreenObjectUvMapNodeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ScreenObjectUvMapNodeData*>(::operator new(sizeof(ScreenObjectUvMapNodeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ScreenObjectUvMapNodeData();
    return ScreenObjectUvMapNode(base, memory::MakeHandle(p, sizeof(ScreenObjectUvMapNodeData) * count, "ScreenObjectUvMapNode", &kScreenObjectUvMapNodeLayout, sizeof(ScreenObjectUvMapNodeData)));
  }
  ScreenObjectUvMapNode at(uint32_t i) const { return ScreenObjectUvMapNode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ScreenObjectUvMapNodeData* direct() const { return static_cast<ScreenObjectUvMapNodeData*>(memory::HandleOwn(address, &kScreenObjectUvMapNodeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kScreenObjectUvMapNodeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvMapNodeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t left_address() const { return address + 0; }
  uint32_t left_bits() const { if (auto* d = direct()) return d->left; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_left_bits(uint32_t v) const { if (auto* d = direct()) { d->left = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  template <typename T = ScreenObjectUvMapNode>
  T left() const { return T(base, left_bits()); }
  void set_left(const View& w) const { set_left_bits(w.address); }
  void set_left(uint32_t v) const { set_left_bits(v); }
  uint32_t parent_address() const { return address + 4; }
  uint32_t parent_bits() const { if (auto* d = direct()) return d->parent; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_parent_bits(uint32_t v) const { if (auto* d = direct()) { d->parent = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = ScreenObjectUvMapNode>
  T parent() const { return T(base, parent_bits()); }
  void set_parent(const View& w) const { set_parent_bits(w.address); }
  void set_parent(uint32_t v) const { set_parent_bits(v); }
  uint32_t right_address() const { return address + 8; }
  uint32_t right_bits() const { if (auto* d = direct()) return d->right; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_right_bits(uint32_t v) const { if (auto* d = direct()) { d->right = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  template <typename T = ScreenObjectUvMapNode>
  T right() const { return T(base, right_bits()); }
  void set_right(const View& w) const { set_right_bits(w.address); }
  void set_right(uint32_t v) const { set_right_bits(v); }
  // x rounded to 0.001
  uint32_t key_address() const { return address + 16; }
  uint64_t key_bits() const { if (auto* d = direct()) return std::bit_cast<uint64_t>(d->key); return uint64_t((memory::IsHandle(address + 16) ? uint64_t(memory::OwnerLoad(address + 16, 8)) : guest::LoadU64(base, address + 16))); }
  void set_key_bits(uint64_t v) const { if (auto* d = direct()) { d->key = std::bit_cast<double>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 8, v); else guest::StoreU64(base, address + 16, v); }
  double key() const { return std::bit_cast<double>(key_bits()); }
  void set_key(double v) const { set_key_bits(std::bit_cast<uint64_t>(v)); }
  // the key's high word
  uint32_t key_high_address() const { return address + 16; }
  uint32_t key_high_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvMapNodeLayout, 16, 4)); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_key_high_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvMapNodeLayout, 16, 4, v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t key_high() const { return uint32_t(key_high_bits()); }
  void set_key_high(uint32_t v) const { set_key_high_bits(uint32_t(v)); }
  // the key's low word
  uint32_t key_low_address() const { return address + 20; }
  uint32_t key_low_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvMapNodeLayout, 20, 4)); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_key_low_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvMapNodeLayout, 20, 4, v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t key_low() const { return uint32_t(key_low_bits()); }
  void set_key_low(uint32_t v) const { set_key_low_bits(uint32_t(v)); }
  // its head at +28, its size at +32
  template <typename T = ScreenObjectUvColumn>
  T column() const { return T(base, address + 24); }
  // 0 red, 1 black
  uint32_t color_address() const { return address + 40; }
  uint8_t color_bits() const { if (auto* d = direct()) return d->color; return uint8_t((memory::IsHandle(address + 40) ? uint8_t(memory::OwnerLoad(address + 40, 1)) : guest::LoadU8(base, address + 40))); }
  void set_color_bits(uint8_t v) const { if (auto* d = direct()) { d->color = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 1, v); else guest::StoreU8(base, address + 40, v); }
  uint8_t color() const { if (auto* d = direct()) return d->color; return uint8_t((memory::IsHandle(address + 40) ? uint8_t(memory::OwnerLoad(address + 40, 1)) : guest::LoadU8(base, address + 40))); }
  void set_color(uint8_t v) const { if (auto* d = direct()) { d->color = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 1, v); else guest::StoreU8(base, address + 40, v); }
  // 1 only for the head
  uint32_t is_nil_address() const { return address + 41; }
  uint8_t is_nil_bits() const { if (auto* d = direct()) return d->is_nil; return uint8_t((memory::IsHandle(address + 41) ? uint8_t(memory::OwnerLoad(address + 41, 1)) : guest::LoadU8(base, address + 41))); }
  void set_is_nil_bits(uint8_t v) const { if (auto* d = direct()) { d->is_nil = v; return; } if (memory::IsHandle(address + 41)) memory::OwnerStore(address + 41, 1, v); else guest::StoreU8(base, address + 41, v); }
  uint8_t is_nil() const { if (auto* d = direct()) return d->is_nil; return uint8_t((memory::IsHandle(address + 41) ? uint8_t(memory::OwnerLoad(address + 41, 1)) : guest::LoadU8(base, address + 41))); }
  void set_is_nil(uint8_t v) const { if (auto* d = direct()) { d->is_nil = v; return; } if (memory::IsHandle(address + 41)) memory::OwnerStore(address + 41, 1, v); else guest::StoreU8(base, address + 41, v); }
};

// embedded; measure: UvMap::InsertUnique.r5, UvMap::ConstructNode.r7, UvMap::ConstructNode.r7@+8.+4
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
};

// embedded; measure: UvColumn, UvColumn::Find.r4, UvColumn::InsertUnique.r4
#pragma pack(push, 1)
struct ScreenObjectUvColumnData {
  uint32_t fault_node_left = 0;  // +0
  uint32_t head = 0;  // +4
  uint32_t size = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(ScreenObjectUvColumnData) == 12);
inline constexpr memory::LayoutField kScreenObjectUvColumnFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kScreenObjectUvColumnLayout = {kScreenObjectUvColumnFields, 3};

struct ScreenObjectUvColumn : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ScreenObjectUvColumnData* data() const { return static_cast<ScreenObjectUvColumnData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ScreenObjectUvColumn New(uint8_t* base) {
    return ScreenObjectUvColumn(base, memory::MakeHandle(new ScreenObjectUvColumnData(), sizeof(ScreenObjectUvColumnData), "ScreenObjectUvColumn", &kScreenObjectUvColumnLayout));
  }
  template <uint32_t Stride = kSize>
  static ScreenObjectUvColumn NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ScreenObjectUvColumnData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ScreenObjectUvColumnData*>(::operator new(sizeof(ScreenObjectUvColumnData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ScreenObjectUvColumnData();
    return ScreenObjectUvColumn(base, memory::MakeHandle(p, sizeof(ScreenObjectUvColumnData) * count, "ScreenObjectUvColumn", &kScreenObjectUvColumnLayout, sizeof(ScreenObjectUvColumnData)));
  }
  ScreenObjectUvColumn at(uint32_t i) const { return ScreenObjectUvColumn(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ScreenObjectUvColumnData* direct() const { return static_cast<ScreenObjectUvColumnData*>(memory::HandleOwn(address, &kScreenObjectUvColumnLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kScreenObjectUvColumnLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t head_bits() const { if (auto* d = direct()) return d->head; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_head_bits(uint32_t v) const { if (auto* d = direct()) { d->head = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = ScreenObjectUvColumnNode>
  T head() const { return T(base, head_bits()); }
  void set_head(const View& w) const { set_head_bits(w.address); }
  void set_head(uint32_t v) const { set_head_bits(v); }
  uint32_t size_address() const { return address + 8; }
  uint32_t size_bits() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_size_bits(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t size() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_size(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // measure fault: a node's left met as the column
  uint32_t fault_node_left_address() const { return address + 0; }
  uint32_t fault_node_left_bits() const { if (auto* d = direct()) return d->fault_node_left; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_fault_node_left_bits(uint32_t v) const { if (auto* d = direct()) { d->fault_node_left = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t fault_node_left() const { if (auto* d = direct()) return d->fault_node_left; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_fault_node_left(uint32_t v) const { if (auto* d = direct()) { d->fault_node_left = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // measure fault: Erase's and InsertUnique's result out (an iterator, stored as one doubleword)
  uint32_t fault_iterator_address() const { return address + 0; }
  uint64_t fault_iterator_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(base, address + 0))); }
  void set_fault_iterator_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  uint64_t fault_iterator() const { return uint64_t(fault_iterator_bits()); }
  void set_fault_iterator(uint64_t v) const { set_fault_iterator_bits(uint64_t(v)); }
  // measure fault: a byte met at +8
  uint32_t fault_size_byte_address() const { return address + 8; }
  uint8_t fault_size_byte_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnLayout, 8, 1)); return uint8_t((memory::IsHandle(address + 8) ? uint8_t(memory::OwnerLoad(address + 8, 1)) : guest::LoadU8(base, address + 8))); }
  void set_fault_size_byte_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnLayout, 8, 1, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 1, v); else guest::StoreU8(base, address + 8, v); }
  uint8_t fault_size_byte() const { return uint8_t(fault_size_byte_bits()); }
  void set_fault_size_byte(uint8_t v) const { set_fault_size_byte_bits(uint8_t(v)); }
};

// allocated; measure: new<?>@UvColumn::AllocateNodes, UvColumn.+4.+8, UvColumn.+4.+8.+4, UvColumn.+4.+8.+4.+8, UvColumn::RotateRight.r4.+0, UvColumn::EraseSubtree.r4, UvColumn::EraseSubtree.r4.+0, UvColumn::CopySubtree.r4, UvColumn::Insert.r4.+4.+8, UvColumn::Insert.r4.+4.+8.+4, UvColumn::Insert.r4.+4.+8.+4.+8, UvColumn::InsertUnique.r4.+4, UvColumn::InsertUnique.r4.+4.+0, UvColumn::InsertUnique.r4.+4.+0.+8, sub_8212EEF8.r3.+4, sub_8212EEF8.r3.+4.+0, sub_8212EEF8.r3.+4.+8
#pragma pack(push, 1)
struct ScreenObjectUvColumnNodeData {
  uint32_t left = 0;  // +0
  uint32_t parent = 0;  // +4
  uint32_t right = 0;  // +8
  uint8_t pad_12[4] = {};
  double key = 0;  // +16
  uint32_t u = 0;  // +24
  uint32_t v = 0;  // +28
  uint8_t color = 0;  // +32
  uint8_t is_nil = 0;  // +33
  uint8_t pad_34[6] = {};
};
#pragma pack(pop)
static_assert(sizeof(ScreenObjectUvColumnNodeData) == 40);
inline constexpr memory::LayoutField kScreenObjectUvColumnNodeFields[] = {{0, 4}, {4, 4}, {8, 4}, {16, 8}, {24, 4}, {28, 4}};
inline constexpr memory::Layout kScreenObjectUvColumnNodeLayout = {kScreenObjectUvColumnNodeFields, 6};

struct ScreenObjectUvColumnNode : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ScreenObjectUvColumnNodeData* data() const { return static_cast<ScreenObjectUvColumnNodeData*>(memory::HandleObject(address)); }
  static ScreenObjectUvColumnNode New(uint8_t* base) {
    return ScreenObjectUvColumnNode(base, memory::MakeHandle(new ScreenObjectUvColumnNodeData(), sizeof(ScreenObjectUvColumnNodeData), "ScreenObjectUvColumnNode", &kScreenObjectUvColumnNodeLayout));
  }
  template <uint32_t Stride = kSize>
  static ScreenObjectUvColumnNode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ScreenObjectUvColumnNodeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ScreenObjectUvColumnNodeData*>(::operator new(sizeof(ScreenObjectUvColumnNodeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ScreenObjectUvColumnNodeData();
    return ScreenObjectUvColumnNode(base, memory::MakeHandle(p, sizeof(ScreenObjectUvColumnNodeData) * count, "ScreenObjectUvColumnNode", &kScreenObjectUvColumnNodeLayout, sizeof(ScreenObjectUvColumnNodeData)));
  }
  ScreenObjectUvColumnNode at(uint32_t i) const { return ScreenObjectUvColumnNode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ScreenObjectUvColumnNodeData* direct() const { return static_cast<ScreenObjectUvColumnNodeData*>(memory::HandleOwn(address, &kScreenObjectUvColumnNodeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kScreenObjectUvColumnNodeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnNodeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t left_address() const { return address + 0; }
  uint32_t left_bits() const { if (auto* d = direct()) return d->left; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_left_bits(uint32_t v) const { if (auto* d = direct()) { d->left = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  template <typename T = ScreenObjectUvColumnNode>
  T left() const { return T(base, left_bits()); }
  void set_left(const View& w) const { set_left_bits(w.address); }
  void set_left(uint32_t v) const { set_left_bits(v); }
  uint32_t parent_address() const { return address + 4; }
  uint32_t parent_bits() const { if (auto* d = direct()) return d->parent; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_parent_bits(uint32_t v) const { if (auto* d = direct()) { d->parent = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = ScreenObjectUvColumnNode>
  T parent() const { return T(base, parent_bits()); }
  void set_parent(const View& w) const { set_parent_bits(w.address); }
  void set_parent(uint32_t v) const { set_parent_bits(v); }
  uint32_t right_address() const { return address + 8; }
  uint32_t right_bits() const { if (auto* d = direct()) return d->right; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_right_bits(uint32_t v) const { if (auto* d = direct()) { d->right = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  template <typename T = ScreenObjectUvColumnNode>
  T right() const { return T(base, right_bits()); }
  void set_right(const View& w) const { set_right_bits(w.address); }
  void set_right(uint32_t v) const { set_right_bits(v); }
  // y rounded to 0.001
  uint32_t key_address() const { return address + 16; }
  uint64_t key_bits() const { if (auto* d = direct()) return std::bit_cast<uint64_t>(d->key); return uint64_t((memory::IsHandle(address + 16) ? uint64_t(memory::OwnerLoad(address + 16, 8)) : guest::LoadU64(base, address + 16))); }
  void set_key_bits(uint64_t v) const { if (auto* d = direct()) { d->key = std::bit_cast<double>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 8, v); else guest::StoreU64(base, address + 16, v); }
  double key() const { return std::bit_cast<double>(key_bits()); }
  void set_key(double v) const { set_key_bits(std::bit_cast<uint64_t>(v)); }
  // the key's high word
  uint32_t key_high_address() const { return address + 16; }
  uint32_t key_high_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnNodeLayout, 16, 4)); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_key_high_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnNodeLayout, 16, 4, v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t key_high() const { return uint32_t(key_high_bits()); }
  void set_key_high(uint32_t v) const { set_key_high_bits(uint32_t(v)); }
  // the key's low word
  uint32_t key_low_address() const { return address + 20; }
  uint32_t key_low_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnNodeLayout, 20, 4)); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_key_low_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnNodeLayout, 20, 4, v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t key_low() const { return uint32_t(key_low_bits()); }
  void set_key_low(uint32_t v) const { set_key_low_bits(uint32_t(v)); }
  // the float u's bits
  uint32_t u_address() const { return address + 24; }
  uint32_t u_bits() const { if (auto* d = direct()) return d->u; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_u_bits(uint32_t v) const { if (auto* d = direct()) { d->u = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t u() const { if (auto* d = direct()) return d->u; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_u(uint32_t v) const { if (auto* d = direct()) { d->u = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // the float v's bits
  uint32_t v_address() const { return address + 28; }
  uint32_t v_bits() const { if (auto* d = direct()) return d->v; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_v_bits(uint32_t v) const { if (auto* d = direct()) { d->v = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t v() const { if (auto* d = direct()) return d->v; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_v(uint32_t v) const { if (auto* d = direct()) { d->v = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // 0 red, 1 black
  uint32_t color_address() const { return address + 32; }
  uint8_t color_bits() const { if (auto* d = direct()) return d->color; return uint8_t((memory::IsHandle(address + 32) ? uint8_t(memory::OwnerLoad(address + 32, 1)) : guest::LoadU8(base, address + 32))); }
  void set_color_bits(uint8_t v) const { if (auto* d = direct()) { d->color = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 1, v); else guest::StoreU8(base, address + 32, v); }
  uint8_t color() const { if (auto* d = direct()) return d->color; return uint8_t((memory::IsHandle(address + 32) ? uint8_t(memory::OwnerLoad(address + 32, 1)) : guest::LoadU8(base, address + 32))); }
  void set_color(uint8_t v) const { if (auto* d = direct()) { d->color = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 1, v); else guest::StoreU8(base, address + 32, v); }
  // 1 only for the head
  uint32_t is_nil_address() const { return address + 33; }
  uint8_t is_nil_bits() const { if (auto* d = direct()) return d->is_nil; return uint8_t((memory::IsHandle(address + 33) ? uint8_t(memory::OwnerLoad(address + 33, 1)) : guest::LoadU8(base, address + 33))); }
  void set_is_nil_bits(uint8_t v) const { if (auto* d = direct()) { d->is_nil = v; return; } if (memory::IsHandle(address + 33)) memory::OwnerStore(address + 33, 1, v); else guest::StoreU8(base, address + 33, v); }
  uint8_t is_nil() const { if (auto* d = direct()) return d->is_nil; return uint8_t((memory::IsHandle(address + 33) ? uint8_t(memory::OwnerLoad(address + 33, 1)) : guest::LoadU8(base, address + 33))); }
  void set_is_nil(uint8_t v) const { if (auto* d = direct()) { d->is_nil = v; return; } if (memory::IsHandle(address + 33)) memory::OwnerStore(address + 33, 1, v); else guest::StoreU8(base, address + 33, v); }
};

// embedded; measure: UvColumn::InsertUnique.r5, UvColumn::NewNode.r7
#pragma pack(push, 1)
struct ScreenObjectUvColumnValueData {
  double key = 0;  // +0
  uint32_t u = 0;  // +8
  uint32_t v = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(ScreenObjectUvColumnValueData) == 16);
inline constexpr memory::LayoutField kScreenObjectUvColumnValueFields[] = {{0, 8}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kScreenObjectUvColumnValueLayout = {kScreenObjectUvColumnValueFields, 3};

struct ScreenObjectUvColumnValue : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ScreenObjectUvColumnValueData* data() const { return static_cast<ScreenObjectUvColumnValueData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ScreenObjectUvColumnValue New(uint8_t* base) {
    return ScreenObjectUvColumnValue(base, memory::MakeHandle(new ScreenObjectUvColumnValueData(), sizeof(ScreenObjectUvColumnValueData), "ScreenObjectUvColumnValue", &kScreenObjectUvColumnValueLayout));
  }
  template <uint32_t Stride = kSize>
  static ScreenObjectUvColumnValue NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ScreenObjectUvColumnValueData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ScreenObjectUvColumnValueData*>(::operator new(sizeof(ScreenObjectUvColumnValueData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ScreenObjectUvColumnValueData();
    return ScreenObjectUvColumnValue(base, memory::MakeHandle(p, sizeof(ScreenObjectUvColumnValueData) * count, "ScreenObjectUvColumnValue", &kScreenObjectUvColumnValueLayout, sizeof(ScreenObjectUvColumnValueData)));
  }
  ScreenObjectUvColumnValue at(uint32_t i) const { return ScreenObjectUvColumnValue(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ScreenObjectUvColumnValueData* direct() const { return static_cast<ScreenObjectUvColumnValueData*>(memory::HandleOwn(address, &kScreenObjectUvColumnValueLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kScreenObjectUvColumnValueLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnValueLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t key_address() const { return address + 0; }
  uint64_t key_bits() const { if (auto* d = direct()) return std::bit_cast<uint64_t>(d->key); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(base, address + 0))); }
  void set_key_bits(uint64_t v) const { if (auto* d = direct()) { d->key = std::bit_cast<double>(v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  double key() const { return std::bit_cast<double>(key_bits()); }
  void set_key(double v) const { set_key_bits(std::bit_cast<uint64_t>(v)); }
  uint32_t key_high_address() const { return address + 0; }
  uint32_t key_high_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnValueLayout, 0, 4)); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_key_high_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnValueLayout, 0, 4, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t key_high() const { return uint32_t(key_high_bits()); }
  void set_key_high(uint32_t v) const { set_key_high_bits(uint32_t(v)); }
  uint32_t key_low_address() const { return address + 4; }
  uint32_t key_low_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnValueLayout, 4, 4)); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_key_low_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScreenObjectUvColumnValueLayout, 4, 4, v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t key_low() const { return uint32_t(key_low_bits()); }
  void set_key_low(uint32_t v) const { set_key_low_bits(uint32_t(v)); }
  // the float u's bits
  uint32_t u_address() const { return address + 8; }
  uint32_t u_bits() const { if (auto* d = direct()) return d->u; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_u_bits(uint32_t v) const { if (auto* d = direct()) { d->u = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t u() const { if (auto* d = direct()) return d->u; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_u(uint32_t v) const { if (auto* d = direct()) { d->u = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the float v's bits
  uint32_t v_address() const { return address + 12; }
  uint32_t v_bits() const { if (auto* d = direct()) return d->v; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_v_bits(uint32_t v) const { if (auto* d = direct()) { d->v = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t v() const { if (auto* d = direct()) return d->v; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_v(uint32_t v) const { if (auto* d = direct()) { d->v = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
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

// embedded; measure: UvMap::ConstructNode.r7@+8
struct ScreenObjectUvMapValueAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: ScreenObject::~ScreenObject.r3@+76124
struct ScreenObjectUvMapAt76124 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: ScreenObject::~ScreenObject.r3@+76124.+4
struct ScreenObjectUvMapNodePart : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: ScreenObject@+76124
struct ScreenObjectUvMapAt76124_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: ScreenObject@+76124.+4.+8.+4
struct ScreenObjectUvMapNodePart_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: UvMap::EraseSubtree.r4@+24
struct ScreenObjectUvColumnAt24 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: UvMap::EraseSubtree.r4@+24.+4
struct ScreenObjectUvColumnNodePart : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: UvMap@+24
struct ScreenObjectUvColumnAt24_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: new<?>@UvColumn::AllocateNodes.+4
struct ScreenObjectUvColumnNodePart_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 34;
  uint8_t unknown_33() const { return guest::LoadU8(base, address + 33); }
  void set_unknown_33(uint8_t v) const { guest::StoreU8(base, address + 33, v); }
  uint32_t unknown_33_address() const { return address + 33; }
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
};

}  // namespace schema
