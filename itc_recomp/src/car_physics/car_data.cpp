// The car data record (vehicle +272) and the vehicle's constructor
// (docs/functions/car-physics.md): see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "matrix_math/matrix_math.h"
#include "schema/car_physics.h"
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

constexpr uint32_t kHalf = 0x82001BE4;  // 0.5

// memset (sub_825A71D0) with 0, as its adapter does it.
void Clear(uint32_t at, uint32_t bytes) {
  guest::ForEachHostPart(at, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

double AsDouble(uint32_t word) {
  return double(std::bit_cast<float>(word));
}

}  // namespace

uint32_t CarDataSetDefaults(uint32_t data, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AECB8 CarData::SetDefaults");
  const uint32_t d = data, f = frame;
  auto one = [&](uint32_t at) { CopyWord(kOne, at); };
  auto zero = [&](uint32_t at) { CopyWord(kZero, at); };
  one(schema::CarData(d).mass_address());
  one(schema::CarData(d).wheelbase_address());
  one(schema::CarData(d).front_track_address());
  one(f + 80);
  one(schema::CarData(d).rear_track_address());
  one(f + 84);
  one(f + 88);
  zero(f + 92);
  vl::Vec4Copy(schema::CarData(d).inertia_factors_address(), f + 80);
  one(f + 96);
  one(f + 100);
  one(f + 104);
  zero(f + 108);
  vl::Vec4Copy(schema::CarData(d).inertia_address(), f + 96);
  vl::Vec4Copy(schema::CarData(d).center_of_mass_offset_address(), kZeroVector);
  vl::Vec4Copy(schema::CarData(d).center_of_mass_air_address(), kZeroVector);
  for (uint32_t at : {128u, 132u, 136u, 264u, 268u}) {
    zero(d + at);
  }
  Clear(schema::CarData(d).unknown_276_address(), 32);
  one(schema::CarData(d).steering_316_address());
  zero(schema::CarData(d).max_steering_angle_address());
  zero(schema::CarData(d).unknown_332_address());
  CopyWord(0x82002950, schema::CarData(d).steering_320_address());
  Clear(schema::CarData(d).front_axle_address(), 272);
  Clear(schema::CarData(d).rear_axle_address(), 272);
  Clear(schema::CarData(d).wheel_setups_address(), 384);
  zero(schema::CarData(d).reverse_ramp_address());
  Clear(schema::CarData(d).gear_count_address(), 436);
  for (uint32_t at : {180u, 1704u, 1808u, 1708u, 1712u, 1716u, 1812u, 1816u, 1720u, 1724u,
                      1728u, 1736u, 1740u, 1744u}) {
    zero(d + at);
  }
  constexpr uint32_t kThird = 0x82001BE8;
  CopyWord(kThird, f + 112);
  zero(schema::CarData(d).rear_height_address());
  CopyWord(kThird, f + 116);
  zero(schema::CarData(d).rear_damping_2_address());
  CopyWord(kThird, f + 120);
  zero(f + 124);
  vl::Vec4Copy(schema::CarData(d).box_1760_address(), f + 112);
  zero(f + 140);
  CopyWord(kHalf, f + 128);
  CopyWord(kHalf, f + 132);
  CopyWord(kHalf, f + 136);
  vl::Vec4Copy(schema::CarData(d).box_1776_address(), f + 128);
  zero(schema::CarData(d).box_value_1792_address());
  zero(schema::CarData(d).box_value_1796_address());
  // Each axle's +152, +160 and +164.
  for (uint32_t at : {496u, 500u, 488u, 768u, 772u, 760u}) {
    one(d + at);
  }
  return schema::CarData(d).box_1776_address();
}

void CarDataConstruct(uint32_t data, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AEF80 CarData::CarData");
  const uint32_t d = data;
  for (uint32_t at : {44u, 60u, 76u, 92u, 108u, 124u, 348u, 428u, 588u, 620u, 700u, 860u}) {
    CopyWord(kZero, d + at);
  }
  // The four wheel set-up records' +12 and +28 (from +880).
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(kZero, schema::CarData(d).wheel_setups_address() + 12 + 96 * i);
    CopyWord(kZero, schema::CarData(d).wheel_setups_address() + 28 + 96 * i);
  }
  CopyWord(kZero, schema::CarData(d).box_1760_address() + 12);
  CopyWord(kZero, schema::CarData(d).box_1776_address() + 12);
  CarDataSetDefaults(d, frame - 192);
}

