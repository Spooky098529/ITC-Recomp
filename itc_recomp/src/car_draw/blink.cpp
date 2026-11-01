// The light kit's blink as plain C++ (docs/functions/car-draw.md:
// LightKit::BlinkLevel and BlinkTimers::Advance); see car_draw.h. Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt): the scalar work
// as the translated code's doubles (rounded to single where it rounds).

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "guest.h"

#include <simde/x86/sse2.h>

#include <bit>
#include <climits>
#include <cmath>

namespace car_draw {

namespace {

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kRaceData = 0x826C5B2C;

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
double Single(double value) {
  return double(float(value));
}
uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}
void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}
double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(LoadU64(base, address));
}

// fctiwz as the translation does it: NaN 0x80000000, saturated at INT_MAX,
// else cvttsd2si (0x80000000 below INT_MIN).
int32_t ToInt32(double value) {
  if (std::isnan(value)) {
    return int32_t(0x80000000u);
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  return simde_mm_cvttsd_si32(simde_mm_load_sd(&value));
}

// The translation's tw/td trap: its warning, then on.
void Trap() {
  ITC_LOG_WARN("tw/td trap hit (type 0)");
}

}  // namespace

// --- LightKit::BlinkLevel ---

double LightKitBlinkLevel(uint8_t* base, uint64_t counter, double a, double b, double rate,
                          uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC4B0 LightKit::BlinkLevel");
  const uint64_t mask = std::rotl(counter, 1) - 1;
  const double scale = Lfs(base, 0x82002C38);
  const uint32_t scene = guest::LoadU32(base, kScene);
  const int32_t race = int32_t(guest::LoadU32(base, scene + 4));
  double full = Lfs(base, 0x82002A3C);
  const double offset = Lfs(base, 0x82000D44);
  const double frames = Single(std::fma(rate, scale, offset));
  guest::StoreU32(base, stack - 16, uint32_t(ToInt32(frames)));
  const int64_t period = int32_t(guest::LoadU32(base, stack - 16));
  const int64_t value = int64_t(counter);
  const int64_t quotient =
      (period != 0 && !(value == INT64_MIN && period == -1)) ? value / period : 0;
  if (period == 0) {
    Trap();
  }
  const uint64_t check = uint64_t(period) & ~mask;
  const uint64_t product = uint64_t(quotient) * uint64_t(period);
  StoreU64(base, stack - 16, uint64_t(period));
  const double length = Single(double(int64_t(LoadU64(base, stack - 16))));
  const uint64_t remainder = counter - product;
  if (int64_t(check) == -1) {
    Trap();
  }
  StoreU64(base, stack - 8, remainder);
  double t = Single(double(int64_t(LoadU64(base, stack - 8))));
  t = Single(t / length);
  if (race == 1 && int32_t(guest::LoadU32(base, guest::LoadU32(base, kRaceData) + 32)) == 2) {
    full = Lfs(base, 0x82002A10);
  }
  if (a > t) {
    return full;
  }
  const double zero = Lfs(base, 0x82000A34);
  if (b == zero) {
    return zero;
  }
  const double one = Lfs(base, 0x82000A30);
  const double after = Single(t - a);
  const double on = Single(one - a);
  const double before = Single(one - t);
  const double k = Single(one / Single(on * b));
  const double out = Single(-std::fma(after, k, -one));
  double in = Single(-std::fma(before, k, -one));
  if (!(out < in)) {
    in = out;
  }
  if (in > one) {
    return full;
  }
  if (in < zero) {
    return zero;
  }
  return Single(in * full);
}

// --- BlinkTimers::Advance ---

void BlinkTimersAdvance(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC958 BlinkTimers::Advance");
  const uint64_t c24 = LoadU64(base, block + 24) + 1;
  const uint64_t c32 = LoadU64(base, block + 32) + 1;
  const uint64_t c16 = LoadU64(base, block + 16) + 1;
  const uint64_t c8 = LoadU64(base, block + 8) + 1;
  const uint32_t record = guest::LoadU32(base, block + 0);
  const double scale = LoadDouble(base, 0x820056D8);
  StoreU64(base, block + 24, c24);
  StoreU64(base, block + 32, c32);
  StoreU64(base, block + 16, c16);
  StoreU64(base, block + 8, c8);
  const double offset = LoadDouble(base, 0x820056C8);
  const double factor = LoadDouble(base, 0x820056D0);
  // The front, sides and rear: the mode at record +4, +20, +36, the rate at
  // +16, +32, +48, the counter at +16, +24, +32.
  struct Group {
    uint32_t mode, rate, counter;
    uint64_t value;
  };
  const Group groups[3] = {{4, 16, 16, c16}, {20, 32, 24, c24}, {36, 48, 32, c32}};
  for (const Group& g : groups) {
    if (int32_t(guest::LoadU32(base, record + g.mode)) < 2) {
      continue;
    }
    StoreU64(base, stack - 16, g.value);
    const double limit = std::fma(Lfs(base, record + g.rate), factor, offset) * scale;
    const double count = double(int64_t(LoadU64(base, stack - 16)));
    if (!(limit > count)) {
      StoreU64(base, block + g.counter, 0);
    }
  }
}

}  // namespace car_draw
