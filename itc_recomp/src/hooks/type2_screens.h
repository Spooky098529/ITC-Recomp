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
void ScreenLeaveToHub(PPCContext& ctx, uint8_t* base);
void ScreenLeaveToHub(lifted::Regs& ctx, uint8_t* base);
// The license plate editor's handlers (r3 the machine, r4 the mode where
// the original reads it).
void PlateCreate(PPCContext& ctx, uint8_t* base);
void PlateCreate(lifted::Regs& ctx, uint8_t* base);
void PlateOpenMovie(PPCContext& ctx, uint8_t* base);
void PlateOpenMovie(lifted::Regs& ctx, uint8_t* base);
void PlateWaitForLoad(PPCContext& ctx, uint8_t* base);
void PlateWaitForLoad(lifted::Regs& ctx, uint8_t* base);
void PlateClose(PPCContext& ctx, uint8_t* base);
void PlateClose(lifted::Regs& ctx, uint8_t* base);
void PlateDestroy(PPCContext& ctx, uint8_t* base);
void PlateDestroy(lifted::Regs& ctx, uint8_t* base);
void PlateOnMessage32(PPCContext& ctx, uint8_t* base);
void PlateOnMessage32(lifted::Regs& ctx, uint8_t* base);
void PlateStart(PPCContext& ctx, uint8_t* base);
void PlateStart(lifted::Regs& ctx, uint8_t* base);
// The pages (r3 the mode).
void PlateLeavePage(PPCContext& ctx, uint8_t* base);
void PlateLeavePage(lifted::Regs& ctx, uint8_t* base);
// The helpers: CharLineKind (r3 the characters), RandomPlate (r4 the
// plate), LoadPlate and ResetMovie (no arguments), ShowRegion (r3 the
// region), ShowDigits and ShowChars (r3 the words), MarkField (r3 the
// field), FinishRegionAnim and FinishDigitAnim (no arguments).
void PlateCharLineKind(PPCContext& ctx, uint8_t* base);
void PlateCharLineKind(lifted::Regs& ctx, uint8_t* base);
void PlateRandom(PPCContext& ctx, uint8_t* base);
void PlateRandom(lifted::Regs& ctx, uint8_t* base);
void PlateLoad(PPCContext& ctx, uint8_t* base);
void PlateLoad(lifted::Regs& ctx, uint8_t* base);
void PlateResetMovie(PPCContext& ctx, uint8_t* base);
void PlateResetMovie(lifted::Regs& ctx, uint8_t* base);
void PlateShowRegion(PPCContext& ctx, uint8_t* base);
void PlateShowRegion(lifted::Regs& ctx, uint8_t* base);
void PlateShowDigits(PPCContext& ctx, uint8_t* base);
void PlateShowDigits(lifted::Regs& ctx, uint8_t* base);
void PlateShowChars(PPCContext& ctx, uint8_t* base);
void PlateShowChars(lifted::Regs& ctx, uint8_t* base);
void PlateMarkField(PPCContext& ctx, uint8_t* base);
void PlateMarkField(lifted::Regs& ctx, uint8_t* base);
void PlateFinishRegionAnim(PPCContext& ctx, uint8_t* base);
void PlateFinishRegionAnim(lifted::Regs& ctx, uint8_t* base);
void PlateFinishDigitAnim(PPCContext& ctx, uint8_t* base);
void PlateFinishDigitAnim(lifted::Regs& ctx, uint8_t* base);
// The cells and menus: ShowRegionCells, ShowDigitCells, ShowCharCells and
// the three menus' set-ups (no arguments), ShowCharLine (r3 the kind),
// ShowPlate (r3 the plate).
void PlateShowRegionCells(PPCContext& ctx, uint8_t* base);
void PlateShowRegionCells(lifted::Regs& ctx, uint8_t* base);
void PlateShowDigitCells(PPCContext& ctx, uint8_t* base);
void PlateShowDigitCells(lifted::Regs& ctx, uint8_t* base);
void PlateShowCharCells(PPCContext& ctx, uint8_t* base);
void PlateShowCharCells(lifted::Regs& ctx, uint8_t* base);
void PlateShowCharLine(PPCContext& ctx, uint8_t* base);
void PlateShowCharLine(lifted::Regs& ctx, uint8_t* base);
void PlateShowPlate(PPCContext& ctx, uint8_t* base);
void PlateShowPlate(lifted::Regs& ctx, uint8_t* base);
void PlateSetUpRegionMenu(PPCContext& ctx, uint8_t* base);
void PlateSetUpRegionMenu(lifted::Regs& ctx, uint8_t* base);
void PlateSetUpNumberMenu(PPCContext& ctx, uint8_t* base);
void PlateSetUpNumberMenu(lifted::Regs& ctx, uint8_t* base);
void PlateSetUpCharsMenu(PPCContext& ctx, uint8_t* base);
void PlateSetUpCharsMenu(lifted::Regs& ctx, uint8_t* base);
// The transitions (r3 the started word's address).
void PlateTransition0(PPCContext& ctx, uint8_t* base);
void PlateTransition0(lifted::Regs& ctx, uint8_t* base);
void PlateTransition1(PPCContext& ctx, uint8_t* base);
void PlateTransition1(lifted::Regs& ctx, uint8_t* base);
void PlateTransition4(PPCContext& ctx, uint8_t* base);
void PlateTransition4(lifted::Regs& ctx, uint8_t* base);
void PlateTransition5(PPCContext& ctx, uint8_t* base);
void PlateTransition5(lifted::Regs& ctx, uint8_t* base);
void PlateTransition6(PPCContext& ctx, uint8_t* base);
void PlateTransition6(lifted::Regs& ctx, uint8_t* base);
void PlateTransition8(PPCContext& ctx, uint8_t* base);
void PlateTransition8(lifted::Regs& ctx, uint8_t* base);
void PlateTransition10(PPCContext& ctx, uint8_t* base);
void PlateTransition10(lifted::Regs& ctx, uint8_t* base);
void PlateTransition11(PPCContext& ctx, uint8_t* base);
void PlateTransition11(lifted::Regs& ctx, uint8_t* base);
void PlateTransition13(PPCContext& ctx, uint8_t* base);
void PlateTransition13(lifted::Regs& ctx, uint8_t* base);
void PlateTransition19(PPCContext& ctx, uint8_t* base);
void PlateTransition19(lifted::Regs& ctx, uint8_t* base);
// The pages (r3 the mode), UpdatePage (r3 the machine, r4 the mode),
// Update (no arguments read).
void PlateRegionPage(PPCContext& ctx, uint8_t* base);
void PlateRegionPage(lifted::Regs& ctx, uint8_t* base);
void PlateNumberPage(PPCContext& ctx, uint8_t* base);
void PlateNumberPage(lifted::Regs& ctx, uint8_t* base);
void PlateCharsPage(PPCContext& ctx, uint8_t* base);
void PlateCharsPage(lifted::Regs& ctx, uint8_t* base);
void PlateUpdatePage(PPCContext& ctx, uint8_t* base);
void PlateUpdatePage(lifted::Regs& ctx, uint8_t* base);
void PlateUpdate(PPCContext& ctx, uint8_t* base);
void PlateUpdate(lifted::Regs& ctx, uint8_t* base);
// The spec sheet's and the build screen's handlers (r3 the machine, r4 the
// mode where the original reads it; the pages r3 the mode).
void SheetCreate(PPCContext& ctx, uint8_t* base);
void SheetCreate(lifted::Regs& ctx, uint8_t* base);
void SheetOpenMovie(PPCContext& ctx, uint8_t* base);
void SheetOpenMovie(lifted::Regs& ctx, uint8_t* base);
void SheetWaitForLoad(PPCContext& ctx, uint8_t* base);
void SheetWaitForLoad(lifted::Regs& ctx, uint8_t* base);
void SheetClose(PPCContext& ctx, uint8_t* base);
void SheetClose(lifted::Regs& ctx, uint8_t* base);
void SheetDestroy(PPCContext& ctx, uint8_t* base);
void SheetDestroy(lifted::Regs& ctx, uint8_t* base);
void BuildCreate(PPCContext& ctx, uint8_t* base);
void BuildCreate(lifted::Regs& ctx, uint8_t* base);
void BuildOpenMovie(PPCContext& ctx, uint8_t* base);
void BuildOpenMovie(lifted::Regs& ctx, uint8_t* base);
void BuildWaitForLoad(PPCContext& ctx, uint8_t* base);
void BuildWaitForLoad(lifted::Regs& ctx, uint8_t* base);
void BuildUnload(PPCContext& ctx, uint8_t* base);
void BuildUnload(lifted::Regs& ctx, uint8_t* base);
void BuildClose(PPCContext& ctx, uint8_t* base);
void BuildClose(lifted::Regs& ctx, uint8_t* base);
void BuildDestroy(PPCContext& ctx, uint8_t* base);
void BuildDestroy(lifted::Regs& ctx, uint8_t* base);
void BuildSaveBuildStates(PPCContext& ctx, uint8_t* base);
void BuildSaveBuildStates(lifted::Regs& ctx, uint8_t* base);
void BuildLeavePage(PPCContext& ctx, uint8_t* base);
void BuildLeavePage(lifted::Regs& ctx, uint8_t* base);
void BuildDetailsPage(PPCContext& ctx, uint8_t* base);
void BuildDetailsPage(lifted::Regs& ctx, uint8_t* base);
void BuildUpdatePage(PPCContext& ctx, uint8_t* base);
void BuildUpdatePage(lifted::Regs& ctx, uint8_t* base);
// The hub's handlers (r3 the machine, r4 the mode where the original reads
// it; Choose r3 the mode, r4 the machine; PostMessage5 r3 the mode) and
// helpers (ImagesLoadedA and B r3 the data, FreeImage r3 the record,
// SetPrompts r3 to r8, the others no arguments).
void HubOnYes(PPCContext& ctx, uint8_t* base);
void HubOnYes(lifted::Regs& ctx, uint8_t* base);
void HubOnNo(PPCContext& ctx, uint8_t* base);
void HubOnNo(lifted::Regs& ctx, uint8_t* base);
void HubImagesLoadedA(PPCContext& ctx, uint8_t* base);
void HubImagesLoadedA(lifted::Regs& ctx, uint8_t* base);
void HubImagesLoadedB(PPCContext& ctx, uint8_t* base);
void HubImagesLoadedB(lifted::Regs& ctx, uint8_t* base);
void HubWaitForLoad(PPCContext& ctx, uint8_t* base);
void HubWaitForLoad(lifted::Regs& ctx, uint8_t* base);
void HubFreeImage(PPCContext& ctx, uint8_t* base);
void HubFreeImage(lifted::Regs& ctx, uint8_t* base);
void HubOpenSelected(PPCContext& ctx, uint8_t* base);
void HubOpenSelected(lifted::Regs& ctx, uint8_t* base);
void HubReenter(PPCContext& ctx, uint8_t* base);
void HubReenter(lifted::Regs& ctx, uint8_t* base);
void HubPostMessage5(PPCContext& ctx, uint8_t* base);
void HubPostMessage5(lifted::Regs& ctx, uint8_t* base);
void HubChoose(PPCContext& ctx, uint8_t* base);
void HubChoose(lifted::Regs& ctx, uint8_t* base);
void HubUnload(PPCContext& ctx, uint8_t* base);
void HubUnload(lifted::Regs& ctx, uint8_t* base);
void HubClose(PPCContext& ctx, uint8_t* base);
void HubClose(lifted::Regs& ctx, uint8_t* base);
void HubSetPrompts(PPCContext& ctx, uint8_t* base);
void HubSetPrompts(lifted::Regs& ctx, uint8_t* base);
void HubBack(PPCContext& ctx, uint8_t* base);
void HubBack(lifted::Regs& ctx, uint8_t* base);
void HubSetUpStatus(PPCContext& ctx, uint8_t* base);
void HubSetUpStatus(lifted::Regs& ctx, uint8_t* base);
void HubNextNotice(PPCContext& ctx, uint8_t* base);
void HubNextNotice(lifted::Regs& ctx, uint8_t* base);
void HubNextNotice2(PPCContext& ctx, uint8_t* base);
void HubNextNotice2(lifted::Regs& ctx, uint8_t* base);
void HubStart(PPCContext& ctx, uint8_t* base);
void HubStart(lifted::Regs& ctx, uint8_t* base);
// The hub's set-up and teardown (Create, Destroy r3 the machine; OpenMovie
// none), the archive's callback ImageLoaded (r4 the buffer, r6 the record)
// and the helpers without adapters (MarkNoticeImages, LoadNoticeImages no
// arguments; FreeImages r3 the data; LoadNoticeImage r3 the data, r4 i).
void HubCreate(PPCContext& ctx, uint8_t* base);
void HubCreate(lifted::Regs& ctx, uint8_t* base);
void HubDestroy(PPCContext& ctx, uint8_t* base);
void HubDestroy(lifted::Regs& ctx, uint8_t* base);
void HubOpenMovie(PPCContext& ctx, uint8_t* base);
void HubOpenMovie(lifted::Regs& ctx, uint8_t* base);
void HubImageLoaded(PPCContext& ctx, uint8_t* base);
void HubImageLoaded(lifted::Regs& ctx, uint8_t* base);
void HubMarkNoticeImages(PPCContext& ctx, uint8_t* base);
void HubMarkNoticeImages(lifted::Regs& ctx, uint8_t* base);
void HubFreeImages(PPCContext& ctx, uint8_t* base);
void HubFreeImages(lifted::Regs& ctx, uint8_t* base);
void HubLoadNoticeImage(PPCContext& ctx, uint8_t* base);
void HubLoadNoticeImage(lifted::Regs& ctx, uint8_t* base);
void HubLoadNoticeImages(PPCContext& ctx, uint8_t* base);
void HubLoadNoticeImages(lifted::Regs& ctx, uint8_t* base);
// The hub's UpdatePage (r3 the machine, r4 the mode), RecordPage (r3 the
// mode), its movie helpers (ShowNoticeImage r3 the image; the others no
// arguments) and transitions (r3 the started word's address).
void HubUpdatePage(PPCContext& ctx, uint8_t* base);
void HubUpdatePage(lifted::Regs& ctx, uint8_t* base);
void HubRecordPage(PPCContext& ctx, uint8_t* base);
void HubRecordPage(lifted::Regs& ctx, uint8_t* base);
void HubSetRecordText(PPCContext& ctx, uint8_t* base);
void HubSetRecordText(lifted::Regs& ctx, uint8_t* base);
void HubShowBusy(PPCContext& ctx, uint8_t* base);
void HubShowBusy(lifted::Regs& ctx, uint8_t* base);
void HubHideBusy(PPCContext& ctx, uint8_t* base);
void HubHideBusy(lifted::Regs& ctx, uint8_t* base);
void HubShowNoticeImage(PPCContext& ctx, uint8_t* base);
void HubShowNoticeImage(lifted::Regs& ctx, uint8_t* base);
void HubShowNotice(PPCContext& ctx, uint8_t* base);
void HubShowNotice(lifted::Regs& ctx, uint8_t* base);
void HubFillMenu(PPCContext& ctx, uint8_t* base);
void HubFillMenu(lifted::Regs& ctx, uint8_t* base);
void HubTransition0(PPCContext& ctx, uint8_t* base);
void HubTransition0(lifted::Regs& ctx, uint8_t* base);
void HubTransition1(PPCContext& ctx, uint8_t* base);
void HubTransition1(lifted::Regs& ctx, uint8_t* base);
void HubTransition2(PPCContext& ctx, uint8_t* base);
void HubTransition2(lifted::Regs& ctx, uint8_t* base);
void HubTransition3(PPCContext& ctx, uint8_t* base);
void HubTransition3(lifted::Regs& ctx, uint8_t* base);
void HubTransition5(PPCContext& ctx, uint8_t* base);
void HubTransition5(lifted::Regs& ctx, uint8_t* base);
void HubTransition6(PPCContext& ctx, uint8_t* base);
void HubTransition6(lifted::Regs& ctx, uint8_t* base);
void HubTransition7(PPCContext& ctx, uint8_t* base);
void HubTransition7(lifted::Regs& ctx, uint8_t* base);
// The hub's pages (MenuPage r3 the mode; NoticePage no arguments read),
// OnDialogDone (r4 the mode) and FillPlayerRecord (no arguments).
void HubMenuPage(PPCContext& ctx, uint8_t* base);
void HubMenuPage(lifted::Regs& ctx, uint8_t* base);
void HubNoticePage(PPCContext& ctx, uint8_t* base);
void HubNoticePage(lifted::Regs& ctx, uint8_t* base);
void HubOnDialogDone(PPCContext& ctx, uint8_t* base);
void HubOnDialogDone(lifted::Regs& ctx, uint8_t* base);
void HubFillPlayerRecord(PPCContext& ctx, uint8_t* base);
void HubFillPlayerRecord(lifted::Regs& ctx, uint8_t* base);
// The build screen's Enter and the helpers without adapters (Fill and
// SetUpStatus no arguments; RemoveAt r3 the list, r4 the position).
void BuildEnter(PPCContext& ctx, uint8_t* base);
void BuildEnter(lifted::Regs& ctx, uint8_t* base);
void BuildFill(PPCContext& ctx, uint8_t* base);
void BuildFill(lifted::Regs& ctx, uint8_t* base);
void BuildSetUpStatus(PPCContext& ctx, uint8_t* base);
void BuildSetUpStatus(lifted::Regs& ctx, uint8_t* base);
void BuildListRemoveAt(PPCContext& ctx, uint8_t* base);
void BuildListRemoveAt(lifted::Regs& ctx, uint8_t* base);
// The hub's Update (no arguments read) and PutCarOnStand (r3 the car; only
// Update calls it, no adapter).
void HubUpdate(PPCContext& ctx, uint8_t* base);
void HubUpdate(lifted::Regs& ctx, uint8_t* base);
void HubPutCarOnStand(PPCContext& ctx, uint8_t* base);
void HubPutCarOnStand(lifted::Regs& ctx, uint8_t* base);
// The build screen's level pages (r3 the mode, not read) and their jumps
// (no arguments; only the pages call them, no adapters).
void BuildGroupsPage(PPCContext& ctx, uint8_t* base);
void BuildGroupsPage(lifted::Regs& ctx, uint8_t* base);
void BuildBuildsPage(PPCContext& ctx, uint8_t* base);
void BuildBuildsPage(lifted::Regs& ctx, uint8_t* base);
void BuildBuildPage(PPCContext& ctx, uint8_t* base);
void BuildBuildPage(lifted::Regs& ctx, uint8_t* base);
void BuildJumpGroupA(PPCContext& ctx, uint8_t* base);
void BuildJumpGroupA(lifted::Regs& ctx, uint8_t* base);
void BuildJumpGroupB(PPCContext& ctx, uint8_t* base);
void BuildJumpGroupB(lifted::Regs& ctx, uint8_t* base);
void BuildJumpBuildA(PPCContext& ctx, uint8_t* base);
void BuildJumpBuildA(lifted::Regs& ctx, uint8_t* base);
void BuildJumpBuildB(PPCContext& ctx, uint8_t* base);
void BuildJumpBuildB(lifted::Regs& ctx, uint8_t* base);
void BuildJumpStandA(PPCContext& ctx, uint8_t* base);
void BuildJumpStandA(lifted::Regs& ctx, uint8_t* base);
void BuildJumpStandB(PPCContext& ctx, uint8_t* base);
void BuildJumpStandB(lifted::Regs& ctx, uint8_t* base);
// The four machines' set-ups (r3 the machine).
void HubInitStates(PPCContext& ctx, uint8_t* base);
void HubInitStates(lifted::Regs& ctx, uint8_t* base);
void SheetInitStates(PPCContext& ctx, uint8_t* base);
void SheetInitStates(lifted::Regs& ctx, uint8_t* base);
void BuildInitStates(PPCContext& ctx, uint8_t* base);
void BuildInitStates(lifted::Regs& ctx, uint8_t* base);
void PlateInitStates(PPCContext& ctx, uint8_t* base);
void PlateInitStates(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
