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

void VehicleReset(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_820B0EE8 Vehicle::Reset");
  const uint32_t v = vehicle, f = frame;
  constexpr uint32_t kAxis = 0x82638270;
  auto zero = [&](uint32_t at) { CopyWord(base, kZero, v + at); };
  auto one = [&](uint32_t at) { CopyWord(base, kOne, v + at); };
  auto word = [&](uint32_t at, uint32_t value) { guest::StoreU32(base, v + at, value); };
  auto byte = [&](uint32_t at, uint8_t value) { guest::StoreU8(base, v + at, value); };
  auto clear = [&](uint32_t at, uint32_t bytes) {
    // memset (sub_825A71D0), as its adapter does it.
    guest::ForEachHostPart(base, v + at, bytes,
                           [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
  };
  vl::Vec4Copy(base, v + 0, kZeroVector);
  vl::Vec4Copy(base, v + 16, kZeroVector);
  vl::Vec4Copy(base, v + 32, kZeroVector);
  zero(48);
  zero(52);
  vl::Vec4Copy(base, v + 64, kZeroVector);
  for (uint32_t at = 80; at < 96; at += 4) {
    zero(at);
  }
  // +96 = (0, 1, 0) (up), its w kept.
  const uint32_t up_w = guest::LoadU32(base, v + 108);
  zero(96);
  one(100);
  zero(104);
  word(108, up_w);
  vl::Vec4Copy(base, v + 112, kZeroVector);
  vl::Vec4Copy(base, v + 128, kZeroVector);
  mm::SetIdentity(base, v + 144);
  mm::SetIdentity(base, v + 208);
  zero(280);
  zero(284);
  zero(292);
  zero(296);
  zero(300);
  word(272, 0);
  zero(304);
  byte(276, 0);
  zero(308);
  word(348, 0);
  for (uint32_t at : {312u, 316u, 320u, 288u, 324u, 328u, 332u}) {
    zero(at);
  }
  for (uint32_t at = 352; at < 416; at += 16) {
    vl::Vec4Copy(base, v + at, kZeroVector);
  }
  clear(416, 1472);
  clear(1888, 128);
  clear(2016, 128);
  double angle_f1 = 0.0;
  for (uint32_t w = 0; w < 4; w++) {
    word(2144 + 4 * w, 0);
    word(2160 + 4 * w, 0);
    word(2176 + 4 * w, 0);
    angle_f1 = mm::QuaternionFromAxisAngle(base, Wheel(v, w) + 48, kAxis, Lfs(base, kOne),
                                           f - 32);
  }
  for (uint32_t at = 2176; at < 2192; at += 4) {
    word(at, 0);
  }
  zero(2192);
  zero(2196);
  zero(2200);
  clear(2204, 144);
  for (uint32_t at : {2348u, 2396u, 2404u, 2408u, 2412u}) {
    one(at);
  }
  CopyWord(base, 0x820029E8, v + 2352);
  one(2416);
  byte(2280, 0);
  zero(2420);
  byte(2249, 1);
  one(2428);
  word(2368, 10);
  CopyWord(base, 0x82002978, v + 2360);
  word(2372, 2);
  one(2432);
  word(2376, 0);
  CopyWord(base, 0x82002978, v + 2356);
  byte(2380, 1);
  one(2436);
  byte(2381, 0);
  byte(2382, 0);
  CopyWord(base, 0x820029E4, v + 2364);
  byte(2383, 0);
  zero(2440);
  byte(2384, 0);
  zero(2448);
  byte(2385, 0);
  zero(2452);
  byte(2386, 0);
  byte(2387, 0);
  word(2392, 0);
  for (uint32_t at : {2400u, 2401u, 2424u, 2444u, 2445u, 2456u}) {
    byte(at, 0);
  }
  vl::Vec4Copy(base, v + 2464, kZeroVector);
  vl::Vec4Copy(base, v + 2480, kZeroVector);
  zero(2500);
  zero(2508);
  word(2496, 0);
  zero(2516);
  byte(2504, 0);
  zero(2520);
  byte(2512, 0);
  word(2524, 0);
  vl::Vec4Copy(base, v + 2528, kAxis);
  clear(2544, 80);
  zero(2628);
  byte(2624, 1);
  byte(2625, 0);
  for (uint32_t at = 2632; at < 2664; at += 4) {
    word(at, 0);
  }
  byte(2664, 0);
  for (uint32_t i = 0; i < 32; i++) {
    word(2668 + 4 * i, 0);
    word(2796 + 4 * i, 0);
  }
  byte(2928, 0);
  CopyWord(base, v + 2356, v + 2924);
  byte(2948, 0);
  zero(2932);
  byte(2960, 0);
  one(2936);
  word(2984, 0);
  one(2940);
  for (uint32_t at : {2944u, 2952u}) {
    zero(at);
  }
  one(2956);
  for (uint32_t at : {2964u, 2968u, 2972u}) {
    zero(at);
  }
  one(2980);
  one(2976);
  for (uint32_t at = 3108; at < 3132; at += 4) {
    zero(at);
  }
  byte(3132, 1);
  for (uint32_t at = 3136; at < 3160; at += 4) {
    word(at, 0);
  }
  for (uint32_t at = 3168; at < 3232; at += 16) {
    vl::Vec4Copy(base, v + at, kAxis);
  }
  zero(3232);
  zero(3236);
  byte(3240, 0);
  byte(3241, 0);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(v, w);
    CopyWord(base, kZero, wheel + 292);
    CopyWord(base, kOne, wheel + 228);
    const uint32_t up_w = guest::LoadU32(base, wheel + 124);
    CopyWord(base, kZero, wheel + 112);
    CopyWord(base, kOne, wheel + 116);
    CopyWord(base, kZero, wheel + 120);
    guest::StoreU32(base, wheel + 124, up_w);
    for (uint32_t at : {220u, 36u, 284u, 288u, 280u}) {
      CopyWord(base, kZero, wheel + at);
    }
  }
  zero(2320);
  zero(2324);
  zero(2224);
  zero(2204);
  zero(2208);
  zero(2216);
  zero(2272);
  byte(2285, 0);
  byte(2284, 0);
  byte(2286, 0);
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
  zero(2340);
  word(2344, 0);
  *f1 = angle_f1;
}

void VehicleSetUpGearing(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B4AA8 Vehicle::SetUpGearing");
  const uint32_t v = vehicle, f = frame;
  const uint32_t data = CarData(base, v);
  const uint32_t e = data + 1268;  // the gearbox
  // The driven wheels' radius (+2332) and the axle's +200 (+2336) by the
  // drive type.
  const uint32_t drive = guest::LoadU32(base, v + 2376);
  double axle;
  if (drive < 1) {
    CopyWord(base, data + 388, v + 2332);
    axle = Lfs(base, data + 536);
  } else {
    CopyWord(base, data + 660, v + 2332);
    axle = Lfs(base, data + 808);
    if (drive != 1) {
      axle = Fmuls(Fadds(axle, Lfs(base, data + 536)), Lfs(base, 0x82001BE4));
    }
  }
  Stfs(base, v + 2336, axle);
  const double final_drive = Lfs(base, e + 332);
  Stfs(base, v + 2328, Fmuls(Fmuls(Lfs(base, e + 272), final_drive), final_drive));
  VehicleSetGearValues(base, v);
  // The shift-up speeds +2632 + 4 x gear.
  const double redline = Lfs(base, e + 16);
  const double one = Lfs(base, kOne);
  const double per_radius = Fdivs(one, Lfs(base, v + 2332));
  const double zero = Lfs(base, kZero);
  CopyWord(base, kZero, v + 2632);
  constexpr uint32_t kTop = 0x8200297C, kScale = 0x820029A4;
  Stfs(base, v + 4 * (guest::LoadU32(base, e) + 658), Fmuls(per_radius, Lfs(base, kTop)));
  if (guest::LoadU32(base, e) > 1) {
    const double scale = Lfs(base, kScale);
    const double limit = Fmuls(redline, Lfs(base, 0x82002A34));
    const double step = Lfs(base, 0x82002A68);
    const double margin = Lfs(base, 0x82002A2C);
    uint32_t out = v + 2636, tables = e + 236;
    for (uint32_t gear = 1;; gear++) {
      const double inverse = Fdivs(one, Lfs(base, e + 4));
      const double drive_ratio = Lfs(base, e + 332);
      const double other = Lfs(base, e + 276);
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
      if (!(gear + 1 < guest::LoadU32(base, e))) {
        break;
      }
    }
  }
  // +2300; the peak of the torque curve (e + 24, 32 points): its index's
  // float (through the frame's +80) into +2296 and +2292.
  Stfs(base, v + 2300,
       Fadds(Fadds(Lfs(base, e + 292), Lfs(base, e + 280)), Lfs(base, e + 200)));
  double peak = zero;
  uint32_t index = 0;
  for (uint32_t i = 0; i < 32; i++) {
    const double point = Lfs(base, e + 24 + 4 * i);
    if (peak < point) {
      peak = point;
      index = i;
    }
  }
  guest::StoreU32(base, f + 80, 0);
  guest::StoreU32(base, f + 84, index);
  const double rpm = Fmuls(double(float(double(index))), Lfs(base, 0x82002A60));
  Stfs(base, v + 2296, rpm);
  const double scaled = Fmuls(rpm, Lfs(base, 0x82002A14));
  Stfs(base, v + 2292, Fmadds(Lfs(base, e + 360), Lfs(base, 0x82002A64), scaled));
}

double VehicleSetSteering(uint8_t* base, uint32_t vehicle, double steer) {
  ITC_NATIVE_RUNNING("sub_820B3D10 Vehicle::SetSteering");
  const uint32_t v = vehicle;
  double clamped = Lfs(base, 0x82001BE0);
  const double one = Lfs(base, kOne);
  guest::StoreU8(base, v + 276, 1);
  if (!(steer < clamped)) {
    clamped = steer > one ? one : steer;
  }
  const double spin = Fmadds(Fabs(Lfs(base, Wheel(v, 1) + 248)), Lfs(base, 0x82001FBC),
                             Fabs(Lfs(base, Wheel(v, 0) + 248)));
  const uint32_t data = CarData(base, v);
  const double per_length = Fdivs(one, Lfs(base, data + 480));
  const double scale = spin > one ? one : spin;
  const double a = Fmuls(Lfs(base, Wheel(v, 0) + 304), per_length);
  const double b = Fmuls(Lfs(base, Wheel(v, 1) + 304), per_length);
  const double turned = c_runtime::Sin(Fmuls(Lfs(base, data + 324), clamped));
  const double full = c_runtime::Sin(Lfs(base, CarData(base, v) + 324));
  Stfs(base, v + 280, clamped);
  const double mean = Fmuls(Fadds(b, a), Lfs(base, 0x82001BE4));
  const double ratio = Fdivs(double(float(turned)), double(float(full)));
  Stfs(base, v + 3108, Fmadds(ratio, scale, mean));
  return full;
}

void SteerRecordFill(uint8_t* base, uint32_t out, uint32_t record, double f1, double f2,
                     double f3) {
  ITC_NATIVE_RUNNING("sub_820B0668 SteerRecord::Fill");
  const double product = Fmuls(f1, f2);
  Stfs(base, out + 8, product);
  guest::StoreU32(base, out + 4, record + 32);
  guest::StoreU32(base, out + 0, record + 16);
  const double scaled = Fmuls(product, Lfs(base, 0x820029DC));
  Stfs(base, out + 12, scaled);
  double x = f1;
  const double knee = Lfs(base, record + 28);
  if (f1 > knee) {
    x = Fmadds(Fsubs(f1, knee), Lfs(base, 0x82002208), knee);
  }
  double g = Fnmsubs(Lfs(base, record + 20), x, Lfs(base, record + 16));
  if (!(g > Lfs(base, kZero))) {
    g = Lfs(base, 0x820029D8);
  }
  const double value = Fmuls(g, x);
  Stfs(base, out + 20, value);
  const double c = Lfs(base, 0x820021E4);
  const double a = Fmuls(product, c);
  const double b = Fmuls(scaled, c);
  const double times = Fmuls(value, f3);
  Stfs(base, out + 16, times);
  Stfs(base, out + 24, Fdivs(value, a));
  Stfs(base, out + 28, Fdivs(value, b));
  Stfs(base, out + 32, Fdivs(times, value));
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
  const double half_width = Fmuls(Lfs(base, record + 16), Lfs(base, 0x82001BE4));
  const double length = Lfs(base, record + 8);
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
      Fmuls(Fadds(Lfs(base, Wheel(v, 1) + 264), Lfs(base, Wheel(v, 0) + 264)), half);
  {
    double t;
    if (!c_runtime::Tan(CurveAngle(base, slip), &t)) {
      return false;
    }
  }
  const uint32_t data = CarData(base, v);
  guest::StoreU8(base, v + 276, 1);
  const double rate = Fmuls(Lfs(base, data + 316), dt);
  const double minus_one = Lfs(base, 0x82001BE0);
  const double back_rate = Fmuls(Lfs(base, data + 320), dt);
  if ((left & 0xFF) != 0) {
    target = minus_one;
  }
  const double one = Lfs(base, kOne);
  if ((right & 0xFF) != 0) {
    target = one;
  }
  const double zero = Lfs(base, kZero);
  double speed = Fsubs(Fabs(Lfs(base, v + 48)), Lfs(base, 0x820029F0));
  if (speed < zero) {
    speed = zero;
  }
  const double load = Fmuls(Fadds(Lfs(base, Wheel(v, 1) + 280), Lfs(base, Wheel(v, 0) + 280)),
                            half);
  const double sensitivity = Lfs(base, data + 328);
  const double aligning = Lfs(base, data + 484);
  const double factor =
      Fdivs(one, Fdivs(Fmuls(sensitivity, f3), Fadds(sensitivity, speed)));
  double toward = Fmuls(factor, rate);
  const double away = Fmuls(factor, back_rate);
  SteerRecordFill(base, v + 2988, data + 336, load, one, aligning);
  double curve;
  SteerRecordCurve(base, v + 2988, slip, &curve);
  const double turning = Fsubs(Lfs(base, v + 304), Lfs(base, v + 92));
  const double steer = Lfs(base, v + 280);
  double move = Fsubs(target, steer);
  Stfs(base, v + 3108, curve);
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
  Stfs(base, v + 280, moved);
  if (moved < minus_one) {
    moved = minus_one;
  } else if (moved > Lfs(base, kOne)) {
    moved = Lfs(base, kOne);
  }
  Stfs(base, v + 280, moved);
  *f1 = curve;
  return true;
}

double VehicleSuspensionSpring(uint8_t* base, uint32_t vehicle, uint32_t w, double change,
                               uint32_t velocity) {
  ITC_NATIVE_RUNNING("sub_820B0198 Vehicle::SuspensionSpring");
  const uint32_t v = vehicle, wheel = Wheel(v, w);
  const uint32_t r = guest::LoadU32(base, v + 2160 + 4 * w) + 96;  // the suspension
  const uint32_t partner = Wheel(v, (w & 1) != 0 ? w - 1 : w + 1);
  const double travel = Lfs(base, wheel + 0);
  double moved = Fadds(travel, change);
  const double previous = Lfs(base, wheel + 12);
  const double roll = Lfs(base, r + 36);
  const double mass = Lfs(base, r + 0);
  const double dt = Lfs(base, v + 2352);
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
      Stfs(base, wheel + 0, Fsubs(travel, leaving));
      return Fneg(Lfs(base, r + 32));
    }
    damping = Fmuls(rebound, change);
  }
  const uint32_t steps = guest::LoadU32(base, v + 2368);
  const double start = Lfs(base, velocity);
  const double step = Lfs(base, v + 2364);
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
      if (!(i < guest::LoadU32(base, v + 2368))) {
        break;
      }
    }
  }
  Stfs(base, wheel + 0, moved);
  const double change_of_speed = Fsubs(Lfs(base, velocity), start);
  const double per_dt = Fdivs(Lfs(base, 0x82001BE0), Lfs(base, v + 2352));
  return Fmuls(Fmuls(change_of_speed, per_dt), Lfs(base, r + 0));
}

