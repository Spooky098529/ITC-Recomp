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

uint32_t CarData(uint32_t vehicle) {
  return schema::Vehicle(vehicle).car_data().address;
}

// fcfid then frsp of a 64-bit integer stored at `at` (the original's std
// then lfd): the integer's float.
double IntegerFloat(uint32_t at, int64_t value) {
  guest::StoreU32(at, uint32_t(uint64_t(value) >> 32));
  guest::StoreU32(at + 4, uint32_t(value));
  return double(float(double(value)));
}

}  // namespace

void MatrixCopyRows(uint32_t out, uint32_t in) {
  ITC_NATIVE_RUNNING("sub_820AFB78 Matrix::CopyRows");
  for (uint32_t row = 0; row < 64; row += 16) {
    vl::Vec4Copy(out + row, in + row);
  }
}

void MatrixMultiplyTranslation(uint32_t out, uint32_t m, uint32_t vector,
                               uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B0D68 Matrix::MultiplyTranslation");
  const uint32_t f = frame;
  // z, y, x and 1.0 at +80 (each an lfs then stfs: its bits).
  CopyWord(schema::PhysicsVector(vector).z_address(), f + 80);
  CopyWord(schema::PhysicsVector(vector).y_address(), f + 84);
  CopyWord(schema::PhysicsVector(vector).x_address(), f + 88);
  CopyWord(kOne, f + 92);
  // The rows: the identity's three, then (x, y, z, 1.0) (the vector unit's
  // shuffles: the words' bits).
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 3; row++) {
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreU32(f + 112 + 16 * row + 4 * i, row == i ? kOneBits : 0);
    }
  }
  guest::StoreU32(f + 160, guest::LoadU32(f + 88));
  guest::StoreU32(f + 164, guest::LoadU32(f + 84));
  guest::StoreU32(f + 168, guest::LoadU32(f + 80));
  guest::StoreU32(f + 172, guest::LoadU32(f + 92));
  mm::Multiply(out, m, f + 112);
  interpolation::BlendState_OnMultiply(out, m, f + 112, 0x820B0E00);
}

void VehicleTractionControl(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B1790 Vehicle::TractionControl");
  const double throttle = double(schema::Vehicle(vehicle).throttle());
  schema::Vehicle(vehicle).set_traction_active(uint8_t(0));
  const double half = Lfs(0x82001BE4);
  // The driven pair: the rear when the front's share is below 0.5.
  const uint32_t first = double(schema::CarData(CarData(vehicle)).front_torque_share()) < half ? 2 : 0;
  const uint32_t a = Wheel(vehicle, first), b = Wheel(vehicle, first + 1);
  const double limit = double(schema::Vehicle(vehicle).traction_limit());
  const double slip = Fmuls(Fadds(double(schema::VehicleWheel(b).slip_long()), double(schema::VehicleWheel(a).slip_long())), half);
  const double spin = Fabs(Fadds(double(schema::VehicleWheel(b).spin()), double(schema::VehicleWheel(a).spin())));
  bool rising = false;
  if (slip > limit) {
    const double threshold = Lfs(0x82000D44);
    if (Fmuls(spin, half) > threshold) {
      rising = true;
      const int32_t count = int32_t(schema::Vehicle(vehicle).traction_counter() + 1);
      schema::Vehicle(vehicle).set_traction_counter(uint32_t(uint32_t(count)));
      // The count against the same threshold (as the original).
      if (IntegerFloat(frame - 16, count) > threshold) {
        schema::Vehicle(vehicle).set_traction_counter(uint32_t(30));
      }
    }
  }
  if (!rising) {
    const int32_t count = int32_t(schema::Vehicle(vehicle).traction_counter() - 1);
    schema::Vehicle(vehicle).set_traction_counter(uint32_t(uint32_t(count)));
    if (count < 0) {
      schema::Vehicle(vehicle).set_traction_counter(uint32_t(0));
    }
  }
  const double per = Lfs(0x820029EC);
  const int32_t count = int32_t(schema::Vehicle(vehicle).traction_counter());
  const double counted = IntegerFloat(frame - 16, count);
  const double one = Lfs(kOne);
  const double rest = Fnmsubs(counted, per, one);
  const double zero = Lfs(kZero);
  double scale;
  if (rest < zero) {
    scale = zero;
  } else if (rest > one) {
    scale = one;
  } else {
    scale = rest;
  }
  const double cut = Fmuls(throttle, scale);
  schema::Vehicle(vehicle).set_throttle(float(cut));
  if (!(throttle == cut)) {
    schema::Vehicle(vehicle).set_traction_active(uint8_t(1));
  }
}

void VehicleUpdateTireGrip(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B1FB8 Vehicle::UpdateTireGrip");
  const double low = Lfs(0x82002A00);
  const double high = Lfs(0x8200296C);
  const double one = Lfs(kOne);
  const double camber_scale = Lfs(0x82002808);
  const double zero = Lfs(kZero);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (schema::VehicleWheel(wheel).on_ground() == 0) {
      schema::VehicleWheel(wheel).set_grip(float(zero));
      schema::VehicleWheel(wheel).set_force_limit(float(zero));
      continue;
    }
    const uint32_t record = guest::LoadU32(schema::Vehicle(vehicle).wheel_axles_address() + 4 * w);
    const uint32_t data = guest::LoadU32(schema::Vehicle(vehicle).wheel_axle_data_address() + 4 * w);
    const double surface = VehicleSurfaceValue12(vehicle, w);
    const double camber = Fadds(double(schema::Vehicle(vehicle).roll()), double(schema::VehicleWheel(wheel).camber()));
    const double rated = Fmuls(double(schema::CarDataAxle(data).unknown_140()), double(schema::CarDataAxle(data).unknown_144()));
    const double loss = Fabs(Fmuls(camber, camber_scale));
    double ratio = Fmuls(Fdivs(double(schema::VehicleWheel(wheel).load()), rated), high);
    const double kept = Fsubs(one, loss);
    if (ratio > high) {
      ratio = high;
    } else if (ratio < low) {
      ratio = low;
    }
    const double force = TireMagicFormula(0x82600D20, ratio);
    double grip = Fmuls(double(schema::CarDataAxle(data).unknown_148()), force);
    grip = Fmuls(grip, kept);
    grip = Fmuls(grip, surface);
    if (grip < zero) {
      grip = zero;
    }
    grip = Fmuls(double(schema::VehicleAxle(record).grip()), grip);
    schema::VehicleWheel(wheel).set_grip(float(grip));
    double limit = Fmuls(double(schema::CarDataAxle(data).unknown_140()), double(schema::CarDataAxle(data).unknown_144()));
    limit = Fmuls(limit, double(schema::Vehicle(vehicle).grip_scale()));
    schema::VehicleWheel(wheel).set_force_limit(float(Fmuls(limit, grip)));
  }
}

