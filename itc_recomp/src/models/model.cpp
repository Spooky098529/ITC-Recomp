// The model class (see model.h), store for store as the recompiled
// functions.

#include "models/model.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/texture_provider.h"
#include "models/xpr.h"

#include <cstdlib>

namespace models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// The five track sets, in the constructor's order.
constexpr uint32_t kTrackSets[] = {304, 324, 344, 364, 384};

// A callee whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Model: {} refused after its checks", what);
    std::abort();
  }
}

// The blocks Model::Release puts off, at most (each one not null; those
// without their header's tag are skipped), and XPR::Release's two.
uint32_t ReleaseNodes(uint8_t* base, uint32_t model) {
  uint32_t nodes = 2;
  for (uint32_t offset : {16u, 20u, kModelShaderPairs}) {
    nodes += Load(base, model + offset) != 0;
  }
  const uint32_t pairs = Load(base, model + kModelShaderPairs);
  const uint32_t count = Load(base, model + kModelShaderPairCount);
  for (uint32_t i = 0; i < count && nodes < 0x10000000u; i++) {
    for (uint32_t word = 0; word < 4; word++) {
      nodes += Load(base, pairs + 16 * i + 4 * word) != 0;
    }
  }
  return nodes;
}

// FreeLater on a block when it isn't null, its checks made.
void FreeIfAny(uint8_t* base, uint32_t block) {
  if (block != 0) {
    Require(FreeLater(base, block), "Memory::FreeLater");
  }
}

// Model::Release's work, its checks made.
uint32_t Release(uint8_t* base, uint32_t model) {
  FreeIfAny(base, Load(base, model + 16));
  FreeIfAny(base, Load(base, model + 20));
  for (uint32_t i = 0; i < Load(base, model + kModelShaderPairCount); i++) {
    const uint32_t pair = Load(base, model + kModelShaderPairs) + 16 * i;
    for (uint32_t word = 0; word < 4; word++) {
      FreeIfAny(base, Load(base, pair + 4 * word));
    }
  }
  FreeIfAny(base, Load(base, model + kModelShaderPairs));
  for (uint32_t offset : {16u, 20u, 24u, 420u, 28u, 40u, 48u, 404u, 412u, 408u, 416u}) {
    Store(base, model + offset, 0);
  }
  const auto r3 = XprRelease(base, model + kModelXpr);
  Require(r3.has_value(), "XPR::Release");
  return *r3;
}

}  // namespace

bool ModelConstruct(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_825948C0 Model::Model");
  if (!CanAllocatePointers(base, kTextureProviderSlots)) {
    return false;
  }
  Store(base, model, kModelVtable);
  XprConstruct(base, model + kModelXpr);
  Require(TextureProviderConstruct(base, model + kModelTextures, -1).has_value(),
          "TextureProvider::TextureProvider");
  model_animation::SkeletonConstruct(base, model + kModelSkeleton);
  for (uint32_t offset : kTrackSets) {
    model_animation::TrackSetConstruct(base, model + offset);
  }
  for (uint32_t offset : {16u, 20u, 28u, 40u, 24u, 48u, 404u, 412u, 408u, 416u, 420u}) {
    Store(base, model + offset, 0);
  }
  return true;
}

std::optional<uint32_t> ModelRelease(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_82594970 Model::Release");
  if (!PendingFreesHaveRoom(base, ReleaseNodes(base, model))) {
    return std::nullopt;
  }
  return Release(base, model);
}

