// A projector's matrices from a point and a direction, with the view
// matrix it builds (see material_passes.h). The vector unit's work as the
// translated code does it (word 0 in the top lane, its shuffles' immediates
// as they are, flush-to-zero on), the scalar work with it off; the frames'
// temporaries written as the originals leave them (not their register
// saves). The matrix functions are matrix_math's and car_physics' plain C++,
// Copy and Multiply noted for frame interpolation as their wrappers note
// the originals' calls.

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "car_physics/car_physics.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

constexpr uint32_t kOneBits = 0x3F800000;
constexpr uint32_t kOne = 0x82000A30;       // 1.0
constexpr uint32_t kZero = 0x82000A34;      // 0.0
constexpr uint32_t kMinusOne = 0x82001BE0;  // probably -1.0
constexpr uint32_t kHalf = 0x82001BE4;      // probably 0.5

simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(guest::LoadU32(base, at), guest::LoadU32(base, at + 4),
                  guest::LoadU32(base, at + 8), guest::LoadU32(base, at + 12));
}
simde__m128 Shuffle(simde__m128 v, int imm) {
  // simde_mm_shuffle_epi32 needs a constant: the two the originals use.
  const simde__m128i i = simde_mm_castps_si128(v);
  return simde_mm_castsi128_ps(imm == 0x78 ? simde_mm_shuffle_epi32(i, 0x78)
                                           : simde_mm_shuffle_epi32(i, 0x9C));
}
simde__m128 Mul(simde__m128 a, simde__m128 b) { return simde_mm_mul_ps(a, b); }
simde__m128 MulAdd(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_add_ps(simde_mm_mul_ps(a, b), c);
}
// vnmsubfp: -(a b - c).
simde__m128 NegMulSub(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(a, b), c),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
}
simde__m128 Dot3(simde__m128 a, simde__m128 b) { return simde_mm_dp_ps(a, b, 0xEF); }
// vsel: b where the mask is set, else a.
simde__m128 Select(simde__m128 mask, simde__m128 a, simde__m128 b) {
  const simde__m128i m = simde_mm_castps_si128(mask);
  return simde_mm_castsi128_ps(
      simde_mm_or_si128(simde_mm_andnot_si128(m, simde_mm_castps_si128(a)),
                        simde_mm_and_si128(m, simde_mm_castps_si128(b))));
}
// vcfsx of (1, 1, 1, 1) by 1: 0.5 in every lane.
simde__m128 Half() {
  return simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
}
// `v` normalized by its squared length `dot` (vrsqrtefp and one Newton
// step), the squared length kept where it's 0.
simde__m128 Normalized(simde__m128 v, simde__m128 dot) {
  const simde__m128 half = Half();
  const simde__m128 estimate = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(dot));
  const simde__m128 halved = Mul(dot, half);
  const simde__m128 zero = simde_mm_cmpeq_ps(dot, simde_mm_setzero_ps());
  const simde__m128 squared = Mul(estimate, estimate);
  const simde__m128 step = NegMulSub(halved, squared, half);
  const simde__m128 refined = MulAdd(estimate, step, estimate);
  return Select(zero, Mul(v, refined), dot);
}
// The identity's four rows (vupkd3d and its shuffles), stored at `at`.
void StoreIdentity(uint8_t* base, uint32_t at) {
  for (uint32_t row = 0; row < 4; row++) {
    Stvx(base, at + 16 * row,
         VectorOf(row == 0 ? kOneBits : 0, row == 1 ? kOneBits : 0, row == 2 ? kOneBits : 0,
                  row == 3 ? kOneBits : 0));
  }
}
// Eight doublewords from `from` to `to`, in order.
void CopyDoublewords(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(base, to + i, guest::LoadU32(base, from + i));
  }
}

}  // namespace

void LookAt(uint8_t* base, uint32_t out, uint32_t position, uint32_t direction, uint32_t up,
            uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1D70 Matrix::LookAt");
  const uint32_t frame = sp - 224;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  StoreIdentity(base, frame + 80);
  CopyDoublewords(base, frame + 144, frame + 80);
  {
    FlushToZero ftz;
    const simde__m128 d = Lvx(base, direction);
    const simde__m128 u = Lvx(base, up);
    const simde__m128 d_78 = Shuffle(d, 0x78);
    const simde__m128 u_9c = Shuffle(u, 0x9C);
    const simde__m128 d_9c = Shuffle(d, 0x9C);
    const simde__m128 u_78 = Shuffle(u, 0x78);
    const simde__m128 dd = Dot3(d, d);
    const simde__m128 side = NegMulSub(u_78, d_9c, Mul(u_9c, d_78));
    const simde__m128 row0 = Normalized(side, Dot3(side, side));
    const simde__m128 row0_78 = Shuffle(row0, 0x78);
    Stvx(base, frame + 144, row0);
    const simde__m128 row0_9c = Shuffle(row0, 0x9C);
    const simde__m128 row2 = Normalized(d, dd);
    const simde__m128 row2_9c = Shuffle(row2, 0x9C);
    Stvx(base, frame + 176, row2);
    const simde__m128 row2_78 = Shuffle(row2, 0x78);
    const simde__m128 row1 = NegMulSub(row2_78, row0_9c, Mul(row2_9c, row0_78));
    Stvx(base, frame + 160, row1);
  }
  car_physics::MatrixMultiplyTranslation(base, frame + 144, frame + 144, position, frame - 192);
  matrix_math::Inverse(base, out, frame + 144);
}

