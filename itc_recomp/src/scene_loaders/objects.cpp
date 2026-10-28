// The scene's loading objects' resets (see objects.h).

#include "scene_loaders/objects.h"

#include "base/log.h"
#include "car_models/car_model.h"
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

namespace {

// Cockpit::Release's checks (nothing changed), with `extra` nodes more for
// what follows it.
bool CockpitReleasable(uint8_t* base, uint32_t cockpit, uint64_t extra) {
  static constexpr uint32_t kSlots[] = {176, 400, 424, 536, 560, 584, 608, 632, 656};
  const uint32_t values = Load(base, cockpit + 208 + 176);
  return AllReleasable(base, cockpit, kSlots) &&
         (values == 0 || memory::IsHeapBlock(values)) &&
         ProvidersClearAll(base, cockpit + kCockpitLoads, kCarModelMembers) &&
         4 + extra <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(base, uint32_t(4 + extra));
}

}  // namespace

std::optional<uint32_t> CockpitRelease(uint8_t* base, uint32_t cockpit) {
  ITC_NATIVE_RUNNING("sub_820D6238 Cockpit::Release");
  if (!CockpitReleasable(base, cockpit, 0)) {
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

std::optional<uint32_t> InteriorOnPartLoaded(uint8_t* base, uint32_t request, uint32_t interior,
                                             const PartLoadedCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820D7578 Interior::OnPartLoaded");
  for (uint32_t i = 0; i < 6; i++) {
    if (Load(base, interior + 164 + kSlotBytes * i) != request) {
      continue;
    }
    const uint32_t slot = interior + 160 + kSlotBytes * i;
    uint32_t freed = 0;
    if (!car_models::LoadSlotReleasable(base, slot, &freed)) {
      return std::nullopt;
    }
    const uint32_t model = interior + 304 + kPartModelBytes * i;
    const uint32_t file = Load(base, slot + 16);
    if (!models::ModelLoad(base, model, file, 0, calls.model_load)) {
      calls.load(calls.user, model, file);
    }
    const auto r3 = car_models::LoadSlotReleaseBuffer(base, slot);
    Require(r3.has_value(), "LoadSlot::ReleaseBuffer");
    Store(base, slot + 20, 3);
    return r3;
  }
  return request;
}

// --- The wheels and the traffic body ---

namespace {

// WheelsLoads::Reset's pending frees' nodes (the XPRs' four and the six
// models'), or nullopt where a provider's slot 2 isn't ClearAll; nothing
// changed.
std::optional<uint64_t> WheelsResetNodes(uint8_t* base, uint32_t wheels) {
  const uint32_t loads = wheels + kWheelsLoads;
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
  return nodes;
}

bool Room(uint8_t* base, uint64_t nodes) {
  return nodes <= 0x3FFFFFFFu && models::PendingFreesHaveRoom(base, uint32_t(nodes));
}

}  // namespace

std::optional<uint32_t> WheelsReset(uint8_t* base, uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8B58 Wheels::Reset");
  const uint32_t loads = wheels + kWheelsLoads;
  // WheelsLoads::Reset's checks, before anything changes (the record's
  // defaults don't touch what they read).
  const auto nodes = WheelsResetNodes(base, wheels);
  if (!nodes || !Room(base, *nodes)) {
    return std::nullopt;
  }
  Store(base, wheels + 24, 0);
  Store(base, wheels + 3268, Load(base, kFirstSet));
  scene_models::WheelsRecordReset(base, wheels + 28);
  const auto r3 = WheelsLoadsReset(base, loads);
  Require(r3.has_value(), "WheelsLoads::Reset");
  return r3;
}

namespace {

// TrafficCar::Reset's pending frees' nodes (the XPRs' four and the six
// models'), or nullopt where the node values aren't the layer's or a
// provider's slot 2 isn't ClearAll; nothing changed.
std::optional<uint64_t> TrafficResetNodes(uint8_t* base, uint32_t traffic) {
  const uint32_t loads = traffic + kTrafficLoads;
  const uint32_t values = Load(base, loads + kTrafficMembers.skeleton + 176);
  uint64_t nodes = 4;
  for (uint32_t i = 0; i < 6; i++) {
    nodes += models::ModelReleaseNodes(base, traffic + 4256 + kPartModelBytes * i);
  }
  if ((values != 0 && !memory::IsHeapBlock(values)) ||
      !ProvidersClearAll(base, loads, kTrafficMembers)) {
    return std::nullopt;
  }
  return nodes;
}

}  // namespace

std::optional<uint32_t> TrafficReset(uint8_t* base, uint32_t traffic) {
  ITC_NATIVE_RUNNING("sub_820D7AB0 TrafficCar::Reset");
  const uint32_t loads = traffic + kTrafficLoads;
  const auto nodes = TrafficResetNodes(base, traffic);
  if (!nodes || !Room(base, *nodes)) {
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

// --- The lifetimes ---

namespace {

// The pool node's part of a constructor: PoolNode::PoolNode, then +16 = 0,
// the vtable, +20 = -1 (the originals' order).
void ConstructNode(uint8_t* base, uint32_t object, uint32_t list, uint32_t vtable) {
  PoolNodeConstruct(base, object, list);
  Store(base, object + 16, 0);
  Store(base, object + 0, vtable);
  Store(base, object + 20, 0xFFFFFFFFu);
}

// The end of every destructor: the base vtable, PoolNode::~PoolNode.
void DestructNode(uint8_t* base, uint32_t object) {
  Store(base, object + 0, kPoolObjectVtable);
  PoolNodeDestruct(base, object);
}

// A fresh part model's Model::Release (after its constructor): the XPR's two
// nodes, the blocks all 0.
constexpr uint64_t kFreshNodes = 2;

// The wheels' eight slots in Wheels::Release's order.
constexpr uint32_t kWheelsSlots[] = {192, 1168, 2144, 2280, 216, 1192, 2168, 2304};

}  // namespace

bool Pool4Construct(uint8_t* base, uint32_t object, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D6A70 Pool4Object::Pool4Object");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots) ||
      !Room(base, kFreshNodes)) {
    return false;
  }
  ConstructNode(base, object, list, kPool4Vtable);
  Require(models::PartModelConstruct(base, object + 112), "PartModel::PartModel");
  Require(Pool4Reset(base, object).has_value(), "Pool4Object::Reset");
  return true;
}

namespace {

// Pool4Object::Release's checks (nothing changed), with `extra` nodes more
// for what follows it: the slot's buffer, then room for Model::Release's
// nodes and Pool4Object::Reset's (a released model's two).
bool Pool4Releasable(uint8_t* base, uint32_t object, uint64_t extra) {
  uint32_t freed = 0;
  return car_models::LoadSlotReleasable(base, object + 80, &freed) &&
         Room(base, models::ModelReleaseNodes(base, object + 112) + kFreshNodes + extra);
}

// Pool4Object::Release's work, its checks made.
uint32_t Pool4ReleaseWork(uint8_t* base, uint32_t object) {
  Require(car_models::LoadSlotReleaseBuffer(base, object + 80).has_value(),
          "LoadSlot::ReleaseBuffer");
  Require(models::ModelRelease(base, object + 112).has_value(), "Model::Release");
  const auto r3 = Pool4Reset(base, object);
  Require(r3.has_value(), "Pool4Object::Reset");
  return *r3;
}

}  // namespace

std::optional<uint32_t> Pool4Release(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820D6A20 Pool4Object::Release");
  if (!Pool4Releasable(base, object, 0)) {
    return std::nullopt;
  }
  return Pool4ReleaseWork(base, object);
}

bool Pool4Destruct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820D6AD0 Pool4Object::~Pool4Object");
  // The model's members aren't touched by its release; ~Model releases a
  // released model again (two nodes).
  if (!models::ModelMembersDestructible(base, object + 112) ||
      !Pool4Releasable(base, object, kFreshNodes)) {
    return false;
  }
  Store(base, object + 0, kPool4Vtable);
  Pool4ReleaseWork(base, object);
  Require(models::PartModelDestruct(base, object + 112).has_value(), "PartModel::~PartModel");
  DestructNode(base, object);
  return true;
}

bool Pool4DeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D6BC0 Pool4Object::DeletingDestructor");
  return (flags & 1) == 0 && Pool4Destruct(base, object);
}

bool WheelsConstruct(uint8_t* base, uint32_t wheels, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D9570 Wheels::Wheels");
  // WheelsLoads::WheelsLoads' check, and Wheels::Reset's on the fresh loads
  // (the providers' vtable's slot 2, the XPRs' four nodes and the six fresh
  // models').
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots) ||
      Load(base, models::kTextureProviderVtable + 8) != kTextureProviderClearAll ||
      !Room(base, 4 + 6 * kFreshNodes)) {
    return false;
  }
  ConstructNode(base, wheels, list, kWheelsVtable);
  scene_models::Record12Reset(base, wheels + 28 + 44);
  scene_models::Record12Reset(base, wheels + 28 + 56);
  Require(WheelsLoadsConstruct(base, wheels + kWheelsLoads), "WheelsLoads::WheelsLoads");
  Require(WheelsReset(base, wheels).has_value(), "Wheels::Reset");
  return true;
}

