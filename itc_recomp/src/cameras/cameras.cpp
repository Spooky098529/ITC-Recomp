#include "cameras/cameras.h"

#include "guest.h"

#include "base/log.h"

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

}  // namespace cameras
