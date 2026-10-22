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

// TrackRoute::Prepare (sub_820F8600): +8 = 1, +0 = 0.
void TrackRoutePrepare(uint8_t* base, uint32_t route);
// TrackRoute::Free (sub_820F8618): +8 = 0, the file +0 freed, +0 = 0.
// False, with nothing done, where the file isn't the allocation layer's (0
// among them). `r3`: what the original leaves there.
bool TrackRouteFree(uint8_t* base, uint32_t route, uint32_t* r3);

}  // namespace track_collision
