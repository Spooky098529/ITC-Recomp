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
#pragma pack(push, 1)
struct AppCameraData {
  uint32_t vtable = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
  uint32_t vtable_8 = 0;  // +8
  uint32_t vtable_12 = 0;  // +12
  uint32_t service = 0;  // +16
  uint8_t pad_20[8] = {};
  uint32_t camera = 0;  // +28
  uint32_t list = 0;  // +32
  uint8_t pad_36[12] = {};
  uint8_t unknown_48[32] = {};  // +48, guest order
  uint8_t pad_80[16] = {};
  uint8_t unknown_96[64] = {};  // +96, guest order
  uint8_t unknown_160[64] = {};  // +160, guest order
  uint8_t pad_224[16] = {};
  uint8_t unknown_240[64] = {};  // +240, guest order
  uint8_t pad_304[12] = {};
  uint32_t unknown_316 = 0;  // +316
  uint32_t unknown_320 = 0;  // +320
  uint32_t unknown_324 = 0;  // +324
  uint32_t unknown_328 = 0;  // +328
  uint32_t unknown_332 = 0;  // +332
  uint32_t unknown_336 = 0;  // +336
  uint32_t unknown_340 = 0;  // +340
  uint32_t unknown_344 = 0;  // +344
  uint8_t pad_348[20] = {};
};
#pragma pack(pop)
static_assert(sizeof(AppCameraData) == 368);
inline constexpr memory::LayoutField kAppCameraFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {28, 4}, {32, 4}, {316, 4}, {320, 4}, {324, 4}, {328, 4}, {332, 4}, {336, 4}, {340, 4}, {344, 4}};
inline constexpr memory::Layout kAppCameraLayout = {kAppCameraFields, 15};

