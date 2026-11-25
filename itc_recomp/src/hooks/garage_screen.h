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
void GarageScreenSetCarRow(lifted::Regs& ctx);
void GarageScreenFillRows(lifted::Regs& ctx);
void GarageScreenSetPrompts(lifted::Regs& ctx);
void GarageScreenSetHelpLine(lifted::Regs& ctx);
void GarageScreenSetUpLists(lifted::Regs& ctx);
// The transitions seen in play (r3 the started word's address).
void GarageScreenTransition0(lifted::Regs& ctx);
void GarageScreenTransition1(lifted::Regs& ctx);
void GarageScreenTransition3(lifted::Regs& ctx);
// The pages seen in play (r3 the mode).
void GarageScreenCarListPage(lifted::Regs& ctx);
void GarageScreenLeavePage(lifted::Regs& ctx);
// UpdatePage and Update (r3 the machine, r4 the mode).
void GarageScreenUpdatePage(lifted::Regs& ctx);
void GarageScreenUpdate(lifted::Regs& ctx);
// The machine's handlers (r3 the machine and r4 the mode where read;
// ReadSave, SetHeader, Start and Destroy no arguments).
void GarageScreenCreate(lifted::Regs& ctx);
void GarageScreenOpenMovie(lifted::Regs& ctx);
void GarageScreenWaitForLoad(lifted::Regs& ctx);
void GarageScreenReadSave(lifted::Regs& ctx);
void GarageScreenSetHeader(lifted::Regs& ctx);
void GarageScreenStart(lifted::Regs& ctx);
void GarageScreenUnload(lifted::Regs& ctx);
void GarageScreenClose(lifted::Regs& ctx);
void GarageScreenDestroy(lifted::Regs& ctx);
// The texts, markers and stand helpers never seen in play
// (SetScrollArrows r3 the scroll, r4 the count; the markers r3 the index;
// ShowSpecTexts r3 a record; ShowCarNames r3 an id, r4 a maker;
// CopyGarageCar r3 out, r4 a garage, r5 an index; CarValue and SalePrice
// r3 a car; the others no arguments).
void GarageScreenSetScrollArrows(lifted::Regs& ctx);
void GarageScreenSetActionMarker(lifted::Regs& ctx);
void GarageScreenSetOptionMarker(lifted::Regs& ctx);
void GarageScreenShowSpecTexts(lifted::Regs& ctx);
void GarageScreenShowCarNames(lifted::Regs& ctx);
void GarageScreenSetPlateQuestion(lifted::Regs& ctx);
void GarageScreenCopyGarageCar(lifted::Regs& ctx);
void GarageScreenShowBusy(lifted::Regs& ctx);
void GarageScreenHideBusy(lifted::Regs& ctx);
void GarageScreenHideBusyNow(lifted::Regs& ctx);
void GarageScreenResetStand(lifted::Regs& ctx);
void GarageScreenSetUpView(lifted::Regs& ctx);
void GarageScreenToggleTurn(lifted::Regs& ctx);
void GarageScreenToggleDoors(lifted::Regs& ctx);
void GarageScreenToggleDoors2(lifted::Regs& ctx);
void GarageScreenTurnLeft(lifted::Regs& ctx);
void GarageScreenTurnRight(lifted::Regs& ctx);
void GarageScreenCarValue(lifted::Regs& ctx);
void GarageScreenSalePrice(lifted::Regs& ctx);
// The car objects never seen in play (r3 a garage car but ReloadStandCar,
// PutCarOnStand and the cameras: no arguments).
void GarageScreenReloadStandCar(lifted::Regs& ctx);
void GarageScreenPutCarOnStand(lifted::Regs& ctx);
void GarageScreenShowCarSpecs(lifted::Regs& ctx);
void GarageScreenSetUseQuestion(lifted::Regs& ctx);
void GarageScreenSetSaleQuestion(lifted::Regs& ctx);
void GarageScreenSetUpPlateEditor(lifted::Regs& ctx);
void GarageScreenSetUpSpecSheet(lifted::Regs& ctx);
void GarageScreenCameraA(lifted::Regs& ctx);
void GarageScreenCameraB(lifted::Regs& ctx);
// The transitions never seen in play (r3 the started word) and the
// actions' and options' set-ups (no arguments).
void GarageScreenTransition2(lifted::Regs& ctx);
void GarageScreenTransition4(lifted::Regs& ctx);
void GarageScreenTransition5(lifted::Regs& ctx);
void GarageScreenTransition6(lifted::Regs& ctx);
void GarageScreenTransition7(lifted::Regs& ctx);
void GarageScreenTransition8(lifted::Regs& ctx);
void GarageScreenTransition9(lifted::Regs& ctx);
void GarageScreenTransition10(lifted::Regs& ctx);
void GarageScreenTransition12(lifted::Regs& ctx);
void GarageScreenTransition13(lifted::Regs& ctx);
void GarageScreenTransition14(lifted::Regs& ctx);
void GarageScreenTransition16(lifted::Regs& ctx);
void GarageScreenTransition17(lifted::Regs& ctx);
void GarageScreenSetUpActions(lifted::Regs& ctx);
void GarageScreenSetUpOptions(lifted::Regs& ctx);
// The pages never seen in play (r3 the mode) and the reply handlers (r3
// the machine, r4 the mode).
void GarageScreenActionsPage(lifted::Regs& ctx);
void GarageScreenViewPage(lifted::Regs& ctx);
void GarageScreenViewOptionsPage(lifted::Regs& ctx);
void GarageScreenOnSpecSheetClosed(lifted::Regs& ctx);
void GarageScreenOnPlateCancelled(lifted::Regs& ctx);
void GarageScreenOnPlateDone(lifted::Regs& ctx);
void GarageScreenOnPlateEditorLeft(lifted::Regs& ctx);
void GarageScreenOnNo(lifted::Regs& ctx);
void GarageScreenOnDialogDone(lifted::Regs& ctx);
// InitStates (r3 the machine).
void GarageScreenInitStates(lifted::Regs& ctx);

}  // namespace hooks
