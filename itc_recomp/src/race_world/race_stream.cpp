// The racers' manager's streaming each tick (docs/functions/race-world.md):
// RaceManager::StreamOpponents, RaceManager::StreamTraffic,
// RaceManager::MoveUnseen, the streaming by the race's kind above them
// (RaceManager::Stream, StreamNear, StreamCruise, StreamTwoPlayers) and
// RaceWorld::Tick. See race_world.h.

#include "race_world/race_world.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "data_tables/data_tables.h"
#include "guest.h"
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
constexpr uint32_t kFlagsOffset = 0x59AE4;     // the race data's flags
constexpr uint32_t kSpacingOffset = 0xA3AE0;   // the race data's traffic numbers' base
constexpr uint32_t kSlowFactor = 0x820027A8;   // 1.5: the km/h scaled against the speed below
constexpr uint32_t kTrafficSpeed = 0x82009974;  // the traffic's speed placed (and its limit)
constexpr uint32_t kTooNear = 0x82002568;      // 20.0: a racer this near the spot refuses it
constexpr uint32_t kTrafficStep = 0x826193C8;  // the physics step's time on a new traffic car

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// A halfword sign-extended to a register (extsh, lha).
uint32_t S16(uint32_t value) {
  return uint32_t(int32_t(int16_t(uint16_t(value))));
}
uint32_t LoadS16(uint8_t* base, uint32_t address) {
  return S16(guest::LoadU16(base, address));
}
// A section's record in the route's table.
uint32_t SectionRecord(uint8_t* base, uint32_t file, uint32_t section) {
  return Load(base, Load(base, file + 12) + (section << 2));
}

// An entry loses its vehicle (when it has one and its slot word is above
// 0): flags &= ~0x40, Vehicle::RemoveFromWorld (its original without
// `heap` or where its plain C++ refuses, with r1 = `frame`), the slot word
// 0 and +20 = -1.
void Lose(uint8_t* base, uint32_t manager, uint32_t entry, uint32_t frame, bool heap,
          const OriginalCall& original) {
  const uint32_t which = Load(base, entry + 20);
  if (int32_t(which) == -1 || int32_t(Load(base, manager + ((which + 584) << 2))) <= 0) {
    return;
  }
  Store(base, entry, Load(base, entry) & ~0x40u);
  const uint32_t car = va::VehicleGet(base, which);
  uint32_t removed = 0;
  if (!heap || !vehicle::VehicleRemoveFromWorld(base, car, &removed)) {
    original(kRemoveFromWorld, frame, car, 0, 0);
  }
  Store(base, manager + ((Load(base, entry + 20) + 584) << 2), 0);
  Store(base, entry + 20, 0xFFFFFFFFu);
}

// An entry with a vehicle put in the racers' list (+2308): +2364 and
// `counter` (+2372 the opponents', +2376 the traffic's) count.
void List(uint8_t* base, uint32_t manager, uint32_t entry, uint32_t counter = 2372) {
  Store(base, manager + ((Load(base, manager + 2364) + 577) << 2), entry);
  const uint32_t listed = Load(base, manager + 2364);
  const uint32_t racers = Load(base, manager + counter);
  Store(base, manager + 2364, listed + 1);
  Store(base, manager + counter, racers + 1);
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
void RemoveFromWorld(uint8_t* base, uint32_t car, uint32_t frame, bool heap,
                     const OriginalCall& original) {
  uint32_t removed = 0;
  if (!heap || !vehicle::VehicleRemoveFromWorld(base, car, &removed)) {
    original(kRemoveFromWorld, frame, car, 0, 0);
  }
}

}  // namespace

