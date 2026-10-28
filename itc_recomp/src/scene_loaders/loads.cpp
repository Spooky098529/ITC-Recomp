// The loading objects' embedded loads (see loads.h), member by member in the
// originals' order.

#include "scene_loaders/loads.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/texture_provider.h"
#include "models/xpr.h"

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

}  // namespace scene_loaders
