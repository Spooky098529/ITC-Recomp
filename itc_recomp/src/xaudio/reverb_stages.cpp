// Stages of XAudio's reverb core as plain C++ (reverb_stages.h;
// docs/functions/xaudio-objects.md).

#include "xaudio/reverb_stages.h"

#include "base/log.h"
#include "guest.h"

#include <rex/platform/fpscr.h>

#include <cmath>

namespace xaudio {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

// Flush to zero off for the scalar unit's float work, as the translated code
// sets it; the setting found on entry put back on return.
class ScalarMode {
 public:
  ScalarMode() : saved_(Fpscr::getcsr()) {
    Fpscr::setcsr(saved_ & ~uint32_t(Fpscr::FlushMask));
  }
  ~ScalarMode() { Fpscr::setcsr(saved_); }
  ScalarMode(const ScalarMode&) = delete;
  ScalarMode& operator=(const ScalarMode&) = delete;

 private:
  uint32_t saved_;
};

double Load(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
void Store(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
void CopyFloat(uint8_t* base, uint32_t destination, uint32_t source) {
  guest::StoreU32(base, destination, guest::LoadU32(base, source));
}
double Round(double value) { return double(float(value)); }
// fmadds: a x b + c, rounded once to a float.
double Fma(double a, double b, double c) { return double(float(std::fma(a, b, c))); }

// The delay lines with three taps (sub_82462CF8, sub_82463060): `index_mask`
// the byte index's mask, `wrap` the entry index's.
void DelayThreeTaps(uint8_t* base, uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b,
                    uint32_t index_mask, uint32_t wrap) {
  ScalarMode mode;
  const uint32_t r11_start = guest::LoadU32(base, line + 12);
  const uint32_t data = line + 44;
  const uint32_t first = guest::LoadU32(base, line + 0);
  const uint32_t write = guest::LoadU32(base, line + 36);
  const uint32_t third = guest::LoadU32(base, line + 24);
  const uint32_t r31 = r11_start - first;
  const double f0 = Load(base, line + 16);
  const uint32_t r30 = r11_start - third;
  const double f13 = Load(base, line + 4);
  uint32_t r11 = write - r11_start;
  const double f12 = Load(base, line + 28);
  CopyFloat(base, out_a, line + 8);
  CopyFloat(base, out_b, line + 32);
  uint32_t r8 = (write << 2) + data;
  double f10 = 0, f11 = 0;
  for (int k = 0; k < 256; k++) {
    const uint32_t r29 = (r11 << 2) & index_mask;
    const uint32_t r28 = ((r31 + r11) << 2) & index_mask;
    const uint32_t r27 = ((r30 + r11) << 2) & index_mask;
    f10 = Load(base, r29 + data);
    out_a += 4;
    f10 = Round(f10 * f0);
    out_b += 4;
    const double f9 = Load(base, r28 + data);
    const double f8 = Load(base, r27 + data);
    r11 += 1;
    CopyFloat(base, r8, in);
    in += 4;
    f11 = Round(f8 * f12);
    Store(base, out_b, f11);
    r8 += 4;
    f10 = Fma(f9, f13, f10);
    Store(base, out_a, f10);
  }
  Store(base, line + 8, f10);
  Store(base, line + 32, f11);
  guest::StoreU32(base, line + 36, (write + 256) & wrap);
}


// The all-pass stages (sub_82462DC8, sub_82463130, sub_824632A0,
// sub_82463410): the state +0 the tap, +4 and +8 the gains, +12 the last
// output, +16 the write index, the line's data at +24. A block of eight
// frames a pass, in the original's order of loads and stores: each frame's
// input x and the line's entry d at the tap (masked to `mask`), v = +8 x d
// + x written to the line (unmasked, from `write`), and +4 x v + d the
// output, a frame late (the first the last block's).
struct AllPass {
  uint8_t* base;
  uint32_t data;
  uint32_t mask;
  double f0, f13;
  double Tap(uint32_t r11, int k) const {
    return Load(base, data + (((r11 + uint32_t(k)) << 2) & mask));
  }
};

// The in-place pass (sub_82462DC8's first half, sub_82463130, sub_824632A0):
// the frames at `p` replaced; r11 the tap's index (+2), `q` the write
// address (+8). Returns the carried output.
double AllPassInPlace(const AllPass& a, uint32_t& p, uint32_t& q, uint32_t& r11, double f11,
                      int passes) {
  uint8_t* base = a.base;
  const double f0 = a.f0, f13 = a.f13;
  for (int pass = 0; pass < passes; pass++) {
    double f10 = Load(base, p + 0);
    Store(base, p + 0, f11);
    double f9 = Load(base, p + 4);
    const double f8 = Load(base, p + 8);
    const double f7 = Load(base, p + 12);
    const double f6 = Load(base, p + 16);
    const double f5 = Load(base, p + 20);
    double f12 = a.Tap(r11, -2);
    f11 = Fma(f0, f12, f10);
    Store(base, q - 8, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, -1);
    Store(base, p + 4, f11);
    f11 = Fma(f0, f12, f9);
    Store(base, q - 4, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 0);
    Store(base, p + 8, f11);
    f11 = Fma(f0, f12, f8);
    Store(base, q + 0, f11);
    f9 = Load(base, p + 28);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 1);
    Store(base, p + 12, f11);
    f11 = Fma(f0, f12, f7);
    Store(base, q + 4, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 2);
    Store(base, p + 16, f11);
    f11 = Fma(f0, f12, f6);
    Store(base, q + 8, f11);
    f10 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 3);
    f11 = Fma(f0, f12, f5);
    Store(base, q + 12, f11);
    Store(base, p + 20, f10);
    f10 = Load(base, p + 24);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 4);
    Store(base, p + 24, f11);
    f11 = Fma(f0, f12, f10);
    Store(base, q + 16, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 5);
    Store(base, p + 28, f11);
    f11 = Fma(f0, f12, f9);
    Store(base, q + 20, f11);
    q += 32;
    p += 32;
    r11 += 8;
    f11 = Fma(f13, f11, f12);
  }
  return f11;
}

