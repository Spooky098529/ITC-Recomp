// The racers' manager's streaming each tick (docs/functions/race-world.md):
// RaceManager::StreamOpponents, RaceManager::StreamTraffic,
// RaceManager::MoveUnseen, the streaming by the race's kind above them
// (RaceManager::Stream, StreamNear, StreamCruise, StreamTwoPlayers) and
// RaceWorld::Tick. See race_world.h.

#include "race_world/race_world.h"

#include "schema/car_physics.h"
#include "schema/collision_world.h"
#include "schema/minimap.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/track_route.h"
#include "schema/vehicle.h"
#include "schema/vehicle_accessors.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "memory/host_copy.h"
#include "hooks/vehicle.h"
#include "racer_controllers/racer_controllers.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cmath>
#include <cstdint>

namespace race_world {

using M = schema::RaceManager;
using G = schema::RaceWorldGlobals;
using L = schema::RaceSortedList;
using N = schema::RaceSortedNode;
using E = schema::MinimapRacerEntry;
using RD = schema::RaceData;
using Car = schema::Car;
using Section = schema::TrackRouteSection;

using namespace car_physics::ops;
namespace va = vehicle_accessors;
namespace vl = vector_library;

namespace {

constexpr uint32_t kInputs = 0x826C5B30;      // the racers' input records
constexpr uint32_t kFarKey = 0x8200222C;      // the key of entries flagged 0x10 or 0x400
constexpr uint32_t kTicksScale = 0x820099D4;  // a slice's ticks, by default
constexpr uint32_t kAhead = 0x820021E8;       // Neighbours 1: one ahead
constexpr uint32_t kBehind = 0x82001070;      // Neighbours 2: one behind
constexpr uint32_t kBoth = 0x820029F0;        // Neighbours 3: both
constexpr uint32_t kGiveVehicle = 0x821706E8;
constexpr uint32_t kRemoveFromWorld = 0x820C20E0;
constexpr uint32_t kSlowFactor = 0x820027A8;   // 1.5: the km/h scaled against the speed below
constexpr uint32_t kTrafficSpeed = 0x82009974;  // the traffic's speed placed (and its limit)
constexpr uint32_t kTooNear = 0x82002568;      // 20.0: a racer this near the spot refuses it
constexpr uint32_t kTrafficStep = 0x826193C8;  // the physics step's time on a new traffic car
constexpr uint32_t kRefreshSpawn = 0x8200996C;    // Refresh: nearer than this given a vehicle
constexpr uint32_t kRefreshDespawn = 0x82009970;  // Refresh: beyond this losing it

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// A halfword sign-extended to a register (extsh, lha).
uint32_t S16(uint32_t value) {
  return uint32_t(int32_t(int16_t(uint16_t(value))));
}
uint32_t LoadS16(uint32_t address) {
  return S16(guest::LoadU16(address));
}
// The racer record `number` (the race data's +48 + 3,936 x number).
Car CarOf(uint32_t number) {
  return Car(number * Car::kSize + RD(Load(kRaceData)).racer_records_address());
}
// A vehicle's status (+6160).
schema::VehicleStatus StatusOf(uint32_t vehicle) {
  return schema::VehicleStatus(schema::Vehicle(vehicle).status_address());
}
// A section's record in the route's table.
uint32_t SectionRecord(uint32_t file, uint32_t section) {
  return Load(schema::TrackRouteFile(file).sections() + (section << 2));
}

// An entry loses its vehicle (when it has one and its slot word is above
// 0): flags &= ~0x40, Vehicle::RemoveFromWorld (its original without
// `heap` or where its plain C++ refuses, with r1 = `frame`), the slot word
// 0 and +20 = -1.
void Lose(uint32_t manager, uint32_t entry, uint32_t frame, bool heap,
          const OriginalCall& original) {
  const uint32_t which = E(entry).vehicle_index();
  if (int32_t(which) == -1 || int32_t(Load(M(manager).vehicle_slots_address() + (which << 2))) <= 0) {
    return;
  }
  E(entry).set_flags(E(entry).flags() & ~0x40u);
  const uint32_t car = va::VehicleGet(which);
  uint32_t removed = 0;
  if (!heap || !vehicle::VehicleRemoveFromWorld(car, &removed)) {
    original(kRemoveFromWorld, frame, car, 0, 0);
  }
  Store(M(manager).vehicle_slots_address() + (E(entry).vehicle_index() << 2), 0);
  E(entry).set_vehicle_index(0xFFFFFFFFu);
}

// An entry with a vehicle put in the racers' list (+2308): +2364 and
// the opponents' (+2372) or with `traffic` the traffic's (+2376) count.
void List(uint32_t manager, uint32_t entry, bool traffic = false) {
  const M m(manager);
  const uint32_t counter =
      traffic ? m.traffic_with_vehicle_address() : m.opponents_with_vehicle_address();
  Store(m.racers_address() + (m.racer_count() << 2), entry);
  const uint32_t listed = m.racer_count();
  const uint32_t racers = Load(counter);
  m.set_racer_count(listed + 1);
  Store(counter, racers + 1);
}

// fctiwz's low word, as the translation leaves it (stfiwx).
uint32_t FctiwzWord(double value) {
  if (std::isnan(value)) {
    return 0x80000000u;
  }
  if (value >= double(INT32_MAX)) {
    return uint32_t(INT32_MAX);
  }
  if (value <= -2147483649.0) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(value));
}

// Vehicle::RemoveFromWorld: its plain C++ with `heap`, else (or where it
// refuses) its original with r1 = `frame`.
void RemoveFromWorld(uint32_t car, uint32_t frame, bool heap,
                     const OriginalCall& original) {
  uint32_t removed = 0;
  if (!heap || !vehicle::VehicleRemoveFromWorld(car, &removed)) {
    original(kRemoveFromWorld, frame, car, 0, 0);
  }
}

}  // namespace

