// TextureProjection::Fit's geometry library (docs/functions/car-scene-draw.md):
// the frustum's planes and corners, a plane's crossing with a line and the
// box over a vector of points, step for step as the translated code (see
// car_scene_draw.h).

#include "car_scene_draw/car_scene_draw.h"

#include "guest.h"

#include "base/log.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>

namespace car_scene_draw {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kOne = 0x82000A30;           // the game's float 1.0
constexpr uint32_t kZero = 0x82000A34;          // 0.0
constexpr uint32_t kBoxMin = 0x8206CE44;        // the box's starting min (probably FLT_MAX)
constexpr uint32_t kBoxMax = 0x8206CE40;        // its starting max (probably -FLT_MAX)
constexpr uint32_t kCrossingEpsilon = 0x8206CE48;  // SegmentCrossing's smallest |plane . dir|

constexpr uint32_t kFrustumFrame = 336;
constexpr uint32_t kBoxFrame = 128;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// lfs and stfs (through the double, as the translated code).
double Lfs(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
double Fmuls(double a, double b) { return double(float(a * b)); }
double Fadds(double a, double b) { return double(float(a + b)); }
double Fsubs(double a, double b) { return double(float(a - b)); }
double Fdivs(double a, double b) { return double(float(a / b)); }
double Fmadds(double a, double b, double c) { return double(float(std::fma(a, b, c))); }
double Fsqrts(double a) { return double(float(std::sqrt(a))); }
double Fneg(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) ^ 0x8000000000000000ull);
}
double Fabs(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) & ~0x8000000000000000ull);
}

// 16 bytes as two ld and two std, both loads first (ld, ld, std, std).
void Copy16(uint8_t* base, uint32_t to, uint32_t from) {
  uint32_t w[4];
  for (int i = 0; i < 4; i++) {
    w[i] = Load(base, from + 4 * i);
  }
  for (int i = 0; i < 4; i++) {
    Store(base, to + 4 * i, w[i]);
  }
}

// --- The vector unit, as the translation does it ---

// Vectors as the translated code holds them: word 0 in the top lane.
simde__m128 VectorOf(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int32_t(w0), int32_t(w1), int32_t(w2), int32_t(w3)));
}
// lvx128: the aligned 16 bytes holding `address`.
simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
}
uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}
// stvx: to the aligned 16 bytes holding `address`.
void Stvx(uint8_t* base, uint32_t address, simde__m128 v) {
  const uint32_t at = address & ~0xFu;
  for (int i = 0; i < 4; i++) {
    Store(base, at + 4 * i, WordOf(v, i));
  }
}
// vpermwi128 as the translation's shuffles.
template <int kImmediate>
simde__m128 Permute(simde__m128 v) {
  return simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v), kImmediate));
}
// vnmsubfp d = -(a b - c), unfused, as translated.
simde__m128 Nmsub(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(a, b), c),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
}

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

}  // namespace

// --- The planes ---

