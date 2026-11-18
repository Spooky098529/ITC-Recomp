// The vehicle's physics step's updates (docs/functions/car-physics.md): see
// car_physics.h. Wheel w's 368-byte record is at vehicle + 416 + 368 w.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "schema/car_physics.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace mm = matrix_math;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kWheelBytes = 368;
// Vehicle::ApplyForces's tip limits (+16 . up below them: tipped), the up
// vector and 0.5.
constexpr uint32_t kTipUpright = 0x82002A34, kTipInAir = 0x82002A30, kTipDown = 0x82002A2C;
constexpr uint32_t kUp = 0x82638280;
constexpr uint32_t kHalfConstant = 0x82001BE4;

uint32_t Wheel(uint32_t vehicle, uint32_t w) {
  return vehicle + 416 + kWheelBytes * w;
}

uint32_t CarData(uint8_t* base, uint32_t vehicle) {
  return schema::Vehicle(base, vehicle).car_data().address;
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
  CopyWord(base, schema::PhysicsVector(base, vector).z_address(), f + 80);
  CopyWord(base, schema::PhysicsVector(base, vector).y_address(), f + 84);
  CopyWord(base, schema::PhysicsVector(base, vector).x_address(), f + 88);
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
  const double throttle = double(schema::Vehicle(base, vehicle).throttle());
  schema::Vehicle(base, vehicle).set_traction_active(uint8_t(0));
  const double half = Lfs(base, 0x82001BE4);
  // The driven pair: the rear when the front's share is below 0.5.
  const uint32_t first = double(schema::CarData(base, CarData(base, vehicle)).front_torque_share()) < half ? 2 : 0;
  const uint32_t a = Wheel(vehicle, first), b = Wheel(vehicle, first + 1);
  const double limit = double(schema::Vehicle(base, vehicle).traction_limit());
  const double slip = Fmuls(Fadds(double(schema::VehicleWheel(base, b).slip_long()), double(schema::VehicleWheel(base, a).slip_long())), half);
  const double spin = Fabs(Fadds(double(schema::VehicleWheel(base, b).spin()), double(schema::VehicleWheel(base, a).spin())));
  bool rising = false;
  if (slip > limit) {
    const double threshold = Lfs(base, 0x82000D44);
    if (Fmuls(spin, half) > threshold) {
      rising = true;
      const int32_t count = int32_t(schema::Vehicle(base, vehicle).traction_counter() + 1);
      schema::Vehicle(base, vehicle).set_traction_counter(uint32_t(uint32_t(count)));
      // The count against the same threshold (as the original).
      if (IntegerFloat(base, frame - 16, count) > threshold) {
        schema::Vehicle(base, vehicle).set_traction_counter(uint32_t(30));
      }
    }
  }
  if (!rising) {
    const int32_t count = int32_t(schema::Vehicle(base, vehicle).traction_counter() - 1);
    schema::Vehicle(base, vehicle).set_traction_counter(uint32_t(uint32_t(count)));
    if (count < 0) {
      schema::Vehicle(base, vehicle).set_traction_counter(uint32_t(0));
    }
  }
  const double per = Lfs(base, 0x820029EC);
  const int32_t count = int32_t(schema::Vehicle(base, vehicle).traction_counter());
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
  schema::Vehicle(base, vehicle).set_throttle(float(cut));
  if (!(throttle == cut)) {
    schema::Vehicle(base, vehicle).set_traction_active(uint8_t(1));
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
    if (schema::VehicleWheel(base, wheel).on_ground() == 0) {
      schema::VehicleWheel(base, wheel).set_grip(float(zero));
      schema::VehicleWheel(base, wheel).set_force_limit(float(zero));
      continue;
    }
    const uint32_t record = guest::LoadU32(base, schema::Vehicle(base, vehicle).wheel_axles_address() + 4 * w);
    const uint32_t data = guest::LoadU32(base, schema::Vehicle(base, vehicle).wheel_axle_data_address() + 4 * w);
    const double surface = VehicleSurfaceValue12(base, vehicle, w);
    const double camber = Fadds(double(schema::Vehicle(base, vehicle).roll()), double(schema::VehicleWheel(base, wheel).camber()));
    const double rated = Fmuls(double(schema::CarDataAxle(base, data).unknown_140()), double(schema::CarDataAxle(base, data).unknown_144()));
    const double loss = Fabs(Fmuls(camber, camber_scale));
    double ratio = Fmuls(Fdivs(double(schema::VehicleWheel(base, wheel).load()), rated), high);
    const double kept = Fsubs(one, loss);
    if (ratio > high) {
      ratio = high;
    } else if (ratio < low) {
      ratio = low;
    }
    const double force = TireMagicFormula(base, 0x82600D20, ratio);
    double grip = Fmuls(double(schema::CarDataAxle(base, data).unknown_148()), force);
    grip = Fmuls(grip, kept);
    grip = Fmuls(grip, surface);
    if (grip < zero) {
      grip = zero;
    }
    grip = Fmuls(double(schema::VehicleAxle(base, record).grip()), grip);
    schema::VehicleWheel(base, wheel).set_grip(float(grip));
    double limit = Fmuls(double(schema::CarDataAxle(base, data).unknown_140()), double(schema::CarDataAxle(base, data).unknown_144()));
    limit = Fmuls(limit, double(schema::Vehicle(base, vehicle).grip_scale()));
    schema::VehicleWheel(base, wheel).set_force_limit(float(Fmuls(limit, grip)));
  }
}

