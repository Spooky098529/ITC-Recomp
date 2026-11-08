// The car dealer's register steps (src/car_dealer/,
// docs/functions/car-dealer.md), shared by the adapters
// (src/hooks/car_dealer.cpp, behind --native_car_dealer) and the harness
// (without the flag). Each takes the registers its original takes and
// leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The machine's set-up and handlers (r3 the machine, r4 the mode where the
// original reads it).
void CarDealerInitStates(PPCContext& ctx, uint8_t* base);
void CarDealerCreate(PPCContext& ctx, uint8_t* base);
void CarDealerOpenMovie(PPCContext& ctx, uint8_t* base);
void CarDealerWaitForLoad(PPCContext& ctx, uint8_t* base);
void CarDealerAfterDetail(PPCContext& ctx, uint8_t* base);
void CarDealerUnload(PPCContext& ctx, uint8_t* base);
void CarDealerClose(PPCContext& ctx, uint8_t* base);
void CarDealerDestroy(PPCContext& ctx, uint8_t* base);
void CarDealerOnYes(PPCContext& ctx, uint8_t* base);
void CarDealerUpdatePage(PPCContext& ctx, uint8_t* base);
// The pages (r3 the mode).
void CarDealerPage0(PPCContext& ctx, uint8_t* base);
void CarDealerPage1(PPCContext& ctx, uint8_t* base);
void CarDealerPage2(PPCContext& ctx, uint8_t* base);
void CarDealerPage3(PPCContext& ctx, uint8_t* base);
void CarDealerPage4(PPCContext& ctx, uint8_t* base);
// The set-up and the purchase (no arguments).
void CarDealerStartFirstRace(PPCContext& ctx, uint8_t* base);
void CarDealerBuildLists(PPCContext& ctx, uint8_t* base);
void CarDealerBuy(PPCContext& ctx, uint8_t* base);
void CarDealerLeave(PPCContext& ctx, uint8_t* base);
// The transitions (r3 the started word).
void CarDealerTransition3(PPCContext& ctx, uint8_t* base);
// The car's texts and the swatches: ShowCarInfo (r3 the index), ShowSwatches
// and FillStatus (no arguments) with adapters; SetNameTexts (r3 the id, r4
// the maker byte), SetPriceTexts (r3 the price), SetSpecTexts (r3 the
// record) and SetSwatchColor (r3 the node, r4 the record) without (only
// native callers reach them).
void CarDealerShowCarInfo(PPCContext& ctx, uint8_t* base);
void CarDealerShowSwatches(PPCContext& ctx, uint8_t* base);
void CarDealerFillStatus(PPCContext& ctx, uint8_t* base);
void CarDealerSetNameTexts(PPCContext& ctx, uint8_t* base);
void CarDealerSetPriceTexts(PPCContext& ctx, uint8_t* base);
void CarDealerSetSpecTexts(PPCContext& ctx, uint8_t* base);
void CarDealerSetSwatchColor(PPCContext& ctx, uint8_t* base);
// The update, the start and the dialogs' end: Update (r3 the machine),
// Start, OnDialogDone (r4 the mode) with adapters; ShowBusy, HideBusy and
// HideBusyNow without (only Update calls them).
void CarDealerUpdate(PPCContext& ctx, uint8_t* base);
void CarDealerStart(PPCContext& ctx, uint8_t* base);
void CarDealerOnDialogDone(PPCContext& ctx, uint8_t* base);
void CarDealerShowBusy(PPCContext& ctx, uint8_t* base);
void CarDealerHideBusy(PPCContext& ctx, uint8_t* base);
void CarDealerHideBusyNow(PPCContext& ctx, uint8_t* base);
// The helpers.
void CarDealerCheckLimits(PPCContext& ctx, uint8_t* base);
void CarDealerReloadStandModel(PPCContext& ctx, uint8_t* base);
void CarDealerSetKindBar(PPCContext& ctx, uint8_t* base);
void CarDealerSetColorBar(PPCContext& ctx, uint8_t* base);
void CarDealerSetScrollArrows(PPCContext& ctx, uint8_t* base);
void CarDealerSetHeader(PPCContext& ctx, uint8_t* base);
void CarDealerSetPrompts(PPCContext& ctx, uint8_t* base);
void HelpLineSet(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
