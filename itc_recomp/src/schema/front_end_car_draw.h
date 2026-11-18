// The views of docs/functions/front-end-car-draw.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/front-end-car-draw.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: GarageAnims.+12
struct GarageAnimValues : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  // stage +32: opened and closed by types 2 and 3 (probably the bonnet; the scene model's +4928)
  float bonnet() const { return guest::LoadFloat(base, address + 0); }
  void set_bonnet(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t bonnet_address() const { return address + 0; }
  // stage +52: opened and closed by types 4 and 5 (the doors' runners, probably)
  float doors() const { return guest::LoadFloat(base, address + 20); }
  void set_doors(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t doors_address() const { return address + 20; }
  // stage +56: the second door value (OpenDoorsB, CloseDoorsB)
  float doors_b() const { return guest::LoadFloat(base, address + 24); }
  void set_doors_b(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t doors_b_address() const { return address + 24; }
  // stage +80: the car's first angle (probably about x; GarageStage::StepAngles' 13 clears it, a preset's +48)
  float angle_x() const { return guest::LoadFloat(base, address + 48); }
  void set_angle_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t angle_x_address() const { return address + 48; }
  // stage +84: the car's yaw (turned 0.5 degrees a step)
  float yaw() const { return guest::LoadFloat(base, address + 52); }
  void set_yaw(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t yaw_address() const { return address + 52; }
  // stage +88: the car's third angle (probably about z; StepAngles' 11 and 12)
  float angle_z() const { return guest::LoadFloat(base, address + 56); }
  void set_angle_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t angle_z_address() const { return address + 56; }
  // stage +96: a word OpenDoorsB and CloseDoorsB set
  uint32_t doors_word() const { return guest::LoadU32(base, address + 64); }
  void set_doors_word(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t doors_word_address() const { return address + 64; }
};

// embedded; measure: GarageAnims
struct GarageAnimList : View {
  using View::View;
  static constexpr uint32_t kSize = 452;
  // entry 0's type: 0 turn, 1 cancel, 2 open, 3 close, 4 open the doors, 5 close them, 6 and 7 the camera
  int32_t type() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_type(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t type_address() const { return address + 0; }
  // entry 0's steps (60.0 or 20.0)
  float steps() const { return guest::LoadFloat(base, address + 4); }
  void set_steps(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t steps_address() const { return address + 4; }
  // entry 0's word (1: turn until cancelled)
  uint32_t word() const { return guest::LoadU32(base, address + 8); }
  void set_word(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t word_address() const { return address + 8; }
  // entry 0's value's address
  template <typename T = GarageAnimValues>
  T value() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_value(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t value_address() const { return address + 12; }
  // entry 0's scene model's index (the stage's +8)
  uint32_t model_index() const { return guest::LoadU32(base, address + 16); }
  void set_model_index(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t model_index_address() const { return address + 16; }
  // entry 0 finished
  uint32_t done() const { return guest::LoadU32(base, address + 20); }
  void set_done(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t done_address() const { return address + 20; }
  // entry 0's float: type 6's (360.0 - the camera's orbit angle) / steps, else 0.0
  float camera_step() const { return guest::LoadFloat(base, address + 24); }
  void set_camera_step(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t camera_step_address() const { return address + 24; }
  // entries 1 to 15 (GarageAnimEntry)
  uint32_t entries_1_to_15_address() const { return address + 28; }
  // the entries in use (stage +6048); no bound check on 16
  uint32_t count() const { return guest::LoadU32(base, address + 448); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t count_address() const { return address + 448; }
};

// embedded; measure: -
struct GarageAnimEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // 0 turn, 1 cancel, 2 open, 3 close, 4 open the doors, 5 close them, 6 and 7 the camera
  int32_t type() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_type(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t type_address() const { return address + 0; }
  float steps() const { return guest::LoadFloat(base, address + 4); }
  void set_steps(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t steps_address() const { return address + 4; }
  // 1: turn until cancelled
  uint32_t word() const { return guest::LoadU32(base, address + 8); }
  void set_word(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t word_address() const { return address + 8; }
  // the value stepped (stage + 32, or the camera at stage + 112)
  template <typename T = GarageAnimValues>
  T value() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_value(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t value_address() const { return address + 12; }
  // the scene model's index in pool 7
  uint32_t model_index() const { return guest::LoadU32(base, address + 16); }
  void set_model_index(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t model_index_address() const { return address + 16; }
  uint32_t done() const { return guest::LoadU32(base, address + 20); }
  void set_done(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t done_address() const { return address + 20; }
  // type 6's (360.0 - the camera's orbit angle) / steps, else 0.0
  float camera_step() const { return guest::LoadFloat(base, address + 24); }
  void set_camera_step(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t camera_step_address() const { return address + 24; }
};

// embedded; measure: Plates
struct FrontEndPlatePoints : View {
  using View::View;
  static constexpr uint32_t kSize = 4580;
  // the front bumper's own plate texture flag (PartsPoints' +2640)
  uint32_t front_plate_set() const { return guest::LoadU32(base, address + 2640); }
  void set_front_plate_set(uint32_t v) const { guest::StoreU32(base, address + 2640, v); }
  uint32_t front_plate_set_address() const { return address + 2640; }
  // the rear bumper's own plate texture flag (its group's +1680)
  uint32_t rear_plate_set() const { return guest::LoadU32(base, address + 4576); }
  void set_rear_plate_set(uint32_t v) const { guest::StoreU32(base, address + 4576, v); }
  uint32_t rear_plate_set_address() const { return address + 4576; }
};

// embedded; measure: TrafficCar.+5060
struct FrontEndSceneParts : View {
  using View::View;
  static constexpr uint32_t kSize = 2820;
  // the point block's (+176) +2640: the plates shown
  uint32_t front_plate_set() const { return guest::LoadU32(base, address + 2816); }
  void set_front_plate_set(uint32_t v) const { guest::StoreU32(base, address + 2816, v); }
  uint32_t front_plate_set_address() const { return address + 2816; }
};

// embedded; measure: Floor
struct FrontEndFloorScene : View {
  using View::View;
  static constexpr uint32_t kSize = 4576;
  // 0 the front end (set a), 1 the race and parking area (CarScene's +4)
  int32_t model_set() const { return int32_t(guest::LoadU32(base, address + 4)); }
  void set_model_set(int32_t v) const { guest::StoreU32(base, address + 4, uint32_t(v)); }
  uint32_t model_set_address() const { return address + 4; }
  // the part model at +4160's shape count (its +412): the shapes Floor::Draw draws
  int32_t floor_shape_count() const { return int32_t(guest::LoadU32(base, address + 4572)); }
  void set_floor_shape_count(int32_t v) const { guest::StoreU32(base, address + 4572, uint32_t(v)); }
  uint32_t floor_shape_count_address() const { return address + 4572; }
};

// embedded; measure: Floor::Draw.r4
struct FloorDrawMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the last row's x and y (set to 0.0, 0.0)
  uint64_t row3_xy() const { return guest::LoadU64(base, address + 48); }
  void set_row3_xy(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t row3_xy_address() const { return address + 48; }
  // the last row's z and w (set to 0.0, 1.0)
  uint64_t row3_zw() const { return guest::LoadU64(base, address + 56); }
  void set_row3_zw(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t row3_zw_address() const { return address + 56; }
};

// The image words docs/functions/front-end-car-draw.md's code uses.
struct FrontEndCarDrawGlobals {
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // -1.0 (the mirror's y)
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // the yaw's wrap
  static float pi(uint8_t* base) { return guest::LoadFloat(base, 0x82002240u); }
  static void set_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002240u, v); }
  static constexpr uint32_t pi_address() { return 0x82002240u; }
  // 360.0 (GarageAnims::Add's type 6)
  static float three_sixty(uint8_t* base) { return guest::LoadFloat(base, 0x82002244u); }
  static void set_three_sixty(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002244u, v); }
  static constexpr uint32_t three_sixty_address() { return 0x82002244u; }
  // 20.0: the camera animations' steps
  static float twenty(uint8_t* base) { return guest::LoadFloat(base, 0x82002568u); }
  static void set_twenty(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002568u, v); }
  static constexpr uint32_t twenty_address() { return 0x82002568u; }
  // pi/180
  static float degree(uint8_t* base) { return guest::LoadFloat(base, 0x82002948u); }
  static void set_degree(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002948u, v); }
  static constexpr uint32_t degree_address() { return 0x82002948u; }
  // the yaw's wrap
  static float minus_pi(uint8_t* base) { return guest::LoadFloat(base, 0x820029C4u); }
  static void set_minus_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029C4u, v); }
  static constexpr uint32_t minus_pi_address() { return 0x820029C4u; }
  // 60.0: the car animations' steps
  static float sixty(uint8_t* base) { return guest::LoadFloat(base, 0x82000F14u); }
  static void set_sixty(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000F14u, v); }
  static constexpr uint32_t sixty_address() { return 0x82000F14u; }
  // pi/360 (0.5 degrees): the yaw's step
  static float half_degree(uint8_t* base) { return guest::LoadFloat(base, 0x82009EA8u); }
  static void set_half_degree(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009EA8u, v); }
  static constexpr uint32_t half_degree_address() { return 0x82009EA8u; }
  // pi/3: the spots' cones' fourth word
  static float third_pi(uint8_t* base) { return guest::LoadFloat(base, 0x82009EB4u); }
  static void set_third_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009EB4u, v); }
  static constexpr uint32_t third_pi_address() { return 0x82009EB4u; }
  // 640.0: the garage camera's focal length over tan(fov)
  static float focal(uint8_t* base) { return guest::LoadFloat(base, 0x82009EB8u); }
  static void set_focal(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009EB8u, v); }
  static constexpr uint32_t focal_address() { return 0x82009EB8u; }
  // the per-car stand animation limits, 48 bytes a car by the scene model's +48: +0 the most GarageAnims::Open and Close take the value to (the first car's here)
  static constexpr uint32_t car_limits_address() { return 0x82635E90u; }
  // the cull swap: the cull modes 2 and 6 swapped while set (toggled around the mirrored car draw)
  static uint32_t mirrored(uint8_t* base) { return guest::LoadU32(base, 0x826C5100u); }
  static void set_mirrored(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5100u, v); }
  static constexpr uint32_t mirrored_address() { return 0x826C5100u; }
  // pixel c40's x, y, z (the stage lights' +1072)
  static constexpr uint32_t ambient_address() { return 0x8265ED00u; }
  // the scene lights' three vectors (the stage lights' +32 + 96i, y negated when mirrored)
  static constexpr uint32_t light_a_address() { return 0x8296B950u; }
  // three (the stage lights' +48 + 96i)
  static constexpr uint32_t light_b_address() { return 0x8296B980u; }
  // three (the stage lights' +64 + 96i)
  static constexpr uint32_t light_c_address() { return 0x8296B9B0u; }
  // the spots' positions, 4 of 8 set (q +0)
  static constexpr uint32_t spot_positions_address() { return 0x8296B9E0u; }
  // the spots' colors, probably (q +80)
  static constexpr uint32_t spot_colors_address() { return 0x8296BA60u; }
  // cleared to 0.0, then q +32
  static constexpr uint32_t spot_a_address() { return 0x8296BAE0u; }
  // cleared to 0.0, then q +48
  static constexpr uint32_t spot_b_address() { return 0x8296BB60u; }
  // the spots' directions, normalized
  static constexpr uint32_t spot_directions_address() { return 0x8296BBE0u; }
  // the spots' cones: cos(inner/2), cos(outer/2), 1 / their difference, pi/3
  static constexpr uint32_t spot_cones_address() { return 0x8296BC60u; }
};

}  // namespace schema
