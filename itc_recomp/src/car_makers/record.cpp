// The scene model record's setters (docs/functions/car-makers.md), step for
// step as the translated code (see car_makers.h).

#include "car_makers/car_makers.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "car_specs/car_specs.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "scene_models/scene_models.h"
#include "strings_and_font/steps.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_makers {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kByteScale = 0x82002200;  // 1/255
constexpr uint32_t kHueScale = 0x82002A14;   // not read
constexpr uint32_t kValue72 = 0x82009788;    // not read
constexpr uint32_t kValue72Two = 0x82001070; // not read
constexpr uint32_t kTireScale = 0x82001BDC;  // not read
constexpr uint32_t kHundred = 0x82002A0C;    // 100.0
constexpr uint32_t kKitEntries = 0x820093E0; // the aero kits' 24-byte entries
constexpr uint32_t kGet82 = 0x820CBC90;      // AeroParts::Get82

// std of a doubleword.
void Std(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

// An unsigned value through the stack to a float (std, lfd, fcfid, frsp),
// its doubleword left at `scratch`.
double Convert(uint8_t* base, uint64_t value, uint32_t scratch) {
  Std(base, scratch, value);
  return Frsp(double(int64_t(value)));
}

// A halfword as a float into `to`, then two bytes x f0 into `to` + 4 and + 8,
// each through its scratch doubleword.
void Triple(uint8_t* base, uint32_t from_half, uint32_t from_b0, uint32_t from_b1, uint32_t to,
            double f0, uint32_t s0, uint32_t s1, uint32_t s2) {
  Stfs(base, to, Convert(base, guest::LoadU16(base, from_half), s0));
  Stfs(base, to + 4, Fmuls(Convert(base, guest::LoadU8(base, from_b0), s1), f0));
  Stfs(base, to + 8, Fmuls(Convert(base, guest::LoadU8(base, from_b1), s2), f0));
}

// An lfs then stfs of one word: its bits (the translated code's float to
// double and back folds to a copy, keeping a signaling NaN).
void CopyFloat(uint8_t* base, uint32_t from, uint32_t to) {
  Store(base, to, Load(base, from));
}

// One decal entry of the car (32 bytes at `e`) into the record's 64-byte
// entry at `r` (its +0, +4 words, +16, +20, +32, +48, +52 floats), in the
// original's order.
void Decal(uint8_t* base, uint32_t r, uint32_t e) {
  Store(base, r + 0, Load(base, e + 24));
  Store(base, r + 4, Load(base, e + 28));
  CopyFloat(base, e + 8, r + 16);
  CopyFloat(base, e + 12, r + 20);
  CopyFloat(base, e + 16, r + 32);
  CopyFloat(base, e + 0, r + 48);
  CopyFloat(base, e + 4, r + 52);
}

// SetChoice with the 12-byte value at `from` (ld of its first doubleword;
// the word +8 rotated into r5's high half), from the frame.
void ChoiceFrom(uint8_t* base, uint32_t out, uint32_t from, uint32_t frame) {
  const uint64_t c = Load(base, from + 8);
  const uint64_t r4 = (uint64_t(Load(base, from)) << 32) | Load(base, from + 4);
  SetChoice(base, out, r4, c << 32, frame);
}

}  // namespace

uint32_t BlockOf(uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8215AAD0 CarModels::BlockOf");
  // By id - 1 (0 to 70); id 1's case is sub_8215AB04.
  static constexpr uint8_t kBlocks[71] = {
      1,  18, 19, 2,  4,  3,  10, 9,  21, 5,  8,  7,  6,  12, 11, 13, 14, 20,
      15, 22, 17, 16, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 41, 42, 43, 44, 36, 37, 38, 39,
      11, 12, 15, 13, 10, 5,  4,  6,  9,  1,  2,  3,  0,  14, 8,  7,  40};
  const uint32_t index = id - 1;
  return index > 70 ? 0 : kBlocks[index];
}