void VehicleUpdateEngineTorque(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B22D8 Vehicle::UpdateEngineTorque");
  const uint32_t data = CarData(vehicle);
  const int32_t gear = schema::Vehicle(vehicle).gear();
  const double limiter_rpm = double(schema::CarData(data).rev_limit_normalized());
  double throttle = Lfs(vehicle + (gear == -1 ? 288 : 284));
  const double zero = Lfs(kZero);
  // The rev limiter: its timer +2244 runs down, the throttle cut meanwhile;
  // set again at the limiter's rpm.
  const double timer = double(schema::Vehicle(vehicle).rev_limiter());
  if (timer > zero) {
    const double left = Fsubs(timer, double(schema::Vehicle(vehicle).dt()));
    schema::Vehicle(vehicle).set_rev_limiter(float(left));
    if (left < zero) {
      schema::Vehicle(vehicle).set_unknown_2248(uint8_t(0));
    }
    throttle = zero;
  } else if (!(double(schema::Vehicle(vehicle).rpm()) < limiter_rpm)) {
    CopyWord(0x82002204, schema::Vehicle(vehicle).rev_limiter_address());
    schema::Vehicle(vehicle).set_unknown_2248(uint8_t(1));
  } else {
    schema::Vehicle(vehicle).set_rev_limiter(float(zero));
    schema::Vehicle(vehicle).set_unknown_2248(uint8_t(0));
  }
  // The clutch: no throttle while it's out, without the handbrake, between
  // gears.
  const double clutch = double(schema::Vehicle(vehicle).clutch());
  const double one = Lfs(kOne);
  if (!(clutch == one) && double(schema::Vehicle(vehicle).handbrake()) == zero) {
    const int32_t next = schema::Vehicle(vehicle).gear_previous();
    if (next != 0 && gear != 0 && next < gear) {
      throttle = zero;
    }
  }
  const double rpm = double(schema::Vehicle(vehicle).rpm());
  schema::Vehicle(vehicle).set_throttle_used(float(throttle));
  const double curve = VehicleEngineTorque(vehicle, rpm, frame);
  const double peak = Fmuls(double(schema::CarData(data).engine_braking()), curve);
  double torque = Fmuls(double(schema::Vehicle(vehicle).torque_scale()), peak);
  torque = Fmuls(torque, throttle);
  if (schema::Vehicle(vehicle).nitrous_on() != 0) {
    // Nitrous above an rpm, while it lasts.
    const double from = Fmuls(double(schema::CarData(data).idle_rpm()), Lfs(0x820029D4));
    bool stop = false;
    if (!(from < rpm)) {
      stop = true;
    } else {
      const double left = double(schema::Vehicle(vehicle).nitrous_value());
      if (!(left == zero) && throttle > Lfs(0x82001BE4)) {
        const double boost = Fmuls(double(schema::Vehicle(vehicle).nitrous_factor()), throttle);
        const double rest = Fsubs(left, throttle);
        schema::Vehicle(vehicle).set_nitrous_value(float(rest));
        torque = Fmuls(boost, torque);
        if (!(rest > zero)) {
          schema::Vehicle(vehicle).set_nitrous_value(float(zero));
          stop = true;
        }
      }
    }
    if (stop) {
      schema::Vehicle(vehicle).set_nitrous_on(uint8_t(0));
    }
  }
  // Engine braking.
  const double off = Fsubs(one, throttle);
  const double idle = double(schema::CarData(data).idle_rpm());
  torque = Fmuls(double(schema::Vehicle(vehicle).oil_factor()), torque);
  const double above = Fsubs(rpm, idle);
  const double braking_scale = double(schema::CarData(data).engine_braking_2());
  const double range = Fsubs(limiter_rpm, idle);
  double braking = Fmuls(above, off);
  braking = Fmuls(braking, braking_scale);
  braking = Fmuls(braking, peak);
  const double net = Fsubs(torque, Fdivs(braking, range));
  schema::Vehicle(vehicle).set_engine_torque(float(net));
  schema::Vehicle(vehicle).set_engine_torque_2236(float(net));
  if (gear != 0 && !(clutch == zero)) {
    schema::Vehicle(vehicle).set_engine_torque_2236(float(net));
    if (gear != -1) {
      return;
    }
    schema::Vehicle(vehicle).set_engine_torque_2236(float(Fmuls(net, Lfs(0x82001BE0))));
    return;
  }
  schema::Vehicle(vehicle).set_engine_torque_2236(float(net));
}

void VehicleUpdateBrakeHeat(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B35C8 Vehicle::UpdateBrakeHeat");
  const double speed = double(schema::Vehicle(vehicle).speed());
  const double speed2 = Fmuls(speed, speed);
  const double dt = double(schema::Vehicle(vehicle).dt());
  const double by_speed = double(schema::CarPhysicsGlobals::brake_cooling_by_speed());
  const double cooling_base = Lfs(0x82600CD8);
  const double fade_max = Lfs(0x82002A0C);
  const double fade_rate = Lfs(0x82002208);
  const double zero = Lfs(kZero);
  const double cooling = Fmuls(Fmadds(speed2, by_speed, cooling_base), dt);
  const double ambient = Lfs(0x82002568);
  // The heat added: kept from one braking wheel to the next.
  double added = zero;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (double(schema::Vehicle(vehicle).brake()) > zero) {
      const uint32_t data = CarData(vehicle);
      if (Lfs(data + (w < 2 ? 264 : 268)) > zero) {
        double heat = Fmuls(double(schema::VehicleWheel(wheel).spin()), double(schema::CarDataAxle(schema::CarData(data).front_axle_address()).radius()));
        heat = Fmuls(heat, double(schema::VehicleWheel(wheel).brake_force()));
        heat = Fmuls(heat, double(schema::Vehicle(vehicle).dt()));
        added = Fmuls(heat, Lfs(0x82600CD4));
      }
    }
    double t = Fadds(added, double(schema::VehicleWheel(wheel).brake_heat()));
    schema::VehicleWheel(wheel).set_brake_heat(float(t));
    const double span = Fsubs(double(schema::CarData(CarData(vehicle)).brake_heat_limit()), ambient);
    const double cool = Fdivs(Fmuls(Fsubs(t, ambient), cooling), span);
    t = Fsubs(t, cool);
    schema::VehicleWheel(wheel).set_brake_heat(float(t));
    const double t_max = double(schema::CarData(CarData(vehicle)).brake_heat_limit());
    if (t < ambient) {
      t = ambient;
    } else if (t_max < t) {
      t = t_max;
    }
    schema::VehicleWheel(wheel).set_brake_heat(float(t));
    if (!(double(schema::Vehicle(vehicle).brake()) > zero) || !(added > cooling)) {
      continue;
    }
    const uint32_t data = CarData(vehicle);
    const double fade_from = double(schema::CarData(data).brake_heat_start());
    if (!(t > fade_from)) {
      continue;
    }
    double fade = Fdivs(Fsubs(t, fade_from), double(schema::CarData(data).brake_heat_limit()));
    fade = Fmadds(fade, fade_rate, double(schema::Vehicle(vehicle).brake_fade()));
    schema::Vehicle(vehicle).set_brake_fade(float(fade));
    if (fade < zero) {
      fade = zero;
    } else if (fade > fade_max) {
      fade = fade_max;
    }
    schema::Vehicle(vehicle).set_brake_fade(float(fade));
  }
  double fade = Fnmsubs(double(schema::Vehicle(vehicle).dt()), Lfs(0x82001BE4),
                        double(schema::Vehicle(vehicle).brake_fade()));
  schema::Vehicle(vehicle).set_brake_fade(float(fade));
  if (fade < zero) {
    fade = zero;
  } else if (fade > fade_max) {
    fade = fade_max;
  }
  schema::Vehicle(vehicle).set_brake_fade(float(fade));
  schema::Vehicle(vehicle).set_brake_fade_copy(float(fade));
}

