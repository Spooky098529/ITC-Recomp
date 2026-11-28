// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 23's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "track_collision/track_collision.h"

// TrackMesh::SliceContains: r3 = a slice, r4 = the section's outline, r5 =
// a point; r3 = 1 or 0.
ITC_DIRECT_ENTRY(820F6A38) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = track_collision::TrackMeshSliceContains(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// TrackMesh::FindSlice: r3 = a section, r4 = a slice or -1, r5 = the point;
// r3 = the slice found or -1, sign-extended as the original's li r3,-1.
ITC_DIRECT_ENTRY(820F6BE8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = uint64_t(int64_t(int32_t(
      track_collision::TrackMeshFindSlice(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))));
}
