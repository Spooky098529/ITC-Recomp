// The race world and the racers' manager as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/race-world.md): the race mode's state machine at +116812
// (RaceWorld::), the racers' manager at [0x826D82F4] (RaceManager::: 80
// entries of 28 bytes from +4, the racers' list at +2308, the vehicle slot
// words at +2336), its sorted lists, the race event's start placements and
// the save data's distance. The manager, the vehicles and the race data are
// guest objects shared with the recompiled code, so these work on guest
// memory, with the originals' loads and stores in their order and their
// arithmetic step for step (src/car_physics/float_ops.h). The originals'
// temporaries are on their stack: `frame` is the original's stack frame
// (its r1 after the stwu) and `sp` the stack pointer it came with; the
// temporaries go at the same offsets, register saves and back chains
// aren't written. Flush-to-zero off on entry (the adapters' job). Callees
// made native earlier are called as plain C++. Adapters:
// src/hooks/race_world.cpp, behind --native_race_world.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "vehicle/vehicle.h"

namespace race_world {

constexpr uint32_t kManager = 0x826D82F4;    // the racers' manager (a pointer)
constexpr uint32_t kRaceData = 0x826C5B2C;   // the race data (a pointer)
constexpr uint32_t kRaceEvent = 0x826C6078;  // the race event (a pointer)
constexpr uint32_t kRoute = 0x826C5B10;      // the course's route

// --- The manager's leaves ---

// RaceManager::EntryOfRecord (sub_821705A8): the manager's first entry (+4
// + 28 i, +2368 of them) whose +24 is `record`, or 0 (also with no
// manager).
uint32_t EntryOfRecord(uint8_t* base, uint32_t record);
// RaceManager::BranchesMeet (sub_82170860, with its tail sub_821708B4): 1
// when route branch `b` (the low byte) is 0 or 29 or the same as `a`'s;
// else by `a` (the low byte; 0 above 29) as its jump table says.
uint32_t BranchesMeet(uint32_t a, uint32_t b);
// Traffic::PickModel (sub_82171070): the two 42-entry tables of car ids
// written below `sp` (-400 and -232, as the original, a leaf without a
// frame), the second for branches 7, 8, 13 and 14; the entry `number` mod
// 42 (C's remainder, read through guest memory as the original) minus 55.
// Returns the original's r3 (64 bits).
uint64_t PickModel(uint8_t* base, uint32_t number, uint32_t branch, uint32_t sp);

// --- The sorted lists (100 nodes of 16 bytes: +0 the item, +4 the key, +8
// the previous, +12 the next; +1600 the first, +1604 the last inserted,
// +1608 the count) ---

// SortedList::Insert (sub_82170AA0): the next node filled (the key as a
// float) and linked in ascending order, searching from the last inserted
// (its first comparison with the key's double, as the original).
void SortedListInsert(uint8_t* base, uint32_t list, uint32_t item, double key);
// SortedList::Clear (sub_82170A88): +1600, +1604, +1608 = 0.
void SortedListClear(uint8_t* base, uint32_t list);

// --- The race world's handlers ---

// RaceWorld::OnPausedTick (sub_82170560): the empty sub_82106540 with (400,
// 400, 0, 0x820099AC), probably a debug print compiled out: nothing to do
// (the adapter leaves the registers as the original).
void OnPausedTick();

// --- The racers' vehicles ---

// RaceWorld::Release (sub_82171360): each racer's vehicle's byte +2384 = 0,
// the count (+2364) read again each pass. Returns Vehicle::Get's last r3,
// or `r3` with no racers (the original's r3).
uint32_t Release(uint8_t* base, uint32_t r3);

// --- The manager's lifetime and its tables ---

// RaceManager::Create (sub_821715B8): 363,592 bytes from operator new into
// [0x826D82F4]; when made, +0 = 0, the 80 entries (from +4, 28 bytes: +0 =
// 0, +4 = -1 (0 for the last three, the traffic), halfwords +8, +10 = -1,
// +12 = 0.0 (the float at 0x82000A34, as lfs and stfs), +16 = 0, +20, +24 =
// -1), the 16 words from +2244 = -1, +2364 to +2388 = 0, the 7 words from
// +2308 and from +2336 = 0 (in the original's order); then the 300 words
// from +362392 = 0 (at 362392 itself with no block, as the original).
// Returns the original's r3 (the manager); nullopt, with nothing done,
// where operator new would take its error path.
std::optional<uint32_t> Create(uint8_t* base);
// RaceManager::NoteMarkedRacer (sub_82172F90): `frame` + 92 = 0.0; each
// racer's body position copied (Vec4::Copy) to `frame` + 80 until one whose
// record (the race data + 3,936 x its +24, + 48) has flags +3888 0x400:
// its three floats stored at 0x826C61A4. `frame`: the original's 144 bytes.
void NoteMarkedRacer(uint8_t* base, uint32_t frame);
// RaceManager::NearSegment (sub_82170BC0): 1 when the point's projection on
// the segment (two 16-byte points) lies past its start and short of its
// length x [0x820021E8], and the point is nearer than `distance` to it;
// else 0. The vectors at `frame` + 80 to + 159 as the original (its 208
// bytes).
uint32_t NearSegment(uint8_t* base, uint32_t segment, uint32_t point, double distance,
                     uint32_t frame);
// RaceManager::BuildNeighbors (sub_82170CB8): for each section s of the
// route, the 1,200-byte record at `table` + 1,200 s (+0 = s, then the
// sections near it) and its count at `table` + 360,000 + 4 s (see the
// row). The points at `frame` + 80 to + 207 as the original (its 336
// bytes).
void BuildNeighbors(uint8_t* base, uint32_t table, uint32_t frame);
// RaceManager::Neighbours (sub_82170920): among the manager's other entries
// (flag 0x1, not 0x20, 0x4 or 0x8) on a section of the same branch as the
// entry at `index`'s, whether one is ahead (TrackRoute::Distance above 0.0,
// under [0x82001FC0]) and behind (above [0x82009798]): 0 neither, 1 ahead,
// 2 behind, 3 both.
uint32_t Neighbours(uint8_t* base, uint32_t manager, uint32_t index);
// RaceManager::JumpSections (sub_821737F8): an entry whose item's kind is 3
// (with the flag at `flag3`) or 2 (with `flag2`), on the sections the
// original lists and not on the player's branch 12 (or 11), farther than
// [0x820099D0] (squared, flat) from `player` when on the player's branch
// (`branch`), is put at section 194 or 252 (201 or 259 for kind 2), slice
// 0, and the flag cleared. `frame`: the original's 176 bytes.
void JumpSections(uint8_t* base, uint32_t entry, uint32_t player, uint32_t branch,
                  uint32_t flag2, uint32_t flag3, uint32_t frame);

// --- The race world's set-up and its handlers ---

// RaceWorld::Init (sub_82174D28, src/race_world/race_world_init.cpp): +9604
// = 1, +9608 = 0 (1 when the race data's +0xA3AF0 is 6 and the current
// stage's word +76 is 2 or more), then the 24 transitions added as the
// original does (its stores and copies replayed in `frame`, its 1,696
// bytes). Returns the last StateMachine::Add's plain C++'s answer
// (`replaced` its flag).
uint32_t Init(uint8_t* base, uint32_t world, uint32_t frame, bool* replaced);
// RaceWorld::ReleaseForStage (sub_821713C0): as Release, then the current
// stage copied into `frame` + 80 (Stages::Copy; its original through
// `original` where it refuses); with the stage of type 2 or 3, or the race
// data's +0xA3AF0 6 and the copy's +0 not 1: each of the stage's racers'
// entries' (EntryOfRecord of Stages::Racer) vehicle's +2384 = 1. `frame`:
// the original's 192 bytes.
void ReleaseForStage(uint8_t* base, uint32_t frame,
                     const std::function<void(uint32_t address, uint32_t sp, uint32_t r3,
                                              uint32_t r4, uint32_t r5)>& original);

// --- The racers' vehicles each tick ---

// A callee's original where its plain C++ refuses, run with r1 = `sp` and
// r3 = `r3`.
using Original = std::function<void(uint32_t address, uint32_t sp, uint32_t r3)>;

// RaceManager::TickVehicles (sub_821714D8): each racer's Vehicle::Tick (its
// vehicle, its +20, its record (the race data + 48 + 3,936 x its +24), 1),
// the count read again each pass. `frame`: the original's 128 bytes;
// `call`: Vehicle::Tick's guest calls.
void TickVehicles(uint8_t* base, uint32_t frame, const vehicle::GuestCall& call);
// RaceManager::RefreshVehicles (sub_82171560): each racer's
// Vehicle::Refresh (its original through `original` where it refuses).
// `frame`: the original's 112 bytes.
void RefreshVehicles(uint8_t* base, uint32_t frame, const Original& original);

// --- The racers' vehicles reset and the world unloaded
// (src/race_world/race_world_reset.cpp; only while --native_heap routes the
// game's heap to the allocation layer: the adapters check) ---

// RaceManager::ResetWorld (sub_82170668): each of the seven vehicles whose
// slot word (+2336) is above 0 removed from the world (Vehicle::
// RemoveFromWorld; its original through `original` where it refuses) and
// the slot = 0; each index stored at `frame` + 80 and its AI driver reset
// (AiDriver::Of, AiDriver::Reset); TrackCollision::ResetWorld; the float at
// 0x826C2D80 = the one at 0x82000A34; +2388 = 0. `frame`: the original's
// 128 bytes.
void ResetWorld(uint8_t* base, uint32_t manager, uint32_t frame, const Original& original);
// RaceManager::ResetVehicles (sub_82171718): +2364, +2372, +2376 = 0; each
// entry (+2368 of them, the count read again each pass) with a vehicle
// (+20 not -1), then the three traffic entries (from +2160): Vehicle::SetUp
// with its record (the race data + 3,936 x its +24 + 48; ResetForCar
// unless its slot word is -1; their callees' originals through
// `setup_original`, with r1 = `frame` - 224), flags |= 0x200 with `flag`,
// the slot = 1, an input record cleared at `frame` + 96 (+128 for traffic)
// and copied to the racer's ([0x826C5B30 + 4 x the index's low byte]),
// AiDriver::Setup (&+20, the record + 3960, &1 at `frame` + 80), listed
// (+2308) and counted (+2364, and +2372 or +2376); +2388 = 0. `frame`: the
// original's 224 bytes.
void ResetVehicles(uint8_t* base, uint32_t manager, uint32_t flag, uint32_t frame,
                   const vehicle::CalleeOriginal& setup_original);
// RaceWorld::Unload (sub_82171250): with the race data's +0xA3AF0 1 and
// +0xA3AF4 2 or 3, SaveData::AddDistance (player 0's save, vehicle 0's
// +6244); Vehicles::Destroy, AiDrivers::Destroy, TrackCollision::Unload,
// operator delete of the manager (each one's original through `original`,
// with r1 = `frame`, where its plain C++ refuses) and [0x826D82F4] = 0.
// `frame`: the original's 112 bytes.
void Unload(uint8_t* base, uint32_t frame, const Original& original);
// --- The route positions ---

// RoutePosition::ToMainRoad (sub_82143608): TrackRoute::ToMainRoad on the
// game's route (a tail call: `sp` is the caller's stack pointer). Returns
// its r3.
uint32_t RoutePositionToMainRoad(uint8_t* base, uint32_t position, uint32_t flag, uint32_t sp);
// RoutePosition::StepToMainRoad (sub_82170F18): the kind (a sign-extended
// halfword) into `frame` + 80, AiPath::BranchMasksFor of it, the slice (+2)
// += step, ToMainRoad(position, 1), TrackRoute::Normalize on the game's
// route with the masks. Returns its r3. `frame`: the original's 112 bytes.
uint32_t StepToMainRoad(uint8_t* base, uint32_t position, uint32_t step, uint32_t kind,
                        uint32_t frame);

// --- The race event's start placements ---

// RaceEvent::Entry (sub_82143508): [[0x826C6078] + 156] + 12 x the word at
// `index`.
uint32_t RaceEventEntry(uint8_t* base, uint32_t index);
// RaceEvent::PlaceEntry (sub_82143150): TrackRoute::Normalize(route,
// position, masks); the entry (the event's list +156, 12 bytes, at the
// sign-extended halfword at `index`) +4, +6 = the position's halfwords;
// then the position's slice += the halfword at `step`. Returns Normalize's
// r3.
uint32_t PlaceEntry(uint8_t* base, uint32_t event, uint32_t route, uint32_t index,
                    uint32_t position, uint32_t step, uint32_t masks);

// --- The save data ---

// SaveData::AddDistance (sub_82116C38): the garage car picked (the save's
// +0x19920-th node of the list at +6120 + 204); the difference between
// `distance` and the car's float +2104 added to the save's floats +4908 and
// +6040, and to the car's +2104 (the car found again).
void AddDistance(uint8_t* base, uint32_t save, double distance);

// SaveData::FirstUnsetFlag (sub_82112190): the first of bits 0 to 5 of the
// save's word +6092 that's clear, or 6.
uint32_t FirstUnsetFlag(uint8_t* base, uint32_t save);

}  // namespace race_world
