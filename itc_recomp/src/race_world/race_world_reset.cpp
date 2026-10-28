// The racers' vehicles reset and the race world unloaded
// (docs/functions/race-world.md): RaceManager::ResetWorld, ResetVehicles and
// RaceWorld::Unload. See race_world.h.

#include "race_world/race_world.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "racer_controllers/racer_controllers.h"
#include "track_collision/track_collision.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_world {

namespace {

constexpr uint32_t kInputs = 0x826C5B30;       // the racers' input records
constexpr uint32_t kUnloadObject = 0x826C5B28;  // TrackCollision::Unload's r3
constexpr uint32_t kResetFloat = 0x826C2D80;
constexpr uint32_t kZeroFloat = 0x82000A34;

// A vehicle slot word: +2336 + 4 x index (the index's low 32 bits shifted,
// as the original's rotate and mask).
uint32_t Slot(uint32_t manager, uint32_t index) {
  return manager + ((index + 584) << 2);
}

}  // namespace

void ResetWorld(uint8_t* base, uint32_t manager, uint32_t frame, const Original& original) {
  ITC_NATIVE_RUNNING("sub_82170668 RaceManager::ResetWorld");
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t slot = manager + 2336 + 4 * i;
    if (int32_t(guest::LoadU32(base, slot)) > 0) {
      const uint32_t vehicle = vehicle_accessors::VehicleGet(base, i);
      uint32_t r3 = 0;
      if (!vehicle::VehicleRemoveFromWorld(base, vehicle, &r3)) {
        original(0x820C20E0, frame, vehicle);
      }
      guest::StoreU32(base, slot, 0);
    }
    guest::StoreU32(base, frame + 80, i);
    const uint64_t driver = ai_drivers::Of(base, frame + 80);
    ai_drivers::ResetDriver(base, uint32_t(driver));
  }
  track_collision::TrackCollisionResetWorld(base);
  // lfs, then stfs: the float through a double, as the original.
  const float value = float(double(guest::LoadFloat(base, kZeroFloat)));
  guest::StoreFloat(base, kResetFloat, value);
  guest::StoreU32(base, manager + 2388, 0);
}

void ResetVehicles(uint8_t* base, uint32_t manager, uint32_t flag, uint32_t frame,
                   const vehicle::CalleeOriginal& setup_original) {
  ITC_NATIVE_RUNNING("sub_82171718 RaceManager::ResetVehicles");
  const int32_t count = int32_t(guest::LoadU32(base, manager + 2368));
  guest::StoreU32(base, manager + 2364, 0);
  guest::StoreU32(base, manager + 2372, 0);
  guest::StoreU32(base, manager + 2376, 0);
  // One entry with a vehicle set up again; `input` its cleared input record
  // on the frame, `counter` +2372 (racers) or +2376 (traffic).
  const auto reset = [&](uint32_t entry, uint32_t input, uint32_t counter) {
    const uint32_t index_at = entry + 20;
    const uint32_t index = guest::LoadU32(base, index_at);
    if (int32_t(index) == -1) {
      return;
    }
    const uint32_t vehicle = vehicle_accessors::VehicleGet(base, index);
    const bool reset_car = int32_t(guest::LoadU32(base, Slot(manager, index))) != -1;
    const uint32_t car = guest::LoadU32(base, entry + 24) * 3936 +
                         guest::LoadU32(base, kRaceData) + 48;
    double f1 = 0.0;
    vehicle::VehicleSetUp(base, vehicle, car, frame - 224, reset_car, true, setup_original, &f1);
    if (flag != 0) {
      guest::StoreU32(base, entry, guest::LoadU32(base, entry) | 0x200);
    }
    guest::StoreU32(base, Slot(manager, guest::LoadU32(base, index_at)), 1);
    ai_drivers::ClearCarInput(base, input);
    const uint32_t to =
        guest::LoadU32(base, kInputs + ((guest::LoadU32(base, index_at) & 0xFF) << 2));
    for (uint32_t w = 0; w < 5; w++) {
      guest::StoreU32(base, to + 4 * w, guest::LoadU32(base, input + 4 * w));
    }
    guest::StoreU32(base, frame + 80, 1);
    const uint32_t id = guest::LoadU32(base, entry + 24) * 3936 +
                        guest::LoadU32(base, kRaceData) + 3960;
    racer_controllers::Setup(base, index_at, id, frame + 80, frame);
    const uint32_t listed = guest::LoadU32(base, manager + 2364);
    guest::StoreU32(base, manager + ((listed + 577) << 2), entry);
    const uint32_t racers = guest::LoadU32(base, manager + 2364);
    const uint32_t counted = guest::LoadU32(base, manager + counter);
    guest::StoreU32(base, manager + 2364, racers + 1);
    guest::StoreU32(base, manager + counter, counted + 1);
  };
  if (count > 0) {
    for (int32_t i = 0; i < int32_t(guest::LoadU32(base, manager + 2368)); i++) {
      reset(manager + 4 + 28 * uint32_t(i), frame + 96, 2372);
    }
  }
  for (uint32_t t = 0; t < 3; t++) {
    reset(manager + 2160 + 28 * t, frame + 128, 2376);
  }
  guest::StoreU32(base, manager + 2388, 0);
}

void Unload(uint8_t* base, uint32_t frame, const Original& original) {
  ITC_NATIVE_RUNNING("sub_82171250 RaceWorld::Unload");
  const uint32_t race = guest::LoadU32(base, kRaceData);
  if (int32_t(guest::LoadU32(base, race + 0xA3AF0)) == 1) {
    const int32_t kind = int32_t(guest::LoadU32(base, race + 0xA3AF4));
    if (kind == 2 || kind == 3) {
      const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
      const uint32_t vehicle = vehicle_accessors::VehicleGet(base, 0);
      AddDistance(base, save, double(guest::LoadFloat(base, vehicle + 6244)));
    }
  }
  if (!vehicle::VehiclesDestroy(base)) {
    original(0x820C3B78, frame, 0);
  }
  uint64_t r3 = 0;
  if (!ai_drivers::Destroy(base, &r3)) {
    original(0x8213B2B8, frame, 0);
  }
  if (!track_collision::TrackCollisionUnload(base)) {
    original(0x820FEFB0, frame, guest::LoadU32(base, kUnloadObject));
  }
  const uint32_t manager = guest::LoadU32(base, kManager);
  if (!memory::crt::OperatorDelete(manager)) {
    original(0x825A70E8, frame, manager);
  }
  guest::StoreU32(base, kManager, 0);
}

}  // namespace race_world
