// The vehicle's physics step's updates (docs/functions/car-physics.md): see
// car_physics.h. Wheel w's 368-byte record is at vehicle + 416 + 368 w.

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

// fcfid then frsp of a 64-bit integer stored at `at` (the original's std
// then lfd): the integer's float.
double IntegerFloat(uint8_t* base, uint32_t at, int64_t value) {
  guest::StoreU32(base, at, uint32_t(uint64_t(value) >> 32));
  guest::StoreU32(base, at + 4, uint32_t(value));
  return double(float(double(value)));
}

}  // namespace

void MatrixCopyRows(uint8_t* base, uint32_t out, uint32_t in) {
  ITC_NATIVE_RUNNING("sub_820AFB78 Matrix::CopyRows");
  for (uint32_t row = 0; row < 64; row += 16) {
    vl::Vec4Copy(base, out + row, in + row);
  }
}

void MatrixMultiplyTranslation(uint8_t* base, uint32_t out, uint32_t m, uint32_t vector,
                               uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B0D68 Matrix::MultiplyTranslation");
  const uint32_t f = frame;
  // z, y, x and 1.0 at +80 (each an lfs then stfs: its bits).
  CopyWord(base, vector + 8, f + 80);
  CopyWord(base, vector + 4, f + 84);
  CopyWord(base, vector + 0, f + 88);
  CopyWord(base, kOne, f + 92);
  // The rows: the identity's three, then (x, y, z, 1.0) (the vector unit's
  // shuffles: the words' bits).
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 3; row++) {
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreU32(base, f + 112 + 16 * row + 4 * i, row == i ? kOneBits : 0);
    }
  }
  guest::StoreU32(base, f + 160, guest::LoadU32(base, f + 88));
  guest::StoreU32(base, f + 164, guest::LoadU32(base, f + 84));
  guest::StoreU32(base, f + 168, guest::LoadU32(base, f + 80));
  guest::StoreU32(base, f + 172, guest::LoadU32(base, f + 92));
  mm::Multiply(base, out, m, f + 112);
  interpolation::BlendState_OnMultiply(base, out, m, f + 112, 0x820B0E00);
}

void VehicleTractionControl(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B1790 Vehicle::TractionControl");
  const double throttle = Lfs(base, vehicle + 284);
  guest::StoreU8(base, vehicle + 2400, 0);
  const double half = Lfs(base, 0x82001BE4);
  // The driven pair: the rear when the front's share is below 0.5.
  const uint32_t first = Lfs(base, CarData(base, vehicle) + 180) < half ? 2 : 0;
  const uint32_t a = Wheel(vehicle, first), b = Wheel(vehicle, first + 1);
  const double limit = Lfs(base, vehicle + 2396);
  const double slip = Fmuls(Fadds(Lfs(base, b + 268), Lfs(base, a + 268)), half);
  const double spin = Fabs(Fadds(Lfs(base, b + 248), Lfs(base, a + 248)));
  bool rising = false;
  if (slip > limit) {
    const double threshold = Lfs(base, 0x82000D44);
    if (Fmuls(spin, half) > threshold) {
      rising = true;
      const int32_t count = int32_t(guest::LoadU32(base, vehicle + 2392) + 1);
      guest::StoreU32(base, vehicle + 2392, uint32_t(count));
      // The count against the same threshold (as the original).
      if (IntegerFloat(base, frame - 16, count) > threshold) {
        guest::StoreU32(base, vehicle + 2392, 30);
      }
    }
  }
  if (!rising) {
    const int32_t count = int32_t(guest::LoadU32(base, vehicle + 2392) - 1);
    guest::StoreU32(base, vehicle + 2392, uint32_t(count));
    if (count < 0) {
      guest::StoreU32(base, vehicle + 2392, 0);
    }
  }
  const double per = Lfs(base, 0x820029EC);
  const int32_t count = int32_t(guest::LoadU32(base, vehicle + 2392));
  const double counted = IntegerFloat(base, frame - 16, count);
  const double one = Lfs(base, kOne);
  const double rest = Fnmsubs(counted, per, one);
  const double zero = Lfs(base, kZero);
  double scale;
  if (rest < zero) {
    scale = zero;
  } else if (rest > one) {
    scale = one;
  } else {
    scale = rest;
  }
  const double cut = Fmuls(throttle, scale);
  Stfs(base, vehicle + 284, cut);
  if (!(throttle == cut)) {
    guest::StoreU8(base, vehicle + 2400, 1);
  }
}

void VehicleUpdateTireGrip(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B1FB8 Vehicle::UpdateTireGrip");
  const double low = Lfs(base, 0x82002A00);
  const double high = Lfs(base, 0x8200296C);
  const double one = Lfs(base, kOne);
  const double camber_scale = Lfs(base, 0x82002808);
  const double zero = Lfs(base, kZero);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (guest::LoadU8(base, wheel + 17) == 0) {
      Stfs(base, wheel + 236, zero);
      Stfs(base, wheel + 316, zero);
      continue;
    }
    const uint32_t record = guest::LoadU32(base, vehicle + 2144 + 4 * w);
    const uint32_t data = guest::LoadU32(base, vehicle + 2160 + 4 * w);
    const double surface = VehicleSurfaceValue12(base, vehicle, w);
    const double camber = Fadds(Lfs(base, vehicle + 88), Lfs(base, wheel + 32));
    const double rated = Fmuls(Lfs(base, data + 140), Lfs(base, data + 144));
    const double loss = Fabs(Fmuls(camber, camber_scale));
    double ratio = Fmuls(Fdivs(Lfs(base, wheel + 280), rated), high);
    const double kept = Fsubs(one, loss);
    if (ratio > high) {
      ratio = high;
    } else if (ratio < low) {
      ratio = low;
    }
    const double force = TireMagicFormula(base, 0x82600D20, ratio);
    double grip = Fmuls(Lfs(base, data + 148), force);
    grip = Fmuls(grip, kept);
    grip = Fmuls(grip, surface);
    if (grip < zero) {
      grip = zero;
    }
    grip = Fmuls(Lfs(base, record + 96), grip);
    Stfs(base, wheel + 236, grip);
    double limit = Fmuls(Lfs(base, data + 140), Lfs(base, data + 144));
    limit = Fmuls(limit, Lfs(base, vehicle + 2408));
    Stfs(base, wheel + 316, Fmuls(limit, grip));
  }
}

