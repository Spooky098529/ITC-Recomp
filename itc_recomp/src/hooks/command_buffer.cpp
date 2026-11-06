// The graphics library's command buffer natively: adapters in front of
// src/graphics/command_buffer.h (docs/functions/command-buffer.md). Each
// leaves r3 as the original does (the return value, or r3 as it came for
// those with none).

#include "guest.h"

#include "base/flags.h"
#include "graphics/command_buffer.h"

ITC_FLAG_BOOL(native_command_buffer, true, "ITC",
              "Run the graphics library's command buffer (docs/functions/command-buffer.md: "
              "Device::WriteSegmentTail sub_8229E768, Device::SaveSegment sub_8229DBF0, "
              "CommandChain::Close sub_8229DFB8, Device::WriteCallback sub_8229E9D0, "
              "Device::NextSpace sub_8229E100 (its wait for the GPU, sub_8229DC28, dropped), "
              "Device::AllocateFromCommandBuffer sub_8229E308) "
              "natively instead of through the translated code (the same results; checked by "
              "--run_harnesses=command_buffer).");

namespace cb = graphics::command_buffer;

#define ITC_COMMAND_BUFFER(address, ...)      \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_command_buffer)) {   \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    __VA_ARGS__;                              \
  }

ITC_COMMAND_BUFFER(8229E768, ctx.r3.u64 = cb::WriteSegmentTail(base, ctx.r3.u32, ctx.r4.u32))
ITC_COMMAND_BUFFER(8229DBF0, cb::SaveSegment(base, ctx.r3.u32))
ITC_COMMAND_BUFFER(8229DFB8, cb::CloseChain(base, ctx.r3.u32))
ITC_COMMAND_BUFFER(8229E9D0, ctx.r3.u64 = cb::WriteCallback(base, ctx.r3.u32, ctx.r4.u32,
                                                          ctx.r5.u32, ctx.r6.u32, ctx.r7.u32))
// NextSpace: the size in and out through r5 (a segment's written back, as
// the original does when it's had; when not, the word is unchanged; a
// segment at address 0 is had).
ITC_COMMAND_BUFFER(8229E100, {
  uint32_t size = guest::LoadU32(base, ctx.r5.u32);
  const uint32_t space = cb::NextSpace(base, ctx.r3.u32, ctx.r4.u32, size, ctx.r6.u32);
  if (!(ctx.r4.u32 & 1)) {
    guest::StoreU32(base, ctx.r5.u32, size);  // unchanged when it failed
  }
  ctx.r3.u64 = space;
})
// Reserve: the recording command buffer's path (never in play) is the
// original's.
ITC_COMMAND_BUFFER(8229E308, {
  if (guest::LoadU32(base, ctx.r3.u32 + cb::kRecorder) != 0) {
    __imp__sub_8229E308(ctx, base);
    return;
  }
  ctx.r3.u64 = cb::Reserve(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
})