void CarDataFillAxle(uint32_t data, uint32_t source, uint32_t is_front) {
  ITC_NATIVE_RUNNING("sub_820AEE98 CarData::FillAxle");
  const uint32_t a = data + ((is_front & 0xFF) != 0 ? 336 : 608);
  const uint32_t s = source;
  auto word = [&](uint32_t at) { return guest::LoadU32(at); };
  // The loads stored as they are keep their bits (lfs then stfs).
  const uint32_t w4 = word(schema::CarDataTireSource(s).s4_address());
  const double s4 = AsDouble(w4), s0 = AsDouble(word(schema::CarDataTireSource(s).s0_address()));
  const double one = Lfs(kOne);
  constexpr uint32_t kTwo = 0x820021E8;
  double f13 = Fdivs(one, s4);
  const double two = Lfs(kTwo);
  double f9 = Fmuls(f13, s0);
  f13 = Fmuls(f13, f13);
  f9 = Fmuls(f9, two);
  schema::CarDataAxle(a).set_tire_16(float(f9));
  f13 = Fmuls(f13, s0);
  schema::CarDataAxle(a).set_tire_20(float(f13));
  guest::StoreU32(schema::CarDataAxle(a).tire_24_address(), word(schema::CarDataTireSource(s).s8_address()));
  guest::StoreU32(schema::CarDataAxle(a).tire_28_address(), w4);
  const uint32_t w20 = word(schema::CarDataTireSource(s).s20_address());
  const double s20 = AsDouble(w20);
  f13 = Fdivs(one, s20);
  const double s16 = AsDouble(word(schema::CarDataTireSource(s).s16_address()));
  f9 = Fmuls(f13, s16);
  f13 = Fmuls(f13, f13);
  Stfs(schema::CarDataAxle(a).tire_32_address(), Fmuls(f9, two));
  f13 = Fmuls(f13, s16);
  Stfs(schema::CarDataAxle(a).tire_32_address() + 4, f13);
  const uint32_t w24 = word(schema::CarDataTireSource(s).s24_address());
  guest::StoreU32(schema::CarDataAxle(a).tire_32_address() + 12, w20);
  guest::StoreU32(schema::CarDataAxle(a).tire_32_address() + 8, w24);
  const uint32_t w32 = word(schema::CarDataTireSource(s).radius_address());
  const double radius = AsDouble(w32);
  guest::StoreU32(schema::CarDataAxle(a).radius_address(), w32);
  schema::CarDataAxle(a).set_inverse_radius(float(Fdivs(one, radius)));
  guest::StoreU32(schema::CarDataAxle(a).width_address(), word(schema::CarDataTireSource(s).width_address()));
  const uint32_t w40 = word(schema::CarDataTireSource(s).mass_address());
  guest::StoreU32(schema::CarDataAxle(a).wheel_mass_address(), w40);
  guest::StoreU32(schema::CarDataAxle(a).unknown_180_address(), word(schema::CarDataTireSource(s).s44_address()));
  guest::StoreU32(schema::CarDataAxle(a).unknown_180_address() + 4, word(schema::CarDataTireSource(s).s48_address()));
  double f0 = Fmuls(AsDouble(w40), radius);
  guest::StoreU32(schema::CarDataAxle(a).unknown_180_address() + 8, word(schema::CarDataTireSource(s).s52_address()));
  guest::StoreU32(schema::CarDataAxle(a).unknown_180_address() + 12, word(schema::CarDataTireSource(s).s56_address()));
  f0 = Fmuls(f0, radius);
  schema::CarDataAxle(a).set_wheel_inertia(float(f0));
  guest::StoreU32(schema::CarDataAxle(a).unknown_140_address(), word(schema::CarDataTireSource(s).s60_address()));
  guest::StoreU32(schema::CarDataAxle(a).unknown_144_address(), word(schema::CarDataTireSource(s).s64_address()));
}

