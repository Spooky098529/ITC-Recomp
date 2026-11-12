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

}  // namespace hooks