std::optional<uint32_t> ModelDestruct(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_82594F58 Model::~Model");
  // Everything the members' plain C++ would leave to the original, before
  // anything changes. XPR::~XPR frees nothing after the release (its words
  // are 0 then), but checks for its two nodes: counted in ReleaseNodes.
  const uint32_t storage = Load(base, model + kModelTextures + kTextureProviderVector +
                                          kVectorFirst);
  if (!PendingFreesHaveRoom(base, ReleaseNodes(base, model)) ||
      !model_animation::TrackSetReleasable(base, model + 384, true) ||
      !model_animation::TrackSetReleasable(base, model + 364, true) ||
      !model_animation::TrackSetReleasable(base, model + 344, false) ||
      !model_animation::TrackSetReleasable(base, model + 324, false) ||
      !model_animation::TrackSetReleasable(base, model + 304, true) ||
      !model_animation::SkeletonDestructible(base, model + kModelSkeleton) ||
      !(storage == 0 || memory::IsHeapBlock(storage))) {
    return std::nullopt;
  }
  Store(base, model, kModelVtable);
  Release(base, model);
  Require(model_animation::TrackSetReleaseArray(base, model + 384), "TrackSet::ReleaseArray");
  Require(model_animation::TrackSetReleaseArray(base, model + 364), "TrackSet::ReleaseArray");
  Require(model_animation::TrackSetRelease(base, model + 344), "TrackSet::Release");
  Require(model_animation::TrackSetRelease(base, model + 324), "TrackSet::Release");
  Require(model_animation::TrackSetReleaseArray(base, model + 304), "TrackSet::ReleaseArray");
  Require(model_animation::SkeletonDestruct(base, model + kModelSkeleton),
          "Skeleton::~Skeleton");
  Require(TextureProviderDestruct(base, model + kModelTextures).has_value(),
          "TextureProvider::~TextureProvider");
  const auto r3 = XprDestruct(base, model + kModelXpr);
  Require(r3.has_value(), "XPR::~XPR");
  return *r3;
}

uint32_t MaterialCount(uint8_t* base, uint32_t model, uint32_t shape, int32_t group) {
  ITC_NATIVE_RUNNING("sub_82594AD8 Model::MaterialCount");
  const uint32_t block = Load(base, shape * 544 + Load(base, model + 40) + 396);
  return Load(base, block + (group == 0 ? 8 : 12));
}

uint32_t Material(uint8_t* base, uint32_t model, uint32_t shape, uint32_t index, int32_t group) {
  ITC_NATIVE_RUNNING("sub_82594B20 Model::Material");
  const uint32_t block = Load(base, shape * 544 + Load(base, model + 40) + 396);
  if (group == 0) {
    return (index << 9) + block + 16;
  }
  return ((Load(base, block + 8) + index) << 9) + block + 16;
}

uint32_t SharedVertexShader(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_825896D8 SharedVertexShader");
  const uint32_t table = Load(base, 0x8296B7B4);
  return table == 0 ? 0 : Load(base, (pair << 2) + table);
}

uint32_t SharedPixelShader(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82589700 SharedPixelShader");
  const uint32_t table = Load(base, 0x8296B7B8);
  return table == 0 ? 0 : Load(base, (pair << 2) + table);
}

std::optional<uint32_t> VertexDeclaration(uint8_t* base, uint32_t a, int32_t b, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82589728 VertexDeclaration");
  constexpr uint32_t kKinds = 0x8265EC00;
  constexpr uint32_t kTable = 0x8296B7F4;
  const uint32_t index = Load(base, (a << 2) + kKinds) << 2;
  const int32_t kind = int32_t(Load(base, (c << 2) + kKinds + 72));
  int32_t offset;
  if (b == 9) {
    offset = -28;
  } else if (b == 10) {
    offset = -56;
  } else if (b == 5) {
    static constexpr int32_t kSmall[3] = {224, 196, 168};
    static constexpr int32_t kLarge[3] = {140, 112, 84};
    if (kind < 0 || kind > 2) {
      return std::nullopt;
    }
    offset = int32_t(a) <= 4 ? kSmall[kind] : kLarge[kind];
  } else if (b == 11) {
    static constexpr int32_t kOffsets[3] = {56, 28, 0};
    if (kind < 0 || kind > 2) {
      return std::nullopt;
    }
    offset = kOffsets[kind];
  } else {
    return std::nullopt;
  }
  return Load(base, index + kTable + uint32_t(offset));
}

}  // namespace models
