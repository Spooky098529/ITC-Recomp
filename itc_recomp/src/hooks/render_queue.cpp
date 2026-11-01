// The render queue's methods natively: adapters in front of
// src/graphics/render_queue.h (docs/functions/device-shaders.md). Each
// leaves r3 as the original does. RenderQueue::Flip and ClearRecording keep
// --find_menu_selection's wrapper (src/debug/menu_selection_scan.cpp
// wrapped them before), around both paths.

#include "hooks/render_queue.h"

#include "guest.h"

#include "base/flags.h"
#include "debug/menu_selection_scan.h"
#include "graphics/render_queue.h"
#include "hooks/models.h"

ITC_FLAG_BOOL(native_render_queue, true, "ITC",
              "Run the render queue's methods (docs/functions/device-shaders.md: "
              "RenderQueue::Init sub_8258ED00, ClearRecording sub_8258ED78, Flip sub_8258E5E0, "
              "Recording sub_8258E5F8, Replaying sub_8258E618, QueueEntry::Sequence "
              "sub_8258E5C0, Add sub_8258EDC8 with PassList::Add sub_8258EB88; PtrVector::PushBack sub_82591540 with Insert "
              "sub_825911C8, which allocates only with --native_models and the native heap; Replay sub_82591238 "
              "with its sort and the chunk walker, the commands going to our own handlers, "
              "src/render/commands.cpp) natively instead of through the translated code (the same "
              "writes; guest calls for the drawer's Init and vtable slots that aren't the "
              "queue's own; checked by --run_harnesses=render_queue but the replay; off, the recompiled replay's "
              "commands reach no renderer and nothing is drawn)");

namespace rq = graphics::render_queue;

rq::GuestCall hooks::RenderQueueCall(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t function, uint32_t sp, uint32_t return_address, int count,
                      uint64_t r3, uint64_t r4, uint64_t r5) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    if (count > 0) ctx.r3.u64 = r3;
    if (count > 1) ctx.r4.u64 = r4;
    if (count > 2) ctx.r5.u64 = r5;
    ctx.lr = return_address;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return rq::CallResult{ctx.r3.u64, ctx.f1.f64};
  };
}

void hooks::NativeRenderQueueClearRecording(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rq::ClearRecording(base, ctx.r3.u32, ctx.r1.u32, RenderQueueCall(ctx, base));
}

void hooks::NativeRenderQueueInit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rq::Init(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, RenderQueueCall(ctx, base));
}

void hooks::NativeRenderQueueAdd(PPCContext& ctx, uint8_t* base) {
  const auto r3 = rq::Add(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.r1.u32,
                          RenderQueueCall(ctx, base));
  if (!r3) {
    __imp__sub_8258EDC8(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

// Add: r3 = the queue, r4 = the chunk, r5 = the bucket, f1 = the key.
// PassList::Add (sub_8258EB88) has no adapter: only Add calls it.
extern "C" REX_FUNC(sub_8258EDC8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_render_queue)) {
    __imp__sub_8258EDC8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  hooks::NativeRenderQueueAdd(ctx, base);
}

// PtrVector::PushBack: r3 = the vector, r4 = the value's address. Insert
// (sub_825911C8) has no adapter: only PushBack calls it.
extern "C" REX_FUNC(sub_82591540) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_render_queue)) {
    if (const auto r3 = rq::PushBack(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32,
                                     hooks::ModelsNativeLifetime())) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_82591540(ctx, base);
}

void hooks::NativeRenderQueueReplay(PPCContext& ctx, uint8_t* base) {
  rq::Replay(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, RenderQueueCall(ctx, base));
}

// Replay: r3 = the queue, r4 = the device, r5 = the video output. Its
// callees (Partition sub_8258EC38, QuickSort sub_82590CB0, ChunkChain::Run
// sub_8258EE98 and the walkers sub_8258E630, sub_8258E780, sub_8258E7E8,
// sub_8258E868) have no adapters: only Replay calls them.
extern "C" REX_FUNC(sub_82591238) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_render_queue)) {
    __imp__sub_82591238(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  hooks::NativeRenderQueueReplay(ctx, base);
}

// Init: r3 = the queue, r4 = the device.
extern "C" REX_FUNC(sub_8258ED00) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_render_queue)) {
    __imp__sub_8258ED00(ctx, base);
    return;
  }
  hooks::NativeRenderQueueInit(ctx, base);
}

// ClearRecording: r3 = the queue.
extern "C" REX_FUNC(sub_8258ED78) {
  REX_FUNC_PROLOGUE();
  debug::MenuSelectionScan_Before(base);
  if (ITC_FLAG(native_render_queue)) {
    ctx.fpscr.disableFlushMode();
    hooks::NativeRenderQueueClearRecording(ctx, base);
  } else {
    __imp__sub_8258ED78(ctx, base);
  }
  debug::MenuSelectionScan_After(base, "call", 0x8258ED78);
}

// Flip: r3 = the queue, left as given.
extern "C" REX_FUNC(sub_8258E5E0) {
  REX_FUNC_PROLOGUE();
  debug::MenuSelectionScan_Before(base);
  if (ITC_FLAG(native_render_queue)) {
    rq::Flip(base, ctx.r3.u32);
  } else {
    __imp__sub_8258E5E0(ctx, base);
  }
  debug::MenuSelectionScan_After(base, "call", 0x8258E5E0);
}

// Recording and Replaying: r3 = the queue; r3 = the list.
extern "C" REX_FUNC(sub_8258E5F8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_render_queue)) {
    __imp__sub_8258E5F8(ctx, base);
    return;
  }
  ctx.r3.u64 = rq::Recording(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_8258E618) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_render_queue)) {
    __imp__sub_8258E618(ctx, base);
    return;
  }
  ctx.r3.u64 = rq::Replaying(base, ctx.r3.u32);
}

// QueueEntry::Sequence: r3 = the entry; f1 = the float at +8.
extern "C" REX_FUNC(sub_8258E5C0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_render_queue)) {
    __imp__sub_8258E5C0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(rq::Sequence(base, ctx.r3.u32));
}
