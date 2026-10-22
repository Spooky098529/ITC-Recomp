// The vehicle's state from its rigid body, its copies and its set-up
// (docs/functions/car-physics.md): see car_physics.h. Wheel w's 368-byte
// record is at vehicle + 416 + 368 w.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

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
  return guest::LoadU32(base, vehicle + 272);
}

uint32_t Body(uint8_t* base, uint32_t vehicle) {
  return guest::LoadU32(base, vehicle + 336);
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
  CopyWord(base, va::RigidBodyField160(Body(base, v)), v + 48);
  vl::Vec4Copy(base, v + 64, va::RigidBodyAngularVelocity(Body(base, v)));
  MatrixCopyRows(base, v + 208, va::RigidBodyMatrix(Body(base, v)));
  // The w of the mount points (+160) and of their transforms (+224).
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, kZero, f + 172 + 16 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, kZero, f + 236 + 16 * i);
  }
  // sin(+88), computed and dropped (no state of its own).
  (void)c_runtime::Sin(Lfs(base, v + 88));
  // The wheels' mount points from the car data's wheel set-up records, the
  // travel (wheel +0) added to y; their +16 into wheel +24 and the z. Then
  // through +208 into wheel +80.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t t = f + 160 + 16 * i;
    vl::Vec4Copy(base, t, CarData(base, v) + 896 + 96 * i);
    const uint32_t record = CarData(base, v) + 96 * i;
    Stfs(base, t + 4, Fadds(Lfs(base, Wheel(v, i) + 0), Lfs(base, t + 4)));
    CopyWord(base, record + 912, Wheel(v, i) + 24);
    CopyWord(base, record + 912, t + 8);
  }
  for (uint32_t i = 0; i < 4; i++) {
    vl::MatrixTransformPoint(base, f + 96, v + 208, f + 160 + 16 * i);
    vl::Vec4Copy(base, f + 224 + 16 * i, f + 96);
  }
  for (uint32_t i = 0; i < 4; i++) {
    vl::Vec4Copy(base, Wheel(v, i) + 80, f + 224 + 16 * i);
  }
  // Again without the travel, into wheel +64.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t t = f + 160 + 16 * i;
    vl::Vec4Copy(base, t, CarData(base, v) + 896 + 96 * i);
    const uint32_t record = CarData(base, v) + 96 * i;
    CopyWord(base, record + 912, Wheel(v, i) + 24);
    CopyWord(base, record + 912, t + 8);
  }
  for (uint32_t i = 0; i < 4; i++) {
    vl::MatrixTransformPoint(base, f + 96, v + 208, f + 160 + 16 * i);
    vl::Vec4Copy(base, f + 224 + 16 * i, f + 96);
  }
  for (uint32_t i = 0; i < 4; i++) {
    vl::Vec4Copy(base, Wheel(v, i) + 64, f + 224 + 16 * i);
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
  MatrixMultiplyTranslation(base, v + 3312, v + 208, f + 96, f - 192);
  mm::Multiply(base, v + 3312, v + 3312, v + 3248);
  interpolation::BlendState_OnMultiply(base, v + 3312, v + 3312, v + 3248, 0x820B16EC);
  MatrixMultiplyTranslation(base, v + 3312, v + 3312, f + 80, f - 192);
  guest::StoreU32(base, Body(base, v) + 12, v + 3312);
  // The axle records: the mounts' midpoints (+1888, +2016) and the tracks
  // (+1992, +2120).
  const double half = Lfs(base, 0x82001BE4);
  Stfs(base, v + 1992, Fsubs(Lfs(base, Wheel(v, 0) + 24), Lfs(base, Wheel(v, 1) + 24)));
  vl::Vec3Sum(base, f + 96, Wheel(v, 0) + 64, Wheel(v, 1) + 64);
  vl::Vec4Scaled(base, f + 80, f + 96, half);
  vl::Vec4Copy(base, v + 1888, f + 80);
  Stfs(base, v + 2120, Fsubs(Lfs(base, Wheel(v, 2) + 24), Lfs(base, Wheel(v, 3) + 24)));
  vl::Vec3Sum(base, f + 96, Wheel(v, 2) + 64, Wheel(v, 3) + 64);
  vl::Vec4Scaled(base, f + 80, f + 96, half);
  vl::Vec4Copy(base, v + 2016, f + 80);
  return half;
}

void WheelCopy(uint8_t* base, uint32_t out, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_820B0A50 Wheel::Copy");
  for (uint32_t at = 0; at < 16; at += 4) {
    CopyWord(base, wheel + at, out + at);
  }
  guest::StoreU8(base, out + 16, guest::LoadU8(base, wheel + 16));
  guest::StoreU8(base, out + 17, guest::LoadU8(base, wheel + 17));
  CopyWord(base, wheel + 20, out + 20);
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
  vl::Vec4Copy(base, out + 64, wheel + 64);
  vl::Vec4Copy(base, out + 80, wheel + 80);
  CopyWord(base, wheel + 96, out + 96);
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
  vl::Vec4Copy(base, out + 336, wheel + 336);
  CopyWord(base, wheel + 352, out + 352);
  CopyWord(base, wheel + 356, out + 356);
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

}  // namespace car_physics