double CarDataConvertUnits(uint32_t data) {
  ITC_NATIVE_RUNNING("sub_820AF018 CarData::ConvertUnits");
  const uint32_t d = data;
  auto at = [&](uint32_t offset) { return Lfs(offset); };
  const double rpm = at(schema::CarData(d).max_rpm_address());
  const double v316 = at(schema::CarData(d).steering_316_address());
  const double v320 = at(schema::CarData(d).steering_320_address());
  double f5 = at(schema::CarData(d).max_steering_angle_address());
  double f3 = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).angles_address() + 4);
  const double one = Lfs(kOne);
  const double f10 = Fdivs(one, rpm);
  double f1 = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).angles_address());
  double f30 = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).angles_address() + 8);
  double f28 = at(schema::CarData(d).unknown_1712_address());
  double f27 = at(schema::CarData(d).unknown_1812_address());
  const double zero = Lfs(kZero);
  const double f26 = at(schema::CarData(d).unknown_1816_address());
  double f9 = zero;
  const double f25 = at(schema::CarData(d).idle_rpm_address());
  const double scale_a = Lfs(0x8200295C);
  double f7 = Fmuls(v316, scale_a);
  const double v1264 = at(schema::CarData(d).reverse_ramp_address());
  const double f8a = Fmuls(v320, scale_a);
  const double v128 = at(schema::CarData(d).handbrake_ramp_address());
  const double f4 = Fdivs(one, v1264);
  const double v540 = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).angles_address());
  const double f6a = Fdivs(one, v128);
  schema::CarData(d).set_handbrake_ramp(float(f6a));
  const double degrees = Lfs(0x82002948);
  const double f2 = Fmuls(v540, degrees);
  const double f31 = Fmuls(at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).angles_address() + 8), degrees);
  const double f29 = Fmuls(at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).angles_address() + 4), degrees);
  schema::CarData(d).set_reverse_ramp(float(f4));
  f5 = Fmuls(f5, degrees);
  Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).angles_address(), f2);
  const double scale_b = Lfs(0x82002958);
  f7 = Fmuls(f7, scale_b);
  schema::CarData(d).set_max_steering_angle(float(f5));
  double f12 = Fmuls(f8a, scale_b);
  double f8 = at(schema::CarData(d).unknown_1716_address());
  f3 = Fmuls(f3, degrees);
  Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).angles_address() + 8, f31);
  f1 = Fmuls(f1, degrees);
  Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).angles_address() + 4, f3);
  f30 = Fmuls(f30, degrees);
  const double scale_c = Lfs(0x82002954);
  f28 = Fmuls(f28, scale_c);
  Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).angles_address(), f1);
  f8 = Fmuls(f8, scale_c);
  Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).angles_address() + 4, f29);
  f27 = Fmuls(f27, scale_c);
  Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).angles_address() + 8, f30);
  const double f0a = Fmuls(f26, scale_c);
  schema::CarData(d).set_unknown_1712(float(f28));
  const double f6 = Fmuls(f25, f10);
  schema::CarData(d).set_unknown_1716(float(f8));
  f7 = Fdivs(one, f7);
  schema::CarData(d).set_unknown_1812(float(f27));
  f12 = Fdivs(one, f12);
  schema::CarData(d).set_steering_316(float(f7));
  schema::CarData(d).set_steering_320(float(f12));
  schema::CarData(d).set_unknown_1816(float(f0a));
  schema::CarData(d).set_idle_rpm(float(f6));
  // The torque table's sum.
  for (uint32_t i = 0; i < 32; i++) {
    f9 = Fadds(at(schema::CarData(d).torque_curve_address() + 4 * i), f9);
  }
  double f0 = Fmuls(at(schema::CarData(d).rev_limit_address()), f10);
  const double v1420 = at(schema::CarData(d).torque_address());
  schema::CarData(d).set_rev_limit_normalized(float(f0));
  double torque = Fmuls(v1420, Lfs(0x82600CC8));
  torque = Fmuls(torque, f9);
  schema::CarData(d).set_engine_inertia(float(torque));
  // Clamped to [0, 1] (bge and ble: a NaN is kept).
  if (f0 < zero) {
    f0 = zero;
  } else if (f0 > one) {
    f0 = one;
  }
  schema::CarData(d).set_rev_limit_normalized(float(f0));
  const double v1616 = at(schema::CarData(d).boost_1616_address());
  const double v184 = at(schema::CarData(d).axle_table_record_address());
  schema::CarData(d).set_boost_1616(float(Fdivs(one, v1616)));
  Stfs(schema::CarData(d).axle_table_record_address(), Fdivs(one, v184));
  return f1;
}

namespace {

// The 16 bytes of a quaternion (ld and std, two doublewords: the bits).
void Copy16(uint32_t from, uint32_t to) {
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyWord(from + i, to + i);
  }
}

// An axle's point (+80) moved by its camber a: y -= (|+60 sin a x 0.5| +
// cos a x radius), z -= sin a x radius (the original's steps; `radius_at`
// the radius it reads, +52).
void MoveAxlePoint(uint32_t axle, double c, double s, double half,
                   uint32_t radius_at) {
  const double width = Lfs(axle + 60);
  const double cr = double(float(c));
  const double radius = Lfs(radius_at);
  const double y = Lfs(axle + 84);
  double t = double(float(s));
  t = Fmuls(width, t);
  t = Fmuls(t, half);
  t = Fabs(t);
  t = Fmadds(cr, radius, t);
  Stfs(axle + 84, Fsubs(y, t));
}

// The clamp the suspension's +124 and its rear twin get: below `low` to
// `low`, above 0.0 to 0.0 (blt, then ble: a NaN is kept).
double ClampToZero(double value, double low, double zero) {
  if (value < low) {
    return low;
  }
  if (value > zero) {
    return zero;
  }
  return value;
}

}  // namespace