double VehicleUpdateWheelSuspension(uint8_t* base, uint32_t vehicle, uint32_t w,
                                    uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B5588 Vehicle::UpdateWheelSuspension");
  const uint32_t v = vehicle, f = frame, wheel = Wheel(v, w);
  const double down = Fneg(Lfs(base, wheel + 96));
  const uint32_t data = guest::LoadU32(base, v + 2160 + 4 * w);
  guest::StoreU8(base, wheel + 16, 0);
  va::RigidBodyPointVelocity(base, f + 96, Body(base, v), wheel + 64);
  const double vertical = double(vl::Vec3DotOf(base, f + 96, v + 160));
  Stfs(base, f + 80, Fneg(vertical));
  const double result = VehicleSuspensionSpring(base, v, w, down, f + 80);
  Stfs(base, wheel + 4, Fneg(Lfs(base, f + 80)));
  double force = Fadds(Lfs(base, data + 128), result);
  const double zero = Lfs(base, kZero);
  if (!(Lfs(base, wheel + 0) > Lfs(base, data + 124))) {
    CopyWord(base, data + 124, wheel + 0);
    force = zero;
    guest::StoreU8(base, wheel + 17, 0);
  }
  if (!(force > zero)) {
    force = zero;
    guest::StoreU8(base, wheel + 17, 0);
  } else {
    guest::StoreU8(base, wheel + 17, 1);
    guest::StoreU32(base, v + 348, guest::LoadU32(base, v + 348) + 1);
  }
  Stfs(base, wheel + 8, force);
  return result;
}

