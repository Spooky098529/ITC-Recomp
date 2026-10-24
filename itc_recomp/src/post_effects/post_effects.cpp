// The post effects natively (post_effects.h, docs/functions/post-effects.md).

#include "post_effects/post_effects.h"

#include "guest.h"

#include "base/log.h"
#include "command_list/command_list.h"

#include <bit>
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

// --- Recording ---

namespace {

constexpr uint32_t kOne = 0x8200882C;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0

// Whether `slots` more fit on the list: each AllocateSlots' check (the
// slots used then below the capacity); the last is the largest unless the
// count wraps, which is left to the original too.
bool Room(uint8_t* base, uint32_t list, uint32_t slots) {
  const uint32_t used = guest::LoadU32(base, list + 12);
  return used + slots < guest::LoadU32(base, list + 8) && used + slots >= used;
}

uint32_t Slot(uint8_t* base, uint32_t list) {
  return command_list::AllocateSlots(base, list, 1);
}

// A slot with [a, b, c].
uint32_t Slot3(uint8_t* base, uint32_t list, uint32_t a, uint32_t b, uint32_t c) {
  const uint32_t s = Slot(base, list);
  guest::StoreU32(base, s + 0, a);
  guest::StoreU32(base, s + 4, b);
  guest::StoreU32(base, s + 8, c);
  return s;
}

// A slot with [a, b] (+8 left as it was).
uint32_t Slot2(uint8_t* base, uint32_t list, uint32_t a, uint32_t b) {
  const uint32_t s = Slot(base, list);
  guest::StoreU32(base, s + 0, a);
  guest::StoreU32(base, s + 4, b);
  return s;
}

// Command 1 with eight render states, [state, value] each.
void RenderStates(uint8_t* base, uint32_t list, const uint32_t (&states)[8][2]) {
  Slot3(base, list, 1, 8, 0);
  for (const auto& state : states) {
    Slot2(base, list, state[0], state[1]);
  }
}

// Command 7's slot and the 16 bytes `read()` gives (four words).
template <typename Read>
uint32_t PixelConstants(uint8_t* base, uint32_t list, uint32_t reg, uint32_t count, Read read) {
  Slot3(base, list, 7, count, (reg << 8) | 0x01010000u | count);
  const uint32_t s = Slot(base, list);
  uint32_t words[4];
  read(words);
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(base, s + 4 * i, words[i]);
  }
  return s;
}

uint32_t SingleBits(double value) {
  return std::bit_cast<uint32_t>(float(value));
}

}  // namespace

std::optional<uint32_t> RecordBeginStates(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82133340 PostEffect::RecordBeginStates");
  if (!Room(base, list, 24)) {
    return std::nullopt;
  }
  Slot3(base, list, 34, 1, 0);
  Slot(base, list);  // command 34's one slot, left as it was
  // Command 2: samplers 0-4, states 0 and 4, = 2.
  Slot3(base, list, 2, 10, 0);
  for (uint32_t sampler = 0; sampler < 5; sampler++) {
    Slot3(base, list, sampler, 0, 2);
    Slot3(base, list, sampler, 4, 2);
  }
  RenderStates(base, list,
               {{96, 0}, {40, 0}, {304, 0}, {324, 1}, {308, 0}, {312, 0}, {316, 0}, {320, 0}});
  Slot3(base, list, 3, 1, 0);
  return Slot2(base, list, 0, 0x10001);
}

std::optional<uint32_t> RecordEndStates(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82133608 PostEffect::RecordEndStates");
  if (!Room(base, list, 13)) {
    return std::nullopt;
  }
  Slot3(base, list, 35, 1, 0);
  Slot(base, list);  // command 35's one slot, left as it was
  RenderStates(base, list,
               {{60, 0}, {40, 1}, {304, 1}, {324, 0}, {308, 1}, {312, 1}, {316, 1}, {320, 1}});
  Slot3(base, list, 3, 1, 0);
  return Slot2(base, list, 0, 0x10001);
}

std::optional<uint32_t> RecordTargets(uint8_t* base, uint32_t list, uint32_t surface,
                                      uint32_t depth) {
  ITC_NATIVE_RUNNING("sub_82133790 PostEffect::RecordTargets");
  if (!Room(base, list, 7)) {
    return std::nullopt;
  }
  Slot3(base, list, 23, 4, 0);
  for (uint32_t target = 0; target < 4; target++) {
    Slot2(base, list, target, surface);
  }
  Slot3(base, list, 24, 1, 0);
  const uint32_t s = Slot(base, list);
  guest::StoreU32(base, s, depth);
  return s;
}

std::optional<uint32_t> RecordPixelConstants(uint8_t* base, uint32_t list, uint32_t source,
                                             uint32_t reg, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82133D00 PostEffect::RecordPixelConstants");
  if (!Room(base, list, 2)) {
    return std::nullopt;
  }
  return PixelConstants(base, list, reg, count, [&](uint32_t (&words)[4]) {
    for (int i = 0; i < 4; i++) {
      words[i] = guest::LoadU32(base, source + 4 * i);
    }
  });
}

