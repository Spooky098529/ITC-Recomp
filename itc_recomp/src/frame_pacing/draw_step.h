// The game's draw step (sub_8215B4F8): records the scene into the render
// command list after each tick's logic. It contains no physics, so it can
// be run again to record the scene from different (interpolated) state. See
// "Separating drawing from the game tick" in docs/functions.md.

#pragma once

#include "guest.h"

namespace frame_pacing {

// Main thread, after a tick: whether this tick ran the draw step (menus and
// loading screens don't), so it can be re-recorded.
bool DrawStep_CanRerecord();

// Main thread: puts the recording structures back as they were before this
// tick's draw step and runs it again, recording from current game memory.
void DrawStep_Rerecord(PPCContext& ctx, uint8_t* base);

// Main thread: makes the recording just made the one the render thread
// replays (and back again). The game itself swaps them later in the tick
// (Scene::Update), so every Show must be followed by a Hide.
void DrawStep_ShowRecording(uint8_t* base);
void DrawStep_HideRecording(uint8_t* base);

// Main thread, once this tick's extra frames are done: forget the snapshot,
// so a tick without a draw step (a menu) never re-records stale state.
void DrawStep_Invalidate();

}  // namespace frame_pacing
