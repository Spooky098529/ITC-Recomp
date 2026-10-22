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

// atan and atan2's constants, the doubles at 0x8266B660: +8 pi/2, +16 pi,
// +24 2 - sqrt(3) (the reduction's bound), +40 sqrt(3), +48 the largest
// argument reduced, +56 to +120 the polynomial, +128 0, pi/6, pi/2 and pi/3
// (added by range); the floats 0.0 at +168 and 1.0 at +176.
constexpr uint64_t kAtanTable[23] = {
    0x3E46A09E667F3BCD, 0x3FF921FB54442D18, 0x400921FB54442D18, 0x3FD126145E9ECD56,
    0x3FE76CF5D0B09955, 0x3FFBB67AE8584CAA, 0x7FD0000000000000, 0xC02B60A651061CE2,
    0xC034817FB9E2BCCB, 0xC020FD3F5C8D6A63, 0xBFEACD7AD9B187BD, 0x4044887CBCC495A9,
    0x40558A12040B6DA5, 0x404DCA0A320DA3D7, 0x402E0C49E14AC710, 0x3FF0000000000000,
    0x0000000000000000, 0x3FE0C152382D7366, 0x3FF921FB54442D18, 0x3FF0C152382D7366,
    0x0010000000000000, 0x000000003F000000, 0x3F80000000000000,
};
double AtanK(uint32_t offset) {
  return D(kAtanTable[offset / 8]);
}