uint32_t SetPaint(uint8_t* base, uint32_t record, uint32_t car, uint32_t pick, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215AC70 SceneModelRecord::SetPaint");
  int32_t p = int32_t(pick);
  if (p == -1) {
    p = int16_t(guest::LoadU16(base, car + 0));
  }
  const int32_t kind = int32_t(Load(base, record + 0));
  const double f0 = Lfs(base, kByteScale);
  const uint32_t s16 = stack - 16, s24 = stack - 24, s32 = stack - 32;
  if (kind == 3) {
    Store(base, record + 176, uint32_t(p));
  } else if (p == -2) {
    for (uint32_t i = 0; i < 3; i++) {
      Triple(base, car + 3376 + 4 * i, car + 3378 + 4 * i, car + 3379 + 4 * i,
             record + 32 + 16 * i, f0, s32, s24, s16);
    }
    Stfs(base, record + 32, Convert(base, guest::LoadU16(base, car + 8), s16));
    Stfs(base, record + 36, Fmuls(Convert(base, guest::LoadU8(base, car + 10), s16), f0));
    Stfs(base, record + 40, Fmuls(Convert(base, guest::LoadU8(base, car + 11), s16), f0));
    Stfs(base, record + 48, Convert(base, guest::LoadU16(base, car + 8), s16));
    Stfs(base, record + 52, Fmuls(Convert(base, guest::LoadU8(base, car + 10), s16), f0));
    Stfs(base, record + 56, Fmuls(Convert(base, guest::LoadU8(base, car + 11), s16), f0));
    const int32_t six = int16_t(guest::LoadU16(base, car + 6));
    if (six == 0) {
      const double zero = Lfs(base, kZero);
      Stfs(base, record + 64, zero);
      Stfs(base, record + 68, zero);
      CopyFloat(base, kValue72, record + 72);
    } else if (six == 1) {
      const double value = Lfs(base, kValue72Two);
      Stfs(base, record + 64, Convert(base, guest::LoadU16(base, car + 8), s16));
      const uint32_t b10 = guest::LoadU8(base, car + 10);
      Stfs(base, record + 72, value);
      const double f12 = Convert(base, b10, s16);
      Stfs(base, record + 68, Fmuls(Fmuls(f12, Lfs(base, kHueScale)), f0));
    }
  } else {
    const uint32_t from = uint32_t(int64_t(p) * 20) + car + 3376;
    for (uint32_t i = 0; i < 3; i++) {
      Triple(base, from + 4 * i, from + 2 + 4 * i, from + 3 + 4 * i, record + 32 + 16 * i, f0,
             s16, s24, s32);
    }
  }
  Store(base, record + 4040, 1);
  Triple(base, car + 3276, car + 3278, car + 3279, record + 80, f0, s16, s16, s16);
  Triple(base, car + 3324, car + 3326, car + 3327, record + 128, f0, s16, s16, s16);
  Triple(base, car + 3288, car + 3290, car + 3291, record + 96, f0, s16, s16, s16);
  Triple(base, car + 3336, car + 3338, car + 3339, record + 144, f0, s16, s16, s16);
  Triple(base, car + 3292, car + 3294, car + 3295, record + 112, f0, s16, s16, s16);
  Triple(base, car + 3340, car + 3342, car + 3343, record + 160, f0, s16, s16, s16);
  CopyFloat(base, car + 3284, record + 4060);
  CopyFloat(base, car + 3332, record + 4076);
  CopyFloat(base, car + 3296, record + 4056);
  CopyFloat(base, car + 3344, record + 4072);
  return record;
}

uint32_t SetWheels(uint8_t* base, uint32_t record, uint32_t car, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215B238 SceneModelRecord::SetWheels");
  Store(base, stack - 112, stack);
  Store(base, record + 4052, Load(base, car + 3308) - 1);
  Store(base, record + 4068, Load(base, car + 3356) - 1);
  Store(base, record + 4104, Load(base, car + 1696));
  Store(base, record + 4124, Load(base, car + 1692));
  uint32_t block2 = vehicle_accessors::ObjectRecord1888(car);
  if (Load(base, block2 + 236) != 0) {
    block2 = vehicle_accessors::ObjectRecord1888(car);
    Store(base, record + 4124, Load(base, block2 + 236));
  }
  using strings_and_font::steps::Fctiwz;
  Store(base, record + 4080, uint32_t(Fctiwz(Lfs(base, car + 3164))));
  const double scale = Lfs(base, kTireScale);
  Stfs(base, record + 4084, Fmuls(Lfs(base, car + 3132), scale));
  const double hundred = Lfs(base, kHundred);
  Stfs(base, record + 4088, Fmuls(Lfs(base, car + 3168), hundred));
  Store(base, record + 4092, uint32_t(Fctiwz(Lfs(base, car + 3256))));
  Stfs(base, record + 4096, Fmuls(Lfs(base, car + 3224), scale));
  Stfs(base, record + 4100, Fmuls(Lfs(base, car + 3260), hundred));
  return block2;
}

