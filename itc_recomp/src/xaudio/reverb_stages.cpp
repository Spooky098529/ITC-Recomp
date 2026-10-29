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
