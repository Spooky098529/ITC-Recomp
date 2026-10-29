// Stages of XAudio's reverb core as plain C++ (B7 in docs/rexglue-removal.md;
// docs/functions/xaudio-objects.md): the leaves ReverbCore::Process
// (sub_82463B90, still recompiled) runs over its 256-frame blocks. Each works
// on its stage's guest state and the core's float buffers, in the original's
// order of loads, stores and roundings (the scalar unit's, flush to zero
// off; a float stored straight from its load keeps its bits, as the
// translated code's folded lfs/stfs pairs do). Built with strict floating
// point and no contraction (CMakeLists.txt). Adapters:
// src/hooks/xaudio_objects.cpp (behind --native_xaudio_objects).

#pragma once

#include <cstdint>

namespace xaudio {

// sub_82462BC8: a one-pole low-pass over `count` frames from `in` to `out`
// (out[0] the last output kept at +20, then a frame each; four at a time,
// then one at a time): the state +16 and +20, the gains from +24 and +28
// (1 - +28, the float at 0x82000A30).
void ReverbLowPass(uint8_t* base, uint32_t state, uint32_t in, uint32_t out, uint32_t count);

// sub_82462CF8 (512 entries) and sub_82463060 (2,048 entries): a delay line
// at +44 written at +36 (a block of 256 from `in`, the index moved on by 256
// and wrapped), read at three taps (+36 less +0, +12 and +24): `out_a` gets
// the first tap x +16 plus the second x +4 (`fmadds`), `out_b` the third x
// +28, each after the last block's (+8 and +32).
void ReverbDelay512(uint8_t* base, uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b);
void ReverbDelay2048(uint8_t* base, uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b);

// sub_82463578: a 1,024-entry delay line at +32 written at +24, two taps
// (+24 less +0 and +12) x +4 and x +16 to `out_a` and `out_b`, each after the
// last block's (+8 and +20).
void ReverbDelay1024(uint8_t* base, uint32_t line, uint32_t in, uint32_t out_a, uint32_t out_b);

}  // namespace xaudio
