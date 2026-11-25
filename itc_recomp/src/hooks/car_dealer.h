// The car dealer's register steps (src/car_dealer/,
// docs/functions/car-dealer.md), shared by the adapters
// (src/hooks/car_dealer.cpp, behind --native_car_dealer) and the harness
// (without the flag). Each takes the registers its original takes and
// leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The machine's set-up and handlers (r3 the machine, r4 the mode where the
// original reads it).
void CarDealerInitStates(lifted::Regs& ctx);
void CarDealerCreate(lifted::Regs& ctx);
void CarDealerOpenMovie(lifted::Regs& ctx);
void CarDealerWaitForLoad(lifted::Regs& ctx);
void CarDealerAfterDetail(lifted::Regs& ctx);
void CarDealerUnload(lifted::Regs& ctx);
void CarDealerClose(lifted::Regs& ctx);
void CarDealerDestroy(lifted::Regs& ctx);
void CarDealerOnYes(lifted::Regs& ctx);
void CarDealerUpdatePage(lifted::Regs& ctx);
// The pages (r3 the mode).
void CarDealerPage0(lifted::Regs& ctx);
void CarDealerPage1(lifted::Regs& ctx);
void CarDealerPage2(lifted::Regs& ctx);
void CarDealerPage3(lifted::Regs& ctx);
void CarDealerPage4(lifted::Regs& ctx);
// The set-up and the purchase (no arguments).
void CarDealerStartFirstRace(lifted::Regs& ctx);
void CarDealerBuildLists(lifted::Regs& ctx);
void CarDealerBuy(lifted::Regs& ctx);
void CarDealerLeave(lifted::Regs& ctx);
// The transitions (r3 the started word).
void CarDealerTransition3(lifted::Regs& ctx);
void CarDealerTransition0(lifted::Regs& ctx);
void CarDealerTransition4(lifted::Regs& ctx);
void CarDealerTransition5(lifted::Regs& ctx);
void CarDealerTransition8(lifted::Regs& ctx);
void CarDealerTransition9(lifted::Regs& ctx);
void CarDealerTransition1(lifted::Regs& ctx);
void CarDealerTransition6(lifted::Regs& ctx);
void CarDealerTransition11(lifted::Regs& ctx);
void CarDealerTransition16(lifted::Regs& ctx);
void CarDealerTransition19(lifted::Regs& ctx);
void CarDealerTransition7(lifted::Regs& ctx);
void CarDealerTransition15(lifted::Regs& ctx);
void CarDealerTransition17(lifted::Regs& ctx);
// The car's texts and the swatches: ShowCarInfo (r3 the index), ShowSwatches
// and FillStatus (no arguments) with adapters; SetNameTexts (r3 the id, r4
// the maker byte), SetPriceTexts (r3 the price), SetSpecTexts (r3 the
// record) and SetSwatchColor (r3 the node, r4 the record) without (only
// native callers reach them).
void CarDealerShowCarInfo(lifted::Regs& ctx);
void CarDealerShowSwatches(lifted::Regs& ctx);
void CarDealerFillStatus(lifted::Regs& ctx);
void CarDealerSetNameTexts(lifted::Regs& ctx);
void CarDealerSetPriceTexts(lifted::Regs& ctx);
void CarDealerSetSpecTexts(lifted::Regs& ctx);
void CarDealerSetSwatchColor(lifted::Regs& ctx);
// The update, the start and the dialogs' end: Update (r3 the machine),
// Start, OnDialogDone (r4 the mode) with adapters; ShowBusy, HideBusy and
// HideBusyNow without (only Update calls them).
void CarDealerUpdate(lifted::Regs& ctx);
void CarDealerStart(lifted::Regs& ctx);
void CarDealerOnDialogDone(lifted::Regs& ctx);
void CarDealerShowBusy(lifted::Regs& ctx);
void CarDealerHideBusy(lifted::Regs& ctx);
void CarDealerHideBusyNow(lifted::Regs& ctx);
// The menus' openers (no arguments).
void CarDealerOpenListMenu(lifted::Regs& ctx);
void CarDealerOpenCarList(lifted::Regs& ctx);
void CarDealerOpenCar(lifted::Regs& ctx);
void CarDealerOpenColors(lifted::Regs& ctx);
// The helpers.
void CarDealerCheckLimits(lifted::Regs& ctx);
void CarDealerReloadStandModel(lifted::Regs& ctx);
void CarDealerSetKindBar(lifted::Regs& ctx);
void CarDealerSetColorBar(lifted::Regs& ctx);
void CarDealerSetScrollArrows(lifted::Regs& ctx);
void CarDealerSetHeader(lifted::Regs& ctx);
void CarDealerSetPrompts(lifted::Regs& ctx);
void HelpLineSet(lifted::Regs& ctx);

}  // namespace hooks
