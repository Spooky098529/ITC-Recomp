// The save screen's register steps (src/save_screen/,
// docs/functions/save-screen.md), shared by the adapters
// (src/hooks/save_screen.cpp, behind --native_save_screen) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The state machine Inits (r3 the machine): `address` one of
// save_screen::kInits.
void SaveScreenInit(lifted::Regs& ctx, uint8_t* base, uint32_t address);
// The machine's handlers (r3 the machine, r4 the mode where read).
void SaveScreenOnEnter(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void SaveScreenWaitMovie(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenKind0(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenKind1(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenKind2(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenKind3(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenKind4(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenKind5(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenKind6(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOpenKind7(lifted::Regs& ctx, uint8_t* base);
void SaveScreenShow(lifted::Regs& ctx, uint8_t* base);
void SaveScreenCancel(lifted::Regs& ctx, uint8_t* base);
void SaveScreenLeave(lifted::Regs& ctx, uint8_t* base);
void SaveScreenOnLeft(lifted::Regs& ctx, uint8_t* base);
void SaveScreenUpdate(lifted::Regs& ctx, uint8_t* base);
void SaveScreenDraw(lifted::Regs& ctx, uint8_t* base);
// ShowKind2 and ShowKind7 (no arguments; no adapters: only Show calls them).
void SaveScreenShowKind2(lifted::Regs& ctx, uint8_t* base);
void SaveScreenShowKind7(lifted::Regs& ctx, uint8_t* base);
// The dialog steps (r3 the started word).
void SaveScreenMoveSelection(lifted::Regs& ctx, uint8_t* base);
void SaveScreenCloseDialog(lifted::Regs& ctx, uint8_t* base);
// The pages (r3 the mode).
void SaveScreenPage0(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage1(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage2(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage3(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage4(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage9(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage16(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage25(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage6(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage7(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage8(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage12(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage13(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage14(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage15(lifted::Regs& ctx, uint8_t* base);
void SaveScreenPage24(lifted::Regs& ctx, uint8_t* base);
// SaveScreen::StoreSettings (no arguments; no adapter: only Page6 calls it).
void SaveScreenStoreSettings(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
