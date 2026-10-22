// The vehicle's physics step's helpers (docs/functions/car-physics.md): see
// car_physics.h. Wheel w's 368-byte record is at vehicle + 416 + 368 w.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <climits>
#include <cmath>

#include <simde/x86/sse2.h>

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kWheelBytes = 368;

uint32_t Wheel(uint32_t vehicle, uint32_t w) {
  return vehicle + 416 + kWheelBytes * w;
}

uint32_t CarData(uint8_t* base, uint32_t vehicle) {
  return guest::LoadU32(base, vehicle + 272);
}

// The surface record of wheel w (its kind the word +176): [vehicle + 4
// (kind + 667)] for the front wheels, + 699 for the rear (32-bit
// arithmetic, as the original); 0 when byte +2664 is clear (none looked up).
uint32_t SurfaceRecord(uint8_t* base, uint32_t vehicle, uint32_t w, bool* looked_up) {
  *looked_up = guest::LoadU8(base, vehicle + 2664) != 0;
  if (!*looked_up) {
    return 0;
  }
  const uint32_t kind = guest::LoadU32(base, Wheel(vehicle, w) + 176);
  const uint32_t index = kind + (w == 0 || w == 1 ? 667 : 699);
  return guest::LoadU32(base, vehicle + (index << 2));
}

// The surface getters' shared steps: f1 = the default, then the record's
// float at `field` when there is one; its word stored at wheel + `out` when
// `out` (an lfs then stfs: its bits).
double SurfaceValue(uint8_t* base, uint32_t vehicle, uint32_t w, uint32_t fallback,
                    uint32_t field, uint32_t out) {
  uint32_t from = fallback;
  bool looked_up = false;
  const uint32_t record = SurfaceRecord(base, vehicle, w, &looked_up);
  if (looked_up && record != 0) {
    from = record + field;
  }
  if (out) {
    CopyWord(base, from, Wheel(vehicle, w) + out);
  }
  return Lfs(base, from);
}

}  // namespace

uint32_t VehicleBeginStep(uint8_t* base, uint32_t vehicle, double dt) {
  ITC_NATIVE_RUNNING("sub_820AFAE8 Vehicle::BeginStep");
  const double rate = Fdivs(Lfs(base, kOne), dt);
  Stfs(base, vehicle + 2352, dt);
  Stfs(base, vehicle + 2356, rate);
  for (uint32_t i = 0; i < 5; i++) {
    vl::Vec4Copy(base, vehicle + 2544 + 16 * i, kZeroVector);
  }
  CopyWord(base, kZero, vehicle + 2448);
  CopyWord(base, kZero, vehicle + 2452);
  for (uint32_t w = 0; w < 4; w++) {
    guest::StoreU32(base, guest::LoadU32(base, vehicle + 2144 + 4 * w) + 116, 0);
  }
  guest::StoreU32(base, vehicle + 3156, guest::LoadU32(base, vehicle + 3156) + 1);
  return vehicle + 2544 + 64;
}

void VehicleWheelLoads(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820AFD30 Vehicle::WheelLoads");
  if (guest::LoadU32(base, vehicle + 348) == 0) {
    for (uint32_t w = 0; w < 4; w++) {
      CopyWord(base, kZero, Wheel(vehicle, w) + 280);
    }
    return;
  }
  const double zero = Lfs(base, kZero);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (guest::LoadU8(base, wheel + 17) == 0) {
      Stfs(base, wheel + 280, zero);
      continue;
    }
    // max(+8, 0.0), a NaN kept too (bge: not less than; the original's
    // second check never stores). Through the double, as the translated
    // code (a signaling NaN comes out quiet).
    const double load = Lfs(base, wheel + 8);
    if (!(load < zero)) {
      Stfs(base, wheel + 280, load);
    } else {
      Stfs(base, wheel + 280, zero);
    }
  }
}

