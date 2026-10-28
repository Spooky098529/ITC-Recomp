// The scene's loading objects' resets (see objects.h).

#include "scene_loaders/objects.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
#include "models/xpr.h"
#include "scenery_lists/scenery_lists.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/pools.h"
#include "scene_models/scene_models.h"

#include <cstdlib>
#include <unordered_set>

namespace scene_loaders {

namespace {

constexpr uint32_t kPhysical = 0xA0000000u;
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kOne = 0x82000A30;   // 1.0

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// A member whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene loaders: {} refused after its checks", what);
    std::abort();
  }
}

// The identity matrix (the four rows the original builds on its frame,
// copied by memcpy).
void Identity(uint8_t* base, uint32_t matrix) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      Store(base, matrix + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
}

// A position (0, 0, 0, 1) at `at`, in the original's store order (+8, +4,
// +0, then w at +12).
void Position(uint8_t* base, uint32_t at, uint32_t zero, uint32_t one) {
  Store(base, at + 8, zero);
  Store(base, at + 4, zero);
  Store(base, at + 0, zero);
  Store(base, at + 12, one);
}

constexpr uint32_t kParts = 216;
constexpr uint32_t kPartModels = 5184;  // in the loads

// Whether `count` part models' Model::Release runs natively, all of them
// (the pending frees take every block they put off).
bool ModelsReleasable(uint8_t* base, uint32_t first, uint32_t count) {
  uint64_t nodes = 0;
  for (uint32_t i = 0; i < count; i++) {
    nodes += models::ModelReleaseNodes(base, first + models::kPartModelBytes * i);
  }
  return nodes <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(base, uint32_t(nodes));
}

// Whether LoadSlot::ReleaseBuffer runs natively on `count` slots from
// `first`, each freeing pages no other frees.
bool SlotsReleasable(uint8_t* base, uint32_t first, uint32_t count) {
  std::unordered_set<uint32_t> pages;
  for (uint32_t i = 0; i < count; i++) {
    uint32_t freed = 0;
    if (!car_models::LoadSlotReleasable(base, first + kSlotBytes * i, &freed) ||
        (freed != 0 && !pages.insert(freed).second)) {
      return false;
    }
  }
  return true;
}

// The constructors' Model::Release on fresh models: no block to put off, but
// XPR::Release's check for two nodes.
bool FreshReleasable(uint8_t* base) { return models::PendingFreesHaveRoom(base, 2); }

}  // namespace

bool CarModelResettable(uint8_t* base, uint32_t car_model) {
  if (car_model >= kPhysical || kPhysical - car_model < kCarModelBytes) {
    return false;
  }
  return CarModelLoadsResettable(base, car_model + kCarModelLoads);
}

std::optional<uint32_t> CarModelReset(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D3E98 CarModel::Reset");
  if (!CarModelResettable(base, car_model)) {
    return std::nullopt;
  }
  Store(base, car_model + kCarModelSet, Load(base, kFirstSet));
  scene_models::CarRecordReset(base, car_model + kCarModelRecord);
  Require(CarModelLoadsReset(base, car_model + kCarModelLoads).has_value(),
          "CarModelLoads::Reset");
  for (uint32_t i = 0; i < 4; i++) {
    Identity(base, car_model + 6720 + 64 * i);
  }
  // The original loads both constants after the tires' matrices.
  const uint32_t one = Load(base, kOne);
  const uint32_t zero = Load(base, kZero);
  for (uint32_t i = 0; i < 4; i++) {
    Position(base, car_model + 6976 + 16 * i, zero, one);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Identity(base, car_model + 7040 + 64 * i);
    Position(base, car_model + 7168 + 16 * i, zero, one);
  }
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, car_model + 7232 + 4 * i, 0xFFFFFFFFu);
  }
  Store(base, car_model + 7200, 0);
  Store(base, car_model + 7216, zero);
  Store(base, car_model + 7220, zero);
  Store(base, car_model + 7224, zero);
  Store(base, car_model + 7228, one);
  return car_model + 7040 + 64;
}

// --- The car parts ---

bool CarPartsLoadsConstruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D6F88 CarPartsLoads::CarPartsLoads");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  for (uint32_t i = 0; i < kParts; i++) {
    Require(models::PartModelConstruct(base, loads + kPartModels + models::kPartModelBytes * i),
            "PartModel::PartModel");
  }
  const uint32_t one = Load(base, kOne);
  for (uint32_t i = 0; i < 30; i++) {
    Store(base, loads + 105436 + 16 * i, one);
  }
  return true;
}