void StreamOpponents(uint32_t manager, uint32_t player, double spawn,
                     double despawn, uint32_t frame, bool heap, const OriginalCall& original) {
  ITC_NATIVE_RUNNING("sub_82172118 RaceManager::StreamOpponents");
  // The player's route position (its vehicle's identity +4; the original
  // split it through its frame's + 80, a value since E5 step 6), its
  // section's record.
  const uint32_t identity = va::VehicleIdentity(va::VehicleGet(E(player).vehicle_index()));
  const uint32_t position = schema::TrackPosition(identity).route_position();
  const int32_t count = int32_t(M(manager).entry_count());
  const uint32_t file = Load(kRoute);
  const uint32_t player_section = position >> 16;
  const uint32_t player_record = SectionRecord(file, player_section);
  // The sorted list at + 176 emptied (+1776 to +1784).
  const L sorted(frame + 176);
  M(manager).set_opponents_with_vehicle(0);
  sorted.set_last_inserted(0);
  sorted.set_count(0);
  sorted.set_first(0);
  const uint32_t list = sorted.address;
  if (count > 0) {
    const double one = Lfs(kOne);
    const double far_key = Lfs(kFarKey);
    const double zero = Lfs(kZero);
    for (int32_t i = 0;;) {
      const uint32_t entry = M(manager).entries_address() + E::kSize * uint32_t(i);
      const uint32_t flags = E(entry).flags();
      bool lose = (flags & 0x20) != 0 || (flags & 0x80) != 0;
      double key = 0.0;
      if (lose) {
      } else if ((flags & 0x4) != 0 || (flags & 0x8) != 0) {
        key = zero;
      } else if ((flags & 0x10) != 0 || (flags & 0x400) != 0) {
        key = far_key;
      } else {
        const uint32_t word = E(entry).position();
        const uint32_t now = Load(kRoute);
        const uint32_t a = Section(player_record).branch();
        // Split as the original did through its frame's + 84.
        const uint32_t section = word >> 16;
        const uint32_t b = Section(SectionRecord(now, section)).branch();
        if (BranchesMeet(a, b) == 0) {
          lose = true;
        } else {
          // Both slices' centers: the entry's at + 128, the player's at +
          // 112; the difference at + 96, its y = 0.0 and w = 1.0; its
          // length + 1.0.
          track_route::SlicePoint(frame + 128, 0, S16(section), S16(word & 0xFFFF));
          track_route::SlicePoint(frame + 112, 0, S16(player_section),
                                  S16(position & 0xFFFF));
          vl::Vec3Sub(frame + 96, frame + 128, frame + 112);
          Stfs(frame + 100, zero);
          Stfs(frame + 108, one);
          key = Fadds(double(vl::Vec3Normalize(frame + 96, frame + 96)), one);
        }
      }
          if (lose) {
        Lose(manager, entry, frame, heap, original);
      } else {
        SortedListInsert(list, entry, key);
      }
      i++;
      if (!(i < int32_t(M(manager).entry_count()))) {
        break;
      }
    }
  }
  // Past the fourth, the vehicle removed.
  uint32_t n = 0;
  for (uint32_t node = L(frame + 176).first().address; node != 0; node = N(node).next().address, n++) {
    const uint32_t entry = N(node).item();
    if (n >= 4) {
      Lose(manager, entry, frame, heap, original);
    }
  }
  // Up to four: nearer than `spawn` given a vehicle (or keeping theirs),
  // beyond `despawn` losing theirs, between keeping theirs.
  for (uint32_t node = L(frame + 176).first().address; node != 0; node = N(node).next().address) {
    const uint32_t entry = N(node).item();
    if (M(manager).opponents_with_vehicle() >= 4) {
      Lose(manager, entry, frame, heap, original);
      continue;
    }
    const double key = double(N(node).key());
    if (key < spawn) {
      if (int32_t(E(entry).vehicle_index()) != -1) {
        List(manager, entry);
        continue;
      }
      // RaceManager::GiveVehicle (never seen in play): its original.
      const uint64_t given = original(kGiveVehicle, frame, manager, entry, 0);
      if (uint32_t(given) == 0) {
        // No free slot: the empty sub_82106540 with (500, 220, 0,
        // 0x820099BC), probably a debug print compiled out.
        continue;
      }
      List(manager, entry);
      ai_drivers::ClearCarInput(frame + 144);
      const uint32_t to = Load(kInputs + ((E(entry).vehicle_index() << 2) & 0x3FC));
      // A host record (E5 step 4) is copied with each field swapped.
      if (!memory::CopyWithHandles(to, frame + 144, 20)) {
        for (uint32_t w = 0; w < 5; w++) {
          Store(to + 4 * w, Load(frame + 144 + 4 * w));
        }
      }
      const uint32_t number = E(entry).record();
      const uint32_t id = CarOf(number).item_id_address();
      Store(frame + 80, 1);
      racer_controllers::Setup(E(entry).vehicle_index_address(), id, frame + 80, frame);
    } else if (key > despawn) {
      Lose(manager, entry, frame, heap, original);
    } else if (int32_t(E(entry).vehicle_index()) != -1) {
      List(manager, entry);
    }
  }
}

