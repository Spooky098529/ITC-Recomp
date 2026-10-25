// XInput as the game uses it, answered from our gamepad reading
// (gamepads.h). Plain C++: host structs and arguments, no PowerPC context or
// guest memory. Called by our physical pad read (physical_pad.h), which
// replaced the game's only caller of its XInput wrappers (sub_825BA3F0,
// sub_825BA3F8, sub_825BA408; no longer called). See
// docs/rexglue-removal.md, B1.

#pragma once

#include <cstdint>

namespace input {

// Return codes, as the SDK's.
constexpr uint32_t kXInputSuccess = 0;
constexpr uint32_t kXInputBadArguments = 0xA0;
constexpr uint32_t kXInputDeviceNotConnected = 0x48F;
constexpr uint32_t kXInputFunctionFailed = 0x65B;

struct XInputGamepad {
  uint16_t buttons;  // XINPUT_GAMEPAD_* bits
  uint8_t left_trigger;
  uint8_t right_trigger;
  int16_t thumb_lx;
  int16_t thumb_ly;  // positive up
  int16_t thumb_rx;
  int16_t thumb_ry;
};

struct XInputState {
  uint32_t packet_number;
  XInputGamepad gamepad;
};

struct XInputCapabilities {
  uint8_t type;
  uint8_t sub_type;
  uint16_t flags;
  XInputGamepad gamepad;  // the inputs it has, as masks
  uint16_t left_motor;
  uint16_t right_motor;
};

// state may be null, to ask whether the pad is connected. Called from one
// thread (Input::Update's).
uint32_t XInput_GetState(uint32_t user, XInputState* state);
uint32_t XInput_GetCapabilities(uint32_t user, uint32_t flags, XInputCapabilities* caps);
// Motor speeds 0..65535, held until the next call.
uint32_t XInput_SetState(uint32_t user, uint16_t left_motor, uint16_t right_motor);

}  // namespace input