void VehicleUpdateBackfire(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B3788 Vehicle::UpdateBackfire");
  const schema::CarData engine_data(CarData(vehicle));
  const double top = Lfs(engine_data.rev_limit_normalized_address());
  const double from_rpm = Fmuls(top, Lfs(0x82002A3C));
  const double half_rpm = Fmuls(top, Lfs(0x82001BE4));
  const double zero = Lfs(kZero);
  if (schema::Vehicle(vehicle).unknown_2512() == 0) {
    schema::Vehicle(vehicle).set_backfire_strength(float(zero));
    schema::Vehicle(vehicle).set_backfire(uint8_t(0));
    schema::Vehicle(vehicle).set_backfire_charge(float(zero));
    return;
  }
  const double throttle = double(schema::Vehicle(vehicle).throttle_used());
  const double dt = double(schema::Vehicle(vehicle).dt());
  double charge;
  // The charge builds on throttle at high rpm in a forward gear.
  if (throttle > Lfs(0x82001070) && double(schema::Vehicle(vehicle).rpm()) > half_rpm &&
      schema::Vehicle(vehicle).gear() > 0 &&
      schema::Vehicle(vehicle).backfire_latch() == 0) {
    const int64_t gear = schema::Vehicle(vehicle).gear();
    const int64_t count = int32_t(schema::Vehicle(vehicle).unknown_2524());
    double rate = Lfs(0x82002A38);
    if (engine_data.turbo() != 0) {
      rate = Fmuls(Lfs(engine_data.boost_1620_address()), rate);
      IntegerFloat(frame - 16, gear);
      IntegerFloat(frame - 8, count);
    } else {
      IntegerFloat(frame - 8, gear);
      IntegerFloat(frame - 16, count);
    }
    const double counted = double(float(double(count)));
    const double geared = double(float(double(gear)));
    const double k = Fdivs(Fmuls(counted, dt), geared);
    charge = Fmadds(k, rate, double(schema::Vehicle(vehicle).backfire_charge()));
  } else {
    charge = Fsubs(double(schema::Vehicle(vehicle).backfire_charge()), dt);
  }
  const int64_t count = int32_t(schema::Vehicle(vehicle).unknown_2524());
  schema::Vehicle(vehicle).set_backfire_charge(float(charge));
  const double counted = IntegerFloat(frame - 8, count);
  const double full =
      Fmadds(Fmuls(dt, counted), Lfs(0x820021F8), Lfs(0x820021F4));
  const double ready = Lfs(0x820021F4);
  if (charge < zero) {
    charge = zero;
  } else if (full < charge) {
    charge = full;
  }
  schema::Vehicle(vehicle).set_backfire_charge(float(charge));
  if (charge < ready) {
    const uint8_t latched = schema::Vehicle(vehicle).backfire_latch();
    schema::Vehicle(vehicle).set_backfire_strength(float(zero));
    schema::Vehicle(vehicle).set_backfire(uint8_t(0));
    if (latched != 0) {
      schema::Vehicle(vehicle).set_backfire_charge(float(zero));
    }
    schema::Vehicle(vehicle).set_backfire_latch(uint8_t(0));
    return;
  }
  // Charged: it fires when the throttle drops, or on a shift.
  const double dropped = Fsubs(double(schema::Vehicle(vehicle).throttle_used_previous()), throttle);
  bool fire = dropped > Lfs(0x82002208);
  if (!fire && schema::Vehicle(vehicle).unknown_2281() != 0 &&
      uint32_t(schema::Vehicle(vehicle).gear_previous()) != 0) {
    fire = true;
  }
  if (!fire) {
    if (schema::Vehicle(vehicle).backfire_latch() != 0) {
      fire = true;
    } else {
      schema::Vehicle(vehicle).set_backfire_latch(uint8_t(0));
      schema::Vehicle(vehicle).set_backfire_strength(float(zero));
      schema::Vehicle(vehicle).set_backfire(uint8_t(0));
      return;
    }
  }
  const double rpm = double(schema::Vehicle(vehicle).rpm());
  const int32_t gear = schema::Vehicle(vehicle).gear();
  if (!(rpm > from_rpm) || gear <= 0) {
    schema::Vehicle(vehicle).set_backfire_charge(float(zero));
    schema::Vehicle(vehicle).set_backfire_strength(float(zero));
    schema::Vehicle(vehicle).set_backfire(uint8_t(0));
    return;
  }
  double strength = Fsubs(rpm, from_rpm);
  schema::Vehicle(vehicle).set_backfire(uint8_t(1));
  const double range = Lfs(engine_data.rev_limit_normalized_address());
  schema::Vehicle(vehicle).set_backfire_charge(float(zero));
  schema::Vehicle(vehicle).set_backfire_latch(uint8_t(0));
  strength = Fdivs(strength, range);
  strength = Fmuls(strength, counted);
  strength = Fdivs(strength, IntegerFloat(frame - 8, gear));
  schema::Vehicle(vehicle).set_backfire_strength(float(strength));
}

void VehicleUpdateTireWear(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B39C8 Vehicle::UpdateTireWear");
  const double one = Lfs(kOne);
  const double zero = Lfs(kZero);
  if (schema::Vehicle(vehicle).tire_wear_mode() == 0) {
    for (uint32_t w = 0; w < 4; w++) {
      schema::VehicleWheel(Wheel(vehicle, w)).set_tire_wear(float(zero));
    }
  } else {
    const uint32_t shares = frame + 80;
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(shares + i, 0);
    }
    const double cap = Lfs(0x82002204);
    const double scale = Lfs(0x82001FBC);
    const double mode2 = Lfs(0x8200299C);
    const double mode1 = Lfs(0x820029A0);
    const double rate = Lfs(0x82002A44);
    for (uint32_t w = 0; w < 4; w++) {
      const uint32_t wheel = Wheel(vehicle, w);
      if (!(double(schema::VehicleWheel(wheel).tire_wear()) < one)) {
        continue;
      }
      const uint32_t data = guest::LoadU32(schema::Vehicle(vehicle).wheel_axle_data_address() + 4 * w);
      const double surface = VehicleSurfaceValue16(vehicle, w);
      double wear = Fmuls(double(schema::VehicleWheel(wheel).load()), double(schema::VehicleWheel(wheel).slip_speed()));
      const double k = double(schema::Vehicle(vehicle).unknown_2436());
      const double dt = double(schema::Vehicle(vehicle).dt());
      const double tire = Lfs(schema::CarDataAxle(data).unknown_180_address());
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
      const uint32_t mode = schema::Vehicle(vehicle).tire_wear_mode();
      if (mode == 2) {
        wear = Fmuls(wear, mode2);
      } else if (mode == 1) {
        wear = Fmuls(wear, mode1);
      }
      Stfs(shares + 4 * w, wear);
    }
    double sum = zero;
    for (uint32_t w = 0; w < 4; w++) {
      sum = Fadds(Lfs(shares + 4 * w), sum);
    }
    const double each = Fmuls(sum, Lfs(0x82002968));
    for (uint32_t w = 0; w < 4; w++) {
      const uint32_t at = schema::VehicleWheel(Wheel(vehicle, w)).tire_wear_address();
      const double worn = Fadds(Lfs(at), each);
      Stfs(at, worn);
      if (worn > one) {
        Stfs(at, one);
      }
    }
  }
  // Each axle's grip from its tires' mean wear.
  const double brake_scale = Lfs(0x82002A40);
  const double half = Lfs(0x82001BE4);
  for (uint32_t axle = 0; axle < 2; axle++) {
    const uint32_t a = schema::VehicleWheel(Wheel(vehicle, 2 * axle)).tire_wear_address();
    const double worn_b = Lfs(a + kWheelBytes);
    const uint32_t data = guest::LoadU32(schema::Vehicle(vehicle).wheel_axle_data_address() + 8 * axle);
    const double worn_a = Lfs(a);
    const double sum = Fadds(worn_b, worn_a);
    const double mix = double(schema::Vehicle(vehicle).unknown_2628());
    const double rest = Fsubs(one, mix);
    const uint32_t record = guest::LoadU32(schema::Vehicle(vehicle).wheel_axles_address() + 8 * axle);
    const double other = Fmuls(Lfs(schema::CarDataAxle(data).unknown_180_address() + 12), mix);
    const double mean = Fmuls(sum, half);
    double loss = Fmuls(mean, mean);
    double grip = Fmadds(rest, Lfs(schema::CarDataAxle(data).unknown_180_address() + 8), other);
    loss = Fmuls(loss, loss);
    if (schema::Vehicle(vehicle).unknown_2625() != 0) {
      loss = zero;
    }
    schema::VehicleAxle(record).set_grip(float(grip));
    if (schema::Vehicle(vehicle).unknown_2383() != 0 && double(schema::Vehicle(vehicle).brake()) > one) {
      grip = Fmuls(grip, brake_scale);
      schema::VehicleAxle(record).set_grip(float(grip));
    }
    const double kept = Fnmsubs(Lfs(schema::CarDataAxle(data).unknown_180_address() + 4), loss, one);
    schema::VehicleAxle(record).set_grip(float(Fmuls(kept, double(schema::VehicleAxle(record).grip()))));
  }
}