void Refresh(uint32_t manager, uint32_t frame, bool heap,
             const OriginalCall& original) {
  ITC_NATIVE_RUNNING("sub_82171978 RaceManager::Refresh");
  // The player's entry: the first flagged 0x4 (the count read again each
  // pass), or none (0: its +20 read at 20).
  uint32_t player = 0;
  if (int32_t(M(manager).entry_count()) > 0) {
    for (int32_t i = 0;;) {
      if ((E(M(manager).entries_address() + E::kSize * uint32_t(i)).flags() & 0x4) != 0) {
        player = M(manager).entries_address() + E::kSize * uint32_t(i);
        break;
      }
      i++;
      if (!(i < int32_t(M(manager).entry_count()))) {
        break;
      }
    }
  }
  // Its route position (its vehicle's identity +4; the original split it
  // through its frame's + 80, a value since E5 step 6), its section's
  // record and its slice's center at + 112.
  const uint32_t identity = va::VehicleIdentity(va::VehicleGet(E(player).vehicle_index()));
  const uint32_t position = schema::TrackPosition(identity).route_position();
  const uint32_t player_section = position >> 16;
  const uint32_t player_record = SectionRecord(Load(kRoute), player_section);
  const uint32_t player_slice = S16(position & 0xFFFF);
  // The sorted list at + 176 emptied and the counts +2364, +2372, +2376 = 0.
  const L sorted(frame + 176);
  sorted.set_first(0);
  M(manager).set_racer_count(0);
  sorted.set_last_inserted(0);
  M(manager).set_opponents_with_vehicle(0);
  sorted.set_count(0);
  M(manager).set_traffic_with_vehicle(0);
  track_route::SlicePoint(frame + 112, 0, S16(player_section), player_slice);
  const uint32_t list = sorted.address;
  if (int32_t(M(manager).entry_count()) > 0) {
    const double one = Lfs(kOne);
    const double far_key = Lfs(kFarKey);
    const double zero = Lfs(kZero);
    for (int32_t i = 0;;) {
      const uint32_t entry = M(manager).entries_address() + E::kSize * uint32_t(i);
      const uint32_t flags = E(entry).flags();
      bool lose = (flags & 0x20) != 0 || (flags & 0x80) != 0;
      double key = 0.0;
      if (lose) {
      } else if ((flags & 0x4) != 0 || (flags & 0x8) != 0) {
        key = zero;
      } else if ((flags & 0x10) != 0 || (flags & 0x400) != 0) {
        key = far_key;
      } else {
        // The entry's position (split as the original did through its
        // frame's + 80; StreamOpponents': + 84).
        const uint32_t word = E(entry).position();
        const uint32_t now = Load(kRoute);
        const uint32_t a = Section(player_record).branch();
        const uint32_t section = word >> 16;
        const uint32_t b = Section(SectionRecord(now, section)).branch();
        if (BranchesMeet(a, b) == 0) {
          lose = true;
        } else {
          // The entry's slice's center at + 128; the difference from the
          // player's at + 96, its y = 0.0 and w = 1.0; its length + 1.0.
          track_route::SlicePoint(frame + 128, 0, S16(word >> 16), S16(word & 0xFFFF));
          vl::Vec3Sub(frame + 96, frame + 128, frame + 112);
          Stfs(frame + 100, zero);
          Stfs(frame + 108, one);
          key = Fadds(double(vl::Vec3Normalize(frame + 96, frame + 96)), one);
        }
      }
      if (lose) {
        Lose(manager, entry, frame, heap, original);
      } else {
        SortedListInsert(list, entry, key);
      }
      i++;
      if (!(i < int32_t(M(manager).entry_count()))) {
        break;
      }
    }
  }
  // Past the fourth, the vehicle removed.
  uint32_t n = 0;
  for (uint32_t node = L(frame + 176).first().address; node != 0; node = N(node).next().address, n++) {
    const uint32_t entry = N(node).item();
    if (n >= 4) {
      Lose(manager, entry, frame, heap, original);
    }
  }
  // Up to four: nearer than [0x8200996C] given a vehicle (or keeping
  // theirs), beyond [0x82009970] losing theirs, between keeping theirs.
  for (uint32_t node = L(frame + 176).first().address; node != 0; node = N(node).next().address) {
    const uint32_t entry = N(node).item();
    if (M(manager).opponents_with_vehicle() >= 4) {
      Lose(manager, entry, frame, heap, original);
      continue;
    }
    const double key = double(N(node).key());
    if (key < Lfs(kRefreshSpawn)) {
      if (int32_t(E(entry).vehicle_index()) != -1) {
        List(manager, entry);
        continue;
      }
      // RaceManager::GiveVehicle (never seen in play): its original. None
      // free: the rest of the list is left as it is (StreamOpponents goes
      // on with the next).
      const uint64_t given = original(kGiveVehicle, frame, manager, entry, 0);
      if (uint32_t(given) == 0) {
        break;
      }
      List(manager, entry);
      ai_drivers::ClearCarInput(frame + 144);
      const uint32_t to = Load(kInputs + ((E(entry).vehicle_index() << 2) & 0x3FC));
      // A host record (E5 step 4) is copied with each field swapped.
      if (!memory::CopyWithHandles(to, frame + 144, 20)) {
        for (uint32_t w = 0; w < 5; w++) {
          Store(to + 4 * w, Load(frame + 144 + 4 * w));
        }
      }
      const uint32_t number = E(entry).record();
      const uint32_t id = CarOf(number).item_id_address();
      Store(frame + 80, 1);
      racer_controllers::Setup(E(entry).vehicle_index_address(), id, frame + 80, frame);
    } else if (key > Lfs(kRefreshDespawn)) {
      Lose(manager, entry, frame, heap, original);
    } else if (int32_t(E(entry).vehicle_index()) != -1) {
      List(manager, entry);
    }
  }
  // The traffic (the three entries at +2160) in use: with the race data's
  // flags 0x4000 each loses its vehicle (+0 = 0, the slot word 0, +20 =
  // -1; no slot word check), else each is listed.
  if ((RD(Load(kRaceData)).flags() & 0x4000) != 0) {
    for (uint32_t t = 0; t < 3; t++) {
      const uint32_t entry = M(manager).traffic_entries_address() + E::kSize * t;
      if (E(entry).flags() == 0) {
        continue;
      }
      RemoveFromWorld(va::VehicleGet(E(entry).vehicle_index()), frame, heap,
                      original);
      const uint32_t which = E(entry).vehicle_index();
      E(entry).set_flags(0);
      Store(M(manager).vehicle_slots_address() + (which << 2), 0);
      E(entry).set_vehicle_index(0xFFFFFFFFu);
    }
    return;
  }
  for (uint32_t t = 0; t < 3; t++) {
    const uint32_t entry = M(manager).traffic_entries_address() + E::kSize * t;
    if (E(entry).flags() != 0) {
      List(manager, entry, true);
    }
  }
}

