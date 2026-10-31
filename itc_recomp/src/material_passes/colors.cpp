// The paint colors' conversions from hue, saturation and value (see
// material_passes.h).

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

constexpr uint32_t kOne = 0x82000A30;        // 1.0
constexpr uint32_t kZero = 0x82000A34;       // 0.0
constexpr uint32_t kSixtieth = 0x820029E8;  // 1/60

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
  if (Lfs(base, color + 4) == Lfs(base, kZero)) {
    Stfs(base, out + 0, Lfs(base, color + 8));
    Stfs(base, out + 4, Lfs(base, color + 8));
    Stfs(base, out + 8, Lfs(base, color + 8));
    return;
  }
  const uint32_t temp = sp - 16;
  const uint64_t hs = Load64(base, color + 0);
  const uint64_t va = Load64(base, color + 8);
  Store64(base, temp + 0, hs);
  Store64(base, temp + 8, va);
  const double hue = Lfs(base, temp + 0);
  const double saturation = Lfs(base, temp + 4);
  const double h = Fmuls(hue, Lfs(base, kSixtieth));
  const double v = Lfs(base, temp + 8);
  const double one = Lfs(base, kOne);
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
      Stfs(base, out + 4, t);
      Stfs(base, out + 8, p);
      Stfs(base, out + 0, v);
      break;
    case 1:
      Stfs(base, out + 0, q);
      Stfs(base, out + 4, v);
      Stfs(base, out + 8, p);
      break;
    case 2:
      Stfs(base, out + 0, p);
      Stfs(base, out + 4, v);
      Stfs(base, out + 8, t);
      break;
    case 3:
      Stfs(base, out + 0, p);
      Stfs(base, out + 4, q);
      Stfs(base, out + 8, v);
      break;
    case 4:
      Stfs(base, out + 0, t);
      Stfs(base, out + 4, p);
      Stfs(base, out + 8, v);
      break;
    case 5:
      Stfs(base, out + 4, p);
      Stfs(base, out + 8, q);
      Stfs(base, out + 0, v);
      break;
    default:
      return;
  }
  Stfs(base, out + 12, one);
}

void HsvToRgb8(uint8_t* base, uint32_t out, uint32_t color) {
  ITC_NATIVE_RUNNING("sub_820C9808 Color::HsvToRgb8");
  const uint32_t s = guest::LoadU8(base, color + 2);
  if (s == 0) {
    guest::StoreU8(base, out + 0, guest::LoadU8(base, color + 3));
    guest::StoreU8(base, out + 1, guest::LoadU8(base, color + 3));
    guest::StoreU8(base, out + 2, guest::LoadU8(base, color + 3));
    return;
  }
  const int32_t hue = int16_t(guest::LoadU16(base, color + 0));
  const int32_t v = guest::LoadU8(base, color + 3);
  const int32_t h6 = int16_t(uint32_t(hue) * 6u);
  const int32_t sector = h6 / 360;
  const int32_t r = int16_t(uint32_t(h6) - uint32_t(sector) * 360u);
  const int32_t si = int32_t(s);
  const uint8_t p = uint8_t((255 - si) * v / 255);
  const uint8_t q = uint8_t((255 - r * si / 360) * v / 255);
  const uint8_t t = uint8_t((255 - (360 - r) * si / 360) * v / 255);
  switch (uint32_t(sector)) {
    case 0: {
      const uint8_t value = guest::LoadU8(base, color + 3);
      guest::StoreU8(base, out + 1, t);
      guest::StoreU8(base, out + 0, value);
      guest::StoreU8(base, out + 2, p);
      break;
    }
    case 1:
      guest::StoreU8(base, out + 0, q);
      guest::StoreU8(base, out + 1, guest::LoadU8(base, color + 3));
      guest::StoreU8(base, out + 2, p);
      break;
    case 2: {
      guest::StoreU8(base, out + 0, p);
      const uint8_t value = guest::LoadU8(base, color + 3);
      guest::StoreU8(base, out + 2, t);
      guest::StoreU8(base, out + 1, value);
      break;
    }
    case 3:
      guest::StoreU8(base, out + 0, p);
      guest::StoreU8(base, out + 1, q);
      guest::StoreU8(base, out + 2, guest::LoadU8(base, color + 3));
      break;
    case 4:
      guest::StoreU8(base, out + 0, t);
      guest::StoreU8(base, out + 1, p);
      guest::StoreU8(base, out + 2, guest::LoadU8(base, color + 3));
      break;
    default: {
      const uint8_t value = guest::LoadU8(base, color + 3);
      guest::StoreU8(base, out + 1, p);
      guest::StoreU8(base, out + 0, value);
      guest::StoreU8(base, out + 2, q);
      break;
    }
  }
}

}  // namespace material_passes
