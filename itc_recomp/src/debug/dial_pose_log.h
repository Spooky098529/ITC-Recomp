#pragma once

#include <cstdint>

namespace debug {

// --log_dial_pose: whether it's on.
bool DialPoseLog_Enabled();
// Draw's per-car update (sub_820DAC68, hooked in
// frame_pacing/per_frame_logic.cpp) stepping car + 704: its value before the
// call (after --per_tick_car_gauges' give-back) and after it.
void DialPoseLog_CarStep(uint8_t* base, uint32_t car, float before, float after);

// The dial functions' entries (hooks/cockpit_dials.cpp): sub_820E45A8 (r3 =
// the dash, r4 = the car, r5 = the camera passes) and sub_820E42E8.
void DialPoseLog_Dials(uint8_t* base, uint32_t dash, uint32_t car, uint32_t r5);
void DialPoseLog_Tach(uint8_t* base, uint32_t dash, uint32_t car);
// A needle's angle as --native_cockpit_dials posed it (`function` "dials" or
// "tach", `needle` its index there), from gauge value `value`.
void DialPoseLog_NativeAngle(uint8_t* base, const char* function, uint32_t needle, uint32_t car,
                             float angle, float value);

}  // namespace debug
