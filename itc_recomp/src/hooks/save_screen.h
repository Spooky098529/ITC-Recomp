// The save screen's register steps (src/save_screen/,
// docs/functions/save-screen.md), shared by the adapters
// (src/hooks/save_screen.cpp, behind --native_save_screen) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The dialog steps (r3 the started word).
void SaveScreenMoveSelection(PPCContext& ctx, uint8_t* base);
void SaveScreenCloseDialog(PPCContext& ctx, uint8_t* base);
// The pages (r3 the mode).
void SaveScreenPage0(PPCContext& ctx, uint8_t* base);
void SaveScreenPage1(PPCContext& ctx, uint8_t* base);
void SaveScreenPage2(PPCContext& ctx, uint8_t* base);
void SaveScreenPage3(PPCContext& ctx, uint8_t* base);
void SaveScreenPage4(PPCContext& ctx, uint8_t* base);
void SaveScreenPage9(PPCContext& ctx, uint8_t* base);
void SaveScreenPage16(PPCContext& ctx, uint8_t* base);
void SaveScreenPage25(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
