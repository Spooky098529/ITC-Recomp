// The vehicle's physics step's helpers (docs/functions/car-physics.md): see
// car_physics.h. Wheel w's 368-byte record is at vehicle + 416 + 368 w.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "schema/car_physics.h"
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

uint32_t CarData(uint32_t vehicle) {
  return schema::Vehicle(vehicle).car_data().address;
}

// The surface record of wheel w (its kind the word +176): [vehicle + 4
// (kind + 667)] for the front wheels, + 699 for the rear (32-bit
// arithmetic, as the original); 0 when byte +2664 is clear (none looked up).
uint32_t SurfaceRecord(uint32_t vehicle, uint32_t w, bool* looked_up) {
  *looked_up = schema::Vehicle(vehicle).surfaces_on() != 0;
  if (!*looked_up) {
    return 0;
  }
  const uint32_t kind = schema::VehicleWheel(Wheel(vehicle, w)).surface();
  const uint32_t index = kind + (w == 0 || w == 1 ? 667 : 699);
  return guest::LoadU32(vehicle + (index << 2));
}

// The surface getters' shared steps: f1 = the default, then the record's
// float at `field` when there is one; its word stored at wheel + `out` when
// `out` (an lfs then stfs: its bits).
double SurfaceValue(uint32_t vehicle, uint32_t w, uint32_t fallback,
                    uint32_t field, uint32_t out) {
  uint32_t from = fallback;
  bool looked_up = false;
  const uint32_t record = SurfaceRecord(vehicle, w, &looked_up);
  if (looked_up && record != 0) {
    from = record + field;
  }
  if (out) {
    CopyWord(from, Wheel(vehicle, w) + out);
  }
  return Lfs(from);
}

}  // namespace

uint32_t VehicleBeginStep(uint32_t vehicle, double dt) {
  ITC_NATIVE_RUNNING("sub_820AFAE8 Vehicle::BeginStep");
  const double rate = Fdivs(Lfs(kOne), dt);
  schema::Vehicle(vehicle).set_dt(float(dt));
  schema::Vehicle(vehicle).set_inverse_dt(float(rate));
  for (uint32_t i = 0; i < 5; i++) {
    vl::Vec4Copy(schema::Vehicle(vehicle).wheel_accumulators_address() + 16 * i, kZeroVector);
  }
  CopyWord(kZero, schema::Vehicle(vehicle).unknown_2448_address());
  CopyWord(kZero, schema::Vehicle(vehicle).unknown_2452_address());
  for (uint32_t w = 0; w < 4; w++) {
    schema::VehicleAxle(guest::LoadU32(schema::Vehicle(vehicle).wheel_axles_address() + 4 * w)).set_locked(uint32_t(0));
  }
  schema::Vehicle(vehicle).set_step_count(uint32_t(schema::Vehicle(vehicle).step_count() + 1));
  return schema::Vehicle(vehicle).wheel_accumulators_address() + 64;
}

void VehicleWheelLoads(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820AFD30 Vehicle::WheelLoads");
  if (schema::Vehicle(vehicle).wheels_on_ground() == 0) {
    for (uint32_t w = 0; w < 4; w++) {
      CopyWord(kZero, schema::VehicleWheel(Wheel(vehicle, w)).load_address());
    }
    return;
  }
  const double zero = Lfs(kZero);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (schema::VehicleWheel(wheel).on_ground() == 0) {
      schema::VehicleWheel(wheel).set_load(float(zero));
      continue;
    }
    // max(+8, 0.0), a NaN kept too (bge: not less than; the original's
    // second check never stores). Through the double, as the translated
    // code (a signaling NaN comes out quiet).
    const double load = double(schema::VehicleWheel(wheel).suspension_force());
    if (!(load < zero)) {
      schema::VehicleWheel(wheel).set_load(float(load));
    } else {
      schema::VehicleWheel(wheel).set_load(float(zero));
    }
  }
}