void VehicleUpdateEngineTorque(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B22D8 Vehicle::UpdateEngineTorque");
  const uint32_t data = CarData(base, vehicle);
  const int32_t gear = int32_t(guest::LoadU32(base, vehicle + 2252));
  const double limiter_rpm = Lfs(base, data + 1284);
  double throttle = Lfs(base, vehicle + (gear == -1 ? 288 : 284));
  const double zero = Lfs(base, kZero);
  // The rev limiter: its timer +2244 runs down, the throttle cut meanwhile;
  // set again at the limiter's rpm.
  const double timer = Lfs(base, vehicle + 2244);
  if (timer > zero) {
    const double left = Fsubs(timer, Lfs(base, vehicle + 2352));
    Stfs(base, vehicle + 2244, left);
    if (left < zero) {
      guest::StoreU8(base, vehicle + 2248, 0);
    }
    throttle = zero;
  } else if (!(Lfs(base, vehicle + 2228) < limiter_rpm)) {
    CopyWord(base, 0x82002204, vehicle + 2244);
    guest::StoreU8(base, vehicle + 2248, 1);
  } else {
    Stfs(base, vehicle + 2244, zero);
    guest::StoreU8(base, vehicle + 2248, 0);
  }
  // The clutch: no throttle while it's out, without the handbrake, between
  // gears.
  const double clutch = Lfs(base, vehicle + 2264);
  const double one = Lfs(base, kOne);
  if (!(clutch == one) && Lfs(base, vehicle + 296) == zero) {
    const int32_t next = int32_t(guest::LoadU32(base, vehicle + 2256));
    if (next != 0 && gear != 0 && next < gear) {
      throttle = zero;
    }
  }
  const double rpm = Lfs(base, vehicle + 2228);
  Stfs(base, vehicle + 2516, throttle);
  const double curve = VehicleEngineTorque(base, vehicle, rpm, frame);
  const double peak = Fmuls(Lfs(base, data + 1552), curve);
  double torque = Fmuls(Lfs(base, vehicle + 2412), peak);
  torque = Fmuls(torque, throttle);
  if (guest::LoadU8(base, vehicle + 2424) != 0) {
    // Nitrous above an rpm, while it lasts.
    const double from = Fmuls(Lfs(base, data + 1628), Lfs(base, 0x820029D4));
    bool stop = false;
    if (!(from < rpm)) {
      stop = true;
    } else {
      const double left = Lfs(base, vehicle + 2420);
      if (!(left == zero) && throttle > Lfs(base, 0x82001BE4)) {
        const double boost = Fmuls(Lfs(base, vehicle + 2416), throttle);
        const double rest = Fsubs(left, throttle);
        Stfs(base, vehicle + 2420, rest);
        torque = Fmuls(boost, torque);
        if (!(rest > zero)) {
          Stfs(base, vehicle + 2420, zero);
          stop = true;
        }
      }
    }
    if (stop) {
      guest::StoreU8(base, vehicle + 2424, 0);
    }
  }
  // Engine braking.
  const double off = Fsubs(one, throttle);
  const double idle = Lfs(base, data + 1628);
  torque = Fmuls(Lfs(base, vehicle + 2428), torque);
  const double above = Fsubs(rpm, idle);
  const double braking_scale = Lfs(base, data + 1556);
  const double range = Fsubs(limiter_rpm, idle);
  double braking = Fmuls(above, off);
  braking = Fmuls(braking, braking_scale);
  braking = Fmuls(braking, peak);
  const double net = Fsubs(torque, Fdivs(braking, range));
  Stfs(base, vehicle + 2240, net);
  Stfs(base, vehicle + 2236, net);
  if (gear != 0 && !(clutch == zero)) {
    Stfs(base, vehicle + 2236, net);
    if (gear != -1) {
      return;
    }
    Stfs(base, vehicle + 2236, Fmuls(net, Lfs(base, 0x82001BE0)));
    return;
  }
  Stfs(base, vehicle + 2236, net);
}

void VehicleUpdateBrakeHeat(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B35C8 Vehicle::UpdateBrakeHeat");
  const double speed = Lfs(base, vehicle + 48);
  const double speed2 = Fmuls(speed, speed);
  const double dt = Lfs(base, vehicle + 2352);
  const double by_speed = Lfs(base, 0x826C2D74);
  const double cooling_base = Lfs(base, 0x82600CD8);
  const double fade_max = Lfs(base, 0x82002A0C);
  const double fade_rate = Lfs(base, 0x82002208);
  const double zero = Lfs(base, kZero);
  const double cooling = Fmuls(Fmadds(speed2, by_speed, cooling_base), dt);
  const double ambient = Lfs(base, 0x82002568);
  // The heat added: kept from one braking wheel to the next.
  double added = zero;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (Lfs(base, vehicle + 292) > zero) {
      const uint32_t data = CarData(base, vehicle);
      if (Lfs(base, data + (w < 2 ? 264 : 268)) > zero) {
        double heat = Fmuls(Lfs(base, wheel + 248), Lfs(base, data + 388));
        heat = Fmuls(heat, Lfs(base, wheel + 212));
        heat = Fmuls(heat, Lfs(base, vehicle + 2352));
        added = Fmuls(heat, Lfs(base, 0x82600CD4));
      }
    }
    double t = Fadds(added, Lfs(base, wheel + 220));
    Stfs(base, wheel + 220, t);
    const double span = Fsubs(Lfs(base, CarData(base, vehicle) + 164), ambient);
    const double cool = Fdivs(Fmuls(Fsubs(t, ambient), cooling), span);
    t = Fsubs(t, cool);
    Stfs(base, wheel + 220, t);
    const double t_max = Lfs(base, CarData(base, vehicle) + 164);
    if (t < ambient) {
      t = ambient;
    } else if (t_max < t) {
      t = t_max;
    }
    Stfs(base, wheel + 220, t);
    if (!(Lfs(base, vehicle + 292) > zero) || !(added > cooling)) {
      continue;
    }
    const uint32_t data = CarData(base, vehicle);
    const double fade_from = Lfs(base, data + 168);
    if (!(t > fade_from)) {
      continue;
    }
    double fade = Fdivs(Fsubs(t, fade_from), Lfs(base, data + 164));
    fade = Fmadds(fade, fade_rate, Lfs(base, vehicle + 3232));
    Stfs(base, vehicle + 3232, fade);
    if (fade < zero) {
      fade = zero;
    } else if (fade > fade_max) {
      fade = fade_max;
    }
    Stfs(base, vehicle + 3232, fade);
  }
  double fade = Fnmsubs(Lfs(base, vehicle + 2352), Lfs(base, 0x82001BE4),
                        Lfs(base, vehicle + 3232));
  Stfs(base, vehicle + 3232, fade);
  if (fade < zero) {
    fade = zero;
  } else if (fade > fade_max) {
    fade = fade_max;
  }
  Stfs(base, vehicle + 3232, fade);
  Stfs(base, vehicle + 3236, fade);
}

