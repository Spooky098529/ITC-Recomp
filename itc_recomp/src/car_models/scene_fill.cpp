// The car scene's set-up from a record (see scene_fill.h).

#include "car_models/scene_fill.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "memory/heap.h"
#include "schema/car_models.h"
#include "scene/scene.h"
#include "second_set/objects.h"

namespace car_models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

using G = schema::CarModelsGlobals;

}  // namespace

uint32_t SceneSetUp(uint8_t* base, uint32_t scene, uint32_t record, uint32_t r5, uint32_t sp,
                    uint32_t pcr, const archive::LoadCall& load,
                    const scene_loaders::CreateCalls& create) {
  ITC_NATIVE_RUNNING("sub_820C8DE8 CarScene::SetUp");
  const uint32_t frame = sp - kSceneSetUpFrame;
  Store(base, frame, sp);
  const schema::CarSceneSetUpRecord rec(base, record);
  Store(base, scene + 5044, r5);
  Store(base, scene + 0, 3);
  Store(base, scene + 4, rec.model_set());
  Store(base, scene + 8, rec.car_models());
  Store(base, scene + 12, rec.parts());
  Store(base, scene + 16, rec.interiors() + rec.cockpits() + rec.traffic());
  Store(base, scene + 20, rec.cockpits());
  Store(base, scene + 24, rec.pool4_objects());
  Store(base, scene + 28, rec.wheels() + rec.traffic() + rec.models());
  Store(base, scene + 32, rec.traffic() + rec.models());
  Store(base, scene + 36, rec.traffic());
  Store(base, scene + 40, rec.load_buffer());
  Store(base, scene + 44, rec.load_buffer_size());
  material_passes::PaintPaletteInit(base, frame);
  if (rec.load_buffer() == 0) {
    Store(base, scene + 5056, G::archive_buffer(base));
    Store(base, scene + 5060, G::archive_buffer_size(base));
  } else {
    const uint32_t buffer = Load(base, scene + 40), size = Load(base, scene + 44);
    Store(base, scene + 5056, buffer);
    Store(base, scene + 5060, size);
  }
  scene::LoadAssets(base, scene, pcr, load);
  // The counts' offsets in the scene, by pool.
  static constexpr uint32_t kCounts[8] = {8, 12, 20, 16, 24, 28, 36, 32};
  using Construct = uint32_t (*)(uint8_t*, uint32_t, uint32_t, uint32_t);
  static constexpr Construct kConstructs[8] = {
      second_set::SceneCarModelConstruct, second_set::ScenePartsConstruct,
      second_set::SceneCockpitConstruct,  second_set::SceneInteriorConstruct,
      second_set::SceneLightsConstruct,   second_set::SceneWheelsConstruct,
      second_set::SceneTrafficConstruct,  second_set::SceneModelConstruct};
  for (uint32_t pool = 0; pool < 8; pool++) {
    const int32_t count = int32_t(Load(base, scene + kCounts[pool]));
    for (int32_t i = 0; i < count; i++) {
      const uint32_t object = memory::AllocPages(kSecondSetBytes[pool], 4096);
      if (object != 0) {
        kConstructs[pool](base, object, scene + 4944 + 12 * pool, frame);
      }
      Store(base, object + 20, uint32_t(i));
    }
  }
  return scene_loaders::CreateA(base, Load(base, scene + 5040), record, create);
}

}  // namespace car_models
