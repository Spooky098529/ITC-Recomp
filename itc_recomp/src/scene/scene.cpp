// The car scene (see scene.h), load for load and store for store as the
// recompiled functions.

#include "scene/scene.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "car_models/tga.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
#include "models/xpr.h"
#include "accessories/accessories.h"
#include "material_passes/material_passes.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/pools.h"
#include "scenery_lists/scenery_lists.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstdlib>
#include <unordered_set>
#include <vector>

namespace scene {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A member whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene: {} refused after its checks", what);
    std::abort();
  }
}

constexpr uint32_t kFirstSetPools = 7;
constexpr uint32_t kFirstSetLists = 56;
constexpr uint32_t kDestroyAFrameBytes = 128;
constexpr uint32_t kMaxNodes = 4096;

}  // namespace

// --- The small pieces ---

void SetUpConstruct(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D0D50 SceneSetUp::SceneSetUp");
  Store(base, record + 36, 0);
  Store(base, record + 40, 0);
}

std::optional<uint32_t> FreePagesIn(uint8_t* base, uint32_t at) {
  ITC_NATIVE_RUNNING("sub_820D09A0 FreePagesIn");
  const uint32_t pages = Load(base, at);
  if (pages == 0 || !memory::IsPages(pages)) {
    return std::nullopt;
  }
  memory::FreePages(pages);
  return 1;
}

void StartTeardownA(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820C3C80 ScenePools::StartTeardownA");
  Store(base, Load(base, kFirstSet) + 4, 4);
  Store(base, Load(base, kFirstSet) + 0, 2);
}

std::optional<uint64_t> StartTeardown(uint8_t* base,
                                      const std::function<uint64_t(uint32_t set)>& release_all) {
  ITC_NATIVE_RUNNING("sub_820C4D88 CarScene::StartTeardown");
  if (Load(base, kScene) == 0) {
    return std::nullopt;
  }
  Store(base, Load(base, kScene) + 0, 4);
  Store(base, Load(base, kScene) + 48, 2);
  StartTeardownA(base);
  return release_all(Load(base, 0x826C5510));  // sub_820F0E50: the accessories' set
}

std::optional<uint32_t> SetInteriorWord(uint8_t* base, uint32_t scene, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_820C8950 CarScene::SetInteriorWord");
  const int32_t count = int32_t(Load(base, scene + 5036));
  std::optional<uint32_t> r3;
  for (int32_t i = 0; i < count; i++) {
    const uint32_t model = vehicle_accessors::ListAt(base, scene + 5028, uint32_t(i));
    r3 = model;
    if (Load(base, model + 5072) != 0) {
      Store(base, Load(base, model + 5068) + 11616, word);
    }
  }
  return r3;
}

// --- The first set's teardown ---

bool DestroyAFreeable(uint8_t* base, uint32_t set) {
  for (uint32_t pool = 0; pool < kFirstSetPools; pool++) {
    uint32_t node = Load(base, set + kFirstSetLists + 12 * pool);
    for (uint32_t n = 0; node != 0; n++) {
      if (n == kMaxNodes || !memory::IsPages(node)) {
        return false;
      }
      node = Load(base, node + 8);
    }
  }
  return true;
}

