// The car data record (vehicle +272) and the vehicle's constructor
// (docs/functions/car-physics.md): see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "vehicle_accessors/vectors.h"

#include <bit>
#include <cstring>

namespace car_physics {

using namespace ops;
namespace vl = vector_library;

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
