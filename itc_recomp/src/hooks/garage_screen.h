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

}  // namespace hooks
