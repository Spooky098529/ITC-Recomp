// The type 2 screens' register steps (src/type2_screens/,
// docs/functions/type2-screens.md), shared by the adapters
// (src/hooks/type2_screens.cpp, behind --native_type2_screens) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// Shared by four machines (r3 the machine, r4 the mode).
void ScreenLeaveToHub(PPCContext& ctx, uint8_t* base);
// The license plate editor's handlers (r3 the machine, r4 the mode where
// the original reads it).
void PlateCreate(PPCContext& ctx, uint8_t* base);
void PlateOpenMovie(PPCContext& ctx, uint8_t* base);
void PlateWaitForLoad(PPCContext& ctx, uint8_t* base);
void PlateClose(PPCContext& ctx, uint8_t* base);
void PlateDestroy(PPCContext& ctx, uint8_t* base);
void PlateOnMessage32(PPCContext& ctx, uint8_t* base);
void PlateStart(PPCContext& ctx, uint8_t* base);
// The pages (r3 the mode).
void PlateLeavePage(PPCContext& ctx, uint8_t* base);
// The helpers: CharLineKind (r3 the characters), RandomPlate (r4 the
// plate), LoadPlate and ResetMovie (no arguments), ShowRegion (r3 the
// region), ShowDigits and ShowChars (r3 the words), MarkField (r3 the
// field), FinishRegionAnim and FinishDigitAnim (no arguments).
void PlateCharLineKind(PPCContext& ctx, uint8_t* base);
void PlateRandom(PPCContext& ctx, uint8_t* base);
void PlateLoad(PPCContext& ctx, uint8_t* base);
void PlateResetMovie(PPCContext& ctx, uint8_t* base);
void PlateShowRegion(PPCContext& ctx, uint8_t* base);
void PlateShowDigits(PPCContext& ctx, uint8_t* base);
void PlateShowChars(PPCContext& ctx, uint8_t* base);
void PlateMarkField(PPCContext& ctx, uint8_t* base);
void PlateFinishRegionAnim(PPCContext& ctx, uint8_t* base);
void PlateFinishDigitAnim(PPCContext& ctx, uint8_t* base);
// The cells and menus: ShowRegionCells, ShowDigitCells, ShowCharCells and
// the three menus' set-ups (no arguments), ShowCharLine (r3 the kind),
// ShowPlate (r3 the plate).
void PlateShowRegionCells(PPCContext& ctx, uint8_t* base);
void PlateShowDigitCells(PPCContext& ctx, uint8_t* base);
void PlateShowCharCells(PPCContext& ctx, uint8_t* base);
void PlateShowCharLine(PPCContext& ctx, uint8_t* base);
void PlateShowPlate(PPCContext& ctx, uint8_t* base);
void PlateSetUpRegionMenu(PPCContext& ctx, uint8_t* base);
void PlateSetUpNumberMenu(PPCContext& ctx, uint8_t* base);
void PlateSetUpCharsMenu(PPCContext& ctx, uint8_t* base);
// The transitions (r3 the started word's address).
void PlateTransition0(PPCContext& ctx, uint8_t* base);
void PlateTransition1(PPCContext& ctx, uint8_t* base);
void PlateTransition4(PPCContext& ctx, uint8_t* base);
void PlateTransition5(PPCContext& ctx, uint8_t* base);
void PlateTransition6(PPCContext& ctx, uint8_t* base);
void PlateTransition8(PPCContext& ctx, uint8_t* base);
void PlateTransition10(PPCContext& ctx, uint8_t* base);
void PlateTransition11(PPCContext& ctx, uint8_t* base);
void PlateTransition13(PPCContext& ctx, uint8_t* base);
void PlateTransition19(PPCContext& ctx, uint8_t* base);
// The pages (r3 the mode), UpdatePage (r3 the machine, r4 the mode),
// Update (no arguments read).
void PlateRegionPage(PPCContext& ctx, uint8_t* base);
void PlateNumberPage(PPCContext& ctx, uint8_t* base);
void PlateCharsPage(PPCContext& ctx, uint8_t* base);
void PlateUpdatePage(PPCContext& ctx, uint8_t* base);
void PlateUpdate(PPCContext& ctx, uint8_t* base);
// The spec sheet's and the build screen's handlers (r3 the machine, r4 the
// mode where the original reads it; the pages r3 the mode).
void SheetCreate(PPCContext& ctx, uint8_t* base);
void SheetOpenMovie(PPCContext& ctx, uint8_t* base);
void SheetWaitForLoad(PPCContext& ctx, uint8_t* base);
void SheetClose(PPCContext& ctx, uint8_t* base);
void SheetDestroy(PPCContext& ctx, uint8_t* base);
void BuildCreate(PPCContext& ctx, uint8_t* base);
void BuildOpenMovie(PPCContext& ctx, uint8_t* base);
void BuildWaitForLoad(PPCContext& ctx, uint8_t* base);
void BuildUnload(PPCContext& ctx, uint8_t* base);
void BuildClose(PPCContext& ctx, uint8_t* base);
void BuildDestroy(PPCContext& ctx, uint8_t* base);
void BuildSaveBuildStates(PPCContext& ctx, uint8_t* base);
void BuildLeavePage(PPCContext& ctx, uint8_t* base);
void BuildDetailsPage(PPCContext& ctx, uint8_t* base);
void BuildUpdatePage(PPCContext& ctx, uint8_t* base);
// The hub's handlers (r3 the machine, r4 the mode where the original reads
// it; Choose r3 the mode, r4 the machine; PostMessage5 r3 the mode) and
// helpers (ImagesLoadedA and B r3 the data, FreeImage r3 the record,
// SetPrompts r3 to r8, the others no arguments).
void HubOnYes(PPCContext& ctx, uint8_t* base);
void HubOnNo(PPCContext& ctx, uint8_t* base);
void HubImagesLoadedA(PPCContext& ctx, uint8_t* base);
void HubImagesLoadedB(PPCContext& ctx, uint8_t* base);
void HubWaitForLoad(PPCContext& ctx, uint8_t* base);
void HubFreeImage(PPCContext& ctx, uint8_t* base);
void HubOpenSelected(PPCContext& ctx, uint8_t* base);
void HubReenter(PPCContext& ctx, uint8_t* base);
void HubPostMessage5(PPCContext& ctx, uint8_t* base);
void HubChoose(PPCContext& ctx, uint8_t* base);
void HubUnload(PPCContext& ctx, uint8_t* base);
void HubClose(PPCContext& ctx, uint8_t* base);
void HubSetPrompts(PPCContext& ctx, uint8_t* base);
void HubBack(PPCContext& ctx, uint8_t* base);
void HubSetUpStatus(PPCContext& ctx, uint8_t* base);
void HubNextNotice(PPCContext& ctx, uint8_t* base);
void HubNextNotice2(PPCContext& ctx, uint8_t* base);
void HubStart(PPCContext& ctx, uint8_t* base);
// The hub's set-up and teardown (Create, Destroy r3 the machine; OpenMovie
// none), the archive's callback ImageLoaded (r4 the buffer, r6 the record)
// and the helpers without adapters (MarkNoticeImages, LoadNoticeImages no
// arguments; FreeImages r3 the data; LoadNoticeImage r3 the data, r4 i).
void HubCreate(PPCContext& ctx, uint8_t* base);
void HubDestroy(PPCContext& ctx, uint8_t* base);
void HubOpenMovie(PPCContext& ctx, uint8_t* base);
void HubImageLoaded(PPCContext& ctx, uint8_t* base);
void HubMarkNoticeImages(PPCContext& ctx, uint8_t* base);
void HubFreeImages(PPCContext& ctx, uint8_t* base);
void HubLoadNoticeImage(PPCContext& ctx, uint8_t* base);
void HubLoadNoticeImages(PPCContext& ctx, uint8_t* base);
// The hub's UpdatePage (r3 the machine, r4 the mode), RecordPage (r3 the
// mode), its movie helpers (ShowNoticeImage r3 the image; the others no
// arguments) and transitions (r3 the started word's address).
void HubUpdatePage(PPCContext& ctx, uint8_t* base);
void HubRecordPage(PPCContext& ctx, uint8_t* base);
void HubSetRecordText(PPCContext& ctx, uint8_t* base);
void HubShowBusy(PPCContext& ctx, uint8_t* base);
void HubHideBusy(PPCContext& ctx, uint8_t* base);
void HubShowNoticeImage(PPCContext& ctx, uint8_t* base);
void HubShowNotice(PPCContext& ctx, uint8_t* base);
void HubFillMenu(PPCContext& ctx, uint8_t* base);
void HubTransition0(PPCContext& ctx, uint8_t* base);
void HubTransition1(PPCContext& ctx, uint8_t* base);
void HubTransition2(PPCContext& ctx, uint8_t* base);
void HubTransition3(PPCContext& ctx, uint8_t* base);
void HubTransition5(PPCContext& ctx, uint8_t* base);
void HubTransition6(PPCContext& ctx, uint8_t* base);
void HubTransition7(PPCContext& ctx, uint8_t* base);
// The hub's pages (MenuPage r3 the mode; NoticePage no arguments read),
// OnDialogDone (r4 the mode) and FillPlayerRecord (no arguments).
void HubMenuPage(PPCContext& ctx, uint8_t* base);
void HubNoticePage(PPCContext& ctx, uint8_t* base);
void HubOnDialogDone(PPCContext& ctx, uint8_t* base);
void HubFillPlayerRecord(PPCContext& ctx, uint8_t* base);
// The build screen's Enter and the helpers without adapters (Fill and
// SetUpStatus no arguments; RemoveAt r3 the list, r4 the position).
void BuildEnter(PPCContext& ctx, uint8_t* base);
void BuildFill(PPCContext& ctx, uint8_t* base);
void BuildSetUpStatus(PPCContext& ctx, uint8_t* base);
void BuildListRemoveAt(PPCContext& ctx, uint8_t* base);
// The build screen's level pages (r3 the mode, not read) and their jumps
// (no arguments; only the pages call them, no adapters).
void BuildGroupsPage(PPCContext& ctx, uint8_t* base);
void BuildBuildsPage(PPCContext& ctx, uint8_t* base);
void BuildBuildPage(PPCContext& ctx, uint8_t* base);
void BuildJumpGroupA(PPCContext& ctx, uint8_t* base);
void BuildJumpGroupB(PPCContext& ctx, uint8_t* base);
void BuildJumpBuildA(PPCContext& ctx, uint8_t* base);
void BuildJumpBuildB(PPCContext& ctx, uint8_t* base);
void BuildJumpStandA(PPCContext& ctx, uint8_t* base);
void BuildJumpStandB(PPCContext& ctx, uint8_t* base);
// The four machines' set-ups (r3 the machine).
void HubInitStates(PPCContext& ctx, uint8_t* base);
void SheetInitStates(PPCContext& ctx, uint8_t* base);
void BuildInitStates(PPCContext& ctx, uint8_t* base);
void PlateInitStates(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