void StreamOpponents(uint8_t* base, uint32_t manager, uint32_t player, double spawn,
                     double despawn, uint32_t frame, bool heap, const OriginalCall& original) {
  ITC_NATIVE_RUNNING("sub_82172118 RaceManager::StreamOpponents");
  // The player's route position (its vehicle's identity +4) at + 80, its
  // section's record.
  const uint32_t identity = va::VehicleIdentity(va::VehicleGet(base, Load(base, player + 20)));
  const uint32_t position = Load(base, identity + 4);
  const int32_t count = int32_t(Load(base, manager + 2368));
  const uint32_t file = Load(base, kRoute);
  Store(base, frame + 80, position);
  const uint32_t player_section = guest::LoadU16(base, frame + 80);
  const uint32_t player_record = SectionRecord(base, file, player_section);
  // The sorted list at + 176 emptied (+1776 to +1784).
  Store(base, manager + 2372, 0);
  Store(base, frame + 1780, 0);
  Store(base, frame + 1784, 0);
  Store(base, frame + 1776, 0);
  const uint32_t list = frame + 176;
  if (count > 0) {
    const double one = Lfs(base, kOne);
    const double far_key = Lfs(base, kFarKey);
    const double zero = Lfs(base, kZero);
    for (int32_t i = 0;;) {
      const uint32_t entry = manager + 4 + 28 * uint32_t(i);
      const uint32_t flags = Load(base, entry);
      bool lose = (flags & 0x20) != 0 || (flags & 0x80) != 0;
      double key = 0.0;
      if (lose) {
      } else if ((flags & 0x4) != 0 || (flags & 0x8) != 0) {
        key = zero;
      } else if ((flags & 0x10) != 0 || (flags & 0x400) != 0) {
        key = far_key;
      } else {
        const uint32_t word = Load(base, entry + 8);
        const uint32_t now = Load(base, kRoute);
        const uint32_t a = guest::LoadU8(base, player_record + 3);
        Store(base, frame + 84, word);
        const uint32_t section = guest::LoadU16(base, frame + 84);
        const uint32_t b = guest::LoadU8(base, SectionRecord(base, now, section) + 3);
        if (BranchesMeet(a, b) == 0) {
          lose = true;
        } else {
          // Both slices' centers: the entry's at + 128, the player's at +
          // 112; the difference at + 96, its y = 0.0 and w = 1.0; its
          // length + 1.0.
          track_route::SlicePoint(base, frame + 128, 0, S16(section), LoadS16(base, frame + 86));
          track_route::SlicePoint(base, frame + 112, 0, S16(player_section),
                                  LoadS16(base, frame + 82));
          vl::Vec3Sub(base, frame + 96, frame + 128, frame + 112);
          Stfs(base, frame + 100, zero);
          Stfs(base, frame + 108, one);
          key = Fadds(double(vl::Vec3Normalize(base, frame + 96, frame + 96)), one);
        }
      }
          if (lose) {
        Lose(base, manager, entry, frame, heap, original);
      } else {
        SortedListInsert(base, list, entry, key);
      }
      i++;
      if (!(i < int32_t(Load(base, manager + 2368)))) {
        break;
      }
    }
  }
  // Past the fourth, the vehicle removed.
  uint32_t n = 0;
  for (uint32_t node = Load(base, frame + 1776); node != 0; node = Load(base, node + 12), n++) {
    const uint32_t entry = Load(base, node);
    if (n >= 4) {
      Lose(base, manager, entry, frame, heap, original);
    }
  }
  // Up to four: nearer than `spawn` given a vehicle (or keeping theirs),
  // beyond `despawn` losing theirs, between keeping theirs.
  for (uint32_t node = Load(base, frame + 1776); node != 0; node = Load(base, node + 12)) {
    const uint32_t entry = Load(base, node);
    if (Load(base, manager + 2372) >= 4) {
      Lose(base, manager, entry, frame, heap, original);
      continue;
    }
    const double key = Lfs(base, node + 4);
    if (key < spawn) {
      if (int32_t(Load(base, entry + 20)) != -1) {
        List(base, manager, entry);
        continue;
      }
      // RaceManager::GiveVehicle (never seen in play): its original.
      const uint64_t given = original(kGiveVehicle, frame, manager, entry, 0);
      if (uint32_t(given) == 0) {
        // No free slot: the empty sub_82106540 with (500, 220, 0,
        // 0x820099BC), probably a debug print compiled out.
        continue;
      }
      List(base, manager, entry);
      ai_drivers::ClearCarInput(base, frame + 144);
      const uint32_t to = Load(base, kInputs + ((Load(base, entry + 20) << 2) & 0x3FC));
      for (uint32_t w = 0; w < 5; w++) {
        Store(base, to + 4 * w, Load(base, frame + 144 + 4 * w));
      }
      const uint32_t number = Load(base, entry + 24);
      const uint32_t id = number * 3936 + Load(base, kRaceData) + 3960;
      Store(base, frame + 80, 1);
      racer_controllers::Setup(base, entry + 20, id, frame + 80, frame);
    } else if (key > despawn) {
      Lose(base, manager, entry, frame, heap, original);
    } else if (int32_t(Load(base, entry + 20)) != -1) {
      List(base, manager, entry);
    }
  }
}

