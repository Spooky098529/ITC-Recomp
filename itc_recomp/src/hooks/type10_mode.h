// The type 10 mode's register steps (src/type10_mode/,
// docs/functions/type10-mode.md), shared by the adapters
// (src/hooks/type10_mode.cpp, behind --native_type10_mode) and the harness
// (without the flag). Each takes the registers its original takes and
// leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Whether --native_type10_mode is on.
bool Type10ModeNative();

// The mode: its Init (r3 the mode), the delegate functions (r3 the answer,
// r4 the mode), the +32 interface (r3 the mode + 32, r4 ForceExit's
// number), FindShown (r3 the block) and the theater's InitStates (r3 the
// machine).
void ModeType10Init(lifted::Regs& ctx, uint8_t* base);
void ModeType10WaitForLoads(lifted::Regs& ctx, uint8_t* base);
void ModeType10NextMode(lifted::Regs& ctx, uint8_t* base);
void ModeType10LeaveScreen(lifted::Regs& ctx, uint8_t* base);
void ModeType10WaitForShown(lifted::Regs& ctx, uint8_t* base);
void ModeType10Leaving(lifted::Regs& ctx, uint8_t* base);
void ModeType10NextModeAnswer(lifted::Regs& ctx, uint8_t* base);
void ModeType10Interrupt(lifted::Regs& ctx, uint8_t* base);
void ModeType10ForceExit(lifted::Regs& ctx, uint8_t* base);
void ModeType10Resume(lifted::Regs& ctx, uint8_t* base);
void ModeType10FindShown(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterInitStates(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterOnList(lifted::Regs& ctx, uint8_t* base);
// The theater's handlers (r3 the machine, r4 the dispatcher where read).
void ReplayTheaterCreate(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterOpenMovie(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterStart(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterStartPage(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterDraw(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterUpdate(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterOnDone(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterOnFailed(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterOnRetry(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterOnBack(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterOnInterrupt(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterOnResume(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterUnload(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterWait(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterLeave(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterDestroy(lifted::Regs& ctx, uint8_t* base);
// The pages (r3 the dispatcher) and SetPrompts (r3 to r8).
void ReplayTheaterListPage(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterAskList(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterAskLoad(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterLeavePage(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterWaitPage(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterSetPrompts(lifted::Regs& ctx, uint8_t* base);
// The transitions (r3 the started word's address) and the list's helpers
// (no arguments; ShowRecord r3 the record).
void ReplayTheaterIntro(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterFadeOut(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterFadeInList(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterShowCursor(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterMoveCursor(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterHideCursor(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterPickReplay(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterBuildList(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterShowListPage(lifted::Regs& ctx, uint8_t* base);
void ReplayTheaterShowRecord(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
