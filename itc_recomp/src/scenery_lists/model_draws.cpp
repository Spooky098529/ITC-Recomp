// The model draws' recorders and the visibility test as plain C++
// (docs/functions/scenery-lists.md); see scenery_lists.h. Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt): the vector
// unit's work as the translation does it (SSE vectors with word 0 in the top
// lane, flush-to-zero on), the scalar work as its doubles rounded to single.

#include "scenery_lists/scenery_lists.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "c_runtime/strings.h"
#include "car_physics/car_physics.h"
#include "matrix_math/matrix_math.h"
#include "models/model.h"
#include "models/texture_provider.h"
#include "schema/command_list.h"
#include "schema/device_shaders.h"
#include "schema/material_passes.h"
#include "schema/models.h"
#include "schema/scenery_lists.h"
#include "schema/sprites.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <algorithm>
#include <bit>
#include <tuple>
#include <cstdlib>

namespace scenery_lists {

namespace {

using Fpscr = ::base::FloatMode;
using G = schema::SceneryListsGlobals;
// Other areas' views the draws read (models.md, material-passes.md).
using ModelView = schema::Model;
using Shape = schema::ModelShape;
using Record = schema::MaterialRecord;

// A call of the texture provider's slot `function` (args: the provider, the
// slot, the index) from `frame` with `lr`: its plain C++ where it's the
// game's slots 5 to 7 (models::CallXprTexture, as its adapter runs it: its
// own guest calls from its 96-byte frame below `frame`, the back chain
// written first), else the guest call.
uint32_t ProviderSlotCall(const DrawCalls& calls, uint32_t function,
                          const uint32_t (&args)[3], uint32_t frame, uint32_t lr) {
  const auto direct = models::CallXprTexture(function, args[0], args[1], args[2], frame,
      [&](uint32_t target, uint32_t inner, uint32_t inner_lr, uint64_t r3,
          uint64_t r4) -> uint64_t {
        guest::StoreU32(inner, frame);
        const uint32_t inner_args[2] = {uint32_t(r3), uint32_t(r4)};
        return calls.call(calls.user, target, inner_args, 2, inner, inner_lr);
      });
  if (direct) {
    return uint32_t(*direct);
  }
  return calls.call(calls.user, function, args, 3, frame, lr);
}


// The matrix calls' return addresses in the originals (for the
// interpolation's notes).
constexpr uint32_t kVisibleMultiplyReturn = 0x8259522C;
constexpr uint32_t kObjectMultiplyReturn = 0x82595438;
constexpr uint32_t kObjectCopyReturn = 0x82595444;
constexpr uint32_t kLightsCopyReturn = 0x825954F4;

// Flush-to-zero on while alive (the vector unit's mode), put back after.
class FlushToZero {
 public:
  FlushToZero() : csr_(Fpscr::getcsr()) { Fpscr::setcsr(csr_ | uint32_t(Fpscr::FlushMask)); }
  ~FlushToZero() { Fpscr::setcsr(csr_); }

 private:
  uint32_t csr_;
};

// Vectors as the translated code holds them: word 0 in the top lane.
simde__m128 VectorAt(uint32_t address) {
  return simde_mm_castsi128_ps(simde_mm_set_epi32(
      int32_t(guest::LoadU32(address + 0)), int32_t(guest::LoadU32(address + 4)),
      int32_t(guest::LoadU32(address + 8)), int32_t(guest::LoadU32(address + 12))));
}
uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}
void StoreVector(uint32_t address, simde__m128 v) {
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(address + 4 * i, WordOf(v, i));
  }
}
simde__m128 Hi(simde__m128 a, simde__m128 b) {
  return simde_mm_castsi128_ps(
      simde_mm_unpackhi_epi32(simde_mm_castps_si128(a), simde_mm_castps_si128(b)));
}
simde__m128 Lo(simde__m128 a, simde__m128 b) {
  return simde_mm_castsi128_ps(
      simde_mm_unpacklo_epi32(simde_mm_castps_si128(a), simde_mm_castps_si128(b)));
}

float AsFloat(uint32_t bits) {
  return std::bit_cast<float>(bits);
}
double Lfs(uint32_t address) {
  return double(guest::LoadFloat(address));
}
// lfs then stfs.
void CopyFloat(uint32_t to, uint32_t from) {
  guest::StoreFloat(to, float(Lfs(from)));
}
void CopyWords(uint32_t to, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(to + 4 * i, guest::LoadU32(from + 4 * i));
  }
}

// The command list's checks before a recorder: its chunk open or none open
// on any list, and room for `slots`.
bool CanRecord(uint32_t list, uint32_t slots) {
  const schema::CommandList l(list);
  if (l.chunk_open() == 0 && schema::CommandListGlobals::open_chunks() != 0) {
    return false;
  }
  const uint32_t used = l.used();
  return used + slots >= used && used + slots < l.capacity();
}

// Command 36 and the chunk closed.
void EndRecord(uint32_t list) {
  const schema::ChunkCommand end(command_list::AllocateSlots(list, 1));
  end.set_arg0(0);
  end.set_id(36);
  end.set_arg1(0);
  command_list::CloseChunk(list);
}

// The normalize loop: `count` vectors from `from` to `out` (both at their
// 16-byte boundaries), x, y, z (and w) times the reciprocal square root of
// x² + y² + z², refined once; a zero length gives the dot product.
void Normalize(uint32_t out, uint32_t from, uint32_t count) {
  FlushToZero ftz;
  const simde__m128 half = simde_mm_mul_ps(
      simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)), simde_mm_castsi128_ps(simde_mm_set1_epi32(0x3F000000)));
  const simde__m128 zero = simde_mm_castsi128_ps(simde_mm_set1_epi32(0));
  for (uint32_t i = 0; i < count; i++) {
    const simde__m128 v = VectorAt((from + 16 * i) & ~0xFu);
    const simde__m128 dot = simde_mm_dp_ps(v, v, 0xEF);
    simde__m128 estimate = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(dot));
    const simde__m128 half_dot = simde_mm_mul_ps(dot, half);
    const simde__m128 is_zero = simde_mm_cmpeq_ps(dot, zero);
    const simde__m128 square = simde_mm_mul_ps(estimate, estimate);
    const simde__m128 step = simde_mm_xor_ps(
        simde_mm_sub_ps(simde_mm_mul_ps(half_dot, square), half),
        simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
    estimate = simde_mm_add_ps(simde_mm_mul_ps(estimate, step), estimate);
    const simde__m128 scaled = simde_mm_mul_ps(v, estimate);
    const simde__m128i result = simde_mm_or_si128(
        simde_mm_andnot_si128(simde_mm_castps_si128(is_zero), simde_mm_castps_si128(scaled)),
        simde_mm_and_si128(simde_mm_castps_si128(is_zero), simde_mm_castps_si128(dot)));
    StoreVector((out + 16 * i) & ~0xFu, simde_mm_castsi128_ps(result));
  }
}

}  // namespace

uint32_t IsShapeVisible(uint32_t model, uint32_t view_projection, uint32_t world,
                        uint32_t shape, uint32_t product) {
  ITC_NATIVE_RUNNING("sub_825951E0 Model::IsShapeVisible");
  const double one = Lfs(G::one_address());
  // The original's w stores before the multiply (overwritten by it).
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    guest::StoreFloat(product + offset, float(one));
  }
  matrix_math::Multiply(product, world, view_projection);
  interpolation::BlendState_OnMultiply(product, world, view_projection,
                                       kVisibleMultiplyReturn);
  // The product's columns, as the translation transposes it.
  simde__m128 v12 = VectorAt(product + 32);
  simde__m128 v9 = VectorAt(product + 16);
  const double left = Lfs(G::visible_left_address());
  const double right = Lfs(G::visible_right_address());
  const double minus_one = Lfs(G::minus_one_address());
  simde__m128 v11 = VectorAt(product + 0);
  const simde__m128 v0 = Hi(v12, v11);
  const double zero = Lfs(G::zero_address());
  v12 = Lo(v12, v11);
  const simde__m128 v10a = VectorAt(product + 48);
  v11 = Lo(v10a, v9);
  const simde__m128 v13 = Hi(v10a, v9);
  const simde__m128 c8 = Hi(v11, v12);
  const simde__m128 c10 = Hi(v13, v0);
  const simde__m128 c9 = Lo(v13, v0);
  const simde__m128 c7 = Lo(v11, v12);
  int32_t near = 0, across = 0, up = 0;
  // The shape's +96 on: its eight corners (not a ModelShape field yet).
  uint32_t corner = ModelView(model).shapes().address + shape * 544 + 96;
  for (int i = 0; i < 8; i++, corner += 16) {
    simde__m128 point;
    {
      FlushToZero ftz;
      const simde__m128 c = VectorAt(corner & ~0xFu);
      const simde__m128 d13 = simde_mm_dp_ps(c, c10, 0xFF);
      const simde__m128 d11 = simde_mm_dp_ps(c, c8, 0xFF);
      const simde__m128 d12 = simde_mm_dp_ps(c, c9, 0xFF);
      const simde__m128 d0 = simde_mm_dp_ps(c, c7, 0xFF);
      point = Hi(Hi(d0, d12), Hi(d11, d13));
    }
    const double w = double(AsFloat(WordOf(point, 3)));
    const double scale = double(float(one / w));
    double x = double(float(double(AsFloat(WordOf(point, 0))) * scale));
    double y = double(float(double(AsFloat(WordOf(point, 1))) * scale));
    const double z = double(float(double(AsFloat(WordOf(point, 2))) * scale));
    if (w < zero) {
      x = double(float(x * minus_one));
      y = double(float(y * minus_one));
      near++;
    } else if (z < zero) {
      near--;
    }
    if (x > right) {
      across++;
    } else if (x < left) {
      across--;
    }
    if (y > right) {
      up++;
    } else if (y < left) {
      up--;
    }
  }
  if (across == 8 || across == -8 || up == 8 || up == -8 || near == 8 || near == -8) {
    return 0;
  }
  return 1;
}

std::optional<uint32_t> RecordObject(uint32_t view_projection, uint32_t world) {
  ITC_NATIVE_RUNNING("sub_825953D0 Recorder::RecordObject");
  const uint32_t list = G::record_list();
  if (!CanRecord(list, 11)) {
    return std::nullopt;
  }
  command_list::OpenChunk(list);
  const uint32_t header = command_list::AllocateSlots(list, 1);
  schema::ChunkCommand(header).set_id(7);
  schema::ChunkCommand(header).set_arg0(9);
  schema::ChunkCommand(header).set_arg1(0x00010009);
  const uint32_t constants = command_list::AllocateSlots(list, 9);
  matrix_math::Multiply(constants, world, view_projection);
  interpolation::BlendState_OnMultiply(constants, world, view_projection,
                                       kObjectMultiplyReturn);
  matrix_math::Copy(constants + 64, world);
  interpolation::BlendState_OnCopy(constants + 64, world, kObjectCopyReturn);
  CopyWords(constants + 128, G::camera_position_address(), 4);
  EndRecord(list);
  return header;
}

std::optional<uint32_t> RecordSceneLights() {
  ITC_NATIVE_RUNNING("sub_82595490 Recorder::RecordSceneLights");
  const uint32_t list = G::record_list();
  if (!CanRecord(list, 65)) {
    return std::nullopt;
  }
  command_list::OpenChunk(list);
  const uint32_t header = command_list::AllocateSlots(list, 1);
  schema::ChunkCommand(header).set_id(7);
  schema::ChunkCommand(header).set_arg0(63);
  schema::ChunkCommand(header).set_arg1(0x0101003F);
  const uint32_t constants = command_list::AllocateSlots(list, 63);
  matrix_math::Copy(constants, G::view_address());
  interpolation::BlendState_OnCopy(constants, G::view_address(), kLightsCopyReturn);
  CopyWords(constants + 64, G::camera_position_address(), 4);
  SceneLightsRecordDirections(constants + 80);
  SceneLightsRecordProjectors(constants + 80 + 560);
  EndRecord(list);
  return header;
}