uint32_t PlaneIntersect3(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t c,
                         uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_825976C8 Plane::Intersect3");
  const uint32_t s = stack;
  const double one = Lfs(base, kOne);
  // b's x, y, z and 1.0 at s - 64, c's at s - 48, then a's over b's, each
  // read back as a vector.
  Stfs(base, s - 64, Lfs(base, b + 0));
  Stfs(base, s - 60, Lfs(base, b + 4));
  Stfs(base, s - 56, Lfs(base, b + 8));
  Stfs(base, s - 52, one);
  Stfs(base, s - 48, Lfs(base, c + 0));
  Stfs(base, s - 44, Lfs(base, c + 4));
  const double cz = Lfs(base, c + 8);
  const simde__m128 vb = Lvx(base, s - 64);
  Stfs(base, s - 40, cz);
  const simde__m128 v10 = Permute<0x78>(vb);
  Stfs(base, s - 36, one);
  const simde__m128 v11 = Permute<0x9C>(vb);
  Stfs(base, s - 64, Lfs(base, a + 0));
  Stfs(base, s - 60, Lfs(base, a + 4));
  const double az = Lfs(base, a + 8);
  const simde__m128 vc = Lvx(base, s - 48);
  Stfs(base, s - 56, az);
  const simde__m128 v8 = Permute<0x9C>(vc);
  Stfs(base, s - 52, one);
  const simde__m128 v9 = Permute<0x78>(vc);
  const simde__m128 v7 = Lvx(base, s - 64);
  {
    // The three cross products (b x c, c x a, a x b, by their use) and the
    // triple product.
    FlushToZero flush;
    const simde__m128 v6 = Permute<0x78>(v7);
    const simde__m128 v5 = Permute<0x9C>(v7);
    simde__m128 v0 = simde_mm_mul_ps(v11, v9);
    simde__m128 v13 = simde_mm_mul_ps(v8, v6);
    simde__m128 v12 = simde_mm_mul_ps(v5, v10);
    v0 = Nmsub(v10, v8, v0);
    v13 = Nmsub(v9, v5, v13);
    v12 = Nmsub(v6, v11, v12);
    Stvx(base, s - 64, v13);
    Stvx(base, s - 32, v12);
    Stvx(base, s - 48, v0);
    v0 = simde_mm_dp_ps(v7, v0, 0xEF);
    Stvx(base, s - 16, v0);
  }
  double f0 = Lfs(base, s - 16);
  Stfs(base, s - 80, f0);
  const uint32_t exponent = Load(base, s - 80) & 0x7F800000u;
  if (exponent == 0 || exponent == 0x7F800000u) {
    return 0;
  }
  double f13 = Lfs(base, a + 12);
  f0 = Fdivs(one, f0);
  double f9 = Fmuls(f13, Lfs(base, s - 48));
  double f8 = Fmuls(f13, Lfs(base, s - 44));
  f13 = Fmuls(f13, Lfs(base, s - 40));
  double f12 = Lfs(base, b + 12);
  const double f7 = Fmuls(f12, Lfs(base, s - 64));
  const double f6 = Fmuls(f12, Lfs(base, s - 60));
  f12 = Fmuls(f12, Lfs(base, s - 56));
  double f11 = Lfs(base, c + 12);
  const double f5 = Fmuls(f11, Lfs(base, s - 32));
  const double f4 = Fmuls(f11, Lfs(base, s - 28));
  f11 = Fmuls(f11, Lfs(base, s - 24));
  Stfs(base, s - 4, one);
  f9 = Fadds(f7, f9);
  f8 = Fadds(f6, f8);
  f13 = Fadds(f12, f13);
  f12 = Fadds(f9, f5);
  f9 = Fadds(f8, f4);
  f13 = Fadds(f13, f11);
  Stfs(base, s - 16, Fmuls(Fneg(f12), f0));
  Stfs(base, s - 12, Fmuls(Fneg(f9), f0));
  Stfs(base, s - 8, Fmuls(Fneg(f13), f0));
  Copy16(base, out, s - 16);
  Stfs(base, out + 12, one);
  return 1;
}

