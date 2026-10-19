// The game's input processors' update (vtable 0x8206C0C4 slot 1,
// sub_82592188) as plain C++. A processor turns a logical pad's 40 values
// into dead-zoned sticks, their lengths and directions, and button masks
// with hold counters (the menus' hold-to-repeat). Input::Update updates one
// per logical pad, and the listeners' first objects are of the same class.
// The objects live in guest memory, where the action mappers read them;
// src/hooks/input_update.cpp copies the fields in and out. See "The input
// processors" in docs/pause-menu-fix.md.

#pragma once

#include <cstdint>

namespace input {

// The fields the update touches, by their offsets in the guest's 416 bytes.
struct InputProcessor {
  float dead_zone[4];       // +16 (read): x, y, trigger, unused; both sticks
  float raw[8];             // +32: the pad's values 0-7 (LX, LY, LT, 0, RX, RY, RT, 0)
  float adjusted[8];        // +64: raw with the dead zone taken out
  float direction[2][4];    // +96, +112: each stick's (x, y) normalized, or 0
  float length[2];          // +128, +132: each stick's length
  float buttons[32];        // +136: the pad's values 8-39
  uint32_t pressed;         // +264: bits newly set this tick
  uint32_t released;        // +268: bits newly clear this tick
  uint32_t held;            // +272: bit i: buttons[i] above the threshold
  int32_t repeat_delay;     // +276 (read)
  int32_t repeat_period;    // +280 (read)
  int32_t held_ticks[32];   // +284: ticks each bit has been held
};

// One tick, from the logical pad's 40 values.
void InputProcessor_Update(InputProcessor& processor, const float values[40]);

}  // namespace input
