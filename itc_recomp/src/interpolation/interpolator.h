// Smooths motion between 30 Hz game ticks.
//
// After each tick, records the camera's
// view matrix and each vehicle's world matrix. Before each frame, writes a
// blend of the last two ticks into guest memory, weighted by how far the
// frame is between ticks; after the frame, puts the real values back so
// game logic never sees them. The picture therefore runs one tick (33 ms)
// behind the game.
//
// Big jumps (a camera switch, a car reset) are drawn as cuts, not blended.
// Enabled by --interpolate (default false). Not effective yet: the game
// records its render commands, with copies of these matrices, during the
// tick, and drawing only replays them. See docs/functions.md.

#pragma once

#include <cstdint>

namespace interpolation {

// Called from the speed getter with each vehicle pointer it sees.
void Interpolation_OnVehicleSeen(uint32_t vehicle);

// Render thread, after each tick while game logic is paused.
void Interpolation_OnTickDone(uint8_t* base);

// Render thread, around each frame. `tick_progress` runs from 0 just after
// the last tick to 1 when the next is due.
void Interpolation_BeginFrame(uint8_t* base, float tick_progress);
void Interpolation_EndFrame(uint8_t* base);

}  // namespace interpolation
