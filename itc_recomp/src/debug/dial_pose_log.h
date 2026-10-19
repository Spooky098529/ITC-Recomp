#pragma once

#include <cstdint>

namespace debug {

// --log_dial_pose: whether it's on.
bool DialPoseLog_Enabled();
// Draw's per-car update (sub_820DAC68, hooked in
// frame_pacing/per_frame_logic.cpp) stepping car + 704: its value before the
// call (after --per_tick_car_gauges' give-back) and after it.
void DialPoseLog_CarStep(uint8_t* base, uint32_t car, float before, float after);

}  // namespace debug