void VehicleUpdateEngineTorque(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B22D8 Vehicle::UpdateEngineTorque");
  const uint32_t data = CarData(base, vehicle);
  const int32_t gear = schema::Vehicle(base, vehicle).gear();
  const double limiter_rpm = double(schema::CarData(base, data).rev_limit_normalized());
  double throttle = Lfs(base, vehicle + (gear == -1 ? 288 : 284));
  const double zero = Lfs(base, kZero);
  // The rev limiter: its timer +2244 runs down, the throttle cut meanwhile;
  // set again at the limiter's rpm.
  const double timer = double(schema::Vehicle(base, vehicle).rev_limiter());
  if (timer > zero) {
    const double left = Fsubs(timer, double(schema::Vehicle(base, vehicle).dt()));
    schema::Vehicle(base, vehicle).set_rev_limiter(float(left));
    if (left < zero) {
      schema::Vehicle(base, vehicle).set_unknown_2248(uint8_t(0));
    }
    throttle = zero;
  } else if (!(double(schema::Vehicle(base, vehicle).rpm()) < limiter_rpm)) {
    CopyWord(base, 0x82002204, schema::Vehicle(base, vehicle).rev_limiter_address());
    schema::Vehicle(base, vehicle).set_unknown_2248(uint8_t(1));
  } else {
    schema::Vehicle(base, vehicle).set_rev_limiter(float(zero));
    schema::Vehicle(base, vehicle).set_unknown_2248(uint8_t(0));
  }
  // The clutch: no throttle while it's out, without the handbrake, between
  // gears.
  const double clutch = double(schema::Vehicle(base, vehicle).clutch());
  const double one = Lfs(base, kOne);
  if (!(clutch == one) && double(schema::Vehicle(base, vehicle).handbrake()) == zero) {
    const int32_t next = schema::Vehicle(base, vehicle).gear_previous();
    if (next != 0 && gear != 0 && next < gear) {
      throttle = zero;
    }
  }
  const double rpm = double(schema::Vehicle(base, vehicle).rpm());
  schema::Vehicle(base, vehicle).set_throttle_used(float(throttle));
  const double curve = VehicleEngineTorque(base, vehicle, rpm, frame);
  const double peak = Fmuls(double(schema::CarData(base, data).engine_braking()), curve);
  double torque = Fmuls(double(schema::Vehicle(base, vehicle).torque_scale()), peak);
  torque = Fmuls(torque, throttle);
  if (schema::Vehicle(base, vehicle).nitrous_on() != 0) {
    // Nitrous above an rpm, while it lasts.
    const double from = Fmuls(double(schema::CarData(base, data).idle_rpm()), Lfs(base, 0x820029D4));
    bool stop = false;
    if (!(from < rpm)) {
      stop = true;
    } else {
      const double left = double(schema::Vehicle(base, vehicle).nitrous_value());
      if (!(left == zero) && throttle > Lfs(base, 0x82001BE4)) {
        const double boost = Fmuls(double(schema::Vehicle(base, vehicle).nitrous_factor()), throttle);
        const double rest = Fsubs(left, throttle);
        schema::Vehicle(base, vehicle).set_nitrous_value(float(rest));
        torque = Fmuls(boost, torque);
        if (!(rest > zero)) {
          schema::Vehicle(base, vehicle).set_nitrous_value(float(zero));
          stop = true;
        }
      }
    }
    if (stop) {
      schema::Vehicle(base, vehicle).set_nitrous_on(uint8_t(0));
    }
  }
  // Engine braking.
  const double off = Fsubs(one, throttle);
  const double idle = double(schema::CarData(base, data).idle_rpm());
  torque = Fmuls(double(schema::Vehicle(base, vehicle).oil_factor()), torque);
  const double above = Fsubs(rpm, idle);
  const double braking_scale = double(schema::CarData(base, data).engine_braking_2());
  const double range = Fsubs(limiter_rpm, idle);
  double braking = Fmuls(above, off);
  braking = Fmuls(braking, braking_scale);
  braking = Fmuls(braking, peak);
  const double net = Fsubs(torque, Fdivs(braking, range));
  schema::Vehicle(base, vehicle).set_engine_torque(float(net));
  schema::Vehicle(base, vehicle).set_engine_torque_2236(float(net));
  if (gear != 0 && !(clutch == zero)) {
    schema::Vehicle(base, vehicle).set_engine_torque_2236(float(net));
    if (gear != -1) {
      return;
    }
    schema::Vehicle(base, vehicle).set_engine_torque_2236(float(Fmuls(net, Lfs(base, 0x82001BE0))));
    return;
  }
  schema::Vehicle(base, vehicle).set_engine_torque_2236(float(net));
}

void VehicleUpdateBrakeHeat(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B35C8 Vehicle::UpdateBrakeHeat");
  const double speed = double(schema::Vehicle(base, vehicle).speed());
  const double speed2 = Fmuls(speed, speed);
  const double dt = double(schema::Vehicle(base, vehicle).dt());
  const double by_speed = double(schema::CarPhysicsGlobals::brake_cooling_by_speed(base));
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
    if (double(schema::Vehicle(base, vehicle).brake()) > zero) {
      const uint32_t data = CarData(base, vehicle);
      if (Lfs(base, data + (w < 2 ? 264 : 268)) > zero) {
        double heat = Fmuls(double(schema::VehicleWheel(base, wheel).spin()), double(schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).radius()));
        heat = Fmuls(heat, double(schema::VehicleWheel(base, wheel).brake_force()));
        heat = Fmuls(heat, double(schema::Vehicle(base, vehicle).dt()));
        added = Fmuls(heat, Lfs(base, 0x82600CD4));
      }
    }
    double t = Fadds(added, double(schema::VehicleWheel(base, wheel).brake_heat()));
    schema::VehicleWheel(base, wheel).set_brake_heat(float(t));
    const double span = Fsubs(double(schema::CarData(base, CarData(base, vehicle)).brake_heat_limit()), ambient);
    const double cool = Fdivs(Fmuls(Fsubs(t, ambient), cooling), span);
    t = Fsubs(t, cool);
    schema::VehicleWheel(base, wheel).set_brake_heat(float(t));
    const double t_max = double(schema::CarData(base, CarData(base, vehicle)).brake_heat_limit());
    if (t < ambient) {
      t = ambient;
    } else if (t_max < t) {
      t = t_max;
    }
    schema::VehicleWheel(base, wheel).set_brake_heat(float(t));
    if (!(double(schema::Vehicle(base, vehicle).brake()) > zero) || !(added > cooling)) {
      continue;
    }
    const uint32_t data = CarData(base, vehicle);
    const double fade_from = double(schema::CarData(base, data).brake_heat_start());
    if (!(t > fade_from)) {
      continue;
    }
    double fade = Fdivs(Fsubs(t, fade_from), double(schema::CarData(base, data).brake_heat_limit()));
    fade = Fmadds(fade, fade_rate, double(schema::Vehicle(base, vehicle).brake_fade()));
    schema::Vehicle(base, vehicle).set_brake_fade(float(fade));
    if (fade < zero) {
      fade = zero;
    } else if (fade > fade_max) {
      fade = fade_max;
    }
    schema::Vehicle(base, vehicle).set_brake_fade(float(fade));
  }
  double fade = Fnmsubs(double(schema::Vehicle(base, vehicle).dt()), Lfs(base, 0x82001BE4),
                        double(schema::Vehicle(base, vehicle).brake_fade()));
  schema::Vehicle(base, vehicle).set_brake_fade(float(fade));
  if (fade < zero) {
    fade = zero;
  } else if (fade > fade_max) {
    fade = fade_max;
  }
  schema::Vehicle(base, vehicle).set_brake_fade(float(fade));
  schema::Vehicle(base, vehicle).set_brake_fade_copy(float(fade));
}