void StreamTraffic(uint32_t manager, uint32_t player, double inner, double outer,
                   uint32_t second, uint32_t frame, bool heap, const StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821724F8 RaceManager::StreamTraffic");
  // The player's route position (its vehicle's identity +4; the original's
  // frame + 84), its body's position and its velocity (+6176; the original
  // copied them to its frame's + 160 and + 176, read by nothing: read and
  // dropped since E5 step 6); the position again (the original's + 80, a
  // value as + 84 since E5 step 6), the second player's when there's one.
  const uint32_t car = va::VehicleGet(E(player).vehicle_index());
  const uint32_t position = schema::TrackPosition(va::VehicleIdentity(car)).route_position();
  const schema::VectorLibVector at(va::RigidBodyPosition(schema::Vehicle(car).body().address));
  (void)at;
  // Vehicle::Get of the player's +20 again and RigidBody::Position, their
  // answers unused.
  uint32_t position80 = schema::TrackPosition(va::VehicleIdentity(car)).route_position();
  const double slice_length = double(schema::TrackRoute(kRoute).file().slice_length());
  if (second != 0) {
    const uint32_t other = va::VehicleGet(E(second).vehicle_index());
    position80 = schema::TrackPosition(va::VehicleIdentity(other)).route_position();
  }
  M(manager).set_traffic_with_vehicle(0);
  // With the race data's flags 0x4000: every traffic car (the entries at
  // +2160) loses its vehicle.
  if ((RD(Load(kRaceData)).flags() & 0x4000) != 0) {
    for (uint32_t t = 0; t < 3; t++) {
      const uint32_t entry = M(manager).traffic_entries_address() + E::kSize * t;
      if (E(entry).flags() == 0) {
        continue;
      }
      RemoveFromWorld(va::VehicleGet(E(entry).vehicle_index()), frame, heap,
                      calls.original);
      const uint32_t which = E(entry).vehicle_index();
      E(entry).set_flags(0);
      Store(M(manager).vehicle_slots_address() + (which << 2), 0);
      E(entry).set_vehicle_index(0xFFFFFFFFu);
    }
    return;
  }
  // The players' slices' centers: the player's at + 208, the second's at +
  // 192.
  track_route::SlicePoint(frame + 208, 0, S16(position >> 16), S16(position & 0xFFFF));
  if (second != 0) {
    track_route::SlicePoint(frame + 192, 0, S16(position80 >> 16),
                            S16(position80 & 0xFFFF));
  }
  // Each traffic entry with a vehicle: its slot word counts its ticks (to
  // 216,000, then 60); farther than `outer` from both players (each flat
  // distance from its slice's center, at + 144, the difference at + 96), it
  // loses its vehicle when its slot word is above 0; else it's listed.
  const double zero = Lfs(kZero);
  for (uint32_t t = 0; t < 3; t++) {
    const uint32_t entry = M(manager).traffic_entries_address() + E::kSize * t;
    if (E(entry).flags() == 0) {
      continue;
    }
    const uint32_t vehicle = va::VehicleGet(E(entry).vehicle_index());
    const uint32_t slot = M(manager).vehicle_slots_address() + (E(entry).vehicle_index() << 2);
    Store(slot, Load(slot) + 1);
    const uint32_t again = M(manager).vehicle_slots_address() + (E(entry).vehicle_index() << 2);
    if (int32_t(Load(again)) > 216000) {
      Store(again, 60);
    }
    // Its route position (the original's frame + 84, a value since E5
    // step 6).
    const uint32_t at_route = schema::TrackPosition(va::VehicleIdentity(vehicle)).route_position();
    track_route::SlicePoint(frame + 144, 0, S16(at_route >> 16), S16(at_route & 0xFFFF));
    vl::Vec3Sub(frame + 96, frame + 144, frame + 208);
    Stfs(frame + 100, zero);
    const double first = double(vl::Vec3Normalize(frame + 96, frame + 96));
    double other = first;
    if (second != 0) {
      vl::Vec3Sub(frame + 96, frame + 144, frame + 192);
      Stfs(frame + 100, zero);
      other = double(vl::Vec3Normalize(frame + 96, frame + 96));
    }
    if (first > outer && other > outer &&
        int32_t(Load(M(manager).vehicle_slots_address() + (E(entry).vehicle_index() << 2))) > 0) {
      RemoveFromWorld(vehicle, frame, heap, calls.original);
      const uint32_t which = E(entry).vehicle_index();
      E(entry).set_flags(0);
      Store(M(manager).vehicle_slots_address() + (which << 2), 0);
      E(entry).set_vehicle_index(0xFFFFFFFFu);
    } else {
      // (The original also counts the entries flagged 0x800, for the
      // neighbor search below, which never runs.)
      List(manager, entry, true);
    }
  }
  if ((RD(Load(kRaceData)).flags() & 0x2000) != 0) {
    return;
  }
  // A new traffic car every 90 ticks at most (+2388 counts them), while
  // there are fewer than +2380.
  if (!(int32_t(M(manager).traffic_with_vehicle()) < int32_t(M(manager).traffic_allowed())) ||
      M(manager).traffic_ticks() < 90) {
    const uint32_t ticks = M(manager).traffic_ticks();
    if (ticks < 90) {
      M(manager).set_traffic_ticks(ticks + 1);
    }
    return;
  }
  M(manager).set_traffic_ticks(0);
  // The first free traffic entry (+0 0) with a free vehicle slot (its word
  // 0 or -1; the slots scanned for each free entry).
  uint32_t t = 0, which = 0xFFFFFFFFu;
  for (;; t++) {
    if (t >= 3) {
      return;
    }
    if (E(M(manager).traffic_entries_address() + E::kSize * t).flags() != 0) {
      continue;
    }
    for (uint32_t k = 0; k < 7; k++) {
      const uint32_t word = Load(M(manager).vehicle_slots_address() + 4 * k);
      if (word == 0 || word == 0xFFFFFFFFu) {
        which = k;
        break;
      }
    }
    if (which != 0xFFFFFFFFu) {
      break;
    }
  }
  const uint32_t entry = M(manager).traffic_entries_address() + E::kSize * t;
  // The spot: the original first looks on a section beside the player's
  // (RaceManager::BuildNeighbors' table, +2392) only while its count of the
  // listed traffic cars flagged 0x800 is below 0, which never holds (it
  // counts up from 0), so the spot is always a route position stepped on the
  // main road (RoutePosition::StepToMainRoad, kind -1) by a number of slices
  // (fctiwz into + 80, its low halfword): with the player's car's +6168 set,
  // 1 - `outer` / the slice length from the second player's position (the
  // player's without one); else, without the race data's flags 0x1000 and
  // with the car's km/h x [0x820027A8] below [0x82009974], 1 - `inner` / the
  // slice length from the same; else `inner` / the slice length from the
  // player's.
  // (The original's two halfwords of -1 at + 88 first, overwritten at once
  // by the position: dropped since E5 step 6. The steps' word at + 80 is a
  // value since then.)
  const uint32_t flag6168 = StatusOf(car).wrong_way();
  Store(frame + 88, position);
  bool less = false;  // the steps 1 - f13
  double f13 = 0.0;
  if (flag6168 != 0) {
    if (second != 0) {
      Store(frame + 88, position80);
    }
    f13 = Fdivs(outer, slice_length);
    less = true;
  } else if ((RD(Load(kRaceData)).flags() & 0x1000) == 0) {
    const double kmh = Fmuls(double(Vehicle_GetSpeedKmh(car)), Lfs(kSlowFactor));
    if (kmh < Lfs(kTrafficSpeed)) {
      if (second != 0) {
        Store(frame + 88, position80);
      }
      f13 = Fdivs(inner, slice_length);
      less = true;
    }
  }
  const uint32_t steps = less ? FctiwzWord(Fsubs(Lfs(kOne), f13))
                             : FctiwzWord(Fdivs(inner, slice_length));
  StepToMainRoad(frame + 88, S16(steps & 0xFFFF), 0xFFFFFFFFu, frame - 112);
  // The spot's center at + 128; refused (nothing more) when a listed racer's
  // body ([0x826D82F4]'s list, its count read again each pass; the position
  // at + 96, its y 0.0) is nearer than [0x82002568] (Vec3::Direction into +
  // 256).
  track_route::SlicePoint(frame + 128, 0, LoadS16(frame + 88),
                          LoadS16(frame + 90));
  Stfs(frame + 132, zero);
  {
    uint32_t global = G::racers_manager().address;
    if (int32_t(M(global).racer_count()) > 0) {
      const double too_near = Lfs(kTooNear);
      for (int32_t i = 0;;) {
        const uint32_t listed = Load(M(global).racers_address() + 4 * uint32_t(i));
        const uint32_t body = schema::Vehicle(va::VehicleGet(E(listed).vehicle_index())).body().address;
        const schema::VectorLibVector point(va::RigidBodyPosition(body));
        Store(frame + 96, point.x_bits());
        Store(frame + 100, point.y_bits());
        Store(frame + 104, point.z_bits());
        Store(frame + 108, point.w_bits());
        Stfs(frame + 100, zero);
        if (double(vl::Vec3Direction(frame + 256, frame + 128, frame + 96)) < too_near) {
          return;
        }
        global = G::racers_manager().address;
        i++;
        if (!(i < int32_t(M(global).racer_count()))) {
          break;
        }
      }
    }
  }
  // The entry: its model (Traffic::PickModel of the race data's +0xA3AE0 +
  // its +16 + its index, and the spot's branch), +0 = 578, +24 the model's
  // traffic record (+2244), +16 the number (less 42 while above 42); its
  // lane by the player's car's +6224 (0 below 0.0, else 1).
  const uint32_t file = Load(kRoute);
  const uint32_t branch =
      Section(SectionRecord(file, guest::LoadU16(frame + 88))).branch();
  const uint32_t number =
      RD(Load(kRaceData)).random_10() + E(entry).traffic_number() + t;
  const uint32_t model = uint32_t(PickModel(number, branch, frame));
  const uint32_t record = Load(M(manager).traffic_models_address() + (model << 2));
  E(entry).set_number(model);
  E(entry).set_flags(578);
  E(entry).set_record(record);
  const uint32_t lane = double(StatusOf(car).across()) < zero ? 0 : 1;
  E(entry).set_traffic_number(number);
  for (int32_t n = int32_t(number); n > 42;) {
    n = int32_t(E(entry).traffic_number()) - 42;
    E(entry).set_traffic_number(uint32_t(n));
  }
  // Its vehicle: Vehicle::SetUp (slot word -1) or ResetForCar with its
  // record (the race data + 48 + 3,936 x +24), the slot 1,
  // Vehicle::PlaceOnRoute on the spot ([0x82009974] the speed, 0.0 across,
  // the lane, 1), an input record cleared at + 224 and copied to the
  // racer's, VehicleStatus's speed record (+6160), one physics step
  // ([0x826193C8]) and VehicleStatus::Update(+6160, it, 0, 1); its driver
  // (AiDriver::Setup: -1 at + 80, 1 at + 84); listed.
  E(entry).set_vehicle_index(which);
  const uint32_t given = va::VehicleGet(which);
  const uint32_t slot = M(manager).vehicle_slots_address() + (which << 2);
  const uint32_t car_record = CarOf(E(entry).record()).address;
  double f1 = 0.0;
  vehicle::VehicleSetUp(given, car_record, frame - 224, int32_t(Load(slot)) != -1,
                        heap, calls.setup_original, &f1);
  Store(slot, 1);
  {
    const double speed = Lfs(kTrafficSpeed);
    if (!vehicle::VehiclePlaceOnRoute(given, frame + 88, speed, zero, int32_t(lane), 1,
                                      frame - 560, calls.place)) {
      vehicle::GuestRegisters r;
      r.r3 = given;
      r.r4 = frame + 88;
      r.r6 = lane;
      r.f1 = speed;
      r.f2 = zero;
      calls.place(0x820C3608, frame, r);
    }
  }
  ai_drivers::ClearCarInput(frame + 224);
  {
    const uint32_t to = Load(kInputs + ((E(entry).vehicle_index() << 2) & 0x3FC));
    // A host record (E5 step 4) is copied with each field swapped.
    if (!memory::CopyWithHandles(to, frame + 224, 20)) {
      for (uint32_t w = 0; w < 5; w++) {
        Store(to + 4 * w, Load(frame + 224 + 4 * w));
      }
    }
  }
  const uint32_t status = schema::Vehicle(given).status_address();
  va::VehicleSpeedRecord(status, given);
  {
    const double dt = Lfs(kTrafficStep);
    if (car_physics::CollisionWorldCallbackKnown()) {
      car_physics::VehiclePhysicsStep(given, dt, frame - 112, [&](uint32_t address) {
        vehicle::GuestRegisters r;
        r.r3 = given;
        calls.tick(address, frame - 112, r);
      });
    } else {
      vehicle::GuestRegisters r;
      r.r3 = given;
      r.f1 = dt;
      calls.tick(0x820B6498, frame, r);
    }
  }
  if (!vehicle::VehicleStatusUpdate(status, given, 0, 1, frame - 384)) {
    vehicle::GuestRegisters r;
    r.r3 = status;
    r.r4 = given;
    r.r5 = 0;
    r.r6 = 1;
    calls.tick(0x820C23E8, frame, r);
  }
  Store(frame + 84, 1);
  Store(frame + 80, 0xFFFFFFFFu);
  racer_controllers::Setup(E(entry).vehicle_index_address(), frame + 80, frame + 84, frame);
  List(manager, entry, true);
}

