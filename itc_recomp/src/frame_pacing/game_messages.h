// The game's message queue (sub_821062E8 dispatches it).
//
// Each main-loop pass, game logic posts messages that the dispatcher
// delivers to every registered handler. Message 0 is Update (all physics and
// fixed-step logic run in its handlers) and message 1 is Draw (the draw step
// that records the scene). See "Separating drawing from the game tick" in
// docs/functions.md.
//
// On a render-only pass, Update messages are consumed without being
// delivered, so the pass draws the scene without advancing the game.
//
// Known bug: menus (and probably the race HUD) are recorded while handling
// Update, so they're missing from render-only frames and flicker. Fix: skip
// Update only for the handlers that run game logic, not the UI ones.

#pragma once

#include <cstdint>

namespace frame_pacing {

// Main thread: whether the coming main-loop pass is render-only.
void GameMessages_SetRenderOnlyPass(bool render_only);

}  // namespace frame_pacing
