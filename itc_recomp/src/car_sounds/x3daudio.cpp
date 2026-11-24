// X3DAudio natively (x3daudio.h, docs/functions/car-sounds.md). Built with
// strict floating point and no contraction (CMakeLists.txt), so every
// rounding below is the one written.

#include "car_sounds/x3daudio.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "guest.h"
#include "schema/car_sounds.h"

#include "base/float_mode.h"

#include <bit>
#include <cmath>

namespace x3daudio {

namespace {

using ConeView = schema::X3DAudioCone;
using Curve = schema::X3DAudioCurve;
using Dsp = schema::X3DAudioDspSettings;
using Emitter = schema::X3DAudioEmitter;
using Handle = schema::X3DAudioHandle;
using Listener = schema::X3DAudioListener;
using Vec = schema::X3DAudioVector;

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

// lfd and stfd: a guest doubleword's bits.
uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return uint64_t(guest::LoadU32(base, address)) << 32 | guest::LoadU32(base, address + 4);
}
void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}
double Lfd(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(LoadU64(base, address));
}

// The matrix's word for destination row `row`, source channel `channel`
// (mullw, add, the shift's low word), from the DSP settings' +8 (the source
// channel count) and +0 (the matrix) read again, as the original does.
uint32_t MatrixWord(uint8_t* base, uint32_t dsp, uint32_t row, uint32_t channel) {
  const uint32_t sources = Dsp(base, dsp).source_channels();
  const uint32_t matrix = Dsp(base, dsp).matrix().address;
  return ((sources * row + channel) << 2) + matrix;
}

// Pan's constants (lfd and lfs from the game's data).
constexpr uint32_t kStereoAzimuths[2] = {0x8201E2B0, 0x8201E2A8};  // the second is f7 too
constexpr uint32_t kSurroundAzimuths[6] = {0x8204D2F0, 0x82002CD8, 0x82002258,
                                           0x8204D2E8, 0x8204D2E0, 0x8204D2D8};  // [2] is f12
constexpr uint32_t kPanLeftStart = 0x8204B940;   // f13: the nearest difference on one side
constexpr uint32_t kPanRightStart = 0x8204B938;  // f11: on the other
constexpr uint32_t kPanWrapBelow = 0x8201E298;   // f8: -pi, probably
constexpr uint32_t kPanWrapAbove = 0x8201E2A0;   // f9: pi, probably
constexpr uint32_t kPanWrap = 0x8201E458;        // f10: 2 pi, probably
constexpr uint32_t kPanNear = 0x8204D2D0;        // the difference that puts all on one speaker
constexpr uint32_t kZero = 0x82000A34;           // lfs: 0, probably
constexpr uint32_t kOne = 0x82000A30;            // lfs: 1, probably

// Matrix's constants (lfs).
constexpr uint32_t kAxisBelow = 0x8204D2F8;  // f24
constexpr uint32_t kAxisAbove = 0x82006910;  // f26

}  // namespace

void VecSub(uint8_t* base, uint32_t a, uint32_t b, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_824819E0 X3DVec::Sub");
  double f13 = double(Vec(base, b).x());
  double f0 = double(Vec(base, a).x());
  Vec(base, out).set_x(float(Single(f0 - f13)));
  f13 = double(Vec(base, b).y());
  f0 = double(Vec(base, a).y());
  Vec(base, out).set_y(float(Single(f0 - f13)));
  f0 = double(Vec(base, a).z());
  f13 = double(Vec(base, b).z());
  Vec(base, out).set_z(float(Single(f0 - f13)));
}

float VecLength(uint8_t* base, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_82481998 X3DVec::Length");
  double f13 = double(Vec(base, v).y());
  f13 = f13 * f13;
  double f0 = double(Vec(base, v).z());
  const double f12 = double(Vec(base, v).x());
  f0 = std::fma(f0, f0, f13);
  f0 = std::fma(f12, f12, f0);
  return float(std::sqrt(f0));
}

float VecDot(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_824819B8 X3DVec::Dot");
  double f13 = double(Vec(base, b).y());
  double f0 = double(Vec(base, a).y());
  f0 = Single(f0 * f13);
  const double f12 = double(Vec(base, b).z());
  f13 = double(Vec(base, a).z());
  const double f11 = double(Vec(base, a).x());
  const double f10 = double(Vec(base, b).x());
  f0 = Single(std::fma(f13, f12, f0));
  return float(std::fma(f11, f10, f0));
}

