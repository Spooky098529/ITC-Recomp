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

// Main thread: records the scene again from current game memory, keeping
// what the tick recorded after the draw step (HUD, text) as it was. Returns
// false if it couldn't, in which case the tick's original recording is in
// place instead.
bool DrawStep_Rerecord(PPCContext& ctx, uint8_t* base);

// Main thread, after the last extra frame of a tick: puts the tick's
// original recording back exactly, for the game's own frame.
void DrawStep_FinishExtraFrames(uint8_t* base);

// Main thread: makes the current recording the one the render thread
// replays (and back again). The game swaps them itself later in the tick
// (Scene::Update), so every Show must be followed by a Hide.
void DrawStep_ShowRecording(uint8_t* base);
void DrawStep_HideRecording(uint8_t* base);

}  // namespace frame_pacing
