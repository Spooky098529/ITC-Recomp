// The racers made, placed and settled (docs/functions/race-world.md):
// RaceWorld::OnLoaded, RaceStart::Place and RaceStart::PlaceRolling. See
// race_world.h.

#include "race_world/race_world.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_world {

using namespace car_physics::ops;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kInputs = 0x826C5B30;        // the racers' input records
constexpr uint32_t kStagesOffset = 0x59620;     // the race data's stages
constexpr uint32_t kFlagsOffset = 0x59AE4;      // the race data's flags
constexpr uint32_t kCountOffset = 48 + 0x595E0;  // the race data's record count
constexpr uint32_t kModeOffset = 0xA3AF0;
constexpr uint32_t kKindOffset = 0xA3AF4;
constexpr uint32_t kCurrent = 0xFFFFFFFFu;       // the current stage
constexpr uint32_t kStartObject = 0x826C7820;    // its +4: the start from an object
constexpr uint32_t kStartPosition = 0x826C6B0C;  // its +2, +4: the start's halfwords
constexpr uint32_t kRollingSpeed = 0x820097FC;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Manager(uint8_t* base) {
  return Load(base, kManager);
}
uint32_t RaceData(uint8_t* base) {
  return Load(base, kRaceData);
}
// A halfword's value sign-extended, as the originals' extsh.
uint32_t S16(uint32_t value) {
  return uint32_t(int32_t(int16_t(uint16_t(value))));
}
// C's remainder by 2 (srawi, addze, rlwinm, subf).
uint32_t Mod2(uint32_t n) {
  return uint32_t(int32_t(n) % 2);
}
uint32_t RacerCount(uint8_t* base) {
  return Load(base, Manager(base) + 2364);
}

// Stages::Copy of the current stage into `out`; its original where the
// plain C++ refuses.
void CopyStage(uint8_t* base, uint32_t out, uint32_t sp, const StartCalls& calls) {
  const uint32_t stages = RaceData(base) + kStagesOffset;
  if (!va::StagesCopy(base, out, stages, kCurrent)) {
    calls.original(0x820FFD88, sp, out, stages, 0xFFFFFFFFFFFFFFFFull);
  }
}

uint32_t HasRacer(uint8_t* base, uint32_t record) {
  return va::StagesHasRacer(base, RaceData(base) + kStagesOffset, record, kCurrent);
}

