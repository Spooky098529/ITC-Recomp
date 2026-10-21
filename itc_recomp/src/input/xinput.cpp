// XInput from our gamepad reading (see xinput.h). What the SDK gave the
// game: its return codes, and SDL's state mapped as its SDL driver mapped it
// (stick y as ~value, triggers as value >> 7, the guide button left out).

#include "xinput.h"

#include "gamepads.h"

#include "base/log.h"

#include <SDL3/SDL.h>

#include <array>

namespace input {
namespace {

constexpr uint32_t kFlagGamepad = 0x01;
constexpr uint32_t kFlagAnyUser = 1u << 30;

// XInput's buttons by SDL_GamepadButton, as the SDK's SDL driver maps them:
// the Elite paddles as Y, B, X, A; the guide button, misc button and
// touchpad (0) left out, as the SDK does without --guide_button.
constexpr std::array<uint16_t, 21> kButtons = {
    0x1000,  // south: A
    0x2000,  // east: B
    0x4000,  // west: X
    0x8000,  // north: Y
    0x0020,  // back
    0,       // guide
    0x0010,  // start
    0x0040,  // left stick
    0x0080,  // right stick
    0x0100,  // left shoulder
    0x0200,  // right shoulder
    0x0001,  // d-pad up
    0x0002,  // d-pad down
    0x0004,  // d-pad left
    0x0008,  // d-pad right
    0,       // misc 1
    0x8000,  // right paddle 1: Y
    0x2000,  // left paddle 1: B
    0x4000,  // right paddle 2: X
    0x1000,  // left paddle 2: A
    0,       // touchpad
};
static_assert(SDL_GAMEPAD_BUTTON_SOUTH == 0 && SDL_GAMEPAD_BUTTON_DPAD_RIGHT == 14 &&
              SDL_GAMEPAD_BUTTON_TOUCHPAD == 20);

// The packet number per pad: one more each read whose state changed since
// the last (the SDK's driver does the same; a pad starts at 1).
struct Packets {
  uint32_t number = 0;  // 0: not connected at the last read
  uint32_t changes = 0;
};
Packets g_packets[kGamepadSlots];

// The user index the SDK used: 0xFF, or any user, is user 0.
uint32_t User(uint32_t user, uint32_t flags) {
  return (user & 0xFF) == 0xFF || (flags & kFlagAnyUser) ? 0 : user;
}

bool AsksForGamepad(uint32_t flags) {
  return !(flags & 0xFF) || (flags & kFlagGamepad);
}

bool Connected(uint32_t user) {
  GamepadState pad;
  return user < kGamepadSlots && Gamepads_Read(int(user), &pad);
}

}  // namespace

uint32_t XInput_GetState(uint32_t user, XInputState* state) {
  user = User(user, 0);
  GamepadState pad;
  if (user >= kGamepadSlots || !Gamepads_Read(int(user), &pad)) {
    if (user < kGamepadSlots && g_packets[user].number) {
      g_packets[user] = {};
      ITC_LOG_INFO("xinput: user {} disconnected", user);
    }
    return kXInputDeviceNotConnected;
  }
  Packets& packets = g_packets[user];
  if (!packets.number) {
    ITC_LOG_INFO("xinput: user {} connected", user);
  }
  if (pad.changes != packets.changes) {
    packets.changes = pad.changes;
    packets.number++;
  }
  if (state) {
    uint16_t buttons = 0;
    for (size_t i = 0; i < kButtons.size(); i++) {
      if (pad.buttons & (1u << i)) {
        buttons |= kButtons[i];
      }
    }
    state->packet_number = packets.number;
    state->gamepad = {
        buttons,
        uint8_t(pad.axes[SDL_GAMEPAD_AXIS_LEFT_TRIGGER] >> 7),
        uint8_t(pad.axes[SDL_GAMEPAD_AXIS_RIGHT_TRIGGER] >> 7),
        pad.axes[SDL_GAMEPAD_AXIS_LEFTX],
        int16_t(~pad.axes[SDL_GAMEPAD_AXIS_LEFTY]),
        pad.axes[SDL_GAMEPAD_AXIS_RIGHTX],
        int16_t(~pad.axes[SDL_GAMEPAD_AXIS_RIGHTY]),
    };
  }
  return kXInputSuccess;
}

// The game stores the capabilities in the pad on connect (+192); nothing
// known reads them, so they're fixed: a wired gamepad with every button but
// the guide (the SDK also flagged wireless pads and pads with no d-pad).
uint32_t XInput_GetCapabilities(uint32_t user, uint32_t flags, XInputCapabilities* caps) {
  if (!caps) {
    return kXInputBadArguments;
  }
  if (!AsksForGamepad(flags) || !Connected(User(user, flags))) {
    return kXInputDeviceNotConnected;
  }
  *caps = {
      0x01,  // XINPUT_DEVTYPE_GAMEPAD
      0x01,  // XINPUT_DEVSUBTYPE_GAMEPAD
      0,
      {0xF3FF, 0xFF, 0xFF, -1, -1, -1, -1},
      0xFFFF,
      0xFFFF,
  };
  return kXInputSuccess;
}

uint32_t XInput_SetState(uint32_t user, uint16_t left_motor, uint16_t right_motor) {
  user = User(user, 0);
  if (!Connected(user)) {
    return kXInputDeviceNotConnected;
  }
  return Gamepads_Rumble(int(user), left_motor, right_motor) ? kXInputSuccess
                                                             : kXInputFunctionFailed;
}

}  // namespace input