uint32_t VehicleUpdateSuspension(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_820B6328 Vehicle::UpdateSuspension");
  const uint32_t v = vehicle, f = frame;
  guest::StoreU32(base, v + 348, 0);
  for (uint32_t w = 0; w < 4; w++) {
    guest::StoreU32(base, v + 2176 + 4 * w, w);
    CopyWord(base, Wheel(v, w) + 0, Wheel(v, w) + 12);
  }
  // The order by load (+280), a bubble sort from the back.
  for (uint32_t pass = 0; pass < 4; pass++) {
    if (pass >= 3) {
      continue;
    }
    uint32_t at = v + 2184;
    for (uint32_t n = 3 - pass; n != 0; n--, at -= 4) {
      const uint32_t a = guest::LoadU32(base, at), b = guest::LoadU32(base, at + 4);
      if (Lfs(base, v + 368 * a + 696) > Lfs(base, v + 368 * b + 696)) {
        guest::StoreU32(base, at + 4, a);
        guest::StoreU32(base, at, b);
      }
    }
  }
  for (uint32_t i = 0; i < 4; i++) {
    VehicleUpdateWheelSuspension(base, v, guest::LoadU32(base, v + 2176 + 4 * i), f - 176);
  }
  double last = 0.0;
  for (uint32_t w = 0; w < 4; w++) {
    last = Lfs(base, Wheel(v, w) + 8);
    vl::Vec4Scaled(base, f + 80, v + 96, last);
    vl::Vec3AddTo(base, v + 2544 + 16 * w, f + 80);
  }
  *f1 = last;
  return v + 2592;
}

