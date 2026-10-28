// The scene's loading objects' resets (see objects.h).

#include "scene_loaders/objects.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
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
