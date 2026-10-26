// The car object's parts as plain C++ (docs/functions/race-setup.md); see
// race_setup.h.

#include "race_setup/race_setup.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "race_overlays/race_overlays.h"
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <climits>
#include <cmath>

namespace race_setup {

namespace {

// The originals' lfs then stfs of one float.
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, float(double(guest::LoadFloat(base, from))));
}

// `count` decal entries from `decals`, as the originals' loops store them.
void ResetDecals(uint8_t* base, uint32_t decals, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t entry = decals + 32 * i;
    CopyFloat(base, entry + 0, kOne);
    guest::StoreU16(base, entry + 20, 0);
    CopyFloat(base, entry + 4, kOne);
    guest::StoreU8(base, entry + 22, 0);
    CopyFloat(base, entry + 8, kZero);
    guest::StoreU8(base, entry + 23, 0);
    CopyFloat(base, entry + 12, kZero);
    guest::StoreU32(base, entry + 28, 0);
    CopyFloat(base, entry + 16, kZero);
    guest::StoreU32(base, entry + 24, 0xFFFFFFFFu);
  }
}

// fctiwz as the translated code does it: NaN and values at or below
// -2^31 - 1 give INT32_MIN, values at or above INT32_MAX give INT32_MAX.
int32_t Fctiwz(double v) {
  if (std::isnan(v) || v <= -2147483649.0) {
    return INT32_MIN;
  }
  if (v >= 2147483647.0) {
    return INT32_MAX;
  }
  return int32_t(v);
}

// lwa, fcfid, frsp, stfs: a signed word as a float.
void WordToFloat(uint8_t* base, uint32_t to, uint32_t from) {
  const int32_t word = int32_t(guest::LoadU32(base, from));
  guest::StoreFloat(base, to, float(double(int64_t(word))));
}

// The three 20-byte entries at a car object's +1588 from `from`: +0, +4
// copied, +8 to +16 converted from integers (in the originals' orders: the
// build's +8, +16, +12, the spec's +8, +12, +16).
void CopyEntries1588(uint8_t* base, uint32_t car, uint32_t from, bool from_build) {
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t to = car + 1588 + 20 * i;
    const uint32_t source = from + 20 * i;
    guest::StoreU32(base, to + 0, guest::LoadU32(base, source + 0));
    guest::StoreU32(base, to + 4, guest::LoadU32(base, source + 4));
    if (from_build) {
      WordToFloat(base, to + 8, source + 8);
      WordToFloat(base, to + 16, source + 16);
      WordToFloat(base, to + 12, source + 12);
    } else {
      WordToFloat(base, to + 8, source + 8);
      WordToFloat(base, to + 12, source + 12);
      WordToFloat(base, to + 16, source + 16);
    }
  }
}

// rand() % n, as divw's remainder (rand is 0 to 32767).
uint32_t RandBelow(uint8_t* base, uint32_t n, uint32_t& last) {
  last = race_overlays::Rand(base);
  return uint32_t(int32_t(last) % int32_t(n));
}

}  // namespace

void ResetDecals40(uint8_t* base, uint32_t decals) {
  ITC_NATIVE_RUNNING("sub_820CC4A0 Car::ResetDecals40");
  ResetDecals(base, decals, 40);
}

void ResetDecals6(uint8_t* base, uint32_t decals) {
  ITC_NATIVE_RUNNING("sub_820CCBF0 Car::ResetDecals6");
  ResetDecals(base, decals, 6);
}

void ResetSettings(uint8_t* base, uint32_t settings) {
  ITC_NATIVE_RUNNING("sub_820CD760 Car::ResetSettings");
  guest::StoreU8(base, settings + 61, 7);
  for (uint32_t i = 56; i <= 59; i++) {
    guest::StoreU8(base, settings + i, 2);
  }
  guest::StoreU8(base, settings + 60, 3);
  for (uint32_t i = 62; i <= 76; i++) {
    guest::StoreU8(base, settings + i, 0);
  }
  guest::StoreU8(base, settings + 77, 10);
  guest::StoreU8(base, settings + 78, 0);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU16(base, settings + 2 * i, 0);
  }
}

void Reset1568(uint8_t* base, uint32_t bytes) {
  ITC_NATIVE_RUNNING("sub_820CCC70 Car::Reset1568");
  guest::StoreU32(base, bytes + 16, 0);
  static constexpr uint8_t kBytes[12] = {0, 0, 5, 0, 0, 1, 2, 1, 2, 2, 2, 2};
  // The original's order: +0, +1, +12, then +2 to +11.
  guest::StoreU8(base, bytes + 0, 0);
  guest::StoreU8(base, bytes + 1, 0);
  guest::StoreU8(base, bytes + 12, 0);
  for (uint32_t i = 2; i < 12; i++) {
    guest::StoreU8(base, bytes + i, kBytes[i]);
  }
}