double CarDataDerive(uint32_t data, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AF1C0 CarData::Derive");
  const uint32_t d = data, f = frame;
  auto at = [&](uint32_t offset) { return Lfs(offset); };
  constexpr uint32_t kRadius = 0x8200296C;   // an axle's radius when 0.0
  constexpr uint32_t kMirror = 0x82001BE0;   // the mirrored wheels' z factor
  constexpr uint32_t kQuarter = 0x82002968;  // the mean of the four points
  constexpr uint32_t kTwo = 0x820021E8;
  constexpr uint32_t kThird = 0x82001BE8;
  constexpr uint32_t kInertia = 0x82001070;
  constexpr uint32_t kDistance = 0x8200222C;  // a distance when 0.0
  constexpr uint32_t kGravity = 0x82002964;
  constexpr uint32_t kSpring = 0x82002960;
  constexpr uint32_t kDamping = 0x820021F8;
  constexpr uint32_t kAxis = 0x82638270;  // the wheels' turning axis
  const uint32_t front = schema::CarData(d).front_axle_address(), rear = schema::CarData(d).rear_axle_address();

  // The radii (+52 of each axle) of 0.0.
  const double zero = Lfs(kZero);
  const double v388 = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).radius_address());
  const double a_front = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).angles_address());
  const double a_rear = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).angles_address());
  const double radius = Lfs(kRadius);
  if (v388 == zero) {
    schema::CarDataAxle(schema::CarData(d).front_axle_address()).set_radius(float(radius));
  }
  if (at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).radius_address()) == zero) {
    schema::CarDataAxle(schema::CarData(d).rear_axle_address()).set_radius(float(radius));
  }

  // The front axle's point: its position moved by the camber.
  vl::Vec4Copy(schema::CarDataAxle(schema::CarData(d).front_axle_address()).point_address(), front);
  const double c_front = c_runtime::Cos(a_front);
  const double s_front = c_runtime::Sin(a_front);
  const double half = Lfs(kHalf);
  MoveAxlePoint(front, c_front, s_front, half, schema::CarDataAxle(schema::CarData(d).front_axle_address()).radius_address());
  {
    const double s = double(float(s_front));
    const double z = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).point_address() + 8);
    Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).point_address() + 8, Fnmsubs(s, at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).radius_address()), z));
  }
  // The rear's.
  vl::Vec4Copy(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).point_address(), rear);
  const double c_rear = c_runtime::Cos(a_rear);
  const double s_rear = c_runtime::Sin(a_rear);
  MoveAxlePoint(rear, c_rear, s_rear, half, schema::CarDataAxle(schema::CarData(d).rear_axle_address()).radius_address());
  const double one = Lfs(kOne);
  {
    const double r_front = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).radius_address());
    const double r_rear = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).radius_address());
    const double z = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).point_address() + 8);
    schema::CarDataAxle(schema::CarData(d).front_axle_address()).set_inverse_radius(float(Fdivs(one, r_front)));
    const double s = double(float(s_rear));
    schema::CarDataAxle(schema::CarData(d).rear_axle_address()).set_inverse_radius(float(Fdivs(one, r_rear)));
    Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).point_address() + 8, Fnmsubs(s, r_rear, z));
  }

  // The four wheel set-up records (+880 + 96i): the position, the point,
  // half the track, sin a, cos a and the turn's quaternion; the second of
  // each pair mirrored.
  vl::Vec4Copy(schema::CarData(d).wheel_setups_address() + 96, front);
  vl::Vec4Copy(schema::CarData(d).wheel_setups_address(), schema::CarData(d).wheel_setups_address() + 96);
  vl::Vec4Copy(schema::CarData(d).wheel_setups_address() + 112, schema::CarDataAxle(schema::CarData(d).front_axle_address()).point_address());
  vl::Vec4Copy(schema::CarData(d).wheel_setups_address() + 16, schema::CarData(d).wheel_setups_address() + 112);
  {
    const double s = double(float(s_front));
    const double r = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).radius_address());
    const double width = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).width_address());
    Stfs(schema::CarData(d).wheel_setups_address() + 32, Fnmsubs(s, r, width));
  }
  Stfs(schema::CarData(d).wheel_setups_address() + 36, s_front);
  Stfs(schema::CarData(d).wheel_setups_address() + 40, c_front);
  mm::QuaternionFromAxisAngle(f + 304, kAxis, a_front, f - 32);
  const double neg_front = Fneg(a_front);
  const double mirror = Lfs(kMirror);
  {
    const double z0 = at(schema::CarData(d).wheel_setups_address() + 8);
    const double z1 = at(schema::CarData(d).wheel_setups_address() + 24);
    const double track = at(schema::CarData(d).wheel_setups_address() + 32);
    Stfs(schema::CarData(d).wheel_setups_address() + 128, Fneg(track));
    Copy16(f + 304, schema::CarData(d).wheel_setups_address() + 48);
    Stfs(schema::CarData(d).wheel_setups_address() + 8, Fmuls(z0, mirror));
    Stfs(schema::CarData(d).wheel_setups_address() + 24, Fmuls(z1, mirror));
  }
  Stfs(schema::CarData(d).wheel_setups_address() + 132, c_runtime::Sin(neg_front));
  Stfs(schema::CarData(d).wheel_setups_address() + 136, c_runtime::Cos(neg_front));
  mm::QuaternionFromAxisAngle(f + 256, kAxis, neg_front, f - 32);
  Copy16(f + 256, schema::CarData(d).wheel_setups_address() + 144);
  vl::Vec4Copy(schema::CarData(d).wheel_setups_address() + 288, rear);
  vl::Vec4Copy(schema::CarData(d).wheel_setups_address() + 192, schema::CarData(d).wheel_setups_address() + 288);
  vl::Vec4Copy(schema::CarData(d).wheel_setups_address() + 304, schema::CarDataAxle(schema::CarData(d).rear_axle_address()).point_address());
  vl::Vec4Copy(schema::CarData(d).wheel_setups_address() + 208, schema::CarData(d).wheel_setups_address() + 304);
  {
    // The front's radius (+388), as the original reads it.
    const double s = double(float(s_rear));
    const double r = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).radius_address());
    const double width = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).width_address());
    Stfs(schema::CarData(d).wheel_setups_address() + 224, Fnmsubs(s, r, width));
  }
  Stfs(schema::CarData(d).wheel_setups_address() + 228, s_rear);
  Stfs(schema::CarData(d).wheel_setups_address() + 232, c_rear);
  mm::QuaternionFromAxisAngle(f + 288, kAxis, a_rear, f - 32);
  const double neg_rear = Fneg(a_rear);
  {
    const double z0 = Fmuls(at(schema::CarData(d).wheel_setups_address() + 200), mirror);
    const double track = at(schema::CarData(d).wheel_setups_address() + 224);
    const double z1 = Fmuls(at(schema::CarData(d).wheel_setups_address() + 216), mirror);
    Stfs(schema::CarData(d).wheel_setups_address() + 200, z0);
    Stfs(schema::CarData(d).wheel_setups_address() + 216, z1);
    Stfs(schema::CarData(d).wheel_setups_address() + 320, Fneg(track));
    Copy16(f + 288, schema::CarData(d).wheel_setups_address() + 240);
  }
  Stfs(schema::CarData(d).wheel_setups_address() + 324, c_runtime::Sin(neg_rear));
  Stfs(schema::CarData(d).wheel_setups_address() + 328, c_runtime::Cos(neg_rear));
  mm::QuaternionFromAxisAngle(f + 272, kAxis, neg_rear, f - 32);
  Copy16(f + 272, schema::CarData(d).wheel_setups_address() + 336);

  // The center of mass (+112): the mean of the four points plus the
  // offset +80.
  const uint32_t center = schema::CarData(d).center_of_mass_air_address();
  vl::Vec4Copy(center, schema::CarData(d).wheel_setups_address() + 16);
  vl::Vec3AddTo(center, schema::CarData(d).wheel_setups_address() + 112);
  vl::Vec3AddTo(center, schema::CarData(d).wheel_setups_address() + 208);
  vl::Vec3AddTo(center, schema::CarData(d).wheel_setups_address() + 304);
  const double quarter = Lfs(kQuarter);
  vl::Vec4ScaleBy(center, quarter);
  vl::Vec3AddTo(center, schema::CarData(d).center_of_mass_offset_address());

  // The total mass, the inertia (+64 to +72), the half tracks and the
  // distances from the center.
  const double two = Lfs(kTwo);
  double mass;
  double dx_front, dx_rear, wheelbase;
  {
    const double m_front = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).wheel_mass_address());
    const double z_front = at(schema::CarData(d).wheel_setups_address() + 24);
    const double m_rear = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).wheel_mass_address());
    double track_front = at(schema::CarData(d).wheel_setups_address() + 120);
    const double wheels = Fadds(m_rear, m_front);
    track_front = Fsubs(z_front, track_front);
    const double z_rear = at(schema::CarData(d).wheel_setups_address() + 312);
    double track_rear = at(schema::CarData(d).wheel_setups_address() + 216);
    track_rear = Fsubs(track_rear, z_rear);
    const double body = at(schema::CarData(d).mass_address());
    const double cx = at(schema::CarData(d).center_of_mass_air_address());
    const double x_rear = at(schema::CarData(d).rear_axle_address());
    const double x_front = at(schema::CarData(d).front_axle_address());
    const double i0 = at(schema::CarData(d).inertia_factors_address());
    dx_front = Fsubs(x_front, cx);
    const double i1 = at(schema::CarData(d).inertia_factors_address() + 4);
    mass = Fmadds(wheels, two, body);
    const double i2 = at(schema::CarData(d).inertia_factors_address() + 8);
    track_front = Fabs(track_front);
    dx_rear = Fsubs(x_rear, cx);
    track_rear = Fabs(track_rear);
    wheelbase = Fsubs(x_front, x_rear);
    const double m0 = Fmuls(i0, mass);
    Stfs(schema::CarData(d).wheel_setups_address() + 32, Fmuls(track_front, half));
    const double third = Lfs(kThird);
    const double m1 = Fmuls(i1, mass);
    const double m2 = Fmuls(i2, mass);
    Stfs(schema::CarData(d).wheel_setups_address() + 128, Fmuls(track_front, third));
    Stfs(schema::CarData(d).wheel_setups_address() + 320, Fmuls(track_rear, third));
    Stfs(schema::CarData(d).wheel_setups_address() + 224, Fmuls(track_rear, half));
    const double inertia = Lfs(kInertia);
    Stfs(schema::CarData(d).inertia_address(), Fmuls(m0, inertia));
    Stfs(schema::CarData(d).inertia_address() + 4, Fmuls(m1, inertia));
    Stfs(schema::CarData(d).inertia_address() + 8, Fmuls(m2, inertia));
  }
  {
    const double fallback = Lfs(kDistance);
    if (dx_rear == zero) {
      dx_rear = fallback;
    }
    if (dx_front == zero) {
      dx_front = fallback;
    }
    if (wheelbase == zero) {
      wheelbase = fallback;
    }
  }

  // The weight shares, the roll axis (+16 to +24) and the stack's vectors.
  double f30;
  {
    const double inv_base = Fdivs(one, wheelbase);
    Stfs(f + 80, Fmsubs(wheelbase, half, at(schema::CarData(d).center_of_mass_offset_address())));
    const double split = at(schema::CarData(d).unknown_312_address());
    const double base_sq = Fmuls(wheelbase, wheelbase);
    double rest = Fsubs(one, split);
    const double v132 = at(schema::CarData(d).front_brake_factor_address());
    double split2 = Fmuls(split, two);
    Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).angles_address() + 12, dx_rear);
    Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).angles_address() + 12, dx_front);
    const uint32_t w148 = guest::LoadU32(schema::CarData(d).unknown_148_address());
    const double v136 = at(schema::CarData(d).rear_brake_factor_address());
    guest::StoreU32(schema::CarData(d).unknown_272_address(), w148);
    const double h_rear = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).unknown_256_address());
    double share_f = Fmuls(inv_base, dx_rear);
    const double h_front = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).unknown_256_address());
    double share_r = Fmuls(inv_base, dx_front);
    const double h84 = at(schema::CarData(d).center_of_mass_offset_address() + 4);
    for (uint32_t o : {92u, 172u, 236u, 140u, 220u, 88u}) {
      Stfs(f + o, zero);
    }
    Stfs(f + 160, wheelbase);
    Stfs(f + 168, zero);
    share_f = Fabs(share_f);
    schema::CarDataAxle(schema::CarData(d).front_axle_address()).set_weight_share(float(share_f));
    share_r = Fabs(share_r);
    schema::CarDataAxle(schema::CarData(d).rear_axle_address()).set_weight_share(float(share_r));
    rest = Fmuls(rest, two);
    schema::CarData(d).set_front_brake(float(Fmuls(split2, v132)));
    Stfs(f + 84, Fsubs(h84, h_front));
    const double load_f = Fmuls(share_f, mass);
    const double load_r = Fmuls(share_r, mass);
    const double g = Lfs(kGravity);
    schema::CarData(d).set_rear_brake(float(Fmuls(rest, v136)));
    const double dh = Fsubs(h_rear, h_front);
    Stfs(f + 164, dh);
    double h = Fmuls(h_front, share_f);
    schema::CarDataAxle(schema::CarData(d).front_axle_address()).set_unknown_220(float(Fmuls(load_f, g)));
    schema::CarDataAxle(schema::CarData(d).rear_axle_address()).set_unknown_220(float(Fmuls(load_r, g)));
    double len = Fmadds(dh, dh, base_sq);
    h = Fmadds(share_r, h_rear, h);
    len = double(float(std::sqrt(len)));
    f30 = Fsubs(h84, h);
    const double k = Fdivs(f30, len);
    Stfs(schema::CarData(d).roll_axis_address(), Fmuls(k, dh));
    Stfs(schema::CarData(d).roll_axis_address() + 4, Fmuls(k, wheelbase));
  }
  vl::Vec4Copy(f + 224, f + 80);
  vl::Vec3NormalizeSelf(f + 224);
  vl::Vec4Copy(f + 128, f + 160);
  vl::Vec3NormalizeSelf(f + 128);
  const double dot = double(vl::Vec3DotOf(f + 80, f + 128));
  vl::Vec4ScaledFrom(f + 320, f + 128, dot);
  vl::Vec3Difference(f + 336, f + 320, f + 80);
  vl::Vec4Copy(f + 208, f + 336);
  {
    const double y = Lfs(f + 212);
    const uint32_t x = guest::LoadU32(f + 208);
    guest::StoreU32(schema::CarData(d).roll_axis_address(), x);
    Stfs(schema::CarData(d).roll_axis_address() + 4, Fneg(y));
  }
  vl::Vec4Copy(schema::CarData(d).center_of_mass_ground_address(), center);
  Stfs(schema::CarData(d).roll_axis_address() + 8, f30);

  // The temporary rigid body and the effective masses at the wheels.
  const uint32_t body = f + 352;
  RigidBodyConstruct(body, f - 96);
  RigidBodyReset(body, f - 176);
  va::RigidBodySetMass(body, at(schema::CarData(d).mass_address()));
  RigidBodySetInertia(body, schema::CarData(d).inertia_address(), f - 320);
  va::RigidBodySetCenterOfMass(body, schema::CarData(d).center_of_mass_ground_address());
  CopyWord(front, f + 96);
  CopyWord(rear, f + 112);
  for (uint32_t o : {100u, 104u, 108u, 116u, 120u, 124u}) {
    Stfs(f + o, zero);
  }
  RigidBodyEffectiveMassAt(body, schema::CarDataAxle(schema::CarData(d).front_axle_address()).effective_mass_address(), f + 96, f - 272);
  RigidBodyEffectiveMassAt(body, schema::CarDataAxle(schema::CarData(d).rear_axle_address()).effective_mass_address(), f + 112, f - 272);
  CopyWord(front, f + 144);
  {
    const uint32_t z = guest::LoadU32(schema::CarDataAxle(schema::CarData(d).front_axle_address()).position_address() + 8);
    guest::StoreU32(f + 152, z);
    const uint32_t y = guest::LoadU32(schema::CarDataAxle(schema::CarData(d).front_axle_address()).position_address() + 4);
    for (uint32_t o : {204u, 252u, 156u}) {
      Stfs(f + o, zero);
    }
    guest::StoreU32(f + 148, y);
  }
  RigidBodyEffectiveMassAt(body, f + 240, f + 144, f - 272);
  CopyWord(rear, f + 176);
  CopyWord(schema::CarDataAxle(schema::CarData(d).front_axle_address()).position_address() + 4, f + 180);
  CopyWord(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).position_address() + 8, f + 184);
  Stfs(f + 188, zero);
  RigidBodyEffectiveMassAt(body, f + 192, f + 176, f - 272);

  // The suspension sub-records (axle +96: d + 432 and d + 704).
  double f1;
  {
    double spring = at(schema::CarData(d).front_spring_address());
    const uint32_t w_front = guest::LoadU32(f + 244);
    const uint32_t w_rear = guest::LoadU32(f + 196);
    const double m_front = AsDouble(w_front);
    const double m_rear = AsDouble(w_rear);
    const double inv_front = Fdivs(one, m_front);
    const uint32_t w556 = guest::LoadU32(schema::CarDataAxle(schema::CarData(d).front_axle_address()).unknown_220_address());
    const double inv_rear = Fdivs(one, m_rear);
    guest::StoreU32(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address(), w_front);
    const double k = Lfs(kSpring);
    spring = Fmuls(spring, k);
    const uint32_t w828 = guest::LoadU32(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).unknown_220_address());
    guest::StoreU32(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address(), w_rear);
    Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 4, inv_front);
    Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address() + 4, inv_rear);
    guest::StoreU32(schema::CarDataAxle(schema::CarData(d).front_axle_address()).unknown_128_address(), w556);
    double km = Fmuls(spring, m_front);
    guest::StoreU32(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).unknown_128_address(), w828);
    Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 8, spring);
    const double damping = Lfs(kDamping);
    f1 = double(vl::Sqrtf(Fmuls(km, damping)));
    double spring_rear = Fmuls(at(schema::CarData(d).rear_spring_address()), k);
    Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 16, Fmuls(at(schema::CarData(d).front_damping_address()), f1));
    const double m_rear2 = Lfs(f + 196);
    const double d1728 = at(schema::CarData(d).front_damping_2_address());
    const double d1732 = at(schema::CarData(d).front_anti_roll_address());
    const double c1 = Fmuls(d1728, f1);
    const double c2 = Fmuls(d1732, k);
    Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 20, c1);
    schema::CarDataAxle(schema::CarData(d).front_axle_address()).set_unknown_132(float(c2));
    Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address() + 8, spring_rear);
    km = Fmuls(spring_rear, m_rear2);
    f1 = double(vl::Sqrtf(Fmuls(km, damping)));
    double r9 = at(schema::CarData(d).rear_anti_roll_address());
    const double s440 = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 8);
    r9 = Fmuls(r9, k);
    const double s464 = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).unknown_128_address());
    schema::CarDataAxle(schema::CarData(d).rear_axle_address()).set_unknown_132(float(r9));
    const double ratio = Fdivs(s464, s440);
    guest::StoreU32(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 12, guest::LoadU32(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 8));
    const double d1744 = at(schema::CarData(d).rear_damping_address());
    const double d1748 = at(schema::CarData(d).rear_damping_2_address());
    const double c3 = Fmuls(d1744, f1);
    const double preload = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 28);
    Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address() + 16, c3);
    const double c4 = Fmuls(d1748, f1);
    const uint32_t w712 = guest::LoadU32(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address() + 8);
    const double s712 = AsDouble(w712);
    Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address() + 20, c4);
    guest::StoreU32(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address() + 12, w712);
    const double clamped = ClampToZero(preload, Fneg(ratio), zero);
    const double ratio_rear = Fdivs(at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).unknown_128_address()), s712);
    Stfs(schema::CarDataAxle(schema::CarData(d).front_axle_address()).suspension_address() + 28, clamped);
    const double preload_rear = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address() + 28);
    Stfs(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).suspension_address() + 28, ClampToZero(preload_rear, Fneg(ratio_rear), zero));
  }
  {
    const double h84 = at(schema::CarData(d).center_of_mass_offset_address() + 4);
    const double h_front = at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).unknown_256_address());
    const double t_front = Fsubs(h_front, h84);
    const double h_rear = at(schema::CarDataAxle(schema::CarData(d).rear_axle_address()).unknown_256_address());
    const double t_rear = Fsubs(h_rear, h84);
    schema::CarDataAxle(schema::CarData(d).front_axle_address()).set_ride_offset(float(t_front));
    schema::CarDataAxle(schema::CarData(d).rear_axle_address()).set_ride_offset(float(t_rear));
    const double v132 = at(schema::CarData(d).front_brake_factor_address());
    double sum = Fadds(at(schema::CarData(d).rear_brake_factor_address()), v132);
    const double body_mass = at(schema::CarData(d).mass_address());
    const double load = Fmuls(at(schema::CarDataAxle(schema::CarData(d).front_axle_address()).unknown_128_address()), two);
    sum = Fmuls(sum, body_mass);
    sum = Fmuls(sum, quarter);
    schema::CarData(d).set_unknown_152(float(Fdivs(load, sum)));
  }
  // The aero factors (1 when 0.0) times the area.
  {
    const bool unset = at(schema::CarData(d).aero_front_address()) == zero;
    double v = at(schema::CarData(d).width_address());
    if (!unset) {
      v = Fmuls(v, at(schema::CarData(d).aero_front_address()));
    }
    schema::CarData(d).set_aero_front(float(Fmuls(v, at(schema::CarData(d).height_address()))));
  }
  {
    const bool unset = at(schema::CarData(d).aero_side_address()) == zero;
    double v = at(schema::CarData(d).length_address());
    if (!unset) {
      v = Fmuls(v, at(schema::CarData(d).aero_side_address()));
    }
    schema::CarData(d).set_aero_side(float(Fmuls(v, at(schema::CarData(d).height_address()))));
  }
  // The 9 reciprocals (+1668 from +1632).
  for (uint32_t i = 0; i < 9; i++) {
    Stfs(schema::CarData(d).shift_rates_address() + 4 * i, Fdivs(one, at(schema::CarData(d).shift_times_address() + 4 * i)));
  }
  // The temporary body's destructor (sub_82106540) is empty.
  return f1;
}

