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
void CarDealerInitStates(lifted::Regs& ctx, uint8_t* base);
void CarDealerCreate(lifted::Regs& ctx, uint8_t* base);
void CarDealerOpenMovie(lifted::Regs& ctx, uint8_t* base);
void CarDealerWaitForLoad(lifted::Regs& ctx, uint8_t* base);
void CarDealerAfterDetail(lifted::Regs& ctx, uint8_t* base);
void CarDealerUnload(lifted::Regs& ctx, uint8_t* base);
void CarDealerClose(lifted::Regs& ctx, uint8_t* base);
void CarDealerDestroy(lifted::Regs& ctx, uint8_t* base);
void CarDealerOnYes(lifted::Regs& ctx, uint8_t* base);
void CarDealerUpdatePage(lifted::Regs& ctx, uint8_t* base);
// The pages (r3 the mode).
void CarDealerPage0(lifted::Regs& ctx, uint8_t* base);
void CarDealerPage1(lifted::Regs& ctx, uint8_t* base);
void CarDealerPage2(lifted::Regs& ctx, uint8_t* base);
void CarDealerPage3(lifted::Regs& ctx, uint8_t* base);
void CarDealerPage4(lifted::Regs& ctx, uint8_t* base);
// The set-up and the purchase (no arguments).
void CarDealerStartFirstRace(lifted::Regs& ctx, uint8_t* base);
void CarDealerBuildLists(lifted::Regs& ctx, uint8_t* base);
void CarDealerBuy(lifted::Regs& ctx, uint8_t* base);
void CarDealerLeave(lifted::Regs& ctx, uint8_t* base);
// The transitions (r3 the started word).
void CarDealerTransition3(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition0(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition4(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition5(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition8(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition9(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition1(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition6(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition11(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition16(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition19(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition7(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition15(lifted::Regs& ctx, uint8_t* base);
void CarDealerTransition17(lifted::Regs& ctx, uint8_t* base);
// The car's texts and the swatches: ShowCarInfo (r3 the index), ShowSwatches
// and FillStatus (no arguments) with adapters; SetNameTexts (r3 the id, r4
// the maker byte), SetPriceTexts (r3 the price), SetSpecTexts (r3 the
// record) and SetSwatchColor (r3 the node, r4 the record) without (only
// native callers reach them).
void CarDealerShowCarInfo(lifted::Regs& ctx, uint8_t* base);
void CarDealerShowSwatches(lifted::Regs& ctx, uint8_t* base);
void CarDealerFillStatus(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetNameTexts(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetPriceTexts(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetSpecTexts(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetSwatchColor(lifted::Regs& ctx, uint8_t* base);
// The update, the start and the dialogs' end: Update (r3 the machine),
// Start, OnDialogDone (r4 the mode) with adapters; ShowBusy, HideBusy and
// HideBusyNow without (only Update calls them).
void CarDealerUpdate(lifted::Regs& ctx, uint8_t* base);
void CarDealerStart(lifted::Regs& ctx, uint8_t* base);
void CarDealerOnDialogDone(lifted::Regs& ctx, uint8_t* base);
void CarDealerShowBusy(lifted::Regs& ctx, uint8_t* base);
void CarDealerHideBusy(lifted::Regs& ctx, uint8_t* base);
void CarDealerHideBusyNow(lifted::Regs& ctx, uint8_t* base);
// The menus' openers (no arguments).
void CarDealerOpenListMenu(lifted::Regs& ctx, uint8_t* base);
void CarDealerOpenCarList(lifted::Regs& ctx, uint8_t* base);
void CarDealerOpenCar(lifted::Regs& ctx, uint8_t* base);
void CarDealerOpenColors(lifted::Regs& ctx, uint8_t* base);
// The helpers.
void CarDealerCheckLimits(lifted::Regs& ctx, uint8_t* base);
void CarDealerReloadStandModel(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetKindBar(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetColorBar(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetScrollArrows(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetHeader(lifted::Regs& ctx, uint8_t* base);
void CarDealerSetPrompts(lifted::Regs& ctx, uint8_t* base);
void HelpLineSet(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
