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
void ModeType9Init(lifted::Regs& ctx);
void ModeType9WaitForLoads(lifted::Regs& ctx);
void ModeType9NextMode(lifted::Regs& ctx);
void ModeType9LeaveScreen(lifted::Regs& ctx);
void ModeType9WaitForShown(lifted::Regs& ctx);
void ModeType9Leaving(lifted::Regs& ctx);
void ModeType9IsLoaded(lifted::Regs& ctx);
void ModeType9FindShown(lifted::Regs& ctx);
void TodaysEventKind(lifted::Regs& ctx);
void TodaysResultInitStates(lifted::Regs& ctx);
void TodaysEventInitStates(lifted::Regs& ctx);
// Today's result's handlers (r3 the machine, r4 the dispatcher where read),
// its helpers (no arguments), pages (r3 the dispatcher) and transitions (r3
// the started word's address).
void TodaysResultCreate(lifted::Regs& ctx);
void TodaysResultOpenMovie(lifted::Regs& ctx);
void TodaysResultWaitForMovie(lifted::Regs& ctx);
void TodaysResultStart(lifted::Regs& ctx);
void TodaysResultReadDay(lifted::Regs& ctx);
void TodaysResultSetUpMovie(lifted::Regs& ctx);
void TodaysResultSetTexts(lifted::Regs& ctx);
void TodaysResultDraw(lifted::Regs& ctx);
void TodaysResultUpdate(lifted::Regs& ctx);
void TodaysResultWaitPage(lifted::Regs& ctx);
void TodaysResultLeavePage(lifted::Regs& ctx);
void TodaysResultQueueMovie(lifted::Regs& ctx);
void TodaysResultIntro(lifted::Regs& ctx);
void TodaysResultShowLoop(lifted::Regs& ctx);
void TodaysResultFadeOut(lifted::Regs& ctx);
void TodaysResultUnload(lifted::Regs& ctx);
void TodaysResultUnloaded(lifted::Regs& ctx);
void TodaysResultLeave(lifted::Regs& ctx);
void TodaysResultDestroy(lifted::Regs& ctx);
// Today's event's handlers (r3 the machine, r4 the dispatcher where read)
// and helpers (no arguments; LoadImagePair r3 the data and r4 an index;
// ImagesLoadedA and B and FreeImages r3 the data).
void TodaysEventCreate(lifted::Regs& ctx);
void TodaysEventMarkImages(lifted::Regs& ctx);
void TodaysEventOpenMovie(lifted::Regs& ctx);
void TodaysEventLoadImages(lifted::Regs& ctx);
void TodaysEventLoadImagePair(lifted::Regs& ctx);
void TodaysEventWaitForMovie(lifted::Regs& ctx);
void TodaysEventImagesLoadedA(lifted::Regs& ctx);
void TodaysEventImagesLoadedB(lifted::Regs& ctx);
void TodaysEventStart(lifted::Regs& ctx);
void TodaysEventSetUpTexts(lifted::Regs& ctx);
void TodaysEventNextUnlockEntry(lifted::Regs& ctx);
void TodaysEventUnload(lifted::Regs& ctx);
void TodaysEventUnloaded(lifted::Regs& ctx);
void TodaysEventLeave(lifted::Regs& ctx);
void TodaysEventDestroy(lifted::Regs& ctx);
void TodaysEventFreeImages(lifted::Regs& ctx);
// Today's event's Draw and Update, pages (r3 the dispatcher, EndPage r4 the
// machine), transitions (r3 the started word) and helpers (no arguments;
// ShowImages r3 an image index).
void TodaysEventDraw(lifted::Regs& ctx);
void TodaysEventUpdate(lifted::Regs& ctx);
void TodaysEventEventPage(lifted::Regs& ctx);
void TodaysEventEndPage(lifted::Regs& ctx);
void TodaysEventQueueEventMovies(lifted::Regs& ctx);
void TodaysEventIntro(lifted::Regs& ctx);
void TodaysEventFadeOut(lifted::Regs& ctx);
void TodaysEventNextUnlock(lifted::Regs& ctx);
void TodaysEventNextPage(lifted::Regs& ctx);
void TodaysEventShowUnlock(lifted::Regs& ctx);
void TodaysEventShowImages(lifted::Regs& ctx);

}  // namespace hooks
