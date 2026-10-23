#include "cameras/cameras.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "command_list/command_list.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "memory/heap.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/avx.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>

namespace cameras {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kOne = 0x82000A30;       // the game's float 1.0
constexpr uint32_t kHalf = 0x82001BE4;      // 0.5
constexpr uint32_t kTwoPi = 0x82001FA8;     // 2 pi
constexpr uint32_t kOverTwoPi = 0x82001FA4;  // 1 / (2 pi)
constexpr uint32_t kRadiansPerDegree = 0x82002948;

// --- Scalar steps, as the translated code's doubles ---

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
uint32_t Bits(double value) {
  return std::bit_cast<uint32_t>(float(value));
}
double Fmuls(double a, double b) {
  return double(float(a * b));
}
double Fadds(double a, double b) {
  return double(float(a + b));
}
double Fsubs(double a, double b) {
  return double(float(a - b));
}
double Fdivs(double a, double b) {
  return double(float(a / b));
}
double Fnmsubs(double a, double b, double c) {
  return double(float(-std::fma(a, b, -c)));
}
// fneg: the sign bit flipped.
double Fneg(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) ^ 0x8000000000000000ull);
}

// An angle into [-pi, pi], as the library does it everywhere: pi with the
// angle's sign (its stored float's sign bit ORed into 0x40490FDB), added,
// times 1 / (2 pi), truncated (fctiwz: NaN gives INT_MIN, INT_MAX from
// INT_MAX up), that many 2 pi taken off (fnmsubs).
double WrapAngle(uint8_t* base, double x) {
  const double two_pi = Lfs(base, kTwoPi);
  const double pi = double(std::bit_cast<float>((Bits(x) & 0x80000000u) | 0x40490FDBu));
  const double turns = Fmuls(Fadds(pi, x), Lfs(base, kOverTwoPi));
  int32_t n;
  if (std::isnan(turns)) {
    n = INT32_MIN;
  } else if (turns >= double(INT_MAX)) {
    n = INT32_MAX;
  } else {
    n = simde_mm_cvttsd_si32(simde_mm_set_sd(turns));
  }
  return Fnmsubs(double(float(double(int64_t(n)))), two_pi, x);
}

// --- The vector unit, as the translation does it ---

// Vectors as the translated code holds them: word 0 in the top lane.
simde__m128 VectorOf(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int32_t(w0), int32_t(w1), int32_t(w2), int32_t(w3)));
}
// lvx: the aligned 16 bytes holding `address`.
simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(guest::LoadU32(base, at + 0), guest::LoadU32(base, at + 4),
                  guest::LoadU32(base, at + 8), guest::LoadU32(base, at + 12));
}
// Word i (0 the top lane).
uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}
double Word0(simde__m128 v) {
  return double(std::bit_cast<float>(WordOf(v, 0)));
}
// stvx: to the aligned 16 bytes holding `address`.
void Stvx(uint8_t* base, uint32_t address, simde__m128 v) {
  const uint32_t at = address & ~0xFu;
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(base, at + 4 * i, WordOf(v, i));
  }
}
// vspltw: one word in every lane (the shuffle's immediate).
template <int kImm>
simde__m128 Splat(simde__m128 v) {
  return simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v), kImm));
}
simde__m128 Mul(simde__m128 a, simde__m128 b) {
  return simde_mm_mul_ps(a, b);
}
simde__m128 Dot(simde__m128 a, simde__m128 b) {
  return simde_mm_dp_ps(a, b, 0xFF);
}

