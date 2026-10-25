// The race HUD's adapters that other overrides call (src/hooks/race_hud.cpp).

#pragma once

#include "guest.h"

namespace race_hud_hooks {

// The rival logo's draw (sub_82211A28; r3 = the logo's entry): natively
// with --native_race_hud, else the original. Its override
// (src/frame_pacing/per_frame_logic.cpp) notes the entry for
// --redraw_rival_logo and calls this.
void DrawRivalLogo(PPCContext& ctx, uint8_t* base);

}  // namespace race_hud_hooks
