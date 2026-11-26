// Predicated tiling natively: adapters in front of
// src/graphics/predicated_tiling.h (docs/functions/predicated-tiling.md).
// Each leaves r3 as the original does. With a recording command buffer
// (never in play) the originals run whole, as the command buffer's adapters
// do.

#include "hooks/predicated_tiling.h"
#include "lifted/names.h"
#include "lifted/functions.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/command_buffer.h"
#include "hooks/command_buffer.h"
#include "hooks/guest_call.h"
#include "lifted/sdk.h"
#include "schema/device_state.h"

ITC_FLAG_BOOL(native_tiling_worker, true, "ITC",
              "Run the tiling worker's wake (Device::WakeWorker sub_822B6A78) natively and drop "
              "its job's work (Device::WorkerJob sub_822B6780: GPU work nothing reads); both "
              "never seen in play (docs/functions/predicated-tiling.md)");
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

pt::GuestCalls hooks::PredicatedTilingCalls(lifted::Regs& ctx) {
  pt::GuestCalls calls;
  calls.end_segment = [&ctx](uint32_t device) {
    guest_call::InFrame(ctx, 128, 0x822B6AEC, [&](uint32_t) {
      ctx.r3.u64 = device;
      lifted::Enter(ctx, game::Device::EndSegment);
    });
  };
  calls.kick = CommandBufferKick(ctx);
  return calls;
}

#define ITC_PREDICATED_TILING(address, ...)                                          \
  ITC_NATIVE_ENTRY_ALWAYS(address) {                                             \
    \
    if (!ITC_FLAG(native_predicated_tiling) || cb::Recording(ctx.r3.u32)) {  \
      lifted::original::c##address(ctx);                                             \
      return;                                                                      \
    }                                                                              \
    __VA_ARGS__;                                                                   \
  }

ITC_PREDICATED_TILING(825BEB80, {
  const uint32_t device = ctx.r3.u32, select = ctx.r4.u32;
  ctx.r3.u64 = pt::SetPredication(device, select, hooks::PredicatedTilingCalls(ctx));
})
ITC_PREDICATED_TILING(822B6AD8, {
  const uint32_t device = ctx.r3.u32;
  ctx.r3.u64 = pt::BeginTiledSegments(device, hooks::PredicatedTilingCalls(ctx));
})

void hooks::EndTiledSegments(lifted::Regs& ctx) {
  if (!ITC_FLAG(native_predicated_tiling) || cb::Recording(ctx.r3.u32)) {
    lifted::original::c822B6BF8(ctx);
    return;
  }
  const uint32_t device = ctx.r3.u32, tiles = ctx.r4.u32, cpus = ctx.r5.u32;
  ctx.r3.u64 = pt::EndTiledSegments(device, tiles, cpus, PredicatedTilingCalls(ctx));
}

// Device::CreateWorkers: the threads made and their priority set through the
// kernel's imports, as the original (the user's call for XAudio's threads,
// 2026-10-29), from a frame like the original's (176 bytes).
pt::WorkerCalls hooks::PredicatedTilingWorkerCalls(lifted::Regs& ctx) {
  pt::WorkerCalls calls;
  calls.end_segment = PredicatedTilingCalls(ctx).end_segment;
  calls.create_thread = [&ctx](uint32_t handle, uint32_t stack, uint32_t id,
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
    lifted::Sdk(ctx, __imp__ExCreateThread);
    return ctx.r3.u32;
  };
  calls.set_priority = [&ctx](uint32_t thread, uint32_t priority) {
    ctx.r3.u64 = thread;
    ctx.r4.u64 = priority;
    ctx.lr = 0x822B6EE8;
    lifted::Sdk(ctx, __imp__KeSetBasePriorityThread);
  };
  return calls;
}

