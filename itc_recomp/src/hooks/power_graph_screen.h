// The power graph screen's register steps (src/power_graph_screen/,
// docs/functions/power-graph-screen.md), shared by the adapters
// (src/hooks/power_graph_screen.cpp, behind --native_power_graph_screen) and
// the harness (without the flag). Each takes the registers its original
// takes and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The members' vtable slots (r4 the mode).
void PowerGraphScreen6PostLeave(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreen6PostClosing(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreen6PostClosed(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreen7PostLeave(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreen7PostClosing(lifted::Regs& ctx, uint8_t* base);
// The machine's handlers (r3 the member, r4 the mode where read).
void PowerGraphScreenOnEnter(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenWaitForLoad(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenShown(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenOpen(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenLeave(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenOnLeft(lifted::Regs& ctx, uint8_t* base);
// The steps (r3 the member, r4 the started word's address).
void PowerGraphScreenStep0(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenStep1(lifted::Regs& ctx, uint8_t* base);
// SetTexts (r3 the member) and SetParts with its helpers (r4 the record's
// +60, a value, a word array): no adapters (their only callers are ours).
void PowerGraphScreenSetTexts(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenSetParts(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenSetPartIcon(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenSetPartRow2(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenSetPartRow1(lifted::Regs& ctx, uint8_t* base);
// The pages (r3 the member, r4 the mode).
void PowerGraphScreenMenuPage(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenClosedPage(lifted::Regs& ctx, uint8_t* base);
// Draw and Update (r3 the member, r4 the mode).
void PowerGraphScreenDraw(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreenUpdate(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
