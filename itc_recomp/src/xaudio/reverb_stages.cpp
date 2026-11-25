// Stages of XAudio's reverb core as plain C++ (reverb_stages.h;
// docs/functions/xaudio-objects.md).

#include "xaudio/reverb_stages.h"

#include "base/log.h"
#include "guest.h"
#include "schema/xaudio_objects.h"
#include "xaudio/ppc_float.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <climits>
#include <cmath>

namespace xaudio {

using schema::ReverbAllPass;
using schema::ReverbCrossfade;
using schema::ReverbDelayLine;

namespace {

using namespace ppc_float;

double Load(uint32_t address) { return double(guest::LoadFloat(address)); }
void Store(uint32_t address, double value) {
  guest::StoreFloat(address, float(value));
}
void CopyFloat(uint32_t destination, uint32_t source) {
  guest::StoreU32(destination, guest::LoadU32(source));
}

// The delay lines with three taps (sub_82462CF8, sub_82463060): `index_mask`
// the byte index's mask, `wrap` the entry index's.
void DelayThreeTaps(uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b,
                    uint32_t index_mask, uint32_t wrap) {
  ScalarMode mode;
  const ReverbDelayLine d(line);
  const uint32_t r11_start = d.tap_2();
  const uint32_t data = d.line_address();
  const uint32_t first = d.tap_1();
  const uint32_t write = d.write_index();
  const uint32_t third = d.tap_3();
  const uint32_t r31 = r11_start - first;
  const double f0 = double(d.gain_2());
  const uint32_t r30 = r11_start - third;
  const double f13 = double(d.gain_1());
  uint32_t r11 = write - r11_start;
  const double f12 = double(d.gain_3());
  CopyFloat(out_a, d.last_a_address());
  CopyFloat(out_b, d.last_b_address());
  uint32_t r8 = (write << 2) + data;
  double f10 = 0, f11 = 0;
  for (int k = 0; k < 256; k++) {
    const uint32_t r29 = (r11 << 2) & index_mask;
    const uint32_t r28 = ((r31 + r11) << 2) & index_mask;
    const uint32_t r27 = ((r30 + r11) << 2) & index_mask;
    f10 = Load(r29 + data);
    out_a += 4;
    f10 = Round(f10 * f0);
    out_b += 4;
    const double f9 = Load(r28 + data);
    const double f8 = Load(r27 + data);
    r11 += 1;
    CopyFloat(r8, in);
    in += 4;
    f11 = Round(f8 * f12);
    Store(out_b, f11);
    r8 += 4;
    f10 = Fma(f9, f13, f10);
    Store(out_a, f10);
  }
  d.set_last_a(float(f10));
  d.set_last_b(float(f11));
  d.set_write_index((write + 256) & wrap);
}


// The all-pass stages (sub_82462DC8, sub_82463130, sub_824632A0,
// sub_82463410): the state +0 the tap, +4 and +8 the gains, +12 the last
// output, +16 the write index, the line's data at +24. A block of eight
// frames a pass, in the original's order of loads and stores: each frame's
// input x and the line's entry d at the tap (masked to `mask`), v = +8 x d
// + x written to the line (unmasked, from `write`), and +4 x v + d the
// output, a frame late (the first the last block's).
struct AllPass {
  uint32_t data;
  uint32_t mask;
  double f0, f13;
  double Tap(uint32_t r11, int k) const {
    return Load(data + (((r11 + uint32_t(k)) << 2) & mask));
  }
};

// The in-place pass (sub_82462DC8's first half, sub_82463130, sub_824632A0):
// the frames at `p` replaced; r11 the tap's index (+2), `q` the write
// address (+8). Returns the carried output.
double AllPassInPlace(const AllPass& a, uint32_t& p, uint32_t& q, uint32_t& r11, double f11,
                      int passes) {
  const double f0 = a.f0, f13 = a.f13;
  for (int pass = 0; pass < passes; pass++) {
    double f10 = Load(p + 0);
    Store(p + 0, f11);
    double f9 = Load(p + 4);
    const double f8 = Load(p + 8);
    const double f7 = Load(p + 12);
    const double f6 = Load(p + 16);
    const double f5 = Load(p + 20);
    double f12 = a.Tap(r11, -2);
    f11 = Fma(f0, f12, f10);
    Store(q - 8, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, -1);
    Store(p + 4, f11);
    f11 = Fma(f0, f12, f9);
    Store(q - 4, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 0);
    Store(p + 8, f11);
    f11 = Fma(f0, f12, f8);
    Store(q + 0, f11);
    f9 = Load(p + 28);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 1);
    Store(p + 12, f11);
    f11 = Fma(f0, f12, f7);
    Store(q + 4, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 2);
    Store(p + 16, f11);
    f11 = Fma(f0, f12, f6);
    Store(q + 8, f11);
    f10 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 3);
    f11 = Fma(f0, f12, f5);
    Store(q + 12, f11);
    Store(p + 20, f10);
    f10 = Load(p + 24);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 4);
    Store(p + 24, f11);
    f11 = Fma(f0, f12, f10);
    Store(q + 16, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 5);
    Store(p + 28, f11);
    f11 = Fma(f0, f12, f9);
    Store(q + 20, f11);
    q += 32;
    p += 32;
    r11 += 8;
    f11 = Fma(f13, f11, f12);
  }
  return f11;
}

