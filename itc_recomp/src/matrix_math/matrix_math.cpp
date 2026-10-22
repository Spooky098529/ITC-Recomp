#include "matrix_math/matrix_math.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "interpolation/blend_state.h"
#include "vehicle_accessors/vectors.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/avx.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>

namespace matrix_math {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kOne = 0x82000A30;       // the game's float 1.0
constexpr uint32_t kZero = 0x82000A34;      // the game's float 0.0
constexpr uint32_t kHalf = 0x82001BE4;      // 0.5
constexpr uint32_t kTwo = 0x820021E8;       // 2.0
constexpr uint32_t kIdentity = 0x82953740;  // the identity matrix

// --- Scalar steps, as the translated code's doubles ---

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// lfs then stfs of one word: its bits.
void CopyWord(uint8_t* base, uint32_t from, uint32_t to) {
  guest::StoreU32(base, to, guest::LoadU32(base, from));
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
simde__m128 VectorAt(uint8_t* base, uint32_t address) {
  return VectorOf(guest::LoadU32(base, address + 0), guest::LoadU32(base, address + 4),
                  guest::LoadU32(base, address + 8), guest::LoadU32(base, address + 12));
}
// Word i (0 the top lane).
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

Words LoadWords(uint8_t* base, uint32_t address) {
  Words m;
  for (uint32_t i = 0; i < 16; i++) {
    m.w[i] = guest::LoadU32(base, address + 4 * i);
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
void Rotate(uint8_t* base, uint32_t out, uint32_t in, uint32_t angle, uint32_t turn) {
  // The angle into [-pi, pi]: pi with the angle's sign (its bits ORed into
  // 0x40490FDB, from the stored float), added, times 1 / (2 pi), truncated
  // (fctiwz: NaN gives INT_MIN, INT_MAX and above INT_MAX), that many 2 pi
  // taken off (fnmsubs).
  double x = Lfs(base, angle);
  const double two_pi = Lfs(base, 0x82001FA8);
  const uint32_t one = std::bit_cast<uint32_t>(float(Lfs(base, kOne)));
  const uint32_t stored = std::bit_cast<uint32_t>(float(x));
  const double pi = double(std::bit_cast<float>((stored & 0x80000000u) | 0x40490FDBu));
  const double turns = Fmuls(Fadds(pi, x), Lfs(base, 0x82001FA4));
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
  const simde__m128 c9c0 = VectorAt(base, 0x820009C0);
  const simde__m128 c9d0 = VectorAt(base, 0x820009D0);
  const simde__m128 c9e0 = VectorAt(base, 0x820009E0);
  const simde__m128 c9f0 = VectorAt(base, 0x820009F0);
  const simde__m128 ca00 = VectorAt(base, 0x82000A00);
  const simde__m128 ca10 = VectorAt(base, 0x82000A10);
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
  const uint32_t zero = std::bit_cast<uint32_t>(float(Lfs(base, kZero)));
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
    StoreVector(base, at + 16 * i, F(rows[i]));
  }
  // out = in * the turn, noted for interpolation as the original's call.
  Multiply(base, out, in, turn);
  constexpr uint32_t kReturn = kAxis == Axis::kZ   ? 0x820A2F28
                               : kAxis == Axis::kX ? 0x820A67E8
                                                   : 0x820A69F0;
  interpolation::BlendState_OnMultiply(base, out, in, turn, kReturn);
}

}  // namespace

void Multiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_8209BD50 Matrix::Multiply");
  const Words bw = LoadWords(base, b);
  const Words aw = LoadWords(base, a);
  Words product;
  {
    FlushToZero flush;
    product = Product(aw, bw);
  }
  const uint32_t at = out & ~0xFu;
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, at + 4 * i, product.w[i]);
  }
}

