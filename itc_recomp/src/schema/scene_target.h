// The views of docs/functions/scene-target.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/scene-target.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: SceneTarget
struct SceneTarget : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // the 1024 x 576 depth surface (SceneTarget::RecordTargets' command 24)
  template <typename T = SurfaceHeader>
  T depth() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_depth(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_depth(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t depth_address() const { return address + 0; }
  // the 1024 x 576 color surface (render targets 0-3)
  template <typename T = SurfaceHeader>
  T color() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_color(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_color(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t color_address() const { return address + 4; }
  // the depth surface at the display's largest tile (1280 x 256, multisampled), for the menus' predicated tiling
  template <typename T = SurfaceHeader>
  T tile_depth() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_tile_depth(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_tile_depth(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t tile_depth_address() const { return address + 8; }
  // the color surface at the largest tile
  template <typename T = SurfaceHeader>
  T tile_color() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_tile_color(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_tile_color(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t tile_color_address() const { return address + 12; }
  // 1024
  uint32_t width() const { return guest::LoadU32(base, address + 16); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t width_address() const { return address + 16; }
  // 576
  uint32_t height() const { return guest::LoadU32(base, address + 20); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // the 1280 x 720 texture the passes resolve into and draw back (Device::CreateTexture's, sub_822A5D08)
  uint32_t texture() const { return guest::LoadU32(base, address + 24); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t texture_address() const { return address + 24; }
  // Modes::SetSceneTargetMode's (mode-tick.md): 0 gives the pass bits 7, 1 4, 2 0, 3 1, 4 8, above 4 0
  uint32_t mode() const { return guest::LoadU32(base, address + 28); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t mode_address() const { return address + 28; }
};

// embedded; measure: TexturedRect
struct TexturedRect : View {
  using View::View;
  static constexpr uint32_t kSize = 120;
  // an identity matrix (RecordPasses' vector unit); not read by the recorder
  uint32_t matrix_address() const { return address + 0; }
  // 0 from RecordPasses; not read by the recorder (probably ColorRect's kind)
  uint32_t kind() const { return guest::LoadU32(base, address + 64); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t kind_address() const { return address + 64; }
  // 0 from RecordPasses
  uint32_t unknown_68() const { return guest::LoadU32(base, address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  // 2 from RecordPasses
  uint32_t unknown_72() const { return guest::LoadU32(base, address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  int32_t x() const { return int32_t(guest::LoadU32(base, address + 80)); }
  void set_x(int32_t v) const { guest::StoreU32(base, address + 80, uint32_t(v)); }
  uint32_t x_address() const { return address + 80; }
  int32_t y() const { return int32_t(guest::LoadU32(base, address + 84)); }
  void set_y(int32_t v) const { guest::StoreU32(base, address + 84, uint32_t(v)); }
  uint32_t y_address() const { return address + 84; }
  uint32_t width() const { return guest::LoadU32(base, address + 88); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t width_address() const { return address + 88; }
  uint32_t height() const { return guest::LoadU32(base, address + 92); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t height_address() const { return address + 92; }
  float u0() const { return guest::LoadFloat(base, address + 96); }
  void set_u0(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t u0_address() const { return address + 96; }
  float v0() const { return guest::LoadFloat(base, address + 100); }
  void set_v0(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t v0_address() const { return address + 100; }
  float u1() const { return guest::LoadFloat(base, address + 104); }
  void set_u1(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t u1_address() const { return address + 104; }
  float v1() const { return guest::LoadFloat(base, address + 108); }
  void set_v1(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t v1_address() const { return address + 108; }
  // the texture (SetTexture, sampler 0)
  uint32_t texture() const { return guest::LoadU32(base, address + 112); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t texture_address() const { return address + 112; }
  // not 0: blending on
  uint32_t blend() const { return guest::LoadU32(base, address + 116); }
  void set_blend(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t blend_address() const { return address + 116; }
};

// The image words docs/functions/scene-target.md's code uses.
struct SceneTargetGlobals {
  template <typename T = SceneTarget>
  static T scene_target(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8266D08Cu)); }
  static void set_scene_target(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8266D08Cu, v.address); }
  static void set_scene_target(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8266D08Cu, v); }
  static constexpr uint32_t scene_target_address() { return 0x8266D08Cu; }
  // the textured and color rectangles' render queue (ColorRect::SetTargets, frame-driver.md)
  static uint32_t rect_queue(uint8_t* base) { return guest::LoadU32(base, 0x826D83D0u); }
  static void set_rect_queue(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83D0u, v); }
  static constexpr uint32_t rect_queue_address() { return 0x826D83D0u; }
  // the rectangles' command list
  static uint32_t rect_list(uint8_t* base) { return guest::LoadU32(base, 0x826D83D4u); }
  static void set_rect_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83D4u, v); }
  static constexpr uint32_t rect_list_address() { return 0x826D83D4u; }
  // the display's tiling block (app-start-up.md's Tiling): +12 the tile count, +16 the tiles
  static uint32_t display_tiling(uint8_t* base) { return guest::LoadU32(base, 0x8260034Cu); }
  static void set_display_tiling(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8260034Cu, v); }
  static constexpr uint32_t display_tiling_address() { return 0x8260034Cu; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 0.8 (1024 / 1280, 576 / 720): the upscale draw's texture coordinates
  static float four_fifths(uint8_t* base) { return guest::LoadFloat(base, 0x82001070u); }
  static void set_four_fifths(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001070u, v); }
  static constexpr uint32_t four_fifths_address() { return 0x82001070u; }
};

}  // namespace schema