bool VehicleUpdateSteering(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B18B8 Vehicle::UpdateSteering");
  const uint32_t data = CarData(vehicle);
  const double zero = Lfs(kZero);
  const double full = double(schema::CarData(data).max_steering_angle());
  double over = Fsubs(Fabs(double(schema::Vehicle(vehicle).speed())), Lfs(0x820029F0));
  if (over < zero) {
    over = zero;
  }
  double limit = full;
  if (schema::Vehicle(vehicle).speed_sensitive_steering() == 1) {
    const double k = double(schema::CarData(data).steering_328());
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
  const uint32_t body = schema::Vehicle(vehicle).body().address;
  const uint32_t matrix = va::RigidBodyMatrix(body);
  CopyWord(matrix + 16, frame + 80);
  CopyWord(kZero, frame + 92);
  CopyWord(matrix + 24, frame + 88);
  CopyWord(matrix + 20, frame + 84);
  const double yaw = double(vl::Vec3DotOf(frame + 80, va::RigidBodyAngularVelocity(body)));
  const double counter = Fmsubs(double(schema::Vehicle(vehicle).slip_term()), Lfs(0x82600D38),
                                Fmuls(Lfs(0x82600D34), yaw));
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
  const double input = double(schema::Vehicle(vehicle).steering_input());
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
    const double wheelbase = double(schema::CarData(data).wheelbase());
    const double track = double(schema::CarData(data).front_track());
    const double span = Fsubs(Fdivs(wheelbase, Fabs(double(float(t)))), track);
    if (!(span > zero)) {
      inner = Lfs(0x82001FB4);
    } else {
      inner = double(float(c_runtime::Atan(Fdivs(wheelbase, span))));
    }
    if (steer < zero) {
      inner = Fneg(inner);
    }
  }
  CopyWord(schema::Vehicle(vehicle).steering_address(), schema::Vehicle(vehicle).steering_previous_address());
  schema::Vehicle(vehicle).set_steering(float(steer));
  if (positive) {
    schema::Vehicle(vehicle).set_front_right_angle(float(steer));
    schema::Vehicle(vehicle).set_front_left_angle(float(inner));
  } else {
    schema::Vehicle(vehicle).set_front_right_angle(float(inner));
    schema::Vehicle(vehicle).set_front_left_angle(float(steer));
  }
  const double stored = double(schema::Vehicle(vehicle).steering());
  schema::Vehicle(vehicle).set_steering_limit_left(float(left));
  schema::Vehicle(vehicle).set_steering_limit_right(float(right));
  double clamped = Fneg(full);
  if (!(stored < clamped)) {
    clamped = full < stored ? full : stored;
  }
  const double smoothed = Fadds(double(schema::Vehicle(vehicle).steering_smoothed()), clamped);
  const double v384 = Lfs(schema::Vehicle(vehicle).ground_velocity_address());
  schema::Vehicle(vehicle).set_steering_smoothed(float(Fmuls(smoothed, Lfs(0x82001BE4))));
  const double k = Fdivs(v384, double(schema::CarData(CarData(vehicle)).wheelbase()));
  schema::Vehicle(vehicle).set_unknown_3116(float(Fmuls(k, stored)));
  return true;
}

bool VehicleUpdateTorqueSplit(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B2100 Vehicle::UpdateTorqueSplit");
  const uint32_t data = CarData(vehicle);
  const double one = Lfs(kOne);
  const double front = double(schema::CarData(data).front_torque_share());
  const double rear = Fsubs(one, front);
  const double zero = Lfs(kZero);
  const bool split = !(front == zero) && !(rear == zero);
  const uint32_t mode = schema::Vehicle(vehicle).drive_type();
  if (split && mode == 2 && schema::Vehicle(vehicle).gear() > 0) {
    return false;
  }
  CopyWord(schema::CarData(data).front_torque_share_address(), schema::VehicleAxle(schema::Vehicle(vehicle).front_axle_address()).torque_share_address());
  schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).set_torque_share(float(rear));
  if (!split) {
    return true;
  }
  const double half = Lfs(0x82001BE4);
  // The wheels' loads (+280).
  auto load = [&](uint32_t w) { return double(schema::VehicleWheel(Wheel(vehicle, w)).load()); };
  if (mode == 2) {
    // Not in a forward gear (the forward one is the original's).
    schema::VehicleAxle(schema::Vehicle(vehicle).front_axle_address()).set_torque_share(float(double(schema::CarData(data).front_torque_share())));
    schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).set_torque_share(float(Fsubs(one, double(schema::CarData(data).front_torque_share()))));
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
    const double least = double(schema::CarData(data).front_torque_share());
    share = Fsubs(one, Fdivs(f, total));
    if (share < least) {
      share = least;
    } else if (share > half) {
      share = half;
    }
  } else if (mode - 4 <= 1) {
    share = double(schema::CarData(data).front_torque_share());
    if (schema::Vehicle(vehicle).gear() > 0) {
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
  schema::VehicleAxle(schema::Vehicle(vehicle).front_axle_address()).set_torque_share(float(share));
  schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).set_torque_share(float(Fsubs(one, share)));
  return true;
}

void VehicleUpdateTurbo(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B24A8 Vehicle::UpdateTurbo");
  const schema::CarData engine_data(CarData(vehicle));
  const double one = Lfs(kOne);
  const bool turbo = engine_data.turbo() != 0;
  const double max_boost = turbo ? Lfs(engine_data.boost_1620_address()) : one;
  const int32_t gear = schema::Vehicle(vehicle).gear();
  const double boost = double(schema::Vehicle(vehicle).boost());
  const double zero = Lfs(kZero);
  double throttle;
  double cap;
  if (gear == -1) {
    throttle = double(schema::Vehicle(vehicle).reverse_throttle());
    cap = one;
  } else {
    throttle = double(schema::Vehicle(vehicle).throttle());
    if (gear == 0) {
      cap = one;
    } else {
      if (gear != 1 && !(double(schema::Vehicle(vehicle).shift_timer()) == zero)) {
        throttle = zero;
      }
      cap = max_boost;
    }
  }
  const double rpm = double(schema::Vehicle(vehicle).rpm());
  const double k = Lfs(engine_data.boost_1616_address());
  double pressure;
  if (boost > one) {
    pressure = Fmuls(Fmuls(k, rpm), rpm);
  } else {
    pressure = Fmuls(rpm, k);
  }
  pressure = Fmuls(pressure, throttle);
  const double floor = Lfs(0x82002A14);
  const double target = throttle == zero ? floor : Fadds(boost, pressure);
  const double rate = Lfs(0x82002A10);
  double now = Fmadds(Fsubs(target, boost), Lfs(0x82001FBC), boost);
  schema::Vehicle(vehicle).set_boost(float(now));
  if (now > cap) {
    now = Fmadds(Fsubs(cap, now), rate, now);
    schema::Vehicle(vehicle).set_boost(float(now));
  }
  if (!turbo) {
    return;
  }
  schema::Vehicle(vehicle).set_blow_off_open(uint8_t(0));
  if (engine_data.blow_off() != 0) {
    // The blow-off valve.
    double held;
    if (throttle == zero) {
      held = Fsubs(double(schema::Vehicle(vehicle).blow_off()), Lfs(0x820021E4));
    } else {
      held = Fadds(double(schema::Vehicle(vehicle).blow_off()), throttle);
    }
    schema::Vehicle(vehicle).set_blow_off(float(held));
    double clamped;
    if (held < zero) {
      clamped = zero;
    } else {
      clamped = Lfs(0x82002A0C);
      if (!(held > clamped)) {
        clamped = held;
      }
    }
    const double current = double(schema::Vehicle(vehicle).boost());
    schema::Vehicle(vehicle).set_blow_off(float(clamped));
    const double half = Lfs(0x82001BE4);
    if (Fmuls(max_boost, Lfs(0x82001070)) > current &&
        Fmuls(Fsubs(max_boost, one), half) < current && throttle == zero &&
        boost > current &&
        !(double(schema::Vehicle(vehicle).unknown_2220()) < Fmuls(Lfs(engine_data.rev_limit_address()), half))) {
      const double limit = Fmuls(max_boost, half);
      const double factor = Fmadds(clamped, Lfs(0x82002A08), one);
      double grown = current;
      for (int i = 0; i < 2; i++) {
        grown = Fmuls(factor, grown);
        if (grown > limit) {
          schema::Vehicle(vehicle).set_blow_off_open(uint8_t(1));
          break;
        }
      }
      if (schema::Vehicle(vehicle).blow_off_open() != 0) {
        double cut = Fmadds(Fmsubs(factor, current, current), rate, current);
        schema::Vehicle(vehicle).set_boost(float(cut));
        if (cut > cap) {
          cut = Fmadds(Fsubs(cap, cut), rate, cut);
          schema::Vehicle(vehicle).set_boost(float(cut));
        }
      }
    }
  } else {
    // The wastegate.
    uint8_t open = 0;
    bool set = false;
    if (target < one) {
      const double above = Fsubs(boost, one);
      if (above > Fmuls(Fsubs(max_boost, one), Lfs(0x82002A04))) {
        double v = Fmuls(above, Lfs(0x82001BE4));
        if (v < zero) {
          v = zero;
        } else if (v > one) {
          v = one;
        }
        schema::Vehicle(vehicle).set_wastegate(float(v));
        open = 1;
        set = true;
      }
    }
    if (!set) {
      schema::Vehicle(vehicle).set_wastegate(float(zero));
    }
    schema::Vehicle(vehicle).set_wastegate_open(uint8_t(open));
  }
  // The torque factor.
  double factor = Fdivs(Fsubs(max_boost, double(schema::Vehicle(vehicle).boost())), Fsubs(max_boost, floor));
  factor = Fnmsubs(factor, Lfs(engine_data.boost_1612_address()), one);
  schema::Vehicle(vehicle).set_turbo_torque_factor(float(factor));
  if (factor > one) {
    schema::Vehicle(vehicle).set_turbo_torque_factor(float(one));
  }
}

