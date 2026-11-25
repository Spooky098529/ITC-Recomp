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
void ModeType12Init(lifted::Regs& ctx);

// The machines' set-ups: the fourteen InitStates (r3 the machine), the
// eight base Inits (r3 the machine, r4 the functors' object) and
// NetScreen::InitBase (r3 the machine, r4 the index, r5 the object).
void NetModeSelectInitStates(lifted::Regs& ctx);
void Type12Screen1InitStates(lifted::Regs& ctx);
void Type12Screen3InitStates(lifted::Regs& ctx);
void NetRankingInitStates(lifted::Regs& ctx);
void NetTaCourseInitStates(lifted::Regs& ctx);
void Type12Screen6InitStates(lifted::Regs& ctx);
void Type12Screen7InitStates(lifted::Regs& ctx);
void Type12Screen8InitStates(lifted::Regs& ctx);
void Type12Screen9InitStates(lifted::Regs& ctx);
void Type12Dialog10InitStates(lifted::Regs& ctx);
void Type12Dialog11InitStates(lifted::Regs& ctx);
void Type12Dialog12InitStates(lifted::Regs& ctx);
void Type12Dialog13InitStates(lifted::Regs& ctx);
void PowerGraphScreen12InitStates(lifted::Regs& ctx);
void Type12Screen6InitBase(lifted::Regs& ctx);
void Type12Screen7InitBase(lifted::Regs& ctx);
void Type12Screen8InitBase(lifted::Regs& ctx);
void Type12Screen9InitBase(lifted::Regs& ctx);
void Type12Dialog10InitBase(lifted::Regs& ctx);
void Type12Dialog11InitBase(lifted::Regs& ctx);
void Type12Dialog12InitBase(lifted::Regs& ctx);
void Type12Dialog13InitBase(lifted::Regs& ctx);
void NetScreenInitBase(lifted::Regs& ctx);

// The block's helpers: Machine (r3 the block, r4 the index; r3 the
// machine, 64 bits as the original adds), FindShown and AllLoaded (r3 the
// block).
void ModeType12Machine(lifted::Regs& ctx);
void ModeType12FindShown(lifted::Regs& ctx);
void ModeType12AllLoaded(lifted::Regs& ctx);
// The delegate functions (r3 the answer, r4 the mode; r3 the answer).
void ModeType12WaitForLoads(lifted::Regs& ctx);
void ModeType12NextMode(lifted::Regs& ctx);
void ModeType12LeaveScreen(lifted::Regs& ctx);
void ModeType12WaitForShown(lifted::Regs& ctx);
void ModeType12Teardown(lifted::Regs& ctx);
// The +32 slots (r3 the mode + 32) and Session::Free (no arguments).
void ModeType12ForceExit(lifted::Regs& ctx);
void ModeType12Resume(lifted::Regs& ctx);
void SessionFree(lifted::Regs& ctx);

// The machines' set-up and teardown handlers by the machine's index (0, 1
// and 3 to 13; explicitly instantiated for those): Create, WaitForMovie and
// Destroy (r3 the machine), OpenMovie (no arguments; a tail call) and Leave
// (r3 the machine, r4 the dispatcher). r3 as the originals leave it.
template <uint32_t kIndex>
void NetMachineCreate(lifted::Regs& ctx);
template <uint32_t kIndex>
void NetMachineOpenMovie(lifted::Regs& ctx);
template <uint32_t kIndex>
void NetMachineWaitForMovie(lifted::Regs& ctx);
template <uint32_t kIndex>
void NetMachineLeave(lifted::Regs& ctx);
template <uint32_t kIndex>
void NetMachineDestroy(lifted::Regs& ctx);
// Type12Screen1::ResetData (r3 the data; only Create calls it: no adapter).
void Type12Screen1ResetData(lifted::Regs& ctx);

// Session::Make (no arguments; r3 left with a session there) and
// Session::Init (r3 the session): no adapters (only the native MenuPage and
// Session::Make call them).
void SessionMake(lifted::Regs& ctx);
void SessionInit(lifted::Regs& ctx);

// The mode select's running screen: Start (r3 the machine), SetUp (no
// arguments; only Start calls it: no adapter), Open, Unload (r3 the
// machine), Unloaded (r3 the machine, r4 the dispatcher), Draw (r3 the
// machine, r4 the dispatcher), Update; the pages (r3 the dispatcher), the
// transitions (r3 the started word), SetCursorY (r3 the integer; only
// MoveCursor calls it: no adapter) and the dialogs' replies.
void NetModeSelectStart(lifted::Regs& ctx);
void NetModeSelectSetUp(lifted::Regs& ctx);
void NetModeSelectOpen(lifted::Regs& ctx);
void NetModeSelectDraw(lifted::Regs& ctx);
void NetModeSelectUpdate(lifted::Regs& ctx);
void NetModeSelectMenuPage(lifted::Regs& ctx);
void NetModeSelectLeavePage(lifted::Regs& ctx);
void NetModeSelectBattlePage(lifted::Regs& ctx);
void NetModeSelectTimeAttackPage(lifted::Regs& ctx);
void NetModeSelectSignInPage(lifted::Regs& ctx);
void NetModeSelectSignInPage2(lifted::Regs& ctx);
void NetModeSelectBackPage(lifted::Regs& ctx);
void NetModeSelectIntro(lifted::Regs& ctx);
void NetModeSelectShowCursor(lifted::Regs& ctx);
void NetModeSelectMoveCursor(lifted::Regs& ctx);
void NetModeSelectSetCursorY(lifted::Regs& ctx);
void NetModeSelectFadeOut(lifted::Regs& ctx);
void NetModeSelectFadeIn(lifted::Regs& ctx);
void NetModeSelectOnNo(lifted::Regs& ctx);
void NetModeSelectOnYes(lifted::Regs& ctx);
void NetModeSelectOnDialogClosed(lifted::Regs& ctx);
void NetModeSelectUnload(lifted::Regs& ctx);
void NetModeSelectUnloaded(lifted::Regs& ctx);

}  // namespace hooks
