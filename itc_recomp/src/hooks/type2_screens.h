// The type 2 screens' register steps (src/type2_screens/,
// docs/functions/type2-screens.md), shared by the adapters
// (src/hooks/type2_screens.cpp, behind --native_type2_screens) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Shared by four machines (r3 the machine, r4 the mode).
void ScreenLeaveToHub(lifted::Regs& ctx);
// The license plate editor's handlers (r3 the machine, r4 the mode where
// the original reads it).
void PlateCreate(lifted::Regs& ctx);
void PlateOpenMovie(lifted::Regs& ctx);
void PlateWaitForLoad(lifted::Regs& ctx);
void PlateClose(lifted::Regs& ctx);
void PlateDestroy(lifted::Regs& ctx);
void PlateOnMessage32(lifted::Regs& ctx);
void PlateStart(lifted::Regs& ctx);
// The pages (r3 the mode).
void PlateLeavePage(lifted::Regs& ctx);
// The helpers: CharLineKind (r3 the characters), RandomPlate (r4 the
// plate), LoadPlate and ResetMovie (no arguments), ShowRegion (r3 the
// region), ShowDigits and ShowChars (r3 the words), MarkField (r3 the
// field), FinishRegionAnim and FinishDigitAnim (no arguments).
void PlateCharLineKind(lifted::Regs& ctx);
void PlateRandom(lifted::Regs& ctx);
void PlateLoad(lifted::Regs& ctx);
void PlateResetMovie(lifted::Regs& ctx);
void PlateShowRegion(lifted::Regs& ctx);
void PlateShowDigits(lifted::Regs& ctx);
void PlateShowChars(lifted::Regs& ctx);
void PlateMarkField(lifted::Regs& ctx);
void PlateFinishRegionAnim(lifted::Regs& ctx);
void PlateFinishDigitAnim(lifted::Regs& ctx);
// The cells and menus: ShowRegionCells, ShowDigitCells, ShowCharCells and
// the three menus' set-ups (no arguments), ShowCharLine (r3 the kind),
// ShowPlate (r3 the plate).
void PlateShowRegionCells(lifted::Regs& ctx);
void PlateShowDigitCells(lifted::Regs& ctx);
void PlateShowCharCells(lifted::Regs& ctx);
void PlateShowCharLine(lifted::Regs& ctx);
void PlateShowPlate(lifted::Regs& ctx);
void PlateSetUpRegionMenu(lifted::Regs& ctx);
void PlateSetUpNumberMenu(lifted::Regs& ctx);
void PlateSetUpCharsMenu(lifted::Regs& ctx);
// The transitions (r3 the started word's address).
void PlateTransition0(lifted::Regs& ctx);
void PlateTransition1(lifted::Regs& ctx);
void PlateTransition4(lifted::Regs& ctx);
void PlateTransition5(lifted::Regs& ctx);
void PlateTransition6(lifted::Regs& ctx);
void PlateTransition8(lifted::Regs& ctx);
void PlateTransition10(lifted::Regs& ctx);
void PlateTransition11(lifted::Regs& ctx);
void PlateTransition13(lifted::Regs& ctx);
void PlateTransition19(lifted::Regs& ctx);
// The pages (r3 the mode), UpdatePage (r3 the machine, r4 the mode),
// Update (no arguments read).
void PlateRegionPage(lifted::Regs& ctx);
void PlateNumberPage(lifted::Regs& ctx);
void PlateCharsPage(lifted::Regs& ctx);
void PlateUpdatePage(lifted::Regs& ctx);
void PlateUpdate(lifted::Regs& ctx);
// The spec sheet's and the build screen's handlers (r3 the machine, r4 the
// mode where the original reads it; the pages r3 the mode).
void SheetCreate(lifted::Regs& ctx);
void SheetOpenMovie(lifted::Regs& ctx);
void SheetWaitForLoad(lifted::Regs& ctx);
void SheetClose(lifted::Regs& ctx);
void SheetDestroy(lifted::Regs& ctx);
void BuildCreate(lifted::Regs& ctx);
void BuildOpenMovie(lifted::Regs& ctx);
void BuildWaitForLoad(lifted::Regs& ctx);
void BuildUnload(lifted::Regs& ctx);
void BuildClose(lifted::Regs& ctx);
void BuildDestroy(lifted::Regs& ctx);
void BuildSaveBuildStates(lifted::Regs& ctx);
void BuildLeavePage(lifted::Regs& ctx);
void BuildDetailsPage(lifted::Regs& ctx);
void BuildUpdatePage(lifted::Regs& ctx);
// The hub's handlers (r3 the machine, r4 the mode where the original reads
// it; Choose r3 the mode, r4 the machine; PostMessage5 r3 the mode) and
// helpers (ImagesLoadedA and B r3 the data, FreeImage r3 the record,
// SetPrompts r3 to r8, the others no arguments).
void HubOnYes(lifted::Regs& ctx);
void HubOnNo(lifted::Regs& ctx);
void HubImagesLoadedA(lifted::Regs& ctx);
void HubImagesLoadedB(lifted::Regs& ctx);
void HubWaitForLoad(lifted::Regs& ctx);
void HubFreeImage(lifted::Regs& ctx);
void HubOpenSelected(lifted::Regs& ctx);
void HubReenter(lifted::Regs& ctx);
void HubPostMessage5(lifted::Regs& ctx);
void HubChoose(lifted::Regs& ctx);
void HubUnload(lifted::Regs& ctx);
void HubClose(lifted::Regs& ctx);
void HubSetPrompts(lifted::Regs& ctx);
void HubBack(lifted::Regs& ctx);
void HubSetUpStatus(lifted::Regs& ctx);
void HubNextNotice(lifted::Regs& ctx);
void HubNextNotice2(lifted::Regs& ctx);
void HubStart(lifted::Regs& ctx);
// The hub's set-up and teardown (Create, Destroy r3 the machine; OpenMovie
// none), the archive's callback ImageLoaded (r4 the buffer, r6 the record)
// and the helpers without adapters (MarkNoticeImages, LoadNoticeImages no
// arguments; FreeImages r3 the data; LoadNoticeImage r3 the data, r4 i).
void HubCreate(lifted::Regs& ctx);
void HubDestroy(lifted::Regs& ctx);
void HubOpenMovie(lifted::Regs& ctx);
void HubImageLoaded(lifted::Regs& ctx);
void HubMarkNoticeImages(lifted::Regs& ctx);
void HubFreeImages(lifted::Regs& ctx);
void HubLoadNoticeImage(lifted::Regs& ctx);
void HubLoadNoticeImages(lifted::Regs& ctx);
// The hub's UpdatePage (r3 the machine, r4 the mode), RecordPage (r3 the
// mode), its movie helpers (ShowNoticeImage r3 the image; the others no
// arguments) and transitions (r3 the started word's address).
void HubUpdatePage(lifted::Regs& ctx);
void HubRecordPage(lifted::Regs& ctx);
void HubSetRecordText(lifted::Regs& ctx);
void HubShowBusy(lifted::Regs& ctx);
void HubHideBusy(lifted::Regs& ctx);
void HubShowNoticeImage(lifted::Regs& ctx);
void HubShowNotice(lifted::Regs& ctx);
void HubFillMenu(lifted::Regs& ctx);
void HubTransition0(lifted::Regs& ctx);
void HubTransition1(lifted::Regs& ctx);
void HubTransition2(lifted::Regs& ctx);
void HubTransition3(lifted::Regs& ctx);
void HubTransition5(lifted::Regs& ctx);
void HubTransition6(lifted::Regs& ctx);
void HubTransition7(lifted::Regs& ctx);
// The hub's pages (MenuPage r3 the mode; NoticePage no arguments read),
// OnDialogDone (r4 the mode) and FillPlayerRecord (no arguments).
void HubMenuPage(lifted::Regs& ctx);
void HubNoticePage(lifted::Regs& ctx);
void HubOnDialogDone(lifted::Regs& ctx);
void HubFillPlayerRecord(lifted::Regs& ctx);
// The build screen's Enter and the helpers without adapters (Fill and
// SetUpStatus no arguments; RemoveAt r3 the list, r4 the position).
void BuildEnter(lifted::Regs& ctx);
void BuildFill(lifted::Regs& ctx);
void BuildSetUpStatus(lifted::Regs& ctx);
void BuildListRemoveAt(lifted::Regs& ctx);
// The hub's Update (no arguments read) and PutCarOnStand (r3 the car; only
// Update calls it, no adapter).
void HubUpdate(lifted::Regs& ctx);
void HubPutCarOnStand(lifted::Regs& ctx);
// The build screen's level pages (r3 the mode, not read) and their jumps
// (no arguments; only the pages call them, no adapters).
void BuildGroupsPage(lifted::Regs& ctx);
void BuildBuildsPage(lifted::Regs& ctx);
void BuildBuildPage(lifted::Regs& ctx);
void BuildJumpGroupA(lifted::Regs& ctx);
void BuildJumpGroupB(lifted::Regs& ctx);
void BuildJumpBuildA(lifted::Regs& ctx);
void BuildJumpBuildB(lifted::Regs& ctx);
void BuildJumpStandA(lifted::Regs& ctx);
void BuildJumpStandB(lifted::Regs& ctx);
// The four machines' set-ups (r3 the machine).
void HubInitStates(lifted::Regs& ctx);
void SheetInitStates(lifted::Regs& ctx);
void BuildInitStates(lifted::Regs& ctx);
void PlateInitStates(lifted::Regs& ctx);

}  // namespace hooks