std::optional<uint32_t> ProjectorMatrices(uint8_t* base, const ProjectorArgs& a, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1EB8 Projector::Matrices");
  const uint32_t f = sp - 544;
  guest::StoreU32(base, f, sp);  // the back chain, as stwu
  const double zero = Lfs(base, kZero);
  Stfs(base, f + 84, zero);
  Stfs(base, f + 88, zero);
  const double one = Lfs(base, kOne);
  for (uint32_t at : {92u, 124u, 140u, 156u}) {
    Stfs(base, f + at, one);
  }
  const double minus_one = Lfs(base, kMinusOne);
  Stfs(base, f + 172, one);
  Stfs(base, f + 236, one);
  Stfs(base, f + 80, a.side != 0 ? minus_one : one);
  matrix_math::Copy(base, f + 112, a.matrix);
  interpolation::BlendState_OnCopy(base, f + 112, a.matrix, 0x820D1F48);
  Stfs(base, f + 152, zero);
  Stfs(base, f + 148, zero);
  Stfs(base, f + 144, zero);
  matrix_math::Vec4Transform(base, f + 224, f + 112, f + 80);
  {
    FlushToZero ftz;
    Stvx(base, f + 80, Dot3(Lvx(base, f + 224), Lvx(base, a.object + 464)));
  }
  const double facing = Lfs(base, f + 80);
  if (!(facing > zero)) {
    return 0;
  }
  const uint32_t world = a.object + 16;
  for (uint32_t at : {316u, 332u, 348u, 364u, 252u, 268u, 284u, 300u, 380u, 396u, 412u, 428u,
                      108u}) {
    Stfs(base, f + at, one);
  }
  matrix_math::Multiply(base, f + 304, world, a.matrix);
  interpolation::BlendState_OnMultiply(base, f + 304, world, a.matrix, 0x820D1FD4);
  matrix_math::Vec4Transform(base, f + 96, f + 304, a.point);
  const double z = Lfs(base, f + 104);
  // The size from the point's depth.
  double size;
  {
    const bool side = a.side != 0;
    double s = Fmadds(z, Lfs(base, side ? 0x82001FD8 : 0x82001FC0), Lfs(base, 0x82001FCC));
    s = Fmuls(s, Lfs(base, 0x82002CC4));
    s = Single(s * std::bit_cast<double>(
                        (uint64_t(guest::LoadU32(base, 0x82003588)) << 32) |
                        guest::LoadU32(base, 0x8200358C)));
    size = Fmuls(s, Lfs(base, side ? 0x82002A14 : 0x82002A10));
  }
  {
    const double small = Lfs(base, 0x82001070);
    if (std::fabs(size) < small) {
      size = Fmuls(size, small);
    }
    const double low = Lfs(base, 0x82002828);
    if (size < low) {
      size = low;
    } else {
      const double high = Lfs(base, 0x820027A8);
      if (size > high) {
        size = high;
      }
    }
  }
  {
    FlushToZero ftz;
    const simde__m128 p = Lvx(base, f + 96);
    const simde__m128 dot = Dot3(p, p);
    StoreIdentity(base, f + 112);
    Stvx(base, f + 80, Normalized(p, dot));
  }
  {
    const double half = Lfs(base, kHalf);
    const double ux = Fmuls(Lfs(base, f + 80), half);
    const double uy = Fmuls(Lfs(base, f + 84), half);
    const double uz = Fmuls(Lfs(base, f + 88), half);
    Stfs(base, f + 96, Fsubs(Lfs(base, f + 96), ux));
    Stfs(base, f + 100, Fsubs(Lfs(base, f + 100), uy));
    Stfs(base, f + 104, Fsubs(z, uz));
  }
  CopyDoublewords(base, f + 240, f + 112);
  static constexpr uint32_t kAt[12] = {176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220};
  const double values[12] = {zero, zero, zero, one, one, zero, zero, one, minus_one, zero, zero, one};
  for (int i = 0; i < 12; i++) {
    Stfs(base, f + kAt[i], values[i]);
  }
  LookAt(base, f + 240, f + 176, a.side != 0 ? f + 192 : f + 208, f + 96, f);
  car_physics::MatrixMultiplyTranslation(base, f + 240, f + 240, f + 96, f - 192);
  matrix_math::Inverse(base, f + 368, world);
  matrix_math::Multiply(base, a.view, f + 368, f + 240);
  interpolation::BlendState_OnMultiply(base, a.view, f + 368, f + 240, 0x820D21F8);
  {
    FlushToZero ftz;
    StoreIdentity(base, f + 112);
  }
  if (!c_runtime::Memcpy(base, a.scale, f + 112, 64)) {
    return std::nullopt;
  }
  const double scaled = Fmuls(size, facing);
  Stfs(base, a.scale + 40, facing);
  Stfs(base, a.scale + 0, scaled);
  Stfs(base, a.scale + 20, scaled);
  return 1;
}

}  // namespace material_passes
