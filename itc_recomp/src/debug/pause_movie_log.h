#pragma once

#include <cstdint>

namespace debug {

// --log_pause_movie: around each update of the pause menu's movie (the
// sub_8211D2F8 call from sub_8220B9E8, hooked in frame_pacing/
// per_frame_logic.cpp), while paused. `r4` is the mode the game asked for
// (0 advance and draw, 1 draw only); `draw_only` is whether
// --per_tick_pause_menu_movie made it a draw-only call.
void PauseMovieLog_Before(uint8_t* base, uint32_t movie, uint32_t r4, bool draw_only);
void PauseMovieLog_After(uint8_t* base);

}  // namespace debug
