// The UI movie library's adapters that other overrides call
// (src/hooks/ui_movies.cpp).

#pragma once

#include "guest.h"

namespace ui_movies_hooks {

// sub_8259DAC0's work (r4 = a key; r3 not read): sub_8259D7B0 with r3 = r4
// and f1 = the float at 0x82001C00, as a tail call. Natively or, with
// --native_ui_movies off, the original. Its override (WATCH_MOVIE_CALL's,
// src/debug/pause_movie_log.cpp) wraps this for --log_pause_movie.
void ResetAfterMovie(PPCContext& ctx, uint8_t* base);

}  // namespace ui_movies_hooks
