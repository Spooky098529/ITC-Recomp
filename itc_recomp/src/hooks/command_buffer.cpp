// The graphics library's command buffer natively: adapters in front of
// src/graphics/command_buffer.h (docs/functions/command-buffer.md). Each
// leaves r3 as the original does (the return value, or r3 as it came for
// those with none).

#include "guest.h"

#include "hooks/command_buffer.h"

#include "base/flags.h"
#include "graphics/command_buffer.h"
#include "hooks/guest_call.h"

ITC_FLAG_BOOL(native_command_buffer, true, "ITC",
              "Run the graphics library's command buffer (docs/functions/command-buffer.md: "
              "Device::WriteSegmentTail sub_8229E768, Device::SaveSegment sub_8229DBF0, "
              "CommandChain::Close sub_8229DFB8, Device::WriteCallback sub_8229E9D0, "
              "Device::NextSpace sub_8229E100 (its wait for the GPU, sub_8229DC28, dropped), "
              "Device::AllocateFromCommandBuffer sub_8229E308, "
              "Device::WriteDefaultDrawRegisters sub_822AF978 without its packets, "
              "CommandChain::NewBlock sub_8229EB10, CommandChain::Init sub_8229ED60, the segment "
              "flow (Device::EndSegment sub_8229EF48, Device::KickSegment sub_8229EDA8, "
              "Device::NextSegment sub_8229EC00, Device::WaitForFence sub_8229E908, its waits "
              "for the GPU dropped), Device::ReserveWords sub_8229F148, "
              "Device::InsertCallback sub_8229F228; the "
              "coherency flush packets sub_8229E5D0 and the data cache flush sub_822AC470 "
              "dropped) "
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
// The chains: a recording command buffer (+13160, never in play) makes
// Reserve the original's, so they run the original whole.
ITC_COMMAND_BUFFER(8229EB10, {
  if (guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32 + cb::kChainDevice) + cb::kRecorder) !=
      0) {
    __imp__sub_8229EB10(ctx, base);
    return;
  }
  ctx.r3.u64 = cb::NewBlock(base, ctx.r3.u32);
})
ITC_COMMAND_BUFFER(8229ED60, {
  if (guest::LoadU32(base, ctx.r4.u32 + cb::kRecorder) != 0) {
    __imp__sub_8229ED60(ctx, base);
    return;
  }
  ctx.r3.u64 = cb::InitChain(base, ctx.r3.u32, ctx.r4.u32);
})
// The segment flow: with a recording command buffer (never in play), the
// originals whole.
graphics::command_buffer::Kick hooks::CommandBufferKick(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t device, uint32_t result, uint32_t address, uint32_t words,
                      uint32_t interrupts, uint32_t list) {
    uint32_t r3 = 0;
    guest_call::InFrame(ctx, base, 128, 0x8229EEE0, [&](uint32_t) {
      ctx.r3.u64 = device;
      ctx.r4.u64 = result;
      ctx.r5.u64 = address;
      ctx.r6.u64 = words;
      ctx.r7.u64 = interrupts;
      ctx.r8.u64 = list;
      sub_8229E848(ctx, base);
      r3 = ctx.r3.u32;
    });
    return r3;
  };
}

void hooks::EndCommandSegment(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_command_buffer) || cb::Recording(base, ctx.r3.u32)) {
    __imp__sub_8229EF48(ctx, base);
    return;
  }
  const uint32_t device = ctx.r3.u32;
  ctx.r3.u64 = cb::EndSegment(base, device, CommandBufferKick(ctx, base));
}

#define ITC_SEGMENT_FLOW(address, ...)                                             \
  extern "C" REX_FUNC(sub_##address) {                                           \
    REX_FUNC_PROLOGUE();                                                         \
    if (!ITC_FLAG(native_command_buffer) || cb::Recording(base, ctx.r3.u32)) {   \
      __imp__sub_##address(ctx, base);                                           \
      return;                                                                    \
    }                                                                            \
    const cb::Kick kick = hooks::CommandBufferKick(ctx, base);                   \
    __VA_ARGS__;                                                                 \
  }

ITC_SEGMENT_FLOW(8229EDA8, {
  const uint32_t device = ctx.r3.u32;
  ctx.r3.u64 = cb::KickSegment(base, device, kick);
})
ITC_SEGMENT_FLOW(8229EC00, {
  const uint32_t device = ctx.r3.u32, words = ctx.r4.u32;
  ctx.r3.u64 = cb::NextSegment(base, device, words, kick);
})
ITC_SEGMENT_FLOW(8229E908, {
  const uint32_t device = ctx.r3.u32, fence = ctx.r4.u32, code = ctx.r5.u32;
  ctx.r3.u64 = cb::WaitForFence(base, device, fence, code, kick);
})

ITC_SEGMENT_FLOW(8229F148, {
  const uint32_t device = ctx.r3.u32, words = ctx.r4.u32;
  ctx.r3.u64 = cb::ReserveWords(base, device, words, kick);
})
ITC_SEGMENT_FLOW(8229F228, {
  const uint32_t device = ctx.r3.u32, flags = ctx.r4.u32, function = ctx.r5.u32,
                 argument = ctx.r6.u32;
  ctx.r3.u64 = cb::InsertCallback(base, device, flags, function, argument, kick);
})

ITC_COMMAND_BUFFER(822AF978,
                   ctx.r3.u64 = cb::WriteDefaultDrawRegisters(base, ctx.r3.u32, ctx.r4.u32))

// Device::WriteFlushPackets (sub_8229E5D0): Dropped. Its block only made the
// GPU flush caches for memory the CPU wrote (COHER_* registers and a
// WAIT_REG_MEM on them), which our kick doesn't act on
// (src/native/gpu_link.cpp: waits have no effect, and those registers are
// read by nothing); the graphics rule on cache flushes (2026-10-24,
// graphics-resources.md). Answers "no block" (r5's word 0, r4's left
// alone), as the original does when there's nothing to flush, so its callers
// (sub_8229EF48, the tiling worker sub_822B6BF8) kick none. The pending
// range it took (sub_8229DE48) has had no writer in play since
// Resource::Lock is native.
extern "C" REX_FUNC(sub_8229E5D0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_command_buffer)) {
    __imp__sub_8229E5D0(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r5.u32, 0);
}

// The data cache flush (sub_822AC470): Dropped (no data cache to flush on
// PC, the user's call, 2026-10-24, as Resource::Unlock leaves it out). Its
// translation writes only its own stack and leaves r3; so does this.
extern "C" REX_FUNC(sub_822AC470) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_command_buffer)) {
    __imp__sub_822AC470(ctx, base);
  }
}
