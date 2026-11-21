// RaceManager::GiveVehicle (sub_821706E8, docs/functions/race-world.md) as
// plain C++: a free vehicle slot given to a racer's entry, the vehicle set
// up from its record, placed on the route and settled. Never seen in play.
// Its callees (the vehicle's set-up, placing, physics step and status) go
// through the dispatch table (hooks/lifted_call.h). Its adapter is the
// override in src/hooks/race_world.cpp (which keeps the harness's stand-in).

#include "race_world/race_world.h"

#include "guest.h"
#include "schema/race_world.h"

#include "base/log.h"
#include "hooks/lifted_call.h"

namespace race_world {

namespace {

constexpr uint32_t kVehicleGet = 0x820C0468;
constexpr uint32_t kResetForCar = 0x820C3150;
constexpr uint32_t kVehicleSetUp = 0x820C33B8;
constexpr uint32_t kPlaceOnRoute = 0x820C3608;
constexpr uint32_t kInputRecordClear = 0x820C96E8;
constexpr uint32_t kBodySetUp = 0x820C0590;
constexpr uint32_t kPhysicsStep = 0x820B6498;
constexpr uint32_t kStatusUpdate = 0x820C23E8;

constexpr uint32_t kInputRecords = 0x826C5B30;  // a pointer a vehicle
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kPlaceSpeed = 0x82009974;
constexpr uint32_t kStepTime = 0x826193C8;

constexpr uint32_t kRecordBytes = 3936;

}  // namespace

uint64_t GiveVehicle(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t manager, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_821706E8 RaceManager::GiveVehicle");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 176);
  auto call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                  std::initializer_list<double> floats = {}) {
    return lifted_call::CallFloats(base, function, frame, r13, args, floats).r3;
  };
  uint32_t index = 0;
  for (;; index++) {
    if (index >= 7) {
      return 0;
    }
    const int32_t slot = int32_t(guest::LoadU32(base, schema::RaceManager(base, manager).vehicle_slots_address() + 4 * index));
    if (slot == 0 || slot == -1) {
      break;
    }
  }
  guest::StoreU32(base, entry + 20, index);
  const uint32_t vehicle = uint32_t(call(kVehicleGet, {index}));
  const uint32_t slot = schema::RaceManager(base, manager).vehicle_slots_address() + 4 * index;
  const uint32_t record = guest::LoadU32(base, kRaceData) +
                          guest::LoadU32(base, entry + 24) * kRecordBytes + 48;
  call(int32_t(guest::LoadU32(base, slot)) == -1 ? kResetForCar : kVehicleSetUp,
       {vehicle, record});
  // Placed at the entry's route position with the start speed, or with flag
  // 0x10 at the record's +3904 with its +3908 (at rest when that's 0.0).
  const double zero = double(guest::LoadFloat(base, kZero));
  double speed;
  if ((guest::LoadU32(base, entry) & 0x10) != 0) {
    guest::StoreU32(base, frame + 80, guest::LoadU32(base, record + 3904));
    speed = double(guest::LoadFloat(base, record + 3908));
    if (speed == zero) {
      guest::StoreU8(base, vehicle + 2384, 1);
    }
  } else {
    guest::StoreU32(base, frame + 80, guest::LoadU32(base, entry + 8));
    speed = double(guest::LoadFloat(base, kPlaceSpeed));
  }
  call(kPlaceOnRoute, {vehicle, frame + 80, 0, 0, 1}, {speed, zero});
  // The vehicle's input record cleared on the frame and copied to its
  // pointer's five words.
  call(kInputRecordClear, {frame + 96});
  const uint32_t to =
      guest::LoadU32(base, kInputRecords + 4 * (guest::LoadU32(base, entry + 20) & 0xFF));
  for (uint32_t k = 0; k < 20; k += 4) {
    guest::StoreU32(base, to + k, guest::LoadU32(base, frame + 96 + k));
  }
  call(kBodySetUp, {vehicle + 6160, vehicle});
  call(kPhysicsStep, {vehicle}, {double(guest::LoadFloat(base, kStepTime))});
  call(kStatusUpdate, {vehicle + 6160, vehicle, 0, 1});
  guest::StoreU32(base, entry, guest::LoadU32(base, entry) | 0x240);
  guest::StoreU32(base, slot, 1);
  return 1;
}

}  // namespace race_world