// Vehicle::PlaceOnRoute (the whole original where its plain C++ refuses).
void PlaceOnRoute(uint8_t* base, uint32_t vehicle, uint32_t position, double speed,
                  double offset, uint32_t lane, uint32_t sp, const StartCalls& calls) {
  if (!vehicle::VehiclePlaceOnRoute(base, vehicle, position, speed, offset, int32_t(lane), 1,
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
void Settle(uint8_t* base, uint32_t frame, uint32_t ticks, Settled which, uint32_t input,
            uint32_t copy, bool note, uint32_t update_lr, uint32_t drive_lr,
            const StartCalls& calls) {
  for (uint32_t tick = ticks; tick != 0; tick--) {
    calls.update_drivers(frame, update_lr);
    for (int32_t i = 0; i < int32_t(RacerCount(base)); i++) {
      const uint32_t entry = Load(base, Manager(base) + 2308 + 4 * uint32_t(i));
      const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
      if (which == Settled::kRollingInStage && (Load(base, entry) & 0x400) == 0) {
        continue;
      }
      if (which != Settled::kAll && HasRacer(base, Load(base, entry + 24)) == 0) {
        continue;
      }
      const uint32_t from = uint32_t(
          calls.drive_car(frame + input, Load(base, entry + 20), 1, frame, drive_lr));
      for (uint32_t w = 0; w < 5; w++) {
        Store(base, frame + copy + 4 * w, Load(base, from + 4 * w));
      }
      const uint32_t to = Load(base, kInputs + ((Load(base, entry + 20) << 2) & 0x3FC));
      for (uint32_t w = 0; w < 5; w++) {
        Store(base, to + 4 * w, Load(base, frame + copy + 4 * w));
      }
      vehicle::VehicleTick(base, vehicle, Load(base, entry + 20), 0, 1, frame - 144, calls.tick);
      if (note) {
        const uint32_t identity = va::VehicleIdentity(vehicle) + 4;
        guest::StoreU16(base, entry + 8, guest::LoadU16(base, identity + 0));
        guest::StoreU16(base, entry + 10, guest::LoadU16(base, identity + 2));
      }
    }
    track_collision::TrackCollisionTick(base, frame - 144, calls.collision);
  }
}

}  // namespace

void OnLoaded(uint8_t* base, uint32_t frame, const StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82173A90 RaceWorld::OnLoaded");
  CopyStage(base, frame + 160, frame, calls);
  bool held = false;
  {
    const uint32_t race = RaceData(base);
    if (va::StagesIsType2Or3(base, race + kStagesOffset, kCurrent) != 0 ||
        (int32_t(Load(base, RaceData(base) + kModeOffset)) == 6 &&
         int32_t(Load(base, frame + 160)) != 1)) {
      const uint32_t flags = RaceData(base) + kFlagsOffset;
      held = true;
      Store(base, flags, Load(base, flags) | 0x2000);
    }
  }
  if (!vehicle::VehiclesCreate(base, frame - 112)) {
    calls.original(0x820C3BD0, frame, 0, 0, 0);
  }
  const uint32_t first = RaceData(base) + 48;
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t vehicle = va::VehicleGet(base, i);
    double f1 = 0.0;
    vehicle::VehicleSetUp(base, vehicle, first, frame - 224, false, true, calls.setup_original,
                          &f1);
    uint32_t r3 = 0;
    if (!vehicle::VehicleRemoveFromWorld(base, vehicle, &r3)) {
      calls.original(0x820C20E0, frame, vehicle, 0, 0);
    }
  }
  uint64_t created = 0;
  if (!ai_drivers::Create(base, &created)) {
    calls.original(0x8213B1E0, frame, 0, 0, 0);
  }
  if (!Create(base)) {
    calls.original(0x821715B8, frame, 0, 0, 0);
  }
  AddRacers(base, Manager(base), RaceData(base) + 48, frame - 2000);
  for (int32_t i = 0; i < int32_t(RacerCount(base)); i++) {
    const uint32_t entry = Load(base, Manager(base) + 2308 + 4 * uint32_t(i));
    const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
    const uint32_t car = Load(base, entry + 24) * 3936 + RaceData(base) + 48;
    double f1 = 0.0;
    vehicle::VehicleSetUp(base, vehicle, car, frame - 224, true, true, calls.setup_original, &f1);
  }
  if (int32_t(RacerCount(base)) > 0) {
    const double across = Lfs(base, kZero);
    for (int32_t i = 0; i < int32_t(RacerCount(base)); i++) {
      const uint32_t slot = 2308 + 4 * uint32_t(i);
      const uint32_t entry = Load(base, Manager(base) + slot);
      const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
      const uint32_t record = Load(base, entry + 24);
      const uint32_t car = record * 3936 + RaceData(base);
      const uint32_t has = HasRacer(base, record);
      bool rest = false;
      if (held || (int32_t(Load(base, RaceData(base) + kModeOffset)) == 6 &&
                   int32_t(Load(base, frame + 160)) != 1)) {
        rest = has != 0;
      }
      guest::StoreU8(base, vehicle + 2384, rest ? 1 : 0);
      const int32_t kind = int32_t(Load(base, RaceData(base) + kKindOffset));
      const uint32_t lane = kind == 0 || kind == 1 ? 0xFFFFFFFFu : Mod2(uint32_t(i));
      track_route::ClampPosition(base, frame + 80, Load(base, Load(base, Manager(base) + slot) + 8));
      PlaceOnRoute(base, vehicle, frame + 80, Lfs(base, car + 3956), across, lane, frame, calls);
    }
  }
  Settle(base, frame, 30, Settled::kAll, 128, 96, false, 0x82173CE8, 0x82173D20, calls);
  for (int32_t i = 0; i < int32_t(RacerCount(base)); i++) {
    const uint32_t entry = Load(base, Manager(base) + 2308 + 4 * uint32_t(i));
    const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
    const uint32_t car = Load(base, entry + 24) * 3936 + RaceData(base);
    Stfs(base, vehicle + 6244, Lfs(base, car + 3968));
  }
  ListRacers(base, Manager(base), frame - 1872, true,
             [&](uint32_t address, uint32_t sp, uint32_t r3) {
               return calls.original(address, sp, r3, 0, 0);
             });
  const uint32_t flags = RaceData(base) + kFlagsOffset;
  Store(base, flags, Load(base, flags) | 0x200);
  NoteMarkedRacer(base, frame - 144);
}

void Place(uint8_t* base, uint32_t object, uint32_t settle, uint32_t back, uint32_t sp,
           const StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82165E20 RaceStart::Place");
  const uint32_t frame = sp - 400;
  Store(base, sp + 20, object);
  Store(base, sp + 28, settle);
  guest::StoreU16(base, frame + 80, 0xFFFF);
  guest::StoreU16(base, frame + 82, 0xFFFF);
  // The player's position.
  uint32_t player = 0;
  bool found = false;
  {
    const uint32_t records = RaceData(base) + 48;
    const int32_t count = int32_t(Load(base, RaceData(base) + kCountOffset));
    for (int32_t i = 0; i < count; i++) {
      if ((Load(base, records + 3936 * uint32_t(i) + 3888) & 0x1) != 0) {
        player = Load(base, EntryOfRecord(base, uint32_t(i)) + 8);
        found = true;
        break;
      }
    }
  }
  if (!found) {
    player = Load(base, frame + 80);
  }
  bool backward = false;
  if (back != 0) {
    backward = true;
  } else if (int32_t(Load(base, RaceData(base) + kModeOffset)) == 1) {
    const int32_t value = int32_t(Load(base, va::SaveDataOf(base, 0) + 6100));
    backward = value == 1;
  }
  int32_t placed = 0;
  int32_t ahead = 1, behind = -1;
  if (int32_t(RacerCount(base)) > 0) {
    const double zero = Lfs(base, kZero);
    for (int32_t i = 0; i < int32_t(RacerCount(base)); i++) {
      const uint32_t entry = Load(base, Manager(base) + 2308 + 4 * uint32_t(i));
      const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
      const uint32_t number = Load(base, entry + 24);
      const uint32_t car = number * 3936 + RaceData(base);
      if (HasRacer(base, number) == 0) {
        continue;
      }
      CopyStage(base, frame + 160, frame, calls);
      const bool stage23 =
          va::StagesIsType2Or3(base, RaceData(base) + kStagesOffset, kCurrent) != 0;
      const bool rest = stage23 || int32_t(Load(base, RaceData(base) + kKindOffset)) == 8;
      guest::StoreU8(base, vehicle + 2384, rest ? 1 : 0);
      uint32_t lane = Mod2(uint32_t(placed));
      const uint32_t race = RaceData(base);
      if (int32_t(Load(base, race + kKindOffset)) == 8) {
        uint32_t start;
        if (Load(base, kStartObject + 4) != 0) {
          start = Load(base, Load(base, Load(base, sp + 20) + 9624) + 7244);
        } else {
          start = (uint32_t(guest::LoadU16(base, kStartPosition + 2)) << 16) |
                  guest::LoadU16(base, kStartPosition + 4);
        }
        Store(base, frame + 80, start);
        track_route::ToMainRoad(base, kRoute, frame + 80, 1, frame - 144);
        const uint32_t section = guest::LoadU16(base, frame + 80);
        const uint32_t file = Load(base, kRoute);
        const uint32_t record = Load(base, (section << 2) + Load(base, file + 12)) + 124;
        const uint32_t slice = S16(guest::LoadU16(base, frame + 82)) * 84 + record;
        const uint32_t lanes = guest::LoadU8(base, slice + 56) & 0xF;
        if (!(int32_t(lane) < int32_t(lanes))) {
          lane = lanes - 1;
          vehicle::RoutePositionStep(base, frame + 80, 0xFFFFFFFFu, 0xFFFFFFFFu, frame - 112);
        }
        PlaceOnRoute(base, vehicle, frame + 80, zero, zero, lane, frame, calls);
        Store(base, car + 3952, Load(base, frame + 80));
        placed++;
        continue;
      }
      const bool type23 = va::StagesIsType2Or3(base, race + kStagesOffset, kCurrent) != 0;
      Store(base, frame + 80, player);
      const uint32_t flags = Load(base, car + 3936);
      if ((flags & 0x1) == 0) {
        const int32_t id = int32_t(Load(base, car + 3960));
        const uint32_t kind =
            id < 0 ? 0xFFFFFFFFu
                   : uint32_t(int32_t(int8_t(guest::LoadU8(base, data_tables::Item(base, uint32_t(id)) + 36))));
        bool step_forward;  // RoutePosition::Step with `ahead`
        uint32_t step = 0;
        if (type23) {
          step_forward = true;
        } else if (backward) {
          step_forward = false;
          step = S16(uint32_t(behind));
        } else {
          const int32_t type = int32_t(Load(base, frame + 160));
          step_forward = type == 3 || type == 1;
          step = S16(uint32_t(ahead));
        }
        if (step_forward) {
          vehicle::RoutePositionStep(base, frame + 80, S16(uint32_t(ahead)), S16(kind),
                                     frame - 112);
        } else {
          StepToMainRoad(base, frame + 80, step, S16(kind), frame - 112);
        }
        ahead++;
        behind--;
      }
      PlaceOnRoute(base, vehicle, frame + 80, Lfs(base, car + 3956), zero, lane, frame, calls);
      placed++;
    }
  }
  if (int32_t(Load(base, sp + 28)) == 0) {
    return;
  }
  CopyStage(base, frame + 160, frame, calls);
  uint32_t ticks = 30;
  if (int32_t(Load(base, RaceData(base) + kModeOffset)) == 6 &&
      int32_t(Load(base, frame + 160)) != 1) {
    ticks = 300;
  }
  Settle(base, frame, ticks, Settled::kInStage, 128, 96, true, 0x8216616C, 0x821661C4, calls);
}

void PlaceRolling(uint8_t* base, uint32_t object, uint32_t settle, uint32_t frame,
                  const StartCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821662E0 RaceStart::PlaceRolling");
  const uint32_t records = RaceData(base) + 48;
  Place(base, object, 0, 0, frame, calls);
  uint32_t player = 0;
  {
    const int32_t count = int32_t(Load(base, records + 0x595E0));
    for (int32_t i = 0; i < count; i++) {
      if ((Load(base, records + 3936 * uint32_t(i) + 3888) & 0x1) != 0) {
        player = EntryOfRecord(base, uint32_t(i));
        break;
      }
    }
  }
  bool all = false;
  if (int32_t(Load(base, RaceData(base) + kModeOffset)) == 1) {
    const int32_t value = int32_t(Load(base, va::SaveDataOf(base, 0) + 6100));
    all = (value <= 999 && value == 1) ||
          va::StagesIsType2Or3(base, RaceData(base) + kStagesOffset, kCurrent) != 0;
  }
  if (int32_t(RacerCount(base)) > 0) {
    const double zero = Lfs(base, kZero);
    int32_t placed = 0, ahead = 10, behind = -10;
    for (int32_t i = 0; i < int32_t(RacerCount(base)); i++) {
      uint32_t entry = Load(base, Manager(base) + 2308 + 4 * uint32_t(i));
      const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
      const uint32_t number = Load(base, entry + 24);
      const uint32_t car = number * 3936 + RaceData(base);
      if (HasRacer(base, number) == 0) {
        continue;
      }
      guest::StoreU8(base, vehicle + 2384, 0);
      if ((Load(base, car + 3936) & 0x1) != 0) {
        continue;
      }
      if ((Load(base, entry) & 0x400) == 0 && !all) {
        continue;
      }
      const uint32_t from = player + 8;
      entry += 8;
      guest::StoreU16(base, entry + 0, guest::LoadU16(base, from + 0));
      guest::StoreU16(base, entry + 2, guest::LoadU16(base, from + 2));
      const uint32_t step = S16(uint32_t(all ? ahead : behind));
      StepToMainRoad(base, entry, step, 0xFFFFFFFFu, frame - 112);
      PlaceOnRoute(base, vehicle, entry, Lfs(base, kRollingSpeed), zero, Mod2(uint32_t(placed)),
                   frame, calls);
      placed++;
      ahead += 2;
      behind -= 2;
    }
  }
  if (int32_t(settle) == 0) {
    return;
  }
  Settle(base, frame, 30, Settled::kRollingInStage, 112, 80, true, 0x821664D8, 0x82166538,
         calls);
}

}  // namespace race_world
