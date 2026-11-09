// The tuning screen's prices (docs/functions/tuning-machine.md): see
// tuning_machine.h. Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt): each price is the translated code's product of the
// car's price (fcfid, frsp) and a rodata float, rounded to single (fmuls)
// and truncated as fctiwz.

#include "tuning_machine/tuning_machine.h"

#include <climits>
#include <cmath>
#include <initializer_list>

#include "base/log.h"
#include "guest.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_machine {

namespace {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// fctiwz as the recompiled code does it: NaN 0x80000000, from INT_MAX
// saturated, below INT_MIN 0x80000000 (cvttsd2si).
inline int32_t TruncateWord(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value < -2147483648.0) {
    return INT32_MIN;
  }
  return int32_t(value);
}

// Consecutive words from `at`.
void Row(uint8_t* base, uint32_t at, std::initializer_list<uint32_t> words) {
  for (const uint32_t word : words) {
    Store(base, at, word);
    at += 4;
  }
}
void Fill(uint8_t* base, uint32_t at, uint32_t count, uint32_t word) {
  for (uint32_t i = 0; i < count; i++) {
    Store(base, at + 4 * i, word);
  }
}

}  // namespace

void SetPrices(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DAD58 TuningScreen::SetPrices");
  const uint32_t s = screen;
  const uint32_t price = Load(base, tuning_screen::SelectedGarageCar(base, s) + 2112);
  // The price's doubleword for fcfid (extsw, std at sp - 144).
  Store(base, sp - 144, int32_t(price) < 0 ? 0xFFFFFFFFu : 0);
  Store(base, sp - 140, price);
  const float f0 = float(double(int32_t(price)));
  // A part's price: the car's times the rodata float at `k`, rounded to
  // single, truncated.
  const auto part = [&](uint32_t k) -> uint32_t {
    const float product = float(double(f0) * double(guest::LoadFloat(base, k)));
    return uint32_t(TruncateWord(double(product)));
  };
  // The part set's rows (40 bytes at +33904, 28 at +34524), the part set
  // [+33900] read again for each store as the original (unchecked).
  const auto by_set = [&](uint32_t offset, uint32_t stride, uint32_t value) {
    Store(base, s + offset + stride * Load(base, s + tuning_screen::kPartSet), value);
  };
  by_set(33904, 40, 0);
  by_set(33908, 40, part(0x8200AD94));
  by_set(33912, 40, part(0x82002C9C));
  by_set(33916, 40, price);
  by_set(33920, 40, part(0x8200AD90));
  by_set(33924, 40, part(0x8200AD8C));
  by_set(33928, 40, part(0x820021E8));
  by_set(33932, 40, part(0x8200AD88));
  by_set(33936, 40, part(0x8200AD84));
  by_set(33940, 40, part(0x820021E4));
  by_set(34524, 28, 0);
  by_set(34528, 28, part(0x82002204));
  by_set(34532, 28, part(0x82002C4C));
  by_set(34536, 28, part(0x82002968));
  by_set(34540, 28, part(0x82002C50));
  by_set(34544, 28, part(0x82002D14));
  by_set(34548, 28, part(0x82002A14));
  // The other items' prices and words, by address (the original's order
  // differs, but no two stores share a word and none is read back but
  // +25172 below, which none of them is).
  Row(base, s + 34964, {0, part(0x82002CBC), part(0x82002208), part(0x8200AD94),
                        part(0x8200AD80), part(0x82002A14), part(0x8200295C)});
  Row(base, s + 35056, {0, part(0x82002C48), part(0x82001FBC), part(0x8200AD7C),
                        part(0x8200AD78), part(0x8200296C), part(0x82002D14)});
  Row(base, s + 35172, {0, part(0x82002CBC), part(0x82002208), part(0x8200AD94),
                        part(0x8200AD80), part(0x82002A14), part(0x8200295C)});
  Row(base, s + 35260, {0, part(0x82002C48), part(0x82001FBC), part(0x8200AD7C),
                        part(0x8200AD78), part(0x8200296C), part(0x82002D14)});
  Row(base, s + 35348, {0, part(0x8200AD74), part(0x82002A10), part(0x82002C9C),
                        part(0x8200AD70), part(0x820029D4), part(0x820027A8)});
  Row(base, s + 35436, {0, part(0x82001FBC), part(0x8200296C), part(0x82001BE4),
                        part(0x82002A3C), part(0x82002A2C), part(0x820029D4)});
  Row(base, s + 35528, {0, part(0x82002CBC), part(0x8200AD94), part(0x82002A14),
                        part(0x82002A1C), part(0x8200AD6C), part(0x8200295C),
                        part(0x82001FBC), part(0x82001BE4), part(0x82002A2C)});
  Row(base, s + 35652, {0, part(0x8200AD68), part(0x82001070), part(0x8200AD90),
                        part(0x8200AD64), part(0x8200AD60), part(0x820021E4)});
  // Three rows alike at +35740, +35888 and +36016.
  for (const uint32_t at : {35740u, 35888u, 36016u}) {
    Row(base, s + at, {0, 0, 0, part(0x8200AD5C), part(0x82001FBC), part(0x82002A1C),
                       part(0x8200AD58), part(0x82002208), part(0x8200AD54), part(0x82009EBC),
                       part(0x8200296C), part(0x8200AD50), part(0x8200AD6C), part(0x82002A10),
                       part(0x8200AD4C)});
  }
  const uint32_t wheel = part(0x82002FB0);
  for (const uint32_t at : {36144u, 36188u, 36232u}) {
    Row(base, s + at, {0, wheel, wheel, wheel, wheel});
  }
  Store(base, s + 36276, 0);
  // +36280 to +36296: the first 0 when +25172 is set (read each time).
  for (uint32_t i = 1; i < 6; i++) {
    if (Load(base, s + 25172) != 0 && i == 1) {
      Store(base, s + 36280, 0);
    } else {
      Store(base, s + (9069 + i) * 4, wheel);
    }
  }
  const uint32_t other = part(0x8200256C);
  Row(base, s + 36328, {0, 0, wheel, other, wheel, other, wheel, other, wheel, other});
  for (const uint32_t at : {36420u, 36456u, 36492u, 36528u}) {
    Row(base, s + at, {0, wheel, wheel, wheel});
  }
  Row(base, s + 36564, {0, part(0x82001BE4), part(0x82001BE4)});
  Store(base, s + 25248, 0);
  Fill(base, s + 25400, 608, 50);
  Row(base, s + 36604, {0, 200});
  Fill(base, s + 36624, 3, 0);
  Row(base, s + 36652, {0, 50, 50, 50});
  Row(base, s + 36688, {0, 100, 100, 100, 100});
  Row(base, s + 36736, {0, 100, 100, 100});
  Fill(base, s + 36772, 6, 0);
  Row(base, s + 33584, {0, 50, 50, 50});
  Fill(base, s + 33640, 20, 50);
  // The conversions' last words below sp (stfiwx and read back).
  Store(base, sp - 160, other);
  Store(base, sp - 156, part(0x8200AD54));
  Store(base, sp - 152, part(0x82009EBC));
  Store(base, sp - 144, part(0x8200AD4C));
}

}  // namespace tuning_machine