simde__m128 Sub(simde__m128 a, simde__m128 b) {
  return simde_mm_sub_ps(a, b);
}
simde__m128 Add(simde__m128 a, simde__m128 b) {
  return simde_mm_add_ps(a, b);
}
// vnmsubfp128 as the translation does it: -((a * b) - c).
simde__m128 NegMulSub(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_xor_ps(Sub(Mul(a, b), c), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
}
// vmsum3fp128: x, y and z multiplied and summed, in every lane.
simde__m128 Dot3(simde__m128 a, simde__m128 b) {
  return simde_mm_dp_ps(a, b, 0xEF);
}
// The 0.5 the translation makes from vspltisw 1 (vcfsx's scale).
simde__m128 Half() {
  return simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
}
template <int kImm>
simde__m128 Shuffle(simde__m128 v) {
  return simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v), kImm));
}
// The library's vector normalize, as the translation does it: the length
// squared (x, y, z) d, r = 1 / sqrt(d) (vrsqrtefp), one Newton step (r +=
// r (0.5 - d r r / 2)... as -((d / 2) r r - 0.5) r + r), v r; where d is 0,
// d itself (vsel on vcmpeqfp).
simde__m128 Normalize(simde__m128 v) {
  const simde__m128 d = Dot3(v, v);
  simde__m128 r = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(d));
  const simde__m128 t = Mul(d, Half());
  const simde__m128 zero = simde_mm_cmpeq_ps(d, simde_mm_setzero_ps());
  const simde__m128 k = NegMulSub(t, Mul(r, r), Half());
  r = Add(Mul(r, k), r);
  const simde__m128 n = Mul(v, r);
  return simde_mm_castsi128_ps(simde_mm_or_si128(
      simde_mm_andnot_si128(simde_mm_castps_si128(zero), simde_mm_castps_si128(n)),
      simde_mm_and_si128(simde_mm_castps_si128(zero), simde_mm_castps_si128(d))));
}
// A word through lfs and stfs (a float register): its bits, as the
// hardware keeps them (the translated code's conversions there are folded
// away, a signaling NaN kept).
uint32_t Through(uint32_t word) {
  return word;
}

// Flush-to-zero on while alive (the vector unit's mode), put back after.
class FlushToZero {
 public:
  FlushToZero() : csr_(Fpscr::getcsr()) { Fpscr::setcsr(csr_ | uint32_t(Fpscr::FlushMask)); }
  ~FlushToZero() { Fpscr::setcsr(csr_); }

 private:
  uint32_t csr_;
};

}  // namespace

void SetFieldOfView(uint8_t* base, uint32_t camera, double degrees) {
  ITC_NATIVE_RUNNING("sub_82593278 Camera::SetFieldOfView");
  const double radians = Fmuls(degrees, Lfs(base, kRadiansPerDegree));
  Stfs(base, camera + 368, WrapAngle(base, radians));
}

void BuildProjection(uint8_t* base, uint32_t camera, uint32_t fov, uint32_t aspect, uint32_t far,
                     uint32_t near) {
  ITC_NATIVE_RUNNING("sub_825932E8 Camera::BuildProjection");
  const uint32_t p = camera + 80;
  const simde__m128 zero = simde_mm_setzero_ps();
  // vupkd3d128's (3.0, 3.0, 0.0, 1.0), turned into (0, 0, 0, 1).
  simde__m128 v13 = simde_mm_castsi128_ps(
      simde_mm_shuffle_epi32(simde_mm_castps_si128(simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f)), 0x54));
  Stvx(base, p, zero);
  Stvx(base, p + 16, zero);
  Stvx(base, p + 48, zero);
  const uint32_t one = Bits(Lfs(base, kOne));
  Stvx(base, p + 32, v13);
  const double half = Fmuls(Lfs(base, fov), Lfs(base, kHalf));
  const double a = Lfs(base, aspect);
  const double x = WrapAngle(base, half);
  const double x2 = Fmuls(x, x);
  const double x3 = Fmuls(x2, x);
  const double n = Lfs(base, near);
  const double f = Lfs(base, far);
  // The polynomials: (1, x, x^2, x^3) and its powers dotted with the
  // coefficients, the translation's steps in its order.
  simde__m128 v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;
  {
    FlushToZero flush;
    v11 = VectorOf(one, Bits(x), Bits(x2), Bits(x3));
    v12 = Mul(v11, v11);
    v9 = Splat<0x55>(v11);
    v11 = Splat<0xAA>(v11);
    v6 = Lvx(base, 0x82000A00);
    v8 = Splat<0x00>(v12);
    v10 = Mul(v12, v11);
    v5 = Lvx(base, 0x820009D0);
    v11 = Mul(v8, v9);
    v4 = Lvx(base, 0x82000A10);
    v3 = Lvx(base, 0x820009E0);
    v9 = Mul(v12, v11);
    v2 = Lvx(base, 0x820009F0);
    v8 = Mul(v10, v11);
    v7 = Mul(v9, v11);
    v6 = Dot(v9, v6);
    v11 = Mul(v8, v11);
    v8 = Dot(v8, v5);
    v9 = Lvx(base, 0x820009C0);
    v7 = Dot(v7, v4);
    v12 = Dot(v12, v2);
    v10 = Dot(v10, v9);
  }
  const double depth = Fdivs(n, Fsubs(n, f));
  simde__m128 v12b;
  {
    FlushToZero flush;
    v12b = Dot(v11, v3);
  }
  const double w = Fneg(Fmuls(f, depth));
  const double sine_sum = Fadds(Fadds(Word0(v7), Word0(v6)), Word0(v12));
  const double cosine_sum = Fadds(Fadds(Word0(v12b), Word0(v8)), Word0(v10));
  const double y_scale = Fdivs(sine_sum, cosine_sum);
  const double x_scale = Fdivs(y_scale, a);
  // The four scales put into the rows (vrlimi128).
  const simde__m128 s = VectorOf(Bits(x_scale), Bits(y_scale), Bits(depth), Bits(w));
  const simde__m128 row0 = simde_mm_blend_ps(zero, simde_mm_permute_ps(s, 228), 8);
  const simde__m128 row1 = simde_mm_blend_ps(zero, simde_mm_permute_ps(s, 228), 4);
  v13 = simde_mm_blend_ps(v13, simde_mm_permute_ps(s, 228), 2);
  const simde__m128 row3 = simde_mm_blend_ps(zero, simde_mm_permute_ps(s, 147), 2);
  Stvx(base, p, row0);
  Stvx(base, p + 16, row1);
  Stvx(base, p + 32, v13);
  Stvx(base, p + 48, row3);
}