struct AppCamera : View {
  using View::View;
  static constexpr uint32_t kSize = 368;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x820026B8;
  AppCameraData* data() const { return static_cast<AppCameraData*>(memory::HandleObject(address)); }
  static AppCamera New(uint8_t* base) {
    return AppCamera(base, memory::MakeHandle(new AppCameraData(), sizeof(AppCameraData), "AppCamera", &kAppCameraLayout));
  }
  template <uint32_t Stride = kSize>
  static AppCamera NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AppCameraData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppCameraData*>(::operator new(sizeof(AppCameraData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppCameraData();
    return AppCamera(base, memory::MakeHandle(p, sizeof(AppCameraData) * count, "AppCamera", &kAppCameraLayout, sizeof(AppCameraData)));
  }
  AppCamera at(uint32_t i) const { return AppCamera(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kAppCameraLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraLayout, offset, width, v); }
  // 0x820026B8
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // 0 (AppCamera::AppCamera)
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4() const { return data()->unknown_4; }
  void set_unknown_4(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_4_bits() const { return data()->unknown_4; }
  void set_unknown_4_bits(uint32_t v) const { data()->unknown_4 = v; }
  // 0x820026A8 (0x82002680 first)
  uint32_t vtable_8_address() const { return address + 8; }
  uint32_t vtable_8() const { return data()->vtable_8; }
  void set_vtable_8(uint32_t v) const { data()->vtable_8 = v; }
  uint32_t vtable_8_bits() const { return data()->vtable_8; }
  void set_vtable_8_bits(uint32_t v) const { data()->vtable_8 = v; }
  // 0x8200269C (0x82002690 first)
  uint32_t vtable_12_address() const { return address + 12; }
  uint32_t vtable_12() const { return data()->vtable_12; }
  void set_vtable_12(uint32_t v) const { data()->vtable_12 = v; }
  uint32_t vtable_12_bits() const { return data()->vtable_12; }
  void set_vtable_12_bits(uint32_t v) const { data()->vtable_12 = v; }
  // the block's +4 (the app's +4)
  uint32_t service_address() const { return address + 16; }
  template <typename T = AppCameraService>
  T service() const { return T(base, data()->service); }
  void set_service(const View& w) const { const uint32_t v = w.address; data()->service = v; }
  void set_service(uint32_t v) const { data()->service = v; }
  uint32_t service_bits() const { return data()->service; }
  void set_service_bits(uint32_t v) const { data()->service = v; }
  // or 0
  uint32_t camera_address() const { return address + 28; }
  template <typename T = AppCameraCamera>
  T camera() const { return T(base, data()->camera); }
  void set_camera(const View& w) const { const uint32_t v = w.address; data()->camera = v; }
  void set_camera(uint32_t v) const { data()->camera = v; }
  uint32_t camera_bits() const { return data()->camera; }
  void set_camera_bits(uint32_t v) const { data()->camera = v; }
  // or 0
  uint32_t list_address() const { return address + 32; }
  template <typename T = AppCameraList>
  T list() const { return T(base, data()->list); }
  void set_list(const View& w) const { const uint32_t v = w.address; data()->list = v; }
  void set_list(uint32_t v) const { data()->list = v; }
  uint32_t list_bits() const { return data()->list; }
  void set_list_bits(uint32_t v) const { data()->list = v; }
  // two vectors, probably (1.0 at +60 and +76)
  uint32_t unknown_48_address() const { return address + 48; }
  uint8_t* unknown_48() const { return reinterpret_cast<uint8_t*>(data()) + 48; }
  // a 4x4 matrix, probably (1.0 at its rows' fourth words)
  uint32_t unknown_96_address() const { return address + 96; }
  uint8_t* unknown_96() const { return reinterpret_cast<uint8_t*>(data()) + 96; }
  // a 4x4 matrix, probably (1.0 at its rows' fourth words)
  uint32_t unknown_160_address() const { return address + 160; }
  uint8_t* unknown_160() const { return reinterpret_cast<uint8_t*>(data()) + 160; }
  // a 4x4 matrix, probably (1.0 at its rows' fourth words)
  uint32_t unknown_240_address() const { return address + 240; }
  uint8_t* unknown_240() const { return reinterpret_cast<uint8_t*>(data()) + 240; }
  // 0 when made
  uint32_t unknown_316_address() const { return address + 316; }
  uint32_t unknown_316() const { return data()->unknown_316; }
  void set_unknown_316(uint32_t v) const { data()->unknown_316 = v; }
  uint32_t unknown_316_bits() const { return data()->unknown_316; }
  void set_unknown_316_bits(uint32_t v) const { data()->unknown_316 = v; }
  // 0 when made
  uint32_t unknown_320_address() const { return address + 320; }
  uint32_t unknown_320() const { return data()->unknown_320; }
  void set_unknown_320(uint32_t v) const { data()->unknown_320 = v; }
  uint32_t unknown_320_bits() const { return data()->unknown_320; }
  void set_unknown_320_bits(uint32_t v) const { data()->unknown_320 = v; }
  // 1 when made
  uint32_t unknown_324_address() const { return address + 324; }
  uint32_t unknown_324() const { return data()->unknown_324; }
  void set_unknown_324(uint32_t v) const { data()->unknown_324 = v; }
  uint32_t unknown_324_bits() const { return data()->unknown_324; }
  void set_unknown_324_bits(uint32_t v) const { data()->unknown_324 = v; }
  // 1 when made
  uint32_t unknown_328_address() const { return address + 328; }
  uint32_t unknown_328() const { return data()->unknown_328; }
  void set_unknown_328(uint32_t v) const { data()->unknown_328 = v; }
  uint32_t unknown_328_bits() const { return data()->unknown_328; }
  void set_unknown_328_bits(uint32_t v) const { data()->unknown_328 = v; }
  // 10 when made
  uint32_t unknown_332_address() const { return address + 332; }
  uint32_t unknown_332() const { return data()->unknown_332; }
  void set_unknown_332(uint32_t v) const { data()->unknown_332 = v; }
  uint32_t unknown_332_bits() const { return data()->unknown_332; }
  void set_unknown_332_bits(uint32_t v) const { data()->unknown_332 = v; }
  // 10 when made
  uint32_t unknown_336_address() const { return address + 336; }
  uint32_t unknown_336() const { return data()->unknown_336; }
  void set_unknown_336(uint32_t v) const { data()->unknown_336 = v; }
  uint32_t unknown_336_bits() const { return data()->unknown_336; }
  void set_unknown_336_bits(uint32_t v) const { data()->unknown_336 = v; }
  // 0 when made
  uint32_t unknown_340_address() const { return address + 340; }
  uint32_t unknown_340() const { return data()->unknown_340; }
  void set_unknown_340(uint32_t v) const { data()->unknown_340 = v; }
  uint32_t unknown_340_bits() const { return data()->unknown_340; }
  void set_unknown_340_bits(uint32_t v) const { data()->unknown_340 = v; }
  // 2 when made
  uint32_t unknown_344_address() const { return address + 344; }
  uint32_t unknown_344() const { return data()->unknown_344; }
  void set_unknown_344(uint32_t v) const { data()->unknown_344 = v; }
  uint32_t unknown_344_bits() const { return data()->unknown_344; }
  void set_unknown_344_bits(uint32_t v) const { data()->unknown_344 = v; }
};

// allocated; measure: AppCamera::Camera, AppCamera.+28
#pragma pack(push, 1)
struct AppCameraCameraData {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[12] = {};
  uint8_t view[64] = {};  // +16, guest order
  uint8_t projection[64] = {};  // +80, guest order
  uint8_t view_projection[64] = {};  // +144, guest order
  uint8_t view_built[64] = {};  // +208, guest order
  uint64_t position_xy = 0;  // +272
  uint64_t position_zw = 0;  // +280
  uint64_t angles_xy = 0;  // +288
  uint64_t angles_zw = 0;  // +296
  uint8_t unknown_304[64] = {};  // +304, guest order
  float field_of_view = 0;  // +368
  float view_scale = 0;  // +372
  float width = 0;  // +376
  float height = 0;  // +380
  float near = 0;  // +384
  float far = 0;  // +388
  float aspect = 0;  // +392
  uint8_t pad_396[4] = {};
  uint8_t unknown_400[16] = {};  // +400, guest order
  uint8_t unknown_416[16] = {};  // +416, guest order
  uint8_t unknown_432[16] = {};  // +432, guest order
  uint8_t unknown_448[16] = {};  // +448, guest order
  uint8_t unknown_464[16] = {};  // +464, guest order
  uint8_t unknown_480[16] = {};  // +480, guest order
  uint8_t unknown_496[16] = {};  // +496, guest order
  uint8_t unknown_512[16] = {};  // +512, guest order
  uint8_t pad_528[32] = {};
  uint8_t unknown_560[16] = {};  // +560, guest order
  uint8_t unknown_576[16] = {};  // +576, guest order
  uint8_t unknown_592[16] = {};  // +592, guest order
  uint8_t unknown_608[16] = {};  // +608, guest order
  uint8_t unknown_624[16] = {};  // +624, guest order
  uint8_t unknown_640[16] = {};  // +640, guest order
  uint8_t unknown_656[16] = {};  // +656, guest order
  float unknown_672 = 0;  // +672
  float unknown_676 = 0;  // +676
  float unknown_680 = 0;  // +680
  float unknown_684 = 0;  // +684
  float unknown_688 = 0;  // +688
  float unknown_692 = 0;  // +692
  uint8_t pad_696[108] = {};
  float unknown_804 = 0;  // +804
  float unknown_808 = 0;  // +808
  float unknown_812 = 0;  // +812
  float unknown_816 = 0;  // +816
  uint8_t pad_820[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(AppCameraCameraData) == 832);
inline constexpr memory::LayoutField kAppCameraCameraFields[] = {{0, 4}, {272, 8}, {280, 8}, {288, 8}, {296, 8}, {368, 4}, {372, 4}, {376, 4}, {380, 4}, {384, 4}, {388, 4}, {392, 4}, {672, 4}, {676, 4}, {680, 4}, {684, 4}, {688, 4}, {692, 4}, {804, 4}, {808, 4}, {812, 4}, {816, 4}};
inline constexpr memory::Layout kAppCameraCameraLayout = {kAppCameraCameraFields, 22};

struct AppCameraCamera : View {
  using View::View;
  static constexpr uint32_t kSize = 832;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x820027B0;
  AppCameraCameraData* data() const { return static_cast<AppCameraCameraData*>(memory::HandleObject(address)); }
  static AppCameraCamera New(uint8_t* base) {
    return AppCameraCamera(base, memory::MakeHandle(new AppCameraCameraData(), sizeof(AppCameraCameraData), "AppCameraCamera", &kAppCameraCameraLayout));
  }
  template <uint32_t Stride = kSize>
  static AppCameraCamera NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AppCameraCameraData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppCameraCameraData*>(::operator new(sizeof(AppCameraCameraData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppCameraCameraData();
    return AppCameraCamera(base, memory::MakeHandle(p, sizeof(AppCameraCameraData) * count, "AppCameraCamera", &kAppCameraCameraLayout, sizeof(AppCameraCameraData)));
  }
  AppCameraCamera at(uint32_t i) const { return AppCameraCamera(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kAppCameraCameraLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, offset, width, v); }
  // 0x82001EC0 (Camera); 0x820027B0 for AppCameraCamera
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = CameraVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // the view matrix (4x4 floats, row-major, row vectors)
  uint32_t view_address() const { return address + 16; }
  uint8_t* view() const { return reinterpret_cast<uint8_t*>(data()) + 16; }
  // the projection (Camera::BuildProjection)
  uint32_t projection_address() const { return address + 80; }
  uint8_t* projection() const { return reinterpret_cast<uint8_t*>(data()) + 80; }
  // view x projection (Matrix::Multiply)
  uint32_t view_projection_address() const { return address + 144; }
  uint8_t* view_projection() const { return reinterpret_cast<uint8_t*>(data()) + 144; }
  // Camera::Update's view as built, then copied to +16
  uint32_t view_built_address() const { return address + 208; }
  uint8_t* view_built() const { return reinterpret_cast<uint8_t*>(data()) + 208; }
  // the position Camera::Update builds the view from (w 1.0); x and y as one doubleword (the originals' ld/std copies)
  uint32_t position_xy_address() const { return address + 272; }
  uint64_t position_xy() const { return data()->position_xy; }
  void set_position_xy(uint64_t v) const { data()->position_xy = v; }
  uint64_t position_xy_bits() const { return data()->position_xy; }
  void set_position_xy_bits(uint64_t v) const { data()->position_xy = v; }
  // the position Camera::Update builds the view from (w 1.0)
  uint32_t position_x_address() const { return address + 272; }
  uint32_t position_x_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 272, 4)); }
  void set_position_x_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 272, 4, v); }
  float position_x() const { return std::bit_cast<float>(position_x_bits()); }
  void set_position_x(float v) const { set_position_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_y_address() const { return address + 276; }
  uint32_t position_y_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 276, 4)); }
  void set_position_y_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 276, 4, v); }
  float position_y() const { return std::bit_cast<float>(position_y_bits()); }
  void set_position_y(float v) const { set_position_y_bits(std::bit_cast<uint32_t>(v)); }
  // z and w as one doubleword
  uint32_t position_zw_address() const { return address + 280; }
  uint64_t position_zw() const { return data()->position_zw; }
  void set_position_zw(uint64_t v) const { data()->position_zw = v; }
  uint64_t position_zw_bits() const { return data()->position_zw; }
  void set_position_zw_bits(uint64_t v) const { data()->position_zw = v; }
  uint32_t position_z_address() const { return address + 280; }
  uint32_t position_z_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 280, 4)); }
  void set_position_z_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 280, 4, v); }
  float position_z() const { return std::bit_cast<float>(position_z_bits()); }
  void set_position_z(float v) const { set_position_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_w_address() const { return address + 284; }
  uint32_t position_w_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 284, 4)); }
  void set_position_w_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 284, 4, v); }
  float position_w() const { return std::bit_cast<float>(position_w_bits()); }
  void set_position_w(float v) const { set_position_w_bits(std::bit_cast<uint32_t>(v)); }
  // the Euler angles: x turns by RotateX, y by RotateY, z by RotateZ (negated); x and y as one doubleword (the originals' ld/std copies)
  uint32_t angles_xy_address() const { return address + 288; }
  uint64_t angles_xy() const { return data()->angles_xy; }
  void set_angles_xy(uint64_t v) const { data()->angles_xy = v; }
  uint64_t angles_xy_bits() const { return data()->angles_xy; }
  void set_angles_xy_bits(uint64_t v) const { data()->angles_xy = v; }
  // the Euler angles: x turns by RotateX, y by RotateY, z by RotateZ (negated)
  uint32_t angles_x_address() const { return address + 288; }
  uint32_t angles_x_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 288, 4)); }
  void set_angles_x_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 288, 4, v); }
  float angles_x() const { return std::bit_cast<float>(angles_x_bits()); }
  void set_angles_x(float v) const { set_angles_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t angles_y_address() const { return address + 292; }
  uint32_t angles_y_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 292, 4)); }
  void set_angles_y_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 292, 4, v); }
  float angles_y() const { return std::bit_cast<float>(angles_y_bits()); }
  void set_angles_y(float v) const { set_angles_y_bits(std::bit_cast<uint32_t>(v)); }
  // z and w as one doubleword
  uint32_t angles_zw_address() const { return address + 296; }
  uint64_t angles_zw() const { return data()->angles_zw; }
  void set_angles_zw(uint64_t v) const { data()->angles_zw = v; }
  uint64_t angles_zw_bits() const { return data()->angles_zw; }
  void set_angles_zw_bits(uint64_t v) const { data()->angles_zw = v; }
  uint32_t angles_z_address() const { return address + 296; }
  uint32_t angles_z_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 296, 4)); }
  void set_angles_z_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 296, 4, v); }
  float angles_z() const { return std::bit_cast<float>(angles_z_bits()); }
  void set_angles_z(float v) const { set_angles_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t angles_w_address() const { return address + 300; }
  uint32_t angles_w_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 300, 4)); }
  void set_angles_w_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraCameraLayout, 300, 4, v); }
  float angles_w() const { return std::bit_cast<float>(angles_w_bits()); }
  void set_angles_w(float v) const { set_angles_w_bits(std::bit_cast<uint32_t>(v)); }
  // a 4x4 matrix, probably (Camera::Camera sets its rows' fourth words to 1.0)
  uint32_t unknown_304_address() const { return address + 304; }
  uint8_t* unknown_304() const { return reinterpret_cast<uint8_t*>(data()) + 304; }
  // radians, wrapped into [-pi, pi]
  uint32_t field_of_view_address() const { return address + 368; }
  float field_of_view() const { return data()->field_of_view; }
  void set_field_of_view(float v) const { data()->field_of_view = v; }
  uint32_t field_of_view_bits() const { return std::bit_cast<uint32_t>(data()->field_of_view); }
  void set_field_of_view_bits(uint32_t v) const { data()->field_of_view = std::bit_cast<float>(v); }
  // Camera::Update: +380 x 0.5 / the half field of view's cosine polynomial x its sine polynomial
  uint32_t view_scale_address() const { return address + 372; }
  float view_scale() const { return data()->view_scale; }
  void set_view_scale(float v) const { data()->view_scale = v; }
  uint32_t view_scale_bits() const { return std::bit_cast<uint32_t>(data()->view_scale); }
  void set_view_scale_bits(uint32_t v) const { data()->view_scale = std::bit_cast<float>(v); }
  // the viewport's width
  uint32_t width_address() const { return address + 376; }
  float width() const { return data()->width; }
  void set_width(float v) const { data()->width = v; }
  uint32_t width_bits() const { return std::bit_cast<uint32_t>(data()->width); }
  void set_width_bits(uint32_t v) const { data()->width = std::bit_cast<float>(v); }
  // the viewport's height
  uint32_t height_address() const { return address + 380; }
  float height() const { return data()->height; }
  void set_height(float v) const { data()->height = v; }
  uint32_t height_bits() const { return std::bit_cast<uint32_t>(data()->height); }
  void set_height_bits(uint32_t v) const { data()->height = std::bit_cast<float>(v); }
  // the near plane
  uint32_t near_address() const { return address + 384; }
  float near() const { return data()->near; }
  void set_near(float v) const { data()->near = v; }
  uint32_t near_bits() const { return std::bit_cast<uint32_t>(data()->near); }
  void set_near_bits(uint32_t v) const { data()->near = std::bit_cast<float>(v); }
  // the far plane
  uint32_t far_address() const { return address + 388; }
  float far() const { return data()->far; }
  void set_far(float v) const { data()->far = v; }
  uint32_t far_bits() const { return std::bit_cast<uint32_t>(data()->far); }
  void set_far_bits(uint32_t v) const { data()->far = std::bit_cast<float>(v); }
  // width / height
  uint32_t aspect_address() const { return address + 392; }
  float aspect() const { return data()->aspect; }
  void set_aspect(float v) const { data()->aspect = v; }
  uint32_t aspect_bits() const { return std::bit_cast<uint32_t>(data()->aspect); }
  void set_aspect_bits(uint32_t v) const { data()->aspect = std::bit_cast<float>(v); }
  // (0, 0, 0, 1) when made
  uint32_t unknown_400_address() const { return address + 400; }
  uint8_t* unknown_400() const { return reinterpret_cast<uint8_t*>(data()) + 400; }
  // (0, 0, 10, 1) when made
  uint32_t unknown_416_address() const { return address + 416; }
  uint8_t* unknown_416() const { return reinterpret_cast<uint8_t*>(data()) + 416; }
  // the position (+272) when made
  uint32_t unknown_432_address() const { return address + 432; }
  uint8_t* unknown_432() const { return reinterpret_cast<uint8_t*>(data()) + 432; }
  // +416 when made
  uint32_t unknown_448_address() const { return address + 448; }
  uint8_t* unknown_448() const { return reinterpret_cast<uint8_t*>(data()) + 448; }
  // +416 when made
  uint32_t unknown_464_address() const { return address + 464; }
  uint8_t* unknown_464() const { return reinterpret_cast<uint8_t*>(data()) + 464; }
  // 1.0 at +492 when made
  uint32_t unknown_480_address() const { return address + 480; }
  uint8_t* unknown_480() const { return reinterpret_cast<uint8_t*>(data()) + 480; }
  // 1.0 at +508 when made
  uint32_t unknown_496_address() const { return address + 496; }
  uint8_t* unknown_496() const { return reinterpret_cast<uint8_t*>(data()) + 496; }
  // 1.0 at +524 when made
  uint32_t unknown_512_address() const { return address + 512; }
  uint8_t* unknown_512() const { return reinterpret_cast<uint8_t*>(data()) + 512; }
  // 1.0 at +572 when made
  uint32_t unknown_560_address() const { return address + 560; }
  uint8_t* unknown_560() const { return reinterpret_cast<uint8_t*>(data()) + 560; }
  // 1.0 at +588 when made
  uint32_t unknown_576_address() const { return address + 576; }
  uint8_t* unknown_576() const { return reinterpret_cast<uint8_t*>(data()) + 576; }
  // 1.0 at +604 when made
  uint32_t unknown_592_address() const { return address + 592; }
  uint8_t* unknown_592() const { return reinterpret_cast<uint8_t*>(data()) + 592; }
  // 1.0 at +620 when made
  uint32_t unknown_608_address() const { return address + 608; }
  uint8_t* unknown_608() const { return reinterpret_cast<uint8_t*>(data()) + 608; }
  // (0, 0, 0, 1) when made
  uint32_t unknown_624_address() const { return address + 624; }
  uint8_t* unknown_624() const { return reinterpret_cast<uint8_t*>(data()) + 624; }
  // (0, 0, 0, 1) when made
  uint32_t unknown_640_address() const { return address + 640; }
  uint8_t* unknown_640() const { return reinterpret_cast<uint8_t*>(data()) + 640; }
  // (0, 0, 0, 1) when made
  uint32_t unknown_656_address() const { return address + 656; }
  uint8_t* unknown_656() const { return reinterpret_cast<uint8_t*>(data()) + 656; }
  // 45.0 when made
  uint32_t unknown_672_address() const { return address + 672; }
  float unknown_672() const { return data()->unknown_672; }
  void set_unknown_672(float v) const { data()->unknown_672 = v; }
  uint32_t unknown_672_bits() const { return std::bit_cast<uint32_t>(data()->unknown_672); }
  void set_unknown_672_bits(uint32_t v) const { data()->unknown_672 = std::bit_cast<float>(v); }
  // 0.09 when made
  uint32_t unknown_676_address() const { return address + 676; }
  float unknown_676() const { return data()->unknown_676; }
  void set_unknown_676(float v) const { data()->unknown_676 = v; }
  uint32_t unknown_676_bits() const { return std::bit_cast<uint32_t>(data()->unknown_676); }
  void set_unknown_676_bits(uint32_t v) const { data()->unknown_676 = std::bit_cast<float>(v); }
  // 1.5 when made
  uint32_t unknown_680_address() const { return address + 680; }
  float unknown_680() const { return data()->unknown_680; }
  void set_unknown_680(float v) const { data()->unknown_680 = v; }
  uint32_t unknown_680_bits() const { return std::bit_cast<uint32_t>(data()->unknown_680); }
  void set_unknown_680_bits(uint32_t v) const { data()->unknown_680 = std::bit_cast<float>(v); }
  // 0.0 when made
  uint32_t unknown_684_address() const { return address + 684; }
  float unknown_684() const { return data()->unknown_684; }
  void set_unknown_684(float v) const { data()->unknown_684 = v; }
  uint32_t unknown_684_bits() const { return std::bit_cast<uint32_t>(data()->unknown_684); }
  void set_unknown_684_bits(uint32_t v) const { data()->unknown_684 = std::bit_cast<float>(v); }
  // 30.0 when made
  uint32_t unknown_688_address() const { return address + 688; }
  float unknown_688() const { return data()->unknown_688; }
  void set_unknown_688(float v) const { data()->unknown_688 = v; }
  uint32_t unknown_688_bits() const { return std::bit_cast<uint32_t>(data()->unknown_688); }
  void set_unknown_688_bits(uint32_t v) const { data()->unknown_688 = std::bit_cast<float>(v); }
  // 200.0 when made
  uint32_t unknown_692_address() const { return address + 692; }
  float unknown_692() const { return data()->unknown_692; }
  void set_unknown_692(float v) const { data()->unknown_692 = v; }
  uint32_t unknown_692_bits() const { return std::bit_cast<uint32_t>(data()->unknown_692); }
  void set_unknown_692_bits(uint32_t v) const { data()->unknown_692 = std::bit_cast<float>(v); }
  // -89.0 when made
  uint32_t unknown_804_address() const { return address + 804; }
  float unknown_804() const { return data()->unknown_804; }
  void set_unknown_804(float v) const { data()->unknown_804 = v; }
  uint32_t unknown_804_bits() const { return std::bit_cast<uint32_t>(data()->unknown_804); }
  void set_unknown_804_bits(uint32_t v) const { data()->unknown_804 = std::bit_cast<float>(v); }
  // 89.0 when made
  uint32_t unknown_808_address() const { return address + 808; }
  float unknown_808() const { return data()->unknown_808; }
  void set_unknown_808(float v) const { data()->unknown_808 = v; }
  uint32_t unknown_808_bits() const { return std::bit_cast<uint32_t>(data()->unknown_808); }
  void set_unknown_808_bits(uint32_t v) const { data()->unknown_808 = std::bit_cast<float>(v); }
  // -45.0 when made
  uint32_t unknown_812_address() const { return address + 812; }
  float unknown_812() const { return data()->unknown_812; }
  void set_unknown_812(float v) const { data()->unknown_812 = v; }
  uint32_t unknown_812_bits() const { return std::bit_cast<uint32_t>(data()->unknown_812); }
  void set_unknown_812_bits(uint32_t v) const { data()->unknown_812 = std::bit_cast<float>(v); }
  // 45.0 when made
  uint32_t unknown_816_address() const { return address + 816; }
  float unknown_816() const { return data()->unknown_816; }
  void set_unknown_816(float v) const { data()->unknown_816 = v; }
  uint32_t unknown_816_bits() const { return std::bit_cast<uint32_t>(data()->unknown_816); }
  void set_unknown_816_bits(uint32_t v) const { data()->unknown_816 = std::bit_cast<float>(v); }
};

