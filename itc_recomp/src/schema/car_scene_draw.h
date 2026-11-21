// The views of docs/functions/car-scene-draw.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-scene-draw.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Vector16.+8, Vector16::InsertN.r4, Vector16::InsertN.r5, Vector16::InsertN.r6, Vector16::PushBack.r4, Plane::SegmentCrossing.r5, Plane::SegmentCrossing.r6, Matrix::LookAt_82597378.r4, Matrix::LookAt_82597378.r5
struct CarScenePoint : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // x and y as one doubleword (the copies' ld/std)
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  // z and w as one doubleword
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // 1.0 for a position
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// allocated; measure: new<?>@Vector16::Allocate
struct Vector16Storage : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // 16-byte CarScenePoints, as many as the capacity (the measure sees the first two)
  uint32_t points_address() const { return address + 0; }
};

// embedded; measure: Vector16, Box::FromPoints.r4
struct Vector16 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // std::vector's allocator word, not read by it; the measure's two pointer reads here are probably Vector16::Insert's iterator (its r3) joined with the vector
  template <typename T = CarScenePoint>
  T unknown_0() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_unknown_0(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // 0 with no storage
  template <typename T = CarScenePoint>
  T first() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  // one past the last point
  template <typename T = CarScenePoint>
  T last() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_last(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  // the end of the storage (the capacity)
  template <typename T = CarScenePoint>
  T end() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_end(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
};

// embedded; measure: Box
struct CarSceneBox : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // starts at [0x8206CE44] (probably FLT_MAX)
  float min_x() const { return guest::LoadFloat(base, address + 0); }
  void set_min_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t min_x_address() const { return address + 0; }
  float min_y() const { return guest::LoadFloat(base, address + 4); }
  void set_min_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t min_y_address() const { return address + 4; }
  float min_z() const { return guest::LoadFloat(base, address + 8); }
  void set_min_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t min_z_address() const { return address + 8; }
  // 1.0
  float min_w() const { return guest::LoadFloat(base, address + 12); }
  void set_min_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t min_w_address() const { return address + 12; }
  // starts at [0x8206CE40] (probably -FLT_MAX)
  float max_x() const { return guest::LoadFloat(base, address + 16); }
  void set_max_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t max_x_address() const { return address + 16; }
  float max_y() const { return guest::LoadFloat(base, address + 20); }
  void set_max_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t max_y_address() const { return address + 20; }
  float max_z() const { return guest::LoadFloat(base, address + 24); }
  void set_max_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t max_z_address() const { return address + 24; }
  // 1.0
  float max_w() const { return guest::LoadFloat(base, address + 28); }
  void set_max_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t max_w_address() const { return address + 28; }
};

// embedded; measure: Plane, Plane::SegmentCrossing.r4
struct CarScenePlane : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // x and y as one doubleword (the copies' ld/std)
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  // the normal (unit length in a frustum)
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  // z and d as one doubleword
  uint64_t zd() const { return guest::LoadU64(base, address + 8); }
  void set_zd(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zd_address() const { return address + 8; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  float d() const { return guest::LoadFloat(base, address + 12); }
  void set_d(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t d_address() const { return address + 12; }
};

// embedded; measure: Frustum
struct CarSceneFrustum : View {
  using View::View;
  static constexpr uint32_t kSize = 256;
  // six CarScenePlanes from the matrix's w column with and against x, y, z: w - x, w + x, w - y, w + y, w - z, w + z
  uint32_t planes_address() const { return address + 0; }
  // six words: each plane's code, 1 for x < 0, 2 for y, 4 for z
  uint32_t plane_codes_address() const { return address + 96; }
  // not written by FromMatrix
  uint32_t unknown_120_address() const { return address + 120; }
  // eight CarScenePoints (w 1.0), corner k from planes 4 or 5, 2 or 3 and 0 or 1 by k's bits
  uint32_t corners_address() const { return address + 128; }
};

// embedded; measure: Frustum::FromMatrix.r4
struct CarSceneMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // row 0, column 0
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

// embedded; measure: ScenePass
struct ScenePass : View {
  using View::View;
  static constexpr uint32_t kSize = 176;
  // the camera pass index (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 0); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t camera_pass_address() const { return address + 0; }
  // the shadows' camera passes
  uint32_t shadow_pass_a() const { return guest::LoadU32(base, address + 4); }
  void set_shadow_pass_a(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t shadow_pass_a_address() const { return address + 4; }
  uint32_t shadow_pass_b() const { return guest::LoadU32(base, address + 8); }
  void set_shadow_pass_b(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t shadow_pass_b_address() const { return address + 8; }
  uint32_t light_kit_pass() const { return guest::LoadU32(base, address + 12); }
  void set_light_kit_pass(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t light_kit_pass_address() const { return address + 12; }
  // probably
  uint32_t parts_shadow_pass() const { return guest::LoadU32(base, address + 16); }
  void set_parts_shadow_pass(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t parts_shadow_pass_address() const { return address + 16; }
  // copied to the render queue's [0x8296B90C] (Model::Draw's group 0)
  uint32_t group0_key() const { return guest::LoadU32(base, address + 20); }
  void set_group0_key(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t group0_key_address() const { return address + 20; }
  // copied to [0x8296B900] (group 1)
  uint32_t group1_key() const { return guest::LoadU32(base, address + 24); }
  void set_group1_key(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t group1_key_address() const { return address + 24; }
  // copied to [0x8296B908] (the depth buckets)
  uint32_t depth_key() const { return guest::LoadU32(base, address + 28); }
  void set_depth_key(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t depth_key_address() const { return address + 28; }
  // the shadow sets' keys, +32 to +48
  uint32_t shadow_set_key_0() const { return guest::LoadU32(base, address + 32); }
  void set_shadow_set_key_0(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t shadow_set_key_0_address() const { return address + 32; }
  uint32_t shadow_set_key_1() const { return guest::LoadU32(base, address + 36); }
  void set_shadow_set_key_1(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t shadow_set_key_1_address() const { return address + 36; }
  uint32_t shadow_set_key_2() const { return guest::LoadU32(base, address + 40); }
  void set_shadow_set_key_2(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t shadow_set_key_2_address() const { return address + 40; }
  uint32_t shadow_set_key_3() const { return guest::LoadU32(base, address + 44); }
  void set_shadow_set_key_3(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t shadow_set_key_3_address() const { return address + 44; }
  uint32_t shadow_set_key_4() const { return guest::LoadU32(base, address + 48); }
  void set_shadow_set_key_4(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t shadow_set_key_4_address() const { return address + 48; }
  uint32_t light_kit_key() const { return guest::LoadU32(base, address + 52); }
  void set_light_kit_key(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t light_kit_key_address() const { return address + 52; }
  uint32_t parts_shadow_key() const { return guest::LoadU32(base, address + 56); }
  void set_parts_shadow_key(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t parts_shadow_key_address() const { return address + 56; }
  // 0x4, 0x10 the plain shadow sets and the tires' depth pass, 0x8 the light kit, 0x20/0x40/0x80 the planar shadows from lights 0 to 2, 0x100 the parts' second depth draw
  uint32_t pass_mask() const { return guest::LoadU32(base, address + 60); }
  void set_pass_mask(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t pass_mask_address() const { return address + 60; }
  // [0x8296B90C] while a car draws (put back after it)
  uint32_t saved_group0_key() const { return guest::LoadU32(base, address + 64); }
  void set_saved_group0_key(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t saved_group0_key_address() const { return address + 64; }
  // [0x8296B900] while a car draws
  uint32_t saved_group1_key() const { return guest::LoadU32(base, address + 68); }
  void set_saved_group1_key(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t saved_group1_key_address() const { return address + 68; }
  // [0x8296B908] while a car draws
  uint32_t saved_depth_key() const { return guest::LoadU32(base, address + 72); }
  void set_saved_depth_key(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t saved_depth_key_address() const { return address + 72; }
  // [0x8296B8FC] saved (car-draw.md)
  uint32_t saved_key_8296B8FC() const { return guest::LoadU32(base, address + 76); }
  void set_saved_key_8296B8FC(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t saved_key_8296B8FC_address() const { return address + 76; }
  // [0x8296B8F8] saved (car-draw.md)
  uint32_t saved_key_8296B8F8() const { return guest::LoadU32(base, address + 80); }
  void set_saved_key_8296B8F8(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t saved_key_8296B8F8_address() const { return address + 80; }
  uint32_t unknown_84_address() const { return address + 84; }
  // ScenePass::SetLight: the light's direction normalized (stvx of the 16 bytes; probably the planar shadow's light)
  float light_x() const { return guest::LoadFloat(base, address + 96); }
  void set_light_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t light_x_address() const { return address + 96; }
  float light_y() const { return guest::LoadFloat(base, address + 100); }
  void set_light_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t light_y_address() const { return address + 100; }
  float light_z() const { return guest::LoadFloat(base, address + 104); }
  void set_light_z(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t light_z_address() const { return address + 104; }
  float light_w() const { return guest::LoadFloat(base, address + 108); }
  void set_light_w(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t light_w_address() const { return address + 108; }
  // SetLight: rows 0 to 2 of the identity (memcpy of 64 bytes from +112, its last row overwritten below)
  uint32_t light_matrix_address() const { return address + 112; }
  // the matrix's last row: -distance x the light's x, y, z, then w 1.0; x and y as one doubleword
  uint64_t light_shift_xy() const { return guest::LoadU64(base, address + 160); }
  void set_light_shift_xy(uint64_t v) const { guest::StoreU64(base, address + 160, v); }
  uint32_t light_shift_xy_address() const { return address + 160; }
  float light_shift_x() const { return guest::LoadFloat(base, address + 160); }
  void set_light_shift_x(float v) const { guest::StoreFloat(base, address + 160, v); }
  uint32_t light_shift_x_address() const { return address + 160; }
  float light_shift_y() const { return guest::LoadFloat(base, address + 164); }
  void set_light_shift_y(float v) const { guest::StoreFloat(base, address + 164, v); }
  uint32_t light_shift_y_address() const { return address + 164; }
  // z and w as one doubleword
  uint64_t light_shift_zw() const { return guest::LoadU64(base, address + 168); }
  void set_light_shift_zw(uint64_t v) const { guest::StoreU64(base, address + 168, v); }
  uint32_t light_shift_zw_address() const { return address + 168; }
  float light_shift_z() const { return guest::LoadFloat(base, address + 168); }
  void set_light_shift_z(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t light_shift_z_address() const { return address + 168; }
  // 1.0
  float light_shift_w() const { return guest::LoadFloat(base, address + 172); }
  void set_light_shift_w(float v) const { guest::StoreFloat(base, address + 172, v); }
  uint32_t light_shift_w_address() const { return address + 172; }
};

// image; measure: -
struct CarSceneDrawRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // w0: the car view's camera pass (0x826BAC00 + 592 x it)
  uint32_t car_pass() const { return guest::LoadU32(base, address + 0); }
  void set_car_pass(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t car_pass_address() const { return address + 0; }
  // w1: the pass whose view-projection (+144) gets the headlight projector's
  uint32_t headlight_pass() const { return guest::LoadU32(base, address + 4); }
  void set_headlight_pass(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t headlight_pass_address() const { return address + 4; }
  // w2: the first height fit's (straight down)
  uint32_t fit_down_pass() const { return guest::LoadU32(base, address + 8); }
  void set_fit_down_pass(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fit_down_pass_address() const { return address + 8; }
  // w3: the second height fit's
  uint32_t fit_band_pass() const { return guest::LoadU32(base, address + 12); }
  void set_fit_band_pass(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fit_band_pass_address() const { return address + 12; }
  // w4: the light's projection's
  uint32_t light_pass() const { return guest::LoadU32(base, address + 16); }
  void set_light_pass(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t light_pass_address() const { return address + 16; }
  // w5: the CameraPasses::RenderTarget id on sampler 8
  uint32_t headlight_target() const { return guest::LoadU32(base, address + 20); }
  void set_headlight_target(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t headlight_target_address() const { return address + 20; }
  // w6: on sampler 9
  uint32_t fit_down_target() const { return guest::LoadU32(base, address + 24); }
  void set_fit_down_target(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t fit_down_target_address() const { return address + 24; }
  // w7: on sampler 10
  uint32_t fit_band_target() const { return guest::LoadU32(base, address + 28); }
  void set_fit_band_target(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t fit_band_target_address() const { return address + 28; }
  // w8: on sampler 7
  uint32_t light_target() const { return guest::LoadU32(base, address + 32); }
  void set_light_target(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t light_target_address() const { return address + 32; }
  // w9: the scene model's index (through CarScene::PlayerModel)
  uint32_t model_index() const { return guest::LoadU32(base, address + 36); }
  void set_model_index(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t model_index_address() const { return address + 36; }
  // w10: the render queue keys of the five chunks: the headlight projector
  uint32_t headlight_key() const { return guest::LoadU32(base, address + 40); }
  void set_headlight_key(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t headlight_key_address() const { return address + 40; }
  // w11: samplers 11 to 15
  uint32_t shared_textures_key() const { return guest::LoadU32(base, address + 44); }
  void set_shared_textures_key(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t shared_textures_key_address() const { return address + 44; }
  // w12
  uint32_t fit_down_key() const { return guest::LoadU32(base, address + 48); }
  void set_fit_down_key(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t fit_down_key_address() const { return address + 48; }
  // w13
  uint32_t fit_band_key() const { return guest::LoadU32(base, address + 52); }
  void set_fit_band_key(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t fit_band_key_address() const { return address + 52; }
  // w14
  uint32_t light_key() const { return guest::LoadU32(base, address + 56); }
  void set_light_key(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t light_key_address() const { return address + 56; }
};

// The image words docs/functions/car-scene-draw.md's code uses.
struct CarSceneDrawGlobals {
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // the first path's light projection's far plane
  static float light_far_plane(uint8_t* base) { return guest::LoadFloat(base, 0x82000C34u); }
  static void set_light_far_plane(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000C34u, v); }
  static constexpr uint32_t light_far_plane_address() { return 0x82000C34u; }
  // the light's fit: its far cut (x the ratio)
  static float fit_light_cut(uint8_t* base) { return guest::LoadFloat(base, 0x82000D44u); }
  static void set_fit_light_cut(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000D44u, v); }
  static constexpr uint32_t fit_light_cut_address() { return 0x82000D44u; }
  // -1.0
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // probably 0.5: Matrix::Perspective's field-of-view factor, the fit's box center, the second path's ambient light
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // the second path's light direction's x; also TextureProjection::Fit's flip +484
  static float light_x(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE8u); }
  static void set_light_x(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE8u, v); }
  static constexpr uint32_t light_x_address() { return 0x82001BE8u; }
  // the first fit's top above the car
  static float fit_down_raise(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_fit_down_raise(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t fit_down_raise_address() { return 0x82001FD8u; }
  // the first path: the light's eye from the model's position along the direction
  static float light_eye_distance(uint8_t* base) { return guest::LoadFloat(base, 0x820021E4u); }
  static void set_light_eye_distance(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E4u, v); }
  static constexpr uint32_t light_eye_distance_address() { return 0x820021E4u; }
  // probably 2.0 (BuildOrtho's and the fit's scale)
  static float two(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_two(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t two_address() { return 0x820021E8u; }
  // the fits' height band around the car; the first path's orthographic size
  static float fit_height_band(uint8_t* base) { return guest::LoadFloat(base, 0x820021F4u); }
  static void set_fit_height_band(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021F4u, v); }
  static constexpr uint32_t fit_height_band_address() { return 0x820021F4u; }
  // the first fit's distance scale; the second path's SetLight distance
  static float fit_down_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002204u); }
  static void set_fit_down_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002204u, v); }
  static constexpr uint32_t fit_down_scale_address() { return 0x82002204u; }
  // the light's fit's distance scale
  static float fit_light_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8200222Cu); }
  static void set_fit_light_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200222Cu, v); }
  static constexpr uint32_t fit_light_scale_address() { return 0x8200222Cu; }
  // the light's fit's least distance (x the ratio)
  static float fit_light_least(uint8_t* base) { return guest::LoadFloat(base, 0x82002674u); }
  static void set_fit_light_least(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002674u, v); }
  static constexpr uint32_t fit_light_least_address() { return 0x82002674u; }
  // TextureProjection::Fit's flip +488
  static float flip_488(uint8_t* base) { return guest::LoadFloat(base, 0x820027A8u); }
  static void set_flip_488(uint8_t* base, float v) { guest::StoreFloat(base, 0x820027A8u, v); }
  static constexpr uint32_t flip_488_address() { return 0x820027A8u; }
  // the second path's second light vector's x, y, z
  static float diffuse(uint8_t* base) { return guest::LoadFloat(base, 0x8200296Cu); }
  static void set_diffuse(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200296Cu, v); }
  static constexpr uint32_t diffuse_address() { return 0x8200296Cu; }
  // the high beams' color factor
  static float beam_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820029D4u); }
  static void set_beam_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D4u, v); }
  static constexpr uint32_t beam_scale_address() { return 0x820029D4u; }
  // the second fit's far cut (x the ratio)
  static float fit_band_cut(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_fit_band_cut(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t fit_band_cut_address() { return 0x82002A0Cu; }
  // the second fit's distance scale
  static float fit_band_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002A1Cu); }
  static void set_fit_band_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A1Cu, v); }
  static constexpr uint32_t fit_band_scale_address() { return 0x82002A1Cu; }
  // the first fit's far cut (x the ratio)
  static float fit_down_cut(uint8_t* base) { return guest::LoadFloat(base, 0x82002C38u); }
  static void set_fit_down_cut(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C38u, v); }
  static constexpr uint32_t fit_down_cut_address() { return 0x82002C38u; }
  // the second fit's least distance (x the ratio)
  static float fit_band_least(uint8_t* base) { return guest::LoadFloat(base, 0x82002CACu); }
  static void set_fit_band_least(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002CACu, v); }
  static constexpr uint32_t fit_band_least_address() { return 0x82002CACu; }
  // the headlight projector's far plane
  static float headlight_far_plane(uint8_t* base) { return guest::LoadFloat(base, 0x82002FA8u); }
  static void set_headlight_far_plane(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002FA8u, v); }
  static constexpr uint32_t headlight_far_plane_address() { return 0x82002FA8u; }
  // over the car view's field of view: the fits' ratio
  static float fit_ratio_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002FACu); }
  static void set_fit_ratio_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002FACu, v); }
  static constexpr uint32_t fit_ratio_scale_address() { return 0x82002FACu; }
  // the first path's SetLight distance
  static float light_distance(uint8_t* base) { return guest::LoadFloat(base, 0x82002FB0u); }
  static void set_light_distance(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002FB0u, v); }
  static constexpr uint32_t light_distance_address() { return 0x82002FB0u; }
  // Box::FromPoints' starting max (probably -FLT_MAX)
  static float box_max(uint8_t* base) { return guest::LoadFloat(base, 0x8206CE40u); }
  static void set_box_max(uint8_t* base, float v) { guest::StoreFloat(base, 0x8206CE40u, v); }
  static constexpr uint32_t box_max_address() { return 0x8206CE40u; }
  // Box::FromPoints' starting min (probably FLT_MAX)
  static float box_min(uint8_t* base) { return guest::LoadFloat(base, 0x8206CE44u); }
  static void set_box_min(uint8_t* base, float v) { guest::StoreFloat(base, 0x8206CE44u, v); }
  static constexpr uint32_t box_min_address() { return 0x8206CE44u; }
  // Plane::SegmentCrossing's smallest |plane . direction|
  static float crossing_epsilon(uint8_t* base) { return guest::LoadFloat(base, 0x8206CE48u); }
  static void set_crossing_epsilon(uint8_t* base, float v) { guest::StoreFloat(base, 0x8206CE48u, v); }
  static constexpr uint32_t crossing_epsilon_address() { return 0x8206CE48u; }
  // TextureProjection::Fit's second +208 matrix's +264
  static float flip_264(uint8_t* base) { return guest::LoadFloat(base, 0x8206CE4Cu); }
  static void set_flip_264(uint8_t* base, float v) { guest::StoreFloat(base, 0x8206CE4Cu, v); }
  static constexpr uint32_t flip_264_address() { return 0x8206CE4Cu; }
  // three CarSceneDrawRecords
  static constexpr uint32_t records_address() { return 0x82002D40u; }
  // four 64-byte pass descriptions (ScenePass's first 64 bytes) for the scene's +4 not 0
  static constexpr uint32_t pass_descriptions_address() { return 0x82002DF8u; }
  // the one for the scene's +4 = 0
  static constexpr uint32_t front_end_description_address() { return 0x82002EF8u; }
  // instead when the mode type [0x826032DC] is 8 (never made in play)
  static constexpr uint32_t mode8_description_address() { return 0x82002F38u; }
  // the render queue's [0x8296B90C] at the pass loop's start
  static uint32_t queue_key_a_start(uint8_t* base) { return guest::LoadU32(base, 0x82601854u); }
  static void set_queue_key_a_start(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82601854u, v); }
  static constexpr uint32_t queue_key_a_start_address() { return 0x82601854u; }
  // [0x8296B900] at the pass loop's start
  static uint32_t queue_key_b_start(uint8_t* base) { return guest::LoadU32(base, 0x82601858u); }
  static void set_queue_key_b_start(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82601858u, v); }
  static constexpr uint32_t queue_key_b_start_address() { return 0x82601858u; }
  // the mode type (8 picks 0x82002F38)
  static uint32_t mode_type(uint8_t* base) { return guest::LoadU32(base, 0x826032DCu); }
  static void set_mode_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032DCu, v); }
  static constexpr uint32_t mode_type_address() { return 0x826032DCu; }
  // the draws' shared view matrix (scenery-lists.md): the pass's +16
  static constexpr uint32_t view_matrix_address() { return 0x8265ECA0u; }
  // the draws' shared eye: the pass's +400
  static constexpr uint32_t view_eye_address() { return 0x8265ECE0u; }
  // pixel c40's x, y, z: the scene's +5280
  static constexpr uint32_t light_vector_address() { return 0x8265ED00u; }
  // the bias matrix the projectors and the fit end with
  static constexpr uint32_t bias_matrix_address() { return 0x8265EED0u; }
  // the render manager (app-start-up.md)
  static View render_manager(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826BABA0u)); }
  static void set_render_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABA0u, v); }
  static constexpr uint32_t render_manager_address() { return 0x826BABA0u; }
  // the viewports' pointer array (camera-modes.md): +4 v viewport v (+1860 its view mode)
  static View viewports(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826C2CE8u)); }
  static void set_viewports(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // the car scene (scene.md)
  template <typename T = CarScene>
  static T scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // the race data (+0xA3AF0 the race kind, +0x59614, +0x59620 the stages, +0x59AE4)
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the word Minimap::Draw reads for the human car (0x826C7820's +4)
  static uint32_t second_human(uint8_t* base) { return guest::LoadU32(base, 0x826C7824u); }
  static void set_second_human(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7824u, v); }
  static constexpr uint32_t second_human_address() { return 0x826C7824u; }
  // the shared assets' texture, or render target 1
  static uint32_t shared_texture(uint8_t* base) { return guest::LoadU32(base, 0x8296B8F0u); }
  static void set_shared_texture(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B8F0u, v); }
  static constexpr uint32_t shared_texture_address() { return 0x8296B8F0u; }
  // the render queue's key for Model::Draw's group 1
  static uint32_t queue_key_group1(uint8_t* base) { return guest::LoadU32(base, 0x8296B900u); }
  static void set_queue_key_group1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B900u, v); }
  static constexpr uint32_t queue_key_group1_address() { return 0x8296B900u; }
  // the depth buckets' key
  static uint32_t queue_key_depth(uint8_t* base) { return guest::LoadU32(base, 0x8296B908u); }
  static void set_queue_key_depth(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B908u, v); }
  static constexpr uint32_t queue_key_depth_address() { return 0x8296B908u; }
  // group 0's key
  static uint32_t queue_key_group0(uint8_t* base) { return guest::LoadU32(base, 0x8296B90Cu); }
  static void set_queue_key_group0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B90Cu, v); }
  static constexpr uint32_t queue_key_group0_address() { return 0x8296B90Cu; }
  // the scene lights' first vector (the light's direction)
  static constexpr uint32_t light_direction_address() { return 0x8296B950u; }
  // three scene light vectors: +0 the ambient ([0x82001BE4] x3, 1.0), +16 the position, +32 (0, 0, 0, 1)
  static constexpr uint32_t light_vectors_a_address() { return 0x8296B980u; }
  // three: +0 ([0x8200296C] x3, 1.0), +16 and +32 (0, 0, 0, 1)
  static constexpr uint32_t light_vectors_b_address() { return 0x8296B9B0u; }
  // the scene lights' slots 0 to 7 (16 bytes): the other cars' headlight positions
  static constexpr uint32_t headlight_positions_address() { return 0x8296B9E0u; }
  // slots 0 to 7: the beams' shapes (SceneModel::BeamShape)
  static constexpr uint32_t headlight_shapes_address() { return 0x8296BA60u; }
  // slots 0 to 7: the first colors (the scene's +5248 to +5256 x BeamColor)
  static constexpr uint32_t headlight_colors_a_address() { return 0x8296BAE0u; }
  // slots 0 to 7: the second colors (the scene's +5264 to +5272)
  static constexpr uint32_t headlight_colors_b_address() { return 0x8296BB60u; }
  // slots 0 to 7: the beams' directions
  static constexpr uint32_t headlight_directions_address() { return 0x8296BBE0u; }
  // slots 0 to 7: the scene's +5440
  static constexpr uint32_t headlight_vectors_address() { return 0x8296BC60u; }
};

}  // namespace schema