void MoveUnseen(uint32_t manager, uint32_t player, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82173E28 RaceManager::MoveUnseen");
  const uint32_t vehicle = va::VehicleGet(E(player).vehicle_index());
  const uint32_t identity = va::VehicleIdentity(vehicle);
  const uint32_t body = schema::Vehicle(vehicle).body().address;
  Store(frame + 88, schema::TrackPosition(identity).route_position());
  const schema::VectorLibVector at(va::RigidBodyPosition(body));
  const uint32_t racers = M(manager).opponents_with_vehicle();
  const uint32_t player_section = guest::LoadU16(frame + 88);
  const uint32_t race_flags = RD(Load(kRaceData)).branch_flags();
  const uint32_t file = Load(kRoute);
  const uint32_t table = schema::TrackRouteFile(file).sections();
  const double slice_length = double(schema::TrackRouteFile(file).slice_length());
  uint32_t flag20 = (race_flags >> 5) & 1;
  const uint32_t player_record = Load((player_section << 2) + table);
  const bool four = racers == 4;
  uint32_t hi[2] = {at.z_bits(), at.w_bits()};
  Store(frame + 80, flag20);
  Store(frame + 92, Section(player_record).branch());
  uint32_t lo[2] = {at.x_bits(), at.y_bits()};
  uint32_t flag10 = (race_flags >> 4) & 1;
  // The player's position (RigidBody::Position) at + 96.
  Store(frame + 96, lo[0]);
  Store(frame + 100, lo[1]);
  Store(frame + 104, hi[0]);
  Store(frame + 108, hi[1]);
  Store(frame + 84, flag10);
  // With the race data's flags 0x10 or 0x20: whether an entry of item kind
  // 2 (sections 200 to 203, 258 to 261; clears + 84) or 3 (187, 188, 193,
  // 194, 251 to 253; clears + 80) is already there.
  if ((flag10 != 0 || flag20 != 0) && int32_t(M(manager).entry_count()) > 0) {
    for (int32_t i = 0;;) {
      const uint32_t entry = M(manager).entries_address() + E::kSize * uint32_t(i);
      const uint32_t number = E(entry).record();
      const uint32_t race = Load(kRaceData);
      const int32_t id = int32_t(
          S16(Car(number * Car::kSize + RD(race).racer_records_address()).item_id()));
      if (id >= 0 && (E(entry).flags() & 0xAC) == 0) {
        const int32_t s = int32_t(S16(E(entry).section()));
        uint32_t which = 0;
        if (s > 203) {
          if (s >= 251) {
            if (s <= 253) {
              which = 3;
            } else if (uint32_t(s - 258) <= 3) {
              which = 2;
            }
          }
        } else if (s >= 200) {
          which = 2;
        } else if (s >= 187) {
          if (s <= 188 || (s > 192 && s <= 194)) {
            which = 3;
          }
        }
        if (which == 3 && flag20 != 0 &&
            schema::OpponentItem(data_tables::Item(uint32_t(id))).location() == 3) {
          flag20 = 0;
          Store(frame + 80, flag20);
        } else if (which == 2 && flag10 != 0 &&
                   schema::OpponentItem(data_tables::Item(uint32_t(id))).location() == 2) {
          flag10 = 0;
          Store(frame + 84, flag10);
        }
      }
      if (flag10 == 0 && flag20 == 0) {
        break;
      }
      i++;
      if (!(i < int32_t(M(manager).entry_count()))) {
        break;
      }
    }
  }
  // Each entry not flagged 0x20 or 0x80 (its index at + 88).
  Store(frame + 88, 0);
  if (!(int32_t(M(manager).entry_count()) > 0)) {
    return;
  }
  const double ahead = Lfs(kAhead);
  const double behind = Lfs(kBehind);
  const double both = Lfs(kBoth);
  const double scale = Lfs(kTicksScale);
  const double one = Lfs(kOne);
  const double zero = Lfs(kZero);
  for (uint32_t i = 0;;) {
    const uint32_t entry = M(manager).entries_address() + E::kSize * i;
    const uint32_t flags = E(entry).flags();
    if ((flags & 0x20) != 0 || (flags & 0x80) != 0) {
    } else if ((flags & 0x40) != 0) {
      // With a vehicle: +8 its track position, +12 0.0, its slot word
      // counting its ticks (to 216,000, then 60).
      const uint32_t own = va::VehicleIdentity(va::VehicleGet(E(entry).vehicle_index()));
      const uint32_t slot = M(manager).vehicle_slots_address() + (E(entry).vehicle_index() << 2);
      E(entry).set_section(schema::TrackPosition(own).route_section());
      E(entry).set_slice(schema::TrackPosition(own).route_slice());
      Stfs(E(entry).slice_ticks_address(), zero);
      Store(slot, Load(slot) + 1);
      const uint32_t again = M(manager).vehicle_slots_address() + (E(entry).vehicle_index() << 2);
      if (int32_t(Load(again)) > 216000) {
        Store(again, 60);
      }
    } else {
      // Without: +12 counts ticks; once it reaches a slice's ticks, a slice
      // on along the route.
      const uint32_t number = E(entry).record();
      const double ticks_so_far = double(E(entry).slice_ticks());
      const uint32_t race = Load(kRaceData);
      Stfs(E(entry).slice_ticks_address(), Fadds(ticks_so_far, one));
      const Car record(number * Car::kSize + RD(race).racer_records_address());
      JumpSections(entry, frame + 96, frame + 92, frame + 84, frame + 80, frame - 176);
      double ticks = 0.0;
      bool met = false;
      if (four && (E(player).flags() & 0x10) != 0) {
        const uint32_t now = Load(kRoute);
        const uint32_t section = E(entry).section();
        const uint32_t a = Section(player_record).branch();
        const uint32_t b = Section(SectionRecord(now, section)).branch();
        if (BranchesMeet(a, b) != 0) {
          met = true;
          if (int32_t(StatusOf(vehicle).wrong_way()) != 0 ||
              double(va::VehicleDistancePerFrame(vehicle)) == zero) {
            ticks = Fmuls(slice_length, scale);
          } else {
            ticks = Fdivs(slice_length, double(va::VehicleDistancePerFrame(vehicle)));
          }
        }
      }
      if (!met) {
        ticks = Fmuls(slice_length, scale);
        switch (int32_t(Neighbours(manager, frame + 88))) {
          case 1:
            ticks = Fmuls(ticks, ahead);
            break;
          case 2:
            ticks = Fmuls(ticks, behind);
            break;
          case 3:
            ticks = Fmuls(ticks, both);
            break;
          default:
            break;
        }
      }
      if (!(double(E(entry).slice_ticks()) < ticks)) {
        const uint32_t item = record.item_id();
        const uint32_t kind =
            int32_t(item) < 0
                ? 0xFFFFFFFFu
                : uint32_t(int32_t(int8_t(schema::OpponentItem(data_tables::Item(item)).location())));
        vehicle::RoutePositionStep(E(entry).position_address(), 1, S16(kind), frame - 112);
        Stfs(E(entry).slice_ticks_address(), Fsubs(double(E(entry).slice_ticks()), ticks));
      }
    }
    i++;
    const int32_t count = int32_t(M(manager).entry_count());
    Store(frame + 88, i);
    if (!(int32_t(i) < count)) {
      break;
    }
  }
}

