// Finds the pause menu's selection (--find_menu_selection), and the code that
// moves it, by pass type (docs/pause-menu-fix.md).
//
// While paused, guest memory is compared over a render-only pass every
// 300 ms. Words that change by a small integer step while a direction is
// held (listener 0's counters at +0x180 or +0x190 non-zero) and never while
// nothing is held are candidates. Candidates are then checked around every
// tick handler, message handler and screen update on every pass, and each
// change is logged with the code making it and the pass type.

#pragma once

#include <cstdint>

namespace debug {

// Main thread, as a main loop pass is about to start. `listener` is input
// listener 0's guest address (0 when not known yet).
void MenuSelectionScan_OnPassStart(uint8_t* base, bool render_only, bool paused,
                                   uint32_t listener);

// Whether anything is watched (--find_menu_selection found candidates, or
// --menu_watch is set).
bool MenuSelectionScan_Active();

// Main thread, around a tick handler, message handler or screen update.
void MenuSelectionScan_Before(uint8_t* base);
void MenuSelectionScan_After(uint8_t* base, const char* kind, uint32_t id);

}  // namespace debug
