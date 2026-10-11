// Smooths motion between 30 Hz game ticks.
//
// The render thread replays the previous tick's render command list on every
// drawn frame (see command_list.h). After each tick, the vertex shader
// constant uploads in that list (which carry the camera and object
// transforms) are paired with the same uploads from the list before it. When
// an upload is replayed, a blend of the two, weighted by how far the frame is
// between ticks, is uploaded instead. What is drawn therefore runs two ticks
// (67 ms) behind the game logic.
//
// Camera switches are drawn as cuts, not blended. Enabled by --interpolate.

#pragma once

#include <cstdint>

namespace interpolation {

// Render thread, after each tick while game logic is paused.
void Interpolation_OnTickDone(uint8_t* base);

// Render thread, before each frame. `tick_progress` runs from 0 just after
// the last tick to 1 when the next is due.
void Interpolation_BeginFrame(float tick_progress);

}  // namespace interpolation