uint32_t FrustumFromMatrix(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597878 Frustum::FromMatrix");
  const uint32_t f = stack - kFrustumFrame;
  Store(base, f, stack);  // the back chain, as stwu
  auto m = [&](int row, int column) { return Lfs(base, matrix + 16 * row + 4 * column); };
  auto st = [&](uint32_t offset, double value) { Stfs(base, f + offset, value); };
  auto copy = [&](uint32_t to, uint32_t from) { Copy16(base, f + to, f + from); };
  const double one = Lfs(base, kOne);
  for (uint32_t k = 0; k < 8; k++) {
    Stfs(base, out + 128 + 16 * k + 12, one);  // the corners' w
  }
  // The six planes from the matrix's w column (3) with and against its x,
  // y and z columns, built through the frame as the original: each written
  // to a temporary, then copied to 128 + 16 k (k = 0 to 5: w - x, w + x,
  // w - y, w + y, w - z, w + z), the d's stored over the copies' 1.0.
  st(80, Fsubs(m(0, 3), m(0, 0)));
  st(224, Fadds(m(0, 0), m(0, 3)));
  st(84, Fsubs(m(1, 3), m(1, 0)));
  st(228, Fadds(m(1, 0), m(1, 3)));
  st(232, Fadds(m(2, 0), m(2, 3)));
  st(88, Fsubs(m(2, 3), m(2, 0)));
  st(92, one);
  st(96, Fsubs(m(0, 3), m(0, 1)));
  st(100, Fsubs(m(1, 3), m(1, 1)));
  st(236, one);
  st(104, Fsubs(m(2, 3), m(2, 1)));
  copy(128, 80);
  st(112, Fadds(m(0, 1), m(0, 3)));
  st(108, one);
  copy(160, 96);  // (the original holds it in registers until after 144)
  st(140, Fsubs(m(3, 3), m(3, 0)));
  st(116, Fadds(m(1, 1), m(1, 3)));
  copy(144, 224);
  st(120, Fadds(m(2, 1), m(2, 3)));
  st(156, Fadds(m(3, 0), m(3, 3)));
  st(124, one);
  copy(176, 112);
  st(172, Fsubs(m(3, 3), m(3, 1)));
  st(112, Fsubs(m(0, 3), m(0, 2)));
  st(116, Fsubs(m(1, 3), m(1, 2)));
  st(96, Fadds(m(0, 2), m(0, 3)));
  st(120, Fsubs(m(2, 3), m(2, 2)));
  st(124, one);
  copy(192, 112);
  st(100, Fadds(m(1, 2), m(1, 3)));
  st(104, Fadds(m(2, 2), m(2, 3)));
  st(108, one);
  copy(208, 96);
  st(188, Fadds(m(3, 1), m(3, 3)));
  st(204, Fsubs(m(3, 3), m(3, 2)));
  st(220, Fadds(m(3, 2), m(3, 3)));
  // Each plane scaled by 1 / the length of its x, y, z (stored y, x, z, d).
  for (uint32_t p = f + 128; p < f + 224; p += 16) {
    double y = Lfs(base, p + 4);
    double x = Lfs(base, p + 0);
    double length = Fmuls(x, x);
    double z = Lfs(base, p + 8);
    double d = Lfs(base, p + 12);
    length = Fmadds(z, z, length);
    length = Fmadds(y, y, length);
    length = Fsqrts(length);
    const double scale = Fdivs(one, length);
    Stfs(base, p + 4, Fmuls(y, scale));
    Stfs(base, p + 0, Fmuls(x, scale));
    Stfs(base, p + 8, Fmuls(z, scale));
    Stfs(base, p + 12, Fmuls(d, scale));
  }
  // The planes copied to out + 16 k through the frame's 80 (d first), their
  // x, y, z kept for the sign codes.
  double xyz[6][3];
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t p = f + 128 + 16 * k;
    const double d = Lfs(base, p + 12);
    for (int i = 0; i < 3; i++) {
      xyz[k][i] = Lfs(base, p + 4 * i);
    }
    st(92, d);
    for (int i = 0; i < 3; i++) {
      st(80 + 4 * i, xyz[k][i]);
    }
    for (uint32_t i = 0; i < 4; i++) {
      Store(base, out + 16 * k + 4 * i, Load(base, f + 80 + 4 * i));
    }
  }
  // +96 + 4 k: plane k's code, 1 for x < 0, 2 for y, 4 for z.
  const double zero = Lfs(base, kZero);
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t code = (xyz[k][0] < zero ? 1u : 0u) | (xyz[k][1] < zero ? 2u : 0u) |
                          (xyz[k][2] < zero ? 4u : 0u);
    Store(base, out + 96 + 4 * k, code);
  }
  // Corner k from planes 4 or 5, 2 or 3 and 0 or 1 by k's bits.
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t a = (k & 1) != 0 ? out + 64 : out + 80;
    const uint32_t b = (k & 2) != 0 ? out + 48 : out + 32;
    const uint32_t c = (k & 4) != 0 ? out + 0 : out + 16;
    PlaneIntersect3(base, out + 128 + 16 * k, a, b, c, f);
  }
  return out;
}