void VehicleUpdateBackfire(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B3788 Vehicle::UpdateBackfire");
  const uint32_t engine = CarData(base, vehicle) + 1268;
  const double top = Lfs(base, engine + 16);
  const double from_rpm = Fmuls(top, Lfs(base, 0x82002A3C));
  const double half_rpm = Fmuls(top, Lfs(base, 0x82001BE4));
  const double zero = Lfs(base, kZero);
  if (guest::LoadU8(base, vehicle + 2512) == 0) {
    Stfs(base, vehicle + 2508, zero);
    guest::StoreU8(base, vehicle + 2504, 0);
    Stfs(base, vehicle + 2500, zero);
    return;
  }
  const double throttle = Lfs(base, vehicle + 2516);
  const double dt = Lfs(base, vehicle + 2352);
  double charge;
  // The charge builds on throttle at high rpm in a forward gear.
  if (throttle > Lfs(base, 0x82001070) && Lfs(base, vehicle + 2228) > half_rpm &&
      int32_t(guest::LoadU32(base, vehicle + 2252)) > 0 &&
      guest::LoadU8(base, vehicle + 3240) == 0) {
    const int64_t gear = int32_t(guest::LoadU32(base, vehicle + 2252));
    const int64_t count = int32_t(guest::LoadU32(base, vehicle + 2524));
    double rate = Lfs(base, 0x82002A38);
    if (guest::LoadU8(base, engine + 336) != 0) {
      rate = Fmuls(Lfs(base, engine + 352), rate);
      IntegerFloat(base, frame - 16, gear);
      IntegerFloat(base, frame - 8, count);
    } else {
      IntegerFloat(base, frame - 8, gear);
      IntegerFloat(base, frame - 16, count);
    }
    const double counted = double(float(double(count)));
    const double geared = double(float(double(gear)));
    const double k = Fdivs(Fmuls(counted, dt), geared);
    charge = Fmadds(k, rate, Lfs(base, vehicle + 2500));
  } else {
    charge = Fsubs(Lfs(base, vehicle + 2500), dt);
  }
  const int64_t count = int32_t(guest::LoadU32(base, vehicle + 2524));
  Stfs(base, vehicle + 2500, charge);
  const double counted = IntegerFloat(base, frame - 8, count);
  const double full =
      Fmadds(Fmuls(dt, counted), Lfs(base, 0x820021F8), Lfs(base, 0x820021F4));
  const double ready = Lfs(base, 0x820021F4);
  if (charge < zero) {
    charge = zero;
  } else if (full < charge) {
    charge = full;
  }
  Stfs(base, vehicle + 2500, charge);
  if (charge < ready) {
    const uint8_t latched = guest::LoadU8(base, vehicle + 3240);
    Stfs(base, vehicle + 2508, zero);
    guest::StoreU8(base, vehicle + 2504, 0);
    if (latched != 0) {
      Stfs(base, vehicle + 2500, zero);
    }
    guest::StoreU8(base, vehicle + 3240, 0);
    return;
  }
  // Charged: it fires when the throttle drops, or on a shift.
  const double dropped = Fsubs(Lfs(base, vehicle + 2520), throttle);
  bool fire = dropped > Lfs(base, 0x82002208);
  if (!fire && guest::LoadU8(base, vehicle + 2281) != 0 &&
      guest::LoadU32(base, vehicle + 2256) != 0) {
    fire = true;
  }
  if (!fire) {
    if (guest::LoadU8(base, vehicle + 3240) != 0) {
      fire = true;
    } else {
      guest::StoreU8(base, vehicle + 3240, 0);
      Stfs(base, vehicle + 2508, zero);
      guest::StoreU8(base, vehicle + 2504, 0);
      return;
    }
  }
  const double rpm = Lfs(base, vehicle + 2228);
  const int32_t gear = int32_t(guest::LoadU32(base, vehicle + 2252));
  if (!(rpm > from_rpm) || gear <= 0) {
    Stfs(base, vehicle + 2500, zero);
    Stfs(base, vehicle + 2508, zero);
    guest::StoreU8(base, vehicle + 2504, 0);
    return;
  }
  double strength = Fsubs(rpm, from_rpm);
  guest::StoreU8(base, vehicle + 2504, 1);
  const double range = Lfs(base, engine + 16);
  Stfs(base, vehicle + 2500, zero);
  guest::StoreU8(base, vehicle + 3240, 0);
  strength = Fdivs(strength, range);
  strength = Fmuls(strength, counted);
  strength = Fdivs(strength, IntegerFloat(base, frame - 8, gear));
  Stfs(base, vehicle + 2508, strength);
}

void VehicleUpdateTireWear(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B39C8 Vehicle::UpdateTireWear");
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  if (guest::LoadU32(base, vehicle + 2496) == 0) {
    for (uint32_t w = 0; w < 4; w++) {
      Stfs(base, Wheel(vehicle, w) + 292, zero);
    }
  } else {
    const uint32_t shares = frame + 80;
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(base, shares + i, 0);
    }
    const double cap = Lfs(base, 0x82002204);
    const double scale = Lfs(base, 0x82001FBC);
    const double mode2 = Lfs(base, 0x8200299C);
    const double mode1 = Lfs(base, 0x820029A0);
    const double rate = Lfs(base, 0x82002A44);
    for (uint32_t w = 0; w < 4; w++) {
      const uint32_t wheel = Wheel(vehicle, w);
      if (!(Lfs(base, wheel + 292) < one)) {
        continue;
      }
      const uint32_t data = guest::LoadU32(base, vehicle + 2160 + 4 * w);
      const double surface = VehicleSurfaceValue16(base, vehicle, w);
      double wear = Fmuls(Lfs(base, wheel + 280), Lfs(base, wheel + 356));
      const double k = Lfs(base, vehicle + 2436);
      const double dt = Lfs(base, vehicle + 2352);
      const double tire = Lfs(base, data + 180);
      wear = Fmuls(wear, rate);
      wear = Fmuls(wear, k);
      wear = Fmuls(wear, dt);
      wear = Fmuls(wear, tire);
      wear = Fmuls(wear, surface);
      wear = Fmuls(wear, scale);
      if (wear < zero) {
        wear = zero;
      } else if (wear > cap) {
        wear = cap;
      }
      const uint32_t mode = guest::LoadU32(base, vehicle + 2496);
      if (mode == 2) {
        wear = Fmuls(wear, mode2);
      } else if (mode == 1) {
        wear = Fmuls(wear, mode1);
      }
      Stfs(base, shares + 4 * w, wear);
    }
    double sum = zero;
    for (uint32_t w = 0; w < 4; w++) {
      sum = Fadds(Lfs(base, shares + 4 * w), sum);
    }
    const double each = Fmuls(sum, Lfs(base, 0x82002968));
    for (uint32_t w = 0; w < 4; w++) {
      const uint32_t at = Wheel(vehicle, w) + 292;
      const double worn = Fadds(Lfs(base, at), each);
      Stfs(base, at, worn);
      if (worn > one) {
        Stfs(base, at, one);
      }
    }
  }
  // Each axle's grip from its tires' mean wear.
  const double brake_scale = Lfs(base, 0x82002A40);
  const double half = Lfs(base, 0x82001BE4);
  for (uint32_t axle = 0; axle < 2; axle++) {
    const uint32_t a = Wheel(vehicle, 2 * axle) + 292;
    const double worn_b = Lfs(base, a + kWheelBytes);
    const uint32_t data = guest::LoadU32(base, vehicle + 2160 + 8 * axle);
    const double worn_a = Lfs(base, a);
    const double sum = Fadds(worn_b, worn_a);
    const double mix = Lfs(base, vehicle + 2628);
    const double rest = Fsubs(one, mix);
    const uint32_t record = guest::LoadU32(base, vehicle + 2144 + 8 * axle);
    const double other = Fmuls(Lfs(base, data + 192), mix);
    const double mean = Fmuls(sum, half);
    double loss = Fmuls(mean, mean);
    double grip = Fmadds(rest, Lfs(base, data + 188), other);
    loss = Fmuls(loss, loss);
    if (guest::LoadU8(base, vehicle + 2625) != 0) {
      loss = zero;
    }
    Stfs(base, record + 96, grip);
    if (guest::LoadU8(base, vehicle + 2383) != 0 && Lfs(base, vehicle + 292) > one) {
      grip = Fmuls(grip, brake_scale);
      Stfs(base, record + 96, grip);
    }
    const double kept = Fnmsubs(Lfs(base, data + 184), loss, one);
    Stfs(base, record + 96, Fmuls(kept, Lfs(base, record + 96)));
  }
}

