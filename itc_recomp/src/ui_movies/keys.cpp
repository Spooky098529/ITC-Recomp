// The UI movie keys' interpolation (sub_8259FA20, sub_8259FB30,
// sub_8259FC18) as plain C++: see ui_movies.h. Step for step as the
// translated code: each single-precision operation is the double operation
// rounded to float (R below), each fmadds std::fma rounded to float. Built
// with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).

#include "ui_movies/ui_movies.h"

#include "c_runtime/math.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse2.h>

#include <bit>
#include <climits>
#include <cmath>

namespace ui_movies {

namespace {

constexpr uint32_t kOne = 0x82000A30;       // 1.0
constexpr uint32_t kTangentScale = 0x82002A38;
constexpr uint32_t kEaseScale = 0x820021E4;

double R(double x) { return double(float(x)); }

double LoadFloat(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(guest::LoadU32(base, address)));
}

double LoadInteger(uint8_t* base, uint32_t address) {
  // lwa, std, lfd, fcfid: the word sign-extended, converted exactly.
  return double(int64_t(int32_t(guest::LoadU32(base, address))));
}

// fctiwz, as translated: NaN gives 0x80000000, too large INT_MAX, else
// truncated (cvttsd2si: too small gives 0x80000000).
uint32_t Fctiwz(double x) {
  if (std::isnan(x)) {
    return 0x80000000u;
  }
  if (x >= double(INT_MAX)) {
    return uint32_t(INT_MAX);
  }
  return uint32_t(simde_mm_cvttsd_si32(simde_mm_load_sd(&x)));
}

// tan (sub_825A8A50) as its adapter gives it with nearest rounding set: the
// result, or nullopt where the original must run.
std::optional<double> Tan(double x) {
  using Fpscr = rex::platform::FPSCRPlatform;
  if ((Fpscr::getcsr() & Fpscr::RoundMaskVal) != 0) {
    return std::nullopt;
  }
  double result;
  if (!c_runtime::Tan(x, &result)) {
    return std::nullopt;
  }
  return result;
}

}  // namespace

uint32_t EaseInteger(uint8_t* base, uint32_t k0, uint32_t k1, double t) {
  const double one = LoadFloat(base, kOne);
  const double c = LoadFloat(base, kEaseScale);
  double f11 = R(LoadFloat(base, k1 + 16) * t);
  double f10 = LoadFloat(base, k1 + 12);
  double f0 = R(one - t);
  f11 = R(f11 * c);
  f10 = R(f10 * f0);
  f11 = R(f11 * f0);
  double f12 = R(std::fma(f10, c, t));
  f0 = R(f11 * f0);
  f12 = R(f12 * t);
  f0 = R(std::fma(f12, t, f0));
  const double s_rest = R(one - f0);
  const double v1 = R(LoadInteger(base, k1 + 8));
  const double v0 = R(LoadInteger(base, k0 + 8));
  f0 = R(v1 * f0);
  f0 = R(std::fma(v0, s_rest, f0));
  return Fctiwz(f0);
}

std::optional<double> CubicFloat(uint8_t* base, uint32_t k0, uint32_t k1, double t) {
  const double one = LoadFloat(base, kOne);
  const double v0 = LoadFloat(base, k0 + 8);
  const double u = R(one - t);
  const double dt = R(LoadFloat(base, k1 + 4) - LoadFloat(base, k0 + 4));
  const std::optional<double> tan1 = Tan(LoadFloat(base, k1 + 16));
  if (!tan1) {
    return std::nullopt;
  }
  const double v1 = LoadFloat(base, k1 + 8);
  const double f26 = R(*tan1);
  const std::optional<double> tan2 = Tan(LoadFloat(base, k1 + 12));
  if (!tan2) {
    return std::nullopt;
  }
  const double c1 = LoadFloat(base, kTangentScale);
  const double c2 = LoadFloat(base, kEaseScale);
  double f12 = R(f26 * dt);
  double f11 = R(LoadFloat(base, k1 + 8) * t);
  double f10 = R(LoadFloat(base, k0 + 8) * u);
  f12 = R(std::fma(f12, c1, v0));
  f11 = R(f11 * t);
  f10 = R(f10 * u);
  f12 = R(f12 * u);
  f12 = R(f12 * u);
  double f9 = R(f12 * t);
  f12 = R(*tan2);
  f9 = R(f9 * c2);
  f12 = R(f12 * dt);
  double f0 = R(std::fma(f12, c1, v1));
  f0 = R(f0 * u);
  f0 = R(f0 * t);
  f0 = R(f0 * t);
  f0 = R(std::fma(f0, c2, f9));
  f0 = R(std::fma(f11, t, f0));
  return R(std::fma(f10, u, f0));
}

std::optional<uint32_t> CubicInteger(uint8_t* base, uint32_t k0, uint32_t k1, double t) {
  const double one = LoadFloat(base, kOne);
  const double u = R(one - t);
  const double dt = R(LoadFloat(base, k1 + 4) - LoadFloat(base, k0 + 4));
  const std::optional<double> tan1 = Tan(LoadFloat(base, k1 + 16));
  if (!tan1) {
    return std::nullopt;
  }
  const double f28 = R(*tan1);
  const std::optional<double> tan2 = Tan(LoadFloat(base, k1 + 12));
  if (!tan2) {
    return std::nullopt;
  }
  const double c1 = LoadFloat(base, kTangentScale);
  const double c2 = LoadFloat(base, kEaseScale);
  double f11 = R(f28 * dt);
  double f12 = R(*tan2);
  f12 = R(f12 * dt);
  // The values: k0's and k1's +8 (read before the tan calls in the
  // original, and again after them).
  const double f10 = R(LoadInteger(base, k0 + 8));
  const double f9 = R(LoadInteger(base, k1 + 8));
  double f8 = LoadInteger(base, k1 + 8);
  double f7 = LoadInteger(base, k0 + 8);
  f11 = R(std::fma(f11, c1, f10));
  double f0 = R(std::fma(f12, c1, f9));
  f8 = R(f8);
  f7 = R(f7);
  f11 = R(f11 * u);
  f0 = R(f0 * u);
  double g10 = R(f8 * t);
  f8 = R(f7 * u);
  f11 = R(f11 * u);
  f0 = R(f0 * t);
  g10 = R(g10 * t);
  f8 = R(f8 * u);
  f11 = R(f11 * t);
  f0 = R(f0 * t);
  f12 = R(f11 * c2);
  f0 = R(std::fma(f0, c2, f12));
  f0 = R(std::fma(g10, t, f0));
  f0 = R(std::fma(f8, u, f0));
  return Fctiwz(f0);
}

}  // namespace ui_movies
