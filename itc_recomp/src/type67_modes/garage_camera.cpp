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
#include "matrix_math/matrix_math.h"
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

namespace {

// GarageCamera::Command's constants (floats).
constexpr uint32_t kTiltStep = 0x82001FBC;   // the tilt's step and its low bound
constexpr uint32_t kTiltHigh = 0x82000A30;   // the tilt's high bound
constexpr uint32_t kStep = 0x82002948;       // the turns' step
constexpr uint32_t kTurnLow = 0x820029C4;    // a turn's low bound (wraps to the high)
constexpr uint32_t kTurnHigh = 0x82002240;   // its high bound (wraps to the low)
constexpr uint32_t kZoomStep = 0x8200222C;   // +180's step
constexpr uint32_t kZoomLow = 0x820021F8;
constexpr uint32_t kZoomHigh = 0x820035B0;
constexpr uint32_t kFovLow = 0x82001FD8;     // +224's bounds
constexpr uint32_t kFovHigh = 0x82002A0C;
constexpr uint32_t kMoveStep = 0x82002204;   // +256, +260 and +264's step

// The clamp the original applies: below `low` gives `low`, else above
// `high` gives `high`.
double Clamp(uint8_t* base, double v, uint32_t low, uint32_t high) {
  const double lo = Lfs(base, low);
  if (v < lo) {
    return lo;
  }
  const double hi = Lfs(base, high);
  return v > hi ? hi : v;
}
// The turns' wrap: below the low bound gives the high one, else above the
// high one the low one.
double Wrap(uint8_t* base, double v) {
  const double lo = Lfs(base, kTurnLow);
  if (v < lo) {
    return Lfs(base, kTurnHigh);
  }
  return v > Lfs(base, kTurnHigh) ? lo : v;
}

}  // namespace

uint64_t GarageCameraCommand(uint8_t* base, uint32_t camera, uint32_t command, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82180358 GarageCamera::Command");
  pf::Mode mode;
  mode.Scalar();
  const uint32_t c = camera;
  const uint32_t frame = sp - 192;
  guest::StoreU32(base, frame, sp);
  auto add = [&](uint32_t field, uint32_t step, bool up) {
    const double v = Lfs(base, c + field), d = Lfs(base, step);
    return pf::Round(up ? v + d : v - d);
  };
  // The commands that mark the camera moved (+272 = 1).
  bool moved = true;
  switch (command) {
    case 0:  // the tilt up and down (sub_821803A0, the recompiler's piece, for 0)
    case 1:
      Stfs(base, c + 100, Clamp(base, add(100, kStep, command == 0), kTiltStep, kTiltHigh));
      break;
    case 2:  // the turn about the car
    case 3:
      Stfs(base, c + 176, Wrap(base, add(176, kStep, command == 2)));
      break;
    case 4:  // the distance
    case 5:
      Stfs(base, c + 180, Clamp(base, add(180, kZoomStep, command == 5), kZoomLow, kZoomHigh));
      break;
    case 6:  // the field of view
    case 7:
      Stfs(base, c + 224, Clamp(base, add(224, kTiltStep, command == 6), kFovLow, kFovHigh));
      moved = false;
      break;
    case 8: case 9: case 10: case 11: case 12: case 13: {  // the angles +240 to +248
      const uint32_t field = 240 + 4 * ((command - 8) / 2) ;
      Stfs(base, c + field, Wrap(base, add(field, kStep, command % 2 == 0)));
      moved = false;
      break;
    }
    case 14: case 15: case 16: case 17: case 18: case 19: {  // the offsets
      static constexpr uint32_t kFields[3] = {260, 256, 264};
      const uint32_t field = kFields[(command - 14) / 2];
      Stfs(base, c + field, add(field, kMoveStep, command % 2 == 0));
      moved = false;
      break;
    }
    case 20: {  // reset
      if (int32_t(guest::LoadU32(base, c + 272)) == 1) {
        guest::StoreU32(base, c + 272, 2);
      }
      GarageCameraReset(base, c, frame - kGarageCameraResetFrame);
      mode.Scalar();
      guest::StoreU32(base, c + 272, 0);
      moved = false;
      break;
    }
    default:  // above 20: only the matrices
      moved = false;
      break;
  }
  if (moved) {
    guest::StoreU32(base, c + 272, 1);
  }
  // The quaternion from the turn (+176) and the tilt (+100); the rotation
  // (+112) the identity (built at the frame's +96, memcpy) turned about x
  // by the tilt (the angle at the frame's +80).
  QuaternionFromAngles(base, c + 192, Lfs(base, c + 176), Lfs(base, c + 100),
                       Lfs(base, 0x82000A34), frame);
  mode.Scalar();
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, frame + 96 + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
  if (!c_runtime::Memcpy(base, c + 112, frame + 96, 64)) {
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, c + 112 + i, guest::LoadU32(base, frame + 96 + i));
    }
  }
  Stfs(base, frame + 80, Lfs(base, c + 100));
  matrix_math::RotateX(base, c + 112, c + 112, frame + 80, frame - 80);
  return c + 112;
}

uint64_t GarageStageCameraCommand(uint8_t* base, uint64_t r3, uint32_t command, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821808F0 GarageStage::CameraCommand");
  const uint32_t stage = guest::LoadU32(base, uint32_t(r3));
  if (int32_t(guest::LoadU32(base, stage + 384)) == 2 && int32_t(command) != 20) {
    return r3;
  }
  return GarageCameraCommand(base, stage + 112, command, sp);
}

