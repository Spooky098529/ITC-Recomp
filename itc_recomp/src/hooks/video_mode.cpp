// XGetVideoMode's callers, without the import: the display's answers are ours
// (src/display/display.h). See B3 in docs/rexglue-removal.md.

#include "guest.h"
#include "guest_call.h"

#include <rex/cvar.h>

#include <algorithm>

#include "display/display.h"

using guest_call::InFrame;

// TEMPORARY (B3): run the recompiled callers instead, to compare.
REXCVAR_DEFINE_BOOL(video_mode_original, false, "ITC",
                    "TEMPORARY: run the recompiled sub_8258D568 and sub_82482B18 instead of "
                    "the native ones");

void VideoModeCheck_AfterDisplaySetup(uint8_t* base, uint32_t object, uint32_t lr, uint32_t r6,
                                      uint32_t result);
void VideoModeCheck_AfterAspect(uint8_t* base, uint32_t object, uint32_t lr, uint32_t result);

namespace {

// The device's copy of the presentation parameters (+0 width, +4 height).
constexpr uint32_t kDeviceParameters = 13188;

constexpr uint32_t kOutOfMemory = 0x8007000E;

// The video output (sub_8258D568's object, Renderer + 64).
constexpr uint32_t kVideoParameters = 16;  // the 124 bytes for Device::Init
constexpr uint32_t kVideoAnswer = 152;  // where XGetVideoMode's answer went (48 bytes)
constexpr uint32_t kVideoDevice = 140;
constexpr uint32_t kVideoSafeArea = 208;  // x, y, width, height
constexpr uint32_t kVideoFrontBuffers = 272;  // two
constexpr uint32_t kVideoWidth = 296;
constexpr uint32_t kVideoHeight = 300;
constexpr uint32_t kVideoInterval = 304;

// The game's constants: the viewport's depth range (82000A34, 82000A30)
// and the safe area's edges as fractions of the screen (82002204,
// 82002A34).
constexpr float kMinZ = 0.0f;
constexpr float kMaxZ = 1.0f;
constexpr float kSafeNear = 0.05f;
constexpr float kSafeFar = 0.95f;

// The vblank callback the set-up gives the device (+15144, by
// sub_8229DA20): counts vblanks and signals an event.
constexpr uint32_t kVblankCallback = 0x8258D4D8;

// The aspect ratios the game picks from (its constants at 8204D2FC and
// 8204D300).
constexpr float kAspect4x3 = 4.0f / 3.0f;
constexpr float kAspect16x9 = 16.0f / 9.0f;

}  // namespace