bool VehicleUpdateSteering(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B18B8 Vehicle::UpdateSteering");
  const uint32_t data = CarData(base, vehicle);
  const double zero = Lfs(base, kZero);
  const double full = Lfs(base, data + 324);
  double over = Fsubs(Fabs(Lfs(base, vehicle + 48)), Lfs(base, 0x820029F0));
  if (over < zero) {
    over = zero;
  }
  double limit = full;
  if (guest::LoadU8(base, vehicle + 276) == 1) {
    const double k = Lfs(base, data + 328);
    limit = Fmuls(Fdivs(k, Fadds(k, over)), full);
  }
  if (limit < zero) {
    limit = zero;
  } else if (full < limit) {
    limit = full;
  }
  double right = limit;
  double left = Fneg(limit);
  // The yaw rate: the body's row 1 (w 0.0) . its angular velocity.
  const uint32_t body = guest::LoadU32(base, vehicle + 336);
  const uint32_t matrix = va::RigidBodyMatrix(body);
  CopyWord(base, matrix + 16, frame + 80);
  CopyWord(base, kZero, frame + 92);
  CopyWord(base, matrix + 24, frame + 88);
  CopyWord(base, matrix + 20, frame + 84);
  const double yaw = double(vl::Vec3DotOf(base, frame + 80, va::RigidBodyAngularVelocity(body)));
  const double counter = Fmsubs(Lfs(base, vehicle + 92), Lfs(base, 0x82600D38),
                                Fmuls(Lfs(base, 0x82600D34), yaw));
  if (!(counter < left)) {
    if (counter > right) {
      right = counter;
      if (counter > full) {
        right = full;
      }
    }
  } else {
    const double floor = Fneg(full);
    left = counter;
    if (counter < floor) {
      left = floor;
    }
  }
  const double input = Lfs(base, vehicle + 280);
  bool positive;
  double steer;
  if (input < zero) {
    steer = Fneg(Fmuls(input, right));
    positive = false;
  } else {
    steer = Fmuls(input, left);
    positive = true;
  }
  // The inner wheel's angle (Ackermann): tan as the C runtime's (the
  // original's _ctrlfp calls around it set and put back nearest rounding,
  // the mode it runs in), or the original where tan would fail.
  double inner = steer;
  if (!(steer == zero)) {
    double t;
    if (!c_runtime::Tan(steer, &t)) {
      return false;
    }
    const double wheelbase = Lfs(base, data + 4);
    const double track = Lfs(base, data + 8);
    const double span = Fsubs(Fdivs(wheelbase, Fabs(double(float(t)))), track);
    if (!(span > zero)) {
      inner = Lfs(base, 0x82001FB4);
    } else {
      inner = double(float(c_runtime::Atan(Fdivs(wheelbase, span))));
    }
    if (steer < zero) {
      inner = Fneg(inner);
    }
  }
  CopyWord(base, vehicle + 304, vehicle + 328);
  Stfs(base, vehicle + 304, steer);
  if (positive) {
    Stfs(base, vehicle + 312, steer);
    Stfs(base, vehicle + 308, inner);
  } else {
    Stfs(base, vehicle + 312, inner);
    Stfs(base, vehicle + 308, steer);
  }
  const double stored = Lfs(base, vehicle + 304);
  Stfs(base, vehicle + 316, left);
  Stfs(base, vehicle + 320, right);
  double clamped = Fneg(full);
  if (!(stored < clamped)) {
    clamped = full < stored ? full : stored;
  }
  const double smoothed = Fadds(Lfs(base, vehicle + 324), clamped);
  const double v384 = Lfs(base, vehicle + 384);
  Stfs(base, vehicle + 324, Fmuls(smoothed, Lfs(base, 0x82001BE4)));
  const double k = Fdivs(v384, Lfs(base, CarData(base, vehicle) + 4));
  Stfs(base, vehicle + 3116, Fmuls(k, stored));
  return true;
}

bool VehicleUpdateTorqueSplit(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B2100 Vehicle::UpdateTorqueSplit");
  const uint32_t data = CarData(base, vehicle);
  const double one = Lfs(base, kOne);
  const double front = Lfs(base, data + 180);
  const double rear = Fsubs(one, front);
  const double zero = Lfs(base, kZero);
  const bool split = !(front == zero) && !(rear == zero);
  const uint32_t mode = guest::LoadU32(base, vehicle + 2376);
  if (split && mode == 2 && int32_t(guest::LoadU32(base, vehicle + 2252)) > 0) {
    return false;
  }
  CopyWord(base, data + 180, vehicle + 1988);
  Stfs(base, vehicle + 2116, rear);
  if (!split) {
    return true;
  }
  const double half = Lfs(base, 0x82001BE4);
  // The wheels' loads (+280).
  auto load = [&](uint32_t w) { return Lfs(base, Wheel(vehicle, w) + 280); };
  if (mode == 2) {
    // Not in a forward gear (the forward one is the original's).
    Stfs(base, vehicle + 1988, Lfs(base, data + 180));
    Stfs(base, vehicle + 2116, Fsubs(one, Lfs(base, data + 180)));
    return true;
  }
  double share;
  if (mode == 3) {
    double f = Fadds(load(1), load(0));
    double r = Fadds(load(3), load(2));
    if ((f == zero && r == zero) || Fadds(r, f) == zero) {
      f = one;
      r = one;
    }
    const double total = Fadds(r, f);
    const double least = Lfs(base, data + 180);
    share = Fsubs(one, Fdivs(f, total));
    if (share < least) {
      share = least;
    } else if (share > half) {
      share = half;
    }
  } else if (mode - 4 <= 1) {
    share = Lfs(base, data + 180);
    if (int32_t(guest::LoadU32(base, vehicle + 2252)) > 0) {
      const double r = Fadds(load(3), load(2));
      const double f = Fadds(load(1), load(0));
      const double rear_half = Fmuls(Fabs(r), half);
      const double front_half = Fmuls(Fabs(f), half);
      const double total = Fadds(Fabs(rear_half), Fabs(front_half));
      if (!(total > zero)) {
        share = half;
      } else {
        share = Fdivs(Fabs(rear_half), total);
        if (share > half) {
          share = half;
        }
      }
    }
  } else {
    return true;
  }
  Stfs(base, vehicle + 1988, share);
  Stfs(base, vehicle + 2116, Fsubs(one, share));
  return true;
}

