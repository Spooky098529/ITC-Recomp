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

// SaveData::FirstUnsetFlag (sub_82112190): the first of bits 0 to 5 of the
// save's word +6092 that's clear, or 6.
uint32_t FirstUnsetFlag(uint8_t* base, uint32_t save);

}  // namespace race_world
