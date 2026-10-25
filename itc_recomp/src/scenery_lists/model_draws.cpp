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
#include "matrix_math/matrix_math.h"
#include "models/model.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <algorithm>
#include <bit>
#include <tuple>
#include <cstdlib>

namespace scenery_lists {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kOne = 0x82000A30;    // 1.0
constexpr uint32_t kZero = 0x82000A34;   // 0.0
constexpr uint32_t kMinusOne = 0x82001BE0;
constexpr uint32_t kRight = 0x82002B90;  // the visibility test's bounds
constexpr uint32_t kLeft = 0x8206CDC4;
constexpr uint32_t kCameraPosition = 0x8265ECE0;  // 16 bytes
constexpr uint32_t kView = 0x8265ECA0;
constexpr uint32_t kProjectorOrigin = 0x8265ED00;

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
simde__m128 VectorAt(uint8_t* base, uint32_t address) {
  return simde_mm_castsi128_ps(simde_mm_set_epi32(
      int32_t(guest::LoadU32(base, address + 0)), int32_t(guest::LoadU32(base, address + 4)),
      int32_t(guest::LoadU32(base, address + 8)), int32_t(guest::LoadU32(base, address + 12))));
}
uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}
void StoreVector(uint8_t* base, uint32_t address, simde__m128 v) {
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(base, address + 4 * i, WordOf(v, i));
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
double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
// lfs then stfs.
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, float(Lfs(base, from)));
}
void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, to + 4 * i, guest::LoadU32(base, from + 4 * i));
  }
}

// The command list's checks before a recorder: its chunk open or none open
// on any list, and room for `slots`.
bool CanRecord(uint8_t* base, uint32_t list, uint32_t slots) {
  if (guest::LoadU8(base, list + 52) == 0 &&
      guest::LoadU32(base, command_list::kOpenChunks) != 0) {
    return false;
  }
  const uint32_t used = guest::LoadU32(base, list + 12);
  return used + slots >= used && used + slots < guest::LoadU32(base, list + 8);
}

// Command 36 and the chunk closed.
void EndRecord(uint8_t* base, uint32_t list) {
  const uint32_t slot = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, slot + 4, 0);
  guest::StoreU32(base, slot + 0, 36);
  guest::StoreU32(base, slot + 8, 0);
  command_list::CloseChunk(base, list);
}

// The normalize loop: `count` vectors from `from` to `out` (both at their
// 16-byte boundaries), x, y, z (and w) times the reciprocal square root of
// x² + y² + z², refined once; a zero length gives the dot product.
void Normalize(uint8_t* base, uint32_t out, uint32_t from, uint32_t count) {
  FlushToZero ftz;
  const simde__m128 half = simde_mm_mul_ps(
      simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)), simde_mm_castsi128_ps(simde_mm_set1_epi32(0x3F000000)));
  const simde__m128 zero = simde_mm_castsi128_ps(simde_mm_set1_epi32(0));
  for (uint32_t i = 0; i < count; i++) {
    const simde__m128 v = VectorAt(base, (from + 16 * i) & ~0xFu);
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
    StoreVector(base, (out + 16 * i) & ~0xFu, simde_mm_castsi128_ps(result));
  }
}

}  // namespace

uint32_t IsShapeVisible(uint8_t* base, uint32_t model, uint32_t view_projection, uint32_t world,
                        uint32_t shape, uint32_t product) {
  ITC_NATIVE_RUNNING("sub_825951E0 Model::IsShapeVisible");
  const double one = Lfs(base, kOne);
  // The original's w stores before the multiply (overwritten by it).
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    guest::StoreFloat(base, product + offset, float(one));
  }
  matrix_math::Multiply(base, product, world, view_projection);
  interpolation::BlendState_OnMultiply(base, product, world, view_projection,
                                       kVisibleMultiplyReturn);
  // The product's columns, as the translation transposes it.
  simde__m128 v12 = VectorAt(base, product + 32);
  simde__m128 v9 = VectorAt(base, product + 16);
  const double left = Lfs(base, kLeft);
  const double right = Lfs(base, kRight);
  const double minus_one = Lfs(base, kMinusOne);
  simde__m128 v11 = VectorAt(base, product + 0);
  const simde__m128 v0 = Hi(v12, v11);
  const double zero = Lfs(base, kZero);
  v12 = Lo(v12, v11);
  const simde__m128 v10a = VectorAt(base, product + 48);
  v11 = Lo(v10a, v9);
  const simde__m128 v13 = Hi(v10a, v9);
  const simde__m128 c8 = Hi(v11, v12);
  const simde__m128 c10 = Hi(v13, v0);
  const simde__m128 c9 = Lo(v13, v0);
  const simde__m128 c7 = Lo(v11, v12);
  int32_t near = 0, across = 0, up = 0;
  uint32_t corner = guest::LoadU32(base, model + 40) + shape * 544 + 96;
  for (int i = 0; i < 8; i++, corner += 16) {
    simde__m128 point;
    {
      FlushToZero ftz;
      const simde__m128 c = VectorAt(base, corner & ~0xFu);
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

std::optional<uint32_t> RecordObject(uint8_t* base, uint32_t view_projection, uint32_t world) {
  ITC_NATIVE_RUNNING("sub_825953D0 Recorder::RecordObject");
  const uint32_t list = guest::LoadU32(base, kRecordList);
  if (!CanRecord(base, list, 11)) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  const uint32_t header = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, header + 0, 7);
  guest::StoreU32(base, header + 4, 9);
  guest::StoreU32(base, header + 8, 0x00010009);
  const uint32_t constants = command_list::AllocateSlots(base, list, 9);
  matrix_math::Multiply(base, constants, world, view_projection);
  interpolation::BlendState_OnMultiply(base, constants, world, view_projection,
                                       kObjectMultiplyReturn);
  matrix_math::Copy(base, constants + 64, world);
  interpolation::BlendState_OnCopy(base, constants + 64, world, kObjectCopyReturn);
  CopyWords(base, constants + 128, kCameraPosition, 4);
  EndRecord(base, list);
  return header;
}

std::optional<uint32_t> RecordSceneLights(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82595490 Recorder::RecordSceneLights");
  const uint32_t list = guest::LoadU32(base, kRecordList);
  if (!CanRecord(base, list, 65)) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  const uint32_t header = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, header + 0, 7);
  guest::StoreU32(base, header + 4, 63);
  guest::StoreU32(base, header + 8, 0x0101003F);
  const uint32_t constants = command_list::AllocateSlots(base, list, 63);
  matrix_math::Copy(base, constants, kView);
  interpolation::BlendState_OnCopy(base, constants, kView, kLightsCopyReturn);
  CopyWords(base, constants + 64, kCameraPosition, 4);
  SceneLightsRecordDirections(base, constants + 80);
  SceneLightsRecordProjectors(base, constants + 80 + 560);
  EndRecord(base, list);
  return header;
}

