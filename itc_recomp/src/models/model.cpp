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
#include "schema/models.h"

#include <cstdlib>

namespace models {

namespace {

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}

using G = schema::ModelsGlobals;

// A shader pair's word `word` (0 to 3: the vertex and pixel shaders, their
// microcode).
uint32_t PairWord(const schema::ModelShaderPair& pair, uint32_t word) {
  switch (word) {
    case 0:
      return pair.vertex_shader();
    case 1:
      return pair.pixel_shader();
    case 2:
      return pair.vertex_microcode();
    default:
      return pair.pixel_microcode();
  }
}

// A callee whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Model: {} refused after its checks", what);
    std::abort();
  }
}

// The blocks Model::Release puts off, at most (each one not null; those
// without their header's tag are skipped), and XPR::Release's two.
uint32_t ReleaseNodes(uint32_t model) {
  const schema::Model m(model);
  uint32_t nodes = 2;
  nodes += m.header_block().address != 0;
  nodes += m.vertex_data() != 0;
  nodes += m.shader_pairs().address != 0;
  const uint32_t pairs = m.shader_pairs().address;
  const uint32_t count = m.shader_pair_count();
  for (uint32_t i = 0; i < count && nodes < 0x10000000u; i++) {
    const schema::ModelShaderPair pair(pairs + schema::ModelShaderPair::kSize * i);
    for (uint32_t word = 0; word < 4; word++) {
      nodes += PairWord(pair, word) != 0;
    }
  }
  return nodes;
}

// FreeLater on a block when it isn't null, its checks made.
void FreeIfAny(uint32_t block) {
  if (block != 0) {
    Require(FreeLater(block), "Memory::FreeLater");
  }
}

// Model::Release's work, its checks made.
uint32_t Release(uint32_t model) {
  const schema::Model m(model);
  FreeIfAny(m.header_block().address);
  FreeIfAny(m.vertex_data());
  for (uint32_t i = 0; i < m.shader_pair_count(); i++) {
    const schema::ModelShaderPair pair(m.shader_pairs().address + schema::ModelShaderPair::kSize * i);
    for (uint32_t word = 0; word < 4; word++) {
      FreeIfAny(PairWord(pair, word));
    }
  }
  FreeIfAny(m.shader_pairs().address);
  m.set_header_block(0u);
  m.set_vertex_data(0u);
  m.set_shader_pairs(0u);
  m.set_shader_pair_count(0u);
  m.set_header(0u);
  m.set_shapes(0u);
  m.set_texture_provider(0u);
  m.set_file_04(0u);
  m.set_shape_count(0u);
  m.set_file_08(0u);
  m.set_file_10(0u);
  const auto r3 = XprRelease(m.xpr_address());
  Require(r3.has_value(), "XPR::Release");
  return *r3;
}

}  // namespace

bool ModelConstruct(uint32_t model) {
  ITC_NATIVE_RUNNING("sub_825948C0 Model::Model");
  if (!CanAllocatePointers(kTextureProviderSlots)) {
    return false;
  }
  const schema::Model m(model);
  m.set_vtable(kModelVtable);
  XprConstruct(m.xpr_address());
  Require(TextureProviderConstruct(m.textures_address(), -1).has_value(),
          "TextureProvider::TextureProvider");
  model_animation::SkeletonConstruct(m.skeleton_address());
  // The five track sets, in the constructor's order.
  for (uint32_t set : {m.track_set_float4_address(), m.track_set_float_a_address(),
                       m.track_set_float_b_address(), m.track_set_material_address(),
                       m.track_set_208_address()}) {
    model_animation::TrackSetConstruct(set);
  }
  m.set_header_block(0u);
  m.set_vertex_data(0u);
  m.set_header(0u);
  m.set_shapes(0u);
  m.set_shader_pairs(0u);
  m.set_texture_provider(0u);
  m.set_file_04(0u);
  m.set_shape_count(0u);
  m.set_file_08(0u);
  m.set_file_10(0u);
  m.set_shader_pair_count(0u);
  return true;
}

std::optional<uint32_t> ModelRelease(uint32_t model) {
  ITC_NATIVE_RUNNING("sub_82594970 Model::Release");
  if (!PendingFreesHaveRoom(ReleaseNodes(model))) {
    return std::nullopt;
  }
  return Release(model);
}

uint32_t ModelReleaseNodes(uint32_t model) {
  return ReleaseNodes(model);
}

