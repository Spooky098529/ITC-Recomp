// The shader pairs' loads at boot (model.h, docs/functions/models.md): each
// pair's two files placed in one buffer and loaded through the archive.

#include "models/model.h"

#include "archive/archive.h"
#include "base/log.h"
#include "guest.h"
#include "schema/models.h"

namespace models {

using G = schema::ModelsGlobals;

std::optional<int64_t> ShadersLoadPair(uint8_t* base, uint32_t asset, uint32_t pair,
                                       const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820A07F0 Shaders::LoadPair");
  const uint32_t vertex_size = archive::AssetSize(base, asset);
  const uint32_t pixel_size = archive::AssetSize(base, asset + 1);
  // The original stores the cursor before its operator new; nothing reads it
  // in between, so the allocation goes first here.
  // A host object (E5 step 5): freed by Shaders::OnPairLoaded's operator
  // delete.
  const schema::ShaderPairLoad pair_load = schema::ShaderPairLoad::New(base);
  const uint32_t cursor = G::shader_cursor(base);
  const uint32_t buffer = G::load_buffer(base);
  const uint32_t vertex_file = buffer + cursor;
  const uint32_t pixel_file = buffer + cursor + vertex_size;
  G::set_shader_cursor(base, cursor + vertex_size + pixel_size);
  pair_load.set_vertex_file(vertex_file);
  pair_load.set_pixel_file(pixel_file);
  pair_load.set_pair(pair);
  load(asset, vertex_file, 0, 0);
  return load(asset + 1, pixel_file, kPairLoaded, pair_load.address);
}

uint32_t ShadersLoadAll(uint8_t* base,
                        const std::function<void(uint32_t asset, uint32_t pair)>& pair) {
  ITC_NATIVE_RUNNING("sub_820A08A0 Shaders::LoadAll");
  G::set_shader_cursor(base, 0u);
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
  SetSharedShaders(base, G::shared_vertex_table_address(), G::shared_pixel_table_address());
  return G::shared_vertex_table_address();
}

}  // namespace models
