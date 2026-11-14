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


uint32_t IsSlotAvailable(uint8_t* base, uint32_t screen, uint64_t slot) {
  ITC_NATIVE_RUNNING("sub_821CE188 TuningScreen::IsSlotAvailable");
  if (int32_t(Load(base, screen + 9656)) == 6 && guest::LoadU8(base, screen + 9660) == 3 &&
      int32_t(Load(base, screen + 33540)) == 1) {
    return Load(base, uint32_t((slot + 6294) << 2) + screen);
  }
  return 1;
}

namespace {

namespace steps = strings_and_font::steps;

double Fmuls(double a, double b) { return double(float(a * b)); }

constexpr uint32_t kOne = 0x82000A30;        // 1.0
constexpr uint32_t kByteScale = 0x82002200;  // probably 1/255
constexpr uint32_t k255 = 0x82000C44;        // probably 255.0
constexpr uint32_t kBarShort = 0x82002C30;   // the bars' two scales
constexpr uint32_t kBarLong = 0x8200B5CC;
constexpr uint32_t kBarLength = 0x820021FC;

// std of a sign-extended word at `at` (the doubleword the original makes
// floats through), then lfd, fcfid and frsp: the single.
double ThroughFrame(uint8_t* base, uint32_t at, uint32_t word) {
  const int64_t x = int32_t(word);
  Store(base, at, uint32_t(uint64_t(x) >> 32));
  Store(base, at + 4, uint32_t(x));
  return steps::Single(double(x));
}

// A primitive's vertex `i`'s color (+8, +12, +16).
void VertexColor(uint8_t* base, uint32_t vertex, uint32_t r, uint32_t g, uint32_t b) {
  Store(base, vertex + 8, r);
  Store(base, vertex + 12, g);
  Store(base, vertex + 16, b);
}

// The RGB answer at frame + 112 scaled by 255.0 and truncated, each through
// frame + 80 (fctiwz, stfiwx, lwz).
void ColorInts(uint8_t* base, uint32_t frame, uint32_t out[3]) {
  const double scale = Lfs(base, k255);
  for (uint32_t k = 0; k < 3; k++) {
    const double c = Fmuls(Lfs(base, frame + 112 + 4 * k), scale);
    Store(base, frame + 80, uint32_t(steps::Fctiwz(c)));
    out[k] = Load(base, frame + 80);
  }
}

// Node (sub_scene, child)'s child `primitive`'s vertices 0 to `count` - 1.
template <uint32_t count>
void Vertices(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
              uint32_t primitive, uint32_t (&v)[count]) {
  const uint32_t p =
      ui_movies::Child(base, ui_movies::FindNode(base, movie, sub_scene, child), primitive);
  for (uint32_t i = 0; i < count; i++) {
    v[i] = ui_movies::PrimitiveVertex(base, p, i);
  }
}

// A bar's x (+0) of vertices 2 to 5 from the screen's word at `field`, read
// again for each: float(word) × the short scale (2, 3) or the long one (4,
// 5), × the length.
void Bar(uint8_t* base, uint32_t frame, uint32_t screen, uint32_t field, const uint32_t (&v)[6]) {
  const double length = Lfs(base, kBarLength);
  for (uint32_t i = 2; i < 6; i++) {
    const double scale = Lfs(base, i < 4 ? kBarShort : kBarLong);
    const double x = ThroughFrame(base, frame + 80, Load(base, screen + field));
    Stfs(base, v[i], Fmuls(Fmuls(x, scale), length));
  }
}

}  // namespace

uint32_t SetPaintGradients(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E9488 TuningScreen::SetPaintGradients");
  const uint32_t frame = Frame(base, sp, 208);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, 2));
  const uint32_t one = Load(base, kOne);
  Store(base, frame + 108, one);
  Store(base, frame + 100, one);
  const double scale = Lfs(base, kByteScale);
  // The value bar: h, s 1.0, v × the scale.
  const double v = ThroughFrame(base, frame + 80, Load(base, screen + 30452));
  const double h = ThroughFrame(base, frame + 88, Load(base, screen + 30444));
  Stfs(base, frame + 104, Fmuls(v, scale));
  Stfs(base, frame + 96, h);
  HsvToRgb(base, frame + 112, frame + 96, frame);
  uint32_t a[4];
  Vertices(base, movie, 21, 3, 4, a);
  uint32_t rgb[3];
  VertexColor(base, a[0], 255, 255, 255);
  VertexColor(base, a[2], 255, 255, 255);
  ColorInts(base, frame, rgb);
  VertexColor(base, a[1], rgb[0], rgb[1], rgb[2]);
  Store(base, frame + 104, one);
  VertexColor(base, a[3], rgb[0], rgb[1], rgb[2]);
  // The saturation bar: h, s × the scale, v 1.0.
  const double s = ThroughFrame(base, frame + 88, Load(base, screen + 30448));
  const double h2 = ThroughFrame(base, frame + 80, Load(base, screen + 30444));
  Stfs(base, frame + 96, h2);
  Stfs(base, frame + 100, Fmuls(s, scale));
  HsvToRgb(base, frame + 112, frame + 96, frame);
  uint32_t b[4];
  Vertices(base, movie, 21, 4, 4, b);
  VertexColor(base, b[0], 0, 0, 0);
  VertexColor(base, b[2], 0, 0, 0);
  ColorInts(base, frame, rgb);
  VertexColor(base, b[1], rgb[0], rgb[1], rgb[2]);
  VertexColor(base, b[3], rgb[0], rgb[1], rgb[2]);
  return b[3];
}

uint32_t SetLightKind0Bars(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EA130 TuningScreen::SetLightKind0Bars");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, 2));
  uint32_t v[6];
  for (uint32_t k = 0; k < 3; k++) {
    Vertices(base, movie, 23, 2 + k, 8, v);
    Bar(base, frame, screen, 30512 + 4 * k, v);
  }
  return v[5];
}

uint32_t SetLightKind1Bar(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EAE68 TuningScreen::SetLightKind1Bar");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, 2));
  uint32_t v[6];
  Vertices(base, movie, 24, 3, 8, v);
  Bar(base, frame, screen, 30552, v);
  return v[5];
}

}  // namespace tuning_pages
