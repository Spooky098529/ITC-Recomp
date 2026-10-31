// The race scene's object (RaceScene::, docs/functions/race-scene.md) as
// plain C++ over guest memory: its making. See race_scene.h.

#include "race_scene/race_scene.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "scene_models/scene_models.h"

namespace race_scene {

namespace {

constexpr uint32_t kOne = 0x82000A30;  // 1.0 (as the image holds it)

// lfs and stfs: a float word through a double, as the translated code.
double Lfs(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

}  // namespace

uint32_t SceneConstruct(uint8_t* base, uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_8215D6C8 RaceScene::RaceScene");
  for (uint32_t i = 0; i < 7; i++) {
    scene_models::SceneModelRecordConstruct(base, scene + 48 + 4288 * i);
  }
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 11; i++) {
    const uint32_t at = scene + 30080 + 44 + 96 * i;
    Stfs(base, at - 16, one);
    Stfs(base, at + 0, one);
    Stfs(base, at + 16, one);
    Stfs(base, at + 32, one);
    Stfs(base, at + 48, one);
  }
  return scene;
}

bool SceneCreate(uint8_t* base) {
  guest::StoreU32(base, kScene, 0);
  const auto block = memory::crt::OperatorNew(base, kSceneBytes);
  if (!block) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8215D738 RaceScene::Create");
  guest::StoreU32(base, kScene, *block != 0 ? SceneConstruct(base, *block) : 0);
  return true;
}

}  // namespace race_scene