void TuningReset(uint8_t* base, uint32_t tuning, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_820CD618 CarTuning::Reset");
  guest::StoreU32(base, tuning + 0, 0);
  guest::StoreU32(base, tuning + 4, flag != 0 ? 1 : 0);
  // (Seven zero words go through the red zone first: not written.)
  const uint32_t word4 = guest::LoadU32(base, tuning + 4);
  guest::StoreU32(base, tuning + 8, 0);
  guest::StoreU32(base, tuning + 16, 0);
  guest::StoreU32(base, tuning + 12, word4 == 1 ? 1 : 0);
  for (uint32_t offset = 20; offset <= 164; offset += 4) {
    guest::StoreU32(base, tuning + offset, 0);
  }
  guest::StoreU32(base, tuning + 168, 3);
  guest::StoreU32(base, tuning + 172, 0);
  for (uint32_t i = 0; i < 5; i++) {
    const uint32_t entry = tuning + 176 + 12 * i;
    guest::StoreU32(base, entry + 8, 0);
    guest::StoreU32(base, entry + 4, 0);
    guest::StoreU32(base, entry + 0, 1);
  }
}

void DecalFromBuild(uint8_t* base, uint32_t decal, uint32_t build) {
  ITC_NATIVE_RUNNING("sub_820CDC18 CarDecal::FromBuild");
  const uint32_t w20 = guest::LoadU32(base, build + 20);
  const uint32_t w24 = guest::LoadU32(base, build + 24);
  const uint32_t w28 = guest::LoadU32(base, build + 28);
  // sub_820CC470: +20.
  guest::StoreU32(base, decal + 20, (w20 & 0xFFFF) << 16 | (w24 & 0xFF) << 8 | (w28 & 0xFF));
  // sub_820CC460, sub_820CC468: +8, +12.
  CopyFloat(base, decal + 8, build + 8);
  CopyFloat(base, decal + 12, build + 12);
  const double scale = double(guest::LoadFloat(base, build + 0));
  uint32_t mirrored = 0;
  if (scale < double(guest::LoadFloat(base, kZero))) {
    // sub_820CC450 (never seen in play): +0 = -(build +0).
    guest::StoreFloat(base, decal + 0, float(-scale));
    mirrored = 1;
  }
  // sub_820CC478: +28.
  guest::StoreU32(base, decal + 28, mirrored);
  // sub_820CC458: +4.
  CopyFloat(base, decal + 4, build + 4);
  vehicle_accessors::Object344SetFloat16(base, decal, double(guest::LoadFloat(base, build + 16)));
  scene_models::SetCarModel(base, decal, guest::LoadU32(base, build + 32));
}

uint32_t CarClear(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820CD7F0 Car::Clear");
  guest::StoreU32(base, car + 12, 0);
  TuningReset(base, car + 1648, 0);
  guest::StoreU16(base, car + 2, 0);
  guest::StoreU16(base, car + 0, 0);
  guest::StoreU16(base, car + 6, 0);
  guest::StoreU16(base, car + 8, 0);
  guest::StoreU8(base, car + 10, 0);
  guest::StoreU16(base, car + 4, 0xFFFF);
  guest::StoreU8(base, car + 11, 0);
  ResetSettings(base, car + 16);
  ResetDecals40(base, car + 288);
  ResetDecals6(base, car + 96);
  Reset1568(base, car + 1568);
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t entry = car + 1588 + 20 * i;
    CopyFloat(base, entry + 8, kZero);
    guest::StoreU32(base, entry + 0, 0);
    CopyFloat(base, entry + 12, kZero);
    guest::StoreU32(base, entry + 4, 0);
    CopyFloat(base, entry + 16, kZero);
  }
  return car + 1568;
}

uint32_t CarInit(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820CD5D8 Car::Init");
  const uint32_t r3 = CarClear(base, car);
  guest::StoreU32(base, car + 3888, 0);
  guest::StoreU32(base, car + 3892, 1);
  return r3;
}

