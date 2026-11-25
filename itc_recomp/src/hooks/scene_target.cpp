// The scene target's passes natively: adapters in front of
// src/scene_target/ (docs/functions/scene-target.md). Each leaves r3 as the
// original does. SceneTarget::RecordTargets (sub_8209AE48) and
// RecordTileTargets (sub_8209AF40) have no adapters: only RecordPasses' and
// RecordBoundTargets' plain C++ call them; nor RecordBoundTargets
// (sub_8209B058): only Glow::Record's plain C++ (--native_post_effects)
// calls it; nor TexturedRect::Record
// (sub_8217C368, its work ITC_REGS_FN(hooks::SceneTargetTexturedRect), which
// RecordPasses' adapter calls) and RecordFlat (sub_8217AA98). With the
// command list's diagnostics on (they name who allocates slots by the
// return address), the originals run.

#include "guest.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "hooks/direct_slots.h"
#include "hooks/guest_call.h"
#include "hooks/scene_target.h"
#include "scene_target/scene_target.h"
#include "schema/app_start_up.h"
#include "schema/scene_target.h"

#include <optional>
#include "lifted/sdk.h"

ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

ITC_FLAG_BOOL(native_scene_target, true, "ITC",
              "Run the scene target's passes (docs/functions/scene-target.md: "
              "SceneTarget::RecordPasses sub_8209B3D0 (no adapter: only FramePasses::Record's adapter calls its work; the render manager's getters and the "
              "render queue's Add guest calls) with RecordTargets sub_8209AE48, RecordTileTargets "
              "sub_8209AF40, TexturedRect::Record sub_8217C368 and RecordFlat sub_8217AA98 (no "
              "adapters: only RecordPasses calls them; the rectangle's render queue Add a guest "
              "call); RecordBoundTargets sub_8209B058 with no adapter: only Glow::Record's plain "
              "C++ calls it) natively instead of through the translated "
              "code (the same writes; the originals with the command list's diagnostics on; "
              "checked by --run_harnesses=scene_target)");

namespace {

bool Native() {
  return ITC_FLAG(native_scene_target) && !ITC_FLAG(log_overlay_uploads) &&
         !ITC_FLAG(log_slot_callers) && !ITC_FLAG(log_recording_by_pass);
}

}  // namespace


void hooks::SceneTargetTexturedRect(lifted::Regs& ctx) {
  const uint32_t rect = ctx.r3.u32;
  const uint64_t bucket = ctx.r5.u64;
  const uint64_t lr = ctx.lr;
  bool done = false;
  guest_call::InFrame(ctx, scene_target::kRecordFlatFrame, 0x8217AEA0, [&](uint32_t frame) {
    const auto chunk = scene_target::TexturedRectRecordFlat(rect, frame);
    if (!chunk) {
      return;
    }
    done = true;
    // The render queue's slot 4: r4 = the chunk, r5 = the bucket, f1 = 0.0.
    ctx.r3.u64 = schema::SceneTargetGlobals::rect_queue();
    ctx.r4.u64 = *chunk;
    ctx.r5.u64 = bucket;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(schema::SceneTargetGlobals::zero());
    const uint32_t function =
        guest::LoadU32(guest::LoadU32(ctx.r3.u32) + 16);
    ctx.ctr.u64 = function;
    ctx.lr = 0x8217AEA0;
    lifted::CallFromContext(ctx, function);
  });
  ctx.lr = lr;
  if (!done) {
    lifted::original::c8217C368(ctx);
  }
}

void hooks::SceneTargetRecordPasses(lifted::Regs& ctx) {
  const uint32_t scene = ctx.r3.u32, list = ctx.r4.u32;
  const uint32_t bucket5 = ctx.r5.u32, bucket6 = ctx.r6.u32, bucket7 = ctx.r7.u32;
  const uint64_t lr = ctx.lr;
  bool done = false;
  guest_call::InFrame(
      ctx, scene_target::kRecordPassesFrame, 0x8209B634, [&](uint32_t frame) {
        auto vtable_call = [&](uint32_t byte, uint32_t return_address) {
          const uint32_t function = guest::LoadU32(guest::LoadU32(ctx.r3.u32) + byte);
          ctx.ctr.u64 = function;
          ctx.lr = return_address;
          // Slot 8's plain C++ where it's the game's (hooks::DirectSlotCall).
          if (!hooks::DirectSlotCall(ctx, function)) {
            lifted::CallFromContext(ctx, function);
          }
        };
        scene_target::PassCalls calls;
        // The render manager's slot 8, its answer's slot 2 (the render
        // queue), the queue's slot 4 (f1 = 0.0, the original's f30).
        calls.submit = [&](uint32_t chunk, uint32_t bucket, uint32_t lr1, uint32_t lr2,
                           uint32_t lr3) {
          ctx.r3.u64 = schema::AppStartUpGlobals::render_app<schema::View>().address;
          vtable_call(32, lr1);
          vtable_call(8, lr2);
          ctx.fpscr.disableFlushMode();
          ctx.f1.f64 = double(schema::SceneTargetGlobals::zero());
          ctx.r5.u64 = bucket;
          ctx.r4.u64 = chunk;
          vtable_call(16, lr3);
        };
        calls.rect = [&](uint32_t rect, uint32_t bucket, uint32_t return_address) {
          ctx.r3.u64 = rect;
          ctx.r4.u64 = 0;
          ctx.r5.u64 = bucket;
          ctx.lr = return_address;
          hooks::SceneTargetTexturedRect(ctx);
        };
        done = scene_target::RecordPasses(frame, scene, list, bucket5, bucket6, bucket7,
                                          calls);
      });
  ctx.lr = lr;
  if (!done) {
    lifted::original::c8209B3D0(ctx);
  }
}

// SceneTarget::RecordPasses: r3 = the scene target, r4 = the command list,
// r5, r6, r7 = the buckets; r3 as the last guest call leaves it (the scene
// target when no pass is recorded). No adapter: only FramePasses::Record's
// adapter calls this.
void hooks::SceneTargetRecordPassesOrOriginal(lifted::Regs& ctx) {
  if (!Native()) {
    lifted::original::c8209B3D0(ctx);
    return;
  }
  hooks::SceneTargetRecordPasses(ctx);
}
