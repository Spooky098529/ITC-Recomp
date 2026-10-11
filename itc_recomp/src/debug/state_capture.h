// Records guest game state to a file once per logic tick, for working out
// which objects hold the car and camera transforms (needed for
// interpolation). Enabled by --capture_dir=<folder>; does nothing otherwise.
//
// Every tick: an 8 KB window of each vehicle object seen by the speed
// getter. Every 10 s: a scan for memory that points at a vehicle, and for
// 4x4 matrices positioned near the car (camera candidates), followed by a
// 2 s burst recording each.
// Read the output with tools/read_capture.py.

#pragma once

#include <cstdint>

namespace debug {

// Called from the speed getter with each vehicle pointer it sees.
void StateCapture_OnVehicleSeen(uint32_t vehicle);

// Called on the render thread after each logic tick, while logic is paused.
void StateCapture_OnTickDone(uint8_t* base);

}  // namespace debug
