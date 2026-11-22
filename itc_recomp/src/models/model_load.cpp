// The model's load (see model_load.h), load for load and store for store as
// the recompiled functions.

#include "models/model_load.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/xpr.h"
#include "native/buffers.h"
#include "schema/models.h"
#include "scenery_lists/scenery_lists.h"
#include "shader_objects/shader_objects.h"
#include "strings_and_font/steps.h"

#include <cstdlib>
#include <cstring>
#include <string>

namespace models {

namespace {

using namespace car_physics::ops;
namespace ma = model_animation;
using G = schema::ModelsGlobals;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kVersion = 0x00150000;
constexpr uint32_t kShapeBytes = schema::ModelShape::kSize;
constexpr uint32_t kMaterialBytes = 512;
// The bits the parse sets in each buffer header's +28 (fetch constant, 1 in
// bits 28-29).
constexpr uint32_t kFetchBits = 0x30000000;
constexpr uint32_t kFetchValue = 0x10000000;
constexpr uint32_t kTextureProviderVtable = 0x8206CCB4;
constexpr uint32_t kTextureProviderSet = 0x82594718;

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Model::Load: {} refused after its checks", what);
    std::abort();
  }
}

std::string Message(uint8_t* base, uint32_t text) {
  const char* host = reinterpret_cast<const char*>(guest::Pointer(base, text));
  std::string message(host, strnlen(host, 256));
  while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) {
    message.pop_back();
  }
  return message;
}

uint32_t WithFetchBits(uint32_t word) { return (word & ~kFetchBits) | kFetchValue; }

}  // namespace

bool LoadFits(uint8_t* base, uint32_t file) {
  if (!native::Buffers_NativeMemcpy()) {
    return false;
  }
  const uint32_t section = schema::PartFile(base, file).shader_section() + file;
  const uint32_t count = schema::ShaderSection(base, section).pair_count();
  uint32_t pair = schema::ShaderSection(base, section).first_entry_address();
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t vertex = pair + schema::ShaderPairEntry::kSize;
    const uint32_t pixel = schema::ShaderPairEntry(base, pair).vertex_size() + vertex;
    if (!c_runtime::XMemCpyBytes(base, vertex, schema::ShaderFile(base, vertex).header_size()) ||
        !c_runtime::XMemCpyBytes(base, pixel, schema::ShaderFile(base, pixel).header_size())) {
      return false;
    }
    pair = schema::ShaderPairEntry(base, pair).pixel_size() + pixel;
  }
  return true;
}

