// The string table's and the font's adapters, for other code
// (src/hooks/strings_and_font.cpp).

#pragma once

#include "guest.h"

namespace hooks {

// For harnesses: while set, this runs instead of Text::Draw (sub_8210A758)
// from its adapter, so a harness sees what its callers hand it (the UI
// movies' texts: src/harnesses/ui_movies_game.cpp) without drawing.
using GuestStandIn = void (*)(PPCContext&, uint8_t*);
extern GuestStandIn g_text_draw_stand_in;

// Strings::FormatV (sub_8210A990)'s register steps without the flag (the
// adapter's and the harness's): r3 = the table, r4 = the output, r5 = the
// id, r6 = the va_list; r3 the output.
void StringsFormatVSteps(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
