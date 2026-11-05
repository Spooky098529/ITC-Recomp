// The frame passes natively: adapters in front of src/frame_passes/
// (docs/functions/frame-passes.md). Each leaves r3 as the original does.
// WorkMem::Flip (sub_82589A78) has no adapter: only FramePasses::FlipWorkMem
// calls it. With the command list's diagnostics on, the recorders'
// originals run.

#include "guest.h"

#include "base/flags.h"
#include "frame_passes/frame_passes.h"
#include "hooks/frame_passes.h"
#include "hooks/guest_call.h"

ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

ITC_FLAG_BOOL(native_frame_passes, true, "ITC",
              "Run the frame passes object (docs/functions/frame-passes.md: "
              "FramePasses::FlipWorkMem sub_82096218 with WorkMem::Flip sub_82589A78 (no adapter: "
              "only FlipWorkMem calls it), the delegate thunk sub_82096350 (its target a tail "
              "call), FramePasses::RecordConstants sub_82096360, RecordSamplers sub_82096030 and "
              "RecordPrimitiveReset sub_82589990 (the render queue's Add a guest call)) natively "
              "instead of through the translated code (the same writes; the recorders' originals "
              "with the command list's diagnostics on; checked by "
              "--run_harnesses=frame_passes)");

namespace fp = frame_passes;

bool hooks::NativeFramePasses() {
  return ITC_FLAG(native_frame_passes) && !ITC_FLAG(log_overlay_uploads) &&
         !ITC_FLAG(log_slot_callers) && !ITC_FLAG(log_recording_by_pass);
}

#define ITC_FRAME_PASSES_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                     \
  if (!hooks::NativeFramePasses()) {       \
    __imp__sub_##address(ctx, base);       \
    return;                                \
  }

// FramePasses::FlipWorkMem: r3 not read, left as it came.
extern "C" REX_FUNC(sub_82096218) {
  ITC_FRAME_PASSES_FALLBACK(82096218)
  fp::FlipWorkMem(base);
}

// The delegate thunk: r3 = the object; its slot 2 as a tail call (r12 the
// vtable and r11 the target, as the original leaves them).
extern "C" REX_FUNC(sub_82096350) {
  ITC_FRAME_PASSES_FALLBACK(82096350)
  const uint32_t target = fp::DelegateSlot2Target(base, ctx.r3.u32);
  ctx.r12.u64 = guest::LoadU32(base, ctx.r3.u32);
  ctx.r11.u64 = target;
  ctx.ctr.u64 = target;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
}

// FramePasses::RecordConstants and RecordSamplers: r4 = the command list;
// r3 the last slot, r4 = 1 (the count of the original's last AllocateSlots).
extern "C" REX_FUNC(sub_82096360) {
  ITC_FRAME_PASSES_FALLBACK(82096360)
  const auto last = fp::RecordConstants(base, ctx.r4.u32);
  if (!last) {
    __imp__sub_82096360(ctx, base);
    return;
  }
  ctx.r3.u64 = *last;
  ctx.r4.u64 = 1;
}

extern "C" REX_FUNC(sub_82096030) {
  ITC_FRAME_PASSES_FALLBACK(82096030)
  const auto last = fp::RecordSamplers(base, ctx.r4.u32);
  if (!last) {
    __imp__sub_82096030(ctx, base);
    return;
  }
  ctx.r3.u64 = *last;
  ctx.r4.u64 = 1;
}

void hooks::FramePassesRecordPrimitiveReset(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  bool done = false;
  guest_call::InFrame(ctx, base, fp::kRecordPrimitiveResetFrame, 0x82589A6C, [&](uint32_t) {
    const auto chunk = fp::RecordPrimitiveReset(base);
    if (!chunk) {
      return;
    }
    done = true;
    // The render queue's slot 4: r4 = the chunk, r5 = 0, f1 = 0.0.
    ctx.r3.u64 = guest::LoadU32(base, fp::kRenderQueue);
    ctx.r4.u64 = *chunk;
    ctx.r5.u64 = 0;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 16);
    ctx.ctr.u64 = function;
    ctx.lr = 0x82589A6C;
    REX_CALL_INDIRECT_FUNC(function);
  });
  ctx.lr = lr;
  if (!done) {
    __imp__sub_82589990(ctx, base);
  }
}

// FramePasses::RecordPrimitiveReset: no arguments; r3 as the render queue's
// Add leaves it.
extern "C" REX_FUNC(sub_82589990) {
  ITC_FRAME_PASSES_FALLBACK(82589990)
  hooks::FramePassesRecordPrimitiveReset(ctx, base);
}
