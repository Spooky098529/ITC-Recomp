#include "audio/output_mix.h"

#include <algorithm>
#include <bit>
#include <cstdint>
#include <cstring>

namespace audio {

namespace {

constexpr float kCenter = 0.70710678f;
constexpr float kBack = 0.70710678f;
constexpr float kStereoScale = 0.58578644f;  // 1 / (1 + 0.707)

float LoadBig(const float* p) {
  uint32_t word;
  std::memcpy(&word, p, sizeof(word));
  return std::bit_cast<float>(std::byteswap(word));
}

float Clamp(float v) {
  return std::clamp(v, -1.0f, 1.0f);
}

}  // namespace

void ToInterleaved51(const float* input, size_t samples, float* output, const float* front) {
  for (size_t i = 0; i < samples; i++) {
    for (size_t c = 0; c < 6; c++) {
      float v = LoadBig(&input[c * samples + i]);
      if (front && c < 2) {
        v += front[i * 2 + c];
      }
      output[i * 6 + c] = Clamp(v);
    }
  }
}

void ToInterleavedStereo(const float* input, size_t samples, float* output,
                         const float* front) {
  for (size_t i = 0; i < samples; i++) {
    float front_left = LoadBig(&input[0 * samples + i]);
    float front_right = LoadBig(&input[1 * samples + i]);
    if (front) {
      front_left += front[i * 2];
      front_right += front[i * 2 + 1];
    }
    const float center = LoadBig(&input[2 * samples + i]) * kCenter;
    const float back_left = LoadBig(&input[4 * samples + i]);
    const float back_right = LoadBig(&input[5 * samples + i]);
    output[i * 2] = Clamp((front_left + center + back_left * kBack) * kStereoScale);
    output[i * 2 + 1] = Clamp((front_right + center + back_right * kBack) * kStereoScale);
  }
}

}  // namespace audio