double VehicleSurfaceValue(uint32_t vehicle, uint32_t w) {
  ITC_NATIVE_RUNNING("sub_820B0508 Vehicle::SurfaceValue");
  return SurfaceValue(vehicle, w, kZero, 0, 0);
}

double VehicleSurfaceValue20(uint32_t vehicle, uint32_t w) {
  ITC_NATIVE_RUNNING("sub_820B0560 Vehicle::SurfaceValue20");
  return SurfaceValue(vehicle, w, kZero, 20, 196);
}

double VehicleSurfaceValue12(uint32_t vehicle, uint32_t w) {
  ITC_NATIVE_RUNNING("sub_820B05B8 Vehicle::SurfaceValue12");
  return SurfaceValue(vehicle, w, kOne, 12, 184);
}

double VehicleSurfaceValue16(uint32_t vehicle, uint32_t w) {
  ITC_NATIVE_RUNNING("sub_820B0610 Vehicle::SurfaceValue16");
  return SurfaceValue(vehicle, w, kOne, 16, 188);
}

uint32_t VehicleAccumulateTireForces(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B0908 Vehicle::AccumulateTireForces");
  const uint32_t f = frame;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    const uint32_t accumulator = schema::Vehicle(vehicle).wheel_accumulators_address() + 16 * w;
    {
      const double dt = double(schema::Vehicle(vehicle).dt());
      const double time = double(schema::Vehicle(vehicle).substep_dt());
      const double k = Fdivs(time, dt);
      const double v284 = double(schema::VehicleWheel(wheel).force_integral());
      const double v308 = double(schema::VehicleWheel(wheel).longitudinal_force());
      const double v288 = double(schema::VehicleWheel(wheel).force_integral_2());
      const double v304 = double(schema::VehicleWheel(wheel).lateral_force());
      schema::VehicleWheel(wheel).set_force_integral(float(Fmadds(k, v308, v284)));
      const double time2 = double(schema::Vehicle(vehicle).substep_dt());
      const double dt2 = double(schema::Vehicle(vehicle).dt());
      const double k2 = Fdivs(time2, dt2);
      schema::VehicleWheel(wheel).set_force_integral_2(float(Fmadds(k2, v304, v288)));
      // (+128 x +304 + +144 x +308) x k into the accumulator.
      vl::Vec4Scaled(f + 96, schema::VehicleWheel(wheel).side_axis_address(), v304);
    }
    vl::Vec4Scaled(f + 112, schema::VehicleWheel(wheel).forward_axis_address(), double(schema::VehicleWheel(wheel).longitudinal_force()));
    vl::Vec3Sum(f + 128, f + 112, f + 96);
    const double k = Fdivs(double(schema::Vehicle(vehicle).substep_dt()), double(schema::Vehicle(vehicle).dt()));
    vl::Vec4Scaled(f + 80, f + 128, k);
    vl::Vec3AddTo(accumulator, f + 80);
  }
  return schema::Vehicle(vehicle).wheel_accumulators_address() + 48;
}

void VehicleRollingResistance(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B2748 Vehicle::RollingResistance");
  if (schema::Vehicle(vehicle).wheels_on_ground() == 0) {
    return;
  }
  const double zero = Lfs(kZero);
  const double half = Lfs(0x82001BE4);
  const double scale = Lfs(0x82002A18);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (schema::VehicleWheel(wheel).on_ground() == 0) {
      continue;
    }
    const uint32_t spin_at = schema::VehicleWheel(wheel).spin_address();
    const double load = double(schema::VehicleWheel(wheel).load());
    const uint32_t data = guest::LoadU32(schema::Vehicle(vehicle).wheel_axle_data_address() + 4 * w);
    double drag = Fmuls(load, scale);
    const uint32_t record = guest::LoadU32(schema::Vehicle(vehicle).wheel_axles_address() + 4 * w);
    double time = double(schema::Vehicle(vehicle).substep_dt());
    const double spin = Lfs(spin_at);
    const double radius = double(schema::CarDataAxle(data).radius());
    double inertia = double(schema::VehicleAxle(record).inertia());
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
      Stfs(spin_at, moved);
      if (moved < zero) {
        Stfs(spin_at, zero);
      }
    } else {
      const double moved = Fadds(spin, drag);
      Stfs(spin_at, moved);
      if (moved > zero) {
        Stfs(spin_at, zero);
      }
    }
  }
}

