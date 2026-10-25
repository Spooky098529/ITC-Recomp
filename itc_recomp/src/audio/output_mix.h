// The game's mixed frames converted for SDL (A6 in docs/rexglue-removal.md):
// XAudio mixes 5.1 (front left, front right, center, LFE, back left, back
// right) as six runs of big-endian floats, one channel after another. SDL
// takes interleaved native floats: all six channels on a 5.1 device, or a
// stereo fold. The numbers are the ones the SDK's driver used (it never
// changed its defaults): the fold puts center and back channels at -3 dB
// into both sides, drops the LFE and scales by 1 / (1 + 0.707); 5.1 passes
// through. Clamped to [-1, 1]. Plain C++.

#pragma once

#include <cstddef>

namespace audio {

// `input` has `samples` samples a channel, six channels one after another.
// `front`, if given, is stereo (interleaved native floats, left then right)
// added to the front channels before the fold and the clamp, as XAudio's
// default matrix routes a stereo voice: the movies' sound
// (docs/native-movies.md).
void ToInterleaved51(const float* input, size_t samples, float* output,
                     const float* front = nullptr);
void ToInterleavedStereo(const float* input, size_t samples, float* output,
                         const float* front = nullptr);

}  // namespace audio