void SetChoice(uint8_t* base, uint32_t out, uint64_t r4, uint64_t r5, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215B1A0 SceneModelRecord::SetChoice");
  Std(base, stack + 40, r5);
  Std(base, stack + 32, r4);
  const uint32_t c = Load(base, stack + 40);
  if (c == 0) {
    Store(base, out + 0, c);
    return;
  }
  const uint32_t a = Load(base, stack + 32);
  Store(base, out + 4, c - 1);
  Store(base, out + 0, 1);
  uint32_t k = 0;
  if (int32_t(a) == 1) {
    const uint32_t b = Load(base, stack + 36);
    k = b < 1 ? 1 : b == 1 ? 2 : 0;
  } else if (int32_t(a) == 2) {
    const uint32_t b = Load(base, stack + 36);
    k = b == 1 ? 1 : 0;
  }
  Store(base, out + 8, uint32_t(uint64_t(a) * 3 + k - 3));
}

void ResetBytes(uint8_t* base, uint32_t at) {
  ITC_NATIVE_RUNNING("sub_820D0900 CarRecord::ResetBytes");
  Store(base, at + 0, 0);
  Store(base, at + 4, 0);
  Store(base, at + 8, 1);
  Store(base, at + 12, 2);
  Store(base, at + 16, 3);
  Store(base, at + 20, 29);
  Store(base, at + 24, 36);
  Store(base, at + 36, 37);
  Store(base, at + 40, 34);
  Store(base, at + 44, 35);
  Store(base, at + 28, 31);
  Store(base, at + 32, 32);
}

uint32_t SetBytes(uint8_t* base, uint32_t record, uint32_t bytes, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215B0E0 CarRecord::SetBytes");
  Store(base, stack - 112, stack);
  const uint32_t at = record + 3632;
  ResetBytes(base, at);
  Store(base, at, 0);
  for (uint32_t i = 1; i <= 11; i++) {
    Store(base, record + 3632 + 4 * i, uint32_t(int32_t(int8_t(guest::LoadU8(base, bytes + i)))));
  }
  Store(base, record + 3684, uint32_t(int32_t(int8_t(guest::LoadU8(base, bytes + 0)))));
  return at;
}

