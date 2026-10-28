// The loading objects' embedded loads (see loads.h), member by member in the
// originals' order.

#include "scene_loaders/loads.h"

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

#include <cstdlib>

namespace scene_loaders {

namespace {

// A member whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene loaders: {} refused after its checks", what);
    std::abort();
  }
}

// Whether TextureProvider::~TextureProvider frees the provider's storage
// natively (none, or a block the layer gave).
bool ProviderDestructible(uint8_t* base, uint32_t provider) {
  const uint32_t storage = guest::LoadU32(
      base, provider + models::kTextureProviderVector + models::kVectorFirst);
  return storage == 0 || memory::IsHeapBlock(storage);
}

// The members CarModelLoads, CockpitLoads and TrafficLoads share, made and
// destroyed with their checks made.
void ConstructMembers(uint8_t* base, uint32_t loads);
void DestructMembers(uint8_t* base, uint32_t loads);

}  // namespace

bool CarModelLoadsConstruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D3DE0 CarModelLoads::CarModelLoads");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  ConstructMembers(base, loads);
  return true;
}

namespace {

void ConstructMembers(uint8_t* base, uint32_t loads) {
  model_animation::SkeletonConstruct(base, loads + kCarModelLoadsSkeleton);
  for (uint32_t offset : kCarModelLoadsXprs) {
    models::XprConstruct(base, loads + offset);
  }
  for (uint32_t offset : kCarModelLoadsProviders) {
    Require(models::TextureProviderConstruct(base, loads + offset, -1).has_value(),
            "TextureProvider::TextureProvider");
  }
}

}  // namespace

bool CarModelLoadsDestructible(uint8_t* base, uint32_t loads) {
  // XPR::Release checks for two nodes each time.
  return ProviderDestructible(base, loads + kCarModelLoadsProviders[0]) &&
         ProviderDestructible(base, loads + kCarModelLoadsProviders[1]) &&
         models::PendingFreesHaveRoom(base, 4) &&
         model_animation::SkeletonDestructible(base, loads + kCarModelLoadsSkeleton);
}

bool ProvidersClearAll(uint8_t* base, uint32_t loads) {
  for (uint32_t offset : kCarModelLoadsProviders) {
    const uint32_t vtable = guest::LoadU32(base, loads + offset);
    if (guest::LoadU32(base, vtable + 8) != kTextureProviderClearAll) {
      return false;
    }
  }
  return true;
}

bool CarModelLoadsDestruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D3E40 CarModelLoads::~CarModelLoads");
  if (!CarModelLoadsDestructible(base, loads)) {
    return false;
  }
  DestructMembers(base, loads);
  return true;
}

namespace {

void DestructMembers(uint8_t* base, uint32_t loads) {
  Require(models::TextureProviderDestruct(base, loads + kCarModelLoadsProviders[1]).has_value(),
          "TextureProvider::~TextureProvider");
  Require(models::TextureProviderDestruct(base, loads + kCarModelLoadsProviders[0]).has_value(),
          "TextureProvider::~TextureProvider");
  Require(models::XprDestruct(base, loads + kCarModelLoadsXprs[1]).has_value(), "XPR::~XPR");
  Require(models::XprDestruct(base, loads + kCarModelLoadsXprs[0]).has_value(), "XPR::~XPR");
  Require(model_animation::SkeletonDestruct(base, loads + kCarModelLoadsSkeleton),
          "Skeleton::~Skeleton");
}

}  // namespace

bool CockpitLoadsConstruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D6010 CockpitLoads::CockpitLoads");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  ConstructMembers(base, loads);
  for (uint32_t i = 0; i < 6; i++) {
    Require(models::PartModelConstruct(base, loads + kCockpitLoadsParts +
                                                 models::kPartModelBytes * i),
            "PartModel::PartModel");
  }
  return true;
}

std::optional<uint32_t> CockpitLoadsDestruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D6090 CockpitLoads::~CockpitLoads");
  uint64_t nodes = 4;  // the XPRs'
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t model = loads + kCockpitLoadsParts + models::kPartModelBytes * i;
    if (!models::ModelMembersDestructible(base, model)) {
      return std::nullopt;
    }
    nodes += models::ModelReleaseNodes(base, model);
  }
  if (nodes > 0x3FFFFFFFu || !models::PendingFreesHaveRoom(base, uint32_t(nodes)) ||
      !CarModelLoadsDestructible(base, loads)) {
    return std::nullopt;
  }
  for (uint32_t i = 6; i-- > 0;) {
    Require(models::PartModelDestruct(base, loads + kCockpitLoadsParts +
                                                models::kPartModelBytes * i)
                .has_value(),
            "PartModel::~PartModel");
  }
  DestructMembers(base, loads);
  return loads + kCarModelLoadsSkeleton;
}

bool CarModelLoadsResettable(uint8_t* base, uint32_t loads) {
  const uint32_t values = guest::LoadU32(base, loads + kCarModelLoadsSkeleton + 176);
  if (values != 0 && !memory::IsHeapBlock(values)) {
    return false;
  }
  return ProvidersClearAll(base, loads) && models::PendingFreesHaveRoom(base, 4);
}

std::optional<uint32_t> CarModelLoadsReset(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D3B58 CarModelLoads::Reset");
  if (!CarModelLoadsResettable(base, loads)) {
    return std::nullopt;
  }
  car_models::LoadSlotReset(base, loads);
  Require(model_animation::SkeletonFreeNodeValues(base, loads + kCarModelLoadsSkeleton),
          "Skeleton::FreeNodeValues");
  for (uint32_t i = 0; i < 2; i++) {
    car_models::LoadSlotReset(base, loads + 224 + 24 * i);
    Require(models::XprRelease(base, loads + kCarModelLoadsXprs[i]).has_value(), "XPR::Release");
    scenery_lists::TextureProviderClearAll(base, loads + kCarModelLoadsProviders[i]);
  }
  guest::StoreU32(base, loads + 360, 0);
  for (uint32_t i = 0; i < 3; i++) {
    car_models::LoadSlotReset(base, loads + 364 + 48 * i);
    car_models::LoadSlotReset(base, loads + 388 + 48 * i);
  }
  for (uint32_t i = 0; i < 6; i++) {
    car_models::LoadSlotReset(base, loads + 652 + 48 * i);
    car_models::LoadSlotReset(base, loads + 676 + 48 * i);
  }
  for (uint32_t offset : {2956u, 2960u, 2972u, 2976u, 2980u, 2984u}) {
    guest::StoreU32(base, loads + offset, 0);
  }
  return loads + 676 + 48 * 5;
}

}  // namespace scene_loaders
