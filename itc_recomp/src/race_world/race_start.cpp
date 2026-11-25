// The racers made, placed and settled (docs/functions/race-world.md):
// RaceWorld::OnLoaded, RaceStart::Place and RaceStart::PlaceRolling. See
// race_world.h.

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

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "memory/host_copy.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_world {

using M = schema::RaceManager;
using G = schema::RaceWorldGlobals;
using E = schema::MinimapRacerEntry;
using RD = schema::RaceData;
using Car = schema::Car;

using namespace car_physics::ops;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kInputs = 0x826C5B30;        // the racers' input records
constexpr uint32_t kCurrent = 0xFFFFFFFFu;       // the current stage
constexpr uint32_t kStartPosition = 0x826C6B0C;  // its +2, +4: the start's halfwords
constexpr uint32_t kRollingSpeed = 0x820097FC;

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint32_t Manager() {
  return G::racers_manager().address;
}
uint32_t RaceData() {
  return Load(kRaceData);
}
// The race data's stages (+0x59620).
uint32_t Stages() {
  return RD(RaceData()).stages_address();
}
// The racer record `number` (the race data's +48 + 3,936 x number).
Car CarOf(uint32_t number) {
  return Car(number * Car::kSize + RD(RaceData()).racer_records_address());
}
// A halfword's value sign-extended, as the originals' extsh.
uint32_t S16(uint32_t value) {
  return uint32_t(int32_t(int16_t(uint16_t(value))));
}
// C's remainder by 2 (srawi, addze, rlwinm, subf).
uint32_t Mod2(uint32_t n) {
  return uint32_t(int32_t(n) % 2);
}
uint32_t RacerCount() {
  return M(Manager()).racer_count();
}

// Stages::Copy of the current stage into `out`; its original where the
// plain C++ refuses.
void CopyStage(uint32_t out, uint32_t sp, const StartCalls& calls) {
  const uint32_t stages = Stages();
  if (!va::StagesCopy(out, stages, kCurrent)) {
    calls.original(0x820FFD88, sp, out, stages, 0xFFFFFFFFFFFFFFFFull);
  }
}

uint32_t HasRacer(uint32_t record) {
  return va::StagesHasRacer(Stages(), record, kCurrent);
}

// Vehicle::PlaceOnRoute (the whole original where its plain C++ refuses).
void PlaceOnRoute(uint32_t vehicle, uint32_t position, double speed,
                  double offset, uint32_t lane, uint32_t sp, const StartCalls& calls) {
  if (!vehicle::VehiclePlaceOnRoute(vehicle, position, speed, offset, int32_t(lane), 1,
                                    sp - 560, calls.place)) {
    vehicle::GuestRegisters r;
    r.r3 = vehicle;
    r.r4 = position;
    r.r6 = uint64_t(int64_t(int32_t(lane)));
    r.f1 = speed;
    r.f2 = offset;
    calls.place(0x820C3608, sp, r);
  }
}

// Which racers the settling ticks drive.
enum class Settled { kAll, kInStage, kRollingInStage };

// The settling ticks: `ticks` times AiDrivers::UpdateAll (`update_lr`), then
// each racer (the count read again each pass) that `which` takes:
// AiDriver::DriveCar into `frame` + `input` (`drive_lr`), its record copied
// to `frame` + `copy` and from there to the racer's input record
// ([0x826C5B30 + 4 x the index's low byte]), Vehicle::Tick(vehicle, index,
// 0, 1); with `note`, the entry's +8 = its vehicle's track position's route
// position (two halfwords); then TrackCollision::Tick.
void Settle(uint32_t frame, uint32_t ticks, Settled which, uint32_t input,
            uint32_t copy, bool note, uint32_t update_lr, uint32_t drive_lr,
            const StartCalls& calls) {
  for (uint32_t tick = ticks; tick != 0; tick--) {
    calls.update_drivers(frame, update_lr);
    for (int32_t i = 0; i < int32_t(RacerCount()); i++) {
      const E entry(Load(M(Manager()).racers_address() + 4 * uint32_t(i)));
      const uint32_t vehicle = va::VehicleGet(entry.vehicle_index());
      if (which == Settled::kRollingInStage && (entry.flags() & 0x400) == 0) {
        continue;
      }
      if (which != Settled::kAll && HasRacer(entry.record()) == 0) {
        continue;
      }
      const uint32_t from = uint32_t(
          calls.drive_car(frame + input, entry.vehicle_index(), 1, frame, drive_lr));
      for (uint32_t w = 0; w < 5; w++) {
        Store(frame + copy + 4 * w, Load(from + 4 * w));
      }
      const uint32_t to = Load(kInputs + ((entry.vehicle_index() << 2) & 0x3FC));
      // A host record (E5 step 4) is copied with each field swapped.
      if (!memory::CopyWithHandles(to, frame + copy, 20)) {
        for (uint32_t w = 0; w < 5; w++) {
          Store(to + 4 * w, Load(frame + copy + 4 * w));
        }
      }
      vehicle::VehicleTick(vehicle, entry.vehicle_index(), 0, 1, frame - 144, calls.tick);
      if (note) {
        const schema::TrackPosition identity(va::VehicleIdentity(vehicle));
        entry.set_section(identity.route_section());
        entry.set_slice(identity.route_slice());
      }
    }
    track_collision::TrackCollisionTick(frame - 144, calls.collision);
  }
}

}  // namespace

