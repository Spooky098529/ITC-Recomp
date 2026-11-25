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
#include "schema/cameras.h"
#include "schema/material_passes.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

constexpr uint32_t kOneBits = 0x3F800000;
using G = schema::MaterialPassesGlobals;

simde__m128 Lvx(uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(guest::LoadU32(at), guest::LoadU32(at + 4),
                  guest::LoadU32(at + 8), guest::LoadU32(at + 12));
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
void StoreIdentity(uint32_t at) {
  for (uint32_t row = 0; row < 4; row++) {
    Stvx(at + 16 * row,
         VectorOf(row == 0 ? kOneBits : 0, row == 1 ? kOneBits : 0, row == 2 ? kOneBits : 0,
                  row == 3 ? kOneBits : 0));
  }
}
// Eight doublewords from `from` to `to`, in order.
void CopyDoublewords(uint32_t to, uint32_t from) {
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(to + i, guest::LoadU32(from + i));
  }
}

}  // namespace

void LookAt(uint32_t out, uint32_t position, uint32_t direction, uint32_t up,
            uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1D70 Matrix::LookAt");
  const uint32_t frame = sp - 224;
  guest::StoreU32(frame, sp);  // the back chain, as stwu
  StoreIdentity(frame + 80);
  CopyDoublewords(frame + 144, frame + 80);
  {
    FlushToZero ftz;
    const simde__m128 d = Lvx(direction);
    const simde__m128 u = Lvx(up);
    const simde__m128 d_78 = Shuffle(d, 0x78);
    const simde__m128 u_9c = Shuffle(u, 0x9C);
    const simde__m128 d_9c = Shuffle(d, 0x9C);
    const simde__m128 u_78 = Shuffle(u, 0x78);
    const simde__m128 dd = Dot3(d, d);
    const simde__m128 side = NegMulSub(u_78, d_9c, Mul(u_9c, d_78));
    const simde__m128 row0 = Normalized(side, Dot3(side, side));
    const simde__m128 row0_78 = Shuffle(row0, 0x78);
    Stvx(frame + 144, row0);
    const simde__m128 row0_9c = Shuffle(row0, 0x9C);
    const simde__m128 row2 = Normalized(d, dd);
    const simde__m128 row2_9c = Shuffle(row2, 0x9C);
    Stvx(frame + 176, row2);
    const simde__m128 row2_78 = Shuffle(row2, 0x78);
    const simde__m128 row1 = NegMulSub(row2_78, row0_9c, Mul(row2_9c, row0_78));
    Stvx(frame + 160, row1);
  }
  car_physics::MatrixMultiplyTranslation(frame + 144, frame + 144, position, frame - 192);
  matrix_math::Inverse(out, frame + 144);
}

std::optional<uint32_t> ProjectorMatrices(const ProjectorArgs& a, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1EB8 Projector::Matrices");
  const uint32_t f = sp - 544;
  guest::StoreU32(f, sp);  // the back chain, as stwu
  const double zero = double(G::zero());
  Stfs(f + 84, zero);
  Stfs(f + 88, zero);
  const double one = double(G::one());
  for (uint32_t at : {92u, 124u, 140u, 156u}) {
    Stfs(f + at, one);
  }
  const double minus_one = double(G::minus_one());
  Stfs(f + 172, one);
  Stfs(f + 236, one);
  Stfs(f + 80, a.side != 0 ? minus_one : one);
  matrix_math::Copy(f + 112, a.matrix);
  interpolation::BlendState_OnCopy(f + 112, a.matrix, 0x820D1F48);
  Stfs(f + 152, zero);
  Stfs(f + 148, zero);
  Stfs(f + 144, zero);
  matrix_math::Vec4Transform(f + 224, f + 112, f + 80);
  {
    FlushToZero ftz;
    Stvx(f + 80, Dot3(Lvx(f + 224), Lvx(schema::CameraPass(a.object).forward_x_address())));
  }
  const double facing = Lfs(f + 80);
  if (!(facing > zero)) {
    return 0;
  }
  const uint32_t world = schema::CameraPass(a.object).view_address();
  for (uint32_t at : {316u, 332u, 348u, 364u, 252u, 268u, 284u, 300u, 380u, 396u, 412u, 428u,
                      108u}) {
    Stfs(f + at, one);
  }
  matrix_math::Multiply(f + 304, world, a.matrix);
  interpolation::BlendState_OnMultiply(f + 304, world, a.matrix, 0x820D1FD4);
  matrix_math::Vec4Transform(f + 96, f + 304, a.point);
  const double z = Lfs(f + 104);
  // The size from the point's depth.
  double size;
  {
    const bool side = a.side != 0;
    const double depth_scale =
        double(side ? G::value_10() : G::projector_depth_scale());
    double s = Fmadds(z, depth_scale, double(G::projector_depth_bias()));
    s = Fmuls(s, double(G::projector_size_scale()));
    s = Single(s * G::projector_size_double());
    size = Fmuls(s, double(side ? G::value_0_6() : G::projector_size_factor()));
  }
  {
    const double small = double(G::value_0_8());
    if (std::fabs(size) < small) {
      size = Fmuls(size, small);
    }
    const double low = double(G::projector_size_min());
    if (size < low) {
      size = low;
    } else {
      const double high = double(G::projector_size_max());
      if (size > high) {
        size = high;
      }
    }
  }
  {
    FlushToZero ftz;
    const simde__m128 p = Lvx(f + 96);
    const simde__m128 dot = Dot3(p, p);
    StoreIdentity(f + 112);
    Stvx(f + 80, Normalized(p, dot));
  }
  {
    const double half = double(G::half());
    const double ux = Fmuls(Lfs(f + 80), half);
    const double uy = Fmuls(Lfs(f + 84), half);
    const double uz = Fmuls(Lfs(f + 88), half);
    Stfs(f + 96, Fsubs(Lfs(f + 96), ux));
    Stfs(f + 100, Fsubs(Lfs(f + 100), uy));
    Stfs(f + 104, Fsubs(z, uz));
  }
  CopyDoublewords(f + 240, f + 112);
  static constexpr uint32_t kAt[12] = {176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220};
  const double values[12] = {zero, zero, zero, one, one, zero, zero, one, minus_one, zero, zero, one};
  for (int i = 0; i < 12; i++) {
    Stfs(f + kAt[i], values[i]);
  }
  LookAt(f + 240, f + 176, a.side != 0 ? f + 192 : f + 208, f + 96, f);
  car_physics::MatrixMultiplyTranslation(f + 240, f + 240, f + 96, f - 192);
  matrix_math::Inverse(f + 368, world);
  matrix_math::Multiply(a.view, f + 368, f + 240);
  interpolation::BlendState_OnMultiply(a.view, f + 368, f + 240, 0x820D21F8);
  {
    FlushToZero ftz;
    StoreIdentity(f + 112);
  }
  if (!c_runtime::Memcpy(a.scale, f + 112, 64)) {
    return std::nullopt;
  }
  const double scaled = Fmuls(size, facing);
  const schema::MaterialPassesMatrix scale(a.scale);
  scale.set_m22(float(facing));
  scale.set_m00(float(scaled));
  scale.set_m11(float(scaled));
  return 1;
}

}  // namespace material_passes