void VehicleUpdateTurbo(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B24A8 Vehicle::UpdateTurbo");
  const uint32_t engine = CarData(base, vehicle) + 1268;
  const double one = Lfs(base, kOne);
  const bool turbo = guest::LoadU8(base, engine + 336) != 0;
  const double max_boost = turbo ? Lfs(base, engine + 352) : one;
  const int32_t gear = int32_t(guest::LoadU32(base, vehicle + 2252));
  const double boost = Lfs(base, vehicle + 2944);
  const double zero = Lfs(base, kZero);
  double throttle;
  double cap;
  if (gear == -1) {
    throttle = Lfs(base, vehicle + 288);
    cap = one;
  } else {
    throttle = Lfs(base, vehicle + 284);
    if (gear == 0) {
      cap = one;
    } else {
      if (gear != 1 && !(Lfs(base, vehicle + 2268) == zero)) {
        throttle = zero;
      }
      cap = max_boost;
    }
  }
  const double rpm = Lfs(base, vehicle + 2228);
  const double k = Lfs(base, engine + 348);
  double pressure;
  if (boost > one) {
    pressure = Fmuls(Fmuls(k, rpm), rpm);
  } else {
    pressure = Fmuls(rpm, k);
  }
  pressure = Fmuls(pressure, throttle);
  const double floor = Lfs(base, 0x82002A14);
  const double target = throttle == zero ? floor : Fadds(boost, pressure);
  const double rate = Lfs(base, 0x82002A10);
  double now = Fmadds(Fsubs(target, boost), Lfs(base, 0x82001FBC), boost);
  Stfs(base, vehicle + 2944, now);
  if (now > cap) {
    now = Fmadds(Fsubs(cap, now), rate, now);
    Stfs(base, vehicle + 2944, now);
  }
  if (!turbo) {
    return;
  }
  guest::StoreU8(base, vehicle + 2960, 0);
  if (guest::LoadU8(base, engine + 337) != 0) {
    // The blow-off valve.
    double held;
    if (throttle == zero) {
      held = Fsubs(Lfs(base, vehicle + 2964), Lfs(base, 0x820021E4));
    } else {
      held = Fadds(Lfs(base, vehicle + 2964), throttle);
    }
    Stfs(base, vehicle + 2964, held);
    double clamped;
    if (held < zero) {
      clamped = zero;
    } else {
      clamped = Lfs(base, 0x82002A0C);
      if (!(held > clamped)) {
        clamped = held;
      }
    }
    const double current = Lfs(base, vehicle + 2944);
    Stfs(base, vehicle + 2964, clamped);
    const double half = Lfs(base, 0x82001BE4);
    if (Fmuls(max_boost, Lfs(base, 0x82001070)) > current &&
        Fmuls(Fsubs(max_boost, one), half) < current && throttle == zero &&
        boost > current &&
        !(Lfs(base, vehicle + 2220) < Fmuls(Lfs(base, engine + 12), half))) {
      const double limit = Fmuls(max_boost, half);
      const double factor = Fmadds(clamped, Lfs(base, 0x82002A08), one);
      double grown = current;
      for (int i = 0; i < 2; i++) {
        grown = Fmuls(factor, grown);
        if (grown > limit) {
          guest::StoreU8(base, vehicle + 2960, 1);
          break;
        }
      }
      if (guest::LoadU8(base, vehicle + 2960) != 0) {
        double cut = Fmadds(Fmsubs(factor, current, current), rate, current);
        Stfs(base, vehicle + 2944, cut);
        if (cut > cap) {
          cut = Fmadds(Fsubs(cap, cut), rate, cut);
          Stfs(base, vehicle + 2944, cut);
        }
      }
    }
  } else {
    // The wastegate.
    uint8_t open = 0;
    bool set = false;
    if (target < one) {
      const double above = Fsubs(boost, one);
      if (above > Fmuls(Fsubs(max_boost, one), Lfs(base, 0x82002A04))) {
        double v = Fmuls(above, Lfs(base, 0x82001BE4));
        if (v < zero) {
          v = zero;
        } else if (v > one) {
          v = one;
        }
        Stfs(base, vehicle + 2952, v);
        open = 1;
        set = true;
      }
    }
    if (!set) {
      Stfs(base, vehicle + 2952, zero);
    }
    guest::StoreU8(base, vehicle + 2948, open);
  }
  // The torque factor.
  double factor = Fdivs(Fsubs(max_boost, Lfs(base, vehicle + 2944)), Fsubs(max_boost, floor));
  factor = Fnmsubs(factor, Lfs(base, engine + 344), one);
  Stfs(base, vehicle + 2956, factor);
  if (factor > one) {
    Stfs(base, vehicle + 2956, one);
  }
}