// --- The streaming by the race's kind and the tick ---

namespace {

constexpr uint32_t kTrafficScale = 0x820021E8;   // the traffic's radii: the speed x this
constexpr uint32_t kOpponentScale = 0x820021F4;  // the opponents' radii: the speed x this
constexpr uint32_t kNearBase = 0x82002A28;       // the near radii's base
constexpr uint32_t kFarBase = 0x82008914;        // the far radii's base
constexpr uint32_t kFarCap = 0x82003384;         // the far radii's cap
constexpr uint32_t kOpponentNearCap = 0x820099CC;  // the opponents' near radius when capped
constexpr uint32_t kTrafficNearCap = 0x820099C8;   // the traffic's near radius when capped
constexpr uint32_t kRecorded = 0x821743A8;         // sub_821743A8 (never seen in play)
constexpr uint32_t kRacerData = 0x826D82E8;        // the race's progress (a pointer)

// The callees' originals with RaceManager::RefreshVehicles' signature.
Original RefreshOriginal(const StartCalls& calls) {
  return [&calls](uint32_t address, uint32_t sp, uint32_t r3) {
    calls.original(address, sp, r3, 0, 0);
  };
}

// StreamNear, and StreamCruise with `cruise` (MoveUnseen after).
void StreamAround(uint32_t manager, uint32_t sp, bool cruise, bool heap,
                  const MakeCalls& make) {
  const uint32_t frame = sp - 160;
  // The player's entry: the first flagged 0x4, or none (0).
  uint32_t player = 0;
  if (int32_t(M(manager).entry_count()) > 0) {
    for (int32_t i = 0;;) {
      if ((E(M(manager).entries_address() + E::kSize * uint32_t(i)).flags() & 0x4) != 0) {
        player = M(manager).entries_address() + E::kSize * uint32_t(i);
        break;
      }
      i++;
      if (!(i < int32_t(M(manager).entry_count()))) {
        break;
      }
    }
  }
  const uint32_t car = va::VehicleGet(E(player).vehicle_index());
  // The radii, each from Vehicle::Speed again.
  double f31 = Lfs(kTrafficScale);
  double f30 = Lfs(kNearBase);
  double f27 = Fmadds(double(va::VehicleSpeed(car)), f31, f30);
  const double f29 = Lfs(kFarBase);
  double f28 = Fmadds(double(va::VehicleSpeed(car)), f31, f29);
  f31 = Lfs(kOpponentScale);
  f30 = Fmadds(double(va::VehicleSpeed(car)), f31, f30);
  double f2 = Fmadds(double(va::VehicleSpeed(car)), f31, f29);
  const double cap = Lfs(kFarCap);
  if (f2 > cap) {
    f2 = cap;
    f30 = Lfs(kOpponentNearCap);
    if (f28 > f30) {
      f28 = f30;
      f27 = Lfs(kTrafficNearCap);
    }
  }
  M(manager).set_racer_count(0);
  StreamOpponents(manager, player, f30, f2, frame - 1920, heap,
                  make(frame - 1920 - 224).original);
  StreamTraffic(manager, player, f27, f28, 0, frame - 464, heap, make(frame - 464 - 224));
  if (cruise) {
    MoveUnseen(manager, player, frame - 272);
  }
}

}  // namespace

