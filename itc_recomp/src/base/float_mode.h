// The host's float mode (x86-64's MXCSR), which our code switches where the
// original sets the 360's: flush-to-zero for vector work, off for scalar
// work, and the rounding mode. In place of the SDK's FPSCRPlatform
// (docs/rexglue-removal.md, "E4: the plan", step 2), with the same masks.

#pragma once

#include <xmmintrin.h>

#include <cstdint>

namespace base {

struct FloatMode {
  // MXCSR's rounding control (bits 13-14).
  static constexpr uint32_t RoundMaskVal = 0x6000;
  // Flush-to-zero (bit 15) and denormals-are-zero (bit 6).
  static constexpr uint32_t FlushMask = 0x8000 | 0x0040;
  // The bits the game's float mode sets: rounding and flushing (the
  // exception masks stay as the host has them).
  static constexpr uint32_t GuestMask = RoundMaskVal | FlushMask;

  static uint32_t getcsr() noexcept { return _mm_getcsr(); }
  static void setcsr(uint32_t csr) noexcept { _mm_setcsr(csr); }

  // The rounding mode as the 360's FPSCR numbers it (0 nearest, 1 toward
  // zero, 2 up, 3 down), from the host's, and back.
  static uint32_t Rounding() noexcept {
    static constexpr uint32_t kHostToGuest[] = {0, 3, 2, 1};
    return kHostToGuest[(getcsr() & RoundMaskVal) >> 13];
  }
  static void SetRounding(uint32_t value) noexcept {
    static constexpr uint32_t kGuestToHost[] = {0x0000, 0x6000, 0x4000, 0x2000};
    setcsr((getcsr() & ~RoundMaskVal) | kGuestToHost[value & 3]);
  }

  // Flush-to-zero off (the scalar mode), written only when it's on.
  static void DisableFlush() noexcept {
    const uint32_t csr = getcsr();
    if ((csr & FlushMask) != 0) {
      setcsr(csr & ~FlushMask);
    }
  }

  // Puts back the game's bits of a mode read earlier, the rest as now.
  static void restoreGuestBits(uint32_t saved) noexcept {
    setcsr((getcsr() & ~GuestMask) | (saved & GuestMask));
  }
};

}  // namespace base
