// Finds game state that advances on render-only passes (--find_per_frame_state).
//
// A render-only pass should leave game state alone; anything that counts
// frames there runs too fast at high frame rates (the race start countdown
// did). Every two seconds, guest memory is compared before and after a
// render-only pass, and words that step like a timer (a float by +-1/30 or
// +-1/60, an integer by +-1) are logged and watched. On later render-only
// passes, each watched word is checked around every main loop tick handler
// and every game message, to name the code that advances it.

#pragma once

#include <cstdint>

namespace debug {

// Main thread, as a main loop pass is about to start (after the frame wait).
void FrameStateDiff_OnPassStart(uint8_t* base, bool render_only);

// Main thread, around a tick handler (by its target) or a delivered message
// (by its id), on render-only passes.
void FrameStateDiff_Before(uint8_t* base);
void FrameStateDiff_After(uint8_t* base, const char* kind, uint32_t id);

// Watches a guest word from now on, logging every change with its values.
void FrameStateDiff_Watch(uint32_t address);

// Testing (--debug_hide): whether a wrapped draw function is never to run.
bool IsHidden(uint32_t function);

// Vec4::Transform's work (matrix_math::Vec4Transform: out = v * m) inside
// the same checks its override (sub_820AB560) puts around it: nothing when
// --debug_hide names it, else the work between FrameStateDiff_Before and
// After. Native callers of Vec4::Transform call this instead of the plain
// C++ alone.
void Vec4Transform(uint8_t* base, uint32_t out, uint32_t m, uint32_t v);

}  // namespace debug
