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

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <bit>

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

}  // namespace scenery_lists
