#pragma once

#include <cstdint>

namespace debug {

// --log_pause_movie: around each UI movie update (sub_8211D2F8, hooked in
// frame_pacing/per_frame_logic.cpp). `r4` is the mode the game asked for (0
// advance and draw, 1 draw only), `caller` the guest lr; `draw_only` is
// whether --per_tick_pause_menu_movie made it a draw-only call.
void PauseMovieLog_Before(uint8_t* base, uint32_t movie, uint32_t r4, uint32_t caller,
                          bool draw_only);
void PauseMovieLog_After(uint8_t* base, uint32_t movie);

}  // namespace debug