void SetViewportSize(uint8_t* base, uint32_t camera, uint32_t width, uint32_t height) {
  ITC_NATIVE_RUNNING("sub_82593500 Camera::SetViewportSize");
  const double w = double(float(double(int64_t(uint64_t(width)))));
  Stfs(base, camera + 376, w);
  const double h = double(float(double(int64_t(uint64_t(height)))));
  Stfs(base, camera + 380, h);
  Stfs(base, camera + 392, Fdivs(w, h));
  BuildProjection(base, camera, camera + 368, camera + 392, camera + 388, camera + 384);
}

namespace {

simde__m128 Unpackhi(simde__m128 a, simde__m128 b) {
  return simde_mm_castsi128_ps(
      simde_mm_unpackhi_epi32(simde_mm_castps_si128(a), simde_mm_castps_si128(b)));
}
simde__m128 Unpacklo(simde__m128 a, simde__m128 b) {
  return simde_mm_castsi128_ps(
      simde_mm_unpacklo_epi32(simde_mm_castps_si128(a), simde_mm_castps_si128(b)));
}

}  // namespace

void Update(uint8_t* base, uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82593568 Camera::Update");
  // The identity at frame + 112 (vupkd3d128's (0, 0, 0, 1) shuffled into
  // each row), turned by the negated angles.
  const uint32_t m = frame + 112;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, m + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
  const uint32_t angle = frame + 80, turn = frame - 80;
  Stfs(base, angle, Fneg(Lfs(base, camera + 292)));
  matrix_math::RotateY(base, m, m, angle, turn);
  Stfs(base, angle, Fneg(Lfs(base, camera + 288)));
  matrix_math::RotateX(base, m, m, angle, turn);
  Stfs(base, angle, Fneg(Lfs(base, camera + 296)));
  matrix_math::RotateZ(base, m, m, angle, turn);
  // The position (+272) dotted with the turned matrix's columns, as the
  // translation's merges and dot products do it, at frame + 96; its x, y
  // and z negated into the fourth row.
  {
    simde__m128 v9 = Lvx(base, m + 48), v11 = Lvx(base, m + 32), v8 = Lvx(base, m + 16),
                v10 = Lvx(base, m);
    simde__m128 v12 = Unpackhi(v9, v8);
    simde__m128 v13 = Unpackhi(v11, v10);
    v11 = Unpacklo(v11, v10);
    v10 = Unpacklo(v9, v8);
    v9 = Unpackhi(v12, v13);
    v13 = Unpacklo(v12, v13);
    simde__m128 v0 = Lvx(base, camera + 272);
    v12 = Unpackhi(v10, v11);
    v11 = Unpacklo(v10, v11);
    {
      FlushToZero flush;
      v10 = Dot(v0, v9);
      v13 = Dot(v0, v13);
      v12 = Dot(v0, v12);
      v0 = Dot(v0, v11);
    }
    v12 = Unpackhi(v12, v10);
    v0 = Unpackhi(v0, v13);
    v0 = Unpackhi(v0, v12);
    Stvx(base, frame + 96, v0);
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    Stfs(base, m + 48 + i, Fneg(Lfs(base, frame + 96 + i)));
  }
  matrix_math::Copy(base, camera + 208, m);
  interpolation::BlendState_OnCopy(base, camera + 208, m, 0x825936C4);
  matrix_math::Copy(base, camera + 16, camera + 208);
  interpolation::BlendState_OnCopy(base, camera + 16, camera + 208, 0x825936D4);
  Stfs(base, camera + 392, Fdivs(Lfs(base, camera + 376), Lfs(base, camera + 380)));
  BuildProjection(base, camera, camera + 368, camera + 392, camera + 388, camera + 384);
  matrix_math::Multiply(base, camera + 144, camera + 16, camera + 80);
  interpolation::BlendState_OnMultiply(base, camera + 144, camera + 16, camera + 80, 0x82593798);
  // +372: the half field of view's polynomials, as BuildProjection's (the
  // translation's steps in its order; the original's stack words held here).
  const double half = Lfs(base, kHalf);
  const uint32_t one = Bits(Lfs(base, kOne));
  const double x = WrapAngle(base, Fmuls(Lfs(base, camera + 368), half));
  const double x2 = Fmuls(x, x);
  const double x3 = Fmuls(x2, x);
  const simde__m128 ca00 = Lvx(base, 0x82000A00), ca10 = Lvx(base, 0x82000A10),
                    c9f0 = Lvx(base, 0x820009F0), c9c0 = Lvx(base, 0x820009C0),
                    c9d0 = Lvx(base, 0x820009D0), c9e0 = Lvx(base, 0x820009E0);
  simde__m128 s80, s96, s176, s192, s208, s224;
  {
    FlushToZero flush;
    simde__m128 v13 = VectorOf(one, Bits(x), Bits(x2), Bits(x3));
    simde__m128 v0 = Mul(v13, v13);
    const simde__m128 v5 = Splat<0x55>(v13);
    v13 = Splat<0xAA>(v13);
    simde__m128 v12 = Mul(v0, v13);
    s208 = Dot(v0, c9f0);
    const simde__m128 v4 = Splat<0x00>(v0);
    s192 = Dot(v12, c9c0);
    v13 = Mul(v4, v5);
    v0 = Mul(v0, v13);
    v12 = Mul(v12, v13);
    const simde__m128 v9 = Mul(v0, v13);
    s96 = Dot(v0, ca00);
    v13 = Mul(v12, v13);
    s224 = Dot(v12, c9d0);
    s176 = Dot(v13, c9e0);
    s80 = Dot(v9, ca10);
  }
  double f13 = Fadds(Word0(s80), Word0(s96));
  f13 = Fadds(f13, Word0(s208));
  double f12 = Fadds(Word0(s176), Word0(s224));
  const double h = Fmuls(Lfs(base, camera + 380), half);
  f12 = Fadds(f12, Word0(s192));
  Stfs(base, camera + 372, Fmuls(Fdivs(h, f12), f13));
}

