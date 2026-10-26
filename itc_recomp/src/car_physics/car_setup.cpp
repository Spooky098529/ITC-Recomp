// The car's setting bytes and tuning applied to its car data
// (docs/functions/car-physics.md): see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "collision/collision.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <initializer_list>

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

namespace {

// Vehicle::ApplyTuning's constants (their values as the game has them).
constexpr uint32_t k0_002 = 0x82002BFC;
constexpr uint32_t k0_005 = 0x82002A08;
constexpr uint32_t k0_01 = 0x8200222C;
constexpr uint32_t k0_015 = 0x82002A18;
constexpr uint32_t k0_02 = 0x82002BB4;
constexpr uint32_t k0_025 = 0x82002C44;
constexpr uint32_t k0_03 = 0x82002C48;
constexpr uint32_t k0_04 = 0x8200256C;
constexpr uint32_t k0_05 = 0x82002204;
constexpr uint32_t k0_06 = 0x82002804;
constexpr uint32_t k0_08 = 0x82002A1C;
constexpr uint32_t k0_1 = 0x82002BBC;
constexpr uint32_t k0_15 = 0x82002C4C;
constexpr uint32_t k0_2 = 0x82002208;
constexpr uint32_t k0_25 = 0x82002968;
constexpr uint32_t k0_3 = 0x8200296C;
constexpr uint32_t k0_35 = 0x82002C50;
constexpr uint32_t k0_4 = 0x82002A10;
constexpr uint32_t k0_46 = 0x82002C8C;
constexpr uint32_t k0_48 = 0x82002C90;
constexpr uint32_t k0_5 = 0x82002BC0;
constexpr uint32_t k0_51 = 0x82002C94;
constexpr uint32_t k0_55 = 0x82002C98;
constexpr uint32_t k0_6 = 0x82002A14;
constexpr uint32_t k0_65 = 0x82002A04;
constexpr uint32_t k0_66 = 0x82002C9C;
constexpr uint32_t k0_7 = 0x82002A3C;
constexpr uint32_t k0_73 = 0x82002CA0;
constexpr uint32_t k0_75 = 0x8200295C;
constexpr uint32_t k0_8 = 0x82001070;
constexpr uint32_t k0_81 = 0x82002CA4;
constexpr uint32_t k0_83 = 0x82002C14;
constexpr uint32_t k0_85 = 0x82002C58;
constexpr uint32_t k0_875 = 0x82002C18;
constexpr uint32_t k0_9 = 0x82002A2C;
constexpr uint32_t k0_95 = 0x82002A34;
constexpr uint32_t k0_975 = 0x82002BF8;
constexpr uint32_t k1_005 = 0x82002C64;
constexpr uint32_t k1_01 = 0x82002B90;
constexpr uint32_t k1_015 = 0x82002C68;
constexpr uint32_t k1_02 = 0x82002C74;
constexpr uint32_t k1_025 = 0x82002C7C;
constexpr uint32_t k1_03 = 0x82002C80;
constexpr uint32_t k1_035 = 0x82002C54;
constexpr uint32_t k1_04 = 0x82002C60;
constexpr uint32_t k1_05 = 0x820029DC;
constexpr uint32_t k1_06 = 0x82002C5C;
constexpr uint32_t k1_08 = 0x82002C2C;
constexpr uint32_t k1_1 = 0x82002CB0;
constexpr uint32_t k1_15 = 0x82002CB4;
constexpr uint32_t k1_18 = 0x82002C28;
constexpr uint32_t k1_2 = 0x820029D4;
constexpr uint32_t k1_25 = 0x82002A40;
constexpr uint32_t k1_28 = 0x82002C24;
constexpr uint32_t k1_3 = 0x820029F0;
constexpr uint32_t k1_38 = 0x82002C20;
constexpr uint32_t k1_4 = 0x82002CB8;
constexpr uint32_t k1_5 = 0x820027A8;
constexpr uint32_t k1_6 = 0x82002C40;
constexpr uint32_t k1_65 = 0x82002C1C;
constexpr uint32_t k2 = 0x820021E8;
constexpr uint32_t k3 = 0x820021E4;
constexpr uint32_t k4 = 0x820021F8;
constexpr uint32_t k4_5 = 0x82002570;
constexpr uint32_t k5 = 0x820021F4;
constexpr uint32_t k6 = 0x82002CAC;
constexpr uint32_t k8 = 0x82000C34;
constexpr uint32_t k10 = 0x82001FD8;
constexpr uint32_t k11 = 0x82002C84;
constexpr uint32_t k12 = 0x82002C78;
constexpr uint32_t k13 = 0x82002C70;
constexpr uint32_t k14 = 0x82002C6C;
constexpr uint32_t k15 = 0x820029D0;
constexpr uint32_t k17 = 0x82002CA8;
constexpr uint32_t k20 = 0x82002568;
constexpr uint32_t k30 = 0x82002BA0;
constexpr uint32_t k75 = 0x82002C3C;
constexpr uint32_t k100 = 0x82002A0C;
constexpr uint32_t k120 = 0x82002C38;
constexpr uint32_t k150 = 0x82001FB8;
constexpr uint32_t k180 = 0x82002C34;
constexpr uint32_t k200 = 0x820027A4;
constexpr uint32_t k240 = 0x82002C30;
constexpr uint32_t kMinus0_005 = 0x82002C10;
constexpr uint32_t kMinus0_01 = 0x82002C0C;
constexpr uint32_t kMinus0_012 = 0x82002C08;
constexpr uint32_t kMinus0_015 = 0x82002C04;
constexpr uint32_t kMinus0_02 = 0x82002C00;
constexpr uint32_t kMinus0_5 = 0x82001BE8;
constexpr uint32_t k2e_5 = 0x82002C88;      // 0.00002
constexpr uint32_t kDragStep = 0x82002BD0;  // 1082.551: an aero part's +8 and +12's scale

// One store of a table the original builds on its stack: the constant's
// word at frame + `offset`.
struct TableStore {
  uint32_t offset;
  uint32_t constant;
};

// The aero parts' memcpy: the record at car + 3636 + 168 or + 204 (36
// bytes) to frame + 208.
constexpr uint32_t kAeroParts = 3636, kCopyBytes = 36;

}  // namespace