void VehicleUpdateBackfire(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B3788 Vehicle::UpdateBackfire");
  const schema::CarData engine_data(base, CarData(base, vehicle));
  const double top = Lfs(base, engine_data.rev_limit_normalized_address());
  const double from_rpm = Fmuls(top, Lfs(base, 0x82002A3C));
  const double half_rpm = Fmuls(top, Lfs(base, 0x82001BE4));
  const double zero = Lfs(base, kZero);
  if (schema::Vehicle(base, vehicle).unknown_2512() == 0) {
    schema::Vehicle(base, vehicle).set_backfire_strength(float(zero));
    schema::Vehicle(base, vehicle).set_backfire(uint8_t(0));
    schema::Vehicle(base, vehicle).set_backfire_charge(float(zero));
    return;
  }
  const double throttle = double(schema::Vehicle(base, vehicle).throttle_used());
  const double dt = double(schema::Vehicle(base, vehicle).dt());
  double charge;
  // The charge builds on throttle at high rpm in a forward gear.
  if (throttle > Lfs(base, 0x82001070) && double(schema::Vehicle(base, vehicle).rpm()) > half_rpm &&
      schema::Vehicle(base, vehicle).gear() > 0 &&
      schema::Vehicle(base, vehicle).backfire_latch() == 0) {
    const int64_t gear = schema::Vehicle(base, vehicle).gear();
    const int64_t count = int32_t(schema::Vehicle(base, vehicle).unknown_2524());
    double rate = Lfs(base, 0x82002A38);
    if (engine_data.turbo() != 0) {
      rate = Fmuls(Lfs(base, engine_data.boost_1620_address()), rate);
      IntegerFloat(base, frame - 16, gear);
      IntegerFloat(base, frame - 8, count);
    } else {
      IntegerFloat(base, frame - 8, gear);
      IntegerFloat(base, frame - 16, count);
    }
    const double counted = double(float(double(count)));
    const double geared = double(float(double(gear)));
    const double k = Fdivs(Fmuls(counted, dt), geared);
    charge = Fmadds(k, rate, double(schema::Vehicle(base, vehicle).backfire_charge()));
  } else {
    charge = Fsubs(double(schema::Vehicle(base, vehicle).backfire_charge()), dt);
  }
  const int64_t count = int32_t(schema::Vehicle(base, vehicle).unknown_2524());
  schema::Vehicle(base, vehicle).set_backfire_charge(float(charge));
  const double counted = IntegerFloat(base, frame - 8, count);
  const double full =
      Fmadds(Fmuls(dt, counted), Lfs(base, 0x820021F8), Lfs(base, 0x820021F4));
  const double ready = Lfs(base, 0x820021F4);
  if (charge < zero) {
    charge = zero;
  } else if (full < charge) {
    charge = full;
  }
  schema::Vehicle(base, vehicle).set_backfire_charge(float(charge));
  if (charge < ready) {
    const uint8_t latched = schema::Vehicle(base, vehicle).backfire_latch();
    schema::Vehicle(base, vehicle).set_backfire_strength(float(zero));
    schema::Vehicle(base, vehicle).set_backfire(uint8_t(0));
    if (latched != 0) {
      schema::Vehicle(base, vehicle).set_backfire_charge(float(zero));
    }
    schema::Vehicle(base, vehicle).set_backfire_latch(uint8_t(0));
    return;
  }
  // Charged: it fires when the throttle drops, or on a shift.
  const double dropped = Fsubs(double(schema::Vehicle(base, vehicle).throttle_used_previous()), throttle);
  bool fire = dropped > Lfs(base, 0x82002208);
  if (!fire && schema::Vehicle(base, vehicle).unknown_2281() != 0 &&
      uint32_t(schema::Vehicle(base, vehicle).gear_previous()) != 0) {
    fire = true;
  }
  if (!fire) {
    if (schema::Vehicle(base, vehicle).backfire_latch() != 0) {
      fire = true;
    } else {
      schema::Vehicle(base, vehicle).set_backfire_latch(uint8_t(0));
      schema::Vehicle(base, vehicle).set_backfire_strength(float(zero));
      schema::Vehicle(base, vehicle).set_backfire(uint8_t(0));
      return;
    }
  }
  const double rpm = double(schema::Vehicle(base, vehicle).rpm());
  const int32_t gear = schema::Vehicle(base, vehicle).gear();
  if (!(rpm > from_rpm) || gear <= 0) {
    schema::Vehicle(base, vehicle).set_backfire_charge(float(zero));
    schema::Vehicle(base, vehicle).set_backfire_strength(float(zero));
    schema::Vehicle(base, vehicle).set_backfire(uint8_t(0));
    return;
  }
  double strength = Fsubs(rpm, from_rpm);
  schema::Vehicle(base, vehicle).set_backfire(uint8_t(1));
  const double range = Lfs(base, engine_data.rev_limit_normalized_address());
  schema::Vehicle(base, vehicle).set_backfire_charge(float(zero));
  schema::Vehicle(base, vehicle).set_backfire_latch(uint8_t(0));
  strength = Fdivs(strength, range);
  strength = Fmuls(strength, counted);
  strength = Fdivs(strength, IntegerFloat(base, frame - 8, gear));
  schema::Vehicle(base, vehicle).set_backfire_strength(float(strength));
}

