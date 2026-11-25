#include "matrix_math/matrix_math.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "camera_modes/camera_modes.h"
#include "interpolation/blend_state.h"
#include "schema/matrix_math.h"
#include "schema/vehicle_accessors.h"
#include "vehicle_accessors/vectors.h"

#include "base/float_mode.h"
#include <simde/x86/avx.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>

namespace matrix_math {

namespace {

using Fpscr = ::base::FloatMode;
using schema::Matrix;
using schema::MatrixMathFloat;
using schema::MatrixMathVector3;
using schema::Quaternion;
using schema::VectorLibVector;
using G = schema::MatrixMathGlobals;

// --- Scalar steps, as the translated code's doubles ---

double Lfs(uint32_t address) {
  return double(guest::LoadFloat(address));
}
void Stfs(uint32_t address, double value) {
  guest::StoreFloat(address, float(value));
}
// lfs then stfs of one word: its bits.
void CopyWord(uint32_t from, uint32_t to) {
  guest::StoreU32(to, guest::LoadU32(from));
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
double Fmadds(double a, double b, double c) {
  return double(float(std::fma(a, b, c)));
}
double Fmsubs(double a, double b, double c) {
  return double(float(std::fma(a, b, -c)));
}
double Fnmsubs(double a, double b, double c) {
  return double(float(-std::fma(a, b, -c)));
}

// --- The vector unit, as the translation does it ---

// Vectors as the translated code holds them: word 0 in the top lane.
simde__m128 VectorOf(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int32_t(w0), int32_t(w1), int32_t(w2), int32_t(w3)));
}
simde__m128 VectorAt(uint32_t address) {
  return VectorOf(guest::LoadU32(address + 0), guest::LoadU32(address + 4),
                  guest::LoadU32(address + 8), guest::LoadU32(address + 12));
}
// Word i (0 the top lane).
uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}
void StoreVector(uint32_t address, simde__m128 v) {
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(address + 4 * i, WordOf(v, i));
  }
}
simde__m128i I(simde__m128 v) {
  return simde_mm_castps_si128(v);
}
simde__m128 F(simde__m128i v) {
  return simde_mm_castsi128_ps(v);
}

// Flush-to-zero on while alive (the vector unit's mode), put back after.
class FlushToZero {
 public:
  FlushToZero() : csr_(Fpscr::getcsr()) { Fpscr::setcsr(csr_ | uint32_t(Fpscr::FlushMask)); }
  ~FlushToZero() { Fpscr::setcsr(csr_); }

 private:
  uint32_t csr_;
};

// A 4x4 matrix's 16 words (row-major).
struct Words {
  uint32_t w[16];
};

Words LoadWords(uint32_t address) {
  Words m;
  for (uint32_t i = 0; i < 16; i++) {
    m.w[i] = guest::LoadU32(address + 4 * i);
  }
  return m;
}

// vmsum4fp128 of a row (or vector, first) with each column of b: one
// element each, as the translated dpps (flush-to-zero on: the caller's).
uint32_t DotColumn(simde__m128 row, const Words& b, int j) {
  const simde__m128 column = VectorOf(b.w[j], b.w[4 + j], b.w[8 + j], b.w[12 + j]);
  return WordOf(simde_mm_dp_ps(row, column, 0xFF), 0);
}

// a * b's 16 words.
Words Product(const Words& a, const Words& b) {
  Words out;
  for (int i = 0; i < 4; i++) {
    const simde__m128 row = VectorOf(a.w[4 * i], a.w[4 * i + 1], a.w[4 * i + 2], a.w[4 * i + 3]);
    for (int j = 0; j < 4; j++) {
      out.w[4 * i + j] = DotColumn(row, b, j);
    }
  }
  return out;
}

// Matrix::RotateX, Y and Z (sub_820A65F0, sub_820A67F8, sub_820A2D30): the
// same code but for which rows of the turn the sine and cosine go in (the
// vpermwi128 immediates, as x86 shuffles in the translation) and the return
// address of their call to Matrix::Multiply.
enum class Axis { kX, kY, kZ };