void VehicleTurnWheels(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AFDA8 Vehicle::TurnWheels");
  const double wrap_low = Lfs(0x820029C4);
  const double wrap_high = Lfs(0x82002240);
  const double turn = Lfs(0x82001FA8);
  const double per_turn = Lfs(0x82001FA4);
  const double low_slope = Lfs(0x820029C0);
  const double low_limit = Lfs(0x820029BC);
  const double high_slope = Lfs(0x820029B8);
  const double high_limit = Lfs(0x820029B4);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t angle_at = schema::VehicleWheel(Wheel(vehicle, w)).angle_address();
    double d = Fmuls(Lfs(angle_at + 220), double(schema::Vehicle(vehicle).dt()));
    d = Fneg(d);
    // The step reshaped past the limits.
    if (d > high_limit) {
      d = Fnmsubs(d, high_slope, high_limit);
    } else if (d < low_limit) {
      d = Fmsubs(d, low_slope, high_limit);
    }
    double angle = Fadds(Lfs(angle_at), d);
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
    guest::StoreU32(frame - 32, uint32_t(whole));
    guest::StoreU32(frame - 24, whole < 0 ? 0xFFFFFFFFu : 0u);
    guest::StoreU32(frame - 20, uint32_t(whole));
    const double turns = double(float(double(whole)));
    angle = Fnmsubs(turns, turn, angle);
    Stfs(angle_at, angle);
    if (angle > wrap_high) {
      Stfs(angle_at, Fsubs(angle, turn));
    } else if (angle < wrap_low) {
      Stfs(angle_at, Fadds(angle, turn));
    }
  }
}

void VehicleBrakeTorques(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B0838 Vehicle::BrakeTorques");
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    CopyWord(kZero, schema::VehicleWheel(wheel).brake_force_address());
    CopyWord(kZero, schema::VehicleWheel(wheel).brake_share_address());
    CopyWord(kZero, schema::VehicleWheel(wheel).brake_torque_address());
  }
  const double handbrake = double(schema::Vehicle(vehicle).handbrake());
  const double brake = double(schema::Vehicle(vehicle).brake());
  if (Fadds(brake, handbrake) == Lfs(kZero)) {
    return;
  }
  const uint32_t data = CarData(vehicle);
  double front = Fmuls(double(schema::CarDataAxle(schema::CarData(data).front_axle_address()).unknown_128()), double(schema::CarData(data).front_brake()));
  const double rear_scale = double(schema::CarDataAxle(schema::CarData(data).rear_axle_address()).unknown_128());
  double rear_handbrake = Fmuls(double(schema::CarData(data).unknown_272()), rear_scale);
  const double rear = Fmuls(double(schema::CarData(data).rear_brake()), rear_scale);
  rear_handbrake = Fmuls(rear_handbrake, handbrake);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    const double x = double(schema::Vehicle(vehicle).brake_efficiency());
    double torque;
    if (w >= 2) {
      torque = Fmuls(double(schema::VehicleWheel(wheel).brake()), x);
      const double braking = Fmuls(torque, rear);
      torque = Fadds(braking, rear_handbrake);
      schema::VehicleWheel(wheel).set_brake_share(float(Fdivs(braking, torque)));
    } else {
      torque = Fmuls(double(schema::VehicleWheel(wheel).brake()), x);
      CopyWord(kOne, schema::VehicleWheel(wheel).brake_share_address());
      torque = Fmuls(torque, front);
    }
    schema::VehicleWheel(wheel).set_brake_torque(float(torque));
  }
}