void VehicleUpdateDrivenAxle(uint8_t* base, uint32_t vehicle, uint32_t first, double drive,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820B31D0 Vehicle::UpdateDrivenAxle");
  const uint32_t v = vehicle;
  const uint32_t record = guest::LoadU32(base, v + 2144 + 4 * first);
  const uint32_t data = guest::LoadU32(base, v + 2160 + 4 * first);
  const uint32_t a = Wheel(v, first), b = Wheel(v, first + 1);
  const double size = Fabs(drive);
  const double half = Lfs(base, 0x82001BE4);
  const double inertia = Fmuls(Lfs(base, record + 108), half);
  // Its temporaries below the caller's r1 (`sp`): the accelerations at -64
  // and -60, the locked flags at -48 and -44, the wheels at -56 and -52.
  const uint32_t accel = sp - 64, locked = sp - 48, wheels = sp - 56;
  guest::StoreU32(base, locked + 0, 0);
  guest::StoreU32(base, locked + 4, 0);
  guest::StoreU32(base, wheels + 0, a);
  guest::StoreU32(base, wheels + 4, b);
  const double grip = Lfs(base, data + 52);
  if (!(size > Lfs(base, 0x8200222C))) {
    // No drive: braked wheels stop and +2192 decays.
    const double zero = Lfs(base, kZero);
    const double decay = Lfs(base, 0x82002968);
    for (uint32_t i = 0; i < 2; i++) {
      const uint32_t wheel = guest::LoadU32(base, wheels + 4 * i);
      if (Lfs(base, wheel + 300) == zero) {
        continue;
      }
      Stfs(base, wheel + 248, zero);
      Stfs(base, wheel + 252, zero);
      const double mean = Lfs(base, v + 2192);
      Stfs(base, v + 2192, mean == zero ? zero : Fmuls(mean, decay));
    }
    return;
  }
  const double zero = Lfs(base, kZero);
  const double bias =
      Lfs(base, v + 284) > zero ? Lfs(base, data + 260) : Lfs(base, data + 264);
  const double torque = Fmuls(inertia, drive);
  const double a_slip = Fabs(Lfs(base, a + 332));
  const double preload = Lfs(base, data + 268);
  const double b_slip = Fabs(Lfs(base, b + 332));
  const double lock = Fmadds(Fabs(torque), bias, preload);
  double a_accel, b_accel;
  if (a_slip == zero && b_slip == zero) {
    a_accel = Fmuls(drive, half);
    b_accel = a_accel;
  } else {
    const double a_reaction = Lfs(base, a + 352);
    const double sum = Fadds(b_slip, a_slip);
    const double b_reaction = Lfs(base, b + 352);
    const double t = Fmadds(a_reaction, a_slip, Fmuls(Fmuls(b_slip, inertia), drive));
    a_accel = Fdivs(Fdivs(Fnmsubs(b_reaction, b_slip, t), inertia), sum);
    b_accel = Fsubs(drive, a_accel);
  }
  const double a_reaction = Lfs(base, a + 352);
  const double b_reaction = Lfs(base, b + 352);
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
    const double step = Lfs(base, v + 2364);
    const double a_spin = Lfs(base, a + 248);
    const double b_spin = Lfs(base, b + 248);
    guest::StoreU32(base, record + 116, 1);
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
      const double step = Lfs(base, v + 2364);
      const uint32_t wheel = guest::LoadU32(base, wheels + 4 * i);
      const double acceleration = Lfs(base, accel + 4 * i);
      const double brake = Lfs(base, wheel + 300);
      const double spin = Lfs(base, wheel + 248);
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
    const double held = Fmuls(Fabs(Lfs(base, wheel + 300)), grip);
    const double old_spin = Lfs(base, wheel + 248);
    if (is_locked != 0) {
      Stfs(base, wheel + 248, zero);
    } else {
      Stfs(base, wheel + 248, Fmadds(Lfs(base, accel + 4 * i), Lfs(base, v + 2364),
                                     Lfs(base, wheel + 248)));
    }
    Stfs(base, wheel + 252, Fmuls(Lfs(base, wheel + 248), inertia));
    const double applied = Fabs(Fmuls(Lfs(base, accel + 4 * i), inertia));
    if (held > zero && !(old_spin == zero)) {
      const double change = Fdivs(Fsubs(Lfs(base, wheel + 248), old_spin), Lfs(base, v + 2364));
      const double share = Lfs(base, wheel + 216);
      Stfs(base, wheel + 212,
           Fabs(Fmuls(Fdivs(Fmsubs(change, inertia, Fsubs(applied, held)), grip), share)));
    }
  }
  const double mean = Lfs(base, v + 2192);
  const double spins = Fadds(Lfs(base, b + 248), Lfs(base, a + 248));
  const double next = mean == zero ? spins : Fmadds(spins, half, mean);
  Stfs(base, v + 2192, Fmuls(next, half));
}

