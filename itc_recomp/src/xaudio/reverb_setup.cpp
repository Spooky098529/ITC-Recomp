// XAudio's reverb core's set-up as plain C++ (reverb_core.h;
// docs/functions/xaudio-objects.md): ReverbCore::Apply (sub_82467A18).
// Translated step by step from the original, in its order of loads,
// stores, roundings and calls, with the registers' names kept (long schedules of address arithmetic over
// the core's ~300 KB of delay lines and filters): the general registers as
// 64-bit words, the float registers as doubles, a compare's result in `cr6`.
// A float stored straight from its load keeps its bits (its word kept in
// wN), as the translated code's folded lfs/stfs pairs do. Flush to zero is
// off, as the translated scalar code sets it. Apply's frame (432 bytes below
// the caller's stack pointer) gets the words the original writes there; its
// register saves aren't written. Built with strict floating point and no
// contraction (CMakeLists.txt).

#include "xaudio/reverb_core.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "xaudio/objects.h"
#include "xaudio/ppc_float.h"

#include <simde/x86/sse2.h>

#include <bit>
#include <climits>
#include <cmath>

namespace xaudio {

namespace {

using namespace ppc_float;

uint64_t L8(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
uint64_t L32(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
uint64_t L64(uint8_t* base, uint32_t address) { return REX_LOAD_U64(address); }
void S8(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU8(base, address, uint8_t(value));
}
void S32(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value));
}
void S64(uint8_t* base, uint32_t address, uint64_t value) { REX_STORE_U64(address, value); }
// lfs: the word as a float, widened.
double ToDouble(uint64_t word) { return double(std::bit_cast<float>(uint32_t(word))); }
// stfs: rounded to a float.
void SF(uint8_t* base, uint32_t address, double value) {
  guest::StoreU32(base, address, std::bit_cast<uint32_t>(float(value)));
}
// A float register's bits (lfd, stfiwx, fcfid, fneg work on them).
uint64_t Bits(double value) { return std::bit_cast<uint64_t>(value); }
double FromBits(uint64_t bits) { return std::bit_cast<double>(bits); }
// fctidz as the translated code computes it: the integer's bits.
double Fctidz(double value) {
  const int64_t integer = std::isnan(value)                 ? int64_t(0x8000000000000000ull)
                          : value > double(LLONG_MAX) ? LLONG_MAX
                                                      : simde_mm_cvttsd_si64(simde_mm_load_sd(&value));
  return FromBits(uint64_t(integer));
}
// rlwinm over the word doubled into both halves, as the translated code.
uint64_t Rlwinm(uint64_t value, int shift, uint64_t mask) {
  return std::rotl(uint64_t(uint32_t(value)) | (value << 32), shift) & mask;
}

// A compare's result (cmplw, cmplwi, fcmpu: an unordered compare sets none).
struct Cr {
  bool lt = false, gt = false, eq = false;
};
template <typename T>
Cr Compare(T a, T b) {
  return {a < b, a > b, a == b};
}
template <typename T>
Cr Compare(T a, int b) {
  return Compare<T>(a, T(b));
}
Cr CompareF(double a, double b) { return {a < b, a > b, a == b}; }

// memcpy's plain C++, or its original (in the caller's frame at `sp`) where
// that refuses (the native parameters copied over themselves, as
// Reverb::Run's +52 to +4 is).
void Memcpy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size,
            uint32_t return_address, uint32_t sp, const GuestCall& call) {
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    Regs regs;
    regs.r[0] = destination;
    regs.r[1] = source;
    regs.r[2] = size;
    regs.count = 3;
    regs.sp = sp;
    call(kMemcpy, return_address, regs);
  }
}

}  // namespace

