// The collision world's owner and the track's collision as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/collision-world.md): the track's collision mesh (at
// 0x826C5B1C) and route (0x826C5B10), the track positions, the ground and
// wall queries the collision world calls, and the world's reset and tick.
// The mesh, the positions and the world are guest data shared with the
// recompiled code, so these work on guest memory, with the originals' loads
// and stores in their order and their arithmetic step for step
// (src/car_physics/float_ops.h), so the results are the translated code's
// bit for bit. Temporaries the originals keep on their stack go at the same
// offsets of `frame` (the original's r1 after its stwu, guest memory).
// Flush-to-zero off on entry (the adapters' job); the collision shapes',
// the car physics' and the vector library's plain C++ is called directly.
// Built with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).
// Adapters: src/hooks/collision_world.cpp.

#pragma once

#include <cstdint>

#include "car_physics/car_physics.h"

namespace track_collision {

// The mesh and the route.
constexpr uint32_t kMesh = 0x826C5B1C;
constexpr uint32_t kRoute = 0x826C5B10;

// --- The world's owner (src/track_collision/track_world.cpp) ---

// sub_8228F090: a * 3 + b (the course's index from the race data's +32
// and +4, among others).
uint32_t TimesThreePlus(uint32_t a, uint32_t b);
// Race::RacerCount (sub_821705F8): the word +2364 of [0x826D82F4].
uint32_t RaceRacerCount(uint8_t* base);
// Racers::CarsCollide (sub_820FEFF8): 0 when the car count (`records` +
// 0x595E0) is 1, or a record's flags (+3888, 3,936 bytes apart) have bit
// 0x8; else 1.
uint32_t RacersCarsCollide(uint8_t* base, uint32_t records);
// TrackCollision::ResetWorld (sub_820FEAF8): CollisionWorld::Reset, then
// the surface settings for 0x02000000, 0x01000000 and 0x04000000 as
// CollisionWorld::Setup sets them. Returns the original's r3 (Reset's).
uint32_t TrackCollisionResetWorld(uint8_t* base);
// TrackCollision::Tick (sub_820FF050): with the mesh and the route loaded
// (their +8 = 2), CollisionWorld::Update (the car physics' plain C++, its
// frame 176 bytes below `frame`, r3 Racers::CarsCollide's answer) with the
// float at 0x8260327C, or 0.0 when the cars don't collide with each other;
// then each racer's vehicle: +6152 = 0, bit 0x1 for a contact with another
// object, else bit 0x2 and +6236 = its strength; +6240 counts the ticks off
// the ground (+348 0), and on landing after 10 or more, bit 0x4 and +6232 =
// the count. `frame`: the original's 144 bytes; `guest` the update's guest
// calls (its adapter's).
void TrackCollisionTick(uint8_t* base, uint32_t frame, const car_physics::CollisionGuestCall& guest);

// --- The mesh's and the route's lifetime (src/track_collision/track_mesh.cpp).
// The pages come from the allocation layer (src/memory/heap.h), so the
// adapters call the allocating and freeing ones only while --native_heap
// routes the game's VirtualAlloc to it (hooks::NativeHeapActive). ---

// TrackMesh::InitScratch (sub_820F6898): +0 = 0, +4 = an 8 KB block. Returns
// the original's r3 (the block).
uint32_t TrackMeshInitScratch(uint8_t* base, uint32_t scratch);
// TrackMesh::Prepare (sub_820F68E8): +8 = 1, +4 = 0, +0 = an 8-byte block
// set up by InitScratch. Returns the original's r3 (InitScratch's).
uint32_t TrackMeshPrepare(uint8_t* base, uint32_t mesh);
// TrackMesh::Free (sub_820F6940): the block +0's 8 KB block and the block
// freed, +0 = 0; the file +4 freed, +4 = 0. False, with nothing done, where
// a block to free isn't the allocation layer's (0 among them: the
// original's VirtualFree runs). `r3`: what the original leaves there.
bool TrackMeshFree(uint8_t* base, uint32_t mesh, uint32_t* r3);
// TrackMesh::OnLoaded (sub_820F69B0, Archive::Load's callback): the file's
// section table (+4) and its entries made pointers, +8 = 2.
void TrackMeshOnLoaded(uint8_t* base, uint32_t mesh);
// TrackMesh::SliceKind (sub_820F6A08): the halfword +24 of the track
// position's slice.
uint32_t TrackMeshSliceKind(uint8_t* base, uint32_t mesh, uint32_t position);
// --- The ground (src/track_collision/track_ground.cpp). A section (the
// mesh file's section table, [[mesh + 4] + 4]): its halfword +0 the slice
// count, six 6-byte links from +4 (+0 the slice they leave from, +2 the
// section and +4 the slice they lead to; -1 for none), +64, +68, +72, +76
// the offsets of its vertices (12 bytes: x, y, z), their colors (a
// halfword each), its slices (28 bytes: +0 the triangle count, +2 the
// first) and its triangles (12 bytes: halfwords +0 to +6 the vertices, +6
// 0xFFFF for a triangle, else a quad; byte +9 the surface kind; halfword
// +10 flags, bit 0 a quad's own height). A track position: halfwords +0
// and +2 the section and slice. ---

// TrackMesh::TriangleInSlice (sub_820F72F8): the first triangle of the
// slice the point is inside in x and z (skipping kinds 4 and 20 when
// `skip_kinds` is 1), its index; else -1.
uint32_t TrackMeshTriangleInSlice(uint8_t* base, uint32_t section, uint32_t slice, uint32_t point,
                                  uint32_t skip_kinds);
// TrackMesh::TriangleNormal (sub_820F7468): out = the plane's normal of the
// triangle's vertices a, b, c (which of its four), w 1.0, normalized by the
// vector unit (the 16 bytes at out rounded down to 16), then w 1.0 again.
void TrackMeshTriangleNormal(uint8_t* base, uint32_t out, uint32_t section, uint32_t triangle,
                             uint32_t a, uint32_t b, uint32_t c);
// TrackMesh::QuadHeight (sub_820F7590): out = the mean of the quad's two
// halves' normals, normalized; returns the mean of their heights at the
// point's x and z (f1). `frame`: the original's 208 bytes.
double TrackMeshQuadHeight(uint8_t* base, uint32_t out, uint32_t section, uint32_t quad,
                           uint32_t point, uint32_t frame);
// TrackMesh::FindTriangle (sub_820F7818): the slice of the triangle under
// the point, near the track position's cell (the section and triangle
// written to `section_out` and `triangle_out`); else -1 (the halfword's
// sign as the original leaves it: a slice from a position or a link
// zero-extended, one from an offset sign-extended).
uint32_t TrackMeshFindTriangle(uint8_t* base, uint32_t mesh, uint32_t section_out,
                               uint32_t triangle_out, uint32_t point, uint32_t position);
// TrackMesh::ColorAt (sub_820F6E18): the three vertices' colors (RGB555
// halfwords) interpolated at the point into out's x, y, z (a quad's half
// the point is on).
void TrackMeshColorAt(uint8_t* base, uint32_t out, uint32_t point, uint32_t triangle,
                      uint32_t vertices, uint32_t colors);
// TrackMesh::GroundHeight (sub_820F7AA0): the point's y set to the ground's
// height under it when within the catch distance; its surface kind, -1
// (no triangle) or -2 (too far). `color` and `normal` 0 or outputs.
// `frame`: the original's 240 bytes.
uint32_t TrackMeshGroundHeight(uint8_t* base, uint32_t mesh, uint32_t point, uint32_t position,
                               uint32_t color, uint32_t normal, uint32_t frame);
// TrackCollision::Ground (sub_820FE8F8): the ground result (+0 the depth,
// +16 and +32 the normal, +48 the surface bits, +52 the track position)
// for the point. Returns 1. `frame`: the original's 144 bytes.
uint32_t TrackCollisionGround(uint8_t* base, uint32_t result, uint32_t point, uint32_t position,
                              uint32_t frame);
// TrackCollision::GroundHits (sub_820FEBD0, the world's second query):
// for a box, of its four lower corners (in the order 0, 2, 1, 3) the one
// lowest against its track position's last height (the box's cache word
// of its place); when below it, the ground under it (Ground), and when
// below that, one hit record: +0 the corner, +16 and +32 the ground's
// normal, +48 [0x8200222C] minus the ground's depth, +56 the surface kind,
// +60 = 0; `count` 1 when the world has room for hits (its +60 above 0),
// else 0. +48 = 0.0 first. False, with nothing done, for a shape that
// isn't a box (the point lists and other shapes never ran in play).
// `frame`: the original's 384 bytes.
bool TrackCollisionGroundHits(uint8_t* base, uint32_t hits, uint32_t box, uint32_t frame,
                              uint32_t* count);
// TrackCollision::GroundCallback (sub_820FEAB0): Ground, then +48 &=
// 0x80FFFFFF. Returns 1. `frame`: the original's 96 bytes.
uint32_t TrackCollisionGroundCallback(uint8_t* base, uint32_t result, uint32_t point,
                                      uint32_t position, uint32_t frame);

// --- The track position's cell (src/track_collision/track_cell.cpp): a
// slice's outline (halfwords +12 to +18 its corners in the section's
// outline, 8-byte x, z points at the section's halfword +2; +18 0xFFFF for
// three), its height +20; a section's bounds (+40 to +60). ---

// TrackMesh::SliceContains (sub_820F6A38): 1 when the point's y is within
// [0x82001BDC] of the slice's height, the point near the outline's center
// and inside it in x and z; else 0.
uint32_t TrackMeshSliceContains(uint8_t* base, uint32_t slice, uint32_t outline, uint32_t point);
// TrackMesh::FindSlice (sub_820F6BE8): the first slice of the section
// containing the point, among all (`slice` -1) or those within 4 of
// `slice`; else -1.
uint32_t TrackMeshFindSlice(uint8_t* base, uint32_t section, uint32_t slice, uint32_t point);
// TrackPosition::CrossSection (sub_820F71B0): the position's slice looked
// for near it in its section, then in the sections its links lead to, then
// from the section's last or first slice; the position's section and
// slice set (the slice 0 when out of range). Returns 1 when one was found.
uint32_t TrackPositionCrossSection(uint8_t* base, uint32_t position, uint32_t mesh,
                                   uint32_t point);
// TrackPosition::FindCell (sub_820F8090): the position's cell moved to the
// slice containing the point (FindCell's row in collision-world.md);
// returns 0, or -1, -2, -3 when the full searches didn't find it either.
// The full searches (sub_820F7EE8, sub_820F7E30, with sub_820F7D70 and
// sub_820F6C88; never seen in play) are ported with it. `frame`: the
// original's 240 bytes.
uint32_t TrackPositionFindCell(uint8_t* base, uint32_t mesh, uint32_t position, uint32_t point,
                               uint32_t frame);
// TrackPosition::Update (sub_820F8470): the track position's cell (FindCell
// on the game's mesh), its point 0.5 higher and the ground under it
// (GroundHeight, the color into +32, the normal into +48; the y put back
// when none), +82 = 1 for surface kinds 4 and 20, plus 2, 4 or 8 for the
// slice's kind 30, 31 or 32, then its route position (+4) followed
// (track_route::Follow, with the route position as it was before, kept in
// the frame's +80). `frame`: the original's 144 bytes. The original's r3
// (Follow's) isn't kept: no caller reads it.
void TrackPositionUpdate(uint8_t* base, uint32_t position, uint32_t frame);
// TrackPosition::Init (sub_820F8578): the point (+16) from the route
// position (+4) (track_route::Point), the cell (+0, +2) from the route
// slice's record's word +32, +80 and +82 = 0, then Update. `frame`: the
// original's 128 bytes (its stack copy of the cell's word not written).
void TrackPositionInit(uint8_t* base, uint32_t position, uint32_t frame);
// sub_820F7D70 (the name ours: TrackMesh::SectionOutcode): bits 1, 2 for
// the point's x below or above the section's bounds (+48, +56, with the
// margin [0x82001FD8]), 4, 8 for y (+44 - +40, +40 + +44), 16, 32 for z
// (+52, +60); 0 inside.
uint32_t TrackMeshSectionOutcode(uint8_t* base, uint32_t mesh, uint32_t point, uint32_t section);
// sub_820F6C88 (TrackMesh::NearestSlice): the section's slice containing
// the point (*best = 0.0), else the one whose center is nearest within
// [0x8200681C] and nearer than *best (*best its squared distance); -1.
uint32_t TrackMeshNearestSlice(uint8_t* base, uint32_t section, uint32_t point, uint32_t best);
// sub_820F7EE8 (TrackPosition::SearchLinked): the sections the position's
// section links to, then every other section, whose bounds hold the point:
// the first slice containing it. The halfwords at 0x826C5B04 list the
// linked sections tried (-1 after the last). Returns 1 when found.
uint32_t TrackPositionSearchLinked(uint8_t* base, uint32_t position, uint32_t point, uint32_t mesh);
// sub_820F7E30 (TrackPosition::SearchAll): every section whose bounds hold
// the point: its nearest slice (NearestSlice), until one contains it.
// Returns 1 when any was found. `frame`: the original's 160 bytes.
uint32_t TrackPositionSearchAll(uint8_t* base, uint32_t position, uint32_t point, uint32_t mesh,
                                uint32_t frame);

// --- The wall query's parts (src/track_collision/track_walls.cpp). A wall
// query (TrackCollision::WallHits' frame + 80): +0 the hit count, +4 2.0,
// +8 the world's hit records, +12 the shape record, +48 to +104 the hit
// being made, +108 the cell count and +112 the cells (two halfwords each:
// section, slice). ---

// WallQuery::AddCell (sub_820FC110): 0 when the cell is in the list, else
// added (no limit) and 1. The cell is stored at `frame` + 28 (the caller's
// frame, as the original does).
uint32_t WallQueryAddCell(uint8_t* base, uint32_t query, uint32_t cell, uint32_t frame);
// WallQuery::GatherCells (sub_820FC188): the cells of the current box's
// four lower corners' track positions, and with each one added, the
// slices before and after it and the cells the section's links lead to
// from those. False, with nothing done, where the shape record's template
// or its current copy isn't a box. `frame`: the original's 192 bytes.
bool WallQueryGatherCells(uint8_t* base, uint32_t query, uint32_t mesh, uint32_t frame,
                          uint32_t* count);
// WallQuery::AddHit (sub_820FC2E0): the next of the world's hit records
// (the last one again when the world's +60 are used) set from the query's
// hit, +56 = the surface bits. Returns the original's r3 (the world's +60).
uint32_t WallQueryAddHit(uint8_t* base, uint32_t query, uint32_t surface);
// Segments::Intersect2D (sub_820FC658): 0 when the segments a-b and c-d
// (`points`: four pointers) don't cross in x and z, else 1 with out's x and
// z the crossing.
uint32_t SegmentsIntersect2D(uint8_t* base, uint32_t points, uint32_t out);

// --- The wall query (src/track_collision/track_wall_query.cpp). The query's
// wall, set by TestCell: +16 and +32 its two ends, +624 the walls tested
// (+628 the list, 6 bytes each: section, the two vertices), +1408 its
// direction, +1424 the direction negated, +1440 its horizontal normal,
// +1456 and +1460 the ends along the direction, +1464 the normal's
// distance. The shape record's template, previous and current copies must
// be boxes: the functions taking the query return false for any other,
// with nothing done. ---

// WallQuery::ModeSkips (sub_820FC870, the name ours): 1 when the word at
// `word` is 2, or by the race mode (the race data's +0xA3AF4) unless the
// stage list's flags (+0x59620 + 1220) have 0x20 or 0x40; else 0.
uint32_t WallQueryModeSkips(uint8_t* base, uint32_t word);
// WallQuery::IgnoresWall (sub_820FD6D8): nonzero when the wall's kind (the
// halfword at `kind`) is skipped for the box's halfwords at `a` (+112) and
// `b` (+114) and the race's settings. `frame`: the original's 96 bytes.
uint32_t WallQueryIgnoresWall(uint8_t* base, uint32_t kind, uint32_t a, uint32_t b,
                              uint32_t frame);
// WallQuery::CornersThroughWall (sub_820FC3C8): each corner of the box
// moving through the wall between its previous and current place, within
// the wall's ends, recorded (AddHit). `hit` 1 when one was. `frame`: the
// original's 304 bytes.
bool WallQueryCornersThroughWall(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* hit);
// WallQuery::EdgesAcrossWall (sub_820FC900): the box's edges (all but 1,
// 3, 9 and 10) against the wall: an edge across it in x and z with an end
// gone through it, or the wall's end pushed out of the face for the edge;
// an edge beside it with both of the wall's ends inside the box, pushed
// out along the edge's move. The deepest of each edge recorded (AddHit).
// `hit` 1 when one was. `frame`: the original's 944 bytes.
bool WallQueryEdgesAcrossWall(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* hit);
// WallQuery::TestCell (sub_820FDC18): each of the cell's walls not skipped
// (IgnoresWall) or already tested (the list +628, at most 128 kept) set as
// the query's wall (its ends from the section's vertices, its direction,
// normal and spans) and tested: CornersThroughWall, else EdgesAcrossWall.
// The cell (two halfwords: section, slice) is kept at `frame` + 292 (the
// caller's frame) as the original does. `frame`: the original's 256 bytes.
bool WallQueryTestCell(uint8_t* base, uint32_t query, uint32_t mesh, uint32_t cell,
                       uint32_t frame);
// TrackCollision::QueryWalls (sub_820FDF38): +624 = 0, the cells near the
// box (GatherCells), then TestCell on each. `frame`: the original's 128
// bytes.
bool TrackCollisionQueryWalls(uint8_t* base, uint32_t query, uint32_t mesh, uint32_t frame);
// TrackCollision::WallHits (sub_820FF1B0, the collision world's first
// query): the shape record's bounds (UpdateBounds), a wall query on the
// frame (+80: +0 the count 0, +4 2.0, +8 `hits`, +12 the record; the
// first record's +64 = 2.0), QueryWalls on the mesh; `count` the hits.
// `frame`: the original's 1,584 bytes.
bool TrackCollisionWallHits(uint8_t* base, uint32_t hits, uint32_t record, uint32_t frame,
                            uint32_t* count);

// The vector unit's normalize as the translated code does it, flush-to-zero
// on: the 16 bytes at `from` rounded down to 16 (x, y, z, w), the squared
// length of x, y, z (vmsum3fp128: dpps 0xEF), its reciprocal square root as
// 1/sqrt and one Newton step (vrsqrtefp, vnmsubfp, vmaddfp), all four lanes
// times it; the squared length itself (0) where it's 0. Stored to the 16
// bytes at `to` rounded down (src/track_collision/track_ground.cpp).
void NormalizeVector(uint8_t* base, uint32_t from, uint32_t to);

// TrackRoute::OnLoaded (sub_820F9050, Archive::Load's callback): the
// route file's offsets made pointers, the race's route bits applied to its
// entries' +148 and +152, +4 the sum of the +12 entries' first halfwords,
// +8 = 2.
void TrackRouteOnLoaded(uint8_t* base, uint32_t route);

// TrackRoute::Prepare (sub_820F8600): +8 = 1, +0 = 0.
void TrackRoutePrepare(uint8_t* base, uint32_t route);
// TrackRoute::Free (sub_820F8618): +8 = 0, the file +0 freed, +0 = 0.
// False, with nothing done, where the file isn't the allocation layer's (0
// among them). `r3`: what the original leaves there.
bool TrackRouteFree(uint8_t* base, uint32_t route, uint32_t* r3);
// Whether TrackMesh::Free and TrackRoute::Free run natively (their blocks
// the layer's pages, or, for the mesh, 0).
bool TrackMeshFreeable(uint8_t* base, uint32_t mesh);
bool TrackRouteFreeable(uint8_t* base, uint32_t route);
// TrackCollision::Unload (sub_820FEFB0): CollisionWorld::Destroy (the car
// physics' plain C++), the mesh and the route freed, the object at
// 0x826C5B28 deleted (the pointer kept). False, with nothing done, where a
// block to free isn't the allocation layer's (or there's no world). Only
// while --native_heap routes the game's heap and VirtualAlloc to the layer.
bool TrackCollisionUnload(uint8_t* base);

}  // namespace track_collision