uint32_t PlaneSegmentCrossing(uint8_t* base, uint32_t out, uint32_t plane, uint32_t b,
                              uint32_t a, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597E58 Plane::SegmentCrossing");
  const uint32_t s = stack;
  const double f12 = Lfs(base, plane + 0);
  double f11 = Lfs(base, plane + 4);
  Stfs(base, s - 28, f11);
  const double ax = Lfs(base, a + 0);
  Stfs(base, s - 32, f12);
  const double one = Lfs(base, kOne);
  double f8 = Lfs(base, b + 0);
  const double ay = Lfs(base, a + 4);
  f8 = Fsubs(f8, ax);
  double f7 = Lfs(base, b + 4);
  f11 = Fmuls(ay, f11);
  Stfs(base, s - 16, f8);
  f7 = Fsubs(f7, ay);
  Stfs(base, s - 20, Lfs(base, kZero));
  const double pz = Lfs(base, plane + 8);
  Stfs(base, s - 24, pz);
  const double az = Lfs(base, a + 8);
  const double bz = Lfs(base, b + 8);
  Stfs(base, s - 12, f7);
  f7 = Fsubs(bz, az);
  Stfs(base, s - 4, one);
  const simde__m128 v7 = Lvx(base, s - 32);  // the plane's x, y, z and 0.0
  f11 = Fmadds(az, pz, f11);
  Stfs(base, s - 8, f7);
  const double pd = Lfs(base, plane + 12);
  const simde__m128 v10 = Lvx(base, s - 16);  // b - a and 1.0
  simde__m128 v0;
  simde__m128 v11;
  {
    FlushToZero flush;
    v11 = simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                          simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
    v0 = simde_mm_dp_ps(v10, v10, 0xEF);
  }
  // plane . a + d.
  double f0 = Fmadds(ax, f12, f11);
  f0 = Fadds(f0, pd);
  simde__m128 v13;
  simde__m128 v12;
  simde__m128 v9;
  {
    FlushToZero flush;
    v13 = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(v0));
    v9 = simde_mm_mul_ps(v0, v11);
    v12 = simde_mm_cmpeq_ps(v0, simde_mm_castsi128_ps(simde_mm_set1_epi32(0)));
  }
  const double minus_distance = Fneg(f0);
  {
    // The direction normalized (one Newton step on the reciprocal square
    // root; 0 where its length is 0), and its dot product with the plane.
    FlushToZero flush;
    const simde__m128 v8 = simde_mm_mul_ps(v13, v13);
    v11 = Nmsub(v9, v8, v11);
    v13 = simde_mm_add_ps(simde_mm_mul_ps(v13, v11), v13);
    v13 = simde_mm_mul_ps(v10, v13);
    v12 = simde_mm_castsi128_ps(
        simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_castps_si128(v12), simde_mm_castps_si128(v13)),
                          simde_mm_and_si128(simde_mm_castps_si128(v12), simde_mm_castps_si128(v0))));
    v0 = simde_mm_dp_ps(v7, v12, 0xEF);
    Stvx(base, s - 16, v12);
    Stvx(base, s - 32, v0);
  }
  const double along = Lfs(base, s - 32);
  if (Fabs(along) < Lfs(base, kCrossingEpsilon)) {
    return 0;
  }
  // t = -(plane . a + d) / (plane . dir); out = dir t, then + a.
  f0 = Fdivs(minus_distance, along);
  double f12b = Lfs(base, s - 16);
  Stfs(base, s - 20, one);
  Stfs(base, s - 4, one);
  Stfs(base, s - 32, Fmuls(f12b, f0));
  f12b = Lfs(base, s - 12);
  Stfs(base, s - 28, Fmuls(f12b, f0));
  f12b = Lfs(base, s - 8);
  Stfs(base, s - 24, Fmuls(f12b, f0));
  Copy16(base, out, s - 32);
  const double x = Fadds(Lfs(base, out + 0), Lfs(base, a + 0));
  Stfs(base, s - 16, x);
  const double ay2 = Lfs(base, a + 4);
  const double az2 = Lfs(base, a + 8);
  const double oy = Lfs(base, out + 4);
  const double oz = Lfs(base, out + 8);
  Stfs(base, s - 12, Fadds(oy, ay2));
  Stfs(base, s - 8, Fadds(oz, az2));
  Copy16(base, out, s - 16);
  Stfs(base, out + 12, one);
  return 1;
}

// --- The box ---

uint32_t Vector16Size(uint8_t* base, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_825971E8 Vector16::Size");
  const uint32_t first = Load(base, vector + 4);
  if (first == 0) {
    return 0;
  }
  return uint32_t(int32_t(Load(base, vector + 8) - first) >> 4);
}

void BoxExtend(uint8_t* base, uint32_t box, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_82597150 Box::Extend");
  // min = the point's unless min is below it (a NaN on either side takes
  // the point's).
  for (uint32_t i = 0; i < 12; i += 4) {
    const double min = Lfs(base, box + i);
    const double p = Lfs(base, point + i);
    Stfs(base, box + i, min < p ? min : p);
  }
  // max = the point's unless max is above it.
  for (uint32_t i = 0; i < 12; i += 4) {
    const double p = Lfs(base, point + i);
    const double max = Lfs(base, box + 16 + i);
    Stfs(base, box + 16 + i, max > p ? max : p);
  }
}

uint32_t BoxFromPoints(uint8_t* base, uint32_t box, uint32_t vector, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597210 Box::FromPoints");
  Store(base, stack - kBoxFrame, stack);  // the back chain, as stwu
  const double min = Lfs(base, kBoxMin);
  Stfs(base, box + 0, min);
  Stfs(base, box + 4, min);
  Stfs(base, box + 8, min);
  const double one = Lfs(base, kOne);
  Stfs(base, box + 12, one);
  Stfs(base, box + 28, one);
  const double max = Lfs(base, kBoxMax);
  Stfs(base, box + 16, max);
  Stfs(base, box + 20, max);
  Stfs(base, box + 24, max);
  if (Vector16Size(base, vector) != 0) {
    uint32_t i = 0;
    uint32_t offset = 0;
    do {
      BoxExtend(base, box, Load(base, vector + 4) + offset);
      i++;
      offset += 16;
    } while (i < Vector16Size(base, vector));
  }
  return box;
}

}  // namespace car_scene_draw