void OnLoaded(uint32_t frame, const StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82173A90 RaceWorld::OnLoaded");
  CopyStage(frame + 160, frame, calls);
  bool held = false;
  {
    const uint32_t race = RaceData();
    if (va::StagesIsType2Or3(RD(race).stages_address(), kCurrent) != 0 ||
        (int32_t(RD(RaceData()).kind()) == 6 &&
         int32_t(Load(frame + 160)) != 1)) {
      const uint32_t flags = RD(RaceData()).flags_address();
      held = true;
      Store(flags, Load(flags) | 0x2000);
    }
  }
  if (!vehicle::VehiclesCreate(frame - 112)) {
    calls.original(0x820C3BD0, frame, 0, 0, 0);
  }
  const uint32_t first = RD(RaceData()).racer_records_address();
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t vehicle = va::VehicleGet(i);
    double f1 = 0.0;
    vehicle::VehicleSetUp(vehicle, first, frame - 224, false, true, calls.setup_original,
                          &f1);
    uint32_t r3 = 0;
    if (!vehicle::VehicleRemoveFromWorld(vehicle, &r3)) {
      calls.original(0x820C20E0, frame, vehicle, 0, 0);
    }
  }
  uint64_t created = 0;
  if (!ai_drivers::Create(&created)) {
    calls.original(0x8213B1E0, frame, 0, 0, 0);
  }
  if (!Create()) {
    calls.original(0x821715B8, frame, 0, 0, 0);
  }
  AddRacers(Manager(), RD(RaceData()).racer_records_address(), frame - 2000);
  for (int32_t i = 0; i < int32_t(RacerCount()); i++) {
    const E entry(Load(M(Manager()).racers_address() + 4 * uint32_t(i)));
    const uint32_t vehicle = va::VehicleGet(entry.vehicle_index());
    const uint32_t car = CarOf(entry.record()).address;
    double f1 = 0.0;
    vehicle::VehicleSetUp(vehicle, car, frame - 224, true, true, calls.setup_original, &f1);
  }
  if (int32_t(RacerCount()) > 0) {
    const double across = Lfs(kZero);
    for (int32_t i = 0; i < int32_t(RacerCount()); i++) {
      const uint32_t slot = 4 * uint32_t(i);
      const E entry(Load(M(Manager()).racers_address() + slot));
      const uint32_t vehicle = va::VehicleGet(entry.vehicle_index());
      const uint32_t record = entry.record();
      const Car car = CarOf(record);
      const uint32_t has = HasRacer(record);
      bool rest = false;
      if (held || (int32_t(RD(RaceData()).kind()) == 6 &&
                   int32_t(Load(frame + 160)) != 1)) {
        rest = has != 0;
      }
      schema::Vehicle(vehicle).set_forced_rest(rest ? 1 : 0);
      const int32_t kind = int32_t(RD(RaceData()).race_mode());
      const uint32_t lane = kind == 0 || kind == 1 ? 0xFFFFFFFFu : Mod2(uint32_t(i));
      track_route::ClampPosition(frame + 80,
          E(Load(M(Manager()).racers_address() + slot)).position());
      PlaceOnRoute(vehicle, frame + 80, double(car.start_float()), across, lane, frame,
                   calls);
    }
  }
  Settle(frame, 30, Settled::kAll, 128, 96, false, 0x82173CE8, 0x82173D20, calls);
  for (int32_t i = 0; i < int32_t(RacerCount()); i++) {
    const E entry(Load(M(Manager()).racers_address() + 4 * uint32_t(i)));
    const uint32_t vehicle = va::VehicleGet(entry.vehicle_index());
    const Car car = CarOf(entry.record());
    Stfs(schema::VehicleStatus(schema::Vehicle(vehicle).status_address())
             .trip_km_address(),
         double(car.racer_value()));
  }
  ListRacers(Manager(), frame - 1872, true,
             [&](uint32_t address, uint32_t sp, uint32_t r3) {
               return calls.original(address, sp, r3, 0, 0);
             });
  const uint32_t flags = RD(RaceData()).flags_address();
  Store(flags, Load(flags) | 0x200);
  NoteMarkedRacer(frame - 144);
}

