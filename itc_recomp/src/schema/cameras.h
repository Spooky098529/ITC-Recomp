// The views of docs/functions/cameras.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/cameras.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: CameraPass, Camera, Billboards.+0
struct CameraPass : View {
  using View::View;
  static constexpr uint32_t kSize = 592;
  // 0x82001EC0 (Camera); 0x820027B0 for AppCameraCamera
  template <typename T = CameraVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the view matrix (4x4 floats, row-major, row vectors)
  uint32_t view_address() const { return address + 16; }
  // the projection (Camera::BuildProjection)
  uint32_t projection_address() const { return address + 80; }
  // view x projection (Matrix::Multiply)
  uint32_t view_projection_address() const { return address + 144; }
  // Camera::Update's view as built, then copied to +16
  uint32_t view_built_address() const { return address + 208; }
  // the position Camera::Update builds the view from (w 1.0); x and y as one doubleword (the originals' ld/std copies)
  uint64_t position_xy() const { return guest::LoadU64(base, address + 272); }
  void set_position_xy(uint64_t v) const { guest::StoreU64(base, address + 272, v); }
  uint32_t position_xy_address() const { return address + 272; }
  // the position Camera::Update builds the view from (w 1.0)
  float position_x() const { return guest::LoadFloat(base, address + 272); }
  void set_position_x(float v) const { guest::StoreFloat(base, address + 272, v); }
  uint32_t position_x_address() const { return address + 272; }
  float position_y() const { return guest::LoadFloat(base, address + 276); }
  void set_position_y(float v) const { guest::StoreFloat(base, address + 276, v); }
  uint32_t position_y_address() const { return address + 276; }
  // z and w as one doubleword
  uint64_t position_zw() const { return guest::LoadU64(base, address + 280); }
  void set_position_zw(uint64_t v) const { guest::StoreU64(base, address + 280, v); }
  uint32_t position_zw_address() const { return address + 280; }
  float position_z() const { return guest::LoadFloat(base, address + 280); }
  void set_position_z(float v) const { guest::StoreFloat(base, address + 280, v); }
  uint32_t position_z_address() const { return address + 280; }
  float position_w() const { return guest::LoadFloat(base, address + 284); }
  void set_position_w(float v) const { guest::StoreFloat(base, address + 284, v); }
  uint32_t position_w_address() const { return address + 284; }
  // the Euler angles: x turns by RotateX, y by RotateY, z by RotateZ (negated); x and y as one doubleword (the originals' ld/std copies)
  uint64_t angles_xy() const { return guest::LoadU64(base, address + 288); }
  void set_angles_xy(uint64_t v) const { guest::StoreU64(base, address + 288, v); }
  uint32_t angles_xy_address() const { return address + 288; }
  // the Euler angles: x turns by RotateX, y by RotateY, z by RotateZ (negated)
  float angles_x() const { return guest::LoadFloat(base, address + 288); }
  void set_angles_x(float v) const { guest::StoreFloat(base, address + 288, v); }
  uint32_t angles_x_address() const { return address + 288; }
  float angles_y() const { return guest::LoadFloat(base, address + 292); }
  void set_angles_y(float v) const { guest::StoreFloat(base, address + 292, v); }
  uint32_t angles_y_address() const { return address + 292; }
  // z and w as one doubleword
  uint64_t angles_zw() const { return guest::LoadU64(base, address + 296); }
  void set_angles_zw(uint64_t v) const { guest::StoreU64(base, address + 296, v); }
  uint32_t angles_zw_address() const { return address + 296; }
  float angles_z() const { return guest::LoadFloat(base, address + 296); }
  void set_angles_z(float v) const { guest::StoreFloat(base, address + 296, v); }
  uint32_t angles_z_address() const { return address + 296; }
  float angles_w() const { return guest::LoadFloat(base, address + 300); }
  void set_angles_w(float v) const { guest::StoreFloat(base, address + 300, v); }
  uint32_t angles_w_address() const { return address + 300; }
  // a 4x4 matrix, probably (Camera::Camera sets its rows' fourth words to 1.0)
  uint32_t unknown_304_address() const { return address + 304; }
  // radians, wrapped into [-pi, pi]
  float field_of_view() const { return guest::LoadFloat(base, address + 368); }
  void set_field_of_view(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t field_of_view_address() const { return address + 368; }
  // Camera::Update: +380 x 0.5 / the half field of view's cosine polynomial x its sine polynomial
  float view_scale() const { return guest::LoadFloat(base, address + 372); }
  void set_view_scale(float v) const { guest::StoreFloat(base, address + 372, v); }
  uint32_t view_scale_address() const { return address + 372; }
  // the viewport's width
  float width() const { return guest::LoadFloat(base, address + 376); }
  void set_width(float v) const { guest::StoreFloat(base, address + 376, v); }
  uint32_t width_address() const { return address + 376; }
  // the viewport's height
  float height() const { return guest::LoadFloat(base, address + 380); }
  void set_height(float v) const { guest::StoreFloat(base, address + 380, v); }
  uint32_t height_address() const { return address + 380; }
  // the near plane
  float near() const { return guest::LoadFloat(base, address + 384); }
  void set_near(float v) const { guest::StoreFloat(base, address + 384, v); }
  uint32_t near_address() const { return address + 384; }
  // the far plane
  float far() const { return guest::LoadFloat(base, address + 388); }
  void set_far(float v) const { guest::StoreFloat(base, address + 388, v); }
  uint32_t far_address() const { return address + 388; }
  // width / height
  float aspect() const { return guest::LoadFloat(base, address + 392); }
  void set_aspect(float v) const { guest::StoreFloat(base, address + 392, v); }
  uint32_t aspect_address() const { return address + 392; }
  // the eye (CameraPass::FromViewport: a viewport entry's +16); x and y as one doubleword (the originals' ld/std copies)
  uint64_t eye_xy() const { return guest::LoadU64(base, address + 400); }
  void set_eye_xy(uint64_t v) const { guest::StoreU64(base, address + 400, v); }
  uint32_t eye_xy_address() const { return address + 400; }
  // the eye (CameraPass::FromViewport: a viewport entry's +16)
  float eye_x() const { return guest::LoadFloat(base, address + 400); }
  void set_eye_x(float v) const { guest::StoreFloat(base, address + 400, v); }
  uint32_t eye_x_address() const { return address + 400; }
  float eye_y() const { return guest::LoadFloat(base, address + 404); }
  void set_eye_y(float v) const { guest::StoreFloat(base, address + 404, v); }
  uint32_t eye_y_address() const { return address + 404; }
  // z and w as one doubleword
  uint64_t eye_zw() const { return guest::LoadU64(base, address + 408); }
  void set_eye_zw(uint64_t v) const { guest::StoreU64(base, address + 408, v); }
  uint32_t eye_zw_address() const { return address + 408; }
  float eye_z() const { return guest::LoadFloat(base, address + 408); }
  void set_eye_z(float v) const { guest::StoreFloat(base, address + 408, v); }
  uint32_t eye_z_address() const { return address + 408; }
  float eye_w() const { return guest::LoadFloat(base, address + 412); }
  void set_eye_w(float v) const { guest::StoreFloat(base, address + 412, v); }
  uint32_t eye_w_address() const { return address + 412; }
  // the target; x and y as one doubleword (the originals' ld/std copies)
  uint64_t target_xy() const { return guest::LoadU64(base, address + 416); }
  void set_target_xy(uint64_t v) const { guest::StoreU64(base, address + 416, v); }
  uint32_t target_xy_address() const { return address + 416; }
  // the target
  float target_x() const { return guest::LoadFloat(base, address + 416); }
  void set_target_x(float v) const { guest::StoreFloat(base, address + 416, v); }
  uint32_t target_x_address() const { return address + 416; }
  float target_y() const { return guest::LoadFloat(base, address + 420); }
  void set_target_y(float v) const { guest::StoreFloat(base, address + 420, v); }
  uint32_t target_y_address() const { return address + 420; }
  // z and w as one doubleword
  uint64_t target_zw() const { return guest::LoadU64(base, address + 424); }
  void set_target_zw(uint64_t v) const { guest::StoreU64(base, address + 424, v); }
  uint32_t target_zw_address() const { return address + 424; }
  float target_z() const { return guest::LoadFloat(base, address + 424); }
  void set_target_z(float v) const { guest::StoreFloat(base, address + 424, v); }
  uint32_t target_z_address() const { return address + 424; }
  float target_w() const { return guest::LoadFloat(base, address + 428); }
  void set_target_w(float v) const { guest::StoreFloat(base, address + 428, v); }
  uint32_t target_w_address() const { return address + 428; }
  // the up; x and y as one doubleword (the originals' ld/std copies)
  uint64_t up_xy() const { return guest::LoadU64(base, address + 432); }
  void set_up_xy(uint64_t v) const { guest::StoreU64(base, address + 432, v); }
  uint32_t up_xy_address() const { return address + 432; }
  // the up
  float up_x() const { return guest::LoadFloat(base, address + 432); }
  void set_up_x(float v) const { guest::StoreFloat(base, address + 432, v); }
  uint32_t up_x_address() const { return address + 432; }
  float up_y() const { return guest::LoadFloat(base, address + 436); }
  void set_up_y(float v) const { guest::StoreFloat(base, address + 436, v); }
  uint32_t up_y_address() const { return address + 436; }
  // z and w as one doubleword
  uint64_t up_zw() const { return guest::LoadU64(base, address + 440); }
  void set_up_zw(uint64_t v) const { guest::StoreU64(base, address + 440, v); }
  uint32_t up_zw_address() const { return address + 440; }
  float up_z() const { return guest::LoadFloat(base, address + 440); }
  void set_up_z(float v) const { guest::StoreFloat(base, address + 440, v); }
  uint32_t up_z_address() const { return address + 440; }
  float up_w() const { return guest::LoadFloat(base, address + 444); }
  void set_up_w(float v) const { guest::StoreFloat(base, address + 444, v); }
  uint32_t up_w_address() const { return address + 444; }
  // Matrix::RotateZ's angle on the view
  float roll() const { return guest::LoadFloat(base, address + 448); }
  void set_roll(float v) const { guest::StoreFloat(base, address + 448, v); }
  uint32_t roll_address() const { return address + 448; }
  // CameraPass::Update: target - eye, normalized (w 1.0); x and y as one doubleword (the originals' ld/std copies)
  uint64_t forward_xy() const { return guest::LoadU64(base, address + 464); }
  void set_forward_xy(uint64_t v) const { guest::StoreU64(base, address + 464, v); }
  uint32_t forward_xy_address() const { return address + 464; }
  // CameraPass::Update: target - eye, normalized (w 1.0)
  float forward_x() const { return guest::LoadFloat(base, address + 464); }
  void set_forward_x(float v) const { guest::StoreFloat(base, address + 464, v); }
  uint32_t forward_x_address() const { return address + 464; }
  float forward_y() const { return guest::LoadFloat(base, address + 468); }
  void set_forward_y(float v) const { guest::StoreFloat(base, address + 468, v); }
  uint32_t forward_y_address() const { return address + 468; }
  // z and w as one doubleword
  uint64_t forward_zw() const { return guest::LoadU64(base, address + 472); }
  void set_forward_zw(uint64_t v) const { guest::StoreU64(base, address + 472, v); }
  uint32_t forward_zw_address() const { return address + 472; }
  float forward_z() const { return guest::LoadFloat(base, address + 472); }
  void set_forward_z(float v) const { guest::StoreFloat(base, address + 472, v); }
  uint32_t forward_z_address() const { return address + 472; }
  float forward_w() const { return guest::LoadFloat(base, address + 476); }
  void set_forward_w(float v) const { guest::StoreFloat(base, address + 476, v); }
  uint32_t forward_w_address() const { return address + 476; }
  // CameraPass::Update: the forward crossed with +432, normalized (stvx)
  uint32_t up_again_address() const { return address + 480; }
  uint32_t viewport_x() const { return guest::LoadU32(base, address + 496); }
  void set_viewport_x(uint32_t v) const { guest::StoreU32(base, address + 496, v); }
  uint32_t viewport_x_address() const { return address + 496; }
  uint32_t viewport_y() const { return guest::LoadU32(base, address + 500); }
  void set_viewport_y(uint32_t v) const { guest::StoreU32(base, address + 500, v); }
  uint32_t viewport_y_address() const { return address + 500; }
  // pixels (Camera::SetViewportSize's width)
  uint32_t viewport_width() const { return guest::LoadU32(base, address + 504); }
  void set_viewport_width(uint32_t v) const { guest::StoreU32(base, address + 504, v); }
  uint32_t viewport_width_address() const { return address + 504; }
  // pixels; CameraPass::SetZoom reads it signed
  uint32_t viewport_height() const { return guest::LoadU32(base, address + 508); }
  void set_viewport_height(uint32_t v) const { guest::StoreU32(base, address + 508, v); }
  uint32_t viewport_height_address() const { return address + 508; }
  // 0 (CameraPass::Init)
  uint32_t unknown_512() const { return guest::LoadU32(base, address + 512); }
  void set_unknown_512(uint32_t v) const { guest::StoreU32(base, address + 512, v); }
  uint32_t unknown_512_address() const { return address + 512; }
  // CameraPass::SetZoom: the field of view is atan(height x 0.5 / zoom)
  float zoom() const { return guest::LoadFloat(base, address + 516); }
  void set_zoom(float v) const { guest::StoreFloat(base, address + 516, v); }
  uint32_t zoom_address() const { return address + 516; }
  // the float at 0x82000A30 (CameraPass::Init)
  float unknown_520() const { return guest::LoadFloat(base, address + 520); }
  void set_unknown_520(float v) const { guest::StoreFloat(base, address + 520, v); }
  uint32_t unknown_520_address() const { return address + 520; }
  // the float at 0x82000A30, 0x82001FD0 for type 0 (CameraPass::Init)
  float unknown_524() const { return guest::LoadFloat(base, address + 524); }
  void set_unknown_524(float v) const { guest::StoreFloat(base, address + 524, v); }
  uint32_t unknown_524_address() const { return address + 524; }
  // the float at 0x82000A34 (CameraPass::Init)
  float unknown_528() const { return guest::LoadFloat(base, address + 528); }
  void set_unknown_528(float v) const { guest::StoreFloat(base, address + 528, v); }
  uint32_t unknown_528_address() const { return address + 528; }
  // the float at 0x82001FD8 (CameraPass::Init)
  float unknown_532() const { return guest::LoadFloat(base, address + 532); }
  void set_unknown_532(float v) const { guest::StoreFloat(base, address + 532, v); }
  uint32_t unknown_532_address() const { return address + 532; }
  // the float at 0x82001FD4 (CameraPass::Init)
  float unknown_536() const { return guest::LoadFloat(base, address + 536); }
  void set_unknown_536(float v) const { guest::StoreFloat(base, address + 536, v); }
  uint32_t unknown_536_address() const { return address + 536; }
  // the float at 0x82001FD4 (CameraPass::Init)
  float unknown_540() const { return guest::LoadFloat(base, address + 540); }
  void set_unknown_540(float v) const { guest::StoreFloat(base, address + 540, v); }
  uint32_t unknown_540_address() const { return address + 540; }
  // the near plane CameraPass::Init copies to +384
  float init_near() const { return guest::LoadFloat(base, address + 544); }
  void set_init_near(float v) const { guest::StoreFloat(base, address + 544, v); }
  uint32_t init_near_address() const { return address + 544; }
  // the far plane CameraPass::Init copies to +388
  float init_far() const { return guest::LoadFloat(base, address + 548); }
  void set_init_far(float v) const { guest::StoreFloat(base, address + 548, v); }
  uint32_t init_far_address() const { return address + 548; }
  // 1 for types 1 to 3, else 0 (CameraPass::Init)
  uint32_t unknown_552() const { return guest::LoadU32(base, address + 552); }
  void set_unknown_552(uint32_t v) const { guest::StoreU32(base, address + 552, v); }
  uint32_t unknown_552_address() const { return address + 552; }
  // 0 (CameraPass::Init)
  uint32_t unknown_556() const { return guest::LoadU32(base, address + 556); }
  void set_unknown_556(uint32_t v) const { guest::StoreU32(base, address + 556, v); }
  uint32_t unknown_556_address() const { return address + 556; }
  // 1 (CameraPass::Init)
  uint32_t unknown_560() const { return guest::LoadU32(base, address + 560); }
  void set_unknown_560(uint32_t v) const { guest::StoreU32(base, address + 560, v); }
  uint32_t unknown_560_address() const { return address + 560; }
  // 0xFF000000, 0 for type 19
  uint32_t clear_color() const { return guest::LoadU32(base, address + 564); }
  void set_clear_color(uint32_t v) const { guest::StoreU32(base, address + 564, v); }
  uint32_t clear_color_address() const { return address + 564; }
  // 1 (CameraPass::Init)
  uint32_t unknown_568() const { return guest::LoadU32(base, address + 568); }
  void set_unknown_568(uint32_t v) const { guest::StoreU32(base, address + 568, v); }
  uint32_t unknown_568_address() const { return address + 568; }
  // an unsigned integer, recorded as a float
  uint32_t clear_depth() const { return guest::LoadU32(base, address + 572); }
  void set_clear_depth(uint32_t v) const { guest::StoreU32(base, address + 572, v); }
  uint32_t clear_depth_address() const { return address + 572; }
  // 0 for none
  template <typename T = CameraSurface>
  T color_surface() const { return T(base, guest::LoadU32(base, address + 576)); }
  void set_color_surface(const View& v) const { guest::StoreU32(base, address + 576, v.address); }
  void set_color_surface(uint32_t v) const { guest::StoreU32(base, address + 576, v); }
  uint32_t color_surface_address() const { return address + 576; }
  // 0 for none
  template <typename T = CameraSurface>
  T depth_surface() const { return T(base, guest::LoadU32(base, address + 580)); }
  void set_depth_surface(const View& v) const { guest::StoreU32(base, address + 580, v.address); }
  void set_depth_surface(uint32_t v) const { guest::StoreU32(base, address + 580, v); }
  uint32_t depth_surface_address() const { return address + 580; }
  // 0 to 20: sets the viewport, planes, clear and render targets
  uint32_t type() const { return guest::LoadU32(base, address + 584); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 584, v); }
  uint32_t type_address() const { return address + 584; }
  // 1 when RecordBegins and RecordEnds record it
  uint32_t enabled() const { return guest::LoadU32(base, address + 588); }
  void set_enabled(uint32_t v) const { guest::StoreU32(base, address + 588, v); }
  uint32_t enabled_address() const { return address + 588; }
};