uint32_t VehicleUpdateTireForces(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B3F80 Vehicle::UpdateTireForces");
  const uint32_t f = frame;
  const double one = Lfs(base, kOne);
  // The driven axles: their base value plus the share of the clutch's
  // torque.
  {
    const double open = Fsubs(one, Lfs(base, vehicle + 2264));
    const uint32_t mode = guest::LoadU32(base, vehicle + 2376);
    const uint32_t data = CarData(base, vehicle);
    const double drive = Fmuls(Fmuls(open, Lfs(base, vehicle + 2320)), Lfs(base, 0x82001BE4));
    if (mode < 1) {
      Stfs(base, vehicle + 1996,
           Fmadds(Lfs(base, vehicle + 1988), drive, Lfs(base, data + 536)));
      Stfs(base, vehicle + 2124, Lfs(base, data + 808));
    } else if (mode == 1) {
      CopyWord(base, data + 536, vehicle + 1996);
      Stfs(base, vehicle + 2124,
           Fmadds(Lfs(base, vehicle + 2116), drive, Lfs(base, data + 808)));
    } else {
      Stfs(base, vehicle + 1996,
           Fmadds(Lfs(base, vehicle + 1988), drive, Lfs(base, data + 536)));
      Stfs(base, vehicle + 2124,
           Fmadds(Lfs(base, vehicle + 2116), drive, Lfs(base, data + 808)));
    }
  }
  const double minus_one = Lfs(base, 0x82001BE0);
  const double least = Lfs(base, 0x82002A54);
  const double zero = Lfs(base, kZero);
  uint32_t r3 = vehicle;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    const uint32_t data = guest::LoadU32(base, vehicle + 2160 + 4 * w);
    const uint32_t body = guest::LoadU32(base, vehicle + 336);
    // The rolling speed (+96: x, 0, 0, 0) and the ground's (+80: along
    // +144, 0, along +128, 0).
    const double rolling = Fmuls(Lfs(base, wheel + 248), Lfs(base, data + 52));
    Stfs(base, f + 140, zero);
    Stfs(base, f + 108, zero);
    Stfs(base, f + 92, zero);
    Stfs(base, f + 100, zero);
    Stfs(base, f + 104, zero);
    Stfs(base, f + 96, rolling);
    va::RigidBodyPointVelocity(base, f + 144, body, wheel + 80);
    vl::Vec4Copy(base, f + 128, f + 144);
    Stfs(base, f + 80, double(vl::Vec3DotOf(base, f + 128, wheel + 144)));
    Stfs(base, f + 84, zero);
    Stfs(base, f + 88, double(vl::Vec3DotOf(base, f + 128, wheel + 128)));
    const double rolling_length = double(vl::Vec3Length(base, f + 96));
    const double ground_length = double(vl::Vec3Length(base, f + 80));
    double speed =
        double(vl::Vec3Length(base, rolling_length < ground_length ? f + 80 : f + 96));
    vl::Vec3Difference(base, f + 112, f + 96, f + 80);
    double slip = double(vl::Vec3Length(base, f + 112));
    Stfs(base, wheel + 356, slip);
    if (slip == zero) {
      Stfs(base, wheel + 304, zero);
      Stfs(base, wheel + 268, zero);
      Stfs(base, wheel + 264, zero);
      Stfs(base, wheel + 332, zero);
      Stfs(base, wheel + 308, zero);
      r3 = f + 112;
      CopyWord(base, wheel + 308, wheel + 332);
      continue;
    }
    const double low = Fdivs(least, Lfs(base, vehicle + 2364));
    if (low > slip) {
      slip = low;
    }
    if (low > speed) {
      speed = low;
    }
    const double inverse = Fdivs(one, speed);
    double along = Fmuls(inverse, Lfs(base, f + 112));
    if (along < minus_one) {
      along = minus_one;
    } else if (along > one) {
      along = one;
    }
    double across = Fmuls(inverse, Lfs(base, f + 120));
    if (across < minus_one) {
      across = minus_one;
    } else if (across > one) {
      across = one;
    }
    // The slip angle: the rolling direction's against the ground's.
    const double rolling_angle = c_runtime::Atan2(Lfs(base, f + 104), Lfs(base, f + 96));
    const double ground_angle = c_runtime::Atan2(Lfs(base, f + 88), Lfs(base, f + 80));
    const double ground_along = Lfs(base, f + 80);
    Stfs(base, wheel + 268, along);
    Stfs(base, wheel + 260, Fsubs(double(float(rolling_angle)), double(float(ground_angle))));
    if (ground_along < zero) {
      Stfs(base, wheel + 268, Fmuls(along, minus_one));
    }
    Stfs(base, wheel + 264, across);
    const double fx = TireMagicFormula(base, 0x82600D00, along);
    const double per = Fdivs(one, slip);
    double longitudinal = Fmuls(Fmuls(per, Fabs(Lfs(base, f + 112))), fx);
    const double fy = TireMagicFormula(base, 0x82600D10, across);
    double lateral = Fmuls(Fmuls(per, Fabs(Lfs(base, f + 120))), fy);
    const double length =
        double(vl::Sqrtf(Fmadds(lateral, lateral, Fmuls(longitudinal, longitudinal))));
    const double scale = Fdivs(one, length);
    if (!(scale > one)) {
      longitudinal = Fmuls(scale, longitudinal);
      lateral = Fmuls(scale, lateral);
    }
    const double load = Fnmsubs(Lfs(base, wheel + 280), zero, one);
    const double limit = Lfs(base, wheel + 316);
    double fl = Fmuls(Fmuls(load, limit), longitudinal);
    double fs = Fmuls(Fmuls(load, limit), lateral);
    // Less the surface's drag x the ground speed's square.
    double drag = Fmuls(VehicleSurfaceValue20(base, vehicle, w), Lfs(base, wheel + 316));
    double g = Lfs(base, f + 80);
    fl = Fnmsubs(Fmuls(drag, g), g, fl);
    drag = Fmuls(VehicleSurfaceValue20(base, vehicle, w), Lfs(base, wheel + 316));
    g = Lfs(base, f + 88);
    Stfs(base, wheel + 308, fl);
    fs = Fnmsubs(Fmuls(drag, g), g, fs);
    Stfs(base, wheel + 304, fs);
    r3 = vehicle;
    CopyWord(base, wheel + 308, wheel + 332);
  }
  return r3;
}

