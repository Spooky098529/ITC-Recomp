// The replay snapshots' record parts (src/replay_snapshots/replay_snapshots.h,
// docs/functions/replay-snapshots.md): each original's stores and calls in
// its order, read from generated/ field by field.

#include "replay_snapshots/replay_snapshots.h"

#include "base/log.h"
#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/car_physics.h"
#include "schema/collision.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/replay_snapshots.h"
#include "schema/type10_mode.h"
#include "schema/vehicle_accessors.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace replay_snapshots {

namespace {

using Record = schema::ReplayRecord;
using Body = schema::ReplayBodyState;
using Physics = schema::ReplayVehiclePhysics;
using Collision = schema::ReplayCollisionState;
using Shape = schema::ReplayShapeState;
using Snapshot = schema::ReplaySnapshot;
using G = schema::ReplaySnapshotsGlobals;

void Byte(uint32_t to, uint32_t from) {
  guest::StoreU8(to, guest::LoadU8(from));
}
void Word(uint32_t to, uint32_t from) {
  guest::StoreU32(to, guest::LoadU32(from));
}
// An ld then std pair: the eight bytes read, then written.
void Dword(uint32_t to, uint32_t from) {
  const uint32_t hi = guest::LoadU32(from), lo = guest::LoadU32(from + 4);
  guest::StoreU32(to, hi);
  guest::StoreU32(to + 4, lo);
}
// An lfs then stfs pair: the word's bits.
void FloatWord(uint32_t to, uint32_t from) {
  car_physics::ops::CopyWord(from, to);
}
// lhz then stw, and lwz then sth (the word's low halfword).
void HalfToWord(uint32_t to, uint32_t from) {
  guest::StoreU32(to, guest::LoadU16(from));
}
void WordToHalf(uint32_t to, uint32_t from) {
  guest::StoreU16(to, uint16_t(guest::LoadU32(from)));
}
void Vec4(uint32_t to, uint32_t from) {
  vector_library::Vec4Copy(to, from);
}
void Matrix(uint32_t to, uint32_t from) {
  car_physics::MatrixCopyRows(to, from);
}
void Wheel(uint32_t to, uint32_t from) {
  car_physics::WheelCopy(to, from);
}
void WheelRecord2(uint32_t to, uint32_t from) {
  car_physics::WheelRecord2Copy(to, from);
}
void TrackPosition(uint32_t to, uint32_t from) {
  vehicle::TrackPositionCopy(to, from);
}
// memcpy (sub_825A7410): its plain C++ (the adapters check
// native::Buffers_NativeMemcpy first), or the same bytes one by one where
// it refuses (ranges that may share memory: never for a record and a
// vehicle).
void Memcpy(uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (native::Buffers_Memcpy(to, from, bytes, caller)) {
    return;
  }
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(to + i, guest::LoadU8(from + i));
  }
}

}  // namespace

// --- The restores ---

void Record5232RestoreTrack(uint32_t record, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_82104310 Record5232::RestoreTrack");
  const schema::Vehicle to(vehicle);
  const Record r(record);
  Word(to.car_flags_address(), r.car_flags_address());
  Word(to.collision_flags_address(), r.collision_flags_address());
  Memcpy(to.status_address(), r.status_address(), 112, 0x82104344);
  for (uint32_t i = 0; i < 5; i++) {
    TrackPosition(to.own_track_position_address() + 96 * i,
        r.track_positions_address() + 96 * i);
  }
}

void BodyStateRestore(uint32_t state, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_821031A0 BodyState::Restore");
  const schema::RigidBody to(body);
  const Body s(state);
  Byte(to.fixed_address(), s.fixed_address());
  FloatWord(to.mass_address(), s.mass_address());
  FloatWord(to.inverse_mass_address(), s.inverse_mass_address());
  Vec4(to.center_of_mass_address(), s.center_of_mass_address());
  Vec4(to.velocity_address(), s.velocity_address());
  FloatWord(to.speed_cap_address(), s.speed_cap_address());
  Vec4(to.force_address(), s.force_address());
  Dword(to.orientation_address(), s.orientation_address());
  Dword(to.orientation_address() + 8, s.orientation_address() + 8);
  Vec4(to.angular_velocity_address(), s.angular_velocity_address());
  FloatWord(to.spin_cap_address(), s.spin_cap_address());
  Vec4(to.torque_address(), s.torque_address());
  Vec4(to.velocity_direction_address(), s.velocity_direction_address());
  FloatWord(to.speed_address(), s.speed_address());
  FloatWord(to.linear_damping_address(), s.linear_damping_address());
  Vec4(to.momentum_address(), s.momentum_address());
  Vec4(to.center_of_mass_offset_address(), s.center_of_mass_offset_address());
  Vec4(to.origin_address(), s.origin_address());
  Matrix(to.orientation_matrix_address(), s.orientation_matrix_address());
  Matrix(to.matrix_address(), s.matrix_address());
  Vec4(to.inertia_address(), s.inertia_address());
  Vec4(to.inverse_inertia_address(), s.inverse_inertia_address());
  FloatWord(to.angular_damping_address(), s.angular_damping_address());
  Vec4(to.angular_momentum_address(), s.angular_momentum_address());
  Matrix(to.world_inverse_inertia_address(), s.world_inverse_inertia_address());
  Vec4(to.gravity_address(), s.gravity_address());
  Vec4(to.weight_address(), s.weight_address());
  FloatWord(to.effective_mass_address(), s.effective_mass_address());
}