uint32_t VehicleUpdateTireForces(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B3F80 Vehicle::UpdateTireForces");
  const uint32_t f = frame;
  const double one = Lfs(kOne);
  // The driven axles: their base value plus the share of the clutch's
  // torque.
  {
    const double open = Fsubs(one, double(schema::Vehicle(vehicle).clutch()));
    const uint32_t mode = schema::Vehicle(vehicle).drive_type();
    const uint32_t data = CarData(vehicle);
    const double drive = Fmuls(Fmuls(open, double(schema::Vehicle(vehicle).unknown_2320())), Lfs(0x82001BE4));
    if (mode < 1) {
      schema::VehicleAxle(schema::Vehicle(vehicle).front_axle_address()).set_inertia(float(Fmadds(double(schema::VehicleAxle(schema::Vehicle(vehicle).front_axle_address()).torque_share()), drive, double(schema::CarDataAxle(schema::CarData(data).front_axle_address()).wheel_inertia()))));
      schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).set_inertia(float(double(schema::CarDataAxle(schema::CarData(data).rear_axle_address()).wheel_inertia())));
    } else if (mode == 1) {
      CopyWord(schema::CarDataAxle(schema::CarData(data).front_axle_address()).wheel_inertia_address(), schema::VehicleAxle(schema::Vehicle(vehicle).front_axle_address()).inertia_address());
      schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).set_inertia(float(Fmadds(double(schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).torque_share()), drive, double(schema::CarDataAxle(schema::CarData(data).rear_axle_address()).wheel_inertia()))));
    } else {
      schema::VehicleAxle(schema::Vehicle(vehicle).front_axle_address()).set_inertia(float(Fmadds(double(schema::VehicleAxle(schema::Vehicle(vehicle).front_axle_address()).torque_share()), drive, double(schema::CarDataAxle(schema::CarData(data).front_axle_address()).wheel_inertia()))));
      schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).set_inertia(float(Fmadds(double(schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).torque_share()), drive, double(schema::CarDataAxle(schema::CarData(data).rear_axle_address()).wheel_inertia()))));
    }
  }
  const double minus_one = Lfs(0x82001BE0);
  const double least = Lfs(0x82002A54);
  const double zero = Lfs(kZero);
  uint32_t r3 = vehicle;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    const uint32_t data = guest::LoadU32(schema::Vehicle(vehicle).wheel_axle_data_address() + 4 * w);
    const uint32_t body = schema::Vehicle(vehicle).body().address;
    // The rolling speed (+96: x, 0, 0, 0) and the ground's (+80: along
    // +144, 0, along +128, 0).
    const double rolling = Fmuls(double(schema::VehicleWheel(wheel).spin()), double(schema::CarDataAxle(data).radius()));
    Stfs(f + 140, zero);
    Stfs(f + 108, zero);
    Stfs(f + 92, zero);
    Stfs(f + 100, zero);
    Stfs(f + 104, zero);
    Stfs(f + 96, rolling);
    va::RigidBodyPointVelocity(f + 144, body, schema::VehicleWheel(wheel).position_address());
    vl::Vec4Copy(f + 128, f + 144);
    Stfs(f + 80, double(vl::Vec3DotOf(f + 128, schema::VehicleWheel(wheel).forward_axis_address())));
    Stfs(f + 84, zero);
    Stfs(f + 88, double(vl::Vec3DotOf(f + 128, schema::VehicleWheel(wheel).side_axis_address())));
    const double rolling_length = double(vl::Vec3Length(f + 96));
    const double ground_length = double(vl::Vec3Length(f + 80));
    double speed =
        double(vl::Vec3Length(rolling_length < ground_length ? f + 80 : f + 96));
    vl::Vec3Difference(f + 112, f + 96, f + 80);
    double slip = double(vl::Vec3Length(f + 112));
    schema::VehicleWheel(wheel).set_slip_speed(float(slip));
    if (slip == zero) {
      schema::VehicleWheel(wheel).set_lateral_force(float(zero));
      schema::VehicleWheel(wheel).set_slip_long(float(zero));
      schema::VehicleWheel(wheel).set_slip_side(float(zero));
      schema::VehicleWheel(wheel).set_longitudinal_force_2(float(zero));
      schema::VehicleWheel(wheel).set_longitudinal_force(float(zero));
      r3 = f + 112;
      CopyWord(schema::VehicleWheel(wheel).longitudinal_force_address(), schema::VehicleWheel(wheel).longitudinal_force_2_address());
      continue;
    }
    const double low = Fdivs(least, double(schema::Vehicle(vehicle).substep_dt()));
    if (low > slip) {
      slip = low;
    }
    if (low > speed) {
      speed = low;
    }
    const double inverse = Fdivs(one, speed);
    double along = Fmuls(inverse, Lfs(f + 112));
    if (along < minus_one) {
      along = minus_one;
    } else if (along > one) {
      along = one;
    }
    double across = Fmuls(inverse, Lfs(f + 120));
    if (across < minus_one) {
      across = minus_one;
    } else if (across > one) {
      across = one;
    }
    // The slip angle: the rolling direction's against the ground's.
    const double rolling_angle = c_runtime::Atan2(Lfs(f + 104), Lfs(f + 96));
    const double ground_angle = c_runtime::Atan2(Lfs(f + 88), Lfs(f + 80));
    const double ground_along = Lfs(f + 80);
    schema::VehicleWheel(wheel).set_slip_long(float(along));
    schema::VehicleWheel(wheel).set_slip_angle(float(Fsubs(double(float(rolling_angle)), double(float(ground_angle)))));
    if (ground_along < zero) {
      schema::VehicleWheel(wheel).set_slip_long(float(Fmuls(along, minus_one)));
    }
    schema::VehicleWheel(wheel).set_slip_side(float(across));
    const double fx = TireMagicFormula(0x82600D00, along);
    const double per = Fdivs(one, slip);
    double longitudinal = Fmuls(Fmuls(per, Fabs(Lfs(f + 112))), fx);
    const double fy = TireMagicFormula(0x82600D10, across);
    double lateral = Fmuls(Fmuls(per, Fabs(Lfs(f + 120))), fy);
    const double length =
        double(vl::Sqrtf(Fmadds(lateral, lateral, Fmuls(longitudinal, longitudinal))));
    const double scale = Fdivs(one, length);
    if (!(scale > one)) {
      longitudinal = Fmuls(scale, longitudinal);
      lateral = Fmuls(scale, lateral);
    }
    const double load = Fnmsubs(double(schema::VehicleWheel(wheel).load()), zero, one);
    const double limit = double(schema::VehicleWheel(wheel).force_limit());
    double fl = Fmuls(Fmuls(load, limit), longitudinal);
    double fs = Fmuls(Fmuls(load, limit), lateral);
    // Less the surface's drag x the ground speed's square.
    double drag = Fmuls(VehicleSurfaceValue20(vehicle, w), double(schema::VehicleWheel(wheel).force_limit()));
    double g = Lfs(f + 80);
    fl = Fnmsubs(Fmuls(drag, g), g, fl);
    drag = Fmuls(VehicleSurfaceValue20(vehicle, w), double(schema::VehicleWheel(wheel).force_limit()));
    g = Lfs(f + 88);
    schema::VehicleWheel(wheel).set_longitudinal_force(float(fl));
    fs = Fnmsubs(Fmuls(drag, g), g, fs);
    schema::VehicleWheel(wheel).set_lateral_force(float(fs));
    r3 = vehicle;
    CopyWord(schema::VehicleWheel(wheel).longitudinal_force_address(), schema::VehicleWheel(wheel).longitudinal_force_2_address());
  }
  return r3;
}

