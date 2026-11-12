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
// Today's result's handlers (r3 the machine, r4 the dispatcher where read),
// its helpers (no arguments), pages (r3 the dispatcher) and transitions (r3
// the started word's address).
void TodaysResultCreate(PPCContext& ctx, uint8_t* base);
void TodaysResultOpenMovie(PPCContext& ctx, uint8_t* base);
void TodaysResultWaitForMovie(PPCContext& ctx, uint8_t* base);
void TodaysResultStart(PPCContext& ctx, uint8_t* base);
void TodaysResultReadDay(PPCContext& ctx, uint8_t* base);
void TodaysResultSetUpMovie(PPCContext& ctx, uint8_t* base);
void TodaysResultSetTexts(PPCContext& ctx, uint8_t* base);
void TodaysResultDraw(PPCContext& ctx, uint8_t* base);
void TodaysResultUpdate(PPCContext& ctx, uint8_t* base);
void TodaysResultWaitPage(PPCContext& ctx, uint8_t* base);
void TodaysResultLeavePage(PPCContext& ctx, uint8_t* base);
void TodaysResultQueueMovie(PPCContext& ctx, uint8_t* base);
void TodaysResultIntro(PPCContext& ctx, uint8_t* base);
void TodaysResultShowLoop(PPCContext& ctx, uint8_t* base);
void TodaysResultFadeOut(PPCContext& ctx, uint8_t* base);
void TodaysResultUnload(PPCContext& ctx, uint8_t* base);
void TodaysResultUnloaded(PPCContext& ctx, uint8_t* base);
void TodaysResultLeave(PPCContext& ctx, uint8_t* base);
void TodaysResultDestroy(PPCContext& ctx, uint8_t* base);
// Today's event's handlers (r3 the machine, r4 the dispatcher where read)
// and helpers (no arguments; LoadImagePair r3 the data and r4 an index;
// ImagesLoadedA and B and FreeImages r3 the data).
void TodaysEventCreate(PPCContext& ctx, uint8_t* base);
void TodaysEventMarkImages(PPCContext& ctx, uint8_t* base);
void TodaysEventOpenMovie(PPCContext& ctx, uint8_t* base);
void TodaysEventLoadImages(PPCContext& ctx, uint8_t* base);
void TodaysEventLoadImagePair(PPCContext& ctx, uint8_t* base);
void TodaysEventWaitForMovie(PPCContext& ctx, uint8_t* base);
void TodaysEventImagesLoadedA(PPCContext& ctx, uint8_t* base);
void TodaysEventImagesLoadedB(PPCContext& ctx, uint8_t* base);
void TodaysEventStart(PPCContext& ctx, uint8_t* base);
void TodaysEventSetUpTexts(PPCContext& ctx, uint8_t* base);
void TodaysEventNextUnlockEntry(PPCContext& ctx, uint8_t* base);
void TodaysEventUnload(PPCContext& ctx, uint8_t* base);
void TodaysEventUnloaded(PPCContext& ctx, uint8_t* base);
void TodaysEventLeave(PPCContext& ctx, uint8_t* base);
void TodaysEventDestroy(PPCContext& ctx, uint8_t* base);
void TodaysEventFreeImages(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