// atan and atan2's shared steps, from t (0 <= t <= 1 or a NaN) and its range
// so far (0, or 2 for 1/|x|): reduced by (sqrt(3) t - 1)/(sqrt(3) + t)
// above 2 - sqrt(3), a rational polynomial, negated for ranges above 1, plus
// the range's constant.
double AtanReduced(double t, int range) {
  if (t > AtanK(24)) {
    range++;
    const double denominator = AtanK(40) + t;
    t = std::fma(AtanK(40), t, -1.0) / denominator;  // fmsub
  }
  const double z = t * t;
  double q = AtanK(112) + z;
  double p = std::fma(AtanK(80), z, AtanK(72));
  q = std::fma(q, z, AtanK(104));
  p = std::fma(p, z, AtanK(64));
  q = std::fma(q, z, AtanK(96));
  p = std::fma(p, z, AtanK(56));
  q = std::fma(q, z, AtanK(88));
  double r = p * z;
  r = r * t;
  r = r / q;
  r = r + t;
  if (range > 1) {
    r = Neg(r);
  }
  return AtanK(128 + 8 * range) + r;
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

namespace {

// fctiwz as the translated code does it: a NaN gives the lowest int, 2^31 - 1
// and above the highest, else cvttsd2si (truncating).
int32_t Fctiwz(double x) {
  if (std::isnan(x)) {
    return INT32_MIN;
  }
  if (x >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  return _mm_cvttsd_si32(_mm_set_sd(x));
}

uint32_t High16(double x) {
  return uint32_t(Bits(x) >> 48);
}

constexpr double kInfinity = D(0x7FF0000000000000);  // 0x8266BC30
constexpr double kNegativeZero = D(0x8000000000000000);  // 0x8266BC50

// log's constants, the doubles at 0x82072588 (+0 sqrt(0.5), +8 ln 2's high
// part, +24 1/ln 10, +32 to +104 the rational polynomial, +96 ln 2's low
// part, +120 2^53 (the denormal scale), +128 the smallest normal, +136 the
// largest finite double).
constexpr uint64_t kLogTable[18] = {
    0x3FE6A09E667F3BCD, 0x3FE6300000000000, 0xBF2BD0105C610CA8, 0x3FDBCB7B1526E50E,
    0xC05007FF12B3B59A, 0x4030624A2016AFED, 0xBFE94415B356BD29, 0xC0880BFE9C0D9077,
    0x40738083FA15267E, 0xC041D5804B67CE0F, 0x40880BFE9C0D9077, 0x405007FF12B3B59A,
    0x3F2BD0105C610CA8, 0x4041D5804B67CE0F, 0x3FE94415B356BD29, 0x4340000000000000,
    0x0010000000000000, 0x7FEFFFFFFFFFFFFF,
};
double LogK(uint32_t address) {  // by guest address, 0x82072588 to 0x82072610
  return D(kLogTable[(address - 0x82072588) / 8]);
}

// pow's constants, the doubles at 0x82072438 (+8 to +136 2^(-j/16) for j =
// 0 to 16, +144 to +208 their low parts (j even), +216 1/ln 2, +224 its
// low part, +232 to +256 the log polynomial, +264 to +312 the exp
// polynomial (ln 2, (ln 2)^2/2...), +320 and +328 the bounds 40960 and
// -40928 on 16 × y log2 x).
constexpr uint64_t kPowTable[42] = {
    0x0000000000000000, 0x3FF0000000000000, 0x3FEEA4AFA2A490DA, 0x3FED5818DCFBA487,
    0x3FEC199BDD85529C, 0x3FEAE89F995AD3AD, 0x3FE9C49182A3F090, 0x3FE8ACE5422AA0DB,
    0x3FE7A11473EB0187, 0x3FE6A09E667F3BCD, 0x3FE5AB07DD485429, 0x3FE4BFDAD5362A27,
    0x3FE3DEA64C123422, 0x3FE306FE0A31B715, 0x3FE2387A6E756238, 0x3FE172B83C7D517B,
    0x3FE0B5586CF9890F, 0x3FE0000000000000, 0x0000000000000000, 0xBC8EA00000000000,
    0x3C71100000000000, 0x3C6C600000000000, 0xBC74100000000000, 0x3C86300000000000,
    0x3C7AE00000000000, 0x3C89B00000000000, 0x3C88A80000000000, 0x3FF71547652B82FE,
    0x3FDC551D94AE0BF8, 0x3FB555555555554D, 0x3F899999999E080E, 0x3F6249242E278DAC,
    0x3F3C78FDDB4AFC28, 0x3FE62E42FEFA39EF, 0x3FCEBFBDFF82C4CE, 0x3FAC6B08D703026D,
    0x3F83B2AB6E131D98, 0x3F55D87E18D7CD9F, 0x3F242F7AE0384C74, 0x3EEF4EDDE392CC80,
    0x40E4000000000000, 0xC0E3FC0000000000,
};
double PowK(uint32_t offset) {
  return D(kPowTable[offset / 8]);
}

// pow's infinity cases (sub_825A8D50): x or y infinite, neither a NaN.
double PowInfinite(double x, double y) {
  const double ax = Abs(x);
  const uint64_t by = Bits(y);
  if (by == 0x7FF0000000000000) {
    if (ax > 1.0) {
      return kInfinity;
    }
    if (ax < 1.0) {
      return 0.0;
    }
    return 1.0;
  }
  if (by == 0xFFF0000000000000) {
    if (ax > 1.0) {
      return 0.0;
    }
    if (ax < 1.0) {
      return kInfinity;
    }
    return kNan;
  }
  if (Bits(x) == 0x7FF0000000000000) {
    if (y > 0.0) {
      return kInfinity;
    }
    if (y < 0.0) {
      return 0.0;
    }
    return 1.0;
  }
  // x is -infinity (the only case left that pow passes).
  const int32_t type = IntType(y);
  if (y > 0.0) {
    return type == 1 ? Neg(kInfinity) : kInfinity;
  }
  if (!(y < 0.0)) {
    return 1.0;
  }
  return type == 1 ? kNegativeZero : 0.0;
}

}  // namespace

double Copysign(double magnitude, double sign) {
  return D((Bits(magnitude) & ~0x8000000000000000ull) | (Bits(sign) & 0x8000000000000000ull));
}

double SetExp(double x, int32_t exponent) {
  const uint64_t high = ((uint32_t(exponent + 1022) << 4) & 0xFFF0) | (High16(x) & 0x800F);
  return D((Bits(x) & 0x0000FFFFFFFFFFFFull) | (high << 48));
}

int32_t GetExp(double x) {
  return int32_t((High16(x) >> 4) & 0x7FF) - 1022;
}

double Decomp(double x, int32_t* exponent) {
  if (x == 0.0) {
    *exponent = 0;
    return x;
  }
  uint64_t bits = Bits(x);
  int32_t e;
  if ((High16(x) & 0x7FF0) == 0) {
    // A denormal: shifted up until the exponent's lowest bit is set (the sign
    // shifts out), then that bit cleared and the sign put back.
    const bool negative = x < 0.0;
    e = -1021;
    do {
      bits <<= 1;
      e--;
    } while ((bits & 0x0010000000000000ull) == 0);
    bits &= ~0x0010000000000000ull;
    if (negative) {
      bits |= 0x8000000000000000ull;
    }
  } else {
    e = int32_t((High16(x) & 0x7FF0) >> 4) - 1022;
  }
  *exponent = e;
  return D((bits & 0x800FFFFFFFFFFFFFull) | 0x3FE0000000000000ull);
}

int32_t IntType(double y) {
  if ((High16(y) & 0x7FF0) == 0 && (Bits(y) & 0x000FFFFFFFFFFFFFull) != 0) {
    return 0;  // a denormal
  }
  if (!(double(Fctid(y)) == y)) {
    return 0;
  }
  const double half = y * 0.5;
  if (!(double(Fctid(half)) == half)) {
    return 1;
  }
  return 2;
}

double Log(double x) {
  if (x == 1.0) {
    return 0.0;
  }
  const uint32_t high = High16(x);
  if ((high & 0x7FF0) == 0x7FF0) {
    if (x > LogK(0x82072610)) {
      return x;  // +infinity
    }
    return Neg(kNan);
  }
  if (!(x > 0.0)) {
    if (x == 0.0) {
      return Neg(kInfinity);
    }
    return Neg(kNan);
  }
  double v = x;
  uint32_t h = high;
  int32_t e;
  if (x < LogK(0x82072608)) {
    v = x * LogK(0x82072600);  // a denormal scaled by 2^53
    h = High16(v);
    e = int32_t((h >> 4) & 0x7FF) - 1075;
  } else {
    e = int32_t(((high & 0x7FF0) >> 4) & 0xFFF) - 1022;
  }
  // The mantissa in [0.5, 1).
  const double m = D((Bits(v) & 0x0000FFFFFFFFFFFFull) |
                     (uint64_t((h & 0x800F) | 0x3FE0) << 48));
  double f0, f13;
  if (m > LogK(0x82072588)) {
    const double f12 = m - 0.5;
    const double f11 = m + 1.0;
    f0 = f12 - 0.5;
    f13 = f11 * 0.5;
  } else {
    f0 = m - 0.5;
    e--;
    const double f12 = f0 + 1.0;
    f13 = f12 * 0.5;
  }
  const double z = f0 / f13;
  const double ln2_high = LogK(0x82072590);
  const double fe = double(int64_t(e));
  const double w = z * z;
  double f7 = w - LogK(0x820725F0);
  const double f6 = fe * LogK(0x820725E8);
  double f9 = -std::fma(w, LogK(0x820725F8), -LogK(0x820725B0));  // fnmsub
  f7 = std::fma(f7, w, LogK(0x820725C8));
  f9 = std::fma(f9, w, -LogK(0x820725E0));  // fmsub
  const double f11 = std::fma(f7, w, -LogK(0x820725D8));
  double r = f9 * w;
  r = r / f11;
  r = r + 1.0;
  r = std::fma(r, z, -f6);
  return std::fma(fe, ln2_high, r);
}

double Pow(double x, double y) {
  if (y == 0.0) {
    return 1.0;
  }
  if (x == 0.0) {
    const int32_t type = IntType(y);
    if (y < 0.0) {
      return type == 1 ? Copysign(kInfinity, x) : kInfinity;
    }
    if (y > 0.0) {
      return type == 1 ? x : 0.0;
    }
  }
  if ((High16(x) & 0x7FF0) == 0x7FF0 || (High16(y) & 0x7FF0) == 0x7FF0) {
    // Infinities and NaNs: a NaN gives x + y; else the infinity cases.
    const uint64_t bx = Bits(x);
    const uint64_t by = Bits(y);
    const uint32_t qx = High16(x) & 0x7FF8;
    const uint32_t qy = High16(y) & 0x7FF8;
    if (qx == 0x7FF0 && (bx & 0x0007FFFFFFFFFFFFull) != 0) {
      return x + y;
    }
    if (qy == 0x7FF0 && (by & 0x0007FFFFFFFFFFFFull) != 0) {
      return x + y;
    }
    if (qx == 0x7FF8 || qy == 0x7FF8) {
      return x + y;
    }
    return PowInfinite(x, y);
  }
  double sign = 1.0;  // f25
  if (x < 0.0) {
    const int32_t type = IntType(y);
    if (type == 1) {
      sign = -1.0;
    } else if (type != 2) {
      return kNan;
    }
    x = Neg(x);
  }
  if (Abs(y) > 1e20) {  // the double at 0x8266B7A0
    if (y < 0.0) {
      x = 1.0 / x;
    }
    if (x > 1.0) {
      return kInfinity * sign;
    }
    if (x < 1.0) {
      return sign * 0.0;
    }
    return sign;
  }
  int32_t ex;
  double m = Decomp(x, &ex);  // f29
  double result;              // f31
  int32_t exponent;           // r4
  if (!(y > 128.0) && IntType(x) != 0 && IntType(y) != 0 && y > 0.0) {
    // An integer power by squaring the mantissa; the exponents added apart.
    int32_t n = Fctiwz(y);
    result = 1.0;
    const int64_t scaled = int64_t(n) * int64_t(ex);
    while (n != 0) {
      if ((n & 1) != 0) {
        result = result * m;
      }
      m = m * m;
      n >>= 1;
    }
    exponent = int32_t(int64_t(GetExp(result)) + scaled);
    if (exponent > 2560) {
      return (kInfinity * result) * sign;
    }
    if (exponent < -2557) {
      return (result * sign) * 0.0;
    }
  } else {
    // log2 x in sixteenths: the mantissa's nearest 2^(-j/16) by a binary
    // search (i = 1 to 16, its sign kept apart), then a series in the rest.
    int32_t i = 1;
    if (!(m > PowK(72))) {
      i = 9;
    }
    if (!(m > PowK(32 + 8 * i))) {
      i += 4;
    }
    if (!(m > PowK(16 + 8 * i))) {
      i += 2;
    }
    const int32_t sixteenths = int32_t(uint32_t(ex) << 4) - i;  // r9
    const int32_t distance = sixteenths < 0 ? -sixteenths : sixteenths;
    const double sixteenth = 0.0625;  // the double at 0x8203CD50
    double f10, f13;
    if (distance == 1) {
      f10 = Log(x) * PowK(216);
      f13 = 0.0;
    } else {
      const double f12_0 = PowK(256);
      const double f11_0 = PowK(216);
      double f0 = PowK(8 + 8 * i);
      const int32_t half = (i + 1) / 2;
      double t = m - f0;
      f0 = f0 + m;
      const double f9 = double(int64_t(sixteenths));
      f13 = PowK(144 + 8 * half);
      f13 = t - f13;
      f10 = f9 * sixteenth;
      const double f9b = PowK(224);
      f13 = f13 / f0;
      f0 = f13 * 2.0;
      f13 = f0 * f0;
      const double f8 = f0 * f9b;
      double p = std::fma(f13, f12_0, PowK(248));
      p = std::fma(p, f13, PowK(240));
      p = std::fma(p, f13, PowK(232));
      f13 = p * f13;
      f13 = f13 * f0;
      f13 = std::fma(f13, f11_0, f8);
      f13 = f13 + f0;
    }
    // y × log2 x in sixteenths, split into whole and fraction with care.
    double f11 = f13 * y;
    const double sixteen = 16.0;  // the double at 0x820143E0
    double f12 = y * sixteen;
    double f13b = double(Fctid(f12));
    f13b = f13b * sixteenth;
    f12 = y - f13b;
    f12 = std::fma(f12, f10, f11);
    f11 = f12 * sixteen;
    f11 = double(Fctid(f11));
    f11 = f11 * sixteenth;
    f13b = std::fma(f13b, f10, f11);
    const double f9 = f12 - f11;
    f12 = f13b * sixteen;
    f12 = double(Fctid(f12));
    f12 = f12 * sixteenth;
    f13b = f13b - f12;
    f13b = f13b + f9;
    f11 = f13b * sixteen;
    f11 = double(Fctid(f11));
    f11 = f11 * sixteenth;
    f12 = f12 + f11;
    f13b = f13b - f11;
    const double whole = f12 * sixteen;
    if (whole > PowK(320)) {
      return kInfinity * sign;
    }
    if (whole < PowK(328)) {
      return sign * 0.0;
    }
    int32_t n = Fctiwz(whole);
    if (f13b > 0.0) {
      n = n + 1;
      f13b = f13b - sixteenth;
    }
    // 2^(fraction) by a polynomial, times 2^(-j/16) from the table.
    double p = std::fma(f13b, PowK(312), PowK(304));
    const int32_t r30 = (n >= 0 ? 1 : 0) + n / 16;
    p = std::fma(p, f13b, PowK(296));
    const uint32_t j = (uint32_t(r30) << 4) - uint32_t(n);
    p = std::fma(p, f13b, PowK(288));
    p = std::fma(p, f13b, PowK(280));
    p = std::fma(p, f13b, PowK(272));
    p = std::fma(p, f13b, PowK(264));
    p = std::fma(p, f13b, 1.0);
    result = p * PowK(8 + 8 * j);
    exponent = GetExp(result) + r30;
  }
  if (exponent > 1024) {
    return kInfinity * sign;
  }
  if (exponent < -1021) {
    return sign * 0.0;
  }
  return SetExp(result, exponent) * sign;
}

namespace {
constexpr double kRoundBound = D(0x43ABC16D674EC800);  // 1e18, at 0x82072430
}  // namespace

double Floor(double x) {
  const double t = double(Fctidz(x));
  const double ax = Abs(x);
  const double below = t - 1.0;
  const double fraction = x - t;
  const double room = kRoundBound - ax;
  double r = Select(fraction, t, below);
  r = Select(room, r, x);
  return Select(Neg(ax), x, r);  // +-0 returned as it is
}

double Ceil(double x) {
  const double t = double(Fctidz(x));
  const double ax = Abs(x);
  const double above = t + 1.0;
  const double fraction = t - x;
  const double room = kRoundBound - ax;
  double r = Select(fraction, t, above);
  r = Select(room, r, x);
  return Select(Neg(ax), x, r);
}

double Atan(double x) {
  const double ax = Abs(x);
  double a;
  if (ax > 1.0) {
    a = ax > AtanK(48) ? AtanK(8) : AtanReduced(1.0 / ax, 2);
  } else {
    a = AtanReduced(ax, 0);
  }
  const double result = Select(x, a, Neg(a));
  if (Bits(ax) == 0) {
    return x;
  }
  return result;
}

double Atan2(double y, double x) {
  double a;
  if (x == 0.0) {
    if (y == 0.0) {
      // atan2(+-0, +0) is y; atan2(+-0, -0) is +-pi.
      if ((Bits(x) >> 63) == 0) {
        return y;
      }
      return (Bits(y) >> 63) != 0 ? Neg(AtanK(16)) : AtanK(16);
    }
    a = AtanK(8);
  } else {
    // The smaller magnitude over the larger: range 2 when |y| is larger.
    const double ax = Abs(x);
    const double ay = Abs(y);
    double t;
    int range = 0;
    if (ay > ax) {
      t = ax / ay;
      range = 2;
    } else {
      t = ay / ax;
    }
    a = AtanReduced(t, range);
  }
  const double behind = AtanK(16) - a;  // pi - a, for x < 0
  const double result = Select(x, a, behind);
  return (Bits(y) >> 63) != 0 ? Neg(result) : result;
}

}  // namespace c_runtime
