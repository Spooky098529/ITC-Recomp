// The UI movie keys' interpolation (sub_8259FA20, sub_8259FB30,
// sub_8259FC18) and the track that applies them (UiMovie::ApplyTrack,
// sub_825A0548) as plain C++: see ui_movies.h. Step for step as the
// translated code: each single-precision operation is the double operation
// rounded to float (R below), each fmadds std::fma rounded to float. Built
// with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).

#include "ui_movies/ui_movies.h"

#include "base/log.h"
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
constexpr uint32_t kZero = 0x82000A34;      // f30 in ApplyTrack
constexpr uint32_t kStepEpsilon = 0x82002A44;

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

// fctidz, as translated: NaN gives 0x8000000000000000, too large LLONG_MAX,
// else truncated (cvttsd2si: too small gives 0x8000000000000000). The low
// word is what stfiwx stores.
uint32_t FctidzLow(double x) {
  if (std::isnan(x)) {
    return 0;
  }
  if (x > double(LLONG_MAX)) {
    return uint32_t(uint64_t(LLONG_MAX));
  }
  return uint32_t(uint64_t(simde_mm_cvttsd_si64(simde_mm_load_sd(&x))));
}

// The step keys (kind 0): the second key when t is within the float at
// 0x82002A44 of 1 (or t is NaN), else the first.
bool StepToSecond(uint8_t* base, double t) {
  const double one = LoadFloat(base, kOne);
  const double distance = t > one ? R(t - one) : R(one - t);
  return !(distance > LoadFloat(base, kStepEpsilon));
}

uint32_t FloatBits(double x) { return std::bit_cast<uint32_t>(float(x)); }

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

void ApplyTrack(uint8_t* base, uint32_t node, uint32_t track, double t, uint32_t k0,
                uint32_t k1, uint32_t frame, const c_runtime::AtexitCall& atexit,
                const KeyCall& keys) {
  ITC_NATIVE_RUNNING("sub_825A0548 UiMovie::ApplyTrack");
  // The descriptor, from TrackProperty called from the original's frame
  // (its own frame 128 bytes below).
  const uint32_t descriptor =
      uint32_t(TrackProperty(base, guest::LoadU32(base, track), frame - 128, atexit));
  const double zero = LoadFloat(base, kZero);
  // The stand-in first key at frame + 112 (+0 kind 0, +4 zero, +8 0, +12 and
  // +16 zero), written whether or not it's used.
  const uint32_t stand_in = frame + 112;
  guest::StoreU32(base, stand_in, 0);
  guest::StoreU32(base, stand_in + 4, FloatBits(zero));
  guest::StoreU32(base, stand_in + 12, FloatBits(zero));
  guest::StoreU32(base, stand_in + 16, FloatBits(zero));
  guest::StoreU32(base, stand_in + 8, 0);
  uint32_t target = guest::LoadU32(base, descriptor) + node;
  if (guest::LoadU8(base, descriptor + 9) != 0) {
    target += VertexOffset(guest::LoadU32(base, node + 112), guest::LoadU32(base, track + 12));
  }
  const bool sets = guest::LoadU8(base, descriptor + 8) != 0;
  if (k0 == 0) {
    // No first key: the stand-in, holding the target's value when the track
    // sets it (as a float for type 0, the word for 1 and 2).
    k0 = stand_in;
    if (sets) {
      const uint32_t type = guest::LoadU32(base, descriptor + 4);
      if (type == 0) {
        guest::StoreU32(base, stand_in + 8, FloatBits(LoadFloat(base, target)));
      } else if (type < 3) {
        guest::StoreU32(base, stand_in + 8, guest::LoadU32(base, target));
      }
    }
  }
  if (k1 == 0) {
    k1 = k0;
  }
  const uint32_t type = guest::LoadU32(base, descriptor + 4);
  if (type >= 3) {
    return;
  }
  const uint32_t kind = guest::LoadU32(base, k1);
  const double one = LoadFloat(base, kOne);
  if (type == 0) {
    double value = zero;
    if (kind == 0) {
      value = LoadFloat(base, (StepToSecond(base, t) ? k1 : k0) + 8);
    } else if (kind == 1) {
      const double f13 = R(LoadFloat(base, k1 + 8) * t);
      value = R(std::fma(R(one - t), LoadFloat(base, k0 + 8), f13));
    } else if (kind == 2) {
      if (const std::optional<double> cubic = CubicFloat(base, k0, k1, t)) {
        value = *cubic;
      } else {
        value = keys.guest(keys.user, 0x8259FB30, k0, k1, t).f1;
      }
    } else if (kind == 3) {
      const double c = LoadFloat(base, kEaseScale);
      double f11 = R(LoadFloat(base, k1 + 16) * t);
      double f10 = LoadFloat(base, k1 + 12);
      const double f9 = LoadFloat(base, k1 + 8);
      const double f8 = LoadFloat(base, k0 + 8);
      double f0 = R(one - t);
      f11 = R(f11 * c);
      f10 = R(f10 * f0);
      f11 = R(f11 * f0);
      double f12 = R(std::fma(f10, c, t));
      f0 = R(f11 * f0);
      f12 = R(f12 * t);
      f0 = R(std::fma(f12, t, f0));
      const double f13 = R(one - f0);
      f0 = R(f9 * f0);
      value = R(std::fma(f13, f8, f0));
    }
    const double start = sets ? zero : LoadFloat(base, target);
    guest::StoreU32(base, target, FloatBits(R(start + value)));
    return;
  }
  // Types 1 (signed) and 2 (unsigned) integers.
  uint32_t value = 0;
  if (kind == 0) {
    value = guest::LoadU32(base, (StepToSecond(base, t) ? k1 : k0) + 8);
  } else if (kind == 1) {
    const uint32_t w1 = guest::LoadU32(base, k1 + 8), w0 = guest::LoadU32(base, k0 + 8);
    const double v1 = type == 1 ? double(int64_t(int32_t(w1))) : double(int64_t(w1));
    const double v0 = type == 1 ? double(int64_t(int32_t(w0))) : double(int64_t(w0));
    const double f13 = R(R(v1) * t);
    const double f0 = R(std::fma(R(v0), R(one - t), f13));
    value = type == 1 ? Fctiwz(f0) : FctidzLow(f0);
  } else if (kind == 2) {
    if (type == 1) {
      if (const std::optional<uint32_t> cubic = CubicInteger(base, k0, k1, t)) {
        value = *cubic;
      } else {
        value = uint32_t(keys.guest(keys.user, 0x8259FC18, k0, k1, t).r3);
      }
    } else {
      // sub_8259FD40, never seen in play: the original.
      value = uint32_t(keys.guest(keys.user, 0x8259FD40, k0, k1, t).r3);
    }
  } else if (kind == 3) {
    // sub_8259FAA8 (type 2), never seen in play: the original.
    value = type == 1 ? EaseInteger(base, k0, k1, t)
                      : uint32_t(keys.guest(keys.user, 0x8259FAA8, k0, k1, t).r3);
  }
  const uint32_t start = sets ? 0 : guest::LoadU32(base, target);
  guest::StoreU32(base, target, start + value);
}

}  // namespace ui_movies
