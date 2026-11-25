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
#include "schema/car_makers.h"
#include "schema/tuning_screen.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_machine {

namespace {

inline uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
inline void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
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
void Row(uint32_t at, std::initializer_list<uint32_t> words) {
  for (const uint32_t word : words) {
    Store(at, word);
    at += 4;
  }
}
void Fill(uint32_t at, uint32_t count, uint32_t word) {
  for (uint32_t i = 0; i < count; i++) {
    Store(at + 4 * i, word);
  }
}

}  // namespace

void SetPrices(uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DAD58 TuningScreen::SetPrices");
  const schema::TuningScreen ts(screen);
  // The selected garage car's price (the car at its +212).
  const uint32_t price =
      schema::GarageCar(tuning_screen::SelectedGarageCar(screen) + 212).spec_744();
  // The part tables (the screen's +34104): offsets below are from them,
  // the screen's own in the comments. The rows at +33904 (40 bytes a part
  // set) and +33584 aren't named in tuning-screen.toml: from the screen.
  const uint32_t tables = ts.part_tables_address();
  // The price's doubleword for fcfid (extsw, std at sp - 144).
  Store(sp - 144, int32_t(price) < 0 ? 0xFFFFFFFFu : 0);
  Store(sp - 140, price);
  const float f0 = float(double(int32_t(price)));
  // A part's price: the car's times the rodata float at `k`, rounded to
  // single, truncated.
  const auto part = [&](uint32_t k) -> uint32_t {
    const float product = float(double(f0) * double(guest::LoadFloat(k)));
    return uint32_t(TruncateWord(double(product)));
  };
  // The part set's rows (40 bytes at +33904, 28 at +34524), the part set
  // [+33900] read again for each store as the original (unchecked).
  const auto by_set = [&](uint32_t at, uint32_t stride, uint32_t value) {
    Store(at + stride * ts.price_class(), value);
  };
  by_set(ts.address + 33904, 40, 0);
  by_set(ts.address + 33908, 40, part(0x8200AD94));
  by_set(ts.address + 33912, 40, part(0x82002C9C));
  by_set(ts.address + 33916, 40, price);
  by_set(ts.address + 33920, 40, part(0x8200AD90));
  by_set(ts.address + 33924, 40, part(0x8200AD8C));
  by_set(ts.address + 33928, 40, part(0x820021E8));
  by_set(ts.address + 33932, 40, part(0x8200AD88));
  by_set(ts.address + 33936, 40, part(0x8200AD84));
  by_set(ts.address + 33940, 40, part(0x820021E4));
  by_set(tables + 420, 28, 0);
  by_set(tables + 424, 28, part(0x82002204));
  by_set(tables + 428, 28, part(0x82002C4C));
  by_set(tables + 432, 28, part(0x82002968));
  by_set(tables + 436, 28, part(0x82002C50));
  by_set(tables + 440, 28, part(0x82002D14));
  by_set(tables + 444, 28, part(0x82002A14));
  // The other items' prices and words, by address (the original's order
  // differs, but no two stores share a word and none is read back but
  // the aero 3 remap (+25172) below, which none of them is).
  // +34964
  Row(tables + 860, {0, part(0x82002CBC), part(0x82002208), part(0x8200AD94),
                           part(0x8200AD80), part(0x82002A14), part(0x8200295C)});
  // +35056
  Row(tables + 952, {0, part(0x82002C48), part(0x82001FBC), part(0x8200AD7C),
                           part(0x8200AD78), part(0x8200296C), part(0x82002D14)});
  // +35172
  Row(tables + 1068, {0, part(0x82002CBC), part(0x82002208), part(0x8200AD94),
                            part(0x8200AD80), part(0x82002A14), part(0x8200295C)});
  // +35260
  Row(tables + 1156, {0, part(0x82002C48), part(0x82001FBC), part(0x8200AD7C),
                            part(0x8200AD78), part(0x8200296C), part(0x82002D14)});
  // +35348
  Row(tables + 1244, {0, part(0x8200AD74), part(0x82002A10), part(0x82002C9C),
                            part(0x8200AD70), part(0x820029D4), part(0x820027A8)});
  // +35436
  Row(tables + 1332, {0, part(0x82001FBC), part(0x8200296C), part(0x82001BE4),
                            part(0x82002A3C), part(0x82002A2C), part(0x820029D4)});
  // +35528
  Row(tables + 1424, {0, part(0x82002CBC), part(0x8200AD94), part(0x82002A14),
                            part(0x82002A1C), part(0x8200AD6C), part(0x8200295C),
                            part(0x82001FBC), part(0x82001BE4), part(0x82002A2C)});
  // +35652
  Row(tables + 1548, {0, part(0x8200AD68), part(0x82001070), part(0x8200AD90),
                            part(0x8200AD64), part(0x8200AD60), part(0x820021E4)});
  // Three rows alike at +35740, +35888 and +36016.
  for (const uint32_t at : {1636u, 1784u, 1912u}) {  // +35740, +35888, +36016
    Row(tables + at, {0, 0, 0, part(0x8200AD5C), part(0x82001FBC), part(0x82002A1C),
                            part(0x8200AD58), part(0x82002208), part(0x8200AD54), part(0x82009EBC),
                            part(0x8200296C), part(0x8200AD50), part(0x8200AD6C), part(0x82002A10),
                            part(0x8200AD4C)});
  }
  const uint32_t wheel = part(0x82002FB0);
  for (const uint32_t at : {2040u, 2084u, 2128u}) {  // +36144, +36188, +36232
    Row(tables + at, {0, wheel, wheel, wheel, wheel});
  }
  Store(tables + 2172, 0);  // +36276
  // +36280 to +36296: the first 0 with the aero 3 remap (read each time).
  for (uint32_t i = 1; i < 6; i++) {
    if (ts.aero3_type_remap() != 0 && i == 1) {
      Store(tables + 2176, 0);
    } else {
      Store(tables + 2172 + 4 * i, wheel);
    }
  }
  const uint32_t other = part(0x8200256C);
  // +36328
  Row(tables + 2224, {0, 0, wheel, other, wheel, other, wheel, other, wheel, other});
  for (const uint32_t at : {2316u, 2352u, 2388u, 2424u}) {  // +36420 to +36528
    Row(tables + at, {0, wheel, wheel, wheel});
  }
  // +36564
  Row(tables + 2460, {0, part(0x82001BE4), part(0x82001BE4)});
  Store(ts.wheel_prices_address(), 0);
  Fill(ts.wheel_prices_address() + 152, 608, 50);  // +25400
  // +36604
  Row(tables + 2500, {0, 200});
  Fill(tables + 2520, 3, 0);  // +36624
  // +36652
  Row(tables + 2548, {0, 50, 50, 50});
  // +36688
  Row(tables + 2584, {0, 100, 100, 100, 100});
  // +36736
  Row(tables + 2632, {0, 100, 100, 100});
  Fill(tables + 2668, 6, 0);  // +36772
  Row(ts.address + 33584, {0, 50, 50, 50});
  Fill(ts.unknown_33640_address(), 20, 50);
  // The conversions' last words below sp (stfiwx and read back).
  Store(sp - 160, other);
  Store(sp - 156, part(0x8200AD54));
  Store(sp - 152, part(0x82009EBC));
  Store(sp - 144, part(0x8200AD4C));
}

}  // namespace tuning_machine
