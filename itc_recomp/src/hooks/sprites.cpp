// The race overlays' sprite functions natively: adapters in front of
// src/sprites/sprites.cpp (docs/functions/sprites.md).

#include "guest.h"

#include "base/flags.h"
#include "sprites/sprites.h"

ITC_FLAG_BOOL(native_sprites, true, "ITC",
              "Run the race overlays' sprite helpers (the sprite lists' reset, "
              "sub_821298C0 and sub_8211EB60) natively instead of through the "
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