// sub_82463130 (256 entries) and sub_824632A0 (512): one in-place pass of
// 256 frames, the write index kept by `wrap`.
void AllPassBlock(uint8_t* base, uint32_t state, uint32_t p, uint32_t mask, uint32_t wrap) {
  ScalarMode mode;
  const uint32_t write = guest::LoadU32(base, state + 16);
  const uint32_t tap = guest::LoadU32(base, state + 0);
  const AllPass a{base, state + 24, mask, Load(base, state + 8), Load(base, state + 4)};
  const double f11 = Load(base, state + 12);
  uint32_t r11 = write - tap + 2;
  uint32_t q = ((write + 2) << 2) + a.data;
  const double last = AllPassInPlace(a, p, q, r11, f11, 32);
  Store(base, state + 12, last);
  Store(base, p + 0, last);
  guest::StoreU32(base, state + 16, (write + 256) & wrap);
}
}  // namespace

void ReverbLowPass(uint8_t* base, uint32_t state, uint32_t in, uint32_t out, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82462BC8 the reverb's low-pass");
  ScalarMode mode;
  double f9 = Load(base, state + 20);
  double f12 = Load(base, state + 16);
  uint32_t r9 = 1;
  CopyFloat(base, out, state + 20);
  uint32_t r11 = out + 4;
  const double f7 = Load(base, 0x82000A30);
  bool ran = false;
  if (int32_t(count) >= 4) {
    ran = true;
    const double f0 = Load(base, state + 28);
    uint32_t r10 = count >> 2;
    const double f10 = Round(f7 - f0);
    const double f11 = Load(base, state + 24);
    r9 = (r10 << 2) + 1;
    const double f13 = Round(f11 * f10);
    do {
      double f6 = Load(base, in);
      double f8 = f12;
      f6 = Round(f6 * f13);
      double f5 = Load(base, in + 4);
      f9 = Round(f0 * f9);
      double f4 = Load(base, in + 8);
      f5 = Round(f5 * f11);
      const double f3 = Load(base, in + 12);
      f4 = Round(f4 * f11);
      r10 -= 1;
      in += 16;
      f12 = Fma(f0, f12, f6);
      f9 = Fma(f13, f8, f9);
      Store(base, r11 + 0, f9);
      f6 = Round(f3 * f11);
      f8 = f12;
      f12 = Round(f0 * f12);
      f12 = Fma(f5, f10, f12);
      f5 = Round(f0 * f9);
      f9 = Fma(f13, f8, f5);
      Store(base, r11 + 4, f9);
      f8 = f12;
      f12 = Round(f0 * f12);
      f5 = Round(f0 * f9);
      f12 = Fma(f4, f10, f12);
      f9 = Fma(f13, f8, f5);
      Store(base, r11 + 8, f9);
      f8 = f12;
      f12 = Round(f0 * f12);
      f12 = Fma(f6, f10, f12);
      f6 = Round(f0 * f9);
      f9 = Fma(f13, f8, f6);
      Store(base, r11 + 12, f9);
      r11 += 16;
    } while (r10 != 0);
  }
  if (!(r9 > count)) {
    ran = true;
    const double f0 = Load(base, state + 28);
    uint32_t r10 = count - r9;
    const double f11 = Round(f7 - f0);
    double f13 = Load(base, state + 24);
    r10 += 1;
    f13 = Round(f11 * f13);
    do {
      const double previous = f12;
      r10 -= 1;
      f12 = Round(f0 * f12);
      const double f10 = Round(f0 * f9);
      const double sample = Load(base, in);
      in += 4;
      f12 = Fma(f13, sample, f12);
      f9 = Fma(f13, previous, f10);
      Store(base, r11, f9);
      r11 += 4;
    } while (r10 != 0);
  }
  if (ran) {
    Store(base, state + 16, f12);
    Store(base, state + 20, f9);
  }
  // With no frames the state is stored back as it was loaded (its bits).
}