bool CarPartsConstruct(uint8_t* base, uint32_t parts, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D7428 CarParts::CarParts");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots) ||
      !FreshReleasable(base)) {
    return false;
  }
  PoolNodeConstruct(base, parts, list);
  Store(base, parts + 20, 0xFFFFFFFFu);
  Store(base, parts + 0, kCarPartsVtable);
  Store(base, parts + 16, 0);
  Require(CarPartsLoadsConstruct(base, parts + kCarPartsLoads), "CarPartsLoads::CarPartsLoads");
  Store(base, parts + 24, 0);
  for (uint32_t i = 0; i < kParts; i++) {
    const uint32_t model = parts + kPartsModels + kPartModelBytes * i;
    car_models::LoadSlotReset(base, parts + kCarPartsLoads + kSlotBytes * i);
    Require(models::ModelRelease(base, model).has_value(), "Model::Release");
    models::PartModelClear(base, model);
  }
  scene_models::PartsRecordReset(base, parts + 28);
  Store(base, parts + kPartsSet, Load(base, kFirstSet));
  return true;
}

std::optional<uint32_t> CarPartsRelease(uint8_t* base, uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820D6D00 CarParts::Release");
  if (!SlotsReleasable(base, parts + kCarPartsLoads, kParts) ||
      !ModelsReleasable(base, parts + kPartsModels, kParts)) {
    return std::nullopt;
  }
  uint32_t model = 0;
  for (uint32_t i = 0; i < kParts; i++) {
    model = parts + kPartsModels + kPartModelBytes * i;
    Require(car_models::LoadSlotReleaseBuffer(base, parts + kCarPartsLoads + kSlotBytes * i)
                .has_value(),
            "LoadSlot::ReleaseBuffer");
    Require(models::ModelRelease(base, model).has_value(), "Model::Release");
    models::PartModelClear(base, model);
  }
  return model;
}

std::optional<uint32_t> CarPartsLoadsDestruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D6FE8 CarPartsLoads::~CarPartsLoads");
  const uint32_t first = loads + kPartModels;
  for (uint32_t i = 0; i < kParts; i++) {
    if (!models::ModelMembersDestructible(base, first + models::kPartModelBytes * i)) {
      return std::nullopt;
    }
  }
  if (!ModelsReleasable(base, first, kParts)) {
    return std::nullopt;
  }
  std::optional<uint32_t> r3;
  for (uint32_t i = kParts; i-- > 0;) {
    r3 = models::PartModelDestruct(base, first + models::kPartModelBytes * i);
    Require(r3.has_value(), "PartModel::~PartModel");
  }
  return r3;
}

// --- The cockpit body and the interior ---

namespace {

// Whether every slot in `slots` (offsets in `object`) is released natively,
// each freeing pages no other frees.
template <size_t N>
bool AllReleasable(uint8_t* base, uint32_t object, const uint32_t (&slots)[N]) {
  std::unordered_set<uint32_t> pages;
  for (uint32_t offset : slots) {
    uint32_t freed = 0;
    if (!car_models::LoadSlotReleasable(base, object + offset, &freed) ||
        (freed != 0 && !pages.insert(freed).second)) {
      return false;
    }
  }
  return true;
}

// The cockpit's +3472 to +3504 (the accessory node's matrix pointer, its
// position (0, 0, 0, 1) copied from the original's frame as doublewords,
// +3504).
void CockpitNode(uint8_t* base, uint32_t cockpit, uint32_t zero, uint32_t one) {
  Store(base, cockpit + 3472, 0);
  Store(base, cockpit + 3488, zero);
  Store(base, cockpit + 3492, zero);
  Store(base, cockpit + 3496, zero);
  Store(base, cockpit + 3500, one);
  Store(base, cockpit + 3504, 0);
}

// The six parts' slots and models in the cockpit (+536 + 24i, +688 + 464i)
// and the interior (+160 + 24i, +304 + 464i): each slot reset,
// Model::Release and PartModel::Clear on its model, the checks made.
void ResetParts(uint8_t* base, uint32_t slots, uint32_t models_at) {
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t model = models_at + kPartModelBytes * i;
    car_models::LoadSlotReset(base, slots + kSlotBytes * i);
    Require(models::ModelRelease(base, model).has_value(), "Model::Release");
    models::PartModelClear(base, model);
  }
}

