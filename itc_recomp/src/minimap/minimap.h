// The race's minimap's parts as plain C++ (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/minimap.md, the fields and globals in
// docs/known-data.md): the race manager's lookups, the course set's points,
// the markers' 80-slot lists, their triangles and the transform they're
// recorded with. Minimap::Draw (sub_820F5740) stays recompiled (Wrapped by
// frame interpolation, src/frame_pacing/per_frame_logic.cpp, and blocked by
// atexit and the course set's lists). The data is guest data shared with
// the recompiled code, so these work on guest memory, with the originals'
// loads and stores in their order. The originals' stack temporaries aren't
// written, except where the interpolation reads them (the transform's
// product). Adapters: src/hooks/minimap.cpp.

#pragma once

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

}  // namespace minimap
