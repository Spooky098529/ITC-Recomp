// The views of docs/functions/ai-drivers.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/ai-drivers.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: AiDriver, AiDriver::Of.ret, [826C6074], sub_8213A200.r3, sub_82138FA8.r3, sub_82139A08.r3, sub_8213F868.ret, sub_8213F7E0.r3, sub_8213F868.r3, AiOther::Mirror.r4, AiPath::JoinCorners.r5.+0, AiPath::ChooseSideForOther.r5, AiPath::JoinCornerPair.r5.+0, AiDriver::Setup.r3
struct AiDriver : View {
  using View::View;
  static constexpr uint32_t kSize = 6160;
  // the racer's index (the driver's in the table)
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
  // measure fault: a route corner's +0 (AiPath::JoinCorners, KeepBranch, TrackRoute::LocateAt)
  uint16_t fault_corner_count() const { return guest::LoadU16(base, address + 0); }
  void set_fault_corner_count(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_corner_count_address() const { return address + 0; }
  // the route position (section, slice) as one word (-1 none)
  uint32_t route_position() const { return guest::LoadU32(base, address + 4); }
  void set_route_position(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t route_position_address() const { return address + 4; }
  uint16_t route_section() const { return guest::LoadU16(base, address + 4); }
  void set_route_section(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t route_section_address() const { return address + 4; }
  uint16_t route_slice() const { return guest::LoadU16(base, address + 6); }
  void set_route_slice(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t route_slice_address() const { return address + 6; }
  // the lane (AiDriver::UpdateLane; -1 none)
  uint16_t lane() const { return guest::LoadU16(base, address + 8); }
  void set_lane(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t lane_address() const { return address + 8; }
  // measure fault: a point's z or a record's +8 (AiOther::Mirror, AiPath::DropPassed, Extend, LeftEntry, TargetPoint)
  float fault_point_z() const { return guest::LoadFloat(base, address + 8); }
  void set_fault_point_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t fault_point_z_address() const { return address + 8; }
  // a speed: 73.0, or the skill's +4 (500.0 when cleared)
  float cruise_speed() const { return guest::LoadFloat(base, address + 12); }
  void set_cruise_speed(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t cruise_speed_address() const { return address + 12; }
  // bit 1 the vehicle's box +114, bit 2 set by AiDriver::StartRace, bit 5 stuck (reversing)
  uint32_t flags() const { return guest::LoadU32(base, address + 16); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t flags_address() const { return address + 16; }
  // measure fault: a car input record's +17 (AiDriver::DriveCar)
  uint8_t fault_input_kind() const { return guest::LoadU8(base, address + 17); }
  void set_fault_input_kind(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t fault_input_kind_address() const { return address + 17; }
  // measure fault: a corner record's +20 (AiDriver::LimitForCorner) or an other-racer record's +20 (AiOther::Mirror)
  float fault_corner_speed() const { return guest::LoadFloat(base, address + 20); }
  void set_fault_corner_speed(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t fault_corner_speed_address() const { return address + 20; }
  // measure fault: an other-racer record's +24 (AiOther::Mirror, AiPath::ChooseSideForOther)
  uint16_t fault_other_state() const { return guest::LoadU16(base, address + 24); }
  void set_fault_other_state(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t fault_other_state_address() const { return address + 24; }
  // measure fault: an other-racer record's +26 (AiPath::ChooseSideForOther)
  uint16_t fault_other_ring_index() const { return guest::LoadU16(base, address + 26); }
  void set_fault_other_ring_index(uint16_t v) const { guest::StoreU16(base, address + 26, v); }
  uint32_t fault_other_ring_index_address() const { return address + 26; }
  // measure fault: a corner's +28 (AiPath::JoinCorners)
  float fault_corner_slope() const { return guest::LoadFloat(base, address + 28); }
  void set_fault_corner_slope(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t fault_corner_slope_address() const { return address + 28; }
  // the 48 bytes AiDriver::ClearPlan sets to their defaults (+32 to +79)
  uint32_t plan_address() const { return address + 32; }
  // 0 to 6
  uint32_t state() const { return guest::LoadU32(base, address + 32); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t state_address() const { return address + 32; }
  // measure fault: an other-racer record's +32 (AiOther::Mirror)
  uint8_t fault_other_flag_a() const { return guest::LoadU8(base, address + 32); }
  void set_fault_other_flag_a(uint8_t v) const { guest::StoreU8(base, address + 32, v); }
  uint32_t fault_other_flag_a_address() const { return address + 32; }
  // measure fault: an other-racer record's +33 (AiOther::Mirror)
  uint8_t fault_other_flag_b() const { return guest::LoadU8(base, address + 33); }
  void set_fault_other_flag_b(uint8_t v) const { guest::StoreU8(base, address + 33, v); }
  uint32_t fault_other_flag_b_address() const { return address + 33; }
  uint32_t previous_state() const { return guest::LoadU32(base, address + 36); }
  void set_previous_state(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t previous_state_address() const { return address + 36; }
  // a mask of racers (the ones to chase or hold off)
  uint16_t racer_mask() const { return guest::LoadU16(base, address + 40); }
  void set_racer_mask(uint16_t v) const { guest::StoreU16(base, address + 40, v); }
  uint32_t racer_mask_address() const { return address + 40; }
  // -1 when cleared (written as a word at a halfword's alignment)
  uint32_t plan_word() const { return guest::LoadU32(base, address + 42); }
  void set_plan_word(uint32_t v) const { guest::StoreU32(base, address + 42, v); }
  uint32_t plan_word_address() const { return address + 42; }
  // four floats, the last 1.0 when cleared
  uint32_t plan_point_a_address() const { return address + 48; }
  float plan_a_x() const { return guest::LoadFloat(base, address + 48); }
  void set_plan_a_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t plan_a_x_address() const { return address + 48; }
  float plan_a_y() const { return guest::LoadFloat(base, address + 52); }
  void set_plan_a_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t plan_a_y_address() const { return address + 52; }
  // measure fault: a route corner's +52 (AiPath::CornerToApex, JoinCorners)
  uint16_t fault_corner_apex() const { return guest::LoadU16(base, address + 52); }
  void set_fault_corner_apex(uint16_t v) const { guest::StoreU16(base, address + 52, v); }
  uint32_t fault_corner_apex_address() const { return address + 52; }
  // measure fault: a route corner's +54 (AiPath::CornerToExit, JoinCorners)
  uint16_t fault_corner_exit() const { return guest::LoadU16(base, address + 54); }
  void set_fault_corner_exit(uint16_t v) const { guest::StoreU16(base, address + 54, v); }
  uint32_t fault_corner_exit_address() const { return address + 54; }
  float plan_a_z() const { return guest::LoadFloat(base, address + 56); }
  void set_plan_a_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t plan_a_z_address() const { return address + 56; }
  float plan_a_w() const { return guest::LoadFloat(base, address + 60); }
  void set_plan_a_w(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t plan_a_w_address() const { return address + 60; }
  // four floats, the last 1.0 when cleared
  float plan_b_x() const { return guest::LoadFloat(base, address + 64); }
  void set_plan_b_x(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t plan_b_x_address() const { return address + 64; }
  float plan_b_y() const { return guest::LoadFloat(base, address + 68); }
  void set_plan_b_y(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t plan_b_y_address() const { return address + 68; }
  float plan_b_z() const { return guest::LoadFloat(base, address + 72); }
  void set_plan_b_z(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t plan_b_z_address() const { return address + 72; }
  float plan_b_w() const { return guest::LoadFloat(base, address + 76); }
  void set_plan_b_w(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t plan_b_w_address() const { return address + 76; }
  // the car's point (its track position's), w 1.0
  float point_x() const { return guest::LoadFloat(base, address + 80); }
  void set_point_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t point_x_address() const { return address + 80; }
  // x and y copied as one doubleword (AiDriver::ReadVehicle)
  uint64_t point_xy() const { return guest::LoadU64(base, address + 80); }
  void set_point_xy(uint64_t v) const { guest::StoreU64(base, address + 80, v); }
  uint32_t point_xy_address() const { return address + 80; }
  float point_y() const { return guest::LoadFloat(base, address + 84); }
  void set_point_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t point_y_address() const { return address + 84; }
  float point_z() const { return guest::LoadFloat(base, address + 88); }
  void set_point_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t point_z_address() const { return address + 88; }
  // z and w copied as one doubleword (AiDriver::ReadVehicle)
  uint64_t point_zw() const { return guest::LoadU64(base, address + 88); }
  void set_point_zw(uint64_t v) const { guest::StoreU64(base, address + 88, v); }
  uint32_t point_zw_address() const { return address + 88; }
  float point_w() const { return guest::LoadFloat(base, address + 92); }
  void set_point_w(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t point_w_address() const { return address + 92; }
  // atan2 of the body's orientation x and z
  float heading() const { return guest::LoadFloat(base, address + 96); }
  void set_heading(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t heading_address() const { return address + 96; }
  float speed_kmh() const { return guest::LoadFloat(base, address + 100); }
  void set_speed_kmh(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t speed_kmh_address() const { return address + 100; }
  float yaw_rate() const { return guest::LoadFloat(base, address + 104); }
  void set_yaw_rate(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t yaw_rate_address() const { return address + 104; }
  // its steering limits (AiDriver::UpdateSteering scales by one or the other)
  float steer_limit_a() const { return guest::LoadFloat(base, address + 108); }
  void set_steer_limit_a(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t steer_limit_a_address() const { return address + 108; }
  float steer_limit_b() const { return guest::LoadFloat(base, address + 112); }
  void set_steer_limit_b(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t steer_limit_b_address() const { return address + 112; }
  // across the road: |cos(+148)| x +120 + |sin(+148)| x +124 (AiDriver::UpdateHalfWidth)
  float half_width() const { return guest::LoadFloat(base, address + 116); }
  void set_half_width(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t half_width_address() const { return address + 116; }
  // from the vehicle's bounds (AiDriver::ReadCar)
  float car_half_width() const { return guest::LoadFloat(base, address + 120); }
  void set_car_half_width(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t car_half_width_address() const { return address + 120; }
  float car_half_length() const { return guest::LoadFloat(base, address + 124); }
  void set_car_half_length(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t car_half_length_address() const { return address + 124; }
  // sqrt(half width^2 + half length^2)
  float car_radius() const { return guest::LoadFloat(base, address + 128); }
  void set_car_radius(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t car_radius_address() const { return address + 128; }
  // a handling factor from the vehicle's +3900 (AiDriver::CornerFactor)
  float corner_grip() const { return guest::LoadFloat(base, address + 132); }
  void set_corner_grip(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t corner_grip_address() const { return address + 132; }
  // AiDriver::SpeedOverDistance's factor
  float distance_factor() const { return guest::LoadFloat(base, address + 136); }
  void set_distance_factor(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t distance_factor_address() const { return address + 136; }
  // a handling factor from the vehicle's +3444, at most 1.0 (probably)
  float steer_factor() const { return guest::LoadFloat(base, address + 140); }
  void set_steer_factor(float v) const { guest::StoreFloat(base, address + 140, v); }
  uint32_t steer_factor_address() const { return address + 140; }
  // its offset across the road
  float road_offset() const { return guest::LoadFloat(base, address + 144); }
  void set_road_offset(float v) const { guest::StoreFloat(base, address + 144, v); }
  uint32_t road_offset_address() const { return address + 144; }
  // its angle to the road (the slice's direction less the heading)
  float road_angle() const { return guest::LoadFloat(base, address + 148); }
  void set_road_angle(float v) const { guest::StoreFloat(base, address + 148, v); }
  uint32_t road_angle_address() const { return address + 148; }
  // written by AiDriver::SteerToTarget; AiPath::SetRanges' shift (x 0.2)
  float steer_shift() const { return guest::LoadFloat(base, address + 152); }
  void set_steer_shift(float v) const { guest::StoreFloat(base, address + 152, v); }
  uint32_t steer_shift_address() const { return address + 152; }
  // its offset along its slice
  float slice_offset() const { return guest::LoadFloat(base, address + 156); }
  void set_slice_offset(float v) const { guest::StoreFloat(base, address + 156, v); }
  uint32_t slice_offset_address() const { return address + 156; }
  // its vehicle (Vehicle::Get)
  template <typename T = Vehicle>
  T vehicle() const { return T(base, guest::LoadU32(base, address + 160)); }
  void set_vehicle(const View& v) const { guest::StoreU32(base, address + 160, v.address); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t vehicle_address() const { return address + 160; }
  // 500.0 none
  float speed_limit() const { return guest::LoadFloat(base, address + 164); }
  void set_speed_limit(float v) const { guest::StoreFloat(base, address + 164, v); }
  uint32_t speed_limit_address() const { return address + 164; }
  // 500.0 none (lowered for a racer ahead, AiDriver::AdjustForOther)
  float speed_limit_other() const { return guest::LoadFloat(base, address + 168); }
  void set_speed_limit_other(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t speed_limit_other_address() const { return address + 168; }
  // seven AiOther, one per other racer by its index
  uint32_t others_address() const { return address + 172; }
  // seven words: the racers to react to (-1 ends)
  uint32_t reacting_address() const { return address + 452; }
  // the racer watched (-1 none)
  uint32_t watched() const { return guest::LoadU32(base, address + 480); }
  void set_watched(uint32_t v) const { guest::StoreU32(base, address + 480, v); }
  uint32_t watched_address() const { return address + 480; }
  // passes counted (AiDriver::UpdateAgainstPlayer)
  uint16_t passes_a() const { return guest::LoadU16(base, address + 484); }
  void set_passes_a(uint16_t v) const { guest::StoreU16(base, address + 484, v); }
  uint32_t passes_a_address() const { return address + 484; }
  uint16_t passes_b() const { return guest::LoadU16(base, address + 486); }
  void set_passes_b(uint16_t v) const { guest::StoreU16(base, address + 486, v); }
  uint32_t passes_b_address() const { return address + 486; }
  // the gap to the player's car
  float player_gap() const { return guest::LoadFloat(base, address + 488); }
  void set_player_gap(float v) const { guest::StoreFloat(base, address + 488, v); }
  uint32_t player_gap_address() const { return address + 488; }
  uint16_t stuck_count() const { return guest::LoadU16(base, address + 492); }
  void set_stuck_count(uint16_t v) const { guest::StoreU16(base, address + 492, v); }
  uint32_t stuck_count_address() const { return address + 492; }
  // 1 when the look-ahead is inside a corner
  uint32_t in_corner() const { return guest::LoadU32(base, address + 496); }
  void set_in_corner(uint32_t v) const { guest::StoreU32(base, address + 496, v); }
  uint32_t in_corner_address() const { return address + 496; }
  // the AI profile, or 0
  template <typename T = AiProfile>
  T profile() const { return T(base, guest::LoadU32(base, address + 500)); }
  void set_profile(const View& v) const { guest::StoreU32(base, address + 500, v.address); }
  void set_profile(uint32_t v) const { guest::StoreU32(base, address + 500, v); }
  uint32_t profile_address() const { return address + 500; }
  // its AiPath
  uint32_t path_address() const { return address + 512; }
  // the path's first ring entry's +12
  float path_entry0_w() const { return guest::LoadFloat(base, address + 540); }
  void set_path_entry0_w(float v) const { guest::StoreFloat(base, address + 540, v); }
  uint32_t path_entry0_w_address() const { return address + 540; }
  uint16_t path_entry0_section() const { return guest::LoadU16(base, address + 544); }
  void set_path_entry0_section(uint16_t v) const { guest::StoreU16(base, address + 544, v); }
  uint32_t path_entry0_section_address() const { return address + 544; }
  uint16_t path_entry0_slice() const { return guest::LoadU16(base, address + 546); }
  void set_path_entry0_slice(uint16_t v) const { guest::StoreU16(base, address + 546, v); }
  uint32_t path_entry0_slice_address() const { return address + 546; }
  // the path's +5136
  uint16_t path_entry_count() const { return guest::LoadU16(base, address + 5648); }
  void set_path_entry_count(uint16_t v) const { guest::StoreU16(base, address + 5648, v); }
  uint32_t path_entry_count_address() const { return address + 5648; }
  // the path's +5138
  uint16_t path_first() const { return guest::LoadU16(base, address + 5650); }
  void set_path_first(uint16_t v) const { guest::StoreU16(base, address + 5650, v); }
  uint32_t path_first_address() const { return address + 5650; }
  // the path's +5142
  uint16_t path_look_ahead() const { return guest::LoadU16(base, address + 5654); }
  void set_path_look_ahead(uint16_t v) const { guest::StoreU16(base, address + 5654, v); }
  uint32_t path_look_ahead_address() const { return address + 5654; }
  // the path's +5162
  uint16_t path_corner_count() const { return guest::LoadU16(base, address + 5674); }
  void set_path_corner_count(uint16_t v) const { guest::StoreU16(base, address + 5674, v); }
  uint32_t path_corner_count_address() const { return address + 5674; }
  // the path's first corner record's +0
  template <typename T = RouteCorner>
  T path_corner0_data() const { return T(base, guest::LoadU32(base, address + 5676)); }
  void set_path_corner0_data(const View& v) const { guest::StoreU32(base, address + 5676, v.address); }
  void set_path_corner0_data(uint32_t v) const { guest::StoreU32(base, address + 5676, v); }
  uint32_t path_corner0_data_address() const { return address + 5676; }
  // the path's second corner record's +0
  template <typename T = RouteCorner>
  T path_corner1_data() const { return T(base, guest::LoadU32(base, address + 5716)); }
  void set_path_corner1_data(const View& v) const { guest::StoreU32(base, address + 5716, v.address); }
  void set_path_corner1_data(uint32_t v) const { guest::StoreU32(base, address + 5716, v); }
  uint32_t path_corner1_data_address() const { return address + 5716; }
  // the path's +5520
  uint32_t path_mode() const { return guest::LoadU32(base, address + 6032); }
  void set_path_mode(uint32_t v) const { guest::StoreU32(base, address + 6032, v); }
  uint32_t path_mode_address() const { return address + 6032; }
  // the path's +5524
  uint32_t path_branch_kind() const { return guest::LoadU32(base, address + 6036); }
  void set_path_branch_kind(uint32_t v) const { guest::StoreU32(base, address + 6036, v); }
  uint32_t path_branch_kind_address() const { return address + 6036; }
  // the path's +5528
  uint32_t path_leader_branch_kind() const { return guest::LoadU32(base, address + 6040); }
  void set_path_leader_branch_kind(uint32_t v) const { guest::StoreU32(base, address + 6040, v); }
  uint32_t path_leader_branch_kind_address() const { return address + 6040; }
  // its AiSkill
  uint32_t skill_address() const { return address + 6048; }
  // the skill's +4
  uint16_t skill_speed() const { return guest::LoadU16(base, address + 6052); }
  void set_skill_speed(uint16_t v) const { guest::StoreU16(base, address + 6052, v); }
  uint32_t skill_speed_address() const { return address + 6052; }
  // the skill's +6
  uint16_t skill_throttle_cap() const { return guest::LoadU16(base, address + 6054); }
  void set_skill_throttle_cap(uint16_t v) const { guest::StoreU16(base, address + 6054, v); }
  uint32_t skill_throttle_cap_address() const { return address + 6054; }
  // the skill's +8
  uint16_t skill_corner() const { return guest::LoadU16(base, address + 6056); }
  void set_skill_corner(uint16_t v) const { guest::StoreU16(base, address + 6056, v); }
  uint32_t skill_corner_address() const { return address + 6056; }
  // the skill's +10
  uint16_t skill_speed_scale() const { return guest::LoadU16(base, address + 6058); }
  void set_skill_speed_scale(uint16_t v) const { guest::StoreU16(base, address + 6058, v); }
  uint32_t skill_speed_scale_address() const { return address + 6058; }
  // the skill's +12
  uint16_t skill_caps_target() const { return guest::LoadU16(base, address + 6060); }
  void set_skill_caps_target(uint16_t v) const { guest::StoreU16(base, address + 6060, v); }
  uint32_t skill_caps_target_address() const { return address + 6060; }
  // the skill's +20
  float skill_percentage() const { return guest::LoadFloat(base, address + 6068); }
  void set_skill_percentage(float v) const { guest::StoreFloat(base, address + 6068, v); }
  uint32_t skill_percentage_address() const { return address + 6068; }
  // the word AiDriver::SetSkill is given (0 in ResetToMode0 and 1)
  uint32_t skill_flag() const { return guest::LoadU32(base, address + 6084); }
  void set_skill_flag(uint32_t v) const { guest::StoreU32(base, address + 6084, v); }
  uint32_t skill_flag_address() const { return address + 6084; }
  float target_speed() const { return guest::LoadFloat(base, address + 6088); }
  void set_target_speed(float v) const { guest::StoreFloat(base, address + 6088, v); }
  uint32_t target_speed_address() const { return address + 6088; }
  // the target point (w 1.0)
  float target_x() const { return guest::LoadFloat(base, address + 6096); }
  void set_target_x(float v) const { guest::StoreFloat(base, address + 6096, v); }
  uint32_t target_x_address() const { return address + 6096; }
  float target_y() const { return guest::LoadFloat(base, address + 6100); }
  void set_target_y(float v) const { guest::StoreFloat(base, address + 6100, v); }
  uint32_t target_y_address() const { return address + 6100; }
  float target_z() const { return guest::LoadFloat(base, address + 6104); }
  void set_target_z(float v) const { guest::StoreFloat(base, address + 6104, v); }
  uint32_t target_z_address() const { return address + 6104; }
  float target_w() const { return guest::LoadFloat(base, address + 6108); }
  void set_target_w(float v) const { guest::StoreFloat(base, address + 6108, v); }
  uint32_t target_w_address() const { return address + 6108; }
  // an angle to the target (AiDriver::EaseThrottleForSlide's)
  float slide_angle() const { return guest::LoadFloat(base, address + 6112); }
  void set_slide_angle(float v) const { guest::StoreFloat(base, address + 6112, v); }
  uint32_t slide_angle_address() const { return address + 6112; }
  // the angle to the target
  float target_angle() const { return guest::LoadFloat(base, address + 6116); }
  void set_target_angle(float v) const { guest::StoreFloat(base, address + 6116, v); }
  uint32_t target_angle_address() const { return address + 6116; }
  // -1.0 to 1.0
  float steering() const { return guest::LoadFloat(base, address + 6120); }
  void set_steering(float v) const { guest::StoreFloat(base, address + 6120, v); }
  uint32_t steering_address() const { return address + 6120; }
  // 0.0 to 1.0
  float throttle() const { return guest::LoadFloat(base, address + 6124); }
  void set_throttle(float v) const { guest::StoreFloat(base, address + 6124, v); }
  uint32_t throttle_address() const { return address + 6124; }
  // 0.0 to 1.0
  float brake() const { return guest::LoadFloat(base, address + 6128); }
  void set_brake(float v) const { guest::StoreFloat(base, address + 6128, v); }
  uint32_t brake_address() const { return address + 6128; }
  // probably
  uint8_t handbrake() const { return guest::LoadU8(base, address + 6132); }
  void set_handbrake(uint8_t v) const { guest::StoreU8(base, address + 6132, v); }
  uint32_t handbrake_address() const { return address + 6132; }
  float steer_request() const { return guest::LoadFloat(base, address + 6136); }
  void set_steer_request(float v) const { guest::StoreFloat(base, address + 6136, v); }
  uint32_t steer_request_address() const { return address + 6136; }
  float throttle_request() const { return guest::LoadFloat(base, address + 6140); }
  void set_throttle_request(float v) const { guest::StoreFloat(base, address + 6140, v); }
  uint32_t throttle_request_address() const { return address + 6140; }
  float brake_request() const { return guest::LoadFloat(base, address + 6144); }
  void set_brake_request(float v) const { guest::StoreFloat(base, address + 6144, v); }
  uint32_t brake_request_address() const { return address + 6144; }
  // AiDriver::UpdateStarting (0x821398C0), then AiDriver::UpdateTrack (0x8213A1B8)
  uint32_t update() const { return guest::LoadU32(base, address + 6148); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 6148, v); }
  uint32_t update_address() const { return address + 6148; }
};

// embedded; measure: AiPath, AiPath::SpreadOffset.ret, AiPath::LeftEntry.r5, AiPath::JoinCorners.r5, AiPath::JoinCornerPair.r5, AiPathEntry::NarrowToOtherEx.r4, AiDriver.+500, AiPathEntry::NarrowToOther.r6, AiPath::NarrowBehind.r4, AiPath::AddCornersAt.r4
struct AiPath : View {
  using View::View;
  static constexpr uint32_t kSize = 5536;
  template <typename T = AiDriver>
  T driver() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_driver(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_driver(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t driver_address() const { return address + 0; }
  // measure fault: a corner record's +4 (AiPath::ApproachCorner, JoinCornerPair, JoinCorners)
  template <typename T = RouteCornerEntry>
  T fault_corner_table() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_fault_corner_table(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_fault_corner_table(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t fault_corner_table_address() const { return address + 4; }
  // measure fault: a corner record's +10 (the corner steps, AiPath::JoinCornerPair)
  uint16_t fault_corner_offset() const { return guest::LoadU16(base, address + 10); }
  void set_fault_corner_offset(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t fault_corner_offset_address() const { return address + 10; }
  // measure fault: a corner record's +14 (AiPath::JoinCorners)
  uint16_t fault_corner_position() const { return guest::LoadU16(base, address + 14); }
  void set_fault_corner_position(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t fault_corner_position_address() const { return address + 14; }
  // the ring: 80 AiPathEntry of 64 bytes (the corner steps' records are also filed here: a measure fault)
  uint32_t entries_address() const { return address + 16; }
  uint16_t entry_count() const { return guest::LoadU16(base, address + 5136); }
  void set_entry_count(uint16_t v) const { guest::StoreU16(base, address + 5136, v); }
  uint32_t entry_count_address() const { return address + 5136; }
  // the first entry's index
  uint16_t first() const { return guest::LoadU16(base, address + 5138); }
  void set_first(uint16_t v) const { guest::StoreU16(base, address + 5138, v); }
  uint32_t first_address() const { return address + 5138; }
  // how many entries to plan (the look-ahead's end)
  uint16_t plan_count() const { return guest::LoadU16(base, address + 5140); }
  void set_plan_count(uint16_t v) const { guest::StoreU16(base, address + 5140, v); }
  uint32_t plan_count_address() const { return address + 5140; }
  // the look-ahead (entries)
  uint16_t look_ahead() const { return guest::LoadU16(base, address + 5142); }
  void set_look_ahead(uint16_t v) const { guest::StoreU16(base, address + 5142, v); }
  uint32_t look_ahead_address() const { return address + 5142; }
  uint16_t section_count() const { return guest::LoadU16(base, address + 5144); }
  void set_section_count(uint16_t v) const { guest::StoreU16(base, address + 5144, v); }
  uint32_t section_count_address() const { return address + 5144; }
  // eight halfwords: the sections the path runs through (-1 none)
  uint32_t sections_address() const { return address + 5146; }
  uint16_t corner_count() const { return guest::LoadU16(base, address + 5162); }
  void set_corner_count(uint16_t v) const { guest::StoreU16(base, address + 5162, v); }
  uint32_t corner_count_address() const { return address + 5162; }
  // eight AiCorner of 44 bytes
  uint32_t corners_address() const { return address + 5164; }
  // the largest step between entries
  float largest_step() const { return guest::LoadFloat(base, address + 5516); }
  void set_largest_step(float v) const { guest::StoreFloat(base, address + 5516, v); }
  uint32_t largest_step_address() const { return address + 5516; }
  // 1 by lane, 0 by branch
  uint32_t mode() const { return guest::LoadU32(base, address + 5520); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 5520, v); }
  uint32_t mode_address() const { return address + 5520; }
  // -1 none
  uint32_t branch_kind() const { return guest::LoadU32(base, address + 5524); }
  void set_branch_kind(uint32_t v) const { guest::StoreU32(base, address + 5524, v); }
  uint32_t branch_kind_address() const { return address + 5524; }
  // -1 none (AiPath::FollowLeaderBranch)
  uint32_t leader_branch_kind() const { return guest::LoadU32(base, address + 5528); }
  void set_leader_branch_kind(uint32_t v) const { guest::StoreU32(base, address + 5528, v); }
  uint32_t leader_branch_kind_address() const { return address + 5528; }
  // AiPath::NormalizeByLane (0x8213B428) or NormalizeByBranch (0x8213C908)
  uint32_t normalizer() const { return guest::LoadU32(base, address + 5532); }
  void set_normalizer(uint32_t v) const { guest::StoreU32(base, address + 5532, v); }
  uint32_t normalizer_address() const { return address + 5532; }
};

// embedded; measure: AiPathEntry, AiPath::NormalizeByLane.r4, AiPath::NormalizeByBranch.r4, AiPathEntry::NarrowToOther.r4, AiPath::ClampOffset.r4, AiPath::ContinueLine.r4, AiPath::ApproachCorner.r6.+0, AiPath::CornerToEntry.r6.+0, AiPath::CornerToApex.r6.+0, AiPath::CornerToExit.r6.+0, AiDriver::LimitByCurve.r7
struct AiPathEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the point on the line
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // 1.0
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
  // the route position as one word (-1 none)
  uint32_t route_position() const { return guest::LoadU32(base, address + 16); }
  void set_route_position(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t route_position_address() const { return address + 16; }
  uint16_t section() const { return guest::LoadU16(base, address + 16); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t section_address() const { return address + 16; }
  uint16_t slice() const { return guest::LoadU16(base, address + 18); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 18, v); }
  uint32_t slice_address() const { return address + 18; }
  uint32_t lane() const { return guest::LoadU32(base, address + 20); }
  void set_lane(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t lane_address() const { return address + 20; }
  // the line's offset across the road
  float offset() const { return guest::LoadFloat(base, address + 24); }
  void set_offset(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t offset_address() const { return address + 24; }
  // the shift from the entry before
  float step() const { return guest::LoadFloat(base, address + 28); }
  void set_step(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t step_address() const { return address + 28; }
  // the road's range
  float range_low() const { return guest::LoadFloat(base, address + 32); }
  void set_range_low(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t range_low_address() const { return address + 32; }
  float range_high() const { return guest::LoadFloat(base, address + 36); }
  void set_range_high(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t range_high_address() const { return address + 36; }
  // the range left (narrowed by the others)
  float room_low() const { return guest::LoadFloat(base, address + 40); }
  void set_room_low(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t room_low_address() const { return address + 40; }
  float room_high() const { return guest::LoadFloat(base, address + 44); }
  void set_room_high(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t room_high_address() const { return address + 44; }
  float target_offset() const { return guest::LoadFloat(base, address + 48); }
  void set_target_offset(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t target_offset_address() const { return address + 48; }
  template <typename T = RouteSlice>
  T slice_record() const { return T(base, guest::LoadU32(base, address + 52)); }
  void set_slice_record(const View& v) const { guest::StoreU32(base, address + 52, v.address); }
  void set_slice_record(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t slice_record_address() const { return address + 52; }
  float unknown_56() const { return guest::LoadFloat(base, address + 56); }
  void set_unknown_56(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  float unknown_60() const { return guest::LoadFloat(base, address + 60); }
  void set_unknown_60(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
};

// embedded; measure: AiCorner, sub_8213A200.r4, sub_8213AB40.r4, sub_8213BDF0.r4, sub_8213BDF0.r5, sub_8213BE80.r4, sub_8213BE80.r5, AiPath::JoinCornersShort.r4, AiPath::JoinCornersShort.r5
struct AiCorner : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // the corner (the route file's +16 table)
  template <typename T = RouteCorner>
  T data() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_data(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t data_address() const { return address + 0; }
  // its table (the corner's +56)
  template <typename T = RouteCornerEntry>
  T table() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_table(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_table(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t table_address() const { return address + 4; }
  // the corner's number
  uint16_t number() const { return guest::LoadU16(base, address + 8); }
  void set_number(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t number_address() const { return address + 8; }
  // its offset in entries
  uint16_t entry_offset() const { return guest::LoadU16(base, address + 10); }
  void set_entry_offset(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t entry_offset_address() const { return address + 10; }
  uint16_t end() const { return guest::LoadU16(base, address + 12); }
  void set_end(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
  // a position in its table
  uint16_t position() const { return guest::LoadU16(base, address + 14); }
  void set_position(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t position_address() const { return address + 14; }
  // 1.0 or -1.0
  float direction() const { return guest::LoadFloat(base, address + 16); }
  void set_direction(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t direction_address() const { return address + 16; }
  // from the driver's +144 (AiDriver::LimitForCorner)
  float speed() const { return guest::LoadFloat(base, address + 20); }
  void set_speed(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t speed_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // joining it to the corner before
  float slope_before() const { return guest::LoadFloat(base, address + 28); }
  void set_slope_before(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t slope_before_address() const { return address + 28; }
  // joining it to the corner after
  float slope_after() const { return guest::LoadFloat(base, address + 32); }
  void set_slope_after(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t slope_after_address() const { return address + 32; }
  uint8_t joined_before() const { return guest::LoadU8(base, address + 36); }
  void set_joined_before(uint8_t v) const { guest::StoreU8(base, address + 36, v); }
  uint32_t joined_before_address() const { return address + 36; }
  uint8_t joined_after() const { return guest::LoadU8(base, address + 37); }
  void set_joined_after(uint8_t v) const { guest::StoreU8(base, address + 37, v); }
  uint32_t joined_after_address() const { return address + 37; }
  // 1 to 3: how the driver steers
  uint32_t kind() const { return guest::LoadU32(base, address + 40); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t kind_address() const { return address + 40; }
};

// embedded; measure: AiOther, AiDriver::AdjustForOther.r4
struct AiOther : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // along the route (200.0 far)
  float gap() const { return guest::LoadFloat(base, address + 0); }
  void set_gap(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t gap_address() const { return address + 0; }
  float closing_rate() const { return guest::LoadFloat(base, address + 4); }
  void set_closing_rate(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t closing_rate_address() const { return address + 4; }
  float last_change() const { return guest::LoadFloat(base, address + 8); }
  void set_last_change(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t last_change_address() const { return address + 8; }
  float factor() const { return guest::LoadFloat(base, address + 12); }
  void set_factor(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t factor_address() const { return address + 12; }
  // the other's speed
  float speed() const { return guest::LoadFloat(base, address + 16); }
  void set_speed(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t speed_address() const { return address + 16; }
  // -1.0 none
  float ticks_to_meet() const { return guest::LoadFloat(base, address + 20); }
  void set_ticks_to_meet(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t ticks_to_meet_address() const { return address + 20; }
  // 0 far, 1 behind, 2 beside, 3 ahead (guesses)
  uint16_t state() const { return guest::LoadU16(base, address + 24); }
  void set_state(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t state_address() const { return address + 24; }
  uint16_t ring_index() const { return guest::LoadU16(base, address + 26); }
  void set_ring_index(uint16_t v) const { guest::StoreU16(base, address + 26, v); }
  uint32_t ring_index_address() const { return address + 26; }
  // what to do
  uint16_t action_flags() const { return guest::LoadU16(base, address + 28); }
  void set_action_flags(uint16_t v) const { guest::StoreU16(base, address + 28, v); }
  uint32_t action_flags_address() const { return address + 28; }
  uint16_t timer() const { return guest::LoadU16(base, address + 30); }
  void set_timer(uint16_t v) const { guest::StoreU16(base, address + 30, v); }
  uint32_t timer_address() const { return address + 30; }
  uint8_t flag_a() const { return guest::LoadU8(base, address + 32); }
  void set_flag_a(uint8_t v) const { guest::StoreU8(base, address + 32, v); }
  uint32_t flag_a_address() const { return address + 32; }
  uint8_t flag_b() const { return guest::LoadU8(base, address + 33); }
  void set_flag_b(uint8_t v) const { guest::StoreU8(base, address + 33, v); }
  uint32_t flag_b_address() const { return address + 33; }
  // 0x8 was ahead, 0x10 was behind, 0x2 and 0x4 a change between them
  uint16_t history_flags() const { return guest::LoadU16(base, address + 34); }
  void set_history_flags(uint16_t v) const { guest::StoreU16(base, address + 34, v); }
  uint32_t history_flags_address() const { return address + 34; }
  uint16_t unknown_36() const { return guest::LoadU16(base, address + 36); }
  void set_unknown_36(uint16_t v) const { guest::StoreU16(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
};

// embedded; measure: AiSkill
struct AiSkill : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // the profile's +120, or 0
  template <typename T = AiSkillSource>
  T source() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_source(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_source(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t source_address() const { return address + 0; }
  // seven halfwords (defaults 500, 80, -100, 0)
  uint16_t speed() const { return guest::LoadU16(base, address + 4); }
  void set_speed(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t speed_address() const { return address + 4; }
  // percent
  uint16_t throttle_cap() const { return guest::LoadU16(base, address + 6); }
  void set_throttle_cap(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t throttle_cap_address() const { return address + 6; }
  // passed to the corners
  uint16_t corner() const { return guest::LoadU16(base, address + 8); }
  void set_corner(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t corner_address() const { return address + 8; }
  // percent
  uint16_t speed_scale() const { return guest::LoadU16(base, address + 10); }
  void set_speed_scale(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t speed_scale_address() const { return address + 10; }
  // whether the target speed is capped
  uint16_t caps_target() const { return guest::LoadU16(base, address + 12); }
  void set_caps_target(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t caps_target_address() const { return address + 12; }
  // a percentage AiOther::PlanReaction reads
  uint16_t reaction_a() const { return guest::LoadU16(base, address + 14); }
  void set_reaction_a(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t reaction_a_address() const { return address + 14; }
  // a percentage AiOther::PlanReaction reads
  uint16_t reaction_b() const { return guest::LoadU16(base, address + 16); }
  void set_reaction_b(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t reaction_b_address() const { return address + 16; }
  // 100.0; x 0.01 into the vehicle's +2432
  float percentage() const { return guest::LoadFloat(base, address + 20); }
  void set_percentage(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t percentage_address() const { return address + 20; }
  // three trigger states of 4 bytes: halfword a timer, byte active
  uint32_t triggers_address() const { return address + 24; }
  uint16_t trigger0_timer() const { return guest::LoadU16(base, address + 24); }
  void set_trigger0_timer(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t trigger0_timer_address() const { return address + 24; }
  uint8_t trigger0_active() const { return guest::LoadU8(base, address + 26); }
  void set_trigger0_active(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t trigger0_active_address() const { return address + 26; }
  // 1 runs the triggers
  uint32_t runs_triggers() const { return guest::LoadU32(base, address + 36); }
  void set_runs_triggers(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t runs_triggers_address() const { return address + 36; }
};

// file; measure: AiDriver@+6048.+0
struct AiSkillSource : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // the skill's +4 to +16 copied from here
  uint16_t speed() const { return guest::LoadU16(base, address + 0); }
  void set_speed(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t speed_address() const { return address + 0; }
  uint16_t throttle_cap() const { return guest::LoadU16(base, address + 2); }
  void set_throttle_cap(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t throttle_cap_address() const { return address + 2; }
  uint16_t corner() const { return guest::LoadU16(base, address + 4); }
  void set_corner(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t corner_address() const { return address + 4; }
  uint16_t speed_scale() const { return guest::LoadU16(base, address + 6); }
  void set_speed_scale(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t speed_scale_address() const { return address + 6; }
  uint16_t caps_target() const { return guest::LoadU16(base, address + 8); }
  void set_caps_target(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t caps_target_address() const { return address + 8; }
  uint16_t reaction_a() const { return guest::LoadU16(base, address + 10); }
  void set_reaction_a(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t reaction_a_address() const { return address + 10; }
  uint16_t reaction_b() const { return guest::LoadU16(base, address + 12); }
  void set_reaction_b(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t reaction_b_address() const { return address + 12; }
  // three triggers of 8 bytes: a kind, a halfword value
  uint32_t triggers_address() const { return address + 16; }
  uint32_t trigger0_kind() const { return guest::LoadU32(base, address + 16); }
  void set_trigger0_kind(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t trigger0_kind_address() const { return address + 16; }
  uint16_t trigger0_value() const { return guest::LoadU16(base, address + 20); }
  void set_trigger0_value(uint16_t v) const { guest::StoreU16(base, address + 20, v); }
  uint32_t trigger0_value_address() const { return address + 20; }
};

// file; measure: AiSkill::TriggerEnds.r5
struct AiSkillTrigger : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // signed
  uint16_t value() const { return guest::LoadU16(base, address + 4); }
  void set_value(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t value_address() const { return address + 4; }
};

// file; measure: sub_8213F7E0.r3.+500, sub_8213F868.r3.+500
struct AiProfile : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  // matched against the race state's records' word +8 (AiSkill::EntryWord)
  uint16_t key() const { return guest::LoadU16(base, address + 0); }
  void set_key(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t key_address() const { return address + 0; }
  // signed: AiPath::BranchMasksFor's, and AiPath::PreferredSections' a (4 to 9)
  uint8_t branch_byte() const { return guest::LoadU8(base, address + 36); }
  void set_branch_byte(uint8_t v) const { guest::StoreU8(base, address + 36, v); }
  uint32_t branch_byte_address() const { return address + 36; }
  // signed: 4 or 5 picks a preferred sections list (AiPath::CompareBranches, NormalizeByBranch)
  uint16_t branch_kind() const { return guest::LoadU16(base, address + 88); }
  void set_branch_kind(uint16_t v) const { guest::StoreU16(base, address + 88, v); }
  uint32_t branch_kind_address() const { return address + 88; }
  // signed: AiPath::PreferredSections' b
  uint8_t preferred_byte() const { return guest::LoadU8(base, address + 90); }
  void set_preferred_byte(uint8_t v) const { guest::StoreU8(base, address + 90, v); }
  uint32_t preferred_byte_address() const { return address + 90; }
  template <typename T = AiSkillSource>
  T skill_source() const { return T(base, guest::LoadU32(base, address + 120)); }
  void set_skill_source(const View& v) const { guest::StoreU32(base, address + 120, v.address); }
  void set_skill_source(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t skill_source_address() const { return address + 120; }
};

// embedded; measure: CarInput, AiDriver::Drive.r4, AiDriver::DriveCar.ret, Vehicle::ApplyInput.r4, new<20>@CarInputs::Create
struct CarInput : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // 0x200 set by a stuck AI driver (probably reverse); the pad's in known-data.md
  uint32_t flags() const { return guest::LoadU32(base, address + 0); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t flags_address() const { return address + 0; }
  // signed, out of 32,767
  uint16_t throttle() const { return guest::LoadU16(base, address + 4); }
  void set_throttle(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t throttle_address() const { return address + 4; }
  // signed, out of 32,767
  uint16_t brake() const { return guest::LoadU16(base, address + 6); }
  void set_brake(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t brake_address() const { return address + 6; }
  // signed, out of 32,767
  uint16_t steering() const { return guest::LoadU16(base, address + 8); }
  void set_steering(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t steering_address() const { return address + 8; }
  // 32,767 for the handbrake, probably
  uint16_t handbrake() const { return guest::LoadU16(base, address + 10); }
  void set_handbrake(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t handbrake_address() const { return address + 10; }
  // signed: probably the camera (from the pad)
  uint16_t camera() const { return guest::LoadU16(base, address + 12); }
  void set_camera(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t camera_address() const { return address + 12; }
  // from the pad: the player's settings byte +1; signed: the throttle's curve (Vehicle::ApplyInput)
  uint8_t throttle_rate() const { return guest::LoadU8(base, address + 14); }
  void set_throttle_rate(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t throttle_rate_address() const { return address + 14; }
  // the settings byte +2; signed: the brake's curve
  uint8_t brake_rate() const { return guest::LoadU8(base, address + 15); }
  void set_brake_rate(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t brake_rate_address() const { return address + 15; }
  // the settings byte +0; signed: the pad's or wheel's steering rate
  uint8_t steering_rate() const { return guest::LoadU8(base, address + 16); }
  void set_steering_rate(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t steering_rate_address() const { return address + 16; }
  // 0 the pad, 1 a wheel, 2 an AI driver
  uint8_t kind() const { return guest::LoadU8(base, address + 17); }
  void set_kind(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t kind_address() const { return address + 17; }
  // the player's three settings bytes (from the pad) (from CarInputRecord, car-sounds)
  uint32_t settings_address() const { return address + 14; }
};

// file; measure: AiCorner.+0, AiPath.+5164, AiCorner::Init.r4, sub_8213AB40.r4.+0, sub_8213BDF0.r4.+0, sub_8213BDF0.r5.+0, sub_8213BE80.r4.+0, sub_8213BE80.r5.+0, AiPath::JoinCornersShort.r4.+0, AiPath::JoinCornersShort.r5.+0, AiPath::SharesSections.r4, AiDriver::LimitSpeedBlend.r5, AiDriver@+5676.+0@+8
struct RouteCorner : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the count of its table's entries
  uint16_t entry_count() const { return guest::LoadU16(base, address + 0); }
  void set_entry_count(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t entry_count_address() const { return address + 0; }
  // measure fault: AiDriver::LimitSpeedBlend's pair (filed as the driver's +0's)
  float fault_word_0() const { return guest::LoadFloat(base, address + 0); }
  void set_fault_word_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t fault_word_0_address() const { return address + 0; }
  // the count of its sections (+36)
  uint16_t section_count() const { return guest::LoadU16(base, address + 2); }
  void set_section_count(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t section_count_address() const { return address + 2; }
  // its sign gives the direction
  float direction() const { return guest::LoadFloat(base, address + 4); }
  void set_direction(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t direction_address() const { return address + 4; }
  // +4 read as a word (AiCorner::Init)
  uint32_t direction_bits() const { return guest::LoadU32(base, address + 4); }
  void set_direction_bits(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t direction_bits_address() const { return address + 4; }
  // +8 to +24: speeds at its points, probably
  float speed_a() const { return guest::LoadFloat(base, address + 8); }
  void set_speed_a(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t speed_a_address() const { return address + 8; }
  float speed_b() const { return guest::LoadFloat(base, address + 12); }
  void set_speed_b(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t speed_b_address() const { return address + 12; }
  float speed_c() const { return guest::LoadFloat(base, address + 16); }
  void set_speed_c(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t speed_c_address() const { return address + 16; }
  float speed_d() const { return guest::LoadFloat(base, address + 20); }
  void set_speed_d(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t speed_d_address() const { return address + 20; }
  float speed_e() const { return guest::LoadFloat(base, address + 24); }
  void set_speed_e(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t speed_e_address() const { return address + 24; }
  // compared with 100.0 when corners overlap (probably)
  float length() const { return guest::LoadFloat(base, address + 28); }
  void set_length(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t length_address() const { return address + 28; }
  // halfwords: the sections it runs through
  uint32_t sections_address() const { return address + 36; }
  // slices from its start: where it begins
  uint16_t start() const { return guest::LoadU16(base, address + 48); }
  void set_start(uint16_t v) const { guest::StoreU16(base, address + 48, v); }
  uint32_t start_address() const { return address + 48; }
  uint16_t turn_in() const { return guest::LoadU16(base, address + 50); }
  void set_turn_in(uint16_t v) const { guest::StoreU16(base, address + 50, v); }
  uint32_t turn_in_address() const { return address + 50; }
  uint16_t apex() const { return guest::LoadU16(base, address + 52); }
  void set_apex(uint16_t v) const { guest::StoreU16(base, address + 52, v); }
  uint32_t apex_address() const { return address + 52; }
  uint16_t exit() const { return guest::LoadU16(base, address + 54); }
  void set_exit(uint16_t v) const { guest::StoreU16(base, address + 54, v); }
  uint32_t exit_address() const { return address + 54; }
  // its table of RouteCornerEntry (entry_count of them)
  uint32_t table_address() const { return address + 56; }
};

// file; measure: AiPath.+4, AiPath::JoinCornersShort.r4.+4
struct RouteCornerEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // an offset across the road
  float offset() const { return guest::LoadFloat(base, address + 4); }
  void set_offset(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t offset_address() const { return address + 4; }
};

// file; measure: AiPathEntry.+52, AiDriver::ReadVehicle.r4, AiPath::LeftEntry.r5.+52, AiPath::AddCornersAt.r4.+52, AiPath::AddCornersAt.r4.+52@+64
struct RouteSlice : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  float center_x() const { return guest::LoadFloat(base, address + 0); }
  void set_center_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t center_x_address() const { return address + 0; }
  // measure fault: the slice's corner entry (+60 + 4i) read by AiCorner::Init
  uint16_t fault_corner_number() const { return guest::LoadU16(base, address + 0); }
  void set_fault_corner_number(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_corner_number_address() const { return address + 0; }
  // measure fault: the slice's corner entry (+62 + 4i) read by AiCorner::Init
  uint16_t fault_corner_slice() const { return guest::LoadU16(base, address + 2); }
  void set_fault_corner_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t fault_corner_slice_address() const { return address + 2; }
  float center_y() const { return guest::LoadFloat(base, address + 4); }
  void set_center_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t center_y_address() const { return address + 4; }
  float center_z() const { return guest::LoadFloat(base, address + 8); }
  void set_center_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t center_z_address() const { return address + 8; }
  float width() const { return guest::LoadFloat(base, address + 12); }
  void set_width(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t width_address() const { return address + 12; }
  // beyond each side
  float margin_left() const { return guest::LoadFloat(base, address + 16); }
  void set_margin_left(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t margin_left_address() const { return address + 16; }
  float margin_right() const { return guest::LoadFloat(base, address + 20); }
  void set_margin_right(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t margin_right_address() const { return address + 20; }
  // the route bits raise and lower these
  float route_bit_a() const { return guest::LoadFloat(base, address + 24); }
  void set_route_bit_a(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t route_bit_a_address() const { return address + 24; }
  float route_bit_b() const { return guest::LoadFloat(base, address + 28); }
  void set_route_bit_b(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t route_bit_b_address() const { return address + 28; }
  // the collision mesh's section and slice
  uint16_t mesh_section() const { return guest::LoadU16(base, address + 32); }
  void set_mesh_section(uint16_t v) const { guest::StoreU16(base, address + 32, v); }
  uint32_t mesh_section_address() const { return address + 32; }
  uint16_t mesh_slice() const { return guest::LoadU16(base, address + 34); }
  void set_mesh_slice(uint16_t v) const { guest::StoreU16(base, address + 34, v); }
  uint32_t mesh_slice_address() const { return address + 34; }
  // the side direction
  float side_x() const { return guest::LoadFloat(base, address + 36); }
  void set_side_x(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t side_x_address() const { return address + 36; }
  float side_z() const { return guest::LoadFloat(base, address + 40); }
  void set_side_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t side_z_address() const { return address + 40; }
  // the forward direction
  float forward_x() const { return guest::LoadFloat(base, address + 44); }
  void set_forward_x(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t forward_x_address() const { return address + 44; }
  float forward_z() const { return guest::LoadFloat(base, address + 48); }
  void set_forward_z(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t forward_z_address() const { return address + 48; }
  // low four bits the lanes' count
  uint8_t lanes() const { return guest::LoadU8(base, address + 56); }
  void set_lanes(uint8_t v) const { guest::StoreU8(base, address + 56, v); }
  uint32_t lanes_address() const { return address + 56; }
  uint8_t kind() const { return guest::LoadU8(base, address + 57); }
  void set_kind(uint8_t v) const { guest::StoreU8(base, address + 57, v); }
  uint32_t kind_address() const { return address + 57; }
  // the corners listed on the slice
  uint16_t corner_count() const { return guest::LoadU16(base, address + 58); }
  void set_corner_count(uint16_t v) const { guest::StoreU16(base, address + 58, v); }
  uint32_t corner_count_address() const { return address + 58; }
  // 4-byte entries: a corner's number, a slice
  uint32_t corners_address() const { return address + 60; }
  uint16_t corner0_number() const { return guest::LoadU16(base, address + 60); }
  void set_corner0_number(uint16_t v) const { guest::StoreU16(base, address + 60, v); }
  uint32_t corner0_number_address() const { return address + 60; }
  uint16_t corner0_slice() const { return guest::LoadU16(base, address + 62); }
  void set_corner0_slice(uint16_t v) const { guest::StoreU16(base, address + 62, v); }
  uint32_t corner0_slice_address() const { return address + 62; }
};

// file; measure: AiPath::CompareBranches.r4, AiPath::CompareBranches.r5
struct RouteSection : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  uint16_t slice_count() const { return guest::LoadU16(base, address + 0); }
  void set_slice_count(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t slice_count_address() const { return address + 0; }
  // low four bits, 0 to 4
  uint8_t kind() const { return guest::LoadU8(base, address + 2); }
  void set_kind(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t kind_address() const { return address + 2; }
  // a bit in the branch masks
  uint8_t branch() const { return guest::LoadU8(base, address + 3); }
  void set_branch(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t branch_address() const { return address + 3; }
};

// stack; measure: AiPath::MeasureOthers.r4, AiPath::AvoidOnLeft.r4, AiPath::AvoidOnRight.r4, AiPath::ChooseSideForOther.r4, sub_8213D898.r4
struct AiOtherSpan : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the driver's record of it's ring index (+26)
  uint16_t first_entry() const { return guest::LoadU16(base, address + 0); }
  void set_first_entry(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t first_entry_address() const { return address + 0; }
  // the entry after it (or the look-ahead +5140)
  uint16_t end_entry() const { return guest::LoadU16(base, address + 2); }
  void set_end_entry(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t end_entry_address() const { return address + 2; }
  // the other's offset across the road
  float other_offset() const { return guest::LoadFloat(base, address + 4); }
  void set_other_offset(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t other_offset_address() const { return address + 4; }
  // the entries' +28 summed up to it
  float step_sum() const { return guest::LoadFloat(base, address + 8); }
  void set_step_sum(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t step_sum_address() const { return address + 8; }
  // the two drivers' +116 added
  float widths() const { return guest::LoadFloat(base, address + 12); }
  void set_widths(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t widths_address() const { return address + 12; }
  // 0.0, then the offset to keep (AiPath::ChooseSideForOther)
  float side_offset() const { return guest::LoadFloat(base, address + 16); }
  void set_side_offset(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t side_offset_address() const { return address + 16; }
};

// embedded; measure: AiDriver@+4
struct AiDriverAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint16_t unknown_0() const { return guest::LoadU16(base, address + 0); }
  void set_unknown_0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_2() const { return guest::LoadU16(base, address + 2); }
  void set_unknown_2(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
};

// embedded; measure: AiDriver@+6096
struct AiDriverAt6096 : View {
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

// embedded; measure: AiDriver@+80
struct AiDriverAt80 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: AiPath.+0.+500@+90
struct AiPathAt90 : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: AiPath.+0@+4
struct AiDriverAt4_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint16_t unknown_0() const { return guest::LoadU16(base, address + 0); }
  void set_unknown_0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: -
struct AiDriverAt80_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: AiPath@+5136
struct AiPathAt5136 : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint16_t unknown_0() const { return guest::LoadU16(base, address + 0); }
  void set_unknown_0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: AiPath@+5164
struct AiPathAt5164 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint16_t unknown_10() const { return guest::LoadU16(base, address + 10); }
  void set_unknown_10(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: AiPath@+5164.+0
struct AiDriverPart : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  uint16_t unknown_50() const { return guest::LoadU16(base, address + 50); }
  void set_unknown_50(uint16_t v) const { guest::StoreU16(base, address + 50, v); }
  uint32_t unknown_50_address() const { return address + 50; }
  uint16_t unknown_52() const { return guest::LoadU16(base, address + 52); }
  void set_unknown_52(uint16_t v) const { guest::StoreU16(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint16_t unknown_54() const { return guest::LoadU16(base, address + 54); }
  void set_unknown_54(uint16_t v) const { guest::StoreU16(base, address + 54, v); }
  uint32_t unknown_54_address() const { return address + 54; }
};

// embedded; measure: AiPath@+5164.+4
struct RouteCornerEntryPart : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: AiDriver@+6048.+0@+16
struct AiSkillSourceAt16 : View {
  using View::View;
  static constexpr uint32_t kSize = 6;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_4() const { return guest::LoadU16(base, address + 4); }
  void set_unknown_4(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: AiDriver@+512
struct AiPathAt512 : View {
  using View::View;
  static constexpr uint32_t kSize = 5536;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_5136() const { return guest::LoadU16(base, address + 5136); }
  void set_unknown_5136(uint16_t v) const { guest::StoreU16(base, address + 5136, v); }
  uint32_t unknown_5136_address() const { return address + 5136; }
  uint16_t unknown_5138() const { return guest::LoadU16(base, address + 5138); }
  void set_unknown_5138(uint16_t v) const { guest::StoreU16(base, address + 5138, v); }
  uint32_t unknown_5138_address() const { return address + 5138; }
  uint16_t unknown_5140() const { return guest::LoadU16(base, address + 5140); }
  void set_unknown_5140(uint16_t v) const { guest::StoreU16(base, address + 5140, v); }
  uint32_t unknown_5140_address() const { return address + 5140; }
  uint16_t unknown_5142() const { return guest::LoadU16(base, address + 5142); }
  void set_unknown_5142(uint16_t v) const { guest::StoreU16(base, address + 5142, v); }
  uint32_t unknown_5142_address() const { return address + 5142; }
  uint16_t unknown_5144() const { return guest::LoadU16(base, address + 5144); }
  void set_unknown_5144(uint16_t v) const { guest::StoreU16(base, address + 5144, v); }
  uint32_t unknown_5144_address() const { return address + 5144; }
  uint16_t unknown_5146() const { return guest::LoadU16(base, address + 5146); }
  void set_unknown_5146(uint16_t v) const { guest::StoreU16(base, address + 5146, v); }
  uint32_t unknown_5146_address() const { return address + 5146; }
  uint16_t unknown_5162() const { return guest::LoadU16(base, address + 5162); }
  void set_unknown_5162(uint16_t v) const { guest::StoreU16(base, address + 5162, v); }
  uint32_t unknown_5162_address() const { return address + 5162; }
  uint32_t unknown_5164() const { return guest::LoadU32(base, address + 5164); }
  void set_unknown_5164(uint32_t v) const { guest::StoreU32(base, address + 5164, v); }
  uint32_t unknown_5164_address() const { return address + 5164; }
  uint16_t unknown_5172() const { return guest::LoadU16(base, address + 5172); }
  void set_unknown_5172(uint16_t v) const { guest::StoreU16(base, address + 5172, v); }
  uint32_t unknown_5172_address() const { return address + 5172; }
  uint16_t unknown_5174() const { return guest::LoadU16(base, address + 5174); }
  void set_unknown_5174(uint16_t v) const { guest::StoreU16(base, address + 5174, v); }
  uint32_t unknown_5174_address() const { return address + 5174; }
  uint32_t unknown_5204() const { return guest::LoadU32(base, address + 5204); }
  void set_unknown_5204(uint32_t v) const { guest::StoreU32(base, address + 5204, v); }
  uint32_t unknown_5204_address() const { return address + 5204; }
  float unknown_5516() const { return guest::LoadFloat(base, address + 5516); }
  void set_unknown_5516(float v) const { guest::StoreFloat(base, address + 5516, v); }
  uint32_t unknown_5516_address() const { return address + 5516; }
  uint32_t unknown_5520() const { return guest::LoadU32(base, address + 5520); }
  void set_unknown_5520(uint32_t v) const { guest::StoreU32(base, address + 5520, v); }
  uint32_t unknown_5520_address() const { return address + 5520; }
  uint32_t unknown_5524() const { return guest::LoadU32(base, address + 5524); }
  void set_unknown_5524(uint32_t v) const { guest::StoreU32(base, address + 5524, v); }
  uint32_t unknown_5524_address() const { return address + 5524; }
  uint32_t unknown_5528() const { return guest::LoadU32(base, address + 5528); }
  void set_unknown_5528(uint32_t v) const { guest::StoreU32(base, address + 5528, v); }
  uint32_t unknown_5528_address() const { return address + 5528; }
  uint32_t unknown_5532() const { return guest::LoadU32(base, address + 5532); }
  void set_unknown_5532(uint32_t v) const { guest::StoreU32(base, address + 5532, v); }
  uint32_t unknown_5532_address() const { return address + 5532; }
};

// embedded; measure: AiDriver@+512.+0@+80
struct AiDriverAt80At80 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: AiDriver@+512.+5164
struct RouteCornerPart : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  uint16_t unknown_48() const { return guest::LoadU16(base, address + 48); }
  void set_unknown_48(uint16_t v) const { guest::StoreU16(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint16_t unknown_54() const { return guest::LoadU16(base, address + 54); }
  void set_unknown_54(uint16_t v) const { guest::StoreU16(base, address + 54, v); }
  uint32_t unknown_54_address() const { return address + 54; }
};

// embedded; measure: AiDriver@+512@+5164
struct AiPathAt5164At5164 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint16_t unknown_10() const { return guest::LoadU16(base, address + 10); }
  void set_unknown_10(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: AiDriver@+5676
struct AiPathAt5676 : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): a corner record's +4 (AiPath::ApproachCorner, JoinCornerPair, JoinCorners)
  uint32_t corner_table() const { return guest::LoadU32(base, address + 4); }
  void set_corner_table(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t corner_table_address() const { return address + 4; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): a corner record's +10 (the corner steps, AiPath::JoinCornerPair)
  uint16_t corner_offset() const { return guest::LoadU16(base, address + 10); }
  void set_corner_offset(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t corner_offset_address() const { return address + 10; }
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  float unknown_20() const { return guest::LoadFloat(base, address + 20); }
  void set_unknown_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
};

// embedded; measure: AiDriver@+5676.+0
struct AiDriverPart_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // the owner's fault_ field before the measure's fixes (2026-11-22): a route corner's +0 (AiPath::JoinCorners, KeepBranch, TrackRoute::LocateAt)
  uint16_t corner_count() const { return guest::LoadU16(base, address + 0); }
  void set_corner_count(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t corner_count_address() const { return address + 0; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): a route corner's +50 (AiPath::CornerToEntry)
  uint16_t corner_turn_in() const { return guest::LoadU16(base, address + 50); }
  void set_corner_turn_in(uint16_t v) const { guest::StoreU16(base, address + 50, v); }
  uint32_t corner_turn_in_address() const { return address + 50; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): a route corner's +52 (AiPath::CornerToApex, JoinCorners)
  uint16_t corner_apex() const { return guest::LoadU16(base, address + 52); }
  void set_corner_apex(uint16_t v) const { guest::StoreU16(base, address + 52, v); }
  uint32_t corner_apex_address() const { return address + 52; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): a route corner's +54 (AiPath::CornerToExit, JoinCorners)
  uint16_t corner_exit() const { return guest::LoadU16(base, address + 54); }
  void set_corner_exit(uint16_t v) const { guest::StoreU16(base, address + 54, v); }
  uint32_t corner_exit_address() const { return address + 54; }
};

// embedded; measure: AiDriver@+6048
struct AiSkillAt6048 : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_4() const { return guest::LoadU16(base, address + 4); }
  void set_unknown_4(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint16_t unknown_14() const { return guest::LoadU16(base, address + 14); }
  void set_unknown_14(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t unknown_14_address() const { return address + 14; }
  uint16_t unknown_16() const { return guest::LoadU16(base, address + 16); }
  void set_unknown_16(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  float unknown_20() const { return guest::LoadFloat(base, address + 20); }
  void set_unknown_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint16_t unknown_24() const { return guest::LoadU16(base, address + 24); }
  void set_unknown_24(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint8_t unknown_26() const { return guest::LoadU8(base, address + 26); }
  void set_unknown_26(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t unknown_26_address() const { return address + 26; }
  uint32_t unknown_36() const { return guest::LoadU32(base, address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
};

// embedded; measure: AiPath.+0.+500@+36
struct AiPathAt36 : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  // the owner's fault_ field before the measure's fixes (2026-11-22): AiPath::PreferredSections' signed byte
  uint8_t byte_0() const { return guest::LoadU8(base, address + 0); }
  void set_byte_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t byte_0_address() const { return address + 0; }
};

// The image words docs/functions/ai-drivers.md's code uses.
struct AiDriversGlobals {
  // 7 drivers of 6,160 bytes (AiDrivers::Create)
  template <typename T = AiDriver>
  static T drivers(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C6074u)); }
  static void set_drivers(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C6074u, v.address); }
  static void set_drivers(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6074u, v); }
  static constexpr uint32_t drivers_address() { return 0x826C6074u; }
  // the race data (+20 the branch flags, +4 a word, +0xA3AF0 and +0xA3AF4)
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // a word a racer: its input (probably)
  static uint32_t inputs(uint8_t* base) { return guest::LoadU32(base, 0x826C5B30u); }
  static void set_inputs(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B30u, v); }
  static constexpr uint32_t inputs_address() { return 0x826C5B30u; }
  // +6100 read by AiPath::RaceAllowsSides, +2184 the skill entries
  template <typename T = RaceProgress>
  static T race_state(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82E8u)); }
  static void set_race_state(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82E8u, v.address); }
  static void set_race_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t race_state_address() { return 0x826D82E8u; }
  // the race: +2308 its racers
  template <typename T = RaceManager>
  static T race(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82F4u)); }
  static void set_race(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82F4u, v.address); }
  static void set_race(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t race_address() { return 0x826D82F4u; }
  // lists of words ending with 0, 16 bytes apart
  static constexpr uint32_t branch_mask_lists_address() { return 0x82008970u; }
  // AiPath::PreferredSections' lists, 64 bytes each
  static constexpr uint32_t section_lists_address() { return 0x82008A08u; }
  // 100.0
  static float hundred(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_hundred(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t hundred_address() { return 0x82002A0Cu; }
  // 500.0
  static float five_hundred(uint8_t* base) { return guest::LoadFloat(base, 0x82001FC0u); }
  static void set_five_hundred(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FC0u, v); }
  static constexpr uint32_t five_hundred_address() { return 0x82001FC0u; }
  static float pi(uint8_t* base) { return guest::LoadFloat(base, 0x82002240u); }
  static void set_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002240u, v); }
  static constexpr uint32_t pi_address() { return 0x82002240u; }
  static float minus_pi(uint8_t* base) { return guest::LoadFloat(base, 0x820029C4u); }
  static void set_minus_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029C4u, v); }
  static constexpr uint32_t minus_pi_address() { return 0x820029C4u; }
  static float two_pi(uint8_t* base) { return guest::LoadFloat(base, 0x82001FA8u); }
  static void set_two_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FA8u, v); }
  static constexpr uint32_t two_pi_address() { return 0x82001FA8u; }
  // 30.0
  static float thirty(uint8_t* base) { return guest::LoadFloat(base, 0x82000D44u); }
  static void set_thirty(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000D44u, v); }
  static constexpr uint32_t thirty_address() { return 0x82000D44u; }
  // 10.0
  static float ten(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_ten(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t ten_address() { return 0x82001FD8u; }
  // 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // 0.1
  static float tenth(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_tenth(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t tenth_address() { return 0x82001FBCu; }
  // -1.0
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // 200.0: a reset record's gap, how far ahead the ring reaches
  static float other_gap(uint8_t* base) { return guest::LoadFloat(base, 0x820027A4u); }
  static void set_other_gap(uint8_t* base, float v) { guest::StoreFloat(base, 0x820027A4u, v); }
  static constexpr uint32_t other_gap_address() { return 0x820027A4u; }
  // 20.0
  static float twenty(uint8_t* base) { return guest::LoadFloat(base, 0x82002568u); }
  static void set_twenty(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002568u, v); }
  static constexpr uint32_t twenty_address() { return 0x82002568u; }
  // 0.2
  static float fifth(uint8_t* base) { return guest::LoadFloat(base, 0x82002208u); }
  static void set_fifth(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002208u, v); }
  static constexpr uint32_t fifth_address() { return 0x82002208u; }
  // 0.8
  static float four_fifths(uint8_t* base) { return guest::LoadFloat(base, 0x82001070u); }
  static void set_four_fifths(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001070u, v); }
  static constexpr uint32_t four_fifths_address() { return 0x82001070u; }
  // -50.0
  static float leader_gap(uint8_t* base) { return guest::LoadFloat(base, 0x82008D40u); }
  static void set_leader_gap(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008D40u, v); }
  static constexpr uint32_t leader_gap_address() { return 0x82008D40u; }
  // 0.01: the percentages' scale
  static float hundredth(uint8_t* base) { return guest::LoadFloat(base, 0x8200222Cu); }
  static void set_hundredth(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200222Cu, v); }
  static constexpr uint32_t hundredth_address() { return 0x8200222Cu; }
  // states 1 and 3's gap limit
  static float react_gap(uint8_t* base) { return guest::LoadFloat(base, 0x8200277Cu); }
  static void set_react_gap(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200277Cu, v); }
  static constexpr uint32_t react_gap_address() { return 0x8200277Cu; }
  // state 1's speed limit
  static float react_speed(uint8_t* base) { return guest::LoadFloat(base, 0x82000F14u); }
  static void set_react_speed(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000F14u, v); }
  static constexpr uint32_t react_speed_address() { return 0x82000F14u; }
  // state 1's rate limit
  static float react_rate(uint8_t* base) { return guest::LoadFloat(base, 0x820029F8u); }
  static void set_react_rate(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029F8u, v); }
  static constexpr uint32_t react_rate_address() { return 0x820029F8u; }
  // 1/30 s
  static float tick(uint8_t* base) { return guest::LoadFloat(base, 0x82617730u); }
  static void set_tick(uint8_t* base, float v) { guest::StoreFloat(base, 0x82617730u, v); }
  static constexpr uint32_t tick_address() { return 0x82617730u; }
  // 1/3.6
  static float per_kmh(uint8_t* base) { return guest::LoadFloat(base, 0x82002BD8u); }
  static void set_per_kmh(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002BD8u, v); }
  static constexpr uint32_t per_kmh_address() { return 0x82002BD8u; }
  // -100.0: AiPath::LeaderBranchKind's first best
  static float minus_hundred(uint8_t* base) { return guest::LoadFloat(base, 0x8200895Cu); }
  static void set_minus_hundred(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200895Cu, v); }
  static constexpr uint32_t minus_hundred_address() { return 0x8200895Cu; }
  // -1/60 (probably)
  static float yaw_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82008E60u); }
  static void set_yaw_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008E60u, v); }
  static constexpr uint32_t yaw_scale_address() { return 0x82008E60u; }
  // -20.0
  static float look_ahead(uint8_t* base) { return guest::LoadFloat(base, 0x82008D44u); }
  static void set_look_ahead(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008D44u, v); }
  static constexpr uint32_t look_ahead_address() { return 0x82008D44u; }
  // 12.0
  static float twelve(uint8_t* base) { return guest::LoadFloat(base, 0x82002C78u); }
  static void set_twelve(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C78u, v); }
  static constexpr uint32_t twelve_address() { return 0x82002C78u; }
};

}  // namespace schema
