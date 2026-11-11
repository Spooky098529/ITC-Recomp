// The type 2 mode's register steps (src/type2_mode/,
// docs/functions/type2-mode.md), shared by the adapters
// (src/hooks/type2_mode.cpp, behind --native_type2_mode) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The block's helpers (r3 the block, r4 an index for MachineOf).
void Type2ModeMachineOf(PPCContext& ctx, uint8_t* base);
void Type2ModeAllLoaded(PPCContext& ctx, uint8_t* base);
void Type2ModeFindShown(PPCContext& ctx, uint8_t* base);
// The delegate functions (r3 the answer, r4 the mode).
void Type2ModeTearDown(PPCContext& ctx, uint8_t* base);
void Type2ModeLeaveScreen(PPCContext& ctx, uint8_t* base);
void Type2ModeAfterLeaveScreen(PPCContext& ctx, uint8_t* base);
void Type2ModeRestart(PPCContext& ctx, uint8_t* base);
void Type2ModeLeaving(PPCContext& ctx, uint8_t* base);
void Type2ModeNextMode(PPCContext& ctx, uint8_t* base);
void Type2ModeWaitForLoads(PPCContext& ctx, uint8_t* base);
// The slot thunks (r3 the answer, r4 the object).
void ModeSlot8Thunk(PPCContext& ctx, uint8_t* base);
void ModeSlot9Thunk(PPCContext& ctx, uint8_t* base);
void ModeSlot10Thunk(PPCContext& ctx, uint8_t* base);
void ModeSlot11Thunk(PPCContext& ctx, uint8_t* base);
void ModeSlot12Thunk(PPCContext& ctx, uint8_t* base);
void ModeSlot13Thunk(PPCContext& ctx, uint8_t* base);
// The +32 interface (r3 the mode + 32; r4 a number for ForceExit).
void Type2ModeInterrupt(PPCContext& ctx, uint8_t* base);
void Type2ModeForceExit(PPCContext& ctx, uint8_t* base);
// Init (r3 the mode).
void Type2ModeInit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
