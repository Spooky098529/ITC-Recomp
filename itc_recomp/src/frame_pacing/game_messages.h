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
// Render-only passes are only used in races (ticks that run the race's
// draw step, sub_8215B4F8). Menus record their UI and act on input outside
// that split, so with render-only passes they flickered and saw each press
// several times; they stay at the game's 30 fps.

#pragma once

#include <cstdint>

namespace frame_pacing {

// Main thread, when a main-loop pass's drawing is done (in WaitForFrame).
void GameMessages_OnPassFinished();

// Main thread: whether the coming main-loop pass is render-only.
void GameMessages_SetRenderOnlyPass(bool render_only);
bool GameMessages_IsRenderOnlyPass();

// Diagnostics: how many logic and render-only passes have run.
void GameMessages_PassCounts(uint64_t& logic, uint64_t& render_only);

// Main thread: whether the last logic pass was in a race, where render-only
// passes are safe. Updated by GameMessages_OnPassFinished.
bool GameMessages_InRace();

}  // namespace frame_pacing
