// The car scene (see scene.h), load for load and store for store as the
// recompiled functions.

#include "scene/scene.h"

#include "accessories/accessories.h"
#include "base/log.h"
#include "car_models/load_slots.h"
#include "car_models/tga.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
#include "models/xpr.h"
#include "schema/accessories.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/material_passes.h"
#include "schema/models.h"
#include "schema/scene.h"
#include "schema/scene_loaders.h"
#include "schema/scene_models.h"
#include "schema/second_set.h"
#include "schema/vehicle_accessors.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/pools.h"
#include "scenery_lists/scenery_lists.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cstdlib>
#include <unordered_set>
#include <vector>

namespace scene {

namespace {

using Globals = schema::SceneGlobals;
using schema::CarScene;
using schema::LoadSlot;
using schema::PoolList;
using schema::PoolNode;
using schema::ScenePools;

// The accessories' set (sub_820F0E50).
uint32_t AccessoriesSet() { return schema::AccessoriesGlobals::set().address; }

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// A member whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene: {} refused after its checks", what);
    std::abort();
  }
}

constexpr uint32_t kFirstSetPools = 7;
constexpr uint32_t kMaxNodes = 4096;

}  // namespace

// --- The small pieces ---

void SetUpConstruct(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D0D50 SceneSetUp::SceneSetUp");
  const schema::CarSceneSetUpRecord set_up(record);
  set_up.set_load_buffer(0);
  set_up.set_load_buffer_size(0);
}

std::optional<uint32_t> FreePagesIn(uint32_t at) {
  ITC_NATIVE_RUNNING("sub_820D09A0 FreePagesIn");
  const uint32_t pages = schema::ScenePagesWord(at).pages();
  if (pages == 0 || !memory::IsPages(pages)) {
    return std::nullopt;
  }
  memory::FreePages(pages);
  return 1;
}

void StartTeardownA() {
  ITC_NATIVE_RUNNING("sub_820C3C80 ScenePools::StartTeardownA");
  Globals::first_set().set_state(4);
  Globals::first_set().set_teardown_count(2);
}

std::optional<uint64_t> StartTeardown(const std::function<uint64_t(uint32_t set)>& release_all) {
  ITC_NATIVE_RUNNING("sub_820C4D88 CarScene::StartTeardown");
  if (Globals::scene().address == 0) {
    return std::nullopt;
  }
  Globals::scene().set_state(4);
  Globals::scene().set_teardown_count(2);
  StartTeardownA();
  return release_all(AccessoriesSet());
}

std::optional<uint32_t> SetInteriorWord(uint32_t scene, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_820C8950 CarScene::SetInteriorWord");
  const CarScene car_scene(scene);
  const int32_t count = int32_t(car_scene.scene_models_count());
  std::optional<uint32_t> r3;
  for (int32_t i = 0; i < count; i++) {
    const uint32_t model =
        vehicle_accessors::ListAt(car_scene.scene_models_first_address(), uint32_t(i));
    r3 = model;
    const schema::SceneModel scene_model(model);
    if (scene_model.interior_ready() != 0) {
      schema::SceneInterior(scene_model.interior()).set_points_accessory_shown(word);
    }
  }
  return r3;
}

uint32_t LoadsDone(uint32_t scene, uint32_t sp, const FinishCall& finish) {
  ITC_NATIVE_RUNNING("sub_820C5798 CarScene::LoadsDone");
  // The original's 128-byte frame (only its back chain) left out: the
  // finishers' guest calls run from `sp` (E5 step 6).
  const uint32_t frame = sp;
  const CarScene car_scene(scene);
  const uint32_t states[8] = {
      car_scene.asset141_state_address(),     car_scene.asset586_state_address(),
      car_scene.asset_slot72_state_address(), car_scene.asset888_state_address(),
      car_scene.glow_part_file_state_address(), car_scene.glow_textures_state_address(),
      car_scene.asset109_state_address(),     car_scene.bundle3_state_address()};
  for (const uint32_t state : states) {
    if (int32_t(Load(state)) == 2) {
      return 0;
    }
  }
  // Pools 1 to 6: the finisher and the return address after its call.
  struct Finisher {
    uint32_t function, lr;
  };
  static constexpr Finisher kFinishers[8] = {
      {0x820D4048, 0x820C5880}, {0x820E2E68, 0x820C58B8}, {0x820DBFF8, 0x820C5900},
      {0x820E4140, 0x820C58E0}, {0x820DDA60, 0x820C5920}, {0x820E7328, 0x820C5940},
      {0x820E5A68, 0x820C5960}, {0, 0}};
  for (uint32_t pool = 0; pool < 8; pool++) {
    const uint32_t list = car_scene.car_models_first_address() + PoolList::kSize * pool;
    for (int32_t i = 0; i < int32_t(PoolList(list).count()); i++) {
      const uint32_t object = vehicle_accessors::ListAt(list, uint32_t(i));
      if (int32_t(PoolNode(object).state()) != 3) {
        continue;
      }
      if (pool == 0) {
        const schema::SceneCarModel car(object);
        if (int32_t(finish(kFinishers[0].function, frame, kFinishers[0].lr,
                           car.car_model().address, uint32_t(i))) == 0 ||
            int32_t(car.car_model<schema::CarModel>().decals_state()) == 2) {
          return 0;
        }
      } else if (pool == 7) {
        if (schema::SceneModel(object).build_pending() != 0) {
          return 0;
        }
      } else if (int32_t(finish(kFinishers[pool].function, frame, kFinishers[pool].lr, object,
                                uint32_t(i))) ==
                 0) {
        return 0;
      }
    }
  }
  return 1;
}