void SceneLightsRecordDirections(uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8258B980 SceneLights::RecordDirections");
  Normalize(out, G::light_directions_address(), 3);
  CopyWords(out + 48, G::headlight_eyes_address(), 32);
  CopyWords(out + 176, G::headlight_shapes_address(), 32);
  Normalize(out + 304, G::headlight_directions_address(), 8);
  CopyWords(out + 432, G::headlight_extras_address(), 32);
}

void SceneLightsRecordProjectors(uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82589B58 SceneLights::RecordProjectors");
  for (uint32_t i = 0; i < 3; i++) {
    CopyFloat(out + 4 * i, G::projector_origin_address() + 4 * i);
  }
  CopyFloat(out + 12, G::one_address());
  struct Run {
    uint32_t to, from, floats;
  };
  static constexpr Run kRuns[] = {
      {16, G::projector_rows_a_address(), 12},
      {64, G::projector_rows_b_address(), 12},
      {112, G::headlight_colors_a_address(), 32},
      {240, G::headlight_colors_b_address(), 32}};
  for (const Run& run : kRuns) {
    for (uint32_t i = 0; i < run.floats; i++) {
      CopyFloat(out + run.to + 4 * i, run.from + 4 * i);
    }
  }
}

namespace {


// A shape (544 bytes from the model's +40).
uint32_t ShapeAt(uint32_t model, uint32_t shape) {
  return shape * 544 + ModelView(model).shapes().address;
}

// Whether RecordShapeDepth records material `record`: its +404 shares the
// mask (when not 0; not a MaterialRecord field yet), and its +388 is 0 or
// the byte is set.
bool DepthRecords(uint32_t record, uint32_t mask, uint32_t with_flagged) {
  if (mask != 0 && (guest::LoadU32(record + 404) & mask) == 0) {
    return false;
  }
  return schema::MaterialRecord(record).render_state_96() == 0 ||
         (with_flagged & 0xFF) != 0;
}

// The first material of the group (as the original works it out).
uint32_t FirstMaterial(uint32_t model, uint32_t shape, int32_t group) {
  const schema::MaterialBlock block =
      schema::ModelShape(ShapeAt(model, shape)).materials();
  if (group == 0) {
    return block.records_address();
  }
  return (block.group0_count() << 9) + block.records_address();
}

// The slots RecordShapeDepth takes (0 when it records nothing).
uint32_t RecordShapeDepthSlots(uint32_t model, uint32_t shape, int32_t group,
                               uint32_t mask, uint32_t with_flagged) {
  if (models::MaterialCount(model, shape, group) == 0) {
    return 0;
  }
  const uint32_t at = ShapeAt(model, shape);
  uint32_t slots = 7;
  if (Shape(at).skinned() == 1 &&
      Shape(at).skin().object() != 0) {
    slots += 2;
  }
  uint32_t record = FirstMaterial(model, shape, group);
  for (int32_t i = 0; i < int32_t(models::MaterialCount(model, shape, group));
       i++, record += 512) {
    if (DepthRecords(record, mask, with_flagged)) {
      slots += 7;
    }
  }
  return slots;
}

// One slot's three words (device-shaders' ChunkCommand: a command's id and
// arguments, or a payload row's), in this order.
void Slot(uint32_t slot, uint32_t w0, uint32_t w1, uint32_t w2) {
  const schema::ChunkCommand c(slot);
  c.set_id(w0);
  c.set_arg0(w1);
  c.set_arg1(w2);
}

// RecordShapeDepth's recording, its room checked.
uint32_t RecordShapeDepthChecked(uint32_t model, uint32_t shape, int32_t group,
                                 uint32_t mask, uint32_t with_flagged) {
  const uint32_t list = G::record_list();
  const uint32_t at = ShapeAt(model, shape);
  const uint32_t stream =
      Shape(at).skinned() == 1 ? Shape(at).skinned_stream1() : Shape(at).vertex_buffer1_address();
  command_list::OpenChunk(list);
  const uint32_t header = command_list::AllocateSlots(list, 1);
  Slot(header, 10, 1, 0);
  const uint32_t declaration = command_list::AllocateSlots(list, 1);
  guest::StoreU32(declaration,
                  DepthVertexDeclaration(int32_t(guest::LoadU32(Shape(at).stream1_address() + 4))));
  Slot(command_list::AllocateSlots(list, 1), 11, 1, 0);
  uint32_t slot = command_list::AllocateSlots(list, 1);
  Slot(slot, 1, stream, 0);
  guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream1_address() + 8));
  Slot(command_list::AllocateSlots(list, 1), 7, 1, 0x01012B01);
  CopyWords(command_list::AllocateSlots(list, 1), G::depth_constant_address(), 4);
  if (Shape(at).skinned() == 1) {
    const uint32_t indices = Shape(at).skin().address;
    if (schema::SkinBlock(indices).object() != 0) {
      Slot(command_list::AllocateSlots(list, 1), 22, 1, 0);
      slot = command_list::AllocateSlots(list, 1);
      schema::ChunkCommand(slot).set_arg1(1);
      schema::ChunkCommand(slot).set_arg0(schema::SkinBlock(indices).object());
    }
  }
  uint32_t record = FirstMaterial(model, shape, group);
  for (int32_t i = 0; i < int32_t(models::MaterialCount(model, shape, group));
       i++, record += 512) {
    if (!DepthRecords(record, mask, with_flagged)) {
      continue;
    }
    const schema::MaterialRecord r(record);
    Slot(command_list::AllocateSlots(list, 1), 1, 2, 0);
    slot = command_list::AllocateSlots(list, 1);
    schema::ChunkCommand(slot).set_id(100);
    schema::ChunkCommand(slot).set_arg0(r.render_state_100());
    slot = command_list::AllocateSlots(list, 1);
    schema::ChunkCommand(slot).set_id(56);
    schema::ChunkCommand(slot).set_arg0(r.cull_mode());
    Slot(command_list::AllocateSlots(list, 1), 12, 1, 0);
    guest::StoreU32(command_list::AllocateSlots(list, 1), r.index_buffer_address());
    Slot(command_list::AllocateSlots(list, 1), 18, 1, 0);
    slot = command_list::AllocateSlots(list, 1);
    guest::StoreU16(slot + 4, 0);
    guest::StoreU16(slot + 6, 0);
    schema::ChunkCommand(slot).set_id(r.primitive_type());
    schema::ChunkCommand(slot).set_arg1(r.draw_count());
  }
  EndRecord(list);
  return header;
}

}  // namespace

std::optional<uint32_t> RecordShapeDepth(uint32_t model, uint32_t shape,
                                         int32_t group, uint32_t mask, uint32_t with_flagged) {
  ITC_NATIVE_RUNNING("sub_825960D0 Model::RecordShapeDepth");
  const uint32_t slots = RecordShapeDepthSlots(model, shape, group, mask, with_flagged);
  if (slots == 0) {
    return 0;
  }
  if (!CanRecord(G::record_list(), slots)) {
    return std::nullopt;
  }
  return RecordShapeDepthChecked(model, shape, group, mask, with_flagged);
}

std::optional<DepthDraw> DrawShapeDepth(uint32_t model, uint32_t view_projection,
                                        uint32_t world, uint32_t shape, uint32_t group,
                                        uint32_t mask, uint32_t with_flagged, uint32_t frame,
                                        const std::array<uint32_t, 9>& context) {
  ITC_NATIVE_RUNNING("sub_82596D48 Model::DrawShapeDepth");
  const uint32_t list = G::record_list();
  const int32_t which = (group & 0xFF) != 0 ? 1 : 0;
  // Every slot it would take, checked before anything is done.
  if (!CanRecord(list,
                 11 + 2 + RecordShapeDepthSlots(model, shape, which, mask, with_flagged))) {
    return std::nullopt;
  }
  DepthDraw draw;
  if (ModelView(model).header().address == 0) {
    draw.r3 = model;
    return draw;
  }
  if ((IsShapeVisible(model, view_projection, world, shape, frame - 96) & 0xFF) == 0) {
    draw.r3 = 0;
    return draw;
  }
  interpolation::BlendState_BeforeRecordObject(view_projection, world);
  interpolation::BlendState_SetRecordContext(context);
  const auto constants = RecordObject(view_projection, world);
  interpolation::BlendState_SetRecordContext({});
  interpolation::BlendState_AfterRecordObject();
  if (!constants) {
    ITC_LOG_ERROR("Model::DrawShapeDepth: RecordObject declined after its checks");
    std::abort();
  }
  command_list::OpenChunk(list);
  const uint32_t entry = command_list::AllocateSlots(list, 2);
  command_list::CloseChunk(list);
  if (models::MaterialCount(model, shape, which) == 0) {
    draw.r3 = list;
    return draw;
  }
  schema::ChunkItem(entry).set_link(*constants);
  guest::StoreU16(entry + 2, 3);
  guest::StoreU16(entry + 18, 4);
  schema::ChunkItem(entry + 16).set_link(
                  RecordShapeDepthChecked(model, shape, which, mask, with_flagged));
  draw.submit = true;
  draw.entry = entry;
  draw.key = G::depth_key();
  draw.depth = -Lfs(schema::RecordedObjectConstants(*constants).depth_address());
  return draw;
}

