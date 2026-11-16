// The type 9 mode's register steps (src/todays_result/,
// docs/functions/todays-result.md), shared by the adapters
// (src/hooks/todays_result.cpp, behind --native_todays_result) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Whether --native_todays_result is on.
bool TodaysResultNative();

// The mode: its Init (r3 the mode), the delegate functions (r3 the answer,
// r4 the mode), IsLoaded (r3 the mode + 32), FindShown (r3 the block), Kind
// (no arguments) and the two machines' InitStates (r3 the machine).
void ModeType9Init(lifted::Regs& ctx, uint8_t* base);
void ModeType9WaitForLoads(lifted::Regs& ctx, uint8_t* base);
void ModeType9NextMode(lifted::Regs& ctx, uint8_t* base);
void ModeType9LeaveScreen(lifted::Regs& ctx, uint8_t* base);
void ModeType9WaitForShown(lifted::Regs& ctx, uint8_t* base);
void ModeType9Leaving(lifted::Regs& ctx, uint8_t* base);
void ModeType9IsLoaded(lifted::Regs& ctx, uint8_t* base);
void ModeType9FindShown(lifted::Regs& ctx, uint8_t* base);
void TodaysEventKind(lifted::Regs& ctx, uint8_t* base);
void TodaysResultInitStates(lifted::Regs& ctx, uint8_t* base);
void TodaysEventInitStates(lifted::Regs& ctx, uint8_t* base);
// Today's result's handlers (r3 the machine, r4 the dispatcher where read),
// its helpers (no arguments), pages (r3 the dispatcher) and transitions (r3
// the started word's address).
void TodaysResultCreate(lifted::Regs& ctx, uint8_t* base);
void TodaysResultOpenMovie(lifted::Regs& ctx, uint8_t* base);
void TodaysResultWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void TodaysResultStart(lifted::Regs& ctx, uint8_t* base);
void TodaysResultReadDay(lifted::Regs& ctx, uint8_t* base);
void TodaysResultSetUpMovie(lifted::Regs& ctx, uint8_t* base);
void TodaysResultSetTexts(lifted::Regs& ctx, uint8_t* base);
void TodaysResultDraw(lifted::Regs& ctx, uint8_t* base);
void TodaysResultUpdate(lifted::Regs& ctx, uint8_t* base);
void TodaysResultWaitPage(lifted::Regs& ctx, uint8_t* base);
void TodaysResultLeavePage(lifted::Regs& ctx, uint8_t* base);
void TodaysResultQueueMovie(lifted::Regs& ctx, uint8_t* base);
void TodaysResultIntro(lifted::Regs& ctx, uint8_t* base);
void TodaysResultShowLoop(lifted::Regs& ctx, uint8_t* base);
void TodaysResultFadeOut(lifted::Regs& ctx, uint8_t* base);
void TodaysResultUnload(lifted::Regs& ctx, uint8_t* base);
void TodaysResultUnloaded(lifted::Regs& ctx, uint8_t* base);
void TodaysResultLeave(lifted::Regs& ctx, uint8_t* base);
void TodaysResultDestroy(lifted::Regs& ctx, uint8_t* base);
// Today's event's handlers (r3 the machine, r4 the dispatcher where read)
// and helpers (no arguments; LoadImagePair r3 the data and r4 an index;
// ImagesLoadedA and B and FreeImages r3 the data).
void TodaysEventCreate(lifted::Regs& ctx, uint8_t* base);
void TodaysEventMarkImages(lifted::Regs& ctx, uint8_t* base);
void TodaysEventOpenMovie(lifted::Regs& ctx, uint8_t* base);
void TodaysEventLoadImages(lifted::Regs& ctx, uint8_t* base);
void TodaysEventLoadImagePair(lifted::Regs& ctx, uint8_t* base);
void TodaysEventWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void TodaysEventImagesLoadedA(lifted::Regs& ctx, uint8_t* base);
void TodaysEventImagesLoadedB(lifted::Regs& ctx, uint8_t* base);
void TodaysEventStart(lifted::Regs& ctx, uint8_t* base);
void TodaysEventSetUpTexts(lifted::Regs& ctx, uint8_t* base);
void TodaysEventNextUnlockEntry(lifted::Regs& ctx, uint8_t* base);
void TodaysEventUnload(lifted::Regs& ctx, uint8_t* base);
void TodaysEventUnloaded(lifted::Regs& ctx, uint8_t* base);
void TodaysEventLeave(lifted::Regs& ctx, uint8_t* base);
void TodaysEventDestroy(lifted::Regs& ctx, uint8_t* base);
void TodaysEventFreeImages(lifted::Regs& ctx, uint8_t* base);
// Today's event's Draw and Update, pages (r3 the dispatcher, EndPage r4 the
// machine), transitions (r3 the started word) and helpers (no arguments;
// ShowImages r3 an image index).
void TodaysEventDraw(lifted::Regs& ctx, uint8_t* base);
void TodaysEventUpdate(lifted::Regs& ctx, uint8_t* base);
void TodaysEventEventPage(lifted::Regs& ctx, uint8_t* base);
void TodaysEventEndPage(lifted::Regs& ctx, uint8_t* base);
void TodaysEventQueueEventMovies(lifted::Regs& ctx, uint8_t* base);
void TodaysEventIntro(lifted::Regs& ctx, uint8_t* base);
void TodaysEventFadeOut(lifted::Regs& ctx, uint8_t* base);
void TodaysEventNextUnlock(lifted::Regs& ctx, uint8_t* base);
void TodaysEventNextPage(lifted::Regs& ctx, uint8_t* base);
void TodaysEventShowUnlock(lifted::Regs& ctx, uint8_t* base);
void TodaysEventShowImages(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