uint32_t VehicleUpdateAtRest(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B2808 Vehicle::UpdateAtRest");
  const uint32_t f = frame;
  const double zero = Lfs(base, kZero);
  uint32_t r3 = vehicle;
  if (guest::LoadU8(base, vehicle + 2384) != 0) {
    guest::StoreU8(base, vehicle + 2928, 1);
  } else {
    const uint32_t object = guest::LoadU32(base, vehicle + 340);
    r3 = object;
    if (object == 0) {
      return 0;
    }
    if (!(Lfs(base, vehicle + 48) < Lfs(base, kOne)) || !(Lfs(base, vehicle + 284) == zero) ||
        !(Lfs(base, vehicle + 288) == zero)) {
      guest::StoreU8(base, vehicle + 2928, 0);
    } else {
      // Resting on a contact: weak enough, and the car's center of mass
      // above it (along +96).
      const uint32_t contact = va::CollisionObjectFirstContact(base, object, 0x03000000);
      r3 = contact;
      if (contact != 0 && double(va::ContactStrength(base, contact)) < Lfs(base, 0x82002A20)) {
        const uint32_t m = va::RigidBodyOrientation(guest::LoadU32(base, vehicle + 336));
        CopyWord(base, m + 48, f + 80);
        Stfs(base, f + 92, zero);
        CopyWord(base, m + 56, f + 88);
        CopyWord(base, m + 52, f + 84);
        vl::Vec3Difference(base, f + 128, va::ContactPosition(contact), f + 80);
        Stfs(base, f + 132, zero);
        const double up = double(vl::Vec3DotOf(base, f + 128, vehicle + 96));
        r3 = f + 128;
        guest::StoreU8(base, vehicle + 2928, up < zero ? 0 : 1);
      }
    }
  }
  // Still, with no input (or in neutral), wheels down.
  const double speed = Fabs(Lfs(base, vehicle + 48));
  const bool no_input =
      Lfs(base, vehicle + 288) == zero && Lfs(base, vehicle + 284) == zero;
  if ((no_input || guest::LoadU32(base, vehicle + 2252) == 0) &&
      guest::LoadU32(base, vehicle + 348) != 0 && Fabs(speed) < Lfs(base, 0x82002A1C)) {
    guest::StoreU8(base, vehicle + 2928, 1);
  }
  if (guest::LoadU8(base, vehicle + 2928) == 0) {
    return r3;
  }
  const uint32_t gear = guest::LoadU32(base, vehicle + 2252);
  guest::StoreU32(base, vehicle + 2252, 0);
  guest::StoreU32(base, vehicle + 2256, gear);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    Stfs(base, wheel + 248, zero);
    Stfs(base, wheel + 252, zero);
    Stfs(base, wheel + 308, zero);
    Stfs(base, wheel + 304, zero);
  }
  const uint32_t body = guest::LoadU32(base, vehicle + 336);
  // The velocity's y kept (its 16 bytes copied, x and z zeroed).
  {
    const uint32_t from = va::RigidBodyVelocity(body);
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(base, f + 128 + i, guest::LoadU32(base, from + i));
    }
    Stfs(base, f + 128, zero);
    Stfs(base, f + 136, zero);
    RigidBodySetVelocity(base, body, f + 128, f - 112);
  }
  // The angular momentum +400's x, y and z zeroed (sub_820B6940 and
  // sub_820B6938, never seen in play: +400, and Vec4::Copy into it).
  {
    const uint32_t from = body + 400;
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(base, f + 96 + i, guest::LoadU32(base, from + i));
    }
    Stfs(base, f + 96, zero);
    Stfs(base, f + 100, zero);
    Stfs(base, f + 104, zero);
    vl::Vec4Copy(base, body + 400, f + 96);
  }
  {
    const uint32_t from = va::RigidBodyAngularVelocity(body);
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(base, f + 112 + i, guest::LoadU32(base, from + i));
    }
    Stfs(base, f + 112, zero);
    Stfs(base, f + 116, zero);
    Stfs(base, f + 120, zero);
    RigidBodySetAngularVelocity(base, body, f + 112, f - 160);
  }
  return body + 96;
}

uint32_t VehicleUpdateAero(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B2A48 Vehicle::UpdateAero");
  const uint32_t f = frame;
  const uint32_t data = CarData(base, vehicle);
  const double zero = Lfs(base, kZero);
  // The drag, against the motion along +112 (forward) and +128 (side).
  const double forward = Lfs(base, vehicle + 384);
  const double forward2 = Fmuls(forward, forward);
  const double k = Fmuls(Lfs(base, data + 1808), Lfs(base, vehicle + 2404));
  const double scale = Lfs(base, 0x82002A14);
  const double minus_one = Lfs(base, 0x82001BE0);
  double drag = Fmuls(Fmuls(Lfs(base, data + 1800), forward2), k);
  drag = Fmuls(drag, scale);
  if (forward > zero) {
    drag = Fmuls(drag, minus_one);
  }
  const double side = Lfs(base, vehicle + 392);
  double side_drag = Fmuls(Fmuls(Fmuls(side, side), Lfs(base, data + 1804)), k);
  side_drag = Fmuls(side_drag, scale);
  if (side > zero) {
    side_drag = Fmuls(side_drag, minus_one);
  }
  vl::Vec4Scaled(base, f + 112, vehicle + 128, side_drag);
  vl::Vec4Scaled(base, f + 176, vehicle + 112, drag);
  vl::Vec3Sum(base, f + 160, f + 176, f + 112);
  vl::Vec3AddTo(base, vehicle + 2608, f + 160);
  uint32_t r3 = vehicle + 2608;
  if (Lfs(base, vehicle + 384) > zero) {
    // The downforce: -(+96) x v^2 x 0.5, x car data +1812 into the front
    // wheels' accumulators, +1816 into the rear's.
    const double down_y = Fneg(Lfs(base, vehicle + 100));
    Stfs(base, f + 108, zero);
    Stfs(base, f + 100, down_y);
    const double half_v2 = Fmuls(forward2, Lfs(base, 0x82001BE4));
    Stfs(base, f + 96, Fneg(Lfs(base, vehicle + 96)));
    Stfs(base, f + 104, Fneg(Lfs(base, vehicle + 104)));
    vl::Vec4Scaled(base, f + 112, f + 96, half_v2);
    vl::Vec4Scaled(base, f + 128, f + 112, Lfs(base, CarData(base, vehicle) + 1812));
    vl::Vec4Scaled(base, f + 144, f + 112, Lfs(base, CarData(base, vehicle) + 1816));
    vl::Vec3AddTo(base, vehicle + 2560, f + 128);
    vl::Vec3AddTo(base, vehicle + 2544, f + 128);
    vl::Vec3AddTo(base, vehicle + 2592, f + 144);
    vl::Vec3AddTo(base, vehicle + 2576, f + 144);
    r3 = vehicle + 2576;
  }
  // The engine's shake.
  const double rpm = Lfs(base, vehicle + 2228);
  const uint32_t engine_data = CarData(base, vehicle);
  Stfs(base, f + 92, zero);
  const double torque = VehicleEngineTorque(base, vehicle, rpm, f);
  r3 = vehicle;
  const double peak = Lfs(base, engine_data + 1552);
  const double per = Fdivs(torque, Lfs(base, engine_data + 1424));
  const double c = Lfs(base, 0x820021F4);
  const double one = Lfs(base, kOne);
  double amount = Fmadds(Fsubs(one, peak), c, Lfs(base, 0x82001FBC));
  const double phase = Fmadds(rpm, c, Lfs(base, vehicle + 2340));
  Stfs(base, vehicle + 2340, phase);
  amount = Fmuls(amount, per);
  amount = Fmuls(amount, Lfs(base, 0x82002A28));
  if (phase < one) {
    return r3;
  }
  const int32_t count = int32_t(guest::LoadU32(base, vehicle + 2344));
  Stfs(base, f + 80, zero);
  Stfs(base, f + 88, zero);
  Stfs(base, f + 92, one);
  const int32_t step = count % 4;
  if (step == 0 || step == 2) {
    const double push = step == 0 ? Fmuls(amount, Lfs(base, 0x82002A24)) : Fmuls(amount, c);
    Stfs(base, f + 84, push);
    vl::Vec3AddTo(base, vehicle + 2544, f + 80);
    vl::Vec3AddTo(base, vehicle + 2560, f + 80);
    r3 = vehicle + 2560;
  }
  const int32_t next = int32_t(guest::LoadU32(base, vehicle + 2344));
  Stfs(base, vehicle + 2340, zero);
  guest::StoreU32(base, vehicle + 2344, next <= 4 ? uint32_t(next + 1) : 0);
  return r3;
}