void VecCross(uint8_t* base, uint32_t a, uint32_t b, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82481A18 X3DVec::Cross");
  double f13 = double(Vec(base, b).y());
  double f0 = double(Vec(base, a).z());
  f0 = Single(f13 * f0);
  double f12 = double(Vec(base, b).z());
  f13 = double(Vec(base, a).y());
  Vec(base, out).set_x(float(Single(std::fma(f13, f12, -f0))));
  f13 = double(Vec(base, b).z());
  f0 = double(Vec(base, a).x());
  f0 = Single(f0 * f13);
  f12 = double(Vec(base, b).x());
  f13 = double(Vec(base, a).z());
  Vec(base, out).set_y(float(Single(std::fma(f13, f12, -f0))));
  f13 = double(Vec(base, b).x());
  f0 = double(Vec(base, a).y());
  f0 = Single(f0 * f13);
  f12 = double(Vec(base, a).x());
  f13 = double(Vec(base, b).y());
  Vec(base, out).set_z(float(Single(std::fma(f13, f12, -f0))));
}

float EvaluateCurve(uint8_t* base, uint32_t curve, double scaler, double distance) {
  ITC_NATIVE_RUNNING("sub_82480818 X3DAudio::EvaluateCurve");
  const uint32_t count = Curve(base, curve).count();
  const double at = Single(distance / scaler);
  uint32_t i = 1;
  bool searched = false;  // the point found (or none past `at`)
  // Four points at a time while at least four are left (a signed check on
  // count - 1, then an unsigned one on count - 3, as the original's).
  if (int32_t(count - 1) >= 4) {
    uint32_t p = Curve(base, curve).points() + 24;
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
    uint32_t p = Curve(base, curve).points() + (i << 3);
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
    const uint32_t p = Curve(base, curve).points() + (i << 3);
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
  const uint32_t p = Curve(base, curve).points() + (count << 3);
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
    const uint32_t curve = uint32_t(Emitter(base, emitter).load_at(field, 4));
    const double distance = double(Dsp(base, dsp).distance());
    const double scaler = double(Emitter(base, emitter).curve_distance_scaler());
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
    Dsp(base, dsp).set_lpf_direct(float(evaluate(72, f8, f6)));
  }
  if (flags & 0x8) {
    Dsp(base, dsp).set_lpf_reverb(float(evaluate(76, f6, f6)));
  }
  if (flags & 0x10) {
    Dsp(base, dsp).set_reverb_level(float(evaluate(80, f7, f8)));
  }
  if (!(flags & 0x1)) {
    return r3;
  }
  f6 = evaluate(64, f7, f8);
  if (Emitter(base, emitter).channel_count() == 1) {
    Stfs(base, Dsp(base, dsp).matrix().address + 0, f6);
    return r3;
  }
  const double f5 = Lfs(base, kLfeAzimuth);
  uint32_t channel = 0;
  // The channels before the first LFE: the volume.
  if (Emitter(base, emitter).channel_count() != 0) {
    uint32_t offset = 0;
    for (;;) {
      const double azimuth = Lfs(base, Emitter(base, emitter).channel_azimuths() + offset);
      if (azimuth == f5) {
        break;
      }
      const uint32_t matrix = Dsp(base, dsp).matrix().address;
      channel += 1;
      Stfs(base, offset + matrix, f6);
      const uint32_t count = Emitter(base, emitter).channel_count();
      offset += 4;
      if (!(channel < count)) {
        break;
      }
    }
  }
  if (!(channel < Emitter(base, emitter).channel_count())) {
    return r3;
  }
  // The first LFE channel and every one after it: the LFE curve at the LFE
  // channels, the volume at the others.
  const double lfe = evaluate(68, f7, f8);
  Stfs(base, (channel << 2) + Dsp(base, dsp).matrix().address, lfe);
  uint32_t next = channel + 1;
  if (!(next < Emitter(base, emitter).channel_count())) {
    return r3;
  }
  uint32_t offset = next << 2;
  for (;;) {
    const double azimuth = Lfs(base, Emitter(base, emitter).channel_azimuths() + offset);
    const uint32_t matrix = Dsp(base, dsp).matrix().address;
    Stfs(base, offset + matrix, azimuth == f5 ? lfe : f6);
    const uint32_t count = Emitter(base, emitter).channel_count();
    next += 1;
    offset += 4;
    if (!(next < count)) {
      break;
    }
  }
  return r3;
}

