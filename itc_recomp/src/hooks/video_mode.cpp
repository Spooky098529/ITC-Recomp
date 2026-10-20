// XGetVideoMode's callers, without the import: the display's answers are ours
// (src/display/display.h). See B3 in docs/rexglue-removal.md.

#include "guest.h"
#include "guest_call.h"

#include <rex/cvar.h>

#include "display/display.h"

using guest_call::InFrame;

// TEMPORARY (B3): run the recompiled callers instead, to compare.
REXCVAR_DEFINE_BOOL(video_mode_original, false, "ITC",
                    "TEMPORARY: run the recompiled sub_82482B18 instead of the native one");

void VideoModeCheck_AfterAspect(uint8_t* base, uint32_t object, uint32_t lr, uint32_t result);

namespace {

// The device's copy of the presentation parameters (+0 width, +4 height).
constexpr uint32_t kDeviceParameters = 13188;

constexpr uint32_t kOutOfMemory = 0x8007000E;

// The aspect ratios the game picks from (its constants at 8204D2FC and
// 8204D300).
constexpr float kAspect4x3 = 4.0f / 3.0f;
constexpr float kAspect16x9 = 16.0f / 9.0f;

}  // namespace

// From sub_82483130 at boot: r3 = the object (+56 the device). Stores the
// back buffer's size as floats (+132, +136) and the display's aspect over
// the back buffer's (+140), makes a 6-index buffer (+60, indices 0-5), a
// 120-byte vertex buffer (+64) and a vertex declaration (+68), then calls
// the object's vtable +96. Returns that call's result, or E_OUTOFMEMORY if
// a resource couldn't be made. The original asked XGetVideoMode only for
// is_widescreen.
extern "C" REX_FUNC(sub_82482B18) {
  REX_FUNC_PROLOGUE();
  const uint32_t object = ctx.r3.u32, lr = ctx.lr;
  if (REXCVAR_GET(video_mode_original)) {
    __imp__sub_82482B18(ctx, base);
    VideoModeCheck_AfterAspect(base, object, lr, ctx.r3.u32);
    return;
  }
  const uint32_t device = guest::LoadU32(base, object + 56);
  const float width = float(guest::LoadU32(base, device + kDeviceParameters));
  const float height = float(guest::LoadU32(base, device + kDeviceParameters + 4));
  guest::StoreFloat(base, object + 132, width);
  guest::StoreFloat(base, object + 136, height);
  const float display_aspect = display::kWidescreen ? kAspect16x9 : kAspect4x3;
  guest::StoreFloat(base, object + 140, display_aspect / (width / height));
  uint32_t result = kOutOfMemory;
  InFrame(ctx, base, 192, 0x82482BE4, [&](uint32_t) {
    ctx.r3.u64 = 12;
    ctx.r4.u64 = 0;
    ctx.r5.u64 = 1;
    ctx.r6.u64 = 0;
    ctx.lr = 0x82482BE4;
    sub_822A6968(ctx, base);  // probably CreateIndexBuffer
    const uint32_t indices = ctx.r3.u32;
    guest::StoreU32(base, object + 60, indices);
    if (!indices) {
      return;
    }
    ctx.r3.u64 = indices;
    ctx.r4.u64 = 0;
    ctx.r5.u64 = 0;
    ctx.r6.u64 = 0;
    ctx.lr = 0x82482C10;
    sub_822A6A18(ctx, base);  // probably Lock
    const uint32_t data = ctx.r3.u32;
    for (uint16_t i = 0; i < 6; i++) {
      guest::StoreU16(base, data + 2 * i, i);
    }
    ctx.r3.u64 = guest::LoadU32(base, object + 60);
    ctx.lr = 0x82482C44;
    sub_822A6A78(ctx, base);  // probably Unlock
    ctx.r3.u64 = 120;
    ctx.r4.u64 = 0;
    ctx.r5.u64 = 0;
    ctx.lr = 0x82482C54;
    sub_822A6830(ctx, base);  // probably CreateVertexBuffer
    guest::StoreU32(base, object + 64, ctx.r3.u32);
    if (!ctx.r3.u32) {
      return;
    }
    ctx.r3.u64 = 0x8264D550;  // the declaration's elements
    ctx.lr = 0x82482C6C;
    sub_822A8980(ctx, base);  // probably CreateVertexDeclaration
    guest::StoreU32(base, object + 68, ctx.r3.u32);
    if (!ctx.r3.u32) {
      return;
    }
    const uint32_t vtable = guest::LoadU32(base, object);
    ctx.r3.u64 = object;
    ctx.lr = 0x82482C8C;
    REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, vtable + 96));
    result = ctx.r3.u32;
  });
  ctx.r3.u64 = result;
  VideoModeCheck_AfterAspect(base, object, lr, ctx.r3.u32);
}