uint32_t VehicleUpdateAtRest(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B2808 Vehicle::UpdateAtRest");
  const uint32_t f = frame;
  const double zero = Lfs(kZero);
  uint32_t r3 = vehicle;
  if (schema::Vehicle(vehicle).forced_rest() != 0) {
    schema::Vehicle(vehicle).set_at_rest(uint8_t(1));
  } else {
    const uint32_t object = schema::Vehicle(vehicle).collision_object();
    r3 = object;
    if (object == 0) {
      return 0;
    }
    if (!(double(schema::Vehicle(vehicle).speed()) < Lfs(kOne)) || !(double(schema::Vehicle(vehicle).throttle()) == zero) ||
        !(double(schema::Vehicle(vehicle).reverse_throttle()) == zero)) {
      schema::Vehicle(vehicle).set_at_rest(uint8_t(0));
    } else {
      // Resting on a contact: weak enough, and the car's center of mass
      // above it (along +96).
      const uint32_t contact = va::CollisionObjectFirstContact(object, 0x03000000);
      r3 = contact;
      if (contact != 0 && double(va::ContactStrength(contact)) < Lfs(0x82002A20)) {
        const uint32_t m = va::RigidBodyOrientation(schema::Vehicle(vehicle).body().address);
        CopyWord(m + 48, f + 80);
        Stfs(f + 92, zero);
        CopyWord(m + 56, f + 88);
        CopyWord(m + 52, f + 84);
        vl::Vec3Difference(f + 128, va::ContactPosition(contact), f + 80);
        Stfs(f + 132, zero);
        const double up = double(vl::Vec3DotOf(f + 128, schema::Vehicle(vehicle).ground_up_address()));
        r3 = f + 128;
        schema::Vehicle(vehicle).set_at_rest(uint8_t(up < zero ? 0 : 1));
      }
    }
  }
  // Still, with no input (or in neutral), wheels down.
  const double speed = Fabs(double(schema::Vehicle(vehicle).speed()));
  const bool no_input =
      double(schema::Vehicle(vehicle).reverse_throttle()) == zero && double(schema::Vehicle(vehicle).throttle()) == zero;
  if ((no_input || uint32_t(schema::Vehicle(vehicle).gear()) == 0) &&
      schema::Vehicle(vehicle).wheels_on_ground() != 0 && Fabs(speed) < Lfs(0x82002A1C)) {
    schema::Vehicle(vehicle).set_at_rest(uint8_t(1));
  }
  if (schema::Vehicle(vehicle).at_rest() == 0) {
    return r3;
  }
  const uint32_t gear = uint32_t(schema::Vehicle(vehicle).gear());
  schema::Vehicle(vehicle).set_gear(int32_t(0));
  schema::Vehicle(vehicle).set_gear_previous(int32_t(gear));
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    schema::VehicleWheel(wheel).set_spin(float(zero));
    schema::VehicleWheel(wheel).set_spin_momentum(float(zero));
    schema::VehicleWheel(wheel).set_longitudinal_force(float(zero));
    schema::VehicleWheel(wheel).set_lateral_force(float(zero));
  }
  const uint32_t body = schema::Vehicle(vehicle).body().address;
  // The velocity's y kept (its 16 bytes copied, x and z zeroed).
  {
    const uint32_t from = va::RigidBodyVelocity(body);
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(f + 128 + i, guest::LoadU32(from + i));
    }
    Stfs(f + 128, zero);
    Stfs(f + 136, zero);
    RigidBodySetVelocity(body, f + 128, f - 112);
  }
  // The angular momentum +400's x, y and z zeroed (sub_820B6940 and
  // sub_820B6938, never seen in play: +400, and Vec4::Copy into it).
  {
    const uint32_t from = schema::RigidBody(body).angular_momentum_address();
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(f + 96 + i, guest::LoadU32(from + i));
    }
    Stfs(f + 96, zero);
    Stfs(f + 100, zero);
    Stfs(f + 104, zero);
    vl::Vec4Copy(schema::RigidBody(body).angular_momentum_address(), f + 96);
  }
  {
    const uint32_t from = va::RigidBodyAngularVelocity(body);
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(f + 112 + i, guest::LoadU32(from + i));
    }
    Stfs(f + 112, zero);
    Stfs(f + 116, zero);
    Stfs(f + 120, zero);
    RigidBodySetAngularVelocity(body, f + 112, f - 160);
  }
  return schema::RigidBody(body).angular_velocity_address();
}

uint32_t VehicleUpdateAero(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B2A48 Vehicle::UpdateAero");
  const uint32_t f = frame;
  const uint32_t data = CarData(vehicle);
  const double zero = Lfs(kZero);
  // The drag, against the motion along +112 (forward) and +128 (side).
  const double forward = Lfs(schema::Vehicle(vehicle).ground_velocity_address());
  const double forward2 = Fmuls(forward, forward);
  const double k = Fmuls(double(schema::CarData(data).unknown_1808()), double(schema::Vehicle(vehicle).drag_scale()));
  const double scale = Lfs(0x82002A14);
  const double minus_one = Lfs(0x82001BE0);
  double drag = Fmuls(Fmuls(double(schema::CarData(data).aero_front()), forward2), k);
  drag = Fmuls(drag, scale);
  if (forward > zero) {
    drag = Fmuls(drag, minus_one);
  }
  const double side = Lfs(schema::Vehicle(vehicle).ground_velocity_address() + 8);
  double side_drag = Fmuls(Fmuls(Fmuls(side, side), double(schema::CarData(data).aero_side())), k);
  side_drag = Fmuls(side_drag, scale);
  if (side > zero) {
    side_drag = Fmuls(side_drag, minus_one);
  }
  vl::Vec4Scaled(f + 112, schema::Vehicle(vehicle).ground_side_address(), side_drag);
  vl::Vec4Scaled(f + 176, schema::Vehicle(vehicle).ground_forward_address(), drag);
  vl::Vec3Sum(f + 160, f + 176, f + 112);
  vl::Vec3AddTo(schema::Vehicle(vehicle).body_accumulator_address(), f + 160);
  uint32_t r3 = schema::Vehicle(vehicle).body_accumulator_address();
  if (Lfs(schema::Vehicle(vehicle).ground_velocity_address()) > zero) {
    // The downforce: -(+96) x v^2 x 0.5, x car data +1812 into the front
    // wheels' accumulators, +1816 into the rear's.
    const double down_y = Fneg(Lfs(schema::Vehicle(vehicle).ground_up_address() + 4));
    Stfs(f + 108, zero);
    Stfs(f + 100, down_y);
    const double half_v2 = Fmuls(forward2, Lfs(0x82001BE4));
    Stfs(f + 96, Fneg(Lfs(schema::Vehicle(vehicle).ground_up_address())));
    Stfs(f + 104, Fneg(Lfs(schema::Vehicle(vehicle).ground_up_address() + 8)));
    vl::Vec4Scaled(f + 112, f + 96, half_v2);
    vl::Vec4Scaled(f + 128, f + 112, double(schema::CarData(CarData(vehicle)).unknown_1812()));
    vl::Vec4Scaled(f + 144, f + 112, double(schema::CarData(CarData(vehicle)).unknown_1816()));
    vl::Vec3AddTo(schema::Vehicle(vehicle).wheel_accumulators_address() + 16, f + 128);
    vl::Vec3AddTo(schema::Vehicle(vehicle).wheel_accumulators_address(), f + 128);
    vl::Vec3AddTo(schema::Vehicle(vehicle).wheel_accumulators_address() + 48, f + 144);
    vl::Vec3AddTo(schema::Vehicle(vehicle).wheel_accumulators_address() + 32, f + 144);
    r3 = schema::Vehicle(vehicle).wheel_accumulators_address() + 32;
  }
  // The engine's shake.
  const double rpm = double(schema::Vehicle(vehicle).rpm());
  const uint32_t engine_data = CarData(vehicle);
  Stfs(f + 92, zero);
  const double torque = VehicleEngineTorque(vehicle, rpm, f);
  r3 = vehicle;
  const double peak = double(schema::CarData(engine_data).engine_braking());
  const double per = Fdivs(torque, double(schema::CarData(engine_data).torque_copy()));
  const double c = Lfs(0x820021F4);
  const double one = Lfs(kOne);
  double amount = Fmadds(Fsubs(one, peak), c, Lfs(0x82001FBC));
  const double phase = Fmadds(rpm, c, double(schema::Vehicle(vehicle).engine_shake()));
  schema::Vehicle(vehicle).set_engine_shake(float(phase));
  amount = Fmuls(amount, per);
  amount = Fmuls(amount, Lfs(0x82002A28));
  if (phase < one) {
    return r3;
  }
  const int32_t count = schema::Vehicle(vehicle).engine_shake_count();
  Stfs(f + 80, zero);
  Stfs(f + 88, zero);
  Stfs(f + 92, one);
  const int32_t step = count % 4;
  if (step == 0 || step == 2) {
    const double push = step == 0 ? Fmuls(amount, Lfs(0x82002A24)) : Fmuls(amount, c);
    Stfs(f + 84, push);
    vl::Vec3AddTo(schema::Vehicle(vehicle).wheel_accumulators_address(), f + 80);
    vl::Vec3AddTo(schema::Vehicle(vehicle).wheel_accumulators_address() + 16, f + 80);
    r3 = schema::Vehicle(vehicle).wheel_accumulators_address() + 16;
  }
  const int32_t next = schema::Vehicle(vehicle).engine_shake_count();
  schema::Vehicle(vehicle).set_engine_shake(float(zero));
  schema::Vehicle(vehicle).set_engine_shake_count(next <= 4 ? next + 1 : 0);
  return r3;
}

