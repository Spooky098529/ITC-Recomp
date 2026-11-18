// The views of docs/functions/material-passes.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/material-passes.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: Model::Material.ret, Model::MaterialAt.ret
struct MaterialRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 512;
  // vertex c13: the paint's first color (SetPaint's A, the palette's), scaled by ScaleBlended below 1.1
  float color_r() const { return guest::LoadFloat(base, address + 0); }
  void set_color_r(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t color_r_address() const { return address + 0; }
  // +0 to +7 as a doubleword
  uint64_t color_xy() const { return guest::LoadU64(base, address + 0); }
  void set_color_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t color_xy_address() const { return address + 0; }
  float color_g() const { return guest::LoadFloat(base, address + 4); }
  void set_color_g(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t color_g_address() const { return address + 4; }
  float color_b() const { return guest::LoadFloat(base, address + 8); }
  void set_color_b(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t color_b_address() const { return address + 8; }
  // +8 to +15 as a doubleword
  uint64_t color_zw() const { return guest::LoadU64(base, address + 8); }
  void set_color_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t color_zw_address() const { return address + 8; }
  // the alpha (SetAlpha, SetPair1002Alpha; 1.0 by ScaleBlended)
  float color_a() const { return guest::LoadFloat(base, address + 12); }
  void set_color_a(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t color_a_address() const { return address + 12; }
  // pixel c65 (w from +88): the palette's third color
  float pixel_c65_r() const { return guest::LoadFloat(base, address + 16); }
  void set_pixel_c65_r(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t pixel_c65_r_address() const { return address + 16; }
  uint64_t pixel_c65_xy() const { return guest::LoadU64(base, address + 16); }
  void set_pixel_c65_xy(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t pixel_c65_xy_address() const { return address + 16; }
  float pixel_c65_g() const { return guest::LoadFloat(base, address + 20); }
  void set_pixel_c65_g(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t pixel_c65_g_address() const { return address + 20; }
  float pixel_c65_b() const { return guest::LoadFloat(base, address + 24); }
  void set_pixel_c65_b(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t pixel_c65_b_address() const { return address + 24; }
  uint64_t pixel_c65_zw() const { return guest::LoadU64(base, address + 24); }
  void set_pixel_c65_zw(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t pixel_c65_zw_address() const { return address + 24; }
  float pixel_c65_a() const { return guest::LoadFloat(base, address + 28); }
  void set_pixel_c65_a(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t pixel_c65_a_address() const { return address + 28; }
  // pixel c66: the paint's third color (SetPaint's C)
  float pixel_c66_r() const { return guest::LoadFloat(base, address + 32); }
  void set_pixel_c66_r(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t pixel_c66_r_address() const { return address + 32; }
  uint64_t pixel_c66_xy() const { return guest::LoadU64(base, address + 32); }
  void set_pixel_c66_xy(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t pixel_c66_xy_address() const { return address + 32; }
  float pixel_c66_g() const { return guest::LoadFloat(base, address + 36); }
  void set_pixel_c66_g(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t pixel_c66_g_address() const { return address + 36; }
  float pixel_c66_b() const { return guest::LoadFloat(base, address + 40); }
  void set_pixel_c66_b(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t pixel_c66_b_address() const { return address + 40; }
  uint64_t pixel_c66_zw() const { return guest::LoadU64(base, address + 40); }
  void set_pixel_c66_zw(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t pixel_c66_zw_address() const { return address + 40; }
  float pixel_c66_a() const { return guest::LoadFloat(base, address + 44); }
  void set_pixel_c66_a(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t pixel_c66_a_address() const { return address + 44; }
  // pixel c63: (1, 1, 1, 1) or the paint's color (SetPaint)
  uint64_t pixel_c63_xy() const { return guest::LoadU64(base, address + 64); }
  void set_pixel_c63_xy(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t pixel_c63_xy_address() const { return address + 64; }
  uint64_t pixel_c63_zw() const { return guest::LoadU64(base, address + 72); }
  void set_pixel_c63_zw(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t pixel_c63_zw_address() const { return address + 72; }
  // pixel c64 (bound as words): the palette entry's +48 to +56 floats into x, y, z
  float pixel_c64_x() const { return guest::LoadFloat(base, address + 80); }
  void set_pixel_c64_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t pixel_c64_x_address() const { return address + 80; }
  uint64_t pixel_c64_xy() const { return guest::LoadU64(base, address + 80); }
  void set_pixel_c64_xy(uint64_t v) const { guest::StoreU64(base, address + 80, v); }
  uint32_t pixel_c64_xy_address() const { return address + 80; }
  float pixel_c64_y() const { return guest::LoadFloat(base, address + 84); }
  void set_pixel_c64_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t pixel_c64_y_address() const { return address + 84; }
  // also pixel c65's w
  float pixel_c64_z() const { return guest::LoadFloat(base, address + 88); }
  void set_pixel_c64_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t pixel_c64_z_address() const { return address + 88; }
  uint64_t pixel_c64_zw() const { return guest::LoadU64(base, address + 88); }
  void set_pixel_c64_zw(uint64_t v) const { guest::StoreU64(base, address + 88, v); }
  uint32_t pixel_c64_zw_address() const { return address + 88; }
  // the part's own shader pair (index into the model's +24, 16 bytes each)
  uint32_t shader_pair() const { return guest::LoadU32(base, address + 96); }
  void set_shader_pair(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t shader_pair_address() const { return address + 96; }
  // bits 2 and 4: sampler 6, the environment texture
  uint32_t flags() const { return guest::LoadU32(base, address + 104); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t flags_address() const { return address + 104; }
  // render state 3 by the blend table (2 set by ScaleBlended)
  uint32_t blend_mode() const { return guest::LoadU32(base, address + 108); }
  void set_blend_mode(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t blend_mode_address() const { return address + 108; }
  uint32_t render_state_100() const { return guest::LoadU32(base, address + 112); }
  void set_render_state_100(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t render_state_100_address() const { return address + 112; }
  // the environment texture's arguments
  uint32_t environment_arg_0() const { return guest::LoadU32(base, address + 120); }
  void set_environment_arg_0(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t environment_arg_0_address() const { return address + 120; }
  uint32_t environment_arg_1() const { return guest::LoadU32(base, address + 124); }
  void set_environment_arg_1(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t environment_arg_1_address() const { return address + 124; }
  uint32_t texture_count() const { return guest::LoadU32(base, address + 128); }
  void set_texture_count(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t texture_count_address() const { return address + 128; }
  // +144 + 4i: texture i's XPR2 index (0 set by SetTexture); the first
  uint32_t xpr_index() const { return guest::LoadU32(base, address + 144); }
  void set_xpr_index(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t xpr_index_address() const { return address + 144; }
  // +176 + 4i: texture i's index in the XPR2, -1 none (SetTexture's texture); the first
  uint32_t texture_index() const { return guest::LoadU32(base, address + 176); }
  void set_texture_index(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t texture_index_address() const { return address + 176; }
  // +208 + 4i: an index into the model's +320 float4s (vertex c9 + i) or -1; the first
  uint32_t color_index() const { return guest::LoadU32(base, address + 208); }
  void set_color_index(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t color_index_address() const { return address + 208; }
  // +240 + 4i: floats (pixel c67); the first
  float texture_float() const { return guest::LoadFloat(base, address + 240); }
  void set_texture_float(float v) const { guest::StoreFloat(base, address + 240, v); }
  uint32_t texture_float_address() const { return address + 240; }
  // +272 + 4i: texture i's sampler state 0; the first
  uint32_t sampler_state_0() const { return guest::LoadU32(base, address + 272); }
  void set_sampler_state_0(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t sampler_state_0_address() const { return address + 272; }
  // +304 + 4i: texture i's sampler state 4; the first
  uint32_t sampler_state_4() const { return guest::LoadU32(base, address + 304); }
  void set_sampler_state_4(uint32_t v) const { guest::StoreU32(base, address + 304, v); }
  uint32_t sampler_state_4_address() const { return address + 304; }
  uint32_t draw_count() const { return guest::LoadU32(base, address + 340); }
  void set_draw_count(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t draw_count_address() const { return address + 340; }
  // render state 56, probably the cull mode: 2 and 6 swapped by FlipCulling and while 0x826C5100 is set
  uint32_t cull_mode() const { return guest::LoadU32(base, address + 344); }
  void set_cull_mode(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t cull_mode_address() const { return address + 344; }
  uint32_t primitive_type() const { return guest::LoadU32(base, address + 348); }
  void set_primitive_type(uint32_t v) const { guest::StoreU32(base, address + 348, v); }
  uint32_t primitive_type_address() const { return address + 348; }
  // an index buffer header (+0x178 its address, relocated)
  uint32_t index_buffer_address() const { return address + 352; }
  // group 0 only
  uint32_t render_state_96() const { return guest::LoadU32(base, address + 388); }
  void set_render_state_96(uint32_t v) const { guest::StoreU32(base, address + 388, v); }
  uint32_t render_state_96_address() const { return address + 388; }
  // the shared shader pair (1000-1005) or 0
  uint32_t shared_pair() const { return guest::LoadU32(base, address + 392); }
  void set_shared_pair(uint32_t v) const { guest::StoreU32(base, address + 392, v); }
  uint32_t shared_pair_address() const { return address + 392; }
  // the material animation's index or -1
  uint32_t material_animation() const { return guest::LoadU32(base, address + 396); }
  void set_material_animation(uint32_t v) const { guest::StoreU32(base, address + 396, v); }
  uint32_t material_animation_address() const { return address + 396; }
  // 1 and 2 the paint's (SetPaint), 1-10 the palette entry (ApplyPalette), 3 and 5 Model::RecordMaterials' textures
  uint32_t category() const { return guest::LoadU32(base, address + 408); }
  void set_category(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t category_address() const { return address + 408; }
  // measure fault: Model::RecordMaterials' pointers into the per-texture arrays (+144, +304 and on, advanced by 4) carry the record's type
  uint32_t fault_640() const { return guest::LoadU32(base, address + 640); }
  void set_fault_640(uint32_t v) const { guest::StoreU32(base, address + 640, v); }
  uint32_t fault_640_address() const { return address + 640; }
  // measure fault: as fault_640
  uint32_t fault_656() const { return guest::LoadU32(base, address + 656); }
  void set_fault_656(uint32_t v) const { guest::StoreU32(base, address + 656, v); }
  uint32_t fault_656_address() const { return address + 656; }
  // measure fault: as fault_640
  uint32_t fault_688() const { return guest::LoadU32(base, address + 688); }
  void set_fault_688(uint32_t v) const { guest::StoreU32(base, address + 688, v); }
  uint32_t fault_688_address() const { return address + 688; }
  // measure fault: as fault_640
  uint32_t fault_784() const { return guest::LoadU32(base, address + 784); }
  void set_fault_784(uint32_t v) const { guest::StoreU32(base, address + 784, v); }
  uint32_t fault_784_address() const { return address + 784; }
  // measure fault: as fault_640
  uint32_t fault_816() const { return guest::LoadU32(base, address + 816); }
  void set_fault_816(uint32_t v) const { guest::StoreU32(base, address + 816, v); }
  uint32_t fault_816_address() const { return address + 816; }
};

// file; measure: PartModel
struct PartModel : View {
  using View::View;
  static constexpr uint32_t kSize = 464;
  static constexpr uint32_t kVtable = 0x8200338C;
  // 0x8200338C (the Model's 0x8206CD18 under it)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the Model's: the part file's +0x10, the shape count
  uint32_t shape_count() const { return guest::LoadU32(base, address + 412); }
  void set_shape_count(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t shape_count_address() const { return address + 412; }
  // with +456: Model::RecordMaterials' textures from +456 (0 at Clear)
  uint32_t use_override_texture() const { return guest::LoadU32(base, address + 432); }
  void set_use_override_texture(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t use_override_texture_address() const { return address + 432; }
  // a texture index replaced by +460 (pair 1003; -1 at Clear)
  uint32_t replaced_texture_index() const { return guest::LoadU32(base, address + 436); }
  void set_replaced_texture_index(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t replaced_texture_index_address() const { return address + 436; }
  // forces shared pair 1005 (0 at Clear)
  uint32_t force_pair_1005() const { return guest::LoadU32(base, address + 440); }
  void set_force_pair_1005(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t force_pair_1005_address() const { return address + 440; }
  // the vinyl sheet: sampler 0 for pairs 1000 and 1004, sampler 1 for 1002 (SetVinylTextures' a)
  uint32_t vinyl_sheet() const { return guest::LoadU32(base, address + 444); }
  void set_vinyl_sheet(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t vinyl_sheet_address() const { return address + 444; }
  // SetVinylTextures' b; not read here
  uint32_t vinyl_texture_b() const { return guest::LoadU32(base, address + 448); }
  void set_vinyl_texture_b(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t vinyl_texture_b_address() const { return address + 448; }
  // category 3's texture
  uint32_t category3_texture() const { return guest::LoadU32(base, address + 452); }
  void set_category3_texture(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t category3_texture_address() const { return address + 452; }
  // an override texture (with +432)
  uint32_t override_texture() const { return guest::LoadU32(base, address + 456); }
  void set_override_texture(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t override_texture_address() const { return address + 456; }
  // the texture that replaces +436's index
  uint32_t replacement_texture() const { return guest::LoadU32(base, address + 460); }
  void set_replacement_texture(uint32_t v) const { guest::StoreU32(base, address + 460, v); }
  uint32_t replacement_texture_address() const { return address + 460; }
};

// file; measure: Color
struct PaintRgb : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float red() const { return guest::LoadFloat(base, address + 0); }
  void set_red(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t red_address() const { return address + 0; }
  // HsvToRgb8's
  uint8_t red8() const { return guest::LoadU8(base, address + 0); }
  void set_red8(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t red8_address() const { return address + 0; }
  uint8_t green8() const { return guest::LoadU8(base, address + 1); }
  void set_green8(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t green8_address() const { return address + 1; }
  uint8_t blue8() const { return guest::LoadU8(base, address + 2); }
  void set_blue8(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t blue8_address() const { return address + 2; }
  float green() const { return guest::LoadFloat(base, address + 4); }
  void set_green(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t green_address() const { return address + 4; }
  float blue() const { return guest::LoadFloat(base, address + 8); }
  void set_blue(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t blue_address() const { return address + 8; }
  // 1.0 (not written for a negative hue, or saturation 0)
  float alpha() const { return guest::LoadFloat(base, address + 12); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t alpha_address() const { return address + 12; }
};

// file; measure: -
struct HsvColor : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // degrees
  float hue() const { return guest::LoadFloat(base, address + 0); }
  void set_hue(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t hue_address() const { return address + 0; }
  // +0 to +7 as a doubleword (HsvToRgb's copy)
  uint64_t hue_saturation() const { return guest::LoadU64(base, address + 0); }
  void set_hue_saturation(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t hue_saturation_address() const { return address + 0; }
  float saturation() const { return guest::LoadFloat(base, address + 4); }
  void set_saturation(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t saturation_address() const { return address + 4; }
  float value() const { return guest::LoadFloat(base, address + 8); }
  void set_value(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t value_address() const { return address + 8; }
  // +8 to +15 as a doubleword (HsvToRgb's copy)
  uint64_t value_w() const { return guest::LoadU64(base, address + 8); }
  void set_value_w(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t value_w_address() const { return address + 8; }
  // 1.0 in the palette; not read
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// allocated; measure: new<32>@CarDealer::OpenColors
struct CarDealerSwatch : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // HsvToRgb's red x [0x82000C44], truncated (fctiwz)
  uint32_t red() const { return guest::LoadU32(base, address + 0); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t red_address() const { return address + 0; }
  // measure fault: Color::HsvToRgb's r4 is the swatch's +16 (an HsvColor), not the swatch
  uint64_t fault_hsv_0() const { return guest::LoadU64(base, address + 0); }
  void set_fault_hsv_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t fault_hsv_0_address() const { return address + 0; }
  uint32_t green() const { return guest::LoadU32(base, address + 4); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t green_address() const { return address + 4; }
  uint32_t blue() const { return guest::LoadU32(base, address + 8); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t blue_address() const { return address + 8; }
  // measure fault: as fault_hsv_0
  uint64_t fault_hsv_8() const { return guest::LoadU64(base, address + 8); }
  void set_fault_hsv_8(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t fault_hsv_8_address() const { return address + 8; }
  // the color record's halfword +4
  float hue() const { return guest::LoadFloat(base, address + 16); }
  void set_hue(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t hue_address() const { return address + 16; }
  // the color record's byte +6 x [0x82002200]
  float saturation() const { return guest::LoadFloat(base, address + 20); }
  void set_saturation(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t saturation_address() const { return address + 20; }
  // the color record's byte +7 plus a kind's offset, clamped to 0-255, x [0x82002200]
  float value() const { return guest::LoadFloat(base, address + 24); }
  void set_value(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t value_address() const { return address + 24; }
};

// file; measure: Color::HsvToRgb8.r4
struct PackedHsv : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // degrees
  int16_t hue() const { return int16_t(guest::LoadU16(base, address + 0)); }
  void set_hue(int16_t v) const { guest::StoreU16(base, address + 0, uint16_t(v)); }
  uint32_t hue_address() const { return address + 0; }
  // 0 gives the value in all three
  uint8_t saturation() const { return guest::LoadU8(base, address + 2); }
  void set_saturation(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t saturation_address() const { return address + 2; }
  uint8_t value() const { return guest::LoadU8(base, address + 3); }
  void set_value(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t value_address() const { return address + 3; }
};

// file; measure: Matrix::TransposeRotation.r4, Projector::Matrices.r5
struct MaterialPassesMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  float m00() const { return guest::LoadFloat(base, address + 0); }
  void set_m00(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t m00_address() const { return address + 0; }
  float m01() const { return guest::LoadFloat(base, address + 4); }
  void set_m01(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t m01_address() const { return address + 4; }
  float m02() const { return guest::LoadFloat(base, address + 8); }
  void set_m02(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t m02_address() const { return address + 8; }
  float m03() const { return guest::LoadFloat(base, address + 12); }
  void set_m03(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t m03_address() const { return address + 12; }
  float m10() const { return guest::LoadFloat(base, address + 16); }
  void set_m10(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t m10_address() const { return address + 16; }
  float m11() const { return guest::LoadFloat(base, address + 20); }
  void set_m11(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t m11_address() const { return address + 20; }
  float m12() const { return guest::LoadFloat(base, address + 24); }
  void set_m12(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t m12_address() const { return address + 24; }
  float m13() const { return guest::LoadFloat(base, address + 28); }
  void set_m13(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t m13_address() const { return address + 28; }
  float m20() const { return guest::LoadFloat(base, address + 32); }
  void set_m20(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t m20_address() const { return address + 32; }
  float m21() const { return guest::LoadFloat(base, address + 36); }
  void set_m21(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t m21_address() const { return address + 36; }
  float m22() const { return guest::LoadFloat(base, address + 40); }
  void set_m22(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t m22_address() const { return address + 40; }
  float m23() const { return guest::LoadFloat(base, address + 44); }
  void set_m23(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t m23_address() const { return address + 44; }
  float m30() const { return guest::LoadFloat(base, address + 48); }
  void set_m30(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t m30_address() const { return address + 48; }
  float m31() const { return guest::LoadFloat(base, address + 52); }
  void set_m31(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t m31_address() const { return address + 52; }
  float m32() const { return guest::LoadFloat(base, address + 56); }
  void set_m32(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t m32_address() const { return address + 56; }
  float m33() const { return guest::LoadFloat(base, address + 60); }
  void set_m33(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t m33_address() const { return address + 60; }
};

// image; measure: -
struct PaintPaletteEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // an HsvColor (w = 1.0): a material's +0 by ApplyPalette
  uint32_t color_a_address() const { return address + 0; }
  // an HsvColor: a material's +32
  uint32_t color_b_address() const { return address + 16; }
  // an HsvColor: a material's +16 (entry j's for shared pair 1000 + j)
  uint32_t color_c_address() const { return address + 32; }
  // into a material's +80 (pixel c64's x)
  float float_0() const { return guest::LoadFloat(base, address + 48); }
  void set_float_0(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t float_0_address() const { return address + 48; }
  // into +84 (entry 11: 1.2 with the scene's +4 set, else 3.0; entry 13: 7.0, else 20.0)
  float float_1() const { return guest::LoadFloat(base, address + 52); }
  void set_float_1(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t float_1_address() const { return address + 52; }
  // into +88 (entry 14: 0.6 with the scene's +4 set, else 0.503125)
  float float_2() const { return guest::LoadFloat(base, address + 56); }
  void set_float_2(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t float_2_address() const { return address + 56; }
  // never written
  uint32_t unused_60() const { return guest::LoadU32(base, address + 60); }
  void set_unused_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unused_60_address() const { return address + 60; }
};

// The image words docs/functions/material-passes.md's code uses.
struct MaterialPassesGlobals {
  // 16 PaintPaletteEntry (64 bytes each), filled by PaintPalette::Init
  static constexpr uint32_t paint_palette_address() { return 0x826C5110u; }
  // 16-byte vectors by 4 x index + sub (sub_820D35D0); the table's size not looked up
  static constexpr uint32_t table_vectors_address() { return 0x82601A00u; }
  // 48-byte records by index (sub_820D3728); the table's size not looked up
  static constexpr uint32_t part_records_address() { return 0x82003680u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // probably -1.0
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // 1/60: a hue's sector (Color::HsvToRgb)
  static float sixtieth(uint8_t* base) { return guest::LoadFloat(base, 0x820029E8u); }
  static void set_sixtieth(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029E8u, v); }
  static constexpr uint32_t sixtieth_address() { return 0x820029E8u; }
  // 1.1: ScaleBlended scales colors below it
  static float blended_limit(uint8_t* base) { return guest::LoadFloat(base, 0x82002CB0u); }
  static void set_blended_limit(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002CB0u, v); }
  static constexpr uint32_t blended_limit_address() { return 0x82002CB0u; }
  // 10.0 (the palette; Projector::Matrices' size from the depth, side set)
  static float value_10(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_value_10(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t value_10_address() { return 0x82001FD8u; }
  // Projector::Matrices' size from the depth, side 0; not looked up
  static float projector_depth_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001FC0u); }
  static void set_projector_depth_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FC0u, v); }
  static constexpr uint32_t projector_depth_scale_address() { return 0x82001FC0u; }
  // Projector::Matrices; not looked up
  static float projector_depth_bias(uint8_t* base) { return guest::LoadFloat(base, 0x82001FCCu); }
  static void set_projector_depth_bias(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FCCu, v); }
  static constexpr uint32_t projector_depth_bias_address() { return 0x82001FCCu; }
  // Projector::Matrices; not looked up
  static float projector_size_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002CC4u); }
  static void set_projector_size_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002CC4u, v); }
  static constexpr uint32_t projector_size_scale_address() { return 0x82002CC4u; }
  // Projector::Matrices' double factor; not looked up
  static double projector_size_double(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x82003588u)); }
  static void set_projector_size_double(uint8_t* base, double v) { guest::StoreU64(base, 0x82003588u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t projector_size_double_address() { return 0x82003588u; }
  // 0.6 (the palette; Projector::Matrices' size factor, side set)
  static float value_0_6(uint8_t* base) { return guest::LoadFloat(base, 0x82002A14u); }
  static void set_value_0_6(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A14u, v); }
  static constexpr uint32_t value_0_6_address() { return 0x82002A14u; }
  // Projector::Matrices, side 0; not looked up
  static float projector_size_factor(uint8_t* base) { return guest::LoadFloat(base, 0x82002A10u); }
  static void set_projector_size_factor(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A10u, v); }
  static constexpr uint32_t projector_size_factor_address() { return 0x82002A10u; }
  // 0.8 (the palette; Projector::Matrices: sizes below it in magnitude scaled by it)
  static float value_0_8(uint8_t* base) { return guest::LoadFloat(base, 0x82001070u); }
  static void set_value_0_8(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001070u, v); }
  static constexpr uint32_t value_0_8_address() { return 0x82001070u; }
  // Projector::Matrices' smallest size; not looked up
  static float projector_size_min(uint8_t* base) { return guest::LoadFloat(base, 0x82002828u); }
  static void set_projector_size_min(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002828u, v); }
  static constexpr uint32_t projector_size_min_address() { return 0x82002828u; }
  // Projector::Matrices' largest size; not looked up
  static float projector_size_max(uint8_t* base) { return guest::LoadFloat(base, 0x820027A8u); }
  static void set_projector_size_max(uint8_t* base, float v) { guest::StoreFloat(base, 0x820027A8u, v); }
  static constexpr uint32_t projector_size_max_address() { return 0x820027A8u; }
  // 0.016667 (the palette)
  static float value_0_016667(uint8_t* base) { return guest::LoadFloat(base, 0x820035C8u); }
  static void set_value_0_016667(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035C8u, v); }
  static constexpr uint32_t value_0_016667_address() { return 0x820035C8u; }
  // 0.01667 (the palette)
  static float value_0_01667(uint8_t* base) { return guest::LoadFloat(base, 0x820035DCu); }
  static void set_value_0_01667(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035DCu, v); }
  static constexpr uint32_t value_0_01667_address() { return 0x820035DCu; }
  // 0.05098 (the palette)
  static float value_0_05098(uint8_t* base) { return guest::LoadFloat(base, 0x820035C0u); }
  static void set_value_0_05098(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035C0u, v); }
  static constexpr uint32_t value_0_05098_address() { return 0x820035C0u; }
  // 0.09375 (the palette)
  static float value_0_09375(uint8_t* base) { return guest::LoadFloat(base, 0x82003598u); }
  static void set_value_0_09375(uint8_t* base, float v) { guest::StoreFloat(base, 0x82003598u, v); }
  static constexpr uint32_t value_0_09375_address() { return 0x82003598u; }
  // 0.1 (the palette)
  static float value_0_1(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_value_0_1(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t value_0_1_address() { return 0x82001FBCu; }
  // 0.125 (the palette)
  static float value_0_125(uint8_t* base) { return guest::LoadFloat(base, 0x82002A88u); }
  static void set_value_0_125(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A88u, v); }
  static constexpr uint32_t value_0_125_address() { return 0x82002A88u; }
  // 0.16 (the palette)
  static float value_0_16(uint8_t* base) { return guest::LoadFloat(base, 0x820035D4u); }
  static void set_value_0_16(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035D4u, v); }
  static constexpr uint32_t value_0_16_address() { return 0x820035D4u; }
  // 0.160133 (the palette)
  static float value_0_160133(uint8_t* base) { return guest::LoadFloat(base, 0x820035D0u); }
  static void set_value_0_160133(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035D0u, v); }
  static constexpr uint32_t value_0_160133_address() { return 0x820035D0u; }
  // 0.199987 (the palette)
  static float value_0_199987(uint8_t* base) { return guest::LoadFloat(base, 0x820035A4u); }
  static void set_value_0_199987(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035A4u, v); }
  static constexpr uint32_t value_0_199987_address() { return 0x820035A4u; }
  // 0.2 (the palette)
  static float value_0_2(uint8_t* base) { return guest::LoadFloat(base, 0x82002208u); }
  static void set_value_0_2(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002208u, v); }
  static constexpr uint32_t value_0_2_address() { return 0x82002208u; }
  // 0.25 (the palette)
  static float value_0_25(uint8_t* base) { return guest::LoadFloat(base, 0x82002968u); }
  static void set_value_0_25(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002968u, v); }
  static constexpr uint32_t value_0_25_address() { return 0x82002968u; }
  // 0.3 (the palette)
  static float value_0_3(uint8_t* base) { return guest::LoadFloat(base, 0x8200296Cu); }
  static void set_value_0_3(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200296Cu, v); }
  static constexpr uint32_t value_0_3_address() { return 0x8200296Cu; }
  // 0.375 (the palette)
  static float value_0_375(uint8_t* base) { return guest::LoadFloat(base, 0x820035B4u); }
  static void set_value_0_375(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035B4u, v); }
  static constexpr uint32_t value_0_375_address() { return 0x820035B4u; }
  // 0.49558 (the palette)
  static float value_0_49558(uint8_t* base) { return guest::LoadFloat(base, 0x820035C4u); }
  static void set_value_0_49558(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035C4u, v); }
  static constexpr uint32_t value_0_49558_address() { return 0x820035C4u; }
  // 0.49668 (the palette)
  static float value_0_49668(uint8_t* base) { return guest::LoadFloat(base, 0x820035E8u); }
  static void set_value_0_49668(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035E8u, v); }
  static constexpr uint32_t value_0_49668_address() { return 0x820035E8u; }
  // 0.503125 (the palette)
  static float value_0_503125(uint8_t* base) { return guest::LoadFloat(base, 0x820035D8u); }
  static void set_value_0_503125(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035D8u, v); }
  static constexpr uint32_t value_0_503125_address() { return 0x820035D8u; }
  // 0.546875 (the palette)
  static float value_0_546875(uint8_t* base) { return guest::LoadFloat(base, 0x820035BCu); }
  static void set_value_0_546875(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035BCu, v); }
  static constexpr uint32_t value_0_546875_address() { return 0x820035BCu; }
  // 0.578125 (the palette)
  static float value_0_578125(uint8_t* base) { return guest::LoadFloat(base, 0x820035E0u); }
  static void set_value_0_578125(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035E0u, v); }
  static constexpr uint32_t value_0_578125_address() { return 0x820035E0u; }
  // 0.640625 (the palette)
  static float value_0_640625(uint8_t* base) { return guest::LoadFloat(base, 0x820035E4u); }
  static void set_value_0_640625(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035E4u, v); }
  static constexpr uint32_t value_0_640625_address() { return 0x820035E4u; }
  // 0.671875 (the palette)
  static float value_0_671875(uint8_t* base) { return guest::LoadFloat(base, 0x8200359Cu); }
  static void set_value_0_671875(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200359Cu, v); }
  static constexpr uint32_t value_0_671875_address() { return 0x8200359Cu; }
  // 0.703125 (the palette)
  static float value_0_703125(uint8_t* base) { return guest::LoadFloat(base, 0x820035ECu); }
  static void set_value_0_703125(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035ECu, v); }
  static constexpr uint32_t value_0_703125_address() { return 0x820035ECu; }
  // 0.828125 (the palette)
  static float value_0_828125(uint8_t* base) { return guest::LoadFloat(base, 0x820035B8u); }
  static void set_value_0_828125(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035B8u, v); }
  static constexpr uint32_t value_0_828125_address() { return 0x820035B8u; }
  // 0.90625 (the palette)
  static float value_0_90625(uint8_t* base) { return guest::LoadFloat(base, 0x820035CCu); }
  static void set_value_0_90625(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035CCu, v); }
  static constexpr uint32_t value_0_90625_address() { return 0x820035CCu; }
  // 0.921875 (the palette)
  static float value_0_921875(uint8_t* base) { return guest::LoadFloat(base, 0x820035F0u); }
  static void set_value_0_921875(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035F0u, v); }
  static constexpr uint32_t value_0_921875_address() { return 0x820035F0u; }
  // 1.2 (the palette)
  static float value_1_2(uint8_t* base) { return guest::LoadFloat(base, 0x820029D4u); }
  static void set_value_1_2(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D4u, v); }
  static constexpr uint32_t value_1_2_address() { return 0x820029D4u; }
  // 2.0 (the palette)
  static float value_2(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_value_2(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t value_2_address() { return 0x820021E8u; }
  // 3.0 (the palette)
  static float value_3(uint8_t* base) { return guest::LoadFloat(base, 0x820021E4u); }
  static void set_value_3(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E4u, v); }
  static constexpr uint32_t value_3_address() { return 0x820021E4u; }
  // 7.0 (the palette)
  static float value_7(uint8_t* base) { return guest::LoadFloat(base, 0x820035B0u); }
  static void set_value_7(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035B0u, v); }
  static constexpr uint32_t value_7_address() { return 0x820035B0u; }
  // 18.0 (the palette)
  static float value_18(uint8_t* base) { return guest::LoadFloat(base, 0x820035A8u); }
  static void set_value_18(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035A8u, v); }
  static constexpr uint32_t value_18_address() { return 0x820035A8u; }
  // 20.0 (the palette)
  static float value_20(uint8_t* base) { return guest::LoadFloat(base, 0x82002568u); }
  static void set_value_20(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002568u, v); }
  static constexpr uint32_t value_20_address() { return 0x82002568u; }
  // 22.3 (the palette)
  static float value_22_3(uint8_t* base) { return guest::LoadFloat(base, 0x820035ACu); }
  static void set_value_22_3(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035ACu, v); }
  static constexpr uint32_t value_22_3_address() { return 0x820035ACu; }
  // 24.0 (the palette)
  static float value_24(uint8_t* base) { return guest::LoadFloat(base, 0x82002228u); }
  static void set_value_24(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002228u, v); }
  static constexpr uint32_t value_24_address() { return 0x82002228u; }
  // 54.0 (the palette)
  static float value_54(uint8_t* base) { return guest::LoadFloat(base, 0x82003590u); }
  static void set_value_54(uint8_t* base, float v) { guest::StoreFloat(base, 0x82003590u, v); }
  static constexpr uint32_t value_54_address() { return 0x82003590u; }
  // 100.0 (the palette)
  static float value_100(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_value_100(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t value_100_address() { return 0x82002A0Cu; }
  // 105.00004 (the palette)
  static float value_105(uint8_t* base) { return guest::LoadFloat(base, 0x82003594u); }
  static void set_value_105(uint8_t* base, float v) { guest::StoreFloat(base, 0x82003594u, v); }
  static constexpr uint32_t value_105_address() { return 0x82003594u; }
  // 115.0 (the palette)
  static float value_115(uint8_t* base) { return guest::LoadFloat(base, 0x820035A0u); }
  static void set_value_115(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035A0u, v); }
  static constexpr uint32_t value_115_address() { return 0x820035A0u; }
};

}  // namespace schema
