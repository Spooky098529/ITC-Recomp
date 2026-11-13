// The type 10 mode's register steps (src/type10_mode/,
// docs/functions/type10-mode.md), shared by the adapters
// (src/hooks/type10_mode.cpp, behind --native_type10_mode) and the harness
// (without the flag). Each takes the registers its original takes and
// leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// Whether --native_type10_mode is on.
bool Type10ModeNative();

// The mode: its Init (r3 the mode), the delegate functions (r3 the answer,
// r4 the mode), the +32 interface (r3 the mode + 32, r4 ForceExit's
// number), FindShown (r3 the block) and the theater's InitStates (r3 the
// machine).
void ModeType10Init(PPCContext& ctx, uint8_t* base);
void ModeType10WaitForLoads(PPCContext& ctx, uint8_t* base);
void ModeType10NextMode(PPCContext& ctx, uint8_t* base);
void ModeType10LeaveScreen(PPCContext& ctx, uint8_t* base);
void ModeType10WaitForShown(PPCContext& ctx, uint8_t* base);
void ModeType10Leaving(PPCContext& ctx, uint8_t* base);
void ModeType10NextModeAnswer(PPCContext& ctx, uint8_t* base);
void ModeType10Interrupt(PPCContext& ctx, uint8_t* base);
void ModeType10ForceExit(PPCContext& ctx, uint8_t* base);
void ModeType10Resume(PPCContext& ctx, uint8_t* base);
void ModeType10FindShown(PPCContext& ctx, uint8_t* base);
void ReplayTheaterInitStates(PPCContext& ctx, uint8_t* base);
void ReplayTheaterOnList(PPCContext& ctx, uint8_t* base);
// The theater's handlers (r3 the machine, r4 the dispatcher where read).
void ReplayTheaterCreate(PPCContext& ctx, uint8_t* base);
void ReplayTheaterOpenMovie(PPCContext& ctx, uint8_t* base);
void ReplayTheaterWaitForMovie(PPCContext& ctx, uint8_t* base);
void ReplayTheaterStart(PPCContext& ctx, uint8_t* base);
void ReplayTheaterStartPage(PPCContext& ctx, uint8_t* base);
void ReplayTheaterDraw(PPCContext& ctx, uint8_t* base);
void ReplayTheaterUpdate(PPCContext& ctx, uint8_t* base);
void ReplayTheaterOnDone(PPCContext& ctx, uint8_t* base);
void ReplayTheaterOnFailed(PPCContext& ctx, uint8_t* base);
void ReplayTheaterOnRetry(PPCContext& ctx, uint8_t* base);
void ReplayTheaterOnBack(PPCContext& ctx, uint8_t* base);
void ReplayTheaterOnInterrupt(PPCContext& ctx, uint8_t* base);
void ReplayTheaterOnResume(PPCContext& ctx, uint8_t* base);
void ReplayTheaterUnload(PPCContext& ctx, uint8_t* base);
void ReplayTheaterWait(PPCContext& ctx, uint8_t* base);
void ReplayTheaterLeave(PPCContext& ctx, uint8_t* base);
void ReplayTheaterDestroy(PPCContext& ctx, uint8_t* base);
// The pages (r3 the dispatcher) and SetPrompts (r3 to r8).
void ReplayTheaterListPage(PPCContext& ctx, uint8_t* base);
void ReplayTheaterAskList(PPCContext& ctx, uint8_t* base);
void ReplayTheaterAskLoad(PPCContext& ctx, uint8_t* base);
void ReplayTheaterLeavePage(PPCContext& ctx, uint8_t* base);
void ReplayTheaterWaitPage(PPCContext& ctx, uint8_t* base);
void ReplayTheaterSetPrompts(PPCContext& ctx, uint8_t* base);
// The transitions (r3 the started word's address) and the list's helpers
// (no arguments; ShowRecord r3 the record).
void ReplayTheaterIntro(PPCContext& ctx, uint8_t* base);
void ReplayTheaterFadeOut(PPCContext& ctx, uint8_t* base);
void ReplayTheaterFadeInList(PPCContext& ctx, uint8_t* base);
void ReplayTheaterShowCursor(PPCContext& ctx, uint8_t* base);
void ReplayTheaterMoveCursor(PPCContext& ctx, uint8_t* base);
void ReplayTheaterHideCursor(PPCContext& ctx, uint8_t* base);
void ReplayTheaterPickReplay(PPCContext& ctx, uint8_t* base);
void ReplayTheaterBuildList(PPCContext& ctx, uint8_t* base);
void ReplayTheaterShowListPage(PPCContext& ctx, uint8_t* base);
void ReplayTheaterShowRecord(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
