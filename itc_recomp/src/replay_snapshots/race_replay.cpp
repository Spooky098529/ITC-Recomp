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
#include "schema/car_physics.h"
#include "schema/race_world.h"
#include "schema/replay_snapshots.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace replay_snapshots {

namespace {

using Record = schema::ReplayRecord;
using Snapshot = schema::ReplaySnapshot;
using G = schema::ReplaySnapshotsGlobals;

constexpr uint32_t kCountA = 0x107AC0;   // player 0's recording's count
constexpr uint32_t kCountB = 0x20F584;   // the second recording's count
constexpr uint32_t kBlockA = 0x20F590;   // the race's start
constexpr uint32_t kBlockB = 0x218DA0;   // the race's end
constexpr uint32_t kRestored = 0x2225B0; // set to 1 by the restores

uint32_t Recordings() { return G::input_recordings(); }
uint32_t Manager() { return G::racers_manager(); }

// Both recordings' counts = 0 (the recordings' pointer read for each).
void ClearCounts() {
  guest::StoreU32(Recordings() + kCountA, 0);
  guest::StoreU32(Recordings() + kCountB, 0);
}

// A block's seven records' words +0 = 0.
void ClearRecords(uint32_t block) {
  for (uint32_t i = 0; i < 7; i++) {
    Record(Snapshot(block).records_address() + Record::kSize * i).set_car_flags(0);
  }
}

// memcpy (sub_825A7410), or the same bytes one by one where its plain C++
// refuses (ranges that may share memory: never for the manager and a
// block).
void Memcpy(uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (native::Buffers_Memcpy(to, from, bytes, caller)) {
    return;
  }
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(to + i, guest::LoadU8(from + i));
  }
}

// The manager's entries and counts put back from a block's head (the
// reverse of RaceSnapshot::Save's copy). `caller`: the two memcpy loops'
// return addresses.
void RestoreHead(uint32_t manager, uint32_t head, uint32_t caller_entries,
                 uint32_t caller_traffic) {
  const Snapshot h(head);
  const schema::RaceManager m(manager);
  for (uint32_t i = 0; i < 77; i++) {
    Memcpy(m.entries_address() + 28 * i, h.entries_address() + 28 * i, 28, caller_entries);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Memcpy(m.traffic_entries_address() + 28 * i, h.traffic_entries_address() + 28 * i, 28,
           caller_traffic);
  }
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(m.vehicle_slots_address() + 4 * i,
                    guest::LoadU32(h.slot_0_address() + 4 * i));
  }
  m.set_racer_count(h.racer_count());
  m.set_entry_count(h.entry_count());
  m.set_opponents_with_vehicle(h.opponent_count());
  m.set_traffic_with_vehicle(h.traffic_listed());
  m.set_traffic_allowed(h.traffic_count());
  m.set_traffic_ticks(h.unknown_2288());
}

void ResetWorld(uint32_t frame, const ReplayCalls& calls) {
  race_world::ResetWorld(Manager(), frame - 128,
                         [&calls](uint32_t address, uint32_t sp, uint32_t r3) {
                           calls.list(address, sp, r3);
                         });
}

void ResetVehicles(uint32_t flag, uint32_t frame, const ReplayCalls& calls) {
  race_world::ResetVehicles(Manager(), flag, frame - 224, calls.setup);
}

uint64_t ListRacers(uint32_t frame, const ReplayCalls& calls) {
  return race_world::ListRacers(Manager(), frame - 1872, true, calls.list);
}

// Vehicle::LinkTrackPositions and CollisionObject::ResetTransform (its
// collision object, +340): their plain C++, or their originals where they
// refuse.
void Relink(uint32_t vehicle, uint32_t frame, const ReplayCalls& calls) {
  uint32_t r3 = 0;
  if (!vehicle::VehicleLinkTrackPositions(vehicle, &r3)) {
    calls.callee(0x820C1D08, frame, vehicle);
  }
  const uint32_t object = schema::Vehicle(vehicle).collision_object();
  if (!car_physics::CollisionObjectResetTransform(object, frame - 112)) {
    calls.callee(0x820B7998, frame, object);
  }
}

// The vehicles put back from a block's records (the block at `block` from
// the recordings), each in use (car_flags not 0): Record5232::Restore, then with
// `place` (RestoreEnd) the vehicle placed on the route unless it's flagged
// 0x81 with no contact bits, then Relink.
void PutBack(uint32_t block, bool place, uint32_t frame,
             const ReplayCalls& calls) {
  const double zero = double(G::zero());
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t offset = Record::kSize * i;
    if (Record(Snapshot(Recordings() + block).records_address() + offset)
            .car_flags() == 0) {
      continue;
    }
    const schema::Vehicle v(vehicle_accessors::VehicleGet(i));
    const uint32_t vehicle = v.address;
    Record5232Restore(Snapshot(Recordings() + block).records_address() + offset,
                      vehicle);
    if (place && ((v.car_flags() & 0x81) == 0 || v.collision_flags() != 0)) {
      // +6224: the status block's +64 (no field; car-physics.toml's).
      const uint32_t moving =
          double(guest::LoadFloat(v.status_address() + 64)) > zero ? 1 : 0;
      const double speed = double(Vehicle_GetSpeedKmh(vehicle));
      const uint32_t position = vehicle_accessors::VehicleIdentity(vehicle) + 4;
      if (!vehicle::VehiclePlaceOnRoute(vehicle, position, speed, zero, int32_t(moving),
                                        1, frame - 560, calls.place)) {
        calls.place_original(frame, vehicle, position, speed, zero, moving, 1);
      }
    }
    Relink(vehicle, frame, calls);
  }
}

}  // namespace

uint64_t RaceReplaySaveStart(uint32_t frame, const ReplayCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82104D40 RaceReplay::SaveStart");
  ClearCounts();
  ClearRecords(Recordings() + kBlockA);
  ClearRecords(Recordings() + kBlockB);
  RaceSnapshotSave(Recordings() + kBlockA);
  ResetWorld(frame, calls);
  ResetVehicles(0, frame, calls);
  PutBack(kBlockA, false, frame, calls);
  return ListRacers(frame, calls);
}

uint32_t RaceReplaySaveEnd() {
  ITC_NATIVE_RUNNING("sub_82104E68 RaceReplay::SaveEnd");
  ClearRecords(Recordings() + kBlockB);
  return RaceSnapshotSave(Recordings() + kBlockB);
}

uint64_t RaceReplayRestoreStart(uint32_t frame, const ReplayCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821046F8 RaceReplay::RestoreStart");
  ClearCounts();
  ResetWorld(frame, calls);
  RestoreHead(Manager(), Recordings() + kBlockA, 0x82104774, 0x821047A0);
  ResetVehicles(1, frame, calls);
  PutBack(kBlockA, false, frame, calls);
  const uint64_t r3 = ListRacers(frame, calls);
  guest::StoreU32(Recordings() + kRestored, 1);
  return r3;
}

uint64_t RaceReplayRestoreEnd(uint32_t frame, const ReplayCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82104898 RaceReplay::RestoreEnd");
  ClearCounts();
  ResetWorld(frame, calls);
  RestoreHead(Manager(), Recordings() + kBlockB, 0x8210491C, 0x82104948);
  ResetVehicles(1, frame, calls);
  PutBack(kBlockB, true, frame, calls);
  const uint64_t r3 = ListRacers(frame, calls);
  guest::StoreU32(Recordings() + kRestored, 1);
  return r3;
}

}  // namespace replay_snapshots
