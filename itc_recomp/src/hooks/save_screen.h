// The save screen's register steps (src/save_screen/,
// docs/functions/save-screen.md), shared by the adapters
// (src/hooks/save_screen.cpp, behind --native_save_screen) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The machine's handlers (r3 the machine, r4 the mode where read).
void SaveScreenOnEnter(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void SaveScreenWaitMovie(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenKind0(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenKind1(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenKind2(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenKind3(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenKind4(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenKind5(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenKind6(PPCContext& ctx, uint8_t* base);
void SaveScreenOpenKind7(PPCContext& ctx, uint8_t* base);
void SaveScreenShow(PPCContext& ctx, uint8_t* base);
void SaveScreenCancel(PPCContext& ctx, uint8_t* base);
void SaveScreenLeave(PPCContext& ctx, uint8_t* base);
void SaveScreenOnLeft(PPCContext& ctx, uint8_t* base);
void SaveScreenUpdate(PPCContext& ctx, uint8_t* base);
void SaveScreenDraw(PPCContext& ctx, uint8_t* base);
// ShowKind2 and ShowKind7 (no arguments; no adapters: only Show calls them).
void SaveScreenShowKind2(PPCContext& ctx, uint8_t* base);
void SaveScreenShowKind7(PPCContext& ctx, uint8_t* base);
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
