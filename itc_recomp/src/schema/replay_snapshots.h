// The views of docs/functions/replay-snapshots.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/replay-snapshots.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceSnapshot, RaceSnapshot::Copy.r4
struct ReplaySnapshot : View {
  using View::View;
  static constexpr uint32_t kSize = 38928;
  // the manager's first 77 entries (28 bytes each, from its +4)
  uint32_t entries_address() const { return address + 0; }
  // the manager's three traffic entries (its +2160)
  uint32_t traffic_entries_address() const { return address + 2156; }
  // the manager's 7 slot words (+2336 to +2360)
  uint32_t slot_0() const { return guest::LoadU32(base, address + 2240); }
  void set_slot_0(uint32_t v) const { guest::StoreU32(base, address + 2240, v); }
  uint32_t slot_0_address() const { return address + 2240; }
  uint32_t slot_1() const { return guest::LoadU32(base, address + 2244); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(base, address + 2244, v); }
  uint32_t slot_1_address() const { return address + 2244; }
  uint32_t slot_2() const { return guest::LoadU32(base, address + 2248); }
  void set_slot_2(uint32_t v) const { guest::StoreU32(base, address + 2248, v); }
  uint32_t slot_2_address() const { return address + 2248; }
  uint32_t slot_3() const { return guest::LoadU32(base, address + 2252); }
  void set_slot_3(uint32_t v) const { guest::StoreU32(base, address + 2252, v); }
  uint32_t slot_3_address() const { return address + 2252; }
  uint32_t slot_4() const { return guest::LoadU32(base, address + 2256); }
  void set_slot_4(uint32_t v) const { guest::StoreU32(base, address + 2256, v); }
  uint32_t slot_4_address() const { return address + 2256; }
  uint32_t slot_5() const { return guest::LoadU32(base, address + 2260); }
  void set_slot_5(uint32_t v) const { guest::StoreU32(base, address + 2260, v); }
  uint32_t slot_5_address() const { return address + 2260; }
  uint32_t slot_6() const { return guest::LoadU32(base, address + 2264); }
  void set_slot_6(uint32_t v) const { guest::StoreU32(base, address + 2264, v); }
  uint32_t slot_6_address() const { return address + 2264; }
  // the manager's +2364
  uint32_t racer_count() const { return guest::LoadU32(base, address + 2268); }
  void set_racer_count(uint32_t v) const { guest::StoreU32(base, address + 2268, v); }
  uint32_t racer_count_address() const { return address + 2268; }
  // the manager's +2368
  uint32_t entry_count() const { return guest::LoadU32(base, address + 2272); }
  void set_entry_count(uint32_t v) const { guest::StoreU32(base, address + 2272, v); }
  uint32_t entry_count_address() const { return address + 2272; }
  // the manager's +2372 (opponents streamed in)
  uint32_t opponent_count() const { return guest::LoadU32(base, address + 2276); }
  void set_opponent_count(uint32_t v) const { guest::StoreU32(base, address + 2276, v); }
  uint32_t opponent_count_address() const { return address + 2276; }
  // the manager's +2376
  uint32_t traffic_listed() const { return guest::LoadU32(base, address + 2280); }
  void set_traffic_listed(uint32_t v) const { guest::StoreU32(base, address + 2280, v); }
  uint32_t traffic_listed_address() const { return address + 2280; }
  // the manager's +2380
  uint32_t traffic_count() const { return guest::LoadU32(base, address + 2284); }
  void set_traffic_count(uint32_t v) const { guest::StoreU32(base, address + 2284, v); }
  uint32_t traffic_count_address() const { return address + 2284; }
  // the manager's +2388
  uint32_t unknown_2288() const { return guest::LoadU32(base, address + 2288); }
  void set_unknown_2288(uint32_t v) const { guest::StoreU32(base, address + 2288, v); }
  uint32_t unknown_2288_address() const { return address + 2288; }
  // 0 at RaceSnapshot::Save, copied by RaceSnapshot::Copy
  uint32_t unknown_2292() const { return guest::LoadU32(base, address + 2292); }
  void set_unknown_2292(uint32_t v) const { guest::StoreU32(base, address + 2292, v); }
  uint32_t unknown_2292_address() const { return address + 2292; }
  // seven ReplayRecords (5,232 bytes each), one a vehicle (Vehicle::Get(i))
  uint32_t records_address() const { return address + 2304; }
};

