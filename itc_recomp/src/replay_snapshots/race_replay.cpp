// The race's snapshots saved and put back (src/replay_snapshots/
// replay_snapshots.h, docs/functions/replay-snapshots.md): RaceReplay::
// SaveStart, SaveEnd, RestoreStart and RestoreEnd, the original's loads,
// stores and calls in its order (the recordings' and the manager's
// pointers read again where it reads them again).

#include "replay_snapshots/replay_snapshots.h"

#include "base/log.h"
#include "car_physics/car_physics.h"
#include "guest.h"
#include "hooks/vehicle.h"
#include "native/buffers.h"
#include "race_world/race_world.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace replay_snapshots {

namespace {

constexpr uint32_t kCountA = 0x107AC0;   // player 0's recording's count
constexpr uint32_t kCountB = 0x20F584;   // the second recording's count
constexpr uint32_t kBlockA = 0x20F590;   // the race's start
constexpr uint32_t kBlockB = 0x218DA0;   // the race's end
constexpr uint32_t kRecords = 2304;      // a block's first record
constexpr uint32_t kRestored = 0x2225B0; // set to 1 by the restores
constexpr uint32_t kZero = 0x82000A34;   // the game's float 0.0

uint32_t Recordings(uint8_t* base) { return guest::LoadU32(base, kRecordings); }
uint32_t Manager(uint8_t* base) { return guest::LoadU32(base, kManager); }

// Both recordings' counts = 0 (the recordings' pointer read for each).
void ClearCounts(uint8_t* base) {
  guest::StoreU32(base, Recordings(base) + kCountA, 0);
  guest::StoreU32(base, Recordings(base) + kCountB, 0);
}

// A block's seven records' words +0 = 0.
void ClearRecords(uint8_t* base, uint32_t block) {
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, block + kRecords + kRecordBytes * i, 0);
  }
}

// memcpy (sub_825A7410), or the same bytes one by one where its plain C++
// refuses (ranges that may share memory: never for the manager and a
// block).
void Memcpy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (native::Buffers_Memcpy(base, to, from, bytes, caller)) {
    return;
  }
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, to + i, guest::LoadU8(base, from + i));
  }
}

// The manager's entries and counts put back from a block's head (the
// reverse of RaceSnapshot::Save's copy). `caller`: the two memcpy loops'
// return addresses.
void RestoreHead(uint8_t* base, uint32_t manager, uint32_t head, uint32_t caller_entries,
                 uint32_t caller_traffic) {
  for (uint32_t i = 0; i < 77; i++) {
    Memcpy(base, manager + 4 + 28 * i, head + 28 * i, 28, caller_entries);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Memcpy(base, manager + 2160 + 28 * i, head + 2156 + 28 * i, 28, caller_traffic);
  }
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, manager + 2336 + 4 * i, guest::LoadU32(base, head + 2240 + 4 * i));
  }
  constexpr uint32_t kCounts[][2] = {{2364, 2268}, {2368, 2272}, {2372, 2276},
                                     {2376, 2280}, {2380, 2284}, {2388, 2288}};
  for (const auto& c : kCounts) {
    guest::StoreU32(base, manager + c[0], guest::LoadU32(base, head + c[1]));
  }
}

void ResetWorld(uint8_t* base, uint32_t frame, const ReplayCalls& calls) {
  race_world::ResetWorld(base, Manager(base), frame - 128,
                         [&calls](uint32_t address, uint32_t sp, uint32_t r3) {
                           calls.list(address, sp, r3);
                         });
}

void ResetVehicles(uint8_t* base, uint32_t flag, uint32_t frame, const ReplayCalls& calls) {
  race_world::ResetVehicles(base, Manager(base), flag, frame - 224, calls.setup);
}

uint64_t ListRacers(uint8_t* base, uint32_t frame, const ReplayCalls& calls) {
  return race_world::ListRacers(base, Manager(base), frame - 1872, true, calls.list);
}