uint32_t SetModel(uint8_t* base, uint32_t car, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820CD8A8 Car::SetModel");
  guest::StoreU32(base, car + 12, id);
  if (car_specs::Exists(base) == 0) {
    return 0;
  }
  const uint32_t spec = car_specs::GetCarSpec(base, guest::LoadU32(base, car + 12));
  TuningReset(base, car + 1648, guest::LoadU8(base, spec + 709) != 0 ? 1 : 0);
  CopyFloat(base, car + 36, spec + 372);
  for (uint32_t i = 0; i < 8; i++) {
    CopyFloat(base, car + 40 + 4 * i, spec + 388 + 4 * i);
  }
  guest::StoreU8(base, car + 76, uint8_t(guest::LoadU32(base, spec + 632)));
  guest::StoreU8(base, car + 77, uint8_t(guest::LoadU32(base, spec + 636)));
  guest::StoreU8(base, car + 74, uint8_t(guest::LoadU32(base, spec + 800)));
  guest::StoreU8(base, car + 75, uint8_t(guest::LoadU32(base, spec + 808)));
  guest::StoreU8(base, car + 72, uint8_t(guest::LoadU32(base, spec + 796)));
  // +72 again (the original's; +73 is left as it was).
  guest::StoreU8(base, car + 72, uint8_t(guest::LoadU32(base, spec + 804)));
  const uint32_t drive = guest::LoadU32(base, spec + 356);
  if (drive != 0 && drive != 1) {
    guest::StoreU8(base, car + 92, 1);
    namespace ops = car_physics::ops;
    double share = ops::Fsubs(ops::Lfs(base, kOne), ops::Lfs(base, spec + 364));
    share = ops::Fmuls(share, ops::Lfs(base, kHundred));
    share = ops::Fmuls(share, ops::Lfs(base, kTwenty));
    share = ops::Fmuls(share, ops::Lfs(base, kTorqueScale));
    // (The original converts through its frame's +80: not written.)
    guest::StoreU8(base, car + 93, uint8_t(uint32_t(Fctiwz(share))));
  } else {
    guest::StoreU8(base, car + 92, 0);
  }
  guest::StoreU32(base, car + 1676, guest::LoadU32(base, spec + 772));
  guest::StoreU32(base, car + 1796, guest::LoadU32(base, spec + 776));
  guest::StoreU32(base, car + 1684, guest::LoadU32(base, spec + 784));
  guest::StoreU32(base, car + 1680, guest::LoadU32(base, spec + 780));
  guest::StoreU32(base, car + 1700, guest::LoadU32(base, spec + 788));
  guest::StoreU32(base, car + 1704, guest::LoadU32(base, spec + 792));
  CopyEntries1588(base, car, spec + 812, false);
  guest::StoreU32(base, car + 1716, guest::LoadU32(base, spec + 888));
  guest::StoreU32(base, car + 1712, guest::LoadU32(base, spec + 884));
  guest::StoreU32(base, car + 1724, guest::LoadU32(base, spec + 896));
  guest::StoreU32(base, car + 1720, guest::LoadU32(base, spec + 892));
  for (uint32_t i = 0; i < 5; i++) {
    const uint32_t to = car + 1824 + 12 * i;
    const uint32_t from = spec + 900 + 8 * i;
    guest::StoreU32(base, to + 8, guest::LoadU32(base, from));
    const int32_t next = int32_t(guest::LoadU32(base, from + 4));
    guest::StoreU32(base, to + 0, uint32_t(next / 3 + 1));
    const int32_t again = int32_t(guest::LoadU32(base, from + 4));
    guest::StoreU32(base, to + 4, uint32_t(again - (again / 3) * 3));
  }
  if (guest::LoadU32(base, spec + 648) <= 2000 && guest::LoadU32(base, spec + 716) <= 4700 &&
      guest::LoadU32(base, spec + 720) <= 2000 && guest::LoadU32(base, spec + 724) <= 1700) {
    guest::StoreU8(base, car + 1571, 0);
    guest::StoreU8(base, car + 1570, 5);
  } else {
    guest::StoreU8(base, car + 1570, 3);
    guest::StoreU8(base, car + 1571, 3);
  }
  guest::StoreU8(base, car + 1572, 0);
  uint32_t last = 0;
  guest::StoreU8(base, car + 1569, uint8_t(RandBelow(base, 14, last)));
  // +1573: 1 to 10; +1574 and +1575: after a 10, 1 to 10, else 1 to 9.
  uint32_t value = RandBelow(base, 10, last);
  if (value == 0) {
    value = 10;
  }
  guest::StoreU8(base, car + 1573, uint8_t(value));
  for (uint32_t offset : {1574u, 1575u}) {
    if (int8_t(value) == 10) {
      value = RandBelow(base, 10, last);
      if (value == 0) {
        value = 10;
      }
    } else {
      value = RandBelow(base, 9, last) + 1;
    }
    guest::StoreU8(base, car + offset, uint8_t(value));
    value &= 0xFF;
  }
  guest::StoreU8(base, car + 1576, uint8_t(RandBelow(base, 9, last) + 1));
  const int32_t car_id = int32_t(id);
  if (car_id == 56 || (car_id > 63 && car_id <= 68) || car_id == 70) {
    guest::StoreU8(base, car + 1580, 1);
  }
  return last;
}

