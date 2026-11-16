// The race's loading's register steps (src/race_loading/,
// docs/functions/race-loading.md), shared by the adapters
// (src/hooks/race_loading.cpp, behind --native_race_loading) and the harness
// (without the flag). Each takes the original's registers (r3 the machine,
// r4 the dispatcher for the handlers) and leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void CourseSetupInit(lifted::Regs& ctx, uint8_t* base);           // sub_821517D8
void CourseSetupStart(lifted::Regs& ctx, uint8_t* base);          // sub_82151408
void CourseSetupBegin(lifted::Regs& ctx, uint8_t* base);          // sub_82150DE0
void CourseSetupOnCourseReady(lifted::Regs& ctx, uint8_t* base);  // sub_82150DD0
void CourseSetupWaitDraw(lifted::Regs& ctx, uint8_t* base);       // sub_821515A0
void CourseSetupWaitCourse(lifted::Regs& ctx, uint8_t* base);     // sub_821514A0
void CourseSetupEndTick(lifted::Regs& ctx, uint8_t* base);        // sub_821516F0
void LoadingScreenShow(lifted::Regs& ctx, uint8_t* base);         // sub_822478D0
void LoadingScreenRun(lifted::Regs& ctx, uint8_t* base);          // sub_82247898
void LoadingScreenFinish(lifted::Regs& ctx, uint8_t* base);       // sub_82247A60
void CheckSystemRequest(lifted::Regs& ctx, uint8_t* base);        // sub_82293460

}  // namespace hooks