// sub_82463130 (256 entries) and sub_824632A0 (512): one in-place pass of
// 256 frames, the write index kept by `wrap`.
void AllPassBlock(uint32_t state, uint32_t p, uint32_t mask, uint32_t wrap) {
  ScalarMode mode;
  const ReverbAllPass v(state);
  const uint32_t write = v.write_index();
  const uint32_t tap = v.tap();
  const AllPass a{v.line_address(), mask, double(v.gain_feedback()), double(v.gain_output())};
  const double f11 = double(v.last_output());
  uint32_t r11 = write - tap + 2;
  uint32_t q = ((write + 2) << 2) + a.data;
  const double last = AllPassInPlace(a, p, q, r11, f11, 32);
  v.set_last_output(float(last));
  Store(p + 0, last);
  v.set_write_index((write + 256) & wrap);
}
}  // namespace

void ReverbLowPass(uint32_t state, uint32_t in, uint32_t out, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82462BC8 the reverb's low-pass");
  ScalarMode mode;
  const schema::ReverbLowPass v(state);
  double f9 = double(v.last_output());
  double f12 = double(v.state());
  uint32_t r9 = 1;
  CopyFloat(out, v.last_output_address());
  uint32_t r11 = out + 4;
  const double f7 = Load(0x82000A30);
  bool ran = false;
  if (int32_t(count) >= 4) {
    ran = true;
    const double f0 = double(v.coefficient());
    uint32_t r10 = count >> 2;
    const double f10 = Round(f7 - f0);
    const double f11 = double(v.gain());
    r9 = (r10 << 2) + 1;
    const double f13 = Round(f11 * f10);
    do {
      double f6 = Load(in);
      double f8 = f12;
      f6 = Round(f6 * f13);
      double f5 = Load(in + 4);
      f9 = Round(f0 * f9);
      double f4 = Load(in + 8);
      f5 = Round(f5 * f11);
      const double f3 = Load(in + 12);
      f4 = Round(f4 * f11);
      r10 -= 1;
      in += 16;
      f12 = Fma(f0, f12, f6);
      f9 = Fma(f13, f8, f9);
      Store(r11 + 0, f9);
      f6 = Round(f3 * f11);
      f8 = f12;
      f12 = Round(f0 * f12);
      f12 = Fma(f5, f10, f12);
      f5 = Round(f0 * f9);
      f9 = Fma(f13, f8, f5);
      Store(r11 + 4, f9);
      f8 = f12;
      f12 = Round(f0 * f12);
      f5 = Round(f0 * f9);
      f12 = Fma(f4, f10, f12);
      f9 = Fma(f13, f8, f5);
      Store(r11 + 8, f9);
      f8 = f12;
      f12 = Round(f0 * f12);
      f12 = Fma(f6, f10, f12);
      f6 = Round(f0 * f9);
      f9 = Fma(f13, f8, f6);
      Store(r11 + 12, f9);
      r11 += 16;
    } while (r10 != 0);
  }
  if (!(r9 > count)) {
    ran = true;
    const double f0 = double(v.coefficient());
    uint32_t r10 = count - r9;
    const double f11 = Round(f7 - f0);
    double f13 = double(v.gain());
    r10 += 1;
    f13 = Round(f11 * f13);
    do {
      const double previous = f12;
      r10 -= 1;
      f12 = Round(f0 * f12);
      const double f10 = Round(f0 * f9);
      const double sample = Load(in);
      in += 4;
      f12 = Fma(f13, sample, f12);
      f9 = Fma(f13, previous, f10);
      Store(r11, f9);
      r11 += 4;
    } while (r10 != 0);
  }
  if (ran) {
    v.set_state(float(f12));
    v.set_last_output(float(f9));
  }
  // With no frames the state is stored back as it was loaded (its bits).
}

void ReverbDelay512(uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b) {
  ITC_NATIVE_RUNNING("sub_82462CF8 the reverb's 512-entry delay line");
  DelayThreeTaps(line, in, out_a, out_b, 0x7FC, 0x1FF);
}

void ReverbDelay2048(uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b) {
  ITC_NATIVE_RUNNING("sub_82463060 the reverb's 2,048-entry delay line");
  DelayThreeTaps(line, in, out_a, out_b, 0x1FFC, 0x7FF);
}