void MeshParse(uint8_t* base, uint32_t model, uint32_t file, const LoadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_825963F0 Mesh::Parse");
  const schema::Model m(base, model);
  const schema::PartFile f(base, file);
  const uint32_t version = f.version();
  if (version != kVersion) {
    // The game's debug print (sub_825ABFE8, its format at 0x8206CDE8).
    ITC_LOG_WARN("Mesh::Parse: version {:#x}, not {:#x} (the game's debug print)", version,
                 kVersion);
  }
  shader_objects::CreateCalls shader_calls;
  shader_calls.init = calls.init;
  shader_calls.user = calls.user;
  Require(shader_objects::LoadShaders(base, model, f.shader_section() + file, shader_calls),
          "Mesh::LoadShaders");
  // The header block: the file's bytes before the vertex data.
  m.set_header_block(AllocVirtual(base, f.vertex_data_offset()));
  Require(native::Buffers_Memcpy(base, m.header_block().address, file, f.vertex_data_offset(),
                                 0x82596448),
          "memcpy");
  const uint32_t header = m.header_block().address;
  const schema::PartFile h(base, header);
  m.set_header(header);
  m.set_table_14(h.table_14() + header);
  m.set_table_18(h.table_18() + header);
  m.set_shapes(h.shapes() + header);
  m.set_table_20(h.table_20() + header);
  m.set_file_04(h.file_04());
  m.set_file_08(h.file_08());
  const uint32_t shapes = h.shape_count();
  m.set_shape_count(shapes);
  m.set_file_10(h.file_10());
  // The shapes' pointers made pointers into the header block.
  if (int32_t(shapes) > 0) {
    for (int32_t i = 0; i < int32_t(m.shape_count()); i++) {
      const uint32_t offset = kShapeBytes * uint32_t(i);
      auto shape_at = [&] { return schema::ModelShape(base, m.shapes().address + offset); };
      {
        const auto shape = shape_at();
        shape.set_materials(shape.materials().address + m.header_block().address);
      }
      {
        const auto shape = shape_at();
        shape.set_skin(shape.skin().address + m.header_block().address);
      }
      {
        const auto shape = shape_at();
        shape.set_skin_array_a(shape.skin_array_a() + m.header_block().address);
      }
      {
        const auto shape = shape_at();
        shape.set_skin_array_b(shape.skin_array_b() + m.header_block().address);
      }
      const auto shape = shape_at();
      const schema::SkinBlock weights = shape.skin();
      const uint32_t w8 = weights.influence_counts().address;
      const uint32_t w12 = weights.weights().address;
      const uint32_t w16 = weights.bone_indices();
      weights.set_influence_counts(m.header_block().address + w8);
      weights.set_weights(m.header_block().address + w12);
      weights.set_bone_indices(m.header_block().address + w16);
      const uint32_t f316 = shape.vertex_buffer0_fetch();
      const uint32_t f348 = shape.vertex_buffer1_fetch();
      const uint32_t f380 = shape.vertex_buffer2_fetch();
      const uint32_t skinned = shape.skinned();
      shape.set_vertex_buffer0_fetch(WithFetchBits(f316));
      shape.set_vertex_buffer1_fetch(WithFetchBits(f348));
      shape.set_vertex_buffer2_fetch(WithFetchBits(f380));
      if (skinned == 1) {
        shape.set_vertex_buffer1_fetch(WithFetchBits(shape.vertex_buffer1_fetch()));
        shape.set_vertex_buffer2_fetch(WithFetchBits(shape.vertex_buffer2_fetch()));
      }
    }
  }
  // The materials' pointers (each record's +384, a material record's:
  // material-passes.md's type).
  if (int32_t(m.shape_count()) > 0) {
    for (int32_t i = 0; i < int32_t(m.shape_count()); i++) {
      const uint32_t offset = kShapeBytes * uint32_t(i);
      auto shape_at = [&] { return schema::ModelShape(base, m.shapes().address + offset); };
      const schema::MaterialBlock materials = shape_at().materials();
      if (int32_t(materials.record_count()) <= 0) {
        continue;
      }
      for (int32_t j = 0;;) {
        const uint32_t entry =
            shape_at().materials().records_address() + kMaterialBytes * uint32_t(j);
        j++;
        Store(base, entry + 384, m.header_block().address + Load(base, entry + 384));
        const schema::MaterialBlock again = shape_at().materials();
        if (!(j < int32_t(again.record_count()))) {
          break;
        }
      }
    }
  }
  // The skeleton and the track sets, with the header block's sections.
  auto call = [&](uint32_t function, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t lr) {
    calls.call(calls.user, function, r3, r4, r5, lr);
  };
  if (m.header().has_sections() != 0) {
    if (f.node_file() != 0) {
      const uint32_t at = f.node_file() + m.header_block().address;
      if (!ma::SkeletonBind(base, m.skeleton_address(), at, 0)) {
        call(0x82599A20, m.skeleton_address(), at, 0, 0x82596674);
      }
    }
    if (f.node_values_section() != 0) {
      ma::SkeletonLoadNodeValues(base, m.skeleton_address(),
                                 m.header_block().address + f.node_values_section());
    }
    if (f.float_section_a() != 0) {
      const uint32_t at = f.float_section_a() + m.header_block().address;
      if (!ma::TrackSetSetupFloat(base, m.track_set_float_a_address(), at)) {
        call(0x825A1BE8, m.track_set_float_a_address(), at, 0, 0x825966AC);
      }
    }
    if (f.float_section_b() != 0) {
      const uint32_t at = m.header_block().address + f.float_section_b();
      if (!ma::TrackSetSetupFloat(base, m.track_set_float_b_address(), at)) {
        call(0x825A1BE8, m.track_set_float_b_address(), at, 0, 0x825966C8);
      }
    }
    if (f.material_section() != 0) {
      const uint32_t at = m.header_block().address + f.material_section();
      if (!ma::TrackSetSetup32(base, m.track_set_material_address(), at)) {
        call(0x825A1218, m.track_set_material_address(), at, 0, 0x825966E4);
      }
    }
    if (f.section_208() != 0) {
      const uint32_t at = m.header_block().address + f.section_208();
      if (!ma::TrackSetSetup208(base, m.track_set_208_address(), at)) {
        call(0x825A1A38, m.track_set_208_address(), at, 0, 0x82596700);
      }
    }
  }
  {
    const uint32_t at = f.float4_section() + m.header_block().address;
    if (!ma::TrackSetSetupFloat4(base, m.track_set_float4_address(), at)) {
      call(0x825A1E08, m.track_set_float4_address(), at, 0, 0x82596714);
    }
  }
  if (m.shape_count() == 0) {
    return;
  }
  // The vertex data in physical memory, the shapes' buffers relocated to it.
  const uint32_t vertices = file + f.vertex_data_offset();
  const uint32_t data = AllocPhysicalEx(base, f.vertex_data_size(), 0, 0xFFFFFFFFu, 4, 0x404);
  m.set_vertex_data(data);
  if (data == 0) {
    ITC_LOG_WARN("Mesh::Parse: the game's debug message: {}", Message(base, 0x8206CDC8));
  }
  Require(native::Buffers_Memcpy(base, m.vertex_data(), vertices, f.vertex_data_size(),
                                 0x8259676C),
          "memcpy");
  if (int32_t(m.shape_count()) <= 0) {
    return;
  }
  for (int32_t i = 0; i < int32_t(m.shape_count()); i++) {
    const uint32_t offset = kShapeBytes * uint32_t(i);
    auto shape = [&] { return schema::ModelShape(base, m.shapes().address + offset); };
    shape().set_stream0_offset(shape().stream0_offset() - f.vertex_data_offset());
    shape().set_stream0_offset(shape().stream0_offset() + m.vertex_data());
    RelocateResource(base, shape().vertex_buffer0_address(), m.vertex_data());
    if (shape().skinned() != 1) {
      shape().set_stream1_offset(shape().stream1_offset() - f.vertex_data_offset());
      shape().set_stream1_offset(shape().stream1_offset() + m.vertex_data());
      RelocateResource(base, shape().vertex_buffer1_address(), m.vertex_data());
      shape().set_stream2_offset(shape().stream2_offset() - f.vertex_data_offset());
      shape().set_stream2_offset(shape().stream2_offset() + m.vertex_data());
      RelocateResource(base, shape().vertex_buffer2_address(), m.vertex_data());
    }
    if (int32_t(shape().materials().record_count()) > 0) {
      for (int32_t j = 0; j < int32_t(shape().materials().record_count()); j++) {
        // A material record's index buffer header (its +0x160).
        RelocateResource(base,
                         shape().materials().records_address() + kMaterialBytes * uint32_t(j) +
                             352,
                         m.vertex_data());
      }
    }
  }
}

