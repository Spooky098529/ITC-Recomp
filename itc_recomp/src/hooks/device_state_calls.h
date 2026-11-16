// The guest calls the graphics device's packet writers keep
// (graphics::device_state::DeviceCalls, src/graphics/device_state.h), made by
// their callers' adapters: src/hooks/device_state.cpp and Device::Init's
// override (src/hooks/graphics_device.cpp).

#pragma once

#include "guest.h"
#include "lifted/entries.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "graphics/device_state.h"
#include "lifted/sdk.h"

namespace hooks {

// The guest calls they keep: the draw's flush helpers (Wrapped, in
// src/native/draw_packets.cpp), table setters that aren't native, and
// SetTexture's original. Each sets the registers the original's call does.
inline graphics::device_state::DeviceCalls DeviceStateCalls(lifted::Regs& ctx, uint8_t* base,
                                                            uint32_t device) {
  graphics::device_state::DeviceCalls calls;
  calls.flush_hiz = [&ctx, base, device](uint64_t mask, uint32_t hiz) {
    ctx.r3.u64 = device;
    ctx.r4.u64 = mask;
    ctx.r5.u64 = hiz;
    ctx.r6.u64 = 0;
    ctx.lr = 0x8229F6B8;
    lifted::Enter(ctx, base, lifted::entry::e822ADC08);
    return ctx.r3.u64;
  };
  calls.flush_registers = [&ctx, base, device](uint64_t mask, uint32_t first, uint32_t shadow) {
    ctx.r3.u64 = device;
    ctx.r4.u64 = mask;
    ctx.r5.u64 = first;
    ctx.r6.u64 = shadow;
    ctx.lr = 0x8229F6E8;
    lifted::Enter(ctx, base, lifted::entry::e822ADE30);
  };
  calls.setter = [&ctx, base, device](uint32_t setter, uint32_t r4, uint32_t r5) {
    ctx.r3.u64 = device;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    ctx.lr = 0x822ACBF0;
    lifted::CallFromContext(ctx, base, setter);
  };
  calls.set_texture = [&ctx, base, device](uint32_t stage, uint32_t texture, uint64_t dirty) {
    ctx.r3.u64 = device;
    ctx.r4.u64 = stage;
    ctx.r5.u64 = texture;
    ctx.r6.u64 = dirty;
    ctx.lr = 0x822ACC8C;
    lifted::original::c822A6008(ctx, base);
  };
  return calls;
}


}  // namespace hooks
