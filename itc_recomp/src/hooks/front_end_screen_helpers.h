// The front end's screen helpers' register steps
// (src/front_end_screen_helpers/,
// docs/functions/front-end-screen-helpers.md), shared by the adapters
// (src/hooks/front_end_screen_helpers.cpp, behind
// --native_front_end_screen_helpers) and the harness (without the flag).
// Each leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The modes' releases (r3 the mode): false, with nothing done, where the
// mode's block isn't the allocation layer's pages (the original must run).
bool FrontEndModeType2Release(lifted::Regs& ctx);
bool FrontEndModeType4Release(lifted::Regs& ctx);
bool FrontEndModeType5Release(lifted::Regs& ctx);
bool FrontEndModeType9Release(lifted::Regs& ctx);
bool FrontEndModeType10Release(lifted::Regs& ctx);
bool FrontEndModeType12Release(lifted::Regs& ctx);

// The dialogs' text setters (r3 the string id, r4 to r10 the varargs).
void FrontEndSaveScreenSetText(lifted::Regs& ctx);
void FrontEndSaveScreenSetNote(lifted::Regs& ctx);
void FrontEndSaveScreenSetMessage(lifted::Regs& ctx);
void FrontEndOptionsSetSaveDialogText(lifted::Regs& ctx);
void FrontEndOptionsSetSaveDialogNote(lifted::Regs& ctx);
void FrontEndOptionsSetCategory6DialogText(lifted::Regs& ctx);
void FrontEndOptionsSetCategory6DialogNote(lifted::Regs& ctx);
// SaveScreen::ShowNotice (r3 the step word).
void FrontEndSaveScreenShowNotice(lifted::Regs& ctx);

// SaveScreen::StartCheck and Start, Options::SetUpSavePage and
// SetUpCategory6Page (no arguments).
void FrontEndSaveScreenStartCheck(lifted::Regs& ctx);
void FrontEndSaveScreenStart(lifted::Regs& ctx);
void FrontEndOptionsSetUpSavePage(lifted::Regs& ctx);
void FrontEndOptionsSetUpCategory6Page(lifted::Regs& ctx);
// The dialogs' steps (r3 the step word): SaveScreen::ShowMessage,
// ShowMessage3 and ShowMessage4, Options::ShowSaveDialog and
// ShowCategory6Dialog.
void FrontEndSaveScreenShowMessage(lifted::Regs& ctx);
void FrontEndSaveScreenShowMessage3(lifted::Regs& ctx);
void FrontEndSaveScreenShowMessage4(lifted::Regs& ctx);
void FrontEndOptionsShowSaveDialog(lifted::Regs& ctx);
void FrontEndOptionsShowCategory6Dialog(lifted::Regs& ctx);
// CarCards::FillAll (no arguments).
void FrontEndCarCardsFillAll(lifted::Regs& ctx);

// The car cards: CarCard::Fill (r3 the node, r4 the entry),
// CarCard::FillDetail, CarDetail::Open (r4 the mode).
void FrontEndCarCardFill(lifted::Regs& ctx);
void FrontEndCarCardFillDetail(lifted::Regs& ctx);
void FrontEndCarDetailOpen(lifted::Regs& ctx);

}  // namespace hooks
