// The car scene draw's float and vector steps as the translated code does
// them (see car_scene_draw.h), shared by its files: the scalar float work
// through doubles rounded to single, the vector unit on SSE vectors with
// word 0 in the top lane. Only for files built with -ffp-model=strict and
// -ffp-contract=off (CMakeLists.txt).

#pragma once

#include "guest.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>
#include <cstdint>

namespace car_scene_draw::ops {

using Fpscr = rex::platform::FPSCRPlatform;

inline constexpr uint32_t kOne = 0x82000A30;   // the game's float 1.0
inline constexpr uint32_t kZero = 0x82000A34;  // 0.0

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// lfs and stfs (through the double, as the translated code).
inline double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
inline void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
inline double Fmuls(double a, double b) { return double(float(a * b)); }
inline double Fadds(double a, double b) { return double(float(a + b)); }
inline double Fsubs(double a, double b) { return double(float(a - b)); }
inline double Fdivs(double a, double b) { return double(float(a / b)); }
inline double Fmadds(double a, double b, double c) { return double(float(std::fma(a, b, c))); }
inline double Fsqrts(double a) { return double(float(std::sqrt(a))); }
inline double Frsp(double a) { return double(float(a)); }
inline double Fneg(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) ^ 0x8000000000000000ull);
}
inline double Fabs(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) & ~0x8000000000000000ull);
}

// 16 bytes as two ld and two std, both loads first (ld, ld, std, std).
inline void Copy16(uint8_t* base, uint32_t to, uint32_t from) {
  uint32_t w[4];
  for (int i = 0; i < 4; i++) {
    w[i] = Load(base, from + 4 * i);
  }
  for (int i = 0; i < 4; i++) {
    Store(base, to + 4 * i, w[i]);
  }
}
// 16 bytes as ld, std, ld, std.
inline void Copy16Interleaved(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t half = 0; half < 16; half += 8) {
    const uint32_t a = Load(base, from + half);
    const uint32_t b = Load(base, from + half + 4);
    Store(base, to + half, a);
    Store(base, to + half + 4, b);
  }
}

// --- The vector unit, as the translation does it ---

// Vectors as the translated code holds them: word 0 in the top lane.
inline simde__m128 VectorOf(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int32_t(w0), int32_t(w1), int32_t(w2), int32_t(w3)));
}
// lvx128: the aligned 16 bytes holding `address`.
inline simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
}
inline uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}
// stvx: to the aligned 16 bytes holding `address`.
inline void Stvx(uint8_t* base, uint32_t address, simde__m128 v) {
  const uint32_t at = address & ~0xFu;
  for (int i = 0; i < 4; i++) {
    Store(base, at + 4 * i, WordOf(v, i));
  }
}
// vpermwi128 as the translation's shuffles (the shuffle's immediate).
template <int kImmediate>
simde__m128 Permute(simde__m128 v) {
  return simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v), kImmediate));
}
// vnmsubfp d = -(a b - c), unfused, as translated.
inline simde__m128 Nmsub(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(a, b), c),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
}
// vsel d = mask ? b : a.
inline simde__m128 Select(simde__m128 a, simde__m128 b, simde__m128 mask) {
  return simde_mm_castsi128_ps(
      simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_castps_si128(mask), simde_mm_castps_si128(a)),
                        simde_mm_and_si128(simde_mm_castps_si128(mask), simde_mm_castps_si128(b))));
}
// vcfsx (1, 1) of vspltisw 1: 0.5 in every lane (computed as translated).
inline simde__m128 Half() {
  return simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
}
// The translated vmsum3fp128: x, y, z dotted, in every lane.
inline simde__m128 Dot3(simde__m128 a, simde__m128 b) { return simde_mm_dp_ps(a, b, 0xEF); }
// v scaled to length 1 by its x, y, z (vrsqrtefp as 1 / sqrt, one Newton
// step), or `dot` (0) where its x, y, z dot product `dot` is 0: the games'
// normalize sequence, as translated. Flush-to-zero must be on.
inline simde__m128 Normalized(simde__m128 v, simde__m128 dot) {
  const simde__m128 half = Half();
  const simde__m128 estimate = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(dot));
  const simde__m128 half_dot = simde_mm_mul_ps(dot, half);
  const simde__m128 zero = simde_mm_cmpeq_ps(dot, simde_mm_castsi128_ps(simde_mm_set1_epi32(0)));
  const simde__m128 square = simde_mm_mul_ps(estimate, estimate);
  const simde__m128 step = Nmsub(half_dot, square, half);
  const simde__m128 refined = simde_mm_add_ps(simde_mm_mul_ps(estimate, step), estimate);
  return Select(simde_mm_mul_ps(v, refined), dot, zero);
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

}  // namespace car_scene_draw::ops
