// The replay's snapshots as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/replay-snapshots.md): the race's state
// (the seven vehicles and the racers' manager) saved into blocks of the
// input recordings and put back, so a replay starts from the race's start.
// A vehicle's 5,232-byte record (Record5232) has its parts: the track
// fields (+0), the rigid body (+608, BodyState), the physics part (+1136,
// Member1136) and the collision object (+4080, Member4080, with its two
// placed shape copies, Part464). Each part has a save (vehicle to record),
// a restore (record to vehicle) and a copy (record to record).
//
// The objects are guest objects shared with the recompiled code, so these
// work on guest memory with the originals' loads and stores in their order:
// an lfs then stfs pair keeps the word's bits (src/car_physics/float_ops.h),
// the vector, matrix, wheel and track position copies are their plain C++
// (vector_library::Vec4Copy, car_physics::MatrixCopyRows, WheelCopy,
// WheelRecord2Copy, vehicle::TrackPositionCopy), memcpy through
// native::Buffers_Memcpy. Register saves and back chains aren't written
// (nothing else is on their stacks). Functions returning uint32_t give the
// r3 the original leaves. Adapters: src/hooks/replay_snapshots.cpp, behind
// --native_replay_snapshots, which run the original when
// native::Buffers_NativeMemcpy() is off.

#pragma once

#include <cstdint>
#include <functional>

#include "race_world/race_world.h"
#include "vehicle/vehicle.h"

