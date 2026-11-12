// The type 9 mode's register steps (src/todays_result/,
// docs/functions/todays-result.md), shared by the adapters
// (src/hooks/todays_result.cpp, behind --native_todays_result) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// Whether --native_todays_result is on.
bool TodaysResultNative();

// The mode: its Init (r3 the mode), the delegate functions (r3 the answer,
// r4 the mode), IsLoaded (r3 the mode + 32), FindShown (r3 the block), Kind
// (no arguments) and the two machines' InitStates (r3 the machine).
void ModeType9Init(PPCContext& ctx, uint8_t* base);
void ModeType9WaitForLoads(PPCContext& ctx, uint8_t* base);
void ModeType9NextMode(PPCContext& ctx, uint8_t* base);
void ModeType9LeaveScreen(PPCContext& ctx, uint8_t* base);
void ModeType9WaitForShown(PPCContext& ctx, uint8_t* base);
void ModeType9Leaving(PPCContext& ctx, uint8_t* base);
void ModeType9IsLoaded(PPCContext& ctx, uint8_t* base);
void ModeType9FindShown(PPCContext& ctx, uint8_t* base);
void TodaysEventKind(PPCContext& ctx, uint8_t* base);
void TodaysResultInitStates(PPCContext& ctx, uint8_t* base);
void TodaysEventInitStates(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