void VehicleUpdateDrivetrain(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B4778 Vehicle::UpdateDrivetrain");
  const uint32_t v = vehicle, f = frame;
  const uint32_t e = CarData(base, v) + 1268;  // the engine
  CopyWord(base, v + 2196, v + 2200);
  const int32_t gear = int32_t(guest::LoadU32(base, v + 2252));
  double ratio = Lfs(base, v + 2304);
  const double zero = Lfs(base, kZero);
  // The clutch squared (0.0 in neutral or with the clutch at 0.0); the
  // ratio negated in reverse.
  double clutch = zero;
  if (gear != 0) {
    const double c = Lfs(base, v + 2264);
    if (!(c == zero)) {
      if (gear == -1) {
        ratio = Fneg(ratio);
      }
      clutch = Fmuls(c, c);
    }
  }
  const uint32_t drive = guest::LoadU32(base, v + 2376);
  const double half = Lfs(base, 0x82001BE4);
  // The driven axles' reaction torques (AxleReactionTorque) and, with one,
  // its half inertia; with both, the halves at the frame's +80 and +84.
  double front = 0.0, rear = 0.0, reaction = 0.0, inertia = 0.0;
  bool both = false;
  if (drive >= 2) {
    front = VehicleAxleReactionTorque(base, v, 0);
    rear = VehicleAxleReactionTorque(base, v, 2);
    both = true;
    const uint32_t front_record = guest::LoadU32(base, v + 2144);
    const uint32_t rear_record = guest::LoadU32(base, v + 2152);
    const double rear_inertia = Lfs(base, rear_record + 108);
    Stfs(base, f + 80, Fmuls(Lfs(base, front_record + 108), half));
    Stfs(base, f + 84, Fmuls(rear_inertia, half));
  } else {
    uint32_t record;
    if (drive == 1) {
      VehicleUpdateFreeAxle(base, v, 0);
      reaction = VehicleAxleReactionTorque(base, v, 2);
      record = guest::LoadU32(base, v + 2152);
    } else {
      reaction = VehicleAxleReactionTorque(base, v, 0);
      VehicleUpdateFreeAxle(base, v, 2);
      record = guest::LoadU32(base, v + 2144);
    }
    inertia = Fmuls(Lfs(base, record + 108), half);
  }
  const double engine_inertia = Fadds(Lfs(base, e + 292), Lfs(base, e + 280));
  const double mean = Lfs(base, v + 2192);
  const double torque = Lfs(base, v + 2240);
  const double step = Lfs(base, v + 2364);
  const double two = Lfs(base, 0x820021E8);
  const double momentum = Lfs(base, v + 2212);
  const double scaled_mean = Fmuls(mean, two);
  Stfs(base, v + 2192, zero);
  const double damping = Lfs(base, 0x820021F8);
  const double one = Lfs(base, kOne);
  const double slip = Fsubs(one, clutch);
  double engine_spin;  // f26
  if (both) {
    const double rear_half = Lfs(base, f + 84);
    const uint32_t rear_record = guest::LoadU32(base, v + 2152);
    const double twice_mean = Fmuls(scaled_mean, two);
    const double front_half = Lfs(base, f + 80);
    const uint32_t front_record = guest::LoadU32(base, v + 2144);
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
  spin = Fmadds(spin, Lfs(base, v + 2364), Lfs(base, v + 2212));
  Stfs(base, v + 2208, Fmuls(spin, engine_inertia));
  const double per_rpm = Lfs(base, e + 4);
  const double low = Fmuls(per_rpm, Lfs(base, e + 360));
  const double scale = Lfs(base, 0x820029A8);
  const double high = Fmuls(per_rpm, scale);
  const double low_spin = Fmuls(low, scale);
  if (spin < low_spin) {
    spin = low_spin;
  } else if (high < spin) {
    spin = high;
  }
  double rpm = Fmuls(Fdivs(spin, Lfs(base, e + 4)), Lfs(base, 0x820029A4));
  Stfs(base, v + 2228, rpm);
  const double idle = Lfs(base, e + 360), limit = Lfs(base, e + 16);
  if (rpm < idle) {
    rpm = idle;
  } else if (limit < rpm) {
    rpm = limit;
  }
  Stfs(base, v + 2228, rpm);
  const double engine = Fmuls(Lfs(base, e + 4), rpm);
  Stfs(base, v + 2220, engine);
  const double old = Lfs(base, v + 2224);
  const double next_momentum = Fmuls(engine, scale);
  const double smoothed = Fmadds(Lfs(base, v + 2220), damping, old);
  Stfs(base, v + 2212, next_momentum);
  const uint64_t bits = (uint64_t(guest::LoadU32(base, 0x82002A58)) << 32) |
                        guest::LoadU32(base, 0x82002A5C);
  Stfs(base, v + 2224, Fmuls(smoothed, std::bit_cast<double>(bits)));
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
  const uint32_t e = CarData(base, v) + 1268;  // the gearbox (r6)
  const uint32_t r4 = manual & 0xFF;
  double f0 = 0.0, f5 = 0.0, f6 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0,
         f13 = 0.0;
  int32_t r7 = 0;
  uint32_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  guest::StoreU8(base, v + 2281, 0);
  guest::StoreU8(base, v + 2282, 0);
  const double f4 = Lfs(base, kOne);
  const double f7 = Lfs(base, kZero);
  if (int32_t(guest::LoadU32(base, e)) == 1) {
    // One gear (probably a CVT): +2288 between e +300 and e +328.
    if (int32_t(guest::LoadU32(base, v + 2252)) <= 0) {
      f0 = Lfs(base, e + 300);
      goto L4EA4;
    }
    f13 = Lfs(base, v + 284);
    if (f13 == f7) {
      f12 = Lfs(base, v + 2220);
      f0 = Lfs(base, 0x82002A84);
      if (f12 < f0) {
        f0 = Lfs(base, e + 300);
        f12 = f0;
        goto L4D84;
      }
    }
    f0 = Lfs(base, e + 300);
    f13 = Fsubs(f4, f13);
    f12 = Lfs(base, e + 328);
    f12 = Fsubs(f12, f0);
    f12 = Fmadds(f12, f13, f0);
  L4D84:
    f10 = Lfs(base, e + 328);
    if (f12 < f10) {
      f0 = f10;
    } else if (!(f0 < f12)) {
      f0 = f12;
    }
    f11 = Lfs(base, v + 2192);
    f13 = f0;
    if (f11 > f4) {
      f9 = Lfs(base, e + 12);
      f8 = Lfs(base, e + 332);
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
    f0 = Fmuls(Lfs(base, e + 332), f11);
    f12 = Lfs(base, e + 360);
    f11 = Lfs(base, e + 4);
    f0 = Fabs(f0);
    f0 = Fdivs(f12, f0);
    f12 = Fmuls(f0, f11);
    f0 = Fmuls(f12, Lfs(base, 0x82002A7C));
    r11 = f0 > f13 ? 1 : 0;
    f0 = StackIntegerFloat(base, f + 80, int32_t(r11));
    if (!(f0 == f7)) {
      f13 = f0;
    }
    f0 = Lfs(base, v + 2288);
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
    Stfs(base, v + 2288, f0);
    f11 = Lfs(base, e + 332);
    f12 = Lfs(base, v + 2336);
    f0 = Lfs(base, v + 2328);
    CopyWord(base, v + 2328, v + 2324);
    Stfs(base, v + 2312, Fadds(f12, f0));
    Stfs(base, v + 2320, f7);
    Stfs(base, v + 2304, Fabs(Fmuls(f11, f13)));
    Stfs(base, v + 2308, Fmuls(Lfs(base, e + 276), Lfs(base, e + 236)));
  }
  if (guest::LoadU8(base, v + 2384) != 0) {
    Stfs(base, v + 2264, f7);
    guest::StoreU32(base, v + 2252, 0);
    goto L5560;
  }
  f5 = Lfs(base, v + 288);
  if (!(f5 == f7) && int32_t(guest::LoadU32(base, v + 2252)) == -1 &&
      Lfs(base, v + 2264) == f4) {
    goto L5560;
  }
  f6 = Lfs(base, v + 292);
  if (!(f6 == f7) && !(Lfs(base, v + 284) == f7) &&
      int32_t(guest::LoadU32(base, v + 2252)) == 0) {
    goto L4F44;
  }
  // The shift timer +2268.
  f0 = Lfs(base, v + 2268);
  if (f0 > f7) {
    f0 = Fsubs(f0, Lfs(base, v + 2352));
    f12 = Lfs(base, v + 2220);
    Stfs(base, v + 2268, f0);
    f13 = Lfs(base, 0x82001FD4);
    if (f12 < f13 && Lfs(base, v + 284) == f7 && f5 == f7 &&
        guest::LoadU32(base, v + 2256) == 0) {
      Stfs(base, v + 2268, f7);
      guest::StoreU8(base, v + 2276, 0);
      guest::StoreU8(base, v + 2277, 0);
      guest::StoreU32(base, v + 2252, 0);
      goto L555C;
    }
    if (f0 > f7) {
      // Still shifting.
      r10 = guest::LoadU32(base, v + 2256);
      if (r10 == 0) {
        Stfs(base, v + 2264, Fnmsubs(Lfs(base, e + 400), f0, f4));
        goto L5560;
      }
      r11 = guest::LoadU32(base, v + 2252);
      if (int32_t(r10) > int32_t(r11)) {
        f0 = Fmuls(Lfs(base, e + 12), Lfs(base, 0x82002A6C));
        if (!(Lfs(base, v + 2212) > f0)) {
          goto L5560;
        }
        goto L503C;
      }
      if (!(int32_t(r10) < int32_t(r11))) {
        goto L4FC4;
      }
      f0 = Lfs(base, e + 332);
      f13 = Lfs(base, v + 2192);
      f12 = Lfs(base, v + 2212);
      f11 = Lfs(base, e + 4 * (r11 + 74));
      f0 = Fmuls(f13, Fabs(Fmuls(f11, f0)));
      if (!(f12 < f0)) {
        goto L5560;
      }
    L503C:
      Stfs(base, v + 2268, f7);
      goto L555C;
    }
    Stfs(base, v + 2268, f7);
    guest::StoreU8(base, v + 2276, 0);
    Stfs(base, v + 2264, f4);
    guest::StoreU8(base, v + 2277, 0);
  }
L4FC4:
  // The shift points: up at f9 ([+2632 + 4 gear]), down below f11.
  r7 = int32_t(guest::LoadU32(base, v + 2252));
  f9 = f7;
  f11 = Lfs(base, 0x82002A70);
  if (r7 > 0) {
    f9 = Lfs(base, v + 4 * uint32_t(r7 + 658));
    f13 = Lfs(base, v + 4 * uint32_t(r7 + 657));
    f11 = Fmuls(f13, Lfs(base, 0x82001070));
  }
  // The driven wheels' mean spin (f12) and mean slip (f0, of those whose
  // +268 isn't the float at 0x82001BE0).
  r5 = guest::LoadU32(base, v + 2376);
  r8 = 0;
  f8 = Lfs(base, 0x82001BE4);
  if (r5 < 1) {
    f13 = Fadds(Lfs(base, Wheel(v, 1) + 248), Lfs(base, Wheel(v, 0) + 248));
    f0 = f7;
    f10 = Lfs(base, 0x82001BE0);
    f12 = Fmuls(f13, f8);
    for (uint32_t w = 0; w < 2; w++) {
      f13 = Lfs(base, Wheel(v, w) + 268);
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
    f13 = Fadds(Lfs(base, Wheel(v, 3) + 248), Lfs(base, Wheel(v, 2) + 248));
    f0 = f7;
    f10 = Lfs(base, 0x82001BE0);
    f12 = Fmuls(f13, f8);
    for (uint32_t w = 2; w < 4; w++) {
      f13 = Lfs(base, Wheel(v, w) + 268);
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
    f12 = Lfs(base, Wheel(v, 2) + 248);
    f0 = Fadds(Lfs(base, Wheel(v, 3) + 248), f12);
    f12 = Lfs(base, Wheel(v, 1) + 248);
    f10 = Lfs(base, Wheel(v, 0) + 248);
    f13 = f7;
    f0 = Fadds(f0, f12);
    f12 = Fadds(f0, f10);
    f0 = Lfs(base, 0x82002968);
    f10 = Lfs(base, 0x82001BE0);
    f12 = Fmuls(f12, f0);
    for (uint32_t w = 0; w < 4; w++) {
      f0 = Lfs(base, Wheel(v, w) + 268);
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
  if (r7 > 0 && guest::LoadU8(base, v + 2286) == 0) {
    r10 = r4;
    r11 = 0;
    if (r10 == 0 && f12 > f9 && Lfs(base, v + 2264) == f4 && f0 < Lfs(base, 0x82001FBC)) {
      r11 = 1;
    }
    r9 = r11;
    r11 = 0;
    if (r10 == 0 && f12 < f11 && Lfs(base, v + 2264) == f4) {
      r11 = 1;
    }
    r10 = r11;
  }
  // The clutch: out while braking near idle on a request, else ramped in.
  f12 = Lfs(base, 0x82002204);
  if (!(f6 == f7) && r7 > 1 &&
      !(Lfs(base, v + 2228) > Fmuls(Lfs(base, e + 360), Lfs(base, 0x820021E8))) &&
      (r4 != 0 || guest::LoadU8(base, v + 2286) != 0)) {
    Stfs(base, v + 2264, f7);
  } else {
    f0 = Fadds(Lfs(base, v + 2264), f12);
    Stfs(base, v + 2264, f0);
    if (!(f0 < f4)) {
      Stfs(base, v + 2264, f4);
    }
  }
  if (r9 != 0 || (guest::LoadU8(base, v + 2284) != 0 && r7 > 0)) {
    goto L5438;
  }
  if (r10 != 0 || (guest::LoadU8(base, v + 2285) != 0 && r7 > 1)) {
    goto L5430;
  }
  if (!(f6 == f7) && !(Lfs(base, v + 284) == f7)) {
    if (r7 != 0) {
      goto L5390;
    }
    Stfs(base, v + 2264, f7);
    goto L5460;
  }
  if (r7 != 0 || Lfs(base, v + 284) == f7) {
    goto L5390;
  }
  // Pulling away from neutral.
  Stfs(base, v + 2260, f7);
  guest::StoreU32(base, v + 2256, 0);
  guest::StoreU32(base, v + 2252, 1);
  f13 = Fmuls(Lfs(base, e + 16), f8);
  r8 = 1;
  if (Lfs(base, v + 2228) > f13) {
    guest::StoreU8(base, v + 2277, 1);
    guest::StoreU8(base, v + 2276, 0);
  } else {
    guest::StoreU8(base, v + 2277, 0);
    guest::StoreU8(base, v + 2276, 1);
  }
  goto L5464;
L5390:
  // Reverse, neutral.
  f13 = Lfs(base, v + 384);
  if ((f13 < f4 || r7 == 0) && !(f5 == f7)) {
    if (r7 == -1) {
      goto L5464;
    }
    guest::StoreU32(base, v + 2252, uint32_t(-1));
    goto L5458;
  }
  if (Lfs(base, v + 2264) == f4 && r7 != 0) {
    f0 = Fmuls(Lfs(base, e + 360), Lfs(base, 0x820027A8));
    if (Lfs(base, v + 2228) < f0 &&
        (Lfs(base, v + 284) == f7 || (r7 == -1 && f5 == f7))) {
      guest::StoreU32(base, v + 2252, 0);
      goto L5458;
    }
  }
  if (!(f13 < f10) || r7 != 1) {
    goto L5464;
  }
  guest::StoreU32(base, v + 2256, 1);
  guest::StoreU32(base, v + 2252, 0);
  goto L545C;
L5430:
  r10 = uint32_t(r7 - 1);
  goto L5448;
L5438:
  if (!(r7 < int32_t(guest::LoadU32(base, e)))) {
    goto L5464;
  }
  r10 = uint32_t(r7 + 1);
L5448:
  guest::StoreU32(base, v + 2252, r10);
  CopyWord(base, 0x82002208, v + 2272);
L5458:
  guest::StoreU32(base, v + 2256, uint32_t(r7));
L545C:
  r8 = 1;
L5460:
  Stfs(base, v + 2260, f7);
L5464:
  if (r8 == 1) {
    // A shift: the timer and the car data's rate, then the gear values.
    const uint32_t up = guest::LoadU8(base, v + 2276);
    if (up != 0 || guest::LoadU8(base, v + 2277) != 0) {
      if (up != 0 && guest::LoadU32(base, v + 2256) == 0) {
        f0 = Lfs(base, 0x8200296C);
      } else {
        f0 = Lfs(base, e + 364);
      }
      Stfs(base, v + 2268, f0);
      Stfs(base, e + 400, Fdivs(f4, f0));
    } else {
      const uint32_t gear = guest::LoadU32(base, v + 2252);
      Stfs(base, v + 2264, f7);
      if (gear == 0) {
        Stfs(base, v + 2268, f7);
      } else {
        const uint32_t old = guest::LoadU32(base, v + 2256);
        f0 = Lfs(base, e + 4 * (old + 91));
        CopyWord(base, e + 4 * (old + 91), v + 2268);
        Stfs(base, e + 4 * (old + 100), Fdivs(f4, f0));
      }
    }
    VehicleSetGearValues(base, v);
    guest::StoreU8(base, v + 2281, 1);
    guest::StoreU8(base, v + 2282, 1);
    goto L5560;
  }
  if (r5 == 0 || int32_t(guest::LoadU32(base, v + 2252)) <= 0) {
    goto L5560;
  }
  if (Lfs(base, v + 296) > f7 && Lfs(base, v + 284) == f7) {
    goto L4F44;
  }
  f0 = Fadds(Lfs(base, v + 2264), f12);
  Stfs(base, v + 2264, f0);
  if (f0 < f4) {
    goto L5560;
  }
L555C:
  Stfs(base, v + 2264, f4);
  goto L5560;
L4F44:
  Stfs(base, v + 2264, f7);
L5560:
  guest::StoreU8(base, v + 2285, 0);
  guest::StoreU8(base, v + 2284, 0);
  guest::StoreU8(base, v + 2286, 0);
}

void VehicleShiftGears(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6430 Vehicle::ShiftGears");
  const uint32_t v = vehicle;
  uint32_t manual = 0;
  if (int32_t(guest::LoadU32(base, CarData(base, v) + 1268)) != 1 &&
      guest::LoadU8(base, v + 2249) == 0) {
    manual = 1;
  }
  VehicleUpdateGearbox(base, v, manual, frame - 112);
  const double sign = int32_t(guest::LoadU32(base, v + 2252)) == -1 ? Lfs(base, 0x82001BE0)
                                                                     : Lfs(base, kOne);
  Stfs(base, v + 2348, sign);
}

}  // namespace car_physics
