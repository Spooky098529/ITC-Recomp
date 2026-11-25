// The type 6 and 7 modes' list, panel and node screens' register steps
// (src/type67_screens/, docs/functions/type67-screens.md), shared by the
// adapters (src/hooks/type67_screens.cpp, behind --native_type67_screens)
// and the harness (without the flag). Each takes the registers its original
// takes (r3 the screen or object, r4 the dispatcher where the original
// reads it) and leaves r3 as the original does; guest calls go through
// `ctx` (hooks::Type67Calls).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "type67_screens/type67_screens.h"

namespace hooks {

// The plain C++'s calls: hooks::Type67Calls, and --native_heap for
// MenuList::Release's plain C++.
type67_screens::Calls Type67ScreensCalls(lifted::Regs& ctx);

// The list screens.
void ListScreenOnEnter(lifted::Regs& ctx);
void ListScreenOpenMovie(lifted::Regs& ctx);
void ListScreenWaitForMovie(lifted::Regs& ctx);
void ListScreenLeave(lifted::Regs& ctx);
void ListScreenOnLeft(lifted::Regs& ctx);
void ListScreenIsLoaded(lifted::Regs& ctx);
void Type6ListScreenOpenMovie(lifted::Regs& ctx);
void Type7ListScreenOpenMovie(lifted::Regs& ctx);
void Type6ListScreenRelease(lifted::Regs& ctx);
void Type7ListScreenRelease(lifted::Regs& ctx);

// The node screen.
void NodeScreenOnEnter(lifted::Regs& ctx);
void NodeScreenOpenMovie(lifted::Regs& ctx);
void NodeScreenWaitForMovie(lifted::Regs& ctx);
void NodeScreenLeave(lifted::Regs& ctx);
void NodeScreenOnLeft(lifted::Regs& ctx);
void Type6NodeScreenIsLoaded(lifted::Regs& ctx);
void Type6NodeScreenOpenMovie(lifted::Regs& ctx);
void Type6NodeScreenRelease(lifted::Regs& ctx);

// The panel groups and the group pair (PanelGroup::Run sub_82259008 has
// no adapter: only GroupPair::Run calls it).
void PanelGroupMovie(lifted::Regs& ctx);
void GroupPairSelectA(lifted::Regs& ctx);
void GroupPairReset(lifted::Regs& ctx);
void GroupPairRun(lifted::Regs& ctx);
void GroupPairWaitForFade(lifted::Regs& ctx);
void GroupPairFinishAll(lifted::Regs& ctx);
void GroupPairDestruct(lifted::Regs& ctx);
void SingleGroupConstruct(lifted::Regs& ctx);
void SingleGroupReset(lifted::Regs& ctx);
void PanelFadeIn(lifted::Regs& ctx);
void MachineScreenDestruct(lifted::Regs& ctx);
void PanelGroupDestruct(lifted::Regs& ctx);
void PanelGroupDeletingDestruct(lifted::Regs& ctx);
void Type6PanelScreenDestruct(lifted::Regs& ctx);
void Type7PanelScreenDestruct(lifted::Regs& ctx);
void Type6PanelScreenDeletingDestruct(lifted::Regs& ctx);

// The panel screens' slots and handlers (Type7PanelScreen::DrawList
// sub_82240908 has no adapter: only Type7PanelScreen::Draw calls it).
void PanelScreenIsLoaded(lifted::Regs& ctx);
void Type6PanelScreenOpenMovies(lifted::Regs& ctx);
void Type7PanelScreenOpenMovies(lifted::Regs& ctx);
void PanelScreenDeleteMovies(lifted::Regs& ctx);
void Type7PanelScreenBack(lifted::Regs& ctx);
void Type6PanelScreenOpenPowerGraph(lifted::Regs& ctx);
void Type7PanelScreenOpenPowerGraph(lifted::Regs& ctx);
void Type7PanelScreenCarList(lifted::Regs& ctx);
void Type7PanelScreenCarEntry(lifted::Regs& ctx);
void Type7PanelScreenCarSlot(lifted::Regs& ctx);
void PanelScreenOnEnter(lifted::Regs& ctx);
void PanelScreenOpenMovie(lifted::Regs& ctx);
void PanelScreenWaitForMovie(lifted::Regs& ctx);
void PanelScreenOpen(lifted::Regs& ctx);
void PanelScreenShow(lifted::Regs& ctx);
void PanelScreenUpdate(lifted::Regs& ctx);
void PanelScreenOnMessage24(lifted::Regs& ctx);
void PanelScreenLeave(lifted::Regs& ctx);
void PanelScreenOnLeft(lifted::Regs& ctx);
void Type7PanelScreenDraw(lifted::Regs& ctx);
void Type7PanelScreenPickCar(lifted::Regs& ctx);

// The node tables.
void NodeTableDestruct(lifted::Regs& ctx);
void NodeTableDeletingDestruct(lifted::Regs& ctx);
void NodeTableSetPairs(lifted::Regs& ctx);
void PairedNodeTableSavePairs(lifted::Regs& ctx);
void ColorNodeTableClear(lifted::Regs& ctx);
void ColorNodeTableSet(lifted::Regs& ctx);
void ColorNodeTableSavePairs(lifted::Regs& ctx);
void NodeTableShow(lifted::Regs& ctx);
void NodeTableShowNode(lifted::Regs& ctx);
void NodeTableSetMarks(lifted::Regs& ctx);
void ColorNodeTableSetColors(lifted::Regs& ctx);
void ColorNodeTableClearColors(lifted::Regs& ctx);
void ColorNodeTableMarkSelected(lifted::Regs& ctx);
void ColorNodeTableRefresh(lifted::Regs& ctx);
void ColorNodeTableReset(lifted::Regs& ctx);
void PairedNodeTableRefresh(lifted::Regs& ctx);

// The list part and its inner part.
void ListPartBaseDestruct(lifted::Regs& ctx);
void ListPartInnerBaseDestruct(lifted::Regs& ctx);
void ListPartBaseDeletingDestruct(lifted::Regs& ctx);
void ListPartInnerBaseDeletingDestruct(lifted::Regs& ctx);
void ListPartInnerDestruct(lifted::Regs& ctx);
void ListPartInnerDeletingDestruct(lifted::Regs& ctx);
void ListPartDestruct(lifted::Regs& ctx);
void ListPartDeletingDestruct(lifted::Regs& ctx);
void ListPartOpen(lifted::Regs& ctx);
void ListPartPick(lifted::Regs& ctx);
void ListPartInput(lifted::Regs& ctx);
void ListPartClose(lifted::Regs& ctx);
void ListPartIsIdle(lifted::Regs& ctx);
void ListPartCanLeave(lifted::Regs& ctx);
void ListPartInnerOpen(lifted::Regs& ctx);
void ListPartInnerPick(lifted::Regs& ctx);
void ListPartInnerInput(lifted::Regs& ctx);
void ListPartInnerIsIdle(lifted::Regs& ctx);
void ListPartInnerClose(lifted::Regs& ctx);
void ListPartInnerCanLeave(lifted::Regs& ctx);
void ListPartInnerPage0(lifted::Regs& ctx);
void ListPartInnerPage1(lifted::Regs& ctx);
void ListPartInnerPage2(lifted::Regs& ctx);
void ListPartInnerStep0(lifted::Regs& ctx);
void ListPartInnerStep1(lifted::Regs& ctx);
void ListPartInnerStep2(lifted::Regs& ctx);

// The list screens' shared helpers, item loaders and handlers.
void ListScreenDestruct(lifted::Regs& ctx);
void ListScreenDeletingDestruct(lifted::Regs& ctx);
void NodeListScreenItemInfo(lifted::Regs& ctx);
void NodeListScreenLoadItem(lifted::Regs& ctx);
void ColorListScreenLoadItem(lifted::Regs& ctx);
void ListScreenShowSelection(lifted::Regs& ctx);
void ListScreenLoopItemNode(lifted::Regs& ctx);
void ListScreenLoopSecondNode(lifted::Regs& ctx);
void ListScreenLoopThirdNode(lifted::Regs& ctx);
void ListScreenSetScrollText(lifted::Regs& ctx);
void ListScreenClearScrollText(lifted::Regs& ctx);
void ListScreenSetArrows(lifted::Regs& ctx);
void ListScreenOnToggleMoved(lifted::Regs& ctx);
void ListScreenShowToggle(lifted::Regs& ctx);
void ListScreenSetIcons(lifted::Regs& ctx);
void ListScreenSetUpMovie(lifted::Regs& ctx);
void ListScreenShow(lifted::Regs& ctx);
void ListScreenDraw(lifted::Regs& ctx);
void ListScreenUpdate(lifted::Regs& ctx);
void ListScreenSuspend(lifted::Regs& ctx);
void ListScreenResume(lifted::Regs& ctx);
void ListScreenOpen(lifted::Regs& ctx);

// The titled list screen.
void TitledListScreenReset(lifted::Regs& ctx);
void TitledListScreenDraw(lifted::Regs& ctx);
void TitledListScreenUpdate(lifted::Regs& ctx);
void TitledListScreenPage2(lifted::Regs& ctx);
void TitledListScreenPage3(lifted::Regs& ctx);
void TitledListScreenPage4(lifted::Regs& ctx);
void TitledListScreenPage7(lifted::Regs& ctx);
void ListScreenLeavePage(lifted::Regs& ctx);
void TitledListScreenStep0(lifted::Regs& ctx);
void TitledListScreenStep1(lifted::Regs& ctx);
void TitledListScreenStep2(lifted::Regs& ctx);
void TitledListScreenOnItemMoved(lifted::Regs& ctx);
void TitledListScreenDestruct(lifted::Regs& ctx);
void NodeListScreenDestruct(lifted::Regs& ctx);
void TitledListScreenDeletingDestruct(lifted::Regs& ctx);
void NodeListScreenDeletingDestruct(lifted::Regs& ctx);

// The color list screen.
void ColorListScreenReset(lifted::Regs& ctx);
void ColorListScreenDraw(lifted::Regs& ctx);
void ColorListScreenUpdate(lifted::Regs& ctx);
void ColorListScreenPage2(lifted::Regs& ctx);
void ColorListScreenPage3(lifted::Regs& ctx);
void ColorListScreenPage4(lifted::Regs& ctx);
void ColorListScreenPage5(lifted::Regs& ctx);
void ColorListScreenPage6(lifted::Regs& ctx);
void ColorListScreenPage7(lifted::Regs& ctx);
void ColorListScreenPage9(lifted::Regs& ctx);
void ColorListScreenPlaceItemCursor(lifted::Regs& ctx);
void ColorListScreenPlaceColorCursor(lifted::Regs& ctx);
void ColorListScreenShowPartNodes(lifted::Regs& ctx);
void ColorListScreenDestruct(lifted::Regs& ctx);
void ColorListScreenDeletingDestruct(lifted::Regs& ctx);
void ColorListScreenStep0(lifted::Regs& ctx);
void ColorListScreenStep1(lifted::Regs& ctx);
void ColorListScreenStep2(lifted::Regs& ctx);
void ColorListScreenStep3(lifted::Regs& ctx);
void ColorListScreenStep4(lifted::Regs& ctx);
void ColorListScreenRunMovie(lifted::Regs& ctx);
void ColorListScreenStep6(lifted::Regs& ctx);
void ColorListScreenStep7(lifted::Regs& ctx);
void ColorListScreenStep8(lifted::Regs& ctx);
void ColorListScreenStep10(lifted::Regs& ctx);

// The type 6 list screen's slots and destructors, the panel and node
// screens' leaves.
void Type6ListScreenSetItemTexts(lifted::Regs& ctx);
void Type6ListScreenShowMarks(lifted::Regs& ctx);
void Type6ListScreenItemInfo(lifted::Regs& ctx);
void NodeScreenItemInfo(lifted::Regs& ctx);
void Type6ListScreenSetUpMenus(lifted::Regs& ctx);
void Type6ListScreenBack(lifted::Regs& ctx);
void Type6ListScreenSetUpRace(lifted::Regs& ctx);
void Type6ListScreenDestruct(lifted::Regs& ctx);
void Type6ListScreenDeletingDestruct(lifted::Regs& ctx);
void PanelScreenOnYes(lifted::Regs& ctx);
void PanelScreenOnNo(lifted::Regs& ctx);
void PanelScreenOnBack(lifted::Regs& ctx);
void PanelScreenOnDialogClosed(lifted::Regs& ctx);
void PanelScreenSuspend(lifted::Regs& ctx);
void PanelScreenOpenEmpty(lifted::Regs& ctx);
void Type6PanelScreenBack(lifted::Regs& ctx);
void Type6PanelScreenCarList(lifted::Regs& ctx);
void Type6PanelScreenCarEntry(lifted::Regs& ctx);
void Type6PanelScreenCarSlot(lifted::Regs& ctx);
void GroupPairDeletingDestruct(lifted::Regs& ctx);
void NodeScreenOpen(lifted::Regs& ctx);

// The type 7 list screen's slots and destructors.
void Type7ListScreenSetItemText(lifted::Regs& ctx);
void Type7ListScreenClearItemText(lifted::Regs& ctx);
void Type7ListScreenSetTitle(lifted::Regs& ctx);
void Type7ListScreenSetDetails(lifted::Regs& ctx);
void Type7ListScreenShowMarks(lifted::Regs& ctx);
void Type7ListScreenItemInfo(lifted::Regs& ctx);
void Type7ListScreenSetUpMenus(lifted::Regs& ctx);
void Type7ListScreenBack(lifted::Regs& ctx);
void Type7ListScreenSetUpRace(lifted::Regs& ctx);
void Type7ListScreenDestruct(lifted::Regs& ctx);
void Type7ListScreenDeletingDestruct(lifted::Regs& ctx);

// The node screen.
void NodeScreenShow(lifted::Regs& ctx);
void NodeScreenDraw(lifted::Regs& ctx);
void NodeScreenUpdate(lifted::Regs& ctx);
void NodeScreenPage2(lifted::Regs& ctx);
void NodeScreenPage4(lifted::Regs& ctx);
void NodeScreenPage5(lifted::Regs& ctx);
void NodeScreenStep0(lifted::Regs& ctx);
void NodeScreenStep1(lifted::Regs& ctx);
void NodeScreenStep2(lifted::Regs& ctx);
void NodeScreenShowSelection(lifted::Regs& ctx);
void NodeScreenOnToggleMoved(lifted::Regs& ctx);
void NodeScreenRefresh(lifted::Regs& ctx);
void NodeScreenShowRecords(lifted::Regs& ctx);
void NodeScreenSetArrows(lifted::Regs& ctx);
void NodeScreenLoadItem(lifted::Regs& ctx);
void NodeScreenDestruct(lifted::Regs& ctx);
void NodeScreenDeletingDestruct(lifted::Regs& ctx);

// The type 6 node screen's own slots and destructors.
void Type6NodeScreenSetUpMenus(lifted::Regs& ctx);
void Type6NodeScreenSetText(lifted::Regs& ctx);
void Type6NodeScreenBack(lifted::Regs& ctx);
void Type6NodeScreenSetIcons(lifted::Regs& ctx);
void Type6NodeScreenSetScrollText(lifted::Regs& ctx);
void Type6NodeScreenShowMarks(lifted::Regs& ctx);
void NodeScreenSetUpTexts(lifted::Regs& ctx);
void Type6NodeScreenDestruct(lifted::Regs& ctx);
void Type6NodeScreenDeletingDestruct(lifted::Regs& ctx);

// The type 6 panel screen's Draw and PickCar (its DrawList and both panel
// screens' AskQuit have no adapters: only the native Draws and DrawLists
// call them).
void Type6PanelScreenDraw(lifted::Regs& ctx);
void Type6PanelScreenPickCar(lifted::Regs& ctx);

// The screen blocks' destructors.
void Type6ScreensDestruct(lifted::Regs& ctx);
void Type7ScreensDestruct(lifted::Regs& ctx);
void Type6ScreensDeletingDestruct(lifted::Regs& ctx);
void Type7ScreensDeletingDestruct(lifted::Regs& ctx);

}  // namespace hooks