uint64_t ReverbCoreApply(uint8_t* base, uint32_t core, uint32_t params, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82467A18 ReverbCore::Apply");
  ScalarMode mode;
  uint64_t r1 = frame, r3 = core, r4 = params;
  [[maybe_unused]] uint64_t r0 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  [[maybe_unused]] uint64_t r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0;
  [[maybe_unused]] uint64_t r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0;
  [[maybe_unused]] uint64_t r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  [[maybe_unused]] double f0 = 0, f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0, f7 = 0;
  [[maybe_unused]] double f8 = 0, f9 = 0, f10 = 0, f11 = 0, f12 = 0, f13 = 0, f14 = 0, f15 = 0;
  [[maybe_unused]] double f16 = 0, f17 = 0, f18 = 0, f19 = 0, f20 = 0, f21 = 0, f22 = 0;
  [[maybe_unused]] double f23 = 0, f24 = 0, f25 = 0, f26 = 0, f27 = 0, f28 = 0, f29 = 0;
  [[maybe_unused]] double f30 = 0, f31 = 0;
  [[maybe_unused]] uint32_t w0 = 0, w1 = 0, w2 = 0, w3 = 0, w4 = 0, w5 = 0, w6 = 0, w7 = 0;
  [[maybe_unused]] uint32_t w8 = 0, w9 = 0, w10 = 0, w11 = 0, w12 = 0, w13 = 0, w14 = 0;
  [[maybe_unused]] uint32_t w15 = 0, w16 = 0, w17 = 0, w18 = 0, w19 = 0, w20 = 0, w21 = 0;
  [[maybe_unused]] uint32_t w22 = 0, w23 = 0, w24 = 0, w25 = 0, w26 = 0, w27 = 0, w28 = 0;
  [[maybe_unused]] uint32_t w29 = 0, w30 = 0, w31 = 0;
  [[maybe_unused]] uint32_t t = 0, ca = 0;
  [[maybe_unused]] Cr cr6;
  r31 = r3;
  r5 = 84;
  r3 = r31 + 4;
  r29 = r4;
  Memcpy(base, uint32_t(r3), uint32_t(r4), uint32_t(r5), 0x82467A40, uint32_t(r1), call);
  r10 = 0xFFFFFFFF82000000ull;
  r11 = L32(base, uint32_t(r29) + 4);
  cr6 = Compare<uint32_t>(uint32_t(r11), 0);
  w0 = L32(base, uint32_t(r10) + 10712); f0 = ToDouble(w0);
  S32(base, uint32_t(r1) + 96, w0);
  if (cr6.eq) {
    r3 = 1;
  } else {
    r11 = r11 & 0xFFFFFFFF;
    w13 = L32(base, uint32_t(r31) + 88); f13 = ToDouble(w13);
    r10 = r1 + 88;
    S64(base, uint32_t(r1) + 120, r11);
    f12 = FromBits(L64(base, uint32_t(r1) + 120));
    f12 = double(int64_t(Bits(f12)));
    f12 = double(float(f12));
    f13 = double(float(f13 * f12));
    f0 = double(float(f13 * f0));
    f0 = Fctidz(f0);
    S32(base, uint32_t(r10), uint32_t(Bits(f0)));
    r3 = L32(base, uint32_t(r1) + 88);
  }
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 4095);
  if (cr6.gt) {
    r3 = 4095;
  }
  r11 = 131072;
  r10 = 131072;
  r11 = r11 | 40712;
  r9 = 196608;
  r8 = 262144;
  r10 = r10 | 61380;
  r9 = r9 | 41056;
  S32(base, uint32_t(r31) + uint32_t(r11), uint32_t(r3));
  r11 = 0xFFFFFFFF82000000ull;
  r8 = r8 | 20736;
  S32(base, uint32_t(r31) + uint32_t(r10), uint32_t(r3));
  S32(base, uint32_t(r31) + uint32_t(r9), uint32_t(r3));
  w10 = L32(base, uint32_t(r11) + 9596); f10 = ToDouble(w10);
  r11 = 0xFFFFFFFF82050000ull;
  S32(base, uint32_t(r31) + uint32_t(r8), uint32_t(r3));
  w0 = L32(base, uint32_t(r29) + 80); f0 = ToDouble(w0);
  r30 = r11 - 21464;
  cr6 = CompareF(f0, f10);
  r11 = 0xFFFFFFFF82050000ull;
  w13 = L32(base, uint32_t(r11) + -21480); f13 = ToDouble(w13);
  S32(base, uint32_t(r1) + 80, w13);
  if (!cr6.lt) {
    r11 = 0xFFFFFFFF82000000ull;
    w12 = L32(base, uint32_t(r11) + 8748); f12 = ToDouble(w12);
    f0 = double(float(f0 * f12));
  } else {
    r11 = 0xFFFFFFFF82000000ull;
    w12 = L32(base, uint32_t(r30) + 4); f12 = ToDouble(w12);
    w11 = L32(base, uint32_t(r11) + 3396); f11 = ToDouble(w11);
    r11 = 0xFFFFFFFF82000000ull;
    f12 = double(float(std::fma(f0, f12, f11)));
    w0 = L32(base, uint32_t(r11) + 8748); f0 = ToDouble(w0);
    f0 = double(float(f12 * f0));
  }
  w12 = L32(base, uint32_t(r31) + 88); f12 = ToDouble(w12);
  r11 = 0xFFFFFFFF82050000ull;
  f12 = double(float(f0 * f12));
  w0 = L32(base, uint32_t(r11) + -18148); f0 = ToDouble(w0);
  r11 = 0xFFFFFFFF82000000ull;
  f25 = double(float(f12 * f0));
  w0 = L32(base, uint32_t(r11) + 10604); f0 = ToDouble(w0);
  cr6 = CompareF(f25, f0);
  if (cr6.lt) {
    f25 = f0; w25 = w0;
  }
  r11 = 0xFFFFFFFF82000000ull;
  w12 = L32(base, uint32_t(r11) + 8680); f12 = ToDouble(w12);
  f30 = double(float(f25 * f12));
  cr6 = CompareF(f30, f0);
  if (cr6.lt) {
    f30 = f0;
  } else {
    cr6 = CompareF(f30, f13);
    if (cr6.gt) {
      f30 = f13;
    }
  }
  r11 = 0xFFFFFFFF82000000ull;
  w0 = L32(base, uint32_t(r29) + 76); f0 = ToDouble(w0);
  w12 = L32(base, uint32_t(r11) + 11324); f12 = ToDouble(w12);
  cr6 = CompareF(f0, f12);
  if (!cr6.lt) {
    f12 = double(float(f0 - f12));
    r11 = 0xFFFFFFFF82000000ull;
    f29 = f13;
    f28 = f13;
    f27 = f13;
    w0 = L32(base, uint32_t(r11) + 9580); f0 = ToDouble(w0);
    f31 = double(float(f12 * f0));
  } else {
    cr6 = CompareF(f0, f10);
    if (!cr6.lt) {
      r11 = 0xFFFFFFFF82050000ull;
      f12 = double(float(f0 - f10));
      f28 = f13;
      f27 = f13;
      w31 = L32(base, uint32_t(r11) + -21484); f31 = ToDouble(w31);
      r11 = 0xFFFFFFFF82000000ull;
      w0 = L32(base, uint32_t(r11) + 9580); f0 = ToDouble(w0);
      f29 = double(float(f12 * f0));
    } else {
      r11 = 0xFFFFFFFF82010000ull;
      w12 = L32(base, uint32_t(r11) + -32716); f12 = ToDouble(w12);
      r11 = 0xFFFFFFFF82050000ull;
      cr6 = CompareF(f0, f12);
      w31 = L32(base, uint32_t(r11) + -21484); f31 = ToDouble(w31);
      r11 = 0xFFFFFFFF82000000ull;
      f29 = f31; w29 = w31;
      if (!cr6.lt) {
        f12 = double(float(f0 - f12));
        w0 = L32(base, uint32_t(r11) + 9580); f0 = ToDouble(w0);
        f27 = f13;
        f28 = double(float(f12 * f0));
      } else {
        w13 = L32(base, uint32_t(r11) + 9580); f13 = ToDouble(w13);
        f28 = f31;
        f27 = double(float(f0 * f13));
      }
    }
  }
  r11 = 0xFFFFFFFF82050000ull;
  w0 = L32(base, uint32_t(r11) + -18152); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f30 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r28 = L32(base, uint32_t(r1) + 88);
  r3 = r28;
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  r11 = 0xFFFFFFFF82050000ull;
  r10 = 131072;
  r9 = 131072;
  r10 = r10 | 240;
  r9 = r9 | 252;
  w0 = L32(base, uint32_t(r11) + -18156); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f30 * f0));
  S32(base, uint32_t(r31) + uint32_t(r10), uint32_t(r3));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r11 = r3 - 1;
  r3 = L32(base, uint32_t(r1) + 88);
  S32(base, uint32_t(r31) + uint32_t(r9), uint32_t(r11));
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  r11 = 131072;
  r10 = 131072;
  r11 = r11 | 2868;
  r8 = r3 - 1;
  r10 = r10 | 2880;
  r9 = r1 + 88;
  S32(base, uint32_t(r31) + uint32_t(r11), uint32_t(r3));
  S32(base, uint32_t(r31) + uint32_t(r10), uint32_t(r8));
  r11 = L32(base, uint32_t(r29) + 4);
  ca = uint32_t(r11) <= 0;
  r11 = static_cast<uint64_t>(0) - r11;
  t = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = t;
  r11 = Rlwinm(r11, 0, 0xFFFFFFFFFFFFFFFDull);
  r11 = Rlwinm(r11, 0, 0xFFFFFFFFFFFFFC0Full);
  r11 = r11 + 1020;
  r11 = r11 & 0xFFFFFFFF;
  S64(base, uint32_t(r1) + 120, r11);
  f0 = FromBits(L64(base, uint32_t(r1) + 120));
  f0 = double(int64_t(Bits(f0)));
  f0 = double(float(f0));
  f0 = double(float(f0 * f30));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r9), uint32_t(Bits(f0)));
  r27 = L32(base, uint32_t(r1) + 88);
  r3 = r27;
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  r10 = 131072;
  r11 = r3;
  r10 = r10 | 14224;
  r3 = r28;
  S32(base, uint32_t(r31) + uint32_t(r10), uint32_t(r11));
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  r11 = 0xFFFFFFFF82050000ull;
  r10 = 131072;
  r9 = 131072;
  r10 = r10 | 18360;
  r9 = r9 | 18372;
  w0 = L32(base, uint32_t(r11) + -18160); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f30 * f0));
  S32(base, uint32_t(r31) + uint32_t(r10), uint32_t(r3));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r11 = r3 - 1;
  r3 = L32(base, uint32_t(r1) + 88);
  S32(base, uint32_t(r31) + uint32_t(r9), uint32_t(r11));
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  r11 = r3;
  r10 = 131072;
  r9 = 131072;
  r10 = r10 | 20988;
  r9 = r9 | 21000;
  r8 = r11 - 1;
  r3 = r27;
  S32(base, uint32_t(r31) + uint32_t(r10), uint32_t(r11));
  S32(base, uint32_t(r31) + uint32_t(r9), uint32_t(r8));
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  f26 = double(float(f31 * f30));
  r11 = 131072;
  r10 = r1 + 88;
  r11 = r11 | 32344;
  S32(base, uint32_t(r31) + uint32_t(r11), uint32_t(r3));
  r11 = 0xFFFFFFFF82050000ull;
  w0 = L32(base, uint32_t(r11) + -18164); f0 = ToDouble(w0);
  f0 = double(float(f26 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r10), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r11 = 0xFFFFFFFF82050000ull;
  r15 = r31 + 196608;
  r14 = r31 + 196608;
  r15 = r15 - 29064;
  r14 = r14 - 26992;
  w0 = L32(base, uint32_t(r11) + -18168); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f30 * f0));
  S32(base, uint32_t(r15) + 0, uint32_t(r3));
  S32(base, uint32_t(r1) + 100, uint32_t(r15));
  S32(base, uint32_t(r1) + 104, uint32_t(r14));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  f13 = double(float(f31 * f25));
  r11 = 0xFFFFFFFF82050000ull;
  r10 = r1 + 88;
  S32(base, uint32_t(r14) + 0, uint32_t(r3));
  w0 = L32(base, uint32_t(r11) + -18172); f0 = ToDouble(w0);
  f0 = double(float(f13 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r10), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r11 = 0xFFFFFFFF82050000ull;
  r16 = r31 + 196608;
  r16 = r16 - 24836;
  w0 = L32(base, uint32_t(r11) + -18176); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f26 * f0));
  S32(base, uint32_t(r16) + 0, uint32_t(r3));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r11 = 0xFFFFFFFF82050000ull;
  r18 = r31 + 196608;
  r17 = r31 + 196608;
  r18 = r18 - 8396;
  r17 = r17 - 6324;
  w0 = L32(base, uint32_t(r11) + -18180); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f30 * f0));
  S32(base, uint32_t(r18) + 0, uint32_t(r3));
  S32(base, uint32_t(r1) + 108, uint32_t(r18));
  S32(base, uint32_t(r1) + 112, uint32_t(r17));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  f13 = double(float(f29 * f25));
  r11 = 0xFFFFFFFF82050000ull;
  r10 = r1 + 88;
  S32(base, uint32_t(r17) + 0, uint32_t(r3));
  w0 = L32(base, uint32_t(r11) + -18184); f0 = ToDouble(w0);
  f0 = double(float(f13 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r10), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  f28 = double(float(f28 * f30));
  r11 = 0xFFFFFFFF82050000ull;
  r19 = r31 + 196608;
  r10 = r1 + 88;
  r19 = r19 - 4168;
  w0 = L32(base, uint32_t(r11) + -18188); f0 = ToDouble(w0);
  S32(base, uint32_t(r19) + 0, uint32_t(r3));
  f0 = double(float(f28 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r10), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  f31 = double(float(f29 * f30));
  r11 = 0xFFFFFFFF82050000ull;
  r20 = r31 + 196608;
  r10 = r1 + 88;
  r20 = r20 + 12272;
  w0 = L32(base, uint32_t(r11) + -18192); f0 = ToDouble(w0);
  S32(base, uint32_t(r20) + 0, uint32_t(r3));
  f0 = double(float(f31 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r10), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r11 = 0xFFFFFFFF82050000ull;
  r22 = r31 + 196608;
  r10 = r3 - 1;
  r22 = r22 + 20488;
  r21 = r31 + 196608;
  w0 = L32(base, uint32_t(r11) + -18196); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f30 * f0));
  r21 = r21 + 28708;
  S32(base, uint32_t(r22) + 0, uint32_t(r10));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  r11 = 0xFFFFFFFF82050000ull;
  S32(base, uint32_t(r21) + 0, uint32_t(r3));
  w0 = L32(base, uint32_t(r11) + -18200); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f26 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  f13 = double(float(f27 * f25));
  r11 = 0xFFFFFFFF82050000ull;
  r23 = r31 + 262144;
  r10 = r1 + 88;
  r23 = r23 - 28612;
  w0 = L32(base, uint32_t(r11) + -18204); f0 = ToDouble(w0);
  S32(base, uint32_t(r23) + 0, uint32_t(r3));
  S32(base, uint32_t(r1) + 116, uint32_t(r23));
  f0 = double(float(f13 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r10), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r11 = 0xFFFFFFFF82050000ull;
  r24 = r31 + 262144;
  r24 = r24 - 24492;
  w0 = L32(base, uint32_t(r11) + -18208); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f28 * f0));
  S32(base, uint32_t(r24) + 0, uint32_t(r3));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r11 = 0xFFFFFFFF82050000ull;
  r25 = r31 + 262144;
  r25 = r25 - 8048;
  w0 = L32(base, uint32_t(r11) + -18212); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f31 * f0));
  S32(base, uint32_t(r25) + 0, uint32_t(r3));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r11 = 0xFFFFFFFF82050000ull;
  r26 = r31 + 262144;
  r26 = r26 + 168;
  w0 = L32(base, uint32_t(r11) + -18216); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f26 * f0));
  S32(base, uint32_t(r26) + 0, uint32_t(r3));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r11 = 0xFFFFFFFF82050000ull;
  r28 = r31 + 262144;
  r27 = r31 + 262144;
  r28 = r28 + 8388;
  r27 = r27 + 16604;
  w0 = L32(base, uint32_t(r11) + -18220); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f26 * f0));
  S32(base, uint32_t(r28) + 0, uint32_t(r3));
  S32(base, uint32_t(r1) + 120, uint32_t(r27));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  r11 = 0xFFFFFFFF82050000ull;
  S32(base, uint32_t(r27) + 0, uint32_t(r3));
  w0 = L32(base, uint32_t(r11) + -18224); f0 = ToDouble(w0);
  r11 = r1 + 88;
  f0 = double(float(f25 * f0));
  f0 = Fctidz(f0);
  S32(base, uint32_t(r11), uint32_t(Bits(f0)));
  r3 = L32(base, uint32_t(r1) + 88);
  r3 = LargestPrimeBelow(base, uint32_t(r3));
  cr6 = Compare<uint32_t>(uint32_t(r3), 0);
  if (cr6.eq) {
    r3 = 1;
  }
  r9 = 262144;
  r11 = L32(base, uint32_t(r28) + 0);
  r10 = L32(base, uint32_t(r27) + 0);
  r7 = 0xFFFFFFFF82010000ull;
  r9 = r9 | 20724;
  r8 = L32(base, uint32_t(r25) + 0);
  r11 = r10 + r11;
  r6 = L32(base, uint32_t(r23) + 0);
  r5 = L32(base, uint32_t(r21) + 0);
  w0 = L32(base, uint32_t(r31) + 88); f0 = ToDouble(w0);
  r4 = L32(base, uint32_t(r22) + 0);
  f30 = FromBits(L64(base, uint32_t(r7) + -2264));
  r7 = L32(base, uint32_t(r24) + 0);
  S32(base, uint32_t(r1) + 88, uint32_t(r9));
  f1 = f30;
  r9 = L32(base, uint32_t(r26) + 0);
  r28 = L32(base, uint32_t(r20) + 0);
  r11 = r11 + r9;
  r27 = L32(base, uint32_t(r19) + 0);
  r26 = L32(base, uint32_t(r17) + 0);
  r11 = r11 + r8;
  r25 = L32(base, uint32_t(r18) + 0);
  r24 = L32(base, uint32_t(r16) + 0);
  r11 = r11 + r7;
  r23 = L32(base, uint32_t(r14) + 0);
  r22 = L32(base, uint32_t(r15) + 0);
  r11 = r11 + r6;
  r21 = L32(base, uint32_t(r1) + 88);
  r11 = r11 + r5;
  r11 = r11 + r4;
  S32(base, uint32_t(r31) + uint32_t(r21), uint32_t(r3));
  w13 = L32(base, uint32_t(r29) + 72); f13 = ToDouble(w13);
  r11 = r11 + r28;
  r11 = r11 + r27;
  r11 = r11 + r26;
  r11 = r11 + r25;
  r11 = r11 + r24;
  r11 = r11 + r23;
  r11 = r11 + r22;
  r11 = r11 + r3;
  r11 = uint64_t(uint32_t(r11) >> 1);
  r11 = r11 & 0xFFFFFFFF;
  S64(base, uint32_t(r1) + 88, r11);
  f12 = FromBits(L64(base, uint32_t(r1) + 88));
  f12 = double(int64_t(Bits(f12)));
  r11 = 0xFFFFFFFF82050000ull;
  f12 = double(float(f12));
  f0 = double(float(f12 / f0));
  f0 = f13 / f0;
  f13 = FromBits(L64(base, uint32_t(r11) + -18232));
  f2 = f13 / f0;
  f1 = c_runtime::Pow(f1, f2);
  r8 = 0xFFFFFFFF82000000ull;
  r10 = L32(base, uint32_t(r29) + 16);
  r11 = 0xFFFFFFFF82010000ull;
  r9 = L32(base, uint32_t(r29) + 12);
  r7 = r30 + 888;
  r6 = r30 + 192;
  r28 = r30 + 376;
  f13 = FromBits(L64(base, uint32_t(r8) + 8888));
  r8 = 0xFFFFFFFF82000000ull;
  f13 = f1 * f13;
  w0 = L32(base, uint32_t(r11) + -30440); f0 = ToDouble(w0);
  r11 = uint64_t(uint32_t(r10) << 2);
  r10 = L32(base, uint32_t(r29) + 8);
  r9 = uint64_t(uint32_t(r9) << 2);
  r10 = uint64_t(uint32_t(r10) << 2);
  r5 = r30 + 504;
  r4 = r30 + 632;
  w28 = L32(base, uint32_t(r11) + uint32_t(r7)); f28 = ToDouble(w28);
  r3 = r30 + 760;
  w9 = L32(base, uint32_t(r11) + uint32_t(r6)); f9 = ToDouble(w9);
  w8 = L32(base, uint32_t(r11) + uint32_t(r28)); f8 = ToDouble(w8);
  r11 = 0xFFFFFFFF82040000ull;
  w25 = L32(base, uint32_t(r10) + uint32_t(r5)); f25 = ToDouble(w25);
  f11 = double(float(sqrt(f13)));
  w13 = L32(base, uint32_t(r8) + 7140); f13 = ToDouble(w13);
  f10 = double(float(f11 * f13));
  w24 = L32(base, uint32_t(r9) + uint32_t(r4)); f24 = ToDouble(w24);
  w23 = L32(base, uint32_t(r10) + uint32_t(r3)); f23 = ToDouble(w23);
  w13 = L32(base, uint32_t(r11) + -11004); f13 = ToDouble(w13);
  r11 = 0xFFFFFFFF82000000ull;
  r26 = L32(base, uint32_t(r29) + 24);
  r23 = r30 + 632;
  r25 = L32(base, uint32_t(r29) + 28);
  r24 = r30 + 504;
  r8 = L32(base, uint32_t(r29) + 32);
  r22 = r30 + 760;
  r7 = L32(base, uint32_t(r29) + 40);
  r15 = r30 + 3144;
  r4 = L32(base, uint32_t(r29) + 36);
  w12 = L32(base, uint32_t(r11) + 10796); f12 = ToDouble(w12);
  r11 = L32(base, uint32_t(r29) + 20);
  w19 = L32(base, uint32_t(r10) + uint32_t(r23)); f19 = ToDouble(w19);
  r10 = uint64_t(uint32_t(r26) << 2);
  f12 = double(float(-std::fma(f11, f0, -f12)));
  w20 = L32(base, uint32_t(r9) + uint32_t(r24)); f20 = ToDouble(w20);
  w18 = L32(base, uint32_t(r9) + uint32_t(r22)); f18 = ToDouble(w18);
  r9 = uint64_t(uint32_t(r25) << 2);
  r14 = r30 + 3144;
  r6 = L32(base, uint32_t(r29) + 44);
  r11 = uint64_t(uint32_t(r11) << 2);
  r5 = L32(base, uint32_t(r29) + 0);
  w0 = L32(base, uint32_t(r10) + uint32_t(r15)); f0 = ToDouble(w0);
  r10 = uint64_t(uint32_t(r8) << 2);
  r3 = r30 + 376;
  f22 = double(float(f28 * f13));
  r10 = r8 + r10;
  f7 = FromBits(Bits(f10) ^ 0x8000000000000000);
  r8 = Rlwinm(r7, 4, 0xFFFFFFF0ull);
  w31 = L32(base, uint32_t(r9) + uint32_t(r14)); f31 = ToDouble(w31);
  r28 = r30 + 888;
  r27 = r30 + 192;
  f21 = double(float(f9 * f12));
  w9 = L32(base, uint32_t(r11) + uint32_t(r3)); f9 = ToDouble(w9);
  r9 = Rlwinm(r10, 1, 0xFFFFFFFEull);
  f26 = double(float(f9 * f12));
  r10 = r8 - r7;
  f29 = double(float(f8 * f12));
  r9 = r9 + r4;
  w27 = L32(base, uint32_t(r11) + uint32_t(r28)); f27 = ToDouble(w27);
  r8 = r10 + r6;
  w9 = L32(base, uint32_t(r11) + uint32_t(r27)); f9 = ToDouble(w9);
  r11 = uint64_t(uint32_t(r9) << 2);
  f16 = double(float(f9 * f12));
  r10 = uint64_t(uint32_t(r6) << 2);
  f15 = double(float(f27 * f13));
  r9 = uint64_t(uint32_t(r8) << 2);
  r21 = r30 + 1016;
  r20 = r30 + 316;
  r19 = r30 + 1536;
  r18 = r30 + 2056;
  f14 = double(float(f26 * f13));
  r17 = r30 + 2600;
  f17 = double(float(f29 * f13));
  r16 = r30 + 132;
  w6 = L32(base, uint32_t(r11) + uint32_t(r21)); f6 = ToDouble(w6);
  w5 = L32(base, uint32_t(r10) + uint32_t(r20)); f5 = ToDouble(w5);
  cr6 = Compare<uint32_t>(uint32_t(r5), 0);
  w4 = L32(base, uint32_t(r11) + uint32_t(r19)); f4 = ToDouble(w4);
  w3 = L32(base, uint32_t(r9) + uint32_t(r18)); f3 = ToDouble(w3);
  w2 = L32(base, uint32_t(r9) + uint32_t(r17)); f2 = ToDouble(w2);
  w1 = L32(base, uint32_t(r10) + uint32_t(r16)); f1 = ToDouble(w1);
  if (cr6.eq) {
    r10 = 1;
  } else {
    r11 = r5 & 0xFFFFFFFF;
    w13 = L32(base, uint32_t(r31) + 88); f13 = ToDouble(w13);
    r10 = r1 + 88;
    S64(base, uint32_t(r1) + 88, r11);
    f12 = FromBits(L64(base, uint32_t(r1) + 88));
    f12 = double(int64_t(Bits(f12)));
    f12 = double(float(f12));
    f13 = double(float(f12 * f13));
    w12 = L32(base, uint32_t(r1) + 96); f12 = ToDouble(w12);
    f13 = double(float(f13 * f12));
    f13 = Fctidz(f13);
    S32(base, uint32_t(r10), uint32_t(Bits(f13)));
    r10 = L32(base, uint32_t(r1) + 88);
  }
  r11 = r31 + 104;
  r9 = L32(base, uint32_t(r11) + 12);
  r8 = L32(base, uint32_t(r11) + 8);
  S32(base, uint32_t(r11) + 16, uint32_t(r10));
  cr6 = Compare<uint32_t>(uint32_t(r9), uint32_t(r8));
  if (cr6.eq) {
    w13 = L32(base, uint32_t(r1) + 80); f13 = ToDouble(w13);
    S32(base, uint32_t(r11) + 8, uint32_t(r10));
    S32(base, uint32_t(r11) + 4, w13);
  }
  r11 = r31 + 65536;
  r11 = r11 + 136;
  r9 = L32(base, uint32_t(r11) + 12);
  r8 = L32(base, uint32_t(r11) + 8);
  S32(base, uint32_t(r11) + 16, uint32_t(r10));
  cr6 = Compare<uint32_t>(uint32_t(r9), uint32_t(r8));
  if (cr6.eq) {
    w13 = L32(base, uint32_t(r1) + 80); f13 = ToDouble(w13);
    S32(base, uint32_t(r11) + 8, uint32_t(r10));
    S32(base, uint32_t(r11) + 4, w13);
  }
  r6 = 131072;
  w12 = L32(base, uint32_t(r30) + 0); f12 = ToDouble(w12);
  r5 = 131072;
  w9 = L32(base, uint32_t(r30) + 4); f9 = ToDouble(w9);
  r6 = r6 | 244;
  f13 = FromBits(Bits(f0) ^ 0x8000000000000000);
  r5 = r5 | 256;
  w8 = L32(base, uint32_t(r30) + 20); f8 = ToDouble(w8);
  r4 = 131072;
  r11 = r31 + 131072;
  r4 = r4 | 268;
  S32(base, uint32_t(r31) + uint32_t(r6), w12);
  r6 = 131072;
  S32(base, uint32_t(r31) + uint32_t(r5), w9);
  r3 = 131072;
  r26 = r6 | 14228;
  r6 = 131072;
  SF(base, uint32_t(r31) + uint32_t(r4), f25);
  r28 = 131072;
  r5 = r6 | 14240;
  r6 = 131072;
  r27 = 131072;
  r4 = r6 | 232;
  r6 = 131072;
  r10 = r31 + 131072;
  r25 = r6 | 236;
  r6 = 131072;
  r11 = r11 + 2332;
  r24 = r6 | 18364;
  r6 = 131072;
  r9 = r31 + 131072;
  r23 = r6 | 18376;
  r6 = 131072;
  SF(base, uint32_t(r11) + 8, f0);
  r3 = r3 | 2872;
  SF(base, uint32_t(r11) + 4, f13);
  r22 = r6 | 18388;
  r6 = 131072;
  r28 = r28 | 2884;
  r21 = r6 | 20992;
  r6 = 131072;
  S32(base, uint32_t(r31) + uint32_t(r3), w12);
  r27 = r27 | 2896;
  r20 = r6 | 21004;
  r6 = 131072;
  S32(base, uint32_t(r31) + uint32_t(r28), w8);
  r10 = r10 + 11104;
  r8 = r31 + 131072;
  r9 = r9 + 12152;
  SF(base, uint32_t(r31) + uint32_t(r27), f22);
  r19 = r6 | 21016;
  r7 = r31 + 131072;
  r8 = r8 + 20452;
  SF(base, uint32_t(r10) + 8, f0);
  r6 = r31 + 131072;
  SF(base, uint32_t(r10) + 4, f13);
  r7 = r7 + 29224;
  SF(base, uint32_t(r9) + 8, f0);
  r6 = r6 + 30272;
  SF(base, uint32_t(r9) + 4, f13);
  S32(base, uint32_t(r31) + uint32_t(r24), w12);
  r11 = 131072;
  SF(base, uint32_t(r31) + uint32_t(r26), f21);
  r10 = 131072;
  SF(base, uint32_t(r31) + uint32_t(r5), f29);
  r11 = r11 | 32348;
  SF(base, uint32_t(r31) + uint32_t(r4), f24);
  r10 = r10 | 32360;
  SF(base, uint32_t(r31) + uint32_t(r25), f23);
  S32(base, uint32_t(r31) + uint32_t(r23), w9);
  SF(base, uint32_t(r31) + uint32_t(r22), f20);
  SF(base, uint32_t(r8) + 8, f0);
  SF(base, uint32_t(r8) + 4, f13);
  S32(base, uint32_t(r31) + uint32_t(r21), w12);
  f12 = FromBits(Bits(f31) ^ 0x8000000000000000);
  S32(base, uint32_t(r31) + uint32_t(r20), w8);
  SF(base, uint32_t(r31) + uint32_t(r19), f15);
  SF(base, uint32_t(r7) + 8, f0);
  SF(base, uint32_t(r7) + 4, f13);
  SF(base, uint32_t(r6) + 8, f0);
  SF(base, uint32_t(r6) + 4, f13);
  SF(base, uint32_t(r31) + uint32_t(r11), f16);
  r11 = 131072;
  r9 = 131072;
  SF(base, uint32_t(r31) + uint32_t(r10), f26);
  r5 = r11 | 40648;
  r9 = r9 | 18352;
  r11 = 131072;
  r8 = 131072;
  r10 = r11 | 40652;
  r8 = r8 | 18356;
  r11 = 131072;
  SF(base, uint32_t(r31) + uint32_t(r9), f19);
  r7 = 131072;
  r9 = r11 | 40680;
  r11 = 131072;
  SF(base, uint32_t(r31) + uint32_t(r8), f18);
  r6 = 131072;
  r8 = r11 | 40684;
  r11 = 131072;
  r7 = r7 | 40620;
  r4 = r11 | 57120;
  r11 = 131072;
  r6 = r6 | 40644;
  r3 = r11 | 61288;
  r11 = 131072;
  r28 = r11 | 61312;
  r11 = 131072;
  r27 = r11 | 61316;
  r11 = 131072;
  r26 = r11 | 61320;
  r11 = 131072;
  r25 = r11 | 61348;
  r11 = 131072;
  r24 = r11 | 61352;
  r11 = 196608;
  r23 = r11 | 12252;
  r11 = L32(base, uint32_t(r1) + 100);
  SF(base, uint32_t(r11) + 8, f31);
  SF(base, uint32_t(r11) + 4, f12);
  r11 = L32(base, uint32_t(r1) + 104);
  SF(base, uint32_t(r11) + 8, f31);
  SF(base, uint32_t(r11) + 4, f12);
  r11 = L32(base, uint32_t(r1) + 108);
  SF(base, uint32_t(r31) + uint32_t(r8), f1);
  r8 = 262144;
  SF(base, uint32_t(r31) + uint32_t(r10), f3);
  r10 = 196608;
  SF(base, uint32_t(r31) + uint32_t(r9), f2);
  r9 = 262144;
  SF(base, uint32_t(r31) + uint32_t(r7), f6);
  r10 = r10 | 57468;
  SF(base, uint32_t(r31) + uint32_t(r6), f5);
  r9 = r9 | 37148;
  SF(base, uint32_t(r31) + uint32_t(r5), f4);
  r8 = r8 | 37152;
  SF(base, uint32_t(r31) + uint32_t(r4), f11);
  SF(base, uint32_t(r11) + 8, f31);
  SF(base, uint32_t(r11) + 4, f12);
  r11 = L32(base, uint32_t(r1) + 112);
  SF(base, uint32_t(r11) + 8, f31);
  SF(base, uint32_t(r11) + 4, f12);
  r11 = L32(base, uint32_t(r1) + 116);
  SF(base, uint32_t(r31) + uint32_t(r24), f1);
  f1 = f30;
  SF(base, uint32_t(r31) + uint32_t(r3), f6);
  SF(base, uint32_t(r31) + uint32_t(r28), f5);
  SF(base, uint32_t(r31) + uint32_t(r27), f4);
  SF(base, uint32_t(r31) + uint32_t(r26), f3);
  SF(base, uint32_t(r31) + uint32_t(r25), f2);
  SF(base, uint32_t(r31) + uint32_t(r23), f11);
  SF(base, uint32_t(r11) + 8, f31);
  SF(base, uint32_t(r11) + 4, f12);
  r11 = 196608;
  SF(base, uint32_t(r31) + uint32_t(r10), f10);
  r11 = r11 | 57472;
  SF(base, uint32_t(r31) + uint32_t(r11), f10);
  r11 = L32(base, uint32_t(r1) + 120);
  SF(base, uint32_t(r11) + 8, f31);
  SF(base, uint32_t(r11) + 4, f12);
  SF(base, uint32_t(r31) + uint32_t(r9), f7);
  SF(base, uint32_t(r31) + uint32_t(r8), f7);
  r11 = 0xFFFFFFFF82050000ull;
  w0 = L32(base, uint32_t(r29) + 68); f0 = ToDouble(w0);
  f29 = FromBits(L64(base, uint32_t(r11) + -18240));
  f2 = f0 * f29;
  f1 = c_runtime::Pow(f1, f2);
  f0 = f1;
  r11 = 131072;
  f1 = f30;
  r11 = r11 | 40716;
  f0 = double(float(f0));
  f0 = double(float(f0 * f28));
  SF(base, uint32_t(r31) + uint32_t(r11), f0);
  w0 = L32(base, uint32_t(r29) + 68); f0 = ToDouble(w0);
  f2 = f0 * f29;
  f1 = c_runtime::Pow(f1, f2);
  f0 = f1;
  r11 = 131072;
  f1 = f30;
  r11 = r11 | 61384;
  f0 = double(float(f0));
  f0 = double(float(f0 * f27));
  SF(base, uint32_t(r31) + uint32_t(r11), f0);
  w0 = L32(base, uint32_t(r29) + 68); f0 = ToDouble(w0);
  f2 = f0 * f29;
  f1 = c_runtime::Pow(f1, f2);
  f0 = f1;
  r11 = 196608;
  f1 = f30;
  r11 = r11 | 41060;
  f0 = double(float(f0));
  f0 = double(float(f0 * f17));
  SF(base, uint32_t(r31) + uint32_t(r11), f0);
  w0 = L32(base, uint32_t(r29) + 68); f0 = ToDouble(w0);
  f2 = f0 * f29;
  f1 = c_runtime::Pow(f1, f2);
  f0 = f1;
  r11 = 262144;
  f1 = f30;
  r11 = r11 | 20740;
  f0 = double(float(f0));
  f0 = double(float(f0 * f14));
  SF(base, uint32_t(r31) + uint32_t(r11), f0);
  w0 = L32(base, uint32_t(r29) + 64); f0 = ToDouble(w0);
  f2 = f0 * f29;
  f1 = c_runtime::Pow(f1, f2);
  w0 = L32(base, uint32_t(r30) + 20); f0 = ToDouble(w0);
  r11 = r31 + 327680;
  f0 = double(float(f31 * f0));
  r10 = r31 + 327680;
  r11 = r11 - 28368;
  f12 = double(float(f1));
  SF(base, uint32_t(r31) + 96, f12);
  r9 = r31 + 327680;
  r10 = r10 - 26296;
  r9 = r9 - 24224;
  r30 = r31 + 131072;
  SF(base, uint32_t(r11) + 8, f0);
  r30 = r30 + 168;
  r3 = r30;
  f13 = FromBits(Bits(f0) ^ 0x8000000000000000);
  SF(base, uint32_t(r11) + 4, f13);
  r11 = r31 + 327680;
  SF(base, uint32_t(r10) + 8, f0);
  SF(base, uint32_t(r10) + 4, f13);
  r11 = r11 - 22152;
  SF(base, uint32_t(r9) + 8, f0);
  SF(base, uint32_t(r9) + 4, f13);
  SF(base, uint32_t(r11) + 8, f0);
  SF(base, uint32_t(r11) + 4, f13);
  w0 = L32(base, uint32_t(r29) + 52); f0 = ToDouble(w0);
  S32(base, uint32_t(r30) + 4, w0);
  FilterCoefficient(base, uint32_t(r3));
  r3 = r30;
  w0 = L32(base, uint32_t(r29) + 56); f0 = ToDouble(w0);
  S32(base, uint32_t(r30) + 8, w0);
  FilterCoefficient(base, uint32_t(r3));
  r3 = r30;
  w0 = L32(base, uint32_t(r29) + 60); f0 = ToDouble(w0);
  S32(base, uint32_t(r30) + 12, w0);
  FilterCoefficient(base, uint32_t(r3));
  w0 = L32(base, uint32_t(r29) + 52); f0 = ToDouble(w0);
  r30 = r31 + 131072;
  r30 = r30 + 200;
  r3 = r30;
  S32(base, uint32_t(r30) + 4, w0);
  FilterCoefficient(base, uint32_t(r3));
  r3 = r30;
  w0 = L32(base, uint32_t(r29) + 56); f0 = ToDouble(w0);
  S32(base, uint32_t(r30) + 8, w0);
  FilterCoefficient(base, uint32_t(r3));
  r3 = r30;
  w0 = L32(base, uint32_t(r29) + 60); f0 = ToDouble(w0);
  S32(base, uint32_t(r30) + 12, w0);
  FilterCoefficient(base, uint32_t(r3));
  r11 = L32(base, uint32_t(r29) + 48);
  cr6 = Compare<uint32_t>(uint32_t(r11), 0);
  if (cr6.eq) {
    r10 = 1;
  } else {
    r11 = r11 & 0xFFFFFFFF;
    w0 = L32(base, uint32_t(r31) + 88); f0 = ToDouble(w0);
    r10 = r1 + 120;
    S64(base, uint32_t(r1) + 120, r11);
    f13 = FromBits(L64(base, uint32_t(r1) + 120));
    f13 = double(int64_t(Bits(f13)));
    f13 = double(float(f13));
    f0 = double(float(f13 * f0));
    w13 = L32(base, uint32_t(r1) + 96); f13 = ToDouble(w13);
    f0 = double(float(f0 * f13));
    f0 = Fctidz(f0);
    S32(base, uint32_t(r10), uint32_t(Bits(f0)));
    r10 = L32(base, uint32_t(r1) + 120);
  }
  r11 = r31 + 327680;
  r11 = r11 - 20080;
  r9 = L32(base, uint32_t(r11) + 12);
  r8 = L32(base, uint32_t(r11) + 8);
  S32(base, uint32_t(r11) + 16, uint32_t(r10));
  cr6 = Compare<uint32_t>(uint32_t(r9), uint32_t(r8));
  if (cr6.eq) {
    w0 = L32(base, uint32_t(r1) + 80); f0 = ToDouble(w0);
    S32(base, uint32_t(r11) + 8, uint32_t(r10));
    S32(base, uint32_t(r11) + 4, w0);
  }
  r11 = r31 + 327680;
  r11 = r11 - 19024;
  r9 = L32(base, uint32_t(r11) + 12);
  r8 = L32(base, uint32_t(r11) + 8);
  S32(base, uint32_t(r11) + 16, uint32_t(r10));
  cr6 = Compare<uint32_t>(uint32_t(r9), uint32_t(r8));
  if (cr6.eq) {
    w0 = L32(base, uint32_t(r1) + 80); f0 = ToDouble(w0);
    S32(base, uint32_t(r11) + 8, uint32_t(r10));
    S32(base, uint32_t(r11) + 4, w0);
  }
  return r3;
}

}  // namespace xaudio
