// The type 2 screens' register steps (src/type2_screens/,
// docs/functions/type2-screens.md), shared by the adapters
// (src/hooks/type2_screens.cpp, behind --native_type2_screens) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// Shared by four machines (r3 the machine, r4 the mode).
void ScreenLeaveToHub(PPCContext& ctx, uint8_t* base);
// The license plate editor's handlers (r3 the machine, r4 the mode where
// the original reads it).
void PlateCreate(PPCContext& ctx, uint8_t* base);
void PlateOpenMovie(PPCContext& ctx, uint8_t* base);
void PlateWaitForLoad(PPCContext& ctx, uint8_t* base);
void PlateClose(PPCContext& ctx, uint8_t* base);
void PlateDestroy(PPCContext& ctx, uint8_t* base);
void PlateOnMessage32(PPCContext& ctx, uint8_t* base);
void PlateStart(PPCContext& ctx, uint8_t* base);
// The pages (r3 the mode).
void PlateLeavePage(PPCContext& ctx, uint8_t* base);
// The helpers: CharLineKind (r3 the characters), RandomPlate (r4 the
// plate), LoadPlate and ResetMovie (no arguments), ShowRegion (r3 the
// region), ShowDigits and ShowChars (r3 the words), MarkField (r3 the
// field), FinishRegionAnim and FinishDigitAnim (no arguments).
void PlateCharLineKind(PPCContext& ctx, uint8_t* base);
void PlateRandom(PPCContext& ctx, uint8_t* base);
void PlateLoad(PPCContext& ctx, uint8_t* base);
void PlateResetMovie(PPCContext& ctx, uint8_t* base);
void PlateShowRegion(PPCContext& ctx, uint8_t* base);
void PlateShowDigits(PPCContext& ctx, uint8_t* base);
void PlateShowChars(PPCContext& ctx, uint8_t* base);
void PlateMarkField(PPCContext& ctx, uint8_t* base);
void PlateFinishRegionAnim(PPCContext& ctx, uint8_t* base);
void PlateFinishDigitAnim(PPCContext& ctx, uint8_t* base);
// The cells and menus: ShowRegionCells, ShowDigitCells, ShowCharCells and
// the three menus' set-ups (no arguments), ShowCharLine (r3 the kind),
// ShowPlate (r3 the plate).
void PlateShowRegionCells(PPCContext& ctx, uint8_t* base);
void PlateShowDigitCells(PPCContext& ctx, uint8_t* base);
void PlateShowCharCells(PPCContext& ctx, uint8_t* base);
void PlateShowCharLine(PPCContext& ctx, uint8_t* base);
void PlateShowPlate(PPCContext& ctx, uint8_t* base);
void PlateSetUpRegionMenu(PPCContext& ctx, uint8_t* base);
void PlateSetUpNumberMenu(PPCContext& ctx, uint8_t* base);
void PlateSetUpCharsMenu(PPCContext& ctx, uint8_t* base);
// The transitions (r3 the started word's address).
void PlateTransition0(PPCContext& ctx, uint8_t* base);
void PlateTransition1(PPCContext& ctx, uint8_t* base);
void PlateTransition4(PPCContext& ctx, uint8_t* base);
void PlateTransition5(PPCContext& ctx, uint8_t* base);
void PlateTransition6(PPCContext& ctx, uint8_t* base);
void PlateTransition8(PPCContext& ctx, uint8_t* base);
void PlateTransition10(PPCContext& ctx, uint8_t* base);
void PlateTransition11(PPCContext& ctx, uint8_t* base);
void PlateTransition13(PPCContext& ctx, uint8_t* base);
void PlateTransition19(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