void SceneLightsRecordDirections(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8258B980 SceneLights::RecordDirections");
  Normalize(base, out, 0x8296B950, 3);
  CopyWords(base, out + 48, 0x8296B9E0, 32);
  CopyWords(base, out + 176, 0x8296BA60, 32);
  Normalize(base, out + 304, 0x8296BBE0, 8);
  CopyWords(base, out + 432, 0x8296BC60, 32);
}

void SceneLightsRecordProjectors(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82589B58 SceneLights::RecordProjectors");
  for (uint32_t i = 0; i < 3; i++) {
    CopyFloat(base, out + 4 * i, kProjectorOrigin + 4 * i);
  }
  CopyFloat(base, out + 12, kOne);
  struct Run {
    uint32_t to, from, floats;
  };
  static constexpr Run kRuns[] = {
      {16, 0x8296B980, 12}, {64, 0x8296B9B0, 12}, {112, 0x8296BAE0, 32}, {240, 0x8296BB60, 32}};
  for (const Run& run : kRuns) {
    for (uint32_t i = 0; i < run.floats; i++) {
      CopyFloat(base, out + run.to + 4 * i, run.from + 4 * i);
    }
  }
}

namespace {

constexpr uint32_t kDepthConstant = 0x8265ED10;  // pixel c43
constexpr uint32_t kDepthKey = 0x8296B8FC;

// A shape (544 bytes from the model's +40).
uint32_t ShapeAt(uint8_t* base, uint32_t model, uint32_t shape) {
  return shape * 544 + guest::LoadU32(base, model + 40);
}

// Whether RecordShapeDepth records material `material` (its +348 at
// `at`): its +404 shares the mask (when not 0), and its +388 is 0 or the
// byte is set.
bool DepthRecords(uint8_t* base, uint32_t at, uint32_t mask, uint32_t with_flagged) {
  if (mask != 0 && (guest::LoadU32(base, at + 56) & mask) == 0) {
    return false;
  }
  return guest::LoadU32(base, at + 40) == 0 || (with_flagged & 0xFF) != 0;
}

// The first material of the group (as the original works it out).
uint32_t FirstMaterial(uint8_t* base, uint32_t model, uint32_t shape, int32_t group) {
  const uint32_t block = guest::LoadU32(base, ShapeAt(base, model, shape) + 396);
  if (group == 0) {
    return block + 16;
  }
  return (guest::LoadU32(base, block + 8) << 9) + block + 16;
}

// The slots RecordShapeDepth takes (0 when it records nothing).
uint32_t RecordShapeDepthSlots(uint8_t* base, uint32_t model, uint32_t shape, int32_t group,
                               uint32_t mask, uint32_t with_flagged) {
  if (models::MaterialCount(base, model, shape, group) == 0) {
    return 0;
  }
  const uint32_t at = ShapeAt(base, model, shape);
  uint32_t slots = 7;
  if (guest::LoadU32(base, at + 428) == 1 &&
      guest::LoadU32(base, guest::LoadU32(base, at + 408) + 20) != 0) {
    slots += 2;
  }
  uint32_t material = FirstMaterial(base, model, shape, group) + 348;
  for (int32_t i = 0; i < int32_t(models::MaterialCount(base, model, shape, group));
       i++, material += 512) {
    if (DepthRecords(base, material, mask, with_flagged)) {
      slots += 7;
    }
  }
  return slots;
}

void Slot(uint8_t* base, uint32_t slot, uint32_t w0, uint32_t w1, uint32_t w2) {
  guest::StoreU32(base, slot + 0, w0);
  guest::StoreU32(base, slot + 4, w1);
  guest::StoreU32(base, slot + 8, w2);
}

// RecordShapeDepth's recording, its room checked.
uint32_t RecordShapeDepthChecked(uint8_t* base, uint32_t model, uint32_t shape, int32_t group,
                                 uint32_t mask, uint32_t with_flagged) {
  const uint32_t list = guest::LoadU32(base, kRecordList);
  const uint32_t at = ShapeAt(base, model, shape);
  const uint32_t stream =
      guest::LoadU32(base, at + 428) == 1 ? guest::LoadU32(base, at + 412) : at + 320;
  command_list::OpenChunk(base, list);
  const uint32_t header = command_list::AllocateSlots(base, list, 1);
  Slot(base, header, 10, 1, 0);
  const uint32_t declaration = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, declaration,
                  DepthVertexDeclaration(base, int32_t(guest::LoadU32(base, at + 260))));
  Slot(base, command_list::AllocateSlots(base, list, 1), 11, 1, 0);
  uint32_t slot = command_list::AllocateSlots(base, list, 1);
  Slot(base, slot, 1, stream, 0);
  guest::StoreU32(base, slot + 12, guest::LoadU32(base, at + 264));
  Slot(base, command_list::AllocateSlots(base, list, 1), 7, 1, 0x01012B01);
  CopyWords(base, command_list::AllocateSlots(base, list, 1), kDepthConstant, 4);
  if (guest::LoadU32(base, at + 428) == 1) {
    const uint32_t indices = guest::LoadU32(base, at + 408);
    if (guest::LoadU32(base, indices + 20) != 0) {
      Slot(base, command_list::AllocateSlots(base, list, 1), 22, 1, 0);
      slot = command_list::AllocateSlots(base, list, 1);
      guest::StoreU32(base, slot + 8, 1);
      guest::StoreU32(base, slot + 4, guest::LoadU32(base, indices + 20));
    }
  }
  uint32_t material = FirstMaterial(base, model, shape, group) + 348;
  for (int32_t i = 0; i < int32_t(models::MaterialCount(base, model, shape, group));
       i++, material += 512) {
    if (!DepthRecords(base, material, mask, with_flagged)) {
      continue;
    }
    Slot(base, command_list::AllocateSlots(base, list, 1), 1, 2, 0);
    slot = command_list::AllocateSlots(base, list, 1);
    guest::StoreU32(base, slot + 0, 100);
    guest::StoreU32(base, slot + 4, guest::LoadU32(base, material - 236));
    slot = command_list::AllocateSlots(base, list, 1);
    guest::StoreU32(base, slot + 0, 56);
    guest::StoreU32(base, slot + 4, guest::LoadU32(base, material - 4));
    Slot(base, command_list::AllocateSlots(base, list, 1), 12, 1, 0);
    guest::StoreU32(base, command_list::AllocateSlots(base, list, 1), material + 4);
    Slot(base, command_list::AllocateSlots(base, list, 1), 18, 1, 0);
    slot = command_list::AllocateSlots(base, list, 1);
    guest::StoreU16(base, slot + 4, 0);
    guest::StoreU16(base, slot + 6, 0);
    guest::StoreU32(base, slot + 0, guest::LoadU32(base, material));
    guest::StoreU32(base, slot + 8, guest::LoadU32(base, material - 8));
  }
  EndRecord(base, list);
  return header;
}

}  // namespace