namespace {

// Wheels::Release's checks (nothing changed), with `extra` nodes more for
// what follows it.
bool WheelsReleasable(uint8_t* base, uint32_t wheels, uint64_t extra) {
  const auto nodes = WheelsResetNodes(base, wheels);
  return AllReleasable(base, wheels, kWheelsSlots) && nodes && Room(base, *nodes + extra);
}

uint32_t WheelsReleaseWork(uint8_t* base, uint32_t wheels) {
  for (uint32_t offset : kWheelsSlots) {
    Require(car_models::LoadSlotReleaseBuffer(base, wheels + offset).has_value(),
            "LoadSlot::ReleaseBuffer");
  }
  const auto r3 = WheelsReset(base, wheels);
  Require(r3.has_value(), "Wheels::Reset");
  return *r3;
}

}  // namespace

std::optional<uint32_t> WheelsRelease(uint8_t* base, uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D8E68 Wheels::Release");
  if (!WheelsReleasable(base, wheels, 0)) {
    return std::nullopt;
  }
  return WheelsReleaseWork(base, wheels);
}

bool WheelsDestruct(uint8_t* base, uint32_t wheels) {
  ITC_NATIVE_RUNNING("sub_820D90B8 Wheels::~Wheels");
  // ~WheelsLoads after the reset: the XPRs' four nodes and the six released
  // models' two each; the members aren't touched by the release.
  if (!WheelsLoadsMembersDestructible(base, wheels + kWheelsLoads) ||
      !WheelsReleasable(base, wheels, 4 + 6 * kFreshNodes)) {
    return false;
  }
  Store(base, wheels + 0, kWheelsVtable);
  WheelsReleaseWork(base, wheels);
  Require(WheelsLoadsDestruct(base, wheels + kWheelsLoads).has_value(),
          "WheelsLoads::~WheelsLoads");
  DestructNode(base, wheels);
  return true;
}

