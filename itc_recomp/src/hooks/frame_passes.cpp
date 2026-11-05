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
#include "hooks/post_effects.h"
#include "hooks/scene_target.h"

ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);
ITC_FLAG_DECLARE(bool, native_video_output);

ITC_FLAG_BOOL(native_frame_passes, true, "ITC",
              "Run the frame passes object (docs/functions/frame-passes.md: "
              "FramePasses::FlipWorkMem sub_82096218 with WorkMem::Flip sub_82589A78 (no adapter: "
              "only FlipWorkMem calls it), the delegate thunk sub_82096350 (its target a tail "
              "call), FramePasses::RecordConstants sub_82096360, RecordSamplers sub_82096030 and "
              "RecordPrimitiveReset sub_82589990 (the render queue's Add a guest call), Job::Wait "
              "sub_82094B10 (inside --find_menu_selection's wrap; the job's object a guest call) "
              "FramePasses::Boot sub_820962B8 (with --native_video_output; the getters and "
              "PostEffects::InitBoot guest calls) and FramePasses::FramePasses sub_82098630 (the "
              "pool's post, Job::Wait's wrap and the run loop's adds guest calls) and "
              "FramePasses::Record sub_82096418 (the getters and the render queue's adds guest "
              "calls; SceneTarget::RecordPasses and RecordPrimitiveReset through their adapters)) natively "
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

void hooks::FramePassesJobWait(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_frame_passes)) {
    __imp__sub_82094B10(ctx, base);
    return;
  }
  const uint32_t job = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 96, 0x82094B48, [&](uint32_t) {
    const bool waited = fp::JobWait(base, job, [&](uint32_t object) {
      // The object's slot 1 with r4 = the job.
      ctx.r3.u64 = object;
      ctx.r4.u64 = job;
      const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object) + 4);
      ctx.r11.u64 = function;
      ctx.ctr.u64 = function;
      ctx.lr = 0x82094B48;
      REX_CALL_INDIRECT_FUNC(function);
    });
    // r11 as the original leaves it: 0 after the wait, else the byte (0).
    ctx.r11.u64 = 0;
    if (!waited) {
      ctx.r3.u64 = job;
    }
  });
  ctx.lr = lr;
}

// FramePasses::Boot: r3 = it; the guest calls from a 112-byte frame like the
// original's (the interface's and the renderer's getters, PostEffects::InitBoot
// through its adapter's work); the device lock over the native video output
// (the original without --native_video_output). r3 the video output.
void hooks::FramePassesBoot(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_frame_passes) || !ITC_FLAG(native_video_output)) {
    __imp__sub_820962B8(ctx, base);
    return;
  }
  const uint32_t passes = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, fp::kBootFrame, 0x820962DC, [&](uint32_t frame) {
    auto vtable_call = [&](uint32_t object, uint32_t byte, uint32_t return_address) {
      ctx.r3.u64 = object;
      const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object) + byte);
      ctx.r11.u64 = function;
      ctx.ctr.u64 = function;
      ctx.lr = return_address;
      REX_CALL_INDIRECT_FUNC(function);
      return ctx.r3.u32;
    };
    const fp::BootCalls calls{
        [&](uint32_t interface) {
          const uint32_t renderer = vtable_call(interface, 8, 0x820962DC);
          return vtable_call(renderer, 12, 0x820962EC);
        },
        [&](uint32_t device) {
          ctx.r3.u64 = device;
          ctx.lr = 0x820962FC;
          hooks::PostEffectsInitBoot(ctx, base);
        }};
    r3 = fp::Boot(base, passes, frame, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

extern "C" REX_FUNC(sub_820962B8) {
  REX_FUNC_PROLOGUE();
  hooks::FramePassesBoot(ctx, base);
}

// FramePasses::FramePasses: r3 = it, r4 = the block; the guest calls from a
// 224-byte frame like the original's (the delegates and the key there, the
// word it reads at its +164 read there); r3 it.
void hooks::FramePassesConstruct(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_frame_passes)) {
    __imp__sub_82098630(ctx, base);
    return;
  }
  const uint32_t passes = ctx.r3.u32;
  const uint32_t block = ctx.r4.u32;
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, fp::kConstructFrame, 0x820986F0, [&](uint32_t frame) {
    auto vtable_call = [&](uint32_t object, uint32_t byte, uint32_t return_address) {
      ctx.r3.u64 = object;
      const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object) + byte);
      ctx.r11.u64 = function;
      ctx.ctr.u64 = function;
      ctx.lr = return_address;
      REX_CALL_INDIRECT_FUNC(function);
      return ctx.r3.u32;
    };
    const fp::ConstructCalls calls{
        [&](uint32_t interface, uint32_t job) {
          const uint32_t pool = vtable_call(interface, 4, 0x820986F0);
          ctx.r5.u64 = 1;
          ctx.r4.u64 = job;
          vtable_call(pool, 4, 0x82098708);
        },
        [&](uint32_t job) {
          ctx.r3.u64 = job;
          ctx.lr = 0x82098710;
          sub_82094B10(ctx, base);  // --find_menu_selection's wrap
        },
        [&](uint32_t loop, uint32_t offset, uint32_t key, uint32_t delegate, uint32_t lr) {
          ctx.r5.u64 = delegate;
          ctx.r4.u64 = key;
          vtable_call(loop, offset, lr);
        }};
    fp::Construct(base, passes, block, frame, guest::LoadU32(base, frame + 164), calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = passes;
}

