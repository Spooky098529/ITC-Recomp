// The car data record (vehicle +272) and the vehicle's constructor
// (docs/functions/car-physics.md): see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"
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

constexpr uint32_t kHalf = 0x82001BE4;  // 0.5

// memset (sub_825A71D0) with 0, as its adapter does it.
void Clear(uint8_t* base, uint32_t at, uint32_t bytes) {
  guest::ForEachHostPart(base, at, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

double AsDouble(uint32_t word) {
  return double(std::bit_cast<float>(word));
}

}  // namespace

uint32_t CarDataSetDefaults(uint8_t* base, uint32_t data, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AECB8 CarData::SetDefaults");
  const uint32_t d = data, f = frame;
  auto one = [&](uint32_t at) { CopyWord(base, kOne, at); };
  auto zero = [&](uint32_t at) { CopyWord(base, kZero, at); };
  one(d + 0);
  one(d + 4);
  one(d + 8);
  one(f + 80);
  one(d + 12);
  one(f + 84);
  one(f + 88);
  zero(f + 92);
  vl::Vec4Copy(base, d + 32, f + 80);
  one(f + 96);
  one(f + 100);
  one(f + 104);
  zero(f + 108);
  vl::Vec4Copy(base, d + 64, f + 96);
  vl::Vec4Copy(base, d + 80, kZeroVector);
  vl::Vec4Copy(base, d + 112, kZeroVector);
  for (uint32_t at : {128u, 132u, 136u, 264u, 268u}) {
    zero(d + at);
  }
  Clear(base, d + 276, 32);
  one(d + 316);
  zero(d + 324);
  zero(d + 332);
  CopyWord(base, 0x82002950, d + 320);
  Clear(base, d + 336, 272);
  Clear(base, d + 608, 272);
  Clear(base, d + 880, 384);
  zero(d + 1264);
  Clear(base, d + 1268, 436);
  for (uint32_t at : {180u, 1704u, 1808u, 1708u, 1712u, 1716u, 1812u, 1816u, 1720u, 1724u,
                      1728u, 1736u, 1740u, 1744u}) {
    zero(d + at);
  }
  constexpr uint32_t kThird = 0x82001BE8;
  CopyWord(base, kThird, f + 112);
  zero(d + 1756);
  CopyWord(base, kThird, f + 116);
  zero(d + 1748);
  CopyWord(base, kThird, f + 120);
  zero(f + 124);
  vl::Vec4Copy(base, d + 1760, f + 112);
  zero(f + 140);
  CopyWord(base, kHalf, f + 128);
  CopyWord(base, kHalf, f + 132);
  CopyWord(base, kHalf, f + 136);
  vl::Vec4Copy(base, d + 1776, f + 128);
  zero(d + 1792);
  zero(d + 1796);
  // Each axle's +152, +160 and +164.
  for (uint32_t at : {496u, 500u, 488u, 768u, 772u, 760u}) {
    one(d + at);
  }
  return d + 1776;
}

void CarDataConstruct(uint8_t* base, uint32_t data, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AEF80 CarData::CarData");
  const uint32_t d = data;
  for (uint32_t at : {44u, 60u, 76u, 92u, 108u, 124u, 348u, 428u, 588u, 620u, 700u, 860u}) {
    CopyWord(base, kZero, d + at);
  }
  // The four wheel set-up records' +12 and +28 (from +880).
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, kZero, d + 892 + 96 * i);
    CopyWord(base, kZero, d + 908 + 96 * i);
  }
  CopyWord(base, kZero, d + 1772);
  CopyWord(base, kZero, d + 1788);
  CarDataSetDefaults(base, d, frame - 192);
}

