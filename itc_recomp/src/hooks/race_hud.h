// The race HUD's adapters that other overrides call (src/hooks/race_hud.cpp).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace race_hud_hooks {

// The rival logo's draw (sub_82211A28; r3 = the logo's entry): natively
// with --native_race_hud, else the original. Its override
// (src/frame_pacing/per_frame_logic.cpp) notes the entry for
// --redraw_rival_logo and calls this.
void DrawRivalLogo(lifted::Regs& ctx);

// For the harnesses only: when set, the draw's override hands each call to
// it instead of drawing (after noting the entry). Set only while a harness
// runs, on the game's thread (the camera modes' Cutscene::Step cases record
// the calls with it). On the SDK's context (the harnesses assign functions
// taking it); the override runs it through lifted::Sdk.
extern void (*g_rival_logo_draw_stand_in)(lifted::Regs& ctx);

}  // namespace race_hud_hooks
