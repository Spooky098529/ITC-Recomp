// The first set of pools' objects after a harness case (ScenePools::CreateA
// and its callers): the texture providers each pool's object holds, a
// provider's storage taken (compared, then freed), and memory read with
// the words pointing into the case's own pages made offsets. Shared by the
// scene_loaders and car_models harnesses.

#pragma once

#include "harness.h"

#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/model.h"
#include "models/texture_provider.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/objects.h"
#include "scene_loaders/pools.h"

#include <vector>

namespace harness::scene_pools_take {

namespace sl = scene_loaders;

// After a case: a provider's vector (at provider + 4: +4 first, +8 last, +12
// end) as whether its storage is a heap block, its size and capacity and
// whether its slots are all 0; then its words cleared and the storage freed.
inline void TakeProvider(uint8_t* base, uint32_t provider, Outputs& out) {
  const uint32_t vector = provider + models::kTextureProviderVector;
  const uint32_t first = guest::LoadU32(base, vector + models::kVectorFirst);
  const uint32_t last = guest::LoadU32(base, vector + models::kVectorLast);
  const uint32_t end = guest::LoadU32(base, vector + models::kVectorEnd);
  const bool heap = first != 0 && memory::IsHeapBlock(first);
  out.Word(heap);
  out.Word(last - first);
  out.Word(end - first);
  bool zeros = true;
  for (uint32_t p = first; heap && p != last && last - first <= 4096; p += 4) {
    zeros = zeros && guest::LoadU32(base, p) == 0;
  }
  out.Word(zeros);
  for (uint32_t offset : {models::kVectorFirst, models::kVectorLast, models::kVectorEnd}) {
    guest::StoreU32(base, vector + offset, 0);
  }
  if (heap) {
    memory::crt::OperatorDelete(first);
  }
}

// The texture providers' offsets in a first-set object of `pool`.
inline std::vector<uint32_t> PoolProviders(uint32_t pool) {
  std::vector<uint32_t> at;
  auto models_from = [&](uint32_t first, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
      at.push_back(first + sl::kPartModelBytes * i + models::kModelTextures);
    }
  };
  switch (pool) {
    case 0:
      for (uint32_t offset : sl::kCarModelMembers.providers) at.push_back(sl::kCarModelLoads + offset);
      break;
    case 1:
      models_from(sl::kPartsModels, sl::kPartsCount);
      break;
    case 2:
      for (uint32_t offset : sl::kCarModelMembers.providers) at.push_back(sl::kCockpitLoads + offset);
      models_from(sl::kCockpitLoads + sl::kCockpitLoadsParts, 6);
      break;
    case 3:
      models_from(304, 6);
      break;
    case 4:
      models_from(112, 1);
      break;
    case 5:
      for (uint32_t i = 0; i < 2; i++) at.push_back(sl::kWheelsLoads + sl::kWheelsProviders + 20 * i);
      for (uint32_t offset : {sl::kWheelsTires, sl::kWheelsTires + 464, sl::kWheelsRims,
                              sl::kWheelsRims + 464, sl::kWheelsCalipers,
                              sl::kWheelsCalipers + 464}) {
        at.push_back(sl::kWheelsLoads + offset + models::kModelTextures);
      }
      break;
    default:
      for (uint32_t offset : sl::kTrafficMembers.providers) at.push_back(sl::kTrafficLoads + offset);
      models_from(sl::kTrafficLoads + sl::kTrafficLoadsParts, 6);
      break;
  }
  return at;
}

struct PageRegion {
  uint32_t start, bytes;
};

// `bytes` from `start`, the words pointing into a region (or 16 past it)
// replaced by a marker of the region and the offset.
inline std::vector<uint8_t> Normalized(uint8_t* base, const std::vector<PageRegion>& regions,
                                uint32_t start, uint32_t bytes) {
  std::vector<uint8_t> data = Read(base, start, bytes);
  for (uint32_t at = 0; at + 4 <= bytes; at += 4) {
    const uint32_t word = guest::LoadU32(base, start + at);
    for (size_t k = 0; k < regions.size(); k++) {
      if (word >= regions[k].start && word - regions[k].start < regions[k].bytes + 16) {
        const uint32_t marker = 0xB0000000u + (uint32_t(k) << 20) + (word - regions[k].start);
        for (int b = 0; b < 4; b++) data[at + b] = uint8_t(marker >> (24 - 8 * b));
        break;
      }
    }
  }
  return data;
}

}  // namespace harness::scene_pools_take
