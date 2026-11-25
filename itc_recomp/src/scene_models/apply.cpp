// The scene model's record re-applies (docs/functions/scene-models.md), step
// for step as the translated code (see apply.h).

#include "scene_models/apply.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/load_poll.h"
#include "schema/material_passes.h"
#include "schema/scene_loaders.h"
#include "schema/scene_models.h"
#include "schema/second_set.h"
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

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(frame, stack);
  return frame;
}

void Memcpy(const ApplyCall& call, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t frame, uint32_t lr) {
  if (!native::Buffers_Memcpy(to, from, bytes, lr)) {
    call(kMemcpy, {to, from, bytes}, frame, lr);
  }
}

}  // namespace

uint64_t ScenePartsApply(const ApplyCall& call, uint32_t parts, uint32_t record,
                         uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E08B0 SceneParts::Apply");
  const schema::SceneParts p(parts);
  if (int32_t(p.scene().model_set()) == 1) {
    return parts;
  }
  // The original's frame (only its back chain), for the guest calls below;
  // not made on the early return, which calls nothing (E5 step 6).
  const uint32_t frame = Frame(stack, 96);
  Memcpy(call, p.record_address(), record, schema::PartsRecord::kSize, frame, 0x820E08E8);
  return call(kPartsBuild, {parts}, frame, 0x820E08F0);
}

uint64_t SceneInteriorApply(const ApplyCall& call, uint32_t interior,
                            uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E4258 SceneInterior::Apply");
  const uint32_t frame = Frame(stack, 112);
  const schema::SceneInterior in(interior);
  constexpr uint32_t kBytes = schema::InteriorRecord::kSize;
  Memcpy(call, in.record_address(), record, kBytes, frame, 0x820E4278);
  Memcpy(call, in.first_set_interior().record_address(), record, kBytes, frame,
         0x820E428C);
  const uint32_t part = in.seat_model();
  if (part != 0) {
    // The scene's seat part models (+784, 464 bytes each).
    in.set_points_seat_model(in.scene().address + part * schema::PartModel::kSize + 784);
  }
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t car = in.car_model().car_model().address;
    const uint32_t texture =
        uint32_t(call(kSelectedTexture, {car}, frame, 0x820E42C0));
    schema::PartModel(Load(in.part_models_address() + 4 * i))
        .set_category3_texture(texture);
  }
  return call(kSetGauges, {interior}, frame, 0x820E42DC);
}

std::optional<uint64_t> SceneModelApplyRecord(const ApplyCall& call,
                                              uint32_t model, uint32_t stack) {
  const schema::SceneModel m(model);
  // 0 the front end.
  if (m.scene().model_set() != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820DA758 SceneModel::ApplyRecord");
  const uint32_t frame = Frame(stack, 144);
  uint64_t r3 = call(kPollLoad, {model}, frame, 0x820DA76C);
  if (int32_t(uint32_t(r3)) == 0) {
    return r3;
  }
  if (int32_t(m.block()) == 5) {
    m.set_interior_gauge1_kind(0);
    m.set_interior_gauge1_present(1);
  }
  const uint32_t car = m.car_model().car_model().address;
  call(kSetRecord, {car, m.car_record_address()}, frame, 0x820DA7B0);
  ScenePartsApply(call, m.parts().address, m.parts_record_address(), frame);
  r3 = SceneInteriorApply(call, m.interior(), m.interior_record_address(), frame);
  if (int32_t(m.kind()) != 2) {
    return r3;
  }
  // Kind 2: the cockpit is rebuilt only on the race's path (the scene's +4
  // 1), which isn't this one; r3 the scene, as the original loads it.
  return uint64_t(m.scene().address);
}

uint64_t SceneModelReloadDecals(const ApplyCall& call, uint32_t model,
                                uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9D48 SceneModel::ReloadDecals");
  const schema::SceneModel m(model);
  if (m.ready() == 0 || int32_t(m.kind()) == 3) {
    return model;
  }
  // The original's frame (only its back chain), for the guest calls below
  // (E5 step 6: not made on the early return).
  const uint32_t frame = Frame(stack, 96);
  m.set_ready(0);
  m.set_decals_reloaded(0);
  // The first set's car model, read for each call.
  call(kSetRecord, {m.car_model().car_model().address, m.car_record_address()}, frame,
       0x820D9D90);
  return call(kReloadDecals, {m.car_model().car_model().address}, frame, 0x820D9D9C);
}

uint64_t SceneModelAddSecondWheels(const ApplyCall& call, uint32_t model,
                                   uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9E00 SceneModel::AddSecondWheels");
  const schema::SceneModel m(model);
  if (m.second_wheels_claimed() != 0) {
    return 0;
  }
  // The original's frame (only its back chain), for the guest call below
  // (E5 step 6: not made on the early return).
  const uint32_t frame = Frame(stack, 112);
  m.set_second_wheels_claimed(1);
  m.set_second_wheels_ready(0);
  const int32_t index = vehicle_accessors::ScenePoolsClaimB(m.scene().address, 5);
  const uint32_t wheels =
      vehicle_accessors::ScenePoolsGetB(m.scene().address, 5, uint32_t(index));
  m.set_second_wheels(wheels);
  SetCarModel(wheels, m.car_model().address);
  call(kSetUpB, {m.scene().address, 5, uint64_t(int64_t(index)), m.wheels_record_address()},
       frame, 0x820D9E80);
  return 1;
}

}  // namespace scene_models
