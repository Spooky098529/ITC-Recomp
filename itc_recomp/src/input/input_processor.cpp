// The input processors' update (see input_processor.h), operation for
// operation as the recompiled sub_82592188. Its single float adds,
// subtracts, multiplies, divides and square roots are done in double and
// rounded to float there, which gives the same bits as float math; its
// fused multiply-adds are done in double here as there. The direction uses
// the translated AltiVec code's SSE operations (dpps with mask 0xEF, 1/sqrt,
// one Newton step as separate multiplies and adds). Not copied: that code
// flushes denormals to zero, and stick values never get that small.

#include "input_processor.h"

#include <smmintrin.h>

#include <cmath>
#include <cstring>

namespace input {
namespace {

constexpr float kZero = 0.0f;  // 0x82000A34
constexpr float kOne = 1.0f;   // 0x82000A30
constexpr float kButtonThreshold = 0.5f;  // 0x82001BE4, 3F000000

}  // namespace

void InputProcessor_Update(InputProcessor& p, const float values[40]) {
  std::memcpy(p.raw, values, sizeof(p.raw));

  const __m128 half = _mm_set1_ps(0.5f);  // vcfsx of 1, scale 1
  for (int stick = 0; stick < 2; stick++) {
    for (int i = 0; i < 4; i++) {
      float raw = p.raw[4 * stick + i];
      float dead_zone = p.dead_zone[i];
      float& out = p.adjusted[4 * stick + i];
      // The compares are false for NaN, which keeps it.
      if (raw > kZero) {
        out = (raw - dead_zone) / (kOne - dead_zone);
        if (out < kZero) {
          out = kZero;
        }
      } else {
        out = (raw + dead_zone) / (kOne - dead_zone);
        if (out > kZero) {
          out = kZero;
        }
      }
    }
    float x = p.adjusted[4 * stick + 0];
    float y = p.adjusted[4 * stick + 1];
    // fmuls, then two fmadds (the first adds 0 * 0, from the zeroed z).
    float length = x * x;
    length = float(std::fma(0.0, 0.0, double(length)));
    length = float(std::fma(double(y), double(y), double(length)));
    p.length[stick] = std::sqrt(length);

    // The translated code keeps AltiVec element 0 in SSE's top lane, so
    // (x, y, 0, 0) is lanes 3 to 0 (mask 0xEF sums lanes 3, 2 and 1).
    __m128 v = _mm_setr_ps(kZero, kZero, y, x);
    __m128 dot = _mm_dp_ps(v, v, 0xEF);
    __m128 estimate = _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(dot));
    __m128 half_dot = _mm_mul_ps(dot, half);
    __m128 squared = _mm_mul_ps(estimate, estimate);
    __m128 step = _mm_xor_ps(_mm_sub_ps(_mm_mul_ps(half_dot, squared), half),
                             _mm_castsi128_ps(_mm_set1_epi32(int(0x80000000))));
    estimate = _mm_add_ps(_mm_mul_ps(estimate, step), estimate);
    __m128 normalized = _mm_mul_ps(v, estimate);
    __m128 zero = _mm_cmpeq_ps(dot, _mm_setzero_ps());
    __m128 result = _mm_or_ps(_mm_andnot_ps(zero, normalized), _mm_and_ps(zero, dot));
    _mm_storeu_ps(p.direction[stick], _mm_shuffle_ps(result, result, _MM_SHUFFLE(0, 1, 2, 3)));
  }

  uint32_t was_held = p.held;
  p.held = 0;
  for (int i = 0; i < 32; i++) {
    float value = values[8 + i];
    p.buttons[i] = value;
    if (value > kButtonThreshold) {
      p.held |= 1u << i;
      int32_t ticks = int32_t(uint32_t(p.held_ticks[i]) + 1);
      p.held_ticks[i] = ticks;
      if (ticks > int32_t(uint32_t(p.repeat_delay) + uint32_t(p.repeat_period))) {
        p.held_ticks[i] = p.repeat_delay;
      }
    } else {
      p.held_ticks[i] = 0;
    }
  }
  uint32_t changed = p.held ^ was_held;
  p.pressed = p.held & changed;
  p.released = changed & was_held;
}

}  // namespace input