void ReverbDelay1024(uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b) {
  ITC_NATIVE_RUNNING("sub_82463578 the reverb's 1,024-entry delay line");
  ScalarMode mode;
  const schema::ReverbDelay1024 d(line);
  const uint32_t write = d.write_index();
  const uint32_t data = d.line_address();
  const uint32_t second = d.tap_b();
  const uint32_t first = d.tap_a();
  uint32_t r8 = (write << 2) + data;
  uint32_t r9 = write - second;
  uint32_t r10 = write - first;
  const double f12 = double(d.gain_a());
  const double f11 = double(d.gain_b());
  CopyFloat(out_a, d.last_a_address());
  CopyFloat(out_b, d.last_b_address());
  double f0 = 0, f13 = 0;
  for (int k = 0; k < 256; k++) {
    const uint32_t r30 = (r10 << 2) & 0xFFC;
    const uint32_t r29 = (r9 << 2) & 0xFFC;
    out_a += 4;
    out_b += 4;
    f0 = Load(r30 + data);
    f13 = Load(r29 + data);
    f0 = Round(f0 * f12);
    CopyFloat(r8, in);
    in += 4;
    f13 = Round(f13 * f11);
    Store(out_a, f0);
    r10 += 1;
    Store(out_b, f13);
    r9 += 1;
    r8 += 4;
  }
  d.set_last_a(float(f0));
  d.set_last_b(float(f13));
  d.set_write_index((write + 256) & 0x3FF);
}

}  // namespace xaudio

namespace xaudio {

void ReverbAllPass128(uint32_t state, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_82462DC8 the reverb's 128-entry all-pass");
  ScalarMode mode;
  const ReverbAllPass v(state);
  const uint32_t write = v.write_index();
  const uint32_t tap = v.tap();
  const AllPass a{v.line_address(), 0x1FC, double(v.gain_feedback()), double(v.gain_output())};
  double f11 = double(v.last_output());
  uint32_t r10 = write - tap + 2;
  uint32_t r9 = ((write + 2) << 2) + a.data;
  // The first 128 frames written from the write index, as the passes above.
  double f12 = AllPassInPlace(a, p, r9, r10, f11, 16);
  // The second 128 written from the line's start (the index 2 - the tap),
  // its loads in the original's second order.
  const double f0 = a.f0, f13 = a.f13;
  uint32_t r9b = 2 - tap;
  uint32_t r10b = a.data + 8;
  for (int pass = 0; pass < 16; pass++) {
    double f10 = Load(p + 0);
    Store(p + 0, f12);
    double f9 = Load(p + 4);
    const double f8 = Load(p + 8);
    const double f7 = Load(p + 12);
    f11 = a.Tap(r9b, -2);
    f12 = Fma(f0, f11, f10);
    Store(r10b - 8, f12);
    const double f6 = Load(p + 16);
    const double f5 = Load(p + 20);
    f11 = Fma(f13, f12, f11);
    f12 = a.Tap(r9b, -1);
    Store(p + 4, f11);
    f11 = Fma(f0, f12, f9);
    Store(r10b - 4, f11);
    f9 = Load(p + 28);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 0);
    Store(p + 8, f11);
    f11 = Fma(f0, f12, f8);
    Store(r10b + 0, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 1);
    Store(p + 12, f11);
    f11 = Fma(f0, f12, f7);
    Store(r10b + 4, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 2);
    Store(p + 16, f11);
    f11 = Fma(f0, f12, f6);
    Store(r10b + 8, f11);
    f10 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 3);
    f11 = Fma(f0, f12, f5);
    Store(r10b + 12, f11);
    Store(p + 20, f10);
    f10 = Load(p + 24);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 4);
    Store(p + 24, f11);
    f11 = Fma(f0, f12, f10);
    Store(r10b + 16, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 5);
    Store(p + 28, f11);
    f11 = Fma(f0, f12, f9);
    Store(r10b + 20, f11);
    p += 32;
    r9b += 8;
    r10b += 32;
    f12 = Fma(f13, f11, f12);
  }
  v.set_last_output(float(f12));
  Store(p + 0, f12);
  v.set_write_index(0);
}

void ReverbAllPass256(uint32_t state, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_82463130 the reverb's 256-entry all-pass");
  AllPassBlock(state, p, 0x3FC, 0xFF);
}

void ReverbAllPass512(uint32_t state, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_824632A0 the reverb's 512-entry all-pass");
  AllPassBlock(state, p, 0x7FC, 0x1FF);
}

