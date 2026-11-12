// The type 12 mode's register steps (src/network_mode/,
// docs/functions/network-mode.md), shared by the adapters
// (src/hooks/network_mode.cpp, behind --native_network_mode) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// Whether --native_network_mode is on.
bool NetworkModeNative();

// sub_82278058 ModeType12::Init (r3 the mode): r3 the second FadeIn's.
void ModeType12Init(PPCContext& ctx, uint8_t* base);

// The machines' set-ups: the fourteen InitStates (r3 the machine), the
// eight base Inits (r3 the machine, r4 the functors' object) and
// NetScreen::InitBase (r3 the machine, r4 the index, r5 the object).
void NetModeSelectInitStates(PPCContext& ctx, uint8_t* base);
void Type12Screen1InitStates(PPCContext& ctx, uint8_t* base);
void Type12Screen3InitStates(PPCContext& ctx, uint8_t* base);
void NetRankingInitStates(PPCContext& ctx, uint8_t* base);
void NetTaCourseInitStates(PPCContext& ctx, uint8_t* base);
void Type12Screen6InitStates(PPCContext& ctx, uint8_t* base);
void Type12Screen7InitStates(PPCContext& ctx, uint8_t* base);
void Type12Screen8InitStates(PPCContext& ctx, uint8_t* base);
void Type12Screen9InitStates(PPCContext& ctx, uint8_t* base);
void Type12Dialog10InitStates(PPCContext& ctx, uint8_t* base);
void Type12Dialog11InitStates(PPCContext& ctx, uint8_t* base);
void Type12Dialog12InitStates(PPCContext& ctx, uint8_t* base);
void Type12Dialog13InitStates(PPCContext& ctx, uint8_t* base);
void PowerGraphScreen12InitStates(PPCContext& ctx, uint8_t* base);
void Type12Screen6InitBase(PPCContext& ctx, uint8_t* base);
void Type12Screen7InitBase(PPCContext& ctx, uint8_t* base);
void Type12Screen8InitBase(PPCContext& ctx, uint8_t* base);
void Type12Screen9InitBase(PPCContext& ctx, uint8_t* base);
void Type12Dialog10InitBase(PPCContext& ctx, uint8_t* base);
void Type12Dialog11InitBase(PPCContext& ctx, uint8_t* base);
void Type12Dialog12InitBase(PPCContext& ctx, uint8_t* base);
void Type12Dialog13InitBase(PPCContext& ctx, uint8_t* base);
void NetScreenInitBase(PPCContext& ctx, uint8_t* base);

// The block's helpers: Machine (r3 the block, r4 the index; r3 the
// machine, 64 bits as the original adds), FindShown and AllLoaded (r3 the
// block).
void ModeType12Machine(PPCContext& ctx, uint8_t* base);
void ModeType12FindShown(PPCContext& ctx, uint8_t* base);
void ModeType12AllLoaded(PPCContext& ctx, uint8_t* base);
// The delegate functions (r3 the answer, r4 the mode; r3 the answer).
void ModeType12WaitForLoads(PPCContext& ctx, uint8_t* base);
void ModeType12NextMode(PPCContext& ctx, uint8_t* base);
void ModeType12LeaveScreen(PPCContext& ctx, uint8_t* base);
void ModeType12WaitForShown(PPCContext& ctx, uint8_t* base);
void ModeType12Teardown(PPCContext& ctx, uint8_t* base);
// The +32 slots (r3 the mode + 32) and Session::Free (no arguments).
void ModeType12ForceExit(PPCContext& ctx, uint8_t* base);
void ModeType12Resume(PPCContext& ctx, uint8_t* base);
void SessionFree(PPCContext& ctx, uint8_t* base);

// The machines' set-up and teardown handlers by the machine's index (0, 1
// and 3 to 13; explicitly instantiated for those): Create, WaitForMovie and
// Destroy (r3 the machine), OpenMovie (no arguments; a tail call) and Leave
// (r3 the machine, r4 the dispatcher). r3 as the originals leave it.
template <uint32_t kIndex>
void NetMachineCreate(PPCContext& ctx, uint8_t* base);
template <uint32_t kIndex>
void NetMachineOpenMovie(PPCContext& ctx, uint8_t* base);
template <uint32_t kIndex>
void NetMachineWaitForMovie(PPCContext& ctx, uint8_t* base);
template <uint32_t kIndex>
void NetMachineLeave(PPCContext& ctx, uint8_t* base);
template <uint32_t kIndex>
void NetMachineDestroy(PPCContext& ctx, uint8_t* base);
// Type12Screen1::ResetData (r3 the data; only Create calls it: no adapter).
void Type12Screen1ResetData(PPCContext& ctx, uint8_t* base);

// Session::Make (no arguments; r3 left with a session there) and
// Session::Init (r3 the session): no adapters (only the native MenuPage and
// Session::Make call them).
void SessionMake(PPCContext& ctx, uint8_t* base);
void SessionInit(PPCContext& ctx, uint8_t* base);

// The mode select's running screen: Start (r3 the machine), SetUp (no
// arguments; only Start calls it: no adapter), Open, Unload (r3 the
// machine), Unloaded (r3 the machine, r4 the dispatcher), Draw (r3 the
// machine, r4 the dispatcher), Update; the pages (r3 the dispatcher), the
// transitions (r3 the started word), SetCursorY (r3 the integer; only
// MoveCursor calls it: no adapter) and the dialogs' replies.
void NetModeSelectStart(PPCContext& ctx, uint8_t* base);
void NetModeSelectSetUp(PPCContext& ctx, uint8_t* base);
void NetModeSelectOpen(PPCContext& ctx, uint8_t* base);
void NetModeSelectDraw(PPCContext& ctx, uint8_t* base);
void NetModeSelectUpdate(PPCContext& ctx, uint8_t* base);
void NetModeSelectMenuPage(PPCContext& ctx, uint8_t* base);
void NetModeSelectLeavePage(PPCContext& ctx, uint8_t* base);
void NetModeSelectBattlePage(PPCContext& ctx, uint8_t* base);
void NetModeSelectTimeAttackPage(PPCContext& ctx, uint8_t* base);
void NetModeSelectSignInPage(PPCContext& ctx, uint8_t* base);
void NetModeSelectSignInPage2(PPCContext& ctx, uint8_t* base);
void NetModeSelectBackPage(PPCContext& ctx, uint8_t* base);
void NetModeSelectIntro(PPCContext& ctx, uint8_t* base);
void NetModeSelectShowCursor(PPCContext& ctx, uint8_t* base);
void NetModeSelectMoveCursor(PPCContext& ctx, uint8_t* base);
void NetModeSelectSetCursorY(PPCContext& ctx, uint8_t* base);
void NetModeSelectFadeOut(PPCContext& ctx, uint8_t* base);
void NetModeSelectFadeIn(PPCContext& ctx, uint8_t* base);
void NetModeSelectOnNo(PPCContext& ctx, uint8_t* base);
void NetModeSelectOnYes(PPCContext& ctx, uint8_t* base);
void NetModeSelectOnDialogClosed(PPCContext& ctx, uint8_t* base);
void NetModeSelectUnload(PPCContext& ctx, uint8_t* base);
void NetModeSelectUnloaded(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