uint32_t FromBuild(uint8_t* base, uint32_t car, uint32_t build) {
  ITC_NATIVE_RUNNING("sub_820CDCE0 Car::FromBuild");
  CarClear(base, car);
  const uint32_t id = guest::LoadU32(base, build + 2788);
  guest::StoreU32(base, car + 12, id);
  SetModel(base, car, id);
  const uint16_t paint = uint16_t(guest::LoadU32(base, build + 2792));
  guest::StoreU16(base, car + 0, paint);
  guest::StoreU16(base, car + 2, paint);
  guest::StoreU16(base, car + 8, uint16_t(guest::LoadU32(base, build + 2800)));
  guest::StoreU8(base, car + 10, uint8_t(guest::LoadU32(base, build + 2804)));
  guest::StoreU8(base, car + 11, uint8_t(guest::LoadU32(base, build + 2808)));
  guest::StoreU16(base, car + 6, uint16_t(guest::LoadU32(base, build + 2796)));
  for (uint32_t k = 0; k < 3; k++) {
    DecalFromBuild(base, car + 96 + 32 * k, build + 392 + 36 * k);
    DecalFromBuild(base, car + 192 + 32 * k, build + 500 + 36 * k);
  }
  static constexpr uint32_t kGroups[8] = {608, 968, 1328, 1688, 1868, 2048, 2408, 2588};
  for (uint32_t g = 0; g < 8; g++) {
    for (uint32_t k = 0; k < 5; k++) {
      DecalFromBuild(base, car + 288 + 160 * g + 32 * k, build + kGroups[g] + 36 * k);
    }
  }
  namespace ops = car_physics::ops;
  const double one = ops::Lfs(base, kOne);
  const double scale = ops::Lfs(base, kGearScale);
  const double zero = ops::Lfs(base, kZero);
  for (uint32_t i = 0; i < 7; i++) {
    const double factor = ops::Lfs(base, build + 4 * i);
    if (factor == zero) {
      continue;
    }
    const double ratio = ops::Fmadds(factor, scale, one);
    ops::Stfs(base, car + 36 + 4 * i, ops::Fmuls(ratio, ops::Lfs(base, car + 36 + 4 * i)));
  }
  // Setting bytes from build words +28 to +116.
  static constexpr uint8_t kSettings[23] = {72, 73, 74, 75, 80, 81, 78, 79, 82, 83, 84, 85,
                                            86, 87, 88, 89, 94, 92, 93, 90, 91, 76, 77};
  for (uint32_t i = 0; i < 23; i++) {
    guest::StoreU8(base, car + kSettings[i], uint8_t(guest::LoadU32(base, build + 28 + 4 * i)));
  }
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, car + 1568 + 4 * i, guest::LoadU32(base, build + 2768 + 4 * i));
  }
  CopyEntries1588(base, car, build + 332, true);
  // The tuning words: build +120 + 4i into car +kTuning[i].
  static constexpr uint16_t kTuning[38] = {
      1656, 1664, 1672, 1668, 1676, 1680, 1684, 1688, 1696, 1692, 1704, 1700, 1708,
      1716, 1712, 1724, 1720, 1744, 1748, 1752, 1756, 1764, 1760, 1768, 1772, 1776,
      1780, 1784, 1728, 1732, 1736, 1740, 1788, 1796, 1800, 1804, 1808, 1812};
  for (uint32_t i = 0; i < 38; i++) {
    guest::StoreU32(base, car + kTuning[i], guest::LoadU32(base, build + 120 + 4 * i));
  }
  uint32_t wheel = 0;
  for (uint32_t i = 0; i < 2; i++) {
    const int32_t number = int32_t(guest::LoadU32(base, build + 232 + 8 * i));
    wheel = car_specs::FirstWheelOfModel(base, number / 10);
    guest::StoreU32(base, car + 1728 + 8 * i, wheel + uint32_t(number % 10));
    guest::StoreU32(base, car + 1732 + 8 * i, guest::LoadU32(base, build + 236 + 8 * i));
  }
  return wheel;
}

}  // namespace race_setup