// --- The first set's teardown ---

bool DestroyAFreeable(uint32_t set) {
  for (uint32_t pool = 0; pool < kFirstSetPools; pool++) {
    uint32_t node =
        PoolList(ScenePools(set).pools_address() + PoolList::kSize * pool).first().address;
    for (uint32_t n = 0; node != 0; n++) {
      if (n == kMaxNodes || !memory::IsPages(node)) {
        return false;
      }
      node = PoolNode(node).next().address;
    }
  }
  return true;
}

void DestroyA(uint32_t set, uint32_t sp,
              const std::function<void(uint32_t object, uint32_t frame)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820C3CF8 ScenePools::DestroyA");
  // The original's frame (only its back chain) left out: the destructors'
  // guest calls run from `sp` (E5 step 6).
  const uint32_t frame = sp;
  for (uint32_t pool = 0; pool < kFirstSetPools; pool++) {
    const uint32_t list = ScenePools(set).pools_address() + PoolList::kSize * pool;
    for (;;) {
      const uint32_t object = vehicle_accessors::ListAt(list, 0);
      if (object == 0) {
        break;
      }
      // The pool's index is compared against 0 to 7 by branches that all
      // take this path.
      destroy(object, frame);
      Require(memory::IsPages(object) && memory::FreePages(object), "VirtualFree");
    }
  }
}

// --- The shared assets ---

namespace {

// The load slots SceneAssets::Reset resets, the XPRs it releases and the
// providers it clears, in its order: each step a slot, an XPR, a provider,
// a part model or a pixels' word (offsets in the assets).
enum class Step : uint8_t { kSlot, kXpr, kProvider, kModel, kPixels };
struct ResetStep {
  Step step;
  uint32_t offset;
};
constexpr ResetStep kResetSteps[] = {
    {Step::kSlot, 0},        {Step::kXpr, 24},        {Step::kSlot, 48},
    {Step::kXpr, 96},        {Step::kProvider, 144},  {Step::kSlot, 72},
    {Step::kXpr, 120},       {Step::kProvider, 164},  {Step::kSlot, 184},
    {Step::kXpr, 208},       {Step::kProvider, 232},  {Step::kSlot, 252},
    {Step::kXpr, 276},       {Step::kProvider, 300},  {Step::kSlot, 320},
    {Step::kXpr, 344},       {Step::kProvider, 368},  {Step::kSlot, 388},
    {Step::kModel, 416},     {Step::kSlot, 880},      {Step::kSlot, 904},
    {Step::kPixels, 948},    {Step::kSlot, 4560},     {Step::kXpr, 4584},
    {Step::kSlot, 952},      {Step::kXpr, 976},       {Step::kProvider, 1000},
    {Step::kSlot, 1020},     {Step::kXpr, 1044},      {Step::kProvider, 1068},
    {Step::kSlot, 4048},     {Step::kSlot, 4072},     {Step::kModel, 4096},
};

uint64_t PendingNodes(uint32_t assets) {
  uint64_t nodes = 2 * std::size(kAssetsXprs);  // XPR::Release checks for two each
  for (const uint32_t model : kAssetsModels) {
    nodes += models::ModelReleaseNodes(assets + model);
  }
  return nodes;
}

bool Room(uint64_t nodes) {
  return nodes <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(uint32_t(nodes));
}

void ReleaseModel(uint32_t model) {
  Require(models::ModelRelease(model).has_value(), "Model::Release");
}

}  // namespace

