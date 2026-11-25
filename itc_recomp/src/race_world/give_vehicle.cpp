// RaceManager::GiveVehicle (sub_821706E8, docs/functions/race-world.md) as
// plain C++: a free vehicle slot given to a racer's entry, the vehicle set
// up from its record, placed on the route and settled. Never seen in play.
// Its callees (the vehicle's set-up, placing, physics step and status) go
// through the dispatch table (hooks/lifted_call.h). Its adapter is the
// override in src/hooks/race_world.cpp (which keeps the harness's stand-in).

#include "race_world/race_world.h"

#include "guest.h"
#include "memory/host_copy.h"
#include "schema/car_physics.h"
#include "schema/minimap.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
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

uint64_t GiveVehicle(uint32_t sp, uint64_t r13, uint32_t manager, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_821706E8 RaceManager::GiveVehicle");
  const uint32_t frame = lifted_call::PushFrame(sp, 176);
  auto call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                  std::initializer_list<double> floats = {}) {
    return lifted_call::CallFloats(function, frame, r13, args, floats).r3;
  };
  uint32_t index = 0;
  for (;; index++) {
    if (index >= 7) {
      return 0;
    }
    const int32_t slot = int32_t(guest::LoadU32(schema::RaceManager(manager).vehicle_slots_address() + 4 * index));
    if (slot == 0 || slot == -1) {
      break;
    }
  }
  const schema::MinimapRacerEntry e(entry);
  e.set_vehicle_index(index);
  const uint32_t vehicle = uint32_t(call(kVehicleGet, {index}));
  const schema::Vehicle v(vehicle);
  const uint32_t slot = schema::RaceManager(manager).vehicle_slots_address() + 4 * index;
  const uint32_t record =
      schema::RaceData(guest::LoadU32(kRaceData)).racer_records_address() +
      e.record() * kRecordBytes;
  call(int32_t(guest::LoadU32(slot)) == -1 ? kResetForCar : kVehicleSetUp,
       {vehicle, record});
  // Placed at the entry's route position with the start speed, or with flag
  // 0x10 at the record's +3904 with its +3908 (at rest when that's 0.0).
  const double zero = double(guest::LoadFloat(kZero));
  double speed;
  if ((e.flags() & 0x10) != 0) {
    const schema::Car car(record);
    guest::StoreU32(frame + 80, car.start_position());
    speed = double(car.start_float());
    if (speed == zero) {
      v.set_forced_rest(1);
    }
  } else {
    guest::StoreU32(frame + 80, e.position());
    speed = double(guest::LoadFloat(kPlaceSpeed));
  }
  call(kPlaceOnRoute, {vehicle, frame + 80, 0, 0, 1}, {speed, zero});
  // The vehicle's input record cleared on the frame and copied to its
  // pointer's five words.
  call(kInputRecordClear, {frame + 96});
  const uint32_t to =
      guest::LoadU32(kInputRecords + 4 * (e.vehicle_index() & 0xFF));
  // A host record (E5 step 4) is copied with each field swapped.
  if (!memory::CopyWithHandles(to, frame + 96, 20)) {
    for (uint32_t k = 0; k < 20; k += 4) {
      guest::StoreU32(to + k, guest::LoadU32(frame + 96 + k));
    }
  }
  call(kBodySetUp, {v.status_address(), vehicle});
  call(kPhysicsStep, {vehicle}, {double(guest::LoadFloat(kStepTime))});
  call(kStatusUpdate, {v.status_address(), vehicle, 0, 1});
  e.set_flags(e.flags() | 0x240);
  guest::StoreU32(slot, 1);
  return 1;
}

}  // namespace race_world