void VehicleUpdateTireWear(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B39C8 Vehicle::UpdateTireWear");
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  if (schema::Vehicle(base, vehicle).tire_wear_mode() == 0) {
    for (uint32_t w = 0; w < 4; w++) {
      schema::VehicleWheel(base, Wheel(vehicle, w)).set_tire_wear(float(zero));
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
      if (!(double(schema::VehicleWheel(base, wheel).tire_wear()) < one)) {
        continue;
      }
      const uint32_t data = guest::LoadU32(base, schema::Vehicle(base, vehicle).wheel_axle_data_address() + 4 * w);
      const double surface = VehicleSurfaceValue16(base, vehicle, w);
      double wear = Fmuls(double(schema::VehicleWheel(base, wheel).load()), double(schema::VehicleWheel(base, wheel).slip_speed()));
      const double k = double(schema::Vehicle(base, vehicle).unknown_2436());
      const double dt = double(schema::Vehicle(base, vehicle).dt());
      const double tire = Lfs(base, schema::CarDataAxle(base, data).unknown_180_address());
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
      const uint32_t mode = schema::Vehicle(base, vehicle).tire_wear_mode();
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
      const uint32_t at = schema::VehicleWheel(base, Wheel(vehicle, w)).tire_wear_address();
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
    const uint32_t a = schema::VehicleWheel(base, Wheel(vehicle, 2 * axle)).tire_wear_address();
    const double worn_b = Lfs(base, a + kWheelBytes);
    const uint32_t data = guest::LoadU32(base, schema::Vehicle(base, vehicle).wheel_axle_data_address() + 8 * axle);
    const double worn_a = Lfs(base, a);
    const double sum = Fadds(worn_b, worn_a);
    const double mix = double(schema::Vehicle(base, vehicle).unknown_2628());
    const double rest = Fsubs(one, mix);
    const uint32_t record = guest::LoadU32(base, schema::Vehicle(base, vehicle).wheel_axles_address() + 8 * axle);
    const double other = Fmuls(Lfs(base, schema::CarDataAxle(base, data).unknown_180_address() + 12), mix);
    const double mean = Fmuls(sum, half);
    double loss = Fmuls(mean, mean);
    double grip = Fmadds(rest, Lfs(base, schema::CarDataAxle(base, data).unknown_180_address() + 8), other);
    loss = Fmuls(loss, loss);
    if (schema::Vehicle(base, vehicle).unknown_2625() != 0) {
      loss = zero;
    }
    schema::VehicleAxle(base, record).set_grip(float(grip));
    if (schema::Vehicle(base, vehicle).unknown_2383() != 0 && double(schema::Vehicle(base, vehicle).brake()) > one) {
      grip = Fmuls(grip, brake_scale);
      schema::VehicleAxle(base, record).set_grip(float(grip));
    }
    const double kept = Fnmsubs(Lfs(base, schema::CarDataAxle(base, data).unknown_180_address() + 4), loss, one);
    schema::VehicleAxle(base, record).set_grip(float(Fmuls(kept, double(schema::VehicleAxle(base, record).grip()))));
  }
}

bool VehicleUpdateSteering(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B18B8 Vehicle::UpdateSteering");
  const uint32_t data = CarData(base, vehicle);
  const double zero = Lfs(base, kZero);
  const double full = double(schema::CarData(base, data).max_steering_angle());
  double over = Fsubs(Fabs(double(schema::Vehicle(base, vehicle).speed())), Lfs(base, 0x820029F0));
  if (over < zero) {
    over = zero;
  }
  double limit = full;
  if (schema::Vehicle(base, vehicle).speed_sensitive_steering() == 1) {
    const double k = double(schema::CarData(base, data).steering_328());
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
  const uint32_t body = schema::Vehicle(base, vehicle).body().address;
  const uint32_t matrix = va::RigidBodyMatrix(body);
  CopyWord(base, matrix + 16, frame + 80);
  CopyWord(base, kZero, frame + 92);
  CopyWord(base, matrix + 24, frame + 88);
  CopyWord(base, matrix + 20, frame + 84);
  const double yaw = double(vl::Vec3DotOf(base, frame + 80, va::RigidBodyAngularVelocity(body)));
  const double counter = Fmsubs(double(schema::Vehicle(base, vehicle).slip_term()), Lfs(base, 0x82600D38),
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
  const double input = double(schema::Vehicle(base, vehicle).steering_input());
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
    const double wheelbase = double(schema::CarData(base, data).wheelbase());
    const double track = double(schema::CarData(base, data).front_track());
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
  CopyWord(base, schema::Vehicle(base, vehicle).steering_address(), schema::Vehicle(base, vehicle).steering_previous_address());
  schema::Vehicle(base, vehicle).set_steering(float(steer));
  if (positive) {
    schema::Vehicle(base, vehicle).set_front_right_angle(float(steer));
    schema::Vehicle(base, vehicle).set_front_left_angle(float(inner));
  } else {
    schema::Vehicle(base, vehicle).set_front_right_angle(float(inner));
    schema::Vehicle(base, vehicle).set_front_left_angle(float(steer));
  }
  const double stored = double(schema::Vehicle(base, vehicle).steering());
  schema::Vehicle(base, vehicle).set_steering_limit_left(float(left));
  schema::Vehicle(base, vehicle).set_steering_limit_right(float(right));
  double clamped = Fneg(full);
  if (!(stored < clamped)) {
    clamped = full < stored ? full : stored;
  }
  const double smoothed = Fadds(double(schema::Vehicle(base, vehicle).steering_smoothed()), clamped);
  const double v384 = Lfs(base, schema::Vehicle(base, vehicle).ground_velocity_address());
  schema::Vehicle(base, vehicle).set_steering_smoothed(float(Fmuls(smoothed, Lfs(base, 0x82001BE4))));
  const double k = Fdivs(v384, double(schema::CarData(base, CarData(base, vehicle)).wheelbase()));
  schema::Vehicle(base, vehicle).set_unknown_3116(float(Fmuls(k, stored)));
  return true;
}

bool VehicleUpdateTorqueSplit(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B2100 Vehicle::UpdateTorqueSplit");
  const uint32_t data = CarData(base, vehicle);
  const double one = Lfs(base, kOne);
  const double front = double(schema::CarData(base, data).front_torque_share());
  const double rear = Fsubs(one, front);
  const double zero = Lfs(base, kZero);
  const bool split = !(front == zero) && !(rear == zero);
  const uint32_t mode = schema::Vehicle(base, vehicle).drive_type();
  if (split && mode == 2 && schema::Vehicle(base, vehicle).gear() > 0) {
    return false;
  }
  CopyWord(base, schema::CarData(base, data).front_torque_share_address(), schema::VehicleAxle(base, schema::Vehicle(base, vehicle).front_axle_address()).torque_share_address());
  schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).set_torque_share(float(rear));
  if (!split) {
    return true;
  }
  const double half = Lfs(base, 0x82001BE4);
  // The wheels' loads (+280).
  auto load = [&](uint32_t w) { return double(schema::VehicleWheel(base, Wheel(vehicle, w)).load()); };
  if (mode == 2) {
    // Not in a forward gear (the forward one is the original's).
    schema::VehicleAxle(base, schema::Vehicle(base, vehicle).front_axle_address()).set_torque_share(float(double(schema::CarData(base, data).front_torque_share())));
    schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).set_torque_share(float(Fsubs(one, double(schema::CarData(base, data).front_torque_share()))));
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
    const double least = double(schema::CarData(base, data).front_torque_share());
    share = Fsubs(one, Fdivs(f, total));
    if (share < least) {
      share = least;
    } else if (share > half) {
      share = half;
    }
  } else if (mode - 4 <= 1) {
    share = double(schema::CarData(base, data).front_torque_share());
    if (schema::Vehicle(base, vehicle).gear() > 0) {
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
  schema::VehicleAxle(base, schema::Vehicle(base, vehicle).front_axle_address()).set_torque_share(float(share));
  schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).set_torque_share(float(Fsubs(one, share)));
  return true;
}

void VehicleUpdateTurbo(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B24A8 Vehicle::UpdateTurbo");
  const schema::CarData engine_data(base, CarData(base, vehicle));
  const double one = Lfs(base, kOne);
  const bool turbo = engine_data.turbo() != 0;
  const double max_boost = turbo ? Lfs(base, engine_data.boost_1620_address()) : one;
  const int32_t gear = schema::Vehicle(base, vehicle).gear();
  const double boost = double(schema::Vehicle(base, vehicle).boost());
  const double zero = Lfs(base, kZero);
  double throttle;
  double cap;
  if (gear == -1) {
    throttle = double(schema::Vehicle(base, vehicle).reverse_throttle());
    cap = one;
  } else {
    throttle = double(schema::Vehicle(base, vehicle).throttle());
    if (gear == 0) {
      cap = one;
    } else {
      if (gear != 1 && !(double(schema::Vehicle(base, vehicle).shift_timer()) == zero)) {
        throttle = zero;
      }
      cap = max_boost;
    }
  }
  const double rpm = double(schema::Vehicle(base, vehicle).rpm());
  const double k = Lfs(base, engine_data.boost_1616_address());
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
  schema::Vehicle(base, vehicle).set_boost(float(now));
  if (now > cap) {
    now = Fmadds(Fsubs(cap, now), rate, now);
    schema::Vehicle(base, vehicle).set_boost(float(now));
  }
  if (!turbo) {
    return;
  }
  schema::Vehicle(base, vehicle).set_blow_off_open(uint8_t(0));
  if (engine_data.blow_off() != 0) {
    // The blow-off valve.
    double held;
    if (throttle == zero) {
      held = Fsubs(double(schema::Vehicle(base, vehicle).blow_off()), Lfs(base, 0x820021E4));
    } else {
      held = Fadds(double(schema::Vehicle(base, vehicle).blow_off()), throttle);
    }
    schema::Vehicle(base, vehicle).set_blow_off(float(held));
    double clamped;
    if (held < zero) {
      clamped = zero;
    } else {
      clamped = Lfs(base, 0x82002A0C);
      if (!(held > clamped)) {
        clamped = held;
      }
    }
    const double current = double(schema::Vehicle(base, vehicle).boost());
    schema::Vehicle(base, vehicle).set_blow_off(float(clamped));
    const double half = Lfs(base, 0x82001BE4);
    if (Fmuls(max_boost, Lfs(base, 0x82001070)) > current &&
        Fmuls(Fsubs(max_boost, one), half) < current && throttle == zero &&
        boost > current &&
        !(double(schema::Vehicle(base, vehicle).unknown_2220()) < Fmuls(Lfs(base, engine_data.rev_limit_address()), half))) {
      const double limit = Fmuls(max_boost, half);
      const double factor = Fmadds(clamped, Lfs(base, 0x82002A08), one);
      double grown = current;
      for (int i = 0; i < 2; i++) {
        grown = Fmuls(factor, grown);
        if (grown > limit) {
          schema::Vehicle(base, vehicle).set_blow_off_open(uint8_t(1));
          break;
        }
      }
      if (schema::Vehicle(base, vehicle).blow_off_open() != 0) {
        double cut = Fmadds(Fmsubs(factor, current, current), rate, current);
        schema::Vehicle(base, vehicle).set_boost(float(cut));
        if (cut > cap) {
          cut = Fmadds(Fsubs(cap, cut), rate, cut);
          schema::Vehicle(base, vehicle).set_boost(float(cut));
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
        schema::Vehicle(base, vehicle).set_wastegate(float(v));
        open = 1;
        set = true;
      }
    }
    if (!set) {
      schema::Vehicle(base, vehicle).set_wastegate(float(zero));
    }
    schema::Vehicle(base, vehicle).set_wastegate_open(uint8_t(open));
  }
  // The torque factor.
  double factor = Fdivs(Fsubs(max_boost, double(schema::Vehicle(base, vehicle).boost())), Fsubs(max_boost, floor));
  factor = Fnmsubs(factor, Lfs(base, engine_data.boost_1612_address()), one);
  schema::Vehicle(base, vehicle).set_turbo_torque_factor(float(factor));
  if (factor > one) {
    schema::Vehicle(base, vehicle).set_turbo_torque_factor(float(one));
  }
}

uint32_t VehicleUpdateTireForces(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B3F80 Vehicle::UpdateTireForces");
  const uint32_t f = frame;
  const double one = Lfs(base, kOne);
  // The driven axles: their base value plus the share of the clutch's
  // torque.
  {
    const double open = Fsubs(one, double(schema::Vehicle(base, vehicle).clutch()));
    const uint32_t mode = schema::Vehicle(base, vehicle).drive_type();
    const uint32_t data = CarData(base, vehicle);
    const double drive = Fmuls(Fmuls(open, double(schema::Vehicle(base, vehicle).unknown_2320())), Lfs(base, 0x82001BE4));
    if (mode < 1) {
      schema::VehicleAxle(base, schema::Vehicle(base, vehicle).front_axle_address()).set_inertia(float(Fmadds(double(schema::VehicleAxle(base, schema::Vehicle(base, vehicle).front_axle_address()).torque_share()), drive, double(schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).wheel_inertia()))));
      schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).set_inertia(float(double(schema::CarDataAxle(base, schema::CarData(base, data).rear_axle_address()).wheel_inertia())));
    } else if (mode == 1) {
      CopyWord(base, schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).wheel_inertia_address(), schema::VehicleAxle(base, schema::Vehicle(base, vehicle).front_axle_address()).inertia_address());
      schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).set_inertia(float(Fmadds(double(schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).torque_share()), drive, double(schema::CarDataAxle(base, schema::CarData(base, data).rear_axle_address()).wheel_inertia()))));
    } else {
      schema::VehicleAxle(base, schema::Vehicle(base, vehicle).front_axle_address()).set_inertia(float(Fmadds(double(schema::VehicleAxle(base, schema::Vehicle(base, vehicle).front_axle_address()).torque_share()), drive, double(schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).wheel_inertia()))));
      schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).set_inertia(float(Fmadds(double(schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).torque_share()), drive, double(schema::CarDataAxle(base, schema::CarData(base, data).rear_axle_address()).wheel_inertia()))));
    }
  }
  const double minus_one = Lfs(base, 0x82001BE0);
  const double least = Lfs(base, 0x82002A54);
  const double zero = Lfs(base, kZero);
  uint32_t r3 = vehicle;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    const uint32_t data = guest::LoadU32(base, schema::Vehicle(base, vehicle).wheel_axle_data_address() + 4 * w);
    const uint32_t body = schema::Vehicle(base, vehicle).body().address;
    // The rolling speed (+96: x, 0, 0, 0) and the ground's (+80: along
    // +144, 0, along +128, 0).
    const double rolling = Fmuls(double(schema::VehicleWheel(base, wheel).spin()), double(schema::CarDataAxle(base, data).radius()));
    Stfs(base, f + 140, zero);
    Stfs(base, f + 108, zero);
    Stfs(base, f + 92, zero);
    Stfs(base, f + 100, zero);
    Stfs(base, f + 104, zero);
    Stfs(base, f + 96, rolling);
    va::RigidBodyPointVelocity(base, f + 144, body, schema::VehicleWheel(base, wheel).position_address());
    vl::Vec4Copy(base, f + 128, f + 144);
    Stfs(base, f + 80, double(vl::Vec3DotOf(base, f + 128, schema::VehicleWheel(base, wheel).forward_axis_address())));
    Stfs(base, f + 84, zero);
    Stfs(base, f + 88, double(vl::Vec3DotOf(base, f + 128, schema::VehicleWheel(base, wheel).side_axis_address())));
    const double rolling_length = double(vl::Vec3Length(base, f + 96));
    const double ground_length = double(vl::Vec3Length(base, f + 80));
    double speed =
        double(vl::Vec3Length(base, rolling_length < ground_length ? f + 80 : f + 96));
    vl::Vec3Difference(base, f + 112, f + 96, f + 80);
    double slip = double(vl::Vec3Length(base, f + 112));
    schema::VehicleWheel(base, wheel).set_slip_speed(float(slip));
    if (slip == zero) {
      schema::VehicleWheel(base, wheel).set_lateral_force(float(zero));
      schema::VehicleWheel(base, wheel).set_slip_long(float(zero));
      schema::VehicleWheel(base, wheel).set_slip_side(float(zero));
      schema::VehicleWheel(base, wheel).set_longitudinal_force_2(float(zero));
      schema::VehicleWheel(base, wheel).set_longitudinal_force(float(zero));
      r3 = f + 112;
      CopyWord(base, schema::VehicleWheel(base, wheel).longitudinal_force_address(), schema::VehicleWheel(base, wheel).longitudinal_force_2_address());
      continue;
    }
    const double low = Fdivs(least, double(schema::Vehicle(base, vehicle).substep_dt()));
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
    schema::VehicleWheel(base, wheel).set_slip_long(float(along));
    schema::VehicleWheel(base, wheel).set_slip_angle(float(Fsubs(double(float(rolling_angle)), double(float(ground_angle)))));
    if (ground_along < zero) {
      schema::VehicleWheel(base, wheel).set_slip_long(float(Fmuls(along, minus_one)));
    }
    schema::VehicleWheel(base, wheel).set_slip_side(float(across));
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
    const double load = Fnmsubs(double(schema::VehicleWheel(base, wheel).load()), zero, one);
    const double limit = double(schema::VehicleWheel(base, wheel).force_limit());
    double fl = Fmuls(Fmuls(load, limit), longitudinal);
    double fs = Fmuls(Fmuls(load, limit), lateral);
    // Less the surface's drag x the ground speed's square.
    double drag = Fmuls(VehicleSurfaceValue20(base, vehicle, w), double(schema::VehicleWheel(base, wheel).force_limit()));
    double g = Lfs(base, f + 80);
    fl = Fnmsubs(Fmuls(drag, g), g, fl);
    drag = Fmuls(VehicleSurfaceValue20(base, vehicle, w), double(schema::VehicleWheel(base, wheel).force_limit()));
    g = Lfs(base, f + 88);
    schema::VehicleWheel(base, wheel).set_longitudinal_force(float(fl));
    fs = Fnmsubs(Fmuls(drag, g), g, fs);
    schema::VehicleWheel(base, wheel).set_lateral_force(float(fs));
    r3 = vehicle;
    CopyWord(base, schema::VehicleWheel(base, wheel).longitudinal_force_address(), schema::VehicleWheel(base, wheel).longitudinal_force_2_address());
  }
  return r3;
}