bool AssetsConstruct(uint32_t assets) {
  ITC_NATIVE_RUNNING("sub_820C5008 SceneAssets::SceneAssets");
  if (!models::CanAllocatePointers(models::kTextureProviderSlots)) {
    return false;
  }
  auto xpr = [&](uint32_t offset) { models::XprConstruct(assets + offset); };
  auto provider = [&](uint32_t offset) {
    Require(models::TextureProviderConstruct(assets + offset, -1).has_value(),
            "TextureProvider::TextureProvider");
  };
  auto model = [&](uint32_t offset) {
    Require(models::PartModelConstruct(assets + offset), "PartModel::PartModel");
  };
  xpr(24);
  xpr(96);
  xpr(120);
  provider(144);
  provider(164);
  xpr(208);
  provider(232);
  xpr(276);
  provider(300);
  xpr(344);
  provider(368);
  model(416);
  car_models::TgaHeaderClear(assets + 928);
  xpr(976);
  provider(1000);
  xpr(1044);
  provider(1068);
  for (uint32_t i = 0; i < 4; i++) {
    model(1184 + 464 * i);
  }
  for (uint32_t i = 0; i < 19; i++) {
    car_models::TgaHeaderClear(assets + 3668 + 20 * i);
  }
  model(4096);
  xpr(4584);
  for (uint32_t i = 0; i < 4; i++) {
    xpr(4704 + 24 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    provider(4800 + 20 * i);
  }
  return true;
}

bool AssetsResettable(uint32_t assets) {
  for (const uint32_t provider : kAssetsProviders) {
    if (schema::TextureProvider(assets + provider).vtable().clear_all() !=
        scene_loaders::kTextureProviderClearAll) {
      return false;
    }
  }
  return Room(PendingNodes(assets));
}

std::optional<uint32_t> AssetsReset(uint32_t assets) {
  ITC_NATIVE_RUNNING("sub_820C4A70 SceneAssets::Reset");
  if (!AssetsResettable(assets)) {
    return std::nullopt;
  }
  auto run = [&](const ResetStep& step) {
    const uint32_t at = assets + step.offset;
    switch (step.step) {
      case Step::kSlot:
        car_models::LoadSlotReset(at);
        break;
      case Step::kXpr:
        Require(models::XprRelease(at).has_value(), "XPR::Release");
        break;
      case Step::kProvider:
        scenery_lists::TextureProviderClearAll(at);
        break;
      case Step::kModel:
        ReleaseModel(at);
        break;
      case Step::kPixels:
        Store(at, 0);
        break;
    }
  };
  for (const ResetStep& step : kResetSteps) {
    run(step);
  }
  for (uint32_t i = 0; i < 4; i++) {
    car_models::LoadSlotReset(assets + 1088 + 24 * i);
    ReleaseModel(assets + 1184 + 464 * i);
    car_models::LoadSlotReset(assets + 3040 + 24 * i);
  }
  for (uint32_t i = 0; i < 19; i++) {
    car_models::LoadSlotReset(assets + 3136 + 24 * i);
    Store(schema::SceneAssets(assets).plate_tga_pixels_address() + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 4; i++) {
    car_models::LoadSlotReset(assets + 4608 + 24 * i);
    Require(models::XprRelease(assets + 4704 + 24 * i).has_value(), "XPR::Release");
    scenery_lists::TextureProviderClearAll(assets + 4800 + 20 * i);
  }
  return assets + 4860;  // ClearAll leaves r3 the provider
}

bool AssetsDestructible(uint32_t assets) {
  for (const uint32_t provider : kAssetsProviders) {
    const uint32_t storage = schema::TextureProvider(assets + provider).first().address;
    if (storage != 0 && !memory::IsHeapBlock(storage)) {
      return false;
    }
  }
  for (const uint32_t model : kAssetsModels) {
    if (!models::ModelMembersDestructible(assets + model)) {
      return false;
    }
  }
  return Room(PendingNodes(assets));
}

std::optional<uint32_t> AssetsDestruct(uint32_t assets) {
  ITC_NATIVE_RUNNING("sub_820C5168 SceneAssets::~SceneAssets");
  if (!AssetsDestructible(assets)) {
    return std::nullopt;
  }
  auto xpr = [&](uint32_t offset) {
    const auto r3 = models::XprDestruct(assets + offset);
    Require(r3.has_value(), "XPR::~XPR");
    return *r3;
  };
  auto provider = [&](uint32_t offset) {
    Require(models::TextureProviderDestruct(assets + offset).has_value(),
            "TextureProvider::~TextureProvider");
  };
  auto model = [&](uint32_t offset) {
    Require(models::PartModelDestruct(assets + offset).has_value(),
            "PartModel::~PartModel");
  };
  for (uint32_t i = 4; i-- > 0;) {
    provider(4800 + 20 * i);
  }
  for (uint32_t i = 4; i-- > 0;) {
    xpr(4704 + 24 * i);
  }
  xpr(4584);
  model(4096);
  // The 19 TGA headers' destructor (sub_82106540) is empty.
  for (uint32_t i = 4; i-- > 0;) {
    model(1184 + 464 * i);
  }
  provider(1068);
  xpr(1044);
  provider(1000);
  xpr(976);
  model(416);
  provider(368);
  xpr(344);
  provider(300);
  xpr(276);
  provider(232);
  xpr(208);
  provider(164);
  provider(144);
  xpr(120);
  xpr(96);
  return xpr(24);
}

// --- The scene ---

bool CarSceneConstruct(uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_820C89A8 CarScene::CarScene");
  if (!models::CanAllocatePointers(models::kTextureProviderSlots)) {
    return false;
  }
  const CarScene car_scene(scene);
  SetUpConstruct(car_scene.model_set_address());
  Require(AssetsConstruct(car_scene.assets_address()), "SceneAssets::SceneAssets");
  for (uint32_t i = 0; i < 8; i++) {
    vehicle_accessors::ListInit(car_scene.car_models_first_address() + PoolList::kSize * i);
  }
  // 1.0, stored as lfs and stfs leave it: the float4s' w's from the pass
  // description's light direction on.
  const uint32_t one = Globals::one();
  for (uint32_t i = 0; i < 5; i++) {
    Store(car_scene.pass_light_direction_w_address() + 16 * i, one);
  }
  for (uint32_t i = 0; i < 8; i++) {
    Store(car_scene.car_light_factor_a_w_address() + 16 * i, one);
  }
  for (uint32_t i = 0; i < 9; i++) {
    Store(car_scene.headlight_position_w_address() + 16 * i, one);
  }
  return true;
}

void ListDeleteAll(uint32_t list, uint32_t sp, const DestroyCall& destroy) {
  ITC_NATIVE_RUNNING("sub_82593FF8 List::DeleteAll");
  // The original's 96-byte frame (only its back chain) left out: the
  // destructors' guest calls run from `sp` (E5 step 6).
  const uint32_t frame = sp;
  const schema::AccessorList nodes(list);
  for (uint32_t node = nodes.first().address; node != 0; node = nodes.first().address) {
    destroy(node, 1, frame);
  }
}

void ListDestruct(uint32_t list, uint32_t sp, const DestroyCall& destroy) {
  ITC_NATIVE_RUNNING("sub_82594188 List::~List");
  ListDeleteAll(list, sp, destroy);
}

bool CarSceneDestructible(uint32_t scene, uint32_t flags) {
  return AssetsDestructible(CarScene(scene).assets_address()) &&
         ((flags & 1) == 0 || memory::IsHeapBlock(scene));
}

std::optional<uint32_t> CarSceneDestruct(uint32_t scene, uint32_t flags,
                                         uint32_t sp, const DestroyCall& destroy) {
  ITC_NATIVE_RUNNING("sub_820C9110 CarScene::~CarScene");
  if (!CarSceneDestructible(scene, flags)) {
    return std::nullopt;
  }
  // The original's 128-byte frame (only its back chain) left out (E5 step
  // 6): the lists' destructors run from `sp`.
  for (uint32_t i = 8; i-- > 0;) {
    // List::~List (sub_82594188) is a jump to DeleteAll.
    ListDestruct(CarScene(scene).car_models_first_address() + PoolList::kSize * i,
                 sp, destroy);
  }
  Require(AssetsDestruct(CarScene(scene).assets_address()).has_value(),
          "SceneAssets::~SceneAssets");
  if (flags & 1) {
    Require(memory::crt::OperatorDelete(scene), "operator delete");
  }
  return scene;
}

// --- The first set's update ---

bool UpdateANative() {
  const ScenePools pools = Globals::first_set();
  const uint32_t set = pools.address;
  if (set == 0 || int32_t(pools.state()) != 4 || pools.teardown_count() != 0) {
    return true;
  }
  const uint32_t records = pools.deferred_releases().address;
  return DestroyAFreeable(set) && records != 0 && memory::IsPages(records) &&
         memory::IsPages(set);
}

std::optional<uint32_t> UpdateA(uint32_t sp, const DestroyCall& destroy,
                                const TextureFreeCall& texture_free) {
  ITC_NATIVE_RUNNING("sub_820C3DA0 ScenePools::UpdateA");
  uint32_t set = Globals::first_set().address;
  if (set == 0) {
    return std::nullopt;
  }
  // The original's 128-byte frame (only its back chain, on the teardown's
  // path) left out: the texture frees and DestroyA run from `sp` (E5 step
  // 6).
  const uint32_t frame = sp;
  // The set's record at `at`, its [+52] read again each time.
  auto record_at = [&](uint32_t at) {
    return schema::CarModelsDeferredRelease(ScenePools(set).deferred_releases().address + at);
  };
  // The four words of a record cleared, [+52] read again for each.
  auto clear = [&](uint32_t at) {
    record_at(at).set_first(0);
    record_at(at).set_first_data(0);
    record_at(at).set_second(0);
    record_at(at).set_second_data(0);
  };
  auto count = [&]() {
    const ScenePools pools(set);
    return int32_t((pools.traffic_count() + pools.car_model_count()) << 2);
  };
  constexpr uint32_t kRecordBytes = schema::CarModelsDeferredRelease::kSize;
  std::optional<uint32_t> r3;
  if (int32_t(ScenePools(set).state()) != 4) {
    const int32_t n = count();
    for (int32_t i = 0; i < n; i++) {
      const uint32_t at = kRecordBytes * uint32_t(i);
      const auto record = record_at(at);
      const int32_t state = int32_t(record.state());
      if (state <= 0) {
        continue;
      }
      record.set_state(uint32_t(state - 1));
      const auto again = record_at(at);
      r3 = again.address;
      if (again.state() != 0) {
        continue;
      }
      texture_free(again.address, again.first_data_address(), frame);
      const auto second = record_at(at);
      r3 = texture_free(second.second_address(), second.second_data_address(), frame);
      clear(at);
    }
    return r3;
  }
  if (ScenePools(set).teardown_count() != 0) {
    ScenePools(set).set_teardown_count(ScenePools(set).teardown_count() - 1);
    return std::nullopt;
  }
  DestroyA(set, frame, [&](uint32_t object, uint32_t destroy_frame) {
    destroy(object, 0, destroy_frame);
  });
  set = Globals::first_set().address;
  const int32_t n = count();
  for (int32_t i = 0; i < n; i++) {
    const uint32_t at = kRecordBytes * uint32_t(i);
    const auto record = record_at(at);
    if (int32_t(record.state()) <= 0) {
      continue;
    }
    texture_free(record.address, record.first_data_address(), frame);
    const auto second = record_at(at);
    texture_free(second.second_address(), second.second_data_address(), frame);
    clear(at);
  }
  Require(FreePagesIn(ScenePools(set).deferred_releases_address()).has_value(),
          "FreePagesIn");
  const uint32_t pages = Globals::first_set().address;
  Require(memory::IsPages(pages) && memory::FreePages(pages), "VirtualFree");
  Globals::set_first_set(uint32_t{0});
  return 1;
}


// --- The release and the maker ---

namespace {

// SceneAssets::Release's steps in its order: a slot's buffer released, or
// a pixels' word's pages freed.
struct ReleaseStep {
  bool pixels;
  uint32_t offset;
};

std::vector<ReleaseStep> ReleaseSteps() {
  std::vector<ReleaseStep> steps;
  for (uint32_t at : {0u, 48u, 72u, 184u, 252u, 320u, 388u, 880u, 904u}) {
    steps.push_back({false, at});
  }
  steps.push_back({true, 948});
  for (uint32_t at : {952u, 1020u, 4048u, 4072u}) {
    steps.push_back({false, at});
  }
  return steps;
}

std::vector<ReleaseStep> ReleaseStepsAfterModel() {
  std::vector<ReleaseStep> steps{{false, 4560}};
  for (uint32_t i = 0; i < 4; i++) {
    steps.push_back({false, 1088 + 24 * i});
    steps.push_back({false, 3040 + 24 * i});
  }
  for (uint32_t i = 0; i < 19; i++) {
    steps.push_back({false, 3136 + 24 * i});
    steps.push_back({true, 3592 + 4 * i});
  }
  for (uint32_t i = 0; i < 4; i++) {
    steps.push_back({false, 4608 + 24 * i});
  }
  return steps;
}

}  // namespace

bool AssetsReleasable(uint32_t assets) {
  std::unordered_set<uint32_t> pages;
  std::vector<ReleaseStep> all = ReleaseSteps();
  for (const ReleaseStep& step : ReleaseStepsAfterModel()) {
    all.push_back(step);
  }
  for (const ReleaseStep& step : all) {
    uint32_t freed = 0;
    if (step.pixels) {
      const uint32_t word = Load(assets + step.offset);
      freed = word != 0 && memory::IsPages(word) ? word : 0;
    } else if (!car_models::LoadSlotReleasable(assets + step.offset, &freed)) {
      return false;
    }
    if (freed != 0 && !pages.insert(freed).second) {
      return false;
    }
  }
  for (const uint32_t provider : kAssetsProviders) {
    if (schema::TextureProvider(assets + provider).vtable().clear_all() !=
        scene_loaders::kTextureProviderClearAll) {
      return false;
    }
  }
  return Room(PendingNodes(assets) + models::ModelReleaseNodes(assets + 4096));
}

std::optional<uint32_t> AssetsRelease(uint32_t assets,
                                      const std::function<void(uint32_t at)>& free_pages_in) {
  ITC_NATIVE_RUNNING("sub_820C4C80 SceneAssets::Release");
  if (!AssetsReleasable(assets)) {
    return std::nullopt;
  }
  auto run = [&](const ReleaseStep& step) {
    const uint32_t at = assets + step.offset;
    if (!step.pixels) {
      Require(car_models::LoadSlotReleaseBuffer(at).has_value(), "LoadSlot::ReleaseBuffer");
    } else if (!FreePagesIn(at)) {
      free_pages_in(at);
    }
  };
  for (const ReleaseStep& step : ReleaseSteps()) {
    run(step);
  }
  ReleaseModel(assets + 4096);
  for (const ReleaseStep& step : ReleaseStepsAfterModel()) {
    run(step);
  }
  const auto r3 = AssetsReset(assets);
  Require(r3.has_value(), "SceneAssets::Reset");
  return r3;
}

bool CreateNative() {
  // CarScene::CarScene's storage, and the fresh assets' Reset (XPR::Release
  // checks for two nodes each; the fresh models' blocks are 0).
  return models::CanAllocatePointers(models::kTextureProviderSlots) &&
         Room(2 * std::size(kAssetsXprs) + 2 * std::size(kAssetsModels));
}

uint64_t Create(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820C8A50 CarScene::Create");
  const uint32_t frame = sp - 128;
  Store(frame, sp);
  const uint32_t pages = memory::AllocPages(kSceneBytes, 4096);
  Globals::set_scene(pages);
  uint32_t scene = pages;
  if (pages != 0) {
    Require(CarSceneConstruct(pages), "CarScene::CarScene");
    scene = Globals::scene().address;
  }
  CarScene(scene).set_state(0);
  Globals::set_queue_key_a(167);
  Globals::set_queue_key_b(270);
  scene_loaders::MakeA();
  Globals::scene().set_first_set(Globals::first_set().address);
  InitLighting(Globals::scene().car_light_factor_a_address(), frame);
  Require(AssetsReset(Globals::scene().assets_address()).has_value(),
          "SceneAssets::Reset");
  Globals::scene().set_set_up_r5(0);
  Globals::scene().set_blink_timers_held(0);
  Globals::scene().set_lighting_fixed(0);
  material_passes::PaintPaletteInit(frame);
  accessories::SetCreate();
  const uint32_t x = Globals::accessories_scale();
  Store(frame + 80, x);
  Store(frame + 84, x);
  Store(frame + 88, x);
  Store(frame + 92, Globals::one());  // 1.0
  const uint32_t set = AccessoriesSet();
  accessories::SetScale(set, frame + 80);
  return set;
}


uint32_t LoadAssets(uint32_t scene, uint32_t pcr, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820C8B40 CarScene::LoadAssets");
  const CarScene car_scene(scene);
  const schema::SceneAssets assets(car_scene.assets_address());
  uint32_t r3 = 0;
  auto shared = [&](uint32_t slot, uint32_t id) {
    r3 = car_models::LoadSlotLoadShared(slot, id, scene, kOnFileLoaded, load);
  };
  auto own = [&](uint32_t slot, uint32_t id) {
    car_models::LoadSlotLoad(slot, id, scene, kOnFileLoaded, pcr, load);
  };
  shared(assets.slot_586_address(), 586);
  if (car_scene.model_set() == 0 || car_scene.set_up_r5() != 0) {
    shared(assets.slot_141_address(), 141);
  }
  shared(assets.slot_888_address(), 888);
  shared(assets.slot_144_address(), 144);
  shared(assets.slot_145_address(), 145);
  own(assets.glow_part_file_slot_address(), 142);
  shared(assets.glow_textures_slot_address(), 143);
  shared(assets.tga_slot_address(), 146);
  shared(assets.slot_106_address(), 106);
  shared(assets.slot_107_address(), 107);
  if (car_scene.model_set() == 0) {
    own(assets.slot_108_address(), 108);
    shared(assets.slot_109_address(), 109);
  }
  shared(assets.slot_105_address(), 105);
  const bool set_a = car_scene.model_set() == 0;
  for (uint32_t i = 0; i < 4; i++) {
    own(assets.part_file_slots_address() + LoadSlot::kSize * i, (set_a ? 129 : 133) + i);
    shared(assets.part_texture_slots_address() + LoadSlot::kSize * i, 137 + i);
  }
  for (uint32_t i = 0; i < 19; i++) {
    shared(assets.plate_tga_slots_address() + LoadSlot::kSize * i, 110 + i);
  }
  const bool bundles_a = car_scene.model_set() == 0;
  for (uint32_t i = 0; i < 4; i++) {
    shared(assets.bundle_slots_address() + LoadSlot::kSize * i, (bundles_a ? 97 : 101) + i);
  }
  return r3;
}


// --- The loads' callback ---

namespace {

constexpr uint32_t kTextureProviderSet = 0x82594718;  // slot +16

enum class Kind : uint8_t { kXpr, kXprProvider, kPart, kTexture, kTga };

// A branch of OnFileLoaded: the slot whose request matched (offsets in the
// assets) and what it finishes.
struct Branch {
  Kind kind;
  uint32_t slot;
  uint32_t a = 0;  // the XPR, the part model or the TGA header
  uint32_t b = 0;  // the provider, the part file's slot or the pixels' word
};

std::optional<Branch> FindBranch(uint32_t assets, uint32_t request) {
  auto is = [&](uint32_t slot) { return LoadSlot(assets + slot).request() == request; };
  if (is(0)) {
    return Branch{Kind::kXpr, 0, 24};
  }
  for (uint32_t i = 0; i < 2; i++) {
    if (is(48 + 24 * i)) {
      return Branch{Kind::kXprProvider, 48 + 24 * i, 96 + 24 * i, 144 + 20 * i};
    }
  }
  if (is(184)) return Branch{Kind::kXprProvider, 184, 208, 232};
  if (is(252)) return Branch{Kind::kXprProvider, 252, 276, 300};
  if (is(320)) return Branch{Kind::kXprProvider, 320, 344, 368};
  if (is(388)) return Branch{Kind::kPart, 388};
  if (is(880)) return Branch{Kind::kTexture, 880, 416, 388};
  if (is(904)) return Branch{Kind::kTga, 904, 928, 948};
  if (is(952)) return Branch{Kind::kXprProvider, 952, 976, 1000};
  if (is(1020)) return Branch{Kind::kXprProvider, 1020, 1044, 1068};
  for (uint32_t i = 0; i < 4; i++) {
    if (is(1088 + 24 * i)) {
      return Branch{Kind::kPart, 1088 + 24 * i};
    }
    if (is(3040 + 24 * i)) {
      return Branch{Kind::kTexture, 3040 + 24 * i, 1184 + 464 * i, 1088 + 24 * i};
    }
  }
  for (uint32_t i = 0; i < 19; i++) {
    if (is(3136 + 24 * i)) {
      return Branch{Kind::kTga, 3136 + 24 * i, 3668 + 20 * i, 3592 + 4 * i};
    }
  }
  if (is(4048)) return Branch{Kind::kPart, 4048};
  if (is(4072)) return Branch{Kind::kTexture, 4072, 4096, 4048};
  if (is(4560)) return Branch{Kind::kXpr, 4560, 4584};
  for (uint32_t i = 0; i < 4; i++) {
    if (is(4608 + 24 * i)) {
      return Branch{Kind::kXprProvider, 4608 + 24 * i, 4704 + 24 * i, 4800 + 20 * i};
    }
  }
  return std::nullopt;
}

uint32_t Release(uint32_t slot) {
  const auto r3 = car_models::LoadSlotReleaseBuffer(slot);
  Require(r3.has_value(), "LoadSlot::ReleaseBuffer");
  return *r3;
}

}  // namespace

bool FileLoadedNative(uint32_t request, uint32_t scene) {
  const uint32_t assets = CarScene(scene).assets_address();
  const auto branch = FindBranch(assets, request);
  if (!branch || branch->kind == Kind::kPart) {
    return true;
  }
  uint32_t first = 0, second = 0;
  if (!car_models::LoadSlotReleasable(assets + branch->slot, &first)) {
    return false;
  }
  if (branch->kind == Kind::kTexture) {
    // The part file's slot released first.
    return car_models::LoadSlotReleasable(assets + branch->b, &second) &&
           (first == 0 || first != second);
  }
  return true;
}

std::optional<uint64_t> FileLoaded(uint32_t request, uint32_t data,
                                   uint32_t scene, uint32_t sp, const FileLoadedCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820C52A0 CarScene::OnFileLoaded");
  const uint32_t assets = CarScene(scene).assets_address();
  // The original's 128-byte frame stays (E5 step 6): it keeps `data` in
  // its caller's area (its frame's +156, `sp` + 28), handed to the TGA's
  // create, and its callees run from the frame so their own parameter
  // homes (their r1 + 20 on) can't reach that word.
  const uint32_t frame = sp - 128;
  const uint32_t data_at = frame + 156;
  Store(data_at, data);
  const auto branch = FindBranch(assets, request);
  if (!branch) {
    return std::nullopt;
  }
  const uint32_t slot = assets + branch->slot;
  auto xpr_create = [&](uint32_t xpr) {
    if (!models::XprCreate(xpr, data)) {
      calls.xpr_create(xpr, data);
    }
  };
  switch (branch->kind) {
    case Kind::kXpr:
      xpr_create(assets + branch->a);
      break;
    case Kind::kXprProvider: {
      const uint32_t xpr = assets + branch->a, provider = assets + branch->b;
      xpr_create(xpr);
      const uint32_t set = schema::TextureProvider(provider).vtable().set();
      if (set != kTextureProviderSet ||
          !scenery_lists::TextureProviderSet(provider, 0, xpr)) {
        calls.provider_set(provider, xpr);
      }
      break;
    }
    case Kind::kPart:
      LoadSlot(slot).set_state(3);
      return std::nullopt;
    case Kind::kTexture: {
      const uint32_t model = assets + branch->a;
      const uint32_t file = LoadSlot(assets + branch->b).buffer();
      memory::PhysicalTag tag("model vertices: scene");
      if (!models::ModelLoad(model, file, data, calls.model_load)) {
        calls.model_load_original(model, file, data);
      }
      Release(assets + branch->b);
      uint64_t r3 = Release(slot);
      LoadSlot(slot).set_state(3);
      if (branch->slot == 880) {
        const int32_t shapes = int32_t(
            schema::PartModel(schema::SceneAssets(assets).glow_model_address())
                .shape_count());
        const double one = double(std::bit_cast<float>(Globals::one()));
        for (int32_t i = 0; i < shapes; i++) {
          r3 = material_passes::ScaleBlended(model, uint32_t(i), one, frame);
        }
      }
      return r3;
    }
    case Kind::kTga: {
      const uint32_t header = assets + branch->a, pixels = assets + branch->b;
      const uint32_t size = LoadSlot(slot).size();
      if (!car_models::TgaCreateImage(header, size, data_at, pixels, frame)) {
        calls.tga_create(header, size, data_at, pixels);
      }
      break;
    }
  }
  const uint32_t r3 = Release(slot);
  LoadSlot(slot).set_state(3);
  return r3;
}


// --- The scene's update ---

void DestroyB(uint32_t scene, uint32_t sp, const FinishCall& finish,
              const std::function<void(uint32_t object, uint32_t frame)>& destroy,
              const FreeCall& free) {
  ITC_NATIVE_RUNNING("sub_820C59B0 ScenePools::DestroyB");
  // The original's 128-byte frame (only its back chain) left out: its
  // callees run from `sp` (E5 step 6).
  const uint32_t frame = sp;
  LoadsDone(scene, frame, finish);
  for (uint32_t pool = 0; pool < 8; pool++) {
    const uint32_t list = CarScene(scene).car_models_first_address() + PoolList::kSize * pool;
    for (;;) {
      const uint32_t object = vehicle_accessors::ListAt(list, 0);
      if (object == 0) {
        break;
      }
      // A jump table on the pool whose eight cases all take this path.
      destroy(object, frame);
      free(object, frame);
    }
  }
}

std::optional<uint64_t> UpdateModels(
    uint32_t scene, uint32_t sp,
    const std::function<uint64_t(uint32_t model, uint32_t frame)>& model_update) {
  ITC_NATIVE_RUNNING("sub_820C5A60 CarScene::UpdateModels");
  const CarScene car_scene(scene);
  const int32_t count = int32_t(car_scene.scene_models_count());
  if (count <= 0) {
    return std::nullopt;
  }
  // The original's 112-byte frame (only its back chain) left out: the
  // models' updates run from `sp` (E5 step 6).
  const uint32_t frame = sp;
  uint64_t r3 = 0;
  for (int32_t i = 0; i < count; i++) {
    const uint32_t model =
        vehicle_accessors::ListAt(car_scene.scene_models_first_address(), uint32_t(i));
    r3 = model;
    if (int32_t(schema::SceneModel(model).state()) == 3) {
      r3 = model_update(model, frame);
    }
  }
  return r3;
}

uint64_t Update(uint32_t sp, uint64_t r3, const UpdateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820C9608 CarScene::Update");
  // The original's 96-byte frame (only its back chain) left out: its
  // callees run from `sp` (E5 step 6).
  const uint32_t frame = sp;
  const uint32_t scene = Globals::scene().address;
  r3 = scene;
  if (scene != 0) {
    const CarScene car_scene(scene);
    if (int32_t(car_scene.state()) != 4) {
      r3 = calls.update_models(scene, frame);
      Globals::set_scene_ticks(3);
    } else if (car_scene.teardown_count() != 0) {
      car_scene.set_teardown_count(car_scene.teardown_count() - 1);
    } else {
      DestroyB(scene, frame, calls.finish, calls.destroy, calls.free_object);
      CarScene current = Globals::scene();
      if (current.own_load_buffer() == 0) {
        current.set_load_buffer(0);
        current = Globals::scene();
      }
      calls.release(current.assets_address(), frame);
      calls.destruct(Globals::scene().address, frame);
      r3 = calls.free_scene(Globals::scene().address, frame);
      Globals::set_scene(uint32_t{0});
    }
  }
  r3 = calls.update_a(r3, frame);
  if (Globals::scene().address == 0) {
    const uint32_t set = AccessoriesSet();
    r3 = set;
    if (set != 0) {
      r3 = calls.accessories_destroy(frame);
    }
  }
  return r3;
}

}  // namespace scene
