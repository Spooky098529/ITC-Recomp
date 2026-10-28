// The loading objects' embedded loads (see loads.h), member by member in the
// originals' order.

#include "scene_loaders/loads.h"

#include "base/log.h"
#include "car_models/load_slots.h"
#include "guest.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
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

}  // namespace

bool CarModelLoadsConstruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D3DE0 CarModelLoads::CarModelLoads");
  if (!models::CanAllocatePointers(base, models::kTextureProviderSlots)) {
    return false;
  }
  model_animation::SkeletonConstruct(base, loads + kCarModelLoadsSkeleton);
  for (uint32_t offset : kCarModelLoadsXprs) {
    models::XprConstruct(base, loads + offset);
  }
  for (uint32_t offset : kCarModelLoadsProviders) {
    Require(models::TextureProviderConstruct(base, loads + offset, -1).has_value(),
            "TextureProvider::TextureProvider");
  }
  return true;
}

bool CarModelLoadsDestruct(uint8_t* base, uint32_t loads) {
  ITC_NATIVE_RUNNING("sub_820D3E40 CarModelLoads::~CarModelLoads");
  // XPR::Release checks for two nodes each time.
  if (!ProviderDestructible(base, loads + kCarModelLoadsProviders[0]) ||
      !ProviderDestructible(base, loads + kCarModelLoadsProviders[1]) ||
      !models::PendingFreesHaveRoom(base, 4) ||
      !model_animation::SkeletonDestructible(base, loads + kCarModelLoadsSkeleton)) {
    return false;
  }
  Require(models::TextureProviderDestruct(base, loads + kCarModelLoadsProviders[1]).has_value(),
          "TextureProvider::~TextureProvider");
  Require(models::TextureProviderDestruct(base, loads + kCarModelLoadsProviders[0]).has_value(),
          "TextureProvider::~TextureProvider");
  Require(models::XprDestruct(base, loads + kCarModelLoadsXprs[1]).has_value(), "XPR::~XPR");
  Require(models::XprDestruct(base, loads + kCarModelLoadsXprs[0]).has_value(), "XPR::~XPR");
  Require(model_animation::SkeletonDestruct(base, loads + kCarModelLoadsSkeleton),
          "Skeleton::~Skeleton");
  return true;
}

bool CarModelLoadsResettable(uint8_t* base, uint32_t loads) {
  const uint32_t values = guest::LoadU32(base, loads + kCarModelLoadsSkeleton + 176);
  if (values != 0 && !memory::IsHeapBlock(values)) {
    return false;
  }
  for (uint32_t offset : kCarModelLoadsProviders) {
    const uint32_t vtable = guest::LoadU32(base, loads + offset);
    if (guest::LoadU32(base, vtable + 8) != kTextureProviderClearAll) {
      return false;
    }
  }
  return models::PendingFreesHaveRoom(base, 4);
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
