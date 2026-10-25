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

// UiMovie::Loop (sub_8211DDD8; r3 = movie, r4 = a sub-scene's index or -1,
// r5 = an animation's index): natively with --native_ui_movies_game and
// without --log_pause_movie, else the original. Its override
// (src/debug/pause_movie_log.cpp) logs it for --log_pause_movie.
void Loop(PPCContext& ctx, uint8_t* base);

// UiMovie::Play (sub_8211D978; r3 = movie, r4 = a sub-scene's index or -1,
// r5 = an animation's index, r6..r9 the slot's +24..+36) and
// UiMovie::PlayNamed (sub_8211DAC8; r4 = a named animation's index, r5..r8
// the slot's +24..+36), as Loop: their overrides log them.
void Play(PPCContext& ctx, uint8_t* base);
void PlayNamed(PPCContext& ctx, uint8_t* base);

// UiMovie::DrawTexts (sub_8211CD90; r3 = movie, r4 = the text list, r5 = 1
// to draw without advancing): natively with --native_ui_movies_game, else
// the original. Its override (src/debug/pause_movie_log.cpp) watches it for
// --log_pause_movie. r3 left as the original leaves it.
void DrawTexts(PPCContext& ctx, uint8_t* base);

// UiMovie::Finish (sub_8211DE38; r3 = movie, r4 = a sub-scene's index or
// -1, r5 = an animation's index) and UiMovie::FinishNamed (sub_8211DF20;
// r4 = a named animation's index), as Loop: their overrides log them for
// --log_pause_movie.
// UiMovie::ApplyAnimation through its override.
void Finish(PPCContext& ctx, uint8_t* base);
void FinishNamed(PPCContext& ctx, uint8_t* base);

}  // namespace ui_movies_game_hooks
