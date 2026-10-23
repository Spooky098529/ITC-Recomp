// The translated code's float steps the string table and font use
// (strings_and_font.cpp, font.cpp): the conversions as the translation does
// them, so results match it bit for bit.

#pragma once

#include "guest.h"

#include <simde/x86/sse2.h>

#include <bit>
#include <climits>
#include <cmath>
#include <cstdint>

namespace strings_and_font::steps {

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

// fctidz: NaN to INT64_MIN, above INT64_MAX to INT64_MAX, else truncated.
inline int64_t Fctidz(double value) {
  if (std::isnan(value)) {
    return INT64_MIN;
  }
  if (value > double(LLONG_MAX)) {
    return LLONG_MAX;
  }
  return simde_mm_cvttsd_si64(simde_mm_set_sd(value));
}

// An integer through the stack and fcfid, rounded to single (frsp).
inline double ToSingle(int64_t value) { return double(float(double(value))); }

// frsp.
inline double Single(double value) { return double(float(value)); }

// lfs.
inline double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}

// stfs.
inline void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

// lfd.
inline double Lfd(uint8_t* base, uint32_t address) {
  const uint64_t bits = (uint64_t(guest::LoadU32(base, address)) << 32) |
                        guest::LoadU32(base, address + 4);
  return std::bit_cast<double>(bits);
}

}  // namespace strings_and_font::steps
