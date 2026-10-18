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

// Which main loop pass this is (counts up by one per pass).
uint64_t GameMessages_PassNumber();

// Any thread, for diagnostics: the current pass's number and type as of its
// start, and whether the caller is the main thread.
void GameMessages_PassFromAnyThread(uint64_t& number, bool& render_only, bool& main_thread);

// Diagnostics: the message handler running (by its target) and the
// message's id, or 0 and ~0 between handlers.
void GameMessages_CurrentHandler(uint32_t& handler, uint32_t& message);

// Diagnostics: how many logic and render-only passes have run.
void GameMessages_PassCounts(uint64_t& logic, uint64_t& render_only);

// Main thread: whether the last logic pass was in a race, where render-only
// passes are safe. Updated by GameMessages_OnPassFinished.
bool GameMessages_InRace();

// Diagnostics: whether the pause screen's Draw handler ran on the last pass,
// and input listener 0's guest address (0 until Input::Update has run in a
// race).
bool GameMessages_Paused();
uint32_t GameMessages_InputListener0();

}  // namespace frame_pacing
