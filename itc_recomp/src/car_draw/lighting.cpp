// The car draw's lighting as plain C++ (docs/functions/car-draw.md:
// Matrix::Shadow and CarLighting::Gather); see car_draw.h. Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt): the vector
// unit's work as the translation does it (SSE vectors with word 0 in the top
// lane, flush-to-zero on), the scalar work as its doubles rounded to single.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "debug/frame_state_diff.h"
#include "guest.h"
#include "hooks/lights.h"
#include "schema/car_draw.h"
#include "schema/car_scene_draw.h"
#include "schema/course_scenery.h"
#include "schema/scenery_lists.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>
#include <cstdlib>

namespace car_draw {

namespace {

using Fpscr = ::base::FloatMode;

using G = schema::CarDrawGlobals;

constexpr uint32_t kOne = 0x82000A30;           // 1.0
constexpr uint32_t kZero = 0x82000A34;          // 0.0
constexpr uint32_t kMergeDistance = 0x820021F8;  // also c43's base
constexpr uint32_t kColorScale = 0x82002200;
constexpr uint32_t kFarthest = 0x82002814;
constexpr uint32_t kConeScale = 0x82002A3C;
constexpr uint32_t kLightPositions = schema::CarSceneDrawGlobals::headlight_positions_address();
constexpr uint32_t kLightAmbient = schema::CarSceneDrawGlobals::headlight_shapes_address();
constexpr uint32_t kLightColorsA = schema::CarSceneDrawGlobals::headlight_colors_a_address();
constexpr uint32_t kLightColorsB = schema::CarSceneDrawGlobals::headlight_colors_b_address();
constexpr uint32_t kLightDirections = schema::CarSceneDrawGlobals::headlight_directions_address();
constexpr uint32_t kLightBlock240 = schema::CarSceneDrawGlobals::headlight_vectors_address();
constexpr uint32_t kShadowWeights = schema::SceneryListsGlobals::depth_constant_address();  // pixel c43 of the depth records

// Flush-to-zero on while alive (the vector unit's mode), put back after.
class FlushToZero {
 public:
  FlushToZero() : csr_(Fpscr::getcsr()) { Fpscr::setcsr(csr_ | uint32_t(Fpscr::FlushMask)); }
  ~FlushToZero() { Fpscr::setcsr(csr_); }
  FlushToZero(const FlushToZero&) = delete;
  FlushToZero& operator=(const FlushToZero&) = delete;

 private:
  uint32_t csr_;
};

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// frsp.
double Single(double value) {
  return double(float(value));
}
// fneg (the sign bit flipped).
double Neg(double value) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(value) ^ 0x8000000000000000ull);
}

void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, to + 4 * i, guest::LoadU32(base, from + 4 * i));
  }
}

// A 16-byte guest vector (lvx: its aligned address) with word 0 in the top
// lane, and back (stvx).
simde__m128 VectorAt(uint8_t* base, uint32_t address) {
  address &= ~0xFu;
  return simde_mm_castsi128_ps(simde_mm_set_epi32(
      int32_t(guest::LoadU32(base, address + 0)), int32_t(guest::LoadU32(base, address + 4)),
      int32_t(guest::LoadU32(base, address + 8)), int32_t(guest::LoadU32(base, address + 12))));
}
void StoreVector(uint8_t* base, uint32_t address, simde__m128 v) {
  address &= ~0xFu;
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(base, address + 4 * i, lanes[3 - i]);
  }
}

