// The car's setting bytes and tuning applied to its car data
// (docs/functions/car-physics.md): see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"

#include <bit>

namespace car_physics {

using namespace ops;

namespace {

// The game's constants (their values as the game has them).
constexpr uint32_t kStep1716 = 0x82002BF4;      // -10.82551: +1716's step by byte 78
constexpr uint32_t kStep1800 = 0x82002BF0;      // 0.004: +1800's step by byte 78
constexpr uint32_t kRadiusScale = 0x82001FD4;   // 2000: the radii's scale for asin
constexpr uint32_t kSettingStep = 0x82002BC0;   // 0.5: a setting byte's unit
constexpr uint32_t kDegrees = 0x82002BDC;       // 57.295776: asin's radians in degrees
constexpr uint32_t kQuarter = 0x82002968;       // 0.25
constexpr uint32_t kSpringStep = 0x82002BB4;    // 0.02
constexpr uint32_t kPowBase = 0x82002BE8;       // a double, about 1.1487 (2^0.2 as a float)
constexpr uint32_t kHeightScale = 0x82002BE4;   // 0.0002
constexpr uint32_t kStiffStep = 0x82002BBC;     // 0.1
constexpr uint32_t kScale148 = 0x820021F4;      // 5
constexpr uint32_t kCenter = 0x8200277C;        // -10: bytes 74 and 75's offset
constexpr uint32_t kStep140 = 0x82002BA4;       // 0.09
constexpr uint32_t kStep180 = 0x82002204;       // 0.05
constexpr uint32_t kNegativeStep = 0x820029F8;  // -0.1: +260's step below zero
constexpr uint32_t kTable2 = 0x820027A8;        // 1.5: the tuning table's third entry
constexpr uint32_t kTable3 = 0x820021E8;        // 2: its fourth
constexpr uint32_t kTableScale = 0x82002BE0;    // 294

// A setting byte through the stack as the original converts it (lbz,
// extsb, std at `at`, lfd, fcfid, frsp): the integer's float.
double StackIntegerFloat(uint8_t* base, uint32_t at, int64_t value) {
  guest::StoreU32(base, at, uint32_t(uint64_t(value) >> 32));
  guest::StoreU32(base, at + 4, uint32_t(value));
  return double(float(double(value)));
}

// pow's own stores into its caller's linkage area (x at r1 + 16, y at
// + 24, stfd), which c_runtime::Pow doesn't make; then pow.
double PowFromFrame(uint8_t* base, uint32_t frame, double x, double y) {
  const uint64_t xb = std::bit_cast<uint64_t>(x), yb = std::bit_cast<uint64_t>(y);
  guest::StoreU32(base, frame + 24, uint32_t(yb >> 32));
  guest::StoreU32(base, frame + 28, uint32_t(yb));
  guest::StoreU32(base, frame + 16, uint32_t(xb >> 32));
  guest::StoreU32(base, frame + 20, uint32_t(xb));
  return c_runtime::Pow(x, y);
}

// One of the original's clamps: a value below the floor (0.0) is the floor
// (not a NaN).
double AtLeast(double value, double floor) {
  return value < floor ? floor : value;
}

}  // namespace

double CarDataApplySettings(uint8_t* base, uint32_t data, uint32_t car, uint32_t spec,
                            uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C05C8 CarData::ApplySettings");
  const uint32_t d = data, s = car + 16, f = frame;
  auto at = [&](uint32_t offset) { return Lfs(base, d + offset); };
  auto put = [&](uint32_t offset, double value) { Stfs(base, d + offset, value); };
  auto setting_value = [&](uint32_t offset) {
    return int64_t(int8_t(guest::LoadU8(base, s + offset)));
  };
  // The setting byte at car + 16 + `offset`, through the frame's +80.
  auto setting = [&](uint32_t offset) {
    return StackIntegerFloat(base, f + 80, setting_value(offset));
  };

  // With the tuning +1756 above 0: +1716 and +1800 by setting byte 78.
  if (int32_t(guest::LoadU32(base, car + 1756)) > 0) {
    const double v1716 = at(1716);
    const double v1800 = at(1800);
    const double step = Lfs(base, kStep1716);
    const double aero_step = Lfs(base, kStep1800);
    put(1716, Fnmsubs(setting(78), step, v1716));
    const double b78 = setting(78);
    put(1800, Fmuls(Fmadds(b78, aero_step, Lfs(base, kOne)), v1800));
  }

  // +548 and +820 += asin(byte 62 or 63 x 0.5 / (the radius +388 or +660 x
  // 2000)) in degrees; +544, +816 += bytes 64, 65, +540, +812 += them x 0.25.
  const double v388 = at(388);
  const double radius_scale = Lfs(base, kRadiusScale);
  const double r_front = Fmuls(v388, radius_scale);
  const double b62 = setting(62);
  const double unit = Lfs(base, kSettingStep);
  double sine = Fdivs(Fmuls(b62, unit), r_front);
  double angle = double(float(c_runtime::Asin(sine)));
  const double v660 = at(660);
  const double v548 = at(548);
  const double r_rear = Fmuls(v660, radius_scale);
  double degrees = Lfs(base, kDegrees);
  put(548, Fmadds(angle, degrees, v548));
  sine = Fdivs(Fmuls(setting(63), unit), r_rear);
  angle = double(float(c_runtime::Asin(sine)));
  const double v820 = at(820);
  const double v544 = at(544);
  const double v816 = at(816);
  const double v540 = at(540);
  const double quarter = Lfs(base, kQuarter);
  put(820, Fmadds(angle, degrees, v820));
  const double v812 = at(812);
  put(544, Fadds(setting(64), v544));
  put(816, Fadds(setting(65), v816));
  put(540, Fmadds(setting(64), quarter, v540));
  put(812, Fmadds(setting(65), quarter, v812));

  // The springs: +1720 and +1732 (bytes 66, 70), +1740 and +1752 (bytes
  // 67, 71): each += byte x step x (the pair's sum), at least 0.0.
  const double zero = Lfs(base, kZero);
  const double spring_step = Lfs(base, kSpringStep);
  {
    const double a = at(1720);
    const double b = at(1732);
    const double scaled = Fmuls(setting(66), spring_step);
    const double sum = Fadds(a, b);
    double value = Fmadds(scaled, sum, a);
    put(1720, value);
    put(1720, AtLeast(value, zero));
    value = Fmadds(Fmuls(setting(70), spring_step), sum, b);
    put(1732, value);
    put(1732, AtLeast(value, zero));
  }
  {
    const double a = at(1740);
    const double b = at(1752);
    const double scaled = Fmuls(setting(67), spring_step);
    const double sum = Fadds(a, b);
    double value = Fmadds(scaled, sum, a);
    put(1740, value);
    put(1740, AtLeast(value, zero));
    value = Fmadds(Fmuls(setting(71), spring_step), sum, b);
    put(1752, value);
    put(1752, AtLeast(value, zero));
  }

  // The dampers: +1724 and +1728 x pow(base, byte 68), +1744 and +1748 x
  // pow(base, byte 69); each pair at most 1.0 (the first compared before
  // the second is written).
  const uint64_t pow_bits =
      (uint64_t(guest::LoadU32(base, kPowBase)) << 32) | guest::LoadU32(base, kPowBase + 4);
  const double pow_base = std::bit_cast<double>(pow_bits);
  double one = 0.0;
  double f1 = 0.0;
  for (const uint32_t pair : {1724u, 1744u}) {
    const uint32_t byte = pair == 1724 ? 68 : 69;
    f1 = PowFromFrame(base, f, pow_base, setting(byte));
    put(pair, Fmuls(double(float(f1)), at(pair)));
    f1 = PowFromFrame(base, f, pow_base, setting(byte));
    const double first = at(pair);
    one = Lfs(base, kOne);
    const double second = Fmuls(double(float(f1)), at(pair + 4));
    put(pair + 4, second);
    if (first > one) {
      put(pair, one);
    }
    if (second > one) {
      put(pair + 4, one);
    }
  }

  // The ride heights: +1736 -= byte 72 x the spec's +300 (below 0) or +304
  // (above) x scale; +1756 -= byte 73 x +308 or +312 x scale; +84 += the
  // two x the unit.
  const double height_scale = Lfs(base, kHeightScale);
  double front = zero;
  {
    const int64_t b = setting_value(72);
    if (b != 0) {
      const double x = Lfs(base, spec + (b < 0 ? 300 : 304));
      const double fb = StackIntegerFloat(base, f + 80, b);
      front = Fmuls(Fmuls(x, fb), height_scale);
    }
  }
  put(1736, Fsubs(at(1736), front));
  double rear = zero;
  {
    const int64_t b = setting_value(73);
    if (b != 0) {
      const double x = Lfs(base, spec + (b < 0 ? 308 : 312));
      const double fb = StackIntegerFloat(base, f + 80, b);
      rear = Fmuls(Fmuls(x, fb), height_scale);
    }
  }
  const double both = Fadds(rear, front);
  const double v1756 = at(1756);
  const double v84 = at(84);
  put(1756, Fsubs(v1756, rear));
  put(312, unit);
  const double stiff_step = Lfs(base, kStiffStep);
  const double v136 = at(136);
  put(84, Fmadds(both, unit, v84));

  // +132, +136 x (1 + (byte 74 or 75 - center) x step); +148 x a scale;
  // +140, +144 = byte 61 x a step.
  const double v148 = at(148);
  const double v132 = at(132);
  const double b74 = setting(74);
  const double scale148 = Lfs(base, kScale148);
  const uint32_t kind = guest::LoadU32(base, d + 172);
  const double scaled148 = Fmuls(v148, scale148);
  const double center = Lfs(base, kCenter);
  put(132, Fmuls(Fmadds(Fsubs(b74, center), stiff_step, one), v132));
  put(148, scaled148);
  put(136, Fmuls(Fmadds(Fsubs(setting(75), center), stiff_step, one), v136));
  const double b61 = setting(61);
  const double step140 = Lfs(base, kStep140);
  put(140, Fmuls(b61, step140));
  put(144, Fmuls(setting(61), step140));

  // With +172 neither 0 nor 1 and setting byte 76 set: +180 = 1 - byte 77 x
  // a step; with +176 1, +260 = 1 + byte 77 x a step (another below 0).
  if (kind != 0 && kind != 1 && guest::LoadU8(base, s + 76) != 0) {
    const double step = Lfs(base, kStep180);
    const uint32_t v176 = guest::LoadU32(base, d + 176);
    put(180, Fnmsubs(setting(77), step, one));
    if (v176 == 1) {
      const double b77 = setting(77);
      const double factor = b77 < zero ? Lfs(base, kNegativeStep) : stiff_step;
      put(260, Fadds(Fmuls(b77, factor), one));
    }
  }

  // +1536 = car +36; +1432 to +1460 = car +40 to +68 (the gear ratios).
  put(1536, Lfs(base, s + 20));
  for (uint32_t i = 0; i < 8; i++) {
    put(1432 + 4 * i, Lfs(base, s + 24 + 4 * i));
  }

  // With the tuning +1704 above 0: the table (1.0, 1.0, two constants) on
  // the frame's +80, read at +1704 (unchecked), x a scale; the front axle
  // (+604, +596, +600) unless +172 is 1, the rear (+876, +868, +872) unless
  // it's 0, by setting bytes 56, 58 and 57, 59; +1700 picks the factor of
  // +600 and +872 (0: 0.0, 1: the unit, else 1.0).
  const uint32_t index = guest::LoadU32(base, car + 1704);
  const uint32_t pick = guest::LoadU32(base, car + 1700);
  if (int32_t(index) > 0) {
    const double factor = pick == 0 ? zero : pick == 1 ? unit : one;
    Stfs(base, f + 80, one);
    Stfs(base, f + 84, one);
    const uint32_t axles = guest::LoadU32(base, d + 172);
    Stfs(base, f + 88, Lfs(base, kTable2));
    Stfs(base, f + 92, Lfs(base, kTable3));
    const double entry = Lfs(base, (index << 2) + (f + 80));
    const double tuned = Fmuls(entry, Lfs(base, kTableScale));
    const double q = Lfs(base, kQuarter);
    if (axles != 1) {
      put(604, Fmuls(setting(56), tuned));
      put(596, Fmuls(setting(58), q));
      put(600, Fmuls(Fmuls(setting(58), factor), q));
    }
    if (axles != 0) {
      put(876, Fmuls(setting(57), tuned));
      put(868, Fmuls(setting(59), q));
      put(872, Fmuls(Fmuls(setting(59), factor), q));
    }
  }
  return f1;
}

}  // namespace car_physics