namespace {

// One speaker's step of Pan's search: the azimuth's difference from it
// (double), wrapped by 2 pi, kept when it's the nearest so far on its side.
struct PanSearch {
  double f8, f9, f10, f11, f12, f13;
  int64_t r29 = -1, r30 = -1;

  void Step(uint8_t* base, double f1, uint32_t address, int64_t index) {
    double f0 = f1 - Lfd(base, address);
    if (f0 > f9) {
      f0 = f0 - f10;
    } else if (f0 < f8) {
      f0 = f0 + f10;
    }
    if (!(f0 > f12) && f0 > f13) {
      r29 = index;
      f13 = f0;
    } else if (!(f0 < f12) && f0 < f11) {
      r30 = index;
      f11 = f0;
    }
  }
};

}  // namespace

uint64_t Pan(uint8_t* base, uint64_t handle, uint32_t dsp, uint32_t channel, double azimuth,
             uint32_t sp, const PanSaves& saves, const TanCall& tan) {
  ITC_NATIVE_RUNNING("sub_82480F80 X3DAudio::Pan");
  // The prologue: __savegprlr_28, f31, the back chain.
  StoreU64(base, sp - 40, saves.r28);
  StoreU64(base, sp - 32, saves.r29);
  StoreU64(base, sp - 24, saves.r30);
  StoreU64(base, sp - 16, saves.r31);
  guest::StoreU32(base, sp - 8, saves.lr);
  StoreU64(base, sp - 48, saves.f31);
  const uint32_t frame = sp - 192;
  guest::StoreU32(base, frame, sp);
  const uint32_t r7 = guest::LoadU32(base, uint32_t(handle) + 0);
  // The two azimuth tables, copied into the frame (lfd, stfd: the bits).
  StoreU64(base, frame + 80, LoadU64(base, kStereoAzimuths[0]));
  const uint64_t f7_bits = LoadU64(base, kStereoAzimuths[1]);
  StoreU64(base, frame + 88, f7_bits);
  StoreU64(base, frame + 96, LoadU64(base, kSurroundAzimuths[0]));
  StoreU64(base, frame + 104, LoadU64(base, kSurroundAzimuths[1]));
  const uint64_t f12_bits = LoadU64(base, kSurroundAzimuths[2]);
  StoreU64(base, frame + 112, f12_bits);
  StoreU64(base, frame + 120, LoadU64(base, kSurroundAzimuths[3]));
  StoreU64(base, frame + 128, LoadU64(base, kSurroundAzimuths[4]));
  StoreU64(base, frame + 136, LoadU64(base, kSurroundAzimuths[5]));
  const double f7 = std::bit_cast<double>(f7_bits);
  // Only 2 and 6 speakers have a table; any other count reads at 0.
  uint32_t table = 0;
  if (r7 == 2) {
    table = frame + 80;
  } else if (r7 == 6) {
    table = frame + 96;
  }
  PanSearch s;
  s.f13 = Lfd(base, kPanLeftStart);
  s.f8 = Lfd(base, kPanWrapBelow);
  s.f10 = Lfd(base, kPanWrap);
  s.f9 = Lfd(base, kPanWrapAbove);
  s.f11 = Lfd(base, kPanRightStart);
  s.f12 = std::bit_cast<double>(f12_bits);
  const double f1 = azimuth;
  uint32_t i = 0;
  // Four speakers at a time while at least four are left (a signed check
  // on the count, then an unsigned one on count - 3).
  if (!(int32_t(r7) < 4)) {
    uint32_t p = table + 16;
    for (;;) {
      s.Step(base, f1, p - 16, int64_t(i));
      s.Step(base, f1, p - 8, int64_t(i) + 1);
      s.Step(base, f1, p + 0, int64_t(i) + 2);
      s.Step(base, f1, p + 8, int64_t(i) + 3);
      i += 4;
      p += 32;
      if (!(i < r7 - 3)) {
        break;
      }
    }
  }
  if (i < r7) {
    uint32_t p = (i << 3) + table;
    for (;;) {
      s.Step(base, f1, p, int64_t(i));
      i += 1;
      p += 8;
      if (!(i < r7)) {
        break;
      }
    }
  }
  double f13 = s.f13;
  double f11 = s.f11;
  if (f13 < s.f12) {
    f13 = std::bit_cast<double>(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000ull);
  }
  if (f11 < s.f12) {
    f11 = std::bit_cast<double>(std::bit_cast<uint64_t>(f11) ^ 0x8000000000000000ull);
  }
  uint32_t near = uint32_t(s.r29);
  uint32_t far = uint32_t(s.r30);
  if (s.r29 == -1 || s.r30 == -1) {
    near = 0;
    far = 0;
  }
  // The channel's old row-0 value, then its column zeroed.
  const double f31 =
      double(guest::LoadFloat(base, (channel << 2) + Dsp(base, dsp).matrix().address));
  if (Dsp(base, dsp).destination_channels() > 0) {
    const double zero = Lfs(base, kZero);
    uint32_t row = 0;
    for (;;) {
      const uint32_t at = MatrixWord(base, dsp, row, channel);
      row += 1;
      Stfs(base, at, zero);
      if (!(row < Dsp(base, dsp).destination_channels())) {
        break;
      }
    }
  }
  const double threshold = Lfd(base, kPanNear);
  if (f13 < threshold) {
    Stfs(base, MatrixWord(base, dsp, near, channel), f31);
    return handle;
  }
  if (f11 < threshold) {
    Stfs(base, MatrixWord(base, dsp, far, channel), f31);
    return handle;
  }
  // The tangent law: tan(f13 f7 / (f11 + f13)), as tan's adapter gives it
  // with nearest rounding set (it leaves r3 = 0xF8: the restored word ^
  // 0xF8), or tan itself.
  const double x = (f13 * f7) / (f11 + f13);
  using Fpscr = ::base::FloatMode;
  TanResult t;
  if ((Fpscr::getcsr() & Fpscr::RoundMaskVal) != 0 || !c_runtime::Tan(x, &t.f1)) {
    t = tan.guest(tan.user, frame, x);
  } else {
    t.r3 = 0xF8;
  }
  const double f0 = Single(t.f1);
  const uint32_t at_near = MatrixWord(base, dsp, near, channel);
  const double gain = Single(f31 / Single(f0 + Lfs(base, kOne)));
  Stfs(base, at_near, gain);
  Stfs(base, MatrixWord(base, dsp, far, channel), Single(f0 * gain));
  return t.r3;
}

