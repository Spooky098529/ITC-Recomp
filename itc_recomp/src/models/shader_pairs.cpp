// The shader pairs' loads at boot (model.h, docs/functions/models.md): each
// pair's two files placed in one buffer and loaded through the archive.

#include "models/model.h"

#include "archive/archive.h"
#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

namespace models {

std::optional<int64_t> ShadersLoadPair(uint8_t* base, uint32_t asset, uint32_t pair,
                                       const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820A07F0 Shaders::LoadPair");
  const uint32_t vertex_size = archive::AssetSize(base, asset);
  const uint32_t pixel_size = archive::AssetSize(base, asset + 1);
  // The original stores the cursor before its operator new; nothing reads it
  // in between, so the allocation goes first here, for the fallback.
  const auto context = memory::crt::OperatorNew(base, 12);
  if (!context) {
    return std::nullopt;
  }
  const uint32_t cursor = guest::LoadU32(base, kShaderCursor);
  const uint32_t buffer = guest::LoadU32(base, kShaderBuffer);
  const uint32_t vertex_file = buffer + cursor;
  const uint32_t pixel_file = buffer + cursor + vertex_size;
  guest::StoreU32(base, kShaderCursor, cursor + vertex_size + pixel_size);
  guest::StoreU32(base, *context + 0, vertex_file);
  guest::StoreU32(base, *context + 4, pixel_file);
  guest::StoreU32(base, *context + 8, pair);
  load(asset, vertex_file, 0, 0);
  return load(asset + 1, pixel_file, kPairLoaded, *context);
}

uint32_t ShadersLoadAll(uint8_t* base,
                        const std::function<void(uint32_t asset, uint32_t pair)>& pair) {
  ITC_NATIVE_RUNNING("sub_820A08A0 Shaders::LoadAll");
  guest::StoreU32(base, kShaderCursor, 0);
  struct Run {
    uint32_t first;
    uint32_t last;
    uint32_t first_pair;
  };
  static constexpr Run kRuns[] = {{3, 69, 0}, {22801, 22811, 1000}, {24258, 24368, 2000}};
  for (const Run& run : kRuns) {
    for (uint32_t asset = run.first, p = run.first_pair; asset <= run.last; asset += 2, p++) {
      pair(asset, p);
    }
  }
  SetSharedShaders(base, kSharedVertexTable, kSharedPixelTable);
  return kSharedVertexTable;
}

}  // namespace models
