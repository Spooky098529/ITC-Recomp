// The render queue's methods natively: adapters in front of
// src/graphics/render_queue.h (docs/functions/device-shaders.md). Each
// leaves r3 as the original does. RenderQueue::Flip and ClearRecording keep
// --find_menu_selection's wrapper (src/debug/menu_selection_scan.cpp
// wrapped them before), around both paths.

#include "hooks/render_queue.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "guest.h"

#include "base/flags.h"
#include "debug/menu_selection_scan.h"
#include "graphics/render_queue.h"
#include "hooks/models.h"
#include "hooks/single_commands.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_render_queue, true, "ITC",
              "Run the render queue's methods (docs/functions/device-shaders.md: "
              "RenderQueue::Init sub_8258ED00, ClearRecording sub_8258ED78, Flip sub_8258E5E0, "
              "Recording sub_8258E5F8, Replaying sub_8258E618, QueueEntry::Sequence "
              "sub_8258E5C0, Add sub_8258EDC8 with PassList::Add sub_8258EB88; PtrVector::PushBack sub_82591540 with Insert "
              "sub_825911C8, which allocates only with --native_models and the native heap; Replay sub_82591238 "
              "with its sort and the chunk walker, the command handlers called as before, where the "
              "native renderer's wrappers see them) natively instead of through the translated code (the same "
              "writes; guest calls for the drawer's Init and vtable slots that aren't the "
              "queue's own; checked by --run_harnesses=render_queue)");

namespace rq = graphics::render_queue;

rq::GuestCall hooks::RenderQueueCall(lifted::Regs& ctx) {
  return [&ctx](uint32_t function, uint32_t sp, uint32_t return_address, int count,
                      uint64_t r3, uint64_t r4, uint64_t r5) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    if (count > 0) ctx.r3.u64 = r3;
    if (count > 1) ctx.r4.u64 = r4;
    if (count > 2) ctx.r5.u64 = r5;
    ctx.lr = return_address;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return rq::CallResult{ctx.r3.u64, ctx.f1.f64};
  };
}

void hooks::NativeRenderQueueClearRecording(lifted::Regs& ctx) {
  ctx.r3.u64 = rq::ClearRecording(ctx.r3.u32, ctx.r1.u32, RenderQueueCall(ctx));
}

void hooks::NativeRenderQueueInit(lifted::Regs& ctx) {
  ctx.r3.u64 = rq::Init(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, RenderQueueCall(ctx));
}

void hooks::NativeRenderQueueAdd(lifted::Regs& ctx) {
  const auto r3 = rq::Add(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.r1.u32,
                          RenderQueueCall(ctx));
  if (!r3) {
    lifted::original::c8258EDC8(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}

// Add: r3 = the queue, r4 = the chunk, r5 = the bucket, f1 = the key.
// PassList::Add (sub_8258EB88) has no adapter: only Add calls it.
ITC_NATIVE_ENTRY(8258EDC8, ITC_FLAG(native_render_queue)) {
  ctx.fpscr.disableFlushMode();
  hooks::NativeRenderQueueAdd(ctx);
}

// PtrVector::PushBack: r3 = the vector, r4 = the value's address. Insert
// (sub_825911C8) has no adapter: only PushBack calls it.
ITC_NATIVE_ENTRY_ALWAYS(82591540) {
  
  if (ITC_FLAG(native_render_queue)) {
    if (const auto r3 = rq::PushBack(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32,
                                     hooks::ModelsNativeLifetime())) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c82591540(ctx);
}

void hooks::NativeRenderQueueReplay(lifted::Regs& ctx) {
  rq::Replay(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, RenderQueueCall(ctx),
             SingleCommands(ctx));
}

// Replay: r3 = the queue, r4 = the device, r5 = the video output. Its
// callees (Partition sub_8258EC38, QuickSort sub_82590CB0, ChunkChain::Run
// sub_8258EE98 and the walkers sub_8258E630, sub_8258E780, sub_8258E7E8,
// sub_8258E868) have no adapters: only Replay calls them.
ITC_NATIVE_ENTRY(82591238, ITC_FLAG(native_render_queue)) {
  ctx.fpscr.disableFlushMode();
  hooks::NativeRenderQueueReplay(ctx);
}

// Init: r3 = the queue, r4 = the device.
ITC_NATIVE_ENTRY(8258ED00, ITC_FLAG(native_render_queue)) {
  hooks::NativeRenderQueueInit(ctx);
}

// ClearRecording: r3 = the queue.
ITC_NATIVE_ENTRY_ALWAYS(8258ED78) {
  
  debug::MenuSelectionScan_Before();
  if (ITC_FLAG(native_render_queue)) {
    ctx.fpscr.disableFlushMode();
    hooks::NativeRenderQueueClearRecording(ctx);
  } else {
    lifted::original::c8258ED78(ctx);
  }
  debug::MenuSelectionScan_After("call", 0x8258ED78);
}

// Flip: r3 = the queue, left as given.
ITC_NATIVE_ENTRY_ALWAYS(8258E5E0) {
  
  debug::MenuSelectionScan_Before();
  if (ITC_FLAG(native_render_queue)) {
    rq::Flip(ctx.r3.u32);
  } else {
    lifted::original::c8258E5E0(ctx);
  }
  debug::MenuSelectionScan_After("call", 0x8258E5E0);
}

// Recording and Replaying: r3 = the queue; r3 = the list.
ITC_NATIVE_ENTRY(8258E5F8, ITC_FLAG(native_render_queue)) {
  ctx.r3.u64 = rq::Recording(ctx.r3.u32);
}

ITC_NATIVE_ENTRY(8258E618, ITC_FLAG(native_render_queue)) {
  ctx.r3.u64 = rq::Replaying(ctx.r3.u32);
}

// QueueEntry::Sequence: r3 = the entry; f1 = the float at +8.
ITC_NATIVE_ENTRY(8258E5C0, ITC_FLAG(native_render_queue)) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(rq::Sequence(ctx.r3.u32));
}
