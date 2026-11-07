// The garage camera's reset and its quaternion
// (docs/functions/type6-type7-modes.md: Quaternion::FromAngles sub_8217C120,
// GarageCamera::Reset sub_8217D888) as plain C++: the translated code's
// float steps (src/xaudio/ppc_float.h), so the results are the original's
// bit for bit, and its frame's stores replayed.

#include "type67_modes/type67_modes.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "xaudio/ppc_float.h"

#include <bit>

namespace type67_modes {

namespace {

namespace pf = xaudio::ppc_float;
using pf::Vec;

// lfs and stfs as the translated code's doubles.
double Lfs(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

Vec Splat(Vec v, int word) {
  switch (word) {
    case 0: return simde_mm_shuffle_epi32(v, 0xFF);
    case 1: return simde_mm_shuffle_epi32(v, 0xAA);
    case 2: return simde_mm_shuffle_epi32(v, 0x55);
    default: return simde_mm_shuffle_epi32(v, 0x00);
  }
}
// vrlimi128 with no rotation: the lanes `mask` picks (the host blend's bits)
// from `b`.
template <int kMask>
Vec Insert(Vec a, Vec b) {
  return pf::I(simde_mm_blend_ps(pf::F(a), pf::F(b), kMask));
}

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}
void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

}  // namespace

void QuaternionFromAngles(uint8_t* base, uint32_t out, double f1, double f2, double f3,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8217C120 Quaternion::FromAngles");
  pf::Mode mode;
  mode.Scalar();
  // The angles below the stack pointer (f3, f1, 0.0, f2), read back as
  // vectors with lvlx.
  Stfs(base, sp - 32, f3);
  Stfs(base, sp - 28, f1);
  Stfs(base, sp - 20, f2);
  Stfs(base, sp - 24, Lfs(base, 0x82000A34));
  const Vec v5 = simde_mm_setzero_si128();
  Vec v9 = pf::Lvlx(base, sp - 28);
  Vec v0 = pf::Lvlx(base, sp - 20);
  Vec v8 = pf::Lvlx(base, sp - 24);
  v0 = pf::I(simde_mm_blend_ps(pf::F(v0), simde_mm_permute_ps(pf::F(v9), 57), 4));
  Vec v13 = pf::Lvlx(base, sp - 32);
  v13 = pf::I(simde_mm_blend_ps(pf::F(v13), simde_mm_permute_ps(pf::F(v8), 57), 4));
  Vec v10 = pf::Lvx(base, 0x82000A20);
  Vec v6 = Splat(v10, 3);
  v10 = Splat(v10, 1);
  v0 = pf::I(simde_mm_blend_ps(pf::F(v0), simde_mm_permute_ps(pf::F(v13), 78), 3));
  Vec v12 = pf::Lvx(base, 0x820009F0);
  Vec v4 = Splat(v12, 1), v2 = Splat(v12, 2), v31 = Splat(v12, 3);
  mode.Vector();
  // vcfsx of 1 scaled by 2^-1: 0.5 in every lane.
  v12 = pf::I(simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                              simde_mm_castsi128_ps(simde_mm_set1_epi32(0x3F000000))));
  Vec v11 = pf::Lvx(base, 0x820009C0);
  Vec v3 = Splat(v11, 1), v1 = Splat(v11, 2), v30 = Splat(v11, 3);
  // vupkd3d128 of zero: (3.0, 3.0, 0.0, 1.0).
  v11 = simde_mm_castps_si128(simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f));
  v0 = pf::Mul(v0, v12);
  v9 = Splat(v11, 3);
  v13 = pf::Mul(v0, v6);
  v13 = pf::I(simde_mm_round_ps(pf::F(v13), SIMDE_MM_FROUND_TO_NEAREST_INT | SIMDE_MM_FROUND_NO_EXC));
  // vnmsubfp: -(v10 v13 - v0).
  v0 = pf::I(simde_mm_xor_ps(pf::F(pf::Sub(pf::Mul(v10, v13), v0)),
                             simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  v10 = pf::Lvx(base, 0x82000A00);
  v6 = Splat(v10, 0);
  const Vec v29 = Splat(v10, 1), v27 = Splat(v10, 2), v25 = Splat(v10, 3);
  v13 = pf::Mul(v0, v0);
  v12 = pf::Mul(v13, v0);
  Vec v7 = pf::Madd(v4, v13, v9);
  v9 = pf::Lvx(base, 0x820009D0);
  v11 = pf::Mul(v13, v13);
  v4 = Splat(v9, 0);
  const Vec v28 = Splat(v9, 1), v26 = Splat(v9, 2), v24 = Splat(v9, 3);
  v8 = pf::Lvx(base, 0x82000A10);
  const Vec v23 = Splat(v8, 0), v21 = Splat(v8, 1);
  v9 = pf::Madd(v3, v12, v0);
  const Vec v19 = Splat(v8, 2);
  v13 = pf::Mul(v12, v13);
  const Vec v17 = Splat(v8, 3);
  v8 = pf::Madd(v2, v11, v7);
  v0 = pf::Mul(v12, v12);
  v10 = pf::Lvx(base, 0x820009E0);
  v12 = pf::Mul(v11, v12);
  const Vec v22 = Splat(v10, 0), v20 = Splat(v10, 1), v18 = Splat(v10, 2);
  v2 = Splat(v10, 3);
  v7 = pf::Madd(v1, v13, v9);
  v3 = pf::Madd(v31, v0, v8);
  v10 = pf::Mul(v11, v11);
  v11 = pf::Mul(v13, v11);
  v7 = pf::Madd(v30, v12, v7);
  v9 = pf::Mul(v13, v13);
  v13 = pf::Mul(v0, v13);
  v8 = pf::Mul(v0, v0);
  v0 = pf::Mul(v12, v0);
  v1 = pf::Mul(v12, v12);
  v6 = pf::Madd(v6, v10, v3);
  v12 = pf::Mul(v10, v12);
  v7 = pf::Madd(v4, v11, v7);
  v3 = pf::Mul(v9, v9);
  v6 = pf::Madd(v29, v9, v6);
  v7 = pf::Madd(v28, v13, v7);
  v4 = pf::Madd(v27, v8, v6);
  v0 = pf::Madd(v26, v0, v7);
  v7 = pf::Mul(v10, v10);
  v10 = pf::Mul(v11, v10);
  // The sign mask: -1 shifted left by 31, words 0 and 3 cleared.
  v6 = Insert<9>(simde_mm_set1_epi32(int(0x80000000)), v5);
  v4 = pf::Madd(v25, v1, v4);
  v1 = pf::Mul(v13, v13);
  v0 = pf::Madd(v24, v12, v0);
  v12 = pf::Mul(v11, v11);
  v11 = pf::Mul(v9, v11);
  v9 = pf::Mul(v13, v9);
  v13 = pf::Mul(v8, v13);
  v8 = pf::Madd(v23, v7, v4);
  v0 = pf::Madd(v22, v10, v0);
  v12 = pf::Madd(v21, v12, v8);
  v0 = pf::Madd(v20, v11, v0);
  v12 = pf::Madd(v19, v3, v12);
  v11 = pf::Madd(v18, v9, v0);
  v0 = pf::Madd(v17, v1, v12);
  v13 = pf::Madd(v2, v13, v11);
  // The sines (v0) and cosines (v13) into the quaternion.
  v12 = Splat(v0, 0);
  v11 = Splat(v0, 1);
  v10 = Splat(v13, 0);
  v8 = Splat(v13, 1);
  v7 = Splat(v13, 2);
  v12 = Insert<8>(v12, v13);
  v11 = Insert<4>(v11, v13);
  v9 = Splat(v0, 2);
  v10 = Insert<8>(v10, v0);
  v8 = Insert<4>(v8, v0);
  v7 = Insert<2>(v7, v0);
  v0 = pf::Mul(v12, v11);
  v9 = Insert<2>(v9, v13);
  v13 = simde_mm_xor_si128(v10, v6);
  v13 = pf::Mul(v13, v8);
  v0 = pf::Mul(v0, v9);
  v0 = pf::Madd(v13, v7, v0);
  pf::Stvx(base, out, v0);
}

void GarageCameraReset(uint8_t* base, uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8217D888 GarageCamera::Reset");
  pf::Mode mode;
  mode.Scalar();
  const uint32_t f = frame;
  const double zero = Lfs(base, 0x82000A34), one = Lfs(base, 0x82000A30);
  // The vectors built in the frame, in the original's store order.
  Stfs(base, f + 100, Lfs(base, 0x82002240));
  Stfs(base, f + 144, zero);
  Stfs(base, f + 148, zero);
  Stfs(base, f + 152, zero);
  Stfs(base, f + 156, one);
  Stfs(base, f + 112, zero);
  Stfs(base, f + 116, zero);
  Stfs(base, f + 120, zero);
  Stfs(base, f + 124, one);
  Stfs(base, f + 160, Lfs(base, 0x82009EE4));
  Stfs(base, f + 140, one);
  // The eye's start (+16) and the target (+32).
  StoreU64(base, camera + 16, LoadU64(base, f + 144));
  Stfs(base, f + 176, zero);
  StoreU64(base, camera + 24, LoadU64(base, f + 152));
  StoreU64(base, camera + 32, LoadU64(base, f + 112));
  Stfs(base, f + 180, zero);
  StoreU64(base, camera + 40, LoadU64(base, f + 120));
  Stfs(base, f + 128, double(float(Lfs(base, camera + 16) - Lfs(base, camera + 32))));
  Stfs(base, f + 132, double(float(Lfs(base, camera + 20) - Lfs(base, camera + 36))));
  Stfs(base, f + 136, double(float(Lfs(base, camera + 24) - Lfs(base, camera + 40))));
  const uint64_t d0 = LoadU64(base, f + 128);
  Stfs(base, f + 184, zero);
  const uint64_t d1 = LoadU64(base, f + 136);
  Stfs(base, f + 188, one);
  const uint64_t e0 = LoadU64(base, f + 176);
  Stfs(base, f + 80, zero);
  const uint64_t e1 = LoadU64(base, f + 184);
  Stfs(base, f + 84, zero);
  Stfs(base, f + 88, zero);
  Stfs(base, f + 92, one);
  StoreU64(base, camera + 48, d0);
  const uint64_t g0 = LoadU64(base, f + 80);
  Stfs(base, f + 96, zero);
  StoreU64(base, camera + 56, d1);
  const uint64_t g1 = LoadU64(base, f + 88);
  Stfs(base, f + 104, zero);
  Stfs(base, f + 108, one);
  StoreU64(base, camera + 64, e0);
  const uint64_t a0 = LoadU64(base, f + 96);
  StoreU64(base, camera + 72, e1);
  const uint64_t a1 = LoadU64(base, f + 104);
  StoreU64(base, camera + 80, g0);
  StoreU64(base, camera + 88, g1);
  StoreU64(base, camera + 240, a0);
  StoreU64(base, camera + 248, a1);
  Stfs(base, f + 172, one);
  Stfs(base, f + 164, Lfs(base, 0x82002A10));
  Stfs(base, f + 168, zero);
  StoreU64(base, camera + 256, LoadU64(base, f + 160));
  StoreU64(base, camera + 264, LoadU64(base, f + 168));
  Stfs(base, camera + 96, zero);
  Stfs(base, camera + 100, Lfs(base, 0x82009EE0));
  Stfs(base, camera + 104, zero);
  Stfs(base, camera + 224, Lfs(base, 0x82000F14));
  // The zoom: 640 / tan(pi / 3), tan in nearest rounding as its _ctrlfp
  // sets it (put back after, as the original's second _ctrlfp does).
  double tangent = 0.0;
  {
    using Fpscr = rex::platform::FPSCRPlatform;
    const uint32_t csr = Fpscr::getcsr();
    Fpscr::setcsr(csr & ~uint32_t(Fpscr::RoundMaskVal));
    if (!c_runtime::Tan(std::bit_cast<double>(LoadU64(base, 0x82009ED8)), &tangent)) {
      ITC_LOG_ERROR("type67_modes: GarageCamera::Reset: tan refused its constant");
    }
    Fpscr::setcsr(csr);
  }
  mode.Scalar();
  Stfs(base, camera + 108, double(float(Lfs(base, 0x82009EB8) / double(float(tangent)))));
  // The identity at the frame's +192, copied to +112 (memcpy).
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, f + 192 + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
  if (!c_runtime::Memcpy(base, camera + 112, f + 192, 64)) {
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, camera + 112 + i, guest::LoadU32(base, f + 192 + i));
    }
  }
  Stfs(base, camera + 176, zero);
  Stfs(base, camera + 180, Lfs(base, 0x82009ED0));
  QuaternionFromAngles(base, camera + 192, zero, zero, zero, f);
}

}  // namespace type67_modes
