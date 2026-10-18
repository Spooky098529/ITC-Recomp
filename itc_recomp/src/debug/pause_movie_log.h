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

// Whether to log animation tracks now: paused, within the passes traced
// after one of the sub-menus' animation calls.
bool PauseMovieLog_TracksWanted();
// One animation track applied (sub_825A0548): `movie` the UI movie being
// updated (0 outside an update), `property`, `type` and `adds` from its
// descriptor, `target` the word written, `before` and `after` its value
// around the call (the same when `skipped`, by --per_tick_ui_movie_tracks).
// Adding tracks are all logged; setting ones only on a word an adding track
// has moved.
void PauseMovieLog_Track(uint32_t movie, uint32_t node, uint32_t property, uint32_t type,
                         bool adds, uint32_t target, uint32_t before, uint32_t after,
                         bool skipped);

}  // namespace debug