namespace {

// UpdateWheelAxes' axes for one wheel: with s, c = SinCos(angle) at
// `frame` + 80 and + 84, +144 = +112 c - +128 s and +128 = +112 s + +128 c
// (the vehicle's axes; the temporaries at `frame` + 96 to + 143, in the
// original's order, wheel 0's first sum at + 128 and the others' at + 96).
void TurnedAxes(uint32_t vehicle, uint32_t wheel, double angle, uint32_t frame,
                bool wheel0) {
  const uint32_t f = frame;
  mm::SinCos(angle, f + 80, f + 84);
  const double s = Lfs(f + 80);
  const double c = Lfs(f + 84);
  if (wheel0) {
    vl::Vec4Scaled(f + 96, schema::Vehicle(vehicle).ground_side_address(), Fneg(s));
    vl::Vec4Scaled(f + 112, schema::Vehicle(vehicle).ground_forward_address(), c);
    vl::Vec3Sum(f + 128, f + 112, f + 96);
    vl::Vec4Copy(schema::VehicleWheel(wheel).forward_axis_address(), f + 128);
  } else {
    vl::Vec4Scaled(f + 128, schema::Vehicle(vehicle).ground_side_address(), Fneg(s));
    vl::Vec4Scaled(f + 112, schema::Vehicle(vehicle).ground_forward_address(), c);
    vl::Vec3Sum(f + 96, f + 112, f + 128);
    vl::Vec4Copy(schema::VehicleWheel(wheel).forward_axis_address(), f + 96);
  }
  vl::Vec4Scaled(f + 128, schema::Vehicle(vehicle).ground_side_address(), c);
  vl::Vec4Scaled(f + 112, schema::Vehicle(vehicle).ground_forward_address(), s);
  vl::Vec3Sum(f + 96, f + 112, f + 128);
  vl::Vec4Copy(schema::VehicleWheel(wheel).side_axis_address(), f + 96);
}

}  // namespace

uint32_t VehicleUpdateWheelAxes(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B1B00 Vehicle::UpdateWheelAxes");
  const uint32_t f = frame;
  const uint32_t w0 = Wheel(vehicle, 0), w1 = Wheel(vehicle, 1);
  const uint32_t w2 = Wheel(vehicle, 2), w3 = Wheel(vehicle, 3);
  const double k_f8 = Lfs(0x820029F8);
  const double k_fc = Lfs(0x820029FC);
  const double cfbc = Lfs(0x82001FBC);
  // The front: the toe from the travel's angle (atan2 of +0 against the
  // lateral offset +24), car data +548, the steering +308 and +312.
  {
    const uint32_t data = CarData(vehicle);
    const double steer1 = double(schema::Vehicle(vehicle).front_right_angle());
    const double steer0 = double(schema::Vehicle(vehicle).front_left_angle());
    const double toe = Lfs(schema::CarDataAxle(schema::CarData(data).front_axle_address()).angles_address() + 8);
    const double a0 = double(float(c_runtime::Atan2(double(schema::VehicleWheel(w0).travel()), double(schema::VehicleWheel(w0).lateral_offset()))));
    const double a1 = double(float(c_runtime::Atan2(double(schema::VehicleWheel(w1).travel()), Fneg(double(schema::VehicleWheel(w1).lateral_offset())))));
    double t0 = Fmadds(double(schema::VehicleWheel(w0).force_integral_2()), k_fc, Fmuls(a0, k_f8));
    t0 = Fadds(t0, toe);
    schema::VehicleWheel(w0).set_toe(float(t0));
    const double angle0 = Fadds(t0, steer0);
    double t1 = Fmadds(double(schema::VehicleWheel(w1).force_integral_2()), k_fc, Fmuls(a1, cfbc));
    t1 = Fsubs(t1, toe);
    schema::VehicleWheel(w1).set_toe(float(t1));
    TurnedAxes(vehicle, w0, angle0, f, true);
    TurnedAxes(vehicle, w1, Fadds(double(schema::VehicleWheel(w1).toe()), steer1), f, false);
  }
  // The rear: no steering (+304 x 0.0), and the vehicle's axes into +2080
  // and +2064.
  {
    const double rear_steer = Fmuls(double(schema::Vehicle(vehicle).steering()), Lfs(kZero));
    vl::Vec4Copy(schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).forward_axis_address(), schema::Vehicle(vehicle).ground_forward_address());
    vl::Vec4Copy(schema::VehicleAxle(schema::Vehicle(vehicle).rear_axle_address()).side_axis_address(), schema::Vehicle(vehicle).ground_side_address());
    const uint32_t data = CarData(vehicle);
    const double toe = Lfs(schema::CarDataAxle(schema::CarData(data).rear_axle_address()).angles_address() + 8);
    const double a2 = double(float(c_runtime::Atan2(double(schema::VehicleWheel(w2).travel()), double(schema::VehicleWheel(w2).lateral_offset()))));
    const double a3 = double(float(c_runtime::Atan2(double(schema::VehicleWheel(w3).travel()), Fneg(double(schema::VehicleWheel(w3).lateral_offset())))));
    const double k_f4 = Lfs(0x820029F4);
    const double m2 = Fmuls(double(schema::VehicleWheel(w2).force_integral_2()), k_f4);
    const double m3 = Fmuls(double(schema::VehicleWheel(w3).force_integral_2()), k_f4);
    double t2 = Fmsubs(a2, cfbc, m2);
    t2 = Fadds(t2, toe);
    schema::VehicleWheel(w2).set_toe(float(t2));
    const double angle2 = Fadds(t2, rear_steer);
    double t3 = Fmsubs(a3, k_f8, m3);
    t3 = Fsubs(t3, toe);
    schema::VehicleWheel(w3).set_toe(float(t3));
    TurnedAxes(vehicle, w2, angle2, f, false);
    TurnedAxes(vehicle, w3, Fadds(rear_steer, double(schema::VehicleWheel(w3).toe())), f, false);
  }
  // Per wheel: the contact's velocity and speeds, the camber and the
  // wheel's quaternion (about the x axis at 0x82638270).
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    CopyWord(schema::VehicleWheel(wheel).forward_speed_address(), schema::VehicleWheel(wheel).forward_speed_previous_address());
    const uint32_t data = guest::LoadU32(schema::Vehicle(vehicle).wheel_axle_data_address() + 4 * w);
    va::RigidBodyPointVelocity(f + 128, schema::Vehicle(vehicle).body().address, schema::VehicleWheel(wheel).contact_point_address());
    vl::Vec4Copy(schema::VehicleWheel(wheel).contact_velocity_address(), f + 128);
    schema::VehicleWheel(wheel).set_forward_speed(float(double(vl::Vec3DotOf(schema::VehicleWheel(wheel).contact_velocity_address(), schema::VehicleWheel(wheel).forward_axis_address()))));
    schema::VehicleWheel(wheel).set_side_speed(float(double(vl::Vec3DotOf(schema::VehicleWheel(wheel).contact_velocity_address(), schema::VehicleWheel(wheel).side_axis_address()))));
    double turn;
    if (w == 0 || w == 2) {
      const double a = double(float(c_runtime::Atan2(double(schema::VehicleWheel(wheel).travel()), double(schema::VehicleWheel(wheel).lateral_offset()))));
      schema::VehicleWheel(wheel).set_camber(float(Fnmsubs(a, double(schema::CarDataAxle(data).unknown_48()), Lfs(schema::CarDataAxle(data).angles_address()))));
      const double b = double(float(c_runtime::Atan2(double(schema::VehicleWheel(wheel).travel()), double(schema::VehicleWheel(wheel).lateral_offset()))));
      turn = Fnmsubs(b, double(schema::CarDataAxle(data).unknown_48()), Lfs(schema::CarDataAxle(data).angles_address() + 4));
    } else {
      const double a =
          double(float(c_runtime::Atan2(double(schema::VehicleWheel(wheel).travel()), Fneg(double(schema::VehicleWheel(wheel).lateral_offset())))));
      schema::VehicleWheel(wheel).set_camber(float(Fmsubs(a, double(schema::CarDataAxle(data).unknown_48()), Lfs(schema::CarDataAxle(data).angles_address()))));
      const double b =
          double(float(c_runtime::Atan2(double(schema::VehicleWheel(wheel).travel()), Fneg(double(schema::VehicleWheel(wheel).lateral_offset())))));
      turn = Fmsubs(b, double(schema::CarDataAxle(data).unknown_48()), Lfs(schema::CarDataAxle(data).angles_address() + 4));
    }
    mm::QuaternionFromAxisAngle(schema::VehicleWheel(wheel).quaternion_address(), 0x82638270, turn, f - 32);
  }
  return schema::VehicleWheel(w3).quaternion_address();
}

