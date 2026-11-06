// The scene model record's setters (docs/functions/car-makers.md), step for
// step as the translated code (see car_makers.h).

#include "car_makers/car_makers.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "guest.h"
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
      Stfs(base, record + 72, Lfs(base, kValue72));
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
  Stfs(base, record + 4060, Lfs(base, car + 3284));
  Stfs(base, record + 4076, Lfs(base, car + 3332));
  Stfs(base, record + 4056, Lfs(base, car + 3296));
  Stfs(base, record + 4072, Lfs(base, car + 3344));
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

}  // namespace car_makers
