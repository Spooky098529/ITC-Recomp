// Predicated tiling natively: adapters in front of
// src/graphics/predicated_tiling.h (docs/functions/predicated-tiling.md).
// Each leaves r3 as the original does. With a recording command buffer
// (never in play) the originals run whole, as the command buffer's adapters
// do.

#include "hooks/predicated_tiling.h"

#include "guest.h"

#include "base/flags.h"
#include "graphics/command_buffer.h"
#include "hooks/command_buffer.h"
#include "hooks/guest_call.h"

ITC_FLAG_BOOL(native_predicated_tiling, true, "ITC",
              "Run the graphics library's predicated tiling "
              "(docs/functions/predicated-tiling.md: Device::SetPredication sub_825BEB80 "
              "without its SET_BIN_MASK packet, Device::BeginTiledSegments sub_822B6AD8 "
              "without the segment chain's header, Device::EndTiledSegments sub_822B6BF8 "
              "with Device::RestoreSegment sub_8229E0C0) natively instead of through the "
              "translated code (the same device fields, chains and kicks; checked by "
              "--run_harnesses=predicated_tiling).");

namespace pt = graphics::predicated_tiling;
namespace cb = graphics::command_buffer;

pt::GuestCalls hooks::PredicatedTilingCalls(PPCContext& ctx, uint8_t* base) {
  pt::GuestCalls calls;
  calls.end_segment = [&ctx, base](uint32_t device) {
    guest_call::InFrame(ctx, base, 128, 0x822B6AEC, [&](uint32_t) {
      ctx.r3.u64 = device;
      sub_8229EF48(ctx, base);
    });
  };
  calls.kick = CommandBufferKick(ctx, base);
  return calls;
}

#define ITC_PREDICATED_TILING(address, ...)                                          \
  extern "C" REX_FUNC(sub_##address) {                                             \
    REX_FUNC_PROLOGUE();                                                           \
    if (!ITC_FLAG(native_predicated_tiling) || cb::Recording(base, ctx.r3.u32)) {  \
      __imp__sub_##address(ctx, base);                                             \
      return;                                                                      \
    }                                                                              \
    __VA_ARGS__;                                                                   \
  }

ITC_PREDICATED_TILING(825BEB80, {
  const uint32_t device = ctx.r3.u32, select = ctx.r4.u32;
  ctx.r3.u64 = pt::SetPredication(base, device, select, hooks::PredicatedTilingCalls(ctx, base));
})
ITC_PREDICATED_TILING(822B6AD8, {
  const uint32_t device = ctx.r3.u32;
  ctx.r3.u64 = pt::BeginTiledSegments(base, device, hooks::PredicatedTilingCalls(ctx, base));
})

void hooks::EndTiledSegments(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_predicated_tiling) || cb::Recording(base, ctx.r3.u32)) {
    __imp__sub_822B6BF8(ctx, base);
    return;
  }
  const uint32_t device = ctx.r3.u32, tiles = ctx.r4.u32, cpus = ctx.r5.u32;
  ctx.r3.u64 = pt::EndTiledSegments(base, device, tiles, cpus, PredicatedTilingCalls(ctx, base));
}
