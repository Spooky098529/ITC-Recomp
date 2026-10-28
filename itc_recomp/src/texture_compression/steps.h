// The PowerPC float steps texture compression's plain C++ is written in
// (src/texture_compression/, docs/functions/texture-compression.md), as the
// translated code does them: lfs widens a single to a double; fmuls, fadds,
// fsubs and fdivs round the double's result to single; fmadds is std::fma on
// the doubles rounded to single; fcfid then frsp converts an integer through a
// double; fctiwz truncates as the translation does. Only for files built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt), with flush-to-zero
// off (the adapters' job). Operands keep the translated code's order (x86
// picks the first operand's NaN).

#pragma once

#include "guest.h"

#include <simde/x86/sse2.h>

#include <cmath>
#include <cstdint>

namespace texture_compression::steps {

inline double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
inline double Fmuls(double a, double b) {
  return double(float(a * b));
}
inline double Fadds(double a, double b) {
  return double(float(a + b));
}
inline double Fsubs(double a, double b) {
  return double(float(a - b));
}
inline double Fdivs(double a, double b) {
  return double(float(a / b));
}
inline double Fmadds(double a, double b, double c) {
  return double(float(std::fma(a, b, c)));
}
// fcfid of a sign-extended (or zero-extended) word, then frsp.
inline double IntToSingle(int64_t value) {
  return double(float(double(value)));
}
// fctiwz: NaN to INT_MIN, INT_MAX and above to INT_MAX, else truncated
// (cvttsd: INT_MIN below the range).
inline int32_t Fctiwz(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  return simde_mm_cvttsd_si32(simde_mm_set_sd(value));
}

}  // namespace texture_compression::steps
