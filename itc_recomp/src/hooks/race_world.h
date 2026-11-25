// The race world's register steps (src/race_world/,
// docs/functions/race-world.md), shared by the adapters
// (src/hooks/race_world.cpp, behind --native_race_world) and the harness
// (without the flag). Each turns flush-to-zero off first and leaves r3 (and
// the registers the original leaves that a caller could read) as the
// original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "race_world/race_world.h"

namespace hooks {

// RaceManager::EntryOfRecord sub_821705A8: r3 = the record's number; r3 =
// the entry or 0.
void RaceWorldEntryOfRecord(lifted::Regs& ctx);
// RaceManager::BranchesMeet sub_82170860: r3, r4 = two branches; r3 = 0 or 1.
void RaceWorldBranchesMeet(lifted::Regs& ctx);
// Traffic::PickModel sub_82171070: r3 = a number, r4 = a branch; r3 = the
// slot.
void RaceWorldPickModel(lifted::Regs& ctx);
// SortedList::Insert sub_82170AA0: r3 = the list, r4 = the item, f1 = the
// key (r3 left). SortedList::Clear sub_82170A88: r3 = the list (r3 left).
void RaceWorldSortedListInsert(lifted::Regs& ctx);
void RaceWorldSortedListClear(lifted::Regs& ctx);
// RaceWorld::OnPausedTick sub_82170560: r3 = the race world; r3, r4 = 400,
// r5 = 0, r6 = 0x820099AC, r11 = 0x82010000 (the original's, for the empty
// call it ends with).
void RaceWorldOnPausedTick(lifted::Regs& ctx);
// RaceWorld::Release sub_82171360: r3 = the race world; r3 = the last
// vehicle (or left).
void RaceWorldRelease(lifted::Regs& ctx);
// RoutePosition::ToMainRoad sub_82143608: r3 = the position, r4 = the flag;
// r3 = TrackRoute::ToMainRoad's.
void RaceWorldToMainRoad(lifted::Regs& ctx);
// RoutePosition::StepToMainRoad sub_82170F18: r3 = the position, r4 = the
// step, r5 = the kind; r3 = Normalize's.
void RaceWorldStepToMainRoad(lifted::Regs& ctx);
// RaceEvent::Entry sub_82143508: r3 = &an index; r3 = the entry.
void RaceWorldRaceEventEntry(lifted::Regs& ctx);
// RaceEvent::PlaceEntry sub_82143150: r3 = the event, r4 = the route, r5 =
// &the index, r6 = &the position, r7 = &the step, r8 = masks; r3 =
// Normalize's.
void RaceWorldPlaceEntry(lifted::Regs& ctx);
// RaceManager::Create sub_821715B8: no arguments; r3 = the manager. The
// original unless --native_heap routes the game's heap to the allocation
// layer, or where operator new's plain C++ refuses.
void RaceWorldCreate(lifted::Regs& ctx);
// RaceManager::NoteMarkedRacer sub_82172F90: r3 not read (r3 not kept: its
// caller doesn't read it).
void RaceWorldNoteMarkedRacer(lifted::Regs& ctx);
// RaceManager::NearSegment sub_82170BC0: r3 = the segment, r4 = the point,
// f1 = the distance; r3 = 0 or 1.
void RaceWorldNearSegment(lifted::Regs& ctx);
// RaceManager::BuildNeighbors sub_82170CB8: r3 = the table (the manager +
// 2392); r3 not kept (its caller doesn't read it).
void RaceWorldBuildNeighbors(lifted::Regs& ctx);
// RaceManager::Neighbours sub_82170920: r3 = the manager, r4 = &an index; r3
// = 0 to 3.
void RaceWorldNeighbours(lifted::Regs& ctx);
// RaceManager::JumpSections sub_821737F8: r3 = the entry, r4 = the player's
// position, r5 = &the player's branch, r6, r7 = &the flags; r3 not kept.
void RaceWorldJumpSections(lifted::Regs& ctx);
// RaceManager::TickVehicles sub_821714D8 and RefreshVehicles sub_82171560:
// no arguments (r3 not kept: RaceWorld::Tick doesn't read it).
void RaceWorldTickVehicles(lifted::Regs& ctx);
void RaceWorldRefreshVehicles(lifted::Regs& ctx);
// RaceWorld::Init sub_82174D28: r3 = the race world; r3 as the last
// StateMachine::Add leaves it.
void RaceWorldInit(lifted::Regs& ctx);
// RaceWorld::ReleaseForStage sub_821713C0: r3 = the race world (r3 not
// kept: the state machine's handler call doesn't read it).
void RaceWorldReleaseForStage(lifted::Regs& ctx);
// RaceManager::ResetWorld sub_82170668: r3 = the manager. RaceManager::
// ResetVehicles sub_82171718: r3 = the manager, r4 = the flag.
// RaceWorld::Unload sub_82171250: r3 = the race world (not read). r3 not
// kept (their callers don't read it). The originals unless --native_heap
// routes the game's heap to the allocation layer.
void RaceWorldResetWorld(lifted::Regs& ctx);
void RaceWorldResetVehicles(lifted::Regs& ctx);
void RaceWorldUnload(lifted::Regs& ctx);
// SaveData::AddDistance sub_82116C38: r3 = the save data, f1 = the
// distance (r3 left).
void RaceWorldAddDistance(lifted::Regs& ctx);
// SaveData::FirstUnsetFlag sub_82112190: r3 = the save data; r3 = 0 to 6.
void RaceWorldFirstUnsetFlag(lifted::Regs& ctx);
// The start placements (r3 not kept: no caller reads it).
// RaceEvent::PlaceStartsCurrent sub_82144A38: r3 = &the kind, r4 = &the
// player's position. RaceEvent::PlaceStarts sub_82144798: r3 = the event,
// r4 = &the kind, r5 = &the player's position. RaceEvent::PlaceGroupFixed
// sub_821443B0: r3 = the event, r4 = &the step, r5 = &the group.
// RaceEvent::PlaceGroup sub_82143A40: r3 = the event, r4 = &the position, r5
// = the step, r6 = &the group, r7 = &the flag. RaceEvent::PlaceGroupTwoRows
// sub_82143B78: r3 = the event, r4, r5 = the positions, r6 = &the step, r7
// = &the group. RaceEvent::PlaceGroupSplit sub_82143920: r3 = the event, r4
// = the position, r5 = &the step, r6 = &the group, r7 = &the row.
// RaceManager::ListRacers sub_82171E50: r3 = the manager; r3 as the
// original leaves it (callers that return right after hand it on).
// RaceManager::AddRacers sub_82173050: r3 = the manager, r4 = the race
// data's racer records (r3 not kept: RaceWorld::OnLoaded doesn't read it).
void RaceWorldListRacers(lifted::Regs& ctx);
void RaceWorldAddRacers(lifted::Regs& ctx);
void RaceWorldPlaceStartsCurrent(lifted::Regs& ctx);
void RaceWorldPlaceStarts(lifted::Regs& ctx);
void RaceWorldPlaceGroupFixed(lifted::Regs& ctx);
void RaceWorldPlaceGroup(lifted::Regs& ctx);
void RaceWorldPlaceGroupTwoRows(lifted::Regs& ctx);
void RaceWorldPlaceGroupSplit(lifted::Regs& ctx);
// RaceWorld::OnLoaded sub_82173A90: r3 = the race world (not read; r3 not
// kept: the state machine's handler call doesn't read it); the original
// unless --native_heap routes the game's heap to the allocation layer.
// RaceStart::Place sub_82165E20: r3 = the object, r4 = the settle flag, r5
// = the back flag. RaceStart::PlaceRolling sub_821662E0: r3 = the object,
// r4 = the settle flag. r3 not kept (their callers don't read it).
void RaceWorldOnLoaded(lifted::Regs& ctx);
void RaceWorldPlace(lifted::Regs& ctx);
void RaceWorldPlaceRolling(lifted::Regs& ctx);
// RaceManager::StreamOpponents sub_82172118: r3 = the manager, r4 = the
// player's entry, f1 = the spawn radius, f2 = the despawn radius
// (Vehicle::RemoveFromWorld's original unless --native_heap routes the
// game's heap to the allocation layer; RaceManager::GiveVehicle's
// original). RaceManager::MoveUnseen sub_82173E28: r3 = the manager, r4 =
// the player's entry. r3 not kept (their callers don't read it).
void RaceWorldStreamOpponents(lifted::Regs& ctx);
// RaceManager::Refresh sub_82171978: r3 = the manager (its callees'
// originals as StreamOpponents'). r3 not kept (its callers don't read it).
void RaceWorldRefresh(lifted::Regs& ctx);
void RaceWorldMoveUnseen(lifted::Regs& ctx);
// RaceManager::StreamTraffic sub_821724F8: r3 = the manager, r4 = the
// player's entry, f1, f2 = the near and far radii, r5 = the second player's
// entry or 0 (Vehicle::RemoveFromWorld's and the physics set-up's originals
// unless --native_heap routes the game's heap to the allocation layer; the
// callees' originals where their plain C++ refuses). r3 not kept (no caller
// reads it).
void RaceWorldStreamTraffic(lifted::Regs& ctx);

// RaceManager::Stream sub_82174338, StreamNear sub_82172E60, StreamCruise
// sub_82174200 and StreamTwoPlayers sub_82172D20: r3 = the manager; RaceWorld::Tick
// sub_82174BC0: r3 = the race world (its callees' originals as StreamOpponents'
// and StreamTraffic's; sub_821743A8's original). r3 not kept (their callers
// don't read it).
void RaceWorldStream(lifted::Regs& ctx);
void RaceWorldStreamNear(lifted::Regs& ctx);
void RaceWorldStreamCruise(lifted::Regs& ctx);
void RaceWorldStreamTwoPlayers(lifted::Regs& ctx);
void RaceWorldTick(lifted::Regs& ctx);

// RaceManager::Refresh's callees' originals as its adapter makes them, for
// native callers in other subsystems (src/race_states/).
race_world::OriginalCall RaceWorldOpponentsCall(lifted::Regs& ctx);
// RaceStart::Place's and PlaceRolling's guest calls as their adapters make
// them (Vehicle::SetUp's frame `setup_frame`), for native callers in other
// subsystems.
race_world::StartCalls RaceWorldStartCalls(lifted::Regs& ctx, uint32_t setup_frame);
// RaceManager::ListRacers' callees' originals as its adapter makes them.
race_world::OriginalR3 RaceWorldListRacersOriginal(lifted::Regs& ctx);

// For the harness: while set, runs instead of RaceManager::GiveVehicle
// sub_821706E8 (never seen in play; its override, which StreamOpponents'
// original and plain C++ both reach), answering with made-up data.
using RaceWorldStandIn = void (*)(lifted::Regs&, uint8_t*);
extern RaceWorldStandIn g_give_vehicle_stand_in;
// For the harness: while set, runs instead of sub_821743A8 (never seen in
// play; its override, which RaceWorld::Tick's original and plain C++ both
// reach).
extern RaceWorldStandIn g_recorded_stand_in;

}  // namespace hooks