void Place(uint32_t object, uint32_t settle, uint32_t back, uint32_t sp,
           const StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82165E20 RaceStart::Place");
  const uint32_t frame = sp - 400;
  // r3 and r4 spilled into the caller's frame, as the original (kept: the
  // caller's words); read back as the values since E5 step 6 (nothing
  // writes them between).
  Store(sp + 20, object);
  Store(sp + 28, settle);
  guest::StoreU16(frame + 80, 0xFFFF);
  guest::StoreU16(frame + 82, 0xFFFF);
  // The player's position.
  uint32_t player = 0;
  bool found = false;
  {
    const uint32_t records = RD(RaceData()).racer_records_address();
    const int32_t count = int32_t(RD(RaceData()).car_count());
    for (int32_t i = 0; i < count; i++) {
      if ((Car(records + Car::kSize * uint32_t(i)).flags() & 0x1) != 0) {
        player = E(EntryOfRecord(uint32_t(i))).position();
        found = true;
        break;
      }
    }
  }
  if (!found) {
    player = Load(frame + 80);
  }
  bool backward = false;
  if (back != 0) {
    backward = true;
  } else if (int32_t(RD(RaceData()).kind()) == 1) {
    const int32_t value = int32_t(schema::SaveData(va::SaveDataOf(0)).day());
    backward = value == 1;
  }
  int32_t placed = 0;
  int32_t ahead = 1, behind = -1;
  if (int32_t(RacerCount()) > 0) {
    const double zero = Lfs(kZero);
    for (int32_t i = 0; i < int32_t(RacerCount()); i++) {
      const E entry(Load(M(Manager()).racers_address() + 4 * uint32_t(i)));
      const uint32_t vehicle = va::VehicleGet(entry.vehicle_index());
      const uint32_t number = entry.record();
      const Car car = CarOf(number);
      if (HasRacer(number) == 0) {
        continue;
      }
      CopyStage(frame + 160, frame, calls);
      const bool stage23 = va::StagesIsType2Or3(Stages(), kCurrent) != 0;
      const bool rest = stage23 || int32_t(RD(RaceData()).race_mode()) == 8;
      schema::Vehicle(vehicle).set_forced_rest(rest ? 1 : 0);
      uint32_t lane = Mod2(uint32_t(placed));
      const uint32_t race = RaceData();
      if (int32_t(RD(race).race_mode()) == 8) {
        uint32_t start;
        if (G::human_flag() != 0) {
          start = Load(Load(object + 9624) + 7244);
        } else {
          start = (uint32_t(guest::LoadU16(kStartPosition + 2)) << 16) |
                  guest::LoadU16(kStartPosition + 4);
        }
        Store(frame + 80, start);
        track_route::ToMainRoad(kRoute, frame + 80, 1, frame - 144);
        const schema::TrackRoutePosition at(frame + 80);
        const uint32_t section = at.section();
        const schema::TrackRouteFile file = schema::TrackRoute(kRoute).file();
        const schema::TrackRouteSection record(Load((section << 2) + file.sections()));
        const schema::TrackRouteSlice slice(S16(at.slice()) * schema::TrackRouteSlice::kSize + record.slices_address());
        const uint32_t lanes = slice.lanes() & 0xF;
        if (!(int32_t(lane) < int32_t(lanes))) {
          lane = lanes - 1;
          vehicle::RoutePositionStep(frame + 80, 0xFFFFFFFFu, 0xFFFFFFFFu, frame - 112);
        }
        PlaceOnRoute(vehicle, frame + 80, zero, zero, lane, frame, calls);
        car.set_start_position(Load(frame + 80));
        placed++;
        continue;
      }
      const bool type23 = va::StagesIsType2Or3(RD(race).stages_address(), kCurrent) != 0;
      Store(frame + 80, player);
      const uint32_t flags = car.flags();
      if ((flags & 0x1) == 0) {
        const int32_t id = int32_t(car.item_id());
        const uint32_t kind =
            id < 0 ? 0xFFFFFFFFu
                   : uint32_t(int32_t(int8_t(
                         schema::OpponentItem(data_tables::Item(uint32_t(id))).location())));
        bool step_forward;  // RoutePosition::Step with `ahead`
        uint32_t step = 0;
        if (type23) {
          step_forward = true;
        } else if (backward) {
          step_forward = false;
          step = S16(uint32_t(behind));
        } else {
          const int32_t type = int32_t(Load(frame + 160));
          step_forward = type == 3 || type == 1;
          step = S16(uint32_t(ahead));
        }
        if (step_forward) {
          vehicle::RoutePositionStep(frame + 80, S16(uint32_t(ahead)), S16(kind),
                                     frame - 112);
        } else {
          StepToMainRoad(frame + 80, step, S16(kind), frame - 112);
        }
        ahead++;
        behind--;
      }
      PlaceOnRoute(vehicle, frame + 80, double(car.start_float()), zero, lane, frame, calls);
      placed++;
    }
  }
  if (int32_t(settle) == 0) {
    return;
  }
  CopyStage(frame + 160, frame, calls);
  uint32_t ticks = 30;
  if (int32_t(RD(RaceData()).kind()) == 6 &&
      int32_t(Load(frame + 160)) != 1) {
    ticks = 300;
  }
  Settle(frame, ticks, Settled::kInStage, 128, 96, true, 0x8216616C, 0x821661C4, calls);
}