bool ModelLoad(uint8_t* base, uint32_t model, uint32_t file, uint32_t xpr,
               const LoadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82596920 Model::Load");
  if (!LoadFits(base, file) || !ModelRelease(base, model)) {
    return false;
  }
  MeshParse(base, model, file, calls);
  const schema::Model m(base, model);
  if (xpr != 0) {
    Require(XprCreate(base, m.xpr_address(), xpr).has_value(), "XPR::Create");
    const uint32_t provider = m.textures_address();
    const uint32_t set = schema::TextureProvider(base, provider).vtable().set();
    if (set != kTextureProviderSet ||
        !scenery_lists::TextureProviderSet(base, provider, 0, m.xpr_address())) {
      calls.call(calls.user, set, provider, 0, m.xpr_address(), 0x82596980);
    }
    m.set_texture_provider(provider);
  }
  if (m.shape_count() == 0) {
    return true;
  }
  const double one = Lfs(base, kOne);
  const double scale = Lfs(base, G::weight_scale_address());
  const double add = Lfs(base, G::weight_add_address());
  const double multiply = Lfs(base, G::weight_multiply_address());
  const double zero = Lfs(base, kZero);
  // A weight quantized: fmadds, fctidz, its low word reloaded (lwz, so
  // unsigned) and converted back (fcfid, frsp), then scaled.
  auto quantize = [&](double weight) {
    const double fused = Fmadds(weight, multiply, add);
    const uint32_t low = uint32_t(strings_and_font::steps::Fctidz(fused));
    return Fmuls(double(float(double(int64_t(low)))), scale);
  };
  for (uint32_t i = 0; i < m.shape_count(); i++) {
    const schema::ModelShape shape(base, m.shapes().address + kShapeBytes * i);
    if (shape.skinned() != 1) {
      continue;
    }
    const schema::SkinBlock weights = shape.skin();
    uint32_t counts = weights.influence_counts().address;
    uint32_t at = weights.weights().address;
    if (weights.vertex_count() == 0) {
      continue;
    }
    for (uint32_t v = 0; v < weights.vertex_count(); v++) {
      double sum0 = zero, sum1 = zero, sum2 = zero;
      if (schema::SkinInfluenceCount(base, counts).count() - 1 != 0) {
        uint32_t k = 0;
        do {
          const schema::SkinWeight w(base, at);
          const double a = Lfs(base, w.x_address());
          const double c = Lfs(base, w.z_address());
          const double b = Lfs(base, w.y_address());
          const double q0 = quantize(a);
          Stfs(base, w.x_address(), q0);
          sum0 = Fadds(q0, sum0);
          const double q1 = quantize(b);
          Stfs(base, w.y_address(), q1);
          sum1 = Fadds(q1, sum1);
          const double q2 = quantize(c);
          Stfs(base, w.z_address(), q2);
          at += schema::SkinWeight::kSize;
          k++;
          sum2 = Fadds(q2, sum2);
        } while (k < schema::SkinInfluenceCount(base, counts).count() - 1);
      }
      const schema::SkinWeight last(base, at);
      Stfs(base, last.x_address(), Fsubs(one, sum0));
      Stfs(base, last.y_address(), Fsubs(one, sum1));
      Stfs(base, last.z_address(), Fsubs(one, sum2));
      at += schema::SkinWeight::kSize;
      counts += schema::SkinInfluenceCount::kSize;
    }
  }
  return true;
}

}  // namespace models
