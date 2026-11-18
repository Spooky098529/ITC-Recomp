// The vehicle's state from its rigid body, its copies and its set-up
// (docs/functions/car-physics.md): see car_physics.h. Wheel w's 368-byte
// record is at vehicle + 416 + 368 w.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "schema/car_physics.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cstring>

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace mm = matrix_math;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kWheelBytes = 368;

uint32_t Wheel(uint32_t vehicle, uint32_t w) {
  return vehicle + 416 + kWheelBytes * w;
}

uint32_t CarData(uint8_t* base, uint32_t vehicle) {
  return schema::Vehicle(base, vehicle).car_data().address;
}

uint32_t Body(uint8_t* base, uint32_t vehicle) {
  return schema::Vehicle(base, vehicle).body().address;
}

// The 16 bytes at `from` into `to` (two ld/std pairs, or the words' bits).
void Copy16(uint8_t* base, uint32_t from, uint32_t to) {
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyWord(base, from + i, to + i);
  }
}

}  // namespace

double VehicleSyncFromBody(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B13C8 Vehicle::SyncFromBody");
  const uint32_t v = vehicle, f = frame;
  // The body matrix's rows 0-2 (w 0.0) through the frame's +144, +128 and
  // +112 into +0, +16 and +32.
  constexpr uint32_t kRowTemps[] = {144, 128, 112};
  for (uint32_t row = 0; row < 3; row++) {
    const uint32_t m = va::RigidBodyMatrix(Body(base, v)) + 16 * row;
    const uint32_t t = f + kRowTemps[row];
    CopyWord(base, m + 0, t + 0);
    CopyWord(base, m + 4, t + 4);
    CopyWord(base, m + 8, t + 8);
    CopyWord(base, kZero, t + 12);
    vl::Vec4Copy(base, v + 16 * row, t);
  }
  CopyWord(base, va::RigidBodyField160(Body(base, v)), schema::Vehicle(base, v).speed_address());
  vl::Vec4Copy(base, schema::Vehicle(base, v).angular_velocity_address(), va::RigidBodyAngularVelocity(Body(base, v)));
  MatrixCopyRows(base, schema::Vehicle(base, v).body_matrix_address(), va::RigidBodyMatrix(Body(base, v)));
  // The w of the mount points (+160) and of their transforms (+224).
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, kZero, f + 172 + 16 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, kZero, f + 236 + 16 * i);
  }
  // sin(+88), computed and dropped (no state of its own).
  (void)c_runtime::Sin(double(schema::Vehicle(base, v).roll()));
  // The wheels' mount points from the car data's wheel set-up records, the
  // travel (wheel +0) added to y; their +16 into wheel +24 and the z. Then
  // through +208 into wheel +80.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t t = f + 160 + 16 * i;
    vl::Vec4Copy(base, t, schema::CarData(base, CarData(base, v)).wheel_setups_address() + 16 + 96 * i);
    const uint32_t record = CarData(base, v) + 96 * i;
    Stfs(base, t + 4, Fadds(double(schema::VehicleWheel(base, Wheel(v, i)).travel()), Lfs(base, t + 4)));
    CopyWord(base, record + 912, schema::VehicleWheel(base, Wheel(v, i)).lateral_offset_address());
    CopyWord(base, record + 912, t + 8);
  }
  for (uint32_t i = 0; i < 4; i++) {
    vl::MatrixTransformPoint(base, f + 96, schema::Vehicle(base, v).body_matrix_address(), f + 160 + 16 * i);
    vl::Vec4Copy(base, f + 224 + 16 * i, f + 96);
  }
  for (uint32_t i = 0; i < 4; i++) {
    vl::Vec4Copy(base, schema::VehicleWheel(base, Wheel(v, i)).position_address(), f + 224 + 16 * i);
  }
  // Again without the travel, into wheel +64.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t t = f + 160 + 16 * i;
    vl::Vec4Copy(base, t, schema::CarData(base, CarData(base, v)).wheel_setups_address() + 16 + 96 * i);
    const uint32_t record = CarData(base, v) + 96 * i;
    CopyWord(base, record + 912, schema::VehicleWheel(base, Wheel(v, i)).lateral_offset_address());
    CopyWord(base, record + 912, t + 8);
  }
  for (uint32_t i = 0; i < 4; i++) {
    vl::MatrixTransformPoint(base, f + 96, schema::Vehicle(base, v).body_matrix_address(), f + 160 + 16 * i);
    vl::Vec4Copy(base, f + 224 + 16 * i, f + 96);
  }
  for (uint32_t i = 0; i < 4; i++) {
    vl::Vec4Copy(base, schema::VehicleWheel(base, Wheel(v, i)).mount_point_address(), f + 224 + 16 * i);
  }
  // +3312 = +3248 about the body's position: translated by -p, +3248, by p.
  CopyWord(base, kZero, f + 88);
  CopyWord(base, kZero, f + 84);
  CopyWord(base, kZero, f + 80);
  CopyWord(base, kOne, f + 92);
  Copy16(base, va::RigidBodyPosition(Body(base, v)), f + 80);
  // Each negated (an lfs, fneg and stfs in one block: the translated code's
  // float bits with the sign flipped, a signaling NaN kept).
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 96 + i, guest::LoadU32(base, f + 80 + i) ^ 0x80000000u);
  }
  MatrixMultiplyTranslation(base, schema::Vehicle(base, v).matrix_3312_address(), schema::Vehicle(base, v).body_matrix_address(), f + 96, f - 192);
  mm::Multiply(base, schema::Vehicle(base, v).matrix_3312_address(), schema::Vehicle(base, v).matrix_3312_address(), schema::Vehicle(base, v).matrix_3248_address());
  interpolation::BlendState_OnMultiply(base, schema::Vehicle(base, v).matrix_3312_address(), schema::Vehicle(base, v).matrix_3312_address(), schema::Vehicle(base, v).matrix_3248_address(), 0x820B16EC);
  MatrixMultiplyTranslation(base, schema::Vehicle(base, v).matrix_3312_address(), schema::Vehicle(base, v).matrix_3312_address(), f + 80, f - 192);
  guest::StoreU32(base, Body(base, v) + 12, schema::Vehicle(base, v).matrix_3312_address());
  // The axle records: the mounts' midpoints (+1888, +2016) and the tracks
  // (+1992, +2120).
  const double half = Lfs(base, 0x82001BE4);
  schema::VehicleAxle(base, schema::Vehicle(base, v).front_axle_address()).set_track(float(Fsubs(double(schema::VehicleWheel(base, Wheel(v, 0)).lateral_offset()), double(schema::VehicleWheel(base, Wheel(v, 1)).lateral_offset()))));
  vl::Vec3Sum(base, f + 96, schema::VehicleWheel(base, Wheel(v, 0)).mount_point_address(), schema::VehicleWheel(base, Wheel(v, 1)).mount_point_address());
  vl::Vec4Scaled(base, f + 80, f + 96, half);
  vl::Vec4Copy(base, schema::Vehicle(base, v).front_axle_address(), f + 80);
  schema::VehicleAxle(base, schema::Vehicle(base, v).rear_axle_address()).set_track(float(Fsubs(double(schema::VehicleWheel(base, Wheel(v, 2)).lateral_offset()), double(schema::VehicleWheel(base, Wheel(v, 3)).lateral_offset()))));
  vl::Vec3Sum(base, f + 96, schema::VehicleWheel(base, Wheel(v, 2)).mount_point_address(), schema::VehicleWheel(base, Wheel(v, 3)).mount_point_address());
  vl::Vec4Scaled(base, f + 80, f + 96, half);
  vl::Vec4Copy(base, schema::Vehicle(base, v).rear_axle_address(), f + 80);
  return half;
}

void WheelCopy(uint8_t* base, uint32_t out, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_820B0A50 Wheel::Copy");
  for (uint32_t at = 0; at < 16; at += 4) {
    CopyWord(base, wheel + at, out + at);
  }
  schema::VehicleWheel(base, out).set_unknown_16(uint8_t(schema::VehicleWheel(base, wheel).unknown_16()));
  schema::VehicleWheel(base, out).set_on_ground(uint8_t(schema::VehicleWheel(base, wheel).on_ground()));
  CopyWord(base, schema::VehicleWheel(base, wheel).track_position_address(), schema::VehicleWheel(base, out).track_position_address());
  for (uint32_t at = 24; at < 40; at += 4) {
    CopyWord(base, wheel + at, out + at);
  }
  // +48 (the quaternion) by two ld/std pairs: each eight bytes read, then
  // written.
  for (uint32_t at = 48; at < 64; at += 8) {
    const uint32_t hi = guest::LoadU32(base, wheel + at), lo = guest::LoadU32(base, wheel + at + 4);
    guest::StoreU32(base, out + at, hi);
    guest::StoreU32(base, out + at + 4, lo);
  }
  vl::Vec4Copy(base, schema::VehicleWheel(base, out).mount_point_address(), schema::VehicleWheel(base, wheel).mount_point_address());
  vl::Vec4Copy(base, schema::VehicleWheel(base, out).position_address(), schema::VehicleWheel(base, wheel).position_address());
  CopyWord(base, schema::VehicleWheel(base, wheel).ground_distance_address(), schema::VehicleWheel(base, out).ground_distance_address());
  for (uint32_t at = 112; at < 176; at += 16) {
    vl::Vec4Copy(base, out + at, wheel + at);
  }
  for (uint32_t at = 176; at < 240; at += 4) {
    CopyWord(base, wheel + at, out + at);
  }
  for (uint32_t at = 240; at < 244; at++) {
    guest::StoreU8(base, out + at, guest::LoadU8(base, wheel + at));
  }
  for (uint32_t at = 244; at < 336; at += 4) {
    CopyWord(base, wheel + at, out + at);
  }
  vl::Vec4Copy(base, schema::VehicleWheel(base, out).contact_velocity_address(), schema::VehicleWheel(base, wheel).contact_velocity_address());
  CopyWord(base, schema::VehicleWheel(base, wheel).reaction_torque_address(), schema::VehicleWheel(base, out).reaction_torque_address());
  CopyWord(base, schema::VehicleWheel(base, wheel).slip_speed_address(), schema::VehicleWheel(base, out).slip_speed_address());
}

