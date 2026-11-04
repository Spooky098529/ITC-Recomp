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

namespace replay_snapshots {

// The input recordings' pointer (docs/known-data.md) and its blocks.
constexpr uint32_t kRecordings = 0x826C5B4C;
constexpr uint32_t kRecordBytes = 5232;
// The racers' manager's pointer (docs/functions/race-world.md).
constexpr uint32_t kManager = 0x826D82F4;

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

}  // namespace replay_snapshots