bool WheelsDeletingDestruct(uint8_t* base, uint32_t wheels, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D95E8 Wheels::DeletingDestructor");
  return (flags & 1) == 0 && WheelsDestruct(base, wheels);
}

// --- The car model, the car parts and the cockpit's destructors ---

namespace {

// The w (+12) of each of a matrix's four rows.
void MatrixW(uint8_t* base, uint32_t matrix, uint32_t one) {
  for (uint32_t row = 0; row < 4; row++) {
    Store(base, matrix + 16 * row + 12, one);
  }
}

}  // namespace

bool CarModelConstruct(uint8_t* base, uint32_t car_model, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D5890 CarModel::CarModel");
  // CarModelLoads::CarModelLoads' check, and CarModel::Reset's on the fresh
  // loads (no node values, the providers' vtable's slot 2, the XPRs' four
  // nodes).
  if (car_model >= kPhysical || kPhysical - car_model < kCarModelBytes ||
      !models::CanAllocatePointers(base, models::kTextureProviderSlots) ||
      Load(base, models::kTextureProviderVtable + 8) != kTextureProviderClearAll ||
      !models::PendingFreesHaveRoom(base, 4)) {
    return false;
  }
  ConstructNode(base, car_model, list, kCarModelVtable);
  scene_models::CarRecordSetW(base, car_model + kCarModelRecord);
  Require(CarModelLoadsConstruct(base, car_model + kCarModelLoads),
          "CarModelLoads::CarModelLoads");
  const uint32_t one = Load(base, kOne);
  for (uint32_t i = 0; i < 4; i++) {
    MatrixW(base, car_model + 6720 + 64 * i, one);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, car_model + 6976 + 16 * i + 12, one);
  }
  for (uint32_t i = 0; i < 2; i++) {
    MatrixW(base, car_model + 7040 + 64 * i, one);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Store(base, car_model + 7168 + 16 * i + 12, one);
  }
  Store(base, car_model + 7228, one);
  Require(CarModelReset(base, car_model).has_value(), "CarModel::Reset");
  return true;
}