uint32_t VehicleUpdateAtRest(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B2808 Vehicle::UpdateAtRest");
  const uint32_t f = frame;
  const double zero = Lfs(base, kZero);
  uint32_t r3 = vehicle;
  if (schema::Vehicle(base, vehicle).forced_rest() != 0) {
    schema::Vehicle(base, vehicle).set_at_rest(uint8_t(1));
  } else {
    const uint32_t object = schema::Vehicle(base, vehicle).collision_object();
    r3 = object;
    if (object == 0) {
      return 0;
    }
    if (!(double(schema::Vehicle(base, vehicle).speed()) < Lfs(base, kOne)) || !(double(schema::Vehicle(base, vehicle).throttle()) == zero) ||
        !(double(schema::Vehicle(base, vehicle).reverse_throttle()) == zero)) {
      schema::Vehicle(base, vehicle).set_at_rest(uint8_t(0));
    } else {
      // Resting on a contact: weak enough, and the car's center of mass
      // above it (along +96).
      const uint32_t contact = va::CollisionObjectFirstContact(base, object, 0x03000000);
      r3 = contact;
      if (contact != 0 && double(va::ContactStrength(base, contact)) < Lfs(base, 0x82002A20)) {
        const uint32_t m = va::RigidBodyOrientation(schema::Vehicle(base, vehicle).body().address);
        CopyWord(base, m + 48, f + 80);
        Stfs(base, f + 92, zero);
        CopyWord(base, m + 56, f + 88);
        CopyWord(base, m + 52, f + 84);
        vl::Vec3Difference(base, f + 128, va::ContactPosition(contact), f + 80);
        Stfs(base, f + 132, zero);
        const double up = double(vl::Vec3DotOf(base, f + 128, schema::Vehicle(base, vehicle).ground_up_address()));
        r3 = f + 128;
        schema::Vehicle(base, vehicle).set_at_rest(uint8_t(up < zero ? 0 : 1));
      }
    }
  }
  // Still, with no input (or in neutral), wheels down.
  const double speed = Fabs(double(schema::Vehicle(base, vehicle).speed()));
  const bool no_input =
      double(schema::Vehicle(base, vehicle).reverse_throttle()) == zero && double(schema::Vehicle(base, vehicle).throttle()) == zero;
  if ((no_input || uint32_t(schema::Vehicle(base, vehicle).gear()) == 0) &&
      schema::Vehicle(base, vehicle).wheels_on_ground() != 0 && Fabs(speed) < Lfs(base, 0x82002A1C)) {
    schema::Vehicle(base, vehicle).set_at_rest(uint8_t(1));
  }
  if (schema::Vehicle(base, vehicle).at_rest() == 0) {
    return r3;
  }
  const uint32_t gear = uint32_t(schema::Vehicle(base, vehicle).gear());
  schema::Vehicle(base, vehicle).set_gear(int32_t(0));
  schema::Vehicle(base, vehicle).set_gear_previous(int32_t(gear));
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    schema::VehicleWheel(base, wheel).set_spin(float(zero));
    schema::VehicleWheel(base, wheel).set_spin_momentum(float(zero));
    schema::VehicleWheel(base, wheel).set_longitudinal_force(float(zero));
    schema::VehicleWheel(base, wheel).set_lateral_force(float(zero));
  }
  const uint32_t body = schema::Vehicle(base, vehicle).body().address;
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
    const uint32_t from = schema::RigidBody(base, body).angular_momentum_address();
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(base, f + 96 + i, guest::LoadU32(base, from + i));
    }
    Stfs(base, f + 96, zero);
    Stfs(base, f + 100, zero);
    Stfs(base, f + 104, zero);
    vl::Vec4Copy(base, schema::RigidBody(base, body).angular_momentum_address(), f + 96);
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
  return schema::RigidBody(base, body).angular_velocity_address();
}

