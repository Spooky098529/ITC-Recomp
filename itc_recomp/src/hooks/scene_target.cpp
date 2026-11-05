// The scene target's passes natively: adapters in front of
// src/scene_target/ (docs/functions/scene-target.md). Each leaves r3 as the
// original does. TexturedRect::RecordFlat (sub_8217AA98) has no adapter:
// only TexturedRect::Record calls it, and the flag switches that. With the
// command list's diagnostics on (they name who allocates slots by the
// return address), the originals run.

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/scene_target.h"
#include "scene_target/scene_target.h"

#include <optional>

ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

ITC_FLAG_BOOL(native_scene_target, true, "ITC",
              "Run the scene target's passes (docs/functions/scene-target.md: "
              "SceneTarget::RecordTargets sub_8209AE48, RecordTileTargets sub_8209AF40, "
              "RecordBoundTargets sub_8209B058, TexturedRect::Record sub_8217C368 with "
              "TexturedRect::RecordFlat sub_8217AA98 (no adapter: only Record calls it; the "
              "render queue's Add a guest call)) natively instead of through the translated "
              "code (the same writes; the originals with the command list's diagnostics on; "
              "checked by --run_harnesses=scene_target)");

namespace {

bool Native() {
  return ITC_FLAG(native_scene_target) && !ITC_FLAG(log_overlay_uploads) &&
         !ITC_FLAG(log_slot_callers) && !ITC_FLAG(log_recording_by_pass);
}

}  // namespace

// Runs the original when the plain C++ has no answer (the list full).
#define ITC_SCENE_TARGET_RECORD(address, ...) \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    std::optional<uint32_t> r3;               \
    if (Native()) {                           \
      r3 = __VA_ARGS__;                       \
    }                                         \
    if (!r3) {                                \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    ctx.r3.u64 = *r3;                         \
  }

// r3 = the scene target, r4 = the command list (r5 not read).
ITC_SCENE_TARGET_RECORD(8209AE48, scene_target::RecordTargets(base, ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_TARGET_RECORD(8209AF40, scene_target::RecordTileTargets(base, ctx.r3.u32, ctx.r4.u32))
// r3 = the command list (r4 passed on as r5, not read).
ITC_SCENE_TARGET_RECORD(8209B058, scene_target::RecordBoundTargets(base, ctx.r3.u32))

void hooks::SceneTargetTexturedRect(PPCContext& ctx, uint8_t* base) {
  const uint32_t rect = ctx.r3.u32;
  const uint64_t bucket = ctx.r5.u64;
  const uint64_t lr = ctx.lr;
  bool done = false;
  guest_call::InFrame(ctx, base, scene_target::kRecordFlatFrame, 0x8217AEA0, [&](uint32_t frame) {
    const auto chunk = scene_target::TexturedRectRecordFlat(base, rect, frame);
    if (!chunk) {
      return;
    }
    done = true;
    // The render queue's slot 4: r4 = the chunk, r5 = the bucket, f1 = 0.0.
    ctx.r3.u64 = guest::LoadU32(base, scene_target::kRectLists);
    ctx.r4.u64 = *chunk;
    ctx.r5.u64 = bucket;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
    const uint32_t function =
        guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 16);
    ctx.ctr.u64 = function;
    ctx.lr = 0x8217AEA0;
    REX_CALL_INDIRECT_FUNC(function);
  });
  ctx.lr = lr;
  if (!done) {
    __imp__sub_8217C368(ctx, base);
  }
}

// TexturedRect::Record: r3 = the rectangle, r4 not read, r5 = the bucket.
extern "C" REX_FUNC(sub_8217C368) {
  REX_FUNC_PROLOGUE();
  if (!Native()) {
    __imp__sub_8217C368(ctx, base);
    return;
  }
  hooks::SceneTargetTexturedRect(ctx, base);
}
