// The race menu's register steps (src/race_menu/,
// docs/functions/race-menu.md), shared by the adapters
// (src/hooks/race_menu.cpp, behind --native_race_menu) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The machine's handlers (r3 the machine, r4 the mode where read).
void RaceMenuCreate(lifted::Regs& ctx);
void RaceMenuOpenMovie(lifted::Regs& ctx);
void RaceMenuWaitForMovie(lifted::Regs& ctx);
void RaceMenuOnYes(lifted::Regs& ctx);
void RaceMenuOnNo(lifted::Regs& ctx);
void RaceMenuOnBack(lifted::Regs& ctx);
void RaceMenuOnDialogClosed(lifted::Regs& ctx);
void RaceMenuUnload(lifted::Regs& ctx);
void RaceMenuLeaveToHub(lifted::Regs& ctx);
void RaceMenuDestroy(lifted::Regs& ctx);
void RaceMenuStart(lifted::Regs& ctx);
// Helpers (r3 the machine). Back's, SetEventRange's, SetUpEvents' and
// Refresh's have no adapters (only the harness runs their steps).
void RaceMenuBack(lifted::Regs& ctx);
void RaceMenuSetEventRange(lifted::Regs& ctx);
void RaceMenuSetUpEvents(lifted::Regs& ctx);
void RaceMenuRefresh(lifted::Regs& ctx);
// Update's callees (r3 the machine, r4 the mode where read); none has an adapter
// (only the harness runs their steps).
void RaceMenuTransition0(lifted::Regs& ctx);
void RaceMenuTransition3(lifted::Regs& ctx);
void RaceMenuTransition5(lifted::Regs& ctx);
void RaceMenuAskConfirm(lifted::Regs& ctx);
void RaceMenuTransition12(lifted::Regs& ctx);
void RaceMenuDebugKindPage(lifted::Regs& ctx);
void RaceMenuDebugEventPage(lifted::Regs& ctx);
void RaceMenuDebugVariantPage(lifted::Regs& ctx);
void RaceMenuAnimateVariantPage(lifted::Regs& ctx);
// The pages Draw runs (r3 the machine; r4 the mode for EventPage): no
// adapters (only the harness runs their steps).
void RaceMenuKindPage(lifted::Regs& ctx);
void RaceMenuAnimateKindPage(lifted::Regs& ctx);
void RaceMenuVariantPage(lifted::Regs& ctx);
void RaceMenuEventPage(lifted::Regs& ctx);
void RaceMenuAnimateEventPage(lifted::Regs& ctx);
// Draw (the (5, 1) functor: r3 the machine, r4 the mode) and its last
// pages (r3 the machine; r4 the mode for LeavePage); only Draw has an
// adapter (the others' steps are the harness's).
void RaceMenuStartEvent(lifted::Regs& ctx);
void RaceMenuStartPage(lifted::Regs& ctx);
void RaceMenuLeavePage(lifted::Regs& ctx);
void RaceMenuDraw(lifted::Regs& ctx);
// Update (the (5, 0) functor: r3 the machine, r4 the mode).
void RaceMenuUpdate(lifted::Regs& ctx);
// Init (r3 the machine).
void RaceMenuInit(lifted::Regs& ctx);

}  // namespace hooks
