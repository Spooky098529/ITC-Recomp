// The garage screen's register steps (src/garage_screen/,
// docs/functions/garage-screen.md), shared by the adapters
// (src/hooks/garage_screen.cpp, behind --native_garage_screen) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The movie 5 helpers (SetCarRow r3 a row node and r4 a garage car;
// SetPrompts r3 to r8; SetHelpLine r3 a string id; the others no
// arguments).
void GarageScreenSetCarRow(lifted::Regs& ctx, uint8_t* base);
void GarageScreenFillRows(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetPrompts(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetHelpLine(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetUpLists(lifted::Regs& ctx, uint8_t* base);
// The transitions seen in play (r3 the started word's address).
void GarageScreenTransition0(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition1(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition3(lifted::Regs& ctx, uint8_t* base);
// The pages seen in play (r3 the mode).
void GarageScreenCarListPage(lifted::Regs& ctx, uint8_t* base);
void GarageScreenLeavePage(lifted::Regs& ctx, uint8_t* base);
// UpdatePage and Update (r3 the machine, r4 the mode).
void GarageScreenUpdatePage(lifted::Regs& ctx, uint8_t* base);
void GarageScreenUpdate(lifted::Regs& ctx, uint8_t* base);
// The machine's handlers (r3 the machine and r4 the mode where read;
// ReadSave, SetHeader, Start and Destroy no arguments).
void GarageScreenCreate(lifted::Regs& ctx, uint8_t* base);
void GarageScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void GarageScreenWaitForLoad(lifted::Regs& ctx, uint8_t* base);
void GarageScreenReadSave(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetHeader(lifted::Regs& ctx, uint8_t* base);
void GarageScreenStart(lifted::Regs& ctx, uint8_t* base);
void GarageScreenUnload(lifted::Regs& ctx, uint8_t* base);
void GarageScreenClose(lifted::Regs& ctx, uint8_t* base);
void GarageScreenDestroy(lifted::Regs& ctx, uint8_t* base);
// The texts, markers and stand helpers never seen in play
// (SetScrollArrows r3 the scroll, r4 the count; the markers r3 the index;
// ShowSpecTexts r3 a record; ShowCarNames r3 an id, r4 a maker;
// CopyGarageCar r3 out, r4 a garage, r5 an index; CarValue and SalePrice
// r3 a car; the others no arguments).
void GarageScreenSetScrollArrows(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetActionMarker(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetOptionMarker(lifted::Regs& ctx, uint8_t* base);
void GarageScreenShowSpecTexts(lifted::Regs& ctx, uint8_t* base);
void GarageScreenShowCarNames(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetPlateQuestion(lifted::Regs& ctx, uint8_t* base);
void GarageScreenCopyGarageCar(lifted::Regs& ctx, uint8_t* base);
void GarageScreenShowBusy(lifted::Regs& ctx, uint8_t* base);
void GarageScreenHideBusy(lifted::Regs& ctx, uint8_t* base);
void GarageScreenHideBusyNow(lifted::Regs& ctx, uint8_t* base);
void GarageScreenResetStand(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetUpView(lifted::Regs& ctx, uint8_t* base);
void GarageScreenToggleTurn(lifted::Regs& ctx, uint8_t* base);
void GarageScreenToggleDoors(lifted::Regs& ctx, uint8_t* base);
void GarageScreenToggleDoors2(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTurnLeft(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTurnRight(lifted::Regs& ctx, uint8_t* base);
void GarageScreenCarValue(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSalePrice(lifted::Regs& ctx, uint8_t* base);
// The car objects never seen in play (r3 a garage car but ReloadStandCar,
// PutCarOnStand and the cameras: no arguments).
void GarageScreenReloadStandCar(lifted::Regs& ctx, uint8_t* base);
void GarageScreenPutCarOnStand(lifted::Regs& ctx, uint8_t* base);
void GarageScreenShowCarSpecs(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetUseQuestion(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetSaleQuestion(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetUpPlateEditor(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetUpSpecSheet(lifted::Regs& ctx, uint8_t* base);
void GarageScreenCameraA(lifted::Regs& ctx, uint8_t* base);
void GarageScreenCameraB(lifted::Regs& ctx, uint8_t* base);
// The transitions never seen in play (r3 the started word) and the
// actions' and options' set-ups (no arguments).
void GarageScreenTransition2(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition4(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition5(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition6(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition7(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition8(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition9(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition10(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition12(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition13(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition14(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition16(lifted::Regs& ctx, uint8_t* base);
void GarageScreenTransition17(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetUpActions(lifted::Regs& ctx, uint8_t* base);
void GarageScreenSetUpOptions(lifted::Regs& ctx, uint8_t* base);
// The pages never seen in play (r3 the mode) and the reply handlers (r3
// the machine, r4 the mode).
void GarageScreenActionsPage(lifted::Regs& ctx, uint8_t* base);
void GarageScreenViewPage(lifted::Regs& ctx, uint8_t* base);
void GarageScreenViewOptionsPage(lifted::Regs& ctx, uint8_t* base);
void GarageScreenOnSpecSheetClosed(lifted::Regs& ctx, uint8_t* base);
void GarageScreenOnPlateCancelled(lifted::Regs& ctx, uint8_t* base);
void GarageScreenOnPlateDone(lifted::Regs& ctx, uint8_t* base);
void GarageScreenOnPlateEditorLeft(lifted::Regs& ctx, uint8_t* base);
void GarageScreenOnNo(lifted::Regs& ctx, uint8_t* base);
void GarageScreenOnDialogDone(lifted::Regs& ctx, uint8_t* base);
// InitStates (r3 the machine).
void GarageScreenInitStates(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
