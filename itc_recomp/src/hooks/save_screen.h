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
void SaveScreenPage6(PPCContext& ctx, uint8_t* base);
void SaveScreenPage7(PPCContext& ctx, uint8_t* base);
void SaveScreenPage8(PPCContext& ctx, uint8_t* base);
void SaveScreenPage12(PPCContext& ctx, uint8_t* base);
void SaveScreenPage13(PPCContext& ctx, uint8_t* base);
void SaveScreenPage14(PPCContext& ctx, uint8_t* base);
void SaveScreenPage15(PPCContext& ctx, uint8_t* base);
void SaveScreenPage24(PPCContext& ctx, uint8_t* base);
// SaveScreen::StoreSettings (no arguments; no adapter: only Page6 calls it).
void SaveScreenStoreSettings(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