uint32_t FromCar(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t record,
                 uint32_t car, uint64_t pick, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215B878 SceneModelRecord::FromCar");
  const uint32_t frame = stack - 256;
  Store(base, frame, stack);
  scene_models::SceneModelRecordReset(base, record);
  const uint32_t flags = Load(base, car + 3888);
  bool cars = true;  // r23
  uint32_t block;
  if ((flags & 0x81) != 0) {
    Store(base, record + 0, 2);
    block = BlockOf(Load(base, car + 12));
  } else if ((flags & 0x2) != 0) {
    Store(base, record + 0, 1);
    block = BlockOf(Load(base, car + 12));
  } else {
    Store(base, record + 0, 3);
    block = BlockOf(Load(base, car + 12));
    cars = false;
  }
  const uint32_t car_record = record + 16;
  Store(base, car_record, block);
  SetWheels(base, record, car, frame);
  SetPaint(base, record, car, uint32_t(pick), frame);
  if (int32_t(Load(base, record + 0)) == 2) {
    bool both = false;  // r28
    scene_models::CockpitRecordReset(base, record + 3892);
    Store(base, record + 3892, Load(base, car + 1760));
    Store(base, record + 3896, Load(base, car + 1760));
    Store(base, record + 3908, Load(base, car + 1780));
    Store(base, record + 3904, Load(base, car + 1772));
    Store(base, record + 3900, Load(base, car + 1768));
    Store(base, record + 3872, Load(base, car + 1800));
    Store(base, record + 3868, Load(base, car + 1804));
    Store(base, record + 3864, Load(base, car + 1808));
    Store(base, record + 3696, Load(base, car + 1812));
    const int32_t seats = int32_t(Load(base, car + 1708));
    if (seats > 0 && seats <= 2) {
      Store(base, record + 3880, 1);
      Store(base, record + 3876, 0);
    } else if (seats > 2 && seats <= 5) {
      both = true;
      Store(base, record + 3880, 1);
      Store(base, record + 3876, 1);
    } else if (seats == 6) {
      both = true;
      Store(base, record + 3880, 1);
      Store(base, record + 3876, 2);
    } else {
      Store(base, record + 3880, 0);
      Store(base, record + 3876, 0);
    }
    bool second = false;
    if (int32_t(Load(base, vehicle_accessors::ObjectRecord1888(car) + 232)) == 1) {
      Store(base, record + 3876, 1);
      second = true;
    } else {
      // The same word read again, as the original does.
      if (int32_t(Load(base, vehicle_accessors::ObjectRecord1888(car) + 232)) == 1) {
        both = true;
        Store(base, record + 3876, 2);
      }
      second = both;
    }
    if (second) {
      ChoiceFrom(base, record + 3804, car + 1860, frame);
      ChoiceFrom(base, record + 3816, car + 1872, frame);
    } else {
      ChoiceFrom(base, record + 3780, car + 1860, frame);
      ChoiceFrom(base, record + 3792, car + 1872, frame);
    }
    ChoiceFrom(base, record + 3852, car + 1824, frame);
    ChoiceFrom(base, record + 3840, car + 1836, frame);
    ChoiceFrom(base, record + 3828, car + 1848, frame);
    const uint32_t light = Load(base, car + 1816);
    if (int32_t(light) != 3) {
      Store(base, record + 3884, light);
      Store(base, record + 3888, Load(base, car + 1820));
    }
  }
  if (cars) {
    Store(base, record + 3756, Load(base, car + 1656));
    if (int32_t(Load(base, car + 1656)) >= 7) {
      Store(base, record + 3776, 2);
    } else if (int32_t(Load(base, car + 1656)) >= 3) {
      Store(base, record + 3776, 1);
    } else {
      Store(base, record + 3776, 0);
    }
    Store(base, record + 3716, Load(base, car + 1664));
    Store(base, record + 3760, Load(base, car + 1668));
    Store(base, record + 3740, Load(base, car + 1768));
    Store(base, record + 3744, Load(base, car + 1772));
    Store(base, record + 3748, Load(base, car + 1776));
    if (Load(base, car + 1784) == 0) {
      Store(base, record + 3712, Load(base, car + 1744));
      Store(base, record + 3720, Load(base, car + 1752));
      Store(base, record + 3724, Load(base, car + 1748));
      uint32_t v = Load(base, car + 1756);
      Store(base, record + 3728, int32_t(v) == -1 ? 13 : v);
      Store(base, record + 3732, Load(base, car + 1760));
      Store(base, record + 3708, int32_t(Load(base, car + 1764)) == 1 ? 1 : 0);
      Store(base, record + 3736, 0);
      Store(base, record + 3752, Load(base, car + 1780));
    } else {
      const uint32_t out = frame + 128, parts_copy = frame + 80;
      uint32_t parts[9];
      car_specs::GetCarParts(base, Load(base, car + 12), parts);
      for (uint32_t i = 0; i < 9; i++) {
        Store(base, out + 4 * i, parts[i]);
      }
      front_end_car_draw::Memcpy(base, calls, parts_copy, out, 36, frame, 0x8215BB98);
      const uint32_t k = Load(base, car + 1784);
      const uint64_t k_less = uint64_t(k) - 1;  // r29
      for (uint32_t i = 0; i < 9; i++) {
        parts[i] = Load(base, parts_copy + 4 * i);
      }
      uint32_t kit = out;
      if (!car_specs::AeroPart82(base, out, parts, int32_t(k))) {
        const uint64_t regs[3] = {out, parts_copy, k};
        kit = uint32_t(calls.call(kGet82, regs, 3, nullptr, 0, frame, 0x8215BBAC, nullptr));
      }
      const uint32_t entry = uint32_t((uint64_t(Load(base, car_record) << 1) + k_less) * 24) +
                             kKitEntries;
      const uint32_t kit_flags = Load(base, kit + 24);
      front_end_car_draw::Memcpy(base, calls, parts_copy, entry, 24, frame, 0x8215BBDC);
      auto word = [&](uint32_t at) { return Load(base, frame + at); };
      if (int32_t(word(80)) != -1) {
        Store(base, record + 3712, word(80));
      }
      if (int32_t(word(84)) != -1) {
        Store(base, record + 3720, word(84));
      }
      if (int32_t(word(88)) != -1) {
        Store(base, record + 3724, word(88));
      }
      if ((kit_flags & 0x2) != 0) {
        const uint32_t v = Load(base, car + 1756);
        Store(base, record + 3728, int32_t(v) == -1 ? 13 : v);
      } else if (int32_t(word(100)) != -1) {
        Store(base, record + 3728, word(100));
      }
      if ((kit_flags & 0x1) != 0) {
        Store(base, record + 3732, Load(base, car + 1760));
        Store(base, record + 3708, int32_t(Load(base, car + 1764)) == 1 ? 1 : 0);
      } else if (int32_t(word(96)) != -1) {
        Store(base, record + 3732, word(96));
      }
      Store(base, record + 3736, (kit_flags & 0x8) != 0 ? uint32_t(k_less + 1) : 0);
      if (int32_t(word(92)) != -1) {
        Store(base, record + 3752, word(92));
      }
    }
  }
  Store(base, record + 3704, Load(base, car + 1792));
  if (cars) {
    // Entries 2, 1, 0 of +96 and 5, 4, 3 (their +96 bytes on) into +192 and
    // +384.
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t r = record + 192 + 64 * i, e = car + 160 - 32 * i;
      Decal(base, r, e);
      Decal(base, r + 192, e + 96);
    }
    // The forty at +288 in runs of five.
    static constexpr uint32_t kRuns[8][2] = {{576, 288},   {1344, 448},  {960, 608},
                                             {1728, 768},  {2112, 928},  {2496, 1088},
                                             {3264, 1248}, {2880, 1408}};
    for (const auto& run : kRuns) {
      for (uint32_t i = 0; i < 5; i++) {
        Decal(base, record + run[0] + 64 * i, car + run[1] + 32 * i);
      }
    }
  }
  const int32_t doors = int16_t(guest::LoadU16(base, car + 4));
  Store(base, record + 3768, (doors == 1 || doors == 2 || doors == 4) ? 1 : 0);
  if (cars) {
    const uint32_t table = frame + 80;
    Store(base, table + 0, 0);
    Store(base, table + 4, 1);
    Store(base, table + 8, 1);
    Store(base, table + 12, 2);
    Store(base, record + 4196, Load(base, car + 1588));
    CopyFloat(base, car + 1596, record + 4200);
    Store(base, table + 16, 2);
    CopyFloat(base, car + 1600, record + 4204);
    CopyFloat(base, car + 1604, record + 4208);
    Store(base, record + 4212, Load(base, car + 1608));
    CopyFloat(base, car + 1616, record + 4216);
    CopyFloat(base, car + 1620, record + 4220);
    CopyFloat(base, car + 1624, record + 4224);
    Store(base, record + 4228, Load(base, car + 1628));
    CopyFloat(base, car + 1636, record + 4232);
    CopyFloat(base, car + 1640, record + 4236);
    CopyFloat(base, car + 1644, record + 4240);
    Store(base, record + 4192, Load(base, car + 1592));
    Store(base, record + 4048, Load(base, table + (Load(base, car + 1716) << 2)));
    Store(base, record + 4064, Load(base, table + (Load(base, car + 1724) << 2)));
  }
  return SetBytes(base, car_record, car + 1568, frame);
}

}  // namespace car_makers
