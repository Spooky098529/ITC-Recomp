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

// fctid and fctidz as the translated code does them: a NaN gives the
// lowest integer, anything above 2^63 the highest, else cvtsd2si (the
// host's rounding mode) or cvttsd2si (truncating), whose out-of-range
// result is the lowest integer too.
int64_t Fctid(double x) {
  if (std::isnan(x)) {
    return INT64_MIN;
  }
  if (x > double(INT64_MAX)) {
    return INT64_MAX;
  }
  return _mm_cvtsd_si64(_mm_set_sd(x));
}

int64_t Fctidz(double x) {
  if (std::isnan(x)) {
    return INT64_MIN;
  }
  if (x > double(INT64_MAX)) {
    return INT64_MAX;
  }
  return _mm_cvttsd_si64(_mm_set_sd(x));
}

// The default NaN out-of-range sin and cos return (0x8266BC38).
constexpr double kNan = D(0xFFF8000000000000);

// sin and cos's constants, the doubles at 0x8266B718: +0 pi/2, +8 1/pi, +16
// the largest argument (2.2e8), +24 the floats 0.0 and 1.0, +32 -1.0 and
// 0.5, +40 and +48 pi in two parts, +56 to +112 the polynomial.
constexpr uint64_t kSinTable[15] = {
    0x3FF921FB54442D18, 0x3FD45F306DC9C883, 0x41AA39DE00000000, 0x000000003F800000,
    0xBF8000003F000000, 0x400921FB54400000, 0x3DE0B4611A600000, 0xBFC5555555555555,
    0x3F811111111110B0, 0xBF2A01A01A013E1A, 0x3EC71DE3A524F063, 0xBE5AE6454B5DC0AB,
    0x3DE6123C686AD430, 0xBD6AE420DC08499C, 0x3CE880FF6993DF95,
};
double SinK(uint32_t offset) {
  return D(kSinTable[offset / 8]);
}

// sin and cos's odd polynomial in r (r2 = r²), the 1.0 at 0x82002250 last.
double SinPolynomial(double r, double r2) {
  double p = std::fma(SinK(112), r2, SinK(104));
  p = std::fma(p, r2, SinK(96));
  p = std::fma(p, r2, SinK(88));
  p = std::fma(p, r2, SinK(80));
  p = std::fma(p, r2, SinK(72));
  p = std::fma(p, r2, SinK(64));
  p = std::fma(p, r2, SinK(56));
  p = std::fma(p, r2, 1.0);
  return p * r;
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

double Sin(double x) {
  const double ax = Abs(x);
  const double sign = Select(x, 1.0, -1.0);
  const double k = double(Fctid(SinK(8) * ax));  // the nearest multiple of pi
  double r = -std::fma(SinK(40), k, -ax);        // fnmsub: |x| - k pi, in two parts
  const int64_t odd = Fctidz(k) & 1;
  r = -std::fma(SinK(48), k, -r);
  double p = SinPolynomial(r, r * r);
  if (odd != 0) {
    p = Neg(p);
  }
  const double result = Select(ax - SinK(16), kNan, p * sign);
  if (Bits(ax) == 0) {
    return x;
  }
  return result;
}

double Cos(double x) {
  const double ax = Abs(x);
  const double shifted = SinK(0) + ax;  // cos x = sin(|x| + pi/2)
  const double rounded = double(Fctid(SinK(8) * shifted));
  const double k = rounded - 0.5;
  const int64_t odd = Fctidz(rounded) & 1;
  double r = -std::fma(SinK(40), k, -ax);
  r = -std::fma(SinK(48), k, -r);
  double p = SinPolynomial(r, r * r);
  if (odd != 0) {
    p = Neg(p);
  }
  const double result = Select(shifted - SinK(16), kNan, p);
  if (ax == 0.0) {
    return 1.0;
  }
  return result;
}

}  // namespace c_runtime