void DestroyA(uint8_t* base, uint32_t set, uint32_t sp,
              const std::function<void(uint32_t object, uint32_t frame)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820C3CF8 ScenePools::DestroyA");
  const uint32_t frame = sp - kDestroyAFrameBytes;
  Store(base, frame, sp);  // the back chain, for the destructors' guest calls
  for (uint32_t pool = 0; pool < kFirstSetPools; pool++) {
    const uint32_t list = pool * 12 + set + kFirstSetLists;
    for (;;) {
      const uint32_t object = vehicle_accessors::ListAt(base, list, 0);
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

uint64_t PendingNodes(uint8_t* base, uint32_t assets) {
  uint64_t nodes = 2 * std::size(kAssetsXprs);  // XPR::Release checks for two each
  for (const uint32_t model : kAssetsModels) {
    nodes += models::ModelReleaseNodes(base, assets + model);
  }
  return nodes;
}

bool Room(uint8_t* base, uint64_t nodes) {
  return nodes <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(base, uint32_t(nodes));
}

void ReleaseModel(uint8_t* base, uint32_t model) {
  Require(models::ModelRelease(base, model).has_value(), "Model::Release");
}

}  // namespace

bool AssetsConstruct(uint8_t* base, uint32_t assets) {
  ITC_NATIVE_RUNNING("sub_820C5008 SceneAssets::SceneAssets");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  auto xpr = [&](uint32_t offset) { models::XprConstruct(base, assets + offset); };
  auto provider = [&](uint32_t offset) {
    Require(models::TextureProviderConstruct(base, assets + offset, -1).has_value(),
            "TextureProvider::TextureProvider");
  };
  auto model = [&](uint32_t offset) {
    Require(models::PartModelConstruct(base, assets + offset), "PartModel::PartModel");
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
  car_models::TgaHeaderClear(base, assets + 928);
  xpr(976);
  provider(1000);
  xpr(1044);
  provider(1068);
  for (uint32_t i = 0; i < 4; i++) {
    model(1184 + 464 * i);
  }
  for (uint32_t i = 0; i < 19; i++) {
    car_models::TgaHeaderClear(base, assets + 3668 + 20 * i);
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

bool AssetsResettable(uint8_t* base, uint32_t assets) {
  for (const uint32_t provider : kAssetsProviders) {
    const uint32_t vtable = Load(base, assets + provider);
    if (Load(base, vtable + 8) != scene_loaders::kTextureProviderClearAll) {
      return false;
    }
  }
  return Room(base, PendingNodes(base, assets));
}

std::optional<uint32_t> AssetsReset(uint8_t* base, uint32_t assets) {
  ITC_NATIVE_RUNNING("sub_820C4A70 SceneAssets::Reset");
  if (!AssetsResettable(base, assets)) {
    return std::nullopt;
  }
  auto run = [&](const ResetStep& step) {
    const uint32_t at = assets + step.offset;
    switch (step.step) {
      case Step::kSlot:
        car_models::LoadSlotReset(base, at);
        break;
      case Step::kXpr:
        Require(models::XprRelease(base, at).has_value(), "XPR::Release");
        break;
      case Step::kProvider:
        scenery_lists::TextureProviderClearAll(base, at);
        break;
      case Step::kModel:
        ReleaseModel(base, at);
        break;
      case Step::kPixels:
        Store(base, at, 0);
        break;
    }
  };
  for (const ResetStep& step : kResetSteps) {
    run(step);
  }
  for (uint32_t i = 0; i < 4; i++) {
    car_models::LoadSlotReset(base, assets + 1088 + 24 * i);
    ReleaseModel(base, assets + 1184 + 464 * i);
    car_models::LoadSlotReset(base, assets + 3040 + 24 * i);
  }
  for (uint32_t i = 0; i < 19; i++) {
    car_models::LoadSlotReset(base, assets + 3136 + 24 * i);
    Store(base, assets + 3592 + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 4; i++) {
    car_models::LoadSlotReset(base, assets + 4608 + 24 * i);
    Require(models::XprRelease(base, assets + 4704 + 24 * i).has_value(), "XPR::Release");
    scenery_lists::TextureProviderClearAll(base, assets + 4800 + 20 * i);
  }
  return assets + 4860;  // ClearAll leaves r3 the provider
}

bool AssetsDestructible(uint8_t* base, uint32_t assets) {
  for (const uint32_t provider : kAssetsProviders) {
    const uint32_t storage = Load(base, assets + provider + models::kTextureProviderVector +
                                            models::kVectorFirst);
    if (storage != 0 && !memory::IsHeapBlock(storage)) {
      return false;
    }
  }
  for (const uint32_t model : kAssetsModels) {
    if (!models::ModelMembersDestructible(base, assets + model)) {
      return false;
    }
  }
  return Room(base, PendingNodes(base, assets));
}

std::optional<uint32_t> AssetsDestruct(uint8_t* base, uint32_t assets) {
  ITC_NATIVE_RUNNING("sub_820C5168 SceneAssets::~SceneAssets");
  if (!AssetsDestructible(base, assets)) {
    return std::nullopt;
  }
  auto xpr = [&](uint32_t offset) {
    const auto r3 = models::XprDestruct(base, assets + offset);
    Require(r3.has_value(), "XPR::~XPR");
    return *r3;
  };
  auto provider = [&](uint32_t offset) {
    Require(models::TextureProviderDestruct(base, assets + offset).has_value(),
            "TextureProvider::~TextureProvider");
  };
  auto model = [&](uint32_t offset) {
    Require(models::PartModelDestruct(base, assets + offset).has_value(),
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

bool CarSceneConstruct(uint8_t* base, uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_820C89A8 CarScene::CarScene");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  SetUpConstruct(base, scene + 4);
  Require(AssetsConstruct(base, scene + kAssets), "SceneAssets::SceneAssets");
  for (uint32_t i = 0; i < 8; i++) {
    vehicle_accessors::ListInit(base, scene + 4944 + 12 * i);
  }
  const uint32_t one = Load(base, 0x82000A30);  // 1.0, stored as lfs and stfs leave it
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, scene + 5180 + 16 * i, one);
  }
  for (uint32_t i = 0; i < 8; i++) {
    Store(base, scene + 5260 + 16 * i, one);
  }
  for (uint32_t i = 0; i < 9; i++) {
    Store(base, scene + 5388 + 16 * i, one);
  }
  return true;
}

void ListDeleteAll(uint8_t* base, uint32_t list, uint32_t sp, const DestroyCall& destroy) {
  ITC_NATIVE_RUNNING("sub_82593FF8 List::DeleteAll");
  const uint32_t frame = sp - 96;
  Store(base, frame, sp);
  for (uint32_t node = Load(base, list); node != 0; node = Load(base, list)) {
    destroy(node, 1, frame);
  }
}

void ListDestruct(uint8_t* base, uint32_t list, uint32_t sp, const DestroyCall& destroy) {
  ITC_NATIVE_RUNNING("sub_82594188 List::~List");
  ListDeleteAll(base, list, sp, destroy);
}

bool CarSceneDestructible(uint8_t* base, uint32_t scene, uint32_t flags) {
  return AssetsDestructible(base, scene + kAssets) &&
         ((flags & 1) == 0 || memory::IsHeapBlock(scene));
}

std::optional<uint32_t> CarSceneDestruct(uint8_t* base, uint32_t scene, uint32_t flags,
                                         uint32_t sp, const DestroyCall& destroy) {
  ITC_NATIVE_RUNNING("sub_820C9110 CarScene::~CarScene");
  if (!CarSceneDestructible(base, scene, flags)) {
    return std::nullopt;
  }
  const uint32_t frame = sp - 128;
  Store(base, frame, sp);
  for (uint32_t i = 8; i-- > 0;) {
    // List::~List (sub_82594188) is a jump to DeleteAll: its frame below
    // ~CarScene's.
    ListDestruct(base, scene + 4944 + 12 * i, frame, destroy);
  }
  Require(AssetsDestruct(base, scene + kAssets).has_value(), "SceneAssets::~SceneAssets");
  if (flags & 1) {
    Require(memory::crt::OperatorDelete(scene), "operator delete");
  }
  return scene;
}

// --- The first set's update ---

bool UpdateANative(uint8_t* base) {
  const uint32_t set = Load(base, kFirstSet);
  if (set == 0 || int32_t(Load(base, set + 4)) != 4 || Load(base, set + 0) != 0) {
    return true;
  }
  const uint32_t records = Load(base, set + 52);
  return DestroyAFreeable(base, set) && records != 0 && memory::IsPages(records) &&
         memory::IsPages(set);
}

std::optional<uint32_t> UpdateA(uint8_t* base, uint32_t sp, const DestroyCall& destroy,
                                const TextureFreeCall& texture_free) {
  ITC_NATIVE_RUNNING("sub_820C3DA0 ScenePools::UpdateA");
  uint32_t set = Load(base, kFirstSet);
  if (set == 0) {
    return std::nullopt;
  }
  const uint32_t frame = sp - 128;
  // The four words of a record cleared, [+52] read again for each.
  auto clear = [&](uint32_t at) {
    for (uint32_t w = 0; w < 4; w++) {
      Store(base, Load(base, set + 52) + at + 4 * w, 0);
    }
  };
  auto count = [&]() { return int32_t((Load(base, set + 40) + Load(base, set + 12)) << 2); };
  std::optional<uint32_t> r3;
  if (int32_t(Load(base, set + 4)) != 4) {
    const int32_t n = count();
    for (int32_t i = 0; i < n; i++) {
      const uint32_t at = 20 * uint32_t(i);
      const uint32_t record = Load(base, set + 52) + at;
      const int32_t state = int32_t(Load(base, record + 16));
      if (state <= 0) {
        continue;
      }
      Store(base, record + 16, uint32_t(state - 1));
      const uint32_t again = Load(base, set + 52) + at;
      r3 = again;
      if (Load(base, again + 16) != 0) {
        continue;
      }
      texture_free(again, again + 4, frame);
      const uint32_t second = Load(base, set + 52) + at;
      r3 = texture_free(second + 8, second + 12, frame);
      clear(at);
    }
    return r3;
  }
  if (Load(base, set + 0) != 0) {
    Store(base, set + 0, Load(base, set + 0) - 1);
    return std::nullopt;
  }
  Store(base, frame, sp);
  DestroyA(base, set, frame, [&](uint32_t object, uint32_t destroy_frame) {
    destroy(object, 0, destroy_frame);
  });
  set = Load(base, kFirstSet);
  const int32_t n = count();
  for (int32_t i = 0; i < n; i++) {
    const uint32_t at = 20 * uint32_t(i);
    const uint32_t record = Load(base, set + 52) + at;
    if (int32_t(Load(base, record + 16)) <= 0) {
      continue;
    }
    texture_free(record, record + 4, frame);
    const uint32_t second = Load(base, set + 52) + at;
    texture_free(second + 8, second + 12, frame);
    clear(at);
  }
  Require(FreePagesIn(base, set + 52).has_value(), "FreePagesIn");
  const uint32_t pages = Load(base, kFirstSet);
  Require(memory::IsPages(pages) && memory::FreePages(pages), "VirtualFree");
  Store(base, kFirstSet, 0);
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

bool AssetsReleasable(uint8_t* base, uint32_t assets) {
  std::unordered_set<uint32_t> pages;
  std::vector<ReleaseStep> all = ReleaseSteps();
  for (const ReleaseStep& step : ReleaseStepsAfterModel()) {
    all.push_back(step);
  }
  for (const ReleaseStep& step : all) {
    uint32_t freed = 0;
    if (step.pixels) {
      const uint32_t word = Load(base, assets + step.offset);
      freed = word != 0 && memory::IsPages(word) ? word : 0;
    } else if (!car_models::LoadSlotReleasable(base, assets + step.offset, &freed)) {
      return false;
    }
    if (freed != 0 && !pages.insert(freed).second) {
      return false;
    }
  }
  for (const uint32_t provider : kAssetsProviders) {
    const uint32_t vtable = Load(base, assets + provider);
    if (Load(base, vtable + 8) != scene_loaders::kTextureProviderClearAll) {
      return false;
    }
  }
  return Room(base, PendingNodes(base, assets) + models::ModelReleaseNodes(base, assets + 4096));
}

std::optional<uint32_t> AssetsRelease(uint8_t* base, uint32_t assets,
                                      const std::function<void(uint32_t at)>& free_pages_in) {
  ITC_NATIVE_RUNNING("sub_820C4C80 SceneAssets::Release");
  if (!AssetsReleasable(base, assets)) {
    return std::nullopt;
  }
  auto run = [&](const ReleaseStep& step) {
    const uint32_t at = assets + step.offset;
    if (!step.pixels) {
      Require(car_models::LoadSlotReleaseBuffer(base, at).has_value(), "LoadSlot::ReleaseBuffer");
    } else if (!FreePagesIn(base, at)) {
      free_pages_in(at);
    }
  };
  for (const ReleaseStep& step : ReleaseSteps()) {
    run(step);
  }
  ReleaseModel(base, assets + 4096);
  for (const ReleaseStep& step : ReleaseStepsAfterModel()) {
    run(step);
  }
  const auto r3 = AssetsReset(base, assets);
  Require(r3.has_value(), "SceneAssets::Reset");
  return r3;
}

bool CreateNative(uint8_t* base) {
  // CarScene::CarScene's storage, and the fresh assets' Reset (XPR::Release
  // checks for two nodes each; the fresh models' blocks are 0).
  return models::CanAllocatePointers(base, models::kTextureProviderSlots) &&
         Room(base, 2 * std::size(kAssetsXprs) + 2 * std::size(kAssetsModels));
}

uint64_t Create(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820C8A50 CarScene::Create");
  const uint32_t frame = sp - 128;
  Store(base, frame, sp);
  const uint32_t pages = memory::AllocPages(kSceneBytes, 4096);
  Store(base, kScene, pages);
  uint32_t scene = pages;
  if (pages != 0) {
    Require(CarSceneConstruct(base, pages), "CarScene::CarScene");
    scene = Load(base, kScene);
  }
  Store(base, scene + 0, 0);
  Store(base, 0x82601854, 167);
  Store(base, 0x82601858, 270);
  scene_loaders::MakeA(base);
  Store(base, Load(base, kScene) + 5040, Load(base, kFirstSet));
  InitLighting(base, Load(base, kScene) + 5248, frame);
  Require(AssetsReset(base, Load(base, kScene) + kAssets).has_value(), "SceneAssets::Reset");
  Store(base, Load(base, kScene) + 5044, 0);
  Store(base, Load(base, kScene) + 5048, 0);
  Store(base, Load(base, kScene) + 5052, 0);
  material_passes::PaintPaletteInit(base, frame);
  accessories::SetCreate(base);
  const uint32_t x = Load(base, 0x82002FE8);
  Store(base, frame + 80, x);
  Store(base, frame + 84, x);
  Store(base, frame + 88, x);
  Store(base, frame + 92, Load(base, 0x82000A30));  // 1.0
  const uint32_t set = Load(base, accessories::kSet);  // sub_820F0E50
  accessories::SetScale(base, set, frame + 80);
  return set;
}

}  // namespace scene