void StreamNear(uint32_t manager, uint32_t sp, bool heap, const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82172E60 RaceManager::StreamNear");
  StreamAround(manager, sp, false, heap, make);
}

void StreamCruise(uint32_t manager, uint32_t sp, bool heap,
                  const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82174200 RaceManager::StreamCruise");
  StreamAround(manager, sp, true, heap, make);
}

void StreamTwoPlayers(uint32_t manager, uint32_t sp, bool heap,
                      const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82172D20 RaceManager::StreamTwoPlayers");
  const uint32_t frame = sp - 128;
  // The one ahead: the second entry when the race data's flags have 0x20
  // or 0x40 and [0x826D82E8]'s +3012 is below its +3984.
  const uint32_t flags = RD(Load(kRaceData)).flags();
  bool second_ahead = false;
  if ((flags & 0x20) != 0 || (flags & 0x40) != 0) {
    const uint32_t data = Load(kRacerData);
    second_ahead = double(schema::RaceProgress(data).e0_route_distance()) <
                   double(schema::RaceProgress(data).e1_route_distance());
  }
  const uint32_t ahead = second_ahead ? M(manager).entries_address() + E::kSize : M(manager).entries_address();
  const uint32_t other = second_ahead ? M(manager).entries_address() : M(manager).entries_address() + E::kSize;
  const double f31 = double(va::VehicleSpeed(va::VehicleGet(E(ahead).vehicle_index())));
  double f13 = double(va::VehicleSpeed(va::VehicleGet(E(other).vehicle_index())));
  if (!(f31 < f13)) {
    f13 = f31;
  }
  double f0 = Fmuls(f13, Lfs(kOpponentScale));
  f13 = Fmuls(f13, Lfs(kTrafficScale));
  const double far_base = Lfs(kFarBase);
  double f2 = Fadds(f0, far_base);
  const double near_base = Lfs(kNearBase);
  double f1 = Fadds(f0, near_base);
  double f30 = Fadds(f13, near_base);
  double f31b = Fadds(f13, far_base);
  f0 = Lfs(kFarCap);
  if (f2 > f0) {
    f2 = f0;
    f1 = Lfs(kOpponentNearCap);
    if (f31b > f1) {
      f31b = f1;
      f30 = Lfs(kTrafficNearCap);
    }
  }
  M(manager).set_racer_count(0);
  StreamOpponents(manager, ahead, f1, f2, frame - 1920, heap,
                  make(frame - 1920 - 224).original);
  StreamTraffic(manager, ahead, f30, f31b, other, frame - 464, heap,
                make(frame - 464 - 224));
}