void WheelRecord2Copy(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820B0CC0 WheelRecord2::Copy");
  for (uint32_t at = 0; at < 96; at += 16) {
    vl::Vec4Copy(base, out + at, record + at);
  }
  for (uint32_t at = 96; at < 120; at += 4) {
    CopyWord(base, record + at, out + at);
  }
}

void VehicleReset(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_820B0EE8 Vehicle::Reset");
  const uint32_t v = vehicle, f = frame;
  constexpr uint32_t kAxis = 0x82638270;
  auto zero = [&](uint32_t at) { CopyWord(base, kZero, at); };
  auto one = [&](uint32_t at) { CopyWord(base, kOne, at); };
  auto word = [&](uint32_t at, uint32_t value) { guest::StoreU32(base, at, value); };
  auto byte = [&](uint32_t at, uint8_t value) { guest::StoreU8(base, at, value); };
  auto clear = [&](uint32_t at, uint32_t bytes) {
    // memset (sub_825A71D0), as its adapter does it.
    guest::ForEachHostPart(base, at, bytes,
                           [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
  };
  vl::Vec4Copy(base, schema::Vehicle(base, v).body_row0_address(), kZeroVector);
  vl::Vec4Copy(base, schema::Vehicle(base, v).body_row1_address(), kZeroVector);
  vl::Vec4Copy(base, schema::Vehicle(base, v).body_row2_address(), kZeroVector);
  zero(schema::Vehicle(base, v).speed_address());
  zero(schema::Vehicle(base, v).unknown_52_address());
  vl::Vec4Copy(base, schema::Vehicle(base, v).angular_velocity_address(), kZeroVector);
  for (uint32_t at = 80; at < 96; at += 4) {
    zero(v + at);
  }
  // +96 = (0, 1, 0) (up), its w kept.
  const uint32_t up_w = guest::LoadU32(base, schema::Vehicle(base, v).ground_up_address() + 12);
  zero(schema::Vehicle(base, v).ground_up_address());
  one(schema::Vehicle(base, v).ground_up_address() + 4);
  zero(schema::Vehicle(base, v).ground_up_address() + 8);
  word(schema::Vehicle(base, v).ground_up_address() + 12, up_w);
  vl::Vec4Copy(base, schema::Vehicle(base, v).ground_forward_address(), kZeroVector);
  vl::Vec4Copy(base, schema::Vehicle(base, v).ground_side_address(), kZeroVector);
  mm::SetIdentity(base, schema::Vehicle(base, v).ground_matrix_address());
  mm::SetIdentity(base, schema::Vehicle(base, v).body_matrix_address());
  zero(schema::Vehicle(base, v).steering_input_address());
  zero(schema::Vehicle(base, v).throttle_address());
  zero(schema::Vehicle(base, v).brake_address());
  zero(schema::Vehicle(base, v).handbrake_address());
  zero(schema::Vehicle(base, v).unknown_300_address());
  word(schema::Vehicle(base, v).car_data_address(), 0);
  zero(schema::Vehicle(base, v).steering_address());
  byte(schema::Vehicle(base, v).speed_sensitive_steering_address(), 0);
  zero(schema::Vehicle(base, v).front_left_angle_address());
  word(schema::Vehicle(base, v).wheels_on_ground_address(), 0);
  for (uint32_t at : {312u, 316u, 320u, 288u, 324u, 328u, 332u}) {
    zero(v + at);
  }
  for (uint32_t at = 352; at < 416; at += 16) {
    vl::Vec4Copy(base, v + at, kZeroVector);
  }
  clear(schema::Vehicle(base, v).wheels_address(), 1472);
  clear(schema::Vehicle(base, v).front_axle_address(), 128);
  clear(schema::Vehicle(base, v).rear_axle_address(), 128);
  double angle_f1 = 0.0;
  for (uint32_t w = 0; w < 4; w++) {
    word(v + (2144 + 4 * w), 0);
    word(v + (2160 + 4 * w), 0);
    word(v + (2176 + 4 * w), 0);
    angle_f1 = mm::QuaternionFromAxisAngle(base, schema::VehicleWheel(base, Wheel(v, w)).quaternion_address(), kAxis, Lfs(base, kOne),
                                           f - 32);
  }
  for (uint32_t at = 2176; at < 2192; at += 4) {
    word(v + at, 0);
  }
  zero(schema::Vehicle(base, v).driven_axle_spin_address());
  zero(schema::Vehicle(base, v).engine_spin_address());
  zero(schema::Vehicle(base, v).engine_spin_previous_address());
  clear(schema::Vehicle(base, v).unknown_2204_address(), 144);
  for (uint32_t at : {2348u, 2396u, 2404u, 2408u, 2412u}) {
    one(v + at);
  }
  CopyWord(base, 0x820029E8, schema::Vehicle(base, v).dt_address());
  one(schema::Vehicle(base, v).nitrous_factor_address());
  byte(schema::Vehicle(base, v).unknown_2280_address(), 0);
  zero(schema::Vehicle(base, v).nitrous_value_address());
  byte(schema::Vehicle(base, v).automatic_address(), 1);
  one(schema::Vehicle(base, v).oil_factor_address());
  word(schema::Vehicle(base, v).substep_count_address(), 10);
  CopyWord(base, 0x82002978, schema::Vehicle(base, v).unknown_2360_address());
  word(schema::Vehicle(base, v).flags_address(), 2);
  one(schema::Vehicle(base, v).brake_efficiency_address());
  word(schema::Vehicle(base, v).drive_type_address(), 0);
  CopyWord(base, 0x82002978, schema::Vehicle(base, v).inverse_dt_address());
  byte(schema::Vehicle(base, v).unknown_2380_address(), 1);
  one(schema::Vehicle(base, v).unknown_2436_address());
  byte(schema::Vehicle(base, v).unknown_2381_address(), 0);
  byte(schema::Vehicle(base, v).unknown_2382_address(), 0);
  CopyWord(base, 0x820029E4, schema::Vehicle(base, v).substep_dt_address());
  byte(schema::Vehicle(base, v).unknown_2383_address(), 0);
  zero(schema::Vehicle(base, v).speed_squared_address());
  byte(schema::Vehicle(base, v).forced_rest_address(), 0);
  zero(schema::Vehicle(base, v).unknown_2448_address());
  byte(schema::Vehicle(base, v).value2388_set_address(), 0);
  zero(schema::Vehicle(base, v).unknown_2452_address());
  byte(schema::Vehicle(base, v).abs_address(), 0);
  byte(schema::Vehicle(base, v).unknown_2387_address(), 0);
  word(schema::Vehicle(base, v).traction_counter_address(), 0);
  for (uint32_t at : {2400u, 2401u, 2424u, 2444u, 2445u, 2456u}) {
    byte(v + at, 0);
  }
  vl::Vec4Copy(base, schema::Vehicle(base, v).body_force_address(), kZeroVector);
  vl::Vec4Copy(base, schema::Vehicle(base, v).body_torque_address(), kZeroVector);
  zero(schema::Vehicle(base, v).backfire_charge_address());
  zero(schema::Vehicle(base, v).backfire_strength_address());
  word(schema::Vehicle(base, v).tire_wear_mode_address(), 0);
  zero(schema::Vehicle(base, v).throttle_used_address());
  byte(schema::Vehicle(base, v).backfire_address(), 0);
  zero(schema::Vehicle(base, v).throttle_used_previous_address());
  byte(schema::Vehicle(base, v).unknown_2512_address(), 0);
  word(schema::Vehicle(base, v).unknown_2524_address(), 0);
  vl::Vec4Copy(base, schema::Vehicle(base, v).axis_address(), kAxis);
  clear(schema::Vehicle(base, v).wheel_accumulators_address(), 80);
  zero(schema::Vehicle(base, v).unknown_2628_address());
  byte(schema::Vehicle(base, v).unknown_2624_address(), 1);
  byte(schema::Vehicle(base, v).unknown_2625_address(), 0);
  for (uint32_t at = 2632; at < 2664; at += 4) {
    word(v + at, 0);
  }
  byte(schema::Vehicle(base, v).surfaces_on_address(), 0);
  for (uint32_t i = 0; i < 32; i++) {
    word(v + (2668 + 4 * i), 0);
    word(v + (2796 + 4 * i), 0);
  }
  byte(schema::Vehicle(base, v).at_rest_address(), 0);
  CopyWord(base, schema::Vehicle(base, v).inverse_dt_address(), schema::Vehicle(base, v).unknown_2924_address());
  byte(schema::Vehicle(base, v).wastegate_open_address(), 0);
  zero(schema::Vehicle(base, v).unknown_2932_address());
  byte(schema::Vehicle(base, v).blow_off_open_address(), 0);
  one(schema::Vehicle(base, v).unknown_2936_address());
  word(schema::Vehicle(base, v).track_position_address(), 0);
  one(schema::Vehicle(base, v).unknown_2940_address());
  for (uint32_t at : {2944u, 2952u}) {
    zero(v + at);
  }
  one(schema::Vehicle(base, v).turbo_torque_factor_address());
  for (uint32_t at : {2964u, 2968u, 2972u}) {
    zero(v + at);
  }
  one(schema::Vehicle(base, v).gauge_3_address());
  one(schema::Vehicle(base, v).gauge_2_address());
  for (uint32_t at = 3108; at < 3132; at += 4) {
    zero(v + at);
  }
  byte(schema::Vehicle(base, v).upright_address(), 1);
  for (uint32_t at = 3136; at < 3160; at += 4) {
    word(v + at, 0);
  }
  for (uint32_t at = 3168; at < 3232; at += 16) {
    vl::Vec4Copy(base, v + at, kAxis);
  }
  zero(schema::Vehicle(base, v).brake_fade_address());
  zero(schema::Vehicle(base, v).brake_fade_copy_address());
  byte(schema::Vehicle(base, v).backfire_latch_address(), 0);
  byte(schema::Vehicle(base, v).tipped_address(), 0);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(v, w);
    CopyWord(base, kZero, schema::VehicleWheel(base, wheel).tire_wear_address());
    CopyWord(base, kOne, schema::VehicleWheel(base, wheel).unknown_228_address());
    const uint32_t up_w = guest::LoadU32(base, schema::VehicleWheel(base, wheel).ground_normal_address() + 12);
    CopyWord(base, kZero, schema::VehicleWheel(base, wheel).ground_normal_address());
    CopyWord(base, kOne, schema::VehicleWheel(base, wheel).ground_normal_address() + 4);
    CopyWord(base, kZero, schema::VehicleWheel(base, wheel).ground_normal_address() + 8);
    guest::StoreU32(base, schema::VehicleWheel(base, wheel).ground_normal_address() + 12, up_w);
    for (uint32_t at : {220u, 36u, 284u, 288u, 280u}) {
      CopyWord(base, kZero, wheel + at);
    }
  }
  zero(schema::Vehicle(base, v).unknown_2320_address());
  zero(schema::Vehicle(base, v).unknown_2324_address());
  zero(schema::Vehicle(base, v).unknown_2224_address());
  zero(schema::Vehicle(base, v).unknown_2204_address());
  zero(schema::Vehicle(base, v).engine_momentum_address());
  zero(schema::Vehicle(base, v).unknown_2216_address());
  zero(schema::Vehicle(base, v).unknown_2272_address());
  byte(schema::Vehicle(base, v).shift_down_request_address(), 0);
  byte(schema::Vehicle(base, v).shift_up_request_address(), 0);
  byte(schema::Vehicle(base, v).unknown_2286_address(), 0);
  // The identity built at the frame's +80 (the vector unit's words) and
  // copied (memcpy, 64 bytes) into +3248, then again into +3312.
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t to : {3248u, 3312u}) {
    for (uint32_t i = 0; i < 16; i++) {
      guest::StoreU32(base, f + 80 + 4 * i, i % 5 == 0 ? kOneBits : 0);
    }
    for (uint32_t i = 0; i < 64; i += 4) {
      CopyWord(base, f + 80 + i, v + to + i);
    }
  }
  zero(schema::Vehicle(base, v).engine_shake_address());
  word(schema::Vehicle(base, v).engine_shake_count_address(), 0);
  *f1 = angle_f1;
}

