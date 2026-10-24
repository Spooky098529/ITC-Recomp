// The post effects natively (post_effects.h, docs/functions/post-effects.md).

#include "post_effects/post_effects.h"

#include "guest.h"

#include "base/log.h"

#include <cmath>

namespace post_effects {

namespace {

// The single-precision constants the originals load (lfs): read from the
// image, as they do.
double Constant(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}

void StoreSingle(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

}  // namespace

uint32_t Get(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82133AE8 PostEffects::Get");
  return guest::LoadU32(base, kEffects + (index << 2));
}

void MarkUpdated(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82133A08 PostEffects::MarkUpdated");
  for (uint32_t i = 0; i < kCount; i++) {
    const uint32_t effect = guest::LoadU32(base, kEffects + 4 * i);
    if (effect != 0) {
      guest::StoreU32(base, effect + 48, 1);
    }
  }
}

void ToggleNoHistory(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82133A38 PostEffects::ToggleNoHistory");
  for (uint32_t i = 0; i < kCount; i++) {
    const uint32_t effect = guest::LoadU32(base, kEffects + 4 * i);
    if (effect != 0) {
      guest::StoreU32(base, effect + 44, guest::LoadU32(base, effect + 44) == 0 ? 1 : 0);
    }
  }
}

void ToggleScreenObjectHidden(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8212EDE0 ScreenObject::ToggleHidden");
  guest::StoreU8(base, object + 25, guest::LoadU8(base, object + 25) == 0 ? 1 : 0);
}

uint32_t ToggleRaceEffects(uint8_t* base, uint32_t race) {
  ITC_NATIVE_RUNNING("sub_8215B690 Race::ToggleEffects");
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    r3 = guest::LoadU32(base, race + 31152 + 4 * i);
    if (r3 != 0) {
      ToggleScreenObjectHidden(base, r3);
    }
  }
  ToggleNoHistory(base);
  const uint32_t scenery = guest::LoadU32(base, 0x826C551C);
  guest::StoreU32(base, scenery + 196, guest::LoadU32(base, scenery + 196) ^ 0x4000u);
  const uint32_t scene = guest::LoadU32(base, 0x826C2E44);
  guest::StoreU32(base, scene + 5048, guest::LoadU32(base, scene + 5048) == 0 ? 1 : 0);
  return r3;
}

uint32_t DestroyRace(uint8_t* base, const std::function<uint32_t(uint32_t, uint32_t)>& destroy) {
  ITC_NATIVE_RUNNING("sub_82133A70 PostEffects::DestroyRace");
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < kCount; i++) {
    const uint32_t at = kEffects + 4 * i;
    r3 = guest::LoadU32(base, at);
    if (r3 == 0 || (guest::LoadU32(base, r3 + 32) & 4) != 0) {
      continue;
    }
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, r3) + 4);
    r3 = destroy(function, r3);
    guest::StoreU32(base, at, 0);
  }
  return r3;
}

void GlowSet(uint8_t* base, uint32_t glow, uint32_t offset, double value) {
  switch (offset) {
    case 8436: ITC_NATIVE_RUNNING("sub_82136EF8 Glow::Set8436"); break;
    case 8440: ITC_NATIVE_RUNNING("sub_82136F00 Glow::Set8440"); break;
    case 8444: ITC_NATIVE_RUNNING("sub_82136F08 Glow::Set8444"); break;
    case 8448: ITC_NATIVE_RUNNING("sub_82136F10 Glow::Set8448"); break;
    case 8452: ITC_NATIVE_RUNNING("sub_82136F18 Glow::SetStrength"); break;
    case 8456: ITC_NATIVE_RUNNING("sub_82136F20 Glow::Set8456"); break;
    case 8460: ITC_NATIVE_RUNNING("sub_82136F28 Glow::SetHistoryWeight"); break;
    case 8464: ITC_NATIVE_RUNNING("sub_82136F30 Glow::Set8464"); break;
    case 8468: ITC_NATIVE_RUNNING("sub_82136F38 Glow::Set8468"); break;
    case 8472: ITC_NATIVE_RUNNING("sub_82136F40 Glow::Set8472"); break;
    case 8476: ITC_NATIVE_RUNNING("sub_82136F48 Glow::Set8476"); break;
    default: break;
  }
  StoreSingle(base, glow + offset, value);
}

void GlowSetFromSpeed(uint8_t* base, uint32_t glow, double speed,
                      const std::function<void(uint32_t, double)>& set) {
  ITC_NATIVE_RUNNING("sub_82133878 Glow::SetFromSpeed");
  // t = (speed - 120) / 130 (as a multiply), clamped to [0, 1] (a NaN
  // stays).
  const double offset = Constant(base, 0x82002C38);
  const double scale = Constant(base, 0x82008844);
  double t = double(float(double(float(speed - offset)) * scale));
  const double low = Constant(base, 0x82000A34);
  const double one = Constant(base, 0x8200882C);
  if (t < low) {
    t = low;
  } else if (t > one) {
    t = one;
  }
  auto slot = [&](uint32_t byte) {
    return guest::LoadU32(base, guest::LoadU32(base, glow) + byte);
  };
  set(slot(8), double(float(-std::fma(t, Constant(base, 0x82002A64), -one))));
  set(slot(24), double(float(std::fma(t, Constant(base, 0x82002574), Constant(base, 0x82008830)))));
  set(slot(32), double(float(std::fma(t, Constant(base, 0x82008840), Constant(base, 0x82008834)))));
}

void ColorFilterSetColor0(uint8_t* base, uint32_t filter, double r, double g, double b) {
  ITC_NATIVE_RUNNING("sub_821350F8 ColorFilter::SetColor0");
  StoreSingle(base, filter + 8080, r);
  StoreSingle(base, filter + 8084, g);
  StoreSingle(base, filter + 8088, b);
  StoreSingle(base, filter + 8092, Constant(base, 0x82000A34));
}

void ColorFilterSet(uint8_t* base, uint32_t filter, uint32_t offset, double value) {
  switch (offset) {
    case 8112: ITC_NATIVE_RUNNING("sub_82135138 ColorFilter::Set8112"); break;
    case 8116: ITC_NATIVE_RUNNING("sub_82135140 ColorFilter::Set8116"); break;
    case 8120: ITC_NATIVE_RUNNING("sub_82135148 ColorFilter::Set8120"); break;
    default: break;
  }
  StoreSingle(base, filter + offset, value);
}

}  // namespace post_effects
