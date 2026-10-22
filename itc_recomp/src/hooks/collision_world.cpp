// The collision world's owner and the track's collision natively: adapters
// in front of src/track_collision/ (docs/functions/collision-world.md). Each
// passes the original's stack frame (its r1 after the stwu) where the plain
// C++ keeps temporaries there, and leaves r3 (and f1 where the original
// leaves a value there) as the original does.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "track_collision/track_collision.h"

ITC_FLAG_BOOL(native_collision_world, true, "ITC",
              "Run the collision world's owner and the track's collision "
              "(docs/functions/collision-world.md: sub_8228F090, Race::RacerCount sub_821705F8, "
              "Racers::CarsCollide sub_820FEFF8, TrackCollision::ResetWorld sub_820FEAF8; the "
              "mesh's and the route's lifetime, sub_820F68E8 with sub_820F6898, sub_820F6940, "
              "sub_820F8600, sub_820F8618 (those allocating or freeing only while --native_heap "
              "routes the game's VirtualAlloc to the allocation layer), sub_820F69B0; the "
              "mesh's lookups, sub_820F6A08; the ground, sub_820F7AA0 with sub_820F72F8, "
              "sub_820F7468, sub_820F7590, sub_820F7818, sub_820F6E18, and "
              "TrackCollision::Ground sub_820FE8F8, GroundCallback sub_820FEAB0 and GroundHits "
              "sub_820FEBD0, for boxes; the track position's cell, sub_820F8090 with sub_820F6A38, "
              "sub_820F6BE8, sub_820F71B0, sub_820F7EE8, sub_820F7E30, sub_820F7D70, "
              "sub_820F6C88; the wall query's parts, sub_820FC188 with sub_820FC110, sub_820FC2E0, "
              "sub_820FC658; the route, sub_820F9050) natively "
              "instead of through the translated code (the same words, bit for bit; checked by "
              "--run_harnesses=collision_world). Those reached only from native callers have no "
              "adapters, and the flag switches their callers: sub_820F6898, sub_820F72F8, "
              "sub_820F7468, sub_820F7590, sub_820F7818, sub_820F6E18, sub_820F6A38, sub_820F6BE8, "
              "sub_820F71B0, sub_820F7EE8, sub_820F7E30, sub_820F7D70, sub_820F6C88, sub_820FC110, "
              "sub_820FE8F8");

// Defines an adapter: `body` uses `ctx` and `base`.
#define ITC_COLLISION_WORLD(address, body)   \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_collision_world)) { \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    ctx.fpscr.disableFlushMode();            \
    body;                                    \
  }
// The same for one allocating or freeing pages: the original unless
// --native_heap routes VirtualAlloc to the allocation layer.
#define ITC_COLLISION_WORLD_PAGES(address, body)                              \
  extern "C" REX_FUNC(sub_##address) {                                        \
    REX_FUNC_PROLOGUE();                                                      \
    if (!ITC_FLAG(native_collision_world) || !hooks::NativeHeapActive()) {     \
      __imp__sub_##address(ctx, base);                                        \
      return;                                                                 \
    }                                                                         \
    body;                                                                     \
  }

using namespace track_collision;

// --- The world's owner ---

ITC_COLLISION_WORLD(8228F090, ctx.r3.u64 = TimesThreePlus(ctx.r3.u32, ctx.r4.u32))
ITC_COLLISION_WORLD(821705F8, ctx.r3.u64 = RaceRacerCount(base))
ITC_COLLISION_WORLD(820FEFF8, ctx.r3.u64 = RacersCarsCollide(base, ctx.r3.u32))
ITC_COLLISION_WORLD(820FEAF8, ctx.r3.u64 = TrackCollisionResetWorld(base))

// --- The mesh's and the route's lifetime ---

// TrackMesh::InitScratch (sub_820F6898) has no adapter: its only caller,
// TrackMesh::Prepare, is native.
ITC_COLLISION_WORLD_PAGES(820F68E8, ctx.r3.u64 = TrackMeshPrepare(base, ctx.r3.u32))
// A block that isn't the layer's: the original.
ITC_COLLISION_WORLD_PAGES(820F6940, uint32_t r3 = 0;
                          if (!TrackMeshFree(base, ctx.r3.u32, &r3)) {
                            __imp__sub_820F6940(ctx, base);
                            return;
                          } ctx.r3.u64 = r3)
