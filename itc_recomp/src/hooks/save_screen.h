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
void SaveScreenInit(lifted::Regs& ctx, uint32_t address);
// The machine's handlers (r3 the machine, r4 the mode where read).
void SaveScreenOnEnter(lifted::Regs& ctx);
void SaveScreenOpenMovie(lifted::Regs& ctx);
void SaveScreenWaitMovie(lifted::Regs& ctx);
void SaveScreenOpenKind0(lifted::Regs& ctx);
void SaveScreenOpenKind1(lifted::Regs& ctx);
void SaveScreenOpenKind2(lifted::Regs& ctx);
void SaveScreenOpenKind3(lifted::Regs& ctx);
void SaveScreenOpenKind4(lifted::Regs& ctx);
void SaveScreenOpenKind5(lifted::Regs& ctx);
void SaveScreenOpenKind6(lifted::Regs& ctx);
void SaveScreenOpenKind7(lifted::Regs& ctx);
void SaveScreenShow(lifted::Regs& ctx);
void SaveScreenCancel(lifted::Regs& ctx);
void SaveScreenLeave(lifted::Regs& ctx);
void SaveScreenOnLeft(lifted::Regs& ctx);
void SaveScreenUpdate(lifted::Regs& ctx);
void SaveScreenDraw(lifted::Regs& ctx);
// ShowKind2 and ShowKind7 (no arguments; no adapters: only Show calls them).
void SaveScreenShowKind2(lifted::Regs& ctx);
void SaveScreenShowKind7(lifted::Regs& ctx);
// The dialog steps (r3 the started word).
void SaveScreenMoveSelection(lifted::Regs& ctx);
void SaveScreenCloseDialog(lifted::Regs& ctx);
// The pages (r3 the mode).
void SaveScreenPage0(lifted::Regs& ctx);
void SaveScreenPage1(lifted::Regs& ctx);
void SaveScreenPage2(lifted::Regs& ctx);
void SaveScreenPage3(lifted::Regs& ctx);
void SaveScreenPage4(lifted::Regs& ctx);
void SaveScreenPage9(lifted::Regs& ctx);
void SaveScreenPage16(lifted::Regs& ctx);
void SaveScreenPage25(lifted::Regs& ctx);
void SaveScreenPage6(lifted::Regs& ctx);
void SaveScreenPage7(lifted::Regs& ctx);
void SaveScreenPage8(lifted::Regs& ctx);
void SaveScreenPage12(lifted::Regs& ctx);
void SaveScreenPage13(lifted::Regs& ctx);
void SaveScreenPage14(lifted::Regs& ctx);
void SaveScreenPage15(lifted::Regs& ctx);
void SaveScreenPage24(lifted::Regs& ctx);
// SaveScreen::StoreSettings (no arguments; no adapter: only Page6 calls it).
void SaveScreenStoreSettings(lifted::Regs& ctx);

}  // namespace hooks
