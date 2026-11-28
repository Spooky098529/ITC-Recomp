// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 14's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include "c_runtime/format.h"
#include "lifted/originals.h"
#include "track_collision/track_collision.h"

// WallQuery::AddHit: r3 = the query, r4 = the surface bits; r3 the world's
// +60.
ITC_DIRECT_ENTRY(820FC2E0) {
  ctx.r3.u64 = track_collision::WallQueryAddHit(ctx.r3.u32, ctx.r4.u32);
}
// Segments::Intersect2D: r3 = the four points' pointers, r4 = the crossing
// out; r3 0 or 1.
ITC_DIRECT_ENTRY(820FC658) {
  ctx.r3.u64 = track_collision::SegmentsIntersect2D(ctx.r3.u32, ctx.r4.u32);
}
// The wide write_char: r3 = the UTF-16 unit, r4 = the stream, r5 = the
// count; r3 left as it came (a void function); the original where the port
// declines.
ITC_DIRECT_ENTRY(825B1368) {
  if (!c_runtime::WriteWideChar(uint16_t(ctx.r3.u32), ctx.r4.u32, ctx.r5.u32)) {
    lifted::original::c825B1368(ctx);
  }
}
