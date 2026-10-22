// The game's UI movie adapters that other overrides call
// (src/hooks/ui_movies_game.cpp).

#pragma once

#include "guest.h"

namespace ui_movies_game_hooks {

// AnimationList::Insert (sub_8211B4D0; r3 = a list, r4 = a 40-byte slot,
// r5 = a position) and AnimationList::RemoveAt (sub_8211B080; r3 = a list,
// r4 = a position): natively or, with --native_ui_movies_game off, the
// originals. Their overrides (WATCH_MOVIE_CALL's,
// src/debug/pause_movie_log.cpp) wrap these for --log_pause_movie.
void Insert(PPCContext& ctx, uint8_t* base);
void RemoveAt(PPCContext& ctx, uint8_t* base);

}  // namespace ui_movies_game_hooks