void VehicleSetUpGearing(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B4AA8 Vehicle::SetUpGearing");
  const uint32_t v = vehicle, f = frame;
  const uint32_t data = CarData(base, v);
  const schema::CarData data_view(base, data);  // the gearbox
  // The driven wheels' radius (+2332) and the axle's +200 (+2336) by the
  // drive type.
  const uint32_t drive = schema::Vehicle(base, v).drive_type();
  double axle;
  if (drive < 1) {
    CopyWord(base, schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).radius_address(), schema::Vehicle(base, v).driven_wheel_radius_address());
    axle = double(schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).wheel_inertia());
  } else {
    CopyWord(base, schema::CarDataAxle(base, schema::CarData(base, data).rear_axle_address()).radius_address(), schema::Vehicle(base, v).driven_wheel_radius_address());
    axle = double(schema::CarDataAxle(base, schema::CarData(base, data).rear_axle_address()).wheel_inertia());
    if (drive != 1) {
      axle = Fmuls(Fadds(axle, double(schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).wheel_inertia())), Lfs(base, 0x82001BE4));
    }
  }
  schema::Vehicle(base, v).set_axle_radius(float(axle));
  const double final_drive = Lfs(base, data_view.final_drive_copy_address());
  schema::Vehicle(base, v).set_unknown_2328(float(Fmuls(Fmuls(Lfs(base, data_view.unknown_1540_address()), final_drive), final_drive)));
  VehicleSetGearValues(base, v);
  // The shift-up speeds +2632 + 4 x gear.
  const double redline = Lfs(base, data_view.rev_limit_normalized_address());
  const double one = Lfs(base, kOne);
  const double per_radius = Fdivs(one, double(schema::Vehicle(base, v).driven_wheel_radius()));
  const double zero = Lfs(base, kZero);
  CopyWord(base, kZero, schema::Vehicle(base, v).shift_up_speeds_address());
  constexpr uint32_t kTop = 0x8200297C, kScale = 0x820029A4;
  Stfs(base, v + 4 * (data_view.gear_count() + 658), Fmuls(per_radius, Lfs(base, kTop)));
  if (data_view.gear_count() > 1) {
    const double scale = Lfs(base, kScale);
    const double limit = Fmuls(redline, Lfs(base, 0x82002A34));
    const double step = Lfs(base, 0x82002A68);
    const double margin = Lfs(base, 0x82002A2C);
    uint32_t out = schema::Vehicle(base, v).shift_up_speeds_address() + 4, tables = data_view.gear_table_3_address() + 4;
    for (uint32_t gear = 1;; gear++) {
      const double inverse = Fdivs(one, Lfs(base, data_view.max_rpm_address()));
      const double drive_ratio = Lfs(base, data_view.final_drive_copy_address());
      const double other = Lfs(base, data_view.unknown_1544_address());
      const double ratio = Fmuls(Lfs(base, tables + 64), drive_ratio);
      const double value = Fmuls(Lfs(base, tables + 0), other);
      const double next_ratio = Fmuls(Lfs(base, tables + 68), drive_ratio);
      const double next_value = Fmuls(Lfs(base, tables + 4), other);
      double speed = zero;
      for (;;) {
        const double at = Fmuls(speed, per_radius);
        const double rpm = Fmuls(Fmuls(inverse, at), scale);
        double x = Fmuls(rpm, ratio);
        if (limit < x) {
          Stfs(base, out, at);
          break;
        }
        const double torque = VehicleEngineTorque(base, v, x, f);
        x = Fmuls(rpm, next_ratio);
        const double next_torque = VehicleEngineTorque(base, v, x, f);
        const double here = Fmuls(Fmuls(torque, value), ratio);
        const double there = Fmuls(Fmuls(Fmuls(next_torque, next_value), next_ratio), margin);
        if (here < there) {
          Stfs(base, out, at);
          break;
        }
        speed = Fadds(speed, step);
        if (!(speed < Lfs(base, kTop))) {
          break;
        }
      }
      tables += 4;
      out += 4;
      if (!(gear + 1 < data_view.gear_count())) {
        break;
      }
    }
  }
  // +2300; the peak of the torque curve (the car data's +1292, 32 points): its index's
  // float (through the frame's +80) into +2296 and +2292.
  schema::Vehicle(base, v).set_unknown_2300(float(Fadds(Fadds(Lfs(base, data_view.inertia_2_address()), Lfs(base, data_view.engine_inertia_address())), Lfs(base, data_view.gear_table_2_address() + 4))));
  double peak = zero;
  uint32_t index = 0;
  for (uint32_t i = 0; i < 32; i++) {
    const double point = Lfs(base, data_view.torque_curve_address() + 4 * i);
    if (peak < point) {
      peak = point;
      index = i;
    }
  }
  guest::StoreU32(base, f + 80, 0);
  guest::StoreU32(base, f + 84, index);
  const double rpm = Fmuls(double(float(double(index))), Lfs(base, 0x82002A60));
  schema::Vehicle(base, v).set_unknown_2296(float(rpm));
  const double scaled = Fmuls(rpm, Lfs(base, 0x82002A14));
  schema::Vehicle(base, v).set_unknown_2292(float(Fmadds(Lfs(base, data_view.idle_rpm_address()), Lfs(base, 0x82002A64), scaled)));
}

double VehicleSetSteering(uint8_t* base, uint32_t vehicle, double steer) {
  ITC_NATIVE_RUNNING("sub_820B3D10 Vehicle::SetSteering");
  const uint32_t v = vehicle;
  double clamped = Lfs(base, 0x82001BE0);
  const double one = Lfs(base, kOne);
  schema::Vehicle(base, v).set_speed_sensitive_steering(uint8_t(1));
  if (!(steer < clamped)) {
    clamped = steer > one ? one : steer;
  }
  const double spin = Fmadds(Fabs(double(schema::VehicleWheel(base, Wheel(v, 1)).spin())), Lfs(base, 0x82001FBC),
                             Fabs(double(schema::VehicleWheel(base, Wheel(v, 0)).spin())));
  const uint32_t data = CarData(base, v);
  const double per_length = Fdivs(one, double(schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).unknown_144()));
  const double scale = spin > one ? one : spin;
  const double a = Fmuls(double(schema::VehicleWheel(base, Wheel(v, 0)).lateral_force()), per_length);
  const double b = Fmuls(double(schema::VehicleWheel(base, Wheel(v, 1)).lateral_force()), per_length);
  const double turned = c_runtime::Sin(Fmuls(double(schema::CarData(base, data).max_steering_angle()), clamped));
  const double full = c_runtime::Sin(double(schema::CarData(base, CarData(base, v)).max_steering_angle()));
  schema::Vehicle(base, v).set_steering_input(float(clamped));
  const double mean = Fmuls(Fadds(b, a), Lfs(base, 0x82001BE4));
  const double ratio = Fdivs(double(float(turned)), double(float(full)));
  schema::Vehicle(base, v).set_steering_result(float(Fmadds(ratio, scale, mean)));
  return full;
}

void SteerRecordFill(uint8_t* base, uint32_t out, uint32_t record, double f1, double f2,
                     double f3) {
  ITC_NATIVE_RUNNING("sub_820B0668 SteerRecord::Fill");
  const double product = Fmuls(f1, f2);
  schema::SteerRecord(base, out).set_load_product(float(product));
  schema::SteerRecord(base, out).set_axle_32(uint32_t(schema::CarDataAxle(base, record).tire_32_address()));
  schema::SteerRecord(base, out).set_axle_16(uint32_t(schema::CarDataAxle(base, record).tire_16_address()));
  const double scaled = Fmuls(product, Lfs(base, 0x820029DC));
  schema::SteerRecord(base, out).set_scaled(float(scaled));
  double x = f1;
  const double knee = double(schema::CarDataAxle(base, record).tire_28());
  if (f1 > knee) {
    x = Fmadds(Fsubs(f1, knee), Lfs(base, 0x82002208), knee);
  }
  double g = Fnmsubs(double(schema::CarDataAxle(base, record).tire_20()), x, double(schema::CarDataAxle(base, record).tire_16()));
  if (!(g > Lfs(base, kZero))) {
    g = Lfs(base, 0x820029D8);
  }
  const double value = Fmuls(g, x);
  schema::SteerRecord(base, out).set_value(float(value));
  const double c = Lfs(base, 0x820021E4);
  const double a = Fmuls(product, c);
  const double b = Fmuls(scaled, c);
  const double times = Fmuls(value, f3);
  schema::SteerRecord(base, out).set_times(float(times));
  schema::SteerRecord(base, out).set_ratio_a(float(Fdivs(value, a)));
  schema::SteerRecord(base, out).set_ratio_b(float(Fdivs(value, b)));
  schema::SteerRecord(base, out).set_ratio_c(float(Fdivs(times, value)));
}

