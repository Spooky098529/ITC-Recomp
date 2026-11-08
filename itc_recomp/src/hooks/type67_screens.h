// The type 6 and 7 modes' list, panel and node screens' register steps
// (src/type67_screens/, docs/functions/type67-screens.md), shared by the
// adapters (src/hooks/type67_screens.cpp, behind --native_type67_screens)
// and the harness (without the flag). Each takes the registers its original
// takes (r3 the screen or object, r4 the dispatcher where the original
// reads it) and leaves r3 as the original does; guest calls go through
// `ctx` (hooks::Type67Calls).

#pragma once

#include "guest.h"
#include "type67_screens/type67_screens.h"

namespace hooks {

// The plain C++'s calls: hooks::Type67Calls, and --native_heap for
// MenuList::Release's plain C++.
type67_screens::Calls Type67ScreensCalls(PPCContext& ctx, uint8_t* base);

// The list screens.
void ListScreenOnEnter(PPCContext& ctx, uint8_t* base);
void ListScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void ListScreenWaitForMovie(PPCContext& ctx, uint8_t* base);
void ListScreenLeave(PPCContext& ctx, uint8_t* base);
void ListScreenOnLeft(PPCContext& ctx, uint8_t* base);
void ListScreenIsLoaded(PPCContext& ctx, uint8_t* base);
void Type6ListScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void Type7ListScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void Type6ListScreenRelease(PPCContext& ctx, uint8_t* base);
void Type7ListScreenRelease(PPCContext& ctx, uint8_t* base);

// The node screen.
void NodeScreenOnEnter(PPCContext& ctx, uint8_t* base);
void NodeScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void NodeScreenWaitForMovie(PPCContext& ctx, uint8_t* base);
void NodeScreenLeave(PPCContext& ctx, uint8_t* base);
void NodeScreenOnLeft(PPCContext& ctx, uint8_t* base);
void Type6NodeScreenIsLoaded(PPCContext& ctx, uint8_t* base);
void Type6NodeScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void Type6NodeScreenRelease(PPCContext& ctx, uint8_t* base);

// The panel groups and the group pair (PanelGroup::Run sub_82259008 has
// no adapter: only GroupPair::Run calls it).
void PanelGroupMovie(PPCContext& ctx, uint8_t* base);
void GroupPairSelectA(PPCContext& ctx, uint8_t* base);
void GroupPairReset(PPCContext& ctx, uint8_t* base);
void GroupPairRun(PPCContext& ctx, uint8_t* base);
void GroupPairWaitForFade(PPCContext& ctx, uint8_t* base);
void GroupPairFinishAll(PPCContext& ctx, uint8_t* base);

// The panel screens' slots and handlers (Type7PanelScreen::DrawList
// sub_82240908 has no adapter: only Type7PanelScreen::Draw calls it).
void PanelScreenIsLoaded(PPCContext& ctx, uint8_t* base);
void Type6PanelScreenOpenMovies(PPCContext& ctx, uint8_t* base);
void Type7PanelScreenOpenMovies(PPCContext& ctx, uint8_t* base);
void PanelScreenDeleteMovies(PPCContext& ctx, uint8_t* base);
void Type7PanelScreenBack(PPCContext& ctx, uint8_t* base);
void Type6PanelScreenOpenPowerGraph(PPCContext& ctx, uint8_t* base);
void Type7PanelScreenOpenPowerGraph(PPCContext& ctx, uint8_t* base);
void Type7PanelScreenCarList(PPCContext& ctx, uint8_t* base);
void Type7PanelScreenCarEntry(PPCContext& ctx, uint8_t* base);
void Type7PanelScreenCarSlot(PPCContext& ctx, uint8_t* base);
void PanelScreenOnEnter(PPCContext& ctx, uint8_t* base);
void PanelScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void PanelScreenWaitForMovie(PPCContext& ctx, uint8_t* base);
void PanelScreenOpen(PPCContext& ctx, uint8_t* base);
void PanelScreenShow(PPCContext& ctx, uint8_t* base);
void PanelScreenUpdate(PPCContext& ctx, uint8_t* base);
void PanelScreenOnMessage24(PPCContext& ctx, uint8_t* base);
void PanelScreenLeave(PPCContext& ctx, uint8_t* base);
void PanelScreenOnLeft(PPCContext& ctx, uint8_t* base);
void PanelScreenDrawList(PPCContext& ctx, uint8_t* base);
void Type7PanelScreenDraw(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
