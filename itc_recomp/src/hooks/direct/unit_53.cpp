// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 53's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "hooks/heap.h"
#include "track_collision/track_collision.h"

// TrackMesh::InitScratch: r3 = the 8-byte block; r3 = the 8 KB block, as the
// original leaves it. It allocates pages: the original unless --native_heap
// routes VirtualAlloc to the allocation layer.
ITC_DIRECT_ENTRY(820F6898) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820F6898(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = track_collision::TrackMeshInitScratch(ctx.r3.u32);
}

// TrackCollision::QueryWalls: r3 = the wall query, r4 = the mesh, from its
// 128-byte frame. r3 isn't kept (its caller doesn't read it). A template
// that isn't a box: the original.
ITC_DIRECT_ENTRY(820FDF38) {
  ctx.fpscr.disableFlushMode();
  if (!track_collision::TrackCollisionQueryWalls(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 128)) {
    lifted::original::c820FDF38(ctx);
  }
}

// TrackCollision::Ground: r3 = the ground result, r4 = the point, r5 = the
// track position (r6 not read), from its 144-byte frame. r3 = 1.
ITC_DIRECT_ENTRY(820FE8F8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = track_collision::TrackCollisionGround(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r1.u32 - 144);
}
