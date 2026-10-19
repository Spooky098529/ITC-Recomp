// The game's physical pad read (sub_82591C18) as plain C++: one pad's
// connection flags, its 40 values and its rumble, from our XInput
// (xinput.h). The pad stays in guest memory with the rest of the input
// manager, which recompiled code still reads; our Input::Update
// (src/hooks/input_update.cpp) copies its fields in and out. See "Physical pad" in docs/pause-menu-fix.md.

#pragma once

#include "xinput.h"

#include <cstdint>

namespace input {

// Flags at +4.
constexpr uint32_t kPadConnected = 1;     // connected at the last read
constexpr uint32_t kPadJustConnected = 2;
constexpr uint32_t kPadJustDisconnected = 4;

struct PadMotor {
  float speed;         // 0..1
  int32_t held;        // nonzero: the countdown is paused
  int32_t ticks_left;  // the speed drops to 0 when it reaches 0
};

// The fields the read touches, by their offsets in the guest's 212 bytes.
struct PhysicalPad {
  uint32_t flags;           // +4
  float values[40];         // +8
  PadMotor motors[2];       // +168 left, +180 right
  XInputCapabilities caps;  // +192, read on connect
};

// Reads the pad for `user`, as the game does once a tick. Returns true if it
// read the capabilities (on a connect), which the adapter then stores.
bool PhysicalPad_Read(PhysicalPad& pad, uint32_t user);

}  // namespace input
