// The views of docs/functions/vehicle.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/vehicle.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Vehicle@+6160
struct VehicleStatus : View {
  using View::View;
  static constexpr uint32_t kSize = 108;
  // the speed in km/h (Vehicle::Tick: the vehicle's +48 x 3.6)
  float speed_kmh() const { return guest::LoadFloat(address + 0); }
  void set_speed_kmh(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t speed_kmh_address() const { return address + 0; }
  // the car's route position (the vehicle's +5236); -1 at the reset
  uint16_t route_section() const { return guest::LoadU16(address + 4); }
  void set_route_section(uint16_t v) const { guest::StoreU16(address + 4, v); }
  uint32_t route_section_address() const { return address + 4; }
  // (the vehicle's +5238); -1 at the reset
  uint16_t route_slice() const { return guest::LoadU16(address + 6); }
  void set_route_slice(uint16_t v) const { guest::StoreU16(address + 6, v); }
  uint32_t route_slice_address() const { return address + 6; }
  // probably wrong way: set past 135 degrees off the slice's direction, cleared under 120
  uint32_t wrong_way() const { return guest::LoadU32(address + 8); }
  void set_wrong_way(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t wrong_way_address() const { return address + 8; }
  // 0 at the reset
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // the body's x axis, flat (y 0.0), normalized
  uint32_t heading_address() const { return address + 16; }
  // the heading's y, set to 0.0
  float heading_y() const { return guest::LoadFloat(address + 20); }
  void set_heading_y(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t heading_y_address() const { return address + 20; }
  // the slice's direction, normalized
  uint32_t direction_address() const { return address + 32; }
  // the car's point on the slice
  uint32_t slice_point_address() const { return address + 48; }
  // the signed distance across from the slice
  float across() const { return guest::LoadFloat(address + 64); }
  void set_across(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t across_address() const { return address + 64; }
  // how far along the slice (0.0 for a zero-length slice)
  float along() const { return guest::LoadFloat(address + 68); }
  void set_along(float v) const { guest::StoreFloat(address + 68, v); }
  uint32_t along_address() const { return address + 68; }
  // the vehicle's +6232, from TrackCollision::Tick
  uint32_t landing_ticks() const { return guest::LoadU32(address + 72); }
  void set_landing_ticks(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t landing_ticks_address() const { return address + 72; }
  // the vehicle's +6236, from TrackCollision::Tick; 0.0 at the reset
  float unknown_76() const { return guest::LoadFloat(address + 76); }
  void set_unknown_76(float v) const { guest::StoreFloat(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  // the vehicle's +6240, from TrackCollision::Tick; 0 at the reset
  uint32_t unknown_80() const { return guest::LoadU32(address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  // probably the trip (km), with the update's flag
  float trip_km() const { return guest::LoadFloat(address + 84); }
  void set_trip_km(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t trip_km_address() const { return address + 84; }
  // probably the total (km)
  float total_km() const { return guest::LoadFloat(address + 88); }
  void set_total_km(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t total_km_address() const { return address + 88; }
  // probably; 80.0 at the reset
  float water_temperature() const { return guest::LoadFloat(address + 92); }
  void set_water_temperature(float v) const { guest::StoreFloat(address + 92, v); }
  uint32_t water_temperature_address() const { return address + 92; }
  // probably; 80.0 at the reset
  float oil_temperature() const { return guest::LoadFloat(address + 96); }
  void set_oil_temperature(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t oil_temperature_address() const { return address + 96; }
  // 1.0 at the reset; from Vehicle::EngineTorque, +104 and the oil's temperature (meaning not known)
  float engine_output() const { return guest::LoadFloat(address + 100); }
  void set_engine_output(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t engine_output_address() const { return address + 100; }
  // the car data's +1420 (the power curve's +1048)
  float torque_scale() const { return guest::LoadFloat(address + 104); }
  void set_torque_scale(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t torque_scale_address() const { return address + 104; }
};

// embedded; measure: PowerCurve
struct PowerCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 1064;
  // PowerCurve::Build's read of entry i - 2's torque for a point below 0.01, met at i = 0 (before the curve)
  float back_2_torque() const { return guest::LoadFloat(address + -28); }
  void set_back_2_torque(float v) const { guest::StoreFloat(address + -28, v); }
  uint32_t back_2_torque_address() const { return address + -28; }
  // entry i - 1's torque, the same way
  float back_1_torque() const { return guest::LoadFloat(address + -12); }
  void set_back_1_torque(float v) const { guest::StoreFloat(address + -12, v); }
  uint32_t back_1_torque_address() const { return address + -12; }
  // 32 entries of 16 bytes: +0 the rpm ((i + 1) x 400), +4 the torque, +8 the power, +12 cleared
  uint32_t entries_address() const { return address + 0; }
  // 32 entries after the peak's shift (the same fields)
  uint32_t shifted_address() const { return address + 512; }
  // the peak power's entry
  uint32_t peak_index() const { return guest::LoadU32(address + 1024); }
  void set_peak_index(uint32_t v) const { guest::StoreU32(address + 1024, v); }
  uint32_t peak_index_address() const { return address + 1024; }
  // the points counted (at 0.01 or more)
  uint32_t count() const { return guest::LoadU32(address + 1028); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 1028, v); }
  uint32_t count_address() const { return address + 1028; }
  // how far the peak may move down (100 rpm steps, at least -20)
  int32_t peak_below() const { return int32_t(guest::LoadU32(address + 1032)); }
  void set_peak_below(int32_t v) const { guest::StoreU32(address + 1032, uint32_t(v)); }
  uint32_t peak_below_address() const { return address + 1032; }
  // how far up (at most 20)
  int32_t peak_above() const { return int32_t(guest::LoadU32(address + 1036)); }
  void set_peak_above(int32_t v) const { guest::StoreU32(address + 1036, uint32_t(v)); }
  uint32_t peak_above_address() const { return address + 1036; }
  // 800.0
  float low_rpm() const { return guest::LoadFloat(address + 1040); }
  void set_low_rpm(float v) const { guest::StoreFloat(address + 1040, v); }
  uint32_t low_rpm_address() const { return address + 1040; }
  // the car data's +1280
  float high_rpm() const { return guest::LoadFloat(address + 1044); }
  void set_high_rpm(float v) const { guest::StoreFloat(address + 1044, v); }
  uint32_t high_rpm_address() const { return address + 1044; }
  // the car data's +1420
  float torque_scale() const { return guest::LoadFloat(address + 1048); }
  void set_torque_scale(float v) const { guest::StoreFloat(address + 1048, v); }
  uint32_t torque_scale_address() const { return address + 1048; }
  // the shifted peak's rpm
  float peak_rpm() const { return guest::LoadFloat(address + 1052); }
  void set_peak_rpm(float v) const { guest::StoreFloat(address + 1052, v); }
  uint32_t peak_rpm_address() const { return address + 1052; }
  // the largest power after the shift
  float most_power() const { return guest::LoadFloat(address + 1056); }
  void set_most_power(float v) const { guest::StoreFloat(address + 1056, v); }
  uint32_t most_power_address() const { return address + 1056; }
  float most_power_rpm() const { return guest::LoadFloat(address + 1060); }
  void set_most_power_rpm(float v) const { guest::StoreFloat(address + 1060, v); }
  uint32_t most_power_rpm_address() const { return address + 1060; }
};

// embedded; measure: PowerCurve::FromEngine.r4
struct PowerCurveEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 156;
  // the car data's +1280: the curve's +1044
  float high_rpm() const { return guest::LoadFloat(address + 12); }
  void set_high_rpm(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t high_rpm_address() const { return address + 12; }
  // the torque curve (PowerCurvePoints, the car data's +1292)
  uint32_t torque_points_address() const { return address + 24; }
};

// embedded; measure: PowerCurve::FromEngine.r4@+24
struct PowerCurvePoints : View {
  using View::View;
  static constexpr uint32_t kSize = 132;
  // 32 floats, the torque at (i + 1) x 400 rpm (relative)
  uint32_t points_address() const { return address + 0; }
  // the car data's +1420
  float torque_scale() const { return guest::LoadFloat(address + 128); }
  void set_torque_scale(float v) const { guest::StoreFloat(address + 128, v); }
  uint32_t torque_scale_address() const { return address + 128; }
};

// embedded; measure: CameraCurve::Load.r3
struct VehicleCameraCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 724;
  // 90 floats from 0x82600EE8 + 360 x the index
  uint32_t values_address() const { return address + 0; }
  // 90 floats set to 0.0
  uint32_t cleared_address() const { return address + 360; }
  // Load's r5
  uint32_t unknown_720() const { return guest::LoadU32(address + 720); }
  void set_unknown_720(uint32_t v) const { guest::StoreU32(address + 720, v); }
  uint32_t unknown_720_address() const { return address + 720; }
};

// embedded; measure: RigidBody::Position.ret
struct VehicleBodyPosition : View {
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

// embedded; measure: Matrix::FromRows.r4, Matrix::FromRows.r5, Matrix::FromRows.r6, Matrix::FromRows.r7
struct VehicleMatrixRow : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t x_bits() const { return guest::LoadU32(address + 0); }
  void set_x_bits(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t x_bits_address() const { return address + 0; }
  uint32_t y_bits() const { return guest::LoadU32(address + 4); }
  void set_y_bits(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t y_bits_address() const { return address + 4; }
  uint32_t z_bits() const { return guest::LoadU32(address + 8); }
  void set_z_bits(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t z_bits_address() const { return address + 8; }
  uint32_t w_bits() const { return guest::LoadU32(address + 12); }
  void set_w_bits(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t w_bits_address() const { return address + 12; }
};

// embedded; measure: TrackPosition::Copy.r4
struct VehicleTrackPositionSource : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  // the mesh section
  uint16_t section() const { return guest::LoadU16(address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  uint16_t slice() const { return guest::LoadU16(address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
  uint16_t route_section() const { return guest::LoadU16(address + 4); }
  void set_route_section(uint16_t v) const { guest::StoreU16(address + 4, v); }
  uint32_t route_section_address() const { return address + 4; }
  uint16_t route_slice() const { return guest::LoadU16(address + 6); }
  void set_route_slice(uint16_t v) const { guest::StoreU16(address + 6, v); }
  uint32_t route_slice_address() const { return address + 6; }
  // the point (+16 to +31) as doublewords
  uint64_t point_xy() const { return guest::LoadU64(address + 16); }
  void set_point_xy(uint64_t v) const { guest::StoreU64(address + 16, v); }
  uint32_t point_xy_address() const { return address + 16; }
  uint64_t point_zw() const { return guest::LoadU64(address + 24); }
  void set_point_zw(uint64_t v) const { guest::StoreU64(address + 24, v); }
  uint32_t point_zw_address() const { return address + 24; }
  // the three vectors after it (+32 to +79), probably the normal and others
  uint64_t vector2_xy() const { return guest::LoadU64(address + 32); }
  void set_vector2_xy(uint64_t v) const { guest::StoreU64(address + 32, v); }
  uint32_t vector2_xy_address() const { return address + 32; }
  uint64_t vector2_zw() const { return guest::LoadU64(address + 40); }
  void set_vector2_zw(uint64_t v) const { guest::StoreU64(address + 40, v); }
  uint32_t vector2_zw_address() const { return address + 40; }
  uint64_t vector3_xy() const { return guest::LoadU64(address + 48); }
  void set_vector3_xy(uint64_t v) const { guest::StoreU64(address + 48, v); }
  uint32_t vector3_xy_address() const { return address + 48; }
  uint64_t vector3_zw() const { return guest::LoadU64(address + 56); }
  void set_vector3_zw(uint64_t v) const { guest::StoreU64(address + 56, v); }
  uint32_t vector3_zw_address() const { return address + 56; }
  uint64_t vector4_xy() const { return guest::LoadU64(address + 64); }
  void set_vector4_xy(uint64_t v) const { guest::StoreU64(address + 64, v); }
  uint32_t vector4_xy_address() const { return address + 64; }
  uint64_t vector4_zw() const { return guest::LoadU64(address + 72); }
  void set_vector4_zw(uint64_t v) const { guest::StoreU64(address + 72, v); }
  uint32_t vector4_zw_address() const { return address + 72; }
  // copied (meaning: collision-world.toml's)
  uint16_t half_80() const { return guest::LoadU16(address + 80); }
  void set_half_80(uint16_t v) const { guest::StoreU16(address + 80, v); }
  uint32_t half_80_address() const { return address + 80; }
  uint16_t half_82() const { return guest::LoadU16(address + 82); }
  void set_half_82(uint16_t v) const { guest::StoreU16(address + 82, v); }
  uint32_t half_82_address() const { return address + 82; }
};

// embedded; measure: VehicleStatus::Update.r5
struct VehicleCarObject : View {
  using View::View;
  static constexpr uint32_t kSize = 1668;
  uint32_t engine_stage() const { return guest::LoadU32(address + 1656); }
  void set_engine_stage(uint32_t v) const { guest::StoreU32(address + 1656, v); }
  uint32_t engine_stage_address() const { return address + 1656; }
  uint32_t engine_part() const { return guest::LoadU32(address + 1664); }
  void set_engine_part(uint32_t v) const { guest::StoreU32(address + 1664, v); }
  uint32_t engine_part_address() const { return address + 1664; }
};

// embedded; measure: sub_820FA8F8.r3
struct VehicleRoutePoint : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float x() const { return guest::LoadFloat(address + 0); }
  void set_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(address + 4); }
  void set_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(address + 8); }
  void set_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t z_address() const { return address + 8; }
};

// The image words docs/functions/vehicle.md's code uses.
struct VehicleGlobals {
  // the seven 6,272-byte vehicles' block (a pointer)
  static uint32_t vehicles() { return guest::LoadU32(0x826C2E34u); }
  static void set_vehicles(uint32_t v) { guest::StoreU32(0x826C2E34u, v); }
  static constexpr uint32_t vehicles_address() { return 0x826C2E34u; }
  // Vehicle::SlipWheelValue's threshold (its f1 / 3.6, set once)
  static float slip_threshold() { return guest::LoadFloat(0x826C2E38u); }
  static void set_slip_threshold(float v) { guest::StoreFloat(0x826C2E38u, v); }
  static constexpr uint32_t slip_threshold_address() { return 0x826C2E38u; }
  // bit 0x1: the threshold set
  static uint32_t slip_flag() { return guest::LoadU32(0x826C2E3Cu); }
  static void set_slip_flag(uint32_t v) { guest::StoreU32(0x826C2E3Cu, v); }
  static constexpr uint32_t slip_flag_address() { return 0x826C2E3Cu; }
  // the course's route (a pointer; +12 its section table)
  static uint32_t route() { return guest::LoadU32(0x826C5B10u); }
  static void set_route(uint32_t v) { guest::StoreU32(0x826C5B10u, v); }
  static constexpr uint32_t route_address() { return 0x826C5B10u; }
  // the racers' car input records (a pointer a racer, by the racer's low byte)
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // 1/60, in .data (the physics step)
  static float step_seconds() { return guest::LoadFloat(0x82601838u); }
  static void set_step_seconds(float v) { guest::StoreFloat(0x82601838u, v); }
  static constexpr uint32_t step_seconds_address() { return 0x82601838u; }
  // four camera curves of 90 floats, .data
  static constexpr uint32_t camera_curves_address() { return 0x82600EE8u; }
  // the surface grip's scale, .data
  static float grip_scale() { return guest::LoadFloat(0x82601488u); }
  static void set_grip_scale(float v) { guest::StoreFloat(0x82601488u, v); }
  static constexpr uint32_t grip_scale_address() { return 0x82601488u; }
  // the tire types' first factor pairs (8 bytes a type), .data
  static constexpr uint32_t grip_pairs_a_address() { return 0x826014A0u; }
  // the second factor pairs, .data
  static constexpr uint32_t grip_pairs_b_address() { return 0x82601560u; }
  // nine floats a tire type (by surface, probably), .data
  static constexpr uint32_t grip_records_address() { return 0x82601580u; }
  // the constant fourth row of Vehicle::PlaceOnRoute's matrix
  static constexpr uint32_t last_row_address() { return 0x826382D0u; }
};

}  // namespace schema