void VehicleAntiLock(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B0718 Vehicle::AntiLock");
  schema::Vehicle(vehicle).set_abs_active(uint8_t(0));
  for (uint32_t w = 0; w < 4; w++) {
    CopyWord(schema::Vehicle(vehicle).brake_address(), schema::VehicleWheel(Wheel(vehicle, w)).brake_address());
  }
  if (schema::Vehicle(vehicle).abs() == 0) {
    return;
  }
  const double kmh = Fmuls(double(schema::Vehicle(vehicle).speed()), Lfs(0x820029E0));
  if (kmh < Lfs(0x82001FD8)) {
    return;
  }
  const double zero = Lfs(kZero);
  if (!(double(schema::Vehicle(vehicle).brake()) > zero)) {
    return;
  }
  const double one = Lfs(kOne);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (schema::VehicleWheel(wheel).on_ground() == 0) {
      continue;
    }
    const double grip = Lfs(vehicle + (w == 0 || w == 1 ? 2000 : 2128));
    const double limit = Fsubs(one, grip);
    const double slip = double(schema::VehicleWheel(wheel).slip_long());
    bool release = false;
    if (slip < zero) {
      const double slip2 = Fmuls(slip, slip);
      const double other = double(schema::VehicleWheel(wheel).slip_side());
      const double length = double(vl::Sqrtf(Fmadds(other, other, slip2)));
      release = length > limit;
    }
    if (release) {
      // The brake released on this wheel, counted.
      schema::VehicleWheel(wheel).set_brake(float(zero));
      schema::VehicleWheel(wheel).set_abs_count(uint32_t(schema::VehicleWheel(wheel).abs_count() + 1));
    } else {
      CopyWord(schema::Vehicle(vehicle).brake_address(), schema::VehicleWheel(wheel).brake_address());
    }
  }
}

uint32_t VehicleSuspensionTorques(uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AFBD8 Vehicle::SuspensionTorques");
  const uint32_t f = frame;
  const double zero = Lfs(kZero);
  uint32_t r3 = vehicle;
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = Wheel(vehicle, w);
    if (schema::VehicleWheel(wheel).on_ground() == 0) {
      continue;
    }
    double v = double(schema::VehicleWheel(wheel).force_integral());
    if (w >= 2) {
      const uint32_t data = CarData(vehicle);
      const double k = Lfs(data + (!(v < zero) ? 1828 : 1824));
      v = Fmuls(k, v);
    } else {
      const double along = double(vl::Vec3DotOf(schema::Vehicle(vehicle).ground_forward_address(), schema::VehicleWheel(wheel).forward_axis_address()));
      const double t = Fmuls(along, v);
      const uint32_t data = CarData(vehicle);
      v = Fneg(Fmuls(t, Lfs(schema::CarData(data).load_transfer_address())));
    }
    vl::Vec4Scaled(f + 96, schema::Vehicle(vehicle).ground_up_address(), v);
    vl::Vec3AddTo(schema::Vehicle(vehicle).wheel_accumulators_address() + 16 * w, f + 96);
    const double moved = Fadds(double(schema::VehicleWheel(wheel).suspension_force()), v);
    guest::StoreU32(f + 80, guest::LoadU32(kZero));
    CopyWord(kOne, f + 84);
    CopyWord(kZero, f + 88);
    CopyWord(kZero, f + 92);
    schema::VehicleWheel(wheel).set_suspension_force(float(moved));
    vl::Vec4Scaled(f + 128, f + 80, moved);
    const double up = double(vl::Vec3DotOf(schema::VehicleWheel(wheel).ground_normal_address(), f + 128));
    vl::Vec4Scaled(f + 144, schema::VehicleWheel(wheel).ground_normal_address(), up);
    vl::Vec4Scaled(f + 160, f + 80, double(schema::VehicleWheel(wheel).suspension_force()));
    vl::Vec3Difference(f + 112, f + 160, f + 144);
    const uint32_t body = schema::Vehicle(vehicle).body().address;
    va::RigidBodyAddTorqueAt(body, f + 112, schema::VehicleWheel(wheel).contact_point_address());
    r3 = schema::RigidBody(body).torque_address();
  }
  return r3;
}