namespace {

// UpdateWheelAxes' axes for one wheel: with s, c = SinCos(angle) at
// `frame` + 80 and + 84, +144 = +112 c - +128 s and +128 = +112 s + +128 c
// (the vehicle's axes; the temporaries at `frame` + 96 to + 143, in the
// original's order, wheel 0's first sum at + 128 and the others' at + 96).
void TurnedAxes(uint8_t* base, uint32_t vehicle, uint32_t wheel, double angle, uint32_t frame,
                bool wheel0) {
  const uint32_t f = frame;
  mm::SinCos(base, angle, f + 80, f + 84);
  const double s = Lfs(base, f + 80);
  const double c = Lfs(base, f + 84);
  if (wheel0) {
    vl::Vec4Scaled(base, f + 96, vehicle + 128, Fneg(s));
    vl::Vec4Scaled(base, f + 112, vehicle + 112, c);
    vl::Vec3Sum(base, f + 128, f + 112, f + 96);
    vl::Vec4Copy(base, wheel + 144, f + 128);
  } else {
    vl::Vec4Scaled(base, f + 128, vehicle + 128, Fneg(s));
    vl::Vec4Scaled(base, f + 112, vehicle + 112, c);
    vl::Vec3Sum(base, f + 96, f + 112, f + 128);
    vl::Vec4Copy(base, wheel + 144, f + 96);
  }
  vl::Vec4Scaled(base, f + 128, vehicle + 128, c);
  vl::Vec4Scaled(base, f + 112, vehicle + 112, s);
  vl::Vec3Sum(base, f + 96, f + 112, f + 128);
  vl::Vec4Copy(base, wheel + 128, f + 96);
}

}  // namespace

uint32_t VehicleUpdateWheelAxes(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B1B00 Vehicle::UpdateWheelAxes");
  const uint32_t f = frame;
  const uint32_t w0 = Wheel(vehicle, 0), w1 = Wheel(vehicle, 1);
  const uint32_t w2 = Wheel(vehicle, 2), w3 = Wheel(vehicle, 3);
  const double k_f8 = Lfs(base, 0x820029F8);
  const double k_fc = Lfs(base, 0x820029FC);
  const double cfbc = Lfs(base, 0x82001FBC);
  // The front: the toe from the travel's angle (atan2 of +0 against the
  // lateral offset +24), car data +548, the steering +308 and +312.
  {
    const uint32_t data = CarData(base, vehicle);
    const double steer1 = Lfs(base, vehicle + 312);
    const double steer0 = Lfs(base, vehicle + 308);
    const double toe = Lfs(base, data + 548);
    const double a0 = double(float(c_runtime::Atan2(Lfs(base, w0 + 0), Lfs(base, w0 + 24))));
    const double a1 = double(float(c_runtime::Atan2(Lfs(base, w1 + 0), Fneg(Lfs(base, w1 + 24)))));
    double t0 = Fmadds(Lfs(base, w0 + 288), k_fc, Fmuls(a0, k_f8));
    t0 = Fadds(t0, toe);
    Stfs(base, w0 + 36, t0);
    const double angle0 = Fadds(t0, steer0);
    double t1 = Fmadds(Lfs(base, w1 + 288), k_fc, Fmuls(a1, cfbc));
    t1 = Fsubs(t1, toe);
    Stfs(base, w1 + 36, t1);
    TurnedAxes(base, vehicle, w0, angle0, f, true);
    TurnedAxes(base, vehicle, w1, Fadds(Lfs(base, w1 + 36), steer1), f, false);
  }
  // The rear: no steering (+304 x 0.0), and the vehicle's axes into +2080
  // and +2064.
  {
    const double rear_steer = Fmuls(Lfs(base, vehicle + 304), Lfs(base, kZero));
    vl::Vec4Copy(base, vehicle + 2080, vehicle + 112);
    vl::Vec4Copy(base, vehicle + 2064, vehicle + 128);
    const uint32_t data = CarData(base, vehicle);
    const double toe = Lfs(base, data + 820);
    const double a2 = double(float(c_runtime::Atan2(Lfs(base, w2 + 0), Lfs(base, w2 + 24))));
    const double a3 = double(float(c_runtime::Atan2(Lfs(base, w3 + 0), Fneg(Lfs(base, w3 + 24)))));
    const double k_f4 = Lfs(base, 0x820029F4);
    const double m2 = Fmuls(Lfs(base, w2 + 288), k_f4);
    const double m3 = Fmuls(Lfs(base, w3 + 288), k_f4);
    double t2 = Fmsubs(a2, cfbc, m2);
    t2 = Fadds(t2, toe);
    Stfs(base, w2 + 36, t2);
    const double angle2 = Fadds(t2, rear_steer);
    double t3 = Fmsubs(a3, k_f8, m3);
    t3 = Fsubs(t3, toe);
    Stfs(base, w3 + 36, t3);
    TurnedAxes(base, vehicle, w2, angle2, f, false);
    TurnedAxes(base, vehicle, w3, Fadds(rear_steer, Lfs(base, w3 + 36)), f, false);
  }
  // Per wheel: the contact's velocity and speeds, the camber and the
  // wheel's quaternion (about the x axis at 0x82638270).
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    CopyWord(base, wheel + 320, wheel + 324);
    const uint32_t data = guest::LoadU32(base, vehicle + 2160 + 4 * w);
    va::RigidBodyPointVelocity(base, f + 128, guest::LoadU32(base, vehicle + 336), wheel + 160);
    vl::Vec4Copy(base, wheel + 336, f + 128);
    Stfs(base, wheel + 320, double(vl::Vec3DotOf(base, wheel + 336, wheel + 144)));
    Stfs(base, wheel + 328, double(vl::Vec3DotOf(base, wheel + 336, wheel + 128)));
    double turn;
    if (w == 0 || w == 2) {
      const double a = double(float(c_runtime::Atan2(Lfs(base, wheel + 0), Lfs(base, wheel + 24))));
      Stfs(base, wheel + 32, Fnmsubs(a, Lfs(base, data + 48), Lfs(base, data + 204)));
      const double b = double(float(c_runtime::Atan2(Lfs(base, wheel + 0), Lfs(base, wheel + 24))));
      turn = Fnmsubs(b, Lfs(base, data + 48), Lfs(base, data + 208));
    } else {
      const double a =
          double(float(c_runtime::Atan2(Lfs(base, wheel + 0), Fneg(Lfs(base, wheel + 24)))));
      Stfs(base, wheel + 32, Fmsubs(a, Lfs(base, data + 48), Lfs(base, data + 204)));
      const double b =
          double(float(c_runtime::Atan2(Lfs(base, wheel + 0), Fneg(Lfs(base, wheel + 24)))));
      turn = Fmsubs(b, Lfs(base, data + 48), Lfs(base, data + 208));
    }
    mm::QuaternionFromAxisAngle(base, wheel + 48, 0x82638270, turn, f - 32);
  }
  return w3 + 48;
}

}  // namespace car_physics