namespace replay_snapshots {

// The input recordings' pointer (docs/known-data.md) and its blocks.
constexpr uint32_t kRecordings = 0x826C5B4C;
constexpr uint32_t kRecordBytes = 5232;
// The racers' manager's pointer (docs/functions/race-world.md).
constexpr uint32_t kManager = 0x826D82F4;
// The race data's pointer (docs/known-data.md).
constexpr uint32_t kRaceData = 0x826C5B2C;

// --- A vehicle's record: the restores (record to vehicle) ---

// Record5232::RestoreTrack (sub_82104310): vehicle +6148, +6152 = record +0,
// +4; memcpy of the 112 bytes at +16 to +6160; the five track positions
// (+128 + 96 i to +5232 + 96 i).
void Record5232RestoreTrack(uint8_t* base, uint32_t record, uint32_t vehicle);
// BodyState::Restore (sub_821031A0): the rigid body's first 516 bytes from
// the record's +608 (same offsets).
void BodyStateRestore(uint8_t* base, uint32_t state, uint32_t body);
// Member1136::Restore (sub_82102C60): the vehicle's physics part (+0 to
// +3241) from the record's 2,944 packed bytes.
void Member1136Restore(uint8_t* base, uint32_t part, uint32_t vehicle);
// Part464::Restore (sub_82102AA8): a placed shape copy from a record part.
void Part464Restore(uint8_t* base, uint32_t part, uint32_t shape);
// Member4080::Restore (sub_82102B98): nothing when the record's +0 is 0 or
// there's no object (r3 = the record part); else the object from the
// record, its +32 = its +20. Returns r3.
uint32_t Member4080Restore(uint8_t* base, uint32_t part, uint32_t object);
// Record5232::Restore (sub_821044D8): RestoreTrack, BodyState::Restore
// (+608, the vehicle's [+336]), Member1136::Restore (+1136),
// Member4080::Restore (+4080, [+340]). Returns r3 (Member4080::Restore's).
uint32_t Record5232Restore(uint8_t* base, uint32_t record, uint32_t vehicle);

// --- A vehicle's record: the saves (vehicle to record) ---

// Record5232::SaveTrack (sub_82104380): the reverse of RestoreTrack.
void Record5232SaveTrack(uint8_t* base, uint32_t record, uint32_t vehicle);
// BodyState::Save (sub_821039F0): the rigid body's first 516 bytes into the
// record's +608.
void BodyStateSave(uint8_t* base, uint32_t state, uint32_t body);
// Member1136::Save (sub_821034B8): the vehicle's physics part packed into
// the record's +1136.
void Member1136Save(uint8_t* base, uint32_t part, uint32_t vehicle);
// Part464::Save (sub_82103300): a placed shape copy into a record part.
void Part464Save(uint8_t* base, uint32_t part, uint32_t shape);
// Member4080::Save (sub_821033F0): with no object, +0 = 0 (r3 the part);
// else +0 = 1 and the object's fields (r3 the part + 1136).
uint32_t Member4080Save(uint8_t* base, uint32_t part, uint32_t object);
// Record5232::Save (sub_82104538): SaveTrack, BodyState::Save,
// Member1136::Save, Member4080::Save. Returns r3 (Member4080::Save's).
uint32_t Record5232Save(uint8_t* base, uint32_t record, uint32_t vehicle);

// --- The race's snapshot (a 38,928-byte block) ---

// RaceSnapshot::Save (sub_82104AB8): +2292 = 0; each vehicle in use
// (Vehicle::Get, +6148 not 0) into its record (+2304 + 5,232 i; the others
// left); the manager's entries and counts into the head (memcpy of 77 and
// 3 entries of 28 bytes, then words). Returns r3 (the last memcpy's).
uint32_t RaceSnapshotSave(uint8_t* base, uint32_t block);

// --- A vehicle's record: the copies (record to record; r3 = out) ---

// Record5232::CopyTrack (sub_821043F0): words +0, +4, memcpy of +16 (112
// bytes), the five track positions.
uint32_t Record5232CopyTrack(uint8_t* base, uint32_t out, uint32_t record);
// BodyState::Copy (sub_82103B50): the 516 bytes at the same offsets.
uint32_t BodyStateCopy(uint8_t* base, uint32_t out, uint32_t record);
// Member1136::Copy (sub_82103CB8): the 2,944 bytes at the same offsets.
uint32_t Member1136Copy(uint8_t* base, uint32_t out, uint32_t record);
// Part464::Copy (sub_821041C0): the 452 bytes at the same offsets.
uint32_t Part464Copy(uint8_t* base, uint32_t out, uint32_t record);
// Member4080::CopyShapes (sub_82104460): Part464::Copy at +0 and +464,
// vectors +928, +944, the matrix +960, the float +1024.
uint32_t Member4080CopyShapes(uint8_t* base, uint32_t out, uint32_t record);
// Member4080::Copy (sub_82104598): word +0, CopyShapes (+16), then +1056 to
// +1136.
uint32_t Member4080Copy(uint8_t* base, uint32_t out, uint32_t record);
// Record5232::Copy (sub_82104BC0): CopyTrack, BodyState::Copy (+608),
// Member1136::Copy (+1136), Member4080::Copy (+4080).
uint32_t Record5232Copy(uint8_t* base, uint32_t out, uint32_t record);
// RaceSnapshot::Copy (sub_82104EA8): memcpy of the head (2,292 bytes),
// +2292, Record5232::Copy of the seven records.
uint32_t RaceSnapshotCopy(uint8_t* base, uint32_t out, uint32_t block);

// --- The kept replay ---

// Stages::Copy's original where its plain C++ refuses (a copy memcpy's
// override must see), run with r1 = `sp`.
using StagesCopyOriginal =
    std::function<void(uint32_t out, uint32_t stages, uint32_t stage, uint32_t sp)>;
// Replay::Keep (sub_82104F68): the race's set-up (`race`, the race data),
// its 93 racer records' entries, the stage (the last, or the list's current
// - 1 with `from_stage`, through `frame` + 80), the last kept result, the
// snapshot of the race's start (RaceSnapshot::Copy of block A at
// `recordings` + 0x20F590), player 0's recording and its count (memcpy of
// 0x107AC4 bytes) and the save data's five words (`save`, or the defaults)
// into the kept replay. `frame`: the original's 272 bytes. Returns r3 (the
// recording's memcpy's: replay + 0x34E20).
uint32_t ReplayKeep(uint8_t* base, uint32_t replay, uint32_t race, uint32_t recordings,
                    uint32_t save, uint32_t from_stage, uint32_t frame,
                    const StagesCopyOriginal& stages_copy);

// --- The race saved and put back (src/replay_snapshots/race_replay.cpp;
// only while --native_heap routes the game's heap to the allocation layer,
// as RaceManager::ResetWorld's and ResetVehicles' plain C++: the adapters
// check) ---

// The guest calls their callees make where the plain C++ refuses, as the
// callees' adapters make them.
struct ReplayCalls {
  // RaceManager::ListRacers' callees' originals (ResetWorld's are the same).
  race_world::OriginalR3 list;
  // RaceManager::ResetVehicles' Vehicle::SetUp callees' originals.
  vehicle::CalleeOriginal setup;
  // Vehicle::LinkTrackPositions' (0x820C1D08) and CollisionObject::
  // ResetTransform's (0x820B7998) originals, run with r1 = `sp`.
  std::function<void(uint32_t address, uint32_t sp, uint32_t r3)> callee;
  // Vehicle::PlaceOnRoute's calls (ResetTransform's original, memcpy), and
  // its original where it refuses (r1 = `sp`).
  vehicle::GuestCall place;
  std::function<void(uint32_t sp, uint32_t vehicle, uint32_t position, double speed,
                     double offset, uint32_t lane, uint32_t direction)>
      place_original;
};
// RaceReplay::SaveStart (sub_82104D40): both counts 0, both blocks' records'
// words 0, RaceSnapshot::Save into block A, then the cars put back from it
// (ResetWorld, ResetVehicles(0), each record in use restored and relinked,
// ListRacers). `frame`: the original's 144 bytes. Returns r3 (ListRacers').
uint64_t RaceReplaySaveStart(uint8_t* base, uint32_t frame, const ReplayCalls& calls);
// RaceReplay::SaveEnd (sub_82104E68): block B's records' words 0, then
// RaceSnapshot::Save into it (a tail call: its r3).
uint32_t RaceReplaySaveEnd(uint8_t* base);
// RaceReplay::RestoreStart (sub_821046F8): both counts 0, ResetWorld, the
// manager's entries and counts from block A's head, ResetVehicles(1), each
// record in use restored and relinked, ListRacers, recordings + 0x2225B0 =
// 1. `frame`: the original's 144 bytes. Returns r3 (ListRacers').
uint64_t RaceReplayRestoreStart(uint8_t* base, uint32_t frame, const ReplayCalls& calls);
// RaceReplay::RestoreEnd (sub_82104898): as RestoreStart with block B, each
// vehicle placed on the route after its restore (Vehicle::PlaceOnRoute at
// its route position, its speed, offset 0.0, lane 1 when +6224 is above
// 0.0, direction 1) unless flagged 0x81 with no contact bits. `frame`: the
// original's 160 bytes.
uint64_t RaceReplayRestoreEnd(uint8_t* base, uint32_t frame, const ReplayCalls& calls);

}  // namespace replay_snapshots
