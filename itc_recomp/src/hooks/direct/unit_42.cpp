// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 42's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "car_scene_draw/car_scene_draw.h"
#include "hooks/billboards.h"
#include "hooks/car_scene_draw.h"
#include "lifted/originals.h"
#include "schema/track_zones.h"
#include "sprites/billboards.h"
#include "track_zones/track_zones.h"

// --- sprites ---

// Billboards::RecordState: r3 = the camera pass set, r4 = the command list,
// r5 = the material, from its frame below r1; r3 = the last slot.
// Flush-to-zero off at entry and exit (its harness steps' mapping); the
// original under the recording's conditions (hooks::BillboardsNative).
ITC_DIRECT_ENTRY(8209C1A0) {
  if (!hooks::BillboardsNative()) {
    lifted::original::c8209C1A0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx);
  const uint32_t r3 = sprites::RecordBillboardState(calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                                    ctx.r5.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

// LightGroups::Draw: r3 = the set, r4 = the groups, r5 = the XPR, r6 = the
// animations; its register step (hooks::LightGroupsDraw), the original
// without its conditions (hooks::LightGroupsNative).
ITC_DIRECT_ENTRY(82148658) {
  if (!hooks::LightGroupsNative()) {
    lifted::original::c82148658(ctx);
    return;
  }
  hooks::LightGroupsDraw(ctx);
}

// --- track_zones ---

// TrackZones::TrackZones: r3 = the owner (left).
ITC_DIRECT_ENTRY(8228B608) { track_zones::OwnerConstruct(ctx.r3.u32); }

// List128::Insert: r3 = the list (left), r4 = &the word, r5 = the position
// (as List64::Insert's adapter).
ITC_DIRECT_ENTRY(8228BC18) {
  track_zones::List128Insert(ctx.r3.u32, schema::TrackZonesListWord(ctx.r4.u32).value(),
                             ctx.r5.u32);
}
