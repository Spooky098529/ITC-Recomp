// The physical pad read (see physical_pad.h), operation for operation as
// the recompiled sub_82591C18. Its float products are single float
// multiplies, which the guest also rounds once (fmuls), so the bits match.

#include "physical_pad.h"

#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#include <utility>

namespace input {
namespace {

// The constants it reads from the image.
constexpr float kOne = 1.0f;                 // 0x82000A30, 3F800000
constexpr float kZero = 0.0f;                // 0x82000A34, 00000000
constexpr float kTriggerScale = 1.0f / 255;  // 0x82002200, 3B808081
constexpr float kStickScale = 1.0f / 32767;  // 0x82002BD4, 38000100
constexpr float kStickScaleNegated = -kStickScale;  // 0x8206CBFC, B8000100
constexpr float kMotorScale = 65535.0f;      // 0x8200223C, 477FFF00
static_assert(std::bit_cast<uint32_t>(kTriggerScale) == 0x3B808081 &&
              std::bit_cast<uint32_t>(kStickScale) == 0x38000100 &&
              std::bit_cast<uint32_t>(kStickScaleNegated) == 0xB8000100 &&
              std::bit_cast<uint32_t>(kMotorScale) == 0x477FFF00);

// XInput button, value slot (values[8 + slot]), in the game's order.
constexpr std::array<std::pair<uint16_t, int>, 14> kButtonSlots = {{
    {0x0001, 0},   // d-pad up
    {0x0002, 1},   // d-pad down
    {0x0004, 2},   // d-pad left
    {0x0008, 3},   // d-pad right
    {0x1000, 4},   // A
    {0x2000, 5},   // B
    {0x4000, 6},   // X
    {0x8000, 7},   // Y
    {0x0100, 12},  // left shoulder
    {0x0040, 14},  // left stick click
    {0x0200, 16},  // right shoulder
    {0x0080, 18},  // right stick click
    {0x0010, 20},  // start
    {0x0020, 21},  // back
}};

// sub_825A1900: one tick of a motor's countdown.
void TickMotor(PadMotor& motor) {
  if (motor.held || motor.ticks_left <= 0) {
    return;
  }
  if (--motor.ticks_left == 0) {
    motor.speed = kZero;
  }
}

// fctidz, kept to its low 16 bits: toward zero, NaN as INT64_MIN, out of
// range saturated.
uint16_t MotorSpeed(float speed) {
  float scaled = speed * kMotorScale;
  int64_t value;
  if (std::isnan(scaled)) {
    value = std::numeric_limits<int64_t>::min();
  } else if (scaled >= 0x1p63f) {
    value = std::numeric_limits<int64_t>::max();
  } else if (scaled < -0x1p63f) {
    value = std::numeric_limits<int64_t>::min();
  } else {
    value = int64_t(scaled);
  }
  return uint16_t(value);
}

}  // namespace

bool PhysicalPad_Read(PhysicalPad& pad, uint32_t user) {
  bool was_connected = pad.flags & kPadConnected;
  std::memset(pad.values, 0, sizeof(pad.values));
  XInputState state;
  if (XInput_GetState(user, &state) != kXInputSuccess) {
    pad.flags &= ~kPadConnected;
    if (was_connected) {
      pad.flags |= kPadJustDisconnected;
    } else {
      pad.flags &= ~kPadJustDisconnected;
    }
    return false;
  }

  bool caps_read = false;
  pad.flags |= kPadConnected;
  if (was_connected) {
    pad.flags &= ~kPadJustConnected;
  } else {
    pad.flags |= kPadJustConnected;
    caps_read = XInput_GetCapabilities(user, 1, &pad.caps) == kXInputSuccess;
  }

  TickMotor(pad.motors[0]);
  TickMotor(pad.motors[1]);
  XInput_SetState(user, MotorSpeed(pad.motors[0].speed), MotorSpeed(pad.motors[1].speed));

  const XInputGamepad& g = state.gamepad;
  for (auto [mask, slot] : kButtonSlots) {
    if (g.buttons & mask) {
      pad.values[8 + slot] = kOne;
    }
  }
  float lt = float(g.left_trigger) * kTriggerScale;
  float rt = float(g.right_trigger) * kTriggerScale;
  float lx = float(g.thumb_lx) * kStickScale;
  float ly = float(g.thumb_ly) * kStickScale;
  float lx_negated = float(g.thumb_lx) * kStickScaleNegated;
  float ly_negated = float(g.thumb_ly) * kStickScaleNegated;
  pad.values[0] = lx;
  pad.values[1] = ly;
  pad.values[2] = lt;
  pad.values[3] = kZero;
  pad.values[4] = float(g.thumb_rx) * kStickScale;
  pad.values[5] = float(g.thumb_ry) * kStickScale;
  pad.values[6] = rt;
  pad.values[7] = kZero;
  pad.values[8 + 13] = lt;
  pad.values[8 + 17] = rt;
  for (int i = 0; i < 2; i++) {  // slots 24-31
    pad.values[32 + 4 * i] = ly;
    pad.values[33 + 4 * i] = ly_negated;
    pad.values[34 + 4 * i] = lx;
    pad.values[35 + 4 * i] = lx_negated;
  }
  return caps_read;
}

}  // namespace input