void Member1136Restore(uint32_t part, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_82102C60 Member1136::Restore");
  const schema::Vehicle to(vehicle);
  const Physics p(part);
  for (uint32_t i = 0; i < 3; i++) {
    Vec4(to.body_row0_address() + 16 * i, p.body_row0_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(to.speed_address() + 4 * i, p.speed_address() + 4 * i);
  }
  Vec4(to.angular_velocity_address(), p.angular_velocity_address());
  for (uint32_t i = 0; i < 4; i++) {
    FloatWord(to.unknown_80_address() + 4 * i, p.unknown_80_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Vec4(to.ground_up_address() + 16 * i, p.ground_up_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Matrix(to.ground_matrix_address() + 64 * i, p.ground_matrix_address() + 64 * i);
  }
  Byte(to.speed_sensitive_steering_address(), p.speed_sensitive_steering_address());
  for (uint32_t i = 0; i < 14; i++) {
    FloatWord(to.steering_input_address() + 4 * i, p.steering_input_address() + 4 * i);
  }
  Word(to.wheels_on_ground_address(), p.wheels_on_ground_address());
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(to.unknown_352_address() + 16 * i, p.unknown_336_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Wheel(to.wheels_address() + 368 * i, p.wheel_0_address() + 368 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    WheelRecord2(to.front_axle_address() + 128 * i, p.front_axle_address() + 128 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Word(to.wheels_by_load_address() + 4 * i, p.wheel_by_load_0_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(to.driven_axle_spin_address() + 4 * i, p.driven_axle_spin_address() + 4 * i);
  }
  Memcpy(to.unknown_2204_address(), p.drivetrain_block_address(), 144, 0x82102E4C);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(to.direction_address() + 4 * i, p.direction_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Word(to.substep_count_address() + 4 * i, p.substep_count_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 8; i++) {
    Byte(to.unknown_2380_address() + i, p.unknown_2332_address() + i);
  }
  FloatWord(to.value2388_address(), p.value2388_address());
  Word(to.traction_counter_address(), p.traction_counter_address());
  FloatWord(to.traction_limit_address(), p.traction_limit_address());
  for (uint32_t i = 0; i < 2; i++) {
    Byte(to.traction_active_address() + i, p.traction_active_address() + i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(to.drag_scale_address() + 4 * i, p.drag_scale_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(to.oil_factor_address() + 4 * i, p.oil_factor_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(to.nitrous_factor_address() + 4 * i, p.nitrous_factor_address() + 4 * i);
  }
  Byte(to.nitrous_on_address(), p.nitrous_on_address());
  FloatWord(to.speed_squared_address(), p.speed_squared_address());
  for (uint32_t i = 0; i < 2; i++) {
    Byte(to.unknown_2444_address() + i, p.unknown_2396_address() + i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(to.unknown_2448_address() + 4 * i, p.unknown_2400_address() + 4 * i);
  }
  Byte(to.unknown_2456_address(), p.unknown_2408_address());
  for (uint32_t i = 0; i < 2; i++) {
    Vec4(to.body_force_address() + 16 * i, p.body_force_address() + 16 * i);
  }
  Word(to.tire_wear_mode_address(), p.tire_wear_mode_address());
  FloatWord(to.backfire_charge_address(), p.backfire_charge_address());
  Byte(to.backfire_address(), p.backfire_address());
  FloatWord(to.backfire_strength_address(), p.backfire_strength_address());
  Byte(to.unknown_2512_address(), p.unknown_2464_address());
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(to.throttle_used_address() + 4 * i, p.throttle_used_address() + 4 * i);
  }
  Word(to.unknown_2524_address(), p.unknown_2476_address());
  for (uint32_t i = 0; i < 6; i++) {
    Vec4(to.axis_address() + 16 * i, p.axis_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Byte(to.unknown_2624_address() + i, p.unknown_2576_address() + i);
  }
  for (uint32_t i = 0; i < 9; i++) {
    FloatWord(to.unknown_2628_address() + 4 * i, p.unknown_2580_address() + 4 * i);
  }
  Byte(to.surfaces_on_address(), p.surfaces_on_address());
  FloatWord(to.unknown_2924_address(), p.unknown_2620_address());
  Byte(to.at_rest_address(), p.at_rest_address());
  for (uint32_t i = 0; i < 4; i++) {
    FloatWord(to.unknown_2932_address() + 4 * i, p.unknown_2628_address() + 4 * i);
  }
  Byte(to.wastegate_open_address(), p.wastegate_open_address());
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(to.wastegate_address() + 4 * i, p.wastegate_address() + 4 * i);
  }
  Byte(to.blow_off_open_address(), p.blow_off_open_address());
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(to.blow_off_address() + 4 * i, p.blow_off_address() + 4 * i);
  }
  Word(to.track_position_address(), p.track_position_address());
  for (uint32_t i = 0; i < 2; i++) {
    Memcpy(to.steer_record_address() + 60 * i,
        p.steer_block_a_address() + 60 * i, 60, 0x821030D8);
  }
  for (uint32_t i = 0; i < 6; i++) {
    FloatWord(to.steering_result_address() + 4 * i, p.steering_result_address() + 4 * i);
  }
  Byte(to.upright_address(), p.upright_address());
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(to.unknown_3136_address() + 4 * i, p.unknown_2832_address() + 4 * i);
  }
  Word(to.step_count_address(), p.step_count_address());
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(to.unknown_3168_address() + 16 * i, p.unknown_2864_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(to.brake_fade_address() + 4 * i, p.brake_fade_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Byte(to.backfire_latch_address() + i, p.backfire_latch_address() + i);
  }
}

void Part464Restore(uint32_t part, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82102AA8 Part464::Restore");
  const schema::BoxShape to(shape);
  const Shape p(part);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(to.car_float_16_address() + 4 * i, p.car_float_16_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(to.center_address() + 16 * i, p.center_address() + 16 * i);
  }
  WordToHalf(to.unknown_112_address(), p.unknown_448_address());
  Vec4(to.half_extent_address(), p.half_extent_address());
  Vec4(to.inverse_half_extent_address(), p.inverse_half_extent_address());
  for (uint32_t i = 0; i < 8; i++) {
    Vec4(to.corners_address() + 16 * i, p.corners_address() + 16 * i);
  }
  FloatWord(to.face_px_distance_address(), p.face_px_distance_address());
  Vec4(to.face_px_normal_address(), p.face_px_normal_address());
  FloatWord(to.face_py_distance_address(), p.face_py_distance_address());
  Vec4(to.face_py_normal_address(), p.face_py_normal_address());
  FloatWord(to.face_pz_distance_address(), p.face_pz_distance_address());
  Vec4(to.face_pz_normal_address(), p.face_pz_normal_address());
  FloatWord(to.face_nx_distance_address(), p.face_nx_distance_address());
  Vec4(to.face_nx_normal_address(), p.face_nx_normal_address());
  FloatWord(to.face_ny_distance_address(), p.face_ny_distance_address());
  Vec4(to.face_ny_normal_address(), p.face_ny_normal_address());
  FloatWord(to.face_nz_distance_address(), p.face_nz_distance_address());
  Vec4(to.face_nz_normal_address(), p.face_nz_normal_address());
}

uint32_t Member4080Restore(uint32_t part, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82102B98 Member4080::Restore");
  const Collision c(part);
  if (c.has_object() == 0 || object == 0) {
    return part;
  }
  const schema::CollisionObject to(object);
  Vec4(to.address + 48, c.box_min_address());
  Vec4(to.address + 64, c.box_max_address());
  Matrix(to.address + 80, c.motion_address());
  // The shape (+20) and the second placed copy (+40) are read before the
  // stores, the first copy (+36) after them, as the original.
  const uint32_t shape = to.shape().address;
  const uint32_t second = to.shape_current();
  FloatWord(to.lowest_travel_address(), c.lowest_travel_address());
  const uint32_t first = to.shape_previous();
  to.set_shape_template(shape);
  Part464Restore(c.shape_previous_address(), first);
  Part464Restore(c.shape_current_address(), second);
  Vec4(to.push_x_address(), c.push_address());
  Vec4(to.pair_torque_x_address(), c.pair_torque_address());
  Word(to.push_count_address(), c.push_count_address());
  Vec4(to.push_scale_x_address(), c.push_scale_address());
  FloatWord(to.torque_scale_address(), c.torque_scale_address());
  Word(to.moved_address(), c.moved_address());
  Vec4(to.push_out_x_address(), c.push_out_address());
  return to.push_out_x_address();
}

uint32_t Record5232Restore(uint32_t record, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_821044D8 Record5232::Restore");
  const Record r(record);
  Record5232RestoreTrack(record, vehicle);
  BodyStateRestore(r.body_address(), schema::Vehicle(vehicle).body().address);
  Member1136Restore(r.physics_address(), vehicle);
  return Member4080Restore(r.collision_address(),
                          schema::Vehicle(vehicle).collision_object());
}

// --- The saves ---

void Record5232SaveTrack(uint32_t record, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_82104380 Record5232::SaveTrack");
  const schema::Vehicle from(vehicle);
  const Record r(record);
  Word(r.car_flags_address(), from.car_flags_address());
  Word(r.collision_flags_address(), from.collision_flags_address());
  Memcpy(r.status_address(), from.status_address(), 112, 0x821043B4);
  for (uint32_t i = 0; i < 5; i++) {
    TrackPosition(r.track_positions_address() + 96 * i,
        from.own_track_position_address() + 96 * i);
  }
}

void BodyStateSave(uint32_t state, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_821039F0 BodyState::Save");
  const schema::RigidBody from(body);
  const Body s(state);
  Byte(s.fixed_address(), from.fixed_address());
  FloatWord(s.mass_address(), from.mass_address());
  FloatWord(s.inverse_mass_address(), from.inverse_mass_address());
  Vec4(s.center_of_mass_address(), from.center_of_mass_address());
  Vec4(s.velocity_address(), from.velocity_address());
  FloatWord(s.speed_cap_address(), from.speed_cap_address());
  Vec4(s.force_address(), from.force_address());
  Dword(s.orientation_address(), from.orientation_address());
  Dword(s.orientation_address() + 8, from.orientation_address() + 8);
  Vec4(s.angular_velocity_address(), from.angular_velocity_address());
  FloatWord(s.spin_cap_address(), from.spin_cap_address());
  Vec4(s.torque_address(), from.torque_address());
  Vec4(s.velocity_direction_address(), from.velocity_direction_address());
  FloatWord(s.speed_address(), from.speed_address());
  FloatWord(s.linear_damping_address(), from.linear_damping_address());
  Vec4(s.momentum_address(), from.momentum_address());
  Vec4(s.center_of_mass_offset_address(), from.center_of_mass_offset_address());
  Vec4(s.origin_address(), from.origin_address());
  Matrix(s.orientation_matrix_address(), from.orientation_matrix_address());
  Matrix(s.matrix_address(), from.matrix_address());
  Vec4(s.inertia_address(), from.inertia_address());
  Vec4(s.inverse_inertia_address(), from.inverse_inertia_address());
  FloatWord(s.angular_damping_address(), from.angular_damping_address());
  Vec4(s.angular_momentum_address(), from.angular_momentum_address());
  Matrix(s.world_inverse_inertia_address(), from.world_inverse_inertia_address());
  Vec4(s.gravity_address(), from.gravity_address());
  Vec4(s.weight_address(), from.weight_address());
  FloatWord(s.effective_mass_address(), from.effective_mass_address());
}

void Member1136Save(uint32_t part, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_821034B8 Member1136::Save");
  const schema::Vehicle from(vehicle);
  const Physics p(part);
  for (uint32_t i = 0; i < 3; i++) {
    Vec4(p.body_row0_address() + 16 * i, from.body_row0_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(p.speed_address() + 4 * i, from.speed_address() + 4 * i);
  }
  Vec4(p.angular_velocity_address(), from.angular_velocity_address());
  for (uint32_t i = 0; i < 4; i++) {
    FloatWord(p.unknown_80_address() + 4 * i, from.unknown_80_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Vec4(p.ground_up_address() + 16 * i, from.ground_up_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Matrix(p.ground_matrix_address() + 64 * i, from.ground_matrix_address() + 64 * i);
  }
  Byte(p.speed_sensitive_steering_address(), from.speed_sensitive_steering_address());
  for (uint32_t i = 0; i < 14; i++) {
    FloatWord(p.steering_input_address() + 4 * i, from.steering_input_address() + 4 * i);
  }
  Word(p.wheels_on_ground_address(), from.wheels_on_ground_address());
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(p.unknown_336_address() + 16 * i, from.unknown_352_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Wheel(p.wheel_0_address() + 368 * i, from.wheels_address() + 368 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    WheelRecord2(p.front_axle_address() + 128 * i, from.front_axle_address() + 128 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Word(p.wheel_by_load_0_address() + 4 * i, from.wheels_by_load_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(p.driven_axle_spin_address() + 4 * i, from.driven_axle_spin_address() + 4 * i);
  }
  Memcpy(p.drivetrain_block_address(), from.unknown_2204_address(), 144, 0x8210369C);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(p.direction_address() + 4 * i, from.direction_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Word(p.substep_count_address() + 4 * i, from.substep_count_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 8; i++) {
    Byte(p.unknown_2332_address() + i, from.unknown_2380_address() + i);
  }
  FloatWord(p.value2388_address(), from.value2388_address());
  Word(p.traction_counter_address(), from.traction_counter_address());
  FloatWord(p.traction_limit_address(), from.traction_limit_address());
  for (uint32_t i = 0; i < 2; i++) {
    Byte(p.traction_active_address() + i, from.traction_active_address() + i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(p.drag_scale_address() + 4 * i, from.drag_scale_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(p.oil_factor_address() + 4 * i, from.oil_factor_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(p.nitrous_factor_address() + 4 * i, from.nitrous_factor_address() + 4 * i);
  }
  Byte(p.nitrous_on_address(), from.nitrous_on_address());
  FloatWord(p.speed_squared_address(), from.speed_squared_address());
  for (uint32_t i = 0; i < 2; i++) {
    Byte(p.unknown_2396_address() + i, from.unknown_2444_address() + i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(p.unknown_2400_address() + 4 * i, from.unknown_2448_address() + 4 * i);
  }
  Byte(p.unknown_2408_address(), from.unknown_2456_address());
  for (uint32_t i = 0; i < 2; i++) {
    Vec4(p.body_force_address() + 16 * i, from.body_force_address() + 16 * i);
  }
  Word(p.tire_wear_mode_address(), from.tire_wear_mode_address());
  FloatWord(p.backfire_charge_address(), from.backfire_charge_address());
  Byte(p.backfire_address(), from.backfire_address());
  FloatWord(p.backfire_strength_address(), from.backfire_strength_address());
  Byte(p.unknown_2464_address(), from.unknown_2512_address());
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(p.throttle_used_address() + 4 * i, from.throttle_used_address() + 4 * i);
  }
  Word(p.unknown_2476_address(), from.unknown_2524_address());
  for (uint32_t i = 0; i < 6; i++) {
    Vec4(p.axis_address() + 16 * i, from.axis_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Byte(p.unknown_2576_address() + i, from.unknown_2624_address() + i);
  }
  for (uint32_t i = 0; i < 9; i++) {
    FloatWord(p.unknown_2580_address() + 4 * i, from.unknown_2628_address() + 4 * i);
  }
  Byte(p.surfaces_on_address(), from.surfaces_on_address());
  FloatWord(p.unknown_2620_address(), from.unknown_2924_address());
  Byte(p.at_rest_address(), from.at_rest_address());
  for (uint32_t i = 0; i < 4; i++) {
    FloatWord(p.unknown_2628_address() + 4 * i, from.unknown_2932_address() + 4 * i);
  }
  Byte(p.wastegate_open_address(), from.wastegate_open_address());
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(p.wastegate_address() + 4 * i, from.wastegate_address() + 4 * i);
  }
  Byte(p.blow_off_open_address(), from.blow_off_open_address());
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(p.blow_off_address() + 4 * i, from.blow_off_address() + 4 * i);
  }
  Word(p.track_position_address(), from.track_position_address());
  for (uint32_t i = 0; i < 2; i++) {
    Memcpy(p.steer_block_a_address() + 60 * i,
        from.steer_record_address() + 60 * i, 60, 0x82103928);
  }
  for (uint32_t i = 0; i < 6; i++) {
    FloatWord(p.steering_result_address() + 4 * i, from.steering_result_address() + 4 * i);
  }
  Byte(p.upright_address(), from.upright_address());
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(p.unknown_2832_address() + 4 * i, from.unknown_3136_address() + 4 * i);
  }
  Word(p.step_count_address(), from.step_count_address());
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(p.unknown_2864_address() + 16 * i, from.unknown_3168_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(p.brake_fade_address() + 4 * i, from.brake_fade_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Byte(p.backfire_latch_address() + i, from.backfire_latch_address() + i);
  }
}

void Part464Save(uint32_t part, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82103300 Part464::Save");
  const schema::BoxShape from(shape);
  const Shape p(part);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(p.car_float_16_address() + 4 * i, from.car_float_16_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(p.center_address() + 16 * i, from.center_address() + 16 * i);
  }
  HalfToWord(p.unknown_448_address(), from.unknown_112_address());
  Vec4(p.half_extent_address(), from.half_extent_address());
  Vec4(p.inverse_half_extent_address(), from.inverse_half_extent_address());
  for (uint32_t i = 0; i < 8; i++) {
    Vec4(p.corners_address() + 16 * i, from.corners_address() + 16 * i);
  }
  FloatWord(p.face_px_distance_address(), from.face_px_distance_address());
  Vec4(p.face_px_normal_address(), from.face_px_normal_address());
  FloatWord(p.face_py_distance_address(), from.face_py_distance_address());
  Vec4(p.face_py_normal_address(), from.face_py_normal_address());
  FloatWord(p.face_pz_distance_address(), from.face_pz_distance_address());
  Vec4(p.face_pz_normal_address(), from.face_pz_normal_address());
  FloatWord(p.face_nx_distance_address(), from.face_nx_distance_address());
  Vec4(p.face_nx_normal_address(), from.face_nx_normal_address());
  FloatWord(p.face_ny_distance_address(), from.face_ny_distance_address());
  Vec4(p.face_ny_normal_address(), from.face_ny_normal_address());
  FloatWord(p.face_nz_distance_address(), from.face_nz_distance_address());
  Vec4(p.face_nz_normal_address(), from.face_nz_normal_address());
}

uint32_t Member4080Save(uint32_t part, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_821033F0 Member4080::Save");
  const Collision c(part);
  if (object == 0) {
    c.set_has_object(0);
    return part;
  }
  const schema::CollisionObject from(object);
  c.set_has_object(1);
  Vec4(c.box_min_address(), from.address + 48);
  Vec4(c.box_max_address(), from.address + 64);
  Matrix(c.motion_address(), from.address + 80);
  FloatWord(c.lowest_travel_address(), from.lowest_travel_address());
  const uint32_t second = from.shape_current();
  const uint32_t first = from.shape_previous();
  Part464Save(c.shape_previous_address(), first);
  Part464Save(c.shape_current_address(), second);
  Vec4(c.push_address(), from.push_x_address());
  Vec4(c.pair_torque_address(), from.pair_torque_x_address());
  Word(c.push_count_address(), from.push_count_address());
  Vec4(c.push_scale_address(), from.push_scale_x_address());
  FloatWord(c.torque_scale_address(), from.torque_scale_address());
  Word(c.moved_address(), from.moved_address());
  Vec4(c.push_out_address(), from.push_out_x_address());
  return c.push_out_address();
}

uint32_t Record5232Save(uint32_t record, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_82104538 Record5232::Save");
  const Record r(record);
  Record5232SaveTrack(record, vehicle);
  BodyStateSave(r.body_address(), schema::Vehicle(vehicle).body().address);
  Member1136Save(r.physics_address(), vehicle);
  return Member4080Save(r.collision_address(),
                          schema::Vehicle(vehicle).collision_object());
}

uint32_t RaceSnapshotSave(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82104AB8 RaceSnapshot::Save");
  const Snapshot b(block);
  b.set_unknown_2292(0);
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t vehicle = vehicle_accessors::VehicleGet(i);
    if (schema::Vehicle(vehicle).car_flags() != 0) {
      Record5232Save(b.records_address() + Record::kSize * i, vehicle);
    }
  }
  // The manager's entries and counts into the block's head.
  const schema::RaceManager manager(G::racers_manager());
  for (uint32_t i = 0; i < 77; i++) {
    Memcpy(b.entries_address() + 28 * i, manager.entries_address() + 28 * i, 28,
           0x82104B28);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Memcpy(b.traffic_entries_address() + 28 * i,
           manager.traffic_entries_address() + 28 * i, 28, 0x82104B54);
  }
  for (uint32_t i = 0; i < 7; i++) {
    Word(b.slot_0_address() + 4 * i, manager.vehicle_slots_address() + 4 * i);
  }
  Word(b.racer_count_address(), manager.racer_count_address());
  Word(b.entry_count_address(), manager.entry_count_address());
  Word(b.opponent_count_address(), manager.opponents_with_vehicle_address());
  Word(b.traffic_listed_address(), manager.traffic_with_vehicle_address());
  Word(b.traffic_count_address(), manager.traffic_allowed_address());
  Word(b.unknown_2288_address(), manager.traffic_ticks_address());
  return b.traffic_entries_address() + 28 * 2;  // the last memcpy's r3
}

// --- The copies (record to record) ---

uint32_t Record5232CopyTrack(uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821043F0 Record5232::CopyTrack");
  const Record o(out), r(record);
  Word(o.car_flags_address(), r.car_flags_address());
  Word(o.collision_flags_address(), r.collision_flags_address());
  Memcpy(o.status_address(), r.status_address(), 112, 0x82104424);
  for (uint32_t i = 0; i < 5; i++) {
    TrackPosition(o.track_positions_address() + 96 * i, r.track_positions_address() + 96 * i);
  }
  return out;
}

uint32_t BodyStateCopy(uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82103B50 BodyState::Copy");
  const Body o(out), s(record);
  Byte(o.fixed_address(), s.fixed_address());
  FloatWord(o.mass_address(), s.mass_address());
  FloatWord(o.inverse_mass_address(), s.inverse_mass_address());
  Vec4(o.center_of_mass_address(), s.center_of_mass_address());
  Vec4(o.velocity_address(), s.velocity_address());
  FloatWord(o.speed_cap_address(), s.speed_cap_address());
  Vec4(o.force_address(), s.force_address());
  Dword(o.orientation_address(), s.orientation_address());
  Dword(o.orientation_address() + 8, s.orientation_address() + 8);
  Vec4(o.angular_velocity_address(), s.angular_velocity_address());
  FloatWord(o.spin_cap_address(), s.spin_cap_address());
  Vec4(o.torque_address(), s.torque_address());
  Vec4(o.velocity_direction_address(), s.velocity_direction_address());
  FloatWord(o.speed_address(), s.speed_address());
  FloatWord(o.linear_damping_address(), s.linear_damping_address());
  Vec4(o.momentum_address(), s.momentum_address());
  Vec4(o.center_of_mass_offset_address(), s.center_of_mass_offset_address());
  Vec4(o.origin_address(), s.origin_address());
  Matrix(o.orientation_matrix_address(), s.orientation_matrix_address());
  Matrix(o.matrix_address(), s.matrix_address());
  Vec4(o.inertia_address(), s.inertia_address());
  Vec4(o.inverse_inertia_address(), s.inverse_inertia_address());
  FloatWord(o.angular_damping_address(), s.angular_damping_address());
  Vec4(o.angular_momentum_address(), s.angular_momentum_address());
  Matrix(o.world_inverse_inertia_address(), s.world_inverse_inertia_address());
  Vec4(o.gravity_address(), s.gravity_address());
  Vec4(o.weight_address(), s.weight_address());
  FloatWord(o.effective_mass_address(), s.effective_mass_address());
  return out;
}

uint32_t Member1136Copy(uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82103CB8 Member1136::Copy");
  const Physics o(out), p(record);
  for (uint32_t i = 0; i < 3; i++) {
    Vec4(o.body_row0_address() + 16 * i, p.body_row0_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(o.speed_address() + 4 * i, p.speed_address() + 4 * i);
  }
  Vec4(o.angular_velocity_address(), p.angular_velocity_address());
  for (uint32_t i = 0; i < 4; i++) {
    FloatWord(o.unknown_80_address() + 4 * i, p.unknown_80_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Vec4(o.ground_up_address() + 16 * i, p.ground_up_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Matrix(o.ground_matrix_address() + 64 * i, p.ground_matrix_address() + 64 * i);
  }
  Byte(o.speed_sensitive_steering_address(), p.speed_sensitive_steering_address());
  for (uint32_t i = 0; i < 14; i++) {
    FloatWord(o.steering_input_address() + 4 * i, p.steering_input_address() + 4 * i);
  }
  Word(o.wheels_on_ground_address(), p.wheels_on_ground_address());
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(o.unknown_336_address() + 16 * i, p.unknown_336_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Wheel(o.wheel_0_address() + 368 * i, p.wheel_0_address() + 368 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    WheelRecord2(o.front_axle_address() + 128 * i, p.front_axle_address() + 128 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Word(o.wheel_by_load_0_address() + 4 * i, p.wheel_by_load_0_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(o.driven_axle_spin_address() + 4 * i, p.driven_axle_spin_address() + 4 * i);
  }
  Memcpy(o.drivetrain_block_address(), p.drivetrain_block_address(), 144, 0x82103E90);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(o.direction_address() + 4 * i, p.direction_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Word(o.substep_count_address() + 4 * i, p.substep_count_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 8; i++) {
    Byte(o.unknown_2332_address() + i, p.unknown_2332_address() + i);
  }
  FloatWord(o.value2388_address(), p.value2388_address());
  Word(o.traction_counter_address(), p.traction_counter_address());
  FloatWord(o.traction_limit_address(), p.traction_limit_address());
  for (uint32_t i = 0; i < 2; i++) {
    Byte(o.traction_active_address() + i, p.traction_active_address() + i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(o.drag_scale_address() + 4 * i, p.drag_scale_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(o.oil_factor_address() + 4 * i, p.oil_factor_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(o.nitrous_factor_address() + 4 * i, p.nitrous_factor_address() + 4 * i);
  }
  Byte(o.nitrous_on_address(), p.nitrous_on_address());
  FloatWord(o.speed_squared_address(), p.speed_squared_address());
  for (uint32_t i = 0; i < 2; i++) {
    Byte(o.unknown_2396_address() + i, p.unknown_2396_address() + i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(o.unknown_2400_address() + 4 * i, p.unknown_2400_address() + 4 * i);
  }
  Byte(o.unknown_2408_address(), p.unknown_2408_address());
  for (uint32_t i = 0; i < 2; i++) {
    Vec4(o.body_force_address() + 16 * i, p.body_force_address() + 16 * i);
  }
  Word(o.tire_wear_mode_address(), p.tire_wear_mode_address());
  FloatWord(o.backfire_charge_address(), p.backfire_charge_address());
  Byte(o.backfire_address(), p.backfire_address());
  FloatWord(o.backfire_strength_address(), p.backfire_strength_address());
  Byte(o.unknown_2464_address(), p.unknown_2464_address());
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(o.throttle_used_address() + 4 * i, p.throttle_used_address() + 4 * i);
  }
  Word(o.unknown_2476_address(), p.unknown_2476_address());
  for (uint32_t i = 0; i < 6; i++) {
    Vec4(o.axis_address() + 16 * i, p.axis_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Byte(o.unknown_2576_address() + i, p.unknown_2576_address() + i);
  }
  for (uint32_t i = 0; i < 9; i++) {
    FloatWord(o.unknown_2580_address() + 4 * i, p.unknown_2580_address() + 4 * i);
  }
  Byte(o.surfaces_on_address(), p.surfaces_on_address());
  FloatWord(o.unknown_2620_address(), p.unknown_2620_address());
  Byte(o.at_rest_address(), p.at_rest_address());
  for (uint32_t i = 0; i < 4; i++) {
    FloatWord(o.unknown_2628_address() + 4 * i, p.unknown_2628_address() + 4 * i);
  }
  Byte(o.wastegate_open_address(), p.wastegate_open_address());
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(o.wastegate_address() + 4 * i, p.wastegate_address() + 4 * i);
  }
  Byte(o.blow_off_open_address(), p.blow_off_open_address());
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(o.blow_off_address() + 4 * i, p.blow_off_address() + 4 * i);
  }
  Word(o.track_position_address(), p.track_position_address());
  for (uint32_t i = 0; i < 2; i++) {
    Memcpy(o.steer_block_a_address() + 60 * i,
        p.steer_block_a_address() + 60 * i, 60, 0x82104108);
  }
  for (uint32_t i = 0; i < 6; i++) {
    FloatWord(o.steering_result_address() + 4 * i, p.steering_result_address() + 4 * i);
  }
  Byte(o.upright_address(), p.upright_address());
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(o.unknown_2832_address() + 4 * i, p.unknown_2832_address() + 4 * i);
  }
  Word(o.step_count_address(), p.step_count_address());
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(o.unknown_2864_address() + 16 * i, p.unknown_2864_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    FloatWord(o.brake_fade_address() + 4 * i, p.brake_fade_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Byte(o.backfire_latch_address() + i, p.backfire_latch_address() + i);
  }
  return out;
}

uint32_t Part464Copy(uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821041C0 Part464::Copy");
  const Shape o(out), p(record);
  for (uint32_t i = 0; i < 2; i++) {
    Vec4(o.half_extent_address() + 16 * i, p.half_extent_address() + 16 * i);
  }
  for (uint32_t i = 0; i < 8; i++) {
    Vec4(o.corners_address() + 16 * i, p.corners_address() + 16 * i);
  }
  FloatWord(o.face_px_distance_address(), p.face_px_distance_address());
  Vec4(o.face_px_normal_address(), p.face_px_normal_address());
  FloatWord(o.face_py_distance_address(), p.face_py_distance_address());
  Vec4(o.face_py_normal_address(), p.face_py_normal_address());
  FloatWord(o.face_pz_distance_address(), p.face_pz_distance_address());
  Vec4(o.face_pz_normal_address(), p.face_pz_normal_address());
  FloatWord(o.face_nx_distance_address(), p.face_nx_distance_address());
  Vec4(o.face_nx_normal_address(), p.face_nx_normal_address());
  FloatWord(o.face_ny_distance_address(), p.face_ny_distance_address());
  Vec4(o.face_ny_normal_address(), p.face_ny_normal_address());
  FloatWord(o.face_nz_distance_address(), p.face_nz_distance_address());
  Vec4(o.face_nz_normal_address(), p.face_nz_normal_address());
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(o.car_float_16_address() + 4 * i, p.car_float_16_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(o.center_address() + 16 * i, p.center_address() + 16 * i);
  }
  Word(o.unknown_448_address(), p.unknown_448_address());

  return out;
}

uint32_t Member4080CopyShapes(uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82104460 Member4080::CopyShapes");
  // r3 and r4 are two states' shape_previous (+16).
  const uint32_t at = Collision(0).shape_previous_address();
  const Collision o(out - at), c(record - at);
  Part464Copy(o.shape_previous_address(), c.shape_previous_address());
  Part464Copy(o.shape_current_address(), c.shape_current_address());
  Vec4(o.box_min_address(), c.box_min_address());
  Vec4(o.box_max_address(), c.box_max_address());
  Matrix(o.motion_address(), c.motion_address());
  FloatWord(o.lowest_travel_address(), c.lowest_travel_address());
  return out;
}

uint32_t Member4080Copy(uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82104598 Member4080::Copy");
  const Collision o(out), c(record);
  Word(o.has_object_address(), c.has_object_address());
  Member4080CopyShapes(o.shape_previous_address(), c.shape_previous_address());
  Vec4(o.push_address(), c.push_address());
  Vec4(o.pair_torque_address(), c.pair_torque_address());
  Word(o.push_count_address(), c.push_count_address());
  Vec4(o.push_scale_address(), c.push_scale_address());
  FloatWord(o.torque_scale_address(), c.torque_scale_address());
  Word(o.moved_address(), c.moved_address());
  Vec4(o.push_out_address(), c.push_out_address());
  return out;
}

uint32_t Record5232Copy(uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82104BC0 Record5232::Copy");
  const Record o(out), r(record);
  Record5232CopyTrack(out, record);
  BodyStateCopy(o.body_address(), r.body_address());
  Member1136Copy(o.physics_address(), r.physics_address());
  Member4080Copy(o.collision_address(), r.collision_address());
  return out;
}

uint32_t RaceSnapshotCopy(uint32_t out, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82104EA8 RaceSnapshot::Copy");
  const Snapshot o(out), b(block);
  Memcpy(out, block, 2292, 0x82104EC4);
  Word(o.unknown_2292_address(), b.unknown_2292_address());
  for (uint32_t i = 0; i < 7; i++) {
    Record5232Copy(o.records_address() + Record::kSize * i,
                   b.records_address() + Record::kSize * i);
  }
  return out;
}

// --- The kept replay ---

uint32_t ReplayKeep(uint32_t replay, uint32_t race, uint32_t recordings,
                    uint32_t save, uint32_t from_stage, uint32_t frame,
                    const StagesCopyOriginal& stages_copy) {
  ITC_NATIVE_RUNNING("sub_82104F68 Replay::Keep");
  const schema::KeptReplay r(replay);
  const schema::RaceData d(race);
  Memcpy(r.race_words_address(), d.route_address(), 32, 0x82104FB0);
  // The car count and the three words after it (stored 84, 92, 96, 88).
  Word(r.race_word_59610_address(), d.car_count_address());
  Word(r.race_word_59618_address(), d.word_59618_address());
  Word(r.race_word_5961C_address(), d.word_5961c_address());
  Word(r.race_word_59614_address(), d.added_count_address());
  Word(r.race_word_A3AE0_address(), d.random_10_address());
  // The 93 racer records' entries (their source through the empty
  // sub_82106540: nothing done). The kept replay names entry 0's words, so
  // entry k is a view 1,908 * k bytes on; the race data's racer records
  // (3,936 bytes from +48) stay offsets.
  for (uint32_t k = 0; k < 93; k++) {
    const uint32_t record = d.racer_records_address() + 3936 * k;
    const schema::KeptReplay entry(replay + 1908 * k);
    Memcpy(entry.racer_flags_address() + 8, record, 1884, 0x8210502C);
    guest::StoreU16(entry.racer_start_position_address(),
                    guest::LoadU16(record + 3904));
    guest::StoreU16(entry.racer_half_1894_address(), guest::LoadU16(record + 3906));
    FloatWord(entry.racer_word_3908_address(), record + 3908);
    Word(entry.racer_id_address(), record + 3912);
    FloatWord(entry.racer_word_3920_address(), record + 3920);
    Word(entry.racer_flags_address(), record + 3888);
    Word(entry.racer_word_3892_address(), record + 3892);
  }
  // The stage: the last (-1), or the list's current - 1 with `from_stage`,
  // into the frame's +80, then to +0x2B588.
  const schema::Stages stages(d.stages_address());
  const uint32_t stage = from_stage != 0 ? stages.current() - 1 : 0xFFFFFFFFu;
  if (!vehicle_accessors::StagesCopy(frame + 80, stages.address, stage)) {
    stages_copy(frame + 80, stages.address, stage, frame);
  }
  Memcpy(r.stage_address(), frame + 80, 80, 0x821050B4);
  // The last kept result's +3916 to +3928.
  const uint32_t last = vehicle_accessors::RacerRecordsAt(schema::RaceData(G::race_data()).results_address(), 0xFFFFFFFFu);
  for (uint32_t i = 0; i < 4; i++) {
    Word(r.result_3916_address() + 4 * i, last + 3916 + 4 * i);
  }
  for (uint32_t i = 0; i < 5; i++) {
    Word(r.race_words_A3AE4_address() + 4 * i, d.replay_word_address() + 4 * i);
  }
  RaceSnapshotCopy(r.snapshot_address(), recordings + 0x20F590);
  Memcpy(r.recording_address(), recordings, 0x107AC4, 0x82105160);
  {
    const uint32_t w0 = guest::LoadU32(r.race_words_address());
    const uint32_t w1 = guest::LoadU32(r.race_words_address() + 24);
    const uint32_t w2 = guest::LoadU32(r.race_words_address() + 28);
    r.set_race_word_4(w0);
    r.set_race_word_28(w1);
    r.set_race_word_32(w2);
  }
  // The four racers the stage lists: their entries' +20.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t racer = guest::LoadU32(r.stage_word_44_address() + 8 * i);
    guest::StoreU32(r.racer_record_words_address() + 4 * i,
                    guest::LoadU32(r.racer_flags_address() + 1908 * racer + 20));
  }
  Word(r.stage_word_0_address(), r.stage_address());
  if (save != 0) {
    for (uint32_t i = 0; i < 5; i++) {
      Word(r.save_pick_address() + 4 * i, save + 8 + 4 * i);
    }
  } else {
    r.set_save_pick_flag(0);
    r.set_save_pick_4(6);
    r.set_save_pick_8(0);
    r.set_save_pick_10(1);
    r.set_save_pick_12(6);
    r.set_save_pick_16(0);
    r.set_save_pick_18(1);
  }
  r.set_unfilled(0);
  return r.recording_address();  // the last memcpy's r3
}

}  // namespace replay_snapshots