uint32_t VehicleUpdateAero(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B2A48 Vehicle::UpdateAero");
  const uint32_t f = frame;
  const uint32_t data = CarData(base, vehicle);
  const double zero = Lfs(base, kZero);
  // The drag, against the motion along +112 (forward) and +128 (side).
  const double forward = Lfs(base, schema::Vehicle(base, vehicle).ground_velocity_address());
  const double forward2 = Fmuls(forward, forward);
  const double k = Fmuls(double(schema::CarData(base, data).unknown_1808()), double(schema::Vehicle(base, vehicle).drag_scale()));
  const double scale = Lfs(base, 0x82002A14);
  const double minus_one = Lfs(base, 0x82001BE0);
  double drag = Fmuls(Fmuls(double(schema::CarData(base, data).aero_front()), forward2), k);
  drag = Fmuls(drag, scale);
  if (forward > zero) {
    drag = Fmuls(drag, minus_one);
  }
  const double side = Lfs(base, schema::Vehicle(base, vehicle).ground_velocity_address() + 8);
  double side_drag = Fmuls(Fmuls(Fmuls(side, side), double(schema::CarData(base, data).aero_side())), k);
  side_drag = Fmuls(side_drag, scale);
  if (side > zero) {
    side_drag = Fmuls(side_drag, minus_one);
  }
  vl::Vec4Scaled(base, f + 112, schema::Vehicle(base, vehicle).ground_side_address(), side_drag);
  vl::Vec4Scaled(base, f + 176, schema::Vehicle(base, vehicle).ground_forward_address(), drag);
  vl::Vec3Sum(base, f + 160, f + 176, f + 112);
  vl::Vec3AddTo(base, schema::Vehicle(base, vehicle).body_accumulator_address(), f + 160);
  uint32_t r3 = schema::Vehicle(base, vehicle).body_accumulator_address();
  if (Lfs(base, schema::Vehicle(base, vehicle).ground_velocity_address()) > zero) {
    // The downforce: -(+96) x v^2 x 0.5, x car data +1812 into the front
    // wheels' accumulators, +1816 into the rear's.
    const double down_y = Fneg(Lfs(base, schema::Vehicle(base, vehicle).ground_up_address() + 4));
    Stfs(base, f + 108, zero);
    Stfs(base, f + 100, down_y);
    const double half_v2 = Fmuls(forward2, Lfs(base, 0x82001BE4));
    Stfs(base, f + 96, Fneg(Lfs(base, schema::Vehicle(base, vehicle).ground_up_address())));
    Stfs(base, f + 104, Fneg(Lfs(base, schema::Vehicle(base, vehicle).ground_up_address() + 8)));
    vl::Vec4Scaled(base, f + 112, f + 96, half_v2);
    vl::Vec4Scaled(base, f + 128, f + 112, double(schema::CarData(base, CarData(base, vehicle)).unknown_1812()));
    vl::Vec4Scaled(base, f + 144, f + 112, double(schema::CarData(base, CarData(base, vehicle)).unknown_1816()));
    vl::Vec3AddTo(base, schema::Vehicle(base, vehicle).wheel_accumulators_address() + 16, f + 128);
    vl::Vec3AddTo(base, schema::Vehicle(base, vehicle).wheel_accumulators_address(), f + 128);
    vl::Vec3AddTo(base, schema::Vehicle(base, vehicle).wheel_accumulators_address() + 48, f + 144);
    vl::Vec3AddTo(base, schema::Vehicle(base, vehicle).wheel_accumulators_address() + 32, f + 144);
    r3 = schema::Vehicle(base, vehicle).wheel_accumulators_address() + 32;
  }
  // The engine's shake.
  const double rpm = double(schema::Vehicle(base, vehicle).rpm());
  const uint32_t engine_data = CarData(base, vehicle);
  Stfs(base, f + 92, zero);
  const double torque = VehicleEngineTorque(base, vehicle, rpm, f);
  r3 = vehicle;
  const double peak = double(schema::CarData(base, engine_data).engine_braking());
  const double per = Fdivs(torque, double(schema::CarData(base, engine_data).torque_copy()));
  const double c = Lfs(base, 0x820021F4);
  const double one = Lfs(base, kOne);
  double amount = Fmadds(Fsubs(one, peak), c, Lfs(base, 0x82001FBC));
  const double phase = Fmadds(rpm, c, double(schema::Vehicle(base, vehicle).engine_shake()));
  schema::Vehicle(base, vehicle).set_engine_shake(float(phase));
  amount = Fmuls(amount, per);
  amount = Fmuls(amount, Lfs(base, 0x82002A28));
  if (phase < one) {
    return r3;
  }
  const int32_t count = schema::Vehicle(base, vehicle).engine_shake_count();
  Stfs(base, f + 80, zero);
  Stfs(base, f + 88, zero);
  Stfs(base, f + 92, one);
  const int32_t step = count % 4;
  if (step == 0 || step == 2) {
    const double push = step == 0 ? Fmuls(amount, Lfs(base, 0x82002A24)) : Fmuls(amount, c);
    Stfs(base, f + 84, push);
    vl::Vec3AddTo(base, schema::Vehicle(base, vehicle).wheel_accumulators_address(), f + 80);
    vl::Vec3AddTo(base, schema::Vehicle(base, vehicle).wheel_accumulators_address() + 16, f + 80);
    r3 = schema::Vehicle(base, vehicle).wheel_accumulators_address() + 16;
  }
  const int32_t next = schema::Vehicle(base, vehicle).engine_shake_count();
  schema::Vehicle(base, vehicle).set_engine_shake(float(zero));
  schema::Vehicle(base, vehicle).set_engine_shake_count(next <= 4 ? next + 1 : 0);
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
    vl::Vec4Scaled(base, f + 96, schema::Vehicle(base, vehicle).ground_side_address(), Fneg(s));
    vl::Vec4Scaled(base, f + 112, schema::Vehicle(base, vehicle).ground_forward_address(), c);
    vl::Vec3Sum(base, f + 128, f + 112, f + 96);
    vl::Vec4Copy(base, schema::VehicleWheel(base, wheel).forward_axis_address(), f + 128);
  } else {
    vl::Vec4Scaled(base, f + 128, schema::Vehicle(base, vehicle).ground_side_address(), Fneg(s));
    vl::Vec4Scaled(base, f + 112, schema::Vehicle(base, vehicle).ground_forward_address(), c);
    vl::Vec3Sum(base, f + 96, f + 112, f + 128);
    vl::Vec4Copy(base, schema::VehicleWheel(base, wheel).forward_axis_address(), f + 96);
  }
  vl::Vec4Scaled(base, f + 128, schema::Vehicle(base, vehicle).ground_side_address(), c);
  vl::Vec4Scaled(base, f + 112, schema::Vehicle(base, vehicle).ground_forward_address(), s);
  vl::Vec3Sum(base, f + 96, f + 112, f + 128);
  vl::Vec4Copy(base, schema::VehicleWheel(base, wheel).side_axis_address(), f + 96);
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
    const double steer1 = double(schema::Vehicle(base, vehicle).front_right_angle());
    const double steer0 = double(schema::Vehicle(base, vehicle).front_left_angle());
    const double toe = Lfs(base, schema::CarDataAxle(base, schema::CarData(base, data).front_axle_address()).angles_address() + 8);
    const double a0 = double(float(c_runtime::Atan2(double(schema::VehicleWheel(base, w0).travel()), double(schema::VehicleWheel(base, w0).lateral_offset()))));
    const double a1 = double(float(c_runtime::Atan2(double(schema::VehicleWheel(base, w1).travel()), Fneg(double(schema::VehicleWheel(base, w1).lateral_offset())))));
    double t0 = Fmadds(double(schema::VehicleWheel(base, w0).force_integral_2()), k_fc, Fmuls(a0, k_f8));
    t0 = Fadds(t0, toe);
    schema::VehicleWheel(base, w0).set_toe(float(t0));
    const double angle0 = Fadds(t0, steer0);
    double t1 = Fmadds(double(schema::VehicleWheel(base, w1).force_integral_2()), k_fc, Fmuls(a1, cfbc));
    t1 = Fsubs(t1, toe);
    schema::VehicleWheel(base, w1).set_toe(float(t1));
    TurnedAxes(base, vehicle, w0, angle0, f, true);
    TurnedAxes(base, vehicle, w1, Fadds(double(schema::VehicleWheel(base, w1).toe()), steer1), f, false);
  }
  // The rear: no steering (+304 x 0.0), and the vehicle's axes into +2080
  // and +2064.
  {
    const double rear_steer = Fmuls(double(schema::Vehicle(base, vehicle).steering()), Lfs(base, kZero));
    vl::Vec4Copy(base, schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).forward_axis_address(), schema::Vehicle(base, vehicle).ground_forward_address());
    vl::Vec4Copy(base, schema::VehicleAxle(base, schema::Vehicle(base, vehicle).rear_axle_address()).side_axis_address(), schema::Vehicle(base, vehicle).ground_side_address());
    const uint32_t data = CarData(base, vehicle);
    const double toe = Lfs(base, schema::CarDataAxle(base, schema::CarData(base, data).rear_axle_address()).angles_address() + 8);
    const double a2 = double(float(c_runtime::Atan2(double(schema::VehicleWheel(base, w2).travel()), double(schema::VehicleWheel(base, w2).lateral_offset()))));
    const double a3 = double(float(c_runtime::Atan2(double(schema::VehicleWheel(base, w3).travel()), Fneg(double(schema::VehicleWheel(base, w3).lateral_offset())))));
    const double k_f4 = Lfs(base, 0x820029F4);
    const double m2 = Fmuls(double(schema::VehicleWheel(base, w2).force_integral_2()), k_f4);
    const double m3 = Fmuls(double(schema::VehicleWheel(base, w3).force_integral_2()), k_f4);
    double t2 = Fmsubs(a2, cfbc, m2);
    t2 = Fadds(t2, toe);
    schema::VehicleWheel(base, w2).set_toe(float(t2));
    const double angle2 = Fadds(t2, rear_steer);
    double t3 = Fmsubs(a3, k_f8, m3);
    t3 = Fsubs(t3, toe);
    schema::VehicleWheel(base, w3).set_toe(float(t3));
    TurnedAxes(base, vehicle, w2, angle2, f, false);
    TurnedAxes(base, vehicle, w3, Fadds(rear_steer, double(schema::VehicleWheel(base, w3).toe())), f, false);
  }
  // Per wheel: the contact's velocity and speeds, the camber and the
  // wheel's quaternion (about the x axis at 0x82638270).
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    CopyWord(base, schema::VehicleWheel(base, wheel).forward_speed_address(), schema::VehicleWheel(base, wheel).forward_speed_previous_address());
    const uint32_t data = guest::LoadU32(base, schema::Vehicle(base, vehicle).wheel_axle_data_address() + 4 * w);
    va::RigidBodyPointVelocity(base, f + 128, schema::Vehicle(base, vehicle).body().address, schema::VehicleWheel(base, wheel).contact_point_address());
    vl::Vec4Copy(base, schema::VehicleWheel(base, wheel).contact_velocity_address(), f + 128);
    schema::VehicleWheel(base, wheel).set_forward_speed(float(double(vl::Vec3DotOf(base, schema::VehicleWheel(base, wheel).contact_velocity_address(), schema::VehicleWheel(base, wheel).forward_axis_address()))));
    schema::VehicleWheel(base, wheel).set_side_speed(float(double(vl::Vec3DotOf(base, schema::VehicleWheel(base, wheel).contact_velocity_address(), schema::VehicleWheel(base, wheel).side_axis_address()))));
    double turn;
    if (w == 0 || w == 2) {
      const double a = double(float(c_runtime::Atan2(double(schema::VehicleWheel(base, wheel).travel()), double(schema::VehicleWheel(base, wheel).lateral_offset()))));
      schema::VehicleWheel(base, wheel).set_camber(float(Fnmsubs(a, double(schema::CarDataAxle(base, data).unknown_48()), Lfs(base, schema::CarDataAxle(base, data).angles_address()))));
      const double b = double(float(c_runtime::Atan2(double(schema::VehicleWheel(base, wheel).travel()), double(schema::VehicleWheel(base, wheel).lateral_offset()))));
      turn = Fnmsubs(b, double(schema::CarDataAxle(base, data).unknown_48()), Lfs(base, schema::CarDataAxle(base, data).angles_address() + 4));
    } else {
      const double a =
          double(float(c_runtime::Atan2(double(schema::VehicleWheel(base, wheel).travel()), Fneg(double(schema::VehicleWheel(base, wheel).lateral_offset())))));
      schema::VehicleWheel(base, wheel).set_camber(float(Fmsubs(a, double(schema::CarDataAxle(base, data).unknown_48()), Lfs(base, schema::CarDataAxle(base, data).angles_address()))));
      const double b =
          double(float(c_runtime::Atan2(double(schema::VehicleWheel(base, wheel).travel()), Fneg(double(schema::VehicleWheel(base, wheel).lateral_offset())))));
      turn = Fmsubs(b, double(schema::CarDataAxle(base, data).unknown_48()), Lfs(base, schema::CarDataAxle(base, data).angles_address() + 4));
    }
    mm::QuaternionFromAxisAngle(base, schema::VehicleWheel(base, wheel).quaternion_address(), 0x82638270, turn, f - 32);
  }
  return schema::VehicleWheel(base, w3).quaternion_address();
}

