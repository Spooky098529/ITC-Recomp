// The type 12 mode's register steps (src/network_mode/,
// docs/functions/network-mode.md), shared by the adapters
// (src/hooks/network_mode.cpp, behind --native_network_mode) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Whether --native_network_mode is on.
bool NetworkModeNative();

// sub_82278058 ModeType12::Init (r3 the mode): r3 the second FadeIn's.
void ModeType12Init(lifted::Regs& ctx, uint8_t* base);

// The machines' set-ups: the fourteen InitStates (r3 the machine), the
// eight base Inits (r3 the machine, r4 the functors' object) and
// NetScreen::InitBase (r3 the machine, r4 the index, r5 the object).
void NetModeSelectInitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Screen1InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Screen3InitStates(lifted::Regs& ctx, uint8_t* base);
void NetRankingInitStates(lifted::Regs& ctx, uint8_t* base);
void NetTaCourseInitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Screen6InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Screen7InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Screen8InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Screen9InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Dialog10InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Dialog11InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Dialog12InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Dialog13InitStates(lifted::Regs& ctx, uint8_t* base);
void PowerGraphScreen12InitStates(lifted::Regs& ctx, uint8_t* base);
void Type12Screen6InitBase(lifted::Regs& ctx, uint8_t* base);
void Type12Screen7InitBase(lifted::Regs& ctx, uint8_t* base);
void Type12Screen8InitBase(lifted::Regs& ctx, uint8_t* base);
void Type12Screen9InitBase(lifted::Regs& ctx, uint8_t* base);
void Type12Dialog10InitBase(lifted::Regs& ctx, uint8_t* base);
void Type12Dialog11InitBase(lifted::Regs& ctx, uint8_t* base);
void Type12Dialog12InitBase(lifted::Regs& ctx, uint8_t* base);
void Type12Dialog13InitBase(lifted::Regs& ctx, uint8_t* base);
void NetScreenInitBase(lifted::Regs& ctx, uint8_t* base);

// The block's helpers: Machine (r3 the block, r4 the index; r3 the
// machine, 64 bits as the original adds), FindShown and AllLoaded (r3 the
// block).
void ModeType12Machine(lifted::Regs& ctx, uint8_t* base);
void ModeType12FindShown(lifted::Regs& ctx, uint8_t* base);
void ModeType12AllLoaded(lifted::Regs& ctx, uint8_t* base);
// The delegate functions (r3 the answer, r4 the mode; r3 the answer).
void ModeType12WaitForLoads(lifted::Regs& ctx, uint8_t* base);
void ModeType12NextMode(lifted::Regs& ctx, uint8_t* base);
void ModeType12LeaveScreen(lifted::Regs& ctx, uint8_t* base);
void ModeType12WaitForShown(lifted::Regs& ctx, uint8_t* base);
void ModeType12Teardown(lifted::Regs& ctx, uint8_t* base);
// The +32 slots (r3 the mode + 32) and Session::Free (no arguments).
void ModeType12ForceExit(lifted::Regs& ctx, uint8_t* base);
void ModeType12Resume(lifted::Regs& ctx, uint8_t* base);
void SessionFree(lifted::Regs& ctx, uint8_t* base);

// The machines' set-up and teardown handlers by the machine's index (0, 1
// and 3 to 13; explicitly instantiated for those): Create, WaitForMovie and
// Destroy (r3 the machine), OpenMovie (no arguments; a tail call) and Leave
// (r3 the machine, r4 the dispatcher). r3 as the originals leave it.
template <uint32_t kIndex>
void NetMachineCreate(lifted::Regs& ctx, uint8_t* base);
template <uint32_t kIndex>
void NetMachineOpenMovie(lifted::Regs& ctx, uint8_t* base);
template <uint32_t kIndex>
void NetMachineWaitForMovie(lifted::Regs& ctx, uint8_t* base);
template <uint32_t kIndex>
void NetMachineLeave(lifted::Regs& ctx, uint8_t* base);
template <uint32_t kIndex>
void NetMachineDestroy(lifted::Regs& ctx, uint8_t* base);
// Type12Screen1::ResetData (r3 the data; only Create calls it: no adapter).
void Type12Screen1ResetData(lifted::Regs& ctx, uint8_t* base);

// Session::Make (no arguments; r3 left with a session there) and
// Session::Init (r3 the session): no adapters (only the native MenuPage and
// Session::Make call them).
void SessionMake(lifted::Regs& ctx, uint8_t* base);
void SessionInit(lifted::Regs& ctx, uint8_t* base);

// The mode select's running screen: Start (r3 the machine), SetUp (no
// arguments; only Start calls it: no adapter), Open, Unload (r3 the
// machine), Unloaded (r3 the machine, r4 the dispatcher), Draw (r3 the
// machine, r4 the dispatcher), Update; the pages (r3 the dispatcher), the
// transitions (r3 the started word), SetCursorY (r3 the integer; only
// MoveCursor calls it: no adapter) and the dialogs' replies.
void NetModeSelectStart(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectSetUp(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectOpen(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectDraw(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectUpdate(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectMenuPage(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectLeavePage(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectBattlePage(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectTimeAttackPage(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectSignInPage(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectSignInPage2(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectBackPage(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectIntro(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectShowCursor(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectMoveCursor(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectSetCursorY(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectFadeOut(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectFadeIn(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectOnNo(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectOnYes(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectOnDialogClosed(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectUnload(lifted::Regs& ctx, uint8_t* base);
void NetModeSelectUnloaded(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
