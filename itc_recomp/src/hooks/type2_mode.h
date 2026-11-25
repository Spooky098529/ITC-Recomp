// The type 2 mode's register steps (src/type2_mode/,
// docs/functions/type2-mode.md), shared by the adapters
// (src/hooks/type2_mode.cpp, behind --native_type2_mode) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The block's helpers (r3 the block, r4 an index for MachineOf).
void Type2ModeMachineOf(lifted::Regs& ctx);
void Type2ModeAllLoaded(lifted::Regs& ctx);
void Type2ModeFindShown(lifted::Regs& ctx);
// The delegate functions (r3 the answer, r4 the mode).
void Type2ModeTearDown(lifted::Regs& ctx);
void Type2ModeLeaveScreen(lifted::Regs& ctx);
void Type2ModeAfterLeaveScreen(lifted::Regs& ctx);
void Type2ModeRestart(lifted::Regs& ctx);
void Type2ModeLeaving(lifted::Regs& ctx);
void Type2ModeNextMode(lifted::Regs& ctx);
void Type2ModeWaitForLoads(lifted::Regs& ctx);
// The slot thunks (r3 the answer, r4 the object).
void ModeSlot8Thunk(lifted::Regs& ctx);
void ModeSlot9Thunk(lifted::Regs& ctx);
void ModeSlot10Thunk(lifted::Regs& ctx);
void ModeSlot11Thunk(lifted::Regs& ctx);
void ModeSlot12Thunk(lifted::Regs& ctx);
void ModeSlot13Thunk(lifted::Regs& ctx);
// The +32 interface (r3 the mode + 32; r4 a number for ForceExit).
void Type2ModeInterrupt(lifted::Regs& ctx);
void Type2ModeForceExit(lifted::Regs& ctx);
// Init (r3 the mode).
void Type2ModeInit(lifted::Regs& ctx);

}  // namespace hooks
