// The type 8 panel screen's register steps (src/type8_screens/,
// docs/functions/type8-screens.md), shared by the adapters
// (src/hooks/type8_screens.cpp, behind --native_type8_screens) and the
// harness (without the flag). Each takes the registers its original takes
// (r3 the screen, panel or group, r4 and r5 its arguments) and leaves r3 as
// the original does; guest calls go through `ctx`
// (hooks::Type67ScreensCalls).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The panels and the panel group.
void Type8PanelGroupConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8Panel8FConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8Panel8EConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8Panel8CConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8Panel8DConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ConfirmPanelConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8PanelGroupDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8Panel8CDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8Panel8CSetStats(lifted::Regs& ctx, uint8_t* base);
void Type8Panel8CShowArrows(lifted::Regs& ctx, uint8_t* base);

// The screen's leaves.
void Type8InitWords(lifted::Regs& ctx, uint8_t* base);
void Type8IsLoaded(lifted::Regs& ctx, uint8_t* base);
void Type8OpenMovies(lifted::Regs& ctx, uint8_t* base);
void Type8DeleteMovies(lifted::Regs& ctx, uint8_t* base);
void Type8OnMessage2(lifted::Regs& ctx, uint8_t* base);
void Type8OnSaveLoaded(lifted::Regs& ctx, uint8_t* base);
void Type8OnAnswer0(lifted::Regs& ctx, uint8_t* base);
void Type8OnAnswer1(lifted::Regs& ctx, uint8_t* base);
void Type8OnAnswer2(lifted::Regs& ctx, uint8_t* base);
void Type8Leave(lifted::Regs& ctx, uint8_t* base);

// The screen's handlers (ShowPlayerNodes, AskQuit and DrawList have no
// adapters: only the native handlers call them).
void Type8FadeInGroup(lifted::Regs& ctx, uint8_t* base);
void Type8UpdateOpening(lifted::Regs& ctx, uint8_t* base);
void Type8Show(lifted::Regs& ctx, uint8_t* base);
void Type8Draw(lifted::Regs& ctx, uint8_t* base);
void Type8OnDialogClosed(lifted::Regs& ctx, uint8_t* base);

// The screen's constructor, destructors, machine and pick.
void Type8Construct(lifted::Regs& ctx, uint8_t* base);
void Type8Destruct(lifted::Regs& ctx, uint8_t* base);
void Type8DeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8InitMachine(lifted::Regs& ctx, uint8_t* base);
void Type8PickCar(lifted::Regs& ctx, uint8_t* base);

// The list screen's list part: the player block (no adapters: only the
// inner part's functions call them), the inner part and the part, and the
// inner parts' shared step and pages.
void Type8BlockConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8BlockSetUp(lifted::Regs& ctx, uint8_t* base);
void Type8BlockPlayOpening(lifted::Regs& ctx, uint8_t* base);
void Type8BlockShowMenuMove(lifted::Regs& ctx, uint8_t* base);
void Type8BlockPlayReady(lifted::Regs& ctx, uint8_t* base);
void Type8BlockPlaceCursor(lifted::Regs& ctx, uint8_t* base);
void Type8BlockShowSelections(lifted::Regs& ctx, uint8_t* base);
void Type8BlockWhileReady(lifted::Regs& ctx, uint8_t* base);
void Type8BlockInput(lifted::Regs& ctx, uint8_t* base);
void Type8BlockUpdateSetting(lifted::Regs& ctx, uint8_t* base);
void Type8BlockSettingMoved(lifted::Regs& ctx, uint8_t* base);
void Type8ListInnerConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListPartConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListInnerDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListInnerDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListPartDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListPartDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListInnerOpen(lifted::Regs& ctx, uint8_t* base);
void Type8ListInnerPick(lifted::Regs& ctx, uint8_t* base);
void Type8ListInnerInput(lifted::Regs& ctx, uint8_t* base);
void Type8ListInnerStep0(lifted::Regs& ctx, uint8_t* base);
void Type8ListInnerPage0(lifted::Regs& ctx, uint8_t* base);
void Type8PartInnerStep1(lifted::Regs& ctx, uint8_t* base);
void Type8PartInnerPage1(lifted::Regs& ctx, uint8_t* base);
void Type8PartInnerPage2(lifted::Regs& ctx, uint8_t* base);

// The color list screen's list part: the color block and the inner
// part's constructor, destructor and IsFirstChoice have no adapters (only
// native callers).
void Type8ColorBlockConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockSetUp(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockPlaceCursorA(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockPlaceCursorB(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockPlaceCursorC(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockPlayOpening(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockShowSelections(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockShowMenuMove(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockPlayReady(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockSecondMoved(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockUpdateSecond(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockThirdMoved(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockUpdateThird(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockWhileReady(lifted::Regs& ctx, uint8_t* base);
void Type8ColorBlockInput(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorPartConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorPartDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorPartDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerIsFirstChoice(lifted::Regs& ctx, uint8_t* base);
void Type8ColorPartIsFirstChoice(lifted::Regs& ctx, uint8_t* base);
void Type8ColorPartThirdChoice(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerOpen(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerPick(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerInput(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerStep0(lifted::Regs& ctx, uint8_t* base);
void Type8ColorInnerPage0(lifted::Regs& ctx, uint8_t* base);

// The list screen: its constructors, destructors, slots, tables' pages and
// steps (DimList and PairedNodeTable::Clear have no adapters: only the
// native pages call them).
void Type8ListBaseConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListScreenConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListBaseDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ListIsLoaded(lifted::Regs& ctx, uint8_t* base);
void Type8ListOpenMovie(lifted::Regs& ctx, uint8_t* base);
void Type8ListRelease(lifted::Regs& ctx, uint8_t* base);
void Type8ListSetItemTexts(lifted::Regs& ctx, uint8_t* base);
void Type8ListItemInfo(lifted::Regs& ctx, uint8_t* base);
void Type8ListShowMarks(lifted::Regs& ctx, uint8_t* base);
void Type8ListSetLabels(lifted::Regs& ctx, uint8_t* base);
void Type8ListSetUpMenus(lifted::Regs& ctx, uint8_t* base);
void Type8ListSetUpRace(lifted::Regs& ctx, uint8_t* base);
void Type8ListDraw(lifted::Regs& ctx, uint8_t* base);
void Type8ListUpdate(lifted::Regs& ctx, uint8_t* base);
void Type8ListPage2(lifted::Regs& ctx, uint8_t* base);
void Type8ListPage3(lifted::Regs& ctx, uint8_t* base);
void Type8ListPage4(lifted::Regs& ctx, uint8_t* base);
void Type8ListPage5(lifted::Regs& ctx, uint8_t* base);
void Type8ListPage6(lifted::Regs& ctx, uint8_t* base);
void Type8ListPage8(lifted::Regs& ctx, uint8_t* base);
void Type8ListStep0(lifted::Regs& ctx, uint8_t* base);
void Type8ListStep1(lifted::Regs& ctx, uint8_t* base);
void Type8ListStep2(lifted::Regs& ctx, uint8_t* base);
void Type8ListStep3(lifted::Regs& ctx, uint8_t* base);

// The color list screen: its constructor, destructor, machine and slots.
void Type8ColorListConstruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListOpenMovie(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListItemInfo(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListInitMachine(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListSetItemText(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListClearItemText(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListSetDetails(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListSetLabels(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListSetUpMenus(lifted::Regs& ctx, uint8_t* base);
void Type8ColorListSetUpRace(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