template <Axis kAxis>
void Rotate(uint32_t out, uint32_t in, uint32_t angle, uint32_t turn) {
  // The angle into [-pi, pi]: pi with the angle's sign (its bits ORed into
  // 0x40490FDB, from the stored float), added, times 1 / (2 pi), truncated
  // (fctiwz: NaN gives INT_MIN, INT_MAX and above INT_MAX), that many 2 pi
  // taken off (fnmsubs).
  double x = double(MatrixMathFloat(angle).value());
  const double two_pi = double(G::two_pi());
  const uint32_t one = std::bit_cast<uint32_t>(G::one());
  const uint32_t stored = std::bit_cast<uint32_t>(float(x));
  const double pi = double(std::bit_cast<float>((stored & 0x80000000u) | 0x40490FDBu));
  const double turns = Fmuls(Fadds(pi, x), double(G::inverse_two_pi()));
  int32_t n;
  if (std::isnan(turns)) {
    n = INT32_MIN;
  } else if (turns >= double(INT32_MAX)) {
    n = INT32_MAX;
  } else {
    n = simde_mm_cvttsd_si32(simde_mm_set_sd(turns));
  }
  x = Fnmsubs(double(float(double(int64_t(n)))), two_pi, x);
  const double x2 = Fmuls(x, x);
  const double x3 = Fmuls(x2, x);
  // The polynomials: (1, x, x^2, x^3) and its powers dotted with the
  // coefficients at 0x820009C0-0x82000A1F (vector unit, flush-to-zero on).
  const simde__m128 c9c0 = VectorAt(G::turn_polynomials_address() + 0);
  const simde__m128 c9d0 = VectorAt(G::turn_polynomials_address() + 16);
  const simde__m128 c9e0 = VectorAt(G::turn_polynomials_address() + 32);
  const simde__m128 c9f0 = VectorAt(G::turn_polynomials_address() + 48);
  const simde__m128 ca00 = VectorAt(G::turn_polynomials_address() + 64);
  const simde__m128 ca10 = VectorAt(G::turn_polynomials_address() + 80);
  simde__m128 v0, v3, v4, v9, v10, v11, v12, v13, d112;
  {
    FlushToZero flush;
    v13 = VectorOf(one, std::bit_cast<uint32_t>(float(x)), std::bit_cast<uint32_t>(float(x2)),
                   std::bit_cast<uint32_t>(float(x3)));
    v0 = simde_mm_mul_ps(v13, v13);
    v4 = F(simde_mm_shuffle_epi32(I(v13), 0x55));
    v13 = F(simde_mm_shuffle_epi32(I(v13), 0xAA));
    v12 = simde_mm_mul_ps(v0, v13);
    d112 = simde_mm_dp_ps(v0, c9f0, 0xFF);
    v3 = F(simde_mm_shuffle_epi32(I(v0), 0x0));
    v11 = simde_mm_dp_ps(v12, c9c0, 0xFF);
    v13 = simde_mm_mul_ps(v3, v4);
    v0 = simde_mm_mul_ps(v0, v13);
    v12 = simde_mm_mul_ps(v12, v13);
    v10 = simde_mm_dp_ps(v0, ca00, 0xFF);
    const simde__m128 v11b = simde_mm_mul_ps(v0, v13);
    v9 = simde_mm_dp_ps(v12, c9d0, 0xFF);
    v0 = simde_mm_mul_ps(v12, v13);
    v13 = simde_mm_dp_ps(v11b, ca10, 0xFF);
  }
  const double f13 = Fadds(double(std::bit_cast<float>(WordOf(v13, 0))),
                           double(std::bit_cast<float>(WordOf(v10, 0))));
  simde__m128 d112b;
  {
    FlushToZero flush;
    d112b = simde_mm_dp_ps(v0, c9e0, 0xFF);
  }
  auto word0 = [](simde__m128 v) { return double(std::bit_cast<float>(WordOf(v, 0))); };
  const uint32_t zero = std::bit_cast<uint32_t>(G::zero());
  const double cosine = Fadds(f13, word0(d112));
  const double sine = Fadds(Fadds(word0(d112b), word0(v9)), word0(v11));
  const uint32_t c = std::bit_cast<uint32_t>(float(cosine));
  const uint32_t s = std::bit_cast<uint32_t>(float(sine));
  const uint32_t minus_s = std::bit_cast<uint32_t>(float(-sine));
  // (s, -s, c, 0), then the turn's rows from it and from vupkd3d128's
  // (3.0, 3.0, 0.0, 1.0) (the 0.0 and 1.0).
  const simde__m128i sc = I(VectorOf(s, minus_s, c, zero));
  const simde__m128i k = I(simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f));
  simde__m128i rows[4];
  if constexpr (kAxis == Axis::kZ) {
    rows[0] = simde_mm_shuffle_epi32(sc, 0x70);
    rows[1] = simde_mm_shuffle_epi32(sc, 0x90);
    rows[2] = simde_mm_shuffle_epi32(k, 0x51);
  } else if constexpr (kAxis == Axis::kX) {
    rows[0] = simde_mm_shuffle_epi32(k, 0x15);
    rows[1] = simde_mm_shuffle_epi32(sc, 0x1C);
    rows[2] = simde_mm_shuffle_epi32(sc, 0x24);
  } else {
    rows[0] = simde_mm_shuffle_epi32(sc, 0x48);
    rows[1] = simde_mm_shuffle_epi32(k, 0x45);
    rows[2] = simde_mm_shuffle_epi32(sc, 0xC4);
  }
  rows[3] = simde_mm_shuffle_epi32(k, 0x54);
  const uint32_t at = turn & ~0xFu;
  for (uint32_t i = 0; i < 4; i++) {
    StoreVector(at + 16 * i, F(rows[i]));
  }
  // out = in * the turn, noted for interpolation as the original's call.
  Multiply(out, in, turn);
  constexpr uint32_t kReturn = kAxis == Axis::kZ   ? 0x820A2F28
                               : kAxis == Axis::kX ? 0x820A67E8
                                                   : 0x820A69F0;
  interpolation::BlendState_OnMultiply(out, in, turn, kReturn);
}

}  // namespace