void Construct(uint8_t* base, uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82593908 Camera::Camera");
  const double zero = Lfs(base, 0x82000A34);
  const double one = Lfs(base, kOne);
  guest::StoreU32(base, camera, 0x82001EC0);
  for (uint32_t at = 28; at <= 364; at += 16) {
    Stfs(base, camera + at, one);
  }
  // The position and the angles (through the original's stack, as words).
  const uint32_t position[4] = {Bits(zero), Bits(Lfs(base, 0x8200277C)),
                                Bits(Lfs(base, 0x82008E48)), Bits(one)};
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, camera + 272 + 4 * i, position[i]);
    guest::StoreU32(base, camera + 288 + 4 * i, i == 3 ? Bits(one) : Bits(zero));
  }
  Stfs(base, camera + 384, Lfs(base, 0x82001FBC));
  Stfs(base, camera + 388, Lfs(base, 0x82001FC8));
  Stfs(base, camera + 376, Lfs(base, 0x82009EB8));
  Stfs(base, camera + 380, Lfs(base, 0x8200B5CC));
  Stfs(base, camera + 392, Lfs(base, 0x82052CFC));
  Stfs(base, camera + 368, WrapAngle(base, Lfs(base, 0x82001FC4)));
  BuildProjection(base, camera, camera + 368, camera + 392, camera + 388, camera + 384);
  Update(base, camera, frame - 320);
}