double VehicleEngineTorque(uint32_t vehicle, double x, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B00C8 Vehicle::EngineTorque");
  const schema::CarData engine_data(CarData(vehicle));
  const uint32_t engine = engine_data.gear_count_address();
  const double one = Lfs(kOne);
  if (!(x < one)) {
    return Lfs(kZero);
  }
  const double floor = Lfs(engine_data.idle_rpm_address());
  if (!(x > floor)) {
    x = floor;
  }
  // The curve's segment: fctidz, its low word through the stack (stfiwx at
  // r1 - 16), less 1, as the translated code (64-bit registers).
  double t = Fmuls(x, Lfs(0x820029CC));
  int64_t whole;
  if (std::isnan(t)) {
    whole = INT64_MIN;
  } else if (t > double(LLONG_MAX)) {
    whole = LLONG_MAX;
  } else {
    whole = simde_mm_cvttsd_si64(simde_mm_set_sd(t));
  }
  guest::StoreU32(frame - 16, uint32_t(whole));
  const uint64_t i = uint64_t(guest::LoadU32(frame - 16)) - 1;
  double torque;
  if (uint32_t(i) == 32) {
    torque = Lfs(engine_data.torque_address());
  } else {
    t = Fsubs(t, one);
    const double zero = Lfs(kZero);
    if (t < zero) {
      t = zero;
    }
    const uint64_t low = i & 0xFFFFFFFFu;
    const uint32_t upper_at = uint32_t(i + 7) << 2;
    const uint32_t lower_at = uint32_t(i + 6) << 2;
    guest::StoreU32(frame - 16, uint32_t(low >> 32));
    guest::StoreU32(frame - 12, uint32_t(low));
    const double upper = Lfs(upper_at + engine);
    const double lower = Lfs(lower_at + engine);
    const double segment = double(float(double(int64_t(low))));
    const double fraction = Fsubs(t, segment);
    const double rest = Fsubs(one, fraction);
    torque = Fmadds(lower, rest, Fmuls(upper, fraction));
  }
  const double scale = Fmuls(double(schema::Vehicle(vehicle).turbo_torque_factor()), Lfs(engine_data.torque_copy_address()));
  return Fmuls(scale, torque);
}

double TireMagicFormula(uint32_t coefficients, double x) {
  ITC_NATIVE_RUNNING("sub_820B09C8 Tire::MagicFormula");
  const uint32_t c = coefficients;
  double a = c_runtime::Atan(Fmuls(Lfs(c + 0), x));
  const double e = Lfs(c + 12);
  const double b = Lfs(c + 0);
  const double atan_bx = double(float(a));
  const double one_less_e = Fsubs(Lfs(kOne), e);
  const double e_over_b = Fdivs(e, b);
  const double term = Fmuls(one_less_e, x);
  const double inner = Fmadds(e_over_b, atan_bx, term);
  a = c_runtime::Atan(Fmuls(inner, b));
  const double atan_inner = double(float(a));
  const double s = c_runtime::Sin(Fmuls(Lfs(c + 4), atan_inner));
  return Fmuls(Lfs(c + 8), double(float(s)));
}