void Multiply(uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_8209BD50 Matrix::Multiply");
  const Words bw = LoadWords(b);
  const Words aw = LoadWords(a);
  Words product;
  {
    FlushToZero flush;
    product = Product(aw, bw);
  }
  const uint32_t at = out & ~0xFu;
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(at + 4 * i, product.w[i]);
  }
}

void Copy(uint32_t dst, uint32_t src) {
  ITC_NATIVE_RUNNING("sub_820A0AC0 Matrix::Copy");
  // Twice a word at a time, once eight bytes at a time (both words read
  // before either is written), then twice a word at a time again.
  for (int pass = 0; pass < 2; pass++) {
    for (uint32_t i = 0; i < 64; i += 4) {
      CopyWord(src + i, dst + i);
    }
  }
  for (uint32_t i = 0; i < 64; i += 8) {
    const uint32_t w0 = guest::LoadU32(src + i);
    const uint32_t w1 = guest::LoadU32(src + i + 4);
    guest::StoreU32(dst + i, w0);
    guest::StoreU32(dst + i + 4, w1);
  }
  for (int pass = 0; pass < 2; pass++) {
    for (uint32_t i = 0; i < 64; i += 4) {
      CopyWord(src + i, dst + i);
    }
  }
}

void Inverse(uint32_t out, uint32_t in) {
  ITC_NATIVE_RUNNING("sub_820A9988 Matrix::Inverse");
  // The translated code's steps on its vector registers, one statement each
  // (the names are the registers'); D3DX's inverse by cofactors. The
  // original's copies of the input and its transposes on its stack are
  // only scratch: the rows are read here once.
  simde__m128 rows[4];
  for (uint32_t i = 0; i < 4; i++) {
    rows[i] = VectorAt(in + 16 * i);
  }
  const simde__m128 sign = simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)));
  simde__m128 v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13;
  simde__m128 v22, v23, v24, v25, v26, v27, v28, v29, v30, v31;
  simde__m128 result[4];
  {
    FlushToZero flush;
    v3 = F(simde_mm_set1_epi32(0));
    v0 = rows[2];
    v12 = rows[0];
    v13 = F(simde_mm_unpackhi_epi32(I(v0), I(v12)));
    v6 = F(simde_mm_unpacklo_epi32(I(v0), I(v12)));
    v11 = rows[3];
    v10 = rows[1];
    v7 = F(simde_mm_unpackhi_epi32(I(v11), I(v10)));
    v12 = F(simde_mm_unpacklo_epi32(I(v11), I(v10)));
    v11 = F(simde_mm_unpackhi_epi32(I(v7), I(v13)));
    v10 = F(simde_mm_unpacklo_epi32(I(v7), I(v13)));
    v9 = F(simde_mm_unpackhi_epi32(I(v12), I(v6)));
    v8 = F(simde_mm_unpacklo_epi32(I(v12), I(v6)));
    v2 = F(simde_mm_shuffle_epi32(I(v10), 0x44));
    v4 = F(simde_mm_shuffle_epi32(I(v11), 0xFA));
    v1 = F(simde_mm_shuffle_epi32(I(v9), 0xFA));
    v31 = F(simde_mm_shuffle_epi32(I(v8), 0x44));
    v4 = simde_mm_mul_ps(v4, v2);
    v29 = F(simde_mm_shuffle_epi32(I(v10), 0xFA));
    v30 = F(simde_mm_shuffle_epi32(I(v9), 0x44));
    v2 = simde_mm_mul_ps(v1, v31);
    v31 = F(simde_mm_shuffle_epi32(I(v11), 0x44));
    v1 = F(simde_mm_shuffle_epi32(I(v8), 0xFA));
    v27 = F(simde_mm_shuffle_epi32(I(v10), 0x9E));
    v26 = F(simde_mm_shuffle_epi32(I(v11), 0x7B));
    v25 = F(simde_mm_shuffle_epi32(I(v8), 0x9E));
    v24 = F(simde_mm_shuffle_epi32(I(v9), 0x7B));
    v23 = F(simde_mm_shuffle_epi32(I(v9), 0x12));
    v4 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v31, v29), v4), sign);
    v0 = v6;
    v5 = v7;
    v0 = simde_mm_blend_ps(v0, simde_mm_permute_ps(v13, 78), 3);
    v5 = simde_mm_blend_ps(v5, simde_mm_permute_ps(v12, 78), 12);
    v12 = simde_mm_blend_ps(v12, simde_mm_permute_ps(v7, 78), 3);
    v7 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v30, v1), v2), sign);
    v13 = simde_mm_blend_ps(v13, simde_mm_permute_ps(v6, 78), 12);
    v0 = simde_mm_mul_ps(v0, v5);
    // vspltisw -1, then vslw by itself: 0x80000000 in each word.
    v1 = F(simde_mm_set1_epi32(int(0x80000000)));
    v29 = F(simde_mm_unpackhi_epi32(I(v1), I(v3)));
    v6 = F(simde_mm_shuffle_epi32(I(v7), 0x39));
    v28 = F(simde_mm_unpackhi_epi32(I(v3), I(v1)));
    v0 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v12, v13), v0), sign);
    v13 = v4;
    v12 = F(simde_mm_shuffle_epi32(I(v7), 0x63));
    v1 = v6;
    v5 = F(simde_mm_shuffle_epi32(I(v13), 0x63));
    v2 = v12;
    v4 = F(simde_mm_shuffle_epi32(I(v13), 0x39));
    v31 = v5;
    v30 = v4;
    v2 = simde_mm_blend_ps(v2, simde_mm_permute_ps(v0, 147), 8);
    v1 = simde_mm_blend_ps(v1, simde_mm_permute_ps(v0, 228), 4);
    v31 = simde_mm_blend_ps(v31, simde_mm_permute_ps(v0, 57), 8);
    v30 = simde_mm_blend_ps(v30, simde_mm_permute_ps(v0, 78), 4);
    v2 = simde_mm_mul_ps(v27, v2);
    v27 = F(simde_mm_shuffle_epi32(I(v10), 0x49));
    v1 = simde_mm_mul_ps(v26, v1);
    v26 = F(simde_mm_shuffle_epi32(I(v11), 0x12));
    v31 = simde_mm_mul_ps(v25, v31);
    v25 = F(simde_mm_shuffle_epi32(I(v8), 0x49));
    v6 = simde_mm_blend_ps(v6, simde_mm_permute_ps(v0, 147), 1);
    v22 = F(simde_mm_alignr_epi8(I(v7), I(v0), 8));
    v12 = simde_mm_blend_ps(v12, simde_mm_permute_ps(v0, 78), 2);
    v30 = simde_mm_mul_ps(v24, v30);
    v5 = simde_mm_blend_ps(v5, simde_mm_permute_ps(v0, 228), 2);
    v7 = F(simde_mm_shuffle_epi32(I(v7), 0xB1));
    v4 = simde_mm_blend_ps(v4, simde_mm_permute_ps(v0, 57), 1);
    v9 = F(simde_mm_shuffle_epi32(I(v9), 0x8D));
    v2 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v27, v6), v2), sign);
    v6 = F(simde_mm_unpacklo_epi32(I(v0), I(v13)));
    v1 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v26, v12), v1), sign);
    v12 = F(simde_mm_shuffle_epi32(I(v10), 0x33));
    v10 = F(simde_mm_shuffle_epi32(I(v22), 0xC7));
    v7 = simde_mm_blend_ps(v7, simde_mm_permute_ps(v0, 147), 9);
    v13 = F(simde_mm_shuffle_epi32(I(v13), 0xB1));
    v31 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v25, v4), v31), sign);
    // vupkd3d128 of zeros: (3.0, 3.0, 0.0, 1.0), word 0 first; only the
    // 1.0 (word 3) is used, splatted below.
    v4 = simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f);
    v9 = F(simde_mm_xor_si128(I(v9), I(v28)));
    v12 = F(simde_mm_xor_si128(I(v12), I(v29)));
    v8 = F(simde_mm_shuffle_epi32(I(v8), 0x33));
    v6 = F(simde_mm_shuffle_epi32(I(v6), 0xCB));
    v13 = simde_mm_blend_ps(v13, simde_mm_permute_ps(v0, 57), 9);
    v30 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v23, v5), v30), sign);
    v5 = F(simde_mm_shuffle_epi32(I(v11), 0x8D));
    v8 = F(simde_mm_xor_si128(I(v8), I(v29)));
    v0 = v13;
    v13 = F(simde_mm_shuffle_epi32(I(v4), 0x0));
    v10 = simde_mm_add_ps(simde_mm_mul_ps(v12, v10), v2);
    v5 = F(simde_mm_xor_si128(I(v5), I(v28)));
    v8 = simde_mm_add_ps(simde_mm_mul_ps(v8, v6), v31);
    v7 = simde_mm_add_ps(simde_mm_mul_ps(v5, v7), v1);
    v9 = simde_mm_add_ps(simde_mm_mul_ps(v9, v0), v30);
    v0 = v13;
    // The determinant, its reciprocal (vrefp: 1 / x) refined by two Newton
    // steps, the estimate kept where the refined one is NaN.
    v11 = simde_mm_dp_ps(v10, v11, 0xFF);
    v12 = simde_mm_div_ps(simde_mm_set1_ps(1), v11);
    v0 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v11, v12), v0), sign);
    v0 = simde_mm_add_ps(simde_mm_mul_ps(v12, v0), v12);
    v13 = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(v11, v0), v13), sign);
    v11 = simde_mm_cmpeq_ps(v0, v0);
    v0 = simde_mm_add_ps(simde_mm_mul_ps(v0, v13), v0);
    v0 = F(simde_mm_or_si128(simde_mm_andnot_si128(I(v11), I(v12)),
                             simde_mm_and_si128(I(v11), I(v0))));
    result[0] = simde_mm_mul_ps(v10, v0);
    result[1] = simde_mm_mul_ps(v7, v0);
    result[2] = simde_mm_mul_ps(v8, v0);
    result[3] = simde_mm_mul_ps(v9, v0);
  }
  const uint32_t at = out & ~0xFu;
  for (uint32_t i = 0; i < 4; i++) {
    StoreVector(at + 16 * i, result[i]);
  }
}