bool ModelMembersDestructible(uint32_t model) {
  const schema::Model m(model);
  const uint32_t storage =
      schema::TextureProvider(m.textures_address()).first().address;
  return model_animation::TrackSetReleasable(m.track_set_208_address(), true) &&
         model_animation::TrackSetReleasable(m.track_set_material_address(), true) &&
         model_animation::TrackSetReleasable(m.track_set_float_b_address(), false) &&
         model_animation::TrackSetReleasable(m.track_set_float_a_address(), false) &&
         model_animation::TrackSetReleasable(m.track_set_float4_address(), true) &&
         model_animation::SkeletonDestructible(m.skeleton_address()) &&
         (storage == 0 || memory::IsHeapBlock(storage));
}

std::optional<uint32_t> ModelDestruct(uint32_t model) {
  ITC_NATIVE_RUNNING("sub_82594F58 Model::~Model");
  // Everything the members' plain C++ would leave to the original, before
  // anything changes. XPR::~XPR frees nothing after the release (its words
  // are 0 then), but checks for its two nodes: counted in ReleaseNodes.
  if (!PendingFreesHaveRoom(ReleaseNodes(model)) ||
      !ModelMembersDestructible(model)) {
    return std::nullopt;
  }
  const schema::Model m(model);
  m.set_vtable(kModelVtable);
  Release(model);
  Require(model_animation::TrackSetReleaseArray(m.track_set_208_address()),
          "TrackSet::ReleaseArray");
  Require(model_animation::TrackSetReleaseArray(m.track_set_material_address()),
          "TrackSet::ReleaseArray");
  Require(model_animation::TrackSetRelease(m.track_set_float_b_address()),
          "TrackSet::Release");
  Require(model_animation::TrackSetRelease(m.track_set_float_a_address()),
          "TrackSet::Release");
  Require(model_animation::TrackSetReleaseArray(m.track_set_float4_address()),
          "TrackSet::ReleaseArray");
  Require(model_animation::SkeletonDestruct(m.skeleton_address()),
          "Skeleton::~Skeleton");
  Require(TextureProviderDestruct(m.textures_address()).has_value(),
          "TextureProvider::~TextureProvider");
  const auto r3 = XprDestruct(m.xpr_address());
  Require(r3.has_value(), "XPR::~XPR");
  return *r3;
}

uint32_t MaterialCount(uint32_t model, uint32_t shape, int32_t group) {
  ITC_NATIVE_RUNNING("sub_82594AD8 Model::MaterialCount");
  const schema::MaterialBlock block =
      schema::ModelShape(shape * schema::ModelShape::kSize +
                                   schema::Model(model).shapes().address)
          .materials();
  return group == 0 ? block.group0_count() : block.group1_count();
}

uint32_t Material(uint32_t model, uint32_t shape, uint32_t index, int32_t group) {
  ITC_NATIVE_RUNNING("sub_82594B20 Model::Material");
  const schema::MaterialBlock block =
      schema::ModelShape(shape * schema::ModelShape::kSize +
                                   schema::Model(model).shapes().address)
          .materials();
  if (group == 0) {
    return (index << 9) + block.records_address();
  }
  return ((block.group0_count() + index) << 9) + block.records_address();
}

uint32_t SharedVertexShader(uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_825896D8 SharedVertexShader");
  const uint32_t table = G::shared_vertex_shaders();
  return table == 0 ? 0 : Load((pair << 2) + table);
}

void SetSharedShaders(uint32_t vertex, uint32_t pixel) {
  ITC_NATIVE_RUNNING("sub_825896C0 SetSharedShaders");
  G::set_shared_vertex_shaders(vertex);
  G::set_shared_pixel_shaders(pixel);
}

uint32_t SharedPixelShader(uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82589700 SharedPixelShader");
  const uint32_t table = G::shared_pixel_shaders();
  return table == 0 ? 0 : Load((pair << 2) + table);
}

std::optional<uint32_t> VertexDeclaration(uint32_t a, int32_t b, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82589728 VertexDeclaration");
  constexpr uint32_t kKinds = G::stream_kinds_address();
  constexpr uint32_t kTable = G::vertex_declarations_address();
  const uint32_t index = Load((a << 2) + kKinds) << 2;
  const int32_t kind = int32_t(Load((c << 2) + kKinds + 72));
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
  return Load(index + kTable + uint32_t(offset));
}

}  // namespace models