void Copy(uint8_t* base, uint32_t dst, uint32_t src) {
  ITC_NATIVE_RUNNING("sub_820A0AC0 Matrix::Copy");
  // Twice a word at a time, once eight bytes at a time (both words read
  // before either is written), then twice a word at a time again.
  for (int pass = 0; pass < 2; pass++) {
    for (uint32_t i = 0; i < 64; i += 4) {
      CopyWord(base, src + i, dst + i);
    }
  }
  for (uint32_t i = 0; i < 64; i += 8) {
    const uint32_t w0 = guest::LoadU32(base, src + i);
    const uint32_t w1 = guest::LoadU32(base, src + i + 4);
    guest::StoreU32(base, dst + i, w0);
    guest::StoreU32(base, dst + i + 4, w1);
  }
  for (int pass = 0; pass < 2; pass++) {
    for (uint32_t i = 0; i < 64; i += 4) {
      CopyWord(base, src + i, dst + i);
    }
  }
}

void Inverse(uint8_t* base, uint32_t out, uint32_t in) {
  ITC_NATIVE_RUNNING("sub_820A9988 Matrix::Inverse");
  // The translated code's steps on its vector registers, one statement each
  // (the names are the registers'); D3DX's inverse by cofactors. The
  // original's copies of the input and its transposes on its stack are
  // only scratch: the rows are read here once.
  simde__m128 rows[4];
  for (uint32_t i = 0; i < 4; i++) {
    rows[i] = VectorAt(base, in + 16 * i);
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
    StoreVector(base, at + 16 * i, result[i]);
  }
}

void RotateX(uint8_t* base, uint32_t out, uint32_t in, uint32_t angle, uint32_t turn) {
  ITC_NATIVE_RUNNING("sub_820A65F0 Matrix::RotateX");
  Rotate<Axis::kX>(base, out, in, angle, turn);
}

void RotateY(uint8_t* base, uint32_t out, uint32_t in, uint32_t angle, uint32_t turn) {
  ITC_NATIVE_RUNNING("sub_820A67F8 Matrix::RotateY");
  Rotate<Axis::kY>(base, out, in, angle, turn);
}

void RotateZ(uint8_t* base, uint32_t out, uint32_t in, uint32_t angle, uint32_t turn) {
  ITC_NATIVE_RUNNING("sub_820A2D30 Matrix::RotateZ");
  Rotate<Axis::kZ>(base, out, in, angle, turn);
}

void ObjectInverse(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820A9BC8 ObjectInverse");
  Inverse(base, object + 560, object + 208);
}

void Vec4Transform(uint8_t* base, uint32_t out, uint32_t m, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_820AB560 Vec4::Transform");
  const Words mw = LoadWords(base, m);
  const simde__m128 vector = VectorAt(base, v & ~0xFu);
  uint32_t result[4];
  {
    FlushToZero flush;
    for (int j = 0; j < 4; j++) {
      result[j] = DotColumn(vector, mw, j);
    }
  }
  const uint32_t at = out & ~0xFu;
  for (uint32_t j = 0; j < 4; j++) {
    guest::StoreU32(base, at + 4 * j, result[j]);
  }
}

void Vec3SwapXZ(uint8_t* base, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_820AB688 Vec3::SwapXZ");
  const uint32_t x = guest::LoadU32(base, v + 0);
  const uint32_t z = guest::LoadU32(base, v + 8);
  guest::StoreU32(base, v + 8, x);
  guest::StoreU32(base, v + 0, z);
}

void SetWsToOne(uint8_t* base, uint32_t m) {
  ITC_NATIVE_RUNNING("sub_8209B038 SetWsToOne");
  const uint32_t one = guest::LoadU32(base, kOne);
  for (uint32_t row = 0; row < 4; row++) {
    guest::StoreU32(base, m + 16 * row + 12, one);
  }
}

void MultiplyUnaligned(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_822B8F78 Matrix::MultiplyUnaligned");
  const Words bw = LoadWords(base, b);
  const Words aw = LoadWords(base, a);
  Words product;
  {
    FlushToZero flush;
    product = Product(aw, bw);
  }
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, out + 4 * i, product.w[i]);
  }
}