void RotateX(uint32_t out, uint32_t in, uint32_t angle, uint32_t turn) {
  ITC_NATIVE_RUNNING("sub_820A65F0 Matrix::RotateX");
  Rotate<Axis::kX>(out, in, angle, turn);
}

void RotateY(uint32_t out, uint32_t in, uint32_t angle, uint32_t turn) {
  ITC_NATIVE_RUNNING("sub_820A67F8 Matrix::RotateY");
  Rotate<Axis::kY>(out, in, angle, turn);
}

void RotateZ(uint32_t out, uint32_t in, uint32_t angle, uint32_t turn) {
  ITC_NATIVE_RUNNING("sub_820A2D30 Matrix::RotateZ");
  Rotate<Axis::kZ>(out, in, angle, turn);
}

void ObjectInverse(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820A9BC8 ObjectInverse");
  Inverse(object + 560, object + 208);
}

void Vec4Transform(uint32_t out, uint32_t m, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_820AB560 Vec4::Transform");
  const Words mw = LoadWords(m);
  const simde__m128 vector = VectorAt(v & ~0xFu);
  uint32_t result[4];
  {
    FlushToZero flush;
    for (int j = 0; j < 4; j++) {
      result[j] = DotColumn(vector, mw, j);
    }
  }
  const uint32_t at = out & ~0xFu;
  for (uint32_t j = 0; j < 4; j++) {
    guest::StoreU32(at + 4 * j, result[j]);
  }
}

