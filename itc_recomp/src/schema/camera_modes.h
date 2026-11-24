// The views of docs/functions/camera-modes.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/camera-modes.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: Viewport, Cutscene.+20, TvCamera.+20, ViewCamera1.+20, ViewCamera2.+20, IntroCamera.+0, new<12>@Viewports::Init.+4
struct Viewport : View {
  using View::View;
  static constexpr uint32_t kSize = 1872;
  // twenty 80-byte entries (ViewportEntry)
  uint32_t entries_address() const { return address + 0; }
  // the eye (Viewport::Set, LookAt)
  uint32_t eye_address() const { return address + 1600; }
  float eye_x() const { return guest::LoadFloat(base, address + 1600); }
  void set_eye_x(float v) const { guest::StoreFloat(base, address + 1600, v); }
  uint32_t eye_x_address() const { return address + 1600; }
  float eye_y() const { return guest::LoadFloat(base, address + 1604); }
  void set_eye_y(float v) const { guest::StoreFloat(base, address + 1604, v); }
  uint32_t eye_y_address() const { return address + 1604; }
  float eye_z() const { return guest::LoadFloat(base, address + 1608); }
  void set_eye_z(float v) const { guest::StoreFloat(base, address + 1608, v); }
  uint32_t eye_z_address() const { return address + 1608; }
  float eye_w() const { return guest::LoadFloat(base, address + 1612); }
  void set_eye_w(float v) const { guest::StoreFloat(base, address + 1612, v); }
  uint32_t eye_w_address() const { return address + 1612; }
  // the look-at target
  uint32_t target_address() const { return address + 1616; }
  float target_x() const { return guest::LoadFloat(base, address + 1616); }
  void set_target_x(float v) const { guest::StoreFloat(base, address + 1616, v); }
  uint32_t target_x_address() const { return address + 1616; }
  float target_y() const { return guest::LoadFloat(base, address + 1620); }
  void set_target_y(float v) const { guest::StoreFloat(base, address + 1620, v); }
  uint32_t target_y_address() const { return address + 1620; }
  float target_z() const { return guest::LoadFloat(base, address + 1624); }
  void set_target_z(float v) const { guest::StoreFloat(base, address + 1624, v); }
  uint32_t target_z_address() const { return address + 1624; }
  float target_w() const { return guest::LoadFloat(base, address + 1628); }
  void set_target_w(float v) const { guest::StoreFloat(base, address + 1628, v); }
  uint32_t target_w_address() const { return address + 1628; }
  // the up vector
  uint32_t up_address() const { return address + 1632; }
  float up_x() const { return guest::LoadFloat(base, address + 1632); }
  void set_up_x(float v) const { guest::StoreFloat(base, address + 1632, v); }
  uint32_t up_x_address() const { return address + 1632; }
  float up_y() const { return guest::LoadFloat(base, address + 1636); }
  void set_up_y(float v) const { guest::StoreFloat(base, address + 1636, v); }
  uint32_t up_y_address() const { return address + 1636; }
  float up_z() const { return guest::LoadFloat(base, address + 1640); }
  void set_up_z(float v) const { guest::StoreFloat(base, address + 1640, v); }
  uint32_t up_z_address() const { return address + 1640; }
  float up_w() const { return guest::LoadFloat(base, address + 1644); }
  void set_up_w(float v) const { guest::StoreFloat(base, address + 1644, v); }
  uint32_t up_w_address() const { return address + 1644; }
  // a fourth vector: the look point
  uint32_t look_address() const { return address + 1648; }
  float look_x() const { return guest::LoadFloat(base, address + 1648); }
  void set_look_x(float v) const { guest::StoreFloat(base, address + 1648, v); }
  uint32_t look_x_address() const { return address + 1648; }
  float look_y() const { return guest::LoadFloat(base, address + 1652); }
  void set_look_y(float v) const { guest::StoreFloat(base, address + 1652, v); }
  uint32_t look_y_address() const { return address + 1652; }
  float look_z() const { return guest::LoadFloat(base, address + 1656); }
  void set_look_z(float v) const { guest::StoreFloat(base, address + 1656, v); }
  uint32_t look_z_address() const { return address + 1656; }
  float look_w() const { return guest::LoadFloat(base, address + 1660); }
  void set_look_w(float v) const { guest::StoreFloat(base, address + 1660, v); }
  uint32_t look_w_address() const { return address + 1660; }
  // the target view mode 1 uses; the cameras' look offset
  uint32_t view1_target_address() const { return address + 1664; }
  float view1_target_x() const { return guest::LoadFloat(base, address + 1664); }
  void set_view1_target_x(float v) const { guest::StoreFloat(base, address + 1664, v); }
  uint32_t view1_target_x_address() const { return address + 1664; }
  float view1_target_y() const { return guest::LoadFloat(base, address + 1668); }
  void set_view1_target_y(float v) const { guest::StoreFloat(base, address + 1668, v); }
  uint32_t view1_target_y_address() const { return address + 1668; }
  float view1_target_z() const { return guest::LoadFloat(base, address + 1672); }
  void set_view1_target_z(float v) const { guest::StoreFloat(base, address + 1672, v); }
  uint32_t view1_target_z_address() const { return address + 1672; }
  float view1_target_w() const { return guest::LoadFloat(base, address + 1676); }
  void set_view1_target_w(float v) const { guest::StoreFloat(base, address + 1676, v); }
  uint32_t view1_target_w_address() const { return address + 1676; }
  // the camera's eye offset (ChaseCamera::Place, MakeShot)
  uint32_t base_eye_address() const { return address + 1680; }
  float base_eye_x() const { return guest::LoadFloat(base, address + 1680); }
  void set_base_eye_x(float v) const { guest::StoreFloat(base, address + 1680, v); }
  uint32_t base_eye_x_address() const { return address + 1680; }
  float base_eye_y() const { return guest::LoadFloat(base, address + 1684); }
  void set_base_eye_y(float v) const { guest::StoreFloat(base, address + 1684, v); }
  uint32_t base_eye_y_address() const { return address + 1684; }
  float base_eye_z() const { return guest::LoadFloat(base, address + 1688); }
  void set_base_eye_z(float v) const { guest::StoreFloat(base, address + 1688, v); }
  uint32_t base_eye_z_address() const { return address + 1688; }
  float base_eye_w() const { return guest::LoadFloat(base, address + 1692); }
  void set_base_eye_w(float v) const { guest::StoreFloat(base, address + 1692, v); }
  uint32_t base_eye_w_address() const { return address + 1692; }
  // the camera's target offset
  uint32_t base_target_address() const { return address + 1696; }
  float base_target_x() const { return guest::LoadFloat(base, address + 1696); }
  void set_base_target_x(float v) const { guest::StoreFloat(base, address + 1696, v); }
  uint32_t base_target_x_address() const { return address + 1696; }
  float base_target_y() const { return guest::LoadFloat(base, address + 1700); }
  void set_base_target_y(float v) const { guest::StoreFloat(base, address + 1700, v); }
  uint32_t base_target_y_address() const { return address + 1700; }
  float base_target_z() const { return guest::LoadFloat(base, address + 1704); }
  void set_base_target_z(float v) const { guest::StoreFloat(base, address + 1704, v); }
  uint32_t base_target_z_address() const { return address + 1704; }
  float base_target_w() const { return guest::LoadFloat(base, address + 1708); }
  void set_base_target_w(float v) const { guest::StoreFloat(base, address + 1708, v); }
  uint32_t base_target_w_address() const { return address + 1708; }
  // the camera's up vector, (0, 1, 0, 1)
  uint32_t base_up_address() const { return address + 1712; }
  float base_up_x() const { return guest::LoadFloat(base, address + 1712); }
  void set_base_up_x(float v) const { guest::StoreFloat(base, address + 1712, v); }
  uint32_t base_up_x_address() const { return address + 1712; }
  float base_up_y() const { return guest::LoadFloat(base, address + 1716); }
  void set_base_up_y(float v) const { guest::StoreFloat(base, address + 1716, v); }
  uint32_t base_up_y_address() const { return address + 1716; }
  float base_up_z() const { return guest::LoadFloat(base, address + 1720); }
  void set_base_up_z(float v) const { guest::StoreFloat(base, address + 1720, v); }
  uint32_t base_up_z_address() const { return address + 1720; }
  float base_up_w() const { return guest::LoadFloat(base, address + 1724); }
  void set_base_up_w(float v) const { guest::StoreFloat(base, address + 1724, v); }
  uint32_t base_up_w_address() const { return address + 1724; }
  // a working vector
  uint32_t unknown_1728_address() const { return address + 1728; }
  // the camera's look offset (ChaseCamera::Place)
  uint32_t base_look_address() const { return address + 1744; }
  float base_look_x() const { return guest::LoadFloat(base, address + 1744); }
  void set_base_look_x(float v) const { guest::StoreFloat(base, address + 1744, v); }
  uint32_t base_look_x_address() const { return address + 1744; }
  float base_look_y() const { return guest::LoadFloat(base, address + 1748); }
  void set_base_look_y(float v) const { guest::StoreFloat(base, address + 1748, v); }
  uint32_t base_look_y_address() const { return address + 1748; }
  float base_look_z() const { return guest::LoadFloat(base, address + 1752); }
  void set_base_look_z(float v) const { guest::StoreFloat(base, address + 1752, v); }
  uint32_t base_look_z_address() const { return address + 1752; }
  float base_look_w() const { return guest::LoadFloat(base, address + 1756); }
  void set_base_look_w(float v) const { guest::StoreFloat(base, address + 1756, v); }
  uint32_t base_look_w_address() const { return address + 1756; }
  // entry 0's last eye, target and up (Viewport::UpdateEntries)
  uint32_t last_view_address() const { return address + 1760; }
  // working vectors
  uint32_t unknown_1808_address() const { return address + 1808; }
  // the TV camera's eye at the next spot (TvCamera::Step)
  uint32_t next_eye_address() const { return address + 1840; }
  float next_eye_x() const { return guest::LoadFloat(base, address + 1840); }
  void set_next_eye_x(float v) const { guest::StoreFloat(base, address + 1840, v); }
  uint32_t next_eye_x_address() const { return address + 1840; }
  float next_eye_y() const { return guest::LoadFloat(base, address + 1844); }
  void set_next_eye_y(float v) const { guest::StoreFloat(base, address + 1844, v); }
  uint32_t next_eye_y_address() const { return address + 1844; }
  float next_eye_z() const { return guest::LoadFloat(base, address + 1848); }
  void set_next_eye_z(float v) const { guest::StoreFloat(base, address + 1848, v); }
  uint32_t next_eye_z_address() const { return address + 1848; }
  float next_eye_w() const { return guest::LoadFloat(base, address + 1852); }
  void set_next_eye_w(float v) const { guest::StoreFloat(base, address + 1852, v); }
  uint32_t next_eye_w_address() const { return address + 1852; }
  // 8,000.0: entry 18's far plane
  float far_18() const { return guest::LoadFloat(base, address + 1856); }
  void set_far_18(float v) const { guest::StoreFloat(base, address + 1856, v); }
  uint32_t far_18_address() const { return address + 1856; }
  // the view (0-5; 1 and 2 enable pass 3, the car overlays read it)
  uint32_t view_mode() const { return guest::LoadU32(base, address + 1860); }
  void set_view_mode(uint32_t v) const { guest::StoreU32(base, address + 1860, v); }
  uint32_t view_mode_address() const { return address + 1860; }
  // entry 0's zoom at set-up (Viewport::Init)
  float zoom() const { return guest::LoadFloat(base, address + 1864); }
  void set_zoom(float v) const { guest::StoreFloat(base, address + 1864, v); }
  uint32_t zoom_address() const { return address + 1864; }
  // pass 0's near plane at set-up
  float near() const { return guest::LoadFloat(base, address + 1868); }
  void set_near(float v) const { guest::StoreFloat(base, address + 1868, v); }
  uint32_t near_address() const { return address + 1868; }
};

