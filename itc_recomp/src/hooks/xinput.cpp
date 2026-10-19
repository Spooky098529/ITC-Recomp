// The game's XInput wrappers answered from our own gamepad reading
// (src/input/gamepads.cpp) instead of the SDK's input system, which isn't
// created any more (ItcApp::OnPreSetup). See docs/rexglue-removal.md, B1.
//
// The wrappers are tiny: each sets up the arguments and jumps to its kernel
// import. Their only caller is the physical pad's read (sub_82591C18, once a
// tick per pad from Input::Update; see "Physical pad" in
// docs/pause-menu-fix.md). They give the game what the SDK gave it: the same
// structs (big-endian in guest memory), the same return codes, and SDL's
// state mapped as the SDK's SDL driver mapped it (stick y as ~value, triggers
// as value >> 7, the guide button left out).

#include "guest.h"
#include "input/gamepads.h"

#include <rex/logging.h>

#include <SDL3/SDL.h>

#include <array>

namespace {

constexpr uint32_t kSuccess = 0;
constexpr uint32_t kBadArguments = 0xA0;
constexpr uint32_t kDeviceNotConnected = 0x48F;
constexpr uint32_t kFunctionFailed = 0x65B;

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

// XInput's packet number per pad: one more each read whose state changed
// since the last (the SDK's driver does the same; a pad starts at 1). Only
// Input::Update's thread reads pads.
struct Packets {
  uint32_t number = 0;  // 0: not connected at the last read
  uint32_t changes = 0;
};
Packets g_packets[input::kGamepadSlots];

// The user index the SDK used: 0xFF, or any user, is user 0.
uint32_t User(uint32_t user, uint32_t flags) {
  return (user & 0xFF) == 0xFF || (flags & kFlagAnyUser) ? 0 : user;
}

bool AsksForGamepad(uint32_t flags) {
  return !(flags & 0xFF) || (flags & kFlagGamepad);
}

bool Connected(uint32_t user) {
  input::GamepadState pad;
  return user < input::kGamepadSlots && input::Gamepads_Read(int(user), &pad);
}

}  // namespace

// XInputGetCapabilities: r3 = user, r4 = flags, r5 = X_INPUT_CAPABILITIES
// (20 bytes). The game stores them in the pad on connect (+192); nothing
// known reads them, so they're fixed: a wired gamepad with every button but
// the guide (the SDK also flagged wireless pads and pads with no d-pad).
extern "C" REX_FUNC(sub_825BA3F0) {
  REX_FUNC_PROLOGUE();
  uint32_t flags = ctx.r4.u32;
  uint32_t caps = ctx.r5.u32;
  uint32_t user = User(ctx.r3.u32, flags);
  if (!caps) {
    ctx.r3.u64 = kBadArguments;
  } else if (!AsksForGamepad(flags) || !Connected(user)) {
    ctx.r3.u64 = kDeviceNotConnected;
  } else {
    guest::StoreU8(base, caps + 0, 0x01);   // XINPUT_DEVTYPE_GAMEPAD
    guest::StoreU8(base, caps + 1, 0x01);   // XINPUT_DEVSUBTYPE_GAMEPAD
    guest::StoreU16(base, caps + 2, 0);     // flags
    guest::StoreU16(base, caps + 4, 0xF3FF);  // every button but the guide
    guest::StoreU8(base, caps + 6, 0xFF);     // triggers
    guest::StoreU8(base, caps + 7, 0xFF);
    for (uint32_t offset = 8; offset < 20; offset += 2) {
      guest::StoreU16(base, caps + offset, 0xFFFF);  // sticks and motors
    }
    ctx.r3.u64 = kSuccess;
  }
}

// XInputGetState: r3 = user, r4 = state (X_INPUT_STATE, 16 bytes; may be 0
// to ask whether the pad is connected). The import's flags are 0.
extern "C" REX_FUNC(sub_825BA3F8) {
  REX_FUNC_PROLOGUE();
  uint32_t out = ctx.r4.u32;
  uint32_t user = User(ctx.r3.u32, 0);
  input::GamepadState pad;
  if (user >= input::kGamepadSlots || !input::Gamepads_Read(int(user), &pad)) {
    if (user < input::kGamepadSlots && g_packets[user].number) {
      g_packets[user] = {};
      REXLOG_INFO("xinput: user {} disconnected", user);
    }
    ctx.r3.u64 = kDeviceNotConnected;
    return;
  }
  Packets& packets = g_packets[user];
  if (!packets.number) {
    REXLOG_INFO("xinput: user {} connected", user);
  }
  if (pad.changes != packets.changes) {
    packets.changes = pad.changes;
    packets.number++;
  }
  if (out) {
    uint16_t buttons = 0;
    for (size_t i = 0; i < kButtons.size(); i++) {
      if (pad.buttons & (1u << i)) {
        buttons |= kButtons[i];
      }
    }
    guest::StoreU32(base, out + 0, packets.number);
    guest::StoreU16(base, out + 4, buttons);
    guest::StoreU8(base, out + 6, uint8_t(pad.axes[SDL_GAMEPAD_AXIS_LEFT_TRIGGER] >> 7));
    guest::StoreU8(base, out + 7, uint8_t(pad.axes[SDL_GAMEPAD_AXIS_RIGHT_TRIGGER] >> 7));
    guest::StoreU16(base, out + 8, uint16_t(pad.axes[SDL_GAMEPAD_AXIS_LEFTX]));
    guest::StoreU16(base, out + 10, uint16_t(~pad.axes[SDL_GAMEPAD_AXIS_LEFTY]));
    guest::StoreU16(base, out + 12, uint16_t(pad.axes[SDL_GAMEPAD_AXIS_RIGHTX]));
    guest::StoreU16(base, out + 14, uint16_t(~pad.axes[SDL_GAMEPAD_AXIS_RIGHTY]));
  }
  ctx.r3.u64 = kSuccess;
}

// XInputSetState: r3 = user, r4 = X_INPUT_VIBRATION (left and right motor
// speeds, u16 each).
extern "C" REX_FUNC(sub_825BA408) {
  REX_FUNC_PROLOGUE();
  uint32_t vibration = ctx.r4.u32;
  uint32_t user = User(ctx.r3.u32, 0);
  if (!vibration) {
    ctx.r3.u64 = kBadArguments;
  } else if (!Connected(user)) {
    ctx.r3.u64 = kDeviceNotConnected;
  } else {
    bool rumbled = input::Gamepads_Rumble(int(user), guest::LoadU16(base, vibration),
                                          guest::LoadU16(base, vibration + 2));
    ctx.r3.u64 = rumbled ? kSuccess : kFunctionFailed;
  }
}