uint32_t Mode(uint8_t* base, int32_t which) {
  ITC_NATIVE_RUNNING("sub_8209F718 CameraPasses::Mode");
  return which == 1 ? guest::LoadU32(base, kMode) : 0xFFFFFFFFu;
}

bool SetMode(uint8_t* base, int32_t which, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8209F738 CameraPasses::SetMode");
  if (which != 1) {
    return true;
  }
  const uint32_t pass = Pass(4);
  const bool sets_view = mode == 0 || mode == 1;
  if (sets_view && guest::LoadU32(base, guest::LoadU32(base, pass) + 4) != 0x82593278) {
    return false;
  }
  guest::StoreU32(base, kMode, mode);
  if (sets_view) {
    SetFieldOfView(base, pass, Lfs(base, mode == 0 ? 0x82001E8C : 0x82001E88));
  }
  return true;
}

uint32_t RenderTarget(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209F7A8 CameraPasses::RenderTarget");
  static constexpr uint32_t kTextures[] = {
      0x826BABE8, 0, 0x826BABDC, 0x826BABC8, 0x826BABD4, 0x826BABFC, 0x826BABF8,
      0x826BABF4, 0x826BABF0, 0x826BABD0, 0x826BABD8, 0x826BABE0, 0x826BABC4,
  };
  if (id > 12) {
    return 0;
  }
  if (id == 1) {
    const uint32_t mode = guest::LoadU32(base, kMode);
    return mode == 0 ? guest::LoadU32(base, 0x826BABEC)
           : mode == 1 ? guest::LoadU32(base, 0x826BABCC)
                       : 0;
  }
  return guest::LoadU32(base, kTextures[id]);
}

void Enable(uint8_t* base, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820A0968 CameraPass::Enable");
  guest::StoreU32(base, pass + 588, 1);
}

void Disable(uint8_t* base, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820A0958 CameraPass::Disable");
  guest::StoreU32(base, pass + 588, 0);
}

uint32_t EnableForView(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820AA1A8 CameraPasses::EnableForView");
  if (guest::LoadU32(base, guest::LoadU32(base, kViewports) + 8) == 0) {
    for (uint32_t i : {0, 10, 11, 12, 19, 20}) {
      Enable(base, Pass(i));
    }
    const uint32_t viewport = guest::LoadU32(base, guest::LoadU32(base, kViewports));
    const uint32_t view_mode = guest::LoadU32(base, viewport + 1860);
    if (view_mode == 2 || view_mode == 1) {
      Enable(base, Pass(3));
    }
  } else {
    for (uint32_t i : {1, 2, 13, 14, 15, 16, 17, 18, 20}) {
      Enable(base, Pass(i));
    }
  }
  if (Mode(base, 1) == 1) {
    for (uint32_t i = 4; i <= 9; i++) {
      Enable(base, Pass(i));
    }
    return Pass(9);
  }
  Enable(base, Pass(4));
  return Pass(4);
}

void SetNear(uint8_t* base, double near) {
  ITC_NATIVE_RUNNING("sub_820AA368 CameraPasses::SetNear");
  Stfs(base, Pass(0) + 384, near);
}

bool ReleaseSurfaces(uint8_t* base, uint32_t pass,
                     const std::function<void(uint32_t surface, uint32_t field)>& release) {
  ITC_NATIVE_RUNNING("sub_820A0C40 CameraPass::ReleaseSurfaces");
  const int32_t type = int32_t(guest::LoadU32(base, pass + 584));
  if (type < 3 || (type > 4 && uint32_t(type - 10) > 10)) {
    return false;
  }
  for (uint32_t field : {576u, 580u}) {
    const uint32_t surface = guest::LoadU32(base, pass + field);
    if (surface != 0) {
      release(surface, field);
      guest::StoreU32(base, pass + field, 0);
    }
  }
  return true;
}