// embedded; measure: -
struct ViewportEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  // 0 at set-up
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the eye
  uint32_t eye_address() const { return address + 16; }
  float eye_x() const { return guest::LoadFloat(base, address + 16); }
  void set_eye_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t eye_x_address() const { return address + 16; }
  float eye_y() const { return guest::LoadFloat(base, address + 20); }
  void set_eye_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t eye_y_address() const { return address + 20; }
  float eye_z() const { return guest::LoadFloat(base, address + 24); }
  void set_eye_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t eye_z_address() const { return address + 24; }
  float eye_w() const { return guest::LoadFloat(base, address + 28); }
  void set_eye_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t eye_w_address() const { return address + 28; }
  // the target
  uint32_t target_address() const { return address + 32; }
  float target_x() const { return guest::LoadFloat(base, address + 32); }
  void set_target_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t target_x_address() const { return address + 32; }
  float target_y() const { return guest::LoadFloat(base, address + 36); }
  void set_target_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t target_y_address() const { return address + 36; }
  float target_z() const { return guest::LoadFloat(base, address + 40); }
  void set_target_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t target_z_address() const { return address + 40; }
  float target_w() const { return guest::LoadFloat(base, address + 44); }
  void set_target_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t target_w_address() const { return address + 44; }
  // the up vector (CameraPass::FromViewport)
  uint32_t up_address() const { return address + 48; }
  float up_x() const { return guest::LoadFloat(base, address + 48); }
  void set_up_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t up_x_address() const { return address + 48; }
  float up_y() const { return guest::LoadFloat(base, address + 52); }
  void set_up_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t up_y_address() const { return address + 52; }
  float up_z() const { return guest::LoadFloat(base, address + 56); }
  void set_up_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t up_z_address() const { return address + 56; }
  float up_w() const { return guest::LoadFloat(base, address + 60); }
  void set_up_w(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t up_w_address() const { return address + 60; }
  // Viewport::SetRoll
  float roll() const { return guest::LoadFloat(base, address + 64); }
  void set_roll(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t roll_address() const { return address + 64; }
  // Viewport::SetZoom: probably a focal length in pixels
  float zoom() const { return guest::LoadFloat(base, address + 68); }
  void set_zoom(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t zoom_address() const { return address + 68; }
  // the far plane
  float far() const { return guest::LoadFloat(base, address + 72); }
  void set_far(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t far_address() const { return address + 72; }
};

// allocated; measure: new<12>@Viewports::Init
struct Viewports : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // player 1's viewport
  template <typename T = Viewport>
  T first() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  // player 2's viewport
  template <typename T = Viewport>
  T second() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_second(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_second(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t second_address() const { return address + 4; }
  // not 0 in split screen
  uint32_t split() const { return guest::LoadU32(base, address + 8); }
  void set_split(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t split_address() const { return address + 8; }
};

// embedded; measure: CameraDirector
struct CameraDirector : View {
  using View::View;
  static constexpr uint32_t kSize = 77412;
  // its StateMachine (game-modes.toml; CameraDirector::InitStates)
  uint32_t machine_address() const { return address + 16; }
  // 1 at set-up, 3 by SetState3
  uint32_t state() const { return guest::LoadU32(base, address + 9620); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t state_address() const { return address + 9620; }
  // the view picked at the race's start (from the save data's camera setting)
  uint32_t start_view() const { return guest::LoadU32(base, address + 9624); }
  void set_start_view(uint32_t v) const { guest::StoreU32(base, address + 9624, v); }
  uint32_t start_view_address() const { return address + 9624; }
  // the view shown (0-5), also each viewport's +1860
  uint32_t view() const { return guest::LoadU32(base, address + 9628); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 9628, v); }
  uint32_t view_address() const { return address + 9628; }
  // the view to go back to after views 3-5
  uint32_t view_kept() const { return guest::LoadU32(base, address + 9632); }
  void set_view_kept(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t view_kept_address() const { return address + 9632; }
  // the player the views follow
  uint32_t followed() const { return guest::LoadU32(base, address + 9636); }
  void set_followed(uint32_t v) const { guest::StoreU32(base, address + 9636, v); }
  uint32_t followed_address() const { return address + 9636; }
  // view 2's camera (ViewCamera2)
  uint32_t view2_camera_address() const { return address + 9648; }
  // player 2's view 2 camera in split screen (ViewCamera2)
  uint32_t view2_camera2_address() const { return address + 10752; }
  // view 1's camera (ViewCamera1)
  uint32_t view1_camera_address() const { return address + 11856; }
  // view 1's camera's look angle (its +1104, from the camera stick)
  float view1_look_angle() const { return guest::LoadFloat(base, address + 12960); }
  void set_view1_look_angle(float v) const { guest::StoreFloat(base, address + 12960, v); }
  uint32_t view1_look_angle_address() const { return address + 12960; }
  // view 0's camera (ChaseCamera)
  uint32_t view0_camera_address() const { return address + 12976; }
  // view 0's camera's angle index (its +372): 2 at the start
  uint32_t view0_angle() const { return guest::LoadU32(base, address + 13348); }
  void set_view0_angle(uint32_t v) const { guest::StoreU32(base, address + 13348, v); }
  uint32_t view0_angle_address() const { return address + 13348; }
  // view 0's camera's +380: set by the camera button
  uint32_t view0_state() const { return guest::LoadU32(base, address + 13356); }
  void set_view0_state(uint32_t v) const { guest::StoreU32(base, address + 13356, v); }
  uint32_t view0_state_address() const { return address + 13356; }
  // view 4's camera (TvCamera)
  uint32_t tv_camera_address() const { return address + 13360; }
  // view 5's camera (Cutscene)
  uint32_t cutscene_address() const { return address + 56896; }
  // view 3's camera (IntroCamera)
  uint32_t intro_camera_address() const { return address + 67760; }
  // the view's camera object
  uint32_t camera() const { return guest::LoadU32(base, address + 68528); }
  void set_camera(uint32_t v) const { guest::StoreU32(base, address + 68528, v); }
  uint32_t camera_address() const { return address + 68528; }
  // player 2's in split screen
  uint32_t camera2() const { return guest::LoadU32(base, address + 68532); }
  void set_camera2(uint32_t v) const { guest::StoreU32(base, address + 68532, v); }
  uint32_t camera2_address() const { return address + 68532; }
  // the player whose input record the update reads (its low byte)
  uint32_t followed_input() const { return guest::LoadU32(base, address + 77408); }
  void set_followed_input(uint32_t v) const { guest::StoreU32(base, address + 77408, v); }
  uint32_t followed_input_address() const { return address + 77408; }
};

// embedded; measure: -
struct CameraObject : View {
  using View::View;
  static constexpr uint32_t kSize = 340;
  // a view camera's vtable
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vehicle followed
  uint32_t vehicle() const { return guest::LoadU32(base, address + 16); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vehicle_address() const { return address + 16; }
  template <typename T = Viewport>
  T viewport() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_viewport(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t viewport_address() const { return address + 20; }
  // the target tracker (CameraTarget, with its track position at +224)
  uint32_t tracker_address() const { return address + 32; }
};

// embedded; measure: CameraTarget::Track.r3
struct CameraTarget : View {
  using View::View;
  static constexpr uint32_t kSize = 308;
  // the body's position, w 1.0
  uint32_t position_address() const { return address + 0; }
  float position_x() const { return guest::LoadFloat(base, address + 0); }
  void set_position_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t position_x_address() const { return address + 0; }
  float position_y() const { return guest::LoadFloat(base, address + 4); }
  void set_position_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t position_y_address() const { return address + 4; }
  float position_z() const { return guest::LoadFloat(base, address + 8); }
  void set_position_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t position_z_address() const { return address + 8; }
  float position_w() const { return guest::LoadFloat(base, address + 12); }
  void set_position_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t position_w_address() const { return address + 12; }
  // the last position
  uint32_t last_position_address() const { return address + 16; }
  // the mean of the vehicle's +1216 and +1584, w 1.0
  uint32_t point_address() const { return address + 32; }
  float point_x() const { return guest::LoadFloat(base, address + 32); }
  void set_point_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t point_x_address() const { return address + 32; }
  float point_y() const { return guest::LoadFloat(base, address + 36); }
  void set_point_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t point_y_address() const { return address + 36; }
  float point_z() const { return guest::LoadFloat(base, address + 40); }
  void set_point_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t point_z_address() const { return address + 40; }
  float point_w() const { return guest::LoadFloat(base, address + 44); }
  void set_point_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t point_w_address() const { return address + 44; }
  // the last point
  uint32_t last_point_address() const { return address + 48; }
  // the body's turn (a matrix, Matrix::FromQuat)
  uint32_t turn_address() const { return address + 64; }
  // the last turn
  uint32_t last_turn_address() const { return address + 128; }
  // the body's rotation (a quaternion)
  uint32_t rotation_address() const { return address + 192; }
  float rotation_x() const { return guest::LoadFloat(base, address + 192); }
  void set_rotation_x(float v) const { guest::StoreFloat(base, address + 192, v); }
  uint32_t rotation_x_address() const { return address + 192; }
  float rotation_y() const { return guest::LoadFloat(base, address + 196); }
  void set_rotation_y(float v) const { guest::StoreFloat(base, address + 196, v); }
  uint32_t rotation_y_address() const { return address + 196; }
  float rotation_z() const { return guest::LoadFloat(base, address + 200); }
  void set_rotation_z(float v) const { guest::StoreFloat(base, address + 200, v); }
  uint32_t rotation_z_address() const { return address + 200; }
  float rotation_w() const { return guest::LoadFloat(base, address + 204); }
  void set_rotation_w(float v) const { guest::StoreFloat(base, address + 204, v); }
  uint32_t rotation_w_address() const { return address + 204; }
  // the last rotation (or the chase camera's blended one)
  uint32_t last_rotation_address() const { return address + 208; }
  float last_rotation_x() const { return guest::LoadFloat(base, address + 208); }
  void set_last_rotation_x(float v) const { guest::StoreFloat(base, address + 208, v); }
  uint32_t last_rotation_x_address() const { return address + 208; }
  float last_rotation_y() const { return guest::LoadFloat(base, address + 212); }
  void set_last_rotation_y(float v) const { guest::StoreFloat(base, address + 212, v); }
  uint32_t last_rotation_y_address() const { return address + 212; }
  float last_rotation_z() const { return guest::LoadFloat(base, address + 216); }
  void set_last_rotation_z(float v) const { guest::StoreFloat(base, address + 216, v); }
  uint32_t last_rotation_z_address() const { return address + 216; }
  float last_rotation_w() const { return guest::LoadFloat(base, address + 220); }
  void set_last_rotation_w(float v) const { guest::StoreFloat(base, address + 220, v); }
  uint32_t last_rotation_w_address() const { return address + 220; }
  // the vehicle's track position (TrackPosition, 84 bytes)
  uint32_t track_position_address() const { return address + 224; }
};

// embedded; measure: ChaseCamera, TvCamera.+43136
struct ChaseCamera : View {
  using View::View;
  static constexpr uint32_t kSize = 384;
  static constexpr uint32_t kVtable = 0x82006B24;
  // 0x82006B24 (slot 0 ChaseCamera::Update)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vehicle followed
  uint32_t vehicle() const { return guest::LoadU32(base, address + 16); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vehicle_address() const { return address + 16; }
  template <typename T = Viewport>
  T viewport() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_viewport(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t viewport_address() const { return address + 20; }
  // the target tracker (CameraTarget, with its track position at +224)
  uint32_t tracker_address() const { return address + 32; }
  // measure fault: Quat::Rotate's quaternion joined with the camera
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // as unknown_4
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // as unknown_4
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // the eye at set-up and after each update
  uint32_t eye_address() const { return address + 352; }
  float eye_x() const { return guest::LoadFloat(base, address + 352); }
  void set_eye_x(float v) const { guest::StoreFloat(base, address + 352, v); }
  uint32_t eye_x_address() const { return address + 352; }
  float eye_y() const { return guest::LoadFloat(base, address + 356); }
  void set_eye_y(float v) const { guest::StoreFloat(base, address + 356, v); }
  uint32_t eye_y_address() const { return address + 356; }
  float eye_z() const { return guest::LoadFloat(base, address + 360); }
  void set_eye_z(float v) const { guest::StoreFloat(base, address + 360, v); }
  uint32_t eye_z_address() const { return address + 360; }
  // 1.0 at set-up
  float unknown_364() const { return guest::LoadFloat(base, address + 364); }
  void set_unknown_364(float v) const { guest::StoreFloat(base, address + 364, v); }
  uint32_t unknown_364_address() const { return address + 364; }
  // 0.0 by ChaseCamera::Place
  float unknown_368() const { return guest::LoadFloat(base, address + 368); }
  void set_unknown_368(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t unknown_368_address() const { return address + 368; }
  // the angle's index in the table at 0x8200930C
  uint32_t angle() const { return guest::LoadU32(base, address + 372); }
  void set_angle(uint32_t v) const { guest::StoreU32(base, address + 372, v); }
  uint32_t angle_address() const { return address + 372; }
  // the track position's section (Place)
  uint16_t section() const { return guest::LoadU16(base, address + 376); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 376, v); }
  uint32_t section_address() const { return address + 376; }
  // its slice
  uint16_t slice() const { return guest::LoadU16(base, address + 378); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 378, v); }
  uint32_t slice_address() const { return address + 378; }
  // set by the camera button: the eye moved along the view's line once
  uint32_t state() const { return guest::LoadU32(base, address + 380); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 380, v); }
  uint32_t state_address() const { return address + 380; }
};

// embedded; measure: ViewCamera1
struct ViewCamera1 : View {
  using View::View;
  static constexpr uint32_t kSize = 1108;
  static constexpr uint32_t kVtable = 0x82006B2C;
  // 0x82006B2C or 0x82006B34
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vehicle followed
  uint32_t vehicle() const { return guest::LoadU32(base, address + 16); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vehicle_address() const { return address + 16; }
  template <typename T = Viewport>
  T viewport() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_viewport(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t viewport_address() const { return address + 20; }
  // the target tracker (CameraTarget, with its track position at +224)
  uint32_t tracker_address() const { return address + 32; }
  // a CameraCurve::Load(+352, 0, 30) object
  uint32_t curve_address() const { return address + 352; }
  // the tracker's rotation at set-up; w 1.0
  uint32_t rotation_address() const { return address + 1088; }
  float rotation_x() const { return guest::LoadFloat(base, address + 1088); }
  void set_rotation_x(float v) const { guest::StoreFloat(base, address + 1088, v); }
  uint32_t rotation_x_address() const { return address + 1088; }
  float rotation_y() const { return guest::LoadFloat(base, address + 1092); }
  void set_rotation_y(float v) const { guest::StoreFloat(base, address + 1092, v); }
  uint32_t rotation_y_address() const { return address + 1092; }
  float rotation_z() const { return guest::LoadFloat(base, address + 1096); }
  void set_rotation_z(float v) const { guest::StoreFloat(base, address + 1096, v); }
  uint32_t rotation_z_address() const { return address + 1096; }
  float rotation_w() const { return guest::LoadFloat(base, address + 1100); }
  void set_rotation_w(float v) const { guest::StoreFloat(base, address + 1100, v); }
  uint32_t rotation_w_address() const { return address + 1100; }
  // the look angle (Matrix::RotateY), from the camera stick
  float look_angle() const { return guest::LoadFloat(base, address + 1104); }
  void set_look_angle(float v) const { guest::StoreFloat(base, address + 1104, v); }
  uint32_t look_angle_address() const { return address + 1104; }
};

// embedded; measure: ViewCamera2
struct ViewCamera2 : View {
  using View::View;
  static constexpr uint32_t kSize = 1104;
  static constexpr uint32_t kVtable = 0x82006B34;
  // 0x82006B2C or 0x82006B34
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vehicle followed
  uint32_t vehicle() const { return guest::LoadU32(base, address + 16); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vehicle_address() const { return address + 16; }
  template <typename T = Viewport>
  T viewport() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_viewport(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t viewport_address() const { return address + 20; }
  // the target tracker (CameraTarget, with its track position at +224)
  uint32_t tracker_address() const { return address + 32; }
  // a CameraCurve::Load(+352, 0, 30) object
  uint32_t curve_address() const { return address + 352; }
  // the tracker's rotation at set-up; w 1.0
  uint32_t rotation_address() const { return address + 1088; }
  float rotation_x() const { return guest::LoadFloat(base, address + 1088); }
  void set_rotation_x(float v) const { guest::StoreFloat(base, address + 1088, v); }
  uint32_t rotation_x_address() const { return address + 1088; }
  float rotation_y() const { return guest::LoadFloat(base, address + 1092); }
  void set_rotation_y(float v) const { guest::StoreFloat(base, address + 1092, v); }
  uint32_t rotation_y_address() const { return address + 1092; }
  float rotation_z() const { return guest::LoadFloat(base, address + 1096); }
  void set_rotation_z(float v) const { guest::StoreFloat(base, address + 1096, v); }
  uint32_t rotation_z_address() const { return address + 1096; }
  float rotation_w() const { return guest::LoadFloat(base, address + 1100); }
  void set_rotation_w(float v) const { guest::StoreFloat(base, address + 1100, v); }
  uint32_t rotation_w_address() const { return address + 1100; }
};

// embedded; measure: IntroCamera
struct IntroCamera : View {
  using View::View;
  static constexpr uint32_t kSize = 760;
  static constexpr uint32_t kVtable = 0x820093C4;
  // 0x820093C4 (slot 0 IntroCamera::Update)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vehicle followed
  uint32_t vehicle() const { return guest::LoadU32(base, address + 16); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vehicle_address() const { return address + 16; }
  template <typename T = Viewport>
  T viewport() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_viewport(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t viewport_address() const { return address + 20; }
  // the target tracker (CameraTarget, with its track position at +224)
  uint32_t tracker_address() const { return address + 32; }
  // measure fault: IntroShot's +4 (the shots joined with the camera)
  uint32_t shot_vehicle() const { return guest::LoadU32(base, address + 4); }
  void set_shot_vehicle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t shot_vehicle_address() const { return address + 4; }
  // measure fault: IntroShot's +8
  uint32_t shot_frames() const { return guest::LoadU32(base, address + 8); }
  void set_shot_frames(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t shot_frames_address() const { return address + 8; }
  // measure fault: IntroShot's +12
  float shot_length() const { return guest::LoadFloat(base, address + 12); }
  void set_shot_length(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t shot_length_address() const { return address + 12; }
  // a 16-slot List16 of 20-byte shots (IntroShot)
  uint32_t shots_address() const { return address + 352; }
  // the current shot's index
  uint32_t shot() const { return guest::LoadU32(base, address + 748); }
  void set_shot(uint32_t v) const { guest::StoreU32(base, address + 748, v); }
  uint32_t shot_address() const { return address + 748; }
  // the frame count
  uint32_t frames() const { return guest::LoadU32(base, address + 752); }
  void set_frames(uint32_t v) const { guest::StoreU32(base, address + 752, v); }
  uint32_t frames_address() const { return address + 752; }
  // 0 shots, 1 and 2 the orbit
  uint32_t phase() const { return guest::LoadU32(base, address + 756); }
  void set_phase(uint32_t v) const { guest::StoreU32(base, address + 756, v); }
  uint32_t phase_address() const { return address + 756; }
};

// embedded; measure: List16::Insert20.r4
struct IntroShot : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  template <typename T = Viewport>
  T viewport() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_viewport(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t viewport_address() const { return address + 0; }
  // the vehicle
  uint32_t vehicle() const { return guest::LoadU32(base, address + 4); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vehicle_address() const { return address + 4; }
  // its frame count
  uint32_t frames() const { return guest::LoadU32(base, address + 8); }
  void set_frames(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t frames_address() const { return address + 8; }
  // the spec's +88 x the float at 0x82002CCC
  float length() const { return guest::LoadFloat(base, address + 12); }
  void set_length(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t length_address() const { return address + 12; }
  // set to 3 once the count reaches 29
  uint32_t phase() const { return guest::LoadU32(base, address + 16); }
  void set_phase(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t phase_address() const { return address + 16; }
};

// embedded; measure: TvCamera, Key::Vec3.r4
struct TvCamera : View {
  using View::View;
  static constexpr uint32_t kSize = 43524;
  static constexpr uint32_t kVtable = 0x820093BC;
  // 0x820093BC (slot 0 TvCamera::Update)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vehicle followed
  uint32_t vehicle() const { return guest::LoadU32(base, address + 16); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vehicle_address() const { return address + 16; }
  template <typename T = Viewport>
  T viewport() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_viewport(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t viewport_address() const { return address + 20; }
  // the target tracker (CameraTarget, with its track position at +224)
  uint32_t tracker_address() const { return address + 32; }
  // measure fault: a path buffer's or a vector's word (CameraPath::Request, Key::Vec3) joined with the camera
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // as unknown_4
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // as unknown_4 (a path buffer's index)
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // as unknown_4 (a path buffer's state)
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // the slice the last step saw
  uint32_t slice() const { return guest::LoadU32(base, address + 352); }
  void set_slice(uint32_t v) const { guest::StoreU32(base, address + 352, v); }
  uint32_t slice_address() const { return address + 352; }
  // the slice one past the record's far end
  uint32_t next() const { return guest::LoadU32(base, address + 356); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t next_address() const { return address + 356; }
  // the route range it's on
  uint32_t spot() const { return guest::LoadU32(base, address + 360); }
  void set_spot(uint32_t v) const { guest::StoreU32(base, address + 360, v); }
  uint32_t spot_address() const { return address + 360; }
  // CameraPath::Evaluate's out
  uint32_t path_out_address() const { return address + 368; }
  // the eye (path_out +16)
  uint32_t eye_address() const { return address + 384; }
  float eye_x() const { return guest::LoadFloat(base, address + 384); }
  void set_eye_x(float v) const { guest::StoreFloat(base, address + 384, v); }
  uint32_t eye_x_address() const { return address + 384; }
  float eye_y() const { return guest::LoadFloat(base, address + 388); }
  void set_eye_y(float v) const { guest::StoreFloat(base, address + 388, v); }
  uint32_t eye_y_address() const { return address + 388; }
  float eye_z() const { return guest::LoadFloat(base, address + 392); }
  void set_eye_z(float v) const { guest::StoreFloat(base, address + 392, v); }
  uint32_t eye_z_address() const { return address + 392; }
  float eye_w() const { return guest::LoadFloat(base, address + 396); }
  void set_eye_w(float v) const { guest::StoreFloat(base, address + 396, v); }
  uint32_t eye_w_address() const { return address + 396; }
  // the target (path_out +32)
  uint32_t target_address() const { return address + 400; }
  float target_x() const { return guest::LoadFloat(base, address + 400); }
  void set_target_x(float v) const { guest::StoreFloat(base, address + 400, v); }
  uint32_t target_x_address() const { return address + 400; }
  float target_y() const { return guest::LoadFloat(base, address + 404); }
  void set_target_y(float v) const { guest::StoreFloat(base, address + 404, v); }
  uint32_t target_y_address() const { return address + 404; }
  float target_z() const { return guest::LoadFloat(base, address + 408); }
  void set_target_z(float v) const { guest::StoreFloat(base, address + 408, v); }
  uint32_t target_z_address() const { return address + 408; }
  float target_w() const { return guest::LoadFloat(base, address + 412); }
  void set_target_w(float v) const { guest::StoreFloat(base, address + 412, v); }
  uint32_t target_w_address() const { return address + 412; }
  // 4 floats (copied 8 bytes at a time too)
  uint32_t unknown_416_address() const { return address + 416; }
  float unknown_416_x() const { return guest::LoadFloat(base, address + 416); }
  void set_unknown_416_x(float v) const { guest::StoreFloat(base, address + 416, v); }
  uint32_t unknown_416_x_address() const { return address + 416; }
  float unknown_416_y() const { return guest::LoadFloat(base, address + 420); }
  void set_unknown_416_y(float v) const { guest::StoreFloat(base, address + 420, v); }
  uint32_t unknown_416_y_address() const { return address + 420; }
  float unknown_416_z() const { return guest::LoadFloat(base, address + 424); }
  void set_unknown_416_z(float v) const { guest::StoreFloat(base, address + 424, v); }
  uint32_t unknown_416_z_address() const { return address + 424; }
  float unknown_416_w() const { return guest::LoadFloat(base, address + 428); }
  void set_unknown_416_w(float v) const { guest::StoreFloat(base, address + 428, v); }
  uint32_t unknown_416_w_address() const { return address + 428; }
  // path_out +64
  float roll() const { return guest::LoadFloat(base, address + 432); }
  void set_roll(float v) const { guest::StoreFloat(base, address + 432, v); }
  uint32_t roll_address() const { return address + 432; }
  // path_out +68
  float zoom() const { return guest::LoadFloat(base, address + 436); }
  void set_zoom(float v) const { guest::StoreFloat(base, address + 436, v); }
  uint32_t zoom_address() const { return address + 436; }
  // 0 while it steps
  uint32_t paused() const { return guest::LoadU32(base, address + 448); }
  void set_paused(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t paused_address() const { return address + 448; }
  // the route ranges' count
  uint32_t range_count() const { return guest::LoadU32(base, address + 452); }
  void set_range_count(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t range_count_address() const { return address + 452; }
  // up to 40 RouteRanges, 1,064 bytes apart
  uint32_t ranges_address() const { return address + 456; }
  // four 28-byte camera path buffers (Buffer28: +12 the index, +24 the state)
  uint32_t buffers_address() const { return address + 43016; }
  // its fallback ChaseCamera
  uint32_t fallback_address() const { return address + 43136; }
  // 0 for the attract filter; the path's state (0, 1, 2)
  uint32_t filter_state() const { return guest::LoadU32(base, address + 43520); }
  void set_filter_state(uint32_t v) const { guest::StoreU32(base, address + 43520, v); }
  uint32_t filter_state_address() const { return address + 43520; }
};

// embedded; measure: Cutscene
struct Cutscene : View {
  using View::View;
  static constexpr uint32_t kSize = 10848;
  static constexpr uint32_t kVtable = 0x82006B3C;
  // 0x82006B3C (slot 0 Cutscene::Update, 1 OnPad)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vehicle followed
  uint32_t vehicle() const { return guest::LoadU32(base, address + 16); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vehicle_address() const { return address + 16; }
  template <typename T = Viewport>
  T viewport() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_viewport(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t viewport_address() const { return address + 20; }
  // the target tracker (CameraTarget, with its track position at +224)
  uint32_t tracker_address() const { return address + 32; }
  // the script, or -1
  uint32_t script() const { return guest::LoadU32(base, address + 352); }
  void set_script(uint32_t v) const { guest::StoreU32(base, address + 352, v); }
  uint32_t script_address() const { return address + 352; }
  // counted on each step
  uint32_t frame() const { return guest::LoadU32(base, address + 356); }
  void set_frame(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t frame_address() const { return address + 356; }
  // the shot stepped
  uint32_t shot() const { return guest::LoadU32(base, address + 360); }
  void set_shot(uint32_t v) const { guest::StoreU32(base, address + 360, v); }
  uint32_t shot_address() const { return address + 360; }
  // a 16-slot List16 of 32-byte FadeCues
  uint32_t cues_address() const { return address + 364; }
  // a 16-slot List16 of 608-byte CutsceneShots
  uint32_t shots_address() const { return address + 952; }
  // 0 at set-up; shots step while 0
  uint32_t unknown_10756() const { return guest::LoadU32(base, address + 10756); }
  void set_unknown_10756(uint32_t v) const { guest::StoreU32(base, address + 10756, v); }
  uint32_t unknown_10756_address() const { return address + 10756; }
  // the rival logo (RivalLogo: +4 its flags, +20 the XPR holder)
  uint32_t rival_logo_address() const { return address + 10760; }
};

// embedded; measure: CutsceneShot
struct CutsceneShot : View {
  using View::View;
  static constexpr uint32_t kSize = 608;
  // the script entry's kind
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // the animation's asset id (22824 + kind)
  uint32_t anim() const { return guest::LoadU32(base, address + 4); }
  void set_anim(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t anim_address() const { return address + 4; }
  // the vehicle
  uint32_t vehicle() const { return guest::LoadU32(base, address + 8); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t vehicle_address() const { return address + 8; }
  // a 16-slot List16 of 32-byte FadeCues
  uint32_t cues_address() const { return address + 12; }
  // the frame
  uint32_t time() const { return guest::LoadU32(base, address + 600); }
  void set_time(uint32_t v) const { guest::StoreU32(base, address + 600, v); }
  uint32_t time_address() const { return address + 600; }
  // 1 at set-up; 0x20 done, 0x10 held, 0x4, 4 not loaded
  uint32_t flags() const { return guest::LoadU32(base, address + 604); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 604, v); }
  uint32_t flags_address() const { return address + 604; }
};

// embedded; measure: CutsceneShot::Init.r4
struct CutsceneScriptEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  uint32_t vehicle() const { return guest::LoadU32(base, address + 4); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vehicle_address() const { return address + 4; }
  // a List16 of 32-byte FadeCues (count at +80)
  uint32_t cues_address() const { return address + 8; }
  // the cue list's count
  uint16_t cue_count() const { return guest::LoadU16(base, address + 80); }
  void set_cue_count(uint16_t v) const { guest::StoreU16(base, address + 80, v); }
  uint32_t cue_count_address() const { return address + 80; }
};

// embedded; measure: FadeCue
struct FadeCue : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // the start frame
  int32_t start() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_start(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t start_address() const { return address + 0; }
  // the end frame
  int32_t end() const { return int32_t(guest::LoadU32(base, address + 4)); }
  void set_end(int32_t v) const { guest::StoreU32(base, address + 4, uint32_t(v)); }
  uint32_t end_address() const { return address + 4; }
  // 0 ScreenFades::FadeIn, 1 FadeOut
  uint32_t kind() const { return guest::LoadU32(base, address + 8); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t kind_address() const { return address + 8; }
  uint32_t red() const { return guest::LoadU32(base, address + 12); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t red_address() const { return address + 12; }
  uint32_t green() const { return guest::LoadU32(base, address + 16); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t green_address() const { return address + 16; }
  uint32_t blue() const { return guest::LoadU32(base, address + 20); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t blue_address() const { return address + 20; }
  // the frame counted up to the end
  int32_t frame() const { return int32_t(guest::LoadU32(base, address + 24)); }
  void set_frame(int32_t v) const { guest::StoreU32(base, address + 24, uint32_t(v)); }
  uint32_t frame_address() const { return address + 24; }
  // not 0: the frame held
  int32_t hold() const { return int32_t(guest::LoadU32(base, address + 28)); }
  void set_hold(int32_t v) const { guest::StoreU32(base, address + 28, uint32_t(v)); }
  uint32_t hold_address() const { return address + 28; }
};

// embedded; measure: CutsceneShot::Step.r4
struct CameraShotOut : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  // the eye
  uint32_t eye_address() const { return address + 16; }
  float eye_x() const { return guest::LoadFloat(base, address + 16); }
  void set_eye_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t eye_x_address() const { return address + 16; }
  float eye_y() const { return guest::LoadFloat(base, address + 20); }
  void set_eye_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t eye_y_address() const { return address + 20; }
  float eye_z() const { return guest::LoadFloat(base, address + 24); }
  void set_eye_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t eye_z_address() const { return address + 24; }
  float eye_w() const { return guest::LoadFloat(base, address + 28); }
  void set_eye_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t eye_w_address() const { return address + 28; }
  // the target
  uint32_t target_address() const { return address + 32; }
  float target_x() const { return guest::LoadFloat(base, address + 32); }
  void set_target_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t target_x_address() const { return address + 32; }
  float target_y() const { return guest::LoadFloat(base, address + 36); }
  void set_target_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t target_y_address() const { return address + 36; }
  float target_z() const { return guest::LoadFloat(base, address + 40); }
  void set_target_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t target_z_address() const { return address + 40; }
  float target_w() const { return guest::LoadFloat(base, address + 44); }
  void set_target_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t target_w_address() const { return address + 44; }
  // the roll (radians), or the path's field of view
  float roll() const { return guest::LoadFloat(base, address + 64); }
  void set_roll(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t roll_address() const { return address + 64; }
  // the zoom
  float zoom() const { return guest::LoadFloat(base, address + 68); }
  void set_zoom(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t zoom_address() const { return address + 68; }
};

// embedded; measure: RivalLogo
struct RivalLogo : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  // the logo (0 to 28), -1 none
  uint32_t logo() const { return guest::LoadU32(base, address + 0); }
  void set_logo(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t logo_address() const { return address + 0; }
  // 0x1, 0x2 ready, 0x4 opening, 0x8 playing
  uint32_t flags() const { return guest::LoadU32(base, address + 4); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t flags_address() const { return address + 4; }
  // the UI movie's id
  uint32_t movie() const { return guest::LoadU32(base, address + 8); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t movie_address() const { return address + 8; }
  // the named animation
  uint32_t anim() const { return guest::LoadU32(base, address + 12); }
  void set_anim(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t anim_address() const { return address + 12; }
  // the movie's asset
  uint32_t asset() const { return guest::LoadU32(base, address + 16); }
  void set_asset(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t asset_address() const { return address + 16; }
  // the XPR holder's +0: -1
  uint32_t holder() const { return guest::LoadU32(base, address + 20); }
  void set_holder(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t holder_address() const { return address + 20; }
  // the holder's XPR (XPR::XPR)
  uint32_t xpr_address() const { return address + 24; }
  // the holder's +28: 2 at set-up, bit 0x10 loaded
  uint32_t xpr_flags() const { return guest::LoadU32(base, address + 48); }
  void set_xpr_flags(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t xpr_flags_address() const { return address + 48; }
  // two 12-byte texture records (the logo's)
  uint32_t textures_address() const { return address + 52; }
  // the movie's texture swapped out
  uint32_t old_texture0() const { return guest::LoadU32(base, address + 76); }
  void set_old_texture0(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t old_texture0_address() const { return address + 76; }
  uint32_t old_texture1() const { return guest::LoadU32(base, address + 80); }
  void set_old_texture1(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t old_texture1_address() const { return address + 80; }
  // 3: the movie's texture to swap
  uint32_t texture_name0() const { return guest::LoadU32(base, address + 84); }
  void set_texture_name0(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t texture_name0_address() const { return address + 84; }
  // 4
  uint32_t texture_name1() const { return guest::LoadU32(base, address + 88); }
  void set_texture_name1(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t texture_name1_address() const { return address + 88; }
};

// embedded; measure: -
struct CameraAnimEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 264;
  // the asset id
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  // up to 64 CameraAnim pointers (CameraAnimObjects)
  uint32_t objects_address() const { return address + 4; }
  // their count
  uint32_t count() const { return guest::LoadU32(base, address + 260); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t count_address() const { return address + 260; }
};

// embedded; measure: CameraAnims::Find.ret
struct CameraAnimObjects : View {
  using View::View;
  static constexpr uint32_t kSize = 260;
  // the first object (64 pointers)
  template <typename T = CameraAnim>
  T first() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  // 64 CameraAnim pointers
  uint32_t objects_address() const { return address + 0; }
  // their count
  uint32_t count() const { return guest::LoadU32(base, address + 256); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t count_address() const { return address + 256; }
};

// allocated; measure: CameraAnim, new<340>@CameraAnim::ParseObject, CameraAnim::EvalTarget.r4, CameraAnims::Find.ret.+0
#pragma pack(push, 1)
struct CameraAnimData {
  uint32_t kind = 0;  // +0
  uint32_t header_size = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
  uint32_t block_count = 0;  // +12
  uint32_t unknown_16 = 0;  // +16
  uint32_t unknown_20 = 0;  // +20
  uint32_t unknown_24 = 0;  // +24
  uint32_t unknown_28 = 0;  // +28
  uint32_t unknown_32 = 0;  // +32
  uint32_t unknown_36 = 0;  // +36
  float unknown_40 = 0;  // +40
  float unknown_44 = 0;  // +44
  uint8_t unknown_48[32] = {};  // +48, guest order
  uint32_t first_block = 0;  // +80
  uint8_t pad_84[252] = {};
  uint32_t blocks_used = 0;  // +336
};
#pragma pack(pop)
static_assert(sizeof(CameraAnimData) == 340);
inline constexpr memory::LayoutField kCameraAnimFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {80, 4}, {336, 4}};
inline constexpr memory::Layout kCameraAnimLayout = {kCameraAnimFields, 14};

struct CameraAnim : View {
  using View::View;
  static constexpr uint32_t kSize = 340;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CameraAnimData* data() const { return static_cast<CameraAnimData*>(memory::HandleObject(address)); }
  static CameraAnim New(uint8_t* base) {
    return CameraAnim(base, memory::MakeHandle(new CameraAnimData(), sizeof(CameraAnimData), "CameraAnim", &kCameraAnimLayout));
  }
  template <uint32_t Stride = kSize>
  static CameraAnim NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CameraAnimData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CameraAnimData*>(::operator new(sizeof(CameraAnimData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CameraAnimData();
    return CameraAnim(base, memory::MakeHandle(p, sizeof(CameraAnimData) * count, "CameraAnim", &kCameraAnimLayout, sizeof(CameraAnimData)));
  }
  CameraAnim at(uint32_t i) const { return CameraAnim(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CameraAnimData* direct() const { return static_cast<CameraAnimData*>(memory::HandleOwn(address, &kCameraAnimLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCameraAnimLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCameraAnimLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 6 a camera, 7 a target, 1 a matrix, 14 the end
  uint32_t kind_address() const { return address + 0; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the file's +4: its blocks start there
  uint32_t header_size_address() const { return address + 4; }
  uint32_t header_size_bits() const { if (auto* d = direct()) return d->header_size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_header_size_bits(uint32_t v) const { if (auto* d = direct()) { d->header_size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t header_size() const { if (auto* d = direct()) return d->header_size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_header_size(uint32_t v) const { if (auto* d = direct()) { d->header_size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8_bits() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_unknown_8_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_unknown_8(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the file object's blocks
  uint32_t block_count_address() const { return address + 12; }
  uint32_t block_count_bits() const { if (auto* d = direct()) return d->block_count; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_block_count_bits(uint32_t v) const { if (auto* d = direct()) { d->block_count = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t block_count() const { if (auto* d = direct()) return d->block_count; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_block_count(uint32_t v) const { if (auto* d = direct()) { d->block_count = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_16_bits() const { if (auto* d = direct()) return d->unknown_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_unknown_16_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16() const { if (auto* d = direct()) return d->unknown_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_unknown_16(uint32_t v) const { if (auto* d = direct()) { d->unknown_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_20_bits() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_unknown_20_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_unknown_20(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_24_bits() const { if (auto* d = direct()) return d->unknown_24; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_unknown_24_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_24 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24() const { if (auto* d = direct()) return d->unknown_24; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_unknown_24(uint32_t v) const { if (auto* d = direct()) { d->unknown_24 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_28_bits() const { if (auto* d = direct()) return d->unknown_28; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_unknown_28_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28() const { if (auto* d = direct()) return d->unknown_28; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_unknown_28(uint32_t v) const { if (auto* d = direct()) { d->unknown_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_32_bits() const { if (auto* d = direct()) return d->unknown_32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_unknown_32_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32() const { if (auto* d = direct()) return d->unknown_32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_unknown_32(uint32_t v) const { if (auto* d = direct()) { d->unknown_32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_36_bits() const { if (auto* d = direct()) return d->unknown_36; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_unknown_36_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_36 = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36() const { if (auto* d = direct()) return d->unknown_36; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_unknown_36(uint32_t v) const { if (auto* d = direct()) { d->unknown_36 = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_40_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_40); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_unknown_40_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_40 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  float unknown_40() const { return std::bit_cast<float>(unknown_40_bits()); }
  void set_unknown_40(float v) const { set_unknown_40_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_44_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_44); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_unknown_44_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_44 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  float unknown_44() const { return std::bit_cast<float>(unknown_44_bits()); }
  void set_unknown_44(float v) const { set_unknown_44_bits(std::bit_cast<uint32_t>(v)); }
  // 0
  uint32_t unknown_48_address() const { return address + 48; }
  uint8_t* unknown_48() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 48 : guest::Pointer(base, address + 48); }
  // up to 64 CameraAnimBlock pointers
  uint32_t blocks_address() const { return address + 80; }
  uint8_t* blocks() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 80 : guest::Pointer(base, address + 80); }
  uint32_t first_block_address() const { return address + 80; }
  uint32_t first_block_bits() const { if (auto* d = direct()) return d->first_block; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_first_block_bits(uint32_t v) const { if (auto* d = direct()) { d->first_block = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  template <typename T = CameraAnimBlock>
  T first_block() const { return T(base, first_block_bits()); }
  void set_first_block(const View& w) const { set_first_block_bits(w.address); }
  void set_first_block(uint32_t v) const { set_first_block_bits(v); }
  // their count
  uint32_t blocks_used_address() const { return address + 336; }
  uint32_t blocks_used_bits() const { if (auto* d = direct()) return d->blocks_used; return uint32_t((memory::IsHandle(address + 336) ? uint32_t(memory::OwnerLoad(address + 336, 4)) : guest::LoadU32(base, address + 336))); }
  void set_blocks_used_bits(uint32_t v) const { if (auto* d = direct()) { d->blocks_used = v; return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 4, v); else guest::StoreU32(base, address + 336, v); }
  uint32_t blocks_used() const { if (auto* d = direct()) return d->blocks_used; return uint32_t((memory::IsHandle(address + 336) ? uint32_t(memory::OwnerLoad(address + 336, 4)) : guest::LoadU32(base, address + 336))); }
  void set_blocks_used(uint32_t v) const { if (auto* d = direct()) { d->blocks_used = v; return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 4, v); else guest::StoreU32(base, address + 336, v); }
};

// allocated; measure: CameraAnimBlock, new<64>@CameraAnim::ParseObject, CameraAnim::EvalTarget.r4.+80, CameraAnims::Find.ret.+0.+80, CameraAnim::EvalTarget.r4.+80.+60
#pragma pack(push, 1)
struct CameraAnimBlockData {
  uint32_t channel = 0;  // +0
  uint32_t header_size = 0;  // +4
  uint32_t data_size = 0;  // +8
  float unknown_12 = 0;  // +12
  float duration = 0;  // +16
  uint8_t unknown_20[40] = {};  // +20, guest order
  uint32_t values = 0;  // +60
};
#pragma pack(pop)
static_assert(sizeof(CameraAnimBlockData) == 64);
inline constexpr memory::LayoutField kCameraAnimBlockFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {60, 4}};
inline constexpr memory::Layout kCameraAnimBlockLayout = {kCameraAnimBlockFields, 6};

struct CameraAnimBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CameraAnimBlockData* data() const { return static_cast<CameraAnimBlockData*>(memory::HandleObject(address)); }
  static CameraAnimBlock New(uint8_t* base) {
    return CameraAnimBlock(base, memory::MakeHandle(new CameraAnimBlockData(), sizeof(CameraAnimBlockData), "CameraAnimBlock", &kCameraAnimBlockLayout));
  }
  template <uint32_t Stride = kSize>
  static CameraAnimBlock NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CameraAnimBlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CameraAnimBlockData*>(::operator new(sizeof(CameraAnimBlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CameraAnimBlockData();
    return CameraAnimBlock(base, memory::MakeHandle(p, sizeof(CameraAnimBlockData) * count, "CameraAnimBlock", &kCameraAnimBlockLayout, sizeof(CameraAnimBlockData)));
  }
  CameraAnimBlock at(uint32_t i) const { return CameraAnimBlock(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CameraAnimBlockData* direct() const { return static_cast<CameraAnimBlockData*>(memory::HandleOwn(address, &kCameraAnimBlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCameraAnimBlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCameraAnimBlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0 x, 1 y, 2 -z, 101 the film's height, 102 the focal length, 109 -roll
  uint32_t channel_address() const { return address + 0; }
  uint32_t channel_bits() const { if (auto* d = direct()) return d->channel; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_channel_bits(uint32_t v) const { if (auto* d = direct()) { d->channel = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t channel() const { if (auto* d = direct()) return d->channel; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_channel(uint32_t v) const { if (auto* d = direct()) { d->channel = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the file block's (CameraAnimFileBlock)
  uint32_t header_size_address() const { return address + 4; }
  uint32_t header_size_bits() const { if (auto* d = direct()) return d->header_size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_header_size_bits(uint32_t v) const { if (auto* d = direct()) { d->header_size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t header_size() const { if (auto* d = direct()) return d->header_size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_header_size(uint32_t v) const { if (auto* d = direct()) { d->header_size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // its data's bytes
  uint32_t data_size_address() const { return address + 8; }
  uint32_t data_size_bits() const { if (auto* d = direct()) return d->data_size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_data_size_bits(uint32_t v) const { if (auto* d = direct()) { d->data_size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t data_size() const { if (auto* d = direct()) return d->data_size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_data_size(uint32_t v) const { if (auto* d = direct()) { d->data_size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_12_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_12); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_unknown_12_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_12 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  float unknown_12() const { return std::bit_cast<float>(unknown_12_bits()); }
  void set_unknown_12(float v) const { set_unknown_12_bits(std::bit_cast<uint32_t>(v)); }
  // CameraAnims::Duration's answer
  uint32_t duration_address() const { return address + 16; }
  uint32_t duration_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->duration); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_duration_bits(uint32_t v) const { if (auto* d = direct()) { d->duration = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  float duration() const { return std::bit_cast<float>(duration_bits()); }
  void set_duration(float v) const { set_duration_bits(std::bit_cast<uint32_t>(v)); }
  // 0
  uint32_t unknown_20_address() const { return address + 20; }
  uint8_t* unknown_20() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 20 : guest::Pointer(base, address + 20); }
  // its floats, one a frame (guest pages); "data" before E5 step 3 (a host view's data() is its struct)
  uint32_t values_address() const { return address + 60; }
  uint32_t values_bits() const { if (auto* d = direct()) return d->values; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_values_bits(uint32_t v) const { if (auto* d = direct()) { d->values = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t values() const { if (auto* d = direct()) return d->values; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_values(uint32_t v) const { if (auto* d = direct()) { d->values = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
};

// embedded; measure: CameraAnims::OnLoaded.r4
struct CameraAnimFileObject : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 14 the end
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // its blocks after this
  uint32_t header_size() const { return guest::LoadU32(base, address + 4); }
  void set_header_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t header_size_address() const { return address + 4; }
  // the next object after this
  uint32_t data_size() const { return guest::LoadU32(base, address + 8); }
  void set_data_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_size_address() const { return address + 8; }
};

// file; measure: -
struct CameraAnimFileBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t channel() const { return guest::LoadU32(base, address + 0); }
  void set_channel(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t channel_address() const { return address + 0; }
  // its data after this
  uint32_t header_size() const { return guest::LoadU32(base, address + 4); }
  void set_header_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t header_size_address() const { return address + 4; }
  // the next block after its data
  uint32_t data_size() const { return guest::LoadU32(base, address + 8); }
  void set_data_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_size_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float duration() const { return guest::LoadFloat(base, address + 16); }
  void set_duration(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t duration_address() const { return address + 16; }
};

// allocated; measure: new<4>@CameraAnims::Load, CameraAnims::OnLoaded.r6
#pragma pack(push, 1)
struct CameraAnimLoadData {
  uint32_t id = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(CameraAnimLoadData) == 4);
inline constexpr memory::LayoutField kCameraAnimLoadFields[] = {{0, 4}};
inline constexpr memory::Layout kCameraAnimLoadLayout = {kCameraAnimLoadFields, 1};

struct CameraAnimLoad : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CameraAnimLoadData* data() const { return static_cast<CameraAnimLoadData*>(memory::HandleObject(address)); }
  static CameraAnimLoad New(uint8_t* base) {
    return CameraAnimLoad(base, memory::MakeHandle(new CameraAnimLoadData(), sizeof(CameraAnimLoadData), "CameraAnimLoad", &kCameraAnimLoadLayout));
  }
  template <uint32_t Stride = kSize>
  static CameraAnimLoad NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CameraAnimLoadData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CameraAnimLoadData*>(::operator new(sizeof(CameraAnimLoadData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CameraAnimLoadData();
    return CameraAnimLoad(base, memory::MakeHandle(p, sizeof(CameraAnimLoadData) * count, "CameraAnimLoad", &kCameraAnimLoadLayout, sizeof(CameraAnimLoadData)));
  }
  CameraAnimLoad at(uint32_t i) const { return CameraAnimLoad(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CameraAnimLoadData* direct() const { return static_cast<CameraAnimLoadData*>(memory::HandleOwn(address, &kCameraAnimLoadLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCameraAnimLoadLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCameraAnimLoadLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // the asset id
  uint32_t id_address() const { return address + 0; }
  uint32_t id_bits() const { if (auto* d = direct()) return d->id; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_id_bits(uint32_t v) const { if (auto* d = direct()) { d->id = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t id() const { if (auto* d = direct()) return d->id; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_id(uint32_t v) const { if (auto* d = direct()) { d->id = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
};

// embedded; measure: CameraAnim::EvalCamera.r5
struct CameraAnimOut : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  uint64_t unknown_0() const { return guest::LoadU64(base, address + 0); }
  void set_unknown_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint64_t unknown_8() const { return guest::LoadU64(base, address + 8); }
  void set_unknown_8(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // x, y, -z, 1.0
  uint32_t position_address() const { return address + 16; }
  float x() const { return guest::LoadFloat(base, address + 16); }
  void set_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t x_address() const { return address + 16; }
  float y() const { return guest::LoadFloat(base, address + 20); }
  void set_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t y_address() const { return address + 20; }
  float z() const { return guest::LoadFloat(base, address + 24); }
  void set_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t z_address() const { return address + 24; }
  float w() const { return guest::LoadFloat(base, address + 28); }
  void set_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t w_address() const { return address + 28; }
  // the roll (degrees)
  float roll() const { return guest::LoadFloat(base, address + 32); }
  void set_roll(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t roll_address() const { return address + 32; }
  float zoom() const { return guest::LoadFloat(base, address + 36); }
  void set_zoom(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t zoom_address() const { return address + 36; }
};

// file; measure: CameraPath
struct CameraPathKey : View {
  using View::View;
  static constexpr uint32_t kSize = 216;
  // also Buffer28's vtable by the measure's join
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t fov_kind() const { return guest::LoadU32(base, address + 8); }
  void set_fov_kind(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fov_kind_address() const { return address + 8; }
  uint32_t fov() const { return guest::LoadU32(base, address + 12); }
  void set_fov(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fov_address() const { return address + 12; }
  uint32_t zoom_kind() const { return guest::LoadU32(base, address + 16); }
  void set_zoom_kind(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t zoom_kind_address() const { return address + 16; }
  uint32_t zoom() const { return guest::LoadU32(base, address + 20); }
  void set_zoom(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t zoom_address() const { return address + 20; }
  uint32_t eye_position_x_kind() const { return guest::LoadU32(base, address + 24); }
  void set_eye_position_x_kind(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t eye_position_x_kind_address() const { return address + 24; }
  uint32_t eye_position_x() const { return guest::LoadU32(base, address + 28); }
  void set_eye_position_x(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t eye_position_x_address() const { return address + 28; }
  uint32_t eye_position_y_kind() const { return guest::LoadU32(base, address + 32); }
  void set_eye_position_y_kind(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t eye_position_y_kind_address() const { return address + 32; }
  uint32_t eye_position_y() const { return guest::LoadU32(base, address + 36); }
  void set_eye_position_y(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t eye_position_y_address() const { return address + 36; }
  uint32_t eye_position_z_kind() const { return guest::LoadU32(base, address + 40); }
  void set_eye_position_z_kind(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t eye_position_z_kind_address() const { return address + 40; }
  uint32_t eye_position_z() const { return guest::LoadU32(base, address + 44); }
  void set_eye_position_z(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t eye_position_z_address() const { return address + 44; }
  uint32_t eye_offset_x_kind() const { return guest::LoadU32(base, address + 48); }
  void set_eye_offset_x_kind(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t eye_offset_x_kind_address() const { return address + 48; }
  uint32_t eye_offset_x() const { return guest::LoadU32(base, address + 52); }
  void set_eye_offset_x(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t eye_offset_x_address() const { return address + 52; }
  uint32_t eye_offset_y_kind() const { return guest::LoadU32(base, address + 56); }
  void set_eye_offset_y_kind(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t eye_offset_y_kind_address() const { return address + 56; }
  uint32_t eye_offset_y() const { return guest::LoadU32(base, address + 60); }
  void set_eye_offset_y(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t eye_offset_y_address() const { return address + 60; }
  uint32_t eye_offset_z_kind() const { return guest::LoadU32(base, address + 64); }
  void set_eye_offset_z_kind(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t eye_offset_z_kind_address() const { return address + 64; }
  uint32_t eye_offset_z() const { return guest::LoadU32(base, address + 68); }
  void set_eye_offset_z(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t eye_offset_z_address() const { return address + 68; }
  uint32_t eye_route_offset_x_kind() const { return guest::LoadU32(base, address + 80); }
  void set_eye_route_offset_x_kind(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t eye_route_offset_x_kind_address() const { return address + 80; }
  uint32_t eye_route_offset_x() const { return guest::LoadU32(base, address + 84); }
  void set_eye_route_offset_x(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t eye_route_offset_x_address() const { return address + 84; }
  uint32_t eye_route_offset_y_kind() const { return guest::LoadU32(base, address + 88); }
  void set_eye_route_offset_y_kind(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t eye_route_offset_y_kind_address() const { return address + 88; }
  uint32_t eye_route_offset_y() const { return guest::LoadU32(base, address + 92); }
  void set_eye_route_offset_y(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t eye_route_offset_y_address() const { return address + 92; }
  uint32_t eye_route_offset_z_kind() const { return guest::LoadU32(base, address + 96); }
  void set_eye_route_offset_z_kind(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t eye_route_offset_z_kind_address() const { return address + 96; }
  uint32_t eye_route_offset_z() const { return guest::LoadU32(base, address + 100); }
  void set_eye_route_offset_z(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t eye_route_offset_z_address() const { return address + 100; }
  uint32_t eye_weight_kind() const { return guest::LoadU32(base, address + 72); }
  void set_eye_weight_kind(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t eye_weight_kind_address() const { return address + 72; }
  uint32_t eye_weight() const { return guest::LoadU32(base, address + 76); }
  void set_eye_weight(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t eye_weight_address() const { return address + 76; }
  uint32_t eye_route_weight_kind() const { return guest::LoadU32(base, address + 104); }
  void set_eye_route_weight_kind(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t eye_route_weight_kind_address() const { return address + 104; }
  uint32_t eye_route_weight() const { return guest::LoadU32(base, address + 108); }
  void set_eye_route_weight(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t eye_route_weight_address() const { return address + 108; }
  uint32_t eye_route_slices_kind() const { return guest::LoadU32(base, address + 112); }
  void set_eye_route_slices_kind(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t eye_route_slices_kind_address() const { return address + 112; }
  uint32_t eye_route_slices() const { return guest::LoadU32(base, address + 116); }
  void set_eye_route_slices(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t eye_route_slices_address() const { return address + 116; }
  uint32_t target_position_x_kind() const { return guest::LoadU32(base, address + 120); }
  void set_target_position_x_kind(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t target_position_x_kind_address() const { return address + 120; }
  uint32_t target_position_x() const { return guest::LoadU32(base, address + 124); }
  void set_target_position_x(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t target_position_x_address() const { return address + 124; }
  uint32_t target_position_y_kind() const { return guest::LoadU32(base, address + 128); }
  void set_target_position_y_kind(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t target_position_y_kind_address() const { return address + 128; }
  uint32_t target_position_y() const { return guest::LoadU32(base, address + 132); }
  void set_target_position_y(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t target_position_y_address() const { return address + 132; }
  uint32_t target_position_z_kind() const { return guest::LoadU32(base, address + 136); }
  void set_target_position_z_kind(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t target_position_z_kind_address() const { return address + 136; }
  uint32_t target_position_z() const { return guest::LoadU32(base, address + 140); }
  void set_target_position_z(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t target_position_z_address() const { return address + 140; }
  uint32_t target_offset_x_kind() const { return guest::LoadU32(base, address + 144); }
  void set_target_offset_x_kind(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t target_offset_x_kind_address() const { return address + 144; }
  uint32_t target_offset_x() const { return guest::LoadU32(base, address + 148); }
  void set_target_offset_x(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t target_offset_x_address() const { return address + 148; }
  uint32_t target_offset_y_kind() const { return guest::LoadU32(base, address + 152); }
  void set_target_offset_y_kind(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t target_offset_y_kind_address() const { return address + 152; }
  uint32_t target_offset_y() const { return guest::LoadU32(base, address + 156); }
  void set_target_offset_y(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t target_offset_y_address() const { return address + 156; }
  uint32_t target_offset_z_kind() const { return guest::LoadU32(base, address + 160); }
  void set_target_offset_z_kind(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t target_offset_z_kind_address() const { return address + 160; }
  uint32_t target_offset_z() const { return guest::LoadU32(base, address + 164); }
  void set_target_offset_z(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t target_offset_z_address() const { return address + 164; }
  uint32_t target_route_offset_x_kind() const { return guest::LoadU32(base, address + 176); }
  void set_target_route_offset_x_kind(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t target_route_offset_x_kind_address() const { return address + 176; }
  uint32_t target_route_offset_x() const { return guest::LoadU32(base, address + 180); }
  void set_target_route_offset_x(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t target_route_offset_x_address() const { return address + 180; }
  uint32_t target_route_offset_y_kind() const { return guest::LoadU32(base, address + 184); }
  void set_target_route_offset_y_kind(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t target_route_offset_y_kind_address() const { return address + 184; }
  uint32_t target_route_offset_y() const { return guest::LoadU32(base, address + 188); }
  void set_target_route_offset_y(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t target_route_offset_y_address() const { return address + 188; }
  uint32_t target_route_offset_z_kind() const { return guest::LoadU32(base, address + 192); }
  void set_target_route_offset_z_kind(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t target_route_offset_z_kind_address() const { return address + 192; }
  uint32_t target_route_offset_z() const { return guest::LoadU32(base, address + 196); }
  void set_target_route_offset_z(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t target_route_offset_z_address() const { return address + 196; }
  uint32_t target_weight_kind() const { return guest::LoadU32(base, address + 168); }
  void set_target_weight_kind(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t target_weight_kind_address() const { return address + 168; }
  uint32_t target_weight() const { return guest::LoadU32(base, address + 172); }
  void set_target_weight(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t target_weight_address() const { return address + 172; }
  uint32_t target_route_weight_kind() const { return guest::LoadU32(base, address + 200); }
  void set_target_route_weight_kind(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t target_route_weight_kind_address() const { return address + 200; }
  uint32_t target_route_weight() const { return guest::LoadU32(base, address + 204); }
  void set_target_route_weight(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t target_route_weight_address() const { return address + 204; }
  uint32_t target_route_slices_kind() const { return guest::LoadU32(base, address + 208); }
  void set_target_route_slices_kind(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t target_route_slices_kind_address() const { return address + 208; }
  uint32_t target_route_slices() const { return guest::LoadU32(base, address + 212); }
  void set_target_route_slices(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t target_route_slices_address() const { return address + 212; }
};

// file; measure: CameraPath::OnLoaded.r4
struct CameraPathFile : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // its 20-byte paths (CameraPathRecord) at +16
  uint32_t path_count() const { return guest::LoadU32(base, address + 0); }
  void set_path_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t path_count_address() const { return address + 0; }
  // its 216-byte keys after them
  uint32_t key_count() const { return guest::LoadU32(base, address + 4); }
  void set_key_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t key_count_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// file; measure: -
struct CameraPathRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // one end slice
  uint32_t near() const { return guest::LoadU32(base, address + 0); }
  void set_near(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t near_address() const { return address + 0; }
  // the other
  uint32_t far() const { return guest::LoadU32(base, address + 4); }
  void set_far(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t far_address() const { return address + 4; }
  // its keys
  uint32_t key_count() const { return guest::LoadU32(base, address + 8); }
  void set_key_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t key_count_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // the first key's index, made its address (FixKeys)
  uint32_t keys() const { return guest::LoadU32(base, address + 16); }
  void set_keys(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t keys_address() const { return address + 16; }
};

// file; measure: Curve::Evaluate.r4
struct CameraCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  float time() const { return guest::LoadFloat(base, address + 0); }
  void set_time(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t time_address() const { return address + 0; }
  float value() const { return guest::LoadFloat(base, address + 4); }
  void set_value(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t value_address() const { return address + 4; }
  float leave() const { return guest::LoadFloat(base, address + 8); }
  void set_leave(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t leave_address() const { return address + 8; }
  float tangent() const { return guest::LoadFloat(base, address + 16); }
  void set_tangent(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t tangent_address() const { return address + 16; }
  // 3 the end key
  uint32_t kind() const { return guest::LoadU32(base, address + 20); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t kind_address() const { return address + 20; }
  // 24-byte keys (CameraCurveKey)
  uint32_t keys_address() const { return address + 0; }
};

// file; measure: -
struct CameraCurveKey : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  float time() const { return guest::LoadFloat(base, address + 0); }
  void set_time(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t time_address() const { return address + 0; }
  float value() const { return guest::LoadFloat(base, address + 4); }
  void set_value(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t value_address() const { return address + 4; }
  // the value it leaves
  float leave() const { return guest::LoadFloat(base, address + 8); }
  void set_leave(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t leave_address() const { return address + 8; }
  // probably: the tangent Curve::Evaluate's Hermite takes at the next key
  float tangent_in() const { return guest::LoadFloat(base, address + 12); }
  void set_tangent_in(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t tangent_in_address() const { return address + 12; }
  // the tangent it leaves with
  float tangent() const { return guest::LoadFloat(base, address + 16); }
  void set_tangent(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t tangent_address() const { return address + 16; }
  // 0 hold, 1 a line, 2 Hermite, 3 the end
  uint32_t kind() const { return guest::LoadU32(base, address + 20); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t kind_address() const { return address + 20; }
};

// embedded; measure: Hermite::Basis.r4, Hermite::Basis.r5
struct HermiteKey : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float value0() const { return guest::LoadFloat(base, address + 0); }
  void set_value0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value0_address() const { return address + 0; }
  float value1() const { return guest::LoadFloat(base, address + 4); }
  void set_value1(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t value1_address() const { return address + 4; }
  float tangent0() const { return guest::LoadFloat(base, address + 8); }
  void set_tangent0(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t tangent0_address() const { return address + 8; }
  float tangent1() const { return guest::LoadFloat(base, address + 12); }
  void set_tangent1(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t tangent1_address() const { return address + 12; }
};

// embedded; measure: Hermite
struct HermiteOut : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  float a() const { return guest::LoadFloat(base, address + 0); }
  void set_a(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t a_address() const { return address + 0; }
  float b() const { return guest::LoadFloat(base, address + 4); }
  void set_b(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t b_address() const { return address + 4; }
};

// embedded; measure: Matrix::FromRows3.r4, Matrix::FromRows3.r5, Matrix::FromRows3.r6, ChaseCamera::Place.r4, ChaseCamera::Place.r5, ChaseCamera::Place.r6, ChaseCamera::Place.r7
struct CameraVec4 : View {
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
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// embedded; measure: Quat::Rotate.r5, Viewport::Offset.r5, Viewport::PullBack.r5, CameraPath::Evaluate.r6
struct CameraVec3 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
};

// embedded; measure: Quat
struct CameraQuat : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // x and y, copied as one
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// embedded; measure: Matrix::FromFloats.r4, Matrix::FromFloats.r5, Matrix::FromFloats.r6, Matrix::FromFloats.r7, Matrix::FromFloats.r8, Matrix::FromFloats.r9, Matrix::FromFloats.r10
struct CameraFloat : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float value() const { return guest::LoadFloat(base, address + 0); }
  void set_value(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: RigidBody::Matrix.ret, CameraAnim::Matrix.r5
struct CameraMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the right axis (copied 8 bytes at a time too)
  uint32_t row0_address() const { return address + 0; }
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

// embedded; measure: Stages::Copy.ret
struct StageCopy : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the stage's first word (1: ... OnRaceStart checks it)
  uint32_t first() const { return guest::LoadU32(base, address + 0); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
};

// embedded; measure: List16, CutsceneShot::Init.r4@+8
struct List16 : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  // 16 nodes: (next, slot) halfwords
  uint32_t nodes_address() const { return address + 0; }
  // the free list's head
  uint16_t free() const { return guest::LoadU16(base, address + 64); }
  void set_free(uint16_t v) const { guest::StoreU16(base, address + 64, v); }
  uint32_t free_address() const { return address + 64; }
  uint16_t unknown_66() const { return guest::LoadU16(base, address + 66); }
  void set_unknown_66(uint16_t v) const { guest::StoreU16(base, address + 66, v); }
  uint32_t unknown_66_address() const { return address + 66; }
  // the head
  uint16_t head() const { return guest::LoadU16(base, address + 68); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 68, v); }
  uint32_t head_address() const { return address + 68; }
  uint16_t unknown_70() const { return guest::LoadU16(base, address + 70); }
  void set_unknown_70(uint16_t v) const { guest::StoreU16(base, address + 70, v); }
  uint32_t unknown_70_address() const { return address + 70; }
  uint16_t count() const { return guest::LoadU16(base, address + 72); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 72, v); }
  uint32_t count_address() const { return address + 72; }
  uint16_t unknown_74() const { return guest::LoadU16(base, address + 74); }
  void set_unknown_74(uint16_t v) const { guest::StoreU16(base, address + 74, v); }
  uint32_t unknown_74_address() const { return address + 74; }
};

// embedded; measure: List40
struct List40 : View {
  using View::View;
  static constexpr uint32_t kSize = 172;
  // 40 nodes: (next, slot) halfwords
  uint32_t nodes_address() const { return address + 0; }
  // the free list's head
  uint16_t free() const { return guest::LoadU16(base, address + 160); }
  void set_free(uint16_t v) const { guest::StoreU16(base, address + 160, v); }
  uint32_t free_address() const { return address + 160; }
  uint16_t head() const { return guest::LoadU16(base, address + 164); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 164, v); }
  uint32_t head_address() const { return address + 164; }
  uint16_t count() const { return guest::LoadU16(base, address + 168); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 168, v); }
  uint32_t count_address() const { return address + 168; }
};

// embedded; measure: List40::Insert.r4
struct List40Word : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: ChaseCamera@+224
struct ChaseCameraAt224 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: TvCamera@+32
struct TvCameraAt32 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: TvCamera@+368
struct TvCameraAt368 : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  float unknown_20() const { return guest::LoadFloat(base, address + 20); }
  void set_unknown_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  float unknown_24() const { return guest::LoadFloat(base, address + 24); }
  void set_unknown_24(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  float unknown_32() const { return guest::LoadFloat(base, address + 32); }
  void set_unknown_32(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  float unknown_36() const { return guest::LoadFloat(base, address + 36); }
  void set_unknown_36(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  float unknown_40() const { return guest::LoadFloat(base, address + 40); }
  void set_unknown_40(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  float unknown_64() const { return guest::LoadFloat(base, address + 64); }
  void set_unknown_64(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  float unknown_68() const { return guest::LoadFloat(base, address + 68); }
  void set_unknown_68(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
};

// embedded; measure: ViewCamera1@+1104
struct ViewCamera1At1104 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: CameraDirector@+13360
struct TvCameraAt13360 : View {
  using View::View;
  static constexpr uint32_t kSize = 43044;
  uint32_t unknown_452() const { return guest::LoadU32(base, address + 452); }
  void set_unknown_452(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t unknown_452_address() const { return address + 452; }
  uint32_t unknown_43028() const { return guest::LoadU32(base, address + 43028); }
  void set_unknown_43028(uint32_t v) const { guest::StoreU32(base, address + 43028, v); }
  uint32_t unknown_43028_address() const { return address + 43028; }
  uint32_t unknown_43040() const { return guest::LoadU32(base, address + 43040); }
  void set_unknown_43040(uint32_t v) const { guest::StoreU32(base, address + 43040, v); }
  uint32_t unknown_43040_address() const { return address + 43040; }
};

// embedded; measure: CameraPath::OnLoaded.r4@+16
struct CameraPathKeyAt16 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: Cutscene@+10760
struct RivalLogoAt10760 : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_76() const { return guest::LoadU32(base, address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t unknown_84() const { return guest::LoadU32(base, address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
};

// embedded; measure: Cutscene@+10760@+20
struct RivalLogoAt20 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
};

// embedded; measure: Cutscene@+952
struct List16At952 : View {
  using View::View;
  static constexpr uint32_t kSize = 70;
  uint16_t unknown_68() const { return guest::LoadU16(base, address + 68); }
  void set_unknown_68(uint16_t v) const { guest::StoreU16(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
};

// embedded; measure: CutsceneShot@+12
struct List16At12 : View {
  using View::View;
  static constexpr uint32_t kSize = 70;
  uint16_t unknown_68() const { return guest::LoadU16(base, address + 68); }
  void set_unknown_68(uint16_t v) const { guest::StoreU16(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
};

// embedded; measure: IntroCamera@+352
struct List16At352 : View {
  using View::View;
  static constexpr uint32_t kSize = 70;
  uint16_t unknown_68() const { return guest::LoadU16(base, address + 68); }
  void set_unknown_68(uint16_t v) const { guest::StoreU16(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
};

// embedded; measure: RaceMode@+10512
struct CameraDirectorAt10512 : View {
  using View::View;
  static constexpr uint32_t kSize = 77392;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_9616() const { return guest::LoadU32(base, address + 9616); }
  void set_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t unknown_9616_address() const { return address + 9616; }
  uint32_t unknown_9620() const { return guest::LoadU32(base, address + 9620); }
  void set_unknown_9620(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t unknown_9620_address() const { return address + 9620; }
  uint32_t unknown_9624() const { return guest::LoadU32(base, address + 9624); }
  void set_unknown_9624(uint32_t v) const { guest::StoreU32(base, address + 9624, v); }
  uint32_t unknown_9624_address() const { return address + 9624; }
  uint32_t unknown_9628() const { return guest::LoadU32(base, address + 9628); }
  void set_unknown_9628(uint32_t v) const { guest::StoreU32(base, address + 9628, v); }
  uint32_t unknown_9628_address() const { return address + 9628; }
  uint32_t unknown_9632() const { return guest::LoadU32(base, address + 9632); }
  void set_unknown_9632(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t unknown_9632_address() const { return address + 9632; }
  uint32_t unknown_9648() const { return guest::LoadU32(base, address + 9648); }
  void set_unknown_9648(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t unknown_9648_address() const { return address + 9648; }
  uint32_t unknown_9664() const { return guest::LoadU32(base, address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
  uint32_t unknown_9668() const { return guest::LoadU32(base, address + 9668); }
  void set_unknown_9668(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t unknown_9668_address() const { return address + 9668; }
  float unknown_9692() const { return guest::LoadFloat(base, address + 9692); }
  void set_unknown_9692(float v) const { guest::StoreFloat(base, address + 9692, v); }
  uint32_t unknown_9692_address() const { return address + 9692; }
  float unknown_9708() const { return guest::LoadFloat(base, address + 9708); }
  void set_unknown_9708(float v) const { guest::StoreFloat(base, address + 9708, v); }
  uint32_t unknown_9708_address() const { return address + 9708; }
  float unknown_9724() const { return guest::LoadFloat(base, address + 9724); }
  void set_unknown_9724(float v) const { guest::StoreFloat(base, address + 9724, v); }
  uint32_t unknown_9724_address() const { return address + 9724; }
  float unknown_9740() const { return guest::LoadFloat(base, address + 9740); }
  void set_unknown_9740(float v) const { guest::StoreFloat(base, address + 9740, v); }
  uint32_t unknown_9740_address() const { return address + 9740; }
  float unknown_9756() const { return guest::LoadFloat(base, address + 9756); }
  void set_unknown_9756(float v) const { guest::StoreFloat(base, address + 9756, v); }
  uint32_t unknown_9756_address() const { return address + 9756; }
  float unknown_9772() const { return guest::LoadFloat(base, address + 9772); }
  void set_unknown_9772(float v) const { guest::StoreFloat(base, address + 9772, v); }
  uint32_t unknown_9772_address() const { return address + 9772; }
  float unknown_9788() const { return guest::LoadFloat(base, address + 9788); }
  void set_unknown_9788(float v) const { guest::StoreFloat(base, address + 9788, v); }
  uint32_t unknown_9788_address() const { return address + 9788; }
  float unknown_9804() const { return guest::LoadFloat(base, address + 9804); }
  void set_unknown_9804(float v) const { guest::StoreFloat(base, address + 9804, v); }
  uint32_t unknown_9804_address() const { return address + 9804; }
  float unknown_9820() const { return guest::LoadFloat(base, address + 9820); }
  void set_unknown_9820(float v) const { guest::StoreFloat(base, address + 9820, v); }
  uint32_t unknown_9820_address() const { return address + 9820; }
  float unknown_9836() const { return guest::LoadFloat(base, address + 9836); }
  void set_unknown_9836(float v) const { guest::StoreFloat(base, address + 9836, v); }
  uint32_t unknown_9836_address() const { return address + 9836; }
  float unknown_9852() const { return guest::LoadFloat(base, address + 9852); }
  void set_unknown_9852(float v) const { guest::StoreFloat(base, address + 9852, v); }
  uint32_t unknown_9852_address() const { return address + 9852; }
  float unknown_9868() const { return guest::LoadFloat(base, address + 9868); }
  void set_unknown_9868(float v) const { guest::StoreFloat(base, address + 9868, v); }
  uint32_t unknown_9868_address() const { return address + 9868; }
  float unknown_9884() const { return guest::LoadFloat(base, address + 9884); }
  void set_unknown_9884(float v) const { guest::StoreFloat(base, address + 9884, v); }
  uint32_t unknown_9884_address() const { return address + 9884; }
  float unknown_9900() const { return guest::LoadFloat(base, address + 9900); }
  void set_unknown_9900(float v) const { guest::StoreFloat(base, address + 9900, v); }
  uint32_t unknown_9900_address() const { return address + 9900; }
  float unknown_9932() const { return guest::LoadFloat(base, address + 9932); }
  void set_unknown_9932(float v) const { guest::StoreFloat(base, address + 9932, v); }
  uint32_t unknown_9932_address() const { return address + 9932; }
  float unknown_9948() const { return guest::LoadFloat(base, address + 9948); }
  void set_unknown_9948(float v) const { guest::StoreFloat(base, address + 9948, v); }
  uint32_t unknown_9948_address() const { return address + 9948; }
  float unknown_9964() const { return guest::LoadFloat(base, address + 9964); }
  void set_unknown_9964(float v) const { guest::StoreFloat(base, address + 9964, v); }
  uint32_t unknown_9964_address() const { return address + 9964; }
  float unknown_9980() const { return guest::LoadFloat(base, address + 9980); }
  void set_unknown_9980(float v) const { guest::StoreFloat(base, address + 9980, v); }
  uint32_t unknown_9980_address() const { return address + 9980; }
  float unknown_10748() const { return guest::LoadFloat(base, address + 10748); }
  void set_unknown_10748(float v) const { guest::StoreFloat(base, address + 10748, v); }
  uint32_t unknown_10748_address() const { return address + 10748; }
  uint32_t unknown_11856() const { return guest::LoadU32(base, address + 11856); }
  void set_unknown_11856(uint32_t v) const { guest::StoreU32(base, address + 11856, v); }
  uint32_t unknown_11856_address() const { return address + 11856; }
  uint32_t unknown_11872() const { return guest::LoadU32(base, address + 11872); }
  void set_unknown_11872(uint32_t v) const { guest::StoreU32(base, address + 11872, v); }
  uint32_t unknown_11872_address() const { return address + 11872; }
  uint32_t unknown_11876() const { return guest::LoadU32(base, address + 11876); }
  void set_unknown_11876(uint32_t v) const { guest::StoreU32(base, address + 11876, v); }
  uint32_t unknown_11876_address() const { return address + 11876; }
  float unknown_11900() const { return guest::LoadFloat(base, address + 11900); }
  void set_unknown_11900(float v) const { guest::StoreFloat(base, address + 11900, v); }
  uint32_t unknown_11900_address() const { return address + 11900; }
  float unknown_11916() const { return guest::LoadFloat(base, address + 11916); }
  void set_unknown_11916(float v) const { guest::StoreFloat(base, address + 11916, v); }
  uint32_t unknown_11916_address() const { return address + 11916; }
  float unknown_11932() const { return guest::LoadFloat(base, address + 11932); }
  void set_unknown_11932(float v) const { guest::StoreFloat(base, address + 11932, v); }
  uint32_t unknown_11932_address() const { return address + 11932; }
  float unknown_11948() const { return guest::LoadFloat(base, address + 11948); }
  void set_unknown_11948(float v) const { guest::StoreFloat(base, address + 11948, v); }
  uint32_t unknown_11948_address() const { return address + 11948; }
  float unknown_11964() const { return guest::LoadFloat(base, address + 11964); }
  void set_unknown_11964(float v) const { guest::StoreFloat(base, address + 11964, v); }
  uint32_t unknown_11964_address() const { return address + 11964; }
  float unknown_11980() const { return guest::LoadFloat(base, address + 11980); }
  void set_unknown_11980(float v) const { guest::StoreFloat(base, address + 11980, v); }
  uint32_t unknown_11980_address() const { return address + 11980; }
  float unknown_11996() const { return guest::LoadFloat(base, address + 11996); }
  void set_unknown_11996(float v) const { guest::StoreFloat(base, address + 11996, v); }
  uint32_t unknown_11996_address() const { return address + 11996; }
  float unknown_12012() const { return guest::LoadFloat(base, address + 12012); }
  void set_unknown_12012(float v) const { guest::StoreFloat(base, address + 12012, v); }
  uint32_t unknown_12012_address() const { return address + 12012; }
  float unknown_12028() const { return guest::LoadFloat(base, address + 12028); }
  void set_unknown_12028(float v) const { guest::StoreFloat(base, address + 12028, v); }
  uint32_t unknown_12028_address() const { return address + 12028; }
  float unknown_12044() const { return guest::LoadFloat(base, address + 12044); }
  void set_unknown_12044(float v) const { guest::StoreFloat(base, address + 12044, v); }
  uint32_t unknown_12044_address() const { return address + 12044; }
  float unknown_12060() const { return guest::LoadFloat(base, address + 12060); }
  void set_unknown_12060(float v) const { guest::StoreFloat(base, address + 12060, v); }
  uint32_t unknown_12060_address() const { return address + 12060; }
  float unknown_12076() const { return guest::LoadFloat(base, address + 12076); }
  void set_unknown_12076(float v) const { guest::StoreFloat(base, address + 12076, v); }
  uint32_t unknown_12076_address() const { return address + 12076; }
  float unknown_12092() const { return guest::LoadFloat(base, address + 12092); }
  void set_unknown_12092(float v) const { guest::StoreFloat(base, address + 12092, v); }
  uint32_t unknown_12092_address() const { return address + 12092; }
  float unknown_12108() const { return guest::LoadFloat(base, address + 12108); }
  void set_unknown_12108(float v) const { guest::StoreFloat(base, address + 12108, v); }
  uint32_t unknown_12108_address() const { return address + 12108; }
  float unknown_12140() const { return guest::LoadFloat(base, address + 12140); }
  void set_unknown_12140(float v) const { guest::StoreFloat(base, address + 12140, v); }
  uint32_t unknown_12140_address() const { return address + 12140; }
  float unknown_12156() const { return guest::LoadFloat(base, address + 12156); }
  void set_unknown_12156(float v) const { guest::StoreFloat(base, address + 12156, v); }
  uint32_t unknown_12156_address() const { return address + 12156; }
  float unknown_12172() const { return guest::LoadFloat(base, address + 12172); }
  void set_unknown_12172(float v) const { guest::StoreFloat(base, address + 12172, v); }
  uint32_t unknown_12172_address() const { return address + 12172; }
  float unknown_12188() const { return guest::LoadFloat(base, address + 12188); }
  void set_unknown_12188(float v) const { guest::StoreFloat(base, address + 12188, v); }
  uint32_t unknown_12188_address() const { return address + 12188; }
  float unknown_12956() const { return guest::LoadFloat(base, address + 12956); }
  void set_unknown_12956(float v) const { guest::StoreFloat(base, address + 12956, v); }
  uint32_t unknown_12956_address() const { return address + 12956; }
  uint32_t unknown_12976() const { return guest::LoadU32(base, address + 12976); }
  void set_unknown_12976(uint32_t v) const { guest::StoreU32(base, address + 12976, v); }
  uint32_t unknown_12976_address() const { return address + 12976; }
  uint32_t unknown_12992() const { return guest::LoadU32(base, address + 12992); }
  void set_unknown_12992(uint32_t v) const { guest::StoreU32(base, address + 12992, v); }
  uint32_t unknown_12992_address() const { return address + 12992; }
  uint32_t unknown_12996() const { return guest::LoadU32(base, address + 12996); }
  void set_unknown_12996(uint32_t v) const { guest::StoreU32(base, address + 12996, v); }
  uint32_t unknown_12996_address() const { return address + 12996; }
  float unknown_13020() const { return guest::LoadFloat(base, address + 13020); }
  void set_unknown_13020(float v) const { guest::StoreFloat(base, address + 13020, v); }
  uint32_t unknown_13020_address() const { return address + 13020; }
  float unknown_13036() const { return guest::LoadFloat(base, address + 13036); }
  void set_unknown_13036(float v) const { guest::StoreFloat(base, address + 13036, v); }
  uint32_t unknown_13036_address() const { return address + 13036; }
  float unknown_13052() const { return guest::LoadFloat(base, address + 13052); }
  void set_unknown_13052(float v) const { guest::StoreFloat(base, address + 13052, v); }
  uint32_t unknown_13052_address() const { return address + 13052; }
  float unknown_13068() const { return guest::LoadFloat(base, address + 13068); }
  void set_unknown_13068(float v) const { guest::StoreFloat(base, address + 13068, v); }
  uint32_t unknown_13068_address() const { return address + 13068; }
  float unknown_13084() const { return guest::LoadFloat(base, address + 13084); }
  void set_unknown_13084(float v) const { guest::StoreFloat(base, address + 13084, v); }
  uint32_t unknown_13084_address() const { return address + 13084; }
  float unknown_13100() const { return guest::LoadFloat(base, address + 13100); }
  void set_unknown_13100(float v) const { guest::StoreFloat(base, address + 13100, v); }
  uint32_t unknown_13100_address() const { return address + 13100; }
  float unknown_13116() const { return guest::LoadFloat(base, address + 13116); }
  void set_unknown_13116(float v) const { guest::StoreFloat(base, address + 13116, v); }
  uint32_t unknown_13116_address() const { return address + 13116; }
  float unknown_13132() const { return guest::LoadFloat(base, address + 13132); }
  void set_unknown_13132(float v) const { guest::StoreFloat(base, address + 13132, v); }
  uint32_t unknown_13132_address() const { return address + 13132; }
  float unknown_13148() const { return guest::LoadFloat(base, address + 13148); }
  void set_unknown_13148(float v) const { guest::StoreFloat(base, address + 13148, v); }
  uint32_t unknown_13148_address() const { return address + 13148; }
  float unknown_13164() const { return guest::LoadFloat(base, address + 13164); }
  void set_unknown_13164(float v) const { guest::StoreFloat(base, address + 13164, v); }
  uint32_t unknown_13164_address() const { return address + 13164; }
  float unknown_13180() const { return guest::LoadFloat(base, address + 13180); }
  void set_unknown_13180(float v) const { guest::StoreFloat(base, address + 13180, v); }
  uint32_t unknown_13180_address() const { return address + 13180; }
  float unknown_13196() const { return guest::LoadFloat(base, address + 13196); }
  void set_unknown_13196(float v) const { guest::StoreFloat(base, address + 13196, v); }
  uint32_t unknown_13196_address() const { return address + 13196; }
  float unknown_13212() const { return guest::LoadFloat(base, address + 13212); }
  void set_unknown_13212(float v) const { guest::StoreFloat(base, address + 13212, v); }
  uint32_t unknown_13212_address() const { return address + 13212; }
  float unknown_13228() const { return guest::LoadFloat(base, address + 13228); }
  void set_unknown_13228(float v) const { guest::StoreFloat(base, address + 13228, v); }
  uint32_t unknown_13228_address() const { return address + 13228; }
  float unknown_13260() const { return guest::LoadFloat(base, address + 13260); }
  void set_unknown_13260(float v) const { guest::StoreFloat(base, address + 13260, v); }
  uint32_t unknown_13260_address() const { return address + 13260; }
  float unknown_13276() const { return guest::LoadFloat(base, address + 13276); }
  void set_unknown_13276(float v) const { guest::StoreFloat(base, address + 13276, v); }
  uint32_t unknown_13276_address() const { return address + 13276; }
  float unknown_13292() const { return guest::LoadFloat(base, address + 13292); }
  void set_unknown_13292(float v) const { guest::StoreFloat(base, address + 13292, v); }
  uint32_t unknown_13292_address() const { return address + 13292; }
  float unknown_13308() const { return guest::LoadFloat(base, address + 13308); }
  void set_unknown_13308(float v) const { guest::StoreFloat(base, address + 13308, v); }
  uint32_t unknown_13308_address() const { return address + 13308; }
  float unknown_13340() const { return guest::LoadFloat(base, address + 13340); }
  void set_unknown_13340(float v) const { guest::StoreFloat(base, address + 13340, v); }
  uint32_t unknown_13340_address() const { return address + 13340; }
  float unknown_69628() const { return guest::LoadFloat(base, address + 69628); }
  void set_unknown_69628(float v) const { guest::StoreFloat(base, address + 69628, v); }
  uint32_t unknown_69628_address() const { return address + 69628; }
  uint32_t unknown_77376() const { return guest::LoadU32(base, address + 77376); }
  void set_unknown_77376(uint32_t v) const { guest::StoreU32(base, address + 77376, v); }
  uint32_t unknown_77376_address() const { return address + 77376; }
  uint32_t unknown_77380() const { return guest::LoadU32(base, address + 77380); }
  void set_unknown_77380(uint32_t v) const { guest::StoreU32(base, address + 77380, v); }
  uint32_t unknown_77380_address() const { return address + 77380; }
  uint32_t unknown_77384() const { return guest::LoadU32(base, address + 77384); }
  void set_unknown_77384(uint32_t v) const { guest::StoreU32(base, address + 77384, v); }
  uint32_t unknown_77384_address() const { return address + 77384; }
  uint32_t unknown_77388() const { return guest::LoadU32(base, address + 77388); }
  void set_unknown_77388(uint32_t v) const { guest::StoreU32(base, address + 77388, v); }
  uint32_t unknown_77388_address() const { return address + 77388; }
};

// embedded; measure: RivalLogo@+20
struct RivalLogoAt20_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(base, address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(base, address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
};

// embedded; measure: TvCamera@+43016
struct CameraPathKeyAt43016 : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
};

// embedded; measure: TvCamera@+43136
struct ChaseCameraAt43136 : View {
  using View::View;
  static constexpr uint32_t kSize = 380;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  float unknown_44() const { return guest::LoadFloat(base, address + 44); }
  void set_unknown_44(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  float unknown_60() const { return guest::LoadFloat(base, address + 60); }
  void set_unknown_60(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  float unknown_76() const { return guest::LoadFloat(base, address + 76); }
  void set_unknown_76(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  float unknown_92() const { return guest::LoadFloat(base, address + 92); }
  void set_unknown_92(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  float unknown_108() const { return guest::LoadFloat(base, address + 108); }
  void set_unknown_108(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  float unknown_124() const { return guest::LoadFloat(base, address + 124); }
  void set_unknown_124(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  float unknown_140() const { return guest::LoadFloat(base, address + 140); }
  void set_unknown_140(float v) const { guest::StoreFloat(base, address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  float unknown_156() const { return guest::LoadFloat(base, address + 156); }
  void set_unknown_156(float v) const { guest::StoreFloat(base, address + 156, v); }
  uint32_t unknown_156_address() const { return address + 156; }
  float unknown_172() const { return guest::LoadFloat(base, address + 172); }
  void set_unknown_172(float v) const { guest::StoreFloat(base, address + 172, v); }
  uint32_t unknown_172_address() const { return address + 172; }
  float unknown_188() const { return guest::LoadFloat(base, address + 188); }
  void set_unknown_188(float v) const { guest::StoreFloat(base, address + 188, v); }
  uint32_t unknown_188_address() const { return address + 188; }
  float unknown_204() const { return guest::LoadFloat(base, address + 204); }
  void set_unknown_204(float v) const { guest::StoreFloat(base, address + 204, v); }
  uint32_t unknown_204_address() const { return address + 204; }
  float unknown_220() const { return guest::LoadFloat(base, address + 220); }
  void set_unknown_220(float v) const { guest::StoreFloat(base, address + 220, v); }
  uint32_t unknown_220_address() const { return address + 220; }
  float unknown_224() const { return guest::LoadFloat(base, address + 224); }
  void set_unknown_224(float v) const { guest::StoreFloat(base, address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  uint64_t unknown_224_8i() const { return guest::LoadU64(base, address + 224); }
  void set_unknown_224_8i(uint64_t v) const { guest::StoreU64(base, address + 224, v); }
  uint32_t unknown_224_8i_address() const { return address + 224; }
  float unknown_228() const { return guest::LoadFloat(base, address + 228); }
  void set_unknown_228(float v) const { guest::StoreFloat(base, address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  float unknown_232() const { return guest::LoadFloat(base, address + 232); }
  void set_unknown_232(float v) const { guest::StoreFloat(base, address + 232, v); }
  uint32_t unknown_232_address() const { return address + 232; }
  uint64_t unknown_232_8i() const { return guest::LoadU64(base, address + 232); }
  void set_unknown_232_8i(uint64_t v) const { guest::StoreU64(base, address + 232, v); }
  uint32_t unknown_232_8i_address() const { return address + 232; }
  float unknown_236() const { return guest::LoadFloat(base, address + 236); }
  void set_unknown_236(float v) const { guest::StoreFloat(base, address + 236, v); }
  uint32_t unknown_236_address() const { return address + 236; }
  float unknown_240() const { return guest::LoadFloat(base, address + 240); }
  void set_unknown_240(float v) const { guest::StoreFloat(base, address + 240, v); }
  uint32_t unknown_240_address() const { return address + 240; }
  uint64_t unknown_240_8i() const { return guest::LoadU64(base, address + 240); }
  void set_unknown_240_8i(uint64_t v) const { guest::StoreU64(base, address + 240, v); }
  uint32_t unknown_240_8i_address() const { return address + 240; }
  float unknown_244() const { return guest::LoadFloat(base, address + 244); }
  void set_unknown_244(float v) const { guest::StoreFloat(base, address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  float unknown_248() const { return guest::LoadFloat(base, address + 248); }
  void set_unknown_248(float v) const { guest::StoreFloat(base, address + 248, v); }
  uint32_t unknown_248_address() const { return address + 248; }
  uint64_t unknown_248_8i() const { return guest::LoadU64(base, address + 248); }
  void set_unknown_248_8i(uint64_t v) const { guest::StoreU64(base, address + 248, v); }
  uint32_t unknown_248_8i_address() const { return address + 248; }
  float unknown_252() const { return guest::LoadFloat(base, address + 252); }
  void set_unknown_252(float v) const { guest::StoreFloat(base, address + 252, v); }
  uint32_t unknown_252_address() const { return address + 252; }
  uint16_t unknown_260() const { return guest::LoadU16(base, address + 260); }
  void set_unknown_260(uint16_t v) const { guest::StoreU16(base, address + 260, v); }
  uint32_t unknown_260_address() const { return address + 260; }
  uint16_t unknown_262() const { return guest::LoadU16(base, address + 262); }
  void set_unknown_262(uint16_t v) const { guest::StoreU16(base, address + 262, v); }
  uint32_t unknown_262_address() const { return address + 262; }
  float unknown_284() const { return guest::LoadFloat(base, address + 284); }
  void set_unknown_284(float v) const { guest::StoreFloat(base, address + 284, v); }
  uint32_t unknown_284_address() const { return address + 284; }
  float unknown_300() const { return guest::LoadFloat(base, address + 300); }
  void set_unknown_300(float v) const { guest::StoreFloat(base, address + 300, v); }
  uint32_t unknown_300_address() const { return address + 300; }
  float unknown_316() const { return guest::LoadFloat(base, address + 316); }
  void set_unknown_316(float v) const { guest::StoreFloat(base, address + 316, v); }
  uint32_t unknown_316_address() const { return address + 316; }
  float unknown_332() const { return guest::LoadFloat(base, address + 332); }
  void set_unknown_332(float v) const { guest::StoreFloat(base, address + 332, v); }
  uint32_t unknown_332_address() const { return address + 332; }
  float unknown_352() const { return guest::LoadFloat(base, address + 352); }
  void set_unknown_352(float v) const { guest::StoreFloat(base, address + 352, v); }
  uint32_t unknown_352_address() const { return address + 352; }
  float unknown_356() const { return guest::LoadFloat(base, address + 356); }
  void set_unknown_356(float v) const { guest::StoreFloat(base, address + 356, v); }
  uint32_t unknown_356_address() const { return address + 356; }
  float unknown_360() const { return guest::LoadFloat(base, address + 360); }
  void set_unknown_360(float v) const { guest::StoreFloat(base, address + 360, v); }
  uint32_t unknown_360_address() const { return address + 360; }
  float unknown_364() const { return guest::LoadFloat(base, address + 364); }
  void set_unknown_364(float v) const { guest::StoreFloat(base, address + 364, v); }
  uint32_t unknown_364_address() const { return address + 364; }
  float unknown_368() const { return guest::LoadFloat(base, address + 368); }
  void set_unknown_368(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t unknown_368_address() const { return address + 368; }
  uint32_t unknown_372() const { return guest::LoadU32(base, address + 372); }
  void set_unknown_372(uint32_t v) const { guest::StoreU32(base, address + 372, v); }
  uint32_t unknown_372_address() const { return address + 372; }
  uint16_t unknown_376() const { return guest::LoadU16(base, address + 376); }
  void set_unknown_376(uint16_t v) const { guest::StoreU16(base, address + 376, v); }
  uint32_t unknown_376_address() const { return address + 376; }
  uint16_t unknown_378() const { return guest::LoadU16(base, address + 378); }
  void set_unknown_378(uint16_t v) const { guest::StoreU16(base, address + 378, v); }
  uint32_t unknown_378_address() const { return address + 378; }
};

// embedded; measure: TvCamera@+43136.+20
struct ViewportPart : View {
  using View::View;
  static constexpr uint32_t kSize = 1872;
  float unknown_1600() const { return guest::LoadFloat(base, address + 1600); }
  void set_unknown_1600(float v) const { guest::StoreFloat(base, address + 1600, v); }
  uint32_t unknown_1600_address() const { return address + 1600; }
  float unknown_1604() const { return guest::LoadFloat(base, address + 1604); }
  void set_unknown_1604(float v) const { guest::StoreFloat(base, address + 1604, v); }
  uint32_t unknown_1604_address() const { return address + 1604; }
  float unknown_1608() const { return guest::LoadFloat(base, address + 1608); }
  void set_unknown_1608(float v) const { guest::StoreFloat(base, address + 1608, v); }
  uint32_t unknown_1608_address() const { return address + 1608; }
  float unknown_1612() const { return guest::LoadFloat(base, address + 1612); }
  void set_unknown_1612(float v) const { guest::StoreFloat(base, address + 1612, v); }
  uint32_t unknown_1612_address() const { return address + 1612; }
  float unknown_1664() const { return guest::LoadFloat(base, address + 1664); }
  void set_unknown_1664(float v) const { guest::StoreFloat(base, address + 1664, v); }
  uint32_t unknown_1664_address() const { return address + 1664; }
  float unknown_1668() const { return guest::LoadFloat(base, address + 1668); }
  void set_unknown_1668(float v) const { guest::StoreFloat(base, address + 1668, v); }
  uint32_t unknown_1668_address() const { return address + 1668; }
  float unknown_1672() const { return guest::LoadFloat(base, address + 1672); }
  void set_unknown_1672(float v) const { guest::StoreFloat(base, address + 1672, v); }
  uint32_t unknown_1672_address() const { return address + 1672; }
  float unknown_1676() const { return guest::LoadFloat(base, address + 1676); }
  void set_unknown_1676(float v) const { guest::StoreFloat(base, address + 1676, v); }
  uint32_t unknown_1676_address() const { return address + 1676; }
  float unknown_1680() const { return guest::LoadFloat(base, address + 1680); }
  void set_unknown_1680(float v) const { guest::StoreFloat(base, address + 1680, v); }
  uint32_t unknown_1680_address() const { return address + 1680; }
  float unknown_1684() const { return guest::LoadFloat(base, address + 1684); }
  void set_unknown_1684(float v) const { guest::StoreFloat(base, address + 1684, v); }
  uint32_t unknown_1684_address() const { return address + 1684; }
  float unknown_1688() const { return guest::LoadFloat(base, address + 1688); }
  void set_unknown_1688(float v) const { guest::StoreFloat(base, address + 1688, v); }
  uint32_t unknown_1688_address() const { return address + 1688; }
  float unknown_1692() const { return guest::LoadFloat(base, address + 1692); }
  void set_unknown_1692(float v) const { guest::StoreFloat(base, address + 1692, v); }
  uint32_t unknown_1692_address() const { return address + 1692; }
  float unknown_1696() const { return guest::LoadFloat(base, address + 1696); }
  void set_unknown_1696(float v) const { guest::StoreFloat(base, address + 1696, v); }
  uint32_t unknown_1696_address() const { return address + 1696; }
  float unknown_1700() const { return guest::LoadFloat(base, address + 1700); }
  void set_unknown_1700(float v) const { guest::StoreFloat(base, address + 1700, v); }
  uint32_t unknown_1700_address() const { return address + 1700; }
  float unknown_1704() const { return guest::LoadFloat(base, address + 1704); }
  void set_unknown_1704(float v) const { guest::StoreFloat(base, address + 1704, v); }
  uint32_t unknown_1704_address() const { return address + 1704; }
  float unknown_1708() const { return guest::LoadFloat(base, address + 1708); }
  void set_unknown_1708(float v) const { guest::StoreFloat(base, address + 1708, v); }
  uint32_t unknown_1708_address() const { return address + 1708; }
  float unknown_1712() const { return guest::LoadFloat(base, address + 1712); }
  void set_unknown_1712(float v) const { guest::StoreFloat(base, address + 1712, v); }
  uint32_t unknown_1712_address() const { return address + 1712; }
  float unknown_1716() const { return guest::LoadFloat(base, address + 1716); }
  void set_unknown_1716(float v) const { guest::StoreFloat(base, address + 1716, v); }
  uint32_t unknown_1716_address() const { return address + 1716; }
  float unknown_1720() const { return guest::LoadFloat(base, address + 1720); }
  void set_unknown_1720(float v) const { guest::StoreFloat(base, address + 1720, v); }
  uint32_t unknown_1720_address() const { return address + 1720; }
  float unknown_1724() const { return guest::LoadFloat(base, address + 1724); }
  void set_unknown_1724(float v) const { guest::StoreFloat(base, address + 1724, v); }
  uint32_t unknown_1724_address() const { return address + 1724; }
  float unknown_1744() const { return guest::LoadFloat(base, address + 1744); }
  void set_unknown_1744(float v) const { guest::StoreFloat(base, address + 1744, v); }
  uint32_t unknown_1744_address() const { return address + 1744; }
  float unknown_1748() const { return guest::LoadFloat(base, address + 1748); }
  void set_unknown_1748(float v) const { guest::StoreFloat(base, address + 1748, v); }
  uint32_t unknown_1748_address() const { return address + 1748; }
  float unknown_1752() const { return guest::LoadFloat(base, address + 1752); }
  void set_unknown_1752(float v) const { guest::StoreFloat(base, address + 1752, v); }
  uint32_t unknown_1752_address() const { return address + 1752; }
  float unknown_1756() const { return guest::LoadFloat(base, address + 1756); }
  void set_unknown_1756(float v) const { guest::StoreFloat(base, address + 1756, v); }
  uint32_t unknown_1756_address() const { return address + 1756; }
  float unknown_1864() const { return guest::LoadFloat(base, address + 1864); }
  void set_unknown_1864(float v) const { guest::StoreFloat(base, address + 1864, v); }
  uint32_t unknown_1864_address() const { return address + 1864; }
  float unknown_1868() const { return guest::LoadFloat(base, address + 1868); }
  void set_unknown_1868(float v) const { guest::StoreFloat(base, address + 1868, v); }
  uint32_t unknown_1868_address() const { return address + 1868; }
};

// The image words docs/functions/camera-modes.md's code uses.
struct CameraModesGlobals {
  // the two viewports
  template <typename T = Viewports>
  static T viewports(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2CE8u)); }
  static void set_viewports(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2CE8u, v.address); }
  static void set_viewports(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // the race data (a pointer)
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the course's route (its file)
  static uint32_t route(uint8_t* base) { return guest::LoadU32(base, 0x826C5B10u); }
  static void set_route(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B10u, v); }
  static constexpr uint32_t route_address() { return 0x826C5B10u; }
  // the car input records by player (20 bytes each)
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // [0x826C7820 + 4]: 0 for player 2 followed
  static uint32_t two_player(uint8_t* base) { return guest::LoadU32(base, 0x826C7824u); }
  static void set_two_player(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7824u, v); }
  static constexpr uint32_t two_player_address() { return 0x826C7824u; }
  // the camera animations' table: 256 264-byte CameraAnimEntries
  static constexpr uint32_t anims_address() { return 0x826C79B8u; }
  // its count
  static uint32_t anim_count(uint8_t* base) { return guest::LoadU32(base, 0x826D81B8u); }
  static void set_anim_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D81B8u, v); }
  static constexpr uint32_t anim_count_address() { return 0x826D81B8u; }
  // Camera::ShakeRandom's step
  static float shake_step(uint8_t* base) { return guest::LoadFloat(base, 0x826D81C0u); }
  static void set_shake_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x826D81C0u, v); }
  static constexpr uint32_t shake_step_address() { return 0x826D81C0u; }
  // its guard (bit 1)
  static uint32_t shake_guard(uint8_t* base) { return guest::LoadU32(base, 0x826D81C4u); }
  static void set_shake_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D81C4u, v); }
  static constexpr uint32_t shake_guard_address() { return 0x826D81C4u; }
  // MakeShot's static (0.75, 1.8, 0, 1), probably
  static constexpr uint32_t intro_offset_address() { return 0x826D81D0u; }
  // MakeShot's static (0, 1, 0, 1)
  static constexpr uint32_t intro_up_address() { return 0x826D81E0u; }
  // their guard (bits 1 and 2)
  static uint32_t intro_guard(uint8_t* base) { return guest::LoadU32(base, 0x826D81F0u); }
  static void set_intro_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D81F0u, v); }
  static constexpr uint32_t intro_guard_address() { return 0x826D81F0u; }
  // ChaseCamera's static look offset
  static constexpr uint32_t chase_look_address() { return 0x826D8200u; }
  // ChaseCamera's static (0, 1, 0, 1)
  static constexpr uint32_t chase_up_address() { return 0x826D8210u; }
  // their guard
  static uint32_t chase_guard(uint8_t* base) { return guest::LoadU32(base, 0x826D8220u); }
  static void set_chase_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8220u, v); }
  static constexpr uint32_t chase_guard_address() { return 0x826D8220u; }
  // ChaseCamera::Update's pull back
  static float pull_back(uint8_t* base) { return guest::LoadFloat(base, 0x826D8224u); }
  static void set_pull_back(uint8_t* base, float v) { guest::StoreFloat(base, 0x826D8224u, v); }
  static constexpr uint32_t pull_back_address() { return 0x826D8224u; }
  // the speed it last saw
  static float last_speed(uint8_t* base) { return guest::LoadFloat(base, 0x826D8228u); }
  static void set_last_speed(uint8_t* base, float v) { guest::StoreFloat(base, 0x826D8228u, v); }
  static constexpr uint32_t last_speed_address() { return 0x826D8228u; }
  // ViewCamera1's four statics (0x826D8230 to 0x826D826F)
  static constexpr uint32_t view1_statics_address() { return 0x826D8230u; }
  // their guard (bits 1 to 8)
  static uint32_t view1_guard(uint8_t* base) { return guest::LoadU32(base, 0x826D8270u); }
  static void set_view1_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8270u, v); }
  static constexpr uint32_t view1_guard_address() { return 0x826D8270u; }
  // ViewCamera2's four statics (0x826D8280 to 0x826D82BF)
  static constexpr uint32_t view2_statics_address() { return 0x826D8280u; }
  // their guard
  static uint32_t view2_guard(uint8_t* base) { return guest::LoadU32(base, 0x826D82C0u); }
  static void set_view2_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82C0u, v); }
  static constexpr uint32_t view2_guard_address() { return 0x826D82C0u; }
  // the frame the intro's shots ended at
  static uint32_t intro_kept(uint8_t* base) { return guest::LoadU32(base, 0x826D82C4u); }
  static void set_intro_kept(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82C4u, v); }
  static constexpr uint32_t intro_kept_address() { return 0x826D82C4u; }
  // the attract race's color filter style (a word)
  static constexpr uint32_t filter_style_address() { return 0x826D82C8u; }
  // its guard (bit 1)
  static uint32_t filter_guard(uint8_t* base) { return guest::LoadU32(base, 0x826D82CCu); }
  static void set_filter_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82CCu, v); }
  static constexpr uint32_t filter_guard_address() { return 0x826D82CCu; }
  // the racers' manager (a pointer)
  static uint32_t racers_manager(uint8_t* base) { return guest::LoadU32(base, 0x826D82F4u); }
  static void set_racers_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
  // TrackRoute::SplinePoint's Catmull-Rom basis (64 bytes)
  static constexpr uint32_t spline_matrix_address() { return 0x826C2D30u; }
  // its guard (bit 1)
  static uint32_t spline_guard(uint8_t* base) { return guest::LoadU32(base, 0x826C2D70u); }
  static void set_spline_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2D70u, v); }
  static constexpr uint32_t spline_guard_address() { return 0x826C2D70u; }
  // the camera paths' records by index (pointers)
  static constexpr uint32_t path_assets_address() { return 0x82618D30u; }
  // set: the intro's frame counts held
  static uint32_t paused(uint8_t* base) { return guest::LoadU32(base, 0x826A50D4u); }
  static void set_paused(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50D4u, v); }
  static constexpr uint32_t paused_address() { return 0x826A50D4u; }
  // ViewCamera1's eye offsets by model (32 bytes each)
  static constexpr uint32_t view1_offsets_address() { return 0x82618DC0u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // 2.0
  static float two(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_two(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t two_address() { return 0x820021E8u; }
  static float two_pi(uint8_t* base) { return guest::LoadFloat(base, 0x82001FA8u); }
  static void set_two_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FA8u, v); }
  static constexpr uint32_t two_pi_address() { return 0x82001FA8u; }
  // 1 / (2 pi)
  static float over_two_pi(uint8_t* base) { return guest::LoadFloat(base, 0x82001FA4u); }
  static void set_over_two_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FA4u, v); }
  static constexpr uint32_t over_two_pi_address() { return 0x82001FA4u; }
  // pi / 180
  static float radians_per_degree(uint8_t* base) { return guest::LoadFloat(base, 0x82002948u); }
  static void set_radians_per_degree(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002948u, v); }
  static constexpr uint32_t radians_per_degree_address() { return 0x82002948u; }
  // CameraAnim::EvalCamera's length scale
  static float film_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002D0Cu); }
  static void set_film_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002D0Cu, v); }
  static constexpr uint32_t film_scale_address() { return 0x82002D0Cu; }
  // CameraAnim::EvalCamera's zoom numerator
  static float zoom_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002C30u); }
  static void set_zoom_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C30u, v); }
  static constexpr uint32_t zoom_scale_address() { return 0x82002C30u; }
  // CameraAnim::Matrix's first RotateY
  static float first_turn(uint8_t* base) { return guest::LoadFloat(base, 0x82001FB4u); }
  static void set_first_turn(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FB4u, v); }
  static constexpr uint32_t first_turn_address() { return 0x82001FB4u; }
  // the z flip (-1.0, probably)
  static float flip(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_flip(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t flip_address() { return 0x82001BE0u; }
  // the cameras' zoom angle (55.0)
  static float chase_degrees(uint8_t* base) { return guest::LoadFloat(base, 0x82009300u); }
  static void set_chase_degrees(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009300u, v); }
  static constexpr uint32_t chase_degrees_address() { return 0x82009300u; }
  // ChaseCamera::Follow's (55.0)
  static float follow_degrees(uint8_t* base) { return guest::LoadFloat(base, 0x82000D44u); }
  static void set_follow_degrees(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000D44u, v); }
  static constexpr uint32_t follow_degrees_address() { return 0x82000D44u; }
  // the rival logo's textures by index; its 12-byte records 304 bytes before
  static constexpr uint32_t logo_textures_address() { return 0x8200CEA0u; }
};

}  // namespace schema
