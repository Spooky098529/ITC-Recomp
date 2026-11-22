// The views of docs/functions/post-effects.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/post-effects.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: PostEffect, [826C5D80], sub_82136DC8.r3, PostEffect::ReleaseSurface.r3
struct PostEffect : View {
  using View::View;
  static constexpr uint32_t kSize = 8064;
  static constexpr uint32_t kVtable = 0x82008838;
  // the kind's vtable (base 0x82008838 between the constructor's steps and after the destructor)
  template <typename T = PostEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // +4 to +39: the table entry (PostEffectTableEntry) copied in; 0 glow, 1, 2 color filter, 3 motion blur, 4, 5 target filter
  uint32_t kind() const { return guest::LoadU32(base, address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // the render queue's key of its main chunk
  uint32_t sort_key() const { return guest::LoadU32(base, address + 8); }
  void set_sort_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sort_key_address() const { return address + 8; }
  // the motion blur's velocity chunks' key (it - 2 for the upload and the blur's chunk)
  uint32_t second_sort_key() const { return guest::LoadU32(base, address + 12); }
  void set_second_sort_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_sort_key_address() const { return address + 12; }
  uint32_t width() const { return guest::LoadU32(base, address + 16); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t width_address() const { return address + 16; }
  uint32_t height() const { return guest::LoadU32(base, address + 20); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // the view's position (by-view resolves and quads)
  uint32_t x() const { return guest::LoadU32(base, address + 24); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t x_address() const { return address + 24; }
  uint32_t y() const { return guest::LoadU32(base, address + 28); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t y_address() const { return address + 28; }
  // bit 2 kept past the race (PostEffects::DestroyRace); bits 0-1 the target filter's target (render target 4 or 3)
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // its camera pass (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 36); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t camera_pass_address() const { return address + 36; }
  // 1 at set-up, cleared after each record
  uint32_t reset() const { return guest::LoadU32(base, address + 40); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t reset_address() const { return address + 40; }
  // PostEffects::ToggleNoHistory
  uint32_t no_history() const { return guest::LoadU32(base, address + 44); }
  void set_no_history(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t no_history_address() const { return address + 44; }
  // set on logic passes (PostEffects::MarkUpdated)
  uint32_t updated() const { return guest::LoadU32(base, address + 48); }
  void set_updated(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t updated_address() const { return address + 48; }
  // skipped when set (PostEffects::RecordAll)
  uint32_t skipped() const { return guest::LoadU32(base, address + 52); }
  void set_skipped(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t skipped_address() const { return address + 52; }
  // two buffers of 100 40-byte vertices (PostEffectQuad's)
  uint32_t vertices_address() const { return address + 56; }
  // vertices used in the buffer in use
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 8056); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 8056, v); }
  uint32_t vertex_count_address() const { return address + 8056; }
  // the buffer in use (0 or 1)
  uint32_t vertex_buffer() const { return guest::LoadU32(base, address + 8060); }
  void set_vertex_buffer(uint32_t v) const { guest::StoreU32(base, address + 8060, v); }
  uint32_t vertex_buffer_address() const { return address + 8060; }
};

// image; measure: [826C5D80].+0, Glow.+0
struct PostEffectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  // Glow::Record, ColorFilter::Record, MotionBlur::Record, TargetFilter::Record
  uint32_t record() const { return guest::LoadU32(base, address + 0); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t record_address() const { return address + 0; }
  // the deleting destructor (r4 = 1 deletes)
  uint32_t destroy() const { return guest::LoadU32(base, address + 4); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t destroy_address() const { return address + 4; }
  // glow only: Glow::Set8436 (+8436)
  uint32_t c33_x_second_setter() const { return guest::LoadU32(base, address + 8); }
  void set_c33_x_second_setter(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t c33_x_second_setter_address() const { return address + 8; }
  // glow only: Glow::Set8440 (+8440)
  uint32_t c33_x_first_setter() const { return guest::LoadU32(base, address + 12); }
  void set_c33_x_first_setter(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t c33_x_first_setter_address() const { return address + 12; }
  // glow only: Glow::Set8444 (+8444)
  uint32_t c34_x_setter() const { return guest::LoadU32(base, address + 16); }
  void set_c34_x_setter(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t c34_x_setter_address() const { return address + 16; }
  // glow only: Glow::Set8448 (+8448)
  uint32_t c34_y_setter() const { return guest::LoadU32(base, address + 20); }
  void set_c34_y_setter(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t c34_y_setter_address() const { return address + 20; }
  // glow only: Glow::Set8452 (+8452)
  uint32_t strength_setter() const { return guest::LoadU32(base, address + 24); }
  void set_strength_setter(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t strength_setter_address() const { return address + 24; }
  // glow only: Glow::Set8456 (+8456)
  uint32_t c33_z_setter() const { return guest::LoadU32(base, address + 28); }
  void set_c33_z_setter(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t c33_z_setter_address() const { return address + 28; }
  // glow only: Glow::Set8460 (+8460)
  uint32_t history_weight_setter() const { return guest::LoadU32(base, address + 32); }
  void set_history_weight_setter(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t history_weight_setter_address() const { return address + 32; }
  // glow only: Glow::Set8464 (+8464)
  uint32_t param_8464_setter() const { return guest::LoadU32(base, address + 36); }
  void set_param_8464_setter(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t param_8464_setter_address() const { return address + 36; }
  // glow only: Glow::Set8468 (+8468)
  uint32_t star_step_setter() const { return guest::LoadU32(base, address + 40); }
  void set_star_step_setter(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t star_step_setter_address() const { return address + 40; }
  // glow only: Glow::Set8472 (+8472)
  uint32_t star_attenuation_setter() const { return guest::LoadU32(base, address + 44); }
  void set_star_attenuation_setter(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t star_attenuation_setter_address() const { return address + 44; }
  // glow only: Glow::Set8476 (+8476)
  uint32_t c33_w_setter() const { return guest::LoadU32(base, address + 48); }
  void set_c33_w_setter(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t c33_w_setter_address() const { return address + 48; }
  // glow only: Glow::SetFromSpeed (slot 24)
  uint32_t from_speed_setter() const { return guest::LoadU32(base, address + 96); }
  void set_from_speed_setter(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t from_speed_setter_address() const { return address + 96; }
};

// allocated; measure: Glow
struct Glow : View {
  using View::View;
  static constexpr uint32_t kSize = 8480;
  static constexpr uint32_t kVtable = 0x820088B0;
  // the kind's vtable (base 0x82008838 between the constructor's steps and after the destructor)
  template <typename T = PostEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // +4 to +39: the table entry (PostEffectTableEntry) copied in; 0 glow, 1, 2 color filter, 3 motion blur, 4, 5 target filter
  uint32_t kind() const { return guest::LoadU32(base, address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // the render queue's key of its main chunk
  uint32_t sort_key() const { return guest::LoadU32(base, address + 8); }
  void set_sort_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sort_key_address() const { return address + 8; }
  // the motion blur's velocity chunks' key (it - 2 for the upload and the blur's chunk)
  uint32_t second_sort_key() const { return guest::LoadU32(base, address + 12); }
  void set_second_sort_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_sort_key_address() const { return address + 12; }
  uint32_t width() const { return guest::LoadU32(base, address + 16); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t width_address() const { return address + 16; }
  uint32_t height() const { return guest::LoadU32(base, address + 20); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // the view's position (by-view resolves and quads)
  uint32_t x() const { return guest::LoadU32(base, address + 24); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t x_address() const { return address + 24; }
  uint32_t y() const { return guest::LoadU32(base, address + 28); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t y_address() const { return address + 28; }
  // bit 2 kept past the race (PostEffects::DestroyRace); bits 0-1 the target filter's target (render target 4 or 3)
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // its camera pass (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 36); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t camera_pass_address() const { return address + 36; }
  // 1 at set-up, cleared after each record
  uint32_t reset() const { return guest::LoadU32(base, address + 40); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t reset_address() const { return address + 40; }
  // PostEffects::ToggleNoHistory
  uint32_t no_history() const { return guest::LoadU32(base, address + 44); }
  void set_no_history(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t no_history_address() const { return address + 44; }
  // set on logic passes (PostEffects::MarkUpdated)
  uint32_t updated() const { return guest::LoadU32(base, address + 48); }
  void set_updated(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t updated_address() const { return address + 48; }
  // skipped when set (PostEffects::RecordAll)
  uint32_t skipped() const { return guest::LoadU32(base, address + 52); }
  void set_skipped(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t skipped_address() const { return address + 52; }
  // two buffers of 100 40-byte vertices (PostEffectQuad's)
  uint32_t vertices_address() const { return address + 56; }
  // vertices used in the buffer in use
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 8056); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 8056, v); }
  uint32_t vertex_count_address() const { return address + 8056; }
  // the buffer in use (0 or 1)
  uint32_t vertex_buffer() const { return guest::LoadU32(base, address + 8060); }
  void set_vertex_buffer(uint32_t v) const { guest::StoreU32(base, address + 8060, v); }
  uint32_t vertex_buffer_address() const { return address + 8060; }
  // the glare definition (GlareDef), its star definition at +48 (+8112)
  uint32_t glare_address() const { return address + 8064; }
  // the glare's +20: added to the lines' turn
  float glare_star_inclination() const { return guest::LoadFloat(base, address + 8084); }
  void set_glare_star_inclination(float v) const { guest::StoreFloat(base, address + 8084, v); }
  uint32_t glare_star_inclination_address() const { return address + 8084; }
  // the glare's +24: the colors scaled about the constant at 0x82008918
  float glare_chromatic_aberration() const { return guest::LoadFloat(base, address + 8088); }
  void set_glare_chromatic_aberration(float v) const { guest::StoreFloat(base, address + 8088, v); }
  uint32_t glare_chromatic_aberration_address() const { return address + 8088; }
  // the star's lines (4 in the game)
  uint32_t star_line_count() const { return guest::LoadU32(base, address + 8112); }
  void set_star_line_count(uint32_t v) const { guest::StoreU32(base, address + 8112, v); }
  uint32_t star_line_count_address() const { return address + 8112; }
  // the star's lines (StarLine, 16 bytes each)
  uint32_t star_lines_address() const { return address + 8116; }
  // the star's +100
  float star_inclination() const { return guest::LoadFloat(base, address + 8212); }
  void set_star_inclination(float v) const { guest::StoreFloat(base, address + 8212, v); }
  uint32_t star_inclination_address() const { return address + 8212; }
  // the star's +104
  uint32_t star_rotation() const { return guest::LoadU32(base, address + 8216); }
  void set_star_rotation(uint32_t v) const { guest::StoreU32(base, address + 8216, v); }
  uint32_t star_rotation_address() const { return address + 8216; }
  // the star's eight colors (16 bytes each)
  uint32_t star_colors_address() const { return address + 8224; }
  // id 1: the frame resolved, full size
  template <typename T = RenderTexture>
  T frame_texture() const { return T(base, guest::LoadU32(base, address + 8352)); }
  void set_frame_texture(const View& v) const { guest::StoreU32(base, address + 8352, v.address); }
  void set_frame_texture(uint32_t v) const { guest::StoreU32(base, address + 8352, v); }
  uint32_t frame_texture_address() const { return address + 8352; }
  // id 6: an eighth of the size
  template <typename T = RenderTexture>
  T eighth_texture() const { return T(base, guest::LoadU32(base, address + 8356)); }
  void set_eighth_texture(const View& v) const { guest::StoreU32(base, address + 8356, v.address); }
  void set_eighth_texture(uint32_t v) const { guest::StoreU32(base, address + 8356, v); }
  uint32_t eighth_texture_address() const { return address + 8356; }
  // id 7: a quarter of the size; the star's line 0's
  template <typename T = RenderTexture>
  T line_texture_0() const { return T(base, guest::LoadU32(base, address + 8360)); }
  void set_line_texture_0(const View& v) const { guest::StoreU32(base, address + 8360, v.address); }
  void set_line_texture_0(uint32_t v) const { guest::StoreU32(base, address + 8360, v); }
  uint32_t line_texture_0_address() const { return address + 8360; }
  // id 8: a quarter of the size; the star's line 1's
  template <typename T = RenderTexture>
  T line_texture_1() const { return T(base, guest::LoadU32(base, address + 8364)); }
  void set_line_texture_1(const View& v) const { guest::StoreU32(base, address + 8364, v.address); }
  void set_line_texture_1(uint32_t v) const { guest::StoreU32(base, address + 8364, v); }
  uint32_t line_texture_1_address() const { return address + 8364; }
  // id 9: a quarter of the size; the star's line 2's
  template <typename T = RenderTexture>
  T line_texture_2() const { return T(base, guest::LoadU32(base, address + 8368)); }
  void set_line_texture_2(const View& v) const { guest::StoreU32(base, address + 8368, v.address); }
  void set_line_texture_2(uint32_t v) const { guest::StoreU32(base, address + 8368, v); }
  uint32_t line_texture_2_address() const { return address + 8368; }
  // id 10: a quarter of the size; the star's line 3's
  template <typename T = RenderTexture>
  T line_texture_3() const { return T(base, guest::LoadU32(base, address + 8372)); }
  void set_line_texture_3(const View& v) const { guest::StoreU32(base, address + 8372, v.address); }
  void set_line_texture_3(uint32_t v) const { guest::StoreU32(base, address + 8372, v); }
  uint32_t line_texture_3_address() const { return address + 8372; }
  // id 11: a quarter of the size; the star's line 4's
  template <typename T = RenderTexture>
  T line_texture_4() const { return T(base, guest::LoadU32(base, address + 8376)); }
  void set_line_texture_4(const View& v) const { guest::StoreU32(base, address + 8376, v.address); }
  void set_line_texture_4(uint32_t v) const { guest::StoreU32(base, address + 8376, v); }
  uint32_t line_texture_4_address() const { return address + 8376; }
  // id 12: a quarter of the size; the star's line 5's
  template <typename T = RenderTexture>
  T line_texture_5() const { return T(base, guest::LoadU32(base, address + 8380)); }
  void set_line_texture_5(const View& v) const { guest::StoreU32(base, address + 8380, v.address); }
  void set_line_texture_5(uint32_t v) const { guest::StoreU32(base, address + 8380, v); }
  uint32_t line_texture_5_address() const { return address + 8380; }
  // id 13: the lines merged, a quarter of the size
  template <typename T = RenderTexture>
  T star_texture() const { return T(base, guest::LoadU32(base, address + 8384)); }
  void set_star_texture(const View& v) const { guest::StoreU32(base, address + 8384, v.address); }
  void set_star_texture(uint32_t v) const { guest::StoreU32(base, address + 8384, v); }
  uint32_t star_texture_address() const { return address + 8384; }
  // id 14: 64 x 64
  template <typename T = RenderTexture>
  T texture_64() const { return T(base, guest::LoadU32(base, address + 8388)); }
  void set_texture_64(const View& v) const { guest::StoreU32(base, address + 8388, v.address); }
  void set_texture_64(uint32_t v) const { guest::StoreU32(base, address + 8388, v); }
  uint32_t texture_64_address() const { return address + 8388; }
  // id 15: 16 x 16
  template <typename T = RenderTexture>
  T texture_16() const { return T(base, guest::LoadU32(base, address + 8392)); }
  void set_texture_16(const View& v) const { guest::StoreU32(base, address + 8392, v.address); }
  void set_texture_16(uint32_t v) const { guest::StoreU32(base, address + 8392, v); }
  uint32_t texture_16_address() const { return address + 8392; }
  // id 16: 4 x 4
  template <typename T = RenderTexture>
  T texture_4() const { return T(base, guest::LoadU32(base, address + 8396)); }
  void set_texture_4(const View& v) const { guest::StoreU32(base, address + 8396, v.address); }
  void set_texture_4(uint32_t v) const { guest::StoreU32(base, address + 8396, v); }
  uint32_t texture_4_address() const { return address + 8396; }
  // id 17: 1 x 1
  template <typename T = RenderTexture>
  T texture_1() const { return T(base, guest::LoadU32(base, address + 8400)); }
  void set_texture_1(const View& v) const { guest::StoreU32(base, address + 8400, v.address); }
  void set_texture_1(uint32_t v) const { guest::StoreU32(base, address + 8400, v); }
  uint32_t texture_1_address() const { return address + 8400; }
  // id 18: 1 x 1, sampler 1 of the final pass (pair 21)
  template <typename T = RenderTexture>
  T final_texture_1() const { return T(base, guest::LoadU32(base, address + 8404)); }
  void set_final_texture_1(const View& v) const { guest::StoreU32(base, address + 8404, v.address); }
  void set_final_texture_1(uint32_t v) const { guest::StoreU32(base, address + 8404, v); }
  uint32_t final_texture_1_address() const { return address + 8404; }
  // id 20 (its own): the glow's history, a quarter of the size
  template <typename T = RenderTexture>
  T history() const { return T(base, guest::LoadU32(base, address + 8408)); }
  void set_history(const View& v) const { guest::StoreU32(base, address + 8408, v.address); }
  void set_history(uint32_t v) const { guest::StoreU32(base, address + 8408, v); }
  uint32_t history_address() const { return address + 8408; }
  // a quarter of the size: the bright pass drawn into it
  template <typename T = SurfaceHeader>
  T quarter_surface() const { return T(base, guest::LoadU32(base, address + 8412)); }
  void set_quarter_surface(const View& v) const { guest::StoreU32(base, address + 8412, v.address); }
  void set_quarter_surface(uint32_t v) const { guest::StoreU32(base, address + 8412, v); }
  uint32_t quarter_surface_address() const { return address + 8412; }
  template <typename T = SurfaceHeader>
  T eighth_surface() const { return T(base, guest::LoadU32(base, address + 8416)); }
  void set_eighth_surface(const View& v) const { guest::StoreU32(base, address + 8416, v.address); }
  void set_eighth_surface(uint32_t v) const { guest::StoreU32(base, address + 8416, v); }
  uint32_t eighth_surface_address() const { return address + 8416; }
  template <typename T = SurfaceHeader>
  T surface_64() const { return T(base, guest::LoadU32(base, address + 8420)); }
  void set_surface_64(const View& v) const { guest::StoreU32(base, address + 8420, v.address); }
  void set_surface_64(uint32_t v) const { guest::StoreU32(base, address + 8420, v); }
  uint32_t surface_64_address() const { return address + 8420; }
  template <typename T = SurfaceHeader>
  T surface_16() const { return T(base, guest::LoadU32(base, address + 8424)); }
  void set_surface_16(const View& v) const { guest::StoreU32(base, address + 8424, v.address); }
  void set_surface_16(uint32_t v) const { guest::StoreU32(base, address + 8424, v); }
  uint32_t surface_16_address() const { return address + 8424; }
  template <typename T = SurfaceHeader>
  T surface_4() const { return T(base, guest::LoadU32(base, address + 8428)); }
  void set_surface_4(const View& v) const { guest::StoreU32(base, address + 8428, v.address); }
  void set_surface_4(uint32_t v) const { guest::StoreU32(base, address + 8428, v); }
  uint32_t surface_4_address() const { return address + 8428; }
  template <typename T = SurfaceHeader>
  T surface_1() const { return T(base, guest::LoadU32(base, address + 8432)); }
  void set_surface_1(const View& v) const { guest::StoreU32(base, address + 8432, v.address); }
  void set_surface_1(uint32_t v) const { guest::StoreU32(base, address + 8432, v); }
  uint32_t surface_1_address() const { return address + 8432; }
  // c33.x of shader pair 10's second draw (Glow::Set8436, slot 2)
  float c33_x_second() const { return guest::LoadFloat(base, address + 8436); }
  void set_c33_x_second(float v) const { guest::StoreFloat(base, address + 8436, v); }
  uint32_t c33_x_second_address() const { return address + 8436; }
  // c33.x of shader pair 10's first draw (slot 3)
  float c33_x_first() const { return guest::LoadFloat(base, address + 8440); }
  void set_c33_x_first(float v) const { guest::StoreFloat(base, address + 8440, v); }
  uint32_t c33_x_first_address() const { return address + 8440; }
  // the bright pass's c34.x (slot 4)
  float c34_x() const { return guest::LoadFloat(base, address + 8444); }
  void set_c34_x(float v) const { guest::StoreFloat(base, address + 8444, v); }
  uint32_t c34_x_address() const { return address + 8444; }
  // the bright pass's c34.y (slot 5)
  float c34_y() const { return guest::LoadFloat(base, address + 8448); }
  void set_c34_y(float v) const { guest::StoreFloat(base, address + 8448, v); }
  uint32_t c34_y_address() const { return address + 8448; }
  // c33.y (Glow::SetStrength, slot 6; blended by frame interpolation)
  float strength() const { return guest::LoadFloat(base, address + 8452); }
  void set_strength(float v) const { guest::StoreFloat(base, address + 8452, v); }
  uint32_t strength_address() const { return address + 8452; }
  // slot 7
  float c33_z() const { return guest::LoadFloat(base, address + 8456); }
  void set_c33_z(float v) const { guest::StoreFloat(base, address + 8456, v); }
  uint32_t c33_z_address() const { return address + 8456; }
  // the history quad's alpha (Glow::SetHistoryWeight, slot 8; blended by frame interpolation)
  float history_weight() const { return guest::LoadFloat(base, address + 8460); }
  void set_history_weight(float v) const { guest::StoreFloat(base, address + 8460, v); }
  uint32_t history_weight_address() const { return address + 8460; }
  // Glow::Set8464 (slot 9); the records don't read it
  float param_8464() const { return guest::LoadFloat(base, address + 8464); }
  void set_param_8464(float v) const { guest::StoreFloat(base, address + 8464, v); }
  uint32_t param_8464_address() const { return address + 8464; }
  // the star's step per sample (slot 10)
  float star_step() const { return guest::LoadFloat(base, address + 8468); }
  void set_star_step(float v) const { guest::StoreFloat(base, address + 8468, v); }
  uint32_t star_step_address() const { return address + 8468; }
  // the star passes' attenuation (slot 11)
  float star_attenuation() const { return guest::LoadFloat(base, address + 8472); }
  void set_star_attenuation(float v) const { guest::StoreFloat(base, address + 8472, v); }
  uint32_t star_attenuation_address() const { return address + 8472; }
  // slot 12
  float c33_w() const { return guest::LoadFloat(base, address + 8476); }
  void set_c33_w(float v) const { guest::StoreFloat(base, address + 8476, v); }
  uint32_t c33_w_address() const { return address + 8476; }
};

// allocated; measure: ColorFilter
struct ColorFilter : View {
  using View::View;
  static constexpr uint32_t kSize = 8128;
  static constexpr uint32_t kVtable = 0x82008860;
  // the kind's vtable (base 0x82008838 between the constructor's steps and after the destructor)
  template <typename T = PostEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // +4 to +39: the table entry (PostEffectTableEntry) copied in; 0 glow, 1, 2 color filter, 3 motion blur, 4, 5 target filter
  uint32_t kind() const { return guest::LoadU32(base, address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // the render queue's key of its main chunk
  uint32_t sort_key() const { return guest::LoadU32(base, address + 8); }
  void set_sort_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sort_key_address() const { return address + 8; }
  // the motion blur's velocity chunks' key (it - 2 for the upload and the blur's chunk)
  uint32_t second_sort_key() const { return guest::LoadU32(base, address + 12); }
  void set_second_sort_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_sort_key_address() const { return address + 12; }
  uint32_t width() const { return guest::LoadU32(base, address + 16); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t width_address() const { return address + 16; }
  uint32_t height() const { return guest::LoadU32(base, address + 20); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // the view's position (by-view resolves and quads)
  uint32_t x() const { return guest::LoadU32(base, address + 24); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t x_address() const { return address + 24; }
  uint32_t y() const { return guest::LoadU32(base, address + 28); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t y_address() const { return address + 28; }
  // bit 2 kept past the race (PostEffects::DestroyRace); bits 0-1 the target filter's target (render target 4 or 3)
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // its camera pass (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 36); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t camera_pass_address() const { return address + 36; }
  // 1 at set-up, cleared after each record
  uint32_t reset() const { return guest::LoadU32(base, address + 40); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t reset_address() const { return address + 40; }
  // PostEffects::ToggleNoHistory
  uint32_t no_history() const { return guest::LoadU32(base, address + 44); }
  void set_no_history(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t no_history_address() const { return address + 44; }
  // set on logic passes (PostEffects::MarkUpdated)
  uint32_t updated() const { return guest::LoadU32(base, address + 48); }
  void set_updated(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t updated_address() const { return address + 48; }
  // skipped when set (PostEffects::RecordAll)
  uint32_t skipped() const { return guest::LoadU32(base, address + 52); }
  void set_skipped(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t skipped_address() const { return address + 52; }
  // two buffers of 100 40-byte vertices (PostEffectQuad's)
  uint32_t vertices_address() const { return address + 56; }
  // vertices used in the buffer in use
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 8056); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 8056, v); }
  uint32_t vertex_count_address() const { return address + 8056; }
  // the buffer in use (0 or 1)
  uint32_t vertex_buffer() const { return guest::LoadU32(base, address + 8060); }
  void set_vertex_buffer(uint32_t v) const { guest::StoreU32(base, address + 8060, v); }
  uint32_t vertex_buffer_address() const { return address + 8060; }
  // id 1: the frame resolved by the view
  template <typename T = RenderTexture>
  T frame_texture() const { return T(base, guest::LoadU32(base, address + 8064)); }
  void set_frame_texture(const View& v) const { guest::StoreU32(base, address + 8064, v.address); }
  void set_frame_texture(uint32_t v) const { guest::StoreU32(base, address + 8064, v); }
  uint32_t frame_texture_address() const { return address + 8064; }
  // pixel bool b0 (1 at set-up)
  uint32_t bool_0() const { return guest::LoadU32(base, address + 8068); }
  void set_bool_0(uint32_t v) const { guest::StoreU32(base, address + 8068, v); }
  uint32_t bool_0_address() const { return address + 8068; }
  // pixel bool b1
  uint32_t bool_1() const { return guest::LoadU32(base, address + 8072); }
  void set_bool_1(uint32_t v) const { guest::StoreU32(base, address + 8072, v); }
  uint32_t bool_1_address() const { return address + 8072; }
  // c37 = its inverses (ColorFilter::SetColor0)
  float color0_r() const { return guest::LoadFloat(base, address + 8080); }
  void set_color0_r(float v) const { guest::StoreFloat(base, address + 8080, v); }
  uint32_t color0_r_address() const { return address + 8080; }
  float color0_g() const { return guest::LoadFloat(base, address + 8084); }
  void set_color0_g(float v) const { guest::StoreFloat(base, address + 8084, v); }
  uint32_t color0_g_address() const { return address + 8084; }
  float color0_b() const { return guest::LoadFloat(base, address + 8088); }
  void set_color0_b(float v) const { guest::StoreFloat(base, address + 8088, v); }
  uint32_t color0_b_address() const { return address + 8088; }
  float color0_a() const { return guest::LoadFloat(base, address + 8092); }
  void set_color0_a(float v) const { guest::StoreFloat(base, address + 8092, v); }
  uint32_t color0_a_address() const { return address + 8092; }
  // c39
  float color1_r() const { return guest::LoadFloat(base, address + 8096); }
  void set_color1_r(float v) const { guest::StoreFloat(base, address + 8096, v); }
  uint32_t color1_r_address() const { return address + 8096; }
  float color1_g() const { return guest::LoadFloat(base, address + 8100); }
  void set_color1_g(float v) const { guest::StoreFloat(base, address + 8100, v); }
  uint32_t color1_g_address() const { return address + 8100; }
  float color1_b() const { return guest::LoadFloat(base, address + 8104); }
  void set_color1_b(float v) const { guest::StoreFloat(base, address + 8104, v); }
  uint32_t color1_b_address() const { return address + 8104; }
  float color1_a() const { return guest::LoadFloat(base, address + 8108); }
  void set_color1_a(float v) const { guest::StoreFloat(base, address + 8108, v); }
  uint32_t color1_a_address() const { return address + 8108; }
  // clamped, (it + 1) x tan(...) is c38.y (ColorFilter::Set8112)
  float strength() const { return guest::LoadFloat(base, address + 8112); }
  void set_strength(float v) const { guest::StoreFloat(base, address + 8112, v); }
  uint32_t strength_address() const { return address + 8112; }
  // ColorFilter::Set8116
  float c38_z() const { return guest::LoadFloat(base, address + 8116); }
  void set_c38_z(float v) const { guest::StoreFloat(base, address + 8116, v); }
  uint32_t c38_z_address() const { return address + 8116; }
  // ColorFilter::Set8120
  float c38_x() const { return guest::LoadFloat(base, address + 8120); }
  void set_c38_x(float v) const { guest::StoreFloat(base, address + 8120, v); }
  uint32_t c38_x_address() const { return address + 8120; }
};

// allocated; measure: MotionBlur
struct MotionBlur : View {
  using View::View;
  static constexpr uint32_t kSize = 8224;
  static constexpr uint32_t kVtable = 0x820088A4;
  // the kind's vtable (base 0x82008838 between the constructor's steps and after the destructor)
  template <typename T = PostEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // +4 to +39: the table entry (PostEffectTableEntry) copied in; 0 glow, 1, 2 color filter, 3 motion blur, 4, 5 target filter
  uint32_t kind() const { return guest::LoadU32(base, address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // the render queue's key of its main chunk
  uint32_t sort_key() const { return guest::LoadU32(base, address + 8); }
  void set_sort_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sort_key_address() const { return address + 8; }
  // the motion blur's velocity chunks' key (it - 2 for the upload and the blur's chunk)
  uint32_t second_sort_key() const { return guest::LoadU32(base, address + 12); }
  void set_second_sort_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_sort_key_address() const { return address + 12; }
  uint32_t width() const { return guest::LoadU32(base, address + 16); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t width_address() const { return address + 16; }
  uint32_t height() const { return guest::LoadU32(base, address + 20); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // the view's position (by-view resolves and quads)
  uint32_t x() const { return guest::LoadU32(base, address + 24); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t x_address() const { return address + 24; }
  uint32_t y() const { return guest::LoadU32(base, address + 28); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t y_address() const { return address + 28; }
  // bit 2 kept past the race (PostEffects::DestroyRace); bits 0-1 the target filter's target (render target 4 or 3)
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // its camera pass (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 36); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t camera_pass_address() const { return address + 36; }
  // 1 at set-up, cleared after each record
  uint32_t reset() const { return guest::LoadU32(base, address + 40); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t reset_address() const { return address + 40; }
  // PostEffects::ToggleNoHistory
  uint32_t no_history() const { return guest::LoadU32(base, address + 44); }
  void set_no_history(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t no_history_address() const { return address + 44; }
  // set on logic passes (PostEffects::MarkUpdated)
  uint32_t updated() const { return guest::LoadU32(base, address + 48); }
  void set_updated(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t updated_address() const { return address + 48; }
  // skipped when set (PostEffects::RecordAll)
  uint32_t skipped() const { return guest::LoadU32(base, address + 52); }
  void set_skipped(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t skipped_address() const { return address + 52; }
  // two buffers of 100 40-byte vertices (PostEffectQuad's)
  uint32_t vertices_address() const { return address + 56; }
  // vertices used in the buffer in use
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 8056); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 8056, v); }
  uint32_t vertex_count_address() const { return address + 8056; }
  // the buffer in use (0 or 1)
  uint32_t vertex_buffer() const { return guest::LoadU32(base, address + 8060); }
  void set_vertex_buffer(uint32_t v) const { guest::StoreU32(base, address + 8060, v); }
  uint32_t vertex_buffer_address() const { return address + 8060; }
  // id 20 (its own), half size: the velocities resolved
  template <typename T = RenderTexture>
  T velocity_texture() const { return T(base, guest::LoadU32(base, address + 8064)); }
  void set_velocity_texture(const View& v) const { guest::StoreU32(base, address + 8064, v.address); }
  void set_velocity_texture(uint32_t v) const { guest::StoreU32(base, address + 8064, v); }
  uint32_t velocity_texture_address() const { return address + 8064; }
  // id 1: the frame resolved by the view
  template <typename T = RenderTexture>
  T frame_texture() const { return T(base, guest::LoadU32(base, address + 8068)); }
  void set_frame_texture(const View& v) const { guest::StoreU32(base, address + 8068, v.address); }
  void set_frame_texture(uint32_t v) const { guest::StoreU32(base, address + 8068, v); }
  uint32_t frame_texture_address() const { return address + 8068; }
  // half size: the velocities drawn into it
  template <typename T = SurfaceHeader>
  T velocity_surface() const { return T(base, guest::LoadU32(base, address + 8072)); }
  void set_velocity_surface(const View& v) const { guest::StoreU32(base, address + 8072, v.address); }
  void set_velocity_surface(uint32_t v) const { guest::StoreU32(base, address + 8072, v); }
  uint32_t velocity_surface_address() const { return address + 8072; }
  // half size, on the EDRAM base
  template <typename T = SurfaceHeader>
  T depth_surface() const { return T(base, guest::LoadU32(base, address + 8076)); }
  void set_depth_surface(const View& v) const { guest::StoreU32(base, address + 8076, v.address); }
  void set_depth_surface(uint32_t v) const { guest::StoreU32(base, address + 8076, v); }
  uint32_t depth_surface_address() const { return address + 8076; }
  // this tick's view-projection (the camera pass's +144, moved on on logic passes)
  uint32_t view_projection_address() const { return address + 8080; }
  // the previous tick's (vertex c100 x4)
  uint32_t previous_view_projection_address() const { return address + 8144; }
  float c40_x() const { return guest::LoadFloat(base, address + 8208); }
  void set_c40_x(float v) const { guest::StoreFloat(base, address + 8208, v); }
  uint32_t c40_x_address() const { return address + 8208; }
  float c40_y() const { return guest::LoadFloat(base, address + 8212); }
  void set_c40_y(float v) const { guest::StoreFloat(base, address + 8212, v); }
  uint32_t c40_y_address() const { return address + 8212; }
  float c40_z() const { return guest::LoadFloat(base, address + 8216); }
  void set_c40_z(float v) const { guest::StoreFloat(base, address + 8216, v); }
  uint32_t c40_z_address() const { return address + 8216; }
  // c41.x = (1 / width)^2 x it x it
  float scale() const { return guest::LoadFloat(base, address + 8220); }
  void set_scale(float v) const { guest::StoreFloat(base, address + 8220, v); }
  uint32_t scale_address() const { return address + 8220; }
};

// allocated; measure: TargetFilter
struct TargetFilter : View {
  using View::View;
  static constexpr uint32_t kSize = 8068;
  static constexpr uint32_t kVtable = 0x8200889C;
  // the kind's vtable (base 0x82008838 between the constructor's steps and after the destructor)
  template <typename T = PostEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // +4 to +39: the table entry (PostEffectTableEntry) copied in; 0 glow, 1, 2 color filter, 3 motion blur, 4, 5 target filter
  uint32_t kind() const { return guest::LoadU32(base, address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // the render queue's key of its main chunk
  uint32_t sort_key() const { return guest::LoadU32(base, address + 8); }
  void set_sort_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sort_key_address() const { return address + 8; }
  // the motion blur's velocity chunks' key (it - 2 for the upload and the blur's chunk)
  uint32_t second_sort_key() const { return guest::LoadU32(base, address + 12); }
  void set_second_sort_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_sort_key_address() const { return address + 12; }
  uint32_t width() const { return guest::LoadU32(base, address + 16); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t width_address() const { return address + 16; }
  uint32_t height() const { return guest::LoadU32(base, address + 20); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // the view's position (by-view resolves and quads)
  uint32_t x() const { return guest::LoadU32(base, address + 24); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t x_address() const { return address + 24; }
  uint32_t y() const { return guest::LoadU32(base, address + 28); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t y_address() const { return address + 28; }
  // bit 2 kept past the race (PostEffects::DestroyRace); bits 0-1 the target filter's target (render target 4 or 3)
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // its camera pass (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 36); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t camera_pass_address() const { return address + 36; }
  // 1 at set-up, cleared after each record
  uint32_t reset() const { return guest::LoadU32(base, address + 40); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t reset_address() const { return address + 40; }
  // PostEffects::ToggleNoHistory
  uint32_t no_history() const { return guest::LoadU32(base, address + 44); }
  void set_no_history(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t no_history_address() const { return address + 44; }
  // set on logic passes (PostEffects::MarkUpdated)
  uint32_t updated() const { return guest::LoadU32(base, address + 48); }
  void set_updated(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t updated_address() const { return address + 48; }
  // skipped when set (PostEffects::RecordAll)
  uint32_t skipped() const { return guest::LoadU32(base, address + 52); }
  void set_skipped(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t skipped_address() const { return address + 52; }
  // two buffers of 100 40-byte vertices (PostEffectQuad's)
  uint32_t vertices_address() const { return address + 56; }
  // vertices used in the buffer in use
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 8056); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 8056, v); }
  uint32_t vertex_count_address() const { return address + 8056; }
  // the buffer in use (0 or 1)
  uint32_t vertex_buffer() const { return guest::LoadU32(base, address + 8060); }
  void set_vertex_buffer(uint32_t v) const { guest::StoreU32(base, address + 8060, v); }
  uint32_t vertex_buffer_address() const { return address + 8060; }
  // drawn into, then resolved into the target
  template <typename T = SurfaceHeader>
  T surface() const { return T(base, guest::LoadU32(base, address + 8064)); }
  void set_surface(const View& v) const { guest::StoreU32(base, address + 8064, v.address); }
  void set_surface(uint32_t v) const { guest::StoreU32(base, address + 8064, v); }
  uint32_t surface_address() const { return address + 8064; }
};

// allocated; measure: new<8112>@PostEffects::Create
struct PostEffectKind1 : View {
  using View::View;
  static constexpr uint32_t kSize = 8112;
  static constexpr uint32_t kVtable = 0x82008850;
  // the kind's vtable (base 0x82008838 between the constructor's steps and after the destructor)
  template <typename T = PostEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // +4 to +39: the table entry (PostEffectTableEntry) copied in; 0 glow, 1, 2 color filter, 3 motion blur, 4, 5 target filter
  uint32_t kind() const { return guest::LoadU32(base, address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // the render queue's key of its main chunk
  uint32_t sort_key() const { return guest::LoadU32(base, address + 8); }
  void set_sort_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sort_key_address() const { return address + 8; }
  // the motion blur's velocity chunks' key (it - 2 for the upload and the blur's chunk)
  uint32_t second_sort_key() const { return guest::LoadU32(base, address + 12); }
  void set_second_sort_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_sort_key_address() const { return address + 12; }
  uint32_t width() const { return guest::LoadU32(base, address + 16); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t width_address() const { return address + 16; }
  uint32_t height() const { return guest::LoadU32(base, address + 20); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // the view's position (by-view resolves and quads)
  uint32_t x() const { return guest::LoadU32(base, address + 24); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t x_address() const { return address + 24; }
  uint32_t y() const { return guest::LoadU32(base, address + 28); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t y_address() const { return address + 28; }
  // bit 2 kept past the race (PostEffects::DestroyRace); bits 0-1 the target filter's target (render target 4 or 3)
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // its camera pass (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 36); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t camera_pass_address() const { return address + 36; }
  // 1 at set-up, cleared after each record
  uint32_t reset() const { return guest::LoadU32(base, address + 40); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t reset_address() const { return address + 40; }
  // PostEffects::ToggleNoHistory
  uint32_t no_history() const { return guest::LoadU32(base, address + 44); }
  void set_no_history(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t no_history_address() const { return address + 44; }
  // set on logic passes (PostEffects::MarkUpdated)
  uint32_t updated() const { return guest::LoadU32(base, address + 48); }
  void set_updated(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t updated_address() const { return address + 48; }
  // skipped when set (PostEffects::RecordAll)
  uint32_t skipped() const { return guest::LoadU32(base, address + 52); }
  void set_skipped(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t skipped_address() const { return address + 52; }
  // two buffers of 100 40-byte vertices (PostEffectQuad's)
  uint32_t vertices_address() const { return address + 56; }
  // vertices used in the buffer in use
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 8056); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 8056, v); }
  uint32_t vertex_count_address() const { return address + 8056; }
  // the buffer in use (0 or 1)
  uint32_t vertex_buffer() const { return guest::LoadU32(base, address + 8060); }
  void set_vertex_buffer(uint32_t v) const { guest::StoreU32(base, address + 8060, v); }
  uint32_t vertex_buffer_address() const { return address + 8060; }
  template <typename T = RenderTexture>
  T texture_0() const { return T(base, guest::LoadU32(base, address + 8064)); }
  void set_texture_0(const View& v) const { guest::StoreU32(base, address + 8064, v.address); }
  void set_texture_0(uint32_t v) const { guest::StoreU32(base, address + 8064, v); }
  uint32_t texture_0_address() const { return address + 8064; }
  template <typename T = RenderTexture>
  T texture_1() const { return T(base, guest::LoadU32(base, address + 8068)); }
  void set_texture_1(const View& v) const { guest::StoreU32(base, address + 8068, v.address); }
  void set_texture_1(uint32_t v) const { guest::StoreU32(base, address + 8068, v); }
  uint32_t texture_1_address() const { return address + 8068; }
  template <typename T = RenderTexture>
  T texture_2() const { return T(base, guest::LoadU32(base, address + 8072)); }
  void set_texture_2(const View& v) const { guest::StoreU32(base, address + 8072, v.address); }
  void set_texture_2(uint32_t v) const { guest::StoreU32(base, address + 8072, v); }
  uint32_t texture_2_address() const { return address + 8072; }
  template <typename T = RenderTexture>
  T texture_3() const { return T(base, guest::LoadU32(base, address + 8076)); }
  void set_texture_3(const View& v) const { guest::StoreU32(base, address + 8076, v.address); }
  void set_texture_3(uint32_t v) const { guest::StoreU32(base, address + 8076, v); }
  uint32_t texture_3_address() const { return address + 8076; }
  template <typename T = RenderTexture>
  T texture_4() const { return T(base, guest::LoadU32(base, address + 8080)); }
  void set_texture_4(const View& v) const { guest::StoreU32(base, address + 8080, v.address); }
  void set_texture_4(uint32_t v) const { guest::StoreU32(base, address + 8080, v); }
  uint32_t texture_4_address() const { return address + 8080; }
  template <typename T = SurfaceHeader>
  T surface_0() const { return T(base, guest::LoadU32(base, address + 8084)); }
  void set_surface_0(const View& v) const { guest::StoreU32(base, address + 8084, v.address); }
  void set_surface_0(uint32_t v) const { guest::StoreU32(base, address + 8084, v); }
  uint32_t surface_0_address() const { return address + 8084; }
  template <typename T = SurfaceHeader>
  T surface_1() const { return T(base, guest::LoadU32(base, address + 8088)); }
  void set_surface_1(const View& v) const { guest::StoreU32(base, address + 8088, v.address); }
  void set_surface_1(uint32_t v) const { guest::StoreU32(base, address + 8088, v); }
  uint32_t surface_1_address() const { return address + 8088; }
  template <typename T = SurfaceHeader>
  T surface_2() const { return T(base, guest::LoadU32(base, address + 8092)); }
  void set_surface_2(const View& v) const { guest::StoreU32(base, address + 8092, v.address); }
  void set_surface_2(uint32_t v) const { guest::StoreU32(base, address + 8092, v); }
  uint32_t surface_2_address() const { return address + 8092; }
  float unknown_8096() const { return guest::LoadFloat(base, address + 8096); }
  void set_unknown_8096(float v) const { guest::StoreFloat(base, address + 8096, v); }
  uint32_t unknown_8096_address() const { return address + 8096; }
  float unknown_8100() const { return guest::LoadFloat(base, address + 8100); }
  void set_unknown_8100(float v) const { guest::StoreFloat(base, address + 8100, v); }
  uint32_t unknown_8100_address() const { return address + 8100; }
  float unknown_8104() const { return guest::LoadFloat(base, address + 8104); }
  void set_unknown_8104(float v) const { guest::StoreFloat(base, address + 8104, v); }
  uint32_t unknown_8104_address() const { return address + 8104; }
  float unknown_8108() const { return guest::LoadFloat(base, address + 8108); }
  void set_unknown_8108(float v) const { guest::StoreFloat(base, address + 8108, v); }
  uint32_t unknown_8108_address() const { return address + 8108; }
};

// allocated; measure: new<8068>@PostEffects::Create#2
struct PostEffectKind4 : View {
  using View::View;
  static constexpr uint32_t kSize = 8068;
  static constexpr uint32_t kVtable = 0x82008848;
  // the kind's vtable (base 0x82008838 between the constructor's steps and after the destructor)
  template <typename T = PostEffectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // +4 to +39: the table entry (PostEffectTableEntry) copied in; 0 glow, 1, 2 color filter, 3 motion blur, 4, 5 target filter
  uint32_t kind() const { return guest::LoadU32(base, address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // the render queue's key of its main chunk
  uint32_t sort_key() const { return guest::LoadU32(base, address + 8); }
  void set_sort_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sort_key_address() const { return address + 8; }
  // the motion blur's velocity chunks' key (it - 2 for the upload and the blur's chunk)
  uint32_t second_sort_key() const { return guest::LoadU32(base, address + 12); }
  void set_second_sort_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_sort_key_address() const { return address + 12; }
  uint32_t width() const { return guest::LoadU32(base, address + 16); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t width_address() const { return address + 16; }
  uint32_t height() const { return guest::LoadU32(base, address + 20); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t height_address() const { return address + 20; }
  // the view's position (by-view resolves and quads)
  uint32_t x() const { return guest::LoadU32(base, address + 24); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t x_address() const { return address + 24; }
  uint32_t y() const { return guest::LoadU32(base, address + 28); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t y_address() const { return address + 28; }
  // bit 2 kept past the race (PostEffects::DestroyRace); bits 0-1 the target filter's target (render target 4 or 3)
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // its camera pass (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 36); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t camera_pass_address() const { return address + 36; }
  // 1 at set-up, cleared after each record
  uint32_t reset() const { return guest::LoadU32(base, address + 40); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t reset_address() const { return address + 40; }
  // PostEffects::ToggleNoHistory
  uint32_t no_history() const { return guest::LoadU32(base, address + 44); }
  void set_no_history(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t no_history_address() const { return address + 44; }
  // set on logic passes (PostEffects::MarkUpdated)
  uint32_t updated() const { return guest::LoadU32(base, address + 48); }
  void set_updated(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t updated_address() const { return address + 48; }
  // skipped when set (PostEffects::RecordAll)
  uint32_t skipped() const { return guest::LoadU32(base, address + 52); }
  void set_skipped(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t skipped_address() const { return address + 52; }
  // two buffers of 100 40-byte vertices (PostEffectQuad's)
  uint32_t vertices_address() const { return address + 56; }
  // vertices used in the buffer in use
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 8056); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 8056, v); }
  uint32_t vertex_count_address() const { return address + 8056; }
  // the buffer in use (0 or 1)
  uint32_t vertex_buffer() const { return guest::LoadU32(base, address + 8060); }
  void set_vertex_buffer(uint32_t v) const { guest::StoreU32(base, address + 8060, v); }
  uint32_t vertex_buffer_address() const { return address + 8060; }
  template <typename T = RenderTexture>
  T texture() const { return T(base, guest::LoadU32(base, address + 8064)); }
  void set_texture(const View& v) const { guest::StoreU32(base, address + 8064, v.address); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 8064, v); }
  uint32_t texture_address() const { return address + 8064; }
};

// image; measure: -
struct PostEffectTableEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  uint32_t sort_key() const { return guest::LoadU32(base, address + 4); }
  void set_sort_key(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t sort_key_address() const { return address + 4; }
  uint32_t second_sort_key() const { return guest::LoadU32(base, address + 8); }
  void set_second_sort_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t second_sort_key_address() const { return address + 8; }
  uint32_t width() const { return guest::LoadU32(base, address + 12); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t width_address() const { return address + 12; }
  uint32_t height() const { return guest::LoadU32(base, address + 16); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t height_address() const { return address + 16; }
  uint32_t x() const { return guest::LoadU32(base, address + 20); }
  void set_x(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t x_address() const { return address + 20; }
  uint32_t y() const { return guest::LoadU32(base, address + 24); }
  void set_y(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t y_address() const { return address + 24; }
  uint32_t flags() const { return guest::LoadU32(base, address + 28); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t flags_address() const { return address + 28; }
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 32); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t camera_pass_address() const { return address + 32; }
};

// allocated; measure: RenderTexture, new<36>@RenderTextures::Acquire, Glow.+8364, Glow.+8368, Glow.+8372, Glow.+8404, RenderTextures::Release.r3.+0
struct RenderTexture : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  uint32_t format() const { return guest::LoadU32(base, address + 0); }
  void set_format(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t format_address() const { return address + 0; }
  uint32_t width() const { return guest::LoadU32(base, address + 4); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t width_address() const { return address + 4; }
  uint32_t height() const { return guest::LoadU32(base, address + 8); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t height_address() const { return address + 8; }
  // two more words of the key (the view's x and y for id 1, else 0)
  uint32_t key0() const { return guest::LoadU32(base, address + 12); }
  void set_key0(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t key0_address() const { return address + 12; }
  uint32_t key1() const { return guest::LoadU32(base, address + 16); }
  void set_key1(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t key1_address() const { return address + 16; }
  uint32_t users() const { return guest::LoadU32(base, address + 20); }
  void set_users(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t users_address() const { return address + 20; }
  // 52 bytes (operator new), set by XGSetTextureHeader
  template <typename T = TextureHeader>
  T header() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_header(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_header(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t header_address() const { return address + 24; }
  // its physical memory (XPhysicalAlloc)
  uint32_t memory() const { return guest::LoadU32(base, address + 28); }
  void set_memory(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t memory_address() const { return address + 28; }
  // 20: its own entry, not the pool's
  uint32_t id() const { return guest::LoadU32(base, address + 32); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t id_address() const { return address + 32; }
};

// embedded; measure: RenderTextures::Release.r3
struct RenderTextureRef : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = RenderTexture>
  T entry() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_entry(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_entry(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t entry_address() const { return address + 0; }
};

// embedded; measure: Glow@+8064
struct GlareDef : View {
  using View::View;
  static constexpr uint32_t kSize = 288;
  float glare_luminance() const { return guest::LoadFloat(base, address + 0); }
  void set_glare_luminance(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t glare_luminance_address() const { return address + 0; }
  float bloom_luminance() const { return guest::LoadFloat(base, address + 4); }
  void set_bloom_luminance(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t bloom_luminance_address() const { return address + 4; }
  float ghost_luminance() const { return guest::LoadFloat(base, address + 8); }
  void set_ghost_luminance(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t ghost_luminance_address() const { return address + 8; }
  float ghost_distortion() const { return guest::LoadFloat(base, address + 12); }
  void set_ghost_distortion(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t ghost_distortion_address() const { return address + 12; }
  float star_luminance() const { return guest::LoadFloat(base, address + 16); }
  void set_star_luminance(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t star_luminance_address() const { return address + 16; }
  // the table's +24 (its +20, the star type, left out)
  float star_inclination() const { return guest::LoadFloat(base, address + 20); }
  void set_star_inclination(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t star_inclination_address() const { return address + 20; }
  float chromatic_aberration_luminance() const { return guest::LoadFloat(base, address + 24); }
  void set_chromatic_aberration_luminance(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t chromatic_aberration_luminance_address() const { return address + 24; }
  float afterimage_sensitivity() const { return guest::LoadFloat(base, address + 28); }
  void set_afterimage_sensitivity(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t afterimage_sensitivity_address() const { return address + 28; }
  float afterimage_ratio() const { return guest::LoadFloat(base, address + 32); }
  void set_afterimage_ratio(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t afterimage_ratio_address() const { return address + 32; }
  float afterimage_luminance() const { return guest::LoadFloat(base, address + 36); }
  void set_afterimage_luminance(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t afterimage_luminance_address() const { return address + 36; }
  // the star definition (StarDef)
  uint32_t star_address() const { return address + 48; }
  uint32_t star_line_count() const { return guest::LoadU32(base, address + 48); }
  void set_star_line_count(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t star_line_count_address() const { return address + 48; }
  float star_def_inclination() const { return guest::LoadFloat(base, address + 148); }
  void set_star_def_inclination(float v) const { guest::StoreFloat(base, address + 148, v); }
  uint32_t star_def_inclination_address() const { return address + 148; }
  uint32_t star_rotation() const { return guest::LoadU32(base, address + 152); }
  void set_star_rotation(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t star_rotation_address() const { return address + 152; }
  uint32_t star_colors_address() const { return address + 160; }
};

// embedded; measure: Glow@+8064@+48
struct StarDef : View {
  using View::View;
  static constexpr uint32_t kSize = 240;
  uint32_t line_count() const { return guest::LoadU32(base, address + 0); }
  void set_line_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t line_count_address() const { return address + 0; }
  // StarLine, 16 bytes each
  uint32_t lines_address() const { return address + 4; }
  float inclination() const { return guest::LoadFloat(base, address + 100); }
  void set_inclination(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t inclination_address() const { return address + 100; }
  // probably a bool (the HDR sample's bRotation)
  uint32_t rotation() const { return guest::LoadU32(base, address + 104); }
  void set_rotation(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t rotation_address() const { return address + 104; }
  // eight colors (r, g, b, a) of 16 bytes
  uint32_t colors_address() const { return address + 112; }
};

// embedded; measure: -
struct StarLine : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t passes() const { return guest::LoadU32(base, address + 0); }
  void set_passes(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t passes_address() const { return address + 0; }
  float sample_length() const { return guest::LoadFloat(base, address + 4); }
  void set_sample_length(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t sample_length_address() const { return address + 4; }
  float attenuation() const { return guest::LoadFloat(base, address + 8); }
  void set_attenuation(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t attenuation_address() const { return address + 8; }
  float angle() const { return guest::LoadFloat(base, address + 12); }
  void set_angle(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t angle_address() const { return address + 12; }
};

// stack; measure: Glow::RecordStarPass.r8
struct GlowStarColor : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float r() const { return guest::LoadFloat(base, address + 0); }
  void set_r(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t r_address() const { return address + 0; }
  float g() const { return guest::LoadFloat(base, address + 4); }
  void set_g(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t g_address() const { return address + 4; }
  float b() const { return guest::LoadFloat(base, address + 8); }
  void set_b(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t b_address() const { return address + 8; }
  float a() const { return guest::LoadFloat(base, address + 12); }
  void set_a(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t a_address() const { return address + 12; }
};

// image; measure: -
struct GlareTableEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  float glare_luminance() const { return guest::LoadFloat(base, address + 0); }
  void set_glare_luminance(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t glare_luminance_address() const { return address + 0; }
  float bloom_luminance() const { return guest::LoadFloat(base, address + 4); }
  void set_bloom_luminance(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t bloom_luminance_address() const { return address + 4; }
  float ghost_luminance() const { return guest::LoadFloat(base, address + 8); }
  void set_ghost_luminance(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t ghost_luminance_address() const { return address + 8; }
  float ghost_distortion() const { return guest::LoadFloat(base, address + 12); }
  void set_ghost_distortion(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t ghost_distortion_address() const { return address + 12; }
  float star_luminance() const { return guest::LoadFloat(base, address + 16); }
  void set_star_luminance(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t star_luminance_address() const { return address + 16; }
  // the star table's entry
  uint32_t star_type() const { return guest::LoadU32(base, address + 20); }
  void set_star_type(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t star_type_address() const { return address + 20; }
  float star_inclination() const { return guest::LoadFloat(base, address + 24); }
  void set_star_inclination(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t star_inclination_address() const { return address + 24; }
  float chromatic_aberration_luminance() const { return guest::LoadFloat(base, address + 28); }
  void set_chromatic_aberration_luminance(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t chromatic_aberration_luminance_address() const { return address + 28; }
  float afterimage_sensitivity() const { return guest::LoadFloat(base, address + 32); }
  void set_afterimage_sensitivity(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t afterimage_sensitivity_address() const { return address + 32; }
  float afterimage_ratio() const { return guest::LoadFloat(base, address + 36); }
  void set_afterimage_ratio(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t afterimage_ratio_address() const { return address + 36; }
  float afterimage_luminance() const { return guest::LoadFloat(base, address + 40); }
  void set_afterimage_luminance(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t afterimage_luminance_address() const { return address + 40; }
};

// image; measure: -
struct StarTableEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t line_count() const { return guest::LoadU32(base, address + 0); }
  void set_line_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t line_count_address() const { return address + 0; }
  uint32_t passes() const { return guest::LoadU32(base, address + 4); }
  void set_passes(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t passes_address() const { return address + 4; }
  float sample_length() const { return guest::LoadFloat(base, address + 8); }
  void set_sample_length(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t sample_length_address() const { return address + 8; }
  float attenuation() const { return guest::LoadFloat(base, address + 12); }
  void set_attenuation(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t attenuation_address() const { return address + 12; }
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  float inclination() const { return guest::LoadFloat(base, address + 20); }
  void set_inclination(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t inclination_address() const { return address + 20; }
  uint32_t rotation() const { return guest::LoadU32(base, address + 24); }
  void set_rotation(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t rotation_address() const { return address + 24; }
};

// embedded; measure: PostEffect::AddQuad.ret
struct PostEffectQuad : View {
  using View::View;
  static constexpr uint32_t kSize = 120;
  float v0_x() const { return guest::LoadFloat(base, address + 0); }
  void set_v0_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t v0_x_address() const { return address + 0; }
  float v0_y() const { return guest::LoadFloat(base, address + 4); }
  void set_v0_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t v0_y_address() const { return address + 4; }
  float v0_z() const { return guest::LoadFloat(base, address + 8); }
  void set_v0_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t v0_z_address() const { return address + 8; }
  float v0_w() const { return guest::LoadFloat(base, address + 12); }
  void set_v0_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t v0_w_address() const { return address + 12; }
  float v0_r() const { return guest::LoadFloat(base, address + 16); }
  void set_v0_r(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t v0_r_address() const { return address + 16; }
  float v0_g() const { return guest::LoadFloat(base, address + 20); }
  void set_v0_g(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t v0_g_address() const { return address + 20; }
  float v0_b() const { return guest::LoadFloat(base, address + 24); }
  void set_v0_b(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t v0_b_address() const { return address + 24; }
  float v0_a() const { return guest::LoadFloat(base, address + 28); }
  void set_v0_a(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t v0_a_address() const { return address + 28; }
  float v0_u() const { return guest::LoadFloat(base, address + 32); }
  void set_v0_u(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t v0_u_address() const { return address + 32; }
  float v0_v() const { return guest::LoadFloat(base, address + 36); }
  void set_v0_v(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t v0_v_address() const { return address + 36; }
  float v1_x() const { return guest::LoadFloat(base, address + 40); }
  void set_v1_x(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t v1_x_address() const { return address + 40; }
  float v1_y() const { return guest::LoadFloat(base, address + 44); }
  void set_v1_y(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t v1_y_address() const { return address + 44; }
  float v1_z() const { return guest::LoadFloat(base, address + 48); }
  void set_v1_z(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t v1_z_address() const { return address + 48; }
  float v1_w() const { return guest::LoadFloat(base, address + 52); }
  void set_v1_w(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t v1_w_address() const { return address + 52; }
  float v1_r() const { return guest::LoadFloat(base, address + 56); }
  void set_v1_r(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t v1_r_address() const { return address + 56; }
  float v1_g() const { return guest::LoadFloat(base, address + 60); }
  void set_v1_g(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t v1_g_address() const { return address + 60; }
  float v1_b() const { return guest::LoadFloat(base, address + 64); }
  void set_v1_b(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t v1_b_address() const { return address + 64; }
  float v1_a() const { return guest::LoadFloat(base, address + 68); }
  void set_v1_a(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t v1_a_address() const { return address + 68; }
  float v1_u() const { return guest::LoadFloat(base, address + 72); }
  void set_v1_u(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t v1_u_address() const { return address + 72; }
  float v1_v() const { return guest::LoadFloat(base, address + 76); }
  void set_v1_v(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t v1_v_address() const { return address + 76; }
  float v2_x() const { return guest::LoadFloat(base, address + 80); }
  void set_v2_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t v2_x_address() const { return address + 80; }
  float v2_y() const { return guest::LoadFloat(base, address + 84); }
  void set_v2_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t v2_y_address() const { return address + 84; }
  float v2_z() const { return guest::LoadFloat(base, address + 88); }
  void set_v2_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t v2_z_address() const { return address + 88; }
  float v2_w() const { return guest::LoadFloat(base, address + 92); }
  void set_v2_w(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t v2_w_address() const { return address + 92; }
  float v2_r() const { return guest::LoadFloat(base, address + 96); }
  void set_v2_r(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t v2_r_address() const { return address + 96; }
  float v2_g() const { return guest::LoadFloat(base, address + 100); }
  void set_v2_g(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t v2_g_address() const { return address + 100; }
  float v2_b() const { return guest::LoadFloat(base, address + 104); }
  void set_v2_b(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t v2_b_address() const { return address + 104; }
  float v2_a() const { return guest::LoadFloat(base, address + 108); }
  void set_v2_a(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t v2_a_address() const { return address + 108; }
  float v2_u() const { return guest::LoadFloat(base, address + 112); }
  void set_v2_u(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t v2_u_address() const { return address + 112; }
  float v2_v() const { return guest::LoadFloat(base, address + 116); }
  void set_v2_v(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t v2_v_address() const { return address + 116; }
};

// stack; measure: PostEffect::RecordPixelConstants.r5
struct PostEffectConstant : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// gpu; measure: CommandList::AllocateBytes.ret
struct ClearItem : View {
  using View::View;
  static constexpr uint32_t kSize = 280;
  // probably; 0
  uint32_t rect_count() const { return guest::LoadU32(base, address + 0); }
  void set_rect_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t rect_count_address() const { return address + 0; }
  // probably; 0
  uint32_t rects() const { return guest::LoadU32(base, address + 4); }
  void set_rects(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t rects_address() const { return address + 4; }
  // 49 color and depth, 1 color, 48 (cameras.md)
  uint32_t flags() const { return guest::LoadU32(base, address + 8); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t flags_address() const { return address + 8; }
  uint32_t color() const { return guest::LoadU32(base, address + 268); }
  void set_color(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t color_address() const { return address + 268; }
  float depth() const { return guest::LoadFloat(base, address + 272); }
  void set_depth(float v) const { guest::StoreFloat(base, address + 272, v); }
  uint32_t depth_address() const { return address + 272; }
  // probably; 0
  uint32_t stencil() const { return guest::LoadU32(base, address + 276); }
  void set_stencil(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t stencil_address() const { return address + 276; }
};

// The image words docs/functions/post-effects.md's code uses.
struct PostEffectsGlobals {
  // 15 PostEffect pointers, one a table entry
  static constexpr uint32_t effects_address() { return 0x826C5D80u; }
  // the effects' 40-byte vertices (PostEffects::InitBoot)
  template <typename T = VertexDeclaration>
  static T vertex_declaration(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5DBCu)); }
  static void set_vertex_declaration(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5DBCu, v.address); }
  static void set_vertex_declaration(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5DBCu, v); }
  static constexpr uint32_t vertex_declaration_address() { return 0x826C5DBCu; }
  // the EDRAM base the effects' surfaces are given (PostEffects::InitEdramBase: 936)
  static uint32_t edram_base(uint8_t* base) { return guest::LoadU32(base, 0x826C5DC0u); }
  static void set_edram_base(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5DC0u, v); }
  static constexpr uint32_t edram_base_address() { return 0x826C5DC0u; }
  // the render texture pool: 20 RenderTexture entries by id
  static constexpr uint32_t render_textures_address() { return 0x826C5DC8u; }
  // 15 PostEffectTableEntry
  static constexpr uint32_t effect_table_address() { return 0x826174A8u; }
  // the effects' vertex elements (CreateVertexDeclaration's input; its length not read here)
  static constexpr uint32_t vertex_elements_address() { return 0x826176C4u; }
  // eight GlareTableEntry
  static constexpr uint32_t glare_table_address() { return 0x82617280u; }
  // six StarTableEntry
  static constexpr uint32_t star_table_address() { return 0x826171D8u; }
  // the render manager (app-start-up.md): slot 8, then 1 the scene's command list, 2 its render queue
  static View render_manager(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826BABA0u)); }
  static void set_render_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABA0u, v); }
  static constexpr uint32_t render_manager_address() { return 0x826BABA0u; }
  // the 21 camera passes (CameraPass, 592 bytes each)
  static constexpr uint32_t camera_passes_address() { return 0x826BAC00u; }
  // the course scenery (Race::ToggleEffects flips its +196 bit 0x4000)
  template <typename T = Scenery>
  static T scenery(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C551Cu)); }
  static void set_scenery(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C551Cu, v.address); }
  static void set_scenery(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C551Cu, v); }
  static constexpr uint32_t scenery_address() { return 0x826C551Cu; }
  // the car scene (Race::ToggleEffects toggles its +5048)
  template <typename T = CarScene>
  static T scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
};

}  // namespace schema