// file; measure: Record5232
struct ReplayRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 5232;
  // the vehicle's +6148 (0: the vehicle not in use, the record skipped)
  uint32_t car_flags() const { return guest::LoadU32(base, address + 0); }
  void set_car_flags(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t car_flags_address() const { return address + 0; }
  // the vehicle's +6152 (its contact bits)
  uint32_t collision_flags() const { return guest::LoadU32(base, address + 4); }
  void set_collision_flags(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t collision_flags_address() const { return address + 4; }
  // the vehicle's +6160 (VehicleStatus, memcpy)
  uint32_t status_address() const { return address + 16; }
  // the vehicle's five track positions (+5232, 96 bytes each; TrackPosition::Copy)
  uint32_t track_positions_address() const { return address + 128; }
  // a ReplayBodyState (516 bytes): the rigid body's (vehicle +336)
  uint32_t body_address() const { return address + 608; }
  // a ReplayVehiclePhysics (2,938 bytes): the vehicle's +0 to +3241, packed
  uint32_t physics_address() const { return address + 1136; }
  // a ReplayCollisionState: the collision object's (vehicle +340)
  uint32_t collision_address() const { return address + 4080; }
};

// embedded; measure: BodyState
struct ReplayBodyState : View {
  using View::View;
  static constexpr uint32_t kSize = 516;
  // set: no inverse mass
  uint8_t fixed() const { return guest::LoadU8(base, address + 0); }
  void set_fixed(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t fixed_address() const { return address + 0; }
  float mass() const { return guest::LoadFloat(base, address + 4); }
  void set_mass(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t mass_address() const { return address + 4; }
  float inverse_mass() const { return guest::LoadFloat(base, address + 8); }
  void set_inverse_mass(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t inverse_mass_address() const { return address + 8; }
  uint32_t center_of_mass_address() const { return address + 16; }
  uint32_t velocity_address() const { return address + 32; }
  float speed_cap() const { return guest::LoadFloat(base, address + 48); }
  void set_speed_cap(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t speed_cap_address() const { return address + 48; }
  uint32_t force_address() const { return address + 64; }
  // a quaternion, copied as two doublewords (ld, std)
  uint32_t orientation_address() const { return address + 80; }
  uint32_t angular_velocity_address() const { return address + 96; }
  float spin_cap() const { return guest::LoadFloat(base, address + 112); }
  void set_spin_cap(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t spin_cap_address() const { return address + 112; }
  uint32_t torque_address() const { return address + 128; }
  uint32_t velocity_direction_address() const { return address + 144; }
  float speed() const { return guest::LoadFloat(base, address + 160); }
  void set_speed(float v) const { guest::StoreFloat(base, address + 160, v); }
  uint32_t speed_address() const { return address + 160; }
  float linear_damping() const { return guest::LoadFloat(base, address + 164); }
  void set_linear_damping(float v) const { guest::StoreFloat(base, address + 164, v); }
  uint32_t linear_damping_address() const { return address + 164; }
  uint32_t momentum_address() const { return address + 176; }
  uint32_t center_of_mass_offset_address() const { return address + 192; }
  uint32_t origin_address() const { return address + 208; }
  uint32_t orientation_matrix_address() const { return address + 224; }
  uint32_t matrix_address() const { return address + 288; }
  uint32_t inertia_address() const { return address + 352; }
  uint32_t inverse_inertia_address() const { return address + 368; }
  float angular_damping() const { return guest::LoadFloat(base, address + 384); }
  void set_angular_damping(float v) const { guest::StoreFloat(base, address + 384, v); }
  uint32_t angular_damping_address() const { return address + 384; }
  uint32_t angular_momentum_address() const { return address + 400; }
  uint32_t world_inverse_inertia_address() const { return address + 416; }
  uint32_t gravity_address() const { return address + 480; }
  uint32_t weight_address() const { return address + 496; }
  float effective_mass() const { return guest::LoadFloat(base, address + 512); }
  void set_effective_mass(float v) const { guest::StoreFloat(base, address + 512, v); }
  uint32_t effective_mass_address() const { return address + 512; }
};

// file; measure: Member1136
struct ReplayVehiclePhysics : View {
  using View::View;
  static constexpr uint32_t kSize = 2944;
  // the vehicle's +0
  uint32_t body_row0_address() const { return address + 0; }
  // the vehicle's +16
  uint32_t body_row1_address() const { return address + 16; }
  // the vehicle's +32
  uint32_t body_row2_address() const { return address + 32; }
  // the vehicle's +48
  float speed() const { return guest::LoadFloat(base, address + 48); }
  void set_speed(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t speed_address() const { return address + 48; }
  // the vehicle's +52
  float unknown_52() const { return guest::LoadFloat(base, address + 52); }
  void set_unknown_52(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  // the vehicle's +64
  uint32_t angular_velocity_address() const { return address + 64; }
  // the vehicle's +80
  float unknown_80() const { return guest::LoadFloat(base, address + 80); }
  void set_unknown_80(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  // the vehicle's +84
  float unknown_84() const { return guest::LoadFloat(base, address + 84); }
  void set_unknown_84(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  // the vehicle's +88
  float roll() const { return guest::LoadFloat(base, address + 88); }
  void set_roll(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t roll_address() const { return address + 88; }
  // the vehicle's +92
  float slip_term() const { return guest::LoadFloat(base, address + 92); }
  void set_slip_term(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t slip_term_address() const { return address + 92; }
  // the vehicle's +96
  uint32_t ground_up_address() const { return address + 96; }
  // the vehicle's +112
  uint32_t ground_forward_address() const { return address + 112; }
  // the vehicle's +128
  uint32_t ground_side_address() const { return address + 128; }
  // the vehicle's +144
  uint32_t ground_matrix_address() const { return address + 144; }
  // the vehicle's +208
  uint32_t body_matrix_address() const { return address + 208; }
  // the vehicle's +276
  uint8_t speed_sensitive_steering() const { return guest::LoadU8(base, address + 272); }
  void set_speed_sensitive_steering(uint8_t v) const { guest::StoreU8(base, address + 272, v); }
  uint32_t speed_sensitive_steering_address() const { return address + 272; }
  // the vehicle's +280
  float steering_input() const { return guest::LoadFloat(base, address + 276); }
  void set_steering_input(float v) const { guest::StoreFloat(base, address + 276, v); }
  uint32_t steering_input_address() const { return address + 276; }
  // the vehicle's +284
  float throttle() const { return guest::LoadFloat(base, address + 280); }
  void set_throttle(float v) const { guest::StoreFloat(base, address + 280, v); }
  uint32_t throttle_address() const { return address + 280; }
  // the vehicle's +288
  float reverse_throttle() const { return guest::LoadFloat(base, address + 284); }
  void set_reverse_throttle(float v) const { guest::StoreFloat(base, address + 284, v); }
  uint32_t reverse_throttle_address() const { return address + 284; }
  // the vehicle's +292
  float brake() const { return guest::LoadFloat(base, address + 288); }
  void set_brake(float v) const { guest::StoreFloat(base, address + 288, v); }
  uint32_t brake_address() const { return address + 288; }
  // the vehicle's +296
  float handbrake() const { return guest::LoadFloat(base, address + 292); }
  void set_handbrake(float v) const { guest::StoreFloat(base, address + 292, v); }
  uint32_t handbrake_address() const { return address + 292; }
  // the vehicle's +300
  float unknown_296() const { return guest::LoadFloat(base, address + 296); }
  void set_unknown_296(float v) const { guest::StoreFloat(base, address + 296, v); }
  uint32_t unknown_296_address() const { return address + 296; }
  // the vehicle's +304
  float steering() const { return guest::LoadFloat(base, address + 300); }
  void set_steering(float v) const { guest::StoreFloat(base, address + 300, v); }
  uint32_t steering_address() const { return address + 300; }
  // the vehicle's +308
  float front_left_angle() const { return guest::LoadFloat(base, address + 304); }
  void set_front_left_angle(float v) const { guest::StoreFloat(base, address + 304, v); }
  uint32_t front_left_angle_address() const { return address + 304; }
  // the vehicle's +312
  float front_right_angle() const { return guest::LoadFloat(base, address + 308); }
  void set_front_right_angle(float v) const { guest::StoreFloat(base, address + 308, v); }
  uint32_t front_right_angle_address() const { return address + 308; }
  // the vehicle's +316
  float steering_limit_left() const { return guest::LoadFloat(base, address + 312); }
  void set_steering_limit_left(float v) const { guest::StoreFloat(base, address + 312, v); }
  uint32_t steering_limit_left_address() const { return address + 312; }
  // the vehicle's +320
  float steering_limit_right() const { return guest::LoadFloat(base, address + 316); }
  void set_steering_limit_right(float v) const { guest::StoreFloat(base, address + 316, v); }
  uint32_t steering_limit_right_address() const { return address + 316; }
  // the vehicle's +324
  float steering_smoothed() const { return guest::LoadFloat(base, address + 320); }
  void set_steering_smoothed(float v) const { guest::StoreFloat(base, address + 320, v); }
  uint32_t steering_smoothed_address() const { return address + 320; }
  // the vehicle's +328
  float steering_previous() const { return guest::LoadFloat(base, address + 324); }
  void set_steering_previous(float v) const { guest::StoreFloat(base, address + 324, v); }
  uint32_t steering_previous_address() const { return address + 324; }
  // the vehicle's +332
  float unknown_328() const { return guest::LoadFloat(base, address + 328); }
  void set_unknown_328(float v) const { guest::StoreFloat(base, address + 328, v); }
  uint32_t unknown_328_address() const { return address + 328; }
  // the vehicle's +348
  uint32_t wheels_on_ground() const { return guest::LoadU32(base, address + 332); }
  void set_wheels_on_ground(uint32_t v) const { guest::StoreU32(base, address + 332, v); }
  uint32_t wheels_on_ground_address() const { return address + 332; }
  // the vehicle's +352
  uint32_t unknown_336_address() const { return address + 336; }
  // the vehicle's +368
  uint32_t wheel_forces_ground_address() const { return address + 352; }
  // the vehicle's +384
  uint32_t ground_velocity_address() const { return address + 368; }
  // the vehicle's +400
  uint32_t unknown_384_address() const { return address + 384; }
  // the vehicle's +416
  uint32_t wheel_0_address() const { return address + 400; }
  // the vehicle's +784
  uint32_t wheel_1_address() const { return address + 768; }
  // the vehicle's +1152
  uint32_t wheel_2_address() const { return address + 1136; }
  // the vehicle's +1520
  uint32_t wheel_3_address() const { return address + 1504; }
  // the vehicle's +1888
  uint32_t front_axle_address() const { return address + 1872; }
  // the vehicle's +2016
  uint32_t rear_axle_address() const { return address + 2000; }
  // the vehicle's +2176: the wheel indices sorted by load
  uint32_t wheel_by_load_0() const { return guest::LoadU32(base, address + 2128); }
  void set_wheel_by_load_0(uint32_t v) const { guest::StoreU32(base, address + 2128, v); }
  uint32_t wheel_by_load_0_address() const { return address + 2128; }
  // the vehicle's +2180
  uint32_t wheel_by_load_1() const { return guest::LoadU32(base, address + 2132); }
  void set_wheel_by_load_1(uint32_t v) const { guest::StoreU32(base, address + 2132, v); }
  uint32_t wheel_by_load_1_address() const { return address + 2132; }
  // the vehicle's +2184
  uint32_t wheel_by_load_2() const { return guest::LoadU32(base, address + 2136); }
  void set_wheel_by_load_2(uint32_t v) const { guest::StoreU32(base, address + 2136, v); }
  uint32_t wheel_by_load_2_address() const { return address + 2136; }
  // the vehicle's +2188
  uint32_t wheel_by_load_3() const { return guest::LoadU32(base, address + 2140); }
  void set_wheel_by_load_3(uint32_t v) const { guest::StoreU32(base, address + 2140, v); }
  uint32_t wheel_by_load_3_address() const { return address + 2140; }
  // the vehicle's +2192
  float driven_axle_spin() const { return guest::LoadFloat(base, address + 2144); }
  void set_driven_axle_spin(float v) const { guest::StoreFloat(base, address + 2144, v); }
  uint32_t driven_axle_spin_address() const { return address + 2144; }
  // the vehicle's +2196
  float engine_spin() const { return guest::LoadFloat(base, address + 2148); }
  void set_engine_spin(float v) const { guest::StoreFloat(base, address + 2148, v); }
  uint32_t engine_spin_address() const { return address + 2148; }
  // the vehicle's +2200
  float engine_spin_previous() const { return guest::LoadFloat(base, address + 2152); }
  void set_engine_spin_previous(float v) const { guest::StoreFloat(base, address + 2152, v); }
  uint32_t engine_spin_previous_address() const { return address + 2152; }
  // the vehicle's +2204 to +2347 (memcpy)
  uint32_t drivetrain_block_address() const { return address + 2156; }
  // the vehicle's +2348
  float direction() const { return guest::LoadFloat(base, address + 2300); }
  void set_direction(float v) const { guest::StoreFloat(base, address + 2300, v); }
  uint32_t direction_address() const { return address + 2300; }
  // the vehicle's +2352
  float dt() const { return guest::LoadFloat(base, address + 2304); }
  void set_dt(float v) const { guest::StoreFloat(base, address + 2304, v); }
  uint32_t dt_address() const { return address + 2304; }
  // the vehicle's +2356
  float inverse_dt() const { return guest::LoadFloat(base, address + 2308); }
  void set_inverse_dt(float v) const { guest::StoreFloat(base, address + 2308, v); }
  uint32_t inverse_dt_address() const { return address + 2308; }
  // the vehicle's +2360
  float unknown_2312() const { return guest::LoadFloat(base, address + 2312); }
  void set_unknown_2312(float v) const { guest::StoreFloat(base, address + 2312, v); }
  uint32_t unknown_2312_address() const { return address + 2312; }
  // the vehicle's +2364
  float substep_dt() const { return guest::LoadFloat(base, address + 2316); }
  void set_substep_dt(float v) const { guest::StoreFloat(base, address + 2316, v); }
  uint32_t substep_dt_address() const { return address + 2316; }
  // the vehicle's +2368
  uint32_t substep_count() const { return guest::LoadU32(base, address + 2320); }
  void set_substep_count(uint32_t v) const { guest::StoreU32(base, address + 2320, v); }
  uint32_t substep_count_address() const { return address + 2320; }
  // the vehicle's +2372
  uint32_t flags() const { return guest::LoadU32(base, address + 2324); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 2324, v); }
  uint32_t flags_address() const { return address + 2324; }
  // the vehicle's +2376
  uint32_t drive_type() const { return guest::LoadU32(base, address + 2328); }
  void set_drive_type(uint32_t v) const { guest::StoreU32(base, address + 2328, v); }
  uint32_t drive_type_address() const { return address + 2328; }
  // the vehicle's +2380
  uint8_t unknown_2332() const { return guest::LoadU8(base, address + 2332); }
  void set_unknown_2332(uint8_t v) const { guest::StoreU8(base, address + 2332, v); }
  uint32_t unknown_2332_address() const { return address + 2332; }
  // the vehicle's +2381
  uint8_t unknown_2333() const { return guest::LoadU8(base, address + 2333); }
  void set_unknown_2333(uint8_t v) const { guest::StoreU8(base, address + 2333, v); }
  uint32_t unknown_2333_address() const { return address + 2333; }
  // the vehicle's +2382
  uint8_t unknown_2334() const { return guest::LoadU8(base, address + 2334); }
  void set_unknown_2334(uint8_t v) const { guest::StoreU8(base, address + 2334, v); }
  uint32_t unknown_2334_address() const { return address + 2334; }
  // the vehicle's +2383
  uint8_t unknown_2335() const { return guest::LoadU8(base, address + 2335); }
  void set_unknown_2335(uint8_t v) const { guest::StoreU8(base, address + 2335, v); }
  uint32_t unknown_2335_address() const { return address + 2335; }
  // the vehicle's +2384
  uint8_t forced_rest() const { return guest::LoadU8(base, address + 2336); }
  void set_forced_rest(uint8_t v) const { guest::StoreU8(base, address + 2336, v); }
  uint32_t forced_rest_address() const { return address + 2336; }
  // the vehicle's +2385
  uint8_t value2388_set() const { return guest::LoadU8(base, address + 2337); }
  void set_value2388_set(uint8_t v) const { guest::StoreU8(base, address + 2337, v); }
  uint32_t value2388_set_address() const { return address + 2337; }
  // the vehicle's +2386
  uint8_t abs() const { return guest::LoadU8(base, address + 2338); }
  void set_abs(uint8_t v) const { guest::StoreU8(base, address + 2338, v); }
  uint32_t abs_address() const { return address + 2338; }
  // the vehicle's +2387
  uint8_t unknown_2339() const { return guest::LoadU8(base, address + 2339); }
  void set_unknown_2339(uint8_t v) const { guest::StoreU8(base, address + 2339, v); }
  uint32_t unknown_2339_address() const { return address + 2339; }
  // the vehicle's +2388
  float value2388() const { return guest::LoadFloat(base, address + 2340); }
  void set_value2388(float v) const { guest::StoreFloat(base, address + 2340, v); }
  uint32_t value2388_address() const { return address + 2340; }
  // the vehicle's +2392
  uint32_t traction_counter() const { return guest::LoadU32(base, address + 2344); }
  void set_traction_counter(uint32_t v) const { guest::StoreU32(base, address + 2344, v); }
  uint32_t traction_counter_address() const { return address + 2344; }
  // the vehicle's +2396
  float traction_limit() const { return guest::LoadFloat(base, address + 2348); }
  void set_traction_limit(float v) const { guest::StoreFloat(base, address + 2348, v); }
  uint32_t traction_limit_address() const { return address + 2348; }
  // the vehicle's +2400
  uint8_t traction_active() const { return guest::LoadU8(base, address + 2352); }
  void set_traction_active(uint8_t v) const { guest::StoreU8(base, address + 2352, v); }
  uint32_t traction_active_address() const { return address + 2352; }
  // the vehicle's +2401
  uint8_t abs_active() const { return guest::LoadU8(base, address + 2353); }
  void set_abs_active(uint8_t v) const { guest::StoreU8(base, address + 2353, v); }
  uint32_t abs_active_address() const { return address + 2353; }
  // the vehicle's +2404
  float drag_scale() const { return guest::LoadFloat(base, address + 2356); }
  void set_drag_scale(float v) const { guest::StoreFloat(base, address + 2356, v); }
  uint32_t drag_scale_address() const { return address + 2356; }
  // the vehicle's +2408
  float grip_scale() const { return guest::LoadFloat(base, address + 2360); }
  void set_grip_scale(float v) const { guest::StoreFloat(base, address + 2360, v); }
  uint32_t grip_scale_address() const { return address + 2360; }
  // the vehicle's +2412
  float torque_scale() const { return guest::LoadFloat(base, address + 2364); }
  void set_torque_scale(float v) const { guest::StoreFloat(base, address + 2364, v); }
  uint32_t torque_scale_address() const { return address + 2364; }
  // the vehicle's +2428
  float oil_factor() const { return guest::LoadFloat(base, address + 2368); }
  void set_oil_factor(float v) const { guest::StoreFloat(base, address + 2368, v); }
  uint32_t oil_factor_address() const { return address + 2368; }
  // the vehicle's +2432
  float brake_efficiency() const { return guest::LoadFloat(base, address + 2372); }
  void set_brake_efficiency(float v) const { guest::StoreFloat(base, address + 2372, v); }
  uint32_t brake_efficiency_address() const { return address + 2372; }
  // the vehicle's +2436
  float unknown_2376() const { return guest::LoadFloat(base, address + 2376); }
  void set_unknown_2376(float v) const { guest::StoreFloat(base, address + 2376, v); }
  uint32_t unknown_2376_address() const { return address + 2376; }
  // the vehicle's +2416
  float nitrous_factor() const { return guest::LoadFloat(base, address + 2380); }
  void set_nitrous_factor(float v) const { guest::StoreFloat(base, address + 2380, v); }
  uint32_t nitrous_factor_address() const { return address + 2380; }
  // the vehicle's +2420
  float nitrous_value() const { return guest::LoadFloat(base, address + 2384); }
  void set_nitrous_value(float v) const { guest::StoreFloat(base, address + 2384, v); }
  uint32_t nitrous_value_address() const { return address + 2384; }
  // the vehicle's +2424
  uint8_t nitrous_on() const { return guest::LoadU8(base, address + 2388); }
  void set_nitrous_on(uint8_t v) const { guest::StoreU8(base, address + 2388, v); }
  uint32_t nitrous_on_address() const { return address + 2388; }
  // the vehicle's +2440
  float speed_squared() const { return guest::LoadFloat(base, address + 2392); }
  void set_speed_squared(float v) const { guest::StoreFloat(base, address + 2392, v); }
  uint32_t speed_squared_address() const { return address + 2392; }
  // the vehicle's +2444
  uint8_t unknown_2396() const { return guest::LoadU8(base, address + 2396); }
  void set_unknown_2396(uint8_t v) const { guest::StoreU8(base, address + 2396, v); }
  uint32_t unknown_2396_address() const { return address + 2396; }
  // the vehicle's +2445
  uint8_t unknown_2397() const { return guest::LoadU8(base, address + 2397); }
  void set_unknown_2397(uint8_t v) const { guest::StoreU8(base, address + 2397, v); }
  uint32_t unknown_2397_address() const { return address + 2397; }
  // the vehicle's +2448
  float unknown_2400() const { return guest::LoadFloat(base, address + 2400); }
  void set_unknown_2400(float v) const { guest::StoreFloat(base, address + 2400, v); }
  uint32_t unknown_2400_address() const { return address + 2400; }
  // the vehicle's +2452
  float unknown_2404() const { return guest::LoadFloat(base, address + 2404); }
  void set_unknown_2404(float v) const { guest::StoreFloat(base, address + 2404, v); }
  uint32_t unknown_2404_address() const { return address + 2404; }
  // the vehicle's +2456
  uint8_t unknown_2408() const { return guest::LoadU8(base, address + 2408); }
  void set_unknown_2408(uint8_t v) const { guest::StoreU8(base, address + 2408, v); }
  uint32_t unknown_2408_address() const { return address + 2408; }
  // the vehicle's +2464
  uint32_t body_force_address() const { return address + 2416; }
  // the vehicle's +2480
  uint32_t body_torque_address() const { return address + 2432; }
  // the vehicle's +2496
  uint32_t tire_wear_mode() const { return guest::LoadU32(base, address + 2448); }
  void set_tire_wear_mode(uint32_t v) const { guest::StoreU32(base, address + 2448, v); }
  uint32_t tire_wear_mode_address() const { return address + 2448; }
  // the vehicle's +2500
  float backfire_charge() const { return guest::LoadFloat(base, address + 2452); }
  void set_backfire_charge(float v) const { guest::StoreFloat(base, address + 2452, v); }
  uint32_t backfire_charge_address() const { return address + 2452; }
  // the vehicle's +2504
  uint8_t backfire() const { return guest::LoadU8(base, address + 2456); }
  void set_backfire(uint8_t v) const { guest::StoreU8(base, address + 2456, v); }
  uint32_t backfire_address() const { return address + 2456; }
  // the vehicle's +2508
  float backfire_strength() const { return guest::LoadFloat(base, address + 2460); }
  void set_backfire_strength(float v) const { guest::StoreFloat(base, address + 2460, v); }
  uint32_t backfire_strength_address() const { return address + 2460; }
  // the vehicle's +2512
  uint8_t unknown_2464() const { return guest::LoadU8(base, address + 2464); }
  void set_unknown_2464(uint8_t v) const { guest::StoreU8(base, address + 2464, v); }
  uint32_t unknown_2464_address() const { return address + 2464; }
  // the vehicle's +2516
  float throttle_used() const { return guest::LoadFloat(base, address + 2468); }
  void set_throttle_used(float v) const { guest::StoreFloat(base, address + 2468, v); }
  uint32_t throttle_used_address() const { return address + 2468; }
  // the vehicle's +2520
  float throttle_used_previous() const { return guest::LoadFloat(base, address + 2472); }
  void set_throttle_used_previous(float v) const { guest::StoreFloat(base, address + 2472, v); }
  uint32_t throttle_used_previous_address() const { return address + 2472; }
  // the vehicle's +2524
  uint32_t unknown_2476() const { return guest::LoadU32(base, address + 2476); }
  void set_unknown_2476(uint32_t v) const { guest::StoreU32(base, address + 2476, v); }
  uint32_t unknown_2476_address() const { return address + 2476; }
  // the vehicle's +2528
  uint32_t axis_address() const { return address + 2480; }
  // the vehicle's +2544
  uint32_t wheel_accumulator_0_address() const { return address + 2496; }
  // the vehicle's +2560
  uint32_t wheel_accumulator_1_address() const { return address + 2512; }
  // the vehicle's +2576
  uint32_t wheel_accumulator_2_address() const { return address + 2528; }
  // the vehicle's +2592
  uint32_t wheel_accumulator_3_address() const { return address + 2544; }
  // the vehicle's +2608
  uint32_t body_accumulator_address() const { return address + 2560; }
  // the vehicle's +2624
  uint8_t unknown_2576() const { return guest::LoadU8(base, address + 2576); }
  void set_unknown_2576(uint8_t v) const { guest::StoreU8(base, address + 2576, v); }
  uint32_t unknown_2576_address() const { return address + 2576; }
  // the vehicle's +2625
  uint8_t unknown_2577() const { return guest::LoadU8(base, address + 2577); }
  void set_unknown_2577(uint8_t v) const { guest::StoreU8(base, address + 2577, v); }
  uint32_t unknown_2577_address() const { return address + 2577; }
  // the vehicle's +2628
  float unknown_2580() const { return guest::LoadFloat(base, address + 2580); }
  void set_unknown_2580(float v) const { guest::StoreFloat(base, address + 2580, v); }
  uint32_t unknown_2580_address() const { return address + 2580; }
  // the vehicle's +2632
  float shift_up_speed_0() const { return guest::LoadFloat(base, address + 2584); }
  void set_shift_up_speed_0(float v) const { guest::StoreFloat(base, address + 2584, v); }
  uint32_t shift_up_speed_0_address() const { return address + 2584; }
  // the vehicle's +2636
  float shift_up_speed_1() const { return guest::LoadFloat(base, address + 2588); }
  void set_shift_up_speed_1(float v) const { guest::StoreFloat(base, address + 2588, v); }
  uint32_t shift_up_speed_1_address() const { return address + 2588; }
  // the vehicle's +2640
  float shift_up_speed_2() const { return guest::LoadFloat(base, address + 2592); }
  void set_shift_up_speed_2(float v) const { guest::StoreFloat(base, address + 2592, v); }
  uint32_t shift_up_speed_2_address() const { return address + 2592; }
  // the vehicle's +2644
  float shift_up_speed_3() const { return guest::LoadFloat(base, address + 2596); }
  void set_shift_up_speed_3(float v) const { guest::StoreFloat(base, address + 2596, v); }
  uint32_t shift_up_speed_3_address() const { return address + 2596; }
  // the vehicle's +2648
  float shift_up_speed_4() const { return guest::LoadFloat(base, address + 2600); }
  void set_shift_up_speed_4(float v) const { guest::StoreFloat(base, address + 2600, v); }
  uint32_t shift_up_speed_4_address() const { return address + 2600; }
  // the vehicle's +2652
  float shift_up_speed_5() const { return guest::LoadFloat(base, address + 2604); }
  void set_shift_up_speed_5(float v) const { guest::StoreFloat(base, address + 2604, v); }
  uint32_t shift_up_speed_5_address() const { return address + 2604; }
  // the vehicle's +2656
  float shift_up_speed_6() const { return guest::LoadFloat(base, address + 2608); }
  void set_shift_up_speed_6(float v) const { guest::StoreFloat(base, address + 2608, v); }
  uint32_t shift_up_speed_6_address() const { return address + 2608; }
  // the vehicle's +2660
  float shift_up_speed_7() const { return guest::LoadFloat(base, address + 2612); }
  void set_shift_up_speed_7(float v) const { guest::StoreFloat(base, address + 2612, v); }
  uint32_t shift_up_speed_7_address() const { return address + 2612; }
  // the vehicle's +2664
  uint8_t surfaces_on() const { return guest::LoadU8(base, address + 2616); }
  void set_surfaces_on(uint8_t v) const { guest::StoreU8(base, address + 2616, v); }
  uint32_t surfaces_on_address() const { return address + 2616; }
  // the vehicle's +2924
  float unknown_2620() const { return guest::LoadFloat(base, address + 2620); }
  void set_unknown_2620(float v) const { guest::StoreFloat(base, address + 2620, v); }
  uint32_t unknown_2620_address() const { return address + 2620; }
  // the vehicle's +2928
  uint8_t at_rest() const { return guest::LoadU8(base, address + 2624); }
  void set_at_rest(uint8_t v) const { guest::StoreU8(base, address + 2624, v); }
  uint32_t at_rest_address() const { return address + 2624; }
  // the vehicle's +2932
  float unknown_2628() const { return guest::LoadFloat(base, address + 2628); }
  void set_unknown_2628(float v) const { guest::StoreFloat(base, address + 2628, v); }
  uint32_t unknown_2628_address() const { return address + 2628; }
  // the vehicle's +2936
  float unknown_2632() const { return guest::LoadFloat(base, address + 2632); }
  void set_unknown_2632(float v) const { guest::StoreFloat(base, address + 2632, v); }
  uint32_t unknown_2632_address() const { return address + 2632; }
  // the vehicle's +2940
  float unknown_2636() const { return guest::LoadFloat(base, address + 2636); }
  void set_unknown_2636(float v) const { guest::StoreFloat(base, address + 2636, v); }
  uint32_t unknown_2636_address() const { return address + 2636; }
  // the vehicle's +2944
  float boost() const { return guest::LoadFloat(base, address + 2640); }
  void set_boost(float v) const { guest::StoreFloat(base, address + 2640, v); }
  uint32_t boost_address() const { return address + 2640; }
  // the vehicle's +2948
  uint8_t wastegate_open() const { return guest::LoadU8(base, address + 2644); }
  void set_wastegate_open(uint8_t v) const { guest::StoreU8(base, address + 2644, v); }
  uint32_t wastegate_open_address() const { return address + 2644; }
  // the vehicle's +2952
  float wastegate() const { return guest::LoadFloat(base, address + 2648); }
  void set_wastegate(float v) const { guest::StoreFloat(base, address + 2648, v); }
  uint32_t wastegate_address() const { return address + 2648; }
  // the vehicle's +2956
  float turbo_torque_factor() const { return guest::LoadFloat(base, address + 2652); }
  void set_turbo_torque_factor(float v) const { guest::StoreFloat(base, address + 2652, v); }
  uint32_t turbo_torque_factor_address() const { return address + 2652; }
  // the vehicle's +2960
  uint8_t blow_off_open() const { return guest::LoadU8(base, address + 2656); }
  void set_blow_off_open(uint8_t v) const { guest::StoreU8(base, address + 2656, v); }
  uint32_t blow_off_open_address() const { return address + 2656; }
  // the vehicle's +2964
  float blow_off() const { return guest::LoadFloat(base, address + 2660); }
  void set_blow_off(float v) const { guest::StoreFloat(base, address + 2660, v); }
  uint32_t blow_off_address() const { return address + 2660; }
  // the vehicle's +2968
  float gauge_0() const { return guest::LoadFloat(base, address + 2664); }
  void set_gauge_0(float v) const { guest::StoreFloat(base, address + 2664, v); }
  uint32_t gauge_0_address() const { return address + 2664; }
  // the vehicle's +2972
  float gauge_1() const { return guest::LoadFloat(base, address + 2668); }
  void set_gauge_1(float v) const { guest::StoreFloat(base, address + 2668, v); }
  uint32_t gauge_1_address() const { return address + 2668; }
  // the vehicle's +2976
  float gauge_2() const { return guest::LoadFloat(base, address + 2672); }
  void set_gauge_2(float v) const { guest::StoreFloat(base, address + 2672, v); }
  uint32_t gauge_2_address() const { return address + 2672; }
  // the vehicle's +2980
  float gauge_3() const { return guest::LoadFloat(base, address + 2676); }
  void set_gauge_3(float v) const { guest::StoreFloat(base, address + 2676, v); }
  uint32_t gauge_3_address() const { return address + 2676; }
  // the vehicle's +2984
  uint32_t track_position() const { return guest::LoadU32(base, address + 2680); }
  void set_track_position(uint32_t v) const { guest::StoreU32(base, address + 2680, v); }
  uint32_t track_position_address() const { return address + 2680; }
  // the vehicle's +2988 to +3047 (memcpy; its SteerRecord first)
  uint32_t steer_block_a_address() const { return address + 2684; }
  // the vehicle's +3048 to +3107 (memcpy)
  uint32_t steer_block_b_address() const { return address + 2744; }
  // the vehicle's +3108
  float steering_result() const { return guest::LoadFloat(base, address + 2804); }
  void set_steering_result(float v) const { guest::StoreFloat(base, address + 2804, v); }
  uint32_t steering_result_address() const { return address + 2804; }
  // the vehicle's +3112
  float yaw_rate() const { return guest::LoadFloat(base, address + 2808); }
  void set_yaw_rate(float v) const { guest::StoreFloat(base, address + 2808, v); }
  uint32_t yaw_rate_address() const { return address + 2808; }
  // the vehicle's +3116
  float unknown_2812() const { return guest::LoadFloat(base, address + 2812); }
  void set_unknown_2812(float v) const { guest::StoreFloat(base, address + 2812, v); }
  uint32_t unknown_2812_address() const { return address + 2812; }
  // the vehicle's +3120
  float unknown_2816() const { return guest::LoadFloat(base, address + 2816); }
  void set_unknown_2816(float v) const { guest::StoreFloat(base, address + 2816, v); }
  uint32_t unknown_2816_address() const { return address + 2816; }
  // the vehicle's +3124
  float unknown_2820() const { return guest::LoadFloat(base, address + 2820); }
  void set_unknown_2820(float v) const { guest::StoreFloat(base, address + 2820, v); }
  uint32_t unknown_2820_address() const { return address + 2820; }
  // the vehicle's +3128
  float unknown_2824() const { return guest::LoadFloat(base, address + 2824); }
  void set_unknown_2824(float v) const { guest::StoreFloat(base, address + 2824, v); }
  uint32_t unknown_2824_address() const { return address + 2824; }
  // the vehicle's +3132
  uint8_t upright() const { return guest::LoadU8(base, address + 2828); }
  void set_upright(uint8_t v) const { guest::StoreU8(base, address + 2828, v); }
  uint32_t upright_address() const { return address + 2828; }
  // the vehicle's +3136
  float unknown_2832() const { return guest::LoadFloat(base, address + 2832); }
  void set_unknown_2832(float v) const { guest::StoreFloat(base, address + 2832, v); }
  uint32_t unknown_2832_address() const { return address + 2832; }
  // the vehicle's +3140
  float unknown_2836() const { return guest::LoadFloat(base, address + 2836); }
  void set_unknown_2836(float v) const { guest::StoreFloat(base, address + 2836, v); }
  uint32_t unknown_2836_address() const { return address + 2836; }
  // the vehicle's +3144
  float unknown_2840() const { return guest::LoadFloat(base, address + 2840); }
  void set_unknown_2840(float v) const { guest::StoreFloat(base, address + 2840, v); }
  uint32_t unknown_2840_address() const { return address + 2840; }
  // the vehicle's +3148
  float unknown_2844() const { return guest::LoadFloat(base, address + 2844); }
  void set_unknown_2844(float v) const { guest::StoreFloat(base, address + 2844, v); }
  uint32_t unknown_2844_address() const { return address + 2844; }
  // the vehicle's +3152
  float unknown_2848() const { return guest::LoadFloat(base, address + 2848); }
  void set_unknown_2848(float v) const { guest::StoreFloat(base, address + 2848, v); }
  uint32_t unknown_2848_address() const { return address + 2848; }
  // the vehicle's +3156
  uint32_t step_count() const { return guest::LoadU32(base, address + 2852); }
  void set_step_count(uint32_t v) const { guest::StoreU32(base, address + 2852, v); }
  uint32_t step_count_address() const { return address + 2852; }
  // the vehicle's +3168
  uint32_t unknown_2864_address() const { return address + 2864; }
  // the vehicle's +3184
  uint32_t unknown_2880_address() const { return address + 2880; }
  // the vehicle's +3200
  uint32_t unknown_2896_address() const { return address + 2896; }
  // the vehicle's +3216
  uint32_t unknown_2912_address() const { return address + 2912; }
  // the vehicle's +3232
  float brake_fade() const { return guest::LoadFloat(base, address + 2928); }
  void set_brake_fade(float v) const { guest::StoreFloat(base, address + 2928, v); }
  uint32_t brake_fade_address() const { return address + 2928; }
  // the vehicle's +3236
  float brake_fade_copy() const { return guest::LoadFloat(base, address + 2932); }
  void set_brake_fade_copy(float v) const { guest::StoreFloat(base, address + 2932, v); }
  uint32_t brake_fade_copy_address() const { return address + 2932; }
  // the vehicle's +3240
  uint8_t backfire_latch() const { return guest::LoadU8(base, address + 2936); }
  void set_backfire_latch(uint8_t v) const { guest::StoreU8(base, address + 2936, v); }
  uint32_t backfire_latch_address() const { return address + 2936; }
  // the vehicle's +3241
  uint8_t tipped() const { return guest::LoadU8(base, address + 2937); }
  void set_tipped(uint8_t v) const { guest::StoreU8(base, address + 2937, v); }
  uint32_t tipped_address() const { return address + 2937; }
};

// file; measure: Member4080
struct ReplayCollisionState : View {
  using View::View;
  static constexpr uint32_t kSize = 1152;
  // 1 when saved from a collision object, 0 for none (nothing restored then)
  uint32_t has_object() const { return guest::LoadU32(base, address + 0); }
  void set_has_object(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t has_object_address() const { return address + 0; }
  // a ReplayShapeState (452 bytes): the object's +36 (the shape record's previous placed copy)
  uint32_t shape_previous_address() const { return address + 16; }
  // a ReplayShapeState: the object's +40 (the current placed copy)
  uint32_t shape_current_address() const { return address + 480; }
  // the object's +48 (its shape record's box, min)
  uint32_t box_min_address() const { return address + 944; }
  // the object's +64 (max)
  uint32_t box_max_address() const { return address + 960; }
  // the object's +80 (the motion matrix)
  uint32_t motion_address() const { return address + 976; }
  // the object's +144
  float lowest_travel() const { return guest::LoadFloat(base, address + 1040); }
  void set_lowest_travel(float v) const { guest::StoreFloat(base, address + 1040, v); }
  uint32_t lowest_travel_address() const { return address + 1040; }
  // the object's +160
  uint32_t push_address() const { return address + 1056; }
  // the object's +176
  uint32_t pair_torque_address() const { return address + 1072; }
  // the object's +192
  uint32_t push_count() const { return guest::LoadU32(base, address + 1088); }
  void set_push_count(uint32_t v) const { guest::StoreU32(base, address + 1088, v); }
  uint32_t push_count_address() const { return address + 1088; }
  // the object's +208
  uint32_t push_scale_address() const { return address + 1104; }
  // the object's +224
  float torque_scale() const { return guest::LoadFloat(base, address + 1120); }
  void set_torque_scale(float v) const { guest::StoreFloat(base, address + 1120, v); }
  uint32_t torque_scale_address() const { return address + 1120; }
  // the object's +288
  uint32_t moved() const { return guest::LoadU32(base, address + 1124); }
  void set_moved(uint32_t v) const { guest::StoreU32(base, address + 1124, v); }
  uint32_t moved_address() const { return address + 1124; }
  // the object's +304
  uint32_t push_out_address() const { return address + 1136; }
};

// file; measure: Part464
struct ReplayShapeState : View {
  using View::View;
  static constexpr uint32_t kSize = 452;
  // the shape's +128
  uint32_t half_extent_address() const { return address + 0; }
  // the shape's +144
  uint32_t inverse_half_extent_address() const { return address + 16; }
  // the shape's +176 to +303: its 8 corners
  uint32_t corners_address() const { return address + 32; }
  // the shape's +304
  float face_px_distance() const { return guest::LoadFloat(base, address + 160); }
  void set_face_px_distance(float v) const { guest::StoreFloat(base, address + 160, v); }
  uint32_t face_px_distance_address() const { return address + 160; }
  // the shape's +320
  uint32_t face_px_normal_address() const { return address + 176; }
  // the shape's +336
  float face_py_distance() const { return guest::LoadFloat(base, address + 192); }
  void set_face_py_distance(float v) const { guest::StoreFloat(base, address + 192, v); }
  uint32_t face_py_distance_address() const { return address + 192; }
  // the shape's +352
  uint32_t face_py_normal_address() const { return address + 208; }
  // the shape's +368
  float face_pz_distance() const { return guest::LoadFloat(base, address + 224); }
  void set_face_pz_distance(float v) const { guest::StoreFloat(base, address + 224, v); }
  uint32_t face_pz_distance_address() const { return address + 224; }
  // the shape's +384
  uint32_t face_pz_normal_address() const { return address + 240; }
  // the shape's +400
  float face_nx_distance() const { return guest::LoadFloat(base, address + 256); }
  void set_face_nx_distance(float v) const { guest::StoreFloat(base, address + 256, v); }
  uint32_t face_nx_distance_address() const { return address + 256; }
  // the shape's +416
  uint32_t face_nx_normal_address() const { return address + 272; }
  // the shape's +432
  float face_ny_distance() const { return guest::LoadFloat(base, address + 288); }
  void set_face_ny_distance(float v) const { guest::StoreFloat(base, address + 288, v); }
  uint32_t face_ny_distance_address() const { return address + 288; }
  // the shape's +448
  uint32_t face_ny_normal_address() const { return address + 304; }
  // the shape's +464
  float face_nz_distance() const { return guest::LoadFloat(base, address + 320); }
  void set_face_nz_distance(float v) const { guest::StoreFloat(base, address + 320, v); }
  uint32_t face_nz_distance_address() const { return address + 320; }
  // the shape's +480
  uint32_t face_nz_normal_address() const { return address + 336; }
  // the shape's +16
  float car_float_16() const { return guest::LoadFloat(base, address + 352); }
  void set_car_float_16(float v) const { guest::StoreFloat(base, address + 352, v); }
  uint32_t car_float_16_address() const { return address + 352; }
  // the shape's +20
  float car_float_20() const { return guest::LoadFloat(base, address + 356); }
  void set_car_float_20(float v) const { guest::StoreFloat(base, address + 356, v); }
  uint32_t car_float_20_address() const { return address + 356; }
  // the shape's +24
  float car_float_24() const { return guest::LoadFloat(base, address + 360); }
  void set_car_float_24(float v) const { guest::StoreFloat(base, address + 360, v); }
  uint32_t car_float_24_address() const { return address + 360; }
  // the shape's +28
  float friction() const { return guest::LoadFloat(base, address + 364); }
  void set_friction(float v) const { guest::StoreFloat(base, address + 364, v); }
  uint32_t friction_address() const { return address + 364; }
  // the shape's +32
  float radius() const { return guest::LoadFloat(base, address + 368); }
  void set_radius(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t radius_address() const { return address + 368; }
  // the shape's +48
  uint32_t center_address() const { return address + 384; }
  // the shape's +64
  uint32_t axis0_address() const { return address + 400; }
  // the shape's +80
  uint32_t axis1_address() const { return address + 416; }
  // the shape's +96
  uint32_t axis2_address() const { return address + 432; }
  // the shape's halfword +112, as a word
  uint32_t unknown_448() const { return guest::LoadU32(base, address + 448); }
  void set_unknown_448(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t unknown_448_address() const { return address + 448; }
};

// embedded; measure: Record5232::CopyTrack.r4@+1136, Record5232::CopyTrack.r4@+608, Part464::Copy.r4, Record5232::CopyTrack.r4@+4080, Record5232::CopyTrack.r4, Record5232::Copy.r4@+4080@+16
struct ReplayCopySource : View {
  using View::View;
  static constexpr uint32_t kSize = 2938;
  // a record part (which one depends on the copy)
  uint32_t part_address() const { return address + 0; }
};

// The image words docs/functions/replay-snapshots.md's code uses.
struct ReplaySnapshotsGlobals {
  // the input recordings (a pointer): +0x107AC0 and +0x20F584 the two recordings' counts, +0x20F590 and +0x218DA0 snapshot blocks A and B, +0x2225B0 set to 1 by the restores
  static uint32_t input_recordings(uint8_t* base) { return guest::LoadU32(base, 0x826C5B4Cu); }
  static void set_input_recordings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B4Cu, v); }
  static constexpr uint32_t input_recordings_address() { return 0x826C5B4Cu; }
  // the kept replay (a pointer): Replay::Keep's r3
  static uint32_t kept_replay(uint8_t* base) { return guest::LoadU32(base, 0x826C5B50u); }
  static void set_kept_replay(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B50u, v); }
  static constexpr uint32_t kept_replay_address() { return 0x826C5B50u; }
  // the racers' manager (a pointer, race-world.md)
  static uint32_t racers_manager(uint8_t* base) { return guest::LoadU32(base, 0x826D82F4u); }
  static void set_racers_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
  // the race data (a pointer): +0x59AE8 the kept results
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // 0.0 (.rdata)
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
};

}  // namespace schema