void Vec3SwapXZ(uint32_t v) {
  ITC_NATIVE_RUNNING("sub_820AB688 Vec3::SwapXZ");
  const VectorLibVector vector(v);
  const uint32_t x = vector.x_bits();
  const uint32_t z = vector.z_bits();
  vector.set_z_bits(x);
  vector.set_x_bits(z);
}

void SetWsToOne(uint32_t m) {
  ITC_NATIVE_RUNNING("sub_8209B038 SetWsToOne");
  const uint32_t one = guest::LoadU32(G::one_address());
  const Matrix matrix(m);
  guest::StoreU32(matrix.m03_address(), one);
  guest::StoreU32(matrix.m13_address(), one);
  guest::StoreU32(matrix.m23_address(), one);
  guest::StoreU32(matrix.m33_address(), one);
}

void MultiplyUnaligned(uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_822B8F78 Matrix::MultiplyUnaligned");
  const Words bw = LoadWords(b);
  const Words aw = LoadWords(a);
  Words product;
  {
    FlushToZero flush;
    product = Product(aw, bw);
  }
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(out + 4 * i, product.w[i]);
  }
}

void MultiplyReversed(uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82293988 Matrix::MultiplyReversed");
  MultiplyUnaligned(out, b, a);
}

void InverseRotate(uint32_t out, uint32_t m, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_82293998 Matrix::InverseRotate");
  // The transpose first (all of m read before anything is written); t(i, j)
  // is the transpose's row i, column j: m's row j, column i.
  const Words mw = LoadWords(m);
  auto t = [&mw](int i, int j) { return double(std::bit_cast<float>(mw.w[4 * j + i])); };
  const MatrixMathVector3 vector(v);
  const double vy = double(vector.y());
  const double vz = double(vector.z());
  const double t20z = Fmuls(vz, t(2, 0));
  const double t22z = Fmuls(vz, t(2, 2));
  const VectorLibVector result(out);
  CopyWord(G::zero_address(), result.w_address());
  const double t11y = Fmuls(vy, t(1, 1));
  const double vx = double(vector.x());
  const double x0 = Fmadds(vx, t(0, 0), t20z);
  const double y0 = Fmadds(vz, t(2, 1), t11y);
  const double z0 = Fmadds(vx, t(0, 2), t22z);
  Stfs(result.x_address(), Fmadds(vy, t(1, 0), x0));
  Stfs(result.y_address(), Fmadds(vx, t(0, 1), y0));
  Stfs(result.z_address(), Fmadds(vy, t(1, 2), z0));
}