void ReverbAllPass512To(uint32_t state, uint32_t in, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82463410 the reverb's 512-entry all-pass to a buffer");
  ScalarMode mode;
  const ReverbAllPass v(state);
  const uint32_t write = v.write_index();
  const uint32_t tap = v.tap();
  const AllPass a{v.line_address(), 0x7FC, double(v.gain_feedback()), double(v.gain_output())};
  const double f0 = a.f0, f13 = a.f13;
  double f11 = double(v.last_output());
  uint32_t r11 = write - tap + 2;
  uint32_t q = ((write + 2) << 2) + a.data;
  for (int pass = 0; pass < 32; pass++) {
    double f10 = Load(in + 0);
    Store(out + 0, f11);
    double f9 = Load(in + 4);
    const double f8 = Load(in + 8);
    const double f7 = Load(in + 12);
    const double f6 = Load(in + 16);
    const double f5 = Load(in + 20);
    double f12 = a.Tap(r11, -2);
    f11 = Fma(f0, f12, f10);
    Store(q - 8, f11);
    f10 = Load(in + 24);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, -1);
    Store(out + 4, f11);
    f11 = Fma(f0, f12, f9);
    Store(q - 4, f11);
    f9 = Load(in + 28);
    in += 32;
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 0);
    Store(out + 8, f11);
    f11 = Fma(f0, f12, f8);
    Store(q + 0, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 1);
    Store(out + 12, f11);
    f11 = Fma(f0, f12, f7);
    Store(q + 4, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 2);
    Store(out + 16, f11);
    f11 = Fma(f0, f12, f6);
    Store(q + 8, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 3);
    Store(out + 20, f11);
    f11 = Fma(f0, f12, f5);
    Store(q + 12, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 4);
    Store(out + 24, f11);
    f11 = Fma(f0, f12, f10);
    Store(q + 16, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 5);
    Store(out + 28, f11);
    f11 = Fma(f0, f12, f9);
    Store(q + 20, f11);
    out += 32;
    q += 32;
    r11 += 8;
    f11 = Fma(f13, f11, f12);
  }
  v.set_last_output(float(f11));
  Store(in + 0, f11);
  v.set_write_index((write + 256) & 0x1FF);
}

}  // namespace xaudio

