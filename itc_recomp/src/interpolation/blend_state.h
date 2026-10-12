// Interpolated game state for drawing.
//
// Each main-loop pass delivers the game's Draw message (see
// frame_pacing/game_messages.h). Game logic ticks at 30 Hz, so drawing shows
// a blend of the last two ticks: the cameras are blended around the Draw
// message, and each object's world matrix as the per-object recorder is
// handed it. Originals are put back afterwards, so game logic never sees the
// blends. See blend_state.cpp.

#pragma once

#include <cstdint>
#include <functional>

namespace interpolation {

// Main thread, before a logic pass (one that runs game logic) starts.
void BlendState_OnLogicPassStart();

// Main thread: the message handler being run (its target), 0 after it.
void BlendState_SetHandler(uint32_t handler);

// Main thread, around the delivery of a Draw message.
void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass);
void BlendState_AfterDraw(uint8_t* base);

// Main thread, around the per-object recorder (sub_825953D0): r4 is the
// camera's view-projection matrix and r5 the object's world matrix.
void BlendState_BeforeRecordObject(uint8_t* base, uint32_t view_projection, uint32_t world);
void BlendState_AfterRecordObject(uint8_t* base);

// Render thread diagnostics (--log_replayed_constants): a frame starting
// (after a logic tick, or not), and each shader constant upload replayed.
void BlendState_OnReplayFrameStart();
void BlendState_OnReplayTick();

// Render thread, around a shader constant upload (command 7) being replayed:
// gives constants recorded with a tick's camera the frame's blended camera.
void BlendState_ReplayConstants(uint8_t* base, uint32_t payload, uint32_t packed,
                                const std::function<void()>& upload);

}  // namespace interpolation
