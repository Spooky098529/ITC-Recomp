// The logical pad build (see logical_pad.h), as the recompiled
// sub_82591918 does it.

#include "logical_pad.h"

namespace input {
namespace {

constexpr float kLow = -1.0f;  // 0x82001BE0, BF800000
constexpr float kHigh = 1.0f;  // 0x82000A30, 3F800000

}  // namespace

void LogicalPad_Add(float values[kPadValues], const float device[kPadValues]) {
  for (int i = 0; i < kPadValues; i++) {
    values[i] = device[i] + values[i];
  }
}

void LogicalPad_Clamp(float values[kPadValues]) {
  // The compares are false for NaN, so it's kept, as by the guest's fcmpu.
  for (int i = 0; i < 8; i++) {
    if (values[i] > kHigh) {
      values[i] = kHigh;
    } else if (values[i] < kLow) {
      values[i] = kLow;
    }
  }
  for (int i = 8; i < kPadValues; i++) {
    if (values[i] > kHigh) {
      values[i] = kHigh;
    }
  }
}

}  // namespace input