namespace {


// SceneryRecordShape's slots, or nullopt where it declines (see the header).
std::optional<uint32_t> SceneryRecordShapeSlots(uint32_t model, uint32_t shape,
                                                int32_t group, uint32_t set_shaders,
                                                uint32_t c76, uint32_t range) {
  const schema::SceneryModel m(model);
  const int32_t count = int32_t(models::MaterialCount(model, shape, group));
  if (count == 0) {
    return 0;
  }
  const uint32_t at = ShapeAt(model, shape);
  if (!models::VertexDeclaration(guest::LoadU32(Shape(at).stream0_address() + 4),
                                 int32_t(guest::LoadU32(Shape(at).stream1_address() + 4)),
                                 guest::LoadU32(Shape(at).stream2_address() + 4))) {
    return std::nullopt;
  }
  const uint32_t type = guest::LoadU32(Shape(at).stream1_address() + 4);
  const bool short_streams = type == 9 || type == 10 || guest::LoadU32(Shape(at).stream2_address() + 4) == 6;
  uint32_t slots = 2 + (short_streams ? 3 : 4) + 1 + 4 + 1 + 12 + 1 + 8 + 1;
  if (Shape(at).skinned() == 1 &&
      Shape(at).skin().object() != 0) {
    slots += 2;
  }
  uint32_t material = models::Material(model, shape, 0, group);
  for (int32_t i = 0; i < count; i++, material += 512) {
    const int32_t textures = int32_t(Record(material).texture_count());
    if (textures > 4 || int32_t(Record(material).material_animation()) != -1) {
      return std::nullopt;
    }
    slots += (set_shaders != 0 ? 4 : 0) + 6 + 6 + 2 + (group == 0 ? 3 : 0) + 2 + 4;
    if (m.texture_provider<schema::View>().address != 0 && textures != 0) {
      slots += 2 + 4 * uint32_t(std::max(textures, 0));
    } else {
      slots += 5;
    }
    const uint32_t flags = Record(material).flags();
    slots += ((flags & 2) != 0 ? 2 : 0) + ((flags & 4) != 0 ? 2 : 0);
    slots += (range != 0 ? 4 : 0) + (c76 != 0 ? 4 : 0);
  }
  return slots;
}

// A slot, which the checks before made sure of.
uint32_t Take(uint32_t list, uint32_t count = 1) {
  const uint32_t slot = command_list::AllocateSlots(list, count);
  if (slot == 0) {
    ITC_LOG_ERROR("scenery_lists: the command list refused a slot after its checks");
    std::abort();
  }
  return slot;
}

uint32_t RecordShapeChecked(const DrawCalls& calls, uint32_t frame,
                            uint32_t model, uint32_t shape, int32_t group, uint32_t set_shaders,
                            uint32_t c76, uint32_t range, uint32_t pair_offset) {
  const schema::SceneryModel m(model);
  const uint32_t list = G::record_list();
  const uint32_t at = ShapeAt(model, shape);
  uint32_t stream = Shape(at).vertex_buffer1_address();
  uint32_t second = Shape(at).vertex_buffer2_address();
  if (Shape(at).skinned() == 1) {
    stream = Shape(at).skinned_stream1();
    second = Shape(at).skinned_stream2();
  }
  command_list::OpenChunk(list);
  const uint32_t header = Take(list);
  guest::StoreU32(frame + 84, header);
  Slot(header, 10, 1, 0);
  const uint32_t declaration = Take(list);
  guest::StoreU32(declaration,
                  *models::VertexDeclaration(guest::LoadU32(Shape(at).stream0_address() + 4),
                                             int32_t(guest::LoadU32(Shape(at).stream1_address() + 4)),
                                             guest::LoadU32(Shape(at).stream2_address() + 4)));
  const uint32_t type = guest::LoadU32(Shape(at).stream1_address() + 4);
  uint32_t slot;
  if (type != 9 && type != 10 && guest::LoadU32(Shape(at).stream2_address() + 4) != 6) {
    Slot(Take(list), 11, 3, 0);
    slot = Take(list);
    Slot(slot, 0, Shape(at).vertex_buffer0_address(), 0);
    guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream0_address() + 8));
    slot = Take(list);
    Slot(slot, 1, stream, 0);
    guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream1_address() + 8));
    slot = Take(list);
    Slot(slot, 2, second, 0);
    guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream2_address() + 8));
  } else {
    Slot(Take(list), 11, 2, 0);
    slot = Take(list);
    Slot(slot, 0, Shape(at).vertex_buffer0_address(), 0);
    guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream0_address() + 8));
    slot = Take(list);
    Slot(slot, 1, stream, 0);
    guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream1_address() + 8));
  }
  if (Shape(at).skinned() == 1) {
    const uint32_t indices = Shape(at).skin().address;
    if (schema::SkinBlock(indices).object() != 0) {
      Slot(Take(list), 22, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_arg1(1);
      schema::ChunkCommand(slot).set_arg0(schema::SkinBlock(indices).object());
    }
  }
  Slot(Take(list), 2, 4, 0);
  for (uint32_t k = 0; k < 4; k++) {
    Slot(Take(list), k, 28, guest::LoadU32(Shape(at).sampler_state_28_address()));
  }
  uint32_t material = models::Material(model, shape, 0, group);
  Slot(Take(list), 2, 12, 0);
  for (uint32_t k = 0; k < 4; k++) {
    Slot(Take(list), k, 16, 4);
    Slot(Take(list), k, 20, 4);
    Slot(Take(list), k, 36, 3);
  }
  const double one = Lfs(G::one_address());
  const double zero = Lfs(G::zero_address());
  if (int32_t(models::MaterialCount(model, shape, group)) > 0) {
    guest::StoreFloat(frame + 116, float(one));
    guest::StoreFloat(frame + 120, float(zero));
    guest::StoreFloat(frame + 124, float(zero));
    for (int32_t i = 0; i < int32_t(models::MaterialCount(model, shape, group));
         i++, material += 512) {
      for (uint32_t offset = 96; offset < 112; offset += 4) {
        guest::StoreFloat(frame + offset, float(one));
      }
      guest::StoreU32(frame + 80, Record(material).blend_mode());
      // The shaders: the model's own pair, or the shared one.
      if (set_shaders != 0) {
        const uint32_t pair = Record(material).shared_pair();
        if (pair == 0) {
          Slot(Take(list), 8, 1, 0);
          guest::StoreU32(Take(list),
                          schema::ModelShaderPair((Record(material).shader_pair() << 4) +
                                                   m.shader_pairs<schema::View>().address).vertex_shader());
          Slot(Take(list), 9, 1, 0);
          guest::StoreU32(Take(list),
                          schema::ModelShaderPair((Record(material).shader_pair() << 4) +
                                                   m.shader_pairs<schema::View>().address).pixel_shader());
        } else {
          Slot(Take(list), 8, 1, 0);
          slot = Take(list);
          guest::StoreU32(slot,
                          models::SharedVertexShader(Record(material).shared_pair() + pair_offset));
          Slot(Take(list), 9, 1, 0);
          slot = Take(list);
          guest::StoreU32(slot,
                          models::SharedPixelShader(Record(material).shared_pair() + pair_offset));
        }
      }
      // Vertex c9-c13: the textures' colors, then the material's +0.
      Slot(Take(list), 7, 5, 0x00010905);
      const uint32_t vertex = Take(list, 5);
      for (int32_t j = 0; j < int32_t(Record(material).texture_count()); j++) {
        const uint32_t out = vertex + 16 * uint32_t(j);
        const int32_t index = int32_t(guest::LoadU32(Record(material).color_index_address() + 4 * uint32_t(j)));
        double third;
        if (index != -1) {
          const uint32_t color = (uint32_t(index) << 4);
          CopyFloat(out + 0, color + m.float4_values());
          CopyFloat(out + 4, color + m.float4_values() + 4);
          third = Lfs(color + m.float4_values() + 8);
        } else {
          guest::StoreFloat(out + 0, float(zero));
          guest::StoreFloat(out + 4, float(zero));
          guest::StoreFloat(out + 8, float(zero));
          guest::StoreFloat(out + 12, float(one));
          third = Lfs(Record(material).texture_float_address() + 4 * uint32_t(j));
        }
        guest::StoreFloat(frame + 96 + 4 * uint32_t(j), float(third));
      }
      CopyWords(vertex + 64, Record(material).color_r_address(), 4);
      // The model's per-draw hook (slot 1).
      {
        const uint32_t args[5] = {model, shape, uint32_t(i), vertex + 64, frame + 80};
        calls.call(calls.user,
                   m.vtable().draw_hook(), args, 5, frame,
                   0x82147170);
      }
      // Pixel c63-c67.
      Slot(Take(list), 7, 5, 0x01013F05);
      const uint32_t pixel = Take(list, 5);
      const uint32_t colors_low = guest::LoadU32(frame + 96);
      const uint32_t colors_low2 = guest::LoadU32(frame + 100);
      const uint32_t colors_high = guest::LoadU32(frame + 104);
      const uint32_t colors_high2 = guest::LoadU32(frame + 108);
      CopyWords(pixel + 0, Record(material).pixel_c63_xy_address(), 4);
      CopyFloat(frame + 112, Record(material).pixel_c64_x_address());
      CopyWords(pixel + 16, frame + 112, 4);
      CopyWords(pixel + 32, Record(material).pixel_c65_r_address(), 4);
      CopyFloat(pixel + 44, Record(material).pixel_c64_z_address());
      CopyWords(pixel + 48, Record(material).pixel_c66_r_address(), 4);
      guest::StoreU32(pixel + 64, colors_low);
      guest::StoreU32(pixel + 68, colors_low2);
      guest::StoreU32(pixel + 72, colors_high);
      guest::StoreU32(pixel + 76, colors_high2);
      // The blend (render state 3) and, for group 0, 96 and 100.
      Slot(Take(list), 3, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(0);
      schema::ChunkCommand(slot).set_arg0(
                      guest::LoadU32((guest::LoadU32(frame + 80) << 2) +
                                               G::blend_states_address()));
      if (group == 0) {
        if (Record(material).render_state_96() != 0) {
          Slot(Take(list), 1, 2, 0);
          slot = Take(list);
          schema::ChunkCommand(slot).set_arg0(1);
          schema::ChunkCommand(slot).set_id(96);
          slot = Take(list);
          schema::ChunkCommand(slot).set_id(100);
          schema::ChunkCommand(slot).set_arg0(64);
        } else {
          Slot(Take(list), 1, 2, 0);
          slot = Take(list);
          schema::ChunkCommand(slot).set_arg0(0);
          schema::ChunkCommand(slot).set_id(96);
          slot = Take(list);
          schema::ChunkCommand(slot).set_id(100);
          schema::ChunkCommand(slot).set_arg0(Record(material).render_state_100());
        }
      }
      Slot(Take(list), 1, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(56);
      schema::ChunkCommand(slot).set_arg0(Record(material).cull_mode());
      // The textures and their sampler states.
      const uint32_t provider = m.texture_provider<schema::View>().address;
      if (provider != 0 && Record(material).texture_count() != 0) {
        slot = Take(list);
        schema::ChunkCommand(slot).set_id(4);
        schema::ChunkCommand(slot).set_arg1(0);
        schema::ChunkCommand(slot).set_arg0(Record(material).texture_count());
        for (int32_t k = 0; k < int32_t(Record(material).texture_count()); k++) {
          const uint32_t id = Record(material).xpr_index_address() + 4 * uint32_t(k);
          const uint32_t set = m.animations<schema::View>().address;
          uint32_t texture = 0;
          bool lookup = true;
          uint32_t first = guest::LoadU32(id);
          uint32_t second_word = guest::LoadU32(Record(material).texture_index_address() + 4 * uint32_t(k));
          if (set != 0 && guest::LoadU32(id) == 0) {
            const uint32_t entry = AnimationCurrentFrame(set, guest::LoadU32(Record(material).texture_index_address() + 4 * uint32_t(k)));
            if (entry == 0) {
              lookup = false;
            } else {
              second_word = schema::AnimationFrameEntry(entry).texture_id();
              first = schema::AnimationFrameEntry(entry).unknown_0();
            }
          }
          if (lookup) {
            const uint32_t from = m.texture_provider<schema::View>().address;
            const uint32_t args[3] = {from, first, second_word};
            texture = ProviderSlotCall(calls,
                             schema::TextureProvider(from).vtable().xpr_texture_5(), args, frame, 0x821473D0);
          }
          slot = Take(list);
          schema::ChunkCommand(slot).set_arg0(texture);
          schema::ChunkCommand(slot).set_id(uint32_t(k));
        }
        slot = Take(list);
        schema::ChunkCommand(slot).set_id(2);
        schema::ChunkCommand(slot).set_arg1(0);
        schema::ChunkCommand(slot).set_arg0(Record(material).texture_count() * 3);
        for (int32_t k = 0; k < int32_t(Record(material).texture_count()); k++) {
          const uint32_t sampler = uint32_t(k);
          slot = Take(list);
          schema::ChunkCommand(slot).set_id(sampler);
          schema::ChunkCommand(slot).set_arg0(0);
          schema::ChunkCommand(slot).set_arg1(guest::LoadU32(Record(material).sampler_state_0_address() + 4 * sampler));
          slot = Take(list);
          schema::ChunkCommand(slot).set_id(sampler);
          schema::ChunkCommand(slot).set_arg0(4);
          schema::ChunkCommand(slot).set_arg1(guest::LoadU32(Record(material).sampler_state_4_address() + 4 * sampler));
          slot = Take(list);
          schema::ChunkCommand(slot).set_id(sampler);
          schema::ChunkCommand(slot).set_arg0(24);
          schema::ChunkCommand(slot).set_arg1(m.sampler_state_24());
        }
      } else {
        Slot(Take(list), 4, 4, 0);
        for (uint32_t k = 0; k < 4; k++) {
          slot = Take(list);
          schema::ChunkCommand(slot).set_id(k);
          schema::ChunkCommand(slot).set_arg0(0);
        }
      }
      // The environment textures.
      for (const auto& [bit, function_slot, lr] :
           {std::tuple<uint32_t, uint32_t, uint32_t>{2, 24, 0x82147534},
            std::tuple<uint32_t, uint32_t, uint32_t>{4, 20, 0x82147590}}) {
        if ((Record(material).flags() & bit) == 0) {
          continue;
        }
        const uint32_t from = m.texture_provider<schema::View>().address;
        const uint32_t args[3] = {from, Record(material).environment_arg_1(),
                                  Record(material).environment_arg_0()};
        const uint32_t texture = ProviderSlotCall(calls,
                             (function_slot == 24 ? schema::TextureProvider(from).vtable().xpr_texture_6()
                                                 : schema::TextureProvider(from).vtable().xpr_texture_5()), args, frame, lr);
        Slot(Take(list), 4, 1, 0);
        slot = Take(list);
        schema::ChunkCommand(slot).set_arg0(texture);
        schema::ChunkCommand(slot).set_id(6);
      }
      // Vertex c86 from the range and pixel c76.
      if (range != 0) {
        Slot(Take(list), 7, 1, 0x00015601);
        slot = Take(list);
        const double near = Lfs(range);
        const double far = Lfs(range + 4);
        const double span = double(float(far - near));
        guest::StoreFloat(slot + 0, float(near));
        guest::StoreFloat(slot + 12, float(zero));
        guest::StoreFloat(slot + 4, float(far));
        guest::StoreFloat(slot + 8, float(double(float(one / span))));
      }
      if (c76 != 0) {
        Slot(Take(list), 7, 1, 0x01014C01);
        slot = Take(list);
        for (uint32_t w = 0; w < 4; w++) {
          CopyFloat(slot + 4 * w, c76 + 4 * w);
        }
      }
      // The draw.
      Slot(Take(list), 12, 1, 0);
      guest::StoreU32(Take(list), Record(material).index_buffer_address());
      Slot(Take(list), 18, 1, 0);
      slot = Take(list);
      guest::StoreU16(slot + 4, 0);
      guest::StoreU16(slot + 6, 0);
      schema::ChunkCommand(slot).set_id(Record(material).primitive_type());
      schema::ChunkCommand(slot).set_arg1(Record(material).draw_count());
      // The constants put back.
      if (range != 0) {
        Slot(Take(list), 7, 1, 0x00015601);
        slot = Take(list);
        for (uint32_t w = 0; w < 4; w++) {
          CopyFloat(slot + 4 * w, G::range_after_address() + 4 * w);
        }
      }
      if (c76 != 0) {
        Slot(Take(list), 7, 1, 0x01014C01);
        slot = Take(list);
        for (uint32_t w = 0; w < 4; w++) {
          CopyFloat(slot + 4 * w, G::pixel_c76_after_address() + 4 * w);
        }
      }
    }
  }
  Slot(Take(list), 2, 8, 0);
  for (uint32_t k = 0; k < 4; k++) {
    slot = Take(list);
    schema::ChunkCommand(slot).set_id(k);
    schema::ChunkCommand(slot).set_arg1(1);
    schema::ChunkCommand(slot).set_arg0(16);
    slot = Take(list);
    schema::ChunkCommand(slot).set_arg1(1);
    schema::ChunkCommand(slot).set_id(k);
    schema::ChunkCommand(slot).set_arg0(20);
  }
  EndRecord(list);
  return header;
}

}  // namespace

std::optional<uint32_t> SceneryRecordShape(const DrawCalls& calls, uint32_t frame,
                                           uint32_t model, uint32_t shape, int32_t group,
                                           uint32_t set_shaders, uint32_t c76, uint32_t range,
                                           uint32_t pair_offset) {
  ITC_NATIVE_RUNNING("sub_82146BD0 SceneryModel::RecordShape");
  const auto slots =
      SceneryRecordShapeSlots(model, shape, group, set_shaders, c76, range);
  if (!slots) {
    return std::nullopt;
  }
  if (*slots == 0) {
    return 0;
  }
  if (!CanRecord(G::record_list(), *slots)) {
    return std::nullopt;
  }
  return RecordShapeChecked(calls, frame, model, shape, group, set_shaders, c76, range,
                            pair_offset);
}

namespace {

// The slots SceneryDrawShape takes when the shape is visible, or nullopt
// where a recorder would decline.
std::optional<uint32_t> SceneryDrawShapeSlots(uint32_t model, uint32_t shape,
                                              uint32_t depth_too, uint32_t c76,
                                              uint32_t range) {
  const auto group0 = SceneryRecordShapeSlots(model, shape, 0, 1, c76, range);
  const auto group1 = SceneryRecordShapeSlots(model, shape, 1, 1, c76, range);
  if (!group0 || !group1) {
    return std::nullopt;
  }
  uint32_t slots = 6 + 11 + 65 + *group0 + *group1;
  if (int32_t(depth_too) != 0) {
    slots += 2 + RecordShapeDepthSlots(model, shape, 0, 0, 0);
  }
  return slots;
}

}  // namespace

std::optional<SceneryDraw> SceneryDrawShape(const DrawCalls& calls,
                                            uint32_t frame, uint32_t model,
                                            uint32_t view_projection, uint32_t world,
                                            uint32_t shape, uint32_t depth_too, uint32_t c76,
                                            uint32_t range, uint32_t pair_offset,
                                            uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_82147D88 SceneryModel::DrawShape");
  const uint32_t list = G::record_list();
  // Every slot it would take, checked before anything is done.
  const auto slots = SceneryDrawShapeSlots(model, shape, depth_too, c76, range);
  if (!slots || !CanRecord(list, *slots)) {
    return std::nullopt;
  }
  SceneryDraw draw;
  if (schema::SceneryModel(model).header() == 0) {
    draw.r3 = model;
    return draw;
  }
  if ((IsShapeVisible(model, view_projection, world, shape, frame - 96) & 0xFF) == 0) {
    draw.r3 = 0;
    return draw;
  }
  command_list::OpenChunk(list);
  const uint32_t entry0 = Take(list, 3);
  const uint32_t entry1 = Take(list, 3);
  command_list::CloseChunk(list);
  interpolation::BlendState_BeforeRecordObject(view_projection, world);
  interpolation::BlendState_SetRecordContext({0x82147E10, caller_r24, world, 0x82960000u, shape,
                                              view_projection, entry1, entry0, model});
  const auto constants = RecordObject(view_projection, world);
  interpolation::BlendState_SetRecordContext({});
  interpolation::BlendState_AfterRecordObject();
  const auto lights = RecordSceneLights();
  if (!constants || !lights) {
    ITC_LOG_ERROR("SceneryModel::DrawShape: a recorder declined after its checks");
    std::abort();
  }
  const uint32_t record_frame = frame - kRecordShapeFrame;
  guest::StoreU32(record_frame, frame);  // its back chain, as stwu
  struct Group {
    int32_t group;
    uint32_t entry;
    uint32_t key;
    bool negate;
    uint32_t lr;
  };
  const Group groups[2] = {{0, entry0, G::group0_key_address(), true, 0x82147EA8},
                           {1, entry1, G::group1_key_address(), false, 0x82147F1C}};
  for (const Group& g : groups) {
    if (models::MaterialCount(model, shape, g.group) == 0) {
      draw.queued = false;
      draw.r3 = 0;
      continue;
    }
    guest::StoreU16(g.entry + 2, 3);
    schema::ChunkItem(g.entry).set_link(*constants);
    guest::StoreU16(g.entry + 18, 3);
    schema::ChunkItem(g.entry + 16).set_link(*lights);
    guest::StoreU16(g.entry + 34, 4);
    schema::ChunkItem(g.entry + 32).set_link(
                    RecordShapeChecked(calls, record_frame, model, shape, g.group, 1, c76,
                                       range, pair_offset));
    const double depth =
        Lfs(schema::RecordedObjectConstants(*constants).depth_address());
    draw.r3 = calls.submit(calls.user, g.entry, guest::LoadU32(g.key),
                           g.negate ? -depth : depth, frame, g.lr);
    draw.queued = true;
  }
  if (int32_t(depth_too) == 0) {
    return draw;
  }
  command_list::OpenChunk(list);
  const uint32_t entry = Take(list, 2);
  command_list::CloseChunk(list);
  if (models::MaterialCount(model, shape, 0) == 0) {
    draw.queued = false;
    draw.r3 = 0;
    return draw;
  }
  guest::StoreU16(entry + 2, 3);
  schema::ChunkItem(entry).set_link(*constants);
  guest::StoreU16(entry + 18, 4);
  schema::ChunkItem(entry + 16).set_link(RecordShapeDepthChecked(model, shape, 0, 0, 0));
  draw.r3 = calls.submit(calls.user, entry, G::scenery_depth_key(),
                         -Lfs(schema::RecordedObjectConstants(*constants)
                                        .depth_address()),
                         frame, 0x82147FB0);
  draw.queued = true;
  return draw;
}

// --- Model::RecordShape and Model::DrawShape (models.md, scenery-lists.md) ---

namespace {


// ModelRecordShape's slots, or nullopt where it declines (see the header).
std::optional<uint32_t> ModelRecordShapeSlots(uint32_t model, uint32_t shape,
                                              int32_t group, uint32_t set_shaders,
                                              uint32_t c76, uint32_t range) {
  if (models::MaterialCount(model, shape, group) == 0) {
    return 0;
  }
  const uint32_t at = ShapeAt(model, shape);
  if (!models::VertexDeclaration(guest::LoadU32(Shape(at).stream0_address() + 4),
                                 int32_t(guest::LoadU32(Shape(at).stream1_address() + 4)),
                                 guest::LoadU32(Shape(at).stream2_address() + 4))) {
    return std::nullopt;
  }
  const uint32_t type = guest::LoadU32(Shape(at).stream1_address() + 4);
  const bool short_streams = type == 9 || type == 10 || guest::LoadU32(Shape(at).stream2_address() + 4) == 6;
  // The header, the declaration, the streams, the samplers' state 28 and
  // the end.
  uint32_t slots = 2 + (short_streams ? 3 : 4) + 5 + 1;
  if (Shape(at).skinned() == 1 &&
      Shape(at).skin().object() != 0) {
    slots += 2;
  }
  const int32_t count = int32_t(models::MaterialCount(model, shape, group));
  const uint32_t provider = ModelView(model).texture_provider().address;
  uint32_t material = FirstMaterial(model, shape, group);
  for (int32_t i = 0; i < count; i++, material += 512) {
    const int32_t textures = int32_t(Record(material).texture_count());
    if (textures > 4 || int32_t(Record(material).material_animation()) != -1) {
      return std::nullopt;
    }
    slots += (set_shaders != 0 ? 4 : 0) + 6 + 6 + 2 + (group == 0 ? 3 : 0) + 2 + 4;
    if (provider != 0 && textures != 0) {
      slots += 2 + 3 * uint32_t(std::max(textures, 0));
    } else {
      slots += 5;
    }
    const uint32_t flags = Record(material).flags();
    const bool ids = int32_t(Record(material).environment_arg_1()) != -1 &&
                     int32_t(Record(material).environment_arg_0()) != -1;
    slots += ((flags & 2) != 0 && ids ? 2 : 0) + ((flags & 4) != 0 && ids ? 2 : 0);
    slots += (range != 0 ? 4 : 0) + (c76 != 0 ? 4 : 0);
  }
  return slots;
}

// The arguments the original saves in its caller's frame (its +332 to +364).
void SaveRecordShapeArguments(uint32_t frame, uint32_t shape, int32_t group,
                              uint32_t set_shaders, uint32_t c76, uint32_t range) {
  guest::StoreU32(frame + 348, set_shaders);
  guest::StoreU32(frame + 356, c76);
  guest::StoreU32(frame + 364, range);
  guest::StoreU32(frame + 332, shape);
  guest::StoreU32(frame + 340, uint32_t(group));
}

uint32_t ModelRecordShapeChecked(const DrawCalls& calls, uint32_t frame,
                                 uint32_t model, uint32_t shape, int32_t group,
                                 uint32_t set_shaders, uint32_t c76, uint32_t range) {
  SaveRecordShapeArguments(frame, shape, group, set_shaders, c76, range);
  const uint32_t list = G::record_list();
  const uint32_t at = ShapeAt(model, shape);
  uint32_t stream = Shape(at).vertex_buffer1_address();
  uint32_t second = Shape(at).vertex_buffer2_address();
  if (Shape(at).skinned() == 1) {
    stream = Shape(at).skinned_stream1();
    second = Shape(at).skinned_stream2();
  }
  command_list::OpenChunk(list);
  uint32_t header = Take(list);
  guest::StoreU32(frame + 92, header);
  Slot(header, 10, 1, 0);
  const uint32_t declaration = Take(list);
  guest::StoreU32(declaration,
                  *models::VertexDeclaration(guest::LoadU32(Shape(at).stream0_address() + 4),
                                             int32_t(guest::LoadU32(Shape(at).stream1_address() + 4)),
                                             guest::LoadU32(Shape(at).stream2_address() + 4)));
  const uint32_t type = guest::LoadU32(Shape(at).stream1_address() + 4);
  const bool short_streams = type == 9 || type == 10 || guest::LoadU32(Shape(at).stream2_address() + 4) == 6;
  uint32_t slot;
  Slot(Take(list), 11, short_streams ? 2 : 3, 0);
  slot = Take(list);
  Slot(slot, 0, Shape(at).vertex_buffer0_address(), 0);
  guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream0_address() + 8));
  slot = Take(list);
  Slot(slot, 1, stream, 0);
  guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream1_address() + 8));
  if (!short_streams) {
    slot = Take(list);
    Slot(slot, 2, second, 0);
    guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream2_address() + 8));
  }
  if (Shape(at).skinned() == 1) {
    const uint32_t indices = Shape(at).skin().address;
    if (schema::SkinBlock(indices).object() != 0) {
      Slot(Take(list), 22, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_arg1(1);
      schema::ChunkCommand(slot).set_arg0(schema::SkinBlock(indices).object());
    }
  }
  Slot(Take(list), 2, 4, 0);
  for (uint32_t k = 0; k < 4; k++) {
    Slot(Take(list), k, 28, guest::LoadU32(Shape(at).sampler_state_28_address()));
  }
  uint32_t material = FirstMaterial(model, shape, group);
  const double one = Lfs(G::one_address());
  const double zero = Lfs(G::zero_address());
  if (int32_t(models::MaterialCount(model, shape, group)) > 0) {
    guest::StoreU32(frame + 88, G::pixel_c76_after_address());
    guest::StoreU32(frame + 84, G::range_after_address());
    for (int32_t i = 0; i < int32_t(models::MaterialCount(model, shape, group));
         i++, material += 512) {
      for (uint32_t offset = 96; offset < 112; offset += 4) {
        guest::StoreFloat(frame + offset, float(one));
      }
      guest::StoreU32(frame + 80, Record(material).blend_mode());
      // The shaders: the model's own pair, or the shared one.
      if (set_shaders != 0) {
        const uint32_t pair = Record(material).shared_pair();
        Slot(Take(list), 8, 1, 0);
        if (pair == 0) {
          guest::StoreU32(Take(list),
                          schema::ModelShaderPair((Record(material).shader_pair() << 4) +
                                                   ModelView(model).shader_pairs().address).vertex_shader());
          Slot(Take(list), 9, 1, 0);
          guest::StoreU32(Take(list),
                          schema::ModelShaderPair((Record(material).shader_pair() << 4) +
                                                   ModelView(model).shader_pairs().address).pixel_shader());
        } else {
          slot = Take(list);
          guest::StoreU32(slot,
                          models::SharedVertexShader(Record(material).shared_pair()));
          Slot(Take(list), 9, 1, 0);
          slot = Take(list);
          guest::StoreU32(slot,
                          models::SharedPixelShader(Record(material).shared_pair()));
        }
      }
      // Vertex c9-c13: the textures' colors, then the material's +0.
      Slot(Take(list), 7, 5, 0x00010905);
      const uint32_t vertex = Take(list, 5);
      for (int32_t j = 0; j < int32_t(Record(material).texture_count()); j++) {
        const uint32_t out = vertex + 16 * uint32_t(j);
        const int32_t index = int32_t(guest::LoadU32(Record(material).color_index_address() + 4 * uint32_t(j)));
        double third;
        if (index != -1) {
          const uint32_t color = (uint32_t(index) << 4);
          CopyFloat(out + 0, color + ModelView(model).float4_values());
          CopyFloat(out + 4, color + ModelView(model).float4_values() + 4);
          third = Lfs(color + ModelView(model).float4_values() + 8);
        } else {
          guest::StoreFloat(out + 0, float(zero));
          guest::StoreFloat(out + 4, float(zero));
          guest::StoreFloat(out + 8, float(zero));
          guest::StoreFloat(out + 12, float(one));
          third = Lfs(Record(material).texture_float_address() + 4 * uint32_t(j));
        }
        guest::StoreFloat(frame + 96 + 4 * uint32_t(j), float(third));
      }
      CopyWords(vertex + 64, Record(material).color_r_address(), 4);
      // (The material animation, +396 not -1, was declined by the count.)
      // The model's per-draw hook (slot 1).
      {
        const uint32_t args[5] = {model, shape, uint32_t(i), vertex + 64, frame + 80};
        calls.call(calls.user, ModelView(model).vtable().draw_hook(), args, 5,
                   frame, 0x82595A9C);
      }
      // Pixel c63-c67: the material's +64, +80 (its w as a float), +16 (w
      // the float at +88), +32, then the colors' third floats.
      Slot(Take(list), 7, 5, 0x01013F05);
      const uint32_t pixel = Take(list, 5);
      uint32_t colors[4];
      for (uint32_t w = 0; w < 4; w++) {
        colors[w] = guest::LoadU32(frame + 96 + 4 * w);
      }
      CopyWords(pixel + 0, Record(material).pixel_c63_xy_address(), 4);
      CopyWords(pixel + 16, Record(material).pixel_c64_x_address(), 4);
      CopyFloat(pixel + 28, material + 92);
      CopyWords(pixel + 32, Record(material).pixel_c65_r_address(), 4);
      CopyFloat(pixel + 44, Record(material).pixel_c64_z_address());
      CopyWords(pixel + 48, Record(material).pixel_c66_r_address(), 4);
      for (uint32_t w = 0; w < 4; w++) {
        guest::StoreU32(pixel + 64 + 4 * w, colors[w]);
      }
      // The blend (render state 3) and, for group 0, 96 and 100.
      Slot(Take(list), 3, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(0);
      schema::ChunkCommand(slot).set_arg0(
                      guest::LoadU32((guest::LoadU32(frame + 80) << 2) +
                                               G::model_blend_states_address()));
      if (group == 0) {
        const bool flagged = Record(material).render_state_96() != 0;
        Slot(Take(list), 1, 2, 0);
        slot = Take(list);
        schema::ChunkCommand(slot).set_arg0(flagged ? 1 : 0);
        schema::ChunkCommand(slot).set_id(96);
        slot = Take(list);
        schema::ChunkCommand(slot).set_id(100);
        schema::ChunkCommand(slot).set_arg0(flagged ? 64 : Record(material).render_state_100());
      }
      Slot(Take(list), 1, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(56);
      schema::ChunkCommand(slot).set_arg0(Record(material).cull_mode());
      // The textures (the provider's slot 5) and their sampler states 0 and
      // 4.
      if (ModelView(model).texture_provider().address != 0 && Record(material).texture_count() != 0) {
        slot = Take(list);
        schema::ChunkCommand(slot).set_id(4);
        schema::ChunkCommand(slot).set_arg1(0);
        schema::ChunkCommand(slot).set_arg0(Record(material).texture_count());
        for (int32_t k = 0; k < int32_t(Record(material).texture_count()); k++) {
          const uint32_t from = ModelView(model).texture_provider().address;
          const uint32_t args[3] = {from, guest::LoadU32(Record(material).xpr_index_address() + 4 * uint32_t(k)),
                                    guest::LoadU32(Record(material).texture_index_address() + 4 * uint32_t(k))};
          const uint32_t texture =
              ProviderSlotCall(calls,
                             schema::TextureProvider(from).vtable().xpr_texture_5(), args, frame, 0x82595CBC);
          slot = Take(list);
          schema::ChunkCommand(slot).set_arg0(texture);
          schema::ChunkCommand(slot).set_id(uint32_t(k));
        }
        slot = Take(list);
        schema::ChunkCommand(slot).set_id(2);
        schema::ChunkCommand(slot).set_arg1(0);
        schema::ChunkCommand(slot).set_arg0(Record(material).texture_count() << 1);
        for (int32_t k = 0; k < int32_t(Record(material).texture_count()); k++) {
          const uint32_t sampler = uint32_t(k);
          slot = Take(list);
          schema::ChunkCommand(slot).set_id(sampler);
          schema::ChunkCommand(slot).set_arg0(0);
          schema::ChunkCommand(slot).set_arg1(guest::LoadU32(Record(material).sampler_state_0_address() + 4 * sampler));
          slot = Take(list);
          schema::ChunkCommand(slot).set_arg0(4);
          schema::ChunkCommand(slot).set_id(sampler);
          schema::ChunkCommand(slot).set_arg1(guest::LoadU32(Record(material).sampler_state_4_address() + 4 * sampler));
        }
      } else {
        Slot(Take(list), 4, 4, 0);
        for (uint32_t k = 0; k < 4; k++) {
          slot = Take(list);
          schema::ChunkCommand(slot).set_id(k);
          schema::ChunkCommand(slot).set_arg0(0);
        }
      }
      // The environment textures (the provider's slots 6 and 5, by +104's
      // bits 0x2 and 0x4), when neither id (+124, +120) is -1.
      for (const auto& [bit, function_slot, lr] :
           {std::tuple<uint32_t, uint32_t, uint32_t>{2, 24, 0x82595E18},
            std::tuple<uint32_t, uint32_t, uint32_t>{4, 20, 0x82595E88}}) {
        if ((Record(material).flags() & bit) == 0) {
          continue;
        }
        const uint32_t first = Record(material).environment_arg_1();
        if (int32_t(first) == -1) {
          continue;
        }
        const uint32_t second_id = Record(material).environment_arg_0();
        if (int32_t(second_id) == -1) {
          continue;
        }
        const uint32_t from = ModelView(model).texture_provider().address;
        const uint32_t args[3] = {from, first, second_id};
        const uint32_t texture = ProviderSlotCall(calls,
                             (function_slot == 24 ? schema::TextureProvider(from).vtable().xpr_texture_6()
                                                 : schema::TextureProvider(from).vtable().xpr_texture_5()), args, frame, lr);
        Slot(Take(list), 4, 1, 0);
        slot = Take(list);
        schema::ChunkCommand(slot).set_arg0(texture);
        schema::ChunkCommand(slot).set_id(6);
      }
      // Vertex c86 from the range (through the frame's +112) and pixel c76.
      if (range != 0) {
        Slot(Take(list), 7, 1, 0x00015601);
        slot = Take(list);
        const double near = Lfs(range);
        const double far = Lfs(range + 4);
        guest::StoreFloat(frame + 112, float(near));
        const double span = double(float(far - near));
        guest::StoreFloat(frame + 124, float(zero));
        guest::StoreFloat(frame + 116, float(far));
        guest::StoreFloat(frame + 120, float(double(float(one / span))));
        CopyWords(slot, frame + 112, 4);
      }
      if (c76 != 0) {
        Slot(Take(list), 7, 1, 0x01014C01);
        CopyWords(Take(list), c76, 4);
      }
      // The draw.
      Slot(Take(list), 12, 1, 0);
      guest::StoreU32(Take(list), Record(material).index_buffer_address());
      Slot(Take(list), 18, 1, 0);
      slot = Take(list);
      guest::StoreU16(slot + 4, 0);
      guest::StoreU16(slot + 6, 0);
      schema::ChunkCommand(slot).set_id(Record(material).primitive_type());
      schema::ChunkCommand(slot).set_arg1(Record(material).draw_count());
      // The constants put back (the addresses from the frame's +84 and +88).
      if (range != 0) {
        Slot(Take(list), 7, 1, 0x00015601);
        CopyWords(Take(list), guest::LoadU32(frame + 84), 4);
      }
      if (c76 != 0) {
        Slot(Take(list), 7, 1, 0x01014C01);
        CopyWords(Take(list), guest::LoadU32(frame + 88), 4);
      }
    }
    header = guest::LoadU32(frame + 92);
  }
  EndRecord(list);
  return header;
}

}  // namespace

std::optional<uint32_t> ModelRecordShape(const DrawCalls& calls, uint32_t frame,
                                         uint32_t model, uint32_t shape, int32_t group,
                                         uint32_t set_shaders, uint32_t c76, uint32_t range) {
  ITC_NATIVE_RUNNING("sub_82595550 Model::RecordShape");
  const auto slots = ModelRecordShapeSlots(model, shape, group, set_shaders, c76, range);
  if (!slots) {
    return std::nullopt;
  }
  if (*slots == 0) {
    SaveRecordShapeArguments(frame, shape, group, set_shaders, c76, range);
    return 0;
  }
  if (!CanRecord(G::record_list(), *slots)) {
    return std::nullopt;
  }
  return ModelRecordShapeChecked(calls, frame, model, shape, group, set_shaders, c76,
                                 range);
}

std::optional<SceneryDraw> ModelDrawShape(const DrawCalls& calls, uint32_t frame,
                                          uint32_t model, uint32_t view_projection,
                                          uint32_t world, uint32_t shape, uint32_t depth_too,
                                          uint32_t c76, uint32_t range, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_82596B18 Model::DrawShape");
  const uint32_t list = G::record_list();
  // Every slot it would take, checked before anything is done.
  {
    const auto group0 = ModelRecordShapeSlots(model, shape, 0, 1, c76, range);
    const auto group1 = ModelRecordShapeSlots(model, shape, 1, 1, c76, range);
    if (!group0 || !group1) {
      return std::nullopt;
    }
    uint32_t slots = 6 + 11 + 65 + *group0 + *group1;
    if (int32_t(depth_too) != 0) {
      slots += 2 + RecordShapeDepthSlots(model, shape, 0, 0, 0);
    }
    if (!CanRecord(list, slots)) {
      return std::nullopt;
    }
  }
  SceneryDraw draw;
  if (ModelView(model).header().address == 0) {
    draw.r3 = model;
    return draw;
  }
  if ((IsShapeVisible(model, view_projection, world, shape, frame - 96) & 0xFF) == 0) {
    draw.r3 = 0;
    return draw;
  }
  command_list::OpenChunk(list);
  const uint32_t entry0 = Take(list, 3);
  const uint32_t entry1 = Take(list, 3);
  command_list::CloseChunk(list);
  // The recorder's wrapper's context: the return address and r24-r31 there
  // (the caller's r24, the world matrix, the shape, the view-projection,
  // 0x82970000, the second and first entries, the model).
  interpolation::BlendState_BeforeRecordObject(view_projection, world);
  interpolation::BlendState_SetRecordContext({0x82596BA0, caller_r24, world, shape,
                                              view_projection, 0x82970000u, entry1, entry0,
                                              model});
  const auto constants = RecordObject(view_projection, world);
  interpolation::BlendState_SetRecordContext({});
  interpolation::BlendState_AfterRecordObject();
  const auto lights = RecordSceneLights();
  if (!constants || !lights) {
    ITC_LOG_ERROR("Model::DrawShape: a recorder declined after its checks");
    std::abort();
  }
  // r3 as the original leaves it: the lights' header until a queue call.
  draw.r3 = *lights;
  const uint32_t record_frame = frame - kModelRecordShapeFrame;
  guest::StoreU32(record_frame, frame);  // its back chain, as stwu
  struct Group {
    int32_t group;
    uint32_t entry;
    uint32_t key;
    bool negate;
    uint32_t lr;
  };
  const Group groups[2] = {{0, entry0, G::group0_key_address(), true, 0x82596C38},
                           {1, entry1, G::group1_key_address(), false, 0x82596CA8}};
  for (const Group& g : groups) {
    if (models::MaterialCount(model, shape, g.group) == 0) {
      continue;
    }
    guest::StoreU16(g.entry + 2, 3);
    schema::ChunkItem(g.entry).set_link(*constants);
    guest::StoreU16(g.entry + 18, 3);
    schema::ChunkItem(g.entry + 16).set_link(*lights);
    guest::StoreU16(g.entry + 34, 4);
    schema::ChunkItem(g.entry + 32).set_link(
                    ModelRecordShapeChecked(calls, record_frame, model, shape, g.group, 1,
                                            c76, range));
    const double depth =
        Lfs(schema::RecordedObjectConstants(*constants).depth_address());
    draw.r3 = calls.submit(calls.user, g.entry, guest::LoadU32(g.key),
                           g.negate ? -depth : depth, frame, g.lr);
    draw.queued = true;
  }
  if (int32_t(depth_too) == 0) {
    return draw;
  }
  command_list::OpenChunk(list);
  const uint32_t entry = Take(list, 2);
  command_list::CloseChunk(list);
  draw.queued = false;
  draw.r3 = list;  // as the chunk's close leaves it
  if (models::MaterialCount(model, shape, 0) == 0) {
    return draw;
  }
  guest::StoreU16(entry + 2, 3);
  schema::ChunkItem(entry).set_link(*constants);
  guest::StoreU16(entry + 18, 4);
  schema::ChunkItem(entry + 16).set_link(RecordShapeDepthChecked(model, shape, 0, 0, 0));
  draw.r3 = calls.submit(calls.user, entry, G::scenery_depth_key(),
                         -Lfs(schema::RecordedObjectConstants(*constants)
                                        .depth_address()),
                         frame, 0x82596D3C);
  draw.queued = true;
  return draw;
}

// --- Model::RecordMaterials (models.md) ---

namespace {

constexpr uint32_t kRenderTarget = 0x8209F7A8;     // CameraPasses::RenderTarget

// Where a material's textures come from (the original's branches).
enum class TextureSource {
  kUntextured,      // no provider or no textures, no shared pair: 4 samplers unset
  kNone,            // a shared pair other than 1000-1004: nothing
  kModel444,        // pairs 1000 and 1004: the model's +444 as sampler 0
  kProvider1001,    // pair 1001: the provider's first texture
  kTwo1002,         // pair 1002: the provider's first, then the model's +444
  kProvider1003,    // pair 1003 without the replacement: the provider's first
  kReplaced1003,    // pair 1003: each texture, +460 where its id is +436's
  kModel452,        // category 3 with one texture: the model's +452
  kRenderTarget,    // category 5 with one texture: CameraPasses::RenderTarget(0)
  kFill456,         // the model's +456 for every texture
  kFill460,         // the model's +460 for every texture
  kProvider,        // the provider's slot 5 for every texture
};

TextureSource SourceOf(uint32_t model, uint32_t material) {
  const uint32_t pair = Record(material).shared_pair();
  const int32_t textures = int32_t(Record(material).texture_count());
  if ((ModelView(model).texture_provider().address == 0 || textures == 0) && pair == 0) {
    return TextureSource::kUntextured;
  }
  const bool replaced = ModelView(model).replacement_texture() != 0 &&
                        Record(material).texture_index() == ModelView(model).replaced_texture_index();
  if (pair != 0) {
    switch (pair) {
      case 1000:
      case 1004:
        return TextureSource::kModel444;
      case 1001:
        return TextureSource::kProvider1001;
      case 1002:
        return TextureSource::kTwo1002;
      case 1003:
        return replaced ? TextureSource::kReplaced1003 : TextureSource::kProvider1003;
      default:
        return TextureSource::kNone;
    }
  }
  switch (int32_t(Record(material).category())) {
    case 3:
      return ModelView(model).category3_texture() != 0 && textures == 1 ? TextureSource::kModel452
                                                                      : TextureSource::kProvider;
    case 5:
      return textures == 1 ? TextureSource::kRenderTarget : TextureSource::kProvider;
    default:
      if (int32_t(ModelView(model).use_override_texture()) != 0 &&
          ModelView(model).override_texture() != 0) {
        return TextureSource::kFill456;
      }
      return replaced ? TextureSource::kFill460 : TextureSource::kProvider;
  }
}

// Whether sampler 6 is set (the environment texture and its states).
bool SetsSampler6(uint32_t material) {
  if ((Record(material).flags() & 4) != 0 ||
      Record(material).shared_pair() == 0) {
    return true;
  }
  switch (int32_t(Record(material).category())) {
    case 1:
    case 2:
    case 7:
    case 9:
    case 10:
      return true;
    default:
      return false;
  }
}

// ModelRecordMaterials' slots, or nullopt where it declines (see the header).
std::optional<uint32_t> ModelRecordMaterialsSlots(uint32_t model, uint32_t shape,
                                                  int32_t group, uint32_t set_shaders) {
  if (models::MaterialCount(model, shape, group) == 0) {
    return 0;
  }
  const uint32_t at = ShapeAt(model, shape);
  if (!models::VertexDeclaration(guest::LoadU32(Shape(at).stream0_address() + 4),
                                 int32_t(guest::LoadU32(Shape(at).stream1_address() + 4)),
                                 guest::LoadU32(Shape(at).stream2_address() + 4))) {
    return std::nullopt;
  }
  const uint32_t type = guest::LoadU32(Shape(at).stream1_address() + 4);
  const bool short_streams = type == 9 || type == 10 || guest::LoadU32(Shape(at).stream2_address() + 4) == 6;
  // The header, the declaration, the streams and the end.
  uint32_t slots = 2 + (short_streams ? 3 : 4) + 1;
  if (Shape(at).skinned() == 1 &&
      Shape(at).skin().object() != 0) {
    slots += 2;
  }
  const int32_t count = int32_t(models::MaterialCount(model, shape, group));
  uint32_t material = models::Material(model, shape, 0, group);
  for (int32_t i = 0; i < count; i++, material += 512) {
    const int32_t textures = int32_t(Record(material).texture_count());
    if (textures > 4) {
      return std::nullopt;
    }
    const uint32_t each = uint32_t(std::max(textures, 0));
    // The shaders, render state 3, vertex c9-c13, pixel c63-c67, 96 for
    // group 0, 48/100/56 and the draw.
    slots += (set_shaders != 0 ? 4 : 0) + 2 + 6 + 6 + (group == 0 ? 2 : 0) + 4 + 4;
    switch (SourceOf(model, material)) {
      case TextureSource::kNone:
        break;
      case TextureSource::kTwo1002:
        slots += 8;
        break;
      case TextureSource::kReplaced1003:
      case TextureSource::kFill456:
      case TextureSource::kFill460:
      case TextureSource::kProvider:
        slots += 2 + 3 * each;
        break;
      default:  // untextured, or one texture with its two sampler states
        slots += 5;
        break;
    }
    if (SetsSampler6(material)) {
      slots += 6;
    }
  }
  return slots;
}

// The two sampler states (0 and 4) of `count` textures.
void TextureSamplerStates(uint32_t list, uint32_t material, int32_t count) {
  uint32_t slot = Take(list);
  schema::ChunkCommand(slot).set_id(2);
  schema::ChunkCommand(slot).set_arg1(0);
  schema::ChunkCommand(slot).set_arg0(uint32_t(count) << 1);
  for (int32_t k = 0; k < count; k++) {
    const uint32_t sampler = uint32_t(k);
    slot = Take(list);
    schema::ChunkCommand(slot).set_id(sampler);
    schema::ChunkCommand(slot).set_arg0(0);
    schema::ChunkCommand(slot).set_arg1(guest::LoadU32(Record(material).sampler_state_0_address() + 4 * sampler));
    slot = Take(list);
    schema::ChunkCommand(slot).set_arg0(4);
    schema::ChunkCommand(slot).set_id(sampler);
    schema::ChunkCommand(slot).set_arg1(guest::LoadU32(Record(material).sampler_state_4_address() + 4 * sampler));
  }
}

// The provider's slot 5 for texture `k` (its ids at +144 and +176).
uint32_t ProviderTexture(const DrawCalls& calls, uint32_t frame, uint32_t model,
                         uint32_t material, uint32_t k, uint32_t lr) {
  const uint32_t provider = ModelView(model).texture_provider().address;
  const uint32_t args[3] = {provider, guest::LoadU32(Record(material).xpr_index_address() + 4 * k),
                            guest::LoadU32(Record(material).texture_index_address() + 4 * k)};
  return ProviderSlotCall(calls,
                             schema::TextureProvider(provider).vtable().xpr_texture_5(), args, frame, lr);
}

void RecordMaterialTextures(const DrawCalls& calls, uint32_t frame,
                            uint32_t list, uint32_t model, uint32_t material) {
  const TextureSource source = SourceOf(model, material);
  uint32_t slot;
  // One texture as sampler 0 (the header, the texture slot's +0 then +4),
  // then its two states.
  auto single = [&](auto texture_of, bool texture_first) {
    uint32_t texture = 0;
    if (texture_first) {
      texture = texture_of();
    }
    Slot(Take(list), 4, 1, 0);
    if (!texture_first) {
      texture = texture_of();
    }
    slot = Take(list);
    schema::ChunkCommand(slot).set_id(0);
    schema::ChunkCommand(slot).set_arg0(texture);
    TextureSamplerStates(list, material, 1);
  };
  // Every texture from `texture_of(k)` (called before its slot), then the
  // states.
  auto each = [&](auto texture_of) {
    slot = Take(list);
    schema::ChunkCommand(slot).set_id(4);
    schema::ChunkCommand(slot).set_arg1(0);
    schema::ChunkCommand(slot).set_arg0(Record(material).texture_count());
    for (int32_t k = 0; k < int32_t(Record(material).texture_count()); k++) {
      const uint32_t texture = texture_of(uint32_t(k));
      slot = Take(list);
      schema::ChunkCommand(slot).set_arg0(texture);
      schema::ChunkCommand(slot).set_id(uint32_t(k));
    }
    TextureSamplerStates(list, material, int32_t(Record(material).texture_count()));
  };
  uint32_t provider_lr = 0x820CF71C;
  switch (int32_t(Record(material).category())) {
    case 0:
      provider_lr = 0x820CFCA4;
      break;
    case 3:
      provider_lr = 0x820CF9AC;
      break;
    case 5:
      provider_lr = 0x820CF860;
      break;
    default:
      break;
  }
  switch (source) {
    case TextureSource::kUntextured:
      Slot(Take(list), 4, 4, 0);
      for (uint32_t k = 0; k < 4; k++) {
        slot = Take(list);
        schema::ChunkCommand(slot).set_id(k);
        schema::ChunkCommand(slot).set_arg0(0);
      }
      break;
    case TextureSource::kNone:
      break;
    case TextureSource::kModel444:
      single([&] { return ModelView(model).vinyl_sheet(); }, false);
      break;
    case TextureSource::kModel452:
      single([&] { return ModelView(model).category3_texture(); }, false);
      break;
    case TextureSource::kProvider1001:
      single([&] { return ProviderTexture(calls, frame, model, material, 0, 0x820D0250); },
             true);
      break;
    case TextureSource::kProvider1003:
      single([&] { return ProviderTexture(calls, frame, model, material, 0, 0x820D0144); },
             false);
      break;
    case TextureSource::kRenderTarget:
      single(
          [&] {
            const uint32_t args[1] = {0};
            return calls.call(calls.user, kRenderTarget, args, 1, frame, 0x820CF814);
          },
          false);
      break;
    case TextureSource::kTwo1002: {
      const uint32_t texture =
          ProviderTexture(calls, frame, model, material, 0, 0x820D017C);
      Slot(Take(list), 4, 2, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(0);
      schema::ChunkCommand(slot).set_arg0(texture);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(1);
      schema::ChunkCommand(slot).set_arg0(ModelView(model).vinyl_sheet());
      // Two textures' states, whatever the count.
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(2);
      schema::ChunkCommand(slot).set_arg0(4);
      schema::ChunkCommand(slot).set_arg1(0);
      for (uint32_t k = 0; k < 2; k++) {
        slot = Take(list);
        schema::ChunkCommand(slot).set_id(k);
        schema::ChunkCommand(slot).set_arg0(0);
        schema::ChunkCommand(slot).set_arg1(guest::LoadU32(Record(material).sampler_state_0_address() + 4 * k));
        slot = Take(list);
        schema::ChunkCommand(slot).set_arg0(4);
        schema::ChunkCommand(slot).set_id(k);
        schema::ChunkCommand(slot).set_arg1(guest::LoadU32(Record(material).sampler_state_4_address() + 4 * k));
      }
      break;
    }
    case TextureSource::kReplaced1003:
      each([&](uint32_t k) {
        if (guest::LoadU32(Record(material).texture_index_address() + 4 * k) == ModelView(model).replaced_texture_index()) {
          return ModelView(model).replacement_texture();
        }
        return ProviderTexture(calls, frame, model, material, k, 0x820D0058);
      });
      break;
    case TextureSource::kFill456:
      each([&](uint32_t) { return ModelView(model).override_texture(); });
      break;
    case TextureSource::kFill460:
      each([&](uint32_t) { return ModelView(model).replacement_texture(); });
      break;
    case TextureSource::kProvider:
      each([&](uint32_t k) {
        return ProviderTexture(calls, frame, model, material, k, provider_lr);
      });
      break;
  }
}

uint32_t ModelRecordMaterialsChecked(const DrawCalls& calls, uint32_t frame,
                                     uint32_t model, uint32_t shape, int32_t group,
                                     uint32_t set_shaders) {
  const uint32_t list = G::record_list();
  const uint32_t at = ShapeAt(model, shape);
  uint32_t stream = Shape(at).vertex_buffer1_address();
  uint32_t second = Shape(at).vertex_buffer2_address();
  if (Shape(at).skinned() == 1) {
    stream = Shape(at).skinned_stream1();
    second = Shape(at).skinned_stream2();
  }
  command_list::OpenChunk(list);
  const uint32_t header = Take(list);
  guest::StoreU32(frame + 84, header);
  Slot(header, 10, 1, 0);
  const uint32_t declaration = Take(list);
  guest::StoreU32(declaration,
                  *models::VertexDeclaration(guest::LoadU32(Shape(at).stream0_address() + 4),
                                             int32_t(guest::LoadU32(Shape(at).stream1_address() + 4)),
                                             guest::LoadU32(Shape(at).stream2_address() + 4)));
  const uint32_t type = guest::LoadU32(Shape(at).stream1_address() + 4);
  const bool short_streams = type == 9 || type == 10 || guest::LoadU32(Shape(at).stream2_address() + 4) == 6;
  uint32_t slot;
  Slot(Take(list), 11, short_streams ? 2 : 3, 0);
  slot = Take(list);
  Slot(slot, 0, Shape(at).vertex_buffer0_address(), 0);
  guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream0_address() + 8));
  slot = Take(list);
  Slot(slot, 1, stream, 0);
  guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream1_address() + 8));
  if (!short_streams) {
    slot = Take(list);
    Slot(slot, 2, second, 0);
    guest::StoreU32(slot + 12, guest::LoadU32(Shape(at).stream2_address() + 8));
  }
  if (Shape(at).skinned() == 1) {
    const uint32_t indices = Shape(at).skin().address;
    if (schema::SkinBlock(indices).object() != 0) {
      Slot(Take(list), 22, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_arg1(1);
      schema::ChunkCommand(slot).set_arg0(schema::SkinBlock(indices).object());
    }
  }
  uint32_t material = models::Material(model, shape, 0, group);
  if (int32_t(models::MaterialCount(model, shape, group)) > 0) {
    const double zero = Lfs(G::zero_address());
    const double one = Lfs(G::one_address());
    guest::StoreU32(frame + 80, G::car_blend_states_address());
    for (int32_t i = 0; i < int32_t(models::MaterialCount(model, shape, group));
         i++, material += 512) {
      for (uint32_t offset = 96; offset < 112; offset += 4) {
        guest::StoreFloat(frame + offset, float(one));
      }
      const uint32_t pair = Record(material).shared_pair();
      // The shaders: the model's own pair, or a shared one (1005 for every
      // material when the model's +440 is set, 1000's for 1004).
      if (set_shaders != 0) {
        Slot(Take(list), 8, 1, 0);
        if (pair == 0 && ModelView(model).force_pair_1005() == 0) {
          guest::StoreU32(Take(list),
                          schema::ModelShaderPair((Record(material).shader_pair() << 4) +
                                                   ModelView(model).shader_pairs().address).vertex_shader());
          Slot(Take(list), 9, 1, 0);
          guest::StoreU32(Take(list),
                          schema::ModelShaderPair((Record(material).shader_pair() << 4) +
                                                   ModelView(model).shader_pairs().address).pixel_shader());
        } else {
          const uint32_t shared = ModelView(model).force_pair_1005() != 0 ? 1005
                                  : pair == 1004                         ? 1000
                                                                         : pair;
          guest::StoreU32(Take(list), models::SharedVertexShader(shared));
          Slot(Take(list), 9, 1, 0);
          guest::StoreU32(Take(list), models::SharedPixelShader(shared));
        }
      }
      // The blend (render state 3, the table at the frame's +80).
      Slot(Take(list), 3, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(0);
      schema::ChunkCommand(slot).set_arg0(
                      guest::LoadU32((Record(material).blend_mode() << 2) +
                                               guest::LoadU32(frame + 80)));
      // Vertex c9-c13: four colors (0, 0, 0, 1) and the textures' third
      // floats (+240) at the frame's +96, each texture's color from the
      // model's table (+320) where its index (+208) isn't -1; then the
      // material's +0.
      Slot(Take(list), 7, 5, 0x00010905);
      const uint32_t vertex = Take(list, 5);
      for (uint32_t j = 0; j < 4; j++) {
        const uint32_t out = vertex + 16 * j;
        guest::StoreFloat(out + 0, float(zero));
        guest::StoreFloat(out + 4, float(zero));
        guest::StoreFloat(out + 8, float(zero));
        guest::StoreFloat(out + 12, float(one));
        CopyFloat(frame + 96 + 4 * j, Record(material).texture_float_address() + 4 * j);
      }
      for (int32_t j = 0; j < int32_t(Record(material).texture_count()); j++) {
        const uint32_t out = vertex + 16 * uint32_t(j);
        const int32_t index = int32_t(guest::LoadU32(Record(material).color_index_address() + 4 * uint32_t(j)));
        double third;
        if (index != -1) {
          const uint32_t color = uint32_t(index) << 4;
          CopyFloat(out + 0, color + ModelView(model).float4_values());
          CopyFloat(out + 4, color + ModelView(model).float4_values() + 4);
          third = Lfs(color + ModelView(model).float4_values() + 8);
        } else {
          guest::StoreFloat(out + 0, float(zero));
          guest::StoreFloat(out + 4, float(zero));
          guest::StoreFloat(out + 8, float(zero));
          guest::StoreFloat(out + 12, float(one));
          third = Lfs(Record(material).texture_float_address() + 4 * uint32_t(j));
        }
        guest::StoreFloat(frame + 96 + 4 * uint32_t(j), float(third));
      }
      CopyWords(vertex + 64, Record(material).color_r_address(), 4);
      // Pixel c63-c67: the material's +64, +80, +16 (w the float at +88),
      // +32, then the frame's four floats (as words).
      Slot(Take(list), 7, 5, 0x01013F05);
      const uint32_t pixel = Take(list, 5);
      uint32_t colors[4];
      for (uint32_t w = 0; w < 4; w++) {
        colors[w] = guest::LoadU32(frame + 96 + 4 * w);
      }
      CopyWords(pixel + 0, Record(material).pixel_c63_xy_address(), 4);
      CopyWords(pixel + 16, Record(material).pixel_c64_x_address(), 4);
      CopyWords(pixel + 32, Record(material).pixel_c65_r_address(), 4);
      CopyFloat(pixel + 44, Record(material).pixel_c64_z_address());
      CopyWords(pixel + 48, Record(material).pixel_c66_r_address(), 4);
      for (uint32_t w = 0; w < 4; w++) {
        guest::StoreU32(pixel + 64 + 4 * w, colors[w]);
      }
      // Render state 96 for group 0 (by +388).
      if (group == 0) {
        Slot(Take(list), 1, 1, 0);
        slot = Take(list);
        schema::ChunkCommand(slot).set_arg0(Record(material).render_state_96() != 0 ? 1 : 0);
        schema::ChunkCommand(slot).set_id(96);
      }
      // 48 (0 when [0x826C5104] is set), 100 and 56 (the cull mode, 2 and 6
      // swapped when [0x826C5100] is set).
      Slot(Take(list), 1, 3, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_arg0(G::render_state_48_off() != 0 ? 0 : 1);
      schema::ChunkCommand(slot).set_id(48);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(100);
      schema::ChunkCommand(slot).set_arg0(Record(material).render_state_100());
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(56);
      uint32_t cull = Record(material).cull_mode();
      if (G::cull_swap() != 0 && (cull == 2 || cull == 6)) {
        cull = cull == 2 ? 6 : 2;
      }
      schema::ChunkCommand(slot).set_arg0(cull);
      RecordMaterialTextures(calls, frame, list, model, material);
      // Sampler 6 and its states 16, 20 and 36.
      if (SetsSampler6(material)) {
        uint32_t texture = G::sampler6_texture();
        if (texture == 0) {
          const uint32_t provider = ModelView(model).texture_provider().address;
          const uint32_t args[3] = {provider, Record(material).environment_arg_1(),
                                    Record(material).environment_arg_0()};
          texture = ProviderSlotCall(calls,
                             schema::TextureProvider(provider).vtable().xpr_texture_6(), args, frame, 0x820CFE74);
        }
        Slot(Take(list), 4, 1, 0);
        slot = Take(list);
        schema::ChunkCommand(slot).set_id(6);
        schema::ChunkCommand(slot).set_arg0(texture);
        Slot(Take(list), 2, 3, 0);
        Slot(Take(list), 6, 16, 4);
        Slot(Take(list), 6, 20, 4);
        Slot(Take(list), 6, 36, 3);
      }
      // The draw.
      Slot(Take(list), 12, 1, 0);
      guest::StoreU32(Take(list), Record(material).index_buffer_address());
      Slot(Take(list), 18, 1, 0);
      slot = Take(list);
      schema::ChunkCommand(slot).set_id(Record(material).primitive_type());
      guest::StoreU16(slot + 4, 0);
      guest::StoreU16(slot + 6, 0);
      schema::ChunkCommand(slot).set_arg1(Record(material).draw_count());
    }
  }
  EndRecord(list);
  return guest::LoadU32(frame + 84);
}

}  // namespace

std::optional<uint32_t> ModelRecordMaterials(const DrawCalls& calls,
                                             uint32_t frame, uint32_t model, uint32_t shape,
                                             int32_t group, uint32_t set_shaders) {
  ITC_NATIVE_RUNNING("sub_820CECA0 Model::RecordMaterials");
  const auto slots = ModelRecordMaterialsSlots(model, shape, group, set_shaders);
  if (!slots) {
    return std::nullopt;
  }
  if (*slots != 0 && !CanRecord(G::record_list(), *slots)) {
    return std::nullopt;
  }
  // The arguments the original saves in its caller's frame.
  guest::StoreU32(frame + 316, shape);
  guest::StoreU32(frame + 324, uint32_t(group));
  guest::StoreU32(frame + 332, set_shaders);
  if (*slots == 0) {
    return 0;
  }
  return ModelRecordMaterialsChecked(calls, frame, model, shape, group, set_shaders);
}

std::optional<uint32_t> ModelRecordMaterialsSlotCount(uint32_t model,
                                                      uint32_t shape, int32_t group,
                                                      uint32_t set_shaders) {
  return ModelRecordMaterialsSlots(model, shape, group, set_shaders);
}

uint32_t RecordShapeDepthSlotCount(uint32_t model, uint32_t shape, int32_t group,
                                   uint32_t mask, uint32_t with_flagged) {
  return RecordShapeDepthSlots(model, shape, group, mask, with_flagged);
}

bool CommandListHasRoom(uint32_t list, uint32_t slots) {
  return CanRecord(list, slots);
}

namespace {


// The model of the List64 node `steps` from the head (0 or 1).
uint32_t ListModel(uint32_t nodes, int steps) {
  auto node_at = [&](int16_t index) { return uint32_t(int32_t(index) << 2) + nodes; };
  uint32_t node = node_at(int16_t(schema::ModelList(nodes - 4).head()));
  if (steps != 0) {
    node = node_at(int16_t(guest::LoadU16(node)));
  }
  const int32_t slot = int16_t(guest::LoadU16(node + 2));
  return guest::LoadU32(uint32_t((slot + 67) << 2) + nodes);
}

// The identity's four rows at +96 (the vector unit's stores).
void StoreIdentity(uint32_t frame) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreU32(frame + 96 + 16 * row + 4 * i, row == i ? 0x3F800000u : 0u);
    }
  }
}

}  // namespace