namespace {

// sub_820B3E98's angle: f1, negated outside the floats at 0x82001FB4 and
// 0x82002A50.
double CurveAngle(uint8_t* base, double angle) {
  if (angle > Lfs(base, 0x82001FB4) || angle < Lfs(base, 0x82002A50)) {
    return Fmuls(angle, Lfs(base, 0x82001BE0));
  }
  return angle;
}

}  // namespace

bool SteerRecordCurve(uint8_t* base, uint32_t record, double angle, double* result) {
  ITC_NATIVE_RUNNING("sub_820B3E98 SteerRecord::Curve");
  const double minus_one = Lfs(base, 0x82001BE0);
  double t;
  if (!c_runtime::Tan(CurveAngle(base, angle), &t)) {
    return false;
  }
  const double tangent = double(float(t));
  const double half_width = Fmuls(double(schema::SteerRecord(base, record).times()), Lfs(base, 0x82001BE4));
  const double length = double(schema::SteerRecord(base, record).load_product());
  double x = Lfs(base, 0x820021E4);
  const double limit = Fmuls(Fdivs(length, half_width), x);
  const double size = Fabs(tangent);
  if (size < limit) {
    x = Fmuls(Fdivs(size, length), half_width);
  }
  const double c = Lfs(base, 0x82002958);
  const double x2 = Fmuls(x, x);
  const double cx = Fmuls(x, c);
  const double x3 = Fmuls(x2, x);
  const double first = Fnmsubs(x2, c, cx);
  const double x4 = Fmuls(x3, x);
  const double second = Fmadds(x3, Lfs(base, 0x82002A4C), first);
  double value = Fnmsubs(x4, Lfs(base, 0x82002A48), second);
  if (tangent < Lfs(base, kZero)) {
    value = Fmuls(value, minus_one);
  }
  *result = value;
  return true;
}

bool VehicleSteerTowards(uint8_t* base, uint32_t vehicle, double target, double dt, double f3,
                         uint32_t left, uint32_t right, double* f1) {
  ITC_NATIVE_RUNNING("sub_820B5FD8 Vehicle::SteerTowards");
  const uint32_t v = vehicle;
  const double half = Lfs(base, 0x82001BE4);
  // The front wheels' mean slip (+264), the curve's angle: where tan would
  // take its _except1 path, nothing done (the original runs).
  const double slip =
      Fmuls(Fadds(double(schema::VehicleWheel(base, Wheel(v, 1)).slip_side()), double(schema::VehicleWheel(base, Wheel(v, 0)).slip_side())), half);
  {
    double t;
    if (!c_runtime::Tan(CurveAngle(base, slip), &t)) {
      return false;
    }
  }
  const uint32_t data = CarData(base, v);
  schema::Vehicle(base, v).set_speed_sensitive_steering(uint8_t(1));
  const double rate = Fmuls(double(schema::CarData(base, data).steering_316()), dt);
  const double minus_one = Lfs(base, 0x82001BE0);
  const double back_rate = Fmuls(double(schema::CarData(base, data).steering_320()), dt);
  if ((left & 0xFF) != 0) {
    target = minus_one;
  }
  const double one = Lfs(base, kOne);
  if ((right & 0xFF) != 0) {
    target = one;
  }
  const double zero = Lfs(base, kZero);
  double speed = Fsubs(Fabs(double(schema::Vehicle(base, v).speed())), Lfs(base, 0x820029F0));
  if (speed < zero) {
    speed = zero;
  }
  const double load = Fmuls(Fadds(double(schema::VehicleWheel(base, Wheel(v, 1)).load()), double(schema::VehicleWheel(base, Wheel(v, 0)).load())),
                            half);
  const double sensitivity = double(schema::CarData(base, data).steering_328());
  const double aligning = double(schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).unknown_148());
  const double factor =
      Fdivs(one, Fdivs(Fmuls(sensitivity, f3), Fadds(sensitivity, speed)));
  double toward = Fmuls(factor, rate);
  const double away = Fmuls(factor, back_rate);
  SteerRecordFill(base, schema::Vehicle(base, v).steer_record_address(), schema::CarData(base, data).front_axle_address(), load, one, aligning);
  double curve;
  SteerRecordCurve(base, schema::Vehicle(base, v).steer_record_address(), slip, &curve);
  const double turning = Fsubs(double(schema::Vehicle(base, v).steering()), double(schema::Vehicle(base, v).slip_term()));
  const double steer = double(schema::Vehicle(base, v).steering_input());
  double move = Fsubs(target, steer);
  schema::Vehicle(base, v).set_steering_result(float(curve));
  if (move > zero) {
    if (turning > zero) {
      toward = away;
    }
    if (move > toward) {
      move = toward;
    }
  } else {
    if (turning < zero) {
      toward = away;
    }
    const double most = Fneg(toward);
    if (move < most) {
      move = most;
    }
  }
  double moved = Fadds(steer, move);
  schema::Vehicle(base, v).set_steering_input(float(moved));
  if (moved < minus_one) {
    moved = minus_one;
  } else if (moved > Lfs(base, kOne)) {
    moved = Lfs(base, kOne);
  }
  schema::Vehicle(base, v).set_steering_input(float(moved));
  *f1 = curve;
  return true;
}

double VehicleSuspensionSpring(uint8_t* base, uint32_t vehicle, uint32_t w, double change,
                               uint32_t velocity) {
  ITC_NATIVE_RUNNING("sub_820B0198 Vehicle::SuspensionSpring");
  const uint32_t v = vehicle, wheel = Wheel(v, w);
  const uint32_t r = schema::CarDataAxle(base, guest::LoadU32(base, schema::Vehicle(base, v).wheel_axle_data_address() + 4 * w)).suspension_address();  // the suspension
  const uint32_t partner = Wheel(v, (w & 1) != 0 ? w - 1 : w + 1);
  const double travel = double(schema::VehicleWheel(base, wheel).travel());
  double moved = Fadds(travel, change);
  const double previous = double(schema::VehicleWheel(base, wheel).travel_previous());
  const double roll = Lfs(base, r + 36);
  const double mass = Lfs(base, r + 0);
  const double dt = double(schema::Vehicle(base, v).dt());
  const double partner_previous = Lfs(base, partner + 12);
  const double c = Lfs(base, 0x82002980);
  const double half = Lfs(base, 0x82001BE4);
  const double zero = Lfs(base, kZero);
  const double anti_roll = Fmuls(Fmuls(Fsubs(partner_previous, previous), half), roll);
  const double spring = Lfs(base, r + 12);
  const double per_spring = Fdivs(mass, spring);
  const double rebound = Lfs(base, r + 20);
  const double rest = Fmadds(per_spring, c, travel);
  const double leaving =
      Fmuls(Fdivs(Fmsubs(rest, spring, anti_roll), rebound), dt);
  double damping;
  if (change > zero) {
    damping = Fmuls(Lfs(base, r + 16), change);
  } else {
    if (leaving > zero && change < Fneg(leaving)) {
      // The wheel leaves the ground: the travel less that, -R32.
      schema::VehicleWheel(base, wheel).set_travel(float(Fsubs(travel, leaving)));
      return Fneg(Lfs(base, r + 32));
    }
    damping = Fmuls(rebound, change);
  }
  const uint32_t steps = schema::Vehicle(base, v).substep_count();
  const double start = Lfs(base, velocity);
  const double step = double(schema::Vehicle(base, v).substep_dt());
  double momentum = Fmsubs(start, mass, damping);
  if (steps != 0) {
    const double stiffer = Lfs(base, 0x82002568);
    const double past = Lfs(base, 0x82001070);
    for (uint32_t i = 0;;) {
      const double limit = Fmuls(Lfs(base, r + 24), past);
      const double k = Lfs(base, r + 12);
      double force;
      if (moved < limit) {
        force = Fneg(Fmuls(moved, k));
      } else {
        const double over = Fsubs(moved, limit);
        const double plain = Fmuls(k, moved);
        force = Fnmsubs(Fmuls(over, k), stiffer, Fneg(plain));
      }
      const double speed = Lfs(base, velocity);
      if (speed > zero) {
        force = Fnmsubs(Lfs(base, r + 16), speed, force);
      } else {
        force = Fnmsubs(speed, Lfs(base, r + 20), force);
      }
      momentum = Fmadds(force, step, momentum);
      const double new_speed = Fmuls(Lfs(base, r + 4), momentum);
      Stfs(base, velocity, new_speed);
      i++;
      moved = Fmadds(new_speed, step, moved);
      if (!(i < schema::Vehicle(base, v).substep_count())) {
        break;
      }
    }
  }
  schema::VehicleWheel(base, wheel).set_travel(float(moved));
  const double change_of_speed = Fsubs(Lfs(base, velocity), start);
  const double per_dt = Fdivs(Lfs(base, 0x82001BE0), double(schema::Vehicle(base, v).dt()));
  return Fmuls(Fmuls(change_of_speed, per_dt), Lfs(base, r + 0));
}

