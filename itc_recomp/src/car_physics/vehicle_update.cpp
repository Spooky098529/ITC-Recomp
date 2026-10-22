// The vehicle's physics step's updates (docs/functions/car-physics.md): see
// car_physics.h. Wheel w's 368-byte record is at vehicle + 416 + 368 w.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "vehicle_accessors/vectors.h"

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace mm = matrix_math;

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

}  // namespace car_physics