void FromViewport(uint8_t* base, uint32_t pass, uint32_t player, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_820AA4A8 CameraPass::FromViewport");
  const uint32_t at =
      guest::LoadU32(base, guest::LoadU32(base, kViewports) + (player << 2)) + entry * 80;
  for (uint32_t i = 0; i < 48; i += 8) {
    const uint32_t high = guest::LoadU32(base, at + 16 + i);
    const uint32_t low = guest::LoadU32(base, at + 20 + i);
    guest::StoreU32(base, pass + 400 + i, high);
    guest::StoreU32(base, pass + 404 + i, low);
  }
  PassSetZoom(base, pass, Lfs(base, at + 68));
  guest::StoreU32(base, pass + 388, Through(guest::LoadU32(base, at + 72)));
  guest::StoreU32(base, pass + 448, Through(guest::LoadU32(base, at + 64)));
}

void PassSetZoom(uint8_t* base, uint32_t pass, double zoom) {
  ITC_NATIVE_RUNNING("sub_820AA378 CameraPass::SetZoom");
  Stfs(base, pass + 516, zoom);
  const int32_t height = int32_t(guest::LoadU32(base, pass + 508));
  const double h = double(float(double(int64_t(height))));
  const double ratio = Fdivs(Fmuls(h, Lfs(base, kHalf)), zoom);
  const double angle = double(float(c_runtime::Atan(ratio)));
  Stfs(base, pass + 368, WrapAngle(base, angle));
}

bool ReleaseViewports(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820AA430 Viewports::Release");
  const uint32_t block = guest::LoadU32(base, kViewports);
  if (!memory::IsPages(block) || !memory::IsPages(guest::LoadU32(base, block + 0)) ||
      !memory::IsPages(guest::LoadU32(base, block + 4))) {
    return false;
  }
  for (uint32_t i = 0; i < 8; i += 4) {
    memory::FreePages(guest::LoadU32(base, guest::LoadU32(base, kViewports) + i));
  }
  memory::FreePages(guest::LoadU32(base, kViewports));
  guest::StoreU32(base, kViewports, 0);
  return true;
}

void ViewportSetZoom(uint8_t* base, uint32_t viewport, double zoom) {
  ITC_NATIVE_RUNNING("sub_820AAE90 Viewport::SetZoom");
  const uint32_t size = guest::LoadU32(base, 0x82001EF0);
  const double half = Fmuls(double(float(double(int64_t(uint64_t(size))))), Lfs(base, kHalf));
  const double angle = double(float(c_runtime::Atan(half)));
  Stfs(base, viewport + 68, zoom);
  Stfs(base, viewport + 148, zoom);
  const double fixed = Fmuls(angle, Lfs(base, 0x82002808));
  for (uint32_t entry = 2; entry <= 7; entry++) {
    Stfs(base, viewport + 80 * entry + 68, fixed);
  }
  for (uint32_t entry = 8; entry <= 17; entry++) {
    Stfs(base, viewport + 80 * entry + 68, zoom);
  }
  const double inverse = Fdivs(Lfs(base, kOne), Lfs(base, viewport + 1856));
  Stfs(base, viewport + 1508, Fmuls(inverse, Lfs(base, 0x82002800)));
  Stfs(base, viewport + 1588, zoom);
}

void ViewportSetRoll(uint8_t* base, uint32_t viewport, double angle) {
  ITC_NATIVE_RUNNING("sub_820AA2D0 Viewport::SetRoll");
  const double pi = Lfs(base, 0x82002240);
  double x = Fadds(angle, pi);
  const double two_pi = Lfs(base, kTwoPi);
  while (x > two_pi) {
    x = Fsubs(x, two_pi);
  }
  const double zero = Lfs(base, 0x82000A34);
  while (x < zero) {
    x = Fadds(x, two_pi);
  }
  x = Fsubs(x, pi);
  Stfs(base, viewport + 64, x);
  Stfs(base, viewport + 144, Fneg(x));
  for (uint32_t entry = 2; entry <= 17; entry++) {
    Stfs(base, viewport + 80 * entry + 64, x);
  }
  Stfs(base, viewport + 1584, x);
}