void Set(uint32_t out, uint32_t in) {
  ITC_NATIVE_RUNNING("sub_822935A0 Matrix::Set");
  for (uint32_t row = 0; row < 64; row += 16) {
    vector_library::Vec4Copy(out + row, in + row);
  }
}

void SetIdentity(uint32_t out) {
  ITC_NATIVE_RUNNING("sub_822936E0 Matrix::SetIdentity");
  Set(out, G::identity_address());
}

void FromQuaternion(uint32_t out, uint32_t q) {
  ITC_NATIVE_RUNNING("sub_822936F0 Matrix::FromQuaternion");
  const double length_squared = double(QuaternionLengthSquared(q));
  const Quaternion quaternion(q);
  const double y = double(quaternion.y());
  const double z = double(quaternion.z());
  const double x = double(quaternion.x());
  const double w = double(quaternion.w());
  const double s = Fdivs(double(G::two()), length_squared);
  const double ys = Fmuls(y, s);
  const double zs = Fmuls(z, s);
  const double xs = Fmuls(x, s);
  const double yys = Fmuls(y, ys);
  const double zzs = Fmuls(z, zs);
  const double xxs = Fmuls(x, xs);
  const double wys = Fmuls(w, ys);
  const double wxs = Fmuls(w, xs);
  const double xys = Fmuls(x, ys);
  const double wzs = Fmuls(w, zs);
  const double xzs = Fmuls(x, zs);
  const double yzs = Fmuls(y, zs);
  const double yy_zz = Fadds(zzs, yys);
  const double xx_zz = Fadds(zzs, xxs);
  const double xx_yy = Fadds(yys, xxs);
  const Matrix matrix(out);
  matrix.set_m01(float(Fadds(xys, wzs)));
  matrix.set_m02(float(Fsubs(xzs, wys)));
  matrix.set_m20(float(Fadds(xzs, wys)));
  const double one = double(G::one());
  matrix.set_m10(float(Fsubs(xys, wzs)));
  const double yz_wx = Fadds(yzs, wxs);
  matrix.set_m00(float(Fsubs(one, yy_zz)));
  const double m11 = Fsubs(one, xx_zz);
  matrix.set_m12(float(yz_wx));
  const double yz_minus_wx = Fsubs(yzs, wxs);
  matrix.set_m11(float(m11));
  const double m22 = Fsubs(one, xx_yy);
  matrix.set_m21(float(yz_minus_wx));
  matrix.set_m22(float(m22));
}

void FromQuaternionAndRow(uint32_t out, uint32_t q) {
  ITC_NATIVE_RUNNING("sub_822937D8 Matrix::FromQuaternionAndRow");
  FromQuaternion(out, q);
  vector_library::Vec4Copy(Matrix(out).m30_address(), G::fourth_row_address());
}

void QuaternionXAxis(uint32_t out, uint32_t q, uint32_t scratch) {
  ITC_NATIVE_RUNNING("sub_82293DF0 Quaternion::XAxis");
  const Quaternion axis(out);
  const double one = double(G::one());
  axis.set_x(float(one));
  const double zero = double(G::zero());
  axis.set_y(float(zero));
  axis.set_z(float(zero));
  axis.set_w(float(one));
  SetIdentity(scratch);
  FromQuaternionAndRow(scratch, q);
  ProjectPoint(out, scratch, out);
}

float QuaternionLengthSquared(uint32_t q) {
  ITC_NATIVE_RUNNING("sub_82293CB0 Quaternion::LengthSquared");
  const Quaternion quaternion(q);
  const double y = double(quaternion.y());
  const double yy = Fmuls(y, y);
  const double x = double(quaternion.x());
  const double z = double(quaternion.z());
  const double w = double(quaternion.w());
  const double sum = Fmadds(z, z, Fmadds(x, x, yy));
  return float(Fmadds(w, w, sum));
}

