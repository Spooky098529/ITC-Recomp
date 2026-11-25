// The car scene's set-up from a record (see scene_fill.h).

#include "car_models/scene_fill.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "memory/heap.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/load_poll.h"
#include "schema/scene_loaders.h"
#include "schema/scene_models.h"
#include "schema/second_set.h"
#include "scene/scene.h"
#include "second_set/objects.h"

namespace car_models {

namespace {

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

using G = schema::CarModelsGlobals;

}  // namespace

uint32_t SceneSetUp(uint32_t scene, uint32_t record, uint32_t r5, uint32_t sp,
                    uint32_t pcr, const archive::LoadCall& load,
                    const scene_loaders::CreateCalls& create) {
  ITC_NATIVE_RUNNING("sub_820C8DE8 CarScene::SetUp");
  const uint32_t frame = sp - kSceneSetUpFrame;
  Store(frame, sp);
  const schema::CarSceneSetUpRecord rec(record);
  const schema::CarScene s(scene);
  s.set_set_up_r5(r5);
  s.set_state(3);
  s.set_model_set(rec.model_set());
  s.set_car_model_count(rec.car_models());
  s.set_parts_count(rec.parts());
  s.set_interior_count(rec.interiors() + rec.cockpits() + rec.traffic());
  s.set_cockpit_count(rec.cockpits());
  s.set_light_kit_count(rec.pool4_objects());
  s.set_wheels_count(rec.wheels() + rec.traffic() + rec.models());
  s.set_scene_model_count(rec.traffic() + rec.models());
  s.set_traffic_count(rec.traffic());
  s.set_own_load_buffer(rec.load_buffer());
  s.set_own_load_buffer_size(rec.load_buffer_size());
  material_passes::PaintPaletteInit(frame);
  if (rec.load_buffer() == 0) {
    s.set_load_buffer(G::archive_buffer());
    s.set_load_buffer_size(G::archive_buffer_size());
  } else {
    const uint32_t buffer = s.own_load_buffer(), size = s.own_load_buffer_size();
    s.set_load_buffer(buffer);
    s.set_load_buffer_size(size);
  }
  scene::LoadAssets(scene, pcr, load);
  // The counts in the scene, by pool.
  using Count = uint32_t (schema::CarScene::*)() const;
  static constexpr Count kCounts[8] = {
      &schema::CarScene::car_model_count,  &schema::CarScene::parts_count,
      &schema::CarScene::cockpit_count,    &schema::CarScene::interior_count,
      &schema::CarScene::light_kit_count,  &schema::CarScene::wheels_count,
      &schema::CarScene::traffic_count,    &schema::CarScene::scene_model_count};
  using Construct = uint32_t (*)(uint8_t*, uint32_t, uint32_t, uint32_t);
  static constexpr Construct kConstructs[8] = {
      second_set::SceneCarModelConstruct, second_set::ScenePartsConstruct,
      second_set::SceneCockpitConstruct,  second_set::SceneInteriorConstruct,
      second_set::SceneLightsConstruct,   second_set::SceneWheelsConstruct,
      second_set::SceneTrafficConstruct,  second_set::SceneModelConstruct};
  for (uint32_t pool = 0; pool < 8; pool++) {
    const int32_t count = int32_t((s.*kCounts[pool])());
    for (int32_t i = 0; i < count; i++) {
      // Pool 1's parts objects in host memory (E5 step 4: a handle, freed by
      // their deleting destructor's operator delete), and pools 0, 2, 3 and 6
      // (the car models, cockpit bodies, interiors and traffic bodies: freed
      // by ScenePools::DestroyB's VirtualFree).
      uint32_t object;
      switch (pool) {
        case 0:
          object = schema::SceneCarModel::New().address;
          break;
        case 1:
          object = schema::SceneParts::New().address;
          break;
        case 2:
          object = schema::SceneCockpit::New().address;
          break;
        case 3:
          object = schema::SceneInterior::New().address;
          break;
        case 6:
          object = schema::SceneTraffic::New().address;
          break;
        case 5:
          // The wheels (E5 step 5): a handle, freed by DestroyB's VirtualFree
          // as the others.
          object = schema::SceneWheels::New().address;
          break;
        case 7:
          // The scene models (E5 step 5): handles, freed by DestroyB's
          // VirtualFree as the others.
          object = schema::SceneModel::New().address;
          break;
        default:
          object = memory::AllocPages(kSecondSetBytes[pool], 4096);
          break;
      }
      if (object != 0) {
        kConstructs[pool](object, s.car_models_first_address() + 12 * pool, frame);
      }
      schema::PoolNode(object).set_index(uint32_t(i));
    }
  }
  return scene_loaders::CreateA(s.first_set(), record, create);
}

}  // namespace car_models
