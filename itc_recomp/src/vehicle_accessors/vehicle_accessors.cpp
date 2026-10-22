#include "vehicle_accessors/vehicle_accessors.h"

#include "guest.h"

#include "base/log.h"

namespace vehicle_accessors {

namespace {

constexpr uint32_t kVehicles = 0x826C2E34;   // the vehicle array's pointer
constexpr uint32_t kSaveData = 0x826C5B58;   // the players' save data pointers
constexpr uint32_t kVehicleBytes = 6272;

}  // namespace

uint32_t VehicleGet(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C0468 Vehicle::Get");
  const uint32_t vehicles = guest::LoadU32(base, kVehicles);
  if (vehicles == 0) {
    return 0;
  }
  return vehicles + kVehicleBytes * index;
}

uint32_t VehicleWheelContact(uint32_t vehicle, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_820C0170 Vehicle::WheelContact");
  return vehicle + 5328 + 96 * wheel;
}

uint32_t RigidBodyPosition(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B67D8 RigidBody::Position");
  return body + 208;
}

uint32_t RigidBodyMatrix(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6930 RigidBody::Matrix");
  return body + 288;
}

uint32_t ContactObject(uint8_t* base, uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8CC0 Contact::Object");
  if (guest::LoadU32(base, contact + 4) & 0xFF000000u) {
    return 0;
  }
  return guest::LoadU32(base, contact);
}

uint32_t ContactPosition(uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8D00 Contact::Position");
  return contact + 16;
}

float ContactStrength(uint8_t* base, uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8D08 Contact::Strength");
  return guest::LoadFloat(base, contact + 64);
}

int32_t RacersIndexOf(uint8_t* base, uint32_t records, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820FF9F8 Racers::IndexOf");
  for (uint32_t i = 0; i < 93; i++) {
    if (guest::LoadU32(base, records + 3912 + 3936 * i) == id) {
      return int32_t(i);
    }
  }
  return -1;
}

uint32_t StagesHasRacer(uint8_t* base, uint32_t stages, uint32_t racer, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFCF8 Stages::HasRacer");
  if (int32_t(stage) == -1) {
    stage = guest::LoadU32(base, stages + 804);
  }
  const uint32_t slots = stages + 80 * stage + 44;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t slot = guest::LoadU32(base, slots + 8 * i);
    if (int32_t(slot) == -1) {
      return 0;
    }
    if (slot == racer) {
      return 1;
    }
  }
  return 0;
}

uint32_t SaveDataOf(uint8_t* base, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_82105620 SaveData::Of");
  return guest::LoadU32(base, kSaveData + 4 * player);
}

uint32_t DebugSwitchOff() {
  ITC_NATIVE_RUNNING("sub_825C8388 DebugSwitchOff");
  return 0;
}

}  // namespace vehicle_accessors
