// The race overlays' sprite functions natively: adapters in front of
// src/sprites/sprites.cpp (docs/functions/sprites.md).

#include "guest.h"

#include <bit>

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "sprites/sprites.h"

ITC_FLAG_BOOL(native_sprites, true, "ITC",
              "Run the race overlays' sprite helpers (the sprite lists' reset, "
              "sub_821298C0 and sub_8211EB60; the layer for a camera pass, sub_82126630; "
              "Vec4::Equal, sub_82123300; Trail::TurnSide, sub_82129940; ScreenRect::Record "
              "and Draw, sub_82128E70 and sub_82129280) natively instead of through the "
              "translated code (same results; checked by --run_harnesses=sprites)");

// AllocateSlots's diagnostics (blend_state.cpp) note each slot's caller: with
// any of them on, the recorders run the originals.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

namespace {
bool SlotDiagnostics() {
  return ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
         ITC_FLAG(log_recording_by_pass);
}

// The render queue (its vtable's slot 4 takes a recorded chunk) and the
// command list the overlays record into.
constexpr uint32_t kRenderQueue = 0x826C5D5C;
constexpr uint32_t kOverlayList = 0x826C5D60;
constexpr uint32_t kZero = 0x82000A34;  // the float 0.0, the chunk's depth
}  // namespace

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

namespace {
// ScreenRect::Record's work for the adapters below: records the rectangle at
// r3 natively, then submits the chunk through the render queue's vtable
// (slot 4: r3 = the queue, r4 = the chunk, r5 = the layer, f1 = 0.0) from a
// frame like the original's (272 bytes), with its return address. False,
// with nothing done, where the original must run instead.
bool RecordScreenRect(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t layer = ctx.r5.u32;
  const std::optional<uint32_t> chunk =
      sprites::RecordScreenRect(base, guest::LoadU32(base, kOverlayList), ctx.r3.u32);
  if (!chunk) {
    return false;
  }
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 272, 0x82129278, [&](uint32_t) {
    const uint32_t queue = guest::LoadU32(base, kRenderQueue);
    ctx.r3.u64 = queue;
    ctx.r4.u64 = *chunk;
    ctx.r5.u64 = layer;
    ctx.f1.f64 = double(guest::LoadFloat(base, kZero));
    REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, queue) + 16));
  });
  ctx.lr = lr;
  return true;
}
}  // namespace

// ScreenRect::Record: r3 = a screen rectangle, r5 = the render queue layer
// (r4, the camera pass, isn't read).
extern "C" REX_FUNC(sub_82128E70) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_sprites) || SlotDiagnostics() || !RecordScreenRect(ctx, base)) {
    __imp__sub_82128E70(ctx, base);
  }
}

// ScreenRect::Draw: r3 = a screen rectangle, r4 = camera pass, r5 = layer:
// recorded unless its +64 is set (the original tail-calls ScreenRect::Record).
extern "C" REX_FUNC(sub_82129280) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_sprites) || SlotDiagnostics()) {
    __imp__sub_82129280(ctx, base);
    return;
  }
  ctx.r11.u64 = guest::LoadU32(base, ctx.r3.u32 + 64);
  ctx.cr6.compare<int32_t>(ctx.r11.s32, 0, ctx.xer);
  if (ctx.r11.u32 != 0) {
    return;
  }
  if (!RecordScreenRect(ctx, base)) {
    __imp__sub_82128E70(ctx, base);
  }
}
