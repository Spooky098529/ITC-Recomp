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

}  // namespace hooks