uint64_t Matrix(uint8_t* base, uint64_t handle, uint32_t listener, uint64_t emitter64,
                uint64_t dsp64, uint64_t f31_bits, uint32_t frame, const TanCall& tan) {
  ITC_NATIVE_RUNNING("sub_82481390 X3DAudio::Matrix");
  const uint32_t e = uint32_t(emitter64);
  const uint32_t dsp = uint32_t(dsp64);
  const uint32_t h = uint32_t(handle);
  // The listener's front, then its frame's two other axes.
  guest::StoreU32(base, frame + 88, guest::LoadU32(base, Listener(base, listener).front_x_address()));
  guest::StoreU32(base, frame + 92, guest::LoadU32(base, Listener(base, listener).front_y_address()));
  guest::StoreU32(base, frame + 96, guest::LoadU32(base, Listener(base, listener).front_z_address()));
  VecCross(base, listener + 12, frame + 88, frame + 120);
  VecCross(base, frame + 88, frame + 120, frame + 152);
  uint64_t r3 = uint64_t(frame) + 88;
  const double f25 = Lfs(base, kZero);
  uint32_t count = Emitter(base, e).channel_count();
  double f30 = f25, f29 = f25, f28 = f25;
  if (count > 1) {
    // The emitter's front scaled by the channel radius.
    const double radius = double(Emitter(base, e).channel_radius());
    const double x = double(Emitter(base, e).front_x());
    const double y = double(Emitter(base, e).front_y());
    f30 = Single(x * radius);
    const double z = double(Emitter(base, e).front_z());
    f29 = Single(y * radius);
    f28 = Single(z * radius);
  }
  if (count == 0) {
    return r3;
  }
  const double f24 = Lfs(base, kAxisBelow);
  const double f26 = Lfs(base, kAxisAbove);
  const double f27 = Lfs(base, kOne);
  const double f23 = Lfs(base, kLfeAzimuth);
  double f31 = std::bit_cast<double>(f31_bits);
  uint32_t channel = 0;  // r30
  uint32_t offset = 0;   // r29: channel * 4
  for (;;) {
    bool rotated = false;
    if (count > 1) {
      const double azimuth = Lfs(base, Emitter(base, e).channel_azimuths() + offset);
      if (!(azimuth == f23)) {
        rotated = true;
        f31 = Single(c_runtime::Cos(azimuth));
        const double s =
            Single(c_runtime::Sin(Lfs(base, Emitter(base, e).channel_azimuths() + offset)));
        // The front offset rotated about the emitter's top (+16, +20, +24)
        // by the azimuth (a rotation matrix from its cos and sin), plus the
        // position (+28 to +36).
        double f13 = double(Emitter(base, e).top_z());
        double f10 = s;
        double f12 = double(Emitter(base, e).top_x());
        double f0 = Single(f27 - f31);
        double f11 = double(Emitter(base, e).top_y());
        double f9 = Single(f12 * f13);
        double f7 = Single(f11 * f13);
        const double f5 = double(Emitter(base, e).position_y());
        double f8 = Single(f11 * f12);
        const double f6 = double(Emitter(base, e).position_x());
        double f1 = Single(f13 * f13);
        const double f4 = double(Emitter(base, e).position_z());
        double f2 = Single(f11 * f11);
        double f3 = Single(f12 * f12);
        f9 = Single(f9 * f0);
        f7 = Single(f7 * f0);
        f8 = Single(f8 * f0);
        f1 = Single(std::fma(f1, f0, f31));
        f2 = Single(std::fma(f2, f0, f31));
        f3 = Single(std::fma(f3, f0, f31));
        f0 = Single(f13 * f10);
        f13 = Single(f12 * f10);
        f12 = Single(f11 * f10);
        f11 = Single(f8 - f0);
        f10 = Single(f7 - f13);
        f13 = Single(f13 + f7);
        f0 = Single(f0 + f8);
        f7 = Single(f12 + f9);
        f12 = Single(f9 - f12);
        f11 = Single(f11 * f29);
        f10 = Single(f10 * f28);
        f13 = Single(f13 * f29);
        f11 = Single(std::fma(f3, f30, f11));
        f10 = Single(std::fma(f2, f29, f10));
        f13 = Single(std::fma(f1, f28, f13));
        f11 = Single(std::fma(f7, f28, f11));
        f0 = Single(std::fma(f0, f30, f10));
        f13 = Single(std::fma(f12, f30, f13));
        Stfs(base, frame + 104, Single(f11 + f6));
        Stfs(base, frame + 108, Single(f0 + f5));
        Stfs(base, frame + 112, Single(f13 + f4));
      }
    }
    if (!rotated) {
      // The emitter's position (words).
      guest::StoreU32(base, frame + 104, guest::LoadU32(base, Emitter(base, e).position_x_address()));
      guest::StoreU32(base, frame + 108, guest::LoadU32(base, Emitter(base, e).position_y_address()));
      guest::StoreU32(base, frame + 112, guest::LoadU32(base, Emitter(base, e).position_z_address()));
    }
    if (Emitter(base, e).channel_count() > 1 &&
        Lfs(base, Emitter(base, e).channel_azimuths() + offset) == f23) {
      // An LFE channel: its column zeroed, its volume on the LFE speaker.
      const double volume = Lfs(base, offset + Dsp(base, dsp).matrix().address);
      if (Dsp(base, dsp).destination_channels() > 0) {
        uint32_t row = 0;
        for (;;) {
          const uint32_t at = MatrixWord(base, dsp, row, channel);
          row += 1;
          Stfs(base, at, f25);
          if (!(row < Dsp(base, dsp).destination_channels())) {
            break;
          }
        }
      }
      const uint32_t lfe = Handle(base, h).lfe_speaker();
      if (int32_t(lfe) != -1) {
        Stfs(base, MatrixWord(base, dsp, lfe, channel), volume);
      }
    } else {
      // The channel's position less the listener's, in the listener's frame.
      VecSub(base, frame + 104, listener + 24, frame + 136);
      r3 = uint64_t(frame) + 104;
      double f13 = Lfs(base, frame + 140);
      double f12 = Lfs(base, frame + 124);
      const double f10 = Single(f13 * f12);
      double f0 = Lfs(base, frame + 144);
      double f7 = Lfs(base, frame + 92);
      double f11 = Lfs(base, frame + 128);
      f13 = Single(f7 * f13);
      f12 = Lfs(base, frame + 136);
      const double f9 = Lfs(base, frame + 120);
      const double f8 = Lfs(base, frame + 96);
      f7 = Lfs(base, frame + 88);
      f11 = Single(std::fma(f0, f11, f10));
      f0 = Single(std::fma(f8, f0, f13));
      const double y = Single(std::fma(f12, f9, f11));
      const double x = Single(std::fma(f7, f12, f0));
      if (y > f26 || x > f26 || y < f24 || x < f24) {
        const double azimuth = Single(c_runtime::Atan2(y, x));
        const PanSaves saves{0x824816E0, emitter64, uint64_t(offset), uint64_t(channel), dsp64,
                             std::bit_cast<uint64_t>(f31)};
        r3 = Pan(base, handle, dsp, channel, azimuth, frame, saves, tan);
      } else {
        // On the listener's axis: the volume split evenly over the
        // speakers but the LFE (the count stored as a doubleword).
        const uint32_t lfe_index = Handle(base, h).lfe_speaker();
        const uint32_t speakers = Dsp(base, dsp).destination_channels();
        uint64_t spread = speakers;
        if (int32_t(lfe_index) != -1) {
          spread = uint64_t(int64_t(speakers) - 1);
        }
        spread &= 0xFFFFFFFFull;
        const uint32_t matrix = Dsp(base, dsp).matrix().address;
        StoreU64(base, frame + 80, spread);
        double volume = Lfs(base, offset + matrix);
        const double n = Single(double(int64_t(LoadU64(base, frame + 80))));
        volume = Single(volume / n);
        if (speakers != 0) {
          uint32_t row = 0;
          for (;;) {
            const bool lfe = row == Handle(base, h).lfe_speaker();
            const uint32_t at = MatrixWord(base, dsp, row, channel);
            Stfs(base, at, lfe ? f25 : volume);
            row += 1;
            if (!(row < Dsp(base, dsp).destination_channels())) {
              break;
            }
          }
        }
      }
    }
    count = Emitter(base, e).channel_count();
    channel += 1;
    offset += 4;
    if (!(channel < count)) {
      break;
    }
  }
  return r3;
}