// The graphics library's display set-up, once from Renderer::Init: r3 = the
// video output, r6 = the vblank interval (2). Fills the presentation
// parameters, makes the device and the two front buffers, sets the viewport
// and a few states, gives the device its vblank callback, and works out
// the safe area. Returns the device, or 0 if it couldn't be made. The
// original also had XGetVideoMode write its answer at +152..+199, which
// nothing reads (checked by overwriting it); it's left out, the bytes
// zeroed as the original did before asking.
extern "C" REX_FUNC(sub_8258D568) {
  REX_FUNC_PROLOGUE();
  const uint32_t object = ctx.r3.u32, interval = ctx.r6.u32, lr = ctx.lr;
  if (REXCVAR_GET(video_mode_original)) {
    __imp__sub_8258D568(ctx, base);
    VideoModeCheck_AfterDisplaySetup(base, object, lr, interval, ctx.r3.u32);
    return;
  }
  auto set = [&](uint32_t offset, uint32_t value) { guest::StoreU32(base, object + offset, value); };
  auto field = [&](uint32_t offset) { return guest::LoadU32(base, object + offset); };
  for (uint32_t i = 0; i < 48; i += 4) {
    set(kVideoAnswer + i, 0);
  }
  const uint32_t parameters = object + kVideoParameters;
  uint32_t result = 0;
  InFrame(ctx, base, 192, 0x8258D5B8, [&](uint32_t frame) {
    ctx.r3.u64 = parameters;
    ctx.r4.u64 = 0;
    ctx.r5.u64 = 124;
    ctx.lr = 0x8258D5B8;
    sub_825A71D0(ctx, base);  // memset
    // The parameters (see Device::SetUpDisplay in graphics_device.cpp).
    set(16, display::kWidth);
    set(20, display::kHeight);
    set(24, 0x18280186);  // the render target's format
    set(28, 0);
    set(32, 0);
    set(40, 1);
    set(52, 1);  // a depth surface
    set(56, 0x1A220197);  // its format
    set(68, 0x80000000);
    set(76, 1);  // no front buffer from the library
    if (interval == 0 || interval == 1) {
      set(kVideoInterval, 1);
    } else if (interval == 2) {
      set(kVideoInterval, 2);
    } else if (interval == 4) {
      set(kVideoInterval, 3);
    } else if (interval == 0x80000000) {
      set(kVideoInterval, 0);
    }
    auto allocate = [&](uint32_t bytes, uint32_t protection, uint32_t return_address) {
      ctx.r3.u64 = bytes;
      ctx.r4.u64 = 0;
      ctx.r5.u64 = 0xFFFFFFFF;
      ctx.r6.u64 = 0;
      ctx.r7.u64 = protection;
      ctx.lr = return_address;
      sub_82296550(ctx, base);  // physical memory
      return ctx.r3.u32;
    };
    const uint32_t small = allocate(65536, 0x404, 0x8258D660);
    const uint32_t large = allocate(1048576, 0x20000404, 0x8258D680);
    set(88, 0);
    set(92, 65536);
    set(96, small);
    set(100, 1048576);
    set(104, large);
    set(108, 256);
    ctx.r3.u64 = 0;
    ctx.r4.u64 = 1;
    ctx.r5.u64 = 0;
    ctx.r6.u64 = 0;
    ctx.r7.u64 = parameters;
    ctx.r8.u64 = object + kVideoDevice;
    ctx.lr = 0x8258D6C8;
    sub_8229DB38(ctx, base);  // makes the device (Device::Init)
    if (ctx.r3.s32 < 0) {
      set(kVideoDevice, 0);
      return;
    }
    const uint32_t width = field(16), height = field(20);
    set(kVideoWidth, width);
    set(kVideoHeight, height);
    auto front_buffer = [&](uint32_t return_address) {
      ctx.r3.u64 = width;
      ctx.r4.u64 = height;
      ctx.r5.u64 = 1;
      ctx.r6.u64 = 1;
      ctx.r7.u64 = 0;
      ctx.r8.u64 = 0x28280106;
      ctx.r9.u64 = 0;
      ctx.r10.u64 = 3;
      ctx.lr = return_address;
      sub_822A5D08(ctx, base);  // probably creates a texture
      return ctx.r3.u32;
    };
    set(kVideoFrontBuffers, front_buffer(0x8258D718));
    set(kVideoFrontBuffers + 4, front_buffer(0x8258D744));
    auto on_device = [&](uint32_t value, uint32_t return_address, auto function) {
      ctx.r3.u64 = field(kVideoDevice);
      ctx.r4.u64 = value;
      ctx.lr = return_address;
      function(ctx, base);
    };
    on_device(6, 0x8258D754, sub_822A1950);  // the Z function
    const uint32_t viewport = frame + 112;
    guest::StoreU32(base, viewport, 0);
    guest::StoreU32(base, viewport + 4, 0);
    guest::StoreU32(base, viewport + 8, width);
    guest::StoreU32(base, viewport + 12, height);
    guest::StoreFloat(base, viewport + 16, kMinZ);
    guest::StoreFloat(base, viewport + 20, kMaxZ);
    on_device(viewport, 0x8258D788, sub_822A3518);  // SetViewport
    on_device(1, 0x8258D794, sub_822A2480);  // the render targets' format switches
    on_device(1, 0x8258D7A0, sub_822A2530);
    on_device(1, 0x8258D7AC, sub_822A25E0);
    on_device(1, 0x8258D7B8, sub_822A2690);
    on_device(0, 0x8258D7C4, sub_822A1EE8);
    on_device(0, 0x8258D7D0, sub_822A1F20);
    on_device(0, 0x8258D7DC, sub_822A1F58);
    on_device(kVblankCallback, 0x8258D7EC, sub_8229DA20);
    const float w = float(field(16)), h = float(field(20));
    const int32_t x0 = int32_t(w * kSafeNear), x1 = int32_t(w * kSafeFar);
    const int32_t y0 = int32_t(h * kSafeNear), y1 = int32_t(h * kSafeFar);
    set(kVideoSafeArea, uint32_t(std::min(x0, x1)));
    set(kVideoSafeArea + 4, uint32_t(std::min(y0, y1)));
    set(kVideoSafeArea + 8, uint32_t(std::max(x0, x1) - std::min(x0, x1)));
    set(kVideoSafeArea + 12, uint32_t(std::max(y0, y1) - std::min(y0, y1)));
    result = field(kVideoDevice);
  });
  ctx.r3.u64 = result;
  VideoModeCheck_AfterDisplaySetup(base, object, lr, interval, ctx.r3.u32);
}

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
