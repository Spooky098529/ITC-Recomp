// The track zones as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/track-zones.md, the layouts in
// docs/known-data.md: 0x828161EC, "track zone", "route range", 0x82603274,
// 0x82011AF0). 70 stretches of the course's route the player's car is
// checked against each tick for the HUD: the zones' owner, the zones, their
// route ranges and the fixed-size lists they're kept in. The owner, the
// zones and the ranges are guest data shared with the recompiled code, so
// these work on guest memory, with the originals' loads and stores in their
// order. The originals' stack temporaries aren't written (positions are
// passed by value), except the range a zone's set-up builds on its stack
// and copies into the zone: that one is at the original's frame offset, so
// the bytes Init leaves unwritten (the lists' data slots) are copied as the
// original copies them. Adapters: src/hooks/track_zones.cpp.

#pragma once

#include <cstdint>

namespace track_zones {

// A position on the course's route: a section and a slice (halfwords).
struct Position {
  uint16_t section;
  uint16_t slice;
};

// The route ([0x826C5B10]; its +12 the sections table, track-route.md) and
// the slices counted along a range's path (RouteRange::OnPath).
constexpr uint32_t kRoute = 0x826C5B10;
constexpr uint32_t kSliceCount = 0x82603274;

// A zone's fields: its 16-slot range list (+4; the ranges at +80, 1,064
// bytes apart), its index, its vehicle and whether the car is inside.
constexpr uint32_t kZoneRanges = 4;
constexpr uint32_t kZoneIndex = 17104;
constexpr uint32_t kZoneVehicle = 17108;
constexpr uint32_t kZoneInside = 17112;
constexpr uint32_t kRangeBytes = 1064;

// --- The fixed-size lists (List16::, List64::, List128::) ---
// One layout for every size N: N nodes of two halfwords (the next node, -1
// for none, and a data slot), then the free list's head (+4N), the list's
// head (+4N + 4), the count (+4N + 8), then the data (+4N + 12).

// List16::Link (sub_821531C0), List64::Link (sub_820FE2C8) and
// List128::Link (sub_8228B5A0): the free head taken and linked in at
// `position` (after the list's head for 0, else after the node at position
// - 1), the count up by 1. No check for a full list.
void List16Link(uint8_t* base, uint32_t list, int16_t position);
void List64Link(uint8_t* base, uint32_t list, int16_t position);
void List128Link(uint8_t* base, uint32_t list, int16_t position);
// List64::Insert (sub_820FE330) and List128::Insert (sub_8228BC18): Link
// with the position's low halfword, then the node `position` steps from the
// head (32 bits, as the originals walk it) has its data slot's word set to
// `word`.
void List64Insert(uint8_t* base, uint32_t list, uint32_t word, uint32_t position);
void List128Insert(uint8_t* base, uint32_t list, uint32_t word, uint32_t position);

// --- The route ranges (RouteRange::) ---

// RouteRange::Init (sub_820FE058): the start and end, both lists reset, the
// state +0 = 1 and +1060 = 1.
void RangeInit(uint8_t* base, uint32_t range, Position start, Position end);
// RouteRange::AddBranch (sub_820FE8C8): `section` (the original's r4, all
// 32 bits) added at the end of the list of sections to take (+12).
void RangeAddBranch(uint8_t* base, uint32_t range, uint32_t section);

// RouteRange::ToNeighbor (sub_820FDF98): `position` mapped onto `section`
// (the section beside its own): out = (section, the slice), the slice moved
// by the two sections' slice counts' difference when one of the position's
// section's forward links 0 and 1 has the same section as one of
// `section`'s; 0 when that slice is below 0 (out still written), else 1.
uint32_t RangeToNeighbor(uint8_t* base, Position* out, uint16_t section, Position position);
// RouteRange::WithinEnds (sub_820FE120): 1 unless the position is on the
// start's section before its slice or on the end's after its slice (and
// the same against the ends mapped onto the sections beside them, links 6
// to 9, when +1060 is set), else 0.
uint32_t RangeWithinEnds(uint8_t* base, uint32_t range, Position position);
// RouteRange::OnPath (sub_820FE398): whether the position is on the path
// from the start's section to the end's: 0 (state 0), 1 (not on it) or 2
// (on it); 0x82603274 = the slices counted along the path.
uint32_t RangeOnPath(uint8_t* base, uint32_t range, Position position);
// RouteRange::Test (sub_820FE6F8): 0 (state 0), 1 (left out, or not on the
// path) or 2 (on it, or on it mapped onto a section beside, links 6 to 9).
uint32_t RangeTest(uint8_t* base, uint32_t range, Position position);
// RouteRange::Update (sub_820FE878): with a section below 0x8000, the state
// +0 = Test; 1 when the state is 2, else 0.
uint32_t RangeUpdate(uint8_t* base, uint32_t range, Position position);

// --- The zones (Zone::, ZoneRanges::) ---

// Zone::Update (sub_8228BC80): the vehicle's track position (+4 of
// Vehicle::Identity), the flag cleared, then each range in list order
// updated until one returns 1 (the flag set). Returns the last range's
// result, or -1 when the list is empty.
int32_t ZoneUpdate(uint8_t* base, uint32_t zone);

// Zone::Reset (sub_8228B540): the range list reset, then its index, vehicle
// and flag (0).
void ZoneReset(uint8_t* base, uint32_t zone, uint32_t index, uint32_t vehicle);
// Zone::IsInside (sub_8228B4F8): the zone's +17112.
uint32_t ZoneIsInside(uint8_t* base, uint32_t zone);
// ZoneRanges::Insert (sub_8228BBA8): List16::Link at `position`, then the
// range's 1,064 bytes at `range` copied to the node's data slot `*copy_to`
// (memcpy, sub_825A7410). False when the copy couldn't be made natively
// (memcpy's override records destinations at or above 0xA0000000, and
// ranges that may share memory are the original's): the link is done and
// the adapter makes the copy through memcpy.
bool ZoneRangesInsert(uint8_t* base, uint32_t list, uint32_t range, uint32_t position,
                      uint32_t* copy_to);

struct SetupCalls;

// --- The zones' owner (TrackZones::, [0x828161EC]) ---

// The owner (0x828161EC), its size, its list of the zones (128 slots at +0)
// and the zones (+1036, 17,116 bytes apart; zone i's vtable 0x82011AF0 +
// 12 x i).
constexpr uint32_t kOwner = 0x828161EC;
constexpr uint32_t kOwnerBytes = 0x124C34;
constexpr uint32_t kZonesAt = 1036;
constexpr uint32_t kZoneBytes = 17116;
constexpr uint32_t kZoneVtables = 0x82011AF0;
// The race data ([0x826C5B2C]: +0x59614 a word, +0x59620 the stage list,
// +0xA3AF0 the mode), 0x826C7824 (a word) and the race ([0x826D82F4]: +2308
// the racers' records).
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRace = 0x826D82F4;
constexpr uint32_t kPlayerFlag = 0x826C7824;

// The calls into code that takes the guest context, made by the adapters:
// the guest function `function` (a zone's vtable slot) with r3 (and r4
// when `set_r4`; the other registers as they are), from `frame` (r1) with
// the link register `lr`; its r3. A set-up's r4 points at (the index, the
// vehicle), which the owner writes at the original's frame offset first.
struct OwnerCalls {
  void* user;
  uint64_t (*call)(void* user, uint32_t function, uint32_t r3, uint32_t r4, bool set_r4,
                   uint32_t frame, uint32_t lr);
};

// Racers::Vehicle (sub_820C9718): racer `index`'s vehicle (Vehicle::Get of
// its +20), or 0 when the racer is 0 or its word +0 lacks bit 0x40.
uint32_t RacersVehicle(uint8_t* base, uint32_t index);
// TrackZones::TrackZones (sub_8228B608): zone i's vtable for the 70 zones.
void OwnerConstruct(uint8_t* base, uint32_t owner);
// TrackZones::Create (sub_8228E788): the owner made (operator new and the
// constructor) and stored at 0x828161EC, its list reset, the 70 zones
// added, then each one's set-up (slot 0, natively when it's the zone's own
// set-up, else through `calls`) with its list position and the vehicle;
// `stack` is the caller's r1. False, with nothing done, when operator new
// can't be native (the native heap off, or a failed allocation): the
// original runs. `r3` = the last set-up's r3.
bool OwnerCreate(uint8_t* base, uint32_t stack, const OwnerCalls& calls,
                 const SetupCalls& setup_calls, uint64_t* r3);
// TrackZones::Destroy (sub_8228B500): the owner deleted when not 0, and
// cleared. False, with nothing done, when operator delete can't be native
// (the native heap off, or a block the layer didn't give). `r3` = the
// owner, as operator delete leaves it.
bool OwnerDestroy(uint8_t* base, uint32_t* r3);
// TrackZones::Update (sub_8228BD30): each zone's slot 1 (Zone::Update
// natively when it's that, else through `calls`). Returns the original's
// r3: the owner with no zones, else the last zone's.
uint64_t OwnerUpdate(uint8_t* base, uint32_t owner, uint32_t stack, const OwnerCalls& calls);
// TrackZones::IsInside (sub_8228BDD0): the list's zone `i`'s slot 2
// (Zone::IsInside natively when it's that, else through `calls`, a tail
// call from `stack` with `lr`). No bounds check.
uint64_t OwnerIsInside(uint8_t* base, uint32_t owner, uint32_t i, uint32_t stack, uint32_t lr,
                       const OwnerCalls& calls);

// --- The zones' set-ups (ZoneN::Setup, slot 0 of zone N's vtable) ---

constexpr uint32_t kZones = 70;

// The copy a set-up's ZoneRanges::Insert can't make natively: memcpy
// (sub_825A7410) of the range at `from` (1,064 bytes) to `to`, made by the
// adapter (src/hooks/track_zones.cpp) from `frame`, the set-up's frame.
struct SetupCalls {
  void* user;
  void (*copy)(void* user, uint32_t to, uint32_t from, uint32_t frame);
};

// The set-up of zone `number` (0 to 69): its address (sub_8228BE20 to
// sub_8228E6A0) and its frame's size.
uint32_t SetupAddress(uint32_t number);
uint32_t SetupFrameBytes(uint32_t number);
// ZoneN::Setup: Zone::Reset with the index and vehicle, then each of its
// hardcoded ranges made in the range on the set-up's stack (`stack`, the
// caller's r1, less the frame's size, + 96: the original's), with
// RouteRange::Init and AddBranch, and copied into the zone at the end of
// its list (ZoneRanges::Insert at the count). The other stack words the
// original writes (the ends at +80, its saved registers) aren't. Returns
// the original's r3: the last copy's destination.
uint32_t ZoneSetup(uint8_t* base, uint32_t number, uint32_t zone, uint32_t index,
                   uint32_t vehicle, uint32_t stack, const SetupCalls& calls);

}  // namespace track_zones
