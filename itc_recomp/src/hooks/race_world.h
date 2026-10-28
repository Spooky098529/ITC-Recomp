// The race world's register steps (src/race_world/,
// docs/functions/race-world.md), shared by the adapters
// (src/hooks/race_world.cpp, behind --native_race_world) and the harness
// (without the flag). Each turns flush-to-zero off first and leaves r3 (and
// the registers the original leaves that a caller could read) as the
// original does.

#pragma once

#include "guest.h"

namespace hooks {

// RaceManager::EntryOfRecord sub_821705A8: r3 = the record's number; r3 =
// the entry or 0.
void RaceWorldEntryOfRecord(PPCContext& ctx, uint8_t* base);
// RaceManager::BranchesMeet sub_82170860: r3, r4 = two branches; r3 = 0 or 1.
void RaceWorldBranchesMeet(PPCContext& ctx, uint8_t* base);
// Traffic::PickModel sub_82171070: r3 = a number, r4 = a branch; r3 = the
// slot.
void RaceWorldPickModel(PPCContext& ctx, uint8_t* base);
// SortedList::Insert sub_82170AA0: r3 = the list, r4 = the item, f1 = the
// key (r3 left). SortedList::Clear sub_82170A88: r3 = the list (r3 left).
void RaceWorldSortedListInsert(PPCContext& ctx, uint8_t* base);
void RaceWorldSortedListClear(PPCContext& ctx, uint8_t* base);
// RaceWorld::OnPausedTick sub_82170560: r3 = the race world; r3, r4 = 400,
// r5 = 0, r6 = 0x820099AC, r11 = 0x82010000 (the original's, for the empty
// call it ends with).
void RaceWorldOnPausedTick(PPCContext& ctx, uint8_t* base);
// RaceWorld::Release sub_82171360: r3 = the race world; r3 = the last
// vehicle (or left).
void RaceWorldRelease(PPCContext& ctx, uint8_t* base);
// RoutePosition::ToMainRoad sub_82143608: r3 = the position, r4 = the flag;
// r3 = TrackRoute::ToMainRoad's.
void RaceWorldToMainRoad(PPCContext& ctx, uint8_t* base);
// RoutePosition::StepToMainRoad sub_82170F18: r3 = the position, r4 = the
// step, r5 = the kind; r3 = Normalize's.
void RaceWorldStepToMainRoad(PPCContext& ctx, uint8_t* base);
// RaceEvent::Entry sub_82143508: r3 = &an index; r3 = the entry.
void RaceWorldRaceEventEntry(PPCContext& ctx, uint8_t* base);
// RaceEvent::PlaceEntry sub_82143150: r3 = the event, r4 = the route, r5 =
// &the index, r6 = &the position, r7 = &the step, r8 = masks; r3 =
// Normalize's.
void RaceWorldPlaceEntry(PPCContext& ctx, uint8_t* base);
// RaceManager::Create sub_821715B8: no arguments; r3 = the manager. The
// original unless --native_heap routes the game's heap to the allocation
// layer, or where operator new's plain C++ refuses.
void RaceWorldCreate(PPCContext& ctx, uint8_t* base);
// RaceManager::NoteMarkedRacer sub_82172F90: r3 not read (r3 not kept: its
// caller doesn't read it).
void RaceWorldNoteMarkedRacer(PPCContext& ctx, uint8_t* base);
// RaceManager::NearSegment sub_82170BC0: r3 = the segment, r4 = the point,
// f1 = the distance; r3 = 0 or 1.
void RaceWorldNearSegment(PPCContext& ctx, uint8_t* base);
// RaceManager::BuildNeighbors sub_82170CB8: r3 = the table (the manager +
// 2392); r3 not kept (its caller doesn't read it).
void RaceWorldBuildNeighbors(PPCContext& ctx, uint8_t* base);
// RaceManager::Neighbours sub_82170920: r3 = the manager, r4 = &an index; r3
// = 0 to 3.
void RaceWorldNeighbours(PPCContext& ctx, uint8_t* base);
// RaceManager::JumpSections sub_821737F8: r3 = the entry, r4 = the player's
// position, r5 = &the player's branch, r6, r7 = &the flags; r3 not kept.
void RaceWorldJumpSections(PPCContext& ctx, uint8_t* base);
// RaceManager::TickVehicles sub_821714D8 and RefreshVehicles sub_82171560:
// no arguments (r3 not kept: RaceWorld::Tick doesn't read it).
void RaceWorldTickVehicles(PPCContext& ctx, uint8_t* base);
void RaceWorldRefreshVehicles(PPCContext& ctx, uint8_t* base);
// SaveData::FirstUnsetFlag sub_82112190: r3 = the save data; r3 = 0 to 6.
void RaceWorldFirstUnsetFlag(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
