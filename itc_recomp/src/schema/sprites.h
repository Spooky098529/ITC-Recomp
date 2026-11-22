// The views of docs/functions/sprites.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/sprites.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Billboards
struct BillboardPass : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = CameraPass>
  T camera_pass() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_camera_pass(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t camera_pass_address() const { return address + 0; }
  // the render queue layer Record hands the chunk to (SetBillboardPass' word28)
  uint32_t layer() const { return guest::LoadU32(base, address + 4); }
  void set_layer(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t layer_address() const { return address + 4; }
};

// stack; measure: Billboards::RecordState.r5, Billboards::Draw.r4
struct BillboardMaterial : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // 1 at Init; LightGroups::Draw: 1024 (flag 0x200), else 64 (0x10) or 32; signed in the compares
  uint32_t alpha_mode() const { return guest::LoadU32(base, address + 0); }
  void set_alpha_mode(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t alpha_mode_address() const { return address + 0; }
  // 1 at Init; LightGroups::Draw: 2 (flag 0x40) or 4
  uint32_t blend_mode() const { return guest::LoadU32(base, address + 4); }
  void set_blend_mode(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t blend_mode_address() const { return address + 4; }
  // 0 at Init
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // 0 at Init; LightGroups::Draw: XPR::GetTexture's; none in stages 0 to 3 when 0
  uint32_t texture() const { return guest::LoadU32(base, address + 12); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t texture_address() const { return address + 12; }
  // 0 at Init
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // 0 at Init; SetLights
  uint32_t light_count() const { return guest::LoadU32(base, address + 32); }
  void set_light_count(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t light_count_address() const { return address + 32; }
  // 0 at Init; SetLights; light_count of them, 48 bytes each
  template <typename T = BillboardLight>
  T lights() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_lights(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_lights(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t lights_address() const { return address + 36; }
};

// file; measure: Billboards::RecordState.r5.+36
struct BillboardLight : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // the position
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // 0 to 255
  float red() const { return guest::LoadFloat(base, address + 16); }
  void set_red(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t red_address() const { return address + 16; }
  float green() const { return guest::LoadFloat(base, address + 20); }
  void set_green(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t green_address() const { return address + 20; }
  float blue() const { return guest::LoadFloat(base, address + 24); }
  void set_blue(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t blue_address() const { return address + 24; }
  float alpha() const { return guest::LoadFloat(base, address + 28); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t alpha_address() const { return address + 28; }
  // drawn when it or the height is above 0.0
  float width() const { return guest::LoadFloat(base, address + 32); }
  void set_width(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t width_address() const { return address + 32; }
  float height() const { return guest::LoadFloat(base, address + 36); }
  void set_height(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t height_address() const { return address + 36; }
};

// file; measure: CourseSet.+1060
struct LightGroups : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  // the groups
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the first LightGroup; each is followed by its lights (48 bytes each), then the next
  uint32_t first_group_address() const { return address + 16; }
};

// file; measure: -
struct LightGroup : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // 0x200 alpha mode 1024, 0x10 64 (else 32); 0x40 blend mode 2 (else 4)
  uint32_t flags() const { return guest::LoadU32(base, address + 0); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t flags_address() const { return address + 0; }
  // the XPR's texture id, or a texture animation's
  uint32_t texture_id() const { return guest::LoadU32(base, address + 4); }
  void set_texture_id(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t texture_id_address() const { return address + 4; }
  // the BillboardLights from +60
  uint32_t light_count() const { return guest::LoadU32(base, address + 44); }
  void set_light_count(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t light_count_address() const { return address + 44; }
};

// file; measure: AnimationSet::CurrentFrame.ret
struct AnimationFrameEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // handed to the texture provider's slot 5 with +4 (SceneryModel::RecordShape)
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t texture_id() const { return guest::LoadU32(base, address + 4); }
  void set_texture_id(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t texture_id_address() const { return address + 4; }
};

// The image words docs/functions/sprites.md's code uses.
struct SpritesGlobals {
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 1.0, copied as bits
  static uint32_t one_word(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_one_word(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t one_word_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // the billboards' draw distance's cap
  static float farthest(uint8_t* base) { return guest::LoadFloat(base, 0x82001BDCu); }
  static void set_farthest(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BDCu, v); }
  static constexpr uint32_t farthest_address() { return 0x82001BDCu; }
  // w of the billboard light's offset (read, not used)
  static float offset_w(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_offset_w(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t offset_w_address() { return 0x82001BE0u; }
  // the billboard light's width and height scale; the sprite list's texture coordinate end
  static float size_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_size_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t size_scale_address() { return 0x82001BE4u; }
  // the billboard quad's forward axis' scale
  static float forward_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE8u); }
  static void set_forward_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE8u, v); }
  static constexpr uint32_t forward_scale_address() { return 0x82001BE8u; }
  // the billboards' 1024 alpha mode's reference
  static float alpha_ref(uint8_t* base) { return guest::LoadFloat(base, 0x8266D090u); }
  static void set_alpha_ref(uint8_t* base, float v) { guest::StoreFloat(base, 0x8266D090u, v); }
  static constexpr uint32_t alpha_ref_address() { return 0x8266D090u; }
  // the billboards' vertex ring: two halves of 4,096 28-byte vertices
  static constexpr uint32_t ring_vertices_address() { return 0x8266D098u; }
  // the ring's half written this pass, 0 or 1
  static uint32_t ring_half(uint8_t* base) { return guest::LoadU32(base, 0x826A5098u); }
  static void set_ring_half(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5098u, v); }
  static constexpr uint32_t ring_half_address() { return 0x826A5098u; }
  // the vertices written into the ring's half
  static uint32_t ring_count(uint8_t* base) { return guest::LoadU32(base, 0x826A509Cu); }
  static void set_ring_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A509Cu, v); }
  static constexpr uint32_t ring_count_address() { return 0x826A509Cu; }
  // the billboards' (app-start-up.toml)
  static uint32_t vertex_shader(uint8_t* base) { return guest::LoadU32(base, 0x826A50A0u); }
  static void set_vertex_shader(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50A0u, v); }
  static constexpr uint32_t vertex_shader_address() { return 0x826A50A0u; }
  static uint32_t pixel_shader(uint8_t* base) { return guest::LoadU32(base, 0x826A50A4u); }
  static void set_pixel_shader(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50A4u, v); }
  static constexpr uint32_t pixel_shader_address() { return 0x826A50A4u; }
  // the billboards' vertex declaration, recorded with command 10
  static uint32_t ring_declaration(uint8_t* base) { return guest::LoadU32(base, 0x826A50A8u); }
  static void set_ring_declaration(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50A8u, v); }
  static constexpr uint32_t ring_declaration_address() { return 0x826A50A8u; }
  // the render manager (its slot 8, the scene)
  static uint32_t render_manager(uint8_t* base) { return guest::LoadU32(base, 0x826BABA0u); }
  static void set_render_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABA0u, v); }
  static constexpr uint32_t render_manager_address() { return 0x826BABA0u; }
  // the sprite recorders' render queue (race-overlays.toml)
  static uint32_t render_queue(uint8_t* base) { return guest::LoadU32(base, 0x826C5D5Cu); }
  static void set_render_queue(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5D5Cu, v); }
  static constexpr uint32_t render_queue_address() { return 0x826C5D5Cu; }
  // the sprite recorders' command list
  static uint32_t command_list(uint8_t* base) { return guest::LoadU32(base, 0x826C5D60u); }
  static void set_command_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5D60u, v); }
  static constexpr uint32_t command_list_address() { return 0x826C5D60u; }
};

}  // namespace schema
