// The C runtime's math functions natively (math.h). Each follows the
// translated code instruction by instruction in its order of operations;
// the comments name the PowerPC steps where they matter.

#include "c_runtime/math.h"

#include <emmintrin.h>

#include <bit>
#include <cmath>
#include <cstdint>

namespace c_runtime {

namespace {

constexpr double D(uint64_t bits) {
  return std::bit_cast<double>(bits);
}

uint64_t Bits(double x) {
  return std::bit_cast<uint64_t>(x);
}

double Abs(double x) {  // fabs: the sign bit cleared
  return D(Bits(x) & ~0x8000000000000000ull);
}

double Neg(double x) {  // fneg: the sign bit flipped
  return D(Bits(x) ^ 0x8000000000000000ull);
}

// fsqrt, as the translated code's sqrt() gives it: the SSE2 instruction
// (a negative argument gives the default NaN, 0xFFF8000000000000).
double Sqrt(double x) {
  return _mm_cvtsd_f64(_mm_sqrt_sd(_mm_setzero_pd(), _mm_set_sd(x)));
}

double Select(double a, double b, double c) {  // fsel: a NaN picks c
  return a >= 0.0 ? b : c;
}

// asin and acos's constants, the doubles at 0x8266B470 (as read from guest
// memory). The floats in the first two words are 0.0, 1.0, 2.0 and 0.5.
constexpr uint64_t kAsinTable[16] = {
    0x000000003F800000, 0x400000003F000000,
    0x0000000000000000, 0x3FF921FB54442D18,  // +16: 0 and pi/2, added for asin
    0x400921FB54442D18, 0x3FF921FB54442D18,  // +32: pi and pi/2, for acos
    0xC03B5E55A83A0A62, 0x404C9AA7360AD48A, 0xC043D82CA9A6DA9F, 0x40244E1764EC3927,
    0xBFE64BBDB5E61E65, 0xC06486C03E2B87CC, 0x407A124F101EB843, 0xC077DDCEFC56A848,
    0x4062DE7C96591C70, 0xC037D2E86EF9861F,
};
double AsinK(uint32_t offset) {
  return D(kAsinTable[offset / 8]);
}

// The rational polynomial asin and acos share: z = the square, s = the
// scaled argument; p(z) × z × s / q(z) + s.
double AsinPolynomial(double z, double s) {
  double q = AsinK(120) + z;
  double p = std::fma(AsinK(80), z, AsinK(72));
  q = std::fma(q, z, AsinK(112));
  p = std::fma(p, z, AsinK(64));
  q = std::fma(q, z, AsinK(104));
  p = std::fma(p, z, AsinK(56));
  q = std::fma(q, z, AsinK(96));
  p = std::fma(p, z, AsinK(48));
  q = std::fma(q, z, AsinK(88));
  double r = p * z;
  r = r * s;
  r = r / q;
  return r + s;
}

}  // namespace

double AsinAcos(double x, bool acos) {
  const double ax = Abs(x);
  double z, s;
  uint32_t index;
  if (ax > 0.5) {
    // |x| above 0.5: s = -2 sqrt((1 - |x|) / 2), z = (1 - |x|) / 2.
    index = acos ? 0 : 1;
    z = (1.0 - ax) * 0.5;
    s = Sqrt(z) * -2.0;  // fmul by 2.0 then fneg, as LLVM folds it in the translated code
  } else {
    index = acos ? 1 : 0;
    z = ax * ax;
    s = ax;
    if (!acos && x == 0.0) {
      return x;
    }
  }
  const double r = AsinPolynomial(z, s);
  if (!acos) {
    const double a = D(kAsinTable[2 + index]) + r;
    return Select(x, a, Neg(a));
  }
  const double negative = D(kAsinTable[4 + index]) + r;
  const double positive = D(kAsinTable[2 + index]) - r;
  return Select(x, positive, negative);
}

double Acos(double x) {
  return AsinAcos(x, true);
}

double Asin(double x) {
  // As the kernel's asin, but adding the constant after the polynomial (the
  // same sum: the constants are never NaN).
  const double ax = Abs(x);
  double z, s;
  uint32_t index;
  if (ax > 0.5) {
    index = 1;
    z = (1.0 - ax) * 0.5;
    s = Sqrt(z) * -2.0;  // fmul by 2.0 then fneg, as LLVM folds it in the translated code
  } else {
    index = 0;
    z = ax * ax;
    s = ax;
    if (x == 0.0) {
      return x;
    }
  }
  const double r = AsinPolynomial(z, s) + D(kAsinTable[2 + index]);
  return Select(x, r, Neg(r));
}

}  // namespace c_runtime
