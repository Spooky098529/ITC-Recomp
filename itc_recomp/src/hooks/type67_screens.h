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
type67_screens::Calls Type67ScreensCalls(lifted::Regs& ctx, uint8_t* base);

// The list screens.
void ListScreenOnEnter(lifted::Regs& ctx, uint8_t* base);
void ListScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void ListScreenWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void ListScreenLeave(lifted::Regs& ctx, uint8_t* base);
void ListScreenOnLeft(lifted::Regs& ctx, uint8_t* base);
void ListScreenIsLoaded(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenRelease(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenRelease(lifted::Regs& ctx, uint8_t* base);

// The node screen.
void NodeScreenOnEnter(lifted::Regs& ctx, uint8_t* base);
void NodeScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void NodeScreenWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void NodeScreenLeave(lifted::Regs& ctx, uint8_t* base);
void NodeScreenOnLeft(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenIsLoaded(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenRelease(lifted::Regs& ctx, uint8_t* base);

// The panel groups and the group pair (PanelGroup::Run sub_82259008 has
// no adapter: only GroupPair::Run calls it).
void PanelGroupMovie(lifted::Regs& ctx, uint8_t* base);
void GroupPairSelectA(lifted::Regs& ctx, uint8_t* base);
void GroupPairReset(lifted::Regs& ctx, uint8_t* base);
void GroupPairRun(lifted::Regs& ctx, uint8_t* base);
void GroupPairWaitForFade(lifted::Regs& ctx, uint8_t* base);
void GroupPairFinishAll(lifted::Regs& ctx, uint8_t* base);
void GroupPairDestruct(lifted::Regs& ctx, uint8_t* base);
void SingleGroupConstruct(lifted::Regs& ctx, uint8_t* base);
void SingleGroupReset(lifted::Regs& ctx, uint8_t* base);
void PanelFadeIn(lifted::Regs& ctx, uint8_t* base);
void MachineScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void PanelGroupDestruct(lifted::Regs& ctx, uint8_t* base);
void PanelGroupDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);

// The panel screens' slots and handlers (Type7PanelScreen::DrawList
// sub_82240908 has no adapter: only Type7PanelScreen::Draw calls it).
void PanelScreenIsLoaded(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenOpenMovies(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenOpenMovies(lifted::Regs& ctx, uint8_t* base);
void PanelScreenDeleteMovies(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenBack(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenOpenPowerGraph(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenOpenPowerGraph(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenCarList(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenCarEntry(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenCarSlot(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOnEnter(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void PanelScreenWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOpen(lifted::Regs& ctx, uint8_t* base);
void PanelScreenShow(lifted::Regs& ctx, uint8_t* base);
void PanelScreenUpdate(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOnMessage24(lifted::Regs& ctx, uint8_t* base);
void PanelScreenLeave(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOnLeft(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenDraw(lifted::Regs& ctx, uint8_t* base);
void Type7PanelScreenPickCar(lifted::Regs& ctx, uint8_t* base);

// The node tables.
void NodeTableDestruct(lifted::Regs& ctx, uint8_t* base);
void NodeTableDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void NodeTableSetPairs(lifted::Regs& ctx, uint8_t* base);
void PairedNodeTableSavePairs(lifted::Regs& ctx, uint8_t* base);
void ColorNodeTableClear(lifted::Regs& ctx, uint8_t* base);
void ColorNodeTableSet(lifted::Regs& ctx, uint8_t* base);
void ColorNodeTableSavePairs(lifted::Regs& ctx, uint8_t* base);
void NodeTableShow(lifted::Regs& ctx, uint8_t* base);
void NodeTableShowNode(lifted::Regs& ctx, uint8_t* base);
void NodeTableSetMarks(lifted::Regs& ctx, uint8_t* base);
void ColorNodeTableSetColors(lifted::Regs& ctx, uint8_t* base);
void ColorNodeTableClearColors(lifted::Regs& ctx, uint8_t* base);
void ColorNodeTableMarkSelected(lifted::Regs& ctx, uint8_t* base);
void ColorNodeTableRefresh(lifted::Regs& ctx, uint8_t* base);
void ColorNodeTableReset(lifted::Regs& ctx, uint8_t* base);
void PairedNodeTableRefresh(lifted::Regs& ctx, uint8_t* base);

// The list part and its inner part.
void ListPartBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void ListPartBaseDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerBaseDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerDestruct(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void ListPartDestruct(lifted::Regs& ctx, uint8_t* base);
void ListPartDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void ListPartOpen(lifted::Regs& ctx, uint8_t* base);
void ListPartPick(lifted::Regs& ctx, uint8_t* base);
void ListPartInput(lifted::Regs& ctx, uint8_t* base);
void ListPartClose(lifted::Regs& ctx, uint8_t* base);
void ListPartIsIdle(lifted::Regs& ctx, uint8_t* base);
void ListPartCanLeave(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerOpen(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerPick(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerInput(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerIsIdle(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerClose(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerCanLeave(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerPage0(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerPage1(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerPage2(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerStep0(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerStep1(lifted::Regs& ctx, uint8_t* base);
void ListPartInnerStep2(lifted::Regs& ctx, uint8_t* base);

// The list screens' shared helpers, item loaders and handlers.
void ListScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void ListScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void NodeListScreenItemInfo(lifted::Regs& ctx, uint8_t* base);
void NodeListScreenLoadItem(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenLoadItem(lifted::Regs& ctx, uint8_t* base);
void ListScreenShowSelection(lifted::Regs& ctx, uint8_t* base);
void ListScreenLoopItemNode(lifted::Regs& ctx, uint8_t* base);
void ListScreenLoopSecondNode(lifted::Regs& ctx, uint8_t* base);
void ListScreenLoopThirdNode(lifted::Regs& ctx, uint8_t* base);
void ListScreenSetScrollText(lifted::Regs& ctx, uint8_t* base);
void ListScreenClearScrollText(lifted::Regs& ctx, uint8_t* base);
void ListScreenSetArrows(lifted::Regs& ctx, uint8_t* base);
void ListScreenOnToggleMoved(lifted::Regs& ctx, uint8_t* base);
void ListScreenShowToggle(lifted::Regs& ctx, uint8_t* base);
void ListScreenSetIcons(lifted::Regs& ctx, uint8_t* base);
void ListScreenSetUpMovie(lifted::Regs& ctx, uint8_t* base);
void ListScreenShow(lifted::Regs& ctx, uint8_t* base);
void ListScreenDraw(lifted::Regs& ctx, uint8_t* base);
void ListScreenUpdate(lifted::Regs& ctx, uint8_t* base);
void ListScreenSuspend(lifted::Regs& ctx, uint8_t* base);
void ListScreenResume(lifted::Regs& ctx, uint8_t* base);
void ListScreenOpen(lifted::Regs& ctx, uint8_t* base);

// The titled list screen.
void TitledListScreenReset(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenDraw(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenUpdate(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenPage2(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenPage3(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenPage4(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenPage7(lifted::Regs& ctx, uint8_t* base);
void ListScreenLeavePage(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenStep0(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenStep1(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenStep2(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenOnItemMoved(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void NodeListScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void TitledListScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void NodeListScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);

// The color list screen.
void ColorListScreenReset(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenDraw(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenUpdate(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPage2(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPage3(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPage4(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPage5(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPage6(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPage7(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPage9(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPlaceItemCursor(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenPlaceColorCursor(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenShowPartNodes(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep0(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep1(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep2(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep3(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep4(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenRunMovie(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep6(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep7(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep8(lifted::Regs& ctx, uint8_t* base);
void ColorListScreenStep10(lifted::Regs& ctx, uint8_t* base);

// The type 6 list screen's slots and destructors, the panel and node
// screens' leaves.
void Type6ListScreenSetItemTexts(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenShowMarks(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenItemInfo(lifted::Regs& ctx, uint8_t* base);
void NodeScreenItemInfo(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenSetUpMenus(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenBack(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenSetUpRace(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void Type6ListScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOnYes(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOnNo(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOnBack(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOnDialogClosed(lifted::Regs& ctx, uint8_t* base);
void PanelScreenSuspend(lifted::Regs& ctx, uint8_t* base);
void PanelScreenOpenEmpty(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenBack(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenCarList(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenCarEntry(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenCarSlot(lifted::Regs& ctx, uint8_t* base);
void GroupPairDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void NodeScreenOpen(lifted::Regs& ctx, uint8_t* base);

// The type 7 list screen's slots and destructors.
void Type7ListScreenSetItemText(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenClearItemText(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenSetTitle(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenSetDetails(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenShowMarks(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenItemInfo(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenSetUpMenus(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenBack(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenSetUpRace(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void Type7ListScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);

// The node screen.
void NodeScreenShow(lifted::Regs& ctx, uint8_t* base);
void NodeScreenDraw(lifted::Regs& ctx, uint8_t* base);
void NodeScreenUpdate(lifted::Regs& ctx, uint8_t* base);
void NodeScreenPage2(lifted::Regs& ctx, uint8_t* base);
void NodeScreenPage4(lifted::Regs& ctx, uint8_t* base);
void NodeScreenPage5(lifted::Regs& ctx, uint8_t* base);
void NodeScreenStep0(lifted::Regs& ctx, uint8_t* base);
void NodeScreenStep1(lifted::Regs& ctx, uint8_t* base);
void NodeScreenStep2(lifted::Regs& ctx, uint8_t* base);
void NodeScreenShowSelection(lifted::Regs& ctx, uint8_t* base);
void NodeScreenOnToggleMoved(lifted::Regs& ctx, uint8_t* base);
void NodeScreenRefresh(lifted::Regs& ctx, uint8_t* base);
void NodeScreenShowRecords(lifted::Regs& ctx, uint8_t* base);
void NodeScreenSetArrows(lifted::Regs& ctx, uint8_t* base);
void NodeScreenLoadItem(lifted::Regs& ctx, uint8_t* base);
void NodeScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void NodeScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);

// The type 6 node screen's own slots and destructors.
void Type6NodeScreenSetUpMenus(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenSetText(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenBack(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenSetIcons(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenSetScrollText(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenShowMarks(lifted::Regs& ctx, uint8_t* base);
void NodeScreenSetUpTexts(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenDestruct(lifted::Regs& ctx, uint8_t* base);
void Type6NodeScreenDeletingDestruct(lifted::Regs& ctx, uint8_t* base);

// The type 6 panel screen's Draw and PickCar (its DrawList and both panel
// screens' AskQuit have no adapters: only the native Draws and DrawLists
// call them).
void Type6PanelScreenDraw(lifted::Regs& ctx, uint8_t* base);
void Type6PanelScreenPickCar(lifted::Regs& ctx, uint8_t* base);

// The screen blocks' destructors.
void Type6ScreensDestruct(lifted::Regs& ctx, uint8_t* base);
void Type7ScreensDestruct(lifted::Regs& ctx, uint8_t* base);
void Type6ScreensDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type7ScreensDeletingDestruct(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