// file; measure: CameraPass.+0, AppCamera.+28.+0, [826BB540]
struct CameraVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // slot 0: Camera::SetViewportSize (sub_82593500)
  uint32_t set_viewport_size() const { return guest::LoadU32(base, address + 0); }
  void set_set_viewport_size(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t set_viewport_size_address() const { return address + 0; }
  // slot 1: Camera::SetFieldOfView (sub_82593278)
  uint32_t set_field_of_view() const { return guest::LoadU32(base, address + 4); }
  void set_set_field_of_view(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t set_field_of_view_address() const { return address + 4; }
  // slot 2: Camera::Update (sub_82593568)
  uint32_t update() const { return guest::LoadU32(base, address + 8); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t update_address() const { return address + 8; }
  // slot 3: sub_825C8390 in AppCameraCamera's vtable
  uint32_t slot_3() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_address() const { return address + 12; }
  // slot 4: sub_8209FB80, never run
  uint32_t destructor() const { return guest::LoadU32(base, address + 16); }
  void set_destructor(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t destructor_address() const { return address + 16; }
};

// embedded; measure: Device::CreateSurface.ret
struct CameraSurface : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // the surface's EDRAM tiles x 5120 (SurfaceHeader::Set); the pass's depth surface starts after its color one (+44 / 5120 tiles)
  uint32_t edram_bytes() const { return guest::LoadU32(base, address + 44); }
  void set_edram_bytes(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t edram_bytes_address() const { return address + 44; }
};

// allocated; measure: new<48>@sub_822A5C50
struct CameraCubeFaceSurface : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // probably the cube map it's a face of
  View texture() const { return View(base, guest::LoadU32(base, address + 24)); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t texture_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
};

// allocated; measure: AppCamera
struct AppCamera : View {
  using View::View;
  static constexpr uint32_t kSize = 368;
  static constexpr uint32_t kVtable = 0x820026B8;
  // 0x820026B8
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 (AppCamera::AppCamera)
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // 0x820026A8 (0x82002680 first)
  uint32_t vtable_8() const { return guest::LoadU32(base, address + 8); }
  void set_vtable_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t vtable_8_address() const { return address + 8; }
  // 0x8200269C (0x82002690 first)
  uint32_t vtable_12() const { return guest::LoadU32(base, address + 12); }
  void set_vtable_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t vtable_12_address() const { return address + 12; }
  // the block's +4 (the app's +4)
  template <typename T = AppCameraService>
  T service() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_service(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_service(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t service_address() const { return address + 16; }
  // or 0
  template <typename T = AppCameraCamera>
  T camera() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_camera(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_camera(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t camera_address() const { return address + 28; }
  // or 0
  template <typename T = AppCameraList>
  T list() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_list(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_list(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t list_address() const { return address + 32; }
  // two vectors, probably (1.0 at +60 and +76)
  uint32_t unknown_48_address() const { return address + 48; }
  // a 4x4 matrix, probably (1.0 at its rows' fourth words)
  uint32_t unknown_96_address() const { return address + 96; }
  // a 4x4 matrix, probably (1.0 at its rows' fourth words)
  uint32_t unknown_160_address() const { return address + 160; }
  // a 4x4 matrix, probably (1.0 at its rows' fourth words)
  uint32_t unknown_240_address() const { return address + 240; }
  // 0 when made
  uint32_t unknown_316() const { return guest::LoadU32(base, address + 316); }
  void set_unknown_316(uint32_t v) const { guest::StoreU32(base, address + 316, v); }
  uint32_t unknown_316_address() const { return address + 316; }
  // 0 when made
  uint32_t unknown_320() const { return guest::LoadU32(base, address + 320); }
  void set_unknown_320(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t unknown_320_address() const { return address + 320; }
  // 1 when made
  uint32_t unknown_324() const { return guest::LoadU32(base, address + 324); }
  void set_unknown_324(uint32_t v) const { guest::StoreU32(base, address + 324, v); }
  uint32_t unknown_324_address() const { return address + 324; }
  // 1 when made
  uint32_t unknown_328() const { return guest::LoadU32(base, address + 328); }
  void set_unknown_328(uint32_t v) const { guest::StoreU32(base, address + 328, v); }
  uint32_t unknown_328_address() const { return address + 328; }
  // 10 when made
  uint32_t unknown_332() const { return guest::LoadU32(base, address + 332); }
  void set_unknown_332(uint32_t v) const { guest::StoreU32(base, address + 332, v); }
  uint32_t unknown_332_address() const { return address + 332; }
  // 10 when made
  uint32_t unknown_336() const { return guest::LoadU32(base, address + 336); }
  void set_unknown_336(uint32_t v) const { guest::StoreU32(base, address + 336, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  // 0 when made
  uint32_t unknown_340() const { return guest::LoadU32(base, address + 340); }
  void set_unknown_340(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t unknown_340_address() const { return address + 340; }
  // 2 when made
  uint32_t unknown_344() const { return guest::LoadU32(base, address + 344); }
  void set_unknown_344(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t unknown_344_address() const { return address + 344; }
};

// allocated; measure: AppCamera::Camera, AppCamera.+28
struct AppCameraCamera : View {
  using View::View;
  static constexpr uint32_t kSize = 832;
  static constexpr uint32_t kVtable = 0x820027B0;
  // 0x82001EC0 (Camera); 0x820027B0 for AppCameraCamera
  template <typename T = CameraVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the view matrix (4x4 floats, row-major, row vectors)
  uint32_t view_address() const { return address + 16; }
  // the projection (Camera::BuildProjection)
  uint32_t projection_address() const { return address + 80; }
  // view x projection (Matrix::Multiply)
  uint32_t view_projection_address() const { return address + 144; }
  // Camera::Update's view as built, then copied to +16
  uint32_t view_built_address() const { return address + 208; }
  // the position Camera::Update builds the view from (w 1.0); x and y as one doubleword (the originals' ld/std copies)
  uint64_t position_xy() const { return guest::LoadU64(base, address + 272); }
  void set_position_xy(uint64_t v) const { guest::StoreU64(base, address + 272, v); }
  uint32_t position_xy_address() const { return address + 272; }
  // the position Camera::Update builds the view from (w 1.0)
  float position_x() const { return guest::LoadFloat(base, address + 272); }
  void set_position_x(float v) const { guest::StoreFloat(base, address + 272, v); }
  uint32_t position_x_address() const { return address + 272; }
  float position_y() const { return guest::LoadFloat(base, address + 276); }
  void set_position_y(float v) const { guest::StoreFloat(base, address + 276, v); }
  uint32_t position_y_address() const { return address + 276; }
  // z and w as one doubleword
  uint64_t position_zw() const { return guest::LoadU64(base, address + 280); }
  void set_position_zw(uint64_t v) const { guest::StoreU64(base, address + 280, v); }
  uint32_t position_zw_address() const { return address + 280; }
  float position_z() const { return guest::LoadFloat(base, address + 280); }
  void set_position_z(float v) const { guest::StoreFloat(base, address + 280, v); }
  uint32_t position_z_address() const { return address + 280; }
  float position_w() const { return guest::LoadFloat(base, address + 284); }
  void set_position_w(float v) const { guest::StoreFloat(base, address + 284, v); }
  uint32_t position_w_address() const { return address + 284; }
  // the Euler angles: x turns by RotateX, y by RotateY, z by RotateZ (negated); x and y as one doubleword (the originals' ld/std copies)
  uint64_t angles_xy() const { return guest::LoadU64(base, address + 288); }
  void set_angles_xy(uint64_t v) const { guest::StoreU64(base, address + 288, v); }
  uint32_t angles_xy_address() const { return address + 288; }
  // the Euler angles: x turns by RotateX, y by RotateY, z by RotateZ (negated)
  float angles_x() const { return guest::LoadFloat(base, address + 288); }
  void set_angles_x(float v) const { guest::StoreFloat(base, address + 288, v); }
  uint32_t angles_x_address() const { return address + 288; }
  float angles_y() const { return guest::LoadFloat(base, address + 292); }
  void set_angles_y(float v) const { guest::StoreFloat(base, address + 292, v); }
  uint32_t angles_y_address() const { return address + 292; }
  // z and w as one doubleword
  uint64_t angles_zw() const { return guest::LoadU64(base, address + 296); }
  void set_angles_zw(uint64_t v) const { guest::StoreU64(base, address + 296, v); }
  uint32_t angles_zw_address() const { return address + 296; }
  float angles_z() const { return guest::LoadFloat(base, address + 296); }
  void set_angles_z(float v) const { guest::StoreFloat(base, address + 296, v); }
  uint32_t angles_z_address() const { return address + 296; }
  float angles_w() const { return guest::LoadFloat(base, address + 300); }
  void set_angles_w(float v) const { guest::StoreFloat(base, address + 300, v); }
  uint32_t angles_w_address() const { return address + 300; }
  // a 4x4 matrix, probably (Camera::Camera sets its rows' fourth words to 1.0)
  uint32_t unknown_304_address() const { return address + 304; }
  // radians, wrapped into [-pi, pi]
  float field_of_view() const { return guest::LoadFloat(base, address + 368); }
  void set_field_of_view(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t field_of_view_address() const { return address + 368; }
  // Camera::Update: +380 x 0.5 / the half field of view's cosine polynomial x its sine polynomial
  float view_scale() const { return guest::LoadFloat(base, address + 372); }
  void set_view_scale(float v) const { guest::StoreFloat(base, address + 372, v); }
  uint32_t view_scale_address() const { return address + 372; }
  // the viewport's width
  float width() const { return guest::LoadFloat(base, address + 376); }
  void set_width(float v) const { guest::StoreFloat(base, address + 376, v); }
  uint32_t width_address() const { return address + 376; }
  // the viewport's height
  float height() const { return guest::LoadFloat(base, address + 380); }
  void set_height(float v) const { guest::StoreFloat(base, address + 380, v); }
  uint32_t height_address() const { return address + 380; }
  // the near plane
  float near() const { return guest::LoadFloat(base, address + 384); }
  void set_near(float v) const { guest::StoreFloat(base, address + 384, v); }
  uint32_t near_address() const { return address + 384; }
  // the far plane
  float far() const { return guest::LoadFloat(base, address + 388); }
  void set_far(float v) const { guest::StoreFloat(base, address + 388, v); }
  uint32_t far_address() const { return address + 388; }
  // width / height
  float aspect() const { return guest::LoadFloat(base, address + 392); }
  void set_aspect(float v) const { guest::StoreFloat(base, address + 392, v); }
  uint32_t aspect_address() const { return address + 392; }
  // (0, 0, 0, 1) when made
  uint32_t unknown_400_address() const { return address + 400; }
  // (0, 0, 10, 1) when made
  uint32_t unknown_416_address() const { return address + 416; }
  // the position (+272) when made
  uint32_t unknown_432_address() const { return address + 432; }
  // +416 when made
  uint32_t unknown_448_address() const { return address + 448; }
  // +416 when made
  uint32_t unknown_464_address() const { return address + 464; }
  // 1.0 at +492 when made
  uint32_t unknown_480_address() const { return address + 480; }
  // 1.0 at +508 when made
  uint32_t unknown_496_address() const { return address + 496; }
  // 1.0 at +524 when made
  uint32_t unknown_512_address() const { return address + 512; }
  // 1.0 at +572 when made
  uint32_t unknown_560_address() const { return address + 560; }
  // 1.0 at +588 when made
  uint32_t unknown_576_address() const { return address + 576; }
  // 1.0 at +604 when made
  uint32_t unknown_592_address() const { return address + 592; }
  // 1.0 at +620 when made
  uint32_t unknown_608_address() const { return address + 608; }
  // (0, 0, 0, 1) when made
  uint32_t unknown_624_address() const { return address + 624; }
  // (0, 0, 0, 1) when made
  uint32_t unknown_640_address() const { return address + 640; }
  // (0, 0, 0, 1) when made
  uint32_t unknown_656_address() const { return address + 656; }
  // 45.0 when made
  float unknown_672() const { return guest::LoadFloat(base, address + 672); }
  void set_unknown_672(float v) const { guest::StoreFloat(base, address + 672, v); }
  uint32_t unknown_672_address() const { return address + 672; }
  // 0.09 when made
  float unknown_676() const { return guest::LoadFloat(base, address + 676); }
  void set_unknown_676(float v) const { guest::StoreFloat(base, address + 676, v); }
  uint32_t unknown_676_address() const { return address + 676; }
  // 1.5 when made
  float unknown_680() const { return guest::LoadFloat(base, address + 680); }
  void set_unknown_680(float v) const { guest::StoreFloat(base, address + 680, v); }
  uint32_t unknown_680_address() const { return address + 680; }
  // 0.0 when made
  float unknown_684() const { return guest::LoadFloat(base, address + 684); }
  void set_unknown_684(float v) const { guest::StoreFloat(base, address + 684, v); }
  uint32_t unknown_684_address() const { return address + 684; }
  // 30.0 when made
  float unknown_688() const { return guest::LoadFloat(base, address + 688); }
  void set_unknown_688(float v) const { guest::StoreFloat(base, address + 688, v); }
  uint32_t unknown_688_address() const { return address + 688; }
  // 200.0 when made
  float unknown_692() const { return guest::LoadFloat(base, address + 692); }
  void set_unknown_692(float v) const { guest::StoreFloat(base, address + 692, v); }
  uint32_t unknown_692_address() const { return address + 692; }
  // -89.0 when made
  float unknown_804() const { return guest::LoadFloat(base, address + 804); }
  void set_unknown_804(float v) const { guest::StoreFloat(base, address + 804, v); }
  uint32_t unknown_804_address() const { return address + 804; }
  // 89.0 when made
  float unknown_808() const { return guest::LoadFloat(base, address + 808); }
  void set_unknown_808(float v) const { guest::StoreFloat(base, address + 808, v); }
  uint32_t unknown_808_address() const { return address + 808; }
  // -45.0 when made
  float unknown_812() const { return guest::LoadFloat(base, address + 812); }
  void set_unknown_812(float v) const { guest::StoreFloat(base, address + 812, v); }
  uint32_t unknown_812_address() const { return address + 812; }
  // 45.0 when made
  float unknown_816() const { return guest::LoadFloat(base, address + 816); }
  void set_unknown_816(float v) const { guest::StoreFloat(base, address + 816, v); }
  uint32_t unknown_816_address() const { return address + 816; }
};

// allocated; measure: AppCameraList, AppCamera.+32, sub_820A9200.r3
struct AppCameraList : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr uint32_t kVtable = 0x82002780;
  // 0x82002780
  template <typename T = AppCameraListVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a std::vector of words (16 bytes) or 0
  template <typename T = NewAny_MessageVector_Allocate>
  T vector() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_vector(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_vector(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vector_address() const { return address + 4; }
};

// file; measure: AppCamera.+32.+0
struct AppCameraListVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 1: AppCameraList::Add (sub_820A9928)
  uint32_t add() const { return guest::LoadU32(base, address + 4); }
  void set_add(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t add_address() const { return address + 4; }
};

// allocated; measure: AppCameraEntry
struct AppCameraEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82002580;
  // 0x82002580
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 when made
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // 0 when made
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // 0 when made
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: AppCamera::AppCamera.r4
struct AppCameraBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the app's +20 interface: slots 1, 3 and 4 take handlers
  template <typename T = AppCameraHandlerHost>
  T handlers() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_handlers(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_handlers(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t handlers_address() const { return address + 0; }
  // the app's +4 interface: slot 1 gives the object whose slot 3 is the screen
  template <typename T = AppCameraService>
  T service() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_service(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_service(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t service_address() const { return address + 4; }
};

// embedded; measure: AppCamera::AppCamera.r4.+0
struct AppCameraHandlerHost : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the measure files its vtable's slots here (a measure fault): slot 1
  uint32_t vtable_slot_1() const { return guest::LoadU32(base, address + 4); }
  void set_vtable_slot_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vtable_slot_1_address() const { return address + 4; }
  // as +4: slot 3
  uint32_t vtable_slot_3() const { return guest::LoadU32(base, address + 12); }
  void set_vtable_slot_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t vtable_slot_3_address() const { return address + 12; }
  // as +4: slot 4
  uint32_t vtable_slot_4() const { return guest::LoadU32(base, address + 16); }
  void set_vtable_slot_4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vtable_slot_4_address() const { return address + 16; }
};

// embedded; measure: AppCamera::AppCamera.r4.+4
struct AppCameraService : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AppCameraServiceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// file; measure: AppCamera::AppCamera.r4.+4.+0
struct AppCameraServiceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // gives the object whose slot 3 is the screen (its +296, +300)
  uint32_t slot_1() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
};

// file; measure: new<?>@MessageVector::Allocate, new<16>@AppCameraList::AppCameraList
struct NewAny_MessageVector_Allocate : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // as storage: the first element; a vector object leaves it unwritten
  uint32_t word_0() const { return guest::LoadU32(base, address + 0); }
  void set_word_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_0_address() const { return address + 0; }
  // a vector of words' first (sub_820A93E8, sub_820A9818); as storage the second element
  uint32_t first() const { return guest::LoadU32(base, address + 4); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  // the vector's last; as storage the third element
  uint32_t last() const { return guest::LoadU32(base, address + 8); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  // the vector's end; as storage the fourth element
  template <typename T = NewAny_MessageVector_Allocate>
  T end() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_end(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
};

// embedded; measure: new<?>@MessageVector::Allocate.+8, sub_820A93E8.r6, sub_820A9818.r4
struct CameraListWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // a word pushed onto AppCameraList's vector, or the slot it goes to
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: sub_820A9738.r3
struct CameraListIterator : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // {vector, element} stored as one doubleword
  uint64_t pair() const { return guest::LoadU64(base, address + 0); }
  void set_pair(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t pair_address() const { return address + 0; }
};

// embedded; measure: CourseView
struct CourseView : View {
  using View::View;
  static constexpr uint32_t kSize = 672;
  // Viewport::Set reads its eye from +0 (eight bytes at a time)
  uint64_t eye_xy() const { return guest::LoadU64(base, address + 0); }
  void set_eye_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t eye_xy_address() const { return address + 0; }
  uint64_t eye_zw() const { return guest::LoadU64(base, address + 8); }
  void set_eye_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t eye_zw_address() const { return address + 8; }
  // CourseView::Draw updates and draws this one's scenery (Scenery::Update, Draw)
  uint32_t scenery() const { return guest::LoadU32(base, address + 552); }
  void set_scenery(uint32_t v) const { guest::StoreU32(base, address + 552, v); }
  uint32_t scenery_address() const { return address + 552; }
  float unknown_664() const { return guest::LoadFloat(base, address + 664); }
  void set_unknown_664(float v) const { guest::StoreFloat(base, address + 664, v); }
  uint32_t unknown_664_address() const { return address + 664; }
  float unknown_668() const { return guest::LoadFloat(base, address + 668); }
  void set_unknown_668(float v) const { guest::StoreFloat(base, address + 668, v); }
  uint32_t unknown_668_address() const { return address + 668; }
};

// embedded; measure: Viewport::Set.r6, Viewport::Set.r7, Viewport::LookAt.r4, Viewport::LookAt.r5
struct CameraVector4 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // copied eight bytes at a time
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
};

// The image words docs/functions/cameras.md's code uses.
struct CamerasGlobals {
  // CameraPasses::SetMode's: 0 a 2D render target 1, 1 a cube map
  static uint32_t mode(uint8_t* base) { return guest::LoadU32(base, 0x826BABC0u); }
  static void set_mode(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABC0u, v); }
  static constexpr uint32_t mode_address() { return 0x826BABC0u; }
  // a texture (CameraPasses::RenderTarget's ids 0 to 12)
  static uint32_t render_target_12(uint8_t* base) { return guest::LoadU32(base, 0x826BABC4u); }
  static void set_render_target_12(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABC4u, v); }
  static constexpr uint32_t render_target_12_address() { return 0x826BABC4u; }
  static uint32_t render_target_3(uint8_t* base) { return guest::LoadU32(base, 0x826BABC8u); }
  static void set_render_target_3(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABC8u, v); }
  static constexpr uint32_t render_target_3_address() { return 0x826BABC8u; }
  // id 1 in mode 1
  static uint32_t render_target_1_cube(uint8_t* base) { return guest::LoadU32(base, 0x826BABCCu); }
  static void set_render_target_1_cube(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABCCu, v); }
  static constexpr uint32_t render_target_1_cube_address() { return 0x826BABCCu; }
  static uint32_t render_target_9(uint8_t* base) { return guest::LoadU32(base, 0x826BABD0u); }
  static void set_render_target_9(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABD0u, v); }
  static constexpr uint32_t render_target_9_address() { return 0x826BABD0u; }
  static uint32_t render_target_4(uint8_t* base) { return guest::LoadU32(base, 0x826BABD4u); }
  static void set_render_target_4(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABD4u, v); }
  static constexpr uint32_t render_target_4_address() { return 0x826BABD4u; }
  static uint32_t render_target_10(uint8_t* base) { return guest::LoadU32(base, 0x826BABD8u); }
  static void set_render_target_10(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABD8u, v); }
  static constexpr uint32_t render_target_10_address() { return 0x826BABD8u; }
  static uint32_t render_target_2(uint8_t* base) { return guest::LoadU32(base, 0x826BABDCu); }
  static void set_render_target_2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABDCu, v); }
  static constexpr uint32_t render_target_2_address() { return 0x826BABDCu; }
  static uint32_t render_target_11(uint8_t* base) { return guest::LoadU32(base, 0x826BABE0u); }
  static void set_render_target_11(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABE0u, v); }
  static constexpr uint32_t render_target_11_address() { return 0x826BABE0u; }
  // handed to CameraPass::Init as r5 (not read by it); 0 at boot
  static uint32_t init_argument(uint8_t* base) { return guest::LoadU32(base, 0x826BABE4u); }
  static void set_init_argument(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABE4u, v); }
  static constexpr uint32_t init_argument_address() { return 0x826BABE4u; }
  static uint32_t render_target_0(uint8_t* base) { return guest::LoadU32(base, 0x826BABE8u); }
  static void set_render_target_0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABE8u, v); }
  static constexpr uint32_t render_target_0_address() { return 0x826BABE8u; }
  // id 1 in mode 0
  static uint32_t render_target_1_2d(uint8_t* base) { return guest::LoadU32(base, 0x826BABECu); }
  static void set_render_target_1_2d(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABECu, v); }
  static constexpr uint32_t render_target_1_2d_address() { return 0x826BABECu; }
  static uint32_t render_target_8(uint8_t* base) { return guest::LoadU32(base, 0x826BABF0u); }
  static void set_render_target_8(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABF0u, v); }
  static constexpr uint32_t render_target_8_address() { return 0x826BABF0u; }
  static uint32_t render_target_7(uint8_t* base) { return guest::LoadU32(base, 0x826BABF4u); }
  static void set_render_target_7(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABF4u, v); }
  static constexpr uint32_t render_target_7_address() { return 0x826BABF4u; }
  static uint32_t render_target_6(uint8_t* base) { return guest::LoadU32(base, 0x826BABF8u); }
  static void set_render_target_6(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABF8u, v); }
  static constexpr uint32_t render_target_6_address() { return 0x826BABF8u; }
  static uint32_t render_target_5(uint8_t* base) { return guest::LoadU32(base, 0x826BABFCu); }
  static void set_render_target_5(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABFCu, v); }
  static constexpr uint32_t render_target_5_address() { return 0x826BABFCu; }
  // the 21 camera passes (CameraPass), 592 bytes each
  static constexpr uint32_t passes_address() { return 0x826BAC00u; }
  // the viewports' 12-byte block (camera-modes.toml): +0 and +4 the two viewports, +8 whether both are in use
  template <typename T = Viewports>
  static T viewports(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2CE8u)); }
  static void set_viewports(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2CE8u, v.address); }
  static void set_viewports(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // (0, 1, 0, 1), made once by Viewport::Init
  static constexpr uint32_t up_vector_address() { return 0x826C2CF0u; }
  // (10, 0, 0, 1), made once by Viewport::Init
  static constexpr uint32_t side_vector_address() { return 0x826C2D00u; }
  // (0, 0, 0, 1), made once by Viewport::Init
  static constexpr uint32_t origin_vector_address() { return 0x826C2D10u; }
  // bits 1, 2, 4: origin_vector, side_vector, up_vector made
  static uint32_t vector_guard(uint8_t* base) { return guest::LoadU32(base, 0x826C2D20u); }
  static void set_vector_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2D20u, v); }
  static constexpr uint32_t vector_guard_address() { return 0x826C2D20u; }
};

}  // namespace schema