std::optional<uint32_t> RecordShapeDepth(uint8_t* base, uint32_t model, uint32_t shape,
                                         int32_t group, uint32_t mask, uint32_t with_flagged) {
  ITC_NATIVE_RUNNING("sub_825960D0 Model::RecordShapeDepth");
  const uint32_t slots = RecordShapeDepthSlots(base, model, shape, group, mask, with_flagged);
  if (slots == 0) {
    return 0;
  }
  if (!CanRecord(base, guest::LoadU32(base, kRecordList), slots)) {
    return std::nullopt;
  }
  return RecordShapeDepthChecked(base, model, shape, group, mask, with_flagged);
}

std::optional<DepthDraw> DrawShapeDepth(uint8_t* base, uint32_t model, uint32_t view_projection,
                                        uint32_t world, uint32_t shape, uint32_t group,
                                        uint32_t mask, uint32_t with_flagged, uint32_t frame,
                                        const std::array<uint32_t, 9>& context) {
  ITC_NATIVE_RUNNING("sub_82596D48 Model::DrawShapeDepth");
  const uint32_t list = guest::LoadU32(base, kRecordList);
  const int32_t which = (group & 0xFF) != 0 ? 1 : 0;
  // Every slot it would take, checked before anything is done.
  if (!CanRecord(base, list,
                 11 + 2 + RecordShapeDepthSlots(base, model, shape, which, mask, with_flagged))) {
    return std::nullopt;
  }
  DepthDraw draw;
  if (guest::LoadU32(base, model + 28) == 0) {
    draw.r3 = model;
    return draw;
  }
  if ((IsShapeVisible(base, model, view_projection, world, shape, frame - 96) & 0xFF) == 0) {
    draw.r3 = 0;
    return draw;
  }
  interpolation::BlendState_BeforeRecordObject(base, view_projection, world);
  interpolation::BlendState_SetRecordContext(context);
  const auto constants = RecordObject(base, view_projection, world);
  interpolation::BlendState_SetRecordContext({});
  interpolation::BlendState_AfterRecordObject(base);
  if (!constants) {
    ITC_LOG_ERROR("Model::DrawShapeDepth: RecordObject declined after its checks");
    std::abort();
  }
  command_list::OpenChunk(base, list);
  const uint32_t entry = command_list::AllocateSlots(base, list, 2);
  command_list::CloseChunk(base, list);
  if (models::MaterialCount(base, model, shape, which) == 0) {
    draw.r3 = list;
    return draw;
  }
  guest::StoreU32(base, entry + 4, *constants);
  guest::StoreU16(base, entry + 2, 3);
  guest::StoreU16(base, entry + 18, 4);
  guest::StoreU32(base, entry + 20,
                  RecordShapeDepthChecked(base, model, shape, which, mask, with_flagged));
  draw.submit = true;
  draw.entry = entry;
  draw.key = guest::LoadU32(base, kDepthKey);
  draw.depth = -Lfs(base, *constants + 72);
  return draw;
}