double VehicleUpdateWheelSuspension(uint8_t* base, uint32_t vehicle, uint32_t w,
                                    uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B5588 Vehicle::UpdateWheelSuspension");
  const uint32_t v = vehicle, f = frame, wheel = Wheel(v, w);
  const double down = Fneg(double(schema::VehicleWheel(base, wheel).ground_distance()));
  const uint32_t data = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axle_data_address() + 4 * w);
  schema::VehicleWheel(base, wheel).set_unknown_16(uint8_t(0));
  va::RigidBodyPointVelocity(base, f + 96, Body(base, v), schema::VehicleWheel(base, wheel).mount_point_address());
  const double vertical = double(vl::Vec3DotOf(base, f + 96, schema::Vehicle(base, v).ground_matrix_address() + 16));
  Stfs(base, f + 80, Fneg(vertical));
  const double result = VehicleSuspensionSpring(base, v, w, down, f + 80);
  schema::VehicleWheel(base, wheel).set_travel_speed(float(Fneg(Lfs(base, f + 80))));
  double force = Fadds(double(schema::CarDataAxle(base, data).unknown_128()), result);
  const double zero = Lfs(base, kZero);
  if (!(double(schema::VehicleWheel(base, wheel).travel()) > Lfs(base, schema::CarDataAxle(base, data).suspension_address() + 28))) {
    CopyWord(base, schema::CarDataAxle(base, data).suspension_address() + 28, schema::VehicleWheel(base, wheel).travel_address());
    force = zero;
    schema::VehicleWheel(base, wheel).set_on_ground(uint8_t(0));
  }
  if (!(force > zero)) {
    force = zero;
    schema::VehicleWheel(base, wheel).set_on_ground(uint8_t(0));
  } else {
    schema::VehicleWheel(base, wheel).set_on_ground(uint8_t(1));
    schema::Vehicle(base, v).set_wheels_on_ground(uint32_t(schema::Vehicle(base, v).wheels_on_ground() + 1));
  }
  schema::VehicleWheel(base, wheel).set_suspension_force(float(force));
  return result;
}

uint32_t VehicleUpdateSuspension(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_820B6328 Vehicle::UpdateSuspension");
  const uint32_t v = vehicle, f = frame;
  schema::Vehicle(base, v).set_wheels_on_ground(uint32_t(0));
  for (uint32_t w = 0; w < 4; w++) {
    guest::StoreU32(base, schema::Vehicle(base, v).wheels_by_load_address() + 4 * w, w);
    CopyWord(base, schema::VehicleWheel(base, Wheel(v, w)).travel_address(), schema::VehicleWheel(base, Wheel(v, w)).travel_previous_address());
  }
  // The order by load (+280), a bubble sort from the back.
  for (uint32_t pass = 0; pass < 4; pass++) {
    if (pass >= 3) {
      continue;
    }
    uint32_t at = schema::Vehicle(base, v).wheels_by_load_address() + 8;
    for (uint32_t n = 3 - pass; n != 0; n--, at -= 4) {
      const uint32_t a = guest::LoadU32(base, at), b = guest::LoadU32(base, at + 4);
      if (Lfs(base, v + 368 * a + 696) > Lfs(base, v + 368 * b + 696)) {
        guest::StoreU32(base, at + 4, a);
        guest::StoreU32(base, at, b);
      }
    }
  }
  for (uint32_t i = 0; i < 4; i++) {
    VehicleUpdateWheelSuspension(base, v, guest::LoadU32(base, schema::Vehicle(base, v).wheels_by_load_address() + 4 * i), f - 176);
  }
  double last = 0.0;
  for (uint32_t w = 0; w < 4; w++) {
    last = double(schema::VehicleWheel(base, Wheel(v, w)).suspension_force());
    vl::Vec4Scaled(base, f + 80, schema::Vehicle(base, v).ground_up_address(), last);
    vl::Vec3AddTo(base, schema::Vehicle(base, v).wheel_accumulators_address() + 16 * w, f + 80);
  }
  *f1 = last;
  return schema::Vehicle(base, v).wheel_accumulators_address() + 48;
}

void VehicleUpdateDrivenAxle(uint8_t* base, uint32_t vehicle, uint32_t first, double drive,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820B31D0 Vehicle::UpdateDrivenAxle");
  const uint32_t v = vehicle;
  const uint32_t record = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axles_address() + 4 * first);
  const uint32_t data = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axle_data_address() + 4 * first);
  const uint32_t a = Wheel(v, first), b = Wheel(v, first + 1);
  const double size = Fabs(drive);
  const double half = Lfs(base, 0x82001BE4);
  const double inertia = Fmuls(double(schema::VehicleAxle(base, record).inertia()), half);
  // Its temporaries below the caller's r1 (`sp`): the accelerations at -64
  // and -60, the locked flags at -48 and -44, the wheels at -56 and -52.
  const uint32_t accel = sp - 64, locked = sp - 48, wheels = sp - 56;
  guest::StoreU32(base, locked + 0, 0);
  guest::StoreU32(base, locked + 4, 0);
  guest::StoreU32(base, wheels + 0, a);
  guest::StoreU32(base, wheels + 4, b);
  const double grip = double(schema::CarDataAxle(base, data).radius());
  if (!(size > Lfs(base, 0x8200222C))) {
    // No drive: braked wheels stop and +2192 decays.
    const double zero = Lfs(base, kZero);
    const double decay = Lfs(base, 0x82002968);
    for (uint32_t i = 0; i < 2; i++) {
      const uint32_t wheel = guest::LoadU32(base, wheels + 4 * i);
      if (double(schema::VehicleWheel(base, wheel).brake_torque()) == zero) {
        continue;
      }
      schema::VehicleWheel(base, wheel).set_spin(float(zero));
      schema::VehicleWheel(base, wheel).set_spin_momentum(float(zero));
      const double mean = double(schema::Vehicle(base, v).driven_axle_spin());
      schema::Vehicle(base, v).set_driven_axle_spin(float(mean == zero ? zero : Fmuls(mean, decay)));
    }
    return;
  }
  const double zero = Lfs(base, kZero);
  const double bias =
      double(schema::Vehicle(base, v).throttle()) > zero ? double(schema::CarDataAxle(base, data).unknown_260()) : double(schema::CarDataAxle(base, data).unknown_264());
  const double torque = Fmuls(inertia, drive);
  const double a_slip = Fabs(double(schema::VehicleWheel(base, a).longitudinal_force_2()));
  const double preload = double(schema::CarDataAxle(base, data).unknown_268());
  const double b_slip = Fabs(double(schema::VehicleWheel(base, b).longitudinal_force_2()));
  const double lock = Fmadds(Fabs(torque), bias, preload);
  double a_accel, b_accel;
  if (a_slip == zero && b_slip == zero) {
    a_accel = Fmuls(drive, half);
    b_accel = a_accel;
  } else {
    const double a_reaction = double(schema::VehicleWheel(base, a).reaction_torque());
    const double sum = Fadds(b_slip, a_slip);
    const double b_reaction = double(schema::VehicleWheel(base, b).reaction_torque());
    const double t = Fmadds(a_reaction, a_slip, Fmuls(Fmuls(b_slip, inertia), drive));
    a_accel = Fdivs(Fdivs(Fnmsubs(b_reaction, b_slip, t), inertia), sum);
    b_accel = Fsubs(drive, a_accel);
  }
  const double a_reaction = double(schema::VehicleWheel(base, a).reaction_torque());
  const double b_reaction = double(schema::VehicleWheel(base, b).reaction_torque());
  const double a_torque = Fmsubs(a_accel, inertia, a_reaction);
  const double b_torque = Fmsubs(b_accel, inertia, b_reaction);
  double larger, smaller;
  bool a_larger;
  if (Fabs(a_torque) > Fabs(b_torque)) {
    larger = Fabs(a_torque);
    smaller = Fabs(b_torque);
    a_larger = true;
  } else {
    larger = Fabs(b_torque);
    smaller = Fabs(a_torque);
    a_larger = false;
  }
  if (Fsubs(larger, smaller) < lock) {
    // Locked: one acceleration for the axle.
    const double step = double(schema::Vehicle(base, v).substep_dt());
    const double a_spin = double(schema::VehicleWheel(base, a).spin());
    const double b_spin = double(schema::VehicleWheel(base, b).spin());
    schema::VehicleAxle(base, record).set_locked(uint32_t(1));
    const double twice = Fmuls(step, Lfs(base, 0x820021E8));
    const double b_value = Fdivs(Fsubs(Fmadds(step, drive, a_spin), b_spin), twice);
    Stfs(base, accel + 4, b_value);
    Stfs(base, accel + 0, Fsubs(drive, b_value));
  } else {
    const double part = Fmuls(lock, half);
    double a_total, b_total;
    // The larger torque's wheel gives up half the lock, the other takes it
    // (by the larger's sign).
    if (a_larger ? !(a_torque > zero) : a_torque > zero) {
      a_total = Fadds(part, a_torque);
      b_total = Fsubs(b_torque, part);
    } else {
      a_total = Fsubs(a_torque, part);
      b_total = Fadds(part, b_torque);
    }
    a_total = Fadds(a_reaction, a_total);
    b_total = Fadds(b_reaction, b_total);
    const double per_inertia = Fdivs(Lfs(base, kOne), inertia);
    Stfs(base, accel + 0, Fmuls(a_total, per_inertia));
    Stfs(base, accel + 4, Fmuls(b_total, per_inertia));
  }
  // Twice over: a wheel whose spin would change sign (or a stopped braked
  // wheel held by its brake) locks, and its excess goes to the other.
  for (int pass = 0; pass < 2; pass++) {
    for (uint32_t i = 0; i < 2; i++) {
      if (guest::LoadU32(base, locked + 4 * i) == 1) {
        continue;
      }
      const double step = double(schema::Vehicle(base, v).substep_dt());
      const uint32_t wheel = guest::LoadU32(base, wheels + 4 * i);
      const double acceleration = Lfs(base, accel + 4 * i);
      const double brake = double(schema::VehicleWheel(base, wheel).brake_torque());
      const double spin = double(schema::VehicleWheel(base, wheel).spin());
      const double held = Fabs(brake);
      double next = Fmadds(step, acceleration, spin);
      const double product = Fmuls(next, spin);
      bool lock_now = product < zero;
      if (!lock_now && spin == zero) {
        lock_now = Fdivs(Fmuls(held, grip), inertia) > Fabs(acceleration);
      }
      if (lock_now) {
        guest::StoreU32(base, locked + 4 * i, 1);
        next = zero;
      }
      if (guest::LoadU32(base, locked + 4 * i) == 0) {
        continue;
      }
      const uint32_t j = (i + 1) & 1;
      const double excess = Fsubs(acceleration, Fdivs(Fsubs(next, spin), step));
      const double other = Lfs(base, accel + 4 * j);
      double moved = Fmuls(other, excess) > zero ? Fsubs(other, excess) : Fadds(other, excess);
      if (!(Fmuls(moved, other) > zero)) {
        moved = zero;
      }
      Stfs(base, accel + 4 * j, moved);
    }
  }
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t wheel = guest::LoadU32(base, wheels + 4 * i);
    const uint32_t is_locked = guest::LoadU32(base, locked + 4 * i);
    const double held = Fmuls(Fabs(double(schema::VehicleWheel(base, wheel).brake_torque())), grip);
    const double old_spin = double(schema::VehicleWheel(base, wheel).spin());
    if (is_locked != 0) {
      schema::VehicleWheel(base, wheel).set_spin(float(zero));
    } else {
      schema::VehicleWheel(base, wheel).set_spin(float(Fmadds(Lfs(base, accel + 4 * i), double(schema::Vehicle(base, v).substep_dt()),
                                     double(schema::VehicleWheel(base, wheel).spin()))));
    }
    schema::VehicleWheel(base, wheel).set_spin_momentum(float(Fmuls(double(schema::VehicleWheel(base, wheel).spin()), inertia)));
    const double applied = Fabs(Fmuls(Lfs(base, accel + 4 * i), inertia));
    if (held > zero && !(old_spin == zero)) {
      const double change = Fdivs(Fsubs(double(schema::VehicleWheel(base, wheel).spin()), old_spin), double(schema::Vehicle(base, v).substep_dt()));
      const double share = double(schema::VehicleWheel(base, wheel).brake_share());
      schema::VehicleWheel(base, wheel).set_brake_force(float(Fabs(Fmuls(Fdivs(Fmsubs(change, inertia, Fsubs(applied, held)), grip), share))));
    }
  }
  const double mean = double(schema::Vehicle(base, v).driven_axle_spin());
  const double spins = Fadds(double(schema::VehicleWheel(base, b).spin()), double(schema::VehicleWheel(base, a).spin()));
  const double next = mean == zero ? spins : Fmadds(spins, half, mean);
  schema::Vehicle(base, v).set_driven_axle_spin(float(Fmuls(next, half)));
}

