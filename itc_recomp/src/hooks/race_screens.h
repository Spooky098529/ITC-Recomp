// The other in-race screens' register steps (src/race_screens/,
// docs/functions/race-screens.md), shared by the adapters
// (src/hooks/race_screens.cpp and, for the functions logged by
// --log_pause_menu and --log_pause_movie, src/debug/pause_menu_log.cpp and
// pause_movie_log.cpp; behind --native_race_screens) and the harness
// (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// ScreenPages::Init sub_8220BA98 (r3 the page object, r4 the page), Go
// sub_8220BA68 (r4 the page, r5 the transition, r6 the queued one), Finish
// sub_8220BA28 (r4 the result).
void RaceScreensPagesInit(PPCContext& ctx, uint8_t* base);
void RaceScreensPagesGo(PPCContext& ctx, uint8_t* base);
void RaceScreensPagesFinish(PPCContext& ctx, uint8_t* base);
// MenuList:: (r3 the list): Selection sub_8220BC20, SetSelection
// sub_8220BC30 (r4), SetEnabled sub_8220BC00 (r4 the item, r5), IsEnabled
// sub_8220BC10 (r4), Fit sub_8220BC40 (r4 quiet), Fired sub_8220C020 (r4 the
// action), Update sub_8220C1E0, Accepted sub_8220BD50, Cancelled
// sub_8220BDE0, Pressed152 sub_8220BE70, Pressed166 sub_8220BF90.
void RaceScreensListSelection(PPCContext& ctx, uint8_t* base);
void RaceScreensListSetSelection(PPCContext& ctx, uint8_t* base);
void RaceScreensListSetEnabled(PPCContext& ctx, uint8_t* base);
void RaceScreensListIsEnabled(PPCContext& ctx, uint8_t* base);
void RaceScreensListFit(PPCContext& ctx, uint8_t* base);
void RaceScreensListFired(PPCContext& ctx, uint8_t* base);
void RaceScreensListUpdate(PPCContext& ctx, uint8_t* base);
void RaceScreensListAccepted(PPCContext& ctx, uint8_t* base);
void RaceScreensListCancelled(PPCContext& ctx, uint8_t* base);
void RaceScreensListPressed152(PPCContext& ctx, uint8_t* base);
void RaceScreensListPressed166(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
