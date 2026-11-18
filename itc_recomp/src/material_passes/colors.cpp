// The paint colors' conversions from hue, saturation and value (see
// material_passes.h).

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/material_passes.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

using G = schema::MaterialPassesGlobals;

uint64_t Load64(uint8_t* base, uint32_t a) {
  return (uint64_t(guest::LoadU32(base, a)) << 32) | guest::LoadU32(base, a + 4);
}
void Store64(uint8_t* base, uint32_t a, uint64_t value) {
  guest::StoreU32(base, a, uint32_t(value >> 32));
  guest::StoreU32(base, a + 4, uint32_t(value));
}

}  // namespace

void HsvToRgb(uint8_t* base, uint32_t out, uint32_t color, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D0D60 Color::HsvToRgb");
  const schema::PaintRgb rgb(base, out);
  const schema::HsvColor hsv(base, color);
  const double saturation0 = double(hsv.saturation());
  if (saturation0 == double(G::zero(base))) {
    rgb.set_red(float(double(hsv.value())));
    rgb.set_green(float(double(hsv.value())));
    rgb.set_blue(float(double(hsv.value())));
    return;
  }
  const uint32_t temp = sp - 16;
  const uint64_t hs = hsv.hue_saturation();
  const uint64_t va = hsv.value_w();
  Store64(base, temp + 0, hs);
  Store64(base, temp + 8, va);
  const double hue = Lfs(base, temp + 0);
  const double saturation = Lfs(base, temp + 4);
  const double h = Fmuls(hue, double(G::sixtieth(base)));
  const double v = Lfs(base, temp + 8);
  const double one = double(G::one(base));
  double p = Fsubs(one, saturation);
  guest::StoreU32(base, temp, uint32_t(Fctiwz(h)));
  const int32_t whole = int32_t(guest::LoadU32(base, temp));
  p = Fmuls(p, v);
  const int64_t sector64 = int64_t(int32_t(uint32_t(whole) - uint32_t(whole / 6) * 6u));
  Store64(base, temp, uint64_t(sector64));
  const double sector_float = Single(double(int64_t(Load64(base, temp))));
  const double f = Fsubs(h, sector_float);
  guest::StoreU32(base, temp, uint32_t(Fctiwz(sector_float)));
  const uint32_t sector = guest::LoadU32(base, temp);
  const double rest = Fsubs(one, f);
  double q = Fnmsubs(saturation, f, one);
  double t = Fnmsubs(rest, saturation, one);
  q = Fmuls(q, v);
  t = Fmuls(t, v);
  switch (sector) {
    case 0:
      rgb.set_green(float(t));
      rgb.set_blue(float(p));
      rgb.set_red(float(v));
      break;
    case 1:
      rgb.set_red(float(q));
      rgb.set_green(float(v));
      rgb.set_blue(float(p));
      break;
    case 2:
      rgb.set_red(float(p));
      rgb.set_green(float(v));
      rgb.set_blue(float(t));
      break;
    case 3:
      rgb.set_red(float(p));
      rgb.set_green(float(q));
      rgb.set_blue(float(v));
      break;
    case 4:
      rgb.set_red(float(t));
      rgb.set_green(float(p));
      rgb.set_blue(float(v));
      break;
    case 5:
      rgb.set_green(float(p));
      rgb.set_blue(float(q));
      rgb.set_red(float(v));
      break;
    default:
      return;
  }
  rgb.set_alpha(float(one));
}

void HsvToRgb8(uint8_t* base, uint32_t out, uint32_t color) {
  ITC_NATIVE_RUNNING("sub_820C9808 Color::HsvToRgb8");
  const schema::PaintRgb rgb(base, out);
  const schema::PackedHsv hsv(base, color);
  const uint32_t s = hsv.saturation();
  if (s == 0) {
    rgb.set_red8(hsv.value());
    rgb.set_green8(hsv.value());
    rgb.set_blue8(hsv.value());
    return;
  }
  const int32_t hue = hsv.hue();
  const int32_t v = hsv.value();
  const int32_t h6 = int16_t(uint32_t(hue) * 6u);
  const int32_t sector = h6 / 360;
  const int32_t r = int16_t(uint32_t(h6) - uint32_t(sector) * 360u);
  const int32_t si = int32_t(s);
  const uint8_t p = uint8_t((255 - si) * v / 255);
  const uint8_t q = uint8_t((255 - r * si / 360) * v / 255);
  const uint8_t t = uint8_t((255 - (360 - r) * si / 360) * v / 255);
  switch (uint32_t(sector)) {
    case 0: {
      const uint8_t value = hsv.value();
      rgb.set_green8(t);
      rgb.set_red8(value);
      rgb.set_blue8(p);
      break;
    }
    case 1:
      rgb.set_red8(q);
      rgb.set_green8(hsv.value());
      rgb.set_blue8(p);
      break;
    case 2: {
      rgb.set_red8(p);
      const uint8_t value = hsv.value();
      rgb.set_blue8(t);
      rgb.set_green8(value);
      break;
    }
    case 3:
      rgb.set_red8(p);
      rgb.set_green8(q);
      rgb.set_blue8(hsv.value());
      break;
    case 4:
      rgb.set_red8(t);
      rgb.set_green8(p);
      rgb.set_blue8(hsv.value());
      break;
    default: {
      const uint8_t value = hsv.value();
      rgb.set_green8(p);
      rgb.set_red8(value);
      rgb.set_blue8(q);
      break;
    }
  }
}

}  // namespace material_passes