ITC_COLLISION_WORLD(820F8600, TrackRoutePrepare(base, ctx.r3.u32))
ITC_COLLISION_WORLD_PAGES(820F8618, uint32_t r3 = 0;
                          if (!TrackRouteFree(base, ctx.r3.u32, &r3)) {
                            __imp__sub_820F8618(ctx, base);
                            return;
                          } ctx.r3.u64 = r3)
// Archive::Load's callback: r6 = the mesh.
ITC_COLLISION_WORLD(820F69B0, TrackMeshOnLoaded(base, ctx.r6.u32))

// --- The mesh's lookups ---

ITC_COLLISION_WORLD(820F6A08, ctx.r3.u64 = TrackMeshSliceKind(base, ctx.r3.u32, ctx.r4.u32))

// --- The ground ---

// TrackMesh::TriangleInSlice, TriangleNormal, QuadHeight, FindTriangle and
// ColorAt (sub_820F72F8, sub_820F7468, sub_820F7590, sub_820F7818,
// sub_820F6E18) have no adapters: their callers in play are native.
// r4 = the point, r5 = the track position, r6 = the color, r7 = the normal.
ITC_COLLISION_WORLD(820F7AA0, ctx.r3.u64 = TrackMeshGroundHeight(base, ctx.r3.u32, ctx.r4.u32,
                                                                 ctx.r5.u32, ctx.r6.u32,
                                                                 ctx.r7.u32, ctx.r1.u32 - 240))
// TrackCollision::Ground (sub_820FE8F8) has no adapter: its callers in
// play, GroundCallback and GroundHits, are native.
// The collision world's +8 (CollisionWorld::Callback).
ITC_COLLISION_WORLD(820FEAB0,
                    ctx.r3.u64 = TrackCollisionGroundCallback(base, ctx.r3.u32, ctx.r4.u32,
                                                              ctx.r5.u32, ctx.r1.u32 - 96))
// The collision world's second query: r3 = the hit records, r4 = the shape
// (r5 not read). A shape that isn't a box: the original.
extern "C" REX_FUNC(sub_820FEBD0) {
  REX_FUNC_PROLOGUE();
  uint32_t count = 0;
  if (ITC_FLAG(native_collision_world)) {
    ctx.fpscr.disableFlushMode();
  }
  if (!ITC_FLAG(native_collision_world) ||
      !TrackCollisionGroundHits(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 384, &count)) {
    __imp__sub_820FEBD0(ctx, base);
    return;
  }
  ctx.r3.u64 = count;
}

// --- The track position's cell ---

// TrackMesh::SliceContains, FindSlice and TrackPosition::CrossSection
// (sub_820F6A38, sub_820F6BE8, sub_820F71B0), and the full searches
// (sub_820F7EE8, sub_820F7E30, sub_820F7D70, sub_820F6C88, never seen in
// play) have no adapters: their callers are native.
// r3 = the mesh, r4 = the track position, r5 = its point.
ITC_COLLISION_WORLD(820F8090, ctx.r3.u64 = TrackPositionFindCell(base, ctx.r3.u32, ctx.r4.u32,
                                                                 ctx.r5.u32, ctx.r1.u32 - 240))

// --- The wall query's parts and the route ---

// WallQuery::AddCell (sub_820FC110) has no adapter: its only caller is
// native.
// r3 = the query, r4 = the mesh. A shape that isn't a box: the original.
extern "C" REX_FUNC(sub_820FC188) {
  REX_FUNC_PROLOGUE();
  uint32_t count = 0;
  if (!ITC_FLAG(native_collision_world) ||
      !WallQueryGatherCells(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192, &count)) {
    __imp__sub_820FC188(ctx, base);
    return;
  }
  ctx.r3.u64 = count;
}
// r4 = the surface bits.
ITC_COLLISION_WORLD(820FC2E0, ctx.r3.u64 = WallQueryAddHit(base, ctx.r3.u32, ctx.r4.u32))
// r3 = the four points, r4 = out.
ITC_COLLISION_WORLD(820FC658, ctx.r3.u64 = SegmentsIntersect2D(base, ctx.r3.u32, ctx.r4.u32))
// Archive::Load's callback: r6 = the route; r3 comes back as 12 (the
// original's count of route bits).
ITC_COLLISION_WORLD(820F9050, TrackRouteOnLoaded(base, ctx.r6.u32); ctx.r3.u64 = 12)