double VehicleSurfaceValue(uint8_t* base, uint32_t vehicle, uint32_t w) {
  ITC_NATIVE_RUNNING("sub_820B0508 Vehicle::SurfaceValue");
  return SurfaceValue(base, vehicle, w, kZero, 0, 0);
}

double VehicleSurfaceValue20(uint8_t* base, uint32_t vehicle, uint32_t w) {
  ITC_NATIVE_RUNNING("sub_820B0560 Vehicle::SurfaceValue20");
  return SurfaceValue(base, vehicle, w, kZero, 20, 196);
}

double VehicleSurfaceValue12(uint8_t* base, uint32_t vehicle, uint32_t w) {
  ITC_NATIVE_RUNNING("sub_820B05B8 Vehicle::SurfaceValue12");
  return SurfaceValue(base, vehicle, w, kOne, 12, 184);
}

double VehicleSurfaceValue16(uint8_t* base, uint32_t vehicle, uint32_t w) {
  ITC_NATIVE_RUNNING("sub_820B0610 Vehicle::SurfaceValue16");
  return SurfaceValue(base, vehicle, w, kOne, 16, 188);
}

uint32_t VehicleAccumulateTireForces(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B0908 Vehicle::AccumulateTireForces");
  const uint32_t f = frame;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    const uint32_t accumulator = vehicle + 2544 + 16 * w;
    {
      const double dt = Lfs(base, vehicle + 2352);
      const double time = Lfs(base, vehicle + 2364);
      const double k = Fdivs(time, dt);
      const double v284 = Lfs(base, wheel + 284);
      const double v308 = Lfs(base, wheel + 308);
      const double v288 = Lfs(base, wheel + 288);
      const double v304 = Lfs(base, wheel + 304);
      Stfs(base, wheel + 284, Fmadds(k, v308, v284));
      const double time2 = Lfs(base, vehicle + 2364);
      const double dt2 = Lfs(base, vehicle + 2352);
      const double k2 = Fdivs(time2, dt2);
      Stfs(base, wheel + 288, Fmadds(k2, v304, v288));
      // (+128 x +304 + +144 x +308) x k into the accumulator.
      vl::Vec4Scaled(base, f + 96, wheel + 128, v304);
    }
    vl::Vec4Scaled(base, f + 112, wheel + 144, Lfs(base, wheel + 308));
    vl::Vec3Sum(base, f + 128, f + 112, f + 96);
    const double k = Fdivs(Lfs(base, vehicle + 2364), Lfs(base, vehicle + 2352));
    vl::Vec4Scaled(base, f + 80, f + 128, k);
    vl::Vec3AddTo(base, accumulator, f + 80);
  }
  return vehicle + 2544 + 48;
}

void VehicleRollingResistance(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B2748 Vehicle::RollingResistance");
  if (guest::LoadU32(base, vehicle + 348) == 0) {
    return;
  }
  const double zero = Lfs(base, kZero);
  const double half = Lfs(base, 0x82001BE4);
  const double scale = Lfs(base, 0x82002A18);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (guest::LoadU8(base, wheel + 17) == 0) {
      continue;
    }
    const uint32_t spin_at = wheel + 248;
    const double load = Lfs(base, wheel + 280);
    const uint32_t data = guest::LoadU32(base, vehicle + 2160 + 4 * w);
    double drag = Fmuls(load, scale);
    const uint32_t record = guest::LoadU32(base, vehicle + 2144 + 4 * w);
    double time = Lfs(base, vehicle + 2364);
    const double spin = Lfs(base, spin_at);
    const double radius = Lfs(base, data + 52);
    double inertia = Lfs(base, record + 108);
    inertia = Fmuls(inertia, half);
    drag = Fmuls(radius, drag);
    time = Fdivs(time, inertia);
    drag = Fabs(drag);
    drag = Fmuls(time, drag);
    // The spin moves toward 0.0 by `drag`, not past it (the branches as
    // the original's: bge and ble are "not less" and "not greater", NaNs
    // taking them).
    if (!(spin < zero)) {
      if (!(spin > zero)) {
        continue;
      }
      const double moved = Fsubs(spin, drag);
      Stfs(base, spin_at, moved);
      if (moved < zero) {
        Stfs(base, spin_at, zero);
      }
    } else {
      const double moved = Fadds(spin, drag);
      Stfs(base, spin_at, moved);
      if (moved > zero) {
        Stfs(base, spin_at, zero);
      }
    }
  }
}