uint32_t VehicleApplyForces(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B2CC0 Vehicle::ApplyForces");
  const uint32_t v = vehicle, f = frame;
  // The speed squared.
  const double speed = double(schema::Vehicle(v).speed());
  schema::Vehicle(v).set_speed_squared(float(Fmuls(speed, speed)));
  // The center of mass: the car data's +96 with wheels down, +112 in the
  // air, set when byte +3132 changes.
  const bool down = schema::Vehicle(v).wheels_on_ground() != 0;
  const uint8_t airborne = schema::Vehicle(v).upright();
  if (down ? airborne != 0 : airborne == 0) {
    schema::Vehicle(v).set_upright(uint8_t(down ? 0 : 1));
    const uint32_t offset = schema::Vehicle(v).car_data().address + (down ? 96 : 112);
    va::RigidBodySetCenterOfMass(schema::Vehicle(v).body().address, offset);
  }
  const uint32_t sum = schema::Vehicle(v).wheel_forces_ground_address();
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyWord(kZero, f + 80 + i);
  }
  vl::Vec4Copy(sum, f + 80);
  // Two dot products the original drops.
  vl::Vec3DotOf(v, schema::Vehicle(v).ground_forward_address());
  vl::Vec3DotOf(schema::Vehicle(v).body_row2_address(), schema::Vehicle(v).ground_side_address());
  const double tip_limit_in_air = Lfs(kTipInAir);
  const double tip_limit_down = Lfs(kTipDown);
  const double tip_limit_upright = Lfs(kTipUpright);
  const uint32_t up = schema::Vehicle(v).body_row1_address();
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t accumulator = schema::Vehicle(v).wheel_accumulators_address() + 16 * w;
    // The wheel's force in the ground frame, summed, and given to the body
    // at its contact point (the wheel's +160, 16 bytes copied).
    mm::InverseRotate(f + 112, schema::Vehicle(v).ground_matrix_address(), accumulator);
    vl::Vec3AddTo(sum, f + 112);
    const uint32_t point = schema::VehicleWheel(Wheel(v, w)).contact_point_address();
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(f + 128 + i, guest::LoadU32(point + i));
    }
    va::RigidBodyAddForceAt(schema::Vehicle(v).body().address, accumulator, f + 128);
    // Tipped too far: the angular velocity set to up x +96, or zero.
    uint32_t spin = 0;
    if (schema::Vehicle(v).wheels_on_ground() != 0) {
      schema::Vehicle(v).set_tipped(uint8_t(0));
      if (!(double(vl::Vec3DotOf(up, kUp)) < tip_limit_down)) {
        continue;
      }
      vl::Vec3Cross(f + 160, up, schema::Vehicle(v).ground_up_address());
      spin = f + 160;
    } else if (schema::Vehicle(v).tipped() == 0) {
      if (!(double(vl::Vec3DotOf(up, kUp)) < tip_limit_in_air)) {
        schema::Vehicle(v).set_tipped(uint8_t(1));
        continue;
      }
      vl::Vec3Cross(f + 176, up, schema::Vehicle(v).ground_up_address());
      spin = f + 176;
    } else {
      const double tilt = double(vl::Vec3DotOf(up, kUp));
      if (!(tilt < tip_limit_upright)) {
        continue;
      }
      for (uint32_t i = 0; i < 16; i += 4) {
        guest::StoreU32(f + 144 + i, guest::LoadU32(kZeroVector + i));
      }
      const uint32_t angular = va::RigidBodyAngularVelocity(schema::Vehicle(v).body().address);
      vl::Vec3Cross(f + 192, angular, up);
      if (!(tilt > double(vl::Vec3DotOf(f + 192, kUp)))) {
        continue;
      }
      spin = f + 144;
    }
    RigidBodySetAngularVelocity(schema::Vehicle(v).body().address, spin, f - 160);
  }
  // +80, +84 from +2976, +2980 by +376 and the summed force's x.
  const double a = Fmuls(double(schema::Vehicle(v).gauge_2()), Lfs(schema::Vehicle(v).wheel_forces_ground_address() + 8));
  const double b = Fmuls(double(schema::Vehicle(v).gauge_3()), Lfs(sum));
  schema::Vehicle(v).set_unknown_80(float(a));
  schema::Vehicle(v).set_unknown_84(float(b));
  // The body's force before (+2464), then +2608 added; at rest the
  // horizontal part cancelled.
  const uint32_t body = schema::Vehicle(v).body().address;
  vl::Vec4Copy(schema::Vehicle(v).body_force_address(), va::RigidBodyForce(body));
  va::RigidBodyAddForce(schema::Vehicle(v).body().address, schema::Vehicle(v).body_accumulator_address());
  const uint32_t force = va::RigidBodyForce(schema::Vehicle(v).body().address);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(f + 112 + i, guest::LoadU32(force + i));
  }
  if (schema::Vehicle(v).at_rest() != 0) {
    // fneg between an lfs and an stfs: the sign bit flipped.
    guest::StoreU32(f + 96, guest::LoadU32(f + 112) ^ 0x80000000u);
    const uint32_t z = guest::LoadU32(f + 120) ^ 0x80000000u;
    CopyWord(kZero, f + 100);
    guest::StoreU32(f + 104, z);
    CopyWord(kZero, f + 108);
    va::RigidBodyAddForce(schema::Vehicle(v).body().address, f + 96);
    CopyWord(kZero, schema::Vehicle(v).body_force_address());
    CopyWord(kZero, schema::Vehicle(v).body_force_address() + 8);
  }
  vl::Vec4Copy(schema::Vehicle(v).body_torque_address(), va::RigidBodyTorque(schema::Vehicle(v).body().address));
  const uint32_t r3 = RigidBodyIntegrate(schema::Vehicle(v).body().address,
                                         double(schema::Vehicle(v).dt()), f - 208);
  // The body's word +516: bit 0 accelerating (throttle +284 above 0.5 in a
  // forward gear, +288 in reverse), bit 1 wheels on the ground.
  const int32_t gear = schema::Vehicle(v).gear();
  const double half = Lfs(kHalfConstant);
  const bool accelerating = (gear > 0 && double(schema::Vehicle(v).throttle()) > half) ||
                            (gear < 0 && double(schema::Vehicle(v).reverse_throttle()) > half);
  schema::RigidBody flags_body = schema::Vehicle(v).body();
  uint32_t bits = flags_body.flags();
  flags_body.set_flags(accelerating ? bits | 1 : bits & ~1u);
  flags_body = schema::Vehicle(v).body();
  bits = flags_body.flags();
  flags_body.set_flags(schema::Vehicle(v).wheels_on_ground() != 0 ? bits | 2 : bits & ~2u);
  return r3;
}

}  // namespace car_physics