std::optional<uint32_t> RecordViewport(uint8_t* base, uint32_t list, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820A0978 CameraPass::RecordViewport");
  // What the original's command list calls would print about: another chunk
  // open (OpenChunk), or the list full on any of its four allocations.
  if (guest::LoadU8(base, list + 52) == 0 && guest::LoadU32(base, command_list::kOpenChunks) != 0) {
    return std::nullopt;
  }
  if (guest::LoadU32(base, list + 12) + 4 >= guest::LoadU32(base, list + 8) ||
      guest::LoadU32(base, list + 12) + 4 < guest::LoadU32(base, list + 12)) {
    // (each allocation compares the count so far; the last one is the
    // largest unless the count wraps, which is left to the original too)
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  const uint32_t chunk = slot();
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  uint32_t s = slot();
  guest::StoreU32(base, s + 8, 0);
  guest::StoreU32(base, s + 0, 30);
  guest::StoreU32(base, s + 4, 1);
  s = slot();
  guest::StoreU16(base, s + 0, uint16_t(guest::LoadU32(base, pass + 496)));
  guest::StoreU16(base, s + 2, uint16_t(guest::LoadU32(base, pass + 500)));
  guest::StoreU16(base, s + 4, uint16_t(guest::LoadU32(base, pass + 504)));
  const uint32_t height = guest::LoadU32(base, pass + 508);
  Stfs(base, s + 8, Lfs(base, 0x82000A34));
  Stfs(base, s + 12, Lfs(base, kOne));
  guest::StoreU16(base, s + 6, uint16_t(height));
  s = slot();
  guest::StoreU32(base, s + 4, 0);
  guest::StoreU32(base, s + 0, 36);
  guest::StoreU32(base, s + 8, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

std::optional<uint32_t> RecordClear(uint8_t* base, uint32_t list, uint32_t flags, uint32_t color,
                                    uint32_t depth) {
  ITC_NATIVE_RUNNING("sub_8209FA38 RecordClear");
  // Another chunk open, or the list full on any allocation (1, 1, 18 and 1
  // slots: the last the largest unless the count wraps, left to the
  // original too).
  if (guest::LoadU8(base, list + 52) == 0 && guest::LoadU32(base, command_list::kOpenChunks) != 0) {
    return std::nullopt;
  }
  const uint32_t used = guest::LoadU32(base, list + 12);
  if (used + 21 >= guest::LoadU32(base, list + 8) || used + 21 < used) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  const uint32_t chunk = command_list::AllocateSlots(base, list, 1);
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  uint32_t s = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, s + 8, 0);
  guest::StoreU32(base, s + 0, 26);
  guest::StoreU32(base, s + 4, 1);
  s = command_list::AllocateBytes(base, list, 280, false);
  guest::StoreU32(base, s + 8, flags);
  guest::StoreU32(base, s + 268, color);
  guest::StoreU32(base, s + 276, 0);
  guest::StoreU32(base, s + 4, 0);
  guest::StoreU32(base, s + 0, 0);
  Stfs(base, s + 272, double(float(double(int64_t(uint64_t(depth))))));
  s = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, s + 4, 0);
  guest::StoreU32(base, s + 0, 36);
  guest::StoreU32(base, s + 8, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

bool PassUpdate(uint8_t* base, uint32_t pass, uint32_t frame,
                const std::function<void(uint32_t key)>& record_viewport) {
  ITC_NATIVE_RUNNING("sub_820A3978 CameraPass::Update");
  if (guest::LoadU32(base, guest::LoadU32(base, pass)) != 0x82593500) {
    return false;
  }
  const uint32_t type = guest::LoadU32(base, pass + 584);
  if (type <= 2) {
    record_viewport(type == 2 ? 371 : 150);
  }
  SetViewportSize(base, pass, guest::LoadU32(base, pass + 504), guest::LoadU32(base, pass + 508));
  if ((type >= 10 && type <= 18) || type == 20) {
    return true;
  }
  // The forward, target - eye, with w 1.0, into +464 (through the stack, as
  // the original copies it).
  const uint32_t one = Bits(Lfs(base, kOne));
  const double fx = Fsubs(Lfs(base, pass + 416), Lfs(base, pass + 400));
  const double fy = Fsubs(Lfs(base, pass + 420), Lfs(base, pass + 404));
  const double fz = Fsubs(Lfs(base, pass + 424), Lfs(base, pass + 408));
  guest::StoreU32(base, pass + 464, Bits(fx));
  guest::StoreU32(base, pass + 468, Bits(fy));
  guest::StoreU32(base, pass + 472, Bits(fz));
  guest::StoreU32(base, pass + 476, one);
  simde__m128 forward;
  {
    FlushToZero flush;
    Stvx(base, pass + 464, Normalize(Lvx(base, pass + 464)));
  }
  // The normalized forward as the stack copy reads it back (w normalized
  // too), then +476 = 1.0.
  forward = VectorOf(guest::LoadU32(base, pass + 464), guest::LoadU32(base, pass + 468),
                     guest::LoadU32(base, pass + 472), guest::LoadU32(base, pass + 476));
  guest::StoreU32(base, pass + 476, one);
  const simde__m128 up = Lvx(base, pass + 432);
  const uint32_t zero = Bits(Lfs(base, 0x82000A34));
  simde__m128 right, up2;
  {
    FlushToZero flush;
    right = NegMulSub(Shuffle<0x78>(up), Shuffle<0x9C>(forward),
                      Mul(Shuffle<0x9C>(up), Shuffle<0x78>(forward)));
    right = Normalize(right);
    forward = Normalize(forward);
    up2 = NegMulSub(Shuffle<0x78>(forward), Shuffle<0x9C>(right),
                    Mul(Shuffle<0x9C>(forward), Shuffle<0x78>(right)));
  }
  // -eye (each word negated through a float register).
  uint32_t eye[4];
  for (uint32_t i = 0; i < 4; i++) {
    eye[i] = Bits(Fneg(Lfs(base, pass + 400 + 4 * i)));
  }
  const simde__m128 minus_eye = VectorOf(eye[0], eye[1], eye[2], eye[3]);
  simde__m128 dots[3];
  {
    FlushToZero flush;
    dots[0] = Dot3(minus_eye, right);
    dots[1] = Dot3(minus_eye, up2);
    dots[2] = Dot3(minus_eye, forward);
  }
  // The view at frame + 128: the right, up and forward as columns, -eye's
  // dots as the fourth row.
  const uint32_t view = frame + 128;
  const simde__m128 columns[3] = {right, up2, forward};
  for (uint32_t row = 0; row < 3; row++) {
    for (uint32_t column = 0; column < 3; column++) {
      guest::StoreU32(base, view + 16 * row + 4 * column, Through(WordOf(columns[column], int(row))));
    }
    guest::StoreU32(base, view + 16 * row + 12, zero);
  }
  for (uint32_t column = 0; column < 3; column++) {
    guest::StoreU32(base, view + 48 + 4 * column, Through(WordOf(dots[column], 0)));
  }
  guest::StoreU32(base, view + 60, one);
  // Copied to +16 and turned by the roll, as the original's calls (noted for
  // interpolation).
  const uint32_t view_out = pass + 16;
  matrix_math::Copy(base, view_out, view);
  interpolation::BlendState_OnCopy(base, view_out, view, 0x820A3D18);
  matrix_math::RotateZ(base, view_out, view_out, pass + 448, frame - 80);
  // +480: the up again, from the forward at +464 (w 1.0) and +432.
  {
    FlushToZero flush;
    const simde__m128 f = Lvx(base, pass + 464);
    const simde__m128 u = Lvx(base, pass + 432);
    Stvx(base, pass + 480,
         NegMulSub(Shuffle<0x78>(u), Shuffle<0x9C>(f), Mul(Shuffle<0x9C>(u), Shuffle<0x78>(f))));
    Stvx(base, pass + 480, Normalize(Lvx(base, pass + 480)));
  }
  matrix_math::Multiply(base, pass + 144, view_out, pass + 80);
  interpolation::BlendState_OnMultiply(base, pass + 144, view_out, pass + 80, 0x820A3D90);
  return true;
}

}  // namespace cameras