void PlaceRolling(uint32_t object, uint32_t settle, uint32_t frame,
                  const StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821662E0 RaceStart::PlaceRolling");
  const RD race(RaceData());
  const uint32_t records = race.racer_records_address();
  Place(object, 0, 0, frame, calls);
  uint32_t player = 0;
  {
    const int32_t count = int32_t(race.car_count());
    for (int32_t i = 0; i < count; i++) {
      if ((Car(records + Car::kSize * uint32_t(i)).flags() & 0x1) != 0) {
        player = EntryOfRecord(uint32_t(i));
        break;
      }
    }
  }
  bool all = false;
  if (int32_t(RD(RaceData()).kind()) == 1) {
    const int32_t value = int32_t(schema::SaveData(va::SaveDataOf(0)).day());
    all = (value <= 999 && value == 1) ||
          va::StagesIsType2Or3(Stages(), kCurrent) != 0;
  }
  if (int32_t(RacerCount()) > 0) {
    const double zero = Lfs(kZero);
    int32_t placed = 0, ahead = 10, behind = -10;
    for (int32_t i = 0; i < int32_t(RacerCount()); i++) {
      const E entry(Load(M(Manager()).racers_address() + 4 * uint32_t(i)));
      const uint32_t vehicle = va::VehicleGet(entry.vehicle_index());
      const uint32_t number = entry.record();
      const Car car = CarOf(number);
      if (HasRacer(number) == 0) {
        continue;
      }
      schema::Vehicle(vehicle).set_forced_rest(0);
      if ((car.flags() & 0x1) != 0) {
        continue;
      }
      if ((entry.flags() & 0x400) == 0 && !all) {
        continue;
      }
      const E from(player);
      entry.set_section(from.section());
      entry.set_slice(from.slice());
      const uint32_t step = S16(uint32_t(all ? ahead : behind));
      StepToMainRoad(entry.position_address(), step, 0xFFFFFFFFu, frame - 112);
      PlaceOnRoute(vehicle, entry.position_address(), Lfs(kRollingSpeed), zero,
                   Mod2(uint32_t(placed)), frame, calls);
      placed++;
      ahead += 2;
      behind -= 2;
    }
  }
  if (int32_t(settle) == 0) {
    return;
  }
  Settle(frame, 30, Settled::kRollingInStage, 112, 80, true, 0x821664D8, 0x82166538,
         calls);
}

}  // namespace race_world