void VehicleUpdateDrivetrain(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B4778 Vehicle::UpdateDrivetrain");
  const uint32_t v = vehicle, f = frame;
  const schema::CarData data_view(base, CarData(base, v));  // the engine
  CopyWord(base, schema::Vehicle(base, v).engine_spin_address(), schema::Vehicle(base, v).engine_spin_previous_address());
  const int32_t gear = schema::Vehicle(base, v).gear();
  double ratio = double(schema::Vehicle(base, v).total_ratio());
  const double zero = Lfs(base, kZero);
  // The clutch squared (0.0 in neutral or with the clutch at 0.0); the
  // ratio negated in reverse.
  double clutch = zero;
  if (gear != 0) {
    const double c = double(schema::Vehicle(base, v).clutch());
    if (!(c == zero)) {
      if (gear == -1) {
        ratio = Fneg(ratio);
      }
      clutch = Fmuls(c, c);
    }
  }
  const uint32_t drive = schema::Vehicle(base, v).drive_type();
  const double half = Lfs(base, 0x82001BE4);
  // The driven axles' reaction torques (AxleReactionTorque) and, with one,
  // its half inertia; with both, the halves at the frame's +80 and +84.
  double front = 0.0, rear = 0.0, reaction = 0.0, inertia = 0.0;
  bool both = false;
  if (drive >= 2) {
    front = VehicleAxleReactionTorque(base, v, 0);
    rear = VehicleAxleReactionTorque(base, v, 2);
    both = true;
    const uint32_t front_record = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axles_address());
    const uint32_t rear_record = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axles_address() + 8);
    const double rear_inertia = Lfs(base, rear_record + 108);
    Stfs(base, f + 80, Fmuls(Lfs(base, front_record + 108), half));
    Stfs(base, f + 84, Fmuls(rear_inertia, half));
  } else {
    uint32_t record;
    if (drive == 1) {
      VehicleUpdateFreeAxle(base, v, 0);
      reaction = VehicleAxleReactionTorque(base, v, 2);
      record = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axles_address() + 8);
    } else {
      reaction = VehicleAxleReactionTorque(base, v, 0);
      VehicleUpdateFreeAxle(base, v, 2);
      record = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axles_address());
    }
    inertia = Fmuls(Lfs(base, record + 108), half);
  }
  const double engine_inertia = Fadds(Lfs(base, data_view.inertia_2_address()), Lfs(base, data_view.engine_inertia_address()));
  const double mean = double(schema::Vehicle(base, v).driven_axle_spin());
  const double torque = double(schema::Vehicle(base, v).engine_torque());
  const double step = double(schema::Vehicle(base, v).substep_dt());
  const double two = Lfs(base, 0x820021E8);
  const double momentum = double(schema::Vehicle(base, v).unknown_2212());
  const double scaled_mean = Fmuls(mean, two);
  schema::Vehicle(base, v).set_driven_axle_spin(float(zero));
  const double damping = Lfs(base, 0x820021F8);
  const double one = Lfs(base, kOne);
  const double slip = Fsubs(one, clutch);
  double engine_spin;  // f26
  if (both) {
    const double rear_half = Lfs(base, f + 84);
    const uint32_t rear_record = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axles_address() + 8);
    const double twice_mean = Fmuls(scaled_mean, two);
    const double front_half = Lfs(base, f + 80);
    const uint32_t front_record = guest::LoadU32(base, schema::Vehicle(base, v).wheel_axles_address());
    const double damped = Fmuls(momentum, damping);
    const double per_rear = Fdivs(one, rear_half);
    const double rear_share = Lfs(base, rear_record + 100);
    const double rear_drive = Fmuls(rear_share, torque);
    const double front_share = Lfs(base, front_record + 100);
    const double per_front = Fdivs(one, front_half);
    const double front_drive = Fmuls(front_share, torque);
    const double rear_part = Fmuls(per_rear, rear_share);
    const double rear_term = Fmuls(Fmadds(rear_drive, ratio, rear), per_rear);
    const double front_reaction = Fmuls(Fmuls(per_front, front), slip);
    const double front_term = Fmadds(front_drive, ratio, front);
    const double shares = Fmuls(Fmuls(Fmadds(per_front, front_share, rear_part), engine_inertia),
                                ratio);
    const double terms = Fmadds(front_term, per_front, rear_term);
    const double rhs = Fmsubs(Fmadds(terms, step, twice_mean), ratio, damped);
    const double lhs = Fmuls(Fmadds(shares, ratio, damping), step);
    engine_spin = Fdivs(rhs, lhs);
    const double residual = Fmuls(Fnmsubs(engine_spin, engine_inertia, torque), ratio);
    const double front_axle = Fmuls(Fmadds(residual, front_share, front), per_front);
    VehicleUpdateDrivenAxle(base, v, 0, Fmadds(front_axle, clutch, front_reaction), f);
    const double rear_reaction = Fmuls(Fmuls(per_rear, rear), slip);
    const double rear_axle = Fmuls(Fmadds(residual, rear_share, rear), per_rear);
    VehicleUpdateDrivenAxle(base, v, 2, Fmadds(rear_axle, clutch, rear_reaction), f);
  } else {
    const double drive_mean = Fmuls(scaled_mean, ratio);
    const double axle_inertia = Fmuls(inertia, two);
    const double geared = Fmuls(engine_inertia, ratio);
    const double t0 = Fdivs(Fmsubs(drive_mean, half, momentum), step);
    const double per_axle = Fdivs(reaction, axle_inertia);
    const double denominator = Fmadds(geared, ratio, axle_inertia);
    double x = Fnmsubs(t0, engine_inertia, torque);
    x = Fmadds(x, ratio, reaction);
    x = Fdivs(x, denominator);
    const double clutched = Fmuls(x, clutch);
    engine_spin = Fmadds(x, ratio, t0);
    VehicleUpdateDrivenAxle(base, v, drive == 0 ? 0 : 2, Fmadds(per_axle, slip, clutched), f);
  }
  // The engine's momentum +2208 and rpm +2228 (clamped to idle +360 and
  // the limit +16), +2220, +2212, +2224.
  const double free_spin = Fmuls(engine_spin, clutch);
  double spin = Fmadds(Fdivs(torque, engine_inertia), slip, free_spin);
  spin = Fmadds(spin, double(schema::Vehicle(base, v).substep_dt()), double(schema::Vehicle(base, v).unknown_2212()));
  schema::Vehicle(base, v).set_engine_momentum(float(Fmuls(spin, engine_inertia)));
  const double per_rpm = Lfs(base, data_view.max_rpm_address());
  const double low = Fmuls(per_rpm, Lfs(base, data_view.idle_rpm_address()));
  const double scale = Lfs(base, 0x820029A8);
  const double high = Fmuls(per_rpm, scale);
  const double low_spin = Fmuls(low, scale);
  if (spin < low_spin) {
    spin = low_spin;
  } else if (high < spin) {
    spin = high;
  }
  double rpm = Fmuls(Fdivs(spin, Lfs(base, data_view.max_rpm_address())), Lfs(base, 0x820029A4));
  schema::Vehicle(base, v).set_rpm(float(rpm));
  const double idle = Lfs(base, data_view.idle_rpm_address()), limit = Lfs(base, data_view.rev_limit_normalized_address());
  if (rpm < idle) {
    rpm = idle;
  } else if (limit < rpm) {
    rpm = limit;
  }
  schema::Vehicle(base, v).set_rpm(float(rpm));
  const double engine = Fmuls(Lfs(base, data_view.max_rpm_address()), rpm);
  schema::Vehicle(base, v).set_unknown_2220(float(engine));
  const double old = double(schema::Vehicle(base, v).unknown_2224());
  const double next_momentum = Fmuls(engine, scale);
  const double smoothed = Fmadds(double(schema::Vehicle(base, v).unknown_2220()), damping, old);
  schema::Vehicle(base, v).set_unknown_2212(float(next_momentum));
  const uint64_t bits = (uint64_t(guest::LoadU32(base, 0x82002A58)) << 32) |
                        guest::LoadU32(base, 0x82002A5C);
  schema::Vehicle(base, v).set_unknown_2224(float(Fmuls(smoothed, std::bit_cast<double>(bits))));
}