// Vehicle::LinkTrackPositions and CollisionObject::ResetTransform (its
// collision object, +340): their plain C++, or their originals where they
// refuse.
void Relink(uint8_t* base, uint32_t vehicle, uint32_t frame, const ReplayCalls& calls) {
  uint32_t r3 = 0;
  if (!vehicle::VehicleLinkTrackPositions(base, vehicle, &r3)) {
    calls.callee(0x820C1D08, frame, vehicle);
  }
  const uint32_t object = guest::LoadU32(base, vehicle + 340);
  if (!car_physics::CollisionObjectResetTransform(base, object, frame - 112)) {
    calls.callee(0x820B7998, frame, object);
  }
}

// The vehicles put back from a block's records (at `records` from the
// recordings), each in use (word +0 not 0): Record5232::Restore, then with
// `place` (RestoreEnd) the vehicle placed on the route unless it's flagged
// 0x81 with no contact bits, then Relink.
void PutBack(uint8_t* base, uint32_t records, bool place, uint32_t frame,
             const ReplayCalls& calls) {
  const double zero = double(guest::LoadFloat(base, kZero));
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t offset = kRecordBytes * i;
    if (guest::LoadU32(base, Recordings(base) + offset + records) == 0) {
      continue;
    }
    const uint32_t vehicle = vehicle_accessors::VehicleGet(base, i);
    Record5232Restore(base, Recordings(base) + offset + records, vehicle);
    if (place && ((guest::LoadU32(base, vehicle + 6148) & 0x81) == 0 ||
                  guest::LoadU32(base, vehicle + 6152) != 0)) {
      const uint32_t moving =
          double(guest::LoadFloat(base, vehicle + 6224)) > zero ? 1 : 0;
      const double speed = double(Vehicle_GetSpeedKmh(base, vehicle));
      const uint32_t position = vehicle_accessors::VehicleIdentity(vehicle) + 4;
      if (!vehicle::VehiclePlaceOnRoute(base, vehicle, position, speed, zero, int32_t(moving),
                                        1, frame - 560, calls.place)) {
        calls.place_original(frame, vehicle, position, speed, zero, moving, 1);
      }
    }
    Relink(base, vehicle, frame, calls);
  }
}

}  // namespace

uint64_t RaceReplaySaveStart(uint8_t* base, uint32_t frame, const ReplayCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82104D40 RaceReplay::SaveStart");
  ClearCounts(base);
  ClearRecords(base, Recordings(base) + kBlockA);
  ClearRecords(base, Recordings(base) + kBlockB);
  RaceSnapshotSave(base, Recordings(base) + kBlockA);
  ResetWorld(base, frame, calls);
  ResetVehicles(base, 0, frame, calls);
  PutBack(base, kBlockA + kRecords, false, frame, calls);
  return ListRacers(base, frame, calls);
}

uint32_t RaceReplaySaveEnd(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82104E68 RaceReplay::SaveEnd");
  ClearRecords(base, Recordings(base) + kBlockB);
  return RaceSnapshotSave(base, Recordings(base) + kBlockB);
}

uint64_t RaceReplayRestoreStart(uint8_t* base, uint32_t frame, const ReplayCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821046F8 RaceReplay::RestoreStart");
  ClearCounts(base);
  ResetWorld(base, frame, calls);
  RestoreHead(base, Manager(base), Recordings(base) + kBlockA, 0x82104774, 0x821047A0);
  ResetVehicles(base, 1, frame, calls);
  PutBack(base, kBlockA + kRecords, false, frame, calls);
  const uint64_t r3 = ListRacers(base, frame, calls);
  guest::StoreU32(base, Recordings(base) + kRestored, 1);
  return r3;
}

uint64_t RaceReplayRestoreEnd(uint8_t* base, uint32_t frame, const ReplayCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82104898 RaceReplay::RestoreEnd");
  ClearCounts(base);
  ResetWorld(base, frame, calls);
  RestoreHead(base, Manager(base), Recordings(base) + kBlockB, 0x8210491C, 0x82104948);
  ResetVehicles(base, 1, frame, calls);
  PutBack(base, kBlockB + kRecords, true, frame, calls);
  const uint64_t r3 = ListRacers(base, frame, calls);
  guest::StoreU32(base, Recordings(base) + kRestored, 1);
  return r3;
}

}  // namespace replay_snapshots
