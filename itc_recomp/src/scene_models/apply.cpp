// The scene model's record re-applies (docs/functions/scene-models.md), step
// for step as the translated code (see apply.h).

#include "scene_models/apply.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/load_poll.h"
#include "schema/scene_models.h"
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace scene_models {

namespace {

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kPollLoad = 0x820DA318;
constexpr uint32_t kSetRecord = 0x820D3C70;
constexpr uint32_t kPartsBuild = 0x820E05E0;
constexpr uint32_t kSelectedTexture = 0x820D4588;
constexpr uint32_t kSetGauges = 0x820E3938;
constexpr uint32_t kReloadDecals = 0x820D5D20;
constexpr uint32_t kSetUpB = 0x820C5B28;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

void Memcpy(uint8_t* base, const ApplyCall& call, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t frame, uint32_t lr) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    call(kMemcpy, {to, from, bytes}, frame, lr);
  }
}

}  // namespace

uint64_t ScenePartsApply(uint8_t* base, const ApplyCall& call, uint32_t parts, uint32_t record,
                         uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E08B0 SceneParts::Apply");
  const uint32_t frame = Frame(base, stack, 96);
  if (int32_t(Load(base, Load(base, parts + 37284) + 4)) == 1) {
    return parts;
  }
  Memcpy(base, call, parts + 108, record, 60, frame, 0x820E08E8);
  return call(kPartsBuild, {parts}, frame, 0x820E08F0);
}

uint64_t SceneInteriorApply(uint8_t* base, const ApplyCall& call, uint32_t interior,
                            uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E4258 SceneInterior::Apply");
  const uint32_t frame = Frame(base, stack, 112);
  Memcpy(base, call, interior + 40, record, 120, frame, 0x820E4278);
  Memcpy(base, call, Load(base, interior + 36) + 28, record, 120, frame, 0x820E428C);
  const uint32_t part = Load(base, interior + 140);
  if (part != 0) {
    Store(base, interior + 9656, Load(base, interior + 11668) + part * 464 + 784);
  }
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t car = Load(base, Load(base, interior + 24) + 24);
    const uint32_t texture =
        uint32_t(call(kSelectedTexture, {car}, frame, 0x820E42C0));
    Store(base, Load(base, interior + 11632 + 4 * i) + 452, texture);
  }
  return call(kSetGauges, {interior}, frame, 0x820E42DC);
}

std::optional<uint64_t> SceneModelApplyRecord(uint8_t* base, const ApplyCall& call,
                                              uint32_t model, uint32_t stack) {
  const schema::SceneModel m(base, model);
  // The scene's +4: 0 the front end.
  if (Load(base, m.scene().address + 4) != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820DA758 SceneModel::ApplyRecord");
  const uint32_t frame = Frame(base, stack, 144);
  uint64_t r3 = call(kPollLoad, {model}, frame, 0x820DA76C);
  if (int32_t(uint32_t(r3)) == 0) {
    return r3;
  }
  if (int32_t(m.block()) == 5) {
    m.set_interior_gauge1_kind(0);
    m.set_interior_gauge1_present(1);
  }
  // The car model's +24: the first set's car model.
  const uint32_t car = Load(base, m.car_model().address + 24);
  call(kSetRecord, {car, m.car_record_address()}, frame, 0x820DA7B0);
  ScenePartsApply(base, call, m.parts().address, m.parts_record_address(), frame);
  r3 = SceneInteriorApply(base, call, m.interior(), m.interior_record_address(), frame);
  if (int32_t(m.kind()) != 2) {
    return r3;
  }
  // Kind 2: the cockpit is rebuilt only on the race's path (the scene's +4
  // 1), which isn't this one; r3 the scene, as the original loads it.
  return uint64_t(m.scene().address);
}

uint64_t SceneModelReloadDecals(uint8_t* base, const ApplyCall& call, uint32_t model,
                                uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9D48 SceneModel::ReloadDecals");
  const uint32_t frame = Frame(base, stack, 96);
  const schema::SceneModel m(base, model);
  if (m.ready() == 0 || int32_t(m.kind()) == 3) {
    return model;
  }
  m.set_ready(0);
  m.set_decals_reloaded(0);
  // The car model's +24 (the first set's car model), read for each call.
  call(kSetRecord, {Load(base, m.car_model().address + 24), m.car_record_address()}, frame,
       0x820D9D90);
  return call(kReloadDecals, {Load(base, m.car_model().address + 24)}, frame, 0x820D9D9C);
}

uint64_t SceneModelAddSecondWheels(uint8_t* base, const ApplyCall& call, uint32_t model,
                                   uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9E00 SceneModel::AddSecondWheels");
  const uint32_t frame = Frame(base, stack, 112);
  const schema::SceneModel m(base, model);
  if (m.second_wheels_claimed() != 0) {
    return 0;
  }
  m.set_second_wheels_claimed(1);
  m.set_second_wheels_ready(0);
  const int32_t index = vehicle_accessors::ScenePoolsClaimB(base, m.scene().address, 5);
  const uint32_t wheels =
      vehicle_accessors::ScenePoolsGetB(base, m.scene().address, 5, uint32_t(index));
  m.set_second_wheels(wheels);
  SetCarModel(base, wheels, m.car_model().address);
  call(kSetUpB, {m.scene().address, 5, uint64_t(int64_t(index)), m.wheels_record_address()},
       frame, 0x820D9E80);
  return 1;
}

}  // namespace scene_models
