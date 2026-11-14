// The type 8 panel screen's register steps (src/type8_screens/,
// docs/functions/type8-screens.md), shared by the adapters
// (src/hooks/type8_screens.cpp, behind --native_type8_screens) and the
// harness (without the flag). Each takes the registers its original takes
// (r3 the screen, panel or group, r4 and r5 its arguments) and leaves r3 as
// the original does; guest calls go through `ctx`
// (hooks::Type67ScreensCalls).

#pragma once

#include "guest.h"

namespace hooks {

// The panels and the panel group.
void Type8PanelGroupConstruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8FConstruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8EConstruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8CConstruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8DConstruct(PPCContext& ctx, uint8_t* base);
void Type8ConfirmPanelConstruct(PPCContext& ctx, uint8_t* base);
void Type8PanelGroupDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8CDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8Panel8CSetStats(PPCContext& ctx, uint8_t* base);
void Type8Panel8CShowArrows(PPCContext& ctx, uint8_t* base);

// The screen's leaves.
void Type8InitWords(PPCContext& ctx, uint8_t* base);
void Type8IsLoaded(PPCContext& ctx, uint8_t* base);
void Type8OpenMovies(PPCContext& ctx, uint8_t* base);
void Type8DeleteMovies(PPCContext& ctx, uint8_t* base);
void Type8OnMessage2(PPCContext& ctx, uint8_t* base);
void Type8OnSaveLoaded(PPCContext& ctx, uint8_t* base);
void Type8OnAnswer0(PPCContext& ctx, uint8_t* base);
void Type8OnAnswer1(PPCContext& ctx, uint8_t* base);
void Type8OnAnswer2(PPCContext& ctx, uint8_t* base);
void Type8Leave(PPCContext& ctx, uint8_t* base);

// The screen's handlers (ShowPlayerNodes, AskQuit and DrawList have no
// adapters: only the native handlers call them).
void Type8FadeInGroup(PPCContext& ctx, uint8_t* base);
void Type8UpdateOpening(PPCContext& ctx, uint8_t* base);
void Type8Show(PPCContext& ctx, uint8_t* base);
void Type8Draw(PPCContext& ctx, uint8_t* base);
void Type8OnDialogClosed(PPCContext& ctx, uint8_t* base);

// The screen's constructor, destructors, machine and pick.
void Type8Construct(PPCContext& ctx, uint8_t* base);
void Type8Destruct(PPCContext& ctx, uint8_t* base);
void Type8DeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8InitMachine(PPCContext& ctx, uint8_t* base);
void Type8PickCar(PPCContext& ctx, uint8_t* base);

// The list screen's list part: the player block (no adapters: only the
// inner part's functions call them), the inner part and the part, and the
// inner parts' shared step and pages.
void Type8BlockConstruct(PPCContext& ctx, uint8_t* base);
void Type8BlockSetUp(PPCContext& ctx, uint8_t* base);
void Type8BlockPlayOpening(PPCContext& ctx, uint8_t* base);
void Type8BlockShowMenuMove(PPCContext& ctx, uint8_t* base);
void Type8BlockPlayReady(PPCContext& ctx, uint8_t* base);
void Type8BlockPlaceCursor(PPCContext& ctx, uint8_t* base);
void Type8BlockShowSelections(PPCContext& ctx, uint8_t* base);
void Type8BlockWhileReady(PPCContext& ctx, uint8_t* base);
void Type8BlockInput(PPCContext& ctx, uint8_t* base);
void Type8BlockUpdateSetting(PPCContext& ctx, uint8_t* base);
void Type8BlockSettingMoved(PPCContext& ctx, uint8_t* base);
void Type8ListInnerConstruct(PPCContext& ctx, uint8_t* base);
void Type8ListPartConstruct(PPCContext& ctx, uint8_t* base);
void Type8ListInnerDestruct(PPCContext& ctx, uint8_t* base);
void Type8ListInnerDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8ListPartDestruct(PPCContext& ctx, uint8_t* base);
void Type8ListPartDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8ListInnerOpen(PPCContext& ctx, uint8_t* base);
void Type8ListInnerPick(PPCContext& ctx, uint8_t* base);
void Type8ListInnerInput(PPCContext& ctx, uint8_t* base);
void Type8ListInnerStep0(PPCContext& ctx, uint8_t* base);
void Type8ListInnerPage0(PPCContext& ctx, uint8_t* base);
void Type8PartInnerStep1(PPCContext& ctx, uint8_t* base);
void Type8PartInnerPage1(PPCContext& ctx, uint8_t* base);
void Type8PartInnerPage2(PPCContext& ctx, uint8_t* base);

// The color list screen's list part: the color block and the inner
// part's constructor, destructor and IsFirstChoice have no adapters (only
// native callers).
void Type8ColorBlockConstruct(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockSetUp(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockPlaceCursorA(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockPlaceCursorB(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockPlaceCursorC(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockPlayOpening(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockShowSelections(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockShowMenuMove(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockPlayReady(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockSecondMoved(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockUpdateSecond(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockThirdMoved(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockUpdateThird(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockWhileReady(PPCContext& ctx, uint8_t* base);
void Type8ColorBlockInput(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerConstruct(PPCContext& ctx, uint8_t* base);
void Type8ColorPartConstruct(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerDestruct(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8ColorPartDestruct(PPCContext& ctx, uint8_t* base);
void Type8ColorPartDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerIsFirstChoice(PPCContext& ctx, uint8_t* base);
void Type8ColorPartIsFirstChoice(PPCContext& ctx, uint8_t* base);
void Type8ColorPartThirdChoice(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerOpen(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerPick(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerInput(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerStep0(PPCContext& ctx, uint8_t* base);
void Type8ColorInnerPage0(PPCContext& ctx, uint8_t* base);

// The list screen: its constructors, destructors, slots, tables' pages and
// steps (DimList and PairedNodeTable::Clear have no adapters: only the
// native pages call them).
void Type8ListBaseConstruct(PPCContext& ctx, uint8_t* base);
void Type8ListScreenConstruct(PPCContext& ctx, uint8_t* base);
void Type8ListBaseDestruct(PPCContext& ctx, uint8_t* base);
void Type8ListBaseDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8ListScreenDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8ListIsLoaded(PPCContext& ctx, uint8_t* base);
void Type8ListOpenMovie(PPCContext& ctx, uint8_t* base);
void Type8ListRelease(PPCContext& ctx, uint8_t* base);
void Type8ListSetItemTexts(PPCContext& ctx, uint8_t* base);
void Type8ListItemInfo(PPCContext& ctx, uint8_t* base);
void Type8ListShowMarks(PPCContext& ctx, uint8_t* base);
void Type8ListSetLabels(PPCContext& ctx, uint8_t* base);
void Type8ListSetUpMenus(PPCContext& ctx, uint8_t* base);
void Type8ListSetUpRace(PPCContext& ctx, uint8_t* base);
void Type8ListDraw(PPCContext& ctx, uint8_t* base);
void Type8ListUpdate(PPCContext& ctx, uint8_t* base);
void Type8ListPage2(PPCContext& ctx, uint8_t* base);
void Type8ListPage3(PPCContext& ctx, uint8_t* base);
void Type8ListPage4(PPCContext& ctx, uint8_t* base);
void Type8ListPage5(PPCContext& ctx, uint8_t* base);
void Type8ListPage6(PPCContext& ctx, uint8_t* base);
void Type8ListPage8(PPCContext& ctx, uint8_t* base);
void Type8ListStep0(PPCContext& ctx, uint8_t* base);
void Type8ListStep1(PPCContext& ctx, uint8_t* base);
void Type8ListStep2(PPCContext& ctx, uint8_t* base);
void Type8ListStep3(PPCContext& ctx, uint8_t* base);

// The color list screen: its constructor, destructor, machine and slots.
void Type8ColorListConstruct(PPCContext& ctx, uint8_t* base);
void Type8ColorListDeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8ColorListOpenMovie(PPCContext& ctx, uint8_t* base);
void Type8ColorListItemInfo(PPCContext& ctx, uint8_t* base);
void Type8ColorListInitMachine(PPCContext& ctx, uint8_t* base);
void Type8ColorListSetItemText(PPCContext& ctx, uint8_t* base);
void Type8ColorListClearItemText(PPCContext& ctx, uint8_t* base);
void Type8ColorListSetDetails(PPCContext& ctx, uint8_t* base);
void Type8ColorListSetLabels(PPCContext& ctx, uint8_t* base);
void Type8ColorListSetUpMenus(PPCContext& ctx, uint8_t* base);
void Type8ColorListSetUpRace(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