void StreamTraffic(uint8_t* base, uint32_t manager, uint32_t player, double inner, double outer,
                   uint32_t second, uint32_t frame, bool heap, const StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821724F8 RaceManager::StreamTraffic");
  // The player's route position (its vehicle's identity +4) at + 84, its
  // body's position at + 160, its velocity (+6176) at + 176; the position
  // again at + 80, the second player's there when there's one.
  const uint32_t car = va::VehicleGet(base, Load(base, player + 20));
  const uint32_t position = Load(base, va::VehicleIdentity(car) + 4);
  Store(base, frame + 84, position);
  const uint32_t at = va::RigidBodyPosition(Load(base, car + 336));
  for (uint32_t w = 0; w < 4; w++) {
    Store(base, frame + 160 + 4 * w, Load(base, at + 4 * w));
  }
  for (uint32_t w = 0; w < 4; w++) {
    Store(base, frame + 176 + 4 * w, Load(base, car + 6176 + 4 * w));
  }
  // Vehicle::Get of the player's +20 again and RigidBody::Position, their
  // answers unused.
  Store(base, frame + 80, Load(base, va::VehicleIdentity(car) + 4));
  const double slice_length = Lfs(base, Load(base, kRoute) + 8);
  if (second != 0) {
    const uint32_t other = va::VehicleGet(base, Load(base, second + 20));
    Store(base, frame + 80, Load(base, va::VehicleIdentity(other) + 4));
  }
  Store(base, manager + 2376, 0);
  // With the race data's flags 0x4000: every traffic car (the entries at
  // +2160) loses its vehicle.
  if ((Load(base, Load(base, kRaceData) + kFlagsOffset) & 0x4000) != 0) {
    for (uint32_t t = 0; t < 3; t++) {
      const uint32_t entry = manager + 2160 + 28 * t;
      if (Load(base, entry) == 0) {
        continue;
      }
      RemoveFromWorld(base, va::VehicleGet(base, Load(base, entry + 20)), frame, heap,
                      calls.original);
      const uint32_t which = Load(base, entry + 20);
      Store(base, entry, 0);
      Store(base, manager + ((which + 584) << 2), 0);
      Store(base, entry + 20, 0xFFFFFFFFu);
    }
    return;
  }
  // The players' slices' centers: the player's at + 208, the second's at +
  // 192.
  track_route::SlicePoint(base, frame + 208, 0, LoadS16(base, frame + 84),
                          LoadS16(base, frame + 86));
  if (second != 0) {
    track_route::SlicePoint(base, frame + 192, 0, LoadS16(base, frame + 80),
                            LoadS16(base, frame + 82));
  }
  // Each traffic entry with a vehicle: its slot word counts its ticks (to
  // 216,000, then 60); farther than `outer` from both players (each flat
  // distance from its slice's center, at + 144, the difference at + 96), it
  // loses its vehicle when its slot word is above 0; else it's listed.
  const double zero = Lfs(base, kZero);
  for (uint32_t t = 0; t < 3; t++) {
    const uint32_t entry = manager + 2160 + 28 * t;
    if (Load(base, entry) == 0) {
      continue;
    }
    const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
    const uint32_t slot = manager + ((Load(base, entry + 20) + 584) << 2);
    Store(base, slot, Load(base, slot) + 1);
    const uint32_t again = manager + ((Load(base, entry + 20) + 584) << 2);
    if (int32_t(Load(base, again)) > 216000) {
      Store(base, again, 60);
    }
    Store(base, frame + 84, Load(base, va::VehicleIdentity(vehicle) + 4));
    track_route::SlicePoint(base, frame + 144, 0, LoadS16(base, frame + 84),
                            LoadS16(base, frame + 86));
    vl::Vec3Sub(base, frame + 96, frame + 144, frame + 208);
    Stfs(base, frame + 100, zero);
    const double first = double(vl::Vec3Normalize(base, frame + 96, frame + 96));
    double other = first;
    if (second != 0) {
      vl::Vec3Sub(base, frame + 96, frame + 144, frame + 192);
      Stfs(base, frame + 100, zero);
      other = double(vl::Vec3Normalize(base, frame + 96, frame + 96));
    }
    if (first > outer && other > outer &&
        int32_t(Load(base, manager + ((Load(base, entry + 20) + 584) << 2))) > 0) {
      RemoveFromWorld(base, vehicle, frame, heap, calls.original);
      const uint32_t which = Load(base, entry + 20);
      Store(base, entry, 0);
      Store(base, manager + ((which + 584) << 2), 0);
      Store(base, entry + 20, 0xFFFFFFFFu);
    } else {
      // (The original also counts the entries flagged 0x800, for the
      // neighbor search below, which never runs.)
      List(base, manager, entry, 2376);
    }
  }
  if ((Load(base, Load(base, kRaceData) + kFlagsOffset) & 0x2000) != 0) {
    return;
  }
  // A new traffic car every 90 ticks at most (+2388 counts them), while
  // there are fewer than +2380.
  if (!(int32_t(Load(base, manager + 2376)) < int32_t(Load(base, manager + 2380))) ||
      Load(base, manager + 2388) < 90) {
    const uint32_t ticks = Load(base, manager + 2388);
    if (ticks < 90) {
      Store(base, manager + 2388, ticks + 1);
    }
    return;
  }
  Store(base, manager + 2388, 0);
  // The first free traffic entry (+0 0) with a free vehicle slot (its word
  // 0 or -1; the slots scanned for each free entry).
  uint32_t t = 0, which = 0xFFFFFFFFu;
  for (;; t++) {
    if (t >= 3) {
      return;
    }
    if (Load(base, manager + 2160 + 28 * t) != 0) {
      continue;
    }
    for (uint32_t k = 0; k < 7; k++) {
      const uint32_t word = Load(base, manager + 2336 + 4 * k);
      if (word == 0 || word == 0xFFFFFFFFu) {
        which = k;
        break;
      }
    }
    if (which != 0xFFFFFFFFu) {
      break;
    }
  }
  const uint32_t entry = manager + 2160 + 28 * t;
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
  guest::StoreU16(base, frame + 88, 0xFFFF);
  guest::StoreU16(base, frame + 90, 0xFFFF);
  const uint32_t flag6168 = Load(base, car + 6168);
  Store(base, frame + 88, position);
  bool less = false;  // the steps 1 - f13
  double f13 = 0.0;
  if (flag6168 != 0) {
    if (second != 0) {
      Store(base, frame + 88, Load(base, frame + 80));
    }
    f13 = Fdivs(outer, slice_length);
    less = true;
  } else if ((Load(base, Load(base, kRaceData) + kFlagsOffset) & 0x1000) == 0) {
    const double kmh = Fmuls(double(Vehicle_GetSpeedKmh(base, car)), Lfs(base, kSlowFactor));
    if (kmh < Lfs(base, kTrafficSpeed)) {
      if (second != 0) {
        Store(base, frame + 88, Load(base, frame + 80));
      }
      f13 = Fdivs(inner, slice_length);
      less = true;
    }
  }
  if (less) {
    Store(base, frame + 80, FctiwzWord(Fsubs(Lfs(base, kOne), f13)));
  } else {
    Store(base, frame + 80, FctiwzWord(Fdivs(inner, slice_length)));
  }
  StepToMainRoad(base, frame + 88, LoadS16(base, frame + 82), 0xFFFFFFFFu, frame - 112);
  // The spot's center at + 128; refused (nothing more) when a listed racer's
  // body ([0x826D82F4]'s list, its count read again each pass; the position
  // at + 96, its y 0.0) is nearer than [0x82002568] (Vec3::Direction into +
  // 256).
  track_route::SlicePoint(base, frame + 128, 0, LoadS16(base, frame + 88),
                          LoadS16(base, frame + 90));
  Stfs(base, frame + 132, zero);
  {
    uint32_t global = Load(base, kManager);
    if (int32_t(Load(base, global + 2364)) > 0) {
      const double too_near = Lfs(base, kTooNear);
      for (int32_t i = 0;;) {
        const uint32_t listed = Load(base, global + 2308 + 4 * uint32_t(i));
        const uint32_t body = Load(base, va::VehicleGet(base, Load(base, listed + 20)) + 336);
        const uint32_t point = va::RigidBodyPosition(body);
        for (uint32_t w = 0; w < 4; w++) {
          Store(base, frame + 96 + 4 * w, Load(base, point + 4 * w));
        }
        Stfs(base, frame + 100, zero);
        if (double(vl::Vec3Direction(base, frame + 256, frame + 128, frame + 96)) < too_near) {
          return;
        }
        global = Load(base, kManager);
        i++;
        if (!(i < int32_t(Load(base, global + 2364)))) {
          break;
        }
      }
    }
  }
  // The entry: its model (Traffic::PickModel of the race data's +0xA3AE0 +
  // its +16 + its index, and the spot's branch), +0 = 578, +24 the model's
  // traffic record (+2244), +16 the number (less 42 while above 42); its
  // lane by the player's car's +6224 (0 below 0.0, else 1).
  const uint32_t file = Load(base, kRoute);
  const uint32_t branch =
      guest::LoadU8(base, SectionRecord(base, file, guest::LoadU16(base, frame + 88)) + 3);
  const uint32_t number =
      Load(base, Load(base, kRaceData) + kSpacingOffset) + Load(base, entry + 16) + t;
  const uint32_t model = uint32_t(PickModel(base, number, branch, frame));
  const uint32_t record = Load(base, manager + ((model + 561) << 2));
  Store(base, entry + 4, model);
  Store(base, entry + 0, 578);
  Store(base, entry + 24, record);
  const uint32_t lane = Lfs(base, car + 6224) < zero ? 0 : 1;
  Store(base, entry + 16, number);
  for (int32_t n = int32_t(number); n > 42;) {
    n = int32_t(Load(base, entry + 16)) - 42;
    Store(base, entry + 16, uint32_t(n));
  }
  // Its vehicle: Vehicle::SetUp (slot word -1) or ResetForCar with its
  // record (the race data + 48 + 3,936 x +24), the slot 1,
  // Vehicle::PlaceOnRoute on the spot ([0x82009974] the speed, 0.0 across,
  // the lane, 1), an input record cleared at + 224 and copied to the
  // racer's, VehicleStatus's speed record (+6160), one physics step
  // ([0x826193C8]) and VehicleStatus::Update(+6160, it, 0, 1); its driver
  // (AiDriver::Setup: -1 at + 80, 1 at + 84); listed.
  Store(base, entry + 20, which);
  const uint32_t given = va::VehicleGet(base, which);
  const uint32_t slot = manager + ((which + 584) << 2);
  const uint32_t car_record = Load(base, kRaceData) + Load(base, entry + 24) * 3936 + 48;
  double f1 = 0.0;
  vehicle::VehicleSetUp(base, given, car_record, frame - 224, int32_t(Load(base, slot)) != -1,
                        heap, calls.setup_original, &f1);
  Store(base, slot, 1);
  {
    const double speed = Lfs(base, kTrafficSpeed);
    if (!vehicle::VehiclePlaceOnRoute(base, given, frame + 88, speed, zero, int32_t(lane), 1,
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
  ai_drivers::ClearCarInput(base, frame + 224);
  {
    const uint32_t to = Load(base, kInputs + ((Load(base, entry + 20) << 2) & 0x3FC));
    for (uint32_t w = 0; w < 5; w++) {
      Store(base, to + 4 * w, Load(base, frame + 224 + 4 * w));
    }
  }
  const uint32_t status = given + 6160;
  va::VehicleSpeedRecord(base, status, given);
  {
    const double dt = Lfs(base, kTrafficStep);
    if (car_physics::CollisionWorldCallbackKnown(base)) {
      car_physics::VehiclePhysicsStep(base, given, dt, frame - 112, [&](uint32_t address) {
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
  if (!vehicle::VehicleStatusUpdate(base, status, given, 0, 1, frame - 384)) {
    vehicle::GuestRegisters r;
    r.r3 = status;
    r.r4 = given;
    r.r5 = 0;
    r.r6 = 1;
    calls.tick(0x820C23E8, frame, r);
  }
  Store(base, frame + 84, 1);
  Store(base, frame + 80, 0xFFFFFFFFu);
  racer_controllers::Setup(base, entry + 20, frame + 80, frame + 84, frame);
  List(base, manager, entry, 2376);
}

void MoveUnseen(uint8_t* base, uint32_t manager, uint32_t player, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82173E28 RaceManager::MoveUnseen");
  const uint32_t vehicle = va::VehicleGet(base, Load(base, player + 20));
  const uint32_t identity = va::VehicleIdentity(vehicle);
  const uint32_t body = Load(base, vehicle + 336);
  Store(base, frame + 88, Load(base, identity + 4));
  const uint32_t at = va::RigidBodyPosition(body);
  const uint32_t racers = Load(base, manager + 2372);
  const uint32_t player_section = guest::LoadU16(base, frame + 88);
  const uint32_t race_flags = Load(base, Load(base, kRaceData) + 20);
  const uint32_t file = Load(base, kRoute);
  const uint32_t table = Load(base, file + 12);
  const double slice_length = Lfs(base, file + 8);
  uint32_t flag20 = (race_flags >> 5) & 1;
  const uint32_t player_record = Load(base, (player_section << 2) + table);
  const bool four = racers == 4;
  uint32_t hi[2] = {Load(base, at + 8), Load(base, at + 12)};
  Store(base, frame + 80, flag20);
  Store(base, frame + 92, guest::LoadU8(base, player_record + 3));
  uint32_t lo[2] = {Load(base, at + 0), Load(base, at + 4)};
  uint32_t flag10 = (race_flags >> 4) & 1;
  // The player's position (RigidBody::Position) at + 96.
  Store(base, frame + 96, lo[0]);
  Store(base, frame + 100, lo[1]);
  Store(base, frame + 104, hi[0]);
  Store(base, frame + 108, hi[1]);
  Store(base, frame + 84, flag10);
  // With the race data's flags 0x10 or 0x20: whether an entry of item kind
  // 2 (sections 200 to 203, 258 to 261; clears + 84) or 3 (187, 188, 193,
  // 194, 251 to 253; clears + 80) is already there.
  if ((flag10 != 0 || flag20 != 0) && int32_t(Load(base, manager + 2368)) > 0) {
    for (int32_t i = 0;;) {
      const uint32_t entry = manager + 4 + 28 * uint32_t(i);
      const uint32_t number = Load(base, entry + 24);
      const uint32_t race = Load(base, kRaceData);
      const int32_t id = int32_t(S16(Load(base, number * 3936 + race + 3960)));
      if (id >= 0 && (Load(base, entry) & 0xAC) == 0) {
        const int32_t s = int32_t(LoadS16(base, entry + 8));
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
            guest::LoadU8(base, data_tables::Item(base, uint32_t(id)) + 36) == 3) {
          flag20 = 0;
          Store(base, frame + 80, flag20);
        } else if (which == 2 && flag10 != 0 &&
                   guest::LoadU8(base, data_tables::Item(base, uint32_t(id)) + 36) == 2) {
          flag10 = 0;
          Store(base, frame + 84, flag10);
        }
      }
      if (flag10 == 0 && flag20 == 0) {
        break;
      }
      i++;
      if (!(i < int32_t(Load(base, manager + 2368)))) {
        break;
      }
    }
  }
  // Each entry not flagged 0x20 or 0x80 (its index at + 88).
  Store(base, frame + 88, 0);
  if (!(int32_t(Load(base, manager + 2368)) > 0)) {
    return;
  }
  const double ahead = Lfs(base, kAhead);
  const double behind = Lfs(base, kBehind);
  const double both = Lfs(base, kBoth);
  const double scale = Lfs(base, kTicksScale);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0;;) {
    const uint32_t entry = manager + 4 + 28 * i;
    const uint32_t flags = Load(base, entry);
    if ((flags & 0x20) != 0 || (flags & 0x80) != 0) {
    } else if ((flags & 0x40) != 0) {
      // With a vehicle: +8 its track position, +12 0.0, its slot word
      // counting its ticks (to 216,000, then 60).
      const uint32_t own = va::VehicleIdentity(va::VehicleGet(base, Load(base, entry + 20)));
      const uint32_t slot = manager + ((Load(base, entry + 20) + 584) << 2);
      guest::StoreU16(base, entry + 8, guest::LoadU16(base, own + 4));
      guest::StoreU16(base, entry + 10, guest::LoadU16(base, own + 6));
      Stfs(base, entry + 12, zero);
      Store(base, slot, Load(base, slot) + 1);
      const uint32_t again = manager + ((Load(base, entry + 20) + 584) << 2);
      if (int32_t(Load(base, again)) > 216000) {
        Store(base, again, 60);
      }
    } else {
      // Without: +12 counts ticks; once it reaches a slice's ticks, a slice
      // on along the route.
      const uint32_t number = Load(base, entry + 24);
      const double ticks_so_far = Lfs(base, entry + 12);
      const uint32_t race = Load(base, kRaceData);
      Stfs(base, entry + 12, Fadds(ticks_so_far, one));
      const uint32_t record = number * 3936 + race;
      JumpSections(base, entry, frame + 96, frame + 92, frame + 84, frame + 80, frame - 176);
      double ticks = 0.0;
      bool met = false;
      if (four && (Load(base, player) & 0x10) != 0) {
        const uint32_t now = Load(base, kRoute);
        const uint32_t section = guest::LoadU16(base, entry + 8);
        const uint32_t a = guest::LoadU8(base, player_record + 3);
        const uint32_t b = guest::LoadU8(base, SectionRecord(base, now, section) + 3);
        if (BranchesMeet(a, b) != 0) {
          met = true;
          if (int32_t(Load(base, vehicle + 6168)) != 0 ||
              double(va::VehicleDistancePerFrame(base, vehicle)) == zero) {
            ticks = Fmuls(slice_length, scale);
          } else {
            ticks = Fdivs(slice_length, double(va::VehicleDistancePerFrame(base, vehicle)));
          }
        }
      }
      if (!met) {
        ticks = Fmuls(slice_length, scale);
        switch (int32_t(Neighbours(base, manager, frame + 88))) {
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
      if (!(Lfs(base, entry + 12) < ticks)) {
        const uint32_t item = Load(base, record + 3960);
        const uint32_t kind =
            int32_t(item) < 0
                ? 0xFFFFFFFFu
                : uint32_t(int32_t(int8_t(guest::LoadU8(base, data_tables::Item(base, item) + 36))));
        vehicle::RoutePositionStep(base, entry + 8, 1, S16(kind), frame - 112);
        Stfs(base, entry + 12, Fsubs(Lfs(base, entry + 12), ticks));
      }
    }
    i++;
    const int32_t count = int32_t(Load(base, manager + 2368));
    Store(base, frame + 88, i);
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
constexpr uint32_t kKindOffset = 0xA3AF4;          // the race data's kind
constexpr uint32_t kRecorded = 0x821743A8;         // sub_821743A8 (never seen in play)
constexpr uint32_t kStart = 0x826C6B0C;            // a byte: which racers' words are cleared
constexpr uint32_t kStartObject = 0x826C7824;      // a word: picks 0x1 or 0x80 by the byte
constexpr uint32_t kRacerData = 0x826D82E8;        // 972-byte records from +2972 (a pointer)

// The callees' originals with RaceManager::RefreshVehicles' signature.
Original RefreshOriginal(const StartCalls& calls) {
  return [&calls](uint32_t address, uint32_t sp, uint32_t r3) {
    calls.original(address, sp, r3, 0, 0);
  };
}

// StreamNear, and StreamCruise with `cruise` (MoveUnseen after).
void StreamAround(uint8_t* base, uint32_t manager, uint32_t sp, bool cruise, bool heap,
                  const MakeCalls& make) {
  const uint32_t frame = sp - 160;
  // The player's entry: the first flagged 0x4, or none (0).
  uint32_t player = 0;
  if (int32_t(Load(base, manager + 2368)) > 0) {
    for (int32_t i = 0;;) {
      if ((Load(base, manager + 4 + 28 * uint32_t(i)) & 0x4) != 0) {
        player = manager + 4 + 28 * uint32_t(i);
        break;
      }
      i++;
      if (!(i < int32_t(Load(base, manager + 2368)))) {
        break;
      }
    }
  }
  const uint32_t car = va::VehicleGet(base, Load(base, player + 20));
  // The radii, each from Vehicle::Speed again.
  double f31 = Lfs(base, kTrafficScale);
  double f30 = Lfs(base, kNearBase);
  double f27 = Fmadds(double(va::VehicleSpeed(base, car)), f31, f30);
  const double f29 = Lfs(base, kFarBase);
  double f28 = Fmadds(double(va::VehicleSpeed(base, car)), f31, f29);
  f31 = Lfs(base, kOpponentScale);
  f30 = Fmadds(double(va::VehicleSpeed(base, car)), f31, f30);
  double f2 = Fmadds(double(va::VehicleSpeed(base, car)), f31, f29);
  const double cap = Lfs(base, kFarCap);
  if (f2 > cap) {
    f2 = cap;
    f30 = Lfs(base, kOpponentNearCap);
    if (f28 > f30) {
      f28 = f30;
      f27 = Lfs(base, kTrafficNearCap);
    }
  }
  Store(base, manager + 2364, 0);
  StreamOpponents(base, manager, player, f30, f2, frame - 1920, heap,
                  make(frame - 1920 - 224).original);
  StreamTraffic(base, manager, player, f27, f28, 0, frame - 464, heap, make(frame - 464 - 224));
  if (cruise) {
    MoveUnseen(base, manager, player, frame - 272);
  }
}

}  // namespace

void StreamNear(uint8_t* base, uint32_t manager, uint32_t sp, bool heap, const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82172E60 RaceManager::StreamNear");
  StreamAround(base, manager, sp, false, heap, make);
}

void StreamCruise(uint8_t* base, uint32_t manager, uint32_t sp, bool heap,
                  const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82174200 RaceManager::StreamCruise");
  StreamAround(base, manager, sp, true, heap, make);
}

void StreamTwoPlayers(uint8_t* base, uint32_t manager, uint32_t sp, bool heap,
                      const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82172D20 RaceManager::StreamTwoPlayers");
  const uint32_t frame = sp - 128;
  // The one ahead: the second entry when the race data's flags have 0x20
  // or 0x40 and [0x826D82E8]'s +3012 is below its +3984.
  const uint32_t flags = Load(base, Load(base, kRaceData) + kFlagsOffset);
  bool second_ahead = false;
  if ((flags & 0x20) != 0 || (flags & 0x40) != 0) {
    const uint32_t data = Load(base, kRacerData);
    second_ahead = Lfs(base, data + 3012) < Lfs(base, data + 3984);
  }
  const uint32_t ahead = second_ahead ? manager + 32 : manager + 4;
  const uint32_t other = second_ahead ? manager + 4 : manager + 32;
  const double f31 = double(va::VehicleSpeed(base, va::VehicleGet(base, Load(base, ahead + 20))));
  double f13 = double(va::VehicleSpeed(base, va::VehicleGet(base, Load(base, other + 20))));
  if (!(f31 < f13)) {
    f13 = f31;
  }
  double f0 = Fmuls(f13, Lfs(base, kOpponentScale));
  f13 = Fmuls(f13, Lfs(base, kTrafficScale));
  const double far_base = Lfs(base, kFarBase);
  double f2 = Fadds(f0, far_base);
  const double near_base = Lfs(base, kNearBase);
  double f1 = Fadds(f0, near_base);
  double f30 = Fadds(f13, near_base);
  double f31b = Fadds(f13, far_base);
  f0 = Lfs(base, kFarCap);
  if (f2 > f0) {
    f2 = f0;
    f1 = Lfs(base, kOpponentNearCap);
    if (f31b > f1) {
      f31b = f1;
      f30 = Lfs(base, kTrafficNearCap);
    }
  }
  Store(base, manager + 2364, 0);
  StreamOpponents(base, manager, ahead, f1, f2, frame - 1920, heap,
                  make(frame - 1920 - 224).original);
  StreamTraffic(base, manager, ahead, f30, f31b, other, frame - 464, heap,
                make(frame - 464 - 224));
}

void Stream(uint8_t* base, uint32_t manager, uint32_t sp, bool heap, const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82174338 RaceManager::Stream");
  const uint32_t kind = Load(base, Load(base, kRaceData) + kKindOffset);
  switch (kind) {
    case 2:
    case 3:
      StreamCruise(base, manager, sp, heap, make);
      return;
    case 5:
      StreamTwoPlayers(base, manager, sp, heap, make);
      return;
    case 0:  // through sub_8217437C, a tail call
    case 1:
    case 4:
    case 6:
    case 7:
    case 8:
      StreamNear(base, manager, sp, heap, make);
      return;
    default:
      return;
  }
}

void Tick(uint8_t* base, uint32_t world, uint32_t frame, bool heap, const MakeCalls& make) {
  ITC_NATIVE_RUNNING("sub_82174BC0 RaceWorld::Tick");
  const StartCalls calls = make(frame - 224);
  if (Load(base, world + 9608) != 0) {
    const uint32_t race = Load(base, kRaceData);
    const uint32_t start = guest::LoadU8(base, kStart);
    if ((Load(base, race + kFlagsOffset) & 0x20) == 0 || start == 255) {
      // sub_821743A8 (never seen in play): its original.
      calls.original(kRecorded, frame, world, 0, 0);
    } else {
      const uint32_t data = Load(base, kRacerData);
      // The flags a record needs: 0x1 or 0x80 by the byte (and the word at
      // 0x826C7824), or either.
      uint32_t want = 0;
      const bool either = start != 0 && start != 1;
      if (start == 0) {
        want = Load(base, kStartObject) != 0 ? 0x1 : 0x80;
      } else if (start == 1) {
        want = Load(base, kStartObject) != 0 ? 0x80 : 0x1;
      }
      const uint32_t records = race + 48;
      const uint32_t count = records + 0x595E0;
      if (int32_t(Load(base, count)) > 0) {
        for (int32_t i = 0;;) {
          const uint32_t flags = Load(base, records + 3888 + 3936 * uint32_t(i));
          const bool match =
              either ? ((flags & 0x1) != 0 || (flags & 0x80) != 0) : (flags & want) == want;
          if (match) {
            const uint32_t entry = EntryOfRecord(base, uint32_t(i));
            Store(base, Load(base, entry + 24) * 972 + data + 2972, 0);
          }
          i++;
          if (!(i < int32_t(Load(base, count)))) {
            break;
          }
        }
      }
    }
  }
  Stream(base, Load(base, kManager), frame, heap, make);
  calls.update_drivers(frame, 0x82174D04);
  TickVehicles(base, frame - 128, calls.tick);
  track_collision::TrackCollisionTick(base, frame - 144, calls.collision);
  RefreshVehicles(base, frame - 112, RefreshOriginal(calls));
}

}  // namespace race_world