std::optional<uint32_t> PointListDraw(const DrawCalls& calls, uint32_t frame,
                                      uint32_t list, uint32_t entry, uint32_t pass,
                                      uint32_t caller_r24, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_82148058 PointList::Draw");
  const schema::ModelList l(list);
  const uint32_t nodes = list + 4;
  const uint32_t points = (entry + 132) << 2;  // the entry's word, from the list
  const int32_t start = l.first_group();
  // Every draw's slots, checked before anything is done: the points drawn
  // (y not below the lowest) times each model's draw.
  {
    const double low = Lfs(G::point_low_address());
    uint64_t slots = 0;
    for (int which = 0; which < 2; which++) {
      const uint32_t model = ListModel(nodes, which);
      const auto draw = SceneryDrawShapeSlots(model, 0, 0, 0, frame + (which ? 176 : 160));
      if (!draw) {
        return std::nullopt;
      }
      const int32_t first = which ? 192 : int32_t(uint32_t(start) << 6);
      const int32_t end = which ? 256 : 192;
      if (which == 0 && start >= 3) {
        continue;
      }
      for (int32_t at = first; at < end; at += 16) {
        const uint32_t point = guest::LoadU32(list + points) + uint32_t(at);
        if (!(Lfs(point + 4) < low)) {
          slots += *draw;
        }
      }
    }
    const uint32_t record_list = G::record_list();
    if (slots > 0xFFFFu || !CanRecord(record_list, uint32_t(slots))) {
      return std::nullopt;
    }
  }
  const double one = Lfs(G::one_address());
  const double zero = Lfs(G::zero_address());
  for (uint32_t offset : {108u, 124u, 140u, 156u}) {
    guest::StoreFloat(frame + offset, float(one));
  }
  for (uint32_t offset : {208u, 212u, 216u}) {
    guest::StoreFloat(frame + offset, float(zero));
  }
  guest::StoreFloat(frame + 220, float(one));
  StoreIdentity(frame);
  const uint32_t view_projection = frame + 240;
  if (!c_runtime::Memcpy(view_projection, pass + 144, 64)) {
    return std::nullopt;
  }
  r3 = view_projection;
  const double far = Lfs(G::point_range_far_address());
  const double near = Lfs(G::point_range_near_address());
  const double turn2 = Lfs(G::point_turn2_address());
  const double low = Lfs(G::point_low_address());
  const double turn = Lfs(G::point_turn_address());
  // One point: its copies and the angle on the stack, then, when its y isn't
  // below the lowest, the turned and moved identity and the model's shape 0.
  // `at` is where the point's x, y, z go (+176 or +160), `range` the range's
  // (+160 or +176).
  auto draw_point = [&](uint32_t model, uint32_t point, uint32_t at, uint32_t range,
                        uint32_t r24) {
    CopyWords(frame + 192, point, 4);
    CopyWords(frame + 224, point, 4);
    CopyFloat(frame + at, frame + 192);
    CopyFloat(frame + at + 8, frame + 200);
    CopyFloat(frame + at + 4, frame + 196);
    const double y = Lfs(frame + 196);
    CopyWords(frame + 208, frame + at, 4);
    guest::StoreFloat(frame + 80, float(Lfs(frame + 236) * turn));
    if (y < low) {
      return;
    }
    StoreIdentity(frame);
    matrix_math::RotateY(frame + 96, frame + 96, frame + 80, frame - 80);
    guest::StoreFloat(frame + 80, float(turn2));
    matrix_math::RotateY(frame + 96, frame + 96, frame + 80, frame - 80);
    car_physics::MatrixMultiplyTranslation(frame + 96, frame + 96, frame + 208,
                                           frame - 192);
    guest::StoreFloat(frame + range + 0, float(near));
    guest::StoreFloat(frame + range + 4, float(far));
    guest::StoreFloat(frame + range + 8, float(zero));
    guest::StoreFloat(frame + range + 12, float(zero));
    guest::StoreU32(frame - kSceneryDrawShapeFrame, frame);  // its back chain
    const auto draw = SceneryDrawShape(calls, frame - kSceneryDrawShapeFrame, model,
                                       view_projection, frame + 96, 0, 0, 0, frame + range, 0,
                                       r24);
    if (!draw) {
      ITC_LOG_ERROR("PointList::Draw: a draw declined after its checks");
      std::abort();
    }
    r3 = draw->r3;
  };
  // The first model at groups +616 to 2.
  const uint32_t first = ListModel(nodes, 0);
  ResetDrawState(first);
  schema::SceneryModel(first).set_texture_provider(l.texture_provider());
  schema::SceneryModel(first).set_animations(l.animations<schema::View>().address);
  uint32_t r24 = caller_r24;
  if (start < 3) {
    guest::StoreFloat(frame + 188, float(one));
    r24 = points;
    uint32_t at = uint32_t(start) << 6;
    do {
      for (int i = 0; i < 4; i++, at += 16) {
        draw_point(first, guest::LoadU32(list + points) + at, 176, 160, r24);
      }
    } while (int32_t(at) < 192);
  }
  // The second model at group 3.
  guest::StoreFloat(frame + 172, float(one));
  const uint32_t second = ListModel(nodes, 1);
  ResetDrawState(second);
  schema::SceneryModel(second).set_texture_provider(l.texture_provider());
  schema::SceneryModel(second).set_animations(l.animations<schema::View>().address);
  for (uint32_t at = 192; at < 256; at += 16) {
    draw_point(second, guest::LoadU32(list + points) + at, 160, 176, r24);
  }
  return r3;
}

}  // namespace scenery_lists