void MultiplyReversed(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82293988 Matrix::MultiplyReversed");
  MultiplyUnaligned(base, out, b, a);
}

void InverseRotate(uint8_t* base, uint32_t out, uint32_t m, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_82293998 Matrix::InverseRotate");
  // The transpose first (all of m read before anything is written); t(i, j)
  // is the transpose's row i, column j: m's row j, column i.
  const Words mw = LoadWords(base, m);
  auto t = [&mw](int i, int j) { return double(std::bit_cast<float>(mw.w[4 * j + i])); };
  const double vy = Lfs(base, v + 4);
  const double vz = Lfs(base, v + 8);
  const double t20z = Fmuls(vz, t(2, 0));
  const double t22z = Fmuls(vz, t(2, 2));
  CopyWord(base, kZero, out + 12);
  const double t11y = Fmuls(vy, t(1, 1));
  const double vx = Lfs(base, v + 0);
  const double x0 = Fmadds(vx, t(0, 0), t20z);
  const double y0 = Fmadds(vz, t(2, 1), t11y);
  const double z0 = Fmadds(vx, t(0, 2), t22z);
  Stfs(base, out + 0, Fmadds(vy, t(1, 0), x0));
  Stfs(base, out + 4, Fmadds(vx, t(0, 1), y0));
  Stfs(base, out + 8, Fmadds(vy, t(1, 2), z0));
}

void Set(uint8_t* base, uint32_t out, uint32_t in) {
  ITC_NATIVE_RUNNING("sub_822935A0 Matrix::Set");
  for (uint32_t row = 0; row < 64; row += 16) {
    vector_library::Vec4Copy(base, out + row, in + row);
  }
}

void SetIdentity(uint8_t* base, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_822936E0 Matrix::SetIdentity");
  Set(base, out, kIdentity);
}

void FromQuaternion(uint8_t* base, uint32_t out, uint32_t q) {
  ITC_NATIVE_RUNNING("sub_822936F0 Matrix::FromQuaternion");
  const double length_squared = double(QuaternionLengthSquared(base, q));
  const double y = Lfs(base, q + 4);
  const double z = Lfs(base, q + 8);
  const double x = Lfs(base, q + 0);
  const double w = Lfs(base, q + 12);
  const double s = Fdivs(Lfs(base, kTwo), length_squared);
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
  Stfs(base, out + 4, Fadds(xys, wzs));
  Stfs(base, out + 8, Fsubs(xzs, wys));
  Stfs(base, out + 32, Fadds(xzs, wys));
  const double one = Lfs(base, kOne);
  Stfs(base, out + 16, Fsubs(xys, wzs));
  const double yz_wx = Fadds(yzs, wxs);
  Stfs(base, out + 0, Fsubs(one, yy_zz));
  const double m11 = Fsubs(one, xx_zz);
  Stfs(base, out + 24, yz_wx);
  const double yz_minus_wx = Fsubs(yzs, wxs);
  Stfs(base, out + 20, m11);
  const double m22 = Fsubs(one, xx_yy);
  Stfs(base, out + 36, yz_minus_wx);
  Stfs(base, out + 40, m22);
}

float QuaternionLengthSquared(uint8_t* base, uint32_t q) {
  ITC_NATIVE_RUNNING("sub_82293CB0 Quaternion::LengthSquared");
  const double y = Lfs(base, q + 4);
  const double yy = Fmuls(y, y);
  const double x = Lfs(base, q + 0);
  const double z = Lfs(base, q + 8);
  const double w = Lfs(base, q + 12);
  const double sum = Fmadds(z, z, Fmadds(x, x, yy));
  return float(Fmadds(w, w, sum));
}