namespace {

// Cone's constant (lfs): the half the angles are scaled by.
constexpr uint32_t kHalf = 0x82001BE4;  // 0.5, probably

}  // namespace

uint64_t Cone(uint8_t* base, uint64_t handle, uint32_t listener, uint32_t emitter,
              uint32_t flags, uint32_t dsp, uint32_t sp, const ConeSaves& saves) {
  ITC_NATIVE_RUNNING("sub_82480BA8 X3DAudio::Cone");
  // The prologue: __savegprlr_29, f29 to f31, the back chain.
  StoreU64(base, sp - 32, saves.r29);
  StoreU64(base, sp - 24, saves.r30);
  StoreU64(base, sp - 16, saves.r31);
  guest::StoreU32(base, sp - 8, saves.lr);
  StoreU64(base, sp - 56, saves.f29);
  StoreU64(base, sp - 48, saves.f30);
  StoreU64(base, sp - 40, saves.f31);
  const uint32_t frame = sp - 160;
  guest::StoreU32(base, frame, sp);
  uint64_t r3 = handle;
  const bool volume = (flags & 0x1) != 0;
  const bool lowpass = (flags & 0xC) != 0;
  const bool reverb = (flags & 0x10) != 0;
  // The values the stores use (f11 the volume, f12 the low-pass, f0 the
  // reverb); the original loads its frame's +80 for those the flags don't
  // ask for, which nothing reads.
  double f11 = 0.0, f12 = 0.0, f0 = 0.0;
  // One cone's values at `at` (+8 inner, +12 outer), from the cone `cone`,
  // or read again from the emitter for each (as the original does after its
  // calls).
  auto pick = [&](uint32_t at, uint32_t cone, bool again) {
    if (volume) {
      f11 = Lfs(base, (again ? Emitter(base, emitter).cone().address : cone) + at);
    }
    if (lowpass) {
      f12 = Lfs(base, (again ? Emitter(base, emitter).cone().address : cone) + at + 8);
    }
    if (reverb) {
      f0 = Lfs(base, (again ? Emitter(base, emitter).cone().address : cone) + at + 16);
    }
  };
  const uint32_t cone = Emitter(base, emitter).cone().address;
  if (double(ConeView(base, cone).outer_angle()) == Lfs(base, kZero)) {
    pick(12, cone, false);
  } else if (double(ConeView(base, cone).inner_angle()) == Lfs(base, kLfeAzimuth) ||
             !(double(Dsp(base, dsp).distance()) > Lfs(base, kAxisAbove))) {
    pick(8, cone, false);
  } else {
    // The cosine of the listener's angle off the emitter's front.
    VecSub(base, listener + 24, emitter + 28, frame + 88);
    const double dot = double(VecDot(base, frame + 88, emitter + 4));
    r3 = uint64_t(frame) + 88;
    const uint32_t c = Emitter(base, emitter).cone().address;
    const double distance = double(Dsp(base, dsp).distance());
    const double inner = double(ConeView(base, c).inner_angle());
    const double f30 = Lfs(base, kHalf);
    const double f31 = Single(dot / distance);
    const double f29 = Single(c_runtime::Cos(Single(inner * f30)));
    if (!(f31 < f29)) {
      pick(8, 0, true);
    } else {
      const double outer = Lfs(base, Emitter(base, emitter).cone().address + 4);
      const double cos_outer = Single(c_runtime::Cos(Single(outer * f30)));
      if (!(f31 > cos_outer)) {
        pick(12, 0, true);
      } else {
        // Between the cones: linear from the inner value to the outer.
        const double f13 = Single(f31 - f29);
        const double t = Single(f13 / Single(cos_outer - f29));
        auto lerp = [&](uint32_t c2, uint32_t at) {
          const double a = Lfs(base, c2 + at);
          const double b = Lfs(base, c2 + at + 4);
          return Single(std::fma(Single(b - a), t, a));
        };
        if (volume) {
          f11 = lerp(Emitter(base, emitter).cone().address, 8);
        }
        if (lowpass) {
          f12 = lerp(Emitter(base, emitter).cone().address, 16);
        }
        if (reverb) {
          f0 = lerp(Emitter(base, emitter).cone().address, 24);
        }
      }
    }
  }
  if (volume) {
    const uint32_t matrix = Dsp(base, dsp).matrix().address;
    Stfs(base, matrix + 0, Single(Lfs(base, matrix + 0) * f11));
  }
  if (flags & 0x4) {
    Dsp(base, dsp).set_lpf_direct(float(Single(f12 * double(Dsp(base, dsp).lpf_direct()))));
  }
  if (flags & 0x8) {
    Dsp(base, dsp).set_lpf_reverb(float(Single(double(Dsp(base, dsp).lpf_reverb()) * f12)));
  }
  if (reverb) {
    Dsp(base, dsp).set_reverb_level(float(Single(double(Dsp(base, dsp).reverb_level()) * f0)));
  }
  return r3;
}