void CarDataFillAxle(uint8_t* base, uint32_t data, uint32_t source, uint32_t is_front) {
  ITC_NATIVE_RUNNING("sub_820AEE98 CarData::FillAxle");
  const uint32_t a = data + ((is_front & 0xFF) != 0 ? 336 : 608);
  const uint32_t s = source;
  auto word = [&](uint32_t at) { return guest::LoadU32(base, s + at); };
  // The loads stored as they are keep their bits (lfs then stfs).
  const uint32_t w4 = word(4);
  const double s4 = AsDouble(w4), s0 = AsDouble(word(0));
  const double one = Lfs(base, kOne);
  constexpr uint32_t kTwo = 0x820021E8;
  double f13 = Fdivs(one, s4);
  const double two = Lfs(base, kTwo);
  double f9 = Fmuls(f13, s0);
  f13 = Fmuls(f13, f13);
  f9 = Fmuls(f9, two);
  Stfs(base, a + 16, f9);
  f13 = Fmuls(f13, s0);
  Stfs(base, a + 20, f13);
  guest::StoreU32(base, a + 24, word(8));
  guest::StoreU32(base, a + 28, w4);
  const uint32_t w20 = word(20);
  const double s20 = AsDouble(w20);
  f13 = Fdivs(one, s20);
  const double s16 = AsDouble(word(16));
  f9 = Fmuls(f13, s16);
  f13 = Fmuls(f13, f13);
  Stfs(base, a + 32, Fmuls(f9, two));
  f13 = Fmuls(f13, s16);
  Stfs(base, a + 36, f13);
  const uint32_t w24 = word(24);
  guest::StoreU32(base, a + 44, w20);
  guest::StoreU32(base, a + 40, w24);
  const uint32_t w32 = word(32);
  const double radius = AsDouble(w32);
  guest::StoreU32(base, a + 52, w32);
  Stfs(base, a + 56, Fdivs(one, radius));
  guest::StoreU32(base, a + 60, word(36));
  const uint32_t w40 = word(40);
  guest::StoreU32(base, a + 64, w40);
  guest::StoreU32(base, a + 180, word(44));
  guest::StoreU32(base, a + 184, word(48));
  double f0 = Fmuls(AsDouble(w40), radius);
  guest::StoreU32(base, a + 188, word(52));
  guest::StoreU32(base, a + 192, word(56));
  f0 = Fmuls(f0, radius);
  Stfs(base, a + 200, f0);
  guest::StoreU32(base, a + 140, word(60));
  guest::StoreU32(base, a + 144, word(64));
}

double CarDataConvertUnits(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_820AF018 CarData::ConvertUnits");
  const uint32_t d = data;
  auto at = [&](uint32_t offset) { return Lfs(base, d + offset); };
  const double rpm = at(1272);
  const double v316 = at(316);
  const double v320 = at(320);
  double f5 = at(324);
  double f3 = at(544);
  const double one = Lfs(base, kOne);
  const double f10 = Fdivs(one, rpm);
  double f1 = at(812);
  double f30 = at(820);
  double f28 = at(1712);
  double f27 = at(1812);
  const double zero = Lfs(base, kZero);
  const double f26 = at(1816);
  double f9 = zero;
  const double f25 = at(1628);
  const double scale_a = Lfs(base, 0x8200295C);
  double f7 = Fmuls(v316, scale_a);
  const double v1264 = at(1264);
  const double f8a = Fmuls(v320, scale_a);
  const double v128 = at(128);
  const double f4 = Fdivs(one, v1264);
  const double v540 = at(540);
  const double f6a = Fdivs(one, v128);
  Stfs(base, d + 128, f6a);
  const double degrees = Lfs(base, 0x82002948);
  const double f2 = Fmuls(v540, degrees);
  const double f31 = Fmuls(at(548), degrees);
  const double f29 = Fmuls(at(816), degrees);
  Stfs(base, d + 1264, f4);
  f5 = Fmuls(f5, degrees);
  Stfs(base, d + 540, f2);
  const double scale_b = Lfs(base, 0x82002958);
  f7 = Fmuls(f7, scale_b);
  Stfs(base, d + 324, f5);
  double f12 = Fmuls(f8a, scale_b);
  double f8 = at(1716);
  f3 = Fmuls(f3, degrees);
  Stfs(base, d + 548, f31);
  f1 = Fmuls(f1, degrees);
  Stfs(base, d + 544, f3);
  f30 = Fmuls(f30, degrees);
  const double scale_c = Lfs(base, 0x82002954);
  f28 = Fmuls(f28, scale_c);
  Stfs(base, d + 812, f1);
  f8 = Fmuls(f8, scale_c);
  Stfs(base, d + 816, f29);
  f27 = Fmuls(f27, scale_c);
  Stfs(base, d + 820, f30);
  const double f0a = Fmuls(f26, scale_c);
  Stfs(base, d + 1712, f28);
  const double f6 = Fmuls(f25, f10);
  Stfs(base, d + 1716, f8);
  f7 = Fdivs(one, f7);
  Stfs(base, d + 1812, f27);
  f12 = Fdivs(one, f12);
  Stfs(base, d + 316, f7);
  Stfs(base, d + 320, f12);
  Stfs(base, d + 1816, f0a);
  Stfs(base, d + 1628, f6);
  // The torque table's sum.
  for (uint32_t i = 0; i < 32; i++) {
    f9 = Fadds(at(1292 + 4 * i), f9);
  }
  double f0 = Fmuls(at(1280), f10);
  const double v1420 = at(1420);
  Stfs(base, d + 1284, f0);
  double torque = Fmuls(v1420, Lfs(base, 0x82600CC8));
  torque = Fmuls(torque, f9);
  Stfs(base, d + 1548, torque);
  // Clamped to [0, 1] (bge and ble: a NaN is kept).
  if (f0 < zero) {
    f0 = zero;
  } else if (f0 > one) {
    f0 = one;
  }
  Stfs(base, d + 1284, f0);
  const double v1616 = at(1616);
  const double v184 = at(184);
  Stfs(base, d + 1616, Fdivs(one, v1616));
  Stfs(base, d + 184, Fdivs(one, v184));
  return f1;
}