uint32_t VehicleApplyForces(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B2CC0 Vehicle::ApplyForces");
  const uint32_t v = vehicle, f = frame;
  // The speed squared.
  const double speed = double(schema::Vehicle(base, v).speed());
  schema::Vehicle(base, v).set_speed_squared(float(Fmuls(speed, speed)));
  // The center of mass: the car data's +96 with wheels down, +112 in the
  // air, set when byte +3132 changes.
  const bool down = schema::Vehicle(base, v).wheels_on_ground() != 0;
  const uint8_t airborne = schema::Vehicle(base, v).upright();
  if (down ? airborne != 0 : airborne == 0) {
    schema::Vehicle(base, v).set_upright(uint8_t(down ? 0 : 1));
    const uint32_t offset = schema::Vehicle(base, v).car_data().address + (down ? 96 : 112);
    va::RigidBodySetCenterOfMass(base, schema::Vehicle(base, v).body().address, offset);
  }
  const uint32_t sum = schema::Vehicle(base, v).wheel_forces_ground_address();
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyWord(base, kZero, f + 80 + i);
  }
  vl::Vec4Copy(base, sum, f + 80);
  // Two dot products the original drops.
  vl::Vec3DotOf(base, v, schema::Vehicle(base, v).ground_forward_address());
  vl::Vec3DotOf(base, schema::Vehicle(base, v).body_row2_address(), schema::Vehicle(base, v).ground_side_address());
  const double tip_limit_in_air = Lfs(base, kTipInAir);
  const double tip_limit_down = Lfs(base, kTipDown);
  const double tip_limit_upright = Lfs(base, kTipUpright);
  const uint32_t up = schema::Vehicle(base, v).body_row1_address();
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t accumulator = schema::Vehicle(base, v).wheel_accumulators_address() + 16 * w;
    // The wheel's force in the ground frame, summed, and given to the body
    // at its contact point (the wheel's +160, 16 bytes copied).
    mm::InverseRotate(base, f + 112, schema::Vehicle(base, v).ground_matrix_address(), accumulator);
    vl::Vec3AddTo(base, sum, f + 112);
    const uint32_t point = schema::VehicleWheel(base, Wheel(v, w)).contact_point_address();
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(base, f + 128 + i, guest::LoadU32(base, point + i));
    }
    va::RigidBodyAddForceAt(base, schema::Vehicle(base, v).body().address, accumulator, f + 128);
    // Tipped too far: the angular velocity set to up x +96, or zero.
    uint32_t spin = 0;
    if (schema::Vehicle(base, v).wheels_on_ground() != 0) {
      schema::Vehicle(base, v).set_tipped(uint8_t(0));
      if (!(double(vl::Vec3DotOf(base, up, kUp)) < tip_limit_down)) {
        continue;
      }
      vl::Vec3Cross(base, f + 160, up, schema::Vehicle(base, v).ground_up_address());
      spin = f + 160;
    } else if (schema::Vehicle(base, v).tipped() == 0) {
      if (!(double(vl::Vec3DotOf(base, up, kUp)) < tip_limit_in_air)) {
        schema::Vehicle(base, v).set_tipped(uint8_t(1));
        continue;
      }
      vl::Vec3Cross(base, f + 176, up, schema::Vehicle(base, v).ground_up_address());
      spin = f + 176;
    } else {
      const double tilt = double(vl::Vec3DotOf(base, up, kUp));
      if (!(tilt < tip_limit_upright)) {
        continue;
      }
      for (uint32_t i = 0; i < 16; i += 4) {
        guest::StoreU32(base, f + 144 + i, guest::LoadU32(base, kZeroVector + i));
      }
      const uint32_t angular = va::RigidBodyAngularVelocity(schema::Vehicle(base, v).body().address);
      vl::Vec3Cross(base, f + 192, angular, up);
      if (!(tilt > double(vl::Vec3DotOf(base, f + 192, kUp)))) {
        continue;
      }
      spin = f + 144;
    }
    RigidBodySetAngularVelocity(base, schema::Vehicle(base, v).body().address, spin, f - 160);
  }
  // +80, +84 from +2976, +2980 by +376 and the summed force's x.
  const double a = Fmuls(double(schema::Vehicle(base, v).gauge_2()), Lfs(base, schema::Vehicle(base, v).wheel_forces_ground_address() + 8));
  const double b = Fmuls(double(schema::Vehicle(base, v).gauge_3()), Lfs(base, sum));
  schema::Vehicle(base, v).set_unknown_80(float(a));
  schema::Vehicle(base, v).set_unknown_84(float(b));
  // The body's force before (+2464), then +2608 added; at rest the
  // horizontal part cancelled.
  const uint32_t body = schema::Vehicle(base, v).body().address;
  vl::Vec4Copy(base, schema::Vehicle(base, v).body_force_address(), va::RigidBodyForce(body));
  va::RigidBodyAddForce(base, schema::Vehicle(base, v).body().address, schema::Vehicle(base, v).body_accumulator_address());
  const uint32_t force = va::RigidBodyForce(schema::Vehicle(base, v).body().address);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 112 + i, guest::LoadU32(base, force + i));
  }
  if (schema::Vehicle(base, v).at_rest() != 0) {
    // fneg between an lfs and an stfs: the sign bit flipped.
    guest::StoreU32(base, f + 96, guest::LoadU32(base, f + 112) ^ 0x80000000u);
    const uint32_t z = guest::LoadU32(base, f + 120) ^ 0x80000000u;
    CopyWord(base, kZero, f + 100);
    guest::StoreU32(base, f + 104, z);
    CopyWord(base, kZero, f + 108);
    va::RigidBodyAddForce(base, schema::Vehicle(base, v).body().address, f + 96);
    CopyWord(base, kZero, schema::Vehicle(base, v).body_force_address());
    CopyWord(base, kZero, schema::Vehicle(base, v).body_force_address() + 8);
  }
  vl::Vec4Copy(base, schema::Vehicle(base, v).body_torque_address(), va::RigidBodyTorque(schema::Vehicle(base, v).body().address));
  const uint32_t r3 = RigidBodyIntegrate(base, schema::Vehicle(base, v).body().address,
                                         double(schema::Vehicle(base, v).dt()), f - 208);
  // The body's word +516: bit 0 accelerating (throttle +284 above 0.5 in a
  // forward gear, +288 in reverse), bit 1 wheels on the ground.
  const int32_t gear = schema::Vehicle(base, v).gear();
  const double half = Lfs(base, kHalfConstant);
  const bool accelerating = (gear > 0 && double(schema::Vehicle(base, v).throttle()) > half) ||
                            (gear < 0 && double(schema::Vehicle(base, v).reverse_throttle()) > half);
  schema::RigidBody flags_body = schema::Vehicle(base, v).body();
  uint32_t bits = flags_body.flags();
  flags_body.set_flags(accelerating ? bits | 1 : bits & ~1u);
  flags_body = schema::Vehicle(base, v).body();
  bits = flags_body.flags();
  flags_body.set_flags(schema::Vehicle(base, v).wheels_on_ground() != 0 ? bits | 2 : bits & ~2u);
  return r3;
}

}  // namespace car_physics