// Host words (E5 step 6: the frame's words only these functions read): a
// float's bits as stfs stores them, read back as lfs does, and a vector's
// words in guest order (word 0 the top lane, as lvx and stvx take them).
uint32_t Bits(double value) {
  return std::bit_cast<uint32_t>(float(value));
}
double FloatOf(uint32_t word) {
  return double(std::bit_cast<float>(word));
}
simde__m128 VectorOf(const uint32_t words[4]) {
  return simde_mm_castsi128_ps(simde_mm_set_epi32(int32_t(words[0]), int32_t(words[1]),
                                                  int32_t(words[2]), int32_t(words[3])));
}
void WordsOf(simde__m128 v, uint32_t words[4]) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  for (int i = 0; i < 4; i++) {
    words[i] = lanes[3 - i];
  }
}
// Four host words stored at `to` in order (as CopyWords from the frame).
void StoreWords(uint8_t* base, uint32_t to, const uint32_t words[4]) {
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, to + 4 * i, words[i]);
  }
}
void LoadWords(uint8_t* base, uint32_t from, uint32_t words[4]) {
  for (uint32_t i = 0; i < 4; i++) {
    words[i] = guest::LoadU32(base, from + 4 * i);
  }
}

// The translation's 0.5 (vcfsx of 1 by 1), with flush-to-zero on.
simde__m128 Half() {
  return simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(0x3F000000)));
}

// The normalize both originals inline (vmsum3fp128, vrsqrtefp, one Newton
// step; a zero length gives the dot product), flush-to-zero on: `v` x, y, z
// (and w) times the refined reciprocal square root of x² + y² + z².
simde__m128 Normalize(simde__m128 v, simde__m128 half) {
  const simde__m128 zero = simde_mm_castsi128_ps(simde_mm_set1_epi32(0));
  const simde__m128 dot = simde_mm_dp_ps(v, v, 0xEF);
  simde__m128 estimate = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(dot));
  const simde__m128 halved = simde_mm_mul_ps(dot, half);
  const simde__m128 at_zero = simde_mm_cmpeq_ps(dot, zero);
  const simde__m128 squared = simde_mm_mul_ps(estimate, estimate);
  const simde__m128 step =
      simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(halved, squared), half),
                      simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
  estimate = simde_mm_add_ps(simde_mm_mul_ps(estimate, step), estimate);
  const simde__m128 scaled = simde_mm_mul_ps(v, estimate);
  return simde_mm_castsi128_ps(simde_mm_or_si128(
      simde_mm_andnot_si128(simde_mm_castps_si128(at_zero), simde_mm_castps_si128(scaled)),
      simde_mm_and_si128(simde_mm_castps_si128(at_zero), simde_mm_castps_si128(dot))));
}

}  // namespace

// --- Matrix::Shadow ---

void MatrixShadow(uint8_t* base, uint32_t out, uint32_t light, uint32_t point, uint32_t normal,
                  uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597FE0 Matrix::Shadow");
  // The normalized normal, which the original stored at `stack` - 16 (below
  // its caller's frame: a leaf's) and read back: host words (E5 step 6).
  // Off a 16-byte boundary (never: the stacks and frames are aligned) the
  // original's stvx and lfs reach different words, so the stack is used.
  const uint32_t n = stack - 16;
  const bool aligned = (n & 0xFu) == 0;
  // The loads before the normalized normal's store, in the original's order.
  const schema::ShadowVector p(base, point), l(base, light);
  const double py = double(p.y());
  const double lx = double(l.x());
  const double pz = double(p.z());
  const double lz = double(l.z());
  const double px = double(p.x());
  const double ly = double(l.y());
  const double lw = double(l.w());
  uint32_t normalized[4];
  {
    FlushToZero ftz;
    const simde__m128 v = Normalize(VectorAt(base, normal), Half());
    if (aligned) {
      WordsOf(v, normalized);
    } else {
      StoreVector(base, n, v);
    }
  }
  if (!aligned) {
    LoadWords(base, n, normalized);
  }
  const double ny = FloatOf(normalized[1]);
  double f12 = Single(py * ny);
  const double nx = FloatOf(normalized[0]);
  double f9 = Single(lx * nx);
  const double nz = FloatOf(normalized[2]);
  const double f7 = Single(lz * nz);
  f12 = Single(std::fma(pz, nz, f12));
  double f10 = Single(std::fma(ly, ny, f7));
  const double d = Single(Neg(std::fma(px, nx, f12)));
  f10 = Single(std::fma(lw, d, f10));
  const double dot = Single(f10 + f9);
  f9 = Single(dot - f9);
  Stfs(base, out + 0, f9);
  // Each row: -L_j n_i, but the diagonal's dot - L_i n_i (fnmsubs), the
  // light's words read again for each.
  const double row[4] = {nx, ny, nz, d};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == 0 && j == 0) {
        continue;
      }
      const double l = Lfs(base, light + 4 * j);
      const double value = i == j ? Single(Neg(std::fma(l, row[i], Neg(dot))))
                                  : Neg(Single(l * row[i]));
      Stfs(base, out + 16 * i + 4 * j, value);
    }
  }
}

