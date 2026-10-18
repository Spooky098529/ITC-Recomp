#pragma once

#include <cstdint>

namespace debug {

// --log_pause_movie: around each UI movie update (sub_8211D2F8, hooked in
// frame_pacing/per_frame_logic.cpp). `r4` is the mode the game asked for (0
// advance and draw, 1 draw only), `caller` the guest lr.
void PauseMovieLog_Before(uint8_t* base, uint32_t movie, uint32_t r4, uint32_t caller);
// `outcome` is what the UI movie replay did with the call (`occurrence`: its
// order among the movie's updates in the pass).
void PauseMovieLog_After(uint8_t* base, uint32_t movie, uint32_t caller, const char* outcome,
                         int occurrence);

}  // namespace debug