void VehicleConstruct(uint32_t vehicle, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_820B42C8 Vehicle::Vehicle");
  const uint32_t v = vehicle;
  auto zero = [&](uint32_t at) { CopyWord(kZero, at); };
  for (uint32_t at : {12u, 28u, 44u, 76u, 108u, 124u, 140u, 364u, 380u, 396u, 412u}) {
    zero(v + at);
  }
  // Each wheel's (+416 + 368 w) +76, +92, +124, +140, +156, +172, +348.
  for (uint32_t w = 0; w < 4; w++) {
    for (uint32_t at : {76u, 92u, 124u, 140u, 156u, 172u, 348u}) {
      zero(v + (416 + 368 * w + at));
    }
  }
  for (uint32_t at : {1900u, 1916u, 1932u, 1948u, 1964u, 1980u, 2028u, 2044u, 2060u, 2076u,
                      2092u, 2108u, 2476u, 2492u, 2540u}) {
    zero(v + at);
  }
  for (uint32_t i = 0; i < 5; i++) {
    zero(v + (2556 + 16 * i));
  }
  for (uint32_t i = 0; i < 4; i++) {
    zero(v + (3180 + 16 * i));
  }
  for (uint32_t at = 3260; at <= 3372; at += 16) {
    CopyWord(kOne, v + at);
  }
  schema::Vehicle(v).set_body(uint32_t(0));
  schema::Vehicle(v).set_collision_object(uint32_t(0));
  schema::Vehicle(v).set_box(uint32_t(0));
  VehicleReset(v, frame - 240, f1);
  schema::Vehicle(v).set_unknown_2624(uint8_t(0));
}

}  // namespace car_physics