bool VehicleApplyTuningCopyFits(uint32_t car, uint32_t frame) {
  // Both sources (car + 3804 to + 3876) checked; the destination on the stack.
  const uint32_t destination = frame + 208, source = car + kAeroParts + 168;
  return destination < 0xA0000000u && c_runtime::InOneView(destination, kCopyBytes) &&
         c_runtime::InOneView(source, 72) && !c_runtime::MayShare(destination, kCopyBytes, source, 72);
}

uint64_t VehicleApplyTuning(uint8_t* base, uint32_t data, uint32_t car, uint32_t frame,
                            double* f1) {
  ITC_NATIVE_RUNNING("sub_820C0D28 Vehicle::ApplyTuning");
  const uint32_t d = data, f = frame, levels = car + 1648, parts = car + kAeroParts;
  auto at = [&](uint32_t offset) { return Lfs(base, d + offset); };
  auto put = [&](uint32_t offset, double value) { Stfs(base, d + offset, value); };
  auto k = [&](uint32_t constant) { return Lfs(base, constant); };
  auto level = [&](uint32_t offset) { return int32_t(guest::LoadU32(base, levels + offset)); };
  // A level x 4 (rlwinm: the low 32 bits).
  auto index = [](int32_t value) { return uint32_t(value) << 2; };
  // lfsx: the table at frame + `table`, read at `index4` (unchecked).
  auto entry = [&](uint32_t table, uint32_t index4) { return Lfs(base, index4 + (f + table)); };
  auto fill = [&](std::initializer_list<TableStore> stores) {
    for (const TableStore& s : stores) {
      CopyWord(base, s.constant, f + s.offset);
    }
  };
  const double one = k(kOne);
  const double zero = k(kZero);

  const uint32_t spec = vehicle_accessors::ObjectRecord1888(car);
  const uint32_t level0 = collision::WordAt8(base, levels);  // car + 1656: the engine stage
  uint64_t r3 = level0;  // WordAt8's
  const double v1420 = at(1420);
  // The mass added at the front (f29) and the rear (f30), the torque curve's
  // factor (f16) and the ride height's change (f17).
  double front = zero;
  double rear = zero;
  double curve = one;
  double height = zero;

  // The engine stage (car + 1656 above 0): the cylinder count by block 2's
  // +68, NA (block 2 +69 0: the table at +160, 17) or turbo (+208, 20).
  if (int32_t(level0) > 0) {
    const uint32_t code = guest::LoadU8(base, spec + 68);
    fill({{160, kOne}, {164, k1_05}, {168, k1_05}, {172, k1_1},  {176, k1_1},
          {180, k1_1}, {184, k1_15}, {188, k1_15}, {192, k1_3},  {196, k1_4},
          {208, kOne}, {212, k1_05}, {216, k1_05}, {220, k1_1},  {224, k1_1},
          {228, k1_15}, {232, k1_15}, {236, k1_2}, {240, k1_2}, {244, k1_25}});
    double cylinders;
    switch (code) {
      case 0: cylinders = k(k3); break;
      case 2: case 4: case 5: cylinders = k(k6); break;
      case 6: cylinders = k(k8); break;
      case 7: cylinders = k(k2); break;
      default: cylinders = k(k4); break;  // 1, 3 and the rest
    }
    const uint32_t i = index(int32_t(level0));
    double factor, scale;
    if (guest::LoadU8(base, spec + 69) != 0) {
      factor = entry(208, i);
      scale = k(k20);
    } else {
      scale = k(k17);
      factor = entry(160, i);
    }
    double x = Fdivs(at(1288), cylinders);
    const double mass = at(0);
    factor = Fsubs(factor, one);
    fill({{208, kZero}, {212, kZero}, {220, k100}, {224, k100}, {228, k100},
          {216, kZero}, {232, k150}, {236, k150}, {240, k200}, {244, k200}});
    x = Fmuls(x, mass);
    const double idle = entry(208, i);
    fill({{208, kOne}, {216, k0_81}, {212, k0_9}, {228, k0_6}, {220, k0_73},
          {224, k0_66}, {232, k0_55}, {236, k0_51}, {240, k0_48}, {244, k0_46}});
    const double scaled = Fmuls(x, scale);
    const double inertia = entry(208, i);
    const double weight = Fmuls(scaled, k(k2e_5));
    put(1548, Fmuls(inertia, at(1548)));
    front = Fmuls(weight, factor);
    put(1628, Fadds(idle, at(1628)));
  }

  // +1676 (unchecked: the table at +96 is read at any level) and +1672.
  {
    const int32_t l28 = level(28);
    const int32_t l24 = level(24);
    fill({{96, kZero}, {100, k10}, {104, k11}, {108, k12}, {112, k13}, {116, k14}, {120, k15}});
    rear = entry(96, index(l28));
    r3 = 0xFFFFFFFF82000000ull;  // lis r3 (a constant's base, sign-extended)
    if (l24 > 0) {
      const bool turbo = guest::LoadU8(base, spec + 69) != 0;
      fill({{128, kOne}, {132, k1_005}, {136, k1_01}, {140, k1_015}, {144, k1_02},
            {148, k1_025}, {152, k1_03}, {96, kOne},  {112, k1_04},  {100, k1_01},
            {104, k1_02}, {108, k1_03},  {120, k1_06}, {116, k1_05}, {160, kOne},
            {164, k0_8},  {168, k0_75},  {172, k0_7},  {180, k0_6},  {184, k0_5},
            {176, k0_65}});
      const uint32_t i = index(l24);
      put(1420, Fmuls(v1420, entry(turbo ? 96 : 128, i)));
      const double mass = at(0);
      const double l24_float = StackIntegerFloat(base, f + 80, int64_t(l24));
      const double m = Fmuls(Fsubs(entry(160, i), one), mass);
      rear = Fmadds(m, k(k0_04), rear);
      curve = Fmadds(l24_float, k(k0_05), one);
    }
  }

  // +1664: the torque +1420 and the front's mass.
  if (const int32_t l16 = level(16); l16 > 0) {
    const bool turbo = guest::LoadU8(base, spec + 69) != 0;
    fill({{104, k1_01}, {164, k1_01}, {96, kOne},   {100, k1_005}, {108, k1_015},
          {112, k1_02}, {116, k1_025}, {120, k1_03}, {160, kOne},   {168, k1_015},
          {172, k1_02}, {176, k1_025}, {180, k1_03}, {184, k1_035}, {208, kOne},
          {212, k0_8},  {216, k0_8},   {220, k0_85}, {224, k0_85},  {228, k0_9},
          {232, k0_9},  {128, kOne},   {132, k0_8},  {136, k0_8},   {140, k0_9},
          {144, k0_9},  {148, k0_9},   {152, k0_9}});
    const uint32_t i = index(l16);
    double scale, torque, m;
    if (turbo) {
      scale = k(k4_5);
      torque = entry(160, i);
      m = entry(128, i);
    } else {
      scale = k(k3);
      torque = entry(96, i);
      m = entry(208, i);
    }
    m = Fsubs(m, one);
    const double v = at(1420);
    const double mass = at(0);
    put(1420, Fmuls(v, torque));
    front = Fmadds(Fmuls(Fmuls(m, mass), scale), k(k0_01), front);
  }

  // +1680: the shift times +1636 to +1664 (the table at +160, at least 0.1)
  // and the mass split by the center of mass.
  if (const int32_t l32 = level(32); l32 > 0) {
    const uint32_t i = index(l32);
    const double wheelbase = at(4);
    const double x80 = at(80);
    const double ahead = Fmadds(wheelbase, k(k0_5), x80);
    fill({{208, kOne}, {212, k0_8}, {216, k0_8}, {220, k0_85}, {224, k0_85},
          {228, k0_9}, {232, k0_9}, {160, k0_4}});
    const double share = Fdivs(ahead, wheelbase);
    const double mass = at(0);
    const double floor = k(k0_1);
    fill({{184, k0_1}, {164, k0_35}, {168, k0_3}, {172, k0_25}, {176, k0_2}, {180, k0_15}});
    const double other = Fsubs(one, share);
    const double m = Fsubs(entry(208, i), one);
    const double weight = k(k0_08);
    rear = Fmadds(Fmuls(Fmuls(m, mass), share), weight, rear);
    front = Fmadds(Fmuls(Fmuls(other, m), mass), weight, front);
    for (uint32_t gear = 1; gear < 9; gear++) {
      const double v = entry(160, i);
      put(1632 + 4 * gear, v);
      if (v < floor) {  // bge past it: a NaN kept
        put(1632 + 4 * gear, floor);
      }
    }
  }

  // +1684: +1500 to +1532 += the table at +208; +1552 and +1560.
  if (const int32_t l36 = level(36); l36 > 0) {
    const uint32_t i = index(l36);
    fill({{208, kZero}, {216, k0_01}, {224, k0_02},  {160, kOne},   {164, k0_8},  {212, k0_005},
          {168, k0_8},  {172, k0_85}, {176, k0_85},  {220, k0_015}, {180, k0_9},  {184, k0_9},
          {96, kOne},   {228, k0_025}, {232, k0_025}, {144, k0_025}});
    const double add = entry(208, i);
    fill({{100, k0_95}, {104, k0_9}, {108, k0_85}, {112, k0_8}, {116, k0_7}, {120, k0_5},
          {128, kZero}, {132, k0_01}, {136, k0_015}, {140, k0_02}, {148, k0_03}, {152, k0_04}});
    for (uint32_t j = 0; j < 9; j++) {
      put(1500 + 4 * j, Fadds(add, at(1500 + 4 * j)));
    }
    const double wheelbase = at(4);
    const double x80 = at(80);
    const double ahead = Fmadds(wheelbase, k(k0_5), x80);
    const double v1552 = at(1552);
    const double mass = at(0);
    const double v1560 = at(1560);
    const double m = Fsubs(entry(160, i), one);
    put(1552, Fadds(entry(128, i), v1552));
    put(1560, Fmuls(entry(96, i), v1560));
    const double share = Fdivs(ahead, wheelbase);
    const double other = Fsubs(one, share);
    const double weight = k(k0_01);
    rear = Fmadds(Fmuls(Fmuls(m, mass), share), weight, rear);
    front = Fmadds(Fmuls(Fmuls(other, m), mass), weight, front);
  }

  // +1688: the springs +1720, +1740 (the table at +160) and +1732, +1752
  // (+128).
  if (const int32_t l40 = level(40); l40 > 0) {
    const uint32_t i = index(l40);
    fill({{208, kOne}, {212, k0_95}, {216, k0_9}, {220, k0_85}, {224, k0_85},
          {184, k1_6}, {152, k1_6}, {228, k0_85}, {232, k0_8}});
    const double m = Fsubs(entry(208, i), one);
    const double mass = at(0);
    fill({{168, k1_2}, {172, k1_3}, {176, k1_4}, {160, kOne}, {164, k1_1}, {180, k1_5}});
    const double front_factor = entry(160, i);
    const double v1720 = at(1720);
    fill({{136, k1_2}, {140, k1_3}, {144, k1_4}, {128, kOne}, {132, k1_1}, {148, k1_5}});
    const double rear_factor = entry(128, i);
    const double v1732 = at(1732);
    put(1720, Fmuls(v1720, front_factor));
    const double v1740 = at(1740);
    put(1732, Fmuls(v1732, rear_factor));
    const double v1752 = at(1752);
    const double weight = k(k0_02);
    front = Fmadds(Fmuls(mass, m), weight, front);
    put(1740, Fmuls(front_factor, v1740));
    put(1752, Fmuls(rear_factor, v1752));
    rear = Fmadds(Fmuls(mass, m), weight, rear);
  }

  // +1696: the brakes +168 (clamped to [0, +164]), +132, +136 (x the
  // table at +160), +156, +160 (/ it) and the wheels' masses +400, +672.
  if (const int32_t l48 = level(48); l48 > 0) {
    const uint32_t i = index(l48);
    fill({{212, k30}, {208, kZero}});
    const double v168 = at(168);
    fill({{180, k1_5}, {216, k75}, {160, kOne}});
    const double v132 = at(132);
    const double v136 = at(136);
    fill({{220, k120}});
    const double v156 = at(156);
    const double v160 = at(160);
    const double v164 = at(164);
    fill({{224, k180}, {228, k240}, {232, k240}});
    double brake = Fadds(entry(208, i), v168);
    put(168, brake);
    fill({{164, k1_08}, {168, k1_18}, {172, k1_28}, {176, k1_38}, {184, k1_65}});
    const double factor = entry(160, i);
    const double inverse = Fdivs(one, factor);
    put(132, Fmuls(factor, v132));
    put(136, Fmuls(v136, factor));
    put(156, Fmuls(v156, inverse));
    put(160, Fmuls(v160, inverse));
    // bge past each: a NaN kept.
    if (brake < zero) {
      brake = zero;
    } else if (v164 < brake) {
      brake = v164;
    }
    fill({{208, kOne}, {212, k0_95}, {216, kOne}, {220, k1_05}, {224, k1_1},
          {228, k1_15}, {232, kOne}});
    const double m = Fsubs(entry(208, i), one);
    const double mass = at(0);
    put(168, brake);
    const double v400 = at(400);
    const double v672 = at(672);
    const double weight = k(k0_04);
    put(400, Fmadds(Fmuls(mass, m), weight, v400));
    put(672, Fmadds(Fmuls(mass, m), weight, v672));
  }

  // The wheels (+1728, +1736 not 0): the axle's wheel mass - 3 + the
  // inch-up (+1732, +1740), both into the front's +400 (as the game has
  // it).
  if (level(80) != 0) {
    const double v = Fsubs(at(400), k(k3));
    put(400, v);
    const double inch = StackIntegerFloat(base, f + 80, int64_t(level(84)));
    put(400, Fadds(inch, v));
  }
  if (level(88) != 0) {
    put(672, Fsubs(at(672), k(k3)));
    const int32_t l92 = level(92);
    const double v = at(400);
    const double inch = StackIntegerFloat(base, f + 80, int64_t(l92));
    put(400, Fadds(inch, v));
  }

  // The wheels' inertias +536, +808 = mass x radius².
  {
    const double radius_front = at(388);
    double m_front = at(400);
    const double radius_rear = at(660);
    m_front = Fmuls(m_front, radius_front);
    const double m_rear = Fmuls(at(672), radius_rear);
    put(536, Fmuls(m_front, radius_front));
    put(808, Fmuls(m_rear, radius_rear));
  }
  if (level(152) != 0) {
    front = Fsubs(front, k(k10));
  }

  // +1708: +1552 (at most 1), the mass split and the ride height (f17).
  if (const int32_t l60 = level(60); l60 > 0) {
    const uint32_t i = index(l60);
    fill({{208, kZero}, {212, k0_01}, {216, k0_02}, {220, k0_03}, {224, k0_04},
          {232, k0_06}, {228, k0_05}});
    const double v1552 = at(1552);
    const double v = Fadds(entry(208, i), v1552);
    put(1552, v);
    if (v > one) {  // ble past it: a NaN kept
      put(1552, one);
    }
    fill({{216, k0_9}, {224, k0_85}, {232, k0_8}, {208, kOne}, {212, k0_95}, {220, k0_875}});
    const double wheelbase = at(4);
    const double x80 = at(80);
    const double mass = at(0);
    fill({{228, k0_83}});
    const double m = Fsubs(entry(208, i), one);
    fill({{208, kZero}, {212, kMinus0_005}, {216, kMinus0_01}, {220, kMinus0_012},
          {224, kMinus0_015}, {228, kMinus0_02}, {232, kMinus0_02}});
    const double ahead = Fmadds(wheelbase, k(k0_5), x80);
    height = entry(208, i);
    const double share = Fdivs(ahead, wheelbase);
    const double other = Fsubs(one, share);
    const double a = Fmuls(mass, share);
    const double b = Fmuls(other, mass);
    rear = Fmadds(Fmuls(a, m), one, rear);
    front = Fmadds(Fmuls(b, m), one, front);
  }

  // The aero: +1800, +1804 = 1, then for road cars (block 2's byte +131 0)
  // the parts at car + 3636 (28 bytes: +0, +4 the masses added, +8, +12
  // x 1082.551 off +1712, +1716, +16, +20 x +1800, +1804; the first sets
  // them) and the record copied to frame + 208 (+16, +20 x +1800, +1804;
  // +28, +32 x 0.002 onto the tracks +8, +12). With +1784 set, the record
  // at +204 and only the parts its flags (+24) pick; else the one at +168
  // and every part. Part 3 is -5 at the rear when +1756 is -1.
  put(1800, one);
  put(1804, one);
  const double track_step = k(k0_002);
  if (guest::LoadU8(base, spec + 131) == 0) {
    const bool flagged = level(136) != 0;
    const double drag = k(kDragStep);
    c_runtime::Memcpy(base, f + 208, parts + (flagged ? 204 : 168), kCopyBytes);
    r3 = f + 208;  // memcpy's r3 (its r3 saved below the stack pointer isn't reproduced)
    auto part = [&](uint32_t p) {
      put(1800, Fmuls(Lfs(base, parts + p + 16), at(1800)));
      rear = Fadds(Lfs(base, parts + p + 4), rear);
      front = Fadds(Lfs(base, parts + p), front);
      put(1804, Fmuls(Lfs(base, parts + p + 20), at(1804)));
      put(1712, Fnmsubs(Lfs(base, parts + p + 8), drag, at(1712)));
      put(1716, Fnmsubs(Lfs(base, parts + p + 12), drag, at(1716)));
    };
    if (!flagged) {
      front = Fadds(Lfs(base, parts), front);
      rear = Fadds(Lfs(base, parts + 4), rear);
      put(1800, Lfs(base, parts + 16));
      put(1804, Lfs(base, parts + 20));
      put(1712, Fnmsubs(Lfs(base, parts + 8), drag, at(1712)));
      put(1716, Fnmsubs(Lfs(base, parts + 12), drag, at(1716)));
      part(28);
      part(56);
    }
    if (!flagged || (guest::LoadU32(base, f + 232) & 2) != 0) {
      if (level(108) == -1) {
        rear = Fsubs(rear, k(k5));
      } else {
        part(84);
      }
    }
    if (!flagged || (guest::LoadU32(base, f + 232) & 1) != 0) {
      part(112);
    }
    if (!flagged) {
      part(140);
    }
    put(1800, Fmuls(Lfs(base, f + 224), at(1800)));
    put(1804, Fmuls(at(1804), Lfs(base, f + 228)));
    front = Fadds(Lfs(base, f + 208), front);
    rear = Fadds(Lfs(base, f + 212), rear);
    const double v1712 = Fnmsubs(Lfs(base, f + 216), drag, at(1712));
    const double v1716 = Fnmsubs(Lfs(base, f + 220), drag, at(1716));
    const double v8 = Fmadds(Lfs(base, f + 236), track_step, at(8));
    const double v12 = Fmadds(Lfs(base, f + 240), track_step, at(12));
    put(1712, v1712);
    put(1716, v1716);
    put(8, v8);
    put(12, v12);
  }

  // The wheels above 0: block 2's +900 (front) and +904 (rear) x 0.002,
  // both onto the front track +8.
  if (level(80) > 0) {
    const uint32_t block2 = vehicle_accessors::ObjectRecord1888(car);
    r3 = block2;
    put(8, Fmadds(Lfs(base, block2 + 900), track_step, at(8)));
  }
  if (level(88) > 0) {
    const uint32_t block2 = vehicle_accessors::ObjectRecord1888(car);
    r3 = block2;
    put(8, Fmadds(Lfs(base, block2 + 904), track_step, at(8)));
  }

  // The masses added at the axles (half the wheelbase ahead of and behind
  // the center): the mass +0, +36 x new / old, the center of mass +80;
  // +84 += the ride height's change.
  {
    const double wheelbase = at(4);
    const double half = Fmuls(wheelbase, k(k0_5));
    const double mass = at(0);
    const double x80 = at(80);
    const double v36 = at(36);
    const double behind = Fmuls(wheelbase, k(kMinus0_5));
    put(84, Fadds(at(84), height));
    double ahead_mass = Fmuls(mass, behind);
    ahead_mass = Fmsubs(mass, x80, ahead_mass);
    ahead_mass = Fmuls(ahead_mass, k(k0_5));
    ahead_mass = Fdivs(ahead_mass, half);
    double rear_mass = Fsubs(mass, ahead_mass);
    const double front_mass = Fadds(ahead_mass, front);
    rear_mass = Fadds(rear_mass, rear);
    const double total = Fadds(rear_mass, front_mass);
    put(0, total);
    double moment = Fmuls(rear_mass, behind);
    const double ratio = Fdivs(total, mass);
    moment = Fmadds(front_mass, half, moment);
    put(36, Fmuls(ratio, v36));
    put(80, Fdivs(moment, total));
  }
  // +1500 to +1532, +1552 and +1832 at most 1 (ble past it: a NaN kept).
  for (uint32_t j = 0; j < 9; j++) {
    if (at(1500 + 4 * j) > one) {
      put(1500 + 4 * j, one);
    }
  }
  if (at(1552) > one) {
    put(1552, one);
  }
  if (at(1832) > one) {
    put(1832, one);
  }

  // An array on the stack nothing reads: the engine stage x 10, at most 100,
  // and four zeros.
  guest::StoreU32(base, f + 100, 0);
  guest::StoreU32(base, f + 104, 0);
  guest::StoreU32(base, f + 108, 0);
  guest::StoreU32(base, f + 112, 0);
  guest::StoreU32(base, f + 96, uint32_t(int32_t(level0 * 100u) / 10));
  for (uint32_t j = 0; j < 5; j++) {
    if (int32_t(guest::LoadU32(base, f + 96 + 4 * j)) > 100) {
      guest::StoreU32(base, f + 96 + 4 * j, 100);
    }
  }

  // The torque curve (+1292, 32 floats): with the factor above 1, each
  // point after the first 1.0 (from the second) that falls below 0.975 x
  // the one before it is x the factor, at most that (bge and ble: a NaN
  // compares as not below and not above).
  int32_t peak = 0;
  for (uint32_t j = 1; j < 32; j++) {
    if (at(1292 + 4 * j) == one) {
      peak = int32_t(j);
      break;
    }
  }
  if (curve > one) {
    const double fall = k(k0_975);
    for (int32_t j = 0; j < 32; j++) {
      if (j <= peak) {
        continue;
      }
      const uint32_t point = 1292 + 4 * uint32_t(j);
      const double limit = Fmuls(at(point - 4), fall);
      double v = at(point);
      if (!(v < limit)) {
        continue;
      }
      v = Fmuls(v, curve);
      put(point, v < limit ? v : limit);
    }
  }
  *f1 = k(k0_04);
  return r3;
}

}  // namespace car_physics
