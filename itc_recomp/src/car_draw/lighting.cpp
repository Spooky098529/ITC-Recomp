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

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>
#include <cstdlib>

namespace car_draw {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kOne = 0x82000A30;           // 1.0
constexpr uint32_t kZero = 0x82000A34;          // 0.0
constexpr uint32_t kMergeDistance = 0x820021F8;  // also c43's base
constexpr uint32_t kColorScale = 0x82002200;
constexpr uint32_t kFarthest = 0x82002814;
constexpr uint32_t kConeScale = 0x82002A3C;
constexpr uint32_t kLightSets = 0x826C551C;
constexpr uint32_t kLightPositions = 0x8296B9E0;
constexpr uint32_t kLightAmbient = 0x8296BA60;
constexpr uint32_t kLightColorsA = 0x8296BAE0;
constexpr uint32_t kLightColorsB = 0x8296BB60;
constexpr uint32_t kLightDirections = 0x8296BBE0;
constexpr uint32_t kLightBlock240 = 0x8296BC60;
constexpr uint32_t kShadowWeights = 0x8265ED10;  // pixel c43 of the depth records

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
  const uint32_t n = stack - 16;
  // The loads before the normalized normal's store, in the original's order.
  const double py = Lfs(base, point + 4);
  const double lx = Lfs(base, light + 0);
  const double pz = Lfs(base, point + 8);
  const double lz = Lfs(base, light + 8);
  const double px = Lfs(base, point + 0);
  const double ly = Lfs(base, light + 4);
  const double lw = Lfs(base, light + 12);
  {
    FlushToZero ftz;
    StoreVector(base, n, Normalize(VectorAt(base, normal), Half()));
  }
  const double ny = Lfs(base, n + 4);
  double f12 = Single(py * ny);
  const double nx = Lfs(base, n + 0);
  double f9 = Single(lx * nx);
  const double nz = Lfs(base, n + 8);
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
  const uint32_t block = guest::LoadU32(base, kScene) + 5248;
  const double f29 = Lfs(base, kZero);
  guest::StoreU32(base, stack + 20, out);
  guest::StoreU32(base, stack + 36, flag);
  const double f31 = Lfs(base, kOne);
  const double f24 = Lfs(base, block + 272);
  Stfs(base, frame + 144, f29);
  Stfs(base, frame + 148, f31);
  Stfs(base, frame + 152, f29);
  Stfs(base, frame + 156, f29);
  // The scene's lighting vectors (+96, +112, +240, +256) into the frame.
  CopyWords(base, frame + 240, block + 96, 4);
  CopyWords(base, frame + 224, block + 112, 4);
  CopyWords(base, frame + 256, block + 240, 4);
  CopyWords(base, frame + 272, block + 256, 4);
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
  Stfs(base, frame + 160, f28);
  debug::Vec4Transform(base, frame + 112, world, frame + 112);
  {
    const double cone = Single(Lfs(base, guest::LoadU32(base, kScene) + 5500) *
                               Lfs(base, kConeScale));
    const double least = Single(c_runtime::Cos(cone));
    const uint32_t owner = guest::LoadU32(base, kLightSets);
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
      uint32_t scene = guest::LoadU32(base, kScene);
      simde__m128 half;
      {
        FlushToZero ftz;
        half = Half();
      }
      guest::StoreU32(base, frame + 80, count);
      const double f30 = Lfs(base, kColorScale);
      uint32_t at = frame + 312;
      uint32_t next = 0;
      do {
        const uint32_t entry = at - 24;
        Stfs(base, frame + 100, Single(Lfs(base, at - 20) - Lfs(base, frame + 116)));
        Stfs(base, frame + 104, Single(Lfs(base, at - 16) - Lfs(base, frame + 120)));
        Stfs(base, frame + 96, Single(Lfs(base, entry) - Lfs(base, frame + 112)));
        Stfs(base, frame + 108, f31);
        Stfs(base, at - 12, f31);
        const double angle = Lfs(base, scene + 5500);
        {
          FlushToZero ftz;
          StoreVector(base, frame + 96, Normalize(VectorAt(base, frame + 96), half));
        }
        const uint32_t i = next;
        const double cosine = Single(c_runtime::Cos(angle));
        next++;
        guest::StoreU32(base, frame + 128, next);
        if (int32_t(flag) != 0) {
          const double y = Lfs(base, frame + 100);
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
        scene = guest::LoadU32(base, kScene);
        if (guest::LoadU32(base, scene + 5052) == 0 && int32_t(i) <= 1) {
          const double cx = Single(Lfs(base, at - 8) * f30);
          const double cy = Single(Lfs(base, at - 4) * f30);
          const double cz = Single(Lfs(base, at + 0) * f30);
          Stfs(base, frame + 204, f31);
          Stfs(base, frame + 220, f31);
          Stfs(base, frame + 192, Single(Lfs(base, frame + 240) * cx));
          Stfs(base, frame + 196, Single(Lfs(base, frame + 244) * cy));
          Stfs(base, frame + 200, Single(Lfs(base, frame + 248) * cz));
          Stfs(base, frame + 208, Single(Lfs(base, frame + 224) * cx));
          Stfs(base, frame + 212, Single(Lfs(base, frame + 228) * cy));
          Stfs(base, frame + 216, Single(Lfs(base, frame + 232) * cz));
          const uint32_t slot = 16 * i;
          CopyWords(base, kLightDirections + slot, frame + 144, 4);
          CopyWords(base, kLightPositions + slot, entry, 4);
          CopyWords(base, kLightColorsA + slot, frame + 192, 4);
          CopyWords(base, kLightBlock240 + slot, frame + 256, 4);
          CopyWords(base, kLightColorsB + slot, frame + 208, 4);
          CopyWords(base, kLightAmbient + slot, frame + 272, 4);
          // As the original's lis and addi leave it (sign-extended).
          r3 = uint64_t(int64_t(int32_t(kLightColorsA))) + slot;
        }
        count--;
        guest::StoreU32(base, frame + 80, count);
        at += 32;
      } while (count != 0);
    }
    if (int32_t(flag) != 0) {
      double f0 = Single(f25 + f26);
      Stfs(base, frame + 168, Single(f26 * f24));
      Stfs(base, frame + 172, Single(f25 * f24));
      Stfs(base, frame + 164, Single(f27 * f24));
      f0 = Single(f0 + f27);
      f0 = Single(f28 - f0);
      Stfs(base, frame + 160, Single(f0 * f24));
      CopyWords(base, kShadowWeights, frame + 160, 4);
    }
    return r3;
  }
}

}  // namespace car_draw