extern "C" REX_FUNC(sub_82098630) {
  REX_FUNC_PROLOGUE();
  hooks::FramePassesConstruct(ctx, base);
}

// FramePasses::Record: r3 = it; the guest calls from a 320-byte frame like
// the original's (the getters, the render queue's adds with f1 = 0.0, the
// original's f31); SceneTarget::RecordPasses and RecordPrimitiveReset
// through their adapters' work. r3 as the last leaves it. The original
// where the plain C++ declines (the getters then called again) or with the
// command list's diagnostics on.
void hooks::FramePassesRecord(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeFramePasses()) {
    __imp__sub_82096418(ctx, base);
    return;
  }
  const uint32_t passes = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  bool done = false;
  guest_call::InFrame(ctx, base, fp::kRecordFrame, 0x82096440, [&](uint32_t frame) {
    auto vcall = [&](uint32_t object, uint32_t byte, uint32_t return_address) {
      ctx.r3.u64 = object;
      const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object) + byte);
      ctx.r11.u64 = function;
      ctx.ctr.u64 = function;
      ctx.lr = return_address;
      REX_CALL_INDIRECT_FUNC(function);
      return ctx.r3.u32;
    };
    const fp::RecordCalls calls{
        vcall,
        [&](uint32_t queue, uint32_t chunk, uint32_t bucket, uint32_t return_address) {
          ctx.fpscr.disableFlushMode();
          ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
          ctx.r5.u64 = bucket;
          ctx.r4.u64 = chunk;
          vcall(queue, 16, return_address);
        },
        [&](uint32_t list) {
          ctx.r7.u64 = 121;
          ctx.r6.u64 = 602;
          ctx.r5.u64 = 150;
          ctx.r4.u64 = list;
          ctx.r3.u64 = guest::LoadU32(base, 0x8266D08C);
          ctx.lr = 0x82098620;
          sub_8209B3D0(ctx, base);
        },
        [&]() {
          ctx.lr = 0x82098624;
          sub_82589990(ctx, base);
        }};
    done = fp::Record(base, passes, frame, calls);
  });
  ctx.lr = lr;
  if (!done) {
    __imp__sub_82096418(ctx, base);
  }
}

// TEMPORARY: the game keeps the original until the three parts are written
// (the harness calls FramePassesRecord directly).
constexpr bool kRecordPartsWritten = false;

extern "C" REX_FUNC(sub_82096418) {
  REX_FUNC_PROLOGUE();
  if (!kRecordPartsWritten) {
    __imp__sub_82096418(ctx, base);
    return;
  }
  hooks::FramePassesRecord(ctx, base);
}
