// The race's minimap's parts as plain C++ (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/minimap.md, the fields and globals in
// docs/known-data.md): the race manager's lookups, the course set's points,
// the markers' 80-slot lists, their triangles and the transform they're
// recorded with. Minimap::Draw (sub_820F5740) stays recompiled (Wrapped by
// frame interpolation, src/frame_pacing/per_frame_logic.cpp, and blocked by
// the course set's lists). The data is guest data shared with
// the recompiled code, so these work on guest memory, with the originals'
// loads and stores in their order. The originals' stack temporaries aren't
// written, except where the interpolation reads them (the transform's
// product). Adapters: src/hooks/minimap.cpp.

#pragma once

#include "c_runtime/exit.h"

#include <cstdint>
#include <optional>

namespace minimap {

// The race's manager (28-byte entries from +4, count +2368; the racers'
// pointers from +2308), the race data, the route.
constexpr uint32_t kRaceManager = 0x826D82F4;
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRoute = 0x826C5B10;

// --- The race manager's lookups ---

// Race::FindPlayerEntry (sub_82170608): the first of `manager`'s entries
// whose word +0 has 0x4 (`which` not 0) or 0x8 (`which` 0), or 0.
uint32_t RaceFindPlayerEntry(uint8_t* base, uint32_t manager, uint32_t which);
// Race::RacerVehicle (sub_82170578): 0 without a manager, else
// Vehicle::Get of racer `racer`'s +20 (no checks).
uint32_t RaceRacerVehicle(uint8_t* base, uint32_t racer);

// SetList::Point (sub_82147D18): out = the x, y and z at
// [list + 528 + 4 x entry] + 64 x point + 16 x part, w = 1.0.
void SetListPoint(uint8_t* base, uint32_t list, uint32_t out, uint32_t entry, uint32_t point,
                  uint32_t part);

// Minimap::RacerMarkerType (sub_820F2D28, with its switch's case 1,
// sub_820F2E68): the marker type of the race manager's entry `entry` for the
// player's branch number `branch` (1 to 4, or -1 for none). nullopt, with
// nothing written, where the original must run: an id whose item's +88 is
// 3 (sub_820F2CA8's chained record, never seen in play).
std::optional<int32_t> RacerMarkerType(uint8_t* base, uint32_t entry, int32_t branch);

// --- The markers' shapes ---

// Minimap::Scaling (sub_820F2EF8): out = an identity with the vector's x, y
// and z at +0, +20 and +40 (read after the identity is written). False,
// with nothing written, for an out in the physical views (the original's
// memcpy is recorded there for the renderer's mirrors).
bool Scaling(uint8_t* base, uint32_t out, uint32_t vector);
// Triangle::Init (sub_820F4040): an identity at +0 (its rows stored at
// their 16-byte boundaries, as stvx), the kind +64 = 0, +68 = 0, +72 = 2,
// and 1.0 at +92, +108 and +124.
void TriangleInit(uint8_t* base, uint32_t triangle);

// --- The 80-slot lists of 64-byte matrices (List80::) ---
// track_zones' layout for 80 slots: nodes from +0 (the next node, -1 for
// none, and a data slot, halfwords), the free head +320, the head +324, the
// count +328, the data from +336.

// List80::Link (sub_820F40C0): the free head taken and linked in at
// `position` (after the head for 0, else after the node at position - 1),
// the count up by 1. No check for a full list.
void List80Link(uint8_t* base, uint32_t list, int16_t position);
// List80::Insert (sub_820F5238): Link at the position's low halfword, then
// Matrix::Copy of `matrix` into the data slot of the node `position` steps
// from the head (noted for interpolation with the original's return
// address). Returns the data slot's address (the original's r3).
uint32_t List80Insert(uint8_t* base, uint32_t list, uint32_t matrix, uint32_t position);
// List80::Copy (sub_820F51A8): out's +12, +28, +44, +60 = 1.0, then the
// data of the node `position` steps from the head copied to out (memcpy, 64
// bytes). False where the copy can't be made natively (an out in the
// physical views, or ranges that share memory: c_runtime::Memcpy); the
// original then runs (its stores the same again).
bool List80Copy(uint8_t* base, uint32_t out, uint32_t list, uint32_t position);

// --- Recording ---

// The command list Primitive::RecordTransform records on.
constexpr uint32_t kTransformList = 0x826D83D4;
// Where Primitive::RecordTransform's product goes: the original's frame
// (r1 - 160 + 80, so r1 - 80), where the interpolation notes it.
constexpr uint32_t kTransformProduct = 80;

// Primitive::RecordTransform (sub_8217AEA8, probably): `product` =
// Matrix::Multiply(the primitive's +0, the pass's view-projection +144),
// noted for interpolation, then a slot of command 7 (+0 = 7, +4 = 0, +8 =
// 1, bytes +12 = 0 and +13 = 4) and four slots with the product's rows on
// the list at [0x826D83D4]. Returns the last slot (the original's r3).
// nullopt, with nothing done, where the list hasn't room for the five slots
// (the original prints a message there and writes through 0).
std::optional<uint32_t> RecordTransform(uint8_t* base, uint32_t pass, uint32_t primitive,
                                        uint32_t product);

// Triangle::Record (sub_8217DAA0): which recording the triangle's kind
// (+64) takes: 0 Triangle::RecordFlat, 1 sub_8217C4C0 (never seen in play;
// the original's, a tail call), above 1 none.
enum class TriangleRecording { kFlat, kOther, kNone };
TriangleRecording TriangleRecord(uint8_t* base, uint32_t triangle);

// The render queue (+0) and the command list (+4) the triangles go to.
constexpr uint32_t kTriangleLists = 0x826D83D0;
// Triangle::RecordFlat's frame (its r1 less this): Primitive::
// RecordTransform's product is made below it, as the original's call does.
constexpr uint32_t kRecordFlatFrame = 128;

// Triangle::RecordFlat (sub_8217C370): on the list at [0x826D83D4] a chunk
// (OpenChunk, its header slot), Primitive::RecordTransform(pass, triangle)
// with its product at `frame` - 80, [13, 4, 0, 0], the three vertices
// (AllocateVertices(3), with its header: x, y, z from +80, +96, +112 and the
// color as ARGB from the low bytes of +140, +128, +132, +136, 16 bytes on
// for the others; +16 to +23 not written), a slot with +0 = 15, CloseChunk,
// and the header's +0, +2, +4, +12 = 0 and +8 = ChunkSlots - 1. Returns the
// header, which the adapter hands to the render queue (its slot 4, with the
// key and 0.0). nullopt, with nothing done, where the original's command
// list calls would print a message (a chunk open on any list, or no room
// for the 14 slots).
std::optional<uint32_t> TriangleRecordFlat(uint8_t* base, uint32_t triangle, uint32_t pass,
                                           uint32_t frame);

// --- The markers' draws ---

// The guest calls a marker's draw makes, from its adapter: Triangle::Record
// (sub_8217DAA0) of the triangle on the draw's frame (with r1 that frame,
// as the original's), memcpy (sub_825A7410) where c_runtime::Memcpy can't,
// and atexit where the native one can't.
struct MarkerCalls {
  c_runtime::AtexitCall atexit;
  void (*record)(void* user, uint32_t triangle, uint32_t pass, uint32_t key) = nullptr;
  void (*memcpy)(void* user, uint32_t to, uint32_t from, uint32_t size) = nullptr;
  void* user = nullptr;
};

// The draws' frames (their r1 less these), the triangle at the frame's +80
// as the originals', and the sort key they record with.
constexpr uint32_t kCarMarkerFrame = 320;
constexpr uint32_t kPointMarkerFrame = 336;
constexpr uint32_t kMarkerTriangle = 80;
constexpr uint32_t kMarkerKey = 104;

// Minimap::DrawCarMarker (sub_820F4888): r3 = the camera pass, r4 = the
// color (four words), r5 = the marker's matrix. The first call (guard bit 0
// of 0x826C5670) fills the seven triangles' table at 0x826C5520 (336 bytes
// from the floats at 0x82000A30, 0x82000A34 and 0x820067BC-0x820067D8) and
// registers its clean-up 0x825DAEF8 with atexit. Then each triangle, on the
// frame's +80: Triangle::Init, its vertices from the table, its three
// colors (the color for triangle 0, else (0, 0, 0, 255)), the matrix copied
// in (memcpy, 64 bytes), and Triangle::Record with key 104.
void DrawCarMarker(uint8_t* base, uint32_t pass, uint32_t color, uint32_t matrix,
                   uint32_t frame, const MarkerCalls& calls);
// Minimap::DrawPointMarker (sub_820F4BA8): the same with 22 triangles (the
// table at 0x826C5680, 1,056 bytes, also from 0x82002C1C and
// 0x820067DC-0x82006800; guard 0x826C5AA0, clean-up 0x825DAF00), 0 to 5 in
// the color.
void DrawPointMarker(uint8_t* base, uint32_t pass, uint32_t color, uint32_t matrix,
                     uint32_t frame, const MarkerCalls& calls);

}  // namespace minimap
