// The second set's objects' lifetimes (see objects.h), store for store as
// the recompiled functions.

#include "second_set/objects.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "native/buffers.h"
#include "scene/scene.h"
#include "scene_loaders/objects.h"
#include "scene_models/scene_models.h"
#include "second_set/blocks.h"

#include <bit>
#include <cstdlib>

namespace second_set {

namespace {

using car_physics::ops::CopyWord;
using car_physics::ops::kOne;
using car_physics::ops::kZero;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    base[address + i] = 0;
  }
}

// The identity (vupkd3d128's rows, stored aligned) at `at`.
void Identity(uint8_t* base, uint32_t at) {
  const uint32_t aligned = at & ~0xFu;
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t w = 0; w < 16; w++) {
    Store(base, aligned + 4 * w, w % 5 == 0 ? one : 0);
  }
}

void Memcpy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, caller)) {
    ITC_LOG_ERROR("Second set: an object's memcpy refused after its checks");
    std::abort();
  }
}

}  // namespace

std::optional<uint32_t> SceneCarModelRelease(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB6C0 SceneCarModel::Release");
  (void)stack;
  const uint32_t car_model = Load(base, object + 24);
  if (car_model != 0) {
    if (!scene_loaders::ReleaseA(base, Load(base, scene::kFirstSet), 0,
                                 Load(base, car_model + 20))) {
      return std::nullopt;
    }
  }
  const uint32_t r3 = scene_models::CarRecordReset(base, object + 32);
  Store(base, object + 24, 0);
  Store(base, object + 3728, 0);
  return r3;
}

uint32_t ScenePartsReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E0510 SceneParts::Reset");
  const uint32_t sp = stack - 96;
  scene_models::PartsRecordReset(base, object + 108);
  PartsPointsReset(base, object + 176, sp);
  Store(base, object + 24, 0);
  Store(base, object + 37284, 0);
  Store(base, object + 28, 0);
  Store(base, object + 104, 0);
  Store(base, object + 37280, 0);
  Store(base, object + 100, 0);
  Store(base, object + 37284, Load(base, scene::kScene));
  Clear(base, object + 32, 68);
  return object + 32;
}

uint32_t SceneCockpitReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DBE20 SceneCockpit::Reset");
  const uint32_t sp = stack - 112;
  Store(base, object + 24, 0);
  Store(base, object + 28, 0);
  Store(base, object + 32, 0);
  Store(base, object + 4312, 0);
  Store(base, object + 4316, Load(base, scene::kScene));
  scene_models::CockpitRecordReset(base, object + 36);
  const uint32_t r3 = CockpitPointsReset(base, object + 192, sp);
  Store(base, object + 4272, object + 36);
  for (uint32_t at = 4288; at < 4312; at += 4) {
    Store(base, object + at, 0);
  }
  return r3;
}

uint32_t SceneInteriorReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3748 SceneInterior::Reset");
  const uint32_t sp = stack - 112;
  for (const uint32_t at : {24u, 28u, 32u, 36u, 11656u, 11660u, 11664u}) {
    Store(base, object + at, 0);
  }
  scene_models::InteriorRecordReset(base, object + 40);
  const uint32_t r3 = InteriorPointsReset(base, object + 160, sp);
  Store(base, object + 11668, Load(base, scene::kScene));
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, object + 11632 + 4 * i, 0);
  }
  return r3;
}

uint32_t SceneLightsReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DCA40 SceneLights::Reset");
  const uint32_t sp = stack - 112;
  for (const uint32_t at : {24u, 28u, 13152u, 13156u}) {
    Store(base, object + at, 0);
  }
  scene_models::Pool4RecordReset(base, object + 32);
  const uint32_t block = object + 96;
  const uint32_t r3 = LightPointsReset(base, block, sp);
  Store(base, object + 13160, Load(base, scene::kScene));
  for (const uint32_t at : {0u, 48u, 4384u, 8720u}) {
    Store(base, block + at, object + 32);
  }
  return r3;
}

uint32_t SceneTrafficReset(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5930 SceneTraffic::Reset");
  const uint32_t sp = stack - 96;
  scene_models::CarRecordReset(base, object + 32);
  const uint32_t r3 = TrafficPointsReset(base, object + 3728, sp);
  Store(base, object + 9764, 0);
  Store(base, object + 9760, 0);
  Store(base, object + 9792, 0);
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, object + 9768 + 4 * i, 0);
  }
  return r3;
}

void SceneModelBlockConstruct(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820DA108 SceneModelBlock::SceneModelBlock");
  for (const uint32_t at : {28u, 44u, 60u, 76u}) {
    CopyWord(base, kOne, block + at);
  }
  for (const uint32_t from : {80u, 336u}) {
    for (uint32_t m = 0; m < 4; m++) {
      for (uint32_t row = 0; row < 4; row++) {
        CopyWord(base, kOne, block + from + 64 * m + 16 * row + 12);
      }
    }
  }
  CopyWord(base, kOne, block + 668);
  CopyWord(base, kOne, block + 684);
}

uint32_t SceneModelBlockReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9F50 SceneModelBlock::Reset");
  const uint32_t f = stack - kSceneModelBlockResetFrameBytes;
  Store(base, block + 8, 0);
  Store(base, block + 12, 0);
  Store(base, block + 0, 0);
  Store(base, block + 4, 0);
  Identity(base, f + 80);
  Memcpy(base, block + 16, f + 80, 64, 0x820D9FC4);
  Store(base, block + 592, 0);
  uint32_t r3 = block + 16;
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t matrix = block + 80 + 64 * k;
    Identity(base, f + 80);
    Memcpy(base, matrix + 256, f + 80, 64, 0x820DA038);
    Identity(base, f + 144);
    Memcpy(base, matrix, f + 144, 64, 0x820DA068);
    r3 = matrix;
    CopyWord(base, kZero, block + 612 + 4 * k);
    Store(base, block + 596 + 4 * k, 2);
  }
  CopyWord(base, kZero, block + 640);
  CopyWord(base, 0x8200296C, block + 628);
  CopyWord(base, 0x82002A10, block + 632);
  CopyWord(base, 0x82002208, block + 636);
  for (uint32_t at = 656; at <= 684; at += 4) {
    CopyWord(base, kZero, block + at);
  }
  Store(base, block + 688, 0);
  CopyWord(base, kZero, block + 692);
  Store(base, block + 724, 0);
  CopyWord(base, kZero, block + 696);
  Store(base, block + 728, 0);
  CopyWord(base, kZero, block + 700);
  Store(base, block + 732, 1);
  return r3;
}

}  // namespace second_set
