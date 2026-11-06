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
              "with Device::RestoreSegment sub_8229E0C0, Device::CreateWorkers sub_822B6D70 "
              "without its EVENT_WRITE_SHD packets, its threads made through the kernel, "
              "Device::WorkerThread sub_822B69A8 with the device's lock a host mutex) natively instead of through the "
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

// Device::CreateWorkers: the threads made and their priority set through the
// kernel's imports, as the original (the user's call for XAudio's threads,
// 2026-10-29), from a frame like the original's (176 bytes).
pt::WorkerCalls hooks::PredicatedTilingWorkerCalls(PPCContext& ctx, uint8_t* base) {
  pt::WorkerCalls calls;
  calls.end_segment = PredicatedTilingCalls(ctx, base).end_segment;
  calls.create_thread = [&ctx, base](uint32_t handle, uint32_t stack, uint32_t id,
                                     uint32_t startup, uint32_t entry, uint32_t parameter,
                                     uint32_t flags) {
    ctx.r3.u64 = handle;
    ctx.r4.u64 = stack;
    ctx.r5.u64 = id;
    ctx.r6.u64 = startup;
    ctx.r7.u64 = entry;
    ctx.r8.u64 = parameter;
    ctx.r9.u64 = flags;
    ctx.lr = 0x822B6ED4;
    __imp__ExCreateThread(ctx, base);
    return ctx.r3.u32;
  };
  calls.set_priority = [&ctx, base](uint32_t thread, uint32_t priority) {
    ctx.r3.u64 = thread;
    ctx.r4.u64 = priority;
    ctx.lr = 0x822B6EE8;
    __imp__KeSetBasePriorityThread(ctx, base);
  };
  return calls;
}

void hooks::NativeCreateWorkers(PPCContext& ctx, uint8_t* base) {
  const uint32_t device = ctx.r3.u32;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, 176, 0x822B6ED4, [&](uint32_t) {
    r3 = pt::CreateWorkers(base, device, PredicatedTilingWorkerCalls(ctx, base));
  });
  ctx.r3.u64 = r3;
}

// No adapter for sub_822B6D70 (removed 2026-11-06): its only caller,
// Device::Init's override (src/hooks/graphics_device.cpp), calls
// NativeCreateWorkers itself with the flag on. (A recording command buffer
// can't exist yet at Device::Init.)

// Device::WorkerThread: the kernel's waits and the event's reset through
// their imports, as the original; the job and the present as guest calls;
// from a frame like the original's (160 bytes; the timeout at its +80).
pt::WorkerLoopCalls hooks::PredicatedTilingWorkerLoopCalls(PPCContext& ctx, uint8_t* base) {
  pt::WorkerLoopCalls calls;
  calls.wait = [&ctx, base](uint32_t object, uint32_t reason, uint32_t mode, uint32_t alertable,
                            uint32_t timeout) {
    ctx.r3.u64 = object;
    ctx.r4.u64 = reason;
    ctx.r5.u64 = mode;
    ctx.r6.u64 = alertable;
    ctx.r7.u64 = timeout;
    ctx.lr = 0x822B6A44;
    __imp__KeWaitForSingleObject(ctx, base);
    return ctx.r3.u32;
  };
  calls.reset_event = [&ctx, base](uint32_t event) {
    ctx.r3.u64 = event;
    ctx.lr = 0x822B6A54;
    __imp__KeResetEvent(ctx, base);
    return ctx.r3.u32;
  };
  calls.job = [&ctx, base](uint32_t block) {
    ctx.r3.u64 = block;
    ctx.lr = 0x822B6A68;
    sub_822B6780(ctx, base);
  };
  calls.present = [&ctx, base](uint32_t device) {
    ctx.r3.u64 = device;
    ctx.lr = 0x822B6A14;
    sub_822A0718(ctx, base);
    ctx.r3.u64 = device;
    ctx.r4.u64 = device + 13444;
    ctx.r5.u64 = 0;
    ctx.lr = 0x822B6A24;
    sub_822A08A0(ctx, base);
  };
  return calls;
}

void hooks::NativeWorkerThread(PPCContext& ctx, uint8_t* base) {
  const uint32_t block = ctx.r3.u32;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, 160, 0x822B6A44, [&](uint32_t frame) {
    r3 = pt::WorkerThread(base, block, frame + 80, PredicatedTilingWorkerLoopCalls(ctx, base));
  });
  ctx.r3.u64 = r3;
}

// The thread's entry point (ExCreateThread's, CreateWorkers').
extern "C" REX_FUNC(sub_822B69A8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_predicated_tiling)) {
    __imp__sub_822B69A8(ctx, base);
    return;
  }
  hooks::NativeWorkerThread(ctx, base);
}
