// Finds the title screen's idle timer (--find_idle_timer): logs guest words
// that step steadily like a timer in menus, and their values when a race
// (the attract race) starts.

#pragma once

#include <cstdint>

namespace debug {

// Main thread, as a main loop pass is about to start (after the frame wait).
void IdleTimerScan_OnPassStart(uint8_t* base, bool render_only, bool in_race);

// Main thread, around a tick handler (by its target) or a delivered message
// handler (by its target), on logic passes: logs the handler changing a
// steady timer.
void IdleTimerScan_Before(uint8_t* base);
void IdleTimerScan_After(uint8_t* base, const char* kind, uint32_t id);

}  // namespace debug
