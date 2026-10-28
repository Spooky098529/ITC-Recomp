// X3DAudio natively (x3daudio.h, docs/functions/car-sounds.md). Built with
// strict floating point and no contraction (CMakeLists.txt), so every
// rounding below is the one written.

#include "car_sounds/x3daudio.h"

#include "base/log.h"
#include "guest.h"

#include <cmath>

namespace x3daudio {

namespace {

// The constants the curves read (lfs from the game's data).
constexpr uint32_t kDefaultLast = 0x82002968;  // f6: the default curves' last values
constexpr uint32_t kDefaultOne = 0x82000A30;   // f7
constexpr uint32_t kDefaultZero = 0x82000A34;  // f8
constexpr uint32_t kLfeAzimuth = 0x82001FA8;   // f5: X3DAUDIO_2PI, probably

// lfs: a guest float widened to double.
double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
// stfs: a double rounded to a guest float.
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// A single-precision result (fadds, fmuls, ...): rounded to float and back.
double Single(double value) { return double(float(value)); }

}  // namespace

void VecSub(uint8_t* base, uint32_t a, uint32_t b, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_824819E0 X3DVec::Sub");
  double f13 = Lfs(base, b + 0);
  double f0 = Lfs(base, a + 0);
  Stfs(base, out + 0, Single(f0 - f13));
  f13 = Lfs(base, b + 4);
  f0 = Lfs(base, a + 4);
  Stfs(base, out + 4, Single(f0 - f13));
  f0 = Lfs(base, a + 8);
  f13 = Lfs(base, b + 8);
  Stfs(base, out + 8, Single(f0 - f13));
}

float VecLength(uint8_t* base, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_82481998 X3DVec::Length");
  double f13 = Lfs(base, v + 4);
  f13 = f13 * f13;
  double f0 = Lfs(base, v + 8);
  const double f12 = Lfs(base, v + 0);
  f0 = std::fma(f0, f0, f13);
  f0 = std::fma(f12, f12, f0);
  return float(std::sqrt(f0));
}

float VecDot(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_824819B8 X3DVec::Dot");
  double f13 = Lfs(base, b + 4);
  double f0 = Lfs(base, a + 4);
  f0 = Single(f0 * f13);
  const double f12 = Lfs(base, b + 8);
  f13 = Lfs(base, a + 8);
  const double f11 = Lfs(base, a + 0);
  const double f10 = Lfs(base, b + 0);
  f0 = Single(std::fma(f13, f12, f0));
  return float(std::fma(f11, f10, f0));
}

void VecCross(uint8_t* base, uint32_t a, uint32_t b, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82481A18 X3DVec::Cross");
  double f13 = Lfs(base, b + 4);
  double f0 = Lfs(base, a + 8);
  f0 = Single(f13 * f0);
  double f12 = Lfs(base, b + 8);
  f13 = Lfs(base, a + 4);
  Stfs(base, out + 0, Single(std::fma(f13, f12, -f0)));
  f13 = Lfs(base, b + 8);
  f0 = Lfs(base, a + 0);
  f0 = Single(f0 * f13);
  f12 = Lfs(base, b + 0);
  f13 = Lfs(base, a + 8);
  Stfs(base, out + 4, Single(std::fma(f13, f12, -f0)));
  f13 = Lfs(base, b + 0);
  f0 = Lfs(base, a + 4);
  f0 = Single(f0 * f13);
  f12 = Lfs(base, a + 0);
  f13 = Lfs(base, b + 4);
  Stfs(base, out + 8, Single(std::fma(f13, f12, -f0)));
}

float EvaluateCurve(uint8_t* base, uint32_t curve, double scaler, double distance) {
  ITC_NATIVE_RUNNING("sub_82480818 X3DAudio::EvaluateCurve");
  const uint32_t count = guest::LoadU32(base, curve + 4);
  const double at = Single(distance / scaler);
  uint32_t i = 1;
  bool searched = false;  // the point found (or none past `at`)
  // Four points at a time while at least four are left (a signed check on
  // count - 1, then an unsigned one on count - 3, as the original's).
  if (int32_t(count - 1) >= 4) {
    uint32_t p = guest::LoadU32(base, curve + 0) + 24;
    for (;;) {
      if (Lfs(base, p - 16) > at) {
        searched = true;
        break;
      }
      if (Lfs(base, p - 8) > at) {
        i += 1;
        searched = true;
        break;
      }
      if (Lfs(base, p + 0) > at) {
        i += 2;
        searched = true;
        break;
      }
      if (Lfs(base, p + 8) > at) {
        i += 3;
        searched = true;
        break;
      }
      i += 4;
      p += 32;
      if (!(i < count - 3)) {
        break;
      }
    }
  }
  if (!searched && i < count) {
    uint32_t p = guest::LoadU32(base, curve + 0) + (i << 3);
    for (;;) {
      if (Lfs(base, p) > at) {
        break;
      }
      i += 1;
      p += 8;
      if (!(i < count)) {
        break;
      }
    }
  }
  if (i < count) {
    const uint32_t p = guest::LoadU32(base, curve + 0) + (i << 3);
    double f13 = Lfs(base, p - 8);
    double f12 = Lfs(base, p + 0);
    const double f0 = Single(at - f13);
    const double f10 = Lfs(base, p - 4);
    f13 = Single(f12 - f13);
    const double f11 = Lfs(base, p + 4);
    f12 = Single(f11 - f10);
    f13 = Single(f12 / f13);
    return float(std::fma(f13, f0, f10));
  }
  const uint32_t p = guest::LoadU32(base, curve + 0) + (count << 3);
  return guest::LoadFloat(base, p - 4);
}

uint32_t Curves(uint8_t* base, uint32_t emitter, uint32_t flags, uint32_t dsp, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82480918 X3DAudio::Curves");
  double f6 = Lfs(base, kDefaultLast);
  const double f7 = Lfs(base, kDefaultOne);
  const double f8 = Lfs(base, kDefaultZero);
  uint32_t r3 = 0;
  // The emitter's curve at `field`, or the default {(f8, first), (f7,
  // last)} built in the frame as the original stores it.
  auto evaluate = [&](uint32_t field, double first, double last) -> double {
    const uint32_t curve = guest::LoadU32(base, emitter + field);
    const double distance = Lfs(base, dsp + 36);
    const double scaler = Lfs(base, emitter + 84);
    if (curve != 0) {
      r3 = curve;
    } else {
      Stfs(base, frame + 96, f8);
      Stfs(base, frame + 100, first);
      Stfs(base, frame + 104, f7);
      guest::StoreU32(base, frame + 84, 2);
      Stfs(base, frame + 108, last);
      guest::StoreU32(base, frame + 80, frame + 96);
      r3 = frame + 80;
    }
    return double(EvaluateCurve(base, r3, scaler, distance));
  };
  if (flags & 0x4) {
    Stfs(base, dsp + 16, evaluate(72, f8, f6));
  }
  if (flags & 0x8) {
    Stfs(base, dsp + 20, evaluate(76, f6, f6));
  }
  if (flags & 0x10) {
    Stfs(base, dsp + 24, evaluate(80, f7, f8));
  }
  if (!(flags & 0x1)) {
    return r3;
  }
  f6 = evaluate(64, f7, f8);
  if (guest::LoadU32(base, emitter + 52) == 1) {
    Stfs(base, guest::LoadU32(base, dsp + 0) + 0, f6);
    return r3;
  }
  const double f5 = Lfs(base, kLfeAzimuth);
  uint32_t channel = 0;
  // The channels before the first LFE: the volume.
  if (guest::LoadU32(base, emitter + 52) != 0) {
    uint32_t offset = 0;
    for (;;) {
      const double azimuth = Lfs(base, guest::LoadU32(base, emitter + 60) + offset);
      if (azimuth == f5) {
        break;
      }
      const uint32_t matrix = guest::LoadU32(base, dsp + 0);
      channel += 1;
      Stfs(base, offset + matrix, f6);
      const uint32_t count = guest::LoadU32(base, emitter + 52);
      offset += 4;
      if (!(channel < count)) {
        break;
      }
    }
  }
  if (!(channel < guest::LoadU32(base, emitter + 52))) {
    return r3;
  }
  // The first LFE channel and every one after it: the LFE curve at the LFE
  // channels, the volume at the others.
  const double lfe = evaluate(68, f7, f8);
  Stfs(base, (channel << 2) + guest::LoadU32(base, dsp + 0), lfe);
  uint32_t next = channel + 1;
  if (!(next < guest::LoadU32(base, emitter + 52))) {
    return r3;
  }
  uint32_t offset = next << 2;
  for (;;) {
    const double azimuth = Lfs(base, guest::LoadU32(base, emitter + 60) + offset);
    const uint32_t matrix = guest::LoadU32(base, dsp + 0);
    Stfs(base, offset + matrix, azimuth == f5 ? lfe : f6);
    const uint32_t count = guest::LoadU32(base, emitter + 52);
    next += 1;
    offset += 4;
    if (!(next < count)) {
      break;
    }
  }
  return r3;
}

}  // namespace x3daudio