void VehicleTurnWheels(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AFDA8 Vehicle::TurnWheels");
  const double wrap_low = Lfs(base, 0x820029C4);
  const double wrap_high = Lfs(base, 0x82002240);
  const double turn = Lfs(base, 0x82001FA8);
  const double per_turn = Lfs(base, 0x82001FA4);
  const double low_slope = Lfs(base, 0x820029C0);
  const double low_limit = Lfs(base, 0x820029BC);
  const double high_slope = Lfs(base, 0x820029B8);
  const double high_limit = Lfs(base, 0x820029B4);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t angle_at = Wheel(vehicle, w) + 28;
    double d = Fmuls(Lfs(base, angle_at + 220), Lfs(base, vehicle + 2352));
    d = Fneg(d);
    // The step reshaped past the limits.
    if (d > high_limit) {
      d = Fnmsubs(d, high_slope, high_limit);
    } else if (d < low_limit) {
      d = Fmsubs(d, low_slope, high_limit);
    }
    double angle = Fadds(Lfs(base, angle_at), d);
    // Wrapped to a turn: angle - trunc(angle x per_turn) x turn (fctiwz,
    // through the stack as the original: stfiwx at r1 - 32, the
    // sign-extended doubleword at r1 - 24).
    const double scaled = Fmuls(angle, per_turn);
    int32_t whole;
    if (std::isnan(scaled)) {
      whole = INT32_MIN;
    } else if (scaled >= double(INT_MAX)) {
      whole = INT_MAX;
    } else {
      whole = simde_mm_cvttsd_si32(simde_mm_set_sd(scaled));
    }
    guest::StoreU32(base, frame - 32, uint32_t(whole));
    guest::StoreU32(base, frame - 24, whole < 0 ? 0xFFFFFFFFu : 0u);
    guest::StoreU32(base, frame - 20, uint32_t(whole));
    const double turns = double(float(double(whole)));
    angle = Fnmsubs(turns, turn, angle);
    Stfs(base, angle_at, angle);
    if (angle > wrap_high) {
      Stfs(base, angle_at, Fsubs(angle, turn));
    } else if (angle < wrap_low) {
      Stfs(base, angle_at, Fadds(angle, turn));
    }
  }
}

void VehicleBrakeTorques(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B0838 Vehicle::BrakeTorques");
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    CopyWord(base, kZero, wheel + 212);
    CopyWord(base, kZero, wheel + 216);
    CopyWord(base, kZero, wheel + 300);
  }
  const double handbrake = Lfs(base, vehicle + 296);
  const double brake = Lfs(base, vehicle + 292);
  if (Fadds(brake, handbrake) == Lfs(base, kZero)) {
    return;
  }
  const uint32_t data = CarData(base, vehicle);
  double front = Fmuls(Lfs(base, data + 464), Lfs(base, data + 264));
  const double rear_scale = Lfs(base, data + 736);
  double rear_handbrake = Fmuls(Lfs(base, data + 272), rear_scale);
  const double rear = Fmuls(Lfs(base, data + 268), rear_scale);
  rear_handbrake = Fmuls(rear_handbrake, handbrake);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    const double x = Lfs(base, vehicle + 2432);
    double torque;
    if (w >= 2) {
      torque = Fmuls(Lfs(base, wheel + 200), x);
      const double braking = Fmuls(torque, rear);
      torque = Fadds(braking, rear_handbrake);
      Stfs(base, wheel + 216, Fdivs(braking, torque));
    } else {
      torque = Fmuls(Lfs(base, wheel + 200), x);
      CopyWord(base, kOne, wheel + 216);
      torque = Fmuls(torque, front);
    }
    Stfs(base, wheel + 300, torque);
  }
}

