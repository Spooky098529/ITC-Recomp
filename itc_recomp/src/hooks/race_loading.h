// The race's loading's register steps (src/race_loading/,
// docs/functions/race-loading.md), shared by the adapters
// (src/hooks/race_loading.cpp, behind --native_race_loading) and the harness
// (without the flag). Each takes the original's registers (r3 the machine,
// r4 the dispatcher for the handlers) and leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

void CourseSetupInit(PPCContext& ctx, uint8_t* base);           // sub_821517D8
void CourseSetupStart(PPCContext& ctx, uint8_t* base);          // sub_82151408
void CourseSetupBegin(PPCContext& ctx, uint8_t* base);          // sub_82150DE0
void CourseSetupOnCourseReady(PPCContext& ctx, uint8_t* base);  // sub_82150DD0
void CourseSetupWaitDraw(PPCContext& ctx, uint8_t* base);       // sub_821515A0
void LoadingScreenShow(PPCContext& ctx, uint8_t* base);         // sub_822478D0
void LoadingScreenRun(PPCContext& ctx, uint8_t* base);          // sub_82247898
void LoadingScreenFinish(PPCContext& ctx, uint8_t* base);       // sub_82247A60
void CheckSystemRequest(PPCContext& ctx, uint8_t* base);        // sub_82293460

}  // namespace hooks
