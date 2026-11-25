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
void ModeType10Init(lifted::Regs& ctx);
void ModeType10WaitForLoads(lifted::Regs& ctx);
void ModeType10NextMode(lifted::Regs& ctx);
void ModeType10LeaveScreen(lifted::Regs& ctx);
void ModeType10WaitForShown(lifted::Regs& ctx);
void ModeType10Leaving(lifted::Regs& ctx);
void ModeType10NextModeAnswer(lifted::Regs& ctx);
void ModeType10Interrupt(lifted::Regs& ctx);
void ModeType10ForceExit(lifted::Regs& ctx);
void ModeType10Resume(lifted::Regs& ctx);
void ModeType10FindShown(lifted::Regs& ctx);
void ReplayTheaterInitStates(lifted::Regs& ctx);
void ReplayTheaterOnList(lifted::Regs& ctx);
// The theater's handlers (r3 the machine, r4 the dispatcher where read).
void ReplayTheaterCreate(lifted::Regs& ctx);
void ReplayTheaterOpenMovie(lifted::Regs& ctx);
void ReplayTheaterWaitForMovie(lifted::Regs& ctx);
void ReplayTheaterStart(lifted::Regs& ctx);
void ReplayTheaterStartPage(lifted::Regs& ctx);
void ReplayTheaterDraw(lifted::Regs& ctx);
void ReplayTheaterUpdate(lifted::Regs& ctx);
void ReplayTheaterOnDone(lifted::Regs& ctx);
void ReplayTheaterOnFailed(lifted::Regs& ctx);
void ReplayTheaterOnRetry(lifted::Regs& ctx);
void ReplayTheaterOnBack(lifted::Regs& ctx);
void ReplayTheaterOnInterrupt(lifted::Regs& ctx);
void ReplayTheaterOnResume(lifted::Regs& ctx);
void ReplayTheaterUnload(lifted::Regs& ctx);
void ReplayTheaterWait(lifted::Regs& ctx);
void ReplayTheaterLeave(lifted::Regs& ctx);
void ReplayTheaterDestroy(lifted::Regs& ctx);
// The pages (r3 the dispatcher) and SetPrompts (r3 to r8).
void ReplayTheaterListPage(lifted::Regs& ctx);
void ReplayTheaterAskList(lifted::Regs& ctx);
void ReplayTheaterAskLoad(lifted::Regs& ctx);
void ReplayTheaterLeavePage(lifted::Regs& ctx);
void ReplayTheaterWaitPage(lifted::Regs& ctx);
void ReplayTheaterSetPrompts(lifted::Regs& ctx);
// The transitions (r3 the started word's address) and the list's helpers
// (no arguments; ShowRecord r3 the record).
void ReplayTheaterIntro(lifted::Regs& ctx);
void ReplayTheaterFadeOut(lifted::Regs& ctx);
void ReplayTheaterFadeInList(lifted::Regs& ctx);
void ReplayTheaterShowCursor(lifted::Regs& ctx);
void ReplayTheaterMoveCursor(lifted::Regs& ctx);
void ReplayTheaterHideCursor(lifted::Regs& ctx);
void ReplayTheaterPickReplay(lifted::Regs& ctx);
void ReplayTheaterBuildList(lifted::Regs& ctx);
void ReplayTheaterShowListPage(lifted::Regs& ctx);
void ReplayTheaterShowRecord(lifted::Regs& ctx);

}  // namespace hooks