uint64_t Doppler(uint8_t* base, uint64_t handle, uint32_t listener, uint32_t emitter,
                 uint32_t dsp, uint32_t sp, const DopplerSaves& saves) {
  ITC_NATIVE_RUNNING("sub_82480E90 X3DAudio::Doppler");
  // The prologue: __savegprlr_28, f31, the back chain.
  StoreU64(base, sp - 40, saves.r28);
  StoreU64(base, sp - 32, saves.r29);
  StoreU64(base, sp - 24, saves.r30);
  StoreU64(base, sp - 16, saves.r31);
  guest::StoreU32(base, sp - 8, saves.lr);
  StoreU64(base, sp - 48, saves.f31);
  const uint32_t frame = sp - 144;
  guest::StoreU32(base, frame, sp);
  const uint32_t h = uint32_t(handle);
  uint64_t r3 = handle;
  double f0 = Lfs(base, kOne);
  double f31 = Lfs(base, kZero);
  double f10 = f31;
  if (double(Dsp(base, dsp).distance()) > Lfs(base, kAxisAbove)) {
    VecSub(base, listener + 24, emitter + 28, frame + 80);
    double f1 = double(VecDot(base, frame + 80, listener + 36));
    f31 = Single(f1 / double(Dsp(base, dsp).distance()));
    f1 = double(VecDot(base, frame + 80, emitter + 40));
    r3 = uint64_t(frame) + 80;
    f10 = Single(f1 / double(Dsp(base, dsp).distance()));
    double f13 = double(Emitter(base, emitter).doppler_scaler());
    double f12 = Single(f13 * f31);
    f0 = double(Handle(base, h).doppler_limit());
    f13 = Single(f13 * f10);
    // Both clamped to [-c, c] (the negation by its sign bit).
    const double negated = std::bit_cast<double>(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000ull);
    if (f12 > f0) {
      f12 = f0;
    } else if (f12 < negated) {
      f12 = negated;
    }
    if (f13 > f0) {
      f13 = f0;
    } else if (f13 < negated) {
      f13 = negated;
    }
    f0 = double(Handle(base, h).speed_of_sound());
    f12 = Single(f0 - f12);
    f0 = Single(f0 - f13);
    f0 = Single(f12 / f0);
  }
  Dsp(base, dsp).set_doppler_factor(float(f0));
  Dsp(base, dsp).set_listener_velocity(float(f31));
  Dsp(base, dsp).set_emitter_velocity(float(f10));
  return r3;
}

