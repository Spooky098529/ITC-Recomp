// Interpolated game state for drawing.
//
// Each main-loop pass delivers the game's Draw message (see
// frame_pacing/game_messages.h). Around it, the matrices the draw step reads
// (camera passes, the render-state block, recorder arguments) are replaced
// with a blend of the last two ticks, then put back, so game logic never
// sees them. See blend_state.cpp.

#pragma once

#include <cstdint>

namespace interpolation {

// Main thread, from the speed getter: a vehicle seen this tick.
void BlendState_OnVehicleSeen(uint32_t vehicle);

// Main thread, from the per-object recorder: a matrix the draw step read.
void BlendState_OnMatrixUsed(uint32_t address);

// Main thread, around the delivery of a Draw message.
void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass);
void BlendState_AfterDraw(uint8_t* base);

}  // namespace interpolation
