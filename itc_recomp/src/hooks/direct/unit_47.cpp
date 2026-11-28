// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 47's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "hooks/lights.h"
#include "replay_snapshots/replay_snapshots.h"

// Record5232::Restore: r3 = the record, r4 = the vehicle; r3 as the last
// part's restore leaves it.
ITC_DIRECT_ENTRY(821044D8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = replay_snapshots::Record5232Restore(ctx.r3.u32, ctx.r4.u32);
}

// RaceSnapshot::Save: r3 = the snapshot block.
ITC_DIRECT_ENTRY(82104AB8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = replay_snapshots::RaceSnapshotSave(ctx.r3.u32);
}

// Lights::FindNearest, as its deleted adapter: r3 = the light sets' owner,
// r4 = the output (32 bytes a light), r5 = the point, r6 = most lights, r9 =
// the direction, f1 = the merge distance, f2 = the farthest squared
// distance, f3 = the least dot product; r3 = the lights written. The
// original where the port declines.
ITC_DIRECT_ENTRY(820F4128) {
  ctx.fpscr.disableFlushMode();
  const auto found = lights::FindNearest(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32,
                                         ctx.r9.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64);
  if (!found) [[unlikely]] {
    lifted::original::c820F4128(ctx);
    return;
  }
  ctx.r3.s64 = int32_t(*found);
}