void hooks::NativeCreateWorkers(lifted::Regs& ctx) {
  const uint32_t device = ctx.r3.u32;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, 176, 0x822B6ED4, [&](uint32_t) {
    r3 = pt::CreateWorkers(device, PredicatedTilingWorkerCalls(ctx));
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
pt::WorkerLoopCalls hooks::PredicatedTilingWorkerLoopCalls(lifted::Regs& ctx) {
  pt::WorkerLoopCalls calls;
  calls.wait = [&ctx](uint32_t object, uint32_t reason, uint32_t mode, uint32_t alertable,
                            uint32_t timeout) {
    ctx.r3.u64 = object;
    ctx.r4.u64 = reason;
    ctx.r5.u64 = mode;
    ctx.r6.u64 = alertable;
    ctx.r7.u64 = timeout;
    ctx.lr = 0x822B6A44;
    lifted::Sdk(ctx, __imp__KeWaitForSingleObject);
    return ctx.r3.u32;
  };
  calls.reset_event = [&ctx](uint32_t event) {
    ctx.r3.u64 = event;
    ctx.lr = 0x822B6A54;
    lifted::Sdk(ctx, __imp__KeResetEvent);
    return ctx.r3.u32;
  };
  calls.job = [&ctx](uint32_t block) {
    ctx.r3.u64 = block;
    ctx.lr = 0x822B6A68;
    lifted::Enter(ctx, game::Device::WorkerJob);
  };
  calls.present = [&ctx](uint32_t device) {
    ctx.r3.u64 = device;
    ctx.lr = 0x822B6A14;
    lifted::Enter(ctx, lifted::e822A0718);
    ctx.r3.u64 = device;
    ctx.r4.u64 = schema::Device(device).front_buffer_header_address();
    ctx.r5.u64 = 0;
    ctx.lr = 0x822B6A24;
    lifted::Enter(ctx, game::Device::Swap);
  };
  return calls;
}

void hooks::NativeWorkerThread(lifted::Regs& ctx) {
  const uint32_t block = ctx.r3.u32;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, 160, 0x822B6A44, [&](uint32_t frame) {
    r3 = pt::WorkerThread(block, frame + 80, PredicatedTilingWorkerLoopCalls(ctx));
  });
  ctx.r3.u64 = r3;
}

// The thread's entry point (ExCreateThread's, CreateWorkers'): the kernel
// calls it by address, so it keeps an override in its function table.
ITC_KERNEL_ENTRY(822B69A8, ITC_FLAG(native_predicated_tiling)) {
  hooks::NativeWorkerThread(ctx);
}

// Device::EndTiledSegments (sub_822B6BF8): its plain C++
// (hooks::EndTiledSegments, --native_predicated_tiling).
ITC_NATIVE_ENTRY_ALWAYS(822B6BF8) {
  hooks::EndTiledSegments(ctx);
}

// Device::WakeWorker (sub_822B6A78), the callback the GPU would raise at a
// tiling's end: nothing raises it (no GPU runs the command buffer; its
// address is data in the callback's words). Its plain C++
// (--native_tiling_worker), KeSetEvent through its import as the
// original's tail call; r3 the argument, the PCR's +268 the CPU.
ITC_NATIVE_ENTRY_ALWAYS(822B6A78) {
  if (!ITC_FLAG(native_tiling_worker)) {
    lifted::original::c822B6A78(ctx);
    return;
  }
  const uint32_t cpu = guest::LoadU8(ctx.r13.u32 + 268);
  ctx.r3.u64 = pt::WakeWorker(ctx.r3.u32, cpu,
      [&ctx](uint32_t event, uint32_t increment, uint32_t wait) {
        ctx.r3.u64 = event;
        ctx.r4.u64 = increment;
        ctx.r5.u64 = wait;
        lifted::Sdk(ctx, __imp__KeSetEvent);
        return ctx.r3.u32;
      });
}

// Device::WorkerJob (sub_822B6780): its work is Dropped with
// --native_tiling_worker (the graphics rule; see
// docs/functions/predicated-tiling.md): the tiled segment chain kicked once
// a tile with the tile's bin select written into the primary ring, the
// draws' tile masks and its markers, GPU work nothing reads since our
// renderer draws the tiles from the command list replay. Never run: its
// wake is never raised.
ITC_NATIVE_ENTRY_ALWAYS(822B6780) {
  if (!ITC_FLAG(native_tiling_worker)) {
    lifted::original::c822B6780(ctx);
    return;
  }
  ITC_NATIVE_RUNNING("sub_822B6780 Device::WorkerJob (dropped)");
}
