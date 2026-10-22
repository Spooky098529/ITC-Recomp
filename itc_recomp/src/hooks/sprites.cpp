// The race overlays' sprite functions natively: adapters in front of
// src/sprites/sprites.cpp (docs/functions/sprites.md).

#include "guest.h"

#include <bit>

#include "base/flags.h"
#include "sprites/sprites.h"

ITC_FLAG_BOOL(native_sprites, true, "ITC",
              "Run the race overlays' sprite helpers (the sprite lists' reset, "
              "sub_821298C0 and sub_8211EB60; the layer for a camera pass, sub_82126630; "
              "Vec4::Equal, sub_82123300; Trail::TurnSide, sub_82129940) natively instead of through the "
              "translated code (same results; checked by --run_harnesses=sprites)");

// SpriteList::Reset: r3 = a sprite list (500 sprites).
extern "C" REX_FUNC(sub_821298C0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_sprites)) {
    __imp__sub_821298C0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  sprites::ResetSprites(base, ctx.r3.u32, sprites::kListCorners);
}

// Sprite::Reset: r3 = a single sprite.
extern "C" REX_FUNC(sub_8211EB60) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_sprites)) {
    __imp__sub_8211EB60(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  sprites::ResetSprites(base, ctx.r3.u32, sprites::kSpriteCorners);
}

// Overlay::LayerFor: r4 = camera pass index (r3, the overlay, isn't read);
// returns the render queue layer.
extern "C" REX_FUNC(sub_82126630) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_sprites)) {
    __imp__sub_82126630(ctx, base);
    return;
  }
  ctx.r3.u64 = sprites::LayerFor(ctx.r4.u32);
}

// Vec4::Equal: r3, r4 = two 4-float vectors; returns 1 if all four
// components are equal, else 0.
extern "C" REX_FUNC(sub_82123300) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_sprites)) {
    __imp__sub_82123300(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  float a[4], b[4];
  for (int i = 0; i < 4; i++) {
    a[i] = guest::LoadFloat(base, ctx.r3.u32 + 4 * i);
    b[i] = guest::LoadFloat(base, ctx.r4.u32 + 4 * i);
  }
  ctx.r3.u64 = sprites::Equal4(a, b) ? 1 : 0;
}

// Trail::TurnSide: r3-r4 and r5-r6 = two 4-float directions in registers (two
// floats a register, the first in the high word); returns 0, 1 or -1.
extern "C" REX_FUNC(sub_82129940) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_sprites)) {
    __imp__sub_82129940(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  auto unpack = [](uint64_t high_pair, uint64_t low_pair, float out[4]) {
    out[0] = std::bit_cast<float>(uint32_t(high_pair >> 32));
    out[1] = std::bit_cast<float>(uint32_t(high_pair));
    out[2] = std::bit_cast<float>(uint32_t(low_pair >> 32));
    out[3] = std::bit_cast<float>(uint32_t(low_pair));
  };
  float a[4], b[4];
  unpack(ctx.r3.u64, ctx.r4.u64, a);
  unpack(ctx.r5.u64, ctx.r6.u64, b);
  ctx.r3.s64 = sprites::TurnSide(a, b, guest::LoadFloat(base, 0x82000A34));
}