// The cockpit's two XPR2s (+400 + 24i slots, +448 + 24i XPRs, +496 + 20i
// providers): with `release`, each slot's buffer released first; the slot
// reset, XPR::Release, the provider's slot 2 (ClearAll), the checks made.
void ResetXprs(uint8_t* base, uint32_t cockpit, bool release) {
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t slot = cockpit + 400 + kSlotBytes * i;
    if (release) {
      Require(car_models::LoadSlotReleaseBuffer(base, slot).has_value(),
              "LoadSlot::ReleaseBuffer");
    }
    car_models::LoadSlotReset(base, slot);
    Require(models::XprRelease(base, cockpit + 448 + kSlotBytes * i).has_value(),
            "XPR::Release");
    scenery_lists::TextureProviderClearAll(base, cockpit + 496 + 20 * i);
  }
}

}  // namespace

bool CockpitConstruct(uint8_t* base, uint32_t cockpit, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D63A0 Cockpit::Cockpit");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots) ||
      Load(base, models::kTextureProviderVtable + 8) != kTextureProviderClearAll ||
      !models::PendingFreesHaveRoom(base, 2)) {
    return false;
  }
  PoolNodeConstruct(base, cockpit, list);
  Store(base, cockpit + 20, 0xFFFFFFFFu);
  Store(base, cockpit + 0, kCockpitVtable);
  Store(base, cockpit + 16, 0);
  Require(CockpitLoadsConstruct(base, cockpit + kCockpitLoads), "CockpitLoads::CockpitLoads");
  const uint32_t one = Load(base, kOne);
  Store(base, cockpit + 3500, one);
  const uint32_t set = Load(base, kFirstSet);
  Store(base, cockpit + 24, 0);
  Store(base, cockpit + 3508, set);
  Require(model_animation::SkeletonFreeNodeValues(base, cockpit + 208),
          "Skeleton::FreeNodeValues");
  scene_models::CockpitRecordReset(base, cockpit + 28);
  CockpitNode(base, cockpit, Load(base, kZero), one);
  car_models::LoadSlotReset(base, cockpit + kCockpitLoads);
  ResetXprs(base, cockpit, false);
  ResetParts(base, cockpit + 536, cockpit + 688);
  return true;
}

std::optional<uint32_t> CockpitRelease(uint8_t* base, uint32_t cockpit) {
  ITC_NATIVE_RUNNING("sub_820D6238 Cockpit::Release");
  static constexpr uint32_t kSlots[] = {176, 400, 424, 536, 560, 584, 608, 632, 656};
  const uint32_t values = Load(base, cockpit + 208 + 176);
  if (!AllReleasable(base, cockpit, kSlots) ||
      (values != 0 && !memory::IsHeapBlock(values)) ||
      !ProvidersClearAll(base, cockpit + kCockpitLoads, kCarModelMembers) ||
      !models::PendingFreesHaveRoom(base, 4)) {
    return std::nullopt;
  }
  Require(car_models::LoadSlotReleaseBuffer(base, cockpit + kCockpitLoads).has_value(),
          "LoadSlot::ReleaseBuffer");
  car_models::LoadSlotReset(base, cockpit + kCockpitLoads);
  Require(model_animation::SkeletonFreeNodeValues(base, cockpit + 208),
          "Skeleton::FreeNodeValues");
  ResetXprs(base, cockpit, true);
  Store(base, cockpit + 3508, 0);
  Store(base, cockpit + 24, 0);
  scene_models::CockpitRecordReset(base, cockpit + 28);
  const uint32_t zero = Load(base, kZero);
  CockpitNode(base, cockpit, zero, Load(base, kOne));
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t slot = cockpit + 536 + kSlotBytes * i;
    Require(car_models::LoadSlotReleaseBuffer(base, slot).has_value(), "LoadSlot::ReleaseBuffer");
    car_models::LoadSlotReset(base, slot);
  }
  return cockpit + 536 + kSlotBytes * 5;
}

