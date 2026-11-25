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
void PowerGraphScreen6PostLeave(lifted::Regs& ctx);
void PowerGraphScreen6PostClosing(lifted::Regs& ctx);
void PowerGraphScreen6PostClosed(lifted::Regs& ctx);
void PowerGraphScreen7PostLeave(lifted::Regs& ctx);
void PowerGraphScreen7PostClosing(lifted::Regs& ctx);
// The machine's handlers (r3 the member, r4 the mode where read).
void PowerGraphScreenOnEnter(lifted::Regs& ctx);
void PowerGraphScreenWaitForLoad(lifted::Regs& ctx);
void PowerGraphScreenShown(lifted::Regs& ctx);
void PowerGraphScreenOpen(lifted::Regs& ctx);
void PowerGraphScreenLeave(lifted::Regs& ctx);
void PowerGraphScreenOnLeft(lifted::Regs& ctx);
// The steps (r3 the member, r4 the started word's address).
void PowerGraphScreenStep0(lifted::Regs& ctx);
void PowerGraphScreenStep1(lifted::Regs& ctx);
// SetTexts (r3 the member) and SetParts with its helpers (r4 the record's
// +60, a value, a word array): no adapters (their only callers are ours).
void PowerGraphScreenSetTexts(lifted::Regs& ctx);
void PowerGraphScreenSetParts(lifted::Regs& ctx);
void PowerGraphScreenSetPartIcon(lifted::Regs& ctx);
void PowerGraphScreenSetPartRow2(lifted::Regs& ctx);
void PowerGraphScreenSetPartRow1(lifted::Regs& ctx);
// The pages (r3 the member, r4 the mode).
void PowerGraphScreenMenuPage(lifted::Regs& ctx);
void PowerGraphScreenClosedPage(lifted::Regs& ctx);
// Draw and Update (r3 the member, r4 the mode).
void PowerGraphScreenDraw(lifted::Regs& ctx);
void PowerGraphScreenUpdate(lifted::Regs& ctx);

}  // namespace hooks