namespace {

// The 16 bytes of a quaternion (ld and std, two doublewords: the bits).
void Copy16(uint8_t* base, uint32_t from, uint32_t to) {
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyWord(base, from + i, to + i);
  }
}

// An axle's point (+80) moved by its camber a: y -= (|+60 sin a x 0.5| +
// cos a x radius), z -= sin a x radius (the original's steps; `radius_at`
// the radius it reads, +52).
void MoveAxlePoint(uint8_t* base, uint32_t axle, double c, double s, double half,
                   uint32_t radius_at) {
  const double width = Lfs(base, axle + 60);
  const double cr = double(float(c));
  const double radius = Lfs(base, radius_at);
  const double y = Lfs(base, axle + 84);
  double t = double(float(s));
  t = Fmuls(width, t);
  t = Fmuls(t, half);
  t = Fabs(t);
  t = Fmadds(cr, radius, t);
  Stfs(base, axle + 84, Fsubs(y, t));
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

double CarDataDerive(uint8_t* base, uint32_t data, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820AF1C0 CarData::Derive");
  const uint32_t d = data, f = frame;
  auto at = [&](uint32_t offset) { return Lfs(base, d + offset); };
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
  const uint32_t front = d + 336, rear = d + 608;

  // The radii (+52 of each axle) of 0.0.
  const double zero = Lfs(base, kZero);
  const double v388 = at(388);
  const double a_front = at(540);
  const double a_rear = at(812);
  const double radius = Lfs(base, kRadius);
  if (v388 == zero) {
    Stfs(base, d + 388, radius);
  }
  if (at(660) == zero) {
    Stfs(base, d + 660, radius);
  }

  // The front axle's point: its position moved by the camber.
  vl::Vec4Copy(base, d + 416, front);
  const double c_front = c_runtime::Cos(a_front);
  const double s_front = c_runtime::Sin(a_front);
  const double half = Lfs(base, kHalf);
  MoveAxlePoint(base, front, c_front, s_front, half, d + 388);
  {
    const double s = double(float(s_front));
    const double z = at(424);
    Stfs(base, d + 424, Fnmsubs(s, at(388), z));
  }
  // The rear's.
  vl::Vec4Copy(base, d + 688, rear);
  const double c_rear = c_runtime::Cos(a_rear);
  const double s_rear = c_runtime::Sin(a_rear);
  MoveAxlePoint(base, rear, c_rear, s_rear, half, d + 660);
  const double one = Lfs(base, kOne);
  {
    const double r_front = at(388);
    const double r_rear = at(660);
    const double z = at(696);
    Stfs(base, d + 392, Fdivs(one, r_front));
    const double s = double(float(s_rear));
    Stfs(base, d + 664, Fdivs(one, r_rear));
    Stfs(base, d + 696, Fnmsubs(s, r_rear, z));
  }

  // The four wheel set-up records (+880 + 96i): the position, the point,
  // half the track, sin a, cos a and the turn's quaternion; the second of
  // each pair mirrored.
  vl::Vec4Copy(base, d + 976, front);
  vl::Vec4Copy(base, d + 880, d + 976);
  vl::Vec4Copy(base, d + 992, d + 416);
  vl::Vec4Copy(base, d + 896, d + 992);
  {
    const double s = double(float(s_front));
    const double r = at(388);
    const double width = at(396);
    Stfs(base, d + 912, Fnmsubs(s, r, width));
  }
  Stfs(base, d + 916, s_front);
  Stfs(base, d + 920, c_front);
  mm::QuaternionFromAxisAngle(base, f + 304, kAxis, a_front, f - 32);
  const double neg_front = Fneg(a_front);
  const double mirror = Lfs(base, kMirror);
  {
    const double z0 = at(888);
    const double z1 = at(904);
    const double track = at(912);
    Stfs(base, d + 1008, Fneg(track));
    Copy16(base, f + 304, d + 928);
    Stfs(base, d + 888, Fmuls(z0, mirror));
    Stfs(base, d + 904, Fmuls(z1, mirror));
  }
  Stfs(base, d + 1012, c_runtime::Sin(neg_front));
  Stfs(base, d + 1016, c_runtime::Cos(neg_front));
  mm::QuaternionFromAxisAngle(base, f + 256, kAxis, neg_front, f - 32);
  Copy16(base, f + 256, d + 1024);
  vl::Vec4Copy(base, d + 1168, rear);
  vl::Vec4Copy(base, d + 1072, d + 1168);
  vl::Vec4Copy(base, d + 1184, d + 688);
  vl::Vec4Copy(base, d + 1088, d + 1184);
  {
    // The front's radius (+388), as the original reads it.
    const double s = double(float(s_rear));
    const double r = at(388);
    const double width = at(668);
    Stfs(base, d + 1104, Fnmsubs(s, r, width));
  }
  Stfs(base, d + 1108, s_rear);
  Stfs(base, d + 1112, c_rear);
  mm::QuaternionFromAxisAngle(base, f + 288, kAxis, a_rear, f - 32);
  const double neg_rear = Fneg(a_rear);
  {
    const double z0 = Fmuls(at(1080), mirror);
    const double track = at(1104);
    const double z1 = Fmuls(at(1096), mirror);
    Stfs(base, d + 1080, z0);
    Stfs(base, d + 1096, z1);
    Stfs(base, d + 1200, Fneg(track));
    Copy16(base, f + 288, d + 1120);
  }
  Stfs(base, d + 1204, c_runtime::Sin(neg_rear));
  Stfs(base, d + 1208, c_runtime::Cos(neg_rear));
  mm::QuaternionFromAxisAngle(base, f + 272, kAxis, neg_rear, f - 32);
  Copy16(base, f + 272, d + 1216);

  // The center of mass (+112): the mean of the four points plus the
  // offset +80.
  const uint32_t center = d + 112;
  vl::Vec4Copy(base, center, d + 896);
  vl::Vec3AddTo(base, center, d + 992);
  vl::Vec3AddTo(base, center, d + 1088);
  vl::Vec3AddTo(base, center, d + 1184);
  const double quarter = Lfs(base, kQuarter);
  vl::Vec4ScaleBy(base, center, quarter);
  vl::Vec3AddTo(base, center, d + 80);

  // The total mass, the inertia (+64 to +72), the half tracks and the
  // distances from the center.
  const double two = Lfs(base, kTwo);
  double mass;
  double dx_front, dx_rear, wheelbase;
  {
    const double m_front = at(400);
    const double z_front = at(904);
    const double m_rear = at(672);
    double track_front = at(1000);
    const double wheels = Fadds(m_rear, m_front);
    track_front = Fsubs(z_front, track_front);
    const double z_rear = at(1192);
    double track_rear = at(1096);
    track_rear = Fsubs(track_rear, z_rear);
    const double body = at(0);
    const double cx = at(112);
    const double x_rear = at(608);
    const double x_front = at(336);
    const double i0 = at(32);
    dx_front = Fsubs(x_front, cx);
    const double i1 = at(36);
    mass = Fmadds(wheels, two, body);
    const double i2 = at(40);
    track_front = Fabs(track_front);
    dx_rear = Fsubs(x_rear, cx);
    track_rear = Fabs(track_rear);
    wheelbase = Fsubs(x_front, x_rear);
    const double m0 = Fmuls(i0, mass);
    Stfs(base, d + 912, Fmuls(track_front, half));
    const double third = Lfs(base, kThird);
    const double m1 = Fmuls(i1, mass);
    const double m2 = Fmuls(i2, mass);
    Stfs(base, d + 1008, Fmuls(track_front, third));
    Stfs(base, d + 1200, Fmuls(track_rear, third));
    Stfs(base, d + 1104, Fmuls(track_rear, half));
    const double inertia = Lfs(base, kInertia);
    Stfs(base, d + 64, Fmuls(m0, inertia));
    Stfs(base, d + 68, Fmuls(m1, inertia));
    Stfs(base, d + 72, Fmuls(m2, inertia));
  }
  {
    const double fallback = Lfs(base, kDistance);
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
    Stfs(base, f + 80, Fmsubs(wheelbase, half, at(80)));
    const double split = at(312);
    const double base_sq = Fmuls(wheelbase, wheelbase);
    double rest = Fsubs(one, split);
    const double v132 = at(132);
    double split2 = Fmuls(split, two);
    Stfs(base, d + 824, dx_rear);
    Stfs(base, d + 552, dx_front);
    const uint32_t w148 = guest::LoadU32(base, d + 148);
    const double v136 = at(136);
    guest::StoreU32(base, d + 272, w148);
    const double h_rear = at(864);
    double share_f = Fmuls(inv_base, dx_rear);
    const double h_front = at(592);
    double share_r = Fmuls(inv_base, dx_front);
    const double h84 = at(84);
    for (uint32_t o : {92u, 172u, 236u, 140u, 220u, 88u}) {
      Stfs(base, f + o, zero);
    }
    Stfs(base, f + 160, wheelbase);
    Stfs(base, f + 168, zero);
    share_f = Fabs(share_f);
    Stfs(base, d + 568, share_f);
    share_r = Fabs(share_r);
    Stfs(base, d + 840, share_r);
    rest = Fmuls(rest, two);
    Stfs(base, d + 264, Fmuls(split2, v132));
    Stfs(base, f + 84, Fsubs(h84, h_front));
    const double load_f = Fmuls(share_f, mass);
    const double load_r = Fmuls(share_r, mass);
    const double g = Lfs(base, kGravity);
    Stfs(base, d + 268, Fmuls(rest, v136));
    const double dh = Fsubs(h_rear, h_front);
    Stfs(base, f + 164, dh);
    double h = Fmuls(h_front, share_f);
    Stfs(base, d + 556, Fmuls(load_f, g));
    Stfs(base, d + 828, Fmuls(load_r, g));
    double len = Fmadds(dh, dh, base_sq);
    h = Fmadds(share_r, h_rear, h);
    len = double(float(std::sqrt(len)));
    f30 = Fsubs(h84, h);
    const double k = Fdivs(f30, len);
    Stfs(base, d + 16, Fmuls(k, dh));
    Stfs(base, d + 20, Fmuls(k, wheelbase));
  }
  vl::Vec4Copy(base, f + 224, f + 80);
  vl::Vec3NormalizeSelf(base, f + 224);
  vl::Vec4Copy(base, f + 128, f + 160);
  vl::Vec3NormalizeSelf(base, f + 128);
  const double dot = double(vl::Vec3DotOf(base, f + 80, f + 128));
  vl::Vec4ScaledFrom(base, f + 320, f + 128, dot);
  vl::Vec3Difference(base, f + 336, f + 320, f + 80);
  vl::Vec4Copy(base, f + 208, f + 336);
  {
    const double y = Lfs(base, f + 212);
    const uint32_t x = guest::LoadU32(base, f + 208);
    guest::StoreU32(base, d + 16, x);
    Stfs(base, d + 20, Fneg(y));
  }
  vl::Vec4Copy(base, d + 96, center);
  Stfs(base, d + 24, f30);

  // The temporary rigid body and the effective masses at the wheels.
  const uint32_t body = f + 352;
  RigidBodyConstruct(base, body, f - 96);
  RigidBodyReset(base, body, f - 176);
  va::RigidBodySetMass(base, body, at(0));
  RigidBodySetInertia(base, body, d + 64, f - 320);
  va::RigidBodySetCenterOfMass(base, body, d + 96);
  CopyWord(base, front, f + 96);
  CopyWord(base, rear, f + 112);
  for (uint32_t o : {100u, 104u, 108u, 116u, 120u, 124u}) {
    Stfs(base, f + o, zero);
  }
  RigidBodyEffectiveMassAt(base, body, d + 576, f + 96, f - 272);
  RigidBodyEffectiveMassAt(base, body, d + 848, f + 112, f - 272);
  CopyWord(base, front, f + 144);
  {
    const uint32_t z = guest::LoadU32(base, d + 344);
    guest::StoreU32(base, f + 152, z);
    const uint32_t y = guest::LoadU32(base, d + 340);
    for (uint32_t o : {204u, 252u, 156u}) {
      Stfs(base, f + o, zero);
    }
    guest::StoreU32(base, f + 148, y);
  }
  RigidBodyEffectiveMassAt(base, body, f + 240, f + 144, f - 272);
  CopyWord(base, rear, f + 176);
  CopyWord(base, d + 340, f + 180);
  CopyWord(base, d + 616, f + 184);
  Stfs(base, f + 188, zero);
  RigidBodyEffectiveMassAt(base, body, f + 192, f + 176, f - 272);

  // The suspension sub-records (axle +96: d + 432 and d + 704).
  double f1;
  {
    double spring = at(1720);
    const uint32_t w_front = guest::LoadU32(base, f + 244);
    const uint32_t w_rear = guest::LoadU32(base, f + 196);
    const double m_front = AsDouble(w_front);
    const double m_rear = AsDouble(w_rear);
    const double inv_front = Fdivs(one, m_front);
    const uint32_t w556 = guest::LoadU32(base, d + 556);
    const double inv_rear = Fdivs(one, m_rear);
    guest::StoreU32(base, d + 432, w_front);
    const double k = Lfs(base, kSpring);
    spring = Fmuls(spring, k);
    const uint32_t w828 = guest::LoadU32(base, d + 828);
    guest::StoreU32(base, d + 704, w_rear);
    Stfs(base, d + 436, inv_front);
    Stfs(base, d + 708, inv_rear);
    guest::StoreU32(base, d + 464, w556);
    double km = Fmuls(spring, m_front);
    guest::StoreU32(base, d + 736, w828);
    Stfs(base, d + 440, spring);
    const double damping = Lfs(base, kDamping);
    f1 = double(vl::Sqrtf(Fmuls(km, damping)));
    double spring_rear = Fmuls(at(1740), k);
    Stfs(base, d + 448, Fmuls(at(1724), f1));
    const double m_rear2 = Lfs(base, f + 196);
    const double d1728 = at(1728);
    const double d1732 = at(1732);
    const double c1 = Fmuls(d1728, f1);
    const double c2 = Fmuls(d1732, k);
    Stfs(base, d + 452, c1);
    Stfs(base, d + 468, c2);
    Stfs(base, d + 712, spring_rear);
    km = Fmuls(spring_rear, m_rear2);
    f1 = double(vl::Sqrtf(Fmuls(km, damping)));
    double r9 = at(1752);
    const double s440 = at(440);
    r9 = Fmuls(r9, k);
    const double s464 = at(464);
    Stfs(base, d + 740, r9);
    const double ratio = Fdivs(s464, s440);
    guest::StoreU32(base, d + 444, guest::LoadU32(base, d + 440));
    const double d1744 = at(1744);
    const double d1748 = at(1748);
    const double c3 = Fmuls(d1744, f1);
    const double preload = at(460);
    Stfs(base, d + 720, c3);
    const double c4 = Fmuls(d1748, f1);
    const uint32_t w712 = guest::LoadU32(base, d + 712);
    const double s712 = AsDouble(w712);
    Stfs(base, d + 724, c4);
    guest::StoreU32(base, d + 716, w712);
    const double clamped = ClampToZero(preload, Fneg(ratio), zero);
    const double ratio_rear = Fdivs(at(736), s712);
    Stfs(base, d + 460, clamped);
    const double preload_rear = at(732);
    Stfs(base, d + 732, ClampToZero(preload_rear, Fneg(ratio_rear), zero));
  }
  {
    const double h84 = at(84);
    const double h_front = at(592);
    const double t_front = Fsubs(h_front, h84);
    const double h_rear = at(864);
    const double t_rear = Fsubs(h_rear, h84);
    Stfs(base, d + 472, t_front);
    Stfs(base, d + 744, t_rear);
    const double v132 = at(132);
    double sum = Fadds(at(136), v132);
    const double body_mass = at(0);
    const double load = Fmuls(at(464), two);
    sum = Fmuls(sum, body_mass);
    sum = Fmuls(sum, quarter);
    Stfs(base, d + 152, Fdivs(load, sum));
  }
  // The aero factors (1 when 0.0) times the area.
  {
    const bool unset = at(1800) == zero;
    double v = at(56);
    if (!unset) {
      v = Fmuls(v, at(1800));
    }
    Stfs(base, d + 1800, Fmuls(v, at(52)));
  }
  {
    const bool unset = at(1804) == zero;
    double v = at(48);
    if (!unset) {
      v = Fmuls(v, at(1804));
    }
    Stfs(base, d + 1804, Fmuls(v, at(52)));
  }
  // The 9 reciprocals (+1668 from +1632).
  for (uint32_t i = 0; i < 9; i++) {
    Stfs(base, d + 1668 + 4 * i, Fdivs(one, at(1632 + 4 * i)));
  }
  // The temporary body's destructor (sub_82106540) is empty.
  return f1;
}

void VehicleConstruct(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_820B42C8 Vehicle::Vehicle");
  const uint32_t v = vehicle;
  auto zero = [&](uint32_t at) { CopyWord(base, kZero, v + at); };
  for (uint32_t at : {12u, 28u, 44u, 76u, 108u, 124u, 140u, 364u, 380u, 396u, 412u}) {
    zero(at);
  }
  // Each wheel's (+416 + 368 w) +76, +92, +124, +140, +156, +172, +348.
  for (uint32_t w = 0; w < 4; w++) {
    for (uint32_t at : {76u, 92u, 124u, 140u, 156u, 172u, 348u}) {
      zero(416 + 368 * w + at);
    }
  }
  for (uint32_t at : {1900u, 1916u, 1932u, 1948u, 1964u, 1980u, 2028u, 2044u, 2060u, 2076u,
                      2092u, 2108u, 2476u, 2492u, 2540u}) {
    zero(at);
  }
  for (uint32_t i = 0; i < 5; i++) {
    zero(2556 + 16 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    zero(3180 + 16 * i);
  }
  for (uint32_t at = 3260; at <= 3372; at += 16) {
    CopyWord(base, kOne, v + at);
  }
  guest::StoreU32(base, v + 336, 0);
  guest::StoreU32(base, v + 340, 0);
  guest::StoreU32(base, v + 344, 0);
  VehicleReset(base, v, frame - 240, f1);
  guest::StoreU8(base, v + 2624, 0);
}

}  // namespace car_physics
