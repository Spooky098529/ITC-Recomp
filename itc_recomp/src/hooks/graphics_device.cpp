// The graphics library's device set-up, natively and without its kernel
// video calls (Vd*): what our renderer needs from each function is kept, the
// rest left out. The steps that stay are the library's own (recompiled)
// functions, called as guest code. See B3 in docs/rexglue-removal.md.

#include "guest.h"
#include "guest_call.h"

#include "display/display.h"

using guest_call::InFrame;

namespace {

// Device fields (see "The functions" under B3).
constexpr uint32_t kDeviceFlags = 10809;
constexpr uint32_t kDeviceParameters = 13188;  // a copy of the 124 bytes
constexpr uint32_t kDeviceFrontBufferFormat = 13252;  // the copy's +64
constexpr uint32_t kDeviceDepthSurface = 13424;
constexpr uint32_t kDeviceFrontBuffer = 13428;
constexpr uint32_t kDeviceRenderTarget = 13432;
constexpr uint32_t kDeviceDisplayWidth = 19988;
constexpr uint32_t kDeviceDisplayHeight = 19992;
constexpr uint32_t kDeviceDisplayPitch = 19996;
constexpr uint32_t kDeviceDisplayFormat = 20000;

// The front buffer's format (parameters +64) as the device creates it: 0
// and 0x28280186 become 0x28280106, 0x282801B6 becomes 0x28280136.
uint32_t FrontBufferFormat(uint32_t requested) {
  if (requested == 0 || requested == 0x28280186) {
    return 0x28280106;
  }
  if (requested == 0x282801B6) {
    return 0x28280136;
  }
  return requested;
}

// The format the device keeps: 0x18280136 and 0x18280106 as 0x2828xxxx.
uint32_t StoredFrontBufferFormat(uint32_t format) {
  if (format == 0x18280136) {
    return 0x28280136;
  }
  if (format == 0x18280106) {
    return 0x28280106;
  }
  return format;
}

}  // namespace

// Device::SetUpDisplay (from Device::Init): r3 device, r4 the presentation
// parameters (+0 width, +4 height, +8 the render target's format, +16
// passed to both surfaces, +36 non-zero for a depth surface, +40 its format,
// +56 and +60 non-zero to skip the render target and the front buffer, +64
// the front buffer's format, +68 the display's format, +96 the scaler's;
// the game sets +60). Creates the front buffer and the
// default render target and depth surface, sets them, and keeps a copy of
// the parameters. Returns 1, or 0 if a surface couldn't be made. The
// display mode is ours (src/display/display.h) instead of VdQueryVideoMode.
extern "C" REX_FUNC(sub_822ACE00) {
  REX_FUNC_PROLOGUE();
  const uint32_t device = ctx.r3.u32, parameters = ctx.r4.u32;
  auto parameter = [&](uint32_t offset) { return guest::LoadU32(base, parameters + offset); };
  const uint32_t width = parameter(0), height = parameter(4);
  const uint32_t format = FrontBufferFormat(parameter(64));
  guest::StoreU32(base, device + kDeviceDisplayWidth, display::kWidth);
  guest::StoreU32(base, device + kDeviceDisplayHeight, display::kHeight);
  guest::StoreU32(base, device + kDeviceDisplayPitch, display::kWidth);
  guest::StoreU32(base, device + kDeviceDisplayFormat, parameter(68));
  bool made = true;
  InFrame(ctx, base, 208, 0x822ACEC8, [&](uint32_t) {
    if (parameter(60) == 0) {
      ctx.r3.u64 = width;
      ctx.r4.u64 = height;
      ctx.r5.u64 = 1;
      ctx.r6.u64 = 1;
      ctx.r7.u64 = 0;
      ctx.r8.u64 = format;
      ctx.r9.u64 = 0;
      ctx.r10.u64 = 3;
      ctx.lr = 0x822ACEC8;
      sub_822A5D08(ctx, base);  // a texture
      if (!ctx.r3.u32) {
        made = false;
        return;
      }
      guest::StoreU32(base, device + kDeviceFrontBuffer, ctx.r3.u32);
    }
    if (parameter(56) == 0) {
      ctx.r3.u64 = width;
      ctx.r4.u64 = height;
      ctx.r5.u64 = parameter(8);
      ctx.r6.u64 = parameter(16);
      ctx.r7.u64 = 0;
      ctx.lr = 0x822ACF00;
      sub_822A5E28(ctx, base);  // a surface
      const uint32_t surface = ctx.r3.u32;
      if (!surface) {
        made = false;
        return;
      }
      guest::StoreU32(base, device + kDeviceRenderTarget, surface);
      ctx.r3.u64 = device;
      ctx.r4.u64 = 0;
      ctx.r5.u64 = surface;
      ctx.lr = 0x822ACF18;
      sub_822A3D30(ctx, base);  // set as render target 0
    }
    if (parameter(36) != 0) {
      ctx.r3.u64 = width;
      ctx.r4.u64 = height;
      ctx.r5.u64 = parameter(40);
      ctx.r6.u64 = parameter(16);
      ctx.r7.u64 = 0;
      ctx.lr = 0x822ACF3C;
      sub_822A5E28(ctx, base);
      const uint32_t surface = ctx.r3.u32;
      if (!surface) {
        made = false;
        return;
      }
      guest::StoreU32(base, device + kDeviceDepthSurface, surface);
      ctx.r3.u64 = device;
      ctx.r4.u64 = surface;
      ctx.lr = 0x822ACF50;
      sub_822A4060(ctx, base);  // set as the depth surface
    }
    // Through the game's memcpy, as the original (it's wrapped for the
    // renderer's memory mirrors).
    ctx.r3.u64 = device + kDeviceParameters;
    ctx.r4.u64 = parameters;
    ctx.r5.u64 = 124;
    ctx.lr = 0x822ACF60;
    sub_825A7410(ctx, base);
    guest::StoreU32(base, device + kDeviceFrontBufferFormat, StoredFrontBufferFormat(format));
    ctx.r3.u64 = device;
    ctx.r4.u64 = parameters + 96;
    ctx.lr = 0x822ACFA0;
    sub_822B42D0(ctx, base);  // the scaler's parameters
    // The display mode is set (sub_822AD0B0) at the next Swap.
    guest::StoreU8(base, device + kDeviceFlags,
                   guest::LoadU8(base, device + kDeviceFlags) | 0x10);
  });
  ctx.r3.u64 = made ? 1 : 0;
}
