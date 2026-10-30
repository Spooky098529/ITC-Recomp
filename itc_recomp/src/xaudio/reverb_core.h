// XAudio's reverb core's signal path as plain C++ (B7 in
// docs/rexglue-removal.md; docs/functions/xaudio-objects.md): ReverbCore::Process
// (sub_82463B90), over the stages in reverb_stages.h. Adapter:
// src/hooks/xaudio_objects.cpp (behind --native_xaudio_objects).

#pragma once

#include "xaudio/voices.h"

#include <cstdint>

namespace xaudio {

// sub_82463B90: one block of 256 frames through the reverb. `core` the core
// (all 64 bits, as the first stage takes them), `inputs` the two input
// channels' pointers (+100 picks them: 1 the first twice, 2 the second twice,
// else both), `outputs` the output channels' (+0, +4, +12 and +16 written;
// +8 not read). `sp` the caller's stack pointer: the frame is the
// 17,888 bytes below it, at the same addresses as the original's, and holds
// the stages' buffers and the words the original leaves there (its register
// saves aside); the caller's word at `sp` + 20 gets the core. Per input
// channel, the early chain (the crossfading delay line, the low-pass, the
// 512-entry delay line, the 128-entry all-pass, the 2,048-entry delay line,
// the 256-entry all-pass, the 512-entry all-pass to a buffer, the 1,024-entry
// delay line), then the two chains mixed by the gains at +0x247B0 to +0x247B4
// and +0x200E8 to +0x200EC (x +96), a frame-at-a-time network over the core's
// filters and delay lines, four 512-entry all-passes, two crossfading
// 256-entry delay lines, and the four outputs from the stages' buffers and
// the inputs, weighted by the gain at +92 (vector work, 256 frames each). Returns r3 as the original leaves it.
uint64_t ReverbCoreProcess(uint8_t* base, uint64_t core, uint32_t inputs, uint32_t outputs,
                           uint32_t sp);

// The core's set-up (reverb_setup.cpp).

// sub_82467A18: the 84-byte native parameters at `params` applied to the
// core at `core`: copied to +4 (memcpy; its original, through `call`, where
// the plain C++ refuses, as for Reverb::Run's +52), then the working values
// from them: the delay lengths (the parameters times the rate at +88 and the
// tables at 0x8204B8C0 to 0x8204B91C, fctidz, rounded down to primes by
// LargestPrimeBelow, clamped) to the lines' states, the gains (five pows)
// and the filters' inputs, with six FilterCoefficients. `frame` is its
// frame (432 bytes; the words the original writes there are written, its
// register saves aren't). Returns r3 as the original leaves it (the second
// low-pass, +0x200C8).
uint64_t ReverbCoreApply(uint8_t* base, uint32_t core, uint32_t params, uint32_t frame,
                         const GuestCall& call);

}  // namespace xaudio