void ReverbDelay512(uint8_t* base, uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b) {
  ITC_NATIVE_RUNNING("sub_82462CF8 the reverb's 512-entry delay line");
  DelayThreeTaps(base, line, in, out_a, out_b, 0x7FC, 0x1FF);
}

void ReverbDelay2048(uint8_t* base, uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b) {
  ITC_NATIVE_RUNNING("sub_82463060 the reverb's 2,048-entry delay line");
  DelayThreeTaps(base, line, in, out_a, out_b, 0x1FFC, 0x7FF);
}

void ReverbDelay1024(uint8_t* base, uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b) {
  ITC_NATIVE_RUNNING("sub_82463578 the reverb's 1,024-entry delay line");
  ScalarMode mode;
  const uint32_t write = guest::LoadU32(base, line + 24);
  const uint32_t data = line + 32;
  const uint32_t second = guest::LoadU32(base, line + 12);
  const uint32_t first = guest::LoadU32(base, line + 0);
  uint32_t r8 = (write << 2) + data;
  uint32_t r9 = write - second;
  uint32_t r10 = write - first;
  const double f12 = Load(base, line + 4);
  const double f11 = Load(base, line + 16);
  CopyFloat(base, out_a, line + 8);
  CopyFloat(base, out_b, line + 20);
  double f0 = 0, f13 = 0;
  for (int k = 0; k < 256; k++) {
    const uint32_t r30 = (r10 << 2) & 0xFFC;
    const uint32_t r29 = (r9 << 2) & 0xFFC;
    out_a += 4;
    out_b += 4;
    f0 = Load(base, r30 + data);
    f13 = Load(base, r29 + data);
    f0 = Round(f0 * f12);
    CopyFloat(base, r8, in);
    in += 4;
    f13 = Round(f13 * f11);
    Store(base, out_a, f0);
    r10 += 1;
    Store(base, out_b, f13);
    r9 += 1;
    r8 += 4;
  }
  Store(base, line + 8, f0);
  Store(base, line + 20, f13);
  guest::StoreU32(base, line + 24, (write + 256) & 0x3FF);
}

}  // namespace xaudio

