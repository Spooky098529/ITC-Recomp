// The translated code's float steps the camera modes use (src/camera_modes/):
// the scalar work as its doubles rounded to single, the fused steps as
// std::fma rounded to single in the PowerPC code's operand order, and the
// vector unit's work on SSE vectors with word 0 in the top lane (as
// src/cameras/cameras.cpp does it).

#pragma once

#include "guest.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>
#include <cstdint>

namespace camera_modes::steps {

using Fpscr = rex::platform::FPSCRPlatform;

// lfs and stfs.
inline double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
inline void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
inline uint32_t Bits(double value) { return std::bit_cast<uint32_t>(float(value)); }

inline double Single(double a) { return double(float(a)); }
inline double Fmuls(double a, double b) { return double(float(a * b)); }
inline double Fadds(double a, double b) { return double(float(a + b)); }
inline double Fsubs(double a, double b) { return double(float(a - b)); }
inline double Fdivs(double a, double b) { return double(float(a / b)); }
// fmadds: a b + c; fmsubs: a b - c; fnmsubs: -(a b - c); fnmadds: -(a b + c).
inline double Fmadds(double a, double b, double c) { return double(float(std::fma(a, b, c))); }
inline double Fmsubs(double a, double b, double c) { return double(float(std::fma(a, b, -c))); }
inline double Fnmsubs(double a, double b, double c) {
  return double(float(-std::fma(a, b, -c)));
}
inline double Fnmadds(double a, double b, double c) {
  return double(float(-std::fma(a, b, c)));
}
// fneg: the sign bit flipped.
inline double Fneg(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) ^ 0x8000000000000000ull);
}

// fctiwz: NaN to INT_MIN, INT_MAX and above to INT_MAX, else truncated.
inline int32_t Fctiwz(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  return simde_mm_cvttsd_si32(simde_mm_set_sd(value));
}

// --- The vector unit ---

// Vectors as the translated code holds them: word 0 in the top lane.
inline simde__m128 VectorOf(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int32_t(w0), int32_t(w1), int32_t(w2), int32_t(w3)));
}
// Word i (0 the top lane).
inline uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}
// stvx: to the aligned 16 bytes holding `address`.
inline void Stvx(uint8_t* base, uint32_t address, simde__m128 v) {
  const uint32_t at = address & ~0xFu;
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(base, at + 4 * i, WordOf(v, i));
  }
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

}  // namespace camera_modes::steps