void ProjectPoint(uint32_t out, uint32_t m, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_82293600 Matrix::ProjectPoint");
  const Matrix matrix(m);
  const VectorLibVector point(p);
  const VectorLibVector result(out);
  const double py = Lfs(point.y_address());
  const double m28 = double(matrix.m13());
  const double w0 = Fmuls(m28, py);
  const double m20 = double(matrix.m11());
  const double m24 = double(matrix.m12());
  const double y0 = Fmuls(m20, py);
  const double m16 = double(matrix.m10());
  const double z0 = Fmuls(m24, py);
  const double px = Lfs(point.x_address());
  const double x0 = Fmuls(py, m16);
  const double m12 = double(matrix.m03());
  const double m4 = double(matrix.m01());
  const double m8 = double(matrix.m02());
  const double m0 = double(matrix.m00());
  const double pz = Lfs(point.z_address());
  const double m44 = double(matrix.m23());
  const double w1 = Fmadds(m12, px, w0);
  const double m36 = double(matrix.m21());
  const double y1 = Fmadds(m4, px, y0);
  const double m32 = double(matrix.m20());
  const double z1 = Fmadds(m8, px, z0);
  const double m60 = double(matrix.m33());
  const double x1 = Fmadds(px, m0, x0);
  const double m40 = double(matrix.m22());
  const double m52 = double(matrix.m31());
  const double m48 = double(matrix.m30());
  const double one = double(G::one());
  const double m56 = double(matrix.m32());
  Stfs(result.w_address(), one);
  const double w2 = Fmadds(m44, pz, w1);
  const double y2 = Fmadds(m36, pz, y1);
  const double z2 = Fmadds(m40, pz, z1);
  const double x2 = Fmadds(m32, pz, x1);
  const double w = Fadds(w2, m60);
  const double y = Fadds(y2, m52);
  const double z = Fadds(z2, m56);
  const double x = Fadds(x2, m48);
  const double inverse = Fdivs(one, w);
  Stfs(result.x_address(), Fmuls(x, inverse));
  Stfs(result.y_address(), Fmuls(inverse, y));
  Stfs(result.z_address(), Fmuls(inverse, z));
}

void QuaternionFromMatrix(uint32_t out, uint32_t m) {
  ITC_NATIVE_RUNNING("sub_82293AD8 Quaternion::FromMatrix");
  const Matrix matrix(m);
  const Quaternion quaternion(out);
  const double m22 = double(matrix.m22());
  const double m00 = double(matrix.m00());
  const double m11 = double(matrix.m11());
  const double trace = Fadds(Fadds(m00, m22), m11);
  const double zero = double(G::zero());
  if (trace > zero) {
    const double s = double(vector_library::Sqrtf(Fadds(trace, double(G::one()))));
    const double half = double(G::half());
    quaternion.set_w(float(Fmuls(s, half)));
    const double m21 = double(matrix.m21());
    const double t = Fdivs(half, s);
    const double m12 = double(matrix.m12());
    quaternion.set_x(float(Fmuls(Fsubs(m12, m21), t)));
    const double m02 = double(matrix.m02());
    const double m20 = double(matrix.m20());
    quaternion.set_y(float(Fmuls(Fsubs(m20, m02), t)));
    const double m01 = double(matrix.m01());
    const double m10 = double(matrix.m10());
    quaternion.set_z(float(Fmuls(Fsubs(m01, m10), t)));
    return;
  }
  // The largest diagonal element i (j and k the next two); q's x, y and z
  // in a table on the original's stack that first held the next indices.
  uint32_t q[3] = {1, 2, 0};
  int i = 0;
  if (m11 > m00) {
    i = 1;
  }
  if (m22 > Lfs(m + 20 * uint32_t(i))) {
    i = 2;
  }
  const int j = int(q[i]);
  const int k = int(q[j]);
  auto at = [m](int row, int column) { return m + uint32_t(16 * row + 4 * column); };
  const double mii = Lfs(at(i, i));
  const double mjj = Lfs(at(j, j));
  const double mkk = Lfs(at(k, k));
  const double r = Fsubs(mii, Fadds(mkk, mjj));
  double s = double(vector_library::Sqrtf(Fadds(r, double(G::one()))));
  const bool s_zero = s == zero;
  const double half = double(G::half());
  q[i] = std::bit_cast<uint32_t>(float(Fmuls(s, half)));
  if (!s_zero) {
    s = Fdivs(half, s);
  }
  const double mjk = Lfs(at(j, k));
  const double mkj = Lfs(at(k, j));
  quaternion.set_w(float(Fmuls(Fsubs(mjk, mkj), s)));
  const double mij = Lfs(at(i, j));
  const double mji = Lfs(at(j, i));
  const double sum_ij = Fadds(mji, mij);
  const double mki = Lfs(at(k, i));
  const double mik = Lfs(at(i, k));
  const double sum_ik = Fadds(mki, mik);
  q[j] = std::bit_cast<uint32_t>(float(Fmuls(sum_ij, s)));
  q[k] = std::bit_cast<uint32_t>(float(Fmuls(sum_ik, s)));
  guest::StoreU32(quaternion.x_address(), q[0]);
  guest::StoreU32(quaternion.y_address(), q[1]);
  guest::StoreU32(quaternion.z_address(), q[2]);
}