namespace {

constexpr uint32_t kBlendStates = 0x82008E6C;   // render state 3 by blend index
constexpr uint32_t kRangeAfter = 0x8265ED20;    // vertex c86 put back
constexpr uint32_t kPixelC76After = 0x8265ED30;  // pixel c76 put back
constexpr uint32_t kGroup0Key = 0x8296B90C;
constexpr uint32_t kGroup1Key = 0x8296B900;
constexpr uint32_t kSceneryDepthKey = 0x8296B908;

// SceneryRecordShape's slots, or nullopt where it declines (see the header).
std::optional<uint32_t> SceneryRecordShapeSlots(uint8_t* base, uint32_t model, uint32_t shape,
                                                int32_t group, uint32_t set_shaders,
                                                uint32_t c76, uint32_t range) {
  const int32_t count = int32_t(models::MaterialCount(base, model, shape, group));
  if (count == 0) {
    return 0;
  }
  const uint32_t at = ShapeAt(base, model, shape);
  if (!models::VertexDeclaration(base, guest::LoadU32(base, at + 244),
                                 int32_t(guest::LoadU32(base, at + 260)),
                                 guest::LoadU32(base, at + 276))) {
    return std::nullopt;
  }
  const uint32_t type = guest::LoadU32(base, at + 260);
  const bool short_streams = type == 9 || type == 10 || guest::LoadU32(base, at + 276) == 6;
  uint32_t slots = 2 + (short_streams ? 3 : 4) + 1 + 4 + 1 + 12 + 1 + 8 + 1;
  if (guest::LoadU32(base, at + 428) == 1 &&
      guest::LoadU32(base, guest::LoadU32(base, at + 408) + 20) != 0) {
    slots += 2;
  }
  uint32_t material = models::Material(base, model, shape, 0, group);
  for (int32_t i = 0; i < count; i++, material += 512) {
    const int32_t textures = int32_t(guest::LoadU32(base, material + 128));
    if (textures > 4 || int32_t(guest::LoadU32(base, material + 396)) != -1) {
      return std::nullopt;
    }
    slots += (set_shaders != 0 ? 4 : 0) + 6 + 6 + 2 + (group == 0 ? 3 : 0) + 2 + 4;
    if (guest::LoadU32(base, model + 48) != 0 && textures != 0) {
      slots += 2 + 4 * uint32_t(std::max(textures, 0));
    } else {
      slots += 5;
    }
    const uint32_t flags = guest::LoadU32(base, material + 104);
    slots += ((flags & 2) != 0 ? 2 : 0) + ((flags & 4) != 0 ? 2 : 0);
    slots += (range != 0 ? 4 : 0) + (c76 != 0 ? 4 : 0);
  }
  return slots;
}

// A slot, which the checks before made sure of.
uint32_t Take(uint8_t* base, uint32_t list, uint32_t count = 1) {
  const uint32_t slot = command_list::AllocateSlots(base, list, count);
  if (slot == 0) {
    ITC_LOG_ERROR("scenery_lists: the command list refused a slot after its checks");
    std::abort();
  }
  return slot;
}

uint32_t RecordShapeChecked(uint8_t* base, const DrawCalls& calls, uint32_t frame,
                            uint32_t model, uint32_t shape, int32_t group, uint32_t set_shaders,
                            uint32_t c76, uint32_t range, uint32_t pair_offset) {
  const uint32_t list = guest::LoadU32(base, kRecordList);
  const uint32_t at = ShapeAt(base, model, shape);
  uint32_t stream = at + 320;
  uint32_t second = at + 352;
  if (guest::LoadU32(base, at + 428) == 1) {
    stream = guest::LoadU32(base, at + 412);
    second = guest::LoadU32(base, at + 420);
  }
  command_list::OpenChunk(base, list);
  const uint32_t header = Take(base, list);
  guest::StoreU32(base, frame + 84, header);
  Slot(base, header, 10, 1, 0);
  const uint32_t declaration = Take(base, list);
  guest::StoreU32(base, declaration,
                  *models::VertexDeclaration(base, guest::LoadU32(base, at + 244),
                                             int32_t(guest::LoadU32(base, at + 260)),
                                             guest::LoadU32(base, at + 276)));
  const uint32_t type = guest::LoadU32(base, at + 260);
  uint32_t slot;
  if (type != 9 && type != 10 && guest::LoadU32(base, at + 276) != 6) {
    Slot(base, Take(base, list), 11, 3, 0);
    slot = Take(base, list);
    Slot(base, slot, 0, at + 288, 0);
    guest::StoreU32(base, slot + 12, guest::LoadU32(base, at + 248));
    slot = Take(base, list);
    Slot(base, slot, 1, stream, 0);
    guest::StoreU32(base, slot + 12, guest::LoadU32(base, at + 264));
    slot = Take(base, list);
    Slot(base, slot, 2, second, 0);
    guest::StoreU32(base, slot + 12, guest::LoadU32(base, at + 280));
  } else {
    Slot(base, Take(base, list), 11, 2, 0);
    slot = Take(base, list);
    Slot(base, slot, 0, at + 288, 0);
    guest::StoreU32(base, slot + 12, guest::LoadU32(base, at + 248));
    slot = Take(base, list);
    Slot(base, slot, 1, stream, 0);
    guest::StoreU32(base, slot + 12, guest::LoadU32(base, at + 264));
  }
  if (guest::LoadU32(base, at + 428) == 1) {
    const uint32_t indices = guest::LoadU32(base, at + 408);
    if (guest::LoadU32(base, indices + 20) != 0) {
      Slot(base, Take(base, list), 22, 1, 0);
      slot = Take(base, list);
      guest::StoreU32(base, slot + 8, 1);
      guest::StoreU32(base, slot + 4, guest::LoadU32(base, indices + 20));
    }
  }
  Slot(base, Take(base, list), 2, 4, 0);
  for (uint32_t k = 0; k < 4; k++) {
    Slot(base, Take(base, list), k, 28, guest::LoadU32(base, at + 432));
  }
  uint32_t material = models::Material(base, model, shape, 0, group);
  Slot(base, Take(base, list), 2, 12, 0);
  for (uint32_t k = 0; k < 4; k++) {
    Slot(base, Take(base, list), k, 16, 4);
    Slot(base, Take(base, list), k, 20, 4);
    Slot(base, Take(base, list), k, 36, 3);
  }
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  if (int32_t(models::MaterialCount(base, model, shape, group)) > 0) {
    guest::StoreFloat(base, frame + 116, float(one));
    guest::StoreFloat(base, frame + 120, float(zero));
    guest::StoreFloat(base, frame + 124, float(zero));
    for (int32_t i = 0; i < int32_t(models::MaterialCount(base, model, shape, group));
         i++, material += 512) {
      for (uint32_t offset = 96; offset < 112; offset += 4) {
        guest::StoreFloat(base, frame + offset, float(one));
      }
      guest::StoreU32(base, frame + 80, guest::LoadU32(base, material + 108));
      // The shaders: the model's own pair, or the shared one.
      if (set_shaders != 0) {
        const uint32_t pair = guest::LoadU32(base, material + 392);
        if (pair == 0) {
          Slot(base, Take(base, list), 8, 1, 0);
          guest::StoreU32(base, Take(base, list),
                          guest::LoadU32(base, (guest::LoadU32(base, material + 96) << 4) +
                                                   guest::LoadU32(base, model + 24)));
          Slot(base, Take(base, list), 9, 1, 0);
          guest::StoreU32(base, Take(base, list),
                          guest::LoadU32(base, (guest::LoadU32(base, material + 96) << 4) +
                                                   guest::LoadU32(base, model + 24) + 4));
        } else {
          Slot(base, Take(base, list), 8, 1, 0);
          slot = Take(base, list);
          guest::StoreU32(base, slot,
                          models::SharedVertexShader(
                              base, guest::LoadU32(base, material + 392) + pair_offset));
          Slot(base, Take(base, list), 9, 1, 0);
          slot = Take(base, list);
          guest::StoreU32(base, slot,
                          models::SharedPixelShader(
                              base, guest::LoadU32(base, material + 392) + pair_offset));
        }
      }
      // Vertex c9-c13: the textures' colors, then the material's +0.
      Slot(base, Take(base, list), 7, 5, 0x00010905);
      const uint32_t vertex = Take(base, list, 5);
      for (int32_t j = 0; j < int32_t(guest::LoadU32(base, material + 128)); j++) {
        const uint32_t out = vertex + 16 * uint32_t(j);
        const int32_t index = int32_t(guest::LoadU32(base, material + 208 + 4 * uint32_t(j)));
        double third;
        if (index != -1) {
          const uint32_t color = (uint32_t(index) << 4);
          CopyFloat(base, out + 0, color + guest::LoadU32(base, model + 320));
          CopyFloat(base, out + 4, color + guest::LoadU32(base, model + 320) + 4);
          third = Lfs(base, color + guest::LoadU32(base, model + 320) + 8);
        } else {
          guest::StoreFloat(base, out + 0, float(zero));
          guest::StoreFloat(base, out + 4, float(zero));
          guest::StoreFloat(base, out + 8, float(zero));
          guest::StoreFloat(base, out + 12, float(one));
          third = Lfs(base, material + 240 + 4 * uint32_t(j));
        }
        guest::StoreFloat(base, frame + 96 + 4 * uint32_t(j), float(third));
      }
      CopyWords(base, vertex + 64, material, 4);
      // The model's per-draw hook (slot 1).
      {
        const uint32_t args[5] = {model, shape, uint32_t(i), vertex + 64, frame + 80};
        calls.call(calls.user,
                   guest::LoadU32(base, guest::LoadU32(base, model) + 4), args, 5, frame,
                   0x82147170);
      }
      // Pixel c63-c67.
      Slot(base, Take(base, list), 7, 5, 0x01013F05);
      const uint32_t pixel = Take(base, list, 5);
      const uint32_t colors_low = guest::LoadU32(base, frame + 96);
      const uint32_t colors_low2 = guest::LoadU32(base, frame + 100);
      const uint32_t colors_high = guest::LoadU32(base, frame + 104);
      const uint32_t colors_high2 = guest::LoadU32(base, frame + 108);
      CopyWords(base, pixel + 0, material + 64, 4);
      CopyFloat(base, frame + 112, material + 80);
      CopyWords(base, pixel + 16, frame + 112, 4);
      CopyWords(base, pixel + 32, material + 16, 4);
      CopyFloat(base, pixel + 44, material + 88);
      CopyWords(base, pixel + 48, material + 32, 4);
      guest::StoreU32(base, pixel + 64, colors_low);
      guest::StoreU32(base, pixel + 68, colors_low2);
      guest::StoreU32(base, pixel + 72, colors_high);
      guest::StoreU32(base, pixel + 76, colors_high2);
      // The blend (render state 3) and, for group 0, 96 and 100.
      Slot(base, Take(base, list), 3, 1, 0);
      slot = Take(base, list);
      guest::StoreU32(base, slot + 0, 0);
      guest::StoreU32(base, slot + 4,
                      guest::LoadU32(base, (guest::LoadU32(base, frame + 80) << 2) + kBlendStates));
      if (group == 0) {
        if (guest::LoadU32(base, material + 388) != 0) {
          Slot(base, Take(base, list), 1, 2, 0);
          slot = Take(base, list);
          guest::StoreU32(base, slot + 4, 1);
          guest::StoreU32(base, slot + 0, 96);
          slot = Take(base, list);
          guest::StoreU32(base, slot + 0, 100);
          guest::StoreU32(base, slot + 4, 64);
        } else {
          Slot(base, Take(base, list), 1, 2, 0);
          slot = Take(base, list);
          guest::StoreU32(base, slot + 4, 0);
          guest::StoreU32(base, slot + 0, 96);
          slot = Take(base, list);
          guest::StoreU32(base, slot + 0, 100);
          guest::StoreU32(base, slot + 4, guest::LoadU32(base, material + 112));
        }
      }
      Slot(base, Take(base, list), 1, 1, 0);
      slot = Take(base, list);
      guest::StoreU32(base, slot + 0, 56);
      guest::StoreU32(base, slot + 4, guest::LoadU32(base, material + 344));
      // The textures and their sampler states.
      const uint32_t provider = guest::LoadU32(base, model + 48);
      if (provider != 0 && guest::LoadU32(base, material + 128) != 0) {
        slot = Take(base, list);
        guest::StoreU32(base, slot + 0, 4);
        guest::StoreU32(base, slot + 8, 0);
        guest::StoreU32(base, slot + 4, guest::LoadU32(base, material + 128));
        for (int32_t k = 0; k < int32_t(guest::LoadU32(base, material + 128)); k++) {
          const uint32_t id = material + 144 + 4 * uint32_t(k);
          const uint32_t set = guest::LoadU32(base, model + 436);
          uint32_t texture = 0;
          bool lookup = true;
          uint32_t first = guest::LoadU32(base, id);
          uint32_t second_word = guest::LoadU32(base, id + 32);
          if (set != 0 && guest::LoadU32(base, id) == 0) {
            const uint32_t entry = AnimationCurrentFrame(base, set, guest::LoadU32(base, id + 32));
            if (entry == 0) {
              lookup = false;
            } else {
              second_word = guest::LoadU32(base, entry + 4);
              first = guest::LoadU32(base, entry + 0);
            }
          }
          if (lookup) {
            const uint32_t from = guest::LoadU32(base, model + 48);
            const uint32_t args[3] = {from, first, second_word};
            texture = calls.call(calls.user,
                                 guest::LoadU32(base, guest::LoadU32(base, from) + 20), args, 3,
                                 frame, 0x821473D0);
          }
          slot = Take(base, list);
          guest::StoreU32(base, slot + 4, texture);
          guest::StoreU32(base, slot + 0, uint32_t(k));
        }
        slot = Take(base, list);
        guest::StoreU32(base, slot + 0, 2);
        guest::StoreU32(base, slot + 8, 0);
        guest::StoreU32(base, slot + 4, guest::LoadU32(base, material + 128) * 3);
        for (int32_t k = 0; k < int32_t(guest::LoadU32(base, material + 128)); k++) {
          const uint32_t sampler = uint32_t(k);
          slot = Take(base, list);
          guest::StoreU32(base, slot + 0, sampler);
          guest::StoreU32(base, slot + 4, 0);
          guest::StoreU32(base, slot + 8, guest::LoadU32(base, material + 272 + 4 * sampler));
          slot = Take(base, list);
          guest::StoreU32(base, slot + 0, sampler);
          guest::StoreU32(base, slot + 4, 4);
          guest::StoreU32(base, slot + 8, guest::LoadU32(base, material + 304 + 4 * sampler));
          slot = Take(base, list);
          guest::StoreU32(base, slot + 0, sampler);
          guest::StoreU32(base, slot + 4, 24);
          guest::StoreU32(base, slot + 8, guest::LoadU32(base, model + 432));
        }
      } else {
        Slot(base, Take(base, list), 4, 4, 0);
        for (uint32_t k = 0; k < 4; k++) {
          slot = Take(base, list);
          guest::StoreU32(base, slot + 0, k);
          guest::StoreU32(base, slot + 4, 0);
        }
      }
      // The environment textures.
      for (const auto& [bit, function_slot, lr] :
           {std::tuple<uint32_t, uint32_t, uint32_t>{2, 24, 0x82147534},
            std::tuple<uint32_t, uint32_t, uint32_t>{4, 20, 0x82147590}}) {
        if ((guest::LoadU32(base, material + 104) & bit) == 0) {
          continue;
        }
        const uint32_t from = guest::LoadU32(base, model + 48);
        const uint32_t args[3] = {from, guest::LoadU32(base, material + 124),
                                  guest::LoadU32(base, material + 120)};
        const uint32_t texture = calls.call(
            calls.user, guest::LoadU32(base, guest::LoadU32(base, from) + function_slot), args,
            3, frame, lr);
        Slot(base, Take(base, list), 4, 1, 0);
        slot = Take(base, list);
        guest::StoreU32(base, slot + 4, texture);
        guest::StoreU32(base, slot + 0, 6);
      }
      // Vertex c86 from the range and pixel c76.
      if (range != 0) {
        Slot(base, Take(base, list), 7, 1, 0x00015601);
        slot = Take(base, list);
        const double near = Lfs(base, range);
        const double far = Lfs(base, range + 4);
        const double span = double(float(far - near));
        guest::StoreFloat(base, slot + 0, float(near));
        guest::StoreFloat(base, slot + 12, float(zero));
        guest::StoreFloat(base, slot + 4, float(far));
        guest::StoreFloat(base, slot + 8, float(double(float(one / span))));
      }
      if (c76 != 0) {
        Slot(base, Take(base, list), 7, 1, 0x01014C01);
        slot = Take(base, list);
        for (uint32_t w = 0; w < 4; w++) {
          CopyFloat(base, slot + 4 * w, c76 + 4 * w);
        }
      }
      // The draw.
      Slot(base, Take(base, list), 12, 1, 0);
      guest::StoreU32(base, Take(base, list), material + 352);
      Slot(base, Take(base, list), 18, 1, 0);
      slot = Take(base, list);
      guest::StoreU16(base, slot + 4, 0);
      guest::StoreU16(base, slot + 6, 0);
      guest::StoreU32(base, slot + 0, guest::LoadU32(base, material + 348));
      guest::StoreU32(base, slot + 8, guest::LoadU32(base, material + 340));
      // The constants put back.
      if (range != 0) {
        Slot(base, Take(base, list), 7, 1, 0x00015601);
        slot = Take(base, list);
        for (uint32_t w = 0; w < 4; w++) {
          CopyFloat(base, slot + 4 * w, kRangeAfter + 4 * w);
        }
      }
      if (c76 != 0) {
        Slot(base, Take(base, list), 7, 1, 0x01014C01);
        slot = Take(base, list);
        for (uint32_t w = 0; w < 4; w++) {
          CopyFloat(base, slot + 4 * w, kPixelC76After + 4 * w);
        }
      }
    }
  }
  Slot(base, Take(base, list), 2, 8, 0);
  for (uint32_t k = 0; k < 4; k++) {
    slot = Take(base, list);
    guest::StoreU32(base, slot + 0, k);
    guest::StoreU32(base, slot + 8, 1);
    guest::StoreU32(base, slot + 4, 16);
    slot = Take(base, list);
    guest::StoreU32(base, slot + 8, 1);
    guest::StoreU32(base, slot + 0, k);
    guest::StoreU32(base, slot + 4, 20);
  }
  EndRecord(base, list);
  return header;
}

}  // namespace

