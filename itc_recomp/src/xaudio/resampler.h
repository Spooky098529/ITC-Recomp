// XAudio's sample rate converter as plain C++ (B7 in docs/rexglue-removal.md;
// docs/functions/xaudio-objects.md, "The resampler"): Resampler::Process,
// which picks a kernel from the table at 0x8204CF08 and calls it, and the six
// kernels that ran in play (16-bit samples, mono and stereo: any ratio with a
// vector path and a scalar fallback, and equal rates for stereo). The 96-byte
// converter state is guest memory shared with the recompiled codecs (+0 the
// input, +4 its end in frames, +8 the position, +12 the format, +13 the
// channels, +16 the input rate, +20 the output (planar: a channel every
// 1,024 bytes), +24 its frames, +28 those written, +32 the output rate, +36
// and +40 the volume ramp's start and end, +44 the ratio, +48 the position's
// fraction, +52 and +56 the last input frame, +76 the kernel, +80 changed
// bits, +84 flags). Translated as the game's code, in the original's order
// of loads, stores and float operations (the vector unit's with flush to
// zero, the scalar unit's without); the stack words the original leaves are
// written too (`sp` is the stack pointer the original is called with).
// Built with strict floating point and no contraction (CMakeLists.txt).
// Adapters: src/hooks/xaudio_objects.cpp (behind --native_xaudio_objects).

#pragma once

#include "xaudio/voices.h"

#include <cstdint>

namespace xaudio {

constexpr uint32_t kResamplerKernels = 0x8204CF08;

// Resampler::Process (sub_8246F140): nothing (0) when the input or the
// output is used up; when +80's low two bits are set, the ratio (+44, the
// rates' quotient as floats) and the kernel (+76) picked again, and with
// +80 set it's cleared; the kernel called (ours for the
// six below, else the translated one through `call` at `frame`, the
// original's 128-byte frame); returns the frames of input used, in full.
uint64_t ResamplerProcess(uint8_t* base, uint32_t state, uint32_t frame, const GuestCall& call);

// The kernels (each takes the state in r3; returns r3 as it leaves it).
// sub_8247E188 (entry 81: any ratio, 16-bit, mono) and sub_8247E260 (entry
// 82: stereo): the vector path (sub_8246E208, sub_8246E4C8) when the output
// is 16-byte aligned, its frames a multiple of 8 and the input lasts, else
// the scalar one (sub_82473E48, sub_82474188: entries 6 and 7).
uint64_t ResampleMono(uint8_t* base, uint32_t state, uint32_t sp);
uint64_t ResampleStereo(uint8_t* base, uint32_t state, uint32_t sp);
uint64_t ResampleMonoScalar(uint8_t* base, uint32_t state, uint32_t sp);
uint64_t ResampleStereoScalar(uint8_t* base, uint32_t state, uint32_t sp);
// sub_8247F558 (entry 107: equal rates, 16-bit, stereo): a vector copy with
// the volume ramp when aligned, else sub_82478E60 (entry 32).
uint64_t CopyStereo(uint8_t* base, uint32_t state, uint32_t sp);
uint64_t CopyStereoScalar(uint8_t* base, uint32_t state, uint32_t sp);

}  // namespace xaudio
