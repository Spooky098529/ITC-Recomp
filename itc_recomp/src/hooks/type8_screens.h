// The type 8 panel screen's register steps (src/type8_screens/,
// docs/functions/type8-screens.md), shared by the adapters
// (src/hooks/type8_screens.cpp, behind --native_type8_screens) and the
// harness (without the flag). Each takes the registers its original takes
// (r3 the screen, panel or group, r4 and r5 its arguments) and leaves r3 as
// the original does; guest calls go through `ctx`
// (hooks::Type67ScreensCalls).

#pragma once

#include "guest.h"

namespace hooks {

// The panels and the panel group.
void Type8PanelGroupConstruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8FConstruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8EConstruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8CConstruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8DConstruct(PPCContext& ctx, uint8_t* base);
void Type8ConfirmPanelConstruct(PPCContext& ctx, uint8_t* base);
void Type8PanelGroupDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8CDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8CSetStats(PPCContext& ctx, uint8_t* base);
void Type8Panel8CShowArrows(PPCContext& ctx, uint8_t* base);

// The screen's leaves.
void Type8InitWords(PPCContext& ctx, uint8_t* base);
void Type8IsLoaded(PPCContext& ctx, uint8_t* base);
void Type8OpenMovies(PPCContext& ctx, uint8_t* base);
void Type8DeleteMovies(PPCContext& ctx, uint8_t* base);
void Type8OnMessage2(PPCContext& ctx, uint8_t* base);
void Type8OnSaveLoaded(PPCContext& ctx, uint8_t* base);
void Type8OnAnswer0(PPCContext& ctx, uint8_t* base);
void Type8OnAnswer1(PPCContext& ctx, uint8_t* base);
void Type8OnAnswer2(PPCContext& ctx, uint8_t* base);
void Type8Leave(PPCContext& ctx, uint8_t* base);

// The screen's handlers (ShowPlayerNodes, AskQuit and DrawList have no
// adapters: only the native handlers call them).
void Type8FadeInGroup(PPCContext& ctx, uint8_t* base);
void Type8UpdateOpening(PPCContext& ctx, uint8_t* base);
void Type8Show(PPCContext& ctx, uint8_t* base);
void Type8Draw(PPCContext& ctx, uint8_t* base);
void Type8OnDialogClosed(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