std::optional<uint64_t> Calculate(uint8_t* base, const CalculateArgs& a, uint32_t sp,
                                  const CalculateSaves& s, const TanCall& tan) {
  const uint32_t flags = uint32_t(a.flags);
  if (flags & 0x42) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82481880 X3DAudioCalculate");
  // The prologue: __savegprlr_27, the back chain.
  StoreU64(base, sp - 48, s.r27);
  StoreU64(base, sp - 40, s.r28);
  StoreU64(base, sp - 32, s.r29);
  StoreU64(base, sp - 24, s.r30);
  StoreU64(base, sp - 16, s.r31);
  guest::StoreU32(base, sp - 8, s.lr);
  const uint32_t frame = sp - 144;
  guest::StoreU32(base, frame, sp);
  // Its registers from here: r27 the flags, r28 the listener, r29 the
  // handle, r30 the DSP settings, r31 the emitter (what its callees save).
  const uint32_t listener = uint32_t(a.listener);
  const uint32_t emitter = uint32_t(a.emitter);
  const uint32_t dsp = uint32_t(a.dsp);
  VecSub(base, listener + 24, emitter + 28, frame + 80);
  uint64_t r3 = uint64_t(frame) + 80;
  Dsp(base, dsp).set_distance(float(double(VecLength(base, frame + 80))));
  if (a.flags & 29) {
    // Curves' prologue: lr, r31, the back chain of its 128-byte frame.
    guest::StoreU32(base, frame - 8, 0x824818FC);
    StoreU64(base, frame - 16, a.emitter);
    guest::StoreU32(base, frame - 128, frame);
    const uint32_t curve = Curves(base, emitter, flags, dsp, frame - 128);
    r3 = curve != 0 ? uint64_t(curve) : a.handle;
    if (Emitter(base, emitter).channel_count() == 1 && Emitter(base, emitter).cone().address != 0) {
      r3 = Cone(base, a.handle, listener, emitter, flags, dsp, frame,
                {0x8248192C, a.handle, a.dsp, a.emitter, s.f29, s.f30, s.f31});
    }
    if (flags & 0x1) {
      // Matrix's prologue: __savegprlr_26, __savefpr_23, the back chain of
      // its 304-byte frame.
      const uint64_t gprs[6] = {s.r26, a.flags, a.listener, a.handle, a.dsp, a.emitter};
      for (uint32_t i = 0; i < 6; i++) {
        StoreU64(base, frame - 56 + 8 * i, gprs[i]);
      }
      guest::StoreU32(base, frame - 8, 0x8248194C);
      const uint64_t fprs[9] = {s.f23, s.f24, s.f25, s.f26, s.f27, s.f28, s.f29, s.f30, s.f31};
      for (uint32_t i = 0; i < 9; i++) {
        StoreU64(base, frame - 128 + 8 * i, fprs[i]);
      }
      guest::StoreU32(base, frame - 304, frame);
      r3 = Matrix(base, a.handle, listener, a.emitter, a.dsp, s.f31, frame - 304, tan);
    }
  }
  if (flags & 0x20) {
    r3 = Doppler(base, a.handle, listener, emitter, dsp, frame,
                 {0x8248198C, a.listener, a.handle, a.dsp, a.emitter, s.f31});
  }
  return r3;
}

}  // namespace x3daudio
