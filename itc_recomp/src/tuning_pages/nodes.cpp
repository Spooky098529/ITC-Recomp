// Movie 2's nodes from the tuning pages and the light kits' color
// conversion (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <cmath>

namespace tuning_pages {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}
double Lfs(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(Load(base, address)));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  Store(base, address, std::bit_cast<uint32_t>(float(value)));
}

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself

}  // namespace

uint32_t ShowPricePanel(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D9E68 TuningScreen::ShowPricePanel");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, 2));
  uint32_t node = ui_movies::FindNode(base, movie, kAll, 19);
  Store(base, node + 80, 1);
  const int32_t state = int32_t(Load(base, screen + 9652));
  const bool hide = state == 16 ? int32_t(Load(base, screen + 25204)) == 0
                                : state == 9 || state == 10 || state == 11 || state == 14;
  if (hide) {
    node = ui_movies::FindNode(base, movie, kAll, 19);
    Store(base, node + 80, 0);
  }
  return node;
}

uint32_t SetBrandLogo(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EED98 TuningScreen::SetBrandLogo");
  static constexpr uint32_t kPictures[16] = {30, 7,  26, 15, 17, 16, 20, 21,
                                             19, 18, 22, 0,  12, 29, 31, 36};
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, 2));
  const uint32_t child = ui_movies::Child(base, ui_movies::FindNode(base, movie, kAll, 17), 0);
  const uint32_t brand = Load(base, screen + 25204) - 1;
  if (brand <= 15) {
    Store(base, child + 108, kPictures[brand]);
  }
  return child;
}

void HsvToRgb(uint8_t* base, uint32_t out, uint32_t in, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224AB50 Color::HsvToRgb");
  namespace steps = strings_and_font::steps;
  const double s = Lfs(base, in + 4);
  if (s == Lfs(base, 0x82000A34)) {
    Stfs(base, out + 0, Lfs(base, in + 8));
    Stfs(base, out + 4, Lfs(base, in + 8));
    Stfs(base, out + 8, Lfs(base, in + 8));
    return;
  }
  // The 16 input bytes copied below the stack pointer and read back.
  for (uint32_t i = 0; i < 16; i += 4) {
    Store(base, sp - 16 + i, Load(base, in + i));
  }
  const double h = Lfs(base, sp - 16);
  const double sat = Lfs(base, sp - 12);
  const double x = double(float(h * Lfs(base, 0x820029E8)));
  const double v = Lfs(base, sp - 8);
  const double one = Lfs(base, 0x82000A30);
  double p = double(float(one - sat));
  Store(base, sp - 16, uint32_t(steps::Fctiwz(x)));
  const int32_t whole = int32_t(Load(base, sp - 16));
  const int32_t remainder = whole % 6;
  p = double(float(p * v));
  const uint64_t wide = uint64_t(int64_t(remainder));
  Store(base, sp - 16, uint32_t(wide >> 32));
  Store(base, sp - 12, uint32_t(wide));
  const double i = double(float(double(int64_t(wide))));
  const double f = double(float(x - i));
  Store(base, sp - 16, uint32_t(steps::Fctiwz(i)));
  const uint32_t sector = Load(base, sp - 16);
  const double rest = double(float(one - f));
  double q = double(float(-std::fma(sat, f, -one)));
  double t = double(float(-std::fma(rest, sat, -one)));
  q = double(float(q * v));
  t = double(float(t * v));
  switch (sector) {
    case 0: Stfs(base, out + 4, t); Stfs(base, out + 8, p); Stfs(base, out + 0, v); break;
    case 1: Stfs(base, out + 0, q); Stfs(base, out + 4, v); Stfs(base, out + 8, p); break;
    case 2: Stfs(base, out + 0, p); Stfs(base, out + 4, v); Stfs(base, out + 8, t); break;
    case 3: Stfs(base, out + 0, p); Stfs(base, out + 4, q); Stfs(base, out + 8, v); break;
    case 4: Stfs(base, out + 0, t); Stfs(base, out + 4, p); Stfs(base, out + 8, v); break;
    case 5: Stfs(base, out + 4, p); Stfs(base, out + 8, q); Stfs(base, out + 0, v); break;
    default: return;
  }
  Stfs(base, out + 12, one);
}

}  // namespace tuning_pages
