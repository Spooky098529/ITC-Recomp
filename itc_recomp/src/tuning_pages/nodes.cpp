// Movie 2's nodes from the tuning pages and the light kits' color
// conversion (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "schema/material_passes.h"
#include "schema/tuning_screen.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <cmath>

namespace tuning_pages {

namespace {

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint32_t s) { return schema::TuningScreen(s); }

using G = schema::TuningPagesGlobals;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}
double Lfs(uint32_t address) {
  return double(std::bit_cast<float>(Load(address)));
}
void Stfs(uint32_t address, double value) {
  Store(address, std::bit_cast<uint32_t>(float(value)));
}

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself

}  // namespace

uint32_t ShowPricePanel(uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D9E68 TuningScreen::ShowPricePanel");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, 2));
  uint32_t node = ui_movies::FindNode(movie, kAll, 19);
  schema::UiMovieNode(node).set_visible(1);
  const int32_t state = int32_t(Screen(screen).state());
  const bool hide = state == 16 ? int32_t(Screen(screen).part_group()) == 0
                                : state == 9 || state == 10 || state == 11 || state == 14;
  if (hide) {
    node = ui_movies::FindNode(movie, kAll, 19);
    schema::UiMovieNode(node).set_visible(0);
  }
  return node;
}

uint32_t SetBrandLogo(uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EED98 TuningScreen::SetBrandLogo");
  static constexpr uint32_t kPictures[16] = {30, 7,  26, 15, 17, 16, 20, 21,
                                             19, 18, 22, 0,  12, 29, 31, 36};
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, 2));
  const uint32_t child = ui_movies::Child(ui_movies::FindNode(movie, kAll, 17), 0);
  const uint32_t brand = Screen(screen).part_group() - 1;
  if (brand <= 15) {
    schema::UiMovieNode(child).set_texture_entry(kPictures[brand]);
  }
  return child;
}

void HsvToRgb(uint32_t out, uint32_t in, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224AB50 Color::HsvToRgb");
  namespace steps = strings_and_font::steps;
  const schema::HsvColor hsv(in);  // material-passes.toml's
  const schema::PaintRgb rgb(out);
  const double s = double(hsv.saturation());
  if (s == Lfs(0x82000A34)) {
    rgb.set_red(float(double(hsv.value())));
    rgb.set_green(float(double(hsv.value())));
    rgb.set_blue(float(double(hsv.value())));
    return;
  }
  // The 16 input bytes copied below the stack pointer and read back.
  for (uint32_t i = 0; i < 16; i += 4) {
    Store(sp - 16 + i, Load(in + i));
  }
  const double h = Lfs(sp - 16);
  const double sat = Lfs(sp - 12);
  const double x = double(float(h * Lfs(0x820029E8)));
  const double v = Lfs(sp - 8);
  const double one = Lfs(0x82000A30);
  double p = double(float(one - sat));
  Store(sp - 16, uint32_t(steps::Fctiwz(x)));
  const int32_t whole = int32_t(Load(sp - 16));
  const int32_t remainder = whole % 6;
  p = double(float(p * v));
  const uint64_t wide = uint64_t(int64_t(remainder));
  Store(sp - 16, uint32_t(wide >> 32));
  Store(sp - 12, uint32_t(wide));
  const double i = double(float(double(int64_t(wide))));
  const double f = double(float(x - i));
  Store(sp - 16, uint32_t(steps::Fctiwz(i)));
  const uint32_t sector = Load(sp - 16);
  const double rest = double(float(one - f));
  double q = double(float(-std::fma(sat, f, -one)));
  double t = double(float(-std::fma(rest, sat, -one)));
  q = double(float(q * v));
  t = double(float(t * v));
  switch (sector) {
    case 0: rgb.set_green(float(t)); rgb.set_blue(float(p)); rgb.set_red(float(v)); break;
    case 1: rgb.set_red(float(q)); rgb.set_green(float(v)); rgb.set_blue(float(p)); break;
    case 2: rgb.set_red(float(p)); rgb.set_green(float(v)); rgb.set_blue(float(t)); break;
    case 3: rgb.set_red(float(p)); rgb.set_green(float(q)); rgb.set_blue(float(v)); break;
    case 4: rgb.set_red(float(t)); rgb.set_green(float(p)); rgb.set_blue(float(v)); break;
    case 5: rgb.set_green(float(p)); rgb.set_blue(float(q)); rgb.set_red(float(v)); break;
    default: return;
  }
  rgb.set_alpha(float(one));
}


uint32_t IsSlotAvailable(uint32_t screen, uint64_t slot) {
  ITC_NATIVE_RUNNING("sub_821CE188 TuningScreen::IsSlotAvailable");
  if (int32_t(Screen(screen).category()) == 6 && Screen(screen).item() == 3 &&
      int32_t(Screen(screen).interior_mode()) == 1) {
    return Load(uint32_t((slot + 6294) << 2) + screen);
  }
  return 1;
}

