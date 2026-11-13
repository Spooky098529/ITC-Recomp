// The power graph screen's register steps (src/power_graph_screen/,
// docs/functions/power-graph-screen.md), shared by the adapters
// (src/hooks/power_graph_screen.cpp, behind --native_power_graph_screen) and
// the harness (without the flag). Each takes the registers its original
// takes and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The members' vtable slots (r4 the mode).
void PowerGraphScreen6PostLeave(PPCContext& ctx, uint8_t* base);
void PowerGraphScreen6PostClosing(PPCContext& ctx, uint8_t* base);
void PowerGraphScreen6PostClosed(PPCContext& ctx, uint8_t* base);
void PowerGraphScreen7PostLeave(PPCContext& ctx, uint8_t* base);
void PowerGraphScreen7PostClosing(PPCContext& ctx, uint8_t* base);
// The machine's handlers (r3 the member, r4 the mode where read).
void PowerGraphScreenOnEnter(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenWaitForLoad(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenShown(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenOpen(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenLeave(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenOnLeft(PPCContext& ctx, uint8_t* base);
// The steps (r3 the member, r4 the started word's address).
void PowerGraphScreenStep0(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenStep1(PPCContext& ctx, uint8_t* base);
// SetTexts (r3 the member) and SetParts with its helpers (r4 the record's
// +60, a value, a word array): no adapters (their only callers are ours).
void PowerGraphScreenSetTexts(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenSetParts(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenSetPartIcon(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenSetPartRow2(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenSetPartRow1(PPCContext& ctx, uint8_t* base);
// The pages (r3 the member, r4 the mode).
void PowerGraphScreenMenuPage(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenClosedPage(PPCContext& ctx, uint8_t* base);
// Draw and Update (r3 the member, r4 the mode).
void PowerGraphScreenDraw(PPCContext& ctx, uint8_t* base);
void PowerGraphScreenUpdate(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