bool CarModelDestruct(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D4750 CarModel::~CarModel");
  // The release's checks hold for ~CarModelLoads after it (the node values
  // it checks are freed by the release); the XPRs check for four nodes in
  // each.
  if (!car_models::CarModelReleasable(base, car_model) ||
      !CarModelLoadsDestructible(base, car_model + kCarModelLoads) ||
      !models::PendingFreesHaveRoom(base, 8)) {
    return false;
  }
  Store(base, car_model + 0, kCarModelVtable);
  Require(car_models::CarModelRelease(base, car_model).has_value(), "CarModel::Release");
  Require(CarModelLoadsDestruct(base, car_model + kCarModelLoads), "CarModelLoads::~CarModelLoads");
  DestructNode(base, car_model);
  return true;
}

bool CarModelDeletingDestruct(uint8_t* base, uint32_t car_model, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D5988 CarModel::DeletingDestructor");
  return (flags & 1) == 0 && CarModelDestruct(base, car_model);
}

bool CarPartsDestruct(uint8_t* base, uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820D7038 CarParts::~CarParts");
  // The release's checks, then ~CarPartsLoads' on the released models (their
  // members untouched, two nodes each).
  const uint32_t first = parts + kPartsModels;
  uint64_t nodes = 0;
  for (uint32_t i = 0; i < kParts; i++) {
    const uint32_t model = first + kPartModelBytes * i;
    if (!models::ModelMembersDestructible(base, model)) {
      return false;
    }
    nodes += models::ModelReleaseNodes(base, model);
  }
  if (!SlotsReleasable(base, parts + kCarPartsLoads, kParts) ||
      !Room(base, nodes + kFreshNodes * kParts)) {
    return false;
  }
  Store(base, parts + 0, kCarPartsVtable);
  Require(CarPartsRelease(base, parts).has_value(), "CarParts::Release");
  Require(CarPartsLoadsDestruct(base, parts + kCarPartsLoads).has_value(),
          "CarPartsLoads::~CarPartsLoads");
  DestructNode(base, parts);
  return true;
}

bool CarPartsDeletingDestruct(uint8_t* base, uint32_t parts, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D74C0 CarParts::DeletingDestructor");
  return (flags & 1) == 0 && CarPartsDestruct(base, parts);
}

bool CockpitDestruct(uint8_t* base, uint32_t cockpit) {
  ITC_NATIVE_RUNNING("sub_820D64D0 Cockpit::~Cockpit");
  // The release doesn't release the six part models: ~CockpitLoads' nodes
  // are theirs now, and the XPRs' four.
  const uint32_t loads = cockpit + kCockpitLoads;
  uint64_t nodes = 4;
  for (uint32_t i = 0; i < 6; i++) {
    nodes += models::ModelReleaseNodes(base, loads + kCockpitLoadsParts + kPartModelBytes * i);
  }
  if (!CockpitLoadsMembersDestructible(base, loads) || !CockpitReleasable(base, cockpit, nodes)) {
    return false;
  }
  Store(base, cockpit + 0, kCockpitVtable);
  Require(CockpitRelease(base, cockpit).has_value(), "Cockpit::Release");
  Require(CockpitLoadsDestruct(base, loads).has_value(), "CockpitLoads::~CockpitLoads");
  DestructNode(base, cockpit);
  return true;
}

