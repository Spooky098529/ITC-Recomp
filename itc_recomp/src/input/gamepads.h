// Gamepads read with SDL, our own instead of the SDK's input system (see
// docs/rexglue-removal.md, A5). No Xbox types here: the game's side (its
// XInput calls) converts what this gives.
//
// Pads take the lowest free of four slots when they connect and keep it
// until they're unplugged, as the SDK assigns them (unplugging pad 1 doesn't
// make pad 2 player 1). Only gamepads: no keyboard (docs/rexglue-removal.md,
// A0).

#pragma once

#include <cstdint>
#include <functional>

namespace input {

constexpr int kGamepadSlots = 4;

struct GamepadState {
  uint32_t buttons;  // bit n: SDL_GamepadButton n held
  // By SDL_GamepadAxis: sticks -32768..32767 (y positive down), triggers
  // 0..32767, as SDL gives them.
  int16_t axes[6];
  uint32_t changes;  // counts the events that changed the state
};

// Queues a function on the UI thread, which pumps SDL's events; never runs
// it in place.
using UiThreadQueue = std::function<void(std::function<void()>)>;

// UI thread, once SDL's window exists (ItcApp::SetupPresentation).
void Gamepads_Start(UiThreadQueue queue);
// UI thread, at shutdown.
void Gamepads_Stop();

// Any thread. Each asks the UI thread to pump SDL's events, so the next read
// sees what's new (as the SDK does). False if no pad is in the slot.
bool Gamepads_Read(int slot, GamepadState* state);
// Motor speeds 0..65535, held until the next call.
bool Gamepads_Rumble(int slot, uint16_t low, uint16_t high);

}  // namespace input
