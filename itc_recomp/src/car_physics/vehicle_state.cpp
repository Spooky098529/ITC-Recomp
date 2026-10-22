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

}  // namespace car_physics
