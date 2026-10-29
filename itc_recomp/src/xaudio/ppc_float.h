// The float steps XAudio's translated kernels share (resampler.cpp,
// reverb_stages.cpp, reverb_core.cpp): the host's flush to zero as the
// translated code sets it, the scalar unit's roundings and the vector unit's
// loads, stores and operations, each as the translated code does it, so the
// plain C++ gets the original's float results bit for bit.

#pragma once

#include "guest.h"

#include <rex/platform/fpscr.h>
#include <rex/ppc/intrinsics.h>
#include <simde/x86/sse4.1.h>

#include <cmath>
#include <cstdint>

namespace xaudio::ppc_float {

using Fpscr = rex::platform::FPSCRPlatform;
using Vec = simde__m128i;

// The host's flush to zero (and denormals as zero) as the translated code
// sets it: on for the vector unit's float operations, off for the scalar
// unit's; the setting found on entry put back on return.
class Mode {
 public:
  Mode() : saved_(Fpscr::getcsr()) {}
  ~Mode() { Fpscr::setcsr(saved_); }
  Mode(const Mode&) = delete;
  Mode& operator=(const Mode&) = delete;
  void Vector() { Fpscr::setcsr(Fpscr::getcsr() | uint32_t(Fpscr::FlushMask)); }
  void Scalar() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

 private:
  uint32_t saved_;
};

// Flush to zero off for the scalar unit's float work, as the translated code
// sets it; the setting found on entry put back on return.
class ScalarMode {
 public:
  ScalarMode() { mode_.Scalar(); }

 private:
  Mode mode_;
};

// fmuls, fadds and fsubs: the double result rounded to a float.
inline double Round(double value) { return double(float(value)); }
// fmadds: a x b + c, rounded once to a float.
inline double Fma(double a, double b, double c) { return double(float(std::fma(a, b, c))); }

// lvx, lvlx and stvx (aligned down, the words byte-swapped, the lanes in the
// translated code's order).
inline Vec Lvx(uint8_t* base, uint32_t address) {
  return simde_mm_shuffle_epi8(
      simde_mm_load_si128(reinterpret_cast<const Vec*>(REX_RAW_ADDR(address & ~0xFu))),
      simde_mm_load_si128(reinterpret_cast<const Vec*>(rex::ppc::VectorMaskL)));
}
inline Vec Lvlx(uint8_t* base, uint32_t address) {
  return simde_mm_shuffle_epi8(
      simde_mm_load_si128(reinterpret_cast<const Vec*>(REX_RAW_ADDR(address & ~0xFu))),
      simde_mm_load_si128(
          reinterpret_cast<const Vec*>(&rex::ppc::VectorMaskL[(address & 0xF) * 16])));
}
inline void Stvx(uint8_t* base, uint32_t address, Vec value) {
  simde_mm_store_si128(
      reinterpret_cast<Vec*>(REX_RAW_ADDR(address & ~0xFu)),
      simde_mm_shuffle_epi8(value,
                            simde_mm_load_si128(reinterpret_cast<const Vec*>(rex::ppc::VectorMaskL))));
}

// Each value passed through an empty asm so nothing moves across the
// control word's changes.
inline Vec Pin(Vec value) {
  asm volatile("" : "+x"(value));
  return value;
}
inline simde__m128 F(Vec v) { return simde_mm_castsi128_ps(Pin(v)); }
inline Vec I(simde__m128 v) { return Pin(simde_mm_castps_si128(v)); }

inline Vec Splat0(Vec v) { return simde_mm_shuffle_epi32(v, 0xFF); }  // vspltw 0
inline Vec Add(Vec a, Vec b) { return I(simde_mm_add_ps(F(a), F(b))); }
inline Vec Sub(Vec a, Vec b) { return I(simde_mm_sub_ps(F(a), F(b))); }
inline Vec Mul(Vec a, Vec b) { return I(simde_mm_mul_ps(F(a), F(b))); }
inline Vec Madd(Vec a, Vec b, Vec c) { return Add(Mul(a, b), c); }  // vmaddfp: a x b, then + c

}  // namespace xaudio::ppc_float