void VehicleAntiLock(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B0718 Vehicle::AntiLock");
  guest::StoreU8(base, vehicle + 2401, 0);
  for (uint32_t w = 0; w < 4; w++) {
    CopyWord(base, vehicle + 292, Wheel(vehicle, w) + 200);
  }
  if (guest::LoadU8(base, vehicle + 2386) == 0) {
    return;
  }
  const double kmh = Fmuls(Lfs(base, vehicle + 48), Lfs(base, 0x820029E0));
  if (kmh < Lfs(base, 0x82001FD8)) {
    return;
  }
  const double zero = Lfs(base, kZero);
  if (!(Lfs(base, vehicle + 292) > zero)) {
    return;
  }
  const double one = Lfs(base, kOne);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (guest::LoadU8(base, wheel + 17) == 0) {
      continue;
    }
    const double grip = Lfs(base, vehicle + (w == 0 || w == 1 ? 2000 : 2128));
    const double limit = Fsubs(one, grip);
    const double slip = Lfs(base, wheel + 268);
    bool release = false;
    if (slip < zero) {
      const double slip2 = Fmuls(slip, slip);
      const double other = Lfs(base, wheel + 264);
      const double length = double(vl::Sqrtf(Fmadds(other, other, slip2)));
      release = length > limit;
    }
    if (release) {
      // The brake released on this wheel, counted.
      Stfs(base, wheel + 200, zero);
      guest::StoreU32(base, wheel + 204, guest::LoadU32(base, wheel + 204) + 1);
    } else {
      CopyWord(base, vehicle + 292, wheel + 200);
    }
  }
}

uint32_t VehicleSuspensionTorques(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AFBD8 Vehicle::SuspensionTorques");
  const uint32_t f = frame;
  const double zero = Lfs(base, kZero);
  uint32_t r3 = vehicle;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (guest::LoadU8(base, wheel + 17) == 0) {
      continue;
    }
    double v = Lfs(base, wheel + 284);
    if (w >= 2) {
      const uint32_t data = CarData(base, vehicle);
      const double k = Lfs(base, data + (!(v < zero) ? 1828 : 1824));
      v = Fmuls(k, v);
    } else {
      const double along = double(vl::Vec3DotOf(base, vehicle + 112, wheel + 144));
      const double t = Fmuls(along, v);
      const uint32_t data = CarData(base, vehicle);
      v = Fneg(Fmuls(t, Lfs(base, data + 1820)));
    }
    vl::Vec4Scaled(base, f + 96, vehicle + 96, v);
    vl::Vec3AddTo(base, vehicle + 2544 + 16 * w, f + 96);
    const double moved = Fadds(Lfs(base, wheel + 8), v);
    guest::StoreU32(base, f + 80, guest::LoadU32(base, kZero));
    CopyWord(base, kOne, f + 84);
    CopyWord(base, kZero, f + 88);
    CopyWord(base, kZero, f + 92);
    Stfs(base, wheel + 8, moved);
    vl::Vec4Scaled(base, f + 128, f + 80, moved);
    const double up = double(vl::Vec3DotOf(base, wheel + 112, f + 128));
    vl::Vec4Scaled(base, f + 144, wheel + 112, up);
    vl::Vec4Scaled(base, f + 160, f + 80, Lfs(base, wheel + 8));
    vl::Vec3Difference(base, f + 112, f + 160, f + 144);
    const uint32_t body = guest::LoadU32(base, vehicle + 336);
    va::RigidBodyAddTorqueAt(base, body, f + 112, wheel + 160);
    r3 = body + 128;
  }
  return r3;
}

}  // namespace car_physics