bool InteriorConstruct(uint8_t* base, uint32_t interior, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D78B8 Interior::Interior");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots) ||
      !models::PendingFreesHaveRoom(base, 2)) {
    return false;
  }
  PoolNodeConstruct(base, interior, list);
  Store(base, interior + 16, 0);
  Store(base, interior + 0, kInteriorVtable);
  Store(base, interior + 20, 0xFFFFFFFFu);
  for (uint32_t i = 0; i < 6; i++) {
    Require(models::PartModelConstruct(base, interior + 304 + kPartModelBytes * i),
            "PartModel::PartModel");
  }
  Store(base, interior + 24, 0);
  ResetParts(base, interior + 160, interior + 304);
  scene_models::InteriorRecordReset(base, interior + 28);
  const uint32_t set = Load(base, kFirstSet);
  Store(base, interior + 3088, 0);
  Store(base, interior + 3092, set);
  return true;
}

// --- The wheels and the traffic body ---

std::optional<uint32_t> WheelsReset(uint8_t* base, uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8B58 Wheels::Reset");
  const uint32_t loads = wheels + kWheelsLoads;
  // WheelsLoads::Reset's checks, before anything changes (the record's
  // defaults don't touch what they read).
  uint64_t nodes = 4;
  for (uint32_t offset : {kWheelsTires, kWheelsTires + 464, kWheelsRims, kWheelsRims + 464,
                          kWheelsCalipers, kWheelsCalipers + 464}) {
    nodes += models::ModelReleaseNodes(base, loads + offset);
  }
  for (uint32_t i = 0; i < 2; i++) {
    if (Load(base, Load(base, loads + kWheelsProviders + 20 * i) + 8) !=
        kTextureProviderClearAll) {
      return std::nullopt;
    }
  }
  if (nodes > 0x3FFFFFFFu || !models::PendingFreesHaveRoom(base, uint32_t(nodes))) {
    return std::nullopt;
  }
  Store(base, wheels + 24, 0);
  Store(base, wheels + 3268, Load(base, kFirstSet));
  scene_models::WheelsRecordReset(base, wheels + 28);
  const auto r3 = WheelsLoadsReset(base, loads);
  Require(r3.has_value(), "WheelsLoads::Reset");
  return r3;
}

std::optional<uint32_t> TrafficReset(uint8_t* base, uint32_t traffic) {
  ITC_NATIVE_RUNNING("sub_820D7AB0 TrafficCar::Reset");
  const uint32_t loads = traffic + kTrafficLoads;
  const uint32_t values = Load(base, loads + kTrafficMembers.skeleton + 176);
  uint64_t nodes = 4;
  for (uint32_t i = 0; i < 6; i++) {
    nodes += models::ModelReleaseNodes(base, traffic + 4256 + kPartModelBytes * i);
  }
  if ((values != 0 && !memory::IsHeapBlock(values)) ||
      !ProvidersClearAll(base, loads, kTrafficMembers) || nodes > 0x3FFFFFFFu ||
      !models::PendingFreesHaveRoom(base, uint32_t(nodes))) {
    return std::nullopt;
  }
  Store(base, traffic + 7392, Load(base, kFirstSet));
  scene_models::CarRecordReset(base, traffic + 32);
  Store(base, traffic + 7056, 0xFFFFFFFFu);
  car_models::LoadSlotReset(base, traffic + 3744);
  Require(model_animation::SkeletonFreeNodeValues(base, loads + kTrafficMembers.skeleton),
          "Skeleton::FreeNodeValues");
  for (uint32_t i = 0; i < 2; i++) {
    car_models::LoadSlotReset(base, traffic + 3968 + kSlotBytes * i);
    Require(models::XprRelease(base, loads + kTrafficMembers.xprs[i]).has_value(),
            "XPR::Release");
    scenery_lists::TextureProviderClearAll(base, loads + kTrafficMembers.providers[i]);
  }
  for (uint32_t offset : {3728u, 3732u, 3736u, 3740u, 7040u, 7044u, 7048u, 7052u}) {
    Store(base, traffic + offset, 0);
  }
  ResetParts(base, traffic + 4104, traffic + 4256);
  return traffic + 4256 + kPartModelBytes * 5;
}

// --- Pool 4's object ---

std::optional<uint32_t> Pool4Reset(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820D6948 Pool4Object::Reset");
  const uint32_t model = object + 112;
  if (!ModelsReleasable(base, model, 1)) {
    return std::nullopt;
  }
  Store(base, object + 24, 0);
  scene_models::Pool4RecordReset(base, object + 28);
  car_models::LoadSlotReset(base, object + 80);
  Require(models::ModelRelease(base, model).has_value(), "Model::Release");
  models::PartModelClear(base, model);
  Store(base, object + 576, Load(base, kFirstSet));
  return model;
}

}  // namespace scene_loaders