void QuaternionMultiply(uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82293D78 Quaternion::Multiply");
  const Quaternion qa(a);
  const Quaternion qb(b);
  const Quaternion product(out);
  const double aw = double(qa.w());
  const double by = double(qb.y());
  const double bw = double(qb.w());
  double f5 = Fmuls(by, aw);
  const double az = double(qa.z());
  const double ax = double(qa.x());
  double f4 = Fmuls(az, bw);
  const double bx = double(qb.x());
  double f6 = Fmuls(ax, bw);
  double f3 = Fmuls(bx, ax);
  const double ay = double(qa.y());
  const double bz = double(qb.z());
  f5 = Fmadds(az, bx, f5);
  f4 = Fmadds(by, ax, f4);
  f6 = Fmadds(bx, aw, f6);
  f3 = Fmsubs(aw, bw, f3);
  const double f0 = Fmadds(bw, ay, f5);
  const double f12 = Fmadds(aw, bz, f4);
  f6 = Fmadds(bz, ay, f6);
  f5 = Fnmsubs(by, ay, f3);
  product.set_y(float(Fnmsubs(ax, bz, f0)));
  product.set_z(float(Fnmsubs(bx, ay, f12)));
  product.set_x(float(Fnmsubs(by, az, f6)));
  product.set_w(float(Fnmsubs(az, bz, f5)));
}

double QuaternionFromAxisAngle(uint32_t out, uint32_t axis, double angle,
                               uint32_t scratch) {
  ITC_NATIVE_RUNNING("sub_82293E68 Quaternion::FromAxisAngle");
  const Quaternion quaternion(out);
  const MatrixMathVector3 axis_vector(axis);
  const double zero = double(G::zero());
  if (angle == zero) {
    quaternion.set_x(float(zero));
    quaternion.set_y(float(zero));
    quaternion.set_z(float(zero));
    quaternion.set_w(float(double(G::one())));
    return angle;
  }
  const double cosine = SinCos(Fmuls(angle, double(G::half())), scratch, scratch + 4);
  const double s = Lfs(scratch);
  quaternion.set_x(float(Fmuls(double(axis_vector.x()), s)));
  quaternion.set_y(float(Fmuls(double(axis_vector.y()), s)));
  const double z = double(axis_vector.z());
  const double c = Lfs(scratch + 4);
  const double zs = Fmuls(z, s);
  quaternion.set_w(float(c));
  quaternion.set_z(float(zs));
  return cosine;
}

void QuaternionNormalizeSelf(uint32_t q) {
  ITC_NATIVE_RUNNING("sub_82293F10 Quaternion::NormalizeSelf");
  const Quaternion quaternion(q);
  const double y = double(quaternion.y());
  const double yy = Fmuls(y, y);
  const double x = double(quaternion.x());
  const double z = double(quaternion.z());
  const double w = double(quaternion.w());
  const double sum = Fmadds(w, w, Fmadds(z, z, Fmadds(x, x, yy)));
  const double length = double(float(std::sqrt(sum)));
  const double inverse = Fdivs(double(G::one()), length);
  quaternion.set_x(float(Fmuls(x, inverse)));
  quaternion.set_w(float(Fmuls(w, inverse)));
  quaternion.set_y(float(Fmuls(y, inverse)));
  quaternion.set_z(float(Fmuls(z, inverse)));
}

double SinCos(double angle, uint32_t sine, uint32_t cosine) {
  ITC_NATIVE_RUNNING("sub_820B0E10 SinCos");
  MatrixMathFloat(sine).set_value(float(c_runtime::Sin(angle)));
  const double c = c_runtime::Cos(angle);
  MatrixMathFloat(cosine).set_value(float(c));
  return c;
}

namespace {

// The static set-ups' body: `diagonal` the word on the diagonal.
int32_t StaticMatrixInit(uint32_t sp, uint32_t matrix, uint32_t diagonal,
                         uint32_t exit_function, const c_runtime::AtexitCall& call) {
  const uint32_t f = sp - 304;
  for (uint32_t i = 0; i < 16; i++) {
    // lfs then stfs: the bits as the translated code keeps them.
    const bool on_diagonal = i % 5 == 0;
    guest::StoreU32(f + 160 + 4 * i,
                    guest::LoadU32(on_diagonal ? diagonal : G::zero_address()));
  }
  uint32_t from[16];
  for (uint32_t i = 0; i < 16; i++) {
    from[i] = f + 220 - 4 * i;
  }
  // The arguments past r10 (the 8th to the 16th pointers) at +84 to +148.
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(f + 84 + 8 * i, from[7 + i]);
  }
  camera_modes::MatrixFromFloats(matrix, from);
  return c_runtime::AtexitFrom(exit_function, call);
}

}  // namespace

int32_t ZeroMatrixStaticInit(uint32_t sp, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D8F88 ZeroMatrix::StaticInit");
  return StaticMatrixInit(sp, G::zero_matrix_address(), G::zero_address(), 0x825DACA0, call);
}

int32_t IdentityMatrixStaticInit(uint32_t sp, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D9060 IdentityMatrix::StaticInit");
  return StaticMatrixInit(sp, G::identity_matrix_address(), G::one_address(), 0x825DAC98,
                          call);
}

void ZeroMatrixExit() { ITC_NATIVE_RUNNING("sub_825DACA0 ZeroMatrix's clean-up"); }

void IdentityMatrixExit() { ITC_NATIVE_RUNNING("sub_825DAC98 IdentityMatrix's clean-up"); }

}  // namespace matrix_math