void ProjectPoint(uint8_t* base, uint32_t out, uint32_t m, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_82293600 Matrix::ProjectPoint");
  const double py = Lfs(base, p + 4);
  const double m28 = Lfs(base, m + 28);
  const double w0 = Fmuls(m28, py);
  const double m20 = Lfs(base, m + 20);
  const double m24 = Lfs(base, m + 24);
  const double y0 = Fmuls(m20, py);
  const double m16 = Lfs(base, m + 16);
  const double z0 = Fmuls(m24, py);
  const double px = Lfs(base, p + 0);
  const double x0 = Fmuls(py, m16);
  const double m12 = Lfs(base, m + 12);
  const double m4 = Lfs(base, m + 4);
  const double m8 = Lfs(base, m + 8);
  const double m0 = Lfs(base, m + 0);
  const double pz = Lfs(base, p + 8);
  const double m44 = Lfs(base, m + 44);
  const double w1 = Fmadds(m12, px, w0);
  const double m36 = Lfs(base, m + 36);
  const double y1 = Fmadds(m4, px, y0);
  const double m32 = Lfs(base, m + 32);
  const double z1 = Fmadds(m8, px, z0);
  const double m60 = Lfs(base, m + 60);
  const double x1 = Fmadds(px, m0, x0);
  const double m40 = Lfs(base, m + 40);
  const double m52 = Lfs(base, m + 52);
  const double m48 = Lfs(base, m + 48);
  const double one = Lfs(base, kOne);
  const double m56 = Lfs(base, m + 56);
  Stfs(base, out + 12, one);
  const double w2 = Fmadds(m44, pz, w1);
  const double y2 = Fmadds(m36, pz, y1);
  const double z2 = Fmadds(m40, pz, z1);
  const double x2 = Fmadds(m32, pz, x1);
  const double w = Fadds(w2, m60);
  const double y = Fadds(y2, m52);
  const double z = Fadds(z2, m56);
  const double x = Fadds(x2, m48);
  const double inverse = Fdivs(one, w);
  Stfs(base, out + 0, Fmuls(x, inverse));
  Stfs(base, out + 4, Fmuls(inverse, y));
  Stfs(base, out + 8, Fmuls(inverse, z));
}

void QuaternionFromMatrix(uint8_t* base, uint32_t out, uint32_t m) {
  ITC_NATIVE_RUNNING("sub_82293AD8 Quaternion::FromMatrix");
  const double m22 = Lfs(base, m + 40);
  const double m00 = Lfs(base, m + 0);
  const double m11 = Lfs(base, m + 20);
  const double trace = Fadds(Fadds(m00, m22), m11);
  const double zero = Lfs(base, kZero);
  if (trace > zero) {
    const double s = double(vector_library::Sqrtf(Fadds(trace, Lfs(base, kOne))));
    const double half = Lfs(base, kHalf);
    Stfs(base, out + 12, Fmuls(s, half));
    const double m21 = Lfs(base, m + 36);
    const double t = Fdivs(half, s);
    const double m12 = Lfs(base, m + 24);
    Stfs(base, out + 0, Fmuls(Fsubs(m12, m21), t));
    const double m02 = Lfs(base, m + 8);
    const double m20 = Lfs(base, m + 32);
    Stfs(base, out + 4, Fmuls(Fsubs(m20, m02), t));
    const double m01 = Lfs(base, m + 4);
    const double m10 = Lfs(base, m + 16);
    Stfs(base, out + 8, Fmuls(Fsubs(m01, m10), t));
    return;
  }
  // The largest diagonal element i (j and k the next two); q's x, y and z
  // in a table on the original's stack that first held the next indices.
  uint32_t q[3] = {1, 2, 0};
  int i = 0;
  if (m11 > m00) {
    i = 1;
  }
  if (m22 > Lfs(base, m + 20 * uint32_t(i))) {
    i = 2;
  }
  const int j = int(q[i]);
  const int k = int(q[j]);
  auto at = [m](int row, int column) { return m + uint32_t(16 * row + 4 * column); };
  const double mii = Lfs(base, at(i, i));
  const double mjj = Lfs(base, at(j, j));
  const double mkk = Lfs(base, at(k, k));
  const double r = Fsubs(mii, Fadds(mkk, mjj));
  double s = double(vector_library::Sqrtf(Fadds(r, Lfs(base, kOne))));
  const bool s_zero = s == zero;
  const double half = Lfs(base, kHalf);
  q[i] = std::bit_cast<uint32_t>(float(Fmuls(s, half)));
  if (!s_zero) {
    s = Fdivs(half, s);
  }
  const double mjk = Lfs(base, at(j, k));
  const double mkj = Lfs(base, at(k, j));
  Stfs(base, out + 12, Fmuls(Fsubs(mjk, mkj), s));
  const double mij = Lfs(base, at(i, j));
  const double mji = Lfs(base, at(j, i));
  const double sum_ij = Fadds(mji, mij);
  const double mki = Lfs(base, at(k, i));
  const double mik = Lfs(base, at(i, k));
  const double sum_ik = Fadds(mki, mik);
  q[j] = std::bit_cast<uint32_t>(float(Fmuls(sum_ij, s)));
  q[k] = std::bit_cast<uint32_t>(float(Fmuls(sum_ik, s)));
  guest::StoreU32(base, out + 0, q[0]);
  guest::StoreU32(base, out + 4, q[1]);
  guest::StoreU32(base, out + 8, q[2]);
}

void QuaternionMultiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82293D78 Quaternion::Multiply");
  const double aw = Lfs(base, a + 12);
  const double by = Lfs(base, b + 4);
  const double bw = Lfs(base, b + 12);
  double f5 = Fmuls(by, aw);
  const double az = Lfs(base, a + 8);
  const double ax = Lfs(base, a + 0);
  double f4 = Fmuls(az, bw);
  const double bx = Lfs(base, b + 0);
  double f6 = Fmuls(ax, bw);
  double f3 = Fmuls(bx, ax);
  const double ay = Lfs(base, a + 4);
  const double bz = Lfs(base, b + 8);
  f5 = Fmadds(az, bx, f5);
  f4 = Fmadds(by, ax, f4);
  f6 = Fmadds(bx, aw, f6);
  f3 = Fmsubs(aw, bw, f3);
  const double f0 = Fmadds(bw, ay, f5);
  const double f12 = Fmadds(aw, bz, f4);
  f6 = Fmadds(bz, ay, f6);
  f5 = Fnmsubs(by, ay, f3);
  Stfs(base, out + 4, Fnmsubs(ax, bz, f0));
  Stfs(base, out + 8, Fnmsubs(bx, ay, f12));
  Stfs(base, out + 0, Fnmsubs(by, az, f6));
  Stfs(base, out + 12, Fnmsubs(az, bz, f5));
}

void QuaternionNormalizeSelf(uint8_t* base, uint32_t q) {
  ITC_NATIVE_RUNNING("sub_82293F10 Quaternion::NormalizeSelf");
  const double y = Lfs(base, q + 4);
  const double yy = Fmuls(y, y);
  const double x = Lfs(base, q + 0);
  const double z = Lfs(base, q + 8);
  const double w = Lfs(base, q + 12);
  const double sum = Fmadds(w, w, Fmadds(z, z, Fmadds(x, x, yy)));
  const double length = double(float(std::sqrt(sum)));
  const double inverse = Fdivs(Lfs(base, kOne), length);
  Stfs(base, q + 0, Fmuls(x, inverse));
  Stfs(base, q + 12, Fmuls(w, inverse));
  Stfs(base, q + 4, Fmuls(y, inverse));
  Stfs(base, q + 8, Fmuls(z, inverse));
}

double SinCos(uint8_t* base, double angle, uint32_t sine, uint32_t cosine) {
  ITC_NATIVE_RUNNING("sub_820B0E10 SinCos");
  Stfs(base, sine, double(float(c_runtime::Sin(angle))));
  const double c = c_runtime::Cos(angle);
  Stfs(base, cosine, double(float(c)));
  return c;
}

}  // namespace matrix_math