// The crossfading delay lines (sub_824624F8, sub_82463620): the state +4
// the crossfade's position (1.0 down to 0.0), +8 the tap faded to, +12 the
// tap faded from, +16 the tap asked for, +20 the last output, +24 the write
// index, the line at +32. Translated in the original's order of loads and
// stores (the callers pass outputs that overlap the inputs).
namespace xaudio {

namespace {

constexpr uint32_t kFadeScale = 0x82002814;  // the frames a crossfade takes, x +4
constexpr uint32_t kOne = 0x82000A30;
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kFadeStep = 0x82002CC4;
constexpr uint32_t kFadeEnd = 0x8204AA84;

int64_t ToInt64(double x) {
  return std::isnan(x)           ? int64_t(0x8000000000000000ull)
         : x > double(LLONG_MAX) ? LLONG_MAX
                                 : simde_mm_cvttsd_si64(simde_mm_load_sd(&x));
}

uint32_t Word(uint32_t address) { return guest::LoadU32(address); }
void SetWord(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The two lines' shared steps, `kMask` the line's byte index mask, `kWrap`
// the write index's.
template <uint32_t kMask, uint32_t kWrap>
struct Crossfade {
  uint32_t s, in, out;
  uint32_t r11;  // the line
  double f0, f11, f12, f13;
  // r3 as the original leaves it (sub_824624F8 uses it as a scratch
  // register in its frame-at-a-time and steady passes; sub_82463620 doesn't).
  bool sets_r3 = false;
  uint64_t r3 = 0;

  double Tap(uint32_t index) const { return Load(((index << 2) & kMask) + r11); }

  // Four frames a pass, crossfading from the tap at r30 to the one at r30 +
  // the taps' difference (r24): the fade's first pass (r30, r31, r6 the
  // write index less the taps, r8 the write address, r10 the output, r7 the
  // input + 8).
  void FourFrames(uint32_t r29, uint32_t r30, uint32_t r31, uint32_t r6, uint32_t r8,
                  uint32_t r10, uint32_t r7, uint32_t r24) {
    const uint32_t r25 = in - out;
    do {
      uint32_t r27 = r30;
      double f10 = f12;
      uint32_t r19 = r6 - 1;
      f12 = Round(f12 - f0);
      const uint32_t r15 = r27;
      const uint32_t w9 = Word(r7 - 8);
      uint32_t r26 = r30 + r24;
      const uint32_t w8 = Word(r7 - 4);
      r27 = r19;
      const uint32_t w7 = Word(r7 + 0);
      const uint32_t r19b = r26;
      const uint32_t w6 = Word(r25 + r10);
      const uint32_t r18 = r31 - 1;
      double f4 = Tap(r15);
      r26 = r31;
      f4 = Round(f4 * f13);
      f13 = Round(f13 + f0);
      const uint32_t r17 = r6 + 1;
      const double f3 = Tap(r19b);
      const uint32_t r19c = r27;
      SetWord(r8 - 8, w9);
      double f5 = f12;
      f12 = Round(f12 - f0);
      r27 = r6;
      const uint32_t r16 = r31 + 1;
      r29 -= 1;
      double f9 = Tap(r19c);
      const uint32_t r19d = r27;
      const uint32_t r27b = r17;
      r30 += 4;
      f10 = Fma(f3, f10, f4);
      f4 = Tap(r18);
      f9 = Round(f9 * f13);
      SetWord(r8 - 4, w8);
      f13 = Round(f13 + f0);
      double f8 = Tap(r19d);
      Store(r10 - 8, f10);
      const uint32_t r18b = r26;
      f10 = f12;
      const uint32_t r26b = r16;
      f12 = Round(f12 - f0);
      r7 += 16;
      r6 += 4;
      r31 += 4;
      f9 = Fma(f4, f5, f9);
      f5 = Tap(r18b);
      f8 = Round(f8 * f13);
      SetWord(r8 + 0, w7);
      f13 = Round(f13 + f0);
      Store(r10 - 4, f9);
      f9 = f12;
      f12 = Round(f12 - f0);
      f10 = Fma(f5, f10, f8);
      Store(r10 + 0, f10);
      f10 = Tap(r27b);
      f10 = Round(f10 * f13);
      f8 = Tap(r26b);
      SetWord(r8 + 4, w6);
      f13 = Round(f13 + f0);
      r8 += 16;
      f10 = Fma(f8, f9, f10);
      Store(r10 + 4, f10);
      r10 += 16;
    } while (r29 != 0);
    ReverbCrossfade(s).set_position(float(f12));
  }

  // The fade's second four-frame pass (after the taps moved on): the same
  // steps, the input read from r7 - 12 and r10 + r25, the outputs at r10 - 4.
  void FourFramesAfter(uint32_t r29, uint32_t r30, uint32_t r31, uint32_t r6, uint32_t r8,
                       uint32_t r10, uint32_t r7, uint32_t r24) {
    const uint32_t r25 = in - out;
    do {
      uint32_t r28 = r30;
      f11 = f12;
      uint32_t r23 = r6 - 1;
      f12 = Round(f12 - f0);
      const uint32_t r16 = r28;
      const uint32_t w10 = Word(r7 - 12);
      uint32_t r27 = r30 + r24;
      const uint32_t w9 = Word(r7 - 8);
      r28 = r23;
      const uint32_t w8 = Word(r10 + r25);
      const uint32_t r23b = r27;
      const uint32_t w7 = Word(r7 + 0);
      const uint32_t r19 = r31 - 1;
      double f5 = Tap(r16);
      r27 = r31;
      f5 = Round(f5 * f13);
      f13 = Round(f13 + f0);
      const uint32_t r18 = r6 + 1;
      const double f4 = Tap(r23b);
      const uint32_t r23c = r28;
      SetWord(r8 - 8, w10);
      double f6 = f12;
      f12 = Round(f12 - f0);
      r28 = r6;
      const uint32_t r17 = r31 + 1;
      r29 -= 1;
      double f10 = Tap(r23c);
      const uint32_t r23d = r28;
      const uint32_t r28b = r18;
      r30 += 4;
      f11 = Fma(f4, f11, f5);
      f5 = Tap(r19);
      f10 = Round(f10 * f13);
      SetWord(r8 - 4, w9);
      f13 = Round(f13 + f0);
      double f9 = Tap(r23d);
      Store(r10 - 4, f11);
      const uint32_t r19b = r27;
      f11 = f12;
      const uint32_t r27b = r17;
      f12 = Round(f12 - f0);
      r7 += 16;
      r6 += 4;
      r31 += 4;
      f10 = Fma(f5, f6, f10);
      f6 = Tap(r19b);
      f9 = Round(f9 * f13);
      SetWord(r8 + 0, w8);
      f13 = Round(f13 + f0);
      Store(r10 + 0, f10);
      f10 = f12;
      f12 = Round(f12 - f0);
      f11 = Fma(f6, f11, f9);
      Store(r10 + 4, f11);
      f11 = Tap(r28b);
      f11 = Round(f11 * f13);
      f9 = Tap(r27b);
      SetWord(r8 + 4, w7);
      f13 = Round(f13 + f0);
      r8 += 16;
      f11 = Fma(f9, f10, f11);
      Store(r10 + 8, f11);
      r10 += 16;
    } while (r29 != 0);
    ReverbCrossfade(s).set_position(float(f12));
  }

  // A frame at a time from frame `first` to 256 (the fade's tail after the
  // taps moved on): returns the write index moved on.
  uint32_t OneAtATime(uint32_t first, uint32_t r9) {
    uint32_t r7 = first << 2;
    const uint32_t r31 = ReverbCrossfade(s).tap_to();
    uint32_t r10 = r9 << 2;
    uint32_t r30 = ReverbCrossfade(s).tap_from();
    const uint32_t start = r7 + out;
    f12 = double(ReverbCrossfade(s).position());
    uint32_t r8 = 256 - first;
    uint32_t r6 = r10 + r11;
    r10 = r9 - r31;
    r7 = r7 + in;
    uint32_t r4 = start + 4;
    r30 = r31 - r30;
    r9 = r8 + r9;
    do {
      double f11b = f12;
      const uint32_t r31b = r30 + r10;
      const uint32_t w10 = Word(r7 + 0);
      const uint32_t r3b = r10;
      if (sets_r3) {
        r3 = (r3b << 2) & kMask;
      }
      f12 = Round(f12 - f0);
      r8 -= 1;
      r10 += 1;
      r7 += 4;
      double f9 = Tap(r3b);
      f9 = Round(f9 * f13);
      const double f8 = Tap(r31b);
      SetWord(r6 + 0, w10);
      f13 = Round(f13 + f0);
      r6 += 4;
      f11b = Fma(f8, f11b, f9);
      Store(r4 + 0, f11b);
      r4 += 4;
    } while (r8 != 0);
    ReverbCrossfade(s).set_position(float(f12));
    return r9;
  }

  // The rest of the fade once the taps moved on (from frame r28), then the
  // last output and the write index stored.
  void Finish(uint32_t r28, uint32_t r9) {
    const uint32_t r24 = ReverbCrossfade(s).tap_asked();
    const uint32_t r10 = ReverbCrossfade(s).tap_to();
    f12 = r24 == r10 ? Load(kZero) : f11;
    const uint32_t r8 = 256 - r28;
    ReverbCrossfade(s).set_position(float(f12));
    f13 = Round(f11 - f12);
    uint32_t r26 = r28;
    WriteTaps(r10, r24);
    if (int32_t(r8) >= 4) {
      const uint32_t r27 = ReverbCrossfade(s).tap_from();
      const uint32_t r29 = ((252 - r28) >> 2) + 1;
      const uint32_t r26c = r29 << 2;
      const uint32_t r30 = r9 - r24;
      const uint32_t r31 = r9 - r27;
      const uint32_t w8 = ((r9 + 2) << 2) + r11;
      const uint32_t r7 = ((r28 + 3) << 2) + in;
      const uint32_t r10b = ((r28 + 2) << 2) + out;
      r9 = r26c + r9;
      r26 = r26c + r28;
      FourFramesAfter(r29, r30, r31 + 2, r30 + 2, w8, r10b, r7, r24 - r27);
    }
    if (r26 < 256) {
      r9 = OneAtATime(r26, r9);
    }
    const uint32_t last = Word(out + 1024);
    SetWord(ReverbCrossfade(s).last_output_address(), last);
    ReverbCrossfade(s).set_write_index(r9 & kWrap);
    ReverbCrossfade(s).set_write_index(r9 & kWrap);
  }

  // The taps moved on (+12 the one faded to, +8 the one asked for): in each
  // original's order.
  bool taps_8_first = false;
  void WriteTaps(uint32_t from, uint32_t to) {
    if (taps_8_first) {
      ReverbCrossfade(s).set_tap_to(to);
      ReverbCrossfade(s).set_tap_from(from);
    } else {
      ReverbCrossfade(s).set_tap_from(from);
      ReverbCrossfade(s).set_tap_to(to);
    }
  }

  // No crossfade (the position past 256 frames): the line written and
  // read at the tap faded to... (the original's two gains, f0 and f13).
  void Steady(uint32_t r9, uint32_t r10, uint32_t r23) {
    const uint32_t r6 = ReverbCrossfade(s).line_address();
    uint32_t r8 = (r9 << 2) + r6;
    uint32_t r7 = out + 4;
    double g0 = Load(kOne);
    uint32_t r11b = r9 - r10;
    double g13 = Round(g0 - f12);
    g0 = f12;
    const double g12 = Load(kFadeStep);
    const uint32_t end = r9 + 256;
    uint32_t r4 = in;
    for (int k = 0; k < 256; k++) {
      const uint32_t r3i = r11b + r23;
      if (sets_r3) {
        r3 = (r3i << 2) & kMask;
      }
      const uint32_t w9 = Word(r4 + 0);
      const uint32_t r31 = r11b;
      r11b += 1;
      r4 += 4;
      double f11b = Load(((r31 << 2) & kMask) + r6);
      double f10 = Load(((r3i << 2) & kMask) + r6);
      f11b = Round(f11b * g13);
      f10 = Round(f10 * g0);
      SetWord(r8 + 0, w9);
      g0 = Round(g0 - g12);
      r8 += 4;
      g13 = Round(g13 + g12);
      f11b = Round(f11b + f10);
      Store(r7 + 0, f11b);
      r7 += 4;
    }
    ReverbCrossfade(s).set_position(float(g0));
    const uint32_t last = Word(out + 1024);
    SetWord(ReverbCrossfade(s).last_output_address(), last);
    ReverbCrossfade(s).set_write_index(end & kWrap);
  }
};

}  // namespace

uint64_t ReverbCrossfadeDelay(uint64_t r3_in, uint32_t in, uint32_t out,
                              uint32_t sp) {
  const uint32_t s = uint32_t(r3_in);
  ITC_NATIVE_RUNNING("sub_824624F8 the reverb's crossfading 16,384-entry delay line");
  ScalarMode mode;
  Crossfade<0xFFFC, 0x3FFF> c{s, in, out, 0, 0, 0, 0, 0};
  c.taps_8_first = false;
  c.sets_r3 = true;
  c.r3 = r3_in;
  uint32_t r8 = ReverbCrossfade(s).tap_from();
  uint32_t r10 = ReverbCrossfade(s).tap_to();
  const uint32_t r23 = r10 - r8;
  CopyFloat(out + 0, ReverbCrossfade(s).last_output_address());
  const bool r6 = r23 > 4 || r8 - r10 > 4;
  double f0 = Load(kFadeScale);
  c.f12 = double(ReverbCrossfade(s).position());
  f0 = Round(c.f12 * f0);
  uint32_t r9 = ReverbCrossfade(s).write_index();
  SetWord(sp - 176, uint32_t(uint64_t(ToInt64(f0))));
  const uint32_t r28 = Word(sp - 176) >> 1;
  if (r28 < 256) {
    uint32_t r24 = 1;
    c.f11 = Load(kOne);
    c.r11 = ReverbCrossfade(s).line_address();
    c.f0 = Load(kFadeStep);
    c.f13 = Round(c.f11 - c.f12);
    if (int32_t(r28) >= 4) {
      const uint32_t r29 = r28 >> 2;
      const uint32_t r30 = r9 - r10;
      const uint32_t r31 = r9 - r8 + 2;
      const uint32_t w8 = ((r9 + 2) << 2) + c.r11;
      const uint32_t r27 = r29 << 2;
      c.FourFrames(r29, r30, r31, r30 + 2, w8, out + 12, in + 8, r23);
      r9 = r27 + r9;
      r24 = r27 + 1;
    }
    if (!(r24 > r28)) {
      // One frame at a time to frame r28.
      uint32_t r7 = r24 << 2;
      const uint32_t r30 = ReverbCrossfade(s).tap_to();
      uint32_t r10b = r28 - r24;
      const uint32_t r27 = ReverbCrossfade(s).tap_from();
      uint32_t r8b = r9 << 2;
      c.f12 = double(ReverbCrossfade(s).position());
      r10b += 1;
      const uint32_t r29 = r7 + in;
      uint32_t r6b = r8b + c.r11;
      r8b = r9 - r30;
      uint32_t r31 = r7 + out;
      r7 = r29 - 4;
      const uint32_t r30b = r30 - r27;
      r9 = r10b + r9;
      do {
        double f10 = c.f12;
        const uint32_t r27b = r30b + r8b;
        const uint32_t w9 = Word(r7 + 0);
        const uint32_t r29b = r8b;
        c.f12 = Round(c.f12 - c.f0);
        r10b -= 1;
        r8b += 1;
        r7 += 4;
        double f8 = c.Tap(r29b);
        f8 = Round(f8 * c.f13);
        const double f7 = c.Tap(r27b);
        SetWord(r6b + 0, w9);
        c.f13 = Round(c.f13 + c.f0);
        r6b += 4;
        f10 = Fma(f7, f10, f8);
        Store(r31 + 0, f10);
        r31 += 4;
      } while (r10b != 0);
      ReverbCrossfade(s).set_position(float(c.f12));
    }
    c.Finish(r28, r9);
    return c.r3;
  }
  // The crossfade is over: a plain copy when the line doesn't wrap in this
  // block and the taps are as asked, else the steady path.
  const uint32_t r31 = r9 + 256;
  const uint32_t r7 = r9 - r8;
  const uint32_t r11 = r9 - r10;
  if (r31 < 16384 && r7 + 256 < 16384 && r11 + 256 < 16384 && int32_t(r7) >= 0 &&
      int32_t(r11) >= 0) {
    uint32_t line = ((r9 + 8) << 2) + s;
    if (!r6) {
      // 65 copies of 16 bytes: the input into the line and the line at the
      // tap faded from into the output (+4), each pair's loads first.
      uint32_t from = line - (r8 << 2);
      uint32_t to = out + 4;
      uint32_t src = in;
      uint64_t r3 = r3_in;
      for (int k = 0; k < 65; k++) {
        // r3 = the line's address as a 64-bit sum with the incoming r3.
        r3 = uint64_t(uint32_t((r9 + 8) << 2)) + r3_in + 16 * uint64_t(k);
        uint8_t a[16], b[16];
        for (int i = 0; i < 16; i++) {
          a[i] = guest::LoadU8(src + i);
          b[i] = guest::LoadU8(from + i);
        }
        for (int i = 0; i < 16; i++) {
          guest::StoreU8(line + i, a[i]);
        }
        for (int i = 0; i < 16; i++) {
          guest::StoreU8(to + i, b[i]);
        }
        line += 16;
        from += 16;
        src += 16;
        to += 16;
      }
      ReverbCrossfade(s).set_write_index(r31);
      double f0b = Load(kFadeEnd);
      f0b = Round(c.f12 - f0b);
      ReverbCrossfade(s).set_position(float(f0b));
      CopyFloat(ReverbCrossfade(s).last_output_address(), out + 1024);
      return r3;
    }
    // The taps differ by more than 4: crossfade from +8's to +12's.
    uint32_t r7b = r10 << 2;
    const uint32_t r6b = r8 << 2;
    uint32_t r8b = out + 4;
    double f0b = Load(kOne);
    uint32_t r10b = line - r7b;
    r7b = r7b - r6b;
    f0b = Round(f0b - c.f12);
    const double f13 = Load(kFadeStep);
    uint32_t src = in;
    for (int k = 0; k < 256; k++) {
      double f11 = Load(r10b + 0);
      const double f10 = c.f12;
      f11 = Round(f11 * f0b);
      const double f9 = Load(r7b + r10b);
      const uint32_t w8 = Word(src + 0);
      SetWord(line + 0, w8);
      c.f12 = Round(c.f12 - f13);
      f0b = Round(f0b + f13);
      line += 4;
      r10b += 4;
      src += 4;
      f11 = Fma(f9, f10, f11);
      Store(r8b + 0, f11);
      r8b += 4;
    }
    const uint32_t last = Word(out + 1024);
    ReverbCrossfade(s).set_write_index(r31);
    ReverbCrossfade(s).set_position(float(c.f12));
    SetWord(ReverbCrossfade(s).last_output_address(), last);
    return r3_in;
  }
  c.Steady(r9, r10, r23);
  return c.r3;
}

void ReverbCrossfadeDelay256(uint32_t s, uint32_t in, uint32_t out, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82463620 the reverb's crossfading 256-entry delay line");
  ScalarMode mode;
  Crossfade<0x3FC, 0xFF> c{s, in, out, 0, 0, 0, 0, 0};
  c.taps_8_first = true;
  CopyFloat(out + 0, ReverbCrossfade(s).last_output_address());
  c.f12 = double(ReverbCrossfade(s).position());
  uint32_t r10 = ReverbCrossfade(s).write_index();
  double f0 = Load(kFadeScale);
  f0 = Round(c.f12 * f0);
  SetWord(sp - 128, uint32_t(uint64_t(ToInt64(f0))));
  const uint32_t r28 = Word(sp - 128) >> 1;
  if (r28 < 256) {
    uint32_t r23 = 1;
    c.f11 = Load(kOne);
    c.r11 = ReverbCrossfade(s).line_address();
    c.f0 = Load(kFadeStep);
    c.f13 = Round(c.f11 - c.f12);
    if (int32_t(r28) >= 4) {
      const uint32_t r6 = ReverbCrossfade(s).tap_to();
      const uint32_t r31 = ReverbCrossfade(s).tap_from();
      const uint32_t r29 = r28 >> 2;
      const uint32_t w8 = ((r10 + 2) << 2) + c.r11;
      const uint32_t r30 = r10 - r6;
      const uint32_t r27 = r29 << 2;
      const uint32_t r26 = r10 - r31;
      c.FourFrames(r29, r30, r26 + 2, r30 + 2, w8, out + 12, in + 8, r6 - r31);
      r10 = r27 + r10;
      r23 = r27 + 1;
    }
    if (!(r23 > r28)) {
      // One frame at a time to frame r28 (the gains' roles as the original
      // has them: the position on the tap faded from, the rest on the other).
      uint32_t r7 = r23 << 2;
      const uint32_t r30 = ReverbCrossfade(s).tap_to();
      uint32_t r9 = r28 - r23;
      uint32_t r27 = ReverbCrossfade(s).tap_from();
      uint32_t r8 = r10 << 2;
      c.f12 = double(ReverbCrossfade(s).position());
      r9 += 1;
      const uint32_t r29 = r7 + in;
      uint32_t r6 = r8 + c.r11;
      r8 = r10 - r30;
      uint32_t r31 = r7 + out;
      r7 = r29 - 4;
      r27 = r30 - r27;
      r10 = r9 + r10;
      do {
        const uint32_t r29b = r27 + r8;
        double f10 = c.f13;
        const uint32_t r30b = r8;
        const uint32_t w9 = Word(r7 + 0);
        c.f13 = Round(c.f13 + c.f0);
        r9 -= 1;
        r8 += 1;
        r7 += 4;
        double f7 = c.Tap(r29b);
        f7 = Round(f7 * c.f12);
        const double f8 = c.Tap(r30b);
        SetWord(r6 + 0, w9);
        c.f12 = Round(c.f12 - c.f0);
        r6 += 4;
        f10 = Fma(f8, f10, f7);
        Store(r31 + 0, f10);
        r31 += 4;
      } while (r9 != 0);
      ReverbCrossfade(s).set_position(float(c.f12));
    }
    c.Finish(r28, r10);
    return;
  }
  const uint32_t r31 = ReverbCrossfade(s).tap_to();
  const uint32_t r30 = ReverbCrossfade(s).tap_from();
  c.Steady(r10, r31, r31 - r30);
}

}  // namespace xaudio