std::optional<uint32_t> SceneryRecordShape(uint8_t* base, const DrawCalls& calls, uint32_t frame,
                                           uint32_t model, uint32_t shape, int32_t group,
                                           uint32_t set_shaders, uint32_t c76, uint32_t range,
                                           uint32_t pair_offset) {
  ITC_NATIVE_RUNNING("sub_82146BD0 SceneryModel::RecordShape");
  const auto slots =
      SceneryRecordShapeSlots(base, model, shape, group, set_shaders, c76, range);
  if (!slots) {
    return std::nullopt;
  }
  if (*slots == 0) {
    return 0;
  }
  if (!CanRecord(base, guest::LoadU32(base, kRecordList), *slots)) {
    return std::nullopt;
  }
  return RecordShapeChecked(base, calls, frame, model, shape, group, set_shaders, c76, range,
                            pair_offset);
}

std::optional<SceneryDraw> SceneryDrawShape(uint8_t* base, const DrawCalls& calls,
                                            uint32_t frame, uint32_t model,
                                            uint32_t view_projection, uint32_t world,
                                            uint32_t shape, uint32_t depth_too, uint32_t c76,
                                            uint32_t range, uint32_t pair_offset,
                                            uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_82147D88 SceneryModel::DrawShape");
  const uint32_t list = guest::LoadU32(base, kRecordList);
  // Every slot it would take, checked before anything is done.
  const auto group0 = SceneryRecordShapeSlots(base, model, shape, 0, 1, c76, range);
  const auto group1 = SceneryRecordShapeSlots(base, model, shape, 1, 1, c76, range);
  if (!group0 || !group1) {
    return std::nullopt;
  }
  uint32_t slots = 6 + 11 + 65 + *group0 + *group1;
  if (int32_t(depth_too) != 0) {
    slots += 2 + RecordShapeDepthSlots(base, model, shape, 0, 0, 0);
  }
  if (!CanRecord(base, list, slots)) {
    return std::nullopt;
  }
  SceneryDraw draw;
  if (guest::LoadU32(base, model + 28) == 0) {
    draw.r3 = model;
    return draw;
  }
  if ((IsShapeVisible(base, model, view_projection, world, shape, frame - 96) & 0xFF) == 0) {
    draw.r3 = 0;
    return draw;
  }
  command_list::OpenChunk(base, list);
  const uint32_t entry0 = Take(base, list, 3);
  const uint32_t entry1 = Take(base, list, 3);
  command_list::CloseChunk(base, list);
  interpolation::BlendState_BeforeRecordObject(base, view_projection, world);
  interpolation::BlendState_SetRecordContext({0x82147E10, caller_r24, world, 0x82960000u, shape,
                                              view_projection, entry1, entry0, model});
  const auto constants = RecordObject(base, view_projection, world);
  interpolation::BlendState_SetRecordContext({});
  interpolation::BlendState_AfterRecordObject(base);
  const auto lights = RecordSceneLights(base);
  if (!constants || !lights) {
    ITC_LOG_ERROR("SceneryModel::DrawShape: a recorder declined after its checks");
    std::abort();
  }
  const uint32_t record_frame = frame - kRecordShapeFrame;
  guest::StoreU32(base, record_frame, frame);  // its back chain, as stwu
  struct Group {
    int32_t group;
    uint32_t entry;
    uint32_t key;
    bool negate;
    uint32_t lr;
  };
  const Group groups[2] = {{0, entry0, kGroup0Key, true, 0x82147EA8},
                           {1, entry1, kGroup1Key, false, 0x82147F1C}};
  for (const Group& g : groups) {
    if (models::MaterialCount(base, model, shape, g.group) == 0) {
      draw.queued = false;
      draw.r3 = 0;
      continue;
    }
    guest::StoreU16(base, g.entry + 2, 3);
    guest::StoreU32(base, g.entry + 4, *constants);
    guest::StoreU16(base, g.entry + 18, 3);
    guest::StoreU32(base, g.entry + 20, *lights);
    guest::StoreU16(base, g.entry + 34, 4);
    guest::StoreU32(base, g.entry + 36,
                    RecordShapeChecked(base, calls, record_frame, model, shape, g.group, 1, c76,
                                       range, pair_offset));
    const double depth = Lfs(base, *constants + 72);
    draw.r3 = calls.submit(calls.user, g.entry, guest::LoadU32(base, g.key),
                           g.negate ? -depth : depth, frame, g.lr);
    draw.queued = true;
  }
  if (int32_t(depth_too) == 0) {
    return draw;
  }
  command_list::OpenChunk(base, list);
  const uint32_t entry = Take(base, list, 2);
  command_list::CloseChunk(base, list);
  if (models::MaterialCount(base, model, shape, 0) == 0) {
    draw.queued = false;
    draw.r3 = 0;
    return draw;
  }
  guest::StoreU16(base, entry + 2, 3);
  guest::StoreU32(base, entry + 4, *constants);
  guest::StoreU16(base, entry + 18, 4);
  guest::StoreU32(base, entry + 20, RecordShapeDepthChecked(base, model, shape, 0, 0, 0));
  draw.r3 = calls.submit(calls.user, entry, guest::LoadU32(base, kSceneryDepthKey),
                         -Lfs(base, *constants + 72), frame, 0x82147FB0);
  draw.queued = true;
  return draw;
}

}  // namespace scenery_lists