// --- CarLighting::Gather ---

uint64_t CarLightingGather(uint8_t* base, const Calls& calls, uint32_t stack, uint64_t out_r3,
                           uint32_t world, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_820C9170 CarLighting::Gather");
  const uint32_t out = uint32_t(out_r3);
  const uint32_t frame = stack - kGatherFrame;
  guest::StoreU32(base, frame, stack);  // the back chain, as stwu
  const schema::CarScene scene_at_entry = G::scene(base);
  // The frame stays for the words handed to other code by address (the
  // point +112, the direction +144, the normal +176, the cone vectors +256
  // and +272, the lights' list +288; FindNearest's original and
  // Matrix::Shadow run from it). The words only this function read are
  // host words (E5 step 6): the scene's vectors (+224, +240), the light's
  // direction (+96), the colors (+192, +208), the shadow weights (+160);
  // the count's and index's spills (+80, +128) and r3 and r5 saved in the
  // caller's frame (+20, +36), which nothing read back, are left out.
  const double f29 = Lfs(base, kZero);
  const double f31 = Lfs(base, kOne);
  const double f24 = double(scene_at_entry.lighting_end());
  Stfs(base, frame + 144, f29);
  Stfs(base, frame + 148, f31);
  Stfs(base, frame + 152, f29);
  Stfs(base, frame + 156, f29);
  // The scene's lighting vectors (its lighting block's +96, +112, +240,
  // +256) into the frame.
  uint32_t vector_a[4], vector_b[4];  // the frame's +240 and +224
  LoadWords(base, scene_at_entry.gather_vector_a_lo_address(), vector_a);
  LoadWords(base, scene_at_entry.gather_vector_b_lo_address(), vector_b);
  CopyWords(base, frame + 256, scene_at_entry.cone_a_lo_address(), 4);
  CopyWords(base, frame + 272, scene_at_entry.cone_b_lo_address(), 4);
  for (uint32_t i = 0; i < 10; i++) {
    guest::StoreU32(base, out + 4 * i, 0);
  }
  // The point (0, 0, 0, 1) and the lights' list's w words.
  Stfs(base, frame + 112, f29);
  Stfs(base, frame + 116, f29);
  Stfs(base, frame + 120, f29);
  Stfs(base, frame + 124, f31);
  for (uint32_t i = 0; i < 10; i++) {
    Stfs(base, frame + 300 + 32 * i, f31);
    Stfs(base, frame + 316 + 32 * i, f31);
  }
  double f27 = f29, f26 = f29, f25 = f29;
  const double f28 = Lfs(base, kMergeDistance);
  uint32_t weights[4] = {Bits(f28), 0, 0, 0};  // the frame's +160
  debug::Vec4Transform(base, frame + 112, world, frame + 112);
  {
    const double cone = Single(double(G::scene(base).cone_angle()) * Lfs(base, kConeScale));
    const double least = Single(c_runtime::Cos(cone));
    const uint32_t owner = schema::CourseSceneryGlobals::created(base).address;
    const double farthest = Lfs(base, kFarthest);
    uint64_t r3;
    if (const auto found = lights::FindNearest(base, owner, frame + 288, frame + 112, 10,
                                               frame + 144, f28, farthest, least)) {
      r3 = *found;
    } else {
      // r3 to r9 as the original's call sets them (r7 and r8 its copies'
      // addresses).
      const uint64_t regs[7] = {owner,       frame + 288, frame + 112, 10,
                                frame + 272, frame + 256, frame + 144};
      r3 = calls.find_nearest(regs, f28, farthest, least, frame, 0x820C92F8);
    }
    uint32_t count = uint32_t(r3);
    if (count != 0) {
      schema::CarScene scene = G::scene(base);
      simde__m128 half;
      {
        FlushToZero ftz;
        half = Half();
      }
      const double f30 = Lfs(base, kColorScale);
      uint32_t at = frame + 312;
      uint32_t next = 0;
      do {
        const uint32_t entry = at - 24;
        uint32_t direction[4];  // the frame's +96
        direction[1] = Bits(Single(Lfs(base, at - 20) - Lfs(base, frame + 116)));
        direction[2] = Bits(Single(Lfs(base, at - 16) - Lfs(base, frame + 120)));
        direction[0] = Bits(Single(Lfs(base, entry) - Lfs(base, frame + 112)));
        direction[3] = Bits(f31);
        Stfs(base, at - 12, f31);
        const double angle = double(scene.cone_angle());
        {
          FlushToZero ftz;
          WordsOf(Normalize(VectorOf(direction), half), direction);
        }
        const uint32_t i = next;
        const double cosine = Single(c_runtime::Cos(angle));
        next++;
        if (int32_t(flag) != 0) {
          const double y = FloatOf(direction[1]);
          if (y > cosine) {
            const double above = Single(y - cosine);
            const double range = Single(f31 - cosine);
            guest::StoreU32(base, out + 4 * i, 1);
            const double strength = Single(Single(std::sqrt(above)) / range);
            if (i < 1) {
              f27 = strength;
            } else if (i == 1) {
              f26 = strength;
            } else if (i < 3) {
              f25 = strength;
            }
            Stfs(base, frame + 176, f29);
            Stfs(base, frame + 180, f31);
            Stfs(base, frame + 184, f29);
            Stfs(base, frame + 188, f29);
            r3 = out_r3 + 64 * i + 48;
            MatrixShadow(base, r3, entry, frame + 112, frame + 176, frame);
          }
        }
        scene = G::scene(base);
        if (scene.lighting_fixed() == 0 && int32_t(i) <= 1) {
          const double cx = Single(Lfs(base, at - 8) * f30);
          const double cy = Single(Lfs(base, at - 4) * f30);
          const double cz = Single(Lfs(base, at + 0) * f30);
          // The frame's +192 and +208.
          const uint32_t colors_a[4] = {Bits(Single(FloatOf(vector_a[0]) * cx)),
                                        Bits(Single(FloatOf(vector_a[1]) * cy)),
                                        Bits(Single(FloatOf(vector_a[2]) * cz)), Bits(f31)};
          const uint32_t colors_b[4] = {Bits(Single(FloatOf(vector_b[0]) * cx)),
                                        Bits(Single(FloatOf(vector_b[1]) * cy)),
                                        Bits(Single(FloatOf(vector_b[2]) * cz)), Bits(f31)};
          const uint32_t slot = 16 * i;
          CopyWords(base, kLightDirections + slot, frame + 144, 4);
          CopyWords(base, kLightPositions + slot, entry, 4);
          StoreWords(base, kLightColorsA + slot, colors_a);
          CopyWords(base, kLightBlock240 + slot, frame + 256, 4);
          StoreWords(base, kLightColorsB + slot, colors_b);
          CopyWords(base, kLightAmbient + slot, frame + 272, 4);
          // As the original's lis and addi leave it (sign-extended).
          r3 = uint64_t(int64_t(int32_t(kLightColorsA))) + slot;
        }
        count--;
        at += 32;
      } while (count != 0);
    }
    if (int32_t(flag) != 0) {
      double f0 = Single(f25 + f26);
      weights[2] = Bits(Single(f26 * f24));
      weights[3] = Bits(Single(f25 * f24));
      weights[1] = Bits(Single(f27 * f24));
      f0 = Single(f0 + f27);
      f0 = Single(f28 - f0);
      weights[0] = Bits(Single(f0 * f24));
      StoreWords(base, kShadowWeights, weights);
    }
    return r3;
  }
}

}  // namespace car_draw