void VehicleSetGearValues(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820B3C90 Vehicle::SetGearValues");
  uint32_t gear = uint32_t(schema::Vehicle(vehicle).gear());
  if (gear == 0xFFFFFFFFu) {
    gear = 0;
  } else if (gear == 0) {
    gear = 1;
  }
  const uint32_t gearbox = schema::CarData(CarData(vehicle)).gear_count_address();
  const double clutch = double(schema::Vehicle(vehicle).unknown_2328());
  const double sum = Fadds(double(schema::Vehicle(vehicle).axle_radius()), clutch);
  const double final_drive = Lfs(gearbox + 332);
  const double ratio = Fmuls(Lfs((uint32_t(gear + 74) << 2) + gearbox), final_drive);
  CopyWord(schema::Vehicle(vehicle).unknown_2328_address(), schema::Vehicle(vehicle).unknown_2324_address());
  CopyWord(kZero, schema::Vehicle(vehicle).unknown_2320_address());
  schema::Vehicle(vehicle).set_unknown_2312(float(sum));
  schema::Vehicle(vehicle).set_total_ratio(float(Fabs(ratio)));
  const double value = Lfs((uint32_t(gear + 58) << 2) + gearbox);
  schema::Vehicle(vehicle).set_gear_value(float(Fmuls(value, Lfs(gearbox + 276))));
}

double VehicleAxleReactionTorque(uint32_t vehicle, uint32_t first) {
  ITC_NATIVE_RUNNING("sub_820B3130 Vehicle::AxleReactionTorque");
  const double zero = Lfs(kZero);
  const uint32_t data = guest::LoadU32(vehicle + (uint32_t(first + 540) << 2));
  double sum = zero;
  if (first >= first + 2) {
    return sum;
  }
  const double minus_one = Lfs(0x82001BE0);
  const double plus_one = Lfs(kOne);
  for (uint32_t w = first; w < first + 2; w++) {
    const uint32_t spin_at = vehicle + kWheelBytes * w + 664;
    const double spin = Lfs(spin_at);
    const double brake = Fabs(Lfs(spin_at + 52));
    double sign;
    if (spin == zero) {
      sign = zero;
    } else if (spin > zero) {
      sign = minus_one;
    } else {
      sign = plus_one;
    }
    double torque = Fmsubs(sign, brake, Lfs(spin_at + 60));
    torque = Fmuls(torque, double(schema::CarDataAxle(data).radius()));
    Stfs(spin_at + 104, torque);
    sum = Fadds(torque, sum);
  }
  return sum;
}

void VehicleUpdateFreeAxle(uint32_t vehicle, uint32_t first) {
  ITC_NATIVE_RUNNING("sub_820B3010 Vehicle::UpdateFreeAxle");
  const uint32_t record = guest::LoadU32(vehicle + (uint32_t(first + 536) << 2));
  const uint32_t data = guest::LoadU32(vehicle + (uint32_t(first + 540) << 2));
  const double radius = double(schema::CarDataAxle(data).radius());
  const double inertia = Fmuls(double(schema::VehicleAxle(record).inertia()), Lfs(0x82001BE4));
  if (first >= first + 2) {
    return;
  }
  const double inverse = Fdivs(Lfs(kOne), inertia);
  const double zero = Lfs(kZero);
  for (uint32_t w = first; w < first + 2; w++) {
    const uint32_t spin_at = vehicle + kWheelBytes * w + 664;
    // The tire's torque (+308 x radius) turns the wheel.
    double drive = Fmuls(Lfs(spin_at + 60), radius);
    const double brake = Fabs(Lfs(spin_at + 52));
    const double time = double(schema::Vehicle(vehicle).substep_dt());
    const double spin0 = Lfs(spin_at);
    drive = Fneg(drive);
    drive = Fmuls(drive, inverse);
    const double spun = Fmadds(drive, time, spin0);
    Stfs(spin_at, spun);
    // The brake slows it toward 0.0, not past it (the branches as the
    // original's: ble and bge are "not greater" and "not less").
    double braking = Fmuls(brake, radius);
    braking = Fmuls(braking, double(schema::Vehicle(vehicle).substep_dt()));
    braking = Fmuls(braking, inverse);
    bool stop = false;
    double moved = spun;
    if (spun > zero) {
      if (spun > braking) {
        moved = Fsubs(spun, braking);
      } else {
        stop = true;
      }
    } else if (!(spun < zero)) {
      stop = true;
    } else if (!(spun < Fneg(braking))) {
      stop = true;
    } else {
      moved = Fadds(spun, braking);
    }
    if (stop) {
      Stfs(spin_at, zero);
    } else {
      Stfs(spin_at, moved);
    }
    const double spin = Lfs(spin_at);
    Stfs(spin_at + 4, Fmuls(spin, inertia));
    if (brake > zero) {
      // +212 = |the brake's change of spin x inertia / radius x +216|.
      double t = Fsubs(spin, spun);
      const double time2 = double(schema::Vehicle(vehicle).substep_dt());
      const double share = Lfs(spin_at - 32);
      t = Fdivs(t, time2);
      t = Fmuls(t, inertia);
      t = Fdivs(t, radius);
      t = Fmuls(t, share);
      Stfs(spin_at - 36, Fabs(t));
    }
  }
}