std::optional<uint32_t> RecordPixelBool(uint8_t* base, uint32_t list, uint32_t value,
                                        uint32_t reg) {
  ITC_NATIVE_RUNNING("sub_82133D70 PostEffect::RecordPixelBool");
  if (!Room(base, list, 2)) {
    return std::nullopt;
  }
  Slot3(base, list, 7, 1, (reg << 8) | 0x01020000u | 1u);
  const uint32_t s = command_list::AllocateBytes(base, list, 4, false);
  guest::StoreU32(base, s, value);
  return s;
}

std::optional<uint32_t> RecordTexelSize(uint8_t* base, uint32_t list, double width,
                                        double height) {
  ITC_NATIVE_RUNNING("sub_82133DD8 PostEffect::RecordTexelSize");
  if (!Room(base, list, 2)) {
    return std::nullopt;
  }
  const double one = Constant(base, kOne);
  const double zero = Constant(base, kZero);
  const uint32_t c32[4] = {SingleBits(double(float(one / width))),
                           SingleBits(double(float(one / height))), SingleBits(zero),
                           SingleBits(zero)};
  return PixelConstants(base, list, 32, 1, [&](uint32_t (&words)[4]) {
    for (int i = 0; i < 4; i++) {
      words[i] = c32[i];
    }
  });
}

uint32_t AddQuad(uint8_t* base, uint32_t effect, uint32_t width, uint32_t height, double value,
                 bool at_view) {
  ITC_NATIVE_RUNNING("sub_82133E30 PostEffect::AddQuad");
  const uint64_t buffer = guest::LoadU32(base, effect + 8060);
  const uint64_t used = guest::LoadU32(base, effect + 8056);
  guest::StoreU32(base, effect + 8056, uint32_t(used + 3));
  const uint32_t v = uint32_t((buffer * 100 + used) * 40 + effect + 56);
  uint32_t x = 0, y = 0;
  if (at_view) {
    x = guest::LoadU32(base, effect + 24);
    y = guest::LoadU32(base, effect + 28);
  }
  const double one = Constant(base, kOne);
  const double zero = Constant(base, kZero);
  // The integers as the original converts them (unsigned 32-bit, through
  // a 64-bit signed conversion), rounded to single.
  const double x0 = double(float(double(int64_t(x))));
  const double y0 = double(float(double(int64_t(y))));
  const double w = double(float(double(int64_t(width))));
  const double h = double(float(double(int64_t(height))));
  const double x1 = double(float(w + x0));
  const double y1 = double(float(h + y0));
  // Each vertex: position, 1, 1, value x3, 1, then its texture coordinates.
  const double vertices[3][10] = {
      {x0, y0, one, one, value, value, value, one, zero, zero},
      {x1, y0, one, one, value, value, value, one, one, zero},
      {x0, y1, one, one, value, value, value, one, zero, one},
  };
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      StoreSingle(base, v + 40 * i + 4 * j, vertices[i][j]);
    }
  }
  return v;
}

std::optional<uint32_t> RecordResolve(uint8_t* base, uint32_t effect, uint32_t list,
                                      uint32_t texture, uint32_t flags, uint32_t more_flags,
                                      bool by_view) {
  ITC_NATIVE_RUNNING("sub_82133F58 PostEffect::RecordResolve");
  if (!Room(base, list, 6)) {
    return std::nullopt;
  }
  Slot3(base, list, 25, 1, 0);
  const uint32_t item = command_list::AllocateResolve(base, list, false);
  if (more_flags != 0) {
    flags |= more_flags << 26;
  }
  guest::StoreU32(base, item + 0, 2);
  if (by_view) {
    guest::StoreU32(base, item + 0, 3);
    guest::StoreU32(base, item + 8, guest::LoadU32(base, effect + 24));
    guest::StoreU32(base, item + 12, guest::LoadU32(base, effect + 28));
    guest::StoreU32(base, item + 16,
                    guest::LoadU32(base, effect + 16) + guest::LoadU32(base, effect + 24));
    guest::StoreU32(base, item + 20,
                    guest::LoadU32(base, effect + 20) + guest::LoadU32(base, effect + 28));
  }
  guest::StoreU32(base, item + 4, flags);
  guest::StoreU32(base, item + 24, texture);
  guest::StoreU32(base, item + 36, 0);
  guest::StoreU32(base, item + 40, 0);
  guest::StoreU32(base, item + 52, 0);
  StoreSingle(base, item + 48, Constant(base, kZero));
  guest::StoreU8(base, item + 44, 0);
  guest::StoreU8(base, item + 45, 0);
  guest::StoreU8(base, item + 46, 0);
  guest::StoreU8(base, item + 47, 255);
  guest::StoreU32(base, item + 56, 0x18280186);
  guest::StoreU32(base, item + 60, more_flags);
  guest::StoreU32(base, item + 64, 0x1A220197);
  return item;
}

}  // namespace post_effects
