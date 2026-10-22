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
// TrackRoute::Prepare (sub_820F8600): +8 = 1, +0 = 0.
void TrackRoutePrepare(uint8_t* base, uint32_t route);
// TrackRoute::Free (sub_820F8618): +8 = 0, the file +0 freed, +0 = 0.
// False, with nothing done, where the file isn't the allocation layer's (0
// among them). `r3`: what the original leaves there.
bool TrackRouteFree(uint8_t* base, uint32_t route, uint32_t* r3);

}  // namespace track_collision