namespace {

// An integer through the stack as the original converts it (extsw, std at
// `at`, lfd, fcfid, frsp): the integer's float.
double StackIntegerFloat(uint8_t* base, uint32_t at, int64_t value) {
  guest::StoreU32(base, at, uint32_t(uint64_t(value) >> 32));
  guest::StoreU32(base, at + 4, uint32_t(value));
  return double(float(double(value)));
}

}  // namespace

void VehicleUpdateGearbox(uint8_t* base, uint32_t vehicle, uint32_t manual, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B4CF0 Vehicle::UpdateGearbox");
  // The original's control flow, label for label (its addresses), with its
  // registers as locals: the branches are many and share values.
  const uint32_t v = vehicle, f = frame;
  const schema::CarData data_view(base, CarData(base, v));  // the gearbox (r6)
  const uint32_t e = data_view.gear_count_address();
  const uint32_t r4 = manual & 0xFF;
  double f0 = 0.0, f5 = 0.0, f6 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0,
         f13 = 0.0;
  int32_t r7 = 0;
  uint32_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  schema::Vehicle(base, v).set_unknown_2281(uint8_t(0));
  schema::Vehicle(base, v).set_unknown_2282(uint8_t(0));
  const double f4 = Lfs(base, kOne);
  const double f7 = Lfs(base, kZero);
  if (int32_t(data_view.gear_count()) == 1) {
    // One gear (probably a CVT): +2288 between e +300 and e +328.
    if (schema::Vehicle(base, v).gear() <= 0) {
      f0 = Lfs(base, data_view.gear_ratios_copy_address() + 4);
      goto L4EA4;
    }
    f13 = double(schema::Vehicle(base, v).throttle());
    if (f13 == f7) {
      f12 = double(schema::Vehicle(base, v).unknown_2220());
      f0 = Lfs(base, 0x82002A84);
      if (f12 < f0) {
        f0 = Lfs(base, data_view.gear_ratios_copy_address() + 4);
        f12 = f0;
        goto L4D84;
      }
    }
    f0 = Lfs(base, data_view.gear_ratios_copy_address() + 4);
    f13 = Fsubs(f4, f13);
    f12 = Lfs(base, data_view.gear_ratios_copy_address() + 32);
    f12 = Fsubs(f12, f0);
    f12 = Fmadds(f12, f13, f0);
  L4D84:
    f10 = Lfs(base, data_view.gear_ratios_copy_address() + 32);
    if (f12 < f10) {
      f0 = f10;
    } else if (!(f0 < f12)) {
      f0 = f12;
    }
    f11 = double(schema::Vehicle(base, v).driven_axle_spin());
    f13 = f0;
    if (f11 > f4) {
      f9 = Lfs(base, data_view.rev_limit_address());
      f8 = Lfs(base, data_view.final_drive_copy_address());
      f0 = Fmuls(f9, Lfs(base, 0x82002A80));
      f9 = Fmuls(f8, f11);
      f0 = Fdivs(f0, f9);
      if (!(f0 > f12) && !(f0 < f10)) {
        if (f12 < f0) {
          f0 = f12;
        }
        f13 = f0;
      }
    }
    f0 = Fmuls(Lfs(base, data_view.final_drive_copy_address()), f11);
    f12 = Lfs(base, data_view.idle_rpm_address());
    f11 = Lfs(base, data_view.max_rpm_address());
    f0 = Fabs(f0);
    f0 = Fdivs(f12, f0);
    f12 = Fmuls(f0, f11);
    f0 = Fmuls(f12, Lfs(base, 0x82002A7C));
    r11 = f0 > f13 ? 1 : 0;
    f0 = StackIntegerFloat(base, f + 80, int32_t(r11));
    if (!(f0 == f7)) {
      f13 = f0;
    }
    f0 = double(schema::Vehicle(base, v).cvt_ratio());
    if (f0 > f7) {
      f12 = Lfs(base, 0x82002A78);
      f11 = Lfs(base, 0x82002A74);
    } else {
      f12 = Lfs(base, 0x82002A74);
      f11 = Lfs(base, 0x82002A78);
    }
    f12 = Fmuls(f0, f12);
    f0 = Fmuls(f0, f11);
    if (f13 < f12) {
      f0 = f12;
    } else if (!(f0 < f13)) {
      f0 = f13;
    }
  L4EA4:
    // The gear values, as Vehicle::SetGearValues sets them.
    f13 = f0;
    schema::Vehicle(base, v).set_cvt_ratio(float(f0));
    f11 = Lfs(base, data_view.final_drive_copy_address());
    f12 = double(schema::Vehicle(base, v).axle_radius());
    f0 = double(schema::Vehicle(base, v).unknown_2328());
    CopyWord(base, schema::Vehicle(base, v).unknown_2328_address(), schema::Vehicle(base, v).unknown_2324_address());
    schema::Vehicle(base, v).set_unknown_2312(float(Fadds(f12, f0)));
    schema::Vehicle(base, v).set_unknown_2320(float(f7));
    schema::Vehicle(base, v).set_total_ratio(float(Fabs(Fmuls(f11, f13))));
    schema::Vehicle(base, v).set_gear_value(float(Fmuls(Lfs(base, data_view.unknown_1544_address()), Lfs(base, data_view.gear_table_3_address() + 4))));
  }
  if (schema::Vehicle(base, v).forced_rest() != 0) {
    schema::Vehicle(base, v).set_clutch(float(f7));
    schema::Vehicle(base, v).set_gear(int32_t(0));
    goto L5560;
  }
  f5 = double(schema::Vehicle(base, v).reverse_throttle());
  if (!(f5 == f7) && schema::Vehicle(base, v).gear() == -1 &&
      double(schema::Vehicle(base, v).clutch()) == f4) {
    goto L5560;
  }
  f6 = double(schema::Vehicle(base, v).brake());
  if (!(f6 == f7) && !(double(schema::Vehicle(base, v).throttle()) == f7) &&
      schema::Vehicle(base, v).gear() == 0) {
    goto L4F44;
  }
  // The shift timer +2268.
  f0 = double(schema::Vehicle(base, v).shift_timer());
  if (f0 > f7) {
    f0 = Fsubs(f0, double(schema::Vehicle(base, v).dt()));
    f12 = double(schema::Vehicle(base, v).unknown_2220());
    schema::Vehicle(base, v).set_shift_timer(float(f0));
    f13 = Lfs(base, 0x82001FD4);
    if (f12 < f13 && double(schema::Vehicle(base, v).throttle()) == f7 && f5 == f7 &&
        uint32_t(schema::Vehicle(base, v).gear_previous()) == 0) {
      schema::Vehicle(base, v).set_shift_timer(float(f7));
      schema::Vehicle(base, v).set_unknown_2276(uint8_t(0));
      schema::Vehicle(base, v).set_unknown_2277(uint8_t(0));
      schema::Vehicle(base, v).set_gear(int32_t(0));
      goto L555C;
    }
    if (f0 > f7) {
      // Still shifting.
      r10 = uint32_t(schema::Vehicle(base, v).gear_previous());
      if (r10 == 0) {
        schema::Vehicle(base, v).set_clutch(float(Fnmsubs(Lfs(base, data_view.shift_rates_address()), f0, f4)));
        goto L5560;
      }
      r11 = uint32_t(schema::Vehicle(base, v).gear());
      if (int32_t(r10) > int32_t(r11)) {
        f0 = Fmuls(Lfs(base, data_view.rev_limit_address()), Lfs(base, 0x82002A6C));
        if (!(double(schema::Vehicle(base, v).unknown_2212()) > f0)) {
          goto L5560;
        }
        goto L503C;
      }
      if (!(int32_t(r10) < int32_t(r11))) {
        goto L4FC4;
      }
      f0 = Lfs(base, data_view.final_drive_copy_address());
      f13 = double(schema::Vehicle(base, v).driven_axle_spin());
      f12 = double(schema::Vehicle(base, v).unknown_2212());
      f11 = Lfs(base, e + 4 * (r11 + 74));
      f0 = Fmuls(f13, Fabs(Fmuls(f11, f0)));
      if (!(f12 < f0)) {
        goto L5560;
      }
    L503C:
      schema::Vehicle(base, v).set_shift_timer(float(f7));
      goto L555C;
    }
    schema::Vehicle(base, v).set_shift_timer(float(f7));
    schema::Vehicle(base, v).set_unknown_2276(uint8_t(0));
    schema::Vehicle(base, v).set_clutch(float(f4));
    schema::Vehicle(base, v).set_unknown_2277(uint8_t(0));
  }
L4FC4:
  // The shift points: up at f9 ([+2632 + 4 gear]), down below f11.
  r7 = schema::Vehicle(base, v).gear();
  f9 = f7;
  f11 = Lfs(base, 0x82002A70);
  if (r7 > 0) {
    f9 = Lfs(base, v + 4 * uint32_t(r7 + 658));
    f13 = Lfs(base, v + 4 * uint32_t(r7 + 657));
    f11 = Fmuls(f13, Lfs(base, 0x82001070));
  }
  // The driven wheels' mean spin (f12) and mean slip (f0, of those whose
  // +268 isn't the float at 0x82001BE0).
  r5 = schema::Vehicle(base, v).drive_type();
  r8 = 0;
  f8 = Lfs(base, 0x82001BE4);
  if (r5 < 1) {
    f13 = Fadds(double(schema::VehicleWheel(base, Wheel(v, 1)).spin()), double(schema::VehicleWheel(base, Wheel(v, 0)).spin()));
    f0 = f7;
    f10 = Lfs(base, 0x82001BE0);
    f12 = Fmuls(f13, f8);
    for (uint32_t w = 0; w < 2; w++) {
      f13 = double(schema::VehicleWheel(base, Wheel(v, w)).slip_long());
      if (!(f13 == f10)) {
        f0 = Fadds(f13, f0);
        r8++;
      }
    }
    if (r8 == 0) {
      goto L51F8;
    }
    f0 = Fdivs(f0, StackIntegerFloat(base, f + 80, int32_t(r8)));
  } else if (r5 == 1) {
    f13 = Fadds(double(schema::VehicleWheel(base, Wheel(v, 3)).spin()), double(schema::VehicleWheel(base, Wheel(v, 2)).spin()));
    f0 = f7;
    f10 = Lfs(base, 0x82001BE0);
    f12 = Fmuls(f13, f8);
    for (uint32_t w = 2; w < 4; w++) {
      f13 = double(schema::VehicleWheel(base, Wheel(v, w)).slip_long());
      if (!(f13 == f10)) {
        f0 = Fadds(f13, f0);
        r8++;
      }
    }
    if (r8 == 0) {
      goto L51F8;
    }
    f0 = Fdivs(f0, StackIntegerFloat(base, f + 80, int32_t(r8)));
  } else {
    f12 = double(schema::VehicleWheel(base, Wheel(v, 2)).spin());
    f0 = Fadds(double(schema::VehicleWheel(base, Wheel(v, 3)).spin()), f12);
    f12 = double(schema::VehicleWheel(base, Wheel(v, 1)).spin());
    f10 = double(schema::VehicleWheel(base, Wheel(v, 0)).spin());
    f13 = f7;
    f0 = Fadds(f0, f12);
    f12 = Fadds(f0, f10);
    f0 = Lfs(base, 0x82002968);
    f10 = Lfs(base, 0x82001BE0);
    f12 = Fmuls(f12, f0);
    for (uint32_t w = 0; w < 4; w++) {
      f0 = double(schema::VehicleWheel(base, Wheel(v, w)).slip_long());
      if (!(f0 == f10)) {
        f13 = Fadds(f0, f13);
        r8++;
      }
    }
    if (r8 == 0) {
      goto L51F8;
    }
    f0 = Fdivs(f13, StackIntegerFloat(base, f + 80, int32_t(r8)));
  }
  goto L51FC;
L51F8:
  f0 = f4;
L51FC:
  // The speed's own shifts (with r4 clear): r9 up, r10 down.
  r9 = 0;
  r10 = 0;
  r8 = 0;
  if (r7 > 0 && schema::Vehicle(base, v).unknown_2286() == 0) {
    r10 = r4;
    r11 = 0;
    if (r10 == 0 && f12 > f9 && double(schema::Vehicle(base, v).clutch()) == f4 && f0 < Lfs(base, 0x82001FBC)) {
      r11 = 1;
    }
    r9 = r11;
    r11 = 0;
    if (r10 == 0 && f12 < f11 && double(schema::Vehicle(base, v).clutch()) == f4) {
      r11 = 1;
    }
    r10 = r11;
  }
  // The clutch: out while braking near idle on a request, else ramped in.
  f12 = Lfs(base, 0x82002204);
  if (!(f6 == f7) && r7 > 1 &&
      !(double(schema::Vehicle(base, v).rpm()) > Fmuls(Lfs(base, data_view.idle_rpm_address()), Lfs(base, 0x820021E8))) &&
      (r4 != 0 || schema::Vehicle(base, v).unknown_2286() != 0)) {
    schema::Vehicle(base, v).set_clutch(float(f7));
  } else {
    f0 = Fadds(double(schema::Vehicle(base, v).clutch()), f12);
    schema::Vehicle(base, v).set_clutch(float(f0));
    if (!(f0 < f4)) {
      schema::Vehicle(base, v).set_clutch(float(f4));
    }
  }
  if (r9 != 0 || (schema::Vehicle(base, v).shift_up_request() != 0 && r7 > 0)) {
    goto L5438;
  }
  if (r10 != 0 || (schema::Vehicle(base, v).shift_down_request() != 0 && r7 > 1)) {
    goto L5430;
  }
  if (!(f6 == f7) && !(double(schema::Vehicle(base, v).throttle()) == f7)) {
    if (r7 != 0) {
      goto L5390;
    }
    schema::Vehicle(base, v).set_clutch(float(f7));
    goto L5460;
  }
  if (r7 != 0 || double(schema::Vehicle(base, v).throttle()) == f7) {
    goto L5390;
  }
  // Pulling away from neutral.
  Stfs(base, schema::Vehicle(base, v).unknown_2260_address(), f7);
  schema::Vehicle(base, v).set_gear_previous(int32_t(0));
  schema::Vehicle(base, v).set_gear(int32_t(1));
  f13 = Fmuls(Lfs(base, data_view.rev_limit_normalized_address()), f8);
  r8 = 1;
  if (double(schema::Vehicle(base, v).rpm()) > f13) {
    schema::Vehicle(base, v).set_unknown_2277(uint8_t(1));
    schema::Vehicle(base, v).set_unknown_2276(uint8_t(0));
  } else {
    schema::Vehicle(base, v).set_unknown_2277(uint8_t(0));
    schema::Vehicle(base, v).set_unknown_2276(uint8_t(1));
  }
  goto L5464;
L5390:
  // Reverse, neutral.
  f13 = Lfs(base, schema::Vehicle(base, v).ground_velocity_address());
  if ((f13 < f4 || r7 == 0) && !(f5 == f7)) {
    if (r7 == -1) {
      goto L5464;
    }
    schema::Vehicle(base, v).set_gear(-1);
    goto L5458;
  }
  if (double(schema::Vehicle(base, v).clutch()) == f4 && r7 != 0) {
    f0 = Fmuls(Lfs(base, data_view.idle_rpm_address()), Lfs(base, 0x820027A8));
    if (double(schema::Vehicle(base, v).rpm()) < f0 &&
        (double(schema::Vehicle(base, v).throttle()) == f7 || (r7 == -1 && f5 == f7))) {
      schema::Vehicle(base, v).set_gear(int32_t(0));
      goto L5458;
    }
  }
  if (!(f13 < f10) || r7 != 1) {
    goto L5464;
  }
  schema::Vehicle(base, v).set_gear_previous(int32_t(1));
  schema::Vehicle(base, v).set_gear(int32_t(0));
  goto L545C;
L5430:
  r10 = uint32_t(r7 - 1);
  goto L5448;
L5438:
  if (!(r7 < int32_t(data_view.gear_count()))) {
    goto L5464;
  }
  r10 = uint32_t(r7 + 1);
L5448:
  schema::Vehicle(base, v).set_gear(int32_t(r10));
  CopyWord(base, 0x82002208, schema::Vehicle(base, v).unknown_2272_address());
L5458:
  schema::Vehicle(base, v).set_gear_previous(int32_t(uint32_t(r7)));
L545C:
  r8 = 1;
L5460:
  Stfs(base, schema::Vehicle(base, v).unknown_2260_address(), f7);
L5464:
  if (r8 == 1) {
    // A shift: the timer and the car data's rate, then the gear values.
    const uint32_t up = schema::Vehicle(base, v).unknown_2276();
    if (up != 0 || schema::Vehicle(base, v).unknown_2277() != 0) {
      if (up != 0 && uint32_t(schema::Vehicle(base, v).gear_previous()) == 0) {
        f0 = Lfs(base, 0x8200296C);
      } else {
        f0 = Lfs(base, data_view.shift_times_address());
      }
      schema::Vehicle(base, v).set_shift_timer(float(f0));
      Stfs(base, data_view.shift_rates_address(), Fdivs(f4, f0));
    } else {
      const uint32_t gear = uint32_t(schema::Vehicle(base, v).gear());
      schema::Vehicle(base, v).set_clutch(float(f7));
      if (gear == 0) {
        schema::Vehicle(base, v).set_shift_timer(float(f7));
      } else {
        const uint32_t old = uint32_t(schema::Vehicle(base, v).gear_previous());
        f0 = Lfs(base, e + 4 * (old + 91));
        CopyWord(base, e + 4 * (old + 91), schema::Vehicle(base, v).shift_timer_address());
        Stfs(base, e + 4 * (old + 100), Fdivs(f4, f0));
      }
    }
    VehicleSetGearValues(base, v);
    schema::Vehicle(base, v).set_unknown_2281(uint8_t(1));
    schema::Vehicle(base, v).set_unknown_2282(uint8_t(1));
    goto L5560;
  }
  if (r5 == 0 || schema::Vehicle(base, v).gear() <= 0) {
    goto L5560;
  }
  if (double(schema::Vehicle(base, v).handbrake()) > f7 && double(schema::Vehicle(base, v).throttle()) == f7) {
    goto L4F44;
  }
  f0 = Fadds(double(schema::Vehicle(base, v).clutch()), f12);
  schema::Vehicle(base, v).set_clutch(float(f0));
  if (f0 < f4) {
    goto L5560;
  }
L555C:
  schema::Vehicle(base, v).set_clutch(float(f4));
  goto L5560;
L4F44:
  schema::Vehicle(base, v).set_clutch(float(f7));
L5560:
  schema::Vehicle(base, v).set_shift_down_request(uint8_t(0));
  schema::Vehicle(base, v).set_shift_up_request(uint8_t(0));
  schema::Vehicle(base, v).set_unknown_2286(uint8_t(0));
}

void VehicleShiftGears(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6430 Vehicle::ShiftGears");
  const uint32_t v = vehicle;
  uint32_t manual = 0;
  if (int32_t(schema::CarData(base, CarData(base, v)).gear_count()) != 1 &&
      schema::Vehicle(base, v).automatic() == 0) {
    manual = 1;
  }
  VehicleUpdateGearbox(base, v, manual, frame - 112);
  const double sign = schema::Vehicle(base, v).gear() == -1 ? Lfs(base, 0x82001BE0)
                                                                     : Lfs(base, kOne);
  schema::Vehicle(base, v).set_direction(float(sign));
}

}  // namespace car_physics