// allocated; measure: AppCameraList, AppCamera.+32, sub_820A9200.r3
#pragma pack(push, 1)
struct AppCameraListData {
  uint32_t vtable = 0;  // +0
  uint32_t vector = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(AppCameraListData) == 8);
inline constexpr memory::LayoutField kAppCameraListFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kAppCameraListLayout = {kAppCameraListFields, 2};

struct AppCameraList : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82002780;
  AppCameraListData* data() const { return static_cast<AppCameraListData*>(memory::HandleObject(address)); }
  static AppCameraList New(uint8_t* base) {
    return AppCameraList(base, memory::MakeHandle(new AppCameraListData(), sizeof(AppCameraListData), "AppCameraList", &kAppCameraListLayout));
  }
  template <uint32_t Stride = kSize>
  static AppCameraList NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AppCameraListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppCameraListData*>(::operator new(sizeof(AppCameraListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppCameraListData();
    return AppCameraList(base, memory::MakeHandle(p, sizeof(AppCameraListData) * count, "AppCameraList", &kAppCameraListLayout, sizeof(AppCameraListData)));
  }
  AppCameraList at(uint32_t i) const { return AppCameraList(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kAppCameraListLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraListLayout, offset, width, v); }
  // 0x82002780
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = AppCameraListVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // a std::vector of words (16 bytes) or 0
  uint32_t vector_address() const { return address + 4; }
  template <typename T = NewAny_MessageVector_Allocate>
  T vector() const { return T(base, data()->vector); }
  void set_vector(const View& w) const { const uint32_t v = w.address; data()->vector = v; }
  void set_vector(uint32_t v) const { data()->vector = v; }
  uint32_t vector_bits() const { return data()->vector; }
  void set_vector_bits(uint32_t v) const { data()->vector = v; }
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
#pragma pack(push, 1)
struct AppCameraEntryData {
  uint32_t vtable = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
  uint32_t unknown_12 = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(AppCameraEntryData) == 16);
inline constexpr memory::LayoutField kAppCameraEntryFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kAppCameraEntryLayout = {kAppCameraEntryFields, 4};

struct AppCameraEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82002580;
  AppCameraEntryData* data() const { return static_cast<AppCameraEntryData*>(memory::HandleObject(address)); }
  static AppCameraEntry New(uint8_t* base) {
    return AppCameraEntry(base, memory::MakeHandle(new AppCameraEntryData(), sizeof(AppCameraEntryData), "AppCameraEntry", &kAppCameraEntryLayout));
  }
  template <uint32_t Stride = kSize>
  static AppCameraEntry NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AppCameraEntryData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppCameraEntryData*>(::operator new(sizeof(AppCameraEntryData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppCameraEntryData();
    return AppCameraEntry(base, memory::MakeHandle(p, sizeof(AppCameraEntryData) * count, "AppCameraEntry", &kAppCameraEntryLayout, sizeof(AppCameraEntryData)));
  }
  AppCameraEntry at(uint32_t i) const { return AppCameraEntry(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kAppCameraEntryLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppCameraEntryLayout, offset, width, v); }
  // 0x82002580
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // 0 when made
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4() const { return data()->unknown_4; }
  void set_unknown_4(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_4_bits() const { return data()->unknown_4; }
  void set_unknown_4_bits(uint32_t v) const { data()->unknown_4 = v; }
  // 0 when made
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8() const { return data()->unknown_8; }
  void set_unknown_8(uint32_t v) const { data()->unknown_8 = v; }
  uint32_t unknown_8_bits() const { return data()->unknown_8; }
  void set_unknown_8_bits(uint32_t v) const { data()->unknown_8 = v; }
  // 0 when made
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_12() const { return data()->unknown_12; }
  void set_unknown_12(uint32_t v) const { data()->unknown_12 = v; }
  uint32_t unknown_12_bits() const { return data()->unknown_12; }
  void set_unknown_12_bits(uint32_t v) const { data()->unknown_12 = v; }
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

// embedded; measure: Camera@+368
struct CameraPassAt368 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: Camera@+384
struct CameraPassAt384 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: Camera@+388
struct CameraPassAt388 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: Camera@+392
struct CameraPassAt392 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: CourseView@+592
struct CourseViewAt592 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint64_t unknown_0() const { return guest::LoadU64(base, address + 0); }
  void set_unknown_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint64_t unknown_8() const { return guest::LoadU64(base, address + 8); }
  void set_unknown_8(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: CourseView@+608
struct CourseViewAt608 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint64_t unknown_0() const { return guest::LoadU64(base, address + 0); }
  void set_unknown_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint64_t unknown_8() const { return guest::LoadU64(base, address + 8); }
  void set_unknown_8(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
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