namespace {

// sub_820B3E08's, sub_820B3E38's and sub_820B3E68's steps: the value
// clamped to [0.0, 1.0] (a NaN kept) into `field`, and returned in f1.
double ClampInto(uint32_t vehicle, double value, uint32_t field) {
  const double zero = Lfs(kZero);
  if (value < zero) {
    value = zero;
  } else {
    const double one = Lfs(kOne);
    if (value > one) {
      value = one;
    }
  }
  Stfs(vehicle + field, value);
  return value;
}

}  // namespace

double VehicleSetBrake(uint32_t vehicle, double value) {
  ITC_NATIVE_RUNNING("sub_820B3E08 Vehicle::SetBrake");
  return ClampInto(vehicle, value, 292);
}

double VehicleSetHandbrake(uint32_t vehicle, double value) {
  ITC_NATIVE_RUNNING("sub_820B3E38 Vehicle::SetHandbrake");
  return ClampInto(vehicle, value, 296);
}

double VehicleSetThrottle(uint32_t vehicle, double value) {
  ITC_NATIVE_RUNNING("sub_820B3E68 Vehicle::SetThrottle");
  return ClampInto(vehicle, value, 284);
}

namespace {

// sub_820B0478's and sub_820B04C0's steps: `field` ramps up by the car
// data's `rate` x time while `on`, to 1.0; else 0.0.
void Ramp(uint32_t vehicle, uint32_t on, double time, uint32_t field,
          uint32_t rate) {
  if ((on & 0xFF) == 0) {
    CopyWord(kZero, vehicle + field);
    return;
  }
  const uint32_t data = CarData(vehicle);
  const double current = Lfs(vehicle + field);
  const double value = Fmadds(Lfs(data + rate), time, current);
  Stfs(vehicle + field, value);
  if (value > Lfs(kOne)) {
    CopyWord(kOne, vehicle + field);
  }
}

}  // namespace

void VehicleRampReverseThrottle(uint32_t vehicle, uint32_t on, double time) {
  ITC_NATIVE_RUNNING("sub_820B0478 Vehicle::RampReverseThrottle");
  Ramp(vehicle, on, time, 288, 1264);
}

void VehicleRampHandbrake(uint32_t vehicle, uint32_t on, double time) {
  ITC_NATIVE_RUNNING("sub_820B04C0 Vehicle::RampHandbrake");
  Ramp(vehicle, on, time, 296, 128);
}

void VehicleSetValue2388(uint32_t vehicle, double value, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_820B3C20 Vehicle::SetValue2388");
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  double clamped;
  if (!(value < zero)) {
    clamped = value > one ? one : value;
  } else {
    clamped = zero;
  }
  schema::Vehicle(vehicle).set_value2388(float(clamped));
  schema::Vehicle(vehicle).set_value2388_set(uint8_t(uint8_t(flag)));
  double rest = Fnmsubs(value, Lfs(0x82002A2C), one);
  if (rest < zero) {
    rest = zero;
  } else if (rest > one) {
    rest = one;
  }
  schema::Vehicle(vehicle).set_traction_limit(float(rest));
}

}  // namespace car_physics
