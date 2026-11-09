// The garage screen's register steps (src/garage_screen/,
// docs/functions/garage-screen.md), shared by the adapters
// (src/hooks/garage_screen.cpp, behind --native_garage_screen) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The movie 5 helpers (SetCarRow r3 a row node and r4 a garage car;
// SetPrompts r3 to r8; SetHelpLine r3 a string id; the others no
// arguments).
void GarageScreenSetCarRow(PPCContext& ctx, uint8_t* base);
void GarageScreenFillRows(PPCContext& ctx, uint8_t* base);
void GarageScreenSetPrompts(PPCContext& ctx, uint8_t* base);
void GarageScreenSetHelpLine(PPCContext& ctx, uint8_t* base);
void GarageScreenSetUpLists(PPCContext& ctx, uint8_t* base);
// The transitions seen in play (r3 the started word's address).
void GarageScreenTransition0(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition1(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition3(PPCContext& ctx, uint8_t* base);
// The pages seen in play (r3 the mode).
void GarageScreenCarListPage(PPCContext& ctx, uint8_t* base);
void GarageScreenLeavePage(PPCContext& ctx, uint8_t* base);
// UpdatePage and Update (r3 the machine, r4 the mode).
void GarageScreenUpdatePage(PPCContext& ctx, uint8_t* base);
void GarageScreenUpdate(PPCContext& ctx, uint8_t* base);
// The machine's handlers (r3 the machine and r4 the mode where read;
// ReadSave, SetHeader, Start and Destroy no arguments).
void GarageScreenCreate(PPCContext& ctx, uint8_t* base);
void GarageScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void GarageScreenWaitForLoad(PPCContext& ctx, uint8_t* base);
void GarageScreenReadSave(PPCContext& ctx, uint8_t* base);
void GarageScreenSetHeader(PPCContext& ctx, uint8_t* base);
void GarageScreenStart(PPCContext& ctx, uint8_t* base);
void GarageScreenUnload(PPCContext& ctx, uint8_t* base);
void GarageScreenClose(PPCContext& ctx, uint8_t* base);
void GarageScreenDestroy(PPCContext& ctx, uint8_t* base);
// The texts, markers and stand helpers never seen in play
// (SetScrollArrows r3 the scroll, r4 the count; the markers r3 the index;
// ShowSpecTexts r3 a record; ShowCarNames r3 an id, r4 a maker;
// CopyGarageCar r3 out, r4 a garage, r5 an index; CarValue and SalePrice
// r3 a car; the others no arguments).
void GarageScreenSetScrollArrows(PPCContext& ctx, uint8_t* base);
void GarageScreenSetActionMarker(PPCContext& ctx, uint8_t* base);
void GarageScreenSetOptionMarker(PPCContext& ctx, uint8_t* base);
void GarageScreenShowSpecTexts(PPCContext& ctx, uint8_t* base);
void GarageScreenShowCarNames(PPCContext& ctx, uint8_t* base);
void GarageScreenSetPlateQuestion(PPCContext& ctx, uint8_t* base);
void GarageScreenCopyGarageCar(PPCContext& ctx, uint8_t* base);
void GarageScreenShowBusy(PPCContext& ctx, uint8_t* base);
void GarageScreenHideBusy(PPCContext& ctx, uint8_t* base);
void GarageScreenHideBusyNow(PPCContext& ctx, uint8_t* base);
void GarageScreenResetStand(PPCContext& ctx, uint8_t* base);
void GarageScreenSetUpView(PPCContext& ctx, uint8_t* base);
void GarageScreenToggleTurn(PPCContext& ctx, uint8_t* base);
void GarageScreenToggleDoors(PPCContext& ctx, uint8_t* base);
void GarageScreenToggleDoors2(PPCContext& ctx, uint8_t* base);
void GarageScreenTurnLeft(PPCContext& ctx, uint8_t* base);
void GarageScreenTurnRight(PPCContext& ctx, uint8_t* base);
void GarageScreenCarValue(PPCContext& ctx, uint8_t* base);
void GarageScreenSalePrice(PPCContext& ctx, uint8_t* base);
// The car objects never seen in play (r3 a garage car but ReloadStandCar,
// PutCarOnStand and the cameras: no arguments).
void GarageScreenReloadStandCar(PPCContext& ctx, uint8_t* base);
void GarageScreenPutCarOnStand(PPCContext& ctx, uint8_t* base);
void GarageScreenShowCarSpecs(PPCContext& ctx, uint8_t* base);
void GarageScreenSetUseQuestion(PPCContext& ctx, uint8_t* base);
void GarageScreenSetSaleQuestion(PPCContext& ctx, uint8_t* base);
void GarageScreenSetUpPlateEditor(PPCContext& ctx, uint8_t* base);
void GarageScreenSetUpSpecSheet(PPCContext& ctx, uint8_t* base);
void GarageScreenCameraA(PPCContext& ctx, uint8_t* base);
void GarageScreenCameraB(PPCContext& ctx, uint8_t* base);
// The transitions never seen in play (r3 the started word) and the
// actions' and options' set-ups (no arguments).
void GarageScreenTransition2(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition4(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition5(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition6(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition7(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition8(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition9(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition10(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition12(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition13(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition14(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition16(PPCContext& ctx, uint8_t* base);
void GarageScreenTransition17(PPCContext& ctx, uint8_t* base);
void GarageScreenSetUpActions(PPCContext& ctx, uint8_t* base);
void GarageScreenSetUpOptions(PPCContext& ctx, uint8_t* base);
// InitStates (r3 the machine).
void GarageScreenInitStates(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