bool CockpitDeletingDestruct(uint8_t* base, uint32_t cockpit, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D6798 Cockpit::DeletingDestructor");
  return (flags & 1) == 0 && CockpitDestruct(base, cockpit);
}

// --- The traffic body's lifetime ---

bool TrafficConstruct(uint8_t* base, uint32_t traffic, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820D8908 TrafficCar::TrafficCar");
  // TrafficLoads::TrafficLoads' check, and TrafficCar::Reset's on the fresh
  // loads (no node values, the providers' vtable's slot 2, the XPRs' four
  // nodes and the six fresh models').
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots) ||
      Load(base, models::kTextureProviderVtable + 8) != kTextureProviderClearAll ||
      !Room(base, 4 + 6 * kFreshNodes)) {
    return false;
  }
  ConstructNode(base, traffic, list, kTrafficVtable);
  scene_models::CarRecordSetW(base, traffic + 32);
  Require(TrafficLoadsConstruct(base, traffic + kTrafficLoads), "TrafficLoads::TrafficLoads");
  const uint32_t one = Load(base, kOne);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, traffic + 7072 + 16 * i + 12, one);
  }
  for (uint32_t i = 0; i < 4; i++) {
    MatrixW(base, traffic + 7136 + 64 * i, one);
  }
  Require(TrafficReset(base, traffic).has_value(), "TrafficCar::Reset");
  return true;
}

namespace {

// TrafficCar::Release's checks (nothing changed), with `extra` nodes more
// for what follows it.
bool TrafficReleasable(uint8_t* base, uint32_t traffic, uint64_t extra) {
  uint32_t freed = 0;
  const auto nodes = TrafficResetNodes(base, traffic);
  return car_models::LoadSlotReleasable(base, traffic + 3744, &freed) && nodes &&
         Room(base, *nodes + extra);
}

uint32_t TrafficReleaseWork(uint8_t* base, uint32_t traffic) {
  for (uint32_t sheet : {3728u, 7040u}) {
    const uint32_t d = Load(base, traffic + sheet + 12);
    const uint32_t c = Load(base, traffic + sheet + 8);
    const uint32_t b = Load(base, traffic + sheet + 4);
    const uint32_t a = Load(base, traffic + sheet + 0);
    car_models::DeferRelease(base, Load(base, traffic + 7392), a, b, c, d);
  }
  Require(car_models::LoadSlotReleaseBuffer(base, traffic + 3744).has_value(),
          "LoadSlot::ReleaseBuffer");
  const auto r3 = TrafficReset(base, traffic);
  Require(r3.has_value(), "TrafficCar::Reset");
  return *r3;
}

}  // namespace

std::optional<uint32_t> TrafficRelease(uint8_t* base, uint32_t traffic) {
  ITC_NATIVE_RUNNING("sub_820D7B90 TrafficCar::Release");
  if (!TrafficReleasable(base, traffic, 0)) {
    return std::nullopt;
  }
  return TrafficReleaseWork(base, traffic);
}

bool TrafficDestruct(uint8_t* base, uint32_t traffic) {
  ITC_NATIVE_RUNNING("sub_820D7E90 TrafficCar::~TrafficCar");
  // ~TrafficLoads after the reset: the six released models' two nodes each
  // and the XPRs' four; the members aren't touched by the release (the node
  // values are freed by it).
  if (!TrafficLoadsMembersDestructible(base, traffic + kTrafficLoads) ||
      !TrafficReleasable(base, traffic, 6 * kFreshNodes + 4)) {
    return false;
  }
  Store(base, traffic + 0, kTrafficVtable);
  TrafficReleaseWork(base, traffic);
  Require(TrafficLoadsDestruct(base, traffic + kTrafficLoads).has_value(),
          "TrafficLoads::~TrafficLoads");
  DestructNode(base, traffic);
  return true;
}

bool TrafficDeletingDestruct(uint8_t* base, uint32_t traffic, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820D89B8 TrafficCar::DeletingDestructor");
  return (flags & 1) == 0 && TrafficDestruct(base, traffic);
}

}  // namespace scene_loaders
