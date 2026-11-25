// The views of docs/functions/car-physics.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-physics.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Vehicle, Vehicle::UpdateDrivenAxle.ret, Vehicle::Get.ret, AiDriver.+160, Vehicle.+2160, Vehicle.+2144, sub_820C0140.r3
struct Vehicle : View {
  using View::View;
  static constexpr uint32_t kSize = 6268;
  // the body matrix's row 0, w 0.0 (Vehicle::SyncFromBody); the car's forward axis is -row 0
  uint32_t body_row0_address() const { return address + 0; }
  // row 1 (up), w 0.0
  uint32_t body_row1_address() const { return address + 16; }
  // row 2, w 0.0
  uint32_t body_row2_address() const { return address + 32; }
  // the speed (m/s), the body's +160
  float speed() const { return guest::LoadFloat(address + 48); }
  void set_speed(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t speed_address() const { return address + 48; }
  float unknown_52() const { return guest::LoadFloat(address + 52); }
  void set_unknown_52(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  // only Vehicle::SetupFromCar's (the car data's +56: the measure's)
  float unknown_56() const { return guest::LoadFloat(address + 56); }
  void set_unknown_56(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  // the body's angular velocity
  uint32_t angular_velocity_address() const { return address + 64; }
  // set by Vehicle::ApplyForces' tip check
  float unknown_80() const { return guest::LoadFloat(address + 80); }
  void set_unknown_80(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  // set by Vehicle::ApplyForces' tip check
  float unknown_84() const { return guest::LoadFloat(address + 84); }
  void set_unknown_84(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  // probably the roll (Vehicle::ProbeGround's last dot product)
  float roll() const { return guest::LoadFloat(address + 88); }
  void set_roll(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t roll_address() const { return address + 88; }
  // the body slip term (side over forward, or atan2, times the speed squared clamped)
  float slip_term() const { return guest::LoadFloat(address + 92); }
  void set_slip_term(float v) const { guest::StoreFloat(address + 92, v); }
  uint32_t slip_term_address() const { return address + 92; }
  // the ground frame's up (the wheels' normals' sum, normalized)
  uint32_t ground_up_address() const { return address + 96; }
  // the ground frame's forward
  uint32_t ground_forward_address() const { return address + 112; }
  // the ground frame's side
  uint32_t ground_side_address() const { return address + 128; }
  // the ground frame as a 4x4 (+144 to +176, w 1.0)
  uint32_t ground_matrix_address() const { return address + 144; }
  // a copy of the body's matrix (Matrix::CopyRows)
  uint32_t body_matrix_address() const { return address + 208; }
  // the car data (the vehicle's +3376)
  template <typename T = CarData>
  T car_data() const { return T(guest::LoadU32(address + 272)); }
  void set_car_data(const View& v) const { guest::StoreU32(address + 272, v.address); }
  void set_car_data(uint32_t v) const { guest::StoreU32(address + 272, v); }
  uint32_t car_data_address() const { return address + 272; }
  // set: the steering limit by the speed
  uint8_t speed_sensitive_steering() const { return guest::LoadU8(address + 276); }
  void set_speed_sensitive_steering(uint8_t v) const { guest::StoreU8(address + 276, v); }
  uint32_t speed_sensitive_steering_address() const { return address + 276; }
  // Vehicle::SetupFromCar's (the car data's +276: the measure's)
  uint32_t car_data_276() const { return guest::LoadU32(address + 276); }
  void set_car_data_276(uint32_t v) const { guest::StoreU32(address + 276, v); }
  uint32_t car_data_276_address() const { return address + 276; }
  // the steering, -1 to 1
  float steering_input() const { return guest::LoadFloat(address + 280); }
  void set_steering_input(float v) const { guest::StoreFloat(address + 280, v); }
  uint32_t steering_input_address() const { return address + 280; }
  float throttle() const { return guest::LoadFloat(address + 284); }
  void set_throttle(float v) const { guest::StoreFloat(address + 284, v); }
  uint32_t throttle_address() const { return address + 284; }
  // a ramped control, probably reverse
  float reverse_throttle() const { return guest::LoadFloat(address + 288); }
  void set_reverse_throttle(float v) const { guest::StoreFloat(address + 288, v); }
  uint32_t reverse_throttle_address() const { return address + 288; }
  float brake() const { return guest::LoadFloat(address + 292); }
  void set_brake(float v) const { guest::StoreFloat(address + 292, v); }
  uint32_t brake_address() const { return address + 292; }
  // probably
  float handbrake() const { return guest::LoadFloat(address + 296); }
  void set_handbrake(float v) const { guest::StoreFloat(address + 296, v); }
  uint32_t handbrake_address() const { return address + 296; }
  float unknown_300() const { return guest::LoadFloat(address + 300); }
  void set_unknown_300(float v) const { guest::StoreFloat(address + 300, v); }
  uint32_t unknown_300_address() const { return address + 300; }
  // the steering angle (Vehicle::UpdateSteering)
  float steering() const { return guest::LoadFloat(address + 304); }
  void set_steering(float v) const { guest::StoreFloat(address + 304, v); }
  uint32_t steering_address() const { return address + 304; }
  // the front wheels' angles (Ackermann)
  float front_left_angle() const { return guest::LoadFloat(address + 308); }
  void set_front_left_angle(float v) const { guest::StoreFloat(address + 308, v); }
  uint32_t front_left_angle_address() const { return address + 308; }
  float front_right_angle() const { return guest::LoadFloat(address + 312); }
  void set_front_right_angle(float v) const { guest::StoreFloat(address + 312, v); }
  uint32_t front_right_angle_address() const { return address + 312; }
  float steering_limit_left() const { return guest::LoadFloat(address + 316); }
  void set_steering_limit_left(float v) const { guest::StoreFloat(address + 316, v); }
  uint32_t steering_limit_left_address() const { return address + 316; }
  float steering_limit_right() const { return guest::LoadFloat(address + 320); }
  void set_steering_limit_right(float v) const { guest::StoreFloat(address + 320, v); }
  uint32_t steering_limit_right_address() const { return address + 320; }
  float steering_smoothed() const { return guest::LoadFloat(address + 324); }
  void set_steering_smoothed(float v) const { guest::StoreFloat(address + 324, v); }
  uint32_t steering_smoothed_address() const { return address + 324; }
  // the old +304
  float steering_previous() const { return guest::LoadFloat(address + 328); }
  void set_steering_previous(float v) const { guest::StoreFloat(address + 328, v); }
  uint32_t steering_previous_address() const { return address + 328; }
  float unknown_332() const { return guest::LoadFloat(address + 332); }
  void set_unknown_332(float v) const { guest::StoreFloat(address + 332, v); }
  uint32_t unknown_332_address() const { return address + 332; }
  // its rigid body (528 bytes)
  template <typename T = RigidBody>
  T body() const { return T(guest::LoadU32(address + 336)); }
  void set_body(const View& v) const { guest::StoreU32(address + 336, v.address); }
  void set_body(uint32_t v) const { guest::StoreU32(address + 336, v); }
  uint32_t body_address() const { return address + 336; }
  // its collision object in the collision world (320 bytes)
  uint32_t collision_object() const { return guest::LoadU32(address + 340); }
  void set_collision_object(uint32_t v) const { guest::StoreU32(address + 340, v); }
  uint32_t collision_object_address() const { return address + 340; }
  // its collision box (BoxShape, 496 bytes)
  uint32_t box() const { return guest::LoadU32(address + 344); }
  void set_box(uint32_t v) const { guest::StoreU32(address + 344, v); }
  uint32_t box_address() const { return address + 344; }
  uint32_t wheels_on_ground() const { return guest::LoadU32(address + 348); }
  void set_wheels_on_ground(uint32_t v) const { guest::StoreU32(address + 348, v); }
  uint32_t wheels_on_ground_address() const { return address + 348; }
  // the zero vector (Vehicle::ResetPhysics)
  uint32_t unknown_352_address() const { return address + 352; }
  // the wheels' forces in the ground frame
  uint32_t wheel_forces_ground_address() const { return address + 368; }
  // the velocity in the ground frame
  uint32_t ground_velocity_address() const { return address + 384; }
  uint32_t unknown_400_address() const { return address + 400; }
  // four VehicleWheel records (368 bytes each)
  uint32_t wheels_address() const { return address + 416; }
  // a VehicleAxle
  uint32_t front_axle_address() const { return address + 1888; }
  // a VehicleAxle
  uint32_t rear_axle_address() const { return address + 2016; }
  // four pointers: each wheel's axle record (+1888 or +2016)
  uint32_t wheel_axles_address() const { return address + 2144; }
  // four pointers: each wheel's axle record in the car data (+336 or +608 there)
  uint32_t wheel_axle_data_address() const { return address + 2160; }
  // the wheel indices sorted by load (+280)
  uint32_t wheels_by_load_address() const { return address + 2176; }
  // the driven axle's mean spin
  float driven_axle_spin() const { return guest::LoadFloat(address + 2192); }
  void set_driven_axle_spin(float v) const { guest::StoreFloat(address + 2192, v); }
  uint32_t driven_axle_spin_address() const { return address + 2192; }
  float engine_spin() const { return guest::LoadFloat(address + 2196); }
  void set_engine_spin(float v) const { guest::StoreFloat(address + 2196, v); }
  uint32_t engine_spin_address() const { return address + 2196; }
  // the old +2196
  float engine_spin_previous() const { return guest::LoadFloat(address + 2200); }
  void set_engine_spin_previous(float v) const { guest::StoreFloat(address + 2200, v); }
  uint32_t engine_spin_previous_address() const { return address + 2200; }
  float unknown_2204() const { return guest::LoadFloat(address + 2204); }
  void set_unknown_2204(float v) const { guest::StoreFloat(address + 2204, v); }
  uint32_t unknown_2204_address() const { return address + 2204; }
  float engine_momentum() const { return guest::LoadFloat(address + 2208); }
  void set_engine_momentum(float v) const { guest::StoreFloat(address + 2208, v); }
  uint32_t engine_momentum_address() const { return address + 2208; }
  // set by Vehicle::UpdateDrivetrain
  float unknown_2212() const { return guest::LoadFloat(address + 2212); }
  void set_unknown_2212(float v) const { guest::StoreFloat(address + 2212, v); }
  uint32_t unknown_2212_address() const { return address + 2212; }
  float unknown_2216() const { return guest::LoadFloat(address + 2216); }
  void set_unknown_2216(float v) const { guest::StoreFloat(address + 2216, v); }
  uint32_t unknown_2216_address() const { return address + 2216; }
  // set by Vehicle::UpdateDrivetrain
  float unknown_2220() const { return guest::LoadFloat(address + 2220); }
  void set_unknown_2220(float v) const { guest::StoreFloat(address + 2220, v); }
  uint32_t unknown_2220_address() const { return address + 2220; }
  // set by Vehicle::UpdateDrivetrain (read by sub_820C0140, the tachometer)
  float unknown_2224() const { return guest::LoadFloat(address + 2224); }
  void set_unknown_2224(float v) const { guest::StoreFloat(address + 2224, v); }
  uint32_t unknown_2224_address() const { return address + 2224; }
  // the rpm, normalized 0 to 1
  float rpm() const { return guest::LoadFloat(address + 2228); }
  void set_rpm(float v) const { guest::StoreFloat(address + 2228, v); }
  uint32_t rpm_address() const { return address + 2228; }
  float unknown_2232() const { return guest::LoadFloat(address + 2232); }
  void set_unknown_2232(float v) const { guest::StoreFloat(address + 2232, v); }
  uint32_t unknown_2232_address() const { return address + 2232; }
  // the engine's torque with nitrous and engine braking (the second of two)
  float engine_torque_2236() const { return guest::LoadFloat(address + 2236); }
  void set_engine_torque_2236(float v) const { guest::StoreFloat(address + 2236, v); }
  uint32_t engine_torque_2236_address() const { return address + 2236; }
  float engine_torque() const { return guest::LoadFloat(address + 2240); }
  void set_engine_torque(float v) const { guest::StoreFloat(address + 2240, v); }
  uint32_t engine_torque_address() const { return address + 2240; }
  float rev_limiter() const { return guest::LoadFloat(address + 2244); }
  void set_rev_limiter(float v) const { guest::StoreFloat(address + 2244, v); }
  uint32_t rev_limiter_address() const { return address + 2244; }
  uint8_t unknown_2248() const { return guest::LoadU8(address + 2248); }
  void set_unknown_2248(uint8_t v) const { guest::StoreU8(address + 2248, v); }
  uint32_t unknown_2248_address() const { return address + 2248; }
  // set: the gearbox shifts by itself (the car's +3892 not 0)
  uint8_t automatic() const { return guest::LoadU8(address + 2249); }
  void set_automatic(uint8_t v) const { guest::StoreU8(address + 2249, v); }
  uint32_t automatic_address() const { return address + 2249; }
  // -1 reverse, 0 neutral
  int32_t gear() const { return int32_t(guest::LoadU32(address + 2252)); }
  void set_gear(int32_t v) const { guest::StoreU32(address + 2252, uint32_t(v)); }
  uint32_t gear_address() const { return address + 2252; }
  int32_t gear_previous() const { return int32_t(guest::LoadU32(address + 2256)); }
  void set_gear_previous(int32_t v) const { guest::StoreU32(address + 2256, uint32_t(v)); }
  uint32_t gear_previous_address() const { return address + 2256; }
  uint32_t unknown_2260() const { return guest::LoadU32(address + 2260); }
  void set_unknown_2260(uint32_t v) const { guest::StoreU32(address + 2260, v); }
  uint32_t unknown_2260_address() const { return address + 2260; }
  float clutch() const { return guest::LoadFloat(address + 2264); }
  void set_clutch(float v) const { guest::StoreFloat(address + 2264, v); }
  uint32_t clutch_address() const { return address + 2264; }
  float shift_timer() const { return guest::LoadFloat(address + 2268); }
  void set_shift_timer(float v) const { guest::StoreFloat(address + 2268, v); }
  uint32_t shift_timer_address() const { return address + 2268; }
  uint32_t unknown_2272() const { return guest::LoadU32(address + 2272); }
  void set_unknown_2272(uint32_t v) const { guest::StoreU32(address + 2272, v); }
  uint32_t unknown_2272_address() const { return address + 2272; }
  uint8_t unknown_2276() const { return guest::LoadU8(address + 2276); }
  void set_unknown_2276(uint8_t v) const { guest::StoreU8(address + 2276, v); }
  uint32_t unknown_2276_address() const { return address + 2276; }
  uint8_t unknown_2277() const { return guest::LoadU8(address + 2277); }
  void set_unknown_2277(uint8_t v) const { guest::StoreU8(address + 2277, v); }
  uint32_t unknown_2277_address() const { return address + 2277; }
  uint8_t unknown_2280() const { return guest::LoadU8(address + 2280); }
  void set_unknown_2280(uint8_t v) const { guest::StoreU8(address + 2280, v); }
  uint32_t unknown_2280_address() const { return address + 2280; }
  uint8_t unknown_2281() const { return guest::LoadU8(address + 2281); }
  void set_unknown_2281(uint8_t v) const { guest::StoreU8(address + 2281, v); }
  uint32_t unknown_2281_address() const { return address + 2281; }
  uint8_t unknown_2282() const { return guest::LoadU8(address + 2282); }
  void set_unknown_2282(uint8_t v) const { guest::StoreU8(address + 2282, v); }
  uint32_t unknown_2282_address() const { return address + 2282; }
  uint8_t shift_up_request() const { return guest::LoadU8(address + 2284); }
  void set_shift_up_request(uint8_t v) const { guest::StoreU8(address + 2284, v); }
  uint32_t shift_up_request_address() const { return address + 2284; }
  uint8_t shift_down_request() const { return guest::LoadU8(address + 2285); }
  void set_shift_down_request(uint8_t v) const { guest::StoreU8(address + 2285, v); }
  uint32_t shift_down_request_address() const { return address + 2285; }
  uint8_t unknown_2286() const { return guest::LoadU8(address + 2286); }
  void set_unknown_2286(uint8_t v) const { guest::StoreU8(address + 2286, v); }
  uint32_t unknown_2286_address() const { return address + 2286; }
  // with one gear (probably a CVT) its ratio
  float cvt_ratio() const { return guest::LoadFloat(address + 2288); }
  void set_cvt_ratio(float v) const { guest::StoreFloat(address + 2288, v); }
  uint32_t cvt_ratio_address() const { return address + 2288; }
  // from the torque curve's peak (Vehicle::SetUpGearing)
  float unknown_2292() const { return guest::LoadFloat(address + 2292); }
  void set_unknown_2292(float v) const { guest::StoreFloat(address + 2292, v); }
  uint32_t unknown_2292_address() const { return address + 2292; }
  // from the torque curve's peak (Vehicle::SetUpGearing)
  float unknown_2296() const { return guest::LoadFloat(address + 2296); }
  void set_unknown_2296(float v) const { guest::StoreFloat(address + 2296, v); }
  uint32_t unknown_2296_address() const { return address + 2296; }
  // Vehicle::SetUpGearing's
  float unknown_2300() const { return guest::LoadFloat(address + 2300); }
  void set_unknown_2300(float v) const { guest::StoreFloat(address + 2300, v); }
  uint32_t unknown_2300_address() const { return address + 2300; }
  // |the gear's ratio x the final drive|
  float total_ratio() const { return guest::LoadFloat(address + 2304); }
  void set_total_ratio(float v) const { guest::StoreFloat(address + 2304, v); }
  uint32_t total_ratio_address() const { return address + 2304; }
  // the gear's other value x the car data's +1544
  float gear_value() const { return guest::LoadFloat(address + 2308); }
  void set_gear_value(float v) const { guest::StoreFloat(address + 2308, v); }
  uint32_t gear_value_address() const { return address + 2308; }
  // +2336 + +2328 (Vehicle::SetGearValues)
  float unknown_2312() const { return guest::LoadFloat(address + 2312); }
  void set_unknown_2312(float v) const { guest::StoreFloat(address + 2312, v); }
  uint32_t unknown_2312_address() const { return address + 2312; }
  float unknown_2316() const { return guest::LoadFloat(address + 2316); }
  void set_unknown_2316(float v) const { guest::StoreFloat(address + 2316, v); }
  uint32_t unknown_2316_address() const { return address + 2316; }
  // 0.0 (Vehicle::SetGearValues)
  float unknown_2320() const { return guest::LoadFloat(address + 2320); }
  void set_unknown_2320(float v) const { guest::StoreFloat(address + 2320, v); }
  uint32_t unknown_2320_address() const { return address + 2320; }
  // +2328 (Vehicle::SetGearValues)
  float unknown_2324() const { return guest::LoadFloat(address + 2324); }
  void set_unknown_2324(float v) const { guest::StoreFloat(address + 2324, v); }
  uint32_t unknown_2324_address() const { return address + 2324; }
  // Vehicle::SetUpGearing's
  float unknown_2328() const { return guest::LoadFloat(address + 2328); }
  void set_unknown_2328(float v) const { guest::StoreFloat(address + 2328, v); }
  uint32_t unknown_2328_address() const { return address + 2328; }
  float driven_wheel_radius() const { return guest::LoadFloat(address + 2332); }
  void set_driven_wheel_radius(float v) const { guest::StoreFloat(address + 2332, v); }
  uint32_t driven_wheel_radius_address() const { return address + 2332; }
  // the driven axle's (Vehicle::SetUpGearing)
  float axle_radius() const { return guest::LoadFloat(address + 2336); }
  void set_axle_radius(float v) const { guest::StoreFloat(address + 2336, v); }
  uint32_t axle_radius_address() const { return address + 2336; }
  // the engine shake's phase (Vehicle::UpdateAero: a push into the front wheels at 1.0 or more)
  float engine_shake() const { return guest::LoadFloat(address + 2340); }
  void set_engine_shake(float v) const { guest::StoreFloat(address + 2340, v); }
  uint32_t engine_shake_address() const { return address + 2340; }
  // the engine shake's counter (0 to 5; a push at 0 and 2)
  int32_t engine_shake_count() const { return int32_t(guest::LoadU32(address + 2344)); }
  void set_engine_shake_count(int32_t v) const { guest::StoreU32(address + 2344, uint32_t(v)); }
  uint32_t engine_shake_count_address() const { return address + 2344; }
  // -1.0 in reverse, else 1.0
  float direction() const { return guest::LoadFloat(address + 2348); }
  void set_direction(float v) const { guest::StoreFloat(address + 2348, v); }
  uint32_t direction_address() const { return address + 2348; }
  // the step's time
  float dt() const { return guest::LoadFloat(address + 2352); }
  void set_dt(float v) const { guest::StoreFloat(address + 2352, v); }
  uint32_t dt_address() const { return address + 2352; }
  float inverse_dt() const { return guest::LoadFloat(address + 2356); }
  void set_inverse_dt(float v) const { guest::StoreFloat(address + 2356, v); }
  uint32_t inverse_dt_address() const { return address + 2356; }
  float unknown_2360() const { return guest::LoadFloat(address + 2360); }
  void set_unknown_2360(float v) const { guest::StoreFloat(address + 2360, v); }
  uint32_t unknown_2360_address() const { return address + 2360; }
  float substep_dt() const { return guest::LoadFloat(address + 2364); }
  void set_substep_dt(float v) const { guest::StoreFloat(address + 2364, v); }
  uint32_t substep_dt_address() const { return address + 2364; }
  // 10
  uint32_t substep_count() const { return guest::LoadU32(address + 2368); }
  void set_substep_count(uint32_t v) const { guest::StoreU32(address + 2368, v); }
  uint32_t substep_count_address() const { return address + 2368; }
  // 0x2 set by the set-up
  uint32_t flags() const { return guest::LoadU32(address + 2372); }
  void set_flags(uint32_t v) const { guest::StoreU32(address + 2372, v); }
  uint32_t flags_address() const { return address + 2372; }
  // the car data's +172: 0 front, 1 rear, 2 and up all-wheel
  uint32_t drive_type() const { return guest::LoadU32(address + 2376); }
  void set_drive_type(uint32_t v) const { guest::StoreU32(address + 2376, v); }
  uint32_t drive_type_address() const { return address + 2376; }
  // set by Vehicle::SetUp
  uint8_t unknown_2380() const { return guest::LoadU8(address + 2380); }
  void set_unknown_2380(uint8_t v) const { guest::StoreU8(address + 2380, v); }
  uint32_t unknown_2380_address() const { return address + 2380; }
  uint8_t unknown_2381() const { return guest::LoadU8(address + 2381); }
  void set_unknown_2381(uint8_t v) const { guest::StoreU8(address + 2381, v); }
  uint32_t unknown_2381_address() const { return address + 2381; }
  uint8_t unknown_2382() const { return guest::LoadU8(address + 2382); }
  void set_unknown_2382(uint8_t v) const { guest::StoreU8(address + 2382, v); }
  uint32_t unknown_2382_address() const { return address + 2382; }
  uint8_t unknown_2383() const { return guest::LoadU8(address + 2383); }
  void set_unknown_2383(uint8_t v) const { guest::StoreU8(address + 2383, v); }
  uint32_t unknown_2383_address() const { return address + 2383; }
  uint8_t forced_rest() const { return guest::LoadU8(address + 2384); }
  void set_forced_rest(uint8_t v) const { guest::StoreU8(address + 2384, v); }
  uint32_t forced_rest_address() const { return address + 2384; }
  // Vehicle::SetValue2388's flag
  uint8_t value2388_set() const { return guest::LoadU8(address + 2385); }
  void set_value2388_set(uint8_t v) const { guest::StoreU8(address + 2385, v); }
  uint32_t value2388_set_address() const { return address + 2385; }
  // probably ABS (the car's byte +77 not 0)
  uint8_t abs() const { return guest::LoadU8(address + 2386); }
  void set_abs(uint8_t v) const { guest::StoreU8(address + 2386, v); }
  uint32_t abs_address() const { return address + 2386; }
  uint8_t unknown_2387() const { return guest::LoadU8(address + 2387); }
  void set_unknown_2387(uint8_t v) const { guest::StoreU8(address + 2387, v); }
  uint32_t unknown_2387_address() const { return address + 2387; }
  // Vehicle::SetValue2388's, 0.0 to 1.0
  float value2388() const { return guest::LoadFloat(address + 2388); }
  void set_value2388(float v) const { guest::StoreFloat(address + 2388, v); }
  uint32_t value2388_address() const { return address + 2388; }
  // traction control's counter, 0 to 30
  uint32_t traction_counter() const { return guest::LoadU32(address + 2392); }
  void set_traction_counter(uint32_t v) const { guest::StoreU32(address + 2392, v); }
  uint32_t traction_counter_address() const { return address + 2392; }
  float traction_limit() const { return guest::LoadFloat(address + 2396); }
  void set_traction_limit(float v) const { guest::StoreFloat(address + 2396, v); }
  uint32_t traction_limit_address() const { return address + 2396; }
  // set when traction control cut the throttle
  uint8_t traction_active() const { return guest::LoadU8(address + 2400); }
  void set_traction_active(uint8_t v) const { guest::StoreU8(address + 2400, v); }
  uint32_t traction_active_address() const { return address + 2400; }
  uint8_t abs_active() const { return guest::LoadU8(address + 2401); }
  void set_abs_active(uint8_t v) const { guest::StoreU8(address + 2401, v); }
  uint32_t abs_active_address() const { return address + 2401; }
  float drag_scale() const { return guest::LoadFloat(address + 2404); }
  void set_drag_scale(float v) const { guest::StoreFloat(address + 2404, v); }
  uint32_t drag_scale_address() const { return address + 2404; }
  float grip_scale() const { return guest::LoadFloat(address + 2408); }
  void set_grip_scale(float v) const { guest::StoreFloat(address + 2408, v); }
  uint32_t grip_scale_address() const { return address + 2408; }
  float torque_scale() const { return guest::LoadFloat(address + 2412); }
  void set_torque_scale(float v) const { guest::StoreFloat(address + 2412, v); }
  uint32_t torque_scale_address() const { return address + 2412; }
  // probably the nitrous: 1.0 to 1.75
  float nitrous_factor() const { return guest::LoadFloat(address + 2416); }
  void set_nitrous_factor(float v) const { guest::StoreFloat(address + 2416, v); }
  uint32_t nitrous_factor_address() const { return address + 2416; }
  // probably the nitrous: 0 to 690
  float nitrous_value() const { return guest::LoadFloat(address + 2420); }
  void set_nitrous_value(float v) const { guest::StoreFloat(address + 2420, v); }
  uint32_t nitrous_value_address() const { return address + 2420; }
  uint8_t nitrous_on() const { return guest::LoadU8(address + 2424); }
  void set_nitrous_on(uint8_t v) const { guest::StoreU8(address + 2424, v); }
  uint32_t nitrous_on_address() const { return address + 2424; }
  // lowered by the oil temperature (probably), at least 0.7
  float oil_factor() const { return guest::LoadFloat(address + 2428); }
  void set_oil_factor(float v) const { guest::StoreFloat(address + 2428, v); }
  uint32_t oil_factor_address() const { return address + 2428; }
  // at least 0.5
  float brake_efficiency() const { return guest::LoadFloat(address + 2432); }
  void set_brake_efficiency(float v) const { guest::StoreFloat(address + 2432, v); }
  uint32_t brake_efficiency_address() const { return address + 2432; }
  float unknown_2436() const { return guest::LoadFloat(address + 2436); }
  void set_unknown_2436(float v) const { guest::StoreFloat(address + 2436, v); }
  uint32_t unknown_2436_address() const { return address + 2436; }
  float speed_squared() const { return guest::LoadFloat(address + 2440); }
  void set_speed_squared(float v) const { guest::StoreFloat(address + 2440, v); }
  uint32_t speed_squared_address() const { return address + 2440; }
  uint8_t unknown_2444() const { return guest::LoadU8(address + 2444); }
  void set_unknown_2444(uint8_t v) const { guest::StoreU8(address + 2444, v); }
  uint32_t unknown_2444_address() const { return address + 2444; }
  uint8_t unknown_2445() const { return guest::LoadU8(address + 2445); }
  void set_unknown_2445(uint8_t v) const { guest::StoreU8(address + 2445, v); }
  uint32_t unknown_2445_address() const { return address + 2445; }
  float unknown_2448() const { return guest::LoadFloat(address + 2448); }
  void set_unknown_2448(float v) const { guest::StoreFloat(address + 2448, v); }
  uint32_t unknown_2448_address() const { return address + 2448; }
  float unknown_2452() const { return guest::LoadFloat(address + 2452); }
  void set_unknown_2452(float v) const { guest::StoreFloat(address + 2452, v); }
  uint32_t unknown_2452_address() const { return address + 2452; }
  uint8_t unknown_2456() const { return guest::LoadU8(address + 2456); }
  void set_unknown_2456(uint8_t v) const { guest::StoreU8(address + 2456, v); }
  uint32_t unknown_2456_address() const { return address + 2456; }
  // a copy of the body's force
  uint32_t body_force_address() const { return address + 2464; }
  // a copy of the body's torque
  uint32_t body_torque_address() const { return address + 2480; }
  uint32_t tire_wear_mode() const { return guest::LoadU32(address + 2496); }
  void set_tire_wear_mode(uint32_t v) const { guest::StoreU32(address + 2496, v); }
  uint32_t tire_wear_mode_address() const { return address + 2496; }
  float backfire_charge() const { return guest::LoadFloat(address + 2500); }
  void set_backfire_charge(float v) const { guest::StoreFloat(address + 2500, v); }
  uint32_t backfire_charge_address() const { return address + 2500; }
  uint8_t backfire() const { return guest::LoadU8(address + 2504); }
  void set_backfire(uint8_t v) const { guest::StoreU8(address + 2504, v); }
  uint32_t backfire_address() const { return address + 2504; }
  float backfire_strength() const { return guest::LoadFloat(address + 2508); }
  void set_backfire_strength(float v) const { guest::StoreFloat(address + 2508, v); }
  uint32_t backfire_strength_address() const { return address + 2508; }
  // set by Vehicle::SetUp (the engine)
  uint8_t unknown_2512() const { return guest::LoadU8(address + 2512); }
  void set_unknown_2512(uint8_t v) const { guest::StoreU8(address + 2512, v); }
  uint32_t unknown_2512_address() const { return address + 2512; }
  // after the rev limiter and the clutch
  float throttle_used() const { return guest::LoadFloat(address + 2516); }
  void set_throttle_used(float v) const { guest::StoreFloat(address + 2516, v); }
  uint32_t throttle_used_address() const { return address + 2516; }
  float throttle_used_previous() const { return guest::LoadFloat(address + 2520); }
  void set_throttle_used_previous(float v) const { guest::StoreFloat(address + 2520, v); }
  uint32_t throttle_used_previous_address() const { return address + 2520; }
  // 6, or the car's word +1672
  uint32_t unknown_2524() const { return guest::LoadU32(address + 2524); }
  void set_unknown_2524(uint32_t v) const { guest::StoreU32(address + 2524, v); }
  uint32_t unknown_2524_address() const { return address + 2524; }
  // an axis (Vehicle::Unflip)
  uint32_t axis_address() const { return address + 2528; }
  // the wheels' force accumulators (16 bytes each)
  uint32_t wheel_accumulators_address() const { return address + 2544; }
  // the body's force accumulator
  uint32_t body_accumulator_address() const { return address + 2608; }
  uint8_t unknown_2624() const { return guest::LoadU8(address + 2624); }
  void set_unknown_2624(uint8_t v) const { guest::StoreU8(address + 2624, v); }
  uint32_t unknown_2624_address() const { return address + 2624; }
  uint8_t unknown_2625() const { return guest::LoadU8(address + 2625); }
  void set_unknown_2625(uint8_t v) const { guest::StoreU8(address + 2625, v); }
  uint32_t unknown_2625_address() const { return address + 2625; }
  float unknown_2628() const { return guest::LoadFloat(address + 2628); }
  void set_unknown_2628(float v) const { guest::StoreFloat(address + 2628, v); }
  uint32_t unknown_2628_address() const { return address + 2628; }
  // the shift-up speeds, a float a gear
  uint32_t shift_up_speeds_address() const { return address + 2632; }
  uint8_t surfaces_on() const { return guest::LoadU8(address + 2664); }
  void set_surfaces_on(uint8_t v) const { guest::StoreU8(address + 2664, v); }
  uint32_t surfaces_on_address() const { return address + 2664; }
  // pointers to the front surface records by surface index (+0 a ground offset, +12 grip, +16 wear, +20 drag)
  uint32_t front_surfaces_address() const { return address + 2668; }
  // the rear's
  uint32_t rear_surfaces_address() const { return address + 2796; }
  float unknown_2924() const { return guest::LoadFloat(address + 2924); }
  void set_unknown_2924(float v) const { guest::StoreFloat(address + 2924, v); }
  uint32_t unknown_2924_address() const { return address + 2924; }
  uint8_t at_rest() const { return guest::LoadU8(address + 2928); }
  void set_at_rest(uint8_t v) const { guest::StoreU8(address + 2928, v); }
  uint32_t at_rest_address() const { return address + 2928; }
  float unknown_2932() const { return guest::LoadFloat(address + 2932); }
  void set_unknown_2932(float v) const { guest::StoreFloat(address + 2932, v); }
  uint32_t unknown_2932_address() const { return address + 2932; }
  float unknown_2936() const { return guest::LoadFloat(address + 2936); }
  void set_unknown_2936(float v) const { guest::StoreFloat(address + 2936, v); }
  uint32_t unknown_2936_address() const { return address + 2936; }
  float unknown_2940() const { return guest::LoadFloat(address + 2940); }
  void set_unknown_2940(float v) const { guest::StoreFloat(address + 2940, v); }
  uint32_t unknown_2940_address() const { return address + 2940; }
  float boost() const { return guest::LoadFloat(address + 2944); }
  void set_boost(float v) const { guest::StoreFloat(address + 2944, v); }
  uint32_t boost_address() const { return address + 2944; }
  uint8_t wastegate_open() const { return guest::LoadU8(address + 2948); }
  void set_wastegate_open(uint8_t v) const { guest::StoreU8(address + 2948, v); }
  uint32_t wastegate_open_address() const { return address + 2948; }
  float wastegate() const { return guest::LoadFloat(address + 2952); }
  void set_wastegate(float v) const { guest::StoreFloat(address + 2952, v); }
  uint32_t wastegate_address() const { return address + 2952; }
  float turbo_torque_factor() const { return guest::LoadFloat(address + 2956); }
  void set_turbo_torque_factor(float v) const { guest::StoreFloat(address + 2956, v); }
  uint32_t turbo_torque_factor_address() const { return address + 2956; }
  uint8_t blow_off_open() const { return guest::LoadU8(address + 2960); }
  void set_blow_off_open(uint8_t v) const { guest::StoreU8(address + 2960, v); }
  uint32_t blow_off_open_address() const { return address + 2960; }
  float blow_off() const { return guest::LoadFloat(address + 2964); }
  void set_blow_off(float v) const { guest::StoreFloat(address + 2964, v); }
  uint32_t blow_off_address() const { return address + 2964; }
  // the gauges +2968 to +2980
  float gauge_0() const { return guest::LoadFloat(address + 2968); }
  void set_gauge_0(float v) const { guest::StoreFloat(address + 2968, v); }
  uint32_t gauge_0_address() const { return address + 2968; }
  float gauge_1() const { return guest::LoadFloat(address + 2972); }
  void set_gauge_1(float v) const { guest::StoreFloat(address + 2972, v); }
  uint32_t gauge_1_address() const { return address + 2972; }
  float gauge_2() const { return guest::LoadFloat(address + 2976); }
  void set_gauge_2(float v) const { guest::StoreFloat(address + 2976, v); }
  uint32_t gauge_2_address() const { return address + 2976; }
  float gauge_3() const { return guest::LoadFloat(address + 2980); }
  void set_gauge_3(float v) const { guest::StoreFloat(address + 2980, v); }
  uint32_t gauge_3_address() const { return address + 2980; }
  // the vehicle's +5232
  uint32_t track_position() const { return guest::LoadU32(address + 2984); }
  void set_track_position(uint32_t v) const { guest::StoreU32(address + 2984, v); }
  uint32_t track_position_address() const { return address + 2984; }
  // a SteerRecord
  uint32_t steer_record_address() const { return address + 2988; }
  float steering_result() const { return guest::LoadFloat(address + 3108); }
  void set_steering_result(float v) const { guest::StoreFloat(address + 3108, v); }
  uint32_t steering_result_address() const { return address + 3108; }
  float yaw_rate() const { return guest::LoadFloat(address + 3112); }
  void set_yaw_rate(float v) const { guest::StoreFloat(address + 3112, v); }
  uint32_t yaw_rate_address() const { return address + 3112; }
  // Vehicle::UpdateSteering's
  float unknown_3116() const { return guest::LoadFloat(address + 3116); }
  void set_unknown_3116(float v) const { guest::StoreFloat(address + 3116, v); }
  uint32_t unknown_3116_address() const { return address + 3116; }
  float unknown_3120() const { return guest::LoadFloat(address + 3120); }
  void set_unknown_3120(float v) const { guest::StoreFloat(address + 3120, v); }
  uint32_t unknown_3120_address() const { return address + 3120; }
  float unknown_3124() const { return guest::LoadFloat(address + 3124); }
  void set_unknown_3124(float v) const { guest::StoreFloat(address + 3124, v); }
  uint32_t unknown_3124_address() const { return address + 3124; }
  float unknown_3128() const { return guest::LoadFloat(address + 3128); }
  void set_unknown_3128(float v) const { guest::StoreFloat(address + 3128, v); }
  uint32_t unknown_3128_address() const { return address + 3128; }
  // cleared when righted
  uint8_t upright() const { return guest::LoadU8(address + 3132); }
  void set_upright(uint8_t v) const { guest::StoreU8(address + 3132, v); }
  uint32_t upright_address() const { return address + 3132; }
  float unknown_3136() const { return guest::LoadFloat(address + 3136); }
  void set_unknown_3136(float v) const { guest::StoreFloat(address + 3136, v); }
  uint32_t unknown_3136_address() const { return address + 3136; }
  float unknown_3140() const { return guest::LoadFloat(address + 3140); }
  void set_unknown_3140(float v) const { guest::StoreFloat(address + 3140, v); }
  uint32_t unknown_3140_address() const { return address + 3140; }
  float unknown_3144() const { return guest::LoadFloat(address + 3144); }
  void set_unknown_3144(float v) const { guest::StoreFloat(address + 3144, v); }
  uint32_t unknown_3144_address() const { return address + 3144; }
  float unknown_3148() const { return guest::LoadFloat(address + 3148); }
  void set_unknown_3148(float v) const { guest::StoreFloat(address + 3148, v); }
  uint32_t unknown_3148_address() const { return address + 3148; }
  float unknown_3152() const { return guest::LoadFloat(address + 3152); }
  void set_unknown_3152(float v) const { guest::StoreFloat(address + 3152, v); }
  uint32_t unknown_3152_address() const { return address + 3152; }
  uint32_t step_count() const { return guest::LoadU32(address + 3156); }
  void set_step_count(uint32_t v) const { guest::StoreU32(address + 3156, v); }
  uint32_t step_count_address() const { return address + 3156; }
  uint32_t unknown_3168_address() const { return address + 3168; }
  // probably the brakes' fade or smoke
  float brake_fade() const { return guest::LoadFloat(address + 3232); }
  void set_brake_fade(float v) const { guest::StoreFloat(address + 3232, v); }
  uint32_t brake_fade_address() const { return address + 3232; }
  float brake_fade_copy() const { return guest::LoadFloat(address + 3236); }
  void set_brake_fade_copy(float v) const { guest::StoreFloat(address + 3236, v); }
  uint32_t brake_fade_copy_address() const { return address + 3236; }
  // the shift's latch
  uint8_t backfire_latch() const { return guest::LoadU8(address + 3240); }
  void set_backfire_latch(uint8_t v) const { guest::StoreU8(address + 3240, v); }
  uint32_t backfire_latch_address() const { return address + 3240; }
  // the tip check's
  uint8_t tipped() const { return guest::LoadU8(address + 3241); }
  void set_tipped(uint8_t v) const { guest::StoreU8(address + 3241, v); }
  uint32_t tipped_address() const { return address + 3241; }
  uint32_t matrix_3248_address() const { return address + 3248; }
  // +3248 about the body's position (the body's +12 points here)
  uint32_t matrix_3312_address() const { return address + 3312; }
  // the car data (CarData, constructed in place)
  uint32_t car_data_block_address() const { return address + 3376; }
  // the car's own track position (halfwords +4, +6 the route position)
  uint32_t own_track_position_address() const { return address + 5232; }
  // the wheels' track positions (96 bytes each)
  uint32_t wheel_track_positions_address() const { return address + 5328; }
  // the car object (the racer record)
  uint32_t car() const { return guest::LoadU32(address + 5712); }
  void set_car(uint32_t v) const { guest::StoreU32(address + 5712, v); }
  uint32_t car_address() const { return address + 5712; }
  // nine 24-byte surface grip records
  uint32_t front_grip_records_address() const { return address + 5716; }
  uint32_t rear_grip_records_address() const { return address + 5932; }
  // the car's +3888
  uint32_t car_flags() const { return guest::LoadU32(address + 6148); }
  void set_car_flags(uint32_t v) const { guest::StoreU32(address + 6148, v); }
  uint32_t car_flags_address() const { return address + 6148; }
  // TrackCollision::Tick's
  uint32_t collision_flags() const { return guest::LoadU32(address + 6152); }
  void set_collision_flags(uint32_t v) const { guest::StoreU32(address + 6152, v); }
  uint32_t collision_flags_address() const { return address + 6152; }
  // the vehicle status (VehicleStatus, vehicle.md)
  uint32_t status_address() const { return address + 6160; }
};

// embedded; measure: Wheel::Copy.r4
struct VehicleWheel : View {
  using View::View;
  static constexpr uint32_t kSize = 368;
  // the suspension travel
  float travel() const { return guest::LoadFloat(address + 0); }
  void set_travel(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t travel_address() const { return address + 0; }
  float travel_speed() const { return guest::LoadFloat(address + 4); }
  void set_travel_speed(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t travel_speed_address() const { return address + 4; }
  float suspension_force() const { return guest::LoadFloat(address + 8); }
  void set_suspension_force(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t suspension_force_address() const { return address + 8; }
  float travel_previous() const { return guest::LoadFloat(address + 12); }
  void set_travel_previous(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t travel_previous_address() const { return address + 12; }
  // cleared by Vehicle::UpdateWheelSuspension
  uint8_t unknown_16() const { return guest::LoadU8(address + 16); }
  void set_unknown_16(uint8_t v) const { guest::StoreU8(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint8_t on_ground() const { return guest::LoadU8(address + 17); }
  void set_on_ground(uint8_t v) const { guest::StoreU8(address + 17, v); }
  uint32_t on_ground_address() const { return address + 17; }
  // the ground probe's handle (a track position)
  uint32_t track_position() const { return guest::LoadU32(address + 20); }
  void set_track_position(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t track_position_address() const { return address + 20; }
  float lateral_offset() const { return guest::LoadFloat(address + 24); }
  void set_lateral_offset(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t lateral_offset_address() const { return address + 24; }
  // the rotation angle
  float angle() const { return guest::LoadFloat(address + 28); }
  void set_angle(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t angle_address() const { return address + 28; }
  float camber() const { return guest::LoadFloat(address + 32); }
  void set_camber(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t camber_address() const { return address + 32; }
  float toe() const { return guest::LoadFloat(address + 36); }
  void set_toe(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t toe_address() const { return address + 36; }
  // not copied by Wheel::Copy
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t quaternion_address() const { return address + 48; }
  // the mount point in the world, no travel
  uint32_t mount_point_address() const { return address + 64; }
  // the wheel's position (the mount with the travel)
  uint32_t position_address() const { return address + 80; }
  // the ground's distance below the mount, plus the surface's offset
  float ground_distance() const { return guest::LoadFloat(address + 96); }
  void set_ground_distance(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t ground_distance_address() const { return address + 96; }
  // not copied by Wheel::Copy
  uint32_t unknown_100_address() const { return address + 100; }
  uint32_t ground_normal_address() const { return address + 112; }
  uint32_t side_axis_address() const { return address + 128; }
  uint32_t forward_axis_address() const { return address + 144; }
  uint32_t contact_point_address() const { return address + 160; }
  // the surface index
  uint32_t surface() const { return guest::LoadU32(address + 176); }
  void set_surface(uint32_t v) const { guest::StoreU32(address + 176, v); }
  uint32_t surface_address() const { return address + 176; }
  float unknown_180() const { return guest::LoadFloat(address + 180); }
  void set_unknown_180(float v) const { guest::StoreFloat(address + 180, v); }
  uint32_t unknown_180_address() const { return address + 180; }
  // the surface record's +12
  float surface_grip() const { return guest::LoadFloat(address + 184); }
  void set_surface_grip(float v) const { guest::StoreFloat(address + 184, v); }
  uint32_t surface_grip_address() const { return address + 184; }
  // the surface record's +16
  float surface_wear() const { return guest::LoadFloat(address + 188); }
  void set_surface_wear(float v) const { guest::StoreFloat(address + 188, v); }
  uint32_t surface_wear_address() const { return address + 188; }
  float unknown_192() const { return guest::LoadFloat(address + 192); }
  void set_unknown_192(float v) const { guest::StoreFloat(address + 192, v); }
  uint32_t unknown_192_address() const { return address + 192; }
  // the surface record's +20
  float surface_drag() const { return guest::LoadFloat(address + 196); }
  void set_surface_drag(float v) const { guest::StoreFloat(address + 196, v); }
  uint32_t surface_drag_address() const { return address + 196; }
  // after ABS
  float brake() const { return guest::LoadFloat(address + 200); }
  void set_brake(float v) const { guest::StoreFloat(address + 200, v); }
  uint32_t brake_address() const { return address + 200; }
  uint32_t abs_count() const { return guest::LoadU32(address + 204); }
  void set_abs_count(uint32_t v) const { guest::StoreU32(address + 204, v); }
  uint32_t abs_count_address() const { return address + 204; }
  float unknown_208() const { return guest::LoadFloat(address + 208); }
  void set_unknown_208(float v) const { guest::StoreFloat(address + 208, v); }
  uint32_t unknown_208_address() const { return address + 208; }
  float brake_force() const { return guest::LoadFloat(address + 212); }
  void set_brake_force(float v) const { guest::StoreFloat(address + 212, v); }
  uint32_t brake_force_address() const { return address + 212; }
  float brake_share() const { return guest::LoadFloat(address + 216); }
  void set_brake_share(float v) const { guest::StoreFloat(address + 216, v); }
  uint32_t brake_share_address() const { return address + 216; }
  float brake_heat() const { return guest::LoadFloat(address + 220); }
  void set_brake_heat(float v) const { guest::StoreFloat(address + 220, v); }
  uint32_t brake_heat_address() const { return address + 220; }
  float unknown_224() const { return guest::LoadFloat(address + 224); }
  void set_unknown_224(float v) const { guest::StoreFloat(address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  // set by the physics set-up
  float unknown_228() const { return guest::LoadFloat(address + 228); }
  void set_unknown_228(float v) const { guest::StoreFloat(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  float unknown_232() const { return guest::LoadFloat(address + 232); }
  void set_unknown_232(float v) const { guest::StoreFloat(address + 232, v); }
  uint32_t unknown_232_address() const { return address + 232; }
  float grip() const { return guest::LoadFloat(address + 236); }
  void set_grip(float v) const { guest::StoreFloat(address + 236, v); }
  uint32_t grip_address() const { return address + 236; }
  uint8_t unknown_240() const { return guest::LoadU8(address + 240); }
  void set_unknown_240(uint8_t v) const { guest::StoreU8(address + 240, v); }
  uint32_t unknown_240_address() const { return address + 240; }
  uint8_t unknown_241() const { return guest::LoadU8(address + 241); }
  void set_unknown_241(uint8_t v) const { guest::StoreU8(address + 241, v); }
  uint32_t unknown_241_address() const { return address + 241; }
  uint8_t unknown_242() const { return guest::LoadU8(address + 242); }
  void set_unknown_242(uint8_t v) const { guest::StoreU8(address + 242, v); }
  uint32_t unknown_242_address() const { return address + 242; }
  uint8_t unknown_243() const { return guest::LoadU8(address + 243); }
  void set_unknown_243(uint8_t v) const { guest::StoreU8(address + 243, v); }
  uint32_t unknown_243_address() const { return address + 243; }
  float unknown_244() const { return guest::LoadFloat(address + 244); }
  void set_unknown_244(float v) const { guest::StoreFloat(address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  float spin() const { return guest::LoadFloat(address + 248); }
  void set_spin(float v) const { guest::StoreFloat(address + 248, v); }
  uint32_t spin_address() const { return address + 248; }
  // spin x inertia
  float spin_momentum() const { return guest::LoadFloat(address + 252); }
  void set_spin_momentum(float v) const { guest::StoreFloat(address + 252, v); }
  uint32_t spin_momentum_address() const { return address + 252; }
  float unknown_256() const { return guest::LoadFloat(address + 256); }
  void set_unknown_256(float v) const { guest::StoreFloat(address + 256, v); }
  uint32_t unknown_256_address() const { return address + 256; }
  float slip_angle() const { return guest::LoadFloat(address + 260); }
  void set_slip_angle(float v) const { guest::StoreFloat(address + 260, v); }
  uint32_t slip_angle_address() const { return address + 260; }
  float slip_side() const { return guest::LoadFloat(address + 264); }
  void set_slip_side(float v) const { guest::StoreFloat(address + 264, v); }
  uint32_t slip_side_address() const { return address + 264; }
  // the longitudinal slip
  float slip_long() const { return guest::LoadFloat(address + 268); }
  void set_slip_long(float v) const { guest::StoreFloat(address + 268, v); }
  uint32_t slip_long_address() const { return address + 268; }
  float unknown_272() const { return guest::LoadFloat(address + 272); }
  void set_unknown_272(float v) const { guest::StoreFloat(address + 272, v); }
  uint32_t unknown_272_address() const { return address + 272; }
  float unknown_276() const { return guest::LoadFloat(address + 276); }
  void set_unknown_276(float v) const { guest::StoreFloat(address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  float load() const { return guest::LoadFloat(address + 280); }
  void set_load(float v) const { guest::StoreFloat(address + 280, v); }
  uint32_t load_address() const { return address + 280; }
  // an integrated force
  float force_integral() const { return guest::LoadFloat(address + 284); }
  void set_force_integral(float v) const { guest::StoreFloat(address + 284, v); }
  uint32_t force_integral_address() const { return address + 284; }
  // an integrated force
  float force_integral_2() const { return guest::LoadFloat(address + 288); }
  void set_force_integral_2(float v) const { guest::StoreFloat(address + 288, v); }
  uint32_t force_integral_2_address() const { return address + 288; }
  float tire_wear() const { return guest::LoadFloat(address + 292); }
  void set_tire_wear(float v) const { guest::StoreFloat(address + 292, v); }
  uint32_t tire_wear_address() const { return address + 292; }
  float unknown_296() const { return guest::LoadFloat(address + 296); }
  void set_unknown_296(float v) const { guest::StoreFloat(address + 296, v); }
  uint32_t unknown_296_address() const { return address + 296; }
  float brake_torque() const { return guest::LoadFloat(address + 300); }
  void set_brake_torque(float v) const { guest::StoreFloat(address + 300, v); }
  uint32_t brake_torque_address() const { return address + 300; }
  float lateral_force() const { return guest::LoadFloat(address + 304); }
  void set_lateral_force(float v) const { guest::StoreFloat(address + 304, v); }
  uint32_t lateral_force_address() const { return address + 304; }
  float longitudinal_force() const { return guest::LoadFloat(address + 308); }
  void set_longitudinal_force(float v) const { guest::StoreFloat(address + 308, v); }
  uint32_t longitudinal_force_address() const { return address + 308; }
  float unknown_312() const { return guest::LoadFloat(address + 312); }
  void set_unknown_312(float v) const { guest::StoreFloat(address + 312, v); }
  uint32_t unknown_312_address() const { return address + 312; }
  float force_limit() const { return guest::LoadFloat(address + 316); }
  void set_force_limit(float v) const { guest::StoreFloat(address + 316, v); }
  uint32_t force_limit_address() const { return address + 316; }
  float forward_speed() const { return guest::LoadFloat(address + 320); }
  void set_forward_speed(float v) const { guest::StoreFloat(address + 320, v); }
  uint32_t forward_speed_address() const { return address + 320; }
  float forward_speed_previous() const { return guest::LoadFloat(address + 324); }
  void set_forward_speed_previous(float v) const { guest::StoreFloat(address + 324, v); }
  uint32_t forward_speed_previous_address() const { return address + 324; }
  float side_speed() const { return guest::LoadFloat(address + 328); }
  void set_side_speed(float v) const { guest::StoreFloat(address + 328, v); }
  uint32_t side_speed_address() const { return address + 328; }
  // Vehicle::UpdateTireForces' copy
  float longitudinal_force_2() const { return guest::LoadFloat(address + 332); }
  void set_longitudinal_force_2(float v) const { guest::StoreFloat(address + 332, v); }
  uint32_t longitudinal_force_2_address() const { return address + 332; }
  // the velocity at the contact
  uint32_t contact_velocity_address() const { return address + 336; }
  float reaction_torque() const { return guest::LoadFloat(address + 352); }
  void set_reaction_torque(float v) const { guest::StoreFloat(address + 352, v); }
  uint32_t reaction_torque_address() const { return address + 352; }
  float slip_speed() const { return guest::LoadFloat(address + 356); }
  void set_slip_speed(float v) const { guest::StoreFloat(address + 356, v); }
  uint32_t slip_speed_address() const { return address + 356; }
};

// embedded; measure: WheelRecord2::Copy.r4, Vehicle.+2152, WheelRecord2::Copy.r3
struct VehicleAxle : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // the mounts' midpoint
  uint32_t mounts_midpoint_address() const { return address + 0; }
  uint32_t contacts_midpoint_address() const { return address + 16; }
  uint32_t unknown_32_address() const { return address + 32; }
  // the wheels' side axis (Vehicle::UpdateWheelAxes)
  uint32_t side_axis_address() const { return address + 48; }
  uint32_t forward_axis_address() const { return address + 64; }
  // the velocity at the contacts' midpoint
  uint32_t velocity_address() const { return address + 80; }
  float grip() const { return guest::LoadFloat(address + 96); }
  void set_grip(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t grip_address() const { return address + 96; }
  float torque_share() const { return guest::LoadFloat(address + 100); }
  void set_torque_share(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t torque_share_address() const { return address + 100; }
  float track() const { return guest::LoadFloat(address + 104); }
  void set_track(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t track_address() const { return address + 104; }
  float inertia() const { return guest::LoadFloat(address + 108); }
  void set_inertia(float v) const { guest::StoreFloat(address + 108, v); }
  uint32_t inertia_address() const { return address + 108; }
  float abs_threshold() const { return guest::LoadFloat(address + 112); }
  void set_abs_threshold(float v) const { guest::StoreFloat(address + 112, v); }
  uint32_t abs_threshold_address() const { return address + 112; }
  uint32_t locked() const { return guest::LoadU32(address + 116); }
  void set_locked(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t locked_address() const { return address + 116; }
};

// embedded; measure: Vehicle@+2988
struct SteerRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // the car data's front axle +16 (its address)
  uint32_t axle_16() const { return guest::LoadU32(address + 0); }
  void set_axle_16(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t axle_16_address() const { return address + 0; }
  // the front axle's +32 (its address)
  uint32_t axle_32() const { return guest::LoadU32(address + 4); }
  void set_axle_32(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t axle_32_address() const { return address + 4; }
  // f1 x f2
  float load_product() const { return guest::LoadFloat(address + 8); }
  void set_load_product(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t load_product_address() const { return address + 8; }
  // +8 x the float at 0x820029DC
  float scaled() const { return guest::LoadFloat(address + 12); }
  void set_scaled(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t scaled_address() const { return address + 12; }
  // +20 x f3
  float times() const { return guest::LoadFloat(address + 16); }
  void set_times(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t times_address() const { return address + 16; }
  float value() const { return guest::LoadFloat(address + 20); }
  void set_value(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t value_address() const { return address + 20; }
  // +20 / (+8 x c)
  float ratio_a() const { return guest::LoadFloat(address + 24); }
  void set_ratio_a(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t ratio_a_address() const { return address + 24; }
  // +20 / (+12 x c)
  float ratio_b() const { return guest::LoadFloat(address + 28); }
  void set_ratio_b(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t ratio_b_address() const { return address + 28; }
  // +16 / +20
  float ratio_c() const { return guest::LoadFloat(address + 32); }
  void set_ratio_c(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t ratio_c_address() const { return address + 32; }
};

// allocated; measure: RigidBody, CollisionObject.+16, CollisionPair.+20.+16, sub_820B68A0.r3, Matrix::Rotate.r5
#pragma pack(push, 1)
struct RigidBodyData {
  uint8_t fixed = 0;  // +0
  uint8_t pad_1[3] = {};
  float mass = 0;  // +4
  float inverse_mass = 0;  // +8
  uint32_t vehicle_matrix = 0;  // +12
  uint8_t center_of_mass[16] = {};  // +16, guest order
  uint8_t velocity[16] = {};  // +32, guest order
  float speed_cap = 0;  // +48
  uint8_t pad_52[12] = {};
  uint8_t force[16] = {};  // +64, guest order
  uint8_t orientation[16] = {};  // +80, guest order
  uint8_t angular_velocity[16] = {};  // +96, guest order
  float spin_cap = 0;  // +112
  uint8_t pad_116[12] = {};
  uint8_t torque[16] = {};  // +128, guest order
  uint8_t velocity_direction[16] = {};  // +144, guest order
  float speed = 0;  // +160
  float linear_damping = 0;  // +164
  uint8_t pad_168[8] = {};
  uint8_t momentum[16] = {};  // +176, guest order
  uint8_t center_of_mass_offset[16] = {};  // +192, guest order
  uint8_t origin[16] = {};  // +208, guest order
  uint8_t orientation_matrix[64] = {};  // +224, guest order
  uint8_t matrix[64] = {};  // +288, guest order
  uint8_t inertia[16] = {};  // +352, guest order
  uint8_t inverse_inertia[16] = {};  // +368, guest order
  float angular_damping = 0;  // +384
  uint8_t pad_388[12] = {};
  uint8_t angular_momentum[16] = {};  // +400, guest order
  uint8_t world_inverse_inertia[64] = {};  // +416, guest order
  uint8_t gravity[16] = {};  // +480, guest order
  uint8_t weight[16] = {};  // +496, guest order
  float effective_mass = 0;  // +512
  uint32_t flags = 0;  // +516
  uint8_t pad_520[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(RigidBodyData) == 528);
inline constexpr memory::LayoutField kRigidBodyFields[] = {{4, 4}, {8, 4}, {12, 4}, {48, 4}, {112, 4}, {160, 4}, {164, 4}, {384, 4}, {512, 4}, {516, 4}};
inline constexpr memory::Layout kRigidBodyLayout = {kRigidBodyFields, 10};

struct RigidBody : View {
  using View::View;
  static constexpr uint32_t kSize = 528;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  RigidBodyData* data() const { return static_cast<RigidBodyData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static RigidBody New() {
    return RigidBody(memory::MakeHandle(new RigidBodyData(), sizeof(RigidBodyData), "RigidBody", &kRigidBodyLayout));
  }
  template <uint32_t Stride = kSize>
  static RigidBody NewArray(uint32_t count) {
    static_assert(sizeof(RigidBodyData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RigidBodyData*>(::operator new(sizeof(RigidBodyData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RigidBodyData();
    return RigidBody(memory::MakeHandle(p, sizeof(RigidBodyData) * count, "RigidBody", &kRigidBodyLayout, sizeof(RigidBodyData)));
  }
  RigidBody at(uint32_t i) const { return RigidBody(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RigidBodyData* direct() const { return static_cast<RigidBodyData*>(memory::HandleOwn(address, &kRigidBodyLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRigidBodyLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRigidBodyLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // set: no inverse mass
  uint32_t fixed_address() const { return address + 0; }
  uint8_t fixed_bits() const { if (auto* d = direct()) return d->fixed; return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(address + 0))); }
  void set_fixed_bits(uint8_t v) const { if (auto* d = direct()) { d->fixed = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(address + 0, v); }
  uint8_t fixed() const { if (auto* d = direct()) return d->fixed; return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(address + 0))); }
  void set_fixed(uint8_t v) const { if (auto* d = direct()) { d->fixed = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(address + 0, v); }
  // Matrix::Rotate's and Quaternion::Multiply's (a vector they're passed: the measure's)
  uint32_t matrix_rotate_0_address() const { return address + 0; }
  uint32_t matrix_rotate_0_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kRigidBodyLayout, 0, 4)); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_matrix_rotate_0_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRigidBodyLayout, 0, 4, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t matrix_rotate_0() const { return uint32_t(matrix_rotate_0_bits()); }
  void set_matrix_rotate_0(uint32_t v) const { set_matrix_rotate_0_bits(uint32_t(v)); }
  // RigidBody::FrictionImpulse's copy of a vector (the measure's)
  uint32_t friction_0_address() const { return address + 0; }
  uint64_t friction_0_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kRigidBodyLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(address + 0))); }
  void set_friction_0_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRigidBodyLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(address + 0, v); }
  uint64_t friction_0() const { return uint64_t(friction_0_bits()); }
  void set_friction_0(uint64_t v) const { set_friction_0_bits(uint64_t(v)); }
  uint32_t mass_address() const { return address + 4; }
  uint32_t mass_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->mass); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_mass_bits(uint32_t v) const { if (auto* d = direct()) { d->mass = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float mass() const { return std::bit_cast<float>(mass_bits()); }
  void set_mass(float v) const { set_mass_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t inverse_mass_address() const { return address + 8; }
  uint32_t inverse_mass_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inverse_mass); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_inverse_mass_bits(uint32_t v) const { if (auto* d = direct()) { d->inverse_mass = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  float inverse_mass() const { return std::bit_cast<float>(inverse_mass_bits()); }
  void set_inverse_mass(float v) const { set_inverse_mass_bits(std::bit_cast<uint32_t>(v)); }
  // RigidBody::FrictionImpulse's copy of a vector (the measure's)
  uint32_t friction_8_address() const { return address + 8; }
  uint64_t friction_8_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kRigidBodyLayout, 8, 8)); return uint64_t((memory::IsHandle(address + 8) ? uint64_t(memory::OwnerLoad(address + 8, 8)) : guest::LoadU64(address + 8))); }
  void set_friction_8_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRigidBodyLayout, 8, 8, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 8, v); else guest::StoreU64(address + 8, v); }
  uint64_t friction_8() const { return uint64_t(friction_8_bits()); }
  void set_friction_8(uint64_t v) const { set_friction_8_bits(uint64_t(v)); }
  // a pointer to the vehicle's +3312
  uint32_t vehicle_matrix_address() const { return address + 12; }
  uint32_t vehicle_matrix_bits() const { if (auto* d = direct()) return d->vehicle_matrix; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_vehicle_matrix_bits(uint32_t v) const { if (auto* d = direct()) { d->vehicle_matrix = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t vehicle_matrix() const { if (auto* d = direct()) return d->vehicle_matrix; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_vehicle_matrix(uint32_t v) const { if (auto* d = direct()) { d->vehicle_matrix = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // in the world, w 0
  uint32_t center_of_mass_address() const { return address + 16; }
  uint8_t* center_of_mass() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 16 : guest::Pointer(address + 16); }
  uint32_t velocity_address() const { return address + 32; }
  uint8_t* velocity() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  uint32_t speed_cap_address() const { return address + 48; }
  uint32_t speed_cap_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->speed_cap); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_speed_cap_bits(uint32_t v) const { if (auto* d = direct()) { d->speed_cap = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float speed_cap() const { return std::bit_cast<float>(speed_cap_bits()); }
  void set_speed_cap(float v) const { set_speed_cap_bits(std::bit_cast<uint32_t>(v)); }
  // summed this step
  uint32_t force_address() const { return address + 64; }
  uint8_t* force() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 64 : guest::Pointer(address + 64); }
  // a quaternion
  uint32_t orientation_address() const { return address + 80; }
  uint8_t* orientation() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 80 : guest::Pointer(address + 80); }
  uint32_t angular_velocity_address() const { return address + 96; }
  uint8_t* angular_velocity() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 96 : guest::Pointer(address + 96); }
  uint32_t spin_cap_address() const { return address + 112; }
  uint32_t spin_cap_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->spin_cap); return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_spin_cap_bits(uint32_t v) const { if (auto* d = direct()) { d->spin_cap = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  float spin_cap() const { return std::bit_cast<float>(spin_cap_bits()); }
  void set_spin_cap(float v) const { set_spin_cap_bits(std::bit_cast<uint32_t>(v)); }
  // summed this step
  uint32_t torque_address() const { return address + 128; }
  uint8_t* torque() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 128 : guest::Pointer(address + 128); }
  uint32_t velocity_direction_address() const { return address + 144; }
  uint8_t* velocity_direction() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 144 : guest::Pointer(address + 144); }
  uint32_t speed_address() const { return address + 160; }
  uint32_t speed_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->speed); return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(address + 160))); }
  void set_speed_bits(uint32_t v) const { if (auto* d = direct()) { d->speed = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(address + 160, v); }
  float speed() const { return std::bit_cast<float>(speed_bits()); }
  void set_speed(float v) const { set_speed_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t linear_damping_address() const { return address + 164; }
  uint32_t linear_damping_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->linear_damping); return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(address + 164))); }
  void set_linear_damping_bits(uint32_t v) const { if (auto* d = direct()) { d->linear_damping = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(address + 164, v); }
  float linear_damping() const { return std::bit_cast<float>(linear_damping_bits()); }
  void set_linear_damping(float v) const { set_linear_damping_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t momentum_address() const { return address + 176; }
  uint8_t* momentum() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 176 : guest::Pointer(address + 176); }
  uint32_t center_of_mass_offset_address() const { return address + 192; }
  uint8_t* center_of_mass_offset() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 192 : guest::Pointer(address + 192); }
  uint32_t origin_address() const { return address + 208; }
  uint8_t* origin() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 208 : guest::Pointer(address + 208); }
  // its fourth row (+272) the center of mass
  uint32_t orientation_matrix_address() const { return address + 224; }
  uint8_t* orientation_matrix() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 224 : guest::Pointer(address + 224); }
  // the body's matrix (+336 its translation, the origin)
  uint32_t matrix_address() const { return address + 288; }
  uint8_t* matrix() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 288 : guest::Pointer(address + 288); }
  // the inertia's diagonal (+352 to +360)
  uint32_t inertia_address() const { return address + 352; }
  uint8_t* inertia() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 352 : guest::Pointer(address + 352); }
  // their reciprocals (+368 to +376)
  uint32_t inverse_inertia_address() const { return address + 368; }
  uint8_t* inverse_inertia() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 368 : guest::Pointer(address + 368); }
  uint32_t angular_damping_address() const { return address + 384; }
  uint32_t angular_damping_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->angular_damping); return uint32_t((memory::IsHandle(address + 384) ? uint32_t(memory::OwnerLoad(address + 384, 4)) : guest::LoadU32(address + 384))); }
  void set_angular_damping_bits(uint32_t v) const { if (auto* d = direct()) { d->angular_damping = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 384)) memory::OwnerStore(address + 384, 4, v); else guest::StoreU32(address + 384, v); }
  float angular_damping() const { return std::bit_cast<float>(angular_damping_bits()); }
  void set_angular_damping(float v) const { set_angular_damping_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t angular_momentum_address() const { return address + 400; }
  uint8_t* angular_momentum() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 400 : guest::Pointer(address + 400); }
  uint32_t world_inverse_inertia_address() const { return address + 416; }
  uint8_t* world_inverse_inertia() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 416 : guest::Pointer(address + 416); }
  uint32_t gravity_address() const { return address + 480; }
  uint8_t* gravity() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 480 : guest::Pointer(address + 480); }
  // gravity x the mass
  uint32_t weight_address() const { return address + 496; }
  uint8_t* weight() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 496 : guest::Pointer(address + 496); }
  // the last effective mass (RigidBody::ContactImpulse)
  uint32_t effective_mass_address() const { return address + 512; }
  uint32_t effective_mass_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->effective_mass); return uint32_t((memory::IsHandle(address + 512) ? uint32_t(memory::OwnerLoad(address + 512, 4)) : guest::LoadU32(address + 512))); }
  void set_effective_mass_bits(uint32_t v) const { if (auto* d = direct()) { d->effective_mass = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 512)) memory::OwnerStore(address + 512, 4, v); else guest::StoreU32(address + 512, v); }
  float effective_mass() const { return std::bit_cast<float>(effective_mass_bits()); }
  void set_effective_mass(float v) const { set_effective_mass_bits(std::bit_cast<uint32_t>(v)); }
  // bit 0 accelerating, bit 1 wheels on the ground
  uint32_t flags_address() const { return address + 516; }
  uint32_t flags_bits() const { if (auto* d = direct()) return d->flags; return uint32_t((memory::IsHandle(address + 516) ? uint32_t(memory::OwnerLoad(address + 516, 4)) : guest::LoadU32(address + 516))); }
  void set_flags_bits(uint32_t v) const { if (auto* d = direct()) { d->flags = v; return; } if (memory::IsHandle(address + 516)) memory::OwnerStore(address + 516, 4, v); else guest::StoreU32(address + 516, v); }
  uint32_t flags() const { if (auto* d = direct()) return d->flags; return uint32_t((memory::IsHandle(address + 516) ? uint32_t(memory::OwnerLoad(address + 516, 4)) : guest::LoadU32(address + 516))); }
  void set_flags(uint32_t v) const { if (auto* d = direct()) { d->flags = v; return; } if (memory::IsHandle(address + 516)) memory::OwnerStore(address + 516, 4, v); else guest::StoreU32(address + 516, v); }
};

// embedded; measure: RigidBody::Velocity.ret, RigidBody::AngularVelocity.ret, RigidBody::Force.ret, Contact::Position.ret, Matrix::MultiplyTranslation.r5, sub_820B6898.ret, RigidBody::SetInertia.r4
struct PhysicsVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(address + 0); }
  void set_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  // x and y copied as one doubleword
  uint64_t xy() const { return guest::LoadU64(address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(address + 4); }
  void set_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(address + 8); }
  void set_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // z and w copied as one doubleword
  uint64_t zw() const { return guest::LoadU64(address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(address + 12); }
  void set_w(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// embedded; measure: sub_820B6928.ret
struct PhysicsMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t row0_address() const { return address + 0; }
  uint32_t row1_address() const { return address + 16; }
  uint32_t row2_address() const { return address + 32; }
  // the translation
  uint32_t row3_address() const { return address + 48; }
};

// embedded; measure: RigidBody::SetFloat48.r4, Vehicle::SuspensionSpring.r6
struct PhysicsFloat : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float value() const { return guest::LoadFloat(address + 0); }
  void set_value(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// image; measure: Tire::MagicFormula.r4
struct TireCoefficients : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float b() const { return guest::LoadFloat(address + 0); }
  void set_b(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t b_address() const { return address + 0; }
  float c() const { return guest::LoadFloat(address + 4); }
  void set_c(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t c_address() const { return address + 4; }
  float d() const { return guest::LoadFloat(address + 8); }
  void set_d(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t d_address() const { return address + 8; }
  float e() const { return guest::LoadFloat(address + 12); }
  void set_e(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t e_address() const { return address + 12; }
};

// embedded; measure: CarData, PowerGraph::Draw.r6, RigidBody::EffectiveMassAt.r4
struct CarData : View {
  using View::View;
  static constexpr uint32_t kSize = 1844;
  // the body's mass (B1 +532)
  float mass() const { return guest::LoadFloat(address + 0); }
  void set_mass(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t mass_address() const { return address + 0; }
  // B1 +536
  float wheelbase() const { return guest::LoadFloat(address + 4); }
  void set_wheelbase(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t wheelbase_address() const { return address + 4; }
  // B1 +540
  float front_track() const { return guest::LoadFloat(address + 8); }
  void set_front_track(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t front_track_address() const { return address + 8; }
  // B1 +544
  float rear_track() const { return guest::LoadFloat(address + 12); }
  void set_rear_track(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t rear_track_address() const { return address + 12; }
  // probably the roll axis
  uint32_t roll_axis_address() const { return address + 16; }
  // B1 +16 to +24
  uint32_t inertia_factors_address() const { return address + 32; }
  // B1 +32
  float length() const { return guest::LoadFloat(address + 48); }
  void set_length(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t length_address() const { return address + 48; }
  float height() const { return guest::LoadFloat(address + 52); }
  void set_height(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t height_address() const { return address + 52; }
  float width() const { return guest::LoadFloat(address + 56); }
  void set_width(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t width_address() const { return address + 56; }
  // the w of +48
  float unknown_60() const { return guest::LoadFloat(address + 60); }
  void set_unknown_60(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t inertia_address() const { return address + 64; }
  // B1 +48 to +56
  uint32_t center_of_mass_offset_address() const { return address + 80; }
  // the center of mass on the ground
  uint32_t center_of_mass_ground_address() const { return address + 96; }
  // the center of mass in the air
  uint32_t center_of_mass_air_address() const { return address + 112; }
  // a ramp rate (B1 +308)
  float handbrake_ramp() const { return guest::LoadFloat(address + 128); }
  void set_handbrake_ramp(float v) const { guest::StoreFloat(address + 128, v); }
  uint32_t handbrake_ramp_address() const { return address + 128; }
  float front_brake_factor() const { return guest::LoadFloat(address + 132); }
  void set_front_brake_factor(float v) const { guest::StoreFloat(address + 132, v); }
  uint32_t front_brake_factor_address() const { return address + 132; }
  float rear_brake_factor() const { return guest::LoadFloat(address + 136); }
  void set_rear_brake_factor(float v) const { guest::StoreFloat(address + 136, v); }
  uint32_t rear_brake_factor_address() const { return address + 136; }
  // an ABS threshold
  float abs_front() const { return guest::LoadFloat(address + 140); }
  void set_abs_front(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t abs_front_address() const { return address + 140; }
  // an ABS threshold
  float abs_rear() const { return guest::LoadFloat(address + 144); }
  void set_abs_rear(float v) const { guest::StoreFloat(address + 144, v); }
  uint32_t abs_rear_address() const { return address + 144; }
  float unknown_148() const { return guest::LoadFloat(address + 148); }
  void set_unknown_148(float v) const { guest::StoreFloat(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  float unknown_152() const { return guest::LoadFloat(address + 152); }
  void set_unknown_152(float v) const { guest::StoreFloat(address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  float unknown_156() const { return guest::LoadFloat(address + 156); }
  void set_unknown_156(float v) const { guest::StoreFloat(address + 156, v); }
  uint32_t unknown_156_address() const { return address + 156; }
  float unknown_160() const { return guest::LoadFloat(address + 160); }
  void set_unknown_160(float v) const { guest::StoreFloat(address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  float brake_heat_limit() const { return guest::LoadFloat(address + 164); }
  void set_brake_heat_limit(float v) const { guest::StoreFloat(address + 164, v); }
  uint32_t brake_heat_limit_address() const { return address + 164; }
  float brake_heat_start() const { return guest::LoadFloat(address + 168); }
  void set_brake_heat_start(float v) const { guest::StoreFloat(address + 168, v); }
  uint32_t brake_heat_start_address() const { return address + 168; }
  // B1 +356: 0 front, 1 rear, 2 and up all-wheel
  uint32_t drive_type() const { return guest::LoadU32(address + 172); }
  void set_drive_type(uint32_t v) const { guest::StoreU32(address + 172, v); }
  uint32_t drive_type_address() const { return address + 172; }
  // an index into 0x8260181C (B1 +360)
  uint32_t axle_table_index() const { return guest::LoadU32(address + 176); }
  void set_axle_table_index(uint32_t v) const { guest::StoreU32(address + 176, v); }
  uint32_t axle_table_index_address() const { return address + 176; }
  // B1 +364
  float front_torque_share() const { return guest::LoadFloat(address + 180); }
  void set_front_torque_share(float v) const { guest::StoreFloat(address + 180, v); }
  uint32_t front_torque_share_address() const { return address + 180; }
  // +184 to +260 from 0x8260181C's record
  uint32_t axle_table_record_address() const { return address + 184; }
  float front_brake() const { return guest::LoadFloat(address + 264); }
  void set_front_brake(float v) const { guest::StoreFloat(address + 264, v); }
  uint32_t front_brake_address() const { return address + 264; }
  float rear_brake() const { return guest::LoadFloat(address + 268); }
  void set_rear_brake(float v) const { guest::StoreFloat(address + 268, v); }
  uint32_t rear_brake_address() const { return address + 268; }
  float unknown_272() const { return guest::LoadFloat(address + 272); }
  void set_unknown_272(float v) const { guest::StoreFloat(address + 272, v); }
  uint32_t unknown_272_address() const { return address + 272; }
  // from B1 +324
  uint32_t unknown_276_address() const { return address + 276; }
  float unknown_312() const { return guest::LoadFloat(address + 312); }
  void set_unknown_312(float v) const { guest::StoreFloat(address + 312, v); }
  uint32_t unknown_312_address() const { return address + 312; }
  // steering (B1 +616)
  float steering_316() const { return guest::LoadFloat(address + 316); }
  void set_steering_316(float v) const { guest::StoreFloat(address + 316, v); }
  uint32_t steering_316_address() const { return address + 316; }
  // steering
  float steering_320() const { return guest::LoadFloat(address + 320); }
  void set_steering_320(float v) const { guest::StoreFloat(address + 320, v); }
  uint32_t steering_320_address() const { return address + 320; }
  float max_steering_angle() const { return guest::LoadFloat(address + 324); }
  void set_max_steering_angle(float v) const { guest::StoreFloat(address + 324, v); }
  uint32_t max_steering_angle_address() const { return address + 324; }
  // steering
  float steering_328() const { return guest::LoadFloat(address + 328); }
  void set_steering_328(float v) const { guest::StoreFloat(address + 328, v); }
  uint32_t steering_328_address() const { return address + 328; }
  float unknown_332() const { return guest::LoadFloat(address + 332); }
  void set_unknown_332(float v) const { guest::StoreFloat(address + 332, v); }
  uint32_t unknown_332_address() const { return address + 332; }
  // a CarDataAxle
  uint32_t front_axle_address() const { return address + 336; }
  // a CarDataAxle
  uint32_t rear_axle_address() const { return address + 608; }
  // four 96-byte wheel set-up records (+0 the axle's position, +16 the wheel's point, +32 half the track, +36 and +40 the camber's sin and cos, +48 a quaternion)
  uint32_t wheel_setups_address() const { return address + 880; }
  // a ramp rate (B1 +556)
  float reverse_ramp() const { return guest::LoadFloat(address + 1264); }
  void set_reverse_ramp(float v) const { guest::StoreFloat(address + 1264, v); }
  uint32_t reverse_ramp_address() const { return address + 1264; }
  // B1 +368
  uint32_t gear_count() const { return guest::LoadU32(address + 1268); }
  void set_gear_count(uint32_t v) const { guest::StoreU32(address + 1268, v); }
  uint32_t gear_count_address() const { return address + 1268; }
  // probably (E +12)
  float max_rpm() const { return guest::LoadFloat(address + 1272); }
  void set_max_rpm(float v) const { guest::StoreFloat(address + 1272, v); }
  uint32_t max_rpm_address() const { return address + 1272; }
  float unknown_1276() const { return guest::LoadFloat(address + 1276); }
  void set_unknown_1276(float v) const { guest::StoreFloat(address + 1276, v); }
  uint32_t unknown_1276_address() const { return address + 1276; }
  // E +20
  float rev_limit() const { return guest::LoadFloat(address + 1280); }
  void set_rev_limit(float v) const { guest::StoreFloat(address + 1280, v); }
  uint32_t rev_limit_address() const { return address + 1280; }
  float rev_limit_normalized() const { return guest::LoadFloat(address + 1284); }
  void set_rev_limit_normalized(float v) const { guest::StoreFloat(address + 1284, v); }
  uint32_t rev_limit_normalized_address() const { return address + 1284; }
  // B2 +8
  float displacement() const { return guest::LoadFloat(address + 1288); }
  void set_displacement(float v) const { guest::StoreFloat(address + 1288, v); }
  uint32_t displacement_address() const { return address + 1288; }
  // 32 floats (E +28)
  uint32_t torque_curve_address() const { return address + 1292; }
  // E +24
  float torque() const { return guest::LoadFloat(address + 1420); }
  void set_torque(float v) const { guest::StoreFloat(address + 1420, v); }
  uint32_t torque_address() const { return address + 1420; }
  float torque_copy() const { return guest::LoadFloat(address + 1424); }
  void set_torque_copy(float v) const { guest::StoreFloat(address + 1424, v); }
  uint32_t torque_copy_address() const { return address + 1424; }
  // the gear ratios, reverse first (B1 +384)
  uint32_t gear_ratios_address() const { return address + 1428; }
  // a per-gear table (B1 +420)
  uint32_t gear_table_2_address() const { return address + 1464; }
  // a per-gear table (B1 +456)
  uint32_t gear_table_3_address() const { return address + 1500; }
  // B1 +372
  float final_drive() const { return guest::LoadFloat(address + 1536); }
  void set_final_drive(float v) const { guest::StoreFloat(address + 1536, v); }
  uint32_t final_drive_address() const { return address + 1536; }
  float unknown_1540() const { return guest::LoadFloat(address + 1540); }
  void set_unknown_1540(float v) const { guest::StoreFloat(address + 1540, v); }
  uint32_t unknown_1540_address() const { return address + 1540; }
  float unknown_1544() const { return guest::LoadFloat(address + 1544); }
  void set_unknown_1544(float v) const { guest::StoreFloat(address + 1544, v); }
  uint32_t unknown_1544_address() const { return address + 1544; }
  // inertia
  float engine_inertia() const { return guest::LoadFloat(address + 1548); }
  void set_engine_inertia(float v) const { guest::StoreFloat(address + 1548, v); }
  uint32_t engine_inertia_address() const { return address + 1548; }
  float engine_braking() const { return guest::LoadFloat(address + 1552); }
  void set_engine_braking(float v) const { guest::StoreFloat(address + 1552, v); }
  uint32_t engine_braking_address() const { return address + 1552; }
  float engine_braking_2() const { return guest::LoadFloat(address + 1556); }
  void set_engine_braking_2(float v) const { guest::StoreFloat(address + 1556, v); }
  uint32_t engine_braking_2_address() const { return address + 1556; }
  // inertia
  float inertia_2() const { return guest::LoadFloat(address + 1560); }
  void set_inertia_2(float v) const { guest::StoreFloat(address + 1560, v); }
  uint32_t inertia_2_address() const { return address + 1560; }
  // the ratios again
  uint32_t gear_ratios_copy_address() const { return address + 1564; }
  float final_drive_copy() const { return guest::LoadFloat(address + 1600); }
  void set_final_drive_copy(float v) const { guest::StoreFloat(address + 1600, v); }
  uint32_t final_drive_copy_address() const { return address + 1600; }
  // E +172
  uint8_t turbo() const { return guest::LoadU8(address + 1604); }
  void set_turbo(uint8_t v) const { guest::StoreU8(address + 1604, v); }
  uint32_t turbo_address() const { return address + 1604; }
  // E +192
  uint8_t blow_off() const { return guest::LoadU8(address + 1605); }
  void set_blow_off(uint8_t v) const { guest::StoreU8(address + 1605, v); }
  uint32_t blow_off_address() const { return address + 1605; }
  float unknown_1608() const { return guest::LoadFloat(address + 1608); }
  void set_unknown_1608(float v) const { guest::StoreFloat(address + 1608, v); }
  uint32_t unknown_1608_address() const { return address + 1608; }
  // the boost (E +176 to +188)
  float boost_1612() const { return guest::LoadFloat(address + 1612); }
  void set_boost_1612(float v) const { guest::StoreFloat(address + 1612, v); }
  uint32_t boost_1612_address() const { return address + 1612; }
  float boost_1616() const { return guest::LoadFloat(address + 1616); }
  void set_boost_1616(float v) const { guest::StoreFloat(address + 1616, v); }
  uint32_t boost_1616_address() const { return address + 1616; }
  float boost_1620() const { return guest::LoadFloat(address + 1620); }
  void set_boost_1620(float v) const { guest::StoreFloat(address + 1620, v); }
  uint32_t boost_1620_address() const { return address + 1620; }
  float unknown_1624() const { return guest::LoadFloat(address + 1624); }
  void set_unknown_1624(float v) const { guest::StoreFloat(address + 1624, v); }
  uint32_t unknown_1624_address() const { return address + 1624; }
  // B1 +600, normalized
  float idle_rpm() const { return guest::LoadFloat(address + 1628); }
  void set_idle_rpm(float v) const { guest::StoreFloat(address + 1628, v); }
  uint32_t idle_rpm_address() const { return address + 1628; }
  // the shift times (B1 +492)
  uint32_t shift_times_address() const { return address + 1632; }
  // their inverses
  uint32_t shift_rates_address() const { return address + 1668; }
  float unknown_1704() const { return guest::LoadFloat(address + 1704); }
  void set_unknown_1704(float v) const { guest::StoreFloat(address + 1704, v); }
  uint32_t unknown_1704_address() const { return address + 1704; }
  float unknown_1708() const { return guest::LoadFloat(address + 1708); }
  void set_unknown_1708(float v) const { guest::StoreFloat(address + 1708, v); }
  uint32_t unknown_1708_address() const { return address + 1708; }
  float unknown_1712() const { return guest::LoadFloat(address + 1712); }
  void set_unknown_1712(float v) const { guest::StoreFloat(address + 1712, v); }
  uint32_t unknown_1712_address() const { return address + 1712; }
  float unknown_1716() const { return guest::LoadFloat(address + 1716); }
  void set_unknown_1716(float v) const { guest::StoreFloat(address + 1716, v); }
  uint32_t unknown_1716_address() const { return address + 1716; }
  // B1 +224
  float front_spring() const { return guest::LoadFloat(address + 1720); }
  void set_front_spring(float v) const { guest::StoreFloat(address + 1720, v); }
  uint32_t front_spring_address() const { return address + 1720; }
  float front_damping() const { return guest::LoadFloat(address + 1724); }
  void set_front_damping(float v) const { guest::StoreFloat(address + 1724, v); }
  uint32_t front_damping_address() const { return address + 1724; }
  float front_damping_2() const { return guest::LoadFloat(address + 1728); }
  void set_front_damping_2(float v) const { guest::StoreFloat(address + 1728, v); }
  uint32_t front_damping_2_address() const { return address + 1728; }
  float front_anti_roll() const { return guest::LoadFloat(address + 1732); }
  void set_front_anti_roll(float v) const { guest::StoreFloat(address + 1732, v); }
  uint32_t front_anti_roll_address() const { return address + 1732; }
  float front_height() const { return guest::LoadFloat(address + 1736); }
  void set_front_height(float v) const { guest::StoreFloat(address + 1736, v); }
  uint32_t front_height_address() const { return address + 1736; }
  float rear_spring() const { return guest::LoadFloat(address + 1740); }
  void set_rear_spring(float v) const { guest::StoreFloat(address + 1740, v); }
  uint32_t rear_spring_address() const { return address + 1740; }
  float rear_damping() const { return guest::LoadFloat(address + 1744); }
  void set_rear_damping(float v) const { guest::StoreFloat(address + 1744, v); }
  uint32_t rear_damping_address() const { return address + 1744; }
  float rear_damping_2() const { return guest::LoadFloat(address + 1748); }
  void set_rear_damping_2(float v) const { guest::StoreFloat(address + 1748, v); }
  uint32_t rear_damping_2_address() const { return address + 1748; }
  float rear_anti_roll() const { return guest::LoadFloat(address + 1752); }
  void set_rear_anti_roll(float v) const { guest::StoreFloat(address + 1752, v); }
  uint32_t rear_anti_roll_address() const { return address + 1752; }
  float rear_height() const { return guest::LoadFloat(address + 1756); }
  void set_rear_height(float v) const { guest::StoreFloat(address + 1756, v); }
  uint32_t rear_height_address() const { return address + 1756; }
  // the collision box's (B1 +64)
  uint32_t box_1760_address() const { return address + 1760; }
  // the collision box's (B1 +80)
  uint32_t box_1776_address() const { return address + 1776; }
  float box_value_1792() const { return guest::LoadFloat(address + 1792); }
  void set_box_value_1792(float v) const { guest::StoreFloat(address + 1792, v); }
  uint32_t box_value_1792_address() const { return address + 1792; }
  float box_value_1796() const { return guest::LoadFloat(address + 1796); }
  void set_box_value_1796(float v) const { guest::StoreFloat(address + 1796, v); }
  uint32_t box_value_1796_address() const { return address + 1796; }
  // drag and downforce: the aero parts' factor x the frontal area
  float aero_front() const { return guest::LoadFloat(address + 1800); }
  void set_aero_front(float v) const { guest::StoreFloat(address + 1800, v); }
  uint32_t aero_front_address() const { return address + 1800; }
  // x the side area
  float aero_side() const { return guest::LoadFloat(address + 1804); }
  void set_aero_side(float v) const { guest::StoreFloat(address + 1804, v); }
  uint32_t aero_side_address() const { return address + 1804; }
  float unknown_1808() const { return guest::LoadFloat(address + 1808); }
  void set_unknown_1808(float v) const { guest::StoreFloat(address + 1808, v); }
  uint32_t unknown_1808_address() const { return address + 1808; }
  float unknown_1812() const { return guest::LoadFloat(address + 1812); }
  void set_unknown_1812(float v) const { guest::StoreFloat(address + 1812, v); }
  uint32_t unknown_1812_address() const { return address + 1812; }
  float unknown_1816() const { return guest::LoadFloat(address + 1816); }
  void set_unknown_1816(float v) const { guest::StoreFloat(address + 1816, v); }
  uint32_t unknown_1816_address() const { return address + 1816; }
  // +1820 to +1828 load transfer
  uint32_t load_transfer_address() const { return address + 1820; }
  float unknown_1832() const { return guest::LoadFloat(address + 1832); }
  void set_unknown_1832(float v) const { guest::StoreFloat(address + 1832, v); }
  uint32_t unknown_1832_address() const { return address + 1832; }
  // probably: x the mass (Vehicle::SetUpPhysics)
  float front_mass_share() const { return guest::LoadFloat(address + 1836); }
  void set_front_mass_share(float v) const { guest::StoreFloat(address + 1836, v); }
  uint32_t front_mass_share_address() const { return address + 1836; }
  // probably: x the mass (Vehicle::SetUpPhysics)
  float rear_mass_share() const { return guest::LoadFloat(address + 1840); }
  void set_rear_mass_share(float v) const { guest::StoreFloat(address + 1840, v); }
  uint32_t rear_mass_share_address() const { return address + 1840; }
};

// embedded; measure: -
struct CarDataAxle : View {
  using View::View;
  static constexpr uint32_t kSize = 272;
  uint32_t position_address() const { return address + 0; }
  // 2.0 x s0 / s4
  float tire_16() const { return guest::LoadFloat(address + 16); }
  void set_tire_16(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t tire_16_address() const { return address + 16; }
  // s0 / s4 squared
  float tire_20() const { return guest::LoadFloat(address + 20); }
  void set_tire_20(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t tire_20_address() const { return address + 20; }
  float tire_24() const { return guest::LoadFloat(address + 24); }
  void set_tire_24(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t tire_24_address() const { return address + 24; }
  float tire_28() const { return guest::LoadFloat(address + 28); }
  void set_tire_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t tire_28_address() const { return address + 28; }
  uint32_t tire_32_address() const { return address + 32; }
  // a factor of the camber's change (Vehicle::UpdateWheelAxes)
  float unknown_48() const { return guest::LoadFloat(address + 48); }
  void set_unknown_48(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  float radius() const { return guest::LoadFloat(address + 52); }
  void set_radius(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t radius_address() const { return address + 52; }
  float inverse_radius() const { return guest::LoadFloat(address + 56); }
  void set_inverse_radius(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t inverse_radius_address() const { return address + 56; }
  // the tire's width
  float width() const { return guest::LoadFloat(address + 60); }
  void set_width(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t width_address() const { return address + 60; }
  // probably
  float wheel_mass() const { return guest::LoadFloat(address + 64); }
  void set_wheel_mass(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t wheel_mass_address() const { return address + 64; }
  uint32_t point_address() const { return address + 80; }
  // the suspension sub-record
  uint32_t suspension_address() const { return address + 96; }
  // added to the suspension's force (Vehicle::UpdateWheelSuspension)
  float unknown_128() const { return guest::LoadFloat(address + 128); }
  void set_unknown_128(float v) const { guest::StoreFloat(address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  // CarData::Derive's
  float unknown_132() const { return guest::LoadFloat(address + 132); }
  void set_unknown_132(float v) const { guest::StoreFloat(address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  // the axle's +256 less the car data's +84 (CarData::Derive)
  float ride_offset() const { return guest::LoadFloat(address + 136); }
  void set_ride_offset(float v) const { guest::StoreFloat(address + 136, v); }
  uint32_t ride_offset_address() const { return address + 136; }
  float unknown_140() const { return guest::LoadFloat(address + 140); }
  void set_unknown_140(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  float unknown_144() const { return guest::LoadFloat(address + 144); }
  void set_unknown_144(float v) const { guest::StoreFloat(address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  // the grip's factor (Vehicle::UpdateTireGrip)
  float unknown_148() const { return guest::LoadFloat(address + 148); }
  void set_unknown_148(float v) const { guest::StoreFloat(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  float unknown_152() const { return guest::LoadFloat(address + 152); }
  void set_unknown_152(float v) const { guest::StoreFloat(address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  float unknown_160() const { return guest::LoadFloat(address + 160); }
  void set_unknown_160(float v) const { guest::StoreFloat(address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  float unknown_164() const { return guest::LoadFloat(address + 164); }
  void set_unknown_164(float v) const { guest::StoreFloat(address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  uint32_t unknown_180_address() const { return address + 180; }
  // probably
  float wheel_inertia() const { return guest::LoadFloat(address + 200); }
  void set_wheel_inertia(float v) const { guest::StoreFloat(address + 200, v); }
  uint32_t wheel_inertia_address() const { return address + 200; }
  // +204 the camber
  uint32_t angles_address() const { return address + 204; }
  float unknown_220() const { return guest::LoadFloat(address + 220); }
  void set_unknown_220(float v) const { guest::StoreFloat(address + 220, v); }
  uint32_t unknown_220_address() const { return address + 220; }
  float weight_share() const { return guest::LoadFloat(address + 232); }
  void set_weight_share(float v) const { guest::StoreFloat(address + 232, v); }
  uint32_t weight_share_address() const { return address + 232; }
  float effective_mass() const { return guest::LoadFloat(address + 240); }
  void set_effective_mass(float v) const { guest::StoreFloat(address + 240, v); }
  uint32_t effective_mass_address() const { return address + 240; }
  float unknown_256() const { return guest::LoadFloat(address + 256); }
  void set_unknown_256(float v) const { guest::StoreFloat(address + 256, v); }
  uint32_t unknown_256_address() const { return address + 256; }
  // with the throttle (Vehicle::UpdateDrivenAxle)
  float unknown_260() const { return guest::LoadFloat(address + 260); }
  void set_unknown_260(float v) const { guest::StoreFloat(address + 260, v); }
  uint32_t unknown_260_address() const { return address + 260; }
  // without it
  float unknown_264() const { return guest::LoadFloat(address + 264); }
  void set_unknown_264(float v) const { guest::StoreFloat(address + 264, v); }
  uint32_t unknown_264_address() const { return address + 264; }
  // a preload (Vehicle::UpdateDrivenAxle)
  float unknown_268() const { return guest::LoadFloat(address + 268); }
  void set_unknown_268(float v) const { guest::StoreFloat(address + 268, v); }
  uint32_t unknown_268_address() const { return address + 268; }
};

// frame; measure: CarData::FillAxle.r4
struct CarDataTireSource : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  float s0() const { return guest::LoadFloat(address + 0); }
  void set_s0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t s0_address() const { return address + 0; }
  float s4() const { return guest::LoadFloat(address + 4); }
  void set_s4(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t s4_address() const { return address + 4; }
  float s8() const { return guest::LoadFloat(address + 8); }
  void set_s8(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t s8_address() const { return address + 8; }
  float s16() const { return guest::LoadFloat(address + 16); }
  void set_s16(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t s16_address() const { return address + 16; }
  float s20() const { return guest::LoadFloat(address + 20); }
  void set_s20(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t s20_address() const { return address + 20; }
  float s24() const { return guest::LoadFloat(address + 24); }
  void set_s24(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t s24_address() const { return address + 24; }
  float radius() const { return guest::LoadFloat(address + 32); }
  void set_radius(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t radius_address() const { return address + 32; }
  float width() const { return guest::LoadFloat(address + 36); }
  void set_width(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t width_address() const { return address + 36; }
  // probably
  float mass() const { return guest::LoadFloat(address + 40); }
  void set_mass(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t mass_address() const { return address + 40; }
  float s44() const { return guest::LoadFloat(address + 44); }
  void set_s44(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t s44_address() const { return address + 44; }
  float s48() const { return guest::LoadFloat(address + 48); }
  void set_s48(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t s48_address() const { return address + 48; }
  float s52() const { return guest::LoadFloat(address + 52); }
  void set_s52(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t s52_address() const { return address + 52; }
  float s56() const { return guest::LoadFloat(address + 56); }
  void set_s56(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t s56_address() const { return address + 56; }
  float s60() const { return guest::LoadFloat(address + 60); }
  void set_s60(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t s60_address() const { return address + 60; }
  float s64() const { return guest::LoadFloat(address + 64); }
  void set_s64(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t s64_address() const { return address + 64; }
};

// allocated; measure: Vehicle::ApplyTuning.r4, Vehicle::SetupFromCar.r4
struct PhysicsCar : View {
  using View::View;
  static constexpr uint32_t kSize = 3892;
  float final_drive() const { return guest::LoadFloat(address + 36); }
  void set_final_drive(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t final_drive_address() const { return address + 36; }
  // +40 to +68
  uint32_t gear_ratios_address() const { return address + 40; }
  // signed setting bytes (+72 to +94)
  uint32_t settings_address() const { return address + 72; }
  // the tuning levels (signed words from +1648)
  uint32_t tuning_address() const { return address + 1648; }
  // 28-byte records
  uint32_t aero_parts_address() const { return address + 3636; }
  // two 36-byte records (+3804, +3840)
  uint32_t aero_records_address() const { return address + 3804; }
  float front_axle_z() const { return guest::LoadFloat(address + 3876); }
  void set_front_axle_z(float v) const { guest::StoreFloat(address + 3876, v); }
  uint32_t front_axle_z_address() const { return address + 3876; }
  float rear_axle_z() const { return guest::LoadFloat(address + 3880); }
  void set_rear_axle_z(float v) const { guest::StoreFloat(address + 3880, v); }
  uint32_t rear_axle_z_address() const { return address + 3880; }
  uint32_t flags() const { return guest::LoadU32(address + 3888); }
  void set_flags(uint32_t v) const { guest::StoreU32(address + 3888, v); }
  uint32_t flags_address() const { return address + 3888; }
};

// allocated; measure: new<72>@CollisionWorld::Create, CollisionObject::Link.r4, CollisionObject::Unlink.r4
#pragma pack(push, 1)
struct CollisionWorldData {
  uint32_t buffer = 0;  // +0
  float default_step = 0;  // +4
  uint32_t ground_callback = 0;  // +8
  uint32_t object_count = 0;  // +12
  uint32_t objects = 0;  // +16
  uint32_t active = 0;  // +20
  uint32_t free = 0;  // +24
  uint32_t active_count = 0;  // +28
  uint32_t pair_capacity = 0;  // +32
  uint32_t pairs = 0;  // +36
  uint32_t pair_count = 0;  // +40
  uint32_t contact_capacity = 0;  // +44
  uint32_t contacts = 0;  // +48
  uint32_t contacts_used = 0;  // +52
  uint32_t updating = 0;  // +56
  uint32_t query_count = 0;  // +60
  uint32_t hits = 0;  // +64
  uint32_t merged_hits = 0;  // +68
};
#pragma pack(pop)
static_assert(sizeof(CollisionWorldData) == 72);
inline constexpr memory::LayoutField kCollisionWorldFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}};
inline constexpr memory::Layout kCollisionWorldLayout = {kCollisionWorldFields, 18};

struct CollisionWorld : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CollisionWorldData* data() const { return static_cast<CollisionWorldData*>(memory::HandleObject(address)); }
  static CollisionWorld New() {
    return CollisionWorld(memory::MakeHandle(new CollisionWorldData(), sizeof(CollisionWorldData), "CollisionWorld", &kCollisionWorldLayout));
  }
  template <uint32_t Stride = kSize>
  static CollisionWorld NewArray(uint32_t count) {
    static_assert(sizeof(CollisionWorldData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CollisionWorldData*>(::operator new(sizeof(CollisionWorldData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CollisionWorldData();
    return CollisionWorld(memory::MakeHandle(p, sizeof(CollisionWorldData) * count, "CollisionWorld", &kCollisionWorldLayout, sizeof(CollisionWorldData)));
  }
  CollisionWorld at(uint32_t i) const { return CollisionWorld(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CollisionWorldData* direct() const { return static_cast<CollisionWorldData*>(memory::HandleOwn(address, &kCollisionWorldLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCollisionWorldLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCollisionWorldLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a caller's buffer (0)
  uint32_t buffer_address() const { return address + 0; }
  uint32_t buffer_bits() const { if (auto* d = direct()) return d->buffer; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->buffer = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t buffer() const { if (auto* d = direct()) return d->buffer; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_buffer(uint32_t v) const { if (auto* d = direct()) { d->buffer = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // 1/30
  uint32_t default_step_address() const { return address + 4; }
  uint32_t default_step_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->default_step); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_default_step_bits(uint32_t v) const { if (auto* d = direct()) { d->default_step = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float default_step() const { return std::bit_cast<float>(default_step_bits()); }
  void set_default_step(float v) const { set_default_step_bits(std::bit_cast<uint32_t>(v)); }
  // 0x820FEAB0 in play
  uint32_t ground_callback_address() const { return address + 8; }
  uint32_t ground_callback_bits() const { if (auto* d = direct()) return d->ground_callback; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_ground_callback_bits(uint32_t v) const { if (auto* d = direct()) { d->ground_callback = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t ground_callback() const { if (auto* d = direct()) return d->ground_callback; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_ground_callback(uint32_t v) const { if (auto* d = direct()) { d->ground_callback = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // 50
  uint32_t object_count_address() const { return address + 12; }
  uint32_t object_count_bits() const { if (auto* d = direct()) return d->object_count; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_object_count_bits(uint32_t v) const { if (auto* d = direct()) { d->object_count = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t object_count() const { if (auto* d = direct()) return d->object_count; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_object_count(uint32_t v) const { if (auto* d = direct()) { d->object_count = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the 320-byte collision objects (a count header at -16)
  uint32_t objects_address() const { return address + 16; }
  uint32_t objects_bits() const { if (auto* d = direct()) return d->objects; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_objects_bits(uint32_t v) const { if (auto* d = direct()) { d->objects = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  template <typename T = CollisionWorldObject>
  T objects() const { return T(objects_bits()); }
  void set_objects(const View& w) const { set_objects_bits(w.address); }
  void set_objects(uint32_t v) const { set_objects_bits(v); }
  // the active list
  uint32_t active_address() const { return address + 20; }
  uint32_t active_bits() const { if (auto* d = direct()) return d->active; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_active_bits(uint32_t v) const { if (auto* d = direct()) { d->active = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  template <typename T = CollisionWorldObject>
  T active() const { return T(active_bits()); }
  void set_active(const View& w) const { set_active_bits(w.address); }
  void set_active(uint32_t v) const { set_active_bits(v); }
  // the free list
  uint32_t free_address() const { return address + 24; }
  uint32_t free_bits() const { if (auto* d = direct()) return d->free; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_free_bits(uint32_t v) const { if (auto* d = direct()) { d->free = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  template <typename T = CollisionWorldObject>
  T free() const { return T(free_bits()); }
  void set_free(const View& w) const { set_free_bits(w.address); }
  void set_free(uint32_t v) const { set_free_bits(v); }
  uint32_t active_count_address() const { return address + 28; }
  uint32_t active_count_bits() const { if (auto* d = direct()) return d->active_count; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_active_count_bits(uint32_t v) const { if (auto* d = direct()) { d->active_count = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t active_count() const { if (auto* d = direct()) return d->active_count; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_active_count(uint32_t v) const { if (auto* d = direct()) { d->active_count = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  // 1,000; never checked
  uint32_t pair_capacity_address() const { return address + 32; }
  uint32_t pair_capacity_bits() const { if (auto* d = direct()) return d->pair_capacity; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_pair_capacity_bits(uint32_t v) const { if (auto* d = direct()) { d->pair_capacity = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t pair_capacity() const { if (auto* d = direct()) return d->pair_capacity; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_pair_capacity(uint32_t v) const { if (auto* d = direct()) { d->pair_capacity = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t pairs_address() const { return address + 36; }
  uint32_t pairs_bits() const { if (auto* d = direct()) return d->pairs; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_pairs_bits(uint32_t v) const { if (auto* d = direct()) { d->pairs = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  template <typename T = CollisionPair>
  T pairs() const { return T(pairs_bits()); }
  void set_pairs(const View& w) const { set_pairs_bits(w.address); }
  void set_pairs(uint32_t v) const { set_pairs_bits(v); }
  uint32_t pair_count_address() const { return address + 40; }
  uint32_t pair_count_bits() const { if (auto* d = direct()) return d->pair_count; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_pair_count_bits(uint32_t v) const { if (auto* d = direct()) { d->pair_count = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t pair_count() const { if (auto* d = direct()) return d->pair_count; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_pair_count(uint32_t v) const { if (auto* d = direct()) { d->pair_count = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // 100
  uint32_t contact_capacity_address() const { return address + 44; }
  uint32_t contact_capacity_bits() const { if (auto* d = direct()) return d->contact_capacity; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_contact_capacity_bits(uint32_t v) const { if (auto* d = direct()) { d->contact_capacity = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t contact_capacity() const { if (auto* d = direct()) return d->contact_capacity; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_contact_capacity(uint32_t v) const { if (auto* d = direct()) { d->contact_capacity = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t contacts_address() const { return address + 48; }
  uint32_t contacts_bits() const { if (auto* d = direct()) return d->contacts; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_contacts_bits(uint32_t v) const { if (auto* d = direct()) { d->contacts = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  template <typename T = CollisionContact>
  T contacts() const { return T(contacts_bits()); }
  void set_contacts(const View& w) const { set_contacts_bits(w.address); }
  void set_contacts(uint32_t v) const { set_contacts_bits(v); }
  uint32_t contacts_used_address() const { return address + 52; }
  uint32_t contacts_used_bits() const { if (auto* d = direct()) return d->contacts_used; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_contacts_used_bits(uint32_t v) const { if (auto* d = direct()) { d->contacts_used = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t contacts_used() const { if (auto* d = direct()) return d->contacts_used; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_contacts_used(uint32_t v) const { if (auto* d = direct()) { d->contacts_used = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // 1 during CollisionWorld::Update
  uint32_t updating_address() const { return address + 56; }
  uint32_t updating_bits() const { if (auto* d = direct()) return d->updating; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_updating_bits(uint32_t v) const { if (auto* d = direct()) { d->updating = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t updating() const { if (auto* d = direct()) return d->updating; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_updating(uint32_t v) const { if (auto* d = direct()) { d->updating = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  // n (32)
  uint32_t query_count_address() const { return address + 60; }
  uint32_t query_count_bits() const { if (auto* d = direct()) return d->query_count; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_query_count_bits(uint32_t v) const { if (auto* d = direct()) { d->query_count = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t query_count() const { if (auto* d = direct()) return d->query_count; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_query_count(uint32_t v) const { if (auto* d = direct()) { d->query_count = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // 2n hit records (the raw hits)
  uint32_t hits_address() const { return address + 64; }
  uint32_t hits_bits() const { if (auto* d = direct()) return d->hits; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_hits_bits(uint32_t v) const { if (auto* d = direct()) { d->hits = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  template <typename T = CollisionHit>
  T hits() const { return T(hits_bits()); }
  void set_hits(const View& w) const { set_hits_bits(w.address); }
  void set_hits(uint32_t v) const { set_hits_bits(v); }
  // their second half (the merged hits)
  uint32_t merged_hits_address() const { return address + 68; }
  uint32_t merged_hits_bits() const { if (auto* d = direct()) return d->merged_hits; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_merged_hits_bits(uint32_t v) const { if (auto* d = direct()) { d->merged_hits = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  template <typename T = CollisionHit>
  T merged_hits() const { return T(merged_hits_bits()); }
  void set_merged_hits(const View& w) const { set_merged_hits_bits(w.address); }
  void set_merged_hits(uint32_t v) const { set_merged_hits_bits(v); }
};

// allocated; measure: new<4294967295>@CollisionWorld::Create
#pragma pack(push, 1)
struct CollisionObjectBlockData {
  uint32_t count = 0;  // +0
  uint8_t pad_4[12] = {};
  uint8_t objects[16000] = {};  // +16, 50 x CollisionWorldObject
};
#pragma pack(pop)
static_assert(sizeof(CollisionObjectBlockData) == 16016);
inline constexpr memory::LayoutField kCollisionObjectBlockFields[] = {{0, 4}, {16, 4}, {32, 4}, {36, 4}, {160, 4}, {208, 4}, {240, 4}, {244, 4}, {248, 4}, {252, 4}, {256, 4}, {260, 4}, {264, 4}, {268, 4}, {272, 4}, {276, 4}, {280, 4}, {284, 4}, {288, 4}, {292, 4}, {296, 4}, {300, 4}, {304, 4}, {336, 4}, {352, 4}, {356, 4}, {480, 4}, {528, 4}, {560, 4}, {564, 4}, {568, 4}, {572, 4}, {576, 4}, {580, 4}, {584, 4}, {588, 4}, {592, 4}, {596, 4}, {600, 4}, {604, 4}, {608, 4}, {612, 4}, {616, 4}, {620, 4}, {624, 4}, {656, 4}, {672, 4}, {676, 4}, {800, 4}, {848, 4}, {880, 4}, {884, 4}, {888, 4}, {892, 4}, {896, 4}, {900, 4}, {904, 4}, {908, 4}, {912, 4}, {916, 4}, {920, 4}, {924, 4}, {928, 4}, {932, 4}, {936, 4}, {940, 4}, {944, 4}, {976, 4}, {992, 4}, {996, 4}, {1120, 4}, {1168, 4}, {1200, 4}, {1204, 4}, {1208, 4}, {1212, 4}, {1216, 4}, {1220, 4}, {1224, 4}, {1228, 4}, {1232, 4}, {1236, 4}, {1240, 4}, {1244, 4}, {1248, 4}, {1252, 4}, {1256, 4}, {1260, 4}, {1264, 4}, {1296, 4}, {1312, 4}, {1316, 4}, {1440, 4}, {1488, 4}, {1520, 4}, {1524, 4}, {1528, 4}, {1532, 4}, {1536, 4}, {1540, 4}, {1544, 4}, {1548, 4}, {1552, 4}, {1556, 4}, {1560, 4}, {1564, 4}, {1568, 4}, {1572, 4}, {1576, 4}, {1580, 4}, {1584, 4}, {1616, 4}, {1632, 4}, {1636, 4}, {1760, 4}, {1808, 4}, {1840, 4}, {1844, 4}, {1848, 4}, {1852, 4}, {1856, 4}, {1860, 4}, {1864, 4}, {1868, 4}, {1872, 4}, {1876, 4}, {1880, 4}, {1884, 4}, {1888, 4}, {1892, 4}, {1896, 4}, {1900, 4}, {1904, 4}, {1936, 4}, {1952, 4}, {1956, 4}, {2080, 4}, {2128, 4}, {2160, 4}, {2164, 4}, {2168, 4}, {2172, 4}, {2176, 4}, {2180, 4}, {2184, 4}, {2188, 4}, {2192, 4}, {2196, 4}, {2200, 4}, {2204, 4}, {2208, 4}, {2212, 4}, {2216, 4}, {2220, 4}, {2224, 4}, {2256, 4}, {2272, 4}, {2276, 4}, {2400, 4}, {2448, 4}, {2480, 4}, {2484, 4}, {2488, 4}, {2492, 4}, {2496, 4}, {2500, 4}, {2504, 4}, {2508, 4}, {2512, 4}, {2516, 4}, {2520, 4}, {2524, 4}, {2528, 4}, {2532, 4}, {2536, 4}, {2540, 4}, {2544, 4}, {2576, 4}, {2592, 4}, {2596, 4}, {2720, 4}, {2768, 4}, {2800, 4}, {2804, 4}, {2808, 4}, {2812, 4}, {2816, 4}, {2820, 4}, {2824, 4}, {2828, 4}, {2832, 4}, {2836, 4}, {2840, 4}, {2844, 4}, {2848, 4}, {2852, 4}, {2856, 4}, {2860, 4}, {2864, 4}, {2896, 4}, {2912, 4}, {2916, 4}, {3040, 4}, {3088, 4}, {3120, 4}, {3124, 4}, {3128, 4}, {3132, 4}, {3136, 4}, {3140, 4}, {3144, 4}, {3148, 4}, {3152, 4}, {3156, 4}, {3160, 4}, {3164, 4}, {3168, 4}, {3172, 4}, {3176, 4}, {3180, 4}, {3184, 4}, {3216, 4}, {3232, 4}, {3236, 4}, {3360, 4}, {3408, 4}, {3440, 4}, {3444, 4}, {3448, 4}, {3452, 4}, {3456, 4}, {3460, 4}, {3464, 4}, {3468, 4}, {3472, 4}, {3476, 4}, {3480, 4}, {3484, 4}, {3488, 4}, {3492, 4}, {3496, 4}, {3500, 4}, {3504, 4}, {3536, 4}, {3552, 4}, {3556, 4}, {3680, 4}, {3728, 4}, {3760, 4}, {3764, 4}, {3768, 4}, {3772, 4}, {3776, 4}, {3780, 4}, {3784, 4}, {3788, 4}, {3792, 4}, {3796, 4}, {3800, 4}, {3804, 4}, {3808, 4}, {3812, 4}, {3816, 4}, {3820, 4}, {3824, 4}, {3856, 4}, {3872, 4}, {3876, 4}, {4000, 4}, {4048, 4}, {4080, 4}, {4084, 4}, {4088, 4}, {4092, 4}, {4096, 4}, {4100, 4}, {4104, 4}, {4108, 4}, {4112, 4}, {4116, 4}, {4120, 4}, {4124, 4}, {4128, 4}, {4132, 4}, {4136, 4}, {4140, 4}, {4144, 4}, {4176, 4}, {4192, 4}, {4196, 4}, {4320, 4}, {4368, 4}, {4400, 4}, {4404, 4}, {4408, 4}, {4412, 4}, {4416, 4}, {4420, 4}, {4424, 4}, {4428, 4}, {4432, 4}, {4436, 4}, {4440, 4}, {4444, 4}, {4448, 4}, {4452, 4}, {4456, 4}, {4460, 4}, {4464, 4}, {4496, 4}, {4512, 4}, {4516, 4}, {4640, 4}, {4688, 4}, {4720, 4}, {4724, 4}, {4728, 4}, {4732, 4}, {4736, 4}, {4740, 4}, {4744, 4}, {4748, 4}, {4752, 4}, {4756, 4}, {4760, 4}, {4764, 4}, {4768, 4}, {4772, 4}, {4776, 4}, {4780, 4}, {4784, 4}, {4816, 4}, {4832, 4}, {4836, 4}, {4960, 4}, {5008, 4}, {5040, 4}, {5044, 4}, {5048, 4}, {5052, 4}, {5056, 4}, {5060, 4}, {5064, 4}, {5068, 4}, {5072, 4}, {5076, 4}, {5080, 4}, {5084, 4}, {5088, 4}, {5092, 4}, {5096, 4}, {5100, 4}, {5104, 4}, {5136, 4}, {5152, 4}, {5156, 4}, {5280, 4}, {5328, 4}, {5360, 4}, {5364, 4}, {5368, 4}, {5372, 4}, {5376, 4}, {5380, 4}, {5384, 4}, {5388, 4}, {5392, 4}, {5396, 4}, {5400, 4}, {5404, 4}, {5408, 4}, {5412, 4}, {5416, 4}, {5420, 4}, {5424, 4}, {5456, 4}, {5472, 4}, {5476, 4}, {5600, 4}, {5648, 4}, {5680, 4}, {5684, 4}, {5688, 4}, {5692, 4}, {5696, 4}, {5700, 4}, {5704, 4}, {5708, 4}, {5712, 4}, {5716, 4}, {5720, 4}, {5724, 4}, {5728, 4}, {5732, 4}, {5736, 4}, {5740, 4}, {5744, 4}, {5776, 4}, {5792, 4}, {5796, 4}, {5920, 4}, {5968, 4}, {6000, 4}, {6004, 4}, {6008, 4}, {6012, 4}, {6016, 4}, {6020, 4}, {6024, 4}, {6028, 4}, {6032, 4}, {6036, 4}, {6040, 4}, {6044, 4}, {6048, 4}, {6052, 4}, {6056, 4}, {6060, 4}, {6064, 4}, {6096, 4}, {6112, 4}, {6116, 4}, {6240, 4}, {6288, 4}, {6320, 4}, {6324, 4}, {6328, 4}, {6332, 4}, {6336, 4}, {6340, 4}, {6344, 4}, {6348, 4}, {6352, 4}, {6356, 4}, {6360, 4}, {6364, 4}, {6368, 4}, {6372, 4}, {6376, 4}, {6380, 4}, {6384, 4}, {6416, 4}, {6432, 4}, {6436, 4}, {6560, 4}, {6608, 4}, {6640, 4}, {6644, 4}, {6648, 4}, {6652, 4}, {6656, 4}, {6660, 4}, {6664, 4}, {6668, 4}, {6672, 4}, {6676, 4}, {6680, 4}, {6684, 4}, {6688, 4}, {6692, 4}, {6696, 4}, {6700, 4}, {6704, 4}, {6736, 4}, {6752, 4}, {6756, 4}, {6880, 4}, {6928, 4}, {6960, 4}, {6964, 4}, {6968, 4}, {6972, 4}, {6976, 4}, {6980, 4}, {6984, 4}, {6988, 4}, {6992, 4}, {6996, 4}, {7000, 4}, {7004, 4}, {7008, 4}, {7012, 4}, {7016, 4}, {7020, 4}, {7024, 4}, {7056, 4}, {7072, 4}, {7076, 4}, {7200, 4}, {7248, 4}, {7280, 4}, {7284, 4}, {7288, 4}, {7292, 4}, {7296, 4}, {7300, 4}, {7304, 4}, {7308, 4}, {7312, 4}, {7316, 4}, {7320, 4}, {7324, 4}, {7328, 4}, {7332, 4}, {7336, 4}, {7340, 4}, {7344, 4}, {7376, 4}, {7392, 4}, {7396, 4}, {7520, 4}, {7568, 4}, {7600, 4}, {7604, 4}, {7608, 4}, {7612, 4}, {7616, 4}, {7620, 4}, {7624, 4}, {7628, 4}, {7632, 4}, {7636, 4}, {7640, 4}, {7644, 4}, {7648, 4}, {7652, 4}, {7656, 4}, {7660, 4}, {7664, 4}, {7696, 4}, {7712, 4}, {7716, 4}, {7840, 4}, {7888, 4}, {7920, 4}, {7924, 4}, {7928, 4}, {7932, 4}, {7936, 4}, {7940, 4}, {7944, 4}, {7948, 4}, {7952, 4}, {7956, 4}, {7960, 4}, {7964, 4}, {7968, 4}, {7972, 4}, {7976, 4}, {7980, 4}, {7984, 4}, {8016, 4}, {8032, 4}, {8036, 4}, {8160, 4}, {8208, 4}, {8240, 4}, {8244, 4}, {8248, 4}, {8252, 4}, {8256, 4}, {8260, 4}, {8264, 4}, {8268, 4}, {8272, 4}, {8276, 4}, {8280, 4}, {8284, 4}, {8288, 4}, {8292, 4}, {8296, 4}, {8300, 4}, {8304, 4}, {8336, 4}, {8352, 4}, {8356, 4}, {8480, 4}, {8528, 4}, {8560, 4}, {8564, 4}, {8568, 4}, {8572, 4}, {8576, 4}, {8580, 4}, {8584, 4}, {8588, 4}, {8592, 4}, {8596, 4}, {8600, 4}, {8604, 4}, {8608, 4}, {8612, 4}, {8616, 4}, {8620, 4}, {8624, 4}, {8656, 4}, {8672, 4}, {8676, 4}, {8800, 4}, {8848, 4}, {8880, 4}, {8884, 4}, {8888, 4}, {8892, 4}, {8896, 4}, {8900, 4}, {8904, 4}, {8908, 4}, {8912, 4}, {8916, 4}, {8920, 4}, {8924, 4}, {8928, 4}, {8932, 4}, {8936, 4}, {8940, 4}, {8944, 4}, {8976, 4}, {8992, 4}, {8996, 4}, {9120, 4}, {9168, 4}, {9200, 4}, {9204, 4}, {9208, 4}, {9212, 4}, {9216, 4}, {9220, 4}, {9224, 4}, {9228, 4}, {9232, 4}, {9236, 4}, {9240, 4}, {9244, 4}, {9248, 4}, {9252, 4}, {9256, 4}, {9260, 4}, {9264, 4}, {9296, 4}, {9312, 4}, {9316, 4}, {9440, 4}, {9488, 4}, {9520, 4}, {9524, 4}, {9528, 4}, {9532, 4}, {9536, 4}, {9540, 4}, {9544, 4}, {9548, 4}, {9552, 4}, {9556, 4}, {9560, 4}, {9564, 4}, {9568, 4}, {9572, 4}, {9576, 4}, {9580, 4}, {9584, 4}, {9616, 4}, {9632, 4}, {9636, 4}, {9760, 4}, {9808, 4}, {9840, 4}, {9844, 4}, {9848, 4}, {9852, 4}, {9856, 4}, {9860, 4}, {9864, 4}, {9868, 4}, {9872, 4}, {9876, 4}, {9880, 4}, {9884, 4}, {9888, 4}, {9892, 4}, {9896, 4}, {9900, 4}, {9904, 4}, {9936, 4}, {9952, 4}, {9956, 4}, {10080, 4}, {10128, 4}, {10160, 4}, {10164, 4}, {10168, 4}, {10172, 4}, {10176, 4}, {10180, 4}, {10184, 4}, {10188, 4}, {10192, 4}, {10196, 4}, {10200, 4}, {10204, 4}, {10208, 4}, {10212, 4}, {10216, 4}, {10220, 4}, {10224, 4}, {10256, 4}, {10272, 4}, {10276, 4}, {10400, 4}, {10448, 4}, {10480, 4}, {10484, 4}, {10488, 4}, {10492, 4}, {10496, 4}, {10500, 4}, {10504, 4}, {10508, 4}, {10512, 4}, {10516, 4}, {10520, 4}, {10524, 4}, {10528, 4}, {10532, 4}, {10536, 4}, {10540, 4}, {10544, 4}, {10576, 4}, {10592, 4}, {10596, 4}, {10720, 4}, {10768, 4}, {10800, 4}, {10804, 4}, {10808, 4}, {10812, 4}, {10816, 4}, {10820, 4}, {10824, 4}, {10828, 4}, {10832, 4}, {10836, 4}, {10840, 4}, {10844, 4}, {10848, 4}, {10852, 4}, {10856, 4}, {10860, 4}, {10864, 4}, {10896, 4}, {10912, 4}, {10916, 4}, {11040, 4}, {11088, 4}, {11120, 4}, {11124, 4}, {11128, 4}, {11132, 4}, {11136, 4}, {11140, 4}, {11144, 4}, {11148, 4}, {11152, 4}, {11156, 4}, {11160, 4}, {11164, 4}, {11168, 4}, {11172, 4}, {11176, 4}, {11180, 4}, {11184, 4}, {11216, 4}, {11232, 4}, {11236, 4}, {11360, 4}, {11408, 4}, {11440, 4}, {11444, 4}, {11448, 4}, {11452, 4}, {11456, 4}, {11460, 4}, {11464, 4}, {11468, 4}, {11472, 4}, {11476, 4}, {11480, 4}, {11484, 4}, {11488, 4}, {11492, 4}, {11496, 4}, {11500, 4}, {11504, 4}, {11536, 4}, {11552, 4}, {11556, 4}, {11680, 4}, {11728, 4}, {11760, 4}, {11764, 4}, {11768, 4}, {11772, 4}, {11776, 4}, {11780, 4}, {11784, 4}, {11788, 4}, {11792, 4}, {11796, 4}, {11800, 4}, {11804, 4}, {11808, 4}, {11812, 4}, {11816, 4}, {11820, 4}, {11824, 4}, {11856, 4}, {11872, 4}, {11876, 4}, {12000, 4}, {12048, 4}, {12080, 4}, {12084, 4}, {12088, 4}, {12092, 4}, {12096, 4}, {12100, 4}, {12104, 4}, {12108, 4}, {12112, 4}, {12116, 4}, {12120, 4}, {12124, 4}, {12128, 4}, {12132, 4}, {12136, 4}, {12140, 4}, {12144, 4}, {12176, 4}, {12192, 4}, {12196, 4}, {12320, 4}, {12368, 4}, {12400, 4}, {12404, 4}, {12408, 4}, {12412, 4}, {12416, 4}, {12420, 4}, {12424, 4}, {12428, 4}, {12432, 4}, {12436, 4}, {12440, 4}, {12444, 4}, {12448, 4}, {12452, 4}, {12456, 4}, {12460, 4}, {12464, 4}, {12496, 4}, {12512, 4}, {12516, 4}, {12640, 4}, {12688, 4}, {12720, 4}, {12724, 4}, {12728, 4}, {12732, 4}, {12736, 4}, {12740, 4}, {12744, 4}, {12748, 4}, {12752, 4}, {12756, 4}, {12760, 4}, {12764, 4}, {12768, 4}, {12772, 4}, {12776, 4}, {12780, 4}, {12784, 4}, {12816, 4}, {12832, 4}, {12836, 4}, {12960, 4}, {13008, 4}, {13040, 4}, {13044, 4}, {13048, 4}, {13052, 4}, {13056, 4}, {13060, 4}, {13064, 4}, {13068, 4}, {13072, 4}, {13076, 4}, {13080, 4}, {13084, 4}, {13088, 4}, {13092, 4}, {13096, 4}, {13100, 4}, {13104, 4}, {13136, 4}, {13152, 4}, {13156, 4}, {13280, 4}, {13328, 4}, {13360, 4}, {13364, 4}, {13368, 4}, {13372, 4}, {13376, 4}, {13380, 4}, {13384, 4}, {13388, 4}, {13392, 4}, {13396, 4}, {13400, 4}, {13404, 4}, {13408, 4}, {13412, 4}, {13416, 4}, {13420, 4}, {13424, 4}, {13456, 4}, {13472, 4}, {13476, 4}, {13600, 4}, {13648, 4}, {13680, 4}, {13684, 4}, {13688, 4}, {13692, 4}, {13696, 4}, {13700, 4}, {13704, 4}, {13708, 4}, {13712, 4}, {13716, 4}, {13720, 4}, {13724, 4}, {13728, 4}, {13732, 4}, {13736, 4}, {13740, 4}, {13744, 4}, {13776, 4}, {13792, 4}, {13796, 4}, {13920, 4}, {13968, 4}, {14000, 4}, {14004, 4}, {14008, 4}, {14012, 4}, {14016, 4}, {14020, 4}, {14024, 4}, {14028, 4}, {14032, 4}, {14036, 4}, {14040, 4}, {14044, 4}, {14048, 4}, {14052, 4}, {14056, 4}, {14060, 4}, {14064, 4}, {14096, 4}, {14112, 4}, {14116, 4}, {14240, 4}, {14288, 4}, {14320, 4}, {14324, 4}, {14328, 4}, {14332, 4}, {14336, 4}, {14340, 4}, {14344, 4}, {14348, 4}, {14352, 4}, {14356, 4}, {14360, 4}, {14364, 4}, {14368, 4}, {14372, 4}, {14376, 4}, {14380, 4}, {14384, 4}, {14416, 4}, {14432, 4}, {14436, 4}, {14560, 4}, {14608, 4}, {14640, 4}, {14644, 4}, {14648, 4}, {14652, 4}, {14656, 4}, {14660, 4}, {14664, 4}, {14668, 4}, {14672, 4}, {14676, 4}, {14680, 4}, {14684, 4}, {14688, 4}, {14692, 4}, {14696, 4}, {14700, 4}, {14704, 4}, {14736, 4}, {14752, 4}, {14756, 4}, {14880, 4}, {14928, 4}, {14960, 4}, {14964, 4}, {14968, 4}, {14972, 4}, {14976, 4}, {14980, 4}, {14984, 4}, {14988, 4}, {14992, 4}, {14996, 4}, {15000, 4}, {15004, 4}, {15008, 4}, {15012, 4}, {15016, 4}, {15020, 4}, {15024, 4}, {15056, 4}, {15072, 4}, {15076, 4}, {15200, 4}, {15248, 4}, {15280, 4}, {15284, 4}, {15288, 4}, {15292, 4}, {15296, 4}, {15300, 4}, {15304, 4}, {15308, 4}, {15312, 4}, {15316, 4}, {15320, 4}, {15324, 4}, {15328, 4}, {15332, 4}, {15336, 4}, {15340, 4}, {15344, 4}, {15376, 4}, {15392, 4}, {15396, 4}, {15520, 4}, {15568, 4}, {15600, 4}, {15604, 4}, {15608, 4}, {15612, 4}, {15616, 4}, {15620, 4}, {15624, 4}, {15628, 4}, {15632, 4}, {15636, 4}, {15640, 4}, {15644, 4}, {15648, 4}, {15652, 4}, {15656, 4}, {15660, 4}, {15664, 4}, {15696, 4}, {15712, 4}, {15716, 4}, {15840, 4}, {15888, 4}, {15920, 4}, {15924, 4}, {15928, 4}, {15932, 4}, {15936, 4}, {15940, 4}, {15944, 4}, {15948, 4}, {15952, 4}, {15956, 4}, {15960, 4}, {15964, 4}, {15968, 4}, {15972, 4}, {15976, 4}, {15980, 4}, {15984, 4}};
inline constexpr memory::Layout kCollisionObjectBlockLayout = {kCollisionObjectBlockFields, 1101};

struct CollisionObjectBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 16016;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CollisionObjectBlockData* data() const { return static_cast<CollisionObjectBlockData*>(memory::HandleObject(address)); }
  static CollisionObjectBlock New() {
    return CollisionObjectBlock(memory::MakeHandle(new CollisionObjectBlockData(), sizeof(CollisionObjectBlockData), "CollisionObjectBlock", &kCollisionObjectBlockLayout));
  }
  template <uint32_t Stride = kSize>
  static CollisionObjectBlock NewArray(uint32_t count) {
    static_assert(sizeof(CollisionObjectBlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CollisionObjectBlockData*>(::operator new(sizeof(CollisionObjectBlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CollisionObjectBlockData();
    return CollisionObjectBlock(memory::MakeHandle(p, sizeof(CollisionObjectBlockData) * count, "CollisionObjectBlock", &kCollisionObjectBlockLayout, sizeof(CollisionObjectBlockData)));
  }
  CollisionObjectBlock at(uint32_t i) const { return CollisionObjectBlock(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CollisionObjectBlockData* direct() const { return static_cast<CollisionObjectBlockData*>(memory::HandleOwn(address, &kCollisionObjectBlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCollisionObjectBlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCollisionObjectBlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t count_address() const { return address + 0; }
  uint32_t count_bits() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_count_bits(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t count() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_count(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the collision objects (CollisionWorld's +16 points here)
  template <typename T = CollisionWorldObject>
  T objects(uint32_t i) const { return T(address + 16 + i * T::kSize); }
};

// embedded; measure: new<72>@CollisionWorld::Create.+20, new<72>@CollisionWorld::Create.+24
#pragma pack(push, 1)
struct CollisionWorldObjectData {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[12] = {};
  uint32_t body = 0;  // +16
  uint32_t shape = 0;  // +20
  uint8_t pad_24[8] = {};
  uint8_t shape_record[80] = {};  // +32, guest order
  uint8_t pad_112[32] = {};
  float lowest_travel = 0;  // +144
  uint8_t pad_148[12] = {};
  uint8_t push[16] = {};  // +160, guest order
  uint8_t pair_torque[16] = {};  // +176, guest order
  uint32_t push_count = 0;  // +192
  uint8_t pad_196[12] = {};
  uint8_t push_scale[16] = {};  // +208, guest order
  float torque_scale = 0;  // +224
  uint32_t next = 0;  // +228
  uint32_t previous = 0;  // +232
  uint32_t unknown_236 = 0;  // +236
  uint32_t first_contact = 0;  // +240
  uint32_t groups = 0;  // +244
  uint32_t collides_with = 0;  // +248
  uint32_t mask = 0;  // +252
  uint32_t response_bits = 0;  // +256
  uint32_t push_out_bits = 0;  // +260
  uint32_t first_pair = 0;  // +264
  uint32_t owner_id = 0;  // +268
  uint32_t no_queries = 0;  // +272
  uint32_t unknown_276 = 0;  // +276
  uint32_t box_friction = 0;  // +280
  uint32_t unmoved = 0;  // +284
  uint32_t moved = 0;  // +288
  uint8_t pad_292[12] = {};
  uint8_t push_out[16] = {};  // +304, guest order
};
#pragma pack(pop)
static_assert(sizeof(CollisionWorldObjectData) == 320);
inline constexpr memory::LayoutField kCollisionWorldObjectFields[] = {{0, 4}, {16, 4}, {20, 4}, {144, 4}, {192, 4}, {224, 4}, {228, 4}, {232, 4}, {236, 4}, {240, 4}, {244, 4}, {248, 4}, {252, 4}, {256, 4}, {260, 4}, {264, 4}, {268, 4}, {272, 4}, {276, 4}, {280, 4}, {284, 4}, {288, 4}};
inline constexpr memory::Layout kCollisionWorldObjectLayout = {kCollisionWorldObjectFields, 22};

struct CollisionWorldObject : View {
  using View::View;
  static constexpr uint32_t kSize = 320;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CollisionWorldObjectData* data() const { return static_cast<CollisionWorldObjectData*>(memory::HandleObject(address)); }
  static CollisionWorldObject New() {
    return CollisionWorldObject(memory::MakeHandle(new CollisionWorldObjectData(), sizeof(CollisionWorldObjectData), "CollisionWorldObject", &kCollisionWorldObjectLayout));
  }
  template <uint32_t Stride = kSize>
  static CollisionWorldObject NewArray(uint32_t count) {
    static_assert(sizeof(CollisionWorldObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CollisionWorldObjectData*>(::operator new(sizeof(CollisionWorldObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CollisionWorldObjectData();
    return CollisionWorldObject(memory::MakeHandle(p, sizeof(CollisionWorldObjectData) * count, "CollisionWorldObject", &kCollisionWorldObjectLayout, sizeof(CollisionWorldObjectData)));
  }
  CollisionWorldObject at(uint32_t i) const { return CollisionWorldObject(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CollisionWorldObjectData* direct() const { return static_cast<CollisionWorldObjectData*>(memory::HandleOwn(address, &kCollisionWorldObjectLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCollisionWorldObjectLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCollisionWorldObjectLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = CollisionWorldObjectVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  uint32_t body_address() const { return address + 16; }
  uint32_t body_bits() const { if (auto* d = direct()) return d->body; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_body_bits(uint32_t v) const { if (auto* d = direct()) { d->body = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  template <typename T = RigidBody>
  T body() const { return T(body_bits()); }
  void set_body(const View& w) const { set_body_bits(w.address); }
  void set_body(uint32_t v) const { set_body_bits(v); }
  // the vehicle's +344 box
  uint32_t shape_address() const { return address + 20; }
  uint32_t shape_bits() const { if (auto* d = direct()) return d->shape; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_shape_bits(uint32_t v) const { if (auto* d = direct()) { d->shape = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t shape() const { if (auto* d = direct()) return d->shape; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_shape(uint32_t v) const { if (auto* d = direct()) { d->shape = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // an embedded shape record
  uint32_t shape_record_address() const { return address + 32; }
  uint8_t* shape_record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  // minus the lowest wheel travel
  uint32_t lowest_travel_address() const { return address + 144; }
  uint32_t lowest_travel_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->lowest_travel); return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_lowest_travel_bits(uint32_t v) const { if (auto* d = direct()) { d->lowest_travel = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  float lowest_travel() const { return std::bit_cast<float>(lowest_travel_bits()); }
  void set_lowest_travel(float v) const { set_lowest_travel_bits(std::bit_cast<uint32_t>(v)); }
  // the largest push per axis
  uint32_t push_address() const { return address + 160; }
  uint8_t* push() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 160 : guest::Pointer(address + 160); }
  // the summed torque from pairs this tick
  uint32_t pair_torque_address() const { return address + 176; }
  uint8_t* pair_torque() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 176 : guest::Pointer(address + 176); }
  uint32_t push_count_address() const { return address + 192; }
  uint32_t push_count_bits() const { if (auto* d = direct()) return d->push_count; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_push_count_bits(uint32_t v) const { if (auto* d = direct()) { d->push_count = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  uint32_t push_count() const { if (auto* d = direct()) return d->push_count; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_push_count(uint32_t v) const { if (auto* d = direct()) { d->push_count = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  // a per-axis scale of the push-out
  uint32_t push_scale_address() const { return address + 208; }
  uint8_t* push_scale() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 208 : guest::Pointer(address + 208); }
  uint32_t torque_scale_address() const { return address + 224; }
  uint32_t torque_scale_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->torque_scale); return uint32_t((memory::IsHandle(address + 224) ? uint32_t(memory::OwnerLoad(address + 224, 4)) : guest::LoadU32(address + 224))); }
  void set_torque_scale_bits(uint32_t v) const { if (auto* d = direct()) { d->torque_scale = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 224)) memory::OwnerStore(address + 224, 4, v); else guest::StoreU32(address + 224, v); }
  float torque_scale() const { return std::bit_cast<float>(torque_scale_bits()); }
  void set_torque_scale(float v) const { set_torque_scale_bits(std::bit_cast<uint32_t>(v)); }
  // in the world's active or free list
  uint32_t next_address() const { return address + 228; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 228) ? uint32_t(memory::OwnerLoad(address + 228, 4)) : guest::LoadU32(address + 228))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 228)) memory::OwnerStore(address + 228, 4, v); else guest::StoreU32(address + 228, v); }
  template <typename T = CollisionWorldObject>
  T next() const { return T(next_bits()); }
  void set_next(const View& w) const { set_next_bits(w.address); }
  void set_next(uint32_t v) const { set_next_bits(v); }
  uint32_t previous_address() const { return address + 232; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 232) ? uint32_t(memory::OwnerLoad(address + 232, 4)) : guest::LoadU32(address + 232))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 232)) memory::OwnerStore(address + 232, 4, v); else guest::StoreU32(address + 232, v); }
  template <typename T = CollisionWorldObject>
  T previous() const { return T(previous_bits()); }
  void set_previous(const View& w) const { set_previous_bits(w.address); }
  void set_previous(uint32_t v) const { set_previous_bits(v); }
  uint32_t unknown_236_address() const { return address + 236; }
  uint32_t unknown_236_bits() const { if (auto* d = direct()) return d->unknown_236; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(address + 236))); }
  void set_unknown_236_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_236 = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(address + 236, v); }
  uint32_t unknown_236() const { if (auto* d = direct()) return d->unknown_236; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(address + 236))); }
  void set_unknown_236(uint32_t v) const { if (auto* d = direct()) { d->unknown_236 = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(address + 236, v); }
  uint32_t first_contact_address() const { return address + 240; }
  uint32_t first_contact_bits() const { if (auto* d = direct()) return d->first_contact; return uint32_t((memory::IsHandle(address + 240) ? uint32_t(memory::OwnerLoad(address + 240, 4)) : guest::LoadU32(address + 240))); }
  void set_first_contact_bits(uint32_t v) const { if (auto* d = direct()) { d->first_contact = v; return; } if (memory::IsHandle(address + 240)) memory::OwnerStore(address + 240, 4, v); else guest::StoreU32(address + 240, v); }
  template <typename T = CollisionContact>
  T first_contact() const { return T(first_contact_bits()); }
  void set_first_contact(const View& w) const { set_first_contact_bits(w.address); }
  void set_first_contact(uint32_t v) const { set_first_contact_bits(v); }
  // its own groups
  uint32_t groups_address() const { return address + 244; }
  uint32_t groups_bits() const { if (auto* d = direct()) return d->groups; return uint32_t((memory::IsHandle(address + 244) ? uint32_t(memory::OwnerLoad(address + 244, 4)) : guest::LoadU32(address + 244))); }
  void set_groups_bits(uint32_t v) const { if (auto* d = direct()) { d->groups = v; return; } if (memory::IsHandle(address + 244)) memory::OwnerStore(address + 244, 4, v); else guest::StoreU32(address + 244, v); }
  uint32_t groups() const { if (auto* d = direct()) return d->groups; return uint32_t((memory::IsHandle(address + 244) ? uint32_t(memory::OwnerLoad(address + 244, 4)) : guest::LoadU32(address + 244))); }
  void set_groups(uint32_t v) const { if (auto* d = direct()) { d->groups = v; return; } if (memory::IsHandle(address + 244)) memory::OwnerStore(address + 244, 4, v); else guest::StoreU32(address + 244, v); }
  uint32_t collides_with_address() const { return address + 248; }
  uint32_t collides_with_bits() const { if (auto* d = direct()) return d->collides_with; return uint32_t((memory::IsHandle(address + 248) ? uint32_t(memory::OwnerLoad(address + 248, 4)) : guest::LoadU32(address + 248))); }
  void set_collides_with_bits(uint32_t v) const { if (auto* d = direct()) { d->collides_with = v; return; } if (memory::IsHandle(address + 248)) memory::OwnerStore(address + 248, 4, v); else guest::StoreU32(address + 248, v); }
  uint32_t collides_with() const { if (auto* d = direct()) return d->collides_with; return uint32_t((memory::IsHandle(address + 248) ? uint32_t(memory::OwnerLoad(address + 248, 4)) : guest::LoadU32(address + 248))); }
  void set_collides_with(uint32_t v) const { if (auto* d = direct()) { d->collides_with = v; return; } if (memory::IsHandle(address + 248)) memory::OwnerStore(address + 248, 4, v); else guest::StoreU32(address + 248, v); }
  uint32_t mask_address() const { return address + 252; }
  uint32_t mask_bits() const { if (auto* d = direct()) return d->mask; return uint32_t((memory::IsHandle(address + 252) ? uint32_t(memory::OwnerLoad(address + 252, 4)) : guest::LoadU32(address + 252))); }
  void set_mask_bits(uint32_t v) const { if (auto* d = direct()) { d->mask = v; return; } if (memory::IsHandle(address + 252)) memory::OwnerStore(address + 252, 4, v); else guest::StoreU32(address + 252, v); }
  uint32_t mask() const { if (auto* d = direct()) return d->mask; return uint32_t((memory::IsHandle(address + 252) ? uint32_t(memory::OwnerLoad(address + 252, 4)) : guest::LoadU32(address + 252))); }
  void set_mask(uint32_t v) const { if (auto* d = direct()) { d->mask = v; return; } if (memory::IsHandle(address + 252)) memory::OwnerStore(address + 252, 4, v); else guest::StoreU32(address + 252, v); }
  uint32_t response_bits_address() const { return address + 256; }
  uint32_t response_bits_bits() const { if (auto* d = direct()) return d->response_bits; return uint32_t((memory::IsHandle(address + 256) ? uint32_t(memory::OwnerLoad(address + 256, 4)) : guest::LoadU32(address + 256))); }
  void set_response_bits_bits(uint32_t v) const { if (auto* d = direct()) { d->response_bits = v; return; } if (memory::IsHandle(address + 256)) memory::OwnerStore(address + 256, 4, v); else guest::StoreU32(address + 256, v); }
  uint32_t response_bits() const { if (auto* d = direct()) return d->response_bits; return uint32_t((memory::IsHandle(address + 256) ? uint32_t(memory::OwnerLoad(address + 256, 4)) : guest::LoadU32(address + 256))); }
  void set_response_bits(uint32_t v) const { if (auto* d = direct()) { d->response_bits = v; return; } if (memory::IsHandle(address + 256)) memory::OwnerStore(address + 256, 4, v); else guest::StoreU32(address + 256, v); }
  uint32_t push_out_bits_address() const { return address + 260; }
  uint32_t push_out_bits_bits() const { if (auto* d = direct()) return d->push_out_bits; return uint32_t((memory::IsHandle(address + 260) ? uint32_t(memory::OwnerLoad(address + 260, 4)) : guest::LoadU32(address + 260))); }
  void set_push_out_bits_bits(uint32_t v) const { if (auto* d = direct()) { d->push_out_bits = v; return; } if (memory::IsHandle(address + 260)) memory::OwnerStore(address + 260, 4, v); else guest::StoreU32(address + 260, v); }
  uint32_t push_out_bits() const { if (auto* d = direct()) return d->push_out_bits; return uint32_t((memory::IsHandle(address + 260) ? uint32_t(memory::OwnerLoad(address + 260, 4)) : guest::LoadU32(address + 260))); }
  void set_push_out_bits(uint32_t v) const { if (auto* d = direct()) { d->push_out_bits = v; return; } if (memory::IsHandle(address + 260)) memory::OwnerStore(address + 260, 4, v); else guest::StoreU32(address + 260, v); }
  uint32_t first_pair_address() const { return address + 264; }
  uint32_t first_pair_bits() const { if (auto* d = direct()) return d->first_pair; return uint32_t((memory::IsHandle(address + 264) ? uint32_t(memory::OwnerLoad(address + 264, 4)) : guest::LoadU32(address + 264))); }
  void set_first_pair_bits(uint32_t v) const { if (auto* d = direct()) { d->first_pair = v; return; } if (memory::IsHandle(address + 264)) memory::OwnerStore(address + 264, 4, v); else guest::StoreU32(address + 264, v); }
  template <typename T = CollisionPair>
  T first_pair() const { return T(first_pair_bits()); }
  void set_first_pair(const View& w) const { set_first_pair_bits(w.address); }
  void set_first_pair(uint32_t v) const { set_first_pair_bits(v); }
  // objects with the same nonzero id don't pair
  uint32_t owner_id_address() const { return address + 268; }
  uint32_t owner_id_bits() const { if (auto* d = direct()) return d->owner_id; return uint32_t((memory::IsHandle(address + 268) ? uint32_t(memory::OwnerLoad(address + 268, 4)) : guest::LoadU32(address + 268))); }
  void set_owner_id_bits(uint32_t v) const { if (auto* d = direct()) { d->owner_id = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 4, v); else guest::StoreU32(address + 268, v); }
  uint32_t owner_id() const { if (auto* d = direct()) return d->owner_id; return uint32_t((memory::IsHandle(address + 268) ? uint32_t(memory::OwnerLoad(address + 268, 4)) : guest::LoadU32(address + 268))); }
  void set_owner_id(uint32_t v) const { if (auto* d = direct()) { d->owner_id = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 4, v); else guest::StoreU32(address + 268, v); }
  uint32_t no_queries_address() const { return address + 272; }
  uint32_t no_queries_bits() const { if (auto* d = direct()) return d->no_queries; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(address + 272))); }
  void set_no_queries_bits(uint32_t v) const { if (auto* d = direct()) { d->no_queries = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(address + 272, v); }
  uint32_t no_queries() const { if (auto* d = direct()) return d->no_queries; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(address + 272))); }
  void set_no_queries(uint32_t v) const { if (auto* d = direct()) { d->no_queries = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(address + 272, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  uint32_t unknown_276_bits() const { if (auto* d = direct()) return d->unknown_276; return uint32_t((memory::IsHandle(address + 276) ? uint32_t(memory::OwnerLoad(address + 276, 4)) : guest::LoadU32(address + 276))); }
  void set_unknown_276_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_276 = v; return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 4, v); else guest::StoreU32(address + 276, v); }
  uint32_t unknown_276() const { if (auto* d = direct()) return d->unknown_276; return uint32_t((memory::IsHandle(address + 276) ? uint32_t(memory::OwnerLoad(address + 276, 4)) : guest::LoadU32(address + 276))); }
  void set_unknown_276(uint32_t v) const { if (auto* d = direct()) { d->unknown_276 = v; return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 4, v); else guest::StoreU32(address + 276, v); }
  // set: friction from the +344 object alone
  uint32_t box_friction_address() const { return address + 280; }
  uint32_t box_friction_bits() const { if (auto* d = direct()) return d->box_friction; return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(address + 280))); }
  void set_box_friction_bits(uint32_t v) const { if (auto* d = direct()) { d->box_friction = v; return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(address + 280, v); }
  uint32_t box_friction() const { if (auto* d = direct()) return d->box_friction; return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(address + 280))); }
  void set_box_friction(uint32_t v) const { if (auto* d = direct()) { d->box_friction = v; return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(address + 280, v); }
  // 0 after a move
  uint32_t unmoved_address() const { return address + 284; }
  uint32_t unmoved_bits() const { if (auto* d = direct()) return d->unmoved; return uint32_t((memory::IsHandle(address + 284) ? uint32_t(memory::OwnerLoad(address + 284, 4)) : guest::LoadU32(address + 284))); }
  void set_unmoved_bits(uint32_t v) const { if (auto* d = direct()) { d->unmoved = v; return; } if (memory::IsHandle(address + 284)) memory::OwnerStore(address + 284, 4, v); else guest::StoreU32(address + 284, v); }
  uint32_t unmoved() const { if (auto* d = direct()) return d->unmoved; return uint32_t((memory::IsHandle(address + 284) ? uint32_t(memory::OwnerLoad(address + 284, 4)) : guest::LoadU32(address + 284))); }
  void set_unmoved(uint32_t v) const { if (auto* d = direct()) { d->unmoved = v; return; } if (memory::IsHandle(address + 284)) memory::OwnerStore(address + 284, 4, v); else guest::StoreU32(address + 284, v); }
  uint32_t moved_address() const { return address + 288; }
  uint32_t moved_bits() const { if (auto* d = direct()) return d->moved; return uint32_t((memory::IsHandle(address + 288) ? uint32_t(memory::OwnerLoad(address + 288, 4)) : guest::LoadU32(address + 288))); }
  void set_moved_bits(uint32_t v) const { if (auto* d = direct()) { d->moved = v; return; } if (memory::IsHandle(address + 288)) memory::OwnerStore(address + 288, 4, v); else guest::StoreU32(address + 288, v); }
  uint32_t moved() const { if (auto* d = direct()) return d->moved; return uint32_t((memory::IsHandle(address + 288) ? uint32_t(memory::OwnerLoad(address + 288, 4)) : guest::LoadU32(address + 288))); }
  void set_moved(uint32_t v) const { if (auto* d = direct()) { d->moved = v; return; } if (memory::IsHandle(address + 288)) memory::OwnerStore(address + 288, 4, v); else guest::StoreU32(address + 288, v); }
  // this pass's push-out
  uint32_t push_out_address() const { return address + 304; }
  uint8_t* push_out() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 304 : guest::Pointer(address + 304); }
};

// image; measure: new<72>@CollisionWorld::Create.+24.+0
struct CollisionWorldObjectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // slot 0 (0x820B9110)
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
};

// allocated; measure: CollisionPair, CollisionWorld, CollisionWorld.+4, CollisionWorld.+4.+8, CollisionWorld.+8, CollisionWorld.+12
struct CollisionPair : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // the previous pair on A's list
  template <typename T = CollisionPair>
  T a_previous() const { return T(guest::LoadU32(address + 0)); }
  void set_a_previous(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_a_previous(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t a_previous_address() const { return address + 0; }
  template <typename T = CollisionPair>
  T b_previous() const { return T(guest::LoadU32(address + 4)); }
  void set_b_previous(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_b_previous(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t b_previous_address() const { return address + 4; }
  template <typename T = CollisionPair>
  T a_next() const { return T(guest::LoadU32(address + 8)); }
  void set_a_next(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_a_next(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t a_next_address() const { return address + 8; }
  template <typename T = CollisionPair>
  T b_next() const { return T(guest::LoadU32(address + 12)); }
  void set_b_next(const View& v) const { guest::StoreU32(address + 12, v.address); }
  void set_b_next(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t b_next_address() const { return address + 12; }
  template <typename T = CollisionWorldObject>
  T a() const { return T(guest::LoadU32(address + 16)); }
  void set_a(const View& v) const { guest::StoreU32(address + 16, v.address); }
  void set_a(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t a_address() const { return address + 16; }
  template <typename T = CollisionWorldObject>
  T b() const { return T(guest::LoadU32(address + 20)); }
  void set_b(const View& v) const { guest::StoreU32(address + 20, v.address); }
  void set_b(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t b_address() const { return address + 20; }
  // 0 when made, then the face that last separated the boxes
  uint32_t separated_face() const { return guest::LoadU32(address + 24); }
  void set_separated_face(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t separated_face_address() const { return address + 24; }
  // the next time to check it
  float next_check() const { return guest::LoadFloat(address + 28); }
  void set_next_check(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t next_check_address() const { return address + 28; }
  // CollisionPair::Respond's object (its +244: the measure's)
  uint32_t object_groups() const { return guest::LoadU32(address + 244); }
  void set_object_groups(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t object_groups_address() const { return address + 244; }
  // the measure's, as +244
  uint32_t object_response_bits() const { return guest::LoadU32(address + 256); }
  void set_object_response_bits(uint32_t v) const { guest::StoreU32(address + 256, v); }
  uint32_t object_response_bits_address() const { return address + 256; }
  // the measure's, as +244
  uint32_t object_push_out_bits() const { return guest::LoadU32(address + 260); }
  void set_object_push_out_bits(uint32_t v) const { guest::StoreU32(address + 260, v); }
  uint32_t object_push_out_bits_address() const { return address + 260; }
  // CollisionWorld::AddPairs' object (the measure's)
  uint32_t object_owner_id() const { return guest::LoadU32(address + 268); }
  void set_object_owner_id(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t object_owner_id_address() const { return address + 268; }
};

// allocated; measure: new<4294967295>@CollisionWorld::Create#4, CollisionWorld::NewContact.ret, Contact, CollisionObject.+240.+68, Contact::Set.r3
#pragma pack(push, 1)
struct CollisionContactData {
  uint32_t other = 0;  // +0
  uint32_t flags = 0;  // +4
  uint8_t pad_8[8] = {};
  uint8_t position[16] = {};  // +16, guest order
  uint8_t normal[16] = {};  // +32, guest order
  float push_direction_x = 0;  // +48
  float push_direction_y = 0;  // +52
  uint8_t pad_56[8] = {};
  float strength = 0;  // +64
  uint32_t next = 0;  // +68
  uint8_t pad_72[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(CollisionContactData) == 80);
inline constexpr memory::LayoutField kCollisionContactFields[] = {{0, 4}, {4, 4}, {48, 4}, {52, 4}, {64, 4}, {68, 4}};
inline constexpr memory::Layout kCollisionContactLayout = {kCollisionContactFields, 6};

struct CollisionContact : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  CollisionContactData* data() const { return static_cast<CollisionContactData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static CollisionContact New() {
    return CollisionContact(memory::MakeHandle(new CollisionContactData(), sizeof(CollisionContactData), "CollisionContact", &kCollisionContactLayout));
  }
  template <uint32_t Stride = kSize>
  static CollisionContact NewArray(uint32_t count) {
    static_assert(sizeof(CollisionContactData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CollisionContactData*>(::operator new(sizeof(CollisionContactData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CollisionContactData();
    return CollisionContact(memory::MakeHandle(p, sizeof(CollisionContactData) * count, "CollisionContact", &kCollisionContactLayout, sizeof(CollisionContactData)));
  }
  CollisionContact at(uint32_t i) const { return CollisionContact(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CollisionContactData* direct() const { return static_cast<CollisionContactData*>(memory::HandleOwn(address, &kCollisionContactLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCollisionContactLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCollisionContactLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the other object
  uint32_t other_address() const { return address + 0; }
  uint32_t other_bits() const { if (auto* d = direct()) return d->other; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_other_bits(uint32_t v) const { if (auto* d = direct()) { d->other = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t other() const { if (auto* d = direct()) return d->other; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_other(uint32_t v) const { if (auto* d = direct()) { d->other = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t flags_address() const { return address + 4; }
  uint32_t flags_bits() const { if (auto* d = direct()) return d->flags; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_flags_bits(uint32_t v) const { if (auto* d = direct()) { d->flags = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t flags() const { if (auto* d = direct()) return d->flags; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_flags(uint32_t v) const { if (auto* d = direct()) { d->flags = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t position_address() const { return address + 16; }
  uint8_t* position() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 16 : guest::Pointer(address + 16); }
  // the hit's normal
  uint32_t normal_address() const { return address + 32; }
  uint8_t* normal() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  uint32_t push_direction_address() const { return address + 48; }
  uint8_t* push_direction() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 48 : guest::Pointer(address + 48); }
  // the approach speed into the surface
  uint32_t strength_address() const { return address + 64; }
  uint32_t strength_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->strength); return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_strength_bits(uint32_t v) const { if (auto* d = direct()) { d->strength = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  float strength() const { return std::bit_cast<float>(strength_bits()); }
  void set_strength(float v) const { set_strength_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t next_address() const { return address + 68; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  template <typename T = CollisionContact>
  T next() const { return T(next_bits()); }
  void set_next(const View& w) const { set_next_bits(w.address); }
  void set_next(uint32_t v) const { set_next_bits(v); }
  // from CollisionObjectContact (vehicle-accessors)
  uint32_t push_direction_x_address() const { return address + 48; }
  uint32_t push_direction_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->push_direction_x); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_push_direction_x_bits(uint32_t v) const { if (auto* d = direct()) { d->push_direction_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float push_direction_x() const { return std::bit_cast<float>(push_direction_x_bits()); }
  void set_push_direction_x(float v) const { set_push_direction_x_bits(std::bit_cast<uint32_t>(v)); }
  // from CollisionObjectContact (vehicle-accessors)
  uint32_t push_direction_y_address() const { return address + 52; }
  uint32_t push_direction_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->push_direction_y); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_push_direction_y_bits(uint32_t v) const { if (auto* d = direct()) { d->push_direction_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  float push_direction_y() const { return std::bit_cast<float>(push_direction_y_bits()); }
  void set_push_direction_y(float v) const { set_push_direction_y_bits(std::bit_cast<uint32_t>(v)); }
};

// allocated; measure: new<4294967295>@CollisionWorld::Create#3, new<4294967295>@CollisionWorld::SetQueryCapacity, CollisionRecords::Merge.r4, CollisionWorld::Create.r6, WallQuery.+8
#pragma pack(push, 1)
struct CollisionHitData {
  uint8_t point[16] = {};  // +0, guest order
  uint8_t normal[16] = {};  // +16, guest order
  uint8_t push[16] = {};  // +32, guest order
  float depth = 0;  // +48
  float depth_2 = 0;  // +52
  uint32_t surface_bits = 0;  // +56
  uint32_t key = 0;  // +60
  float time = 0;  // +64
  uint8_t pad_68[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(CollisionHitData) == 80);
inline constexpr memory::LayoutField kCollisionHitFields[] = {{48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}};
inline constexpr memory::Layout kCollisionHitLayout = {kCollisionHitFields, 5};

struct CollisionHit : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  CollisionHitData* data() const { return static_cast<CollisionHitData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static CollisionHit New() {
    return CollisionHit(memory::MakeHandle(new CollisionHitData(), sizeof(CollisionHitData), "CollisionHit", &kCollisionHitLayout));
  }
  template <uint32_t Stride = kSize>
  static CollisionHit NewArray(uint32_t count) {
    static_assert(sizeof(CollisionHitData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CollisionHitData*>(::operator new(sizeof(CollisionHitData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CollisionHitData();
    return CollisionHit(memory::MakeHandle(p, sizeof(CollisionHitData) * count, "CollisionHit", &kCollisionHitLayout, sizeof(CollisionHitData)));
  }
  CollisionHit at(uint32_t i) const { return CollisionHit(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CollisionHitData* direct() const { return static_cast<CollisionHitData*>(memory::HandleOwn(address, &kCollisionHitLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCollisionHitLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCollisionHitLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t point_address() const { return address + 0; }
  uint8_t* point() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  // probably the surface normal
  uint32_t normal_address() const { return address + 16; }
  uint8_t* normal() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 16 : guest::Pointer(address + 16); }
  // the push direction
  uint32_t push_address() const { return address + 32; }
  uint8_t* push() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(address + 32); }
  uint32_t depth_address() const { return address + 48; }
  uint32_t depth_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->depth); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_depth_bits(uint32_t v) const { if (auto* d = direct()) { d->depth = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float depth() const { return std::bit_cast<float>(depth_bits()); }
  void set_depth(float v) const { set_depth_bits(std::bit_cast<uint32_t>(v)); }
  // the depth again (walls)
  uint32_t depth_2_address() const { return address + 52; }
  uint32_t depth_2_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->depth_2); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_depth_2_bits(uint32_t v) const { if (auto* d = direct()) { d->depth_2 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  float depth_2() const { return std::bit_cast<float>(depth_2_bits()); }
  void set_depth_2(float v) const { set_depth_2_bits(std::bit_cast<uint32_t>(v)); }
  // 0x07000000
  uint32_t surface_bits_address() const { return address + 56; }
  uint32_t surface_bits_bits() const { if (auto* d = direct()) return d->surface_bits; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_surface_bits_bits(uint32_t v) const { if (auto* d = direct()) { d->surface_bits = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t surface_bits() const { if (auto* d = direct()) return d->surface_bits; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_surface_bits(uint32_t v) const { if (auto* d = direct()) { d->surface_bits = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  // hits with the same key are merged
  uint32_t key_address() const { return address + 60; }
  uint32_t key_bits() const { if (auto* d = direct()) return d->key; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_key_bits(uint32_t v) const { if (auto* d = direct()) { d->key = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t key() const { if (auto* d = direct()) return d->key; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_key(uint32_t v) const { if (auto* d = direct()) { d->key = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // 2.0 from the wall query
  uint32_t time_address() const { return address + 64; }
  uint32_t time_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->time); return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_time_bits(uint32_t v) const { if (auto* d = direct()) { d->time = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  float time() const { return std::bit_cast<float>(time_bits()); }
  void set_time(float v) const { set_time_bits(std::bit_cast<uint32_t>(v)); }
};

// frame; measure: CollisionObject::Respond.r6
struct CollisionSkipFlag : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t set() const { return guest::LoadU32(address + 0); }
  void set_set(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t set_address() const { return address + 0; }
};

// image; measure: Vehicle.+344.+0
struct VehicleBoxVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // slot 0
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
};

// embedded; measure: RaceScreens@+16.+440
struct VehiclePart : View {
  using View::View;
  static constexpr uint32_t kSize = 6264;
  uint32_t unknown_2252() const { return guest::LoadU32(address + 2252); }
  void set_unknown_2252(uint32_t v) const { guest::StoreU32(address + 2252, v); }
  uint32_t unknown_2252_address() const { return address + 2252; }
  float unknown_2420() const { return guest::LoadFloat(address + 2420); }
  void set_unknown_2420(float v) const { guest::StoreFloat(address + 2420, v); }
  uint32_t unknown_2420_address() const { return address + 2420; }
  float unknown_2944() const { return guest::LoadFloat(address + 2944); }
  void set_unknown_2944(float v) const { guest::StoreFloat(address + 2944, v); }
  uint32_t unknown_2944_address() const { return address + 2944; }
  uint32_t unknown_5712() const { return guest::LoadU32(address + 5712); }
  void set_unknown_5712(uint32_t v) const { guest::StoreU32(address + 5712, v); }
  uint32_t unknown_5712_address() const { return address + 5712; }
  uint32_t unknown_6168() const { return guest::LoadU32(address + 6168); }
  void set_unknown_6168(uint32_t v) const { guest::StoreU32(address + 6168, v); }
  uint32_t unknown_6168_address() const { return address + 6168; }
  float unknown_6252() const { return guest::LoadFloat(address + 6252); }
  void set_unknown_6252(float v) const { guest::StoreFloat(address + 6252, v); }
  uint32_t unknown_6252_address() const { return address + 6252; }
  float unknown_6256() const { return guest::LoadFloat(address + 6256); }
  void set_unknown_6256(float v) const { guest::StoreFloat(address + 6256, v); }
  uint32_t unknown_6256_address() const { return address + 6256; }
  float unknown_6260() const { return guest::LoadFloat(address + 6260); }
  void set_unknown_6260(float v) const { guest::StoreFloat(address + 6260, v); }
  uint32_t unknown_6260_address() const { return address + 6260; }
};

// embedded; measure: Vehicles::DestroyAll.r3@+37632
struct VehicleAt37632 : View {
  using View::View;
  static constexpr uint32_t kSize = 348;
  uint32_t unknown_336() const { return guest::LoadU32(address + 336); }
  void set_unknown_336(uint32_t v) const { guest::StoreU32(address + 336, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  uint32_t unknown_340() const { return guest::LoadU32(address + 340); }
  void set_unknown_340(uint32_t v) const { guest::StoreU32(address + 340, v); }
  uint32_t unknown_340_address() const { return address + 340; }
  uint32_t unknown_344() const { return guest::LoadU32(address + 344); }
  void set_unknown_344(uint32_t v) const { guest::StoreU32(address + 344, v); }
  uint32_t unknown_344_address() const { return address + 344; }
};

// embedded; measure: Vehicles::DestroyAll.r3@+37632.+344.+0
struct VehicleBoxVtablePart : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// The image words docs/functions/car-physics.md's code uses.
struct CarPhysicsGlobals {
  // a float in writable data: the brakes' cooling by speed (Vehicle::UpdateBrakeHeat)
  static float brake_cooling_by_speed() { return guest::LoadFloat(0x826C2D74u); }
  static void set_brake_cooling_by_speed(float v) { guest::StoreFloat(0x826C2D74u, v); }
  static constexpr uint32_t brake_cooling_by_speed_address() { return 0x826C2D74u; }
  // the first query (0x820FF1B0, TrackCollision::WallHits)
  static uint32_t query_walls() { return guest::LoadU32(0x826C2D78u); }
  static void set_query_walls(uint32_t v) { guest::StoreU32(0x826C2D78u, v); }
  static constexpr uint32_t query_walls_address() { return 0x826C2D78u; }
  // the second (0x820FEBD0, TrackCollision::GroundHits)
  static uint32_t query_ground() { return guest::LoadU32(0x826C2D7Cu); }
  static void set_query_ground(uint32_t v) { guest::StoreU32(0x826C2D7Cu, v); }
  static constexpr uint32_t query_ground_address() { return 0x826C2D7Cu; }
  // the pairs' clock
  static float pair_clock() { return guest::LoadFloat(0x826C2D80u); }
  static void set_pair_clock(float v) { guest::StoreFloat(0x826C2D80u, v); }
  static constexpr uint32_t pair_clock_address() { return 0x826C2D80u; }
  static float last_step() { return guest::LoadFloat(0x826C2D84u); }
  static void set_last_step(float v) { guest::StoreFloat(0x826C2D84u, v); }
  static constexpr uint32_t last_step_address() { return 0x826C2D84u; }
  template <typename T = CollisionWorld>
  static T collision_world() { return T(guest::LoadU32(0x826C2D88u)); }
  static void set_collision_world(const View& v) { guest::StoreU32(0x826C2D88u, v.address); }
  static void set_collision_world(uint32_t v) { guest::StoreU32(0x826C2D88u, v); }
  static constexpr uint32_t collision_world_address() { return 0x826C2D88u; }
  // four groups of four floats (CollisionSurface::A to D: the default and three bits')
  static constexpr uint32_t surface_settings_address() { return 0x826C2DF0u; }
  // six pointers to 88-byte records (by the car data's +176)
  static constexpr uint32_t axle_records_address() { return 0x8260181Cu; }
};

}  // namespace schema