namespace {

namespace steps = strings_and_font::steps;

double Fmuls(double a, double b) { return double(float(a * b)); }

// std of a sign-extended word at `at` (the doubleword the original makes
// floats through), then lfd, fcfid and frsp: the single.
double ThroughFrame(uint32_t at, uint32_t word) {
  const int64_t x = int32_t(word);
  Store(at, uint32_t(uint64_t(x) >> 32));
  Store(at + 4, uint32_t(x));
  return steps::Single(double(x));
}

// A primitive's vertex `i`'s color (+8, +12, +16).
void VertexColor(uint32_t vertex, uint32_t r, uint32_t g, uint32_t b) {
  const schema::UiMovieVertex v(vertex);
  v.set_red(r);
  v.set_green(g);
  v.set_blue(b);
}

// The RGB answer at frame + 112 scaled by 255.0 and truncated, each through
// frame + 80 (fctiwz, stfiwx, lwz).
void ColorInts(uint32_t frame, uint32_t out[3]) {
  const double scale = double(G::byte_max());
  for (uint32_t k = 0; k < 3; k++) {
    const double c = Fmuls(Lfs(frame + 112 + 4 * k), scale);
    Store(frame + 80, uint32_t(steps::Fctiwz(c)));
    out[k] = Load(frame + 80);
  }
}

// Node (sub_scene, child)'s child `primitive`'s vertices 0 to `count` - 1.
template <uint32_t count>
void Vertices(uint32_t movie, uint32_t sub_scene, uint32_t child,
              uint32_t primitive, uint32_t (&v)[count]) {
  const uint32_t p =
      ui_movies::Child(ui_movies::FindNode(movie, sub_scene, child), primitive);
  for (uint32_t i = 0; i < count; i++) {
    v[i] = ui_movies::PrimitiveVertex(p, i);
  }
}

// A bar's x (+0) of vertices 2 to 5 from the screen's word at `field`, read
// again for each: float(word) × the short scale (2, 3) or the long one (4,
// 5), × the length.
void Bar(uint32_t frame, uint32_t screen, uint32_t field, const uint32_t (&v)[6]) {
  const double length = double(G::bar_length());
  for (uint32_t i = 2; i < 6; i++) {
    const double scale = (i < 4 ? double(G::bar_short()) : double(G::bar_long()));
    const double x = ThroughFrame(frame + 80, Load(screen + field));
    schema::UiMovieVertex(v[i]).set_x(float(Fmuls(Fmuls(x, scale), length)));
  }
}

}  // namespace

uint32_t SetPaintGradients(uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E9488 TuningScreen::SetPaintGradients");
  const uint32_t frame = Frame(sp, 208);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, 2));
  const uint32_t one = Load(G::one_address());
  Store(frame + 108, one);
  Store(frame + 100, one);
  const double scale = double(G::byte_scale());
  // The value bar: h, s 1.0, v × the scale.
  const double v = ThroughFrame(frame + 80, Screen(screen).paint_value());
  const double h = ThroughFrame(frame + 88, Screen(screen).paint_hue());
  Stfs(frame + 104, Fmuls(v, scale));
  Stfs(frame + 96, h);
  HsvToRgb(frame + 112, frame + 96, frame);
  uint32_t a[4];
  Vertices(movie, 21, 3, 4, a);
  uint32_t rgb[3];
  VertexColor(a[0], 255, 255, 255);
  VertexColor(a[2], 255, 255, 255);
  ColorInts(frame, rgb);
  VertexColor(a[1], rgb[0], rgb[1], rgb[2]);
  Store(frame + 104, one);
  VertexColor(a[3], rgb[0], rgb[1], rgb[2]);
  // The saturation bar: h, s × the scale, v 1.0.
  const double s = ThroughFrame(frame + 88, Screen(screen).paint_saturation());
  const double h2 = ThroughFrame(frame + 80, Screen(screen).paint_hue());
  Stfs(frame + 96, h2);
  Stfs(frame + 100, Fmuls(s, scale));
  HsvToRgb(frame + 112, frame + 96, frame);
  uint32_t b[4];
  Vertices(movie, 21, 4, 4, b);
  VertexColor(b[0], 0, 0, 0);
  VertexColor(b[2], 0, 0, 0);
  ColorInts(frame, rgb);
  VertexColor(b[1], rgb[0], rgb[1], rgb[2]);
  VertexColor(b[3], rgb[0], rgb[1], rgb[2]);
  return b[3];
}

uint32_t SetLightKind0Bars(uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EA130 TuningScreen::SetLightKind0Bars");
  const uint32_t frame = Frame(sp, 176);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, 2));
  uint32_t v[6];
  for (uint32_t k = 0; k < 3; k++) {
    Vertices(movie, 23, 2 + k, 8, v);
    Bar(frame, screen, 30512 + 4 * k, v);
  }
  return v[5];
}

uint32_t SetLightKind1Bar(uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EAE68 TuningScreen::SetLightKind1Bar");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, 2));
  uint32_t v[6];
  Vertices(movie, 24, 3, 8, v);
  Bar(frame, screen, 30552, v);
  return v[5];
}

}  // namespace tuning_pages
