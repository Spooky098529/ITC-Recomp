// The front end's screen helpers' register steps
// (src/front_end_screen_helpers/,
// docs/functions/front-end-screen-helpers.md), shared by the adapters
// (src/hooks/front_end_screen_helpers.cpp, behind
// --native_front_end_screen_helpers) and the harness (without the flag).
// Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// The modes' releases (r3 the mode): false, with nothing done, where the
// mode's block isn't the allocation layer's pages (the original must run).
bool FrontEndModeType2Release(PPCContext& ctx, uint8_t* base);
bool FrontEndModeType4Release(PPCContext& ctx, uint8_t* base);
bool FrontEndModeType5Release(PPCContext& ctx, uint8_t* base);
bool FrontEndModeType9Release(PPCContext& ctx, uint8_t* base);
bool FrontEndModeType10Release(PPCContext& ctx, uint8_t* base);
bool FrontEndModeType12Release(PPCContext& ctx, uint8_t* base);

// The dialogs' text setters (r3 the string id, r4 to r10 the varargs).
void FrontEndSaveScreenSetText(PPCContext& ctx, uint8_t* base);
void FrontEndSaveScreenSetNote(PPCContext& ctx, uint8_t* base);
void FrontEndSaveScreenSetMessage(PPCContext& ctx, uint8_t* base);
void FrontEndOptionsSetSaveDialogText(PPCContext& ctx, uint8_t* base);
void FrontEndOptionsSetSaveDialogNote(PPCContext& ctx, uint8_t* base);
void FrontEndOptionsSetCategory6DialogText(PPCContext& ctx, uint8_t* base);
void FrontEndOptionsSetCategory6DialogNote(PPCContext& ctx, uint8_t* base);
// SaveScreen::ShowNotice (r3 the step word).
void FrontEndSaveScreenShowNotice(PPCContext& ctx, uint8_t* base);

// The car cards: CarCard::Fill (r3 the node, r4 the entry),
// CarCard::FillDetail, CarDetail::Open (r4 the mode).
void FrontEndCarCardFill(PPCContext& ctx, uint8_t* base);
void FrontEndCarCardFillDetail(PPCContext& ctx, uint8_t* base);
void FrontEndCarDetailOpen(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
