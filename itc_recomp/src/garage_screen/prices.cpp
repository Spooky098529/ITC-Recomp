// The garage screen's car value and sale price, never seen in play
// (docs/functions/garage-screen.md): see garage_screen.h.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"

namespace garage_screen {

namespace {

constexpr uint32_t kZero = 0x82000A34;        // the float 0.0
constexpr uint32_t kValueTable = 0x8200A260;  // CarValue's weights (around it)
constexpr uint32_t kPriceTable = 0x8200A348;  // SalePrice's constants

// The weight of the car's part bit `n` (0 to 453) in CarValue: the address
// of its float, or 0 for none (as the original's ranges test it).
uint32_t WeightAt(uint32_t n) {
  const uint32_t t = kValueTable;
  auto in = [n](uint32_t from, uint32_t to) { return n >= from && n <= to; };
  if (in(1, 9)) return t - 64 + 4 * n;
  if (in(11, 19)) return t - 104 + 4 * n;
  if (in(21, 26)) return t - 108 + 4 * n;
  if (in(28, 33)) return t - 136 + 4 * n;
  if (in(35, 40)) return t - 140 + 4 * n;
  if (in(42, 47)) return t - 144 + 4 * n;
  if (in(49, 54)) return t - 148 + 4 * n;
  if (in(56, 61)) return t - 152 + 4 * n;
  if (in(63, 68)) return t - 156 + 4 * n;
  if (in(70, 75)) return t - 160 + 4 * n;
  if (in(77, 79)) return t - 164 + 4 * n;
  if (in(81, 83)) return t - 168 + 4 * n;
  if (in(85, 87)) return t - 172 + 4 * n;
  if (in(89, 94)) return t - 176 + 4 * n;
  // 96 to 99 fall through to the ranges below (the original's test of 96
  // and 20 can't hold); 100 to 107 one table (its two loads the same).
  if (in(100, 107)) return t - 148 + 4 * n;
  if (in(109, 118) || in(120, 129) || in(131, 140) || in(142, 151) || in(153, 157)) {
    return t + 204;
  }
  if (in(165, 169) || in(159, 163)) return t + 212;
  if (in(171, 173) || in(175, 177) || in(179, 181) || in(183, 185)) return t + 204;
  if (in(187, 188)) return t + 232;
  return 0;
}

}  // namespace

double CarValue(uint8_t* base, uint32_t car, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82192378 GarageScreen::CarValue");
  // A leaf below the caller's frame: the count's doubleword at sp - 112.
  const double count = IntToFloat(base, sp - 112, Load(base, car + 1900));
  double f1 = Lfs(base, kZero);
  for (uint32_t n = 0; n < 454; n++) {
    // The part's bit in the words from +1904 (bit n mod 32 from the low end
    // of word n / 32).
    const uint32_t word = Load(base, car + 1904 + 4 * (n >> 5));
    if ((word & (1u << (n & 31))) == 0) {
      continue;
    }
    const uint32_t at = WeightAt(n);
    if (at != 0) {
      f1 = double(float(std::fma(count, Lfs(base, at), f1)));
    }
  }
  return f1;
}

uint64_t SalePrice(uint8_t* base, uint32_t car, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82192690 GarageScreen::SalePrice");
  const uint32_t frame = Frame(base, sp, 112);
  const double f13 = IntToFloat(base, frame + 80, Load(base, car + 1900));
  const double f8 = double(float(f13 * Lfs(base, kPriceTable)));
  const double value = CarValue(base, car, frame);
  // The factor: the first of nine thresholds (+52 on) not below the car's
  // float +1892 picks the one beside it (+84 on), the ninth's past them.
  const double mileage = Lfs(base, car + 1892);
  uint32_t i = 0;
  while (i < 8 && Lfs(base, kPriceTable + 52 + 4 * i) < mileage) {
    i++;
  }
  const double factor = Lfs(base, kPriceTable + 84 + 4 * i);
  double f0 = double(float(value * Lfs(base, 0x82002208)));
  f0 = double(float(f0 + f8));
  f0 = double(float(f0 * factor));
  Store(base, frame + 80, Fctiwz(f0));
  const uint32_t price = Load(base, frame + 80);
  return int32_t(price) < 50 ? 50 : price;
}

}  // namespace garage_screen