void GarageLightsInit(uint8_t* base, uint32_t lights, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8217D3C8 GarageLights::Init");
  pf::Mode mode;
  const uint32_t f = frame;
  // The 11 records (96 bytes each) from the table, a doubleword at a time.
  for (uint32_t i = 0; i < 11 * 96; i += 8) {
    StoreU64(base, lights + 16 + i, LoadU64(base, kGarageLightsTable + i));
  }
  // The ambient (0.08, 0.08, 0.08, 1.0) through the frame to +1072 and
  // 0x8265ED00.
  mode.Scalar();
  const double ambient = Lfs(base, 0x82002A1C);
  const double one = Lfs(base, 0x82000A30);
  Stfs(base, f + 80, ambient);
  Stfs(base, f + 84, ambient);
  Stfs(base, f + 88, ambient);
  Stfs(base, f + 92, one);
  for (uint32_t i = 0; i < 16; i += 8) {
    StoreU64(base, lights + 1072 + i, LoadU64(base, f + 80 + i));
  }
  for (uint32_t i = 0; i < 16; i += 8) {
    StoreU64(base, 0x8265ED00 + i, LoadU64(base, lights + 1072 + i));
  }
  // The three lights' constants.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t light = lights + 32 + 96 * i;
    for (uint32_t w = 0; w < 16; w += 8) {
      StoreU64(base, 0x8296B950 + 16 * i + w, LoadU64(base, light + w));
      StoreU64(base, 0x8296B980 + 16 * i + w, LoadU64(base, light + 16 + w));
      StoreU64(base, 0x8296B9B0 + 16 * i + w, LoadU64(base, light + 32 + w));
    }
  }
  // The four spots (q = +304 + 96i).
  mode.Vector();
  const Vec half = pf::I(simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                                         simde_mm_castsi128_ps(simde_mm_set1_epi32(0x3F000000))));
  const Vec zeros = simde_mm_setzero_si128();
  mode.Scalar();
  Stfs(base, f + 108, Lfs(base, 0x82009EB4));
  const double radians = Lfs(base, 0x82002948), scale = Lfs(base, 0x82001BE4);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t q = lights + 304 + 96 * i, at = 16 * i;
    mode.Scalar();
    Stfs(base, f + 124, one);
    Stfs(base, f + 92, one);
    auto cosine = [&](uint32_t address) {
      mode.Scalar();
      const double angle = double(float(double(float(Lfs(base, address) * scale)) * radians));
      const double c = c_runtime::Cos(angle);
      mode.Scalar();
      return c;
    };
    const double c0 = double(float(cosine(q + 64)));
    const double c1 = double(float(cosine(q + 68)));
    const double c2 = cosine(q + 68);
    const double c3 = cosine(q + 64);
    const double difference = double(float(c0 - c1));
    Stfs(base, f + 100, double(float(c2)));
    const Vec v12 = pf::Lvx(base, q + 16);
    mode.Vector();
    const Vec v0 = pf::I(simde_mm_dp_ps(pf::F(v12), pf::F(v12), 0xEF));
    const uint64_t p0 = LoadU64(base, q + 80), p1 = LoadU64(base, q + 88);
    StoreU64(base, f + 112, p0);
    StoreU64(base, f + 120, p1);
    mode.Scalar();
    Stfs(base, f + 104, double(float(one / difference)));
    Stfs(base, f + 96, double(float(c3)));
    StoreU64(base, 0x8296B9E0 + at, LoadU64(base, q));
    mode.Vector();
    Vec v13 = pf::I(simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(pf::F(v0))));
    const Vec v9 = pf::Mul(v0, half);
    const Vec equal = pf::I(simde_mm_cmpeq_ps(pf::F(v0), pf::F(zeros)));
    StoreU64(base, 0x8296B9E0 + at + 8, LoadU64(base, q + 8));
    const Vec v8 = pf::Mul(v13, v13);
    // vnmsubfp: -(v9 v8 - 0.5), one Newton step.
    const Vec v10 = pf::I(simde_mm_xor_ps(pf::F(pf::Sub(pf::Mul(v9, v8), half)),
                                          simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
    v13 = pf::Madd(v13, v10, v13);
    v13 = pf::Mul(v12, v13);
    const Vec v11 = simde_mm_or_si128(simde_mm_andnot_si128(equal, v13), simde_mm_and_si128(equal, v0));
    pf::Stvx(base, f + 80, v11);
    mode.Scalar();
    StoreU64(base, 0x8296BBE0 + at, LoadU64(base, f + 80));
    StoreU64(base, 0x8296BBE0 + at + 8, LoadU64(base, f + 88));
    StoreU64(base, 0x8296BAE0 + at, LoadU64(base, q + 32));
    StoreU64(base, 0x8296BAE0 + at + 8, LoadU64(base, q + 40));
    StoreU64(base, 0x8296BB60 + at, LoadU64(base, q + 48));
    StoreU64(base, 0x8296BB60 + at + 8, LoadU64(base, q + 56));
    StoreU64(base, 0x8296BC60 + at, LoadU64(base, f + 96));
    StoreU64(base, 0x8296BA60 + at, LoadU64(base, f + 112));
    StoreU64(base, 0x8296BC60 + at + 8, LoadU64(base, f + 104));
    StoreU64(base, 0x8296BA60 + at + 8, LoadU64(base, f + 120));
  }
}

}  // namespace type67_modes