namespace xaudio {

void ReverbAllPass128(uint8_t* base, uint32_t state, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_82462DC8 the reverb's 128-entry all-pass");
  ScalarMode mode;
  const uint32_t write = guest::LoadU32(base, state + 16);
  const uint32_t tap = guest::LoadU32(base, state + 0);
  const AllPass a{base, state + 24, 0x1FC, Load(base, state + 8), Load(base, state + 4)};
  double f11 = Load(base, state + 12);
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
    double f10 = Load(base, p + 0);
    Store(base, p + 0, f12);
    double f9 = Load(base, p + 4);
    const double f8 = Load(base, p + 8);
    const double f7 = Load(base, p + 12);
    f11 = a.Tap(r9b, -2);
    f12 = Fma(f0, f11, f10);
    Store(base, r10b - 8, f12);
    const double f6 = Load(base, p + 16);
    const double f5 = Load(base, p + 20);
    f11 = Fma(f13, f12, f11);
    f12 = a.Tap(r9b, -1);
    Store(base, p + 4, f11);
    f11 = Fma(f0, f12, f9);
    Store(base, r10b - 4, f11);
    f9 = Load(base, p + 28);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 0);
    Store(base, p + 8, f11);
    f11 = Fma(f0, f12, f8);
    Store(base, r10b + 0, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 1);
    Store(base, p + 12, f11);
    f11 = Fma(f0, f12, f7);
    Store(base, r10b + 4, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 2);
    Store(base, p + 16, f11);
    f11 = Fma(f0, f12, f6);
    Store(base, r10b + 8, f11);
    f10 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 3);
    f11 = Fma(f0, f12, f5);
    Store(base, r10b + 12, f11);
    Store(base, p + 20, f10);
    f10 = Load(base, p + 24);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 4);
    Store(base, p + 24, f11);
    f11 = Fma(f0, f12, f10);
    Store(base, r10b + 16, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r9b, 5);
    Store(base, p + 28, f11);
    f11 = Fma(f0, f12, f9);
    Store(base, r10b + 20, f11);
    p += 32;
    r9b += 8;
    r10b += 32;
    f12 = Fma(f13, f11, f12);
  }
  Store(base, state + 12, f12);
  Store(base, p + 0, f12);
  guest::StoreU32(base, state + 16, 0);
}

void ReverbAllPass256(uint8_t* base, uint32_t state, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_82463130 the reverb's 256-entry all-pass");
  AllPassBlock(base, state, p, 0x3FC, 0xFF);
}

void ReverbAllPass512(uint8_t* base, uint32_t state, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_824632A0 the reverb's 512-entry all-pass");
  AllPassBlock(base, state, p, 0x7FC, 0x1FF);
}

void ReverbAllPass512To(uint8_t* base, uint32_t state, uint32_t in, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82463410 the reverb's 512-entry all-pass to a buffer");
  ScalarMode mode;
  const uint32_t write = guest::LoadU32(base, state + 16);
  const uint32_t tap = guest::LoadU32(base, state + 0);
  const AllPass a{base, state + 24, 0x7FC, Load(base, state + 8), Load(base, state + 4)};
  const double f0 = a.f0, f13 = a.f13;
  double f11 = Load(base, state + 12);
  uint32_t r11 = write - tap + 2;
  uint32_t q = ((write + 2) << 2) + a.data;
  for (int pass = 0; pass < 32; pass++) {
    double f10 = Load(base, in + 0);
    Store(base, out + 0, f11);
    double f9 = Load(base, in + 4);
    const double f8 = Load(base, in + 8);
    const double f7 = Load(base, in + 12);
    const double f6 = Load(base, in + 16);
    const double f5 = Load(base, in + 20);
    double f12 = a.Tap(r11, -2);
    f11 = Fma(f0, f12, f10);
    Store(base, q - 8, f11);
    f10 = Load(base, in + 24);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, -1);
    Store(base, out + 4, f11);
    f11 = Fma(f0, f12, f9);
    Store(base, q - 4, f11);
    f9 = Load(base, in + 28);
    in += 32;
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 0);
    Store(base, out + 8, f11);
    f11 = Fma(f0, f12, f8);
    Store(base, q + 0, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 1);
    Store(base, out + 12, f11);
    f11 = Fma(f0, f12, f7);
    Store(base, q + 4, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 2);
    Store(base, out + 16, f11);
    f11 = Fma(f0, f12, f6);
    Store(base, q + 8, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 3);
    Store(base, out + 20, f11);
    f11 = Fma(f0, f12, f5);
    Store(base, q + 12, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 4);
    Store(base, out + 24, f11);
    f11 = Fma(f0, f12, f10);
    Store(base, q + 16, f11);
    f11 = Fma(f13, f11, f12);
    f12 = a.Tap(r11, 5);
    Store(base, out + 28, f11);
    f11 = Fma(f0, f12, f9);
    Store(base, q + 20, f11);
    out += 32;
    q += 32;
    r11 += 8;
    f11 = Fma(f13, f11, f12);
  }
  Store(base, state + 12, f11);
  Store(base, in + 0, f11);
  guest::StoreU32(base, state + 16, (write + 256) & 0x1FF);
}

}  // namespace xaudio
