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
void RaceMenuCreate(lifted::Regs& ctx, uint8_t* base);
void RaceMenuOpenMovie(lifted::Regs& ctx, uint8_t* base);
void RaceMenuWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void RaceMenuOnYes(lifted::Regs& ctx, uint8_t* base);
void RaceMenuOnNo(lifted::Regs& ctx, uint8_t* base);
void RaceMenuOnBack(lifted::Regs& ctx, uint8_t* base);
void RaceMenuOnDialogClosed(lifted::Regs& ctx, uint8_t* base);
void RaceMenuUnload(lifted::Regs& ctx, uint8_t* base);
void RaceMenuLeaveToHub(lifted::Regs& ctx, uint8_t* base);
void RaceMenuDestroy(lifted::Regs& ctx, uint8_t* base);
void RaceMenuStart(lifted::Regs& ctx, uint8_t* base);
// Helpers (r3 the machine). Back's, SetEventRange's, SetUpEvents' and
// Refresh's have no adapters (only the harness runs their steps).
void RaceMenuBack(lifted::Regs& ctx, uint8_t* base);
void RaceMenuSetEventRange(lifted::Regs& ctx, uint8_t* base);
void RaceMenuSetUpEvents(lifted::Regs& ctx, uint8_t* base);
void RaceMenuRefresh(lifted::Regs& ctx, uint8_t* base);
// Update's callees (r3 the machine, r4 the mode where read); none has an adapter
// (only the harness runs their steps).
void RaceMenuTransition0(lifted::Regs& ctx, uint8_t* base);
void RaceMenuTransition3(lifted::Regs& ctx, uint8_t* base);
void RaceMenuTransition5(lifted::Regs& ctx, uint8_t* base);
void RaceMenuAskConfirm(lifted::Regs& ctx, uint8_t* base);
void RaceMenuTransition12(lifted::Regs& ctx, uint8_t* base);
void RaceMenuDebugKindPage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuDebugEventPage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuDebugVariantPage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuAnimateVariantPage(lifted::Regs& ctx, uint8_t* base);
// The pages Draw runs (r3 the machine; r4 the mode for EventPage): no
// adapters (only the harness runs their steps).
void RaceMenuKindPage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuAnimateKindPage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuVariantPage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuEventPage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuAnimateEventPage(lifted::Regs& ctx, uint8_t* base);
// Draw (the (5, 1) functor: r3 the machine, r4 the mode) and its last
// pages (r3 the machine; r4 the mode for LeavePage); only Draw has an
// adapter (the others' steps are the harness's).
void RaceMenuStartEvent(lifted::Regs& ctx, uint8_t* base);
void RaceMenuStartPage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuLeavePage(lifted::Regs& ctx, uint8_t* base);
void RaceMenuDraw(lifted::Regs& ctx, uint8_t* base);
// Update (the (5, 0) functor: r3 the machine, r4 the mode).
void RaceMenuUpdate(lifted::Regs& ctx, uint8_t* base);
// Init (r3 the machine).
void RaceMenuInit(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