void Stream(uint32_t manager, uint32_t sp, bool heap, const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82174338 RaceManager::Stream");
  const uint32_t kind = RD(Load(kRaceData)).race_mode();
  switch (kind) {
    case 2:
    case 3:
      StreamCruise(manager, sp, heap, make);
      return;
    case 5:
      StreamTwoPlayers(manager, sp, heap, make);
      return;
    case 0:  // through sub_8217437C, a tail call
    case 1:
    case 4:
    case 6:
    case 7:
    case 8:
      StreamNear(manager, sp, heap, make);
      return;
    default:
      return;
  }
}

void Tick(uint32_t world, uint32_t frame, bool heap, const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82174BC0 RaceWorld::Tick");
  const StartCalls calls = make(frame - 224);
  if (schema::RaceWorld(world).two_humans() != 0) {
    const uint32_t race = Load(kRaceData);
    const uint32_t start = G::start_byte();
    if ((RD(race).flags() & 0x20) == 0 || start == 255) {
      // sub_821743A8 (never seen in play): its original.
      calls.original(kRecorded, frame, world, 0, 0);
    } else {
      const uint32_t data = Load(kRacerData);
      // The flags a record needs: 0x1 or 0x80 by the byte (and the word at
      // 0x826C7824), or either.
      uint32_t want = 0;
      const bool either = start != 0 && start != 1;
      if (start == 0) {
        want = G::human_flag() != 0 ? 0x1 : 0x80;
      } else if (start == 1) {
        want = G::human_flag() != 0 ? 0x80 : 0x1;
      }
      const uint32_t records = RD(race).racer_records_address();
      const uint32_t count = RD(race).car_count_address();
      if (int32_t(Load(count)) > 0) {
        for (int32_t i = 0;;) {
          const uint32_t flags = Car(records + Car::kSize * uint32_t(i)).flags();
          const bool match =
              either ? ((flags & 0x1) != 0 || (flags & 0x80) != 0) : (flags & want) == want;
          if (match) {
            const uint32_t entry = EntryOfRecord(uint32_t(i));
            // Its results entry's +784 (972 bytes each).
            Store(E(entry).record() * 972 + schema::RaceProgress(data).e0_sp_address(),
                  0);
          }
          i++;
          if (!(i < int32_t(Load(count)))) {
            break;
          }
        }
      }
    }
  }
  Stream(G::racers_manager().address, frame, heap, make);
  calls.update_drivers(frame, 0x82174D04);
  TickVehicles(frame - 128, calls.tick);
  track_collision::TrackCollisionTick(frame - 144, calls.collision);
  RefreshVehicles(frame - 112, RefreshOriginal(calls));
}

}  // namespace race_world
