// The stage's small setters (docs/functions/stand-models.md), step for step
// as the translated code (see stand_models.h).

#include "stand_models/stand_models.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "guest.h"
#include "scene_models/apply.h"
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace stand_models {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kSetUpB = 0x820C5B28;
constexpr uint32_t kApplyRecord = 0x820DA758;
constexpr uint32_t kDecalScale = 0x82001BE0;

uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// SceneModel::ApplyRecord from `frame`: its plain C++, or where it declines
// (the race's path) its adapter, which runs the original.
uint64_t ApplyRecord(uint8_t* base, const GuestCall& call, uint32_t model, uint32_t frame,
                     uint32_t lr) {
  if (const auto r3 = scene_models::SceneModelApplyRecord(base, call, model, frame)) {
    return *r3;
  }
  return call(kApplyRecord, {model}, frame, lr);
}

}  // namespace

uint32_t StageModel(uint8_t* base, uint32_t holder) {
  const uint32_t stage = Load(base, holder);
  return vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7, Load(base, stage + 8));
}

uint64_t MakeModel(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t block,
                   uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217FCB0 GarageStage::MakeModel");
  const uint32_t frame = Frame(base, stack, 4384);
  const uint32_t holder = uint32_t(r3);
  const uint32_t scene = Load(base, kScene);
  if (Load(base, Load(base, holder) + 16) != 0) {
    return r3;
  }
  const uint32_t record = frame + 80;
  scene_models::SceneModelRecordConstruct(base, record);
  scene_models::SceneModelRecordReset(base, record);
  Store(base, frame + 96, block);
  const int32_t index = vehicle_accessors::ScenePoolsClaimB(base, scene, 7);
  Store(base, Load(base, holder) + 8, uint32_t(index));
  const uint32_t at = Load(base, Load(base, holder) + 8);
  const uint32_t model =
      uint32_t(call(kSetUpB, {scene, 7, at, record}, frame, 0x8217FD2C));
  Store(base, model + 4296, word);
  Store(base, Load(base, holder) + 6056, word);
  Store(base, Load(base, holder) + 16, 1);
  Store(base, Load(base, kHolder) + 20, 1);
  return model;
}

uint32_t SetInteriorClass(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178FE8 GarageStage::SetInteriorClass");
  Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  const int32_t v = int32_t(value);
  Store(base, model + 3808, v >= 7 ? 2 : v >= 3 ? 1 : 0);
  return model;
}

uint32_t SetWord5016(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179598 GarageStage::SetWord5016");
  Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  Store(base, model + 5016, value);
  return model;
}

uint32_t SetWheelWord(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179A18 GarageStage::SetWheelWord");
  Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  Store(base, model + 4076, value);
  return model;
}

double LightValue(uint8_t* base, uint32_t points) {
  ITC_NATIVE_RUNNING("sub_820DF1A0 SceneParts::LightValue");
  return Lfs(base, points + 27664);
}

uint32_t SetLightsOn(uint8_t* base, uint32_t holder, uint32_t flag, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_82179420 GarageStage::SetLightsOn");
  Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  if (flag != 0) {
    Store(base, model + 4976, 1);
    Stfs(base, model + 4980, Lfs(base, kZero));
  } else {
    Store(base, model + 4976, 0);
    const uint32_t points = Load(base, model + 5060) + 176;
    const double value = LightValue(base, points);
    Stfs(base, model + 4980, value);
    *f1 = value;
    return points;  // r3 as LightValue (a leaf) leaves it
  }
  return model;
}

uint32_t SetDecal(uint8_t* base, uint32_t holder, uint32_t entry, uint32_t a, uint32_t b,
                  double f1, double f2, double f3, double f4, uint32_t stack, uint32_t at) {
  if (at == kDecalA) {
    ITC_NATIVE_RUNNING("sub_82179818 GarageStage::SetDecalA");
  } else {
    ITC_NATIVE_RUNNING("sub_821798D8 GarageStage::SetDecalB");
  }
  const uint32_t frame = Frame(base, stack, 160);
  Store(base, frame + 80, 2);
  Store(base, frame + 84, 1);
  Store(base, frame + 88, 0);
  const uint32_t model = StageModel(base, holder);
  // The table read unchecked, as the original (rlwinm: the index's low 30
  // bits times 4, wrapping).
  const uint32_t word = Load(base, frame + 80 + (entry << 2));
  const uint32_t e = model + (word << 6);
  Stfs(base, e + at + 16, f1);
  Store(base, e + at + 0, a);
  Stfs(base, e + at + 20, f2);
  Store(base, e + at + 4, b);
  Stfs(base, e + at + 32, f3);
  Stfs(base, e + at + 52, f4);
  Stfs(base, e + at + 48, b != 0 ? Fmuls(f4, Lfs(base, kDecalScale)) : f4);
  return model;
}

uint64_t SetPart(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t slot,
                 uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178F00 GarageStage::SetPart");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  // The jump table (case 0 the split piece sub_82178F68): the parts
  // record's words, none for 13.
  static constexpr uint32_t kAt[16] = {3744, 3748, 3752, 3756, 3760, 3764, 3768, 3768,
                                       3772, 3772, 3776, 3780, 3784, 0,    3788, 3792};
  if (slot <= 15 && kAt[slot] != 0) {
    Store(base, model + kAt[slot], word);
  }
  return ApplyRecord(base, call, model, frame, 0x82178FD0);
}

uint64_t SetInteriorWord(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t kind,
                         uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179050 GarageStage::SetInteriorWord");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  const int32_t k = int32_t(kind);
  if (k == 1) {
    Store(base, model + 3908, word);
  } else if (k == 4) {
    Store(base, model + 3900, word);
  } else if (k == 5) {
    Store(base, model + 3896, word);
  }
  return ApplyRecord(base, call, model, frame, 0x821790B8);
}

uint64_t SetInteriorLevel(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t value,
                          uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821791C0 GarageStage::SetInteriorLevel");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  const int32_t v = int32_t(value);
  Store(base, model + 3908, v < 3 ? 0 : v < 6 ? 1 : 2);
  Store(base, model + 3912, v < 1 ? 0 : 1);
  return ApplyRecord(base, call, model, frame, 0x82179238);
}

uint64_t SetInteriorPair(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t a,
                         uint32_t b, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179250 GarageStage::SetInteriorPair");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  Store(base, model + 3904, a);
  Store(base, model + 3900, b);
  return ApplyRecord(base, call, model, frame, 0x82179294);
}

uint64_t ClearChoice(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t choice,
                     uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179378 GarageStage::ClearChoice");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  Store(base, choice * 12 + model + 3812, 0);
  return ApplyRecord(base, call, model, frame, 0x821793BC);
}

uint64_t SetAero(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t word,
                 uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179548 GarageStage::SetAero");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  Store(base, model + 3728, word);
  return ApplyRecord(base, call, model, frame, 0x82179580);
}

uint64_t ReloadDecals(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178EC0 GarageStage::ReloadDecals");
  const uint32_t frame = Frame(base, stack, 96);
  return scene_models::SceneModelReloadDecals(base, call, StageModel(base, holder), frame);
}

}  // namespace stand_models
