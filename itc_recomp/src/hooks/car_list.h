// The car list's register steps (src/car_list/, docs/functions/car-list.md),
// shared by the adapters (src/hooks/car_list.cpp, behind --native_car_list)
// and the harness (without the flag). Each takes the registers its original
// takes and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The panel groups and panels (r3 the group or panel, r4 to r7 their
// arguments).
void CarListSetCaption(PPCContext& ctx, uint8_t* base);
void CarListSetCaption(lifted::Regs& ctx, uint8_t* base);
void CarListPartFinishAll(PPCContext& ctx, uint8_t* base);
void CarListPartFinishAll(lifted::Regs& ctx, uint8_t* base);
void CarListTabSetCursorY(PPCContext& ctx, uint8_t* base);
void CarListTabSetCursorY(lifted::Regs& ctx, uint8_t* base);
void CarListTabSetButtons(PPCContext& ctx, uint8_t* base);
void CarListTabSetButtons(lifted::Regs& ctx, uint8_t* base);
void CarListRowSetRow(PPCContext& ctx, uint8_t* base);
void CarListRowSetRow(lifted::Regs& ctx, uint8_t* base);
void CarListRowSetArrows(PPCContext& ctx, uint8_t* base);
void CarListRowSetArrows(lifted::Regs& ctx, uint8_t* base);
void CarListRowUnhighlight(PPCContext& ctx, uint8_t* base);
void CarListRowUnhighlight(lifted::Regs& ctx, uint8_t* base);
void CarListRowHide(PPCContext& ctx, uint8_t* base);
void CarListRowHide(lifted::Regs& ctx, uint8_t* base);
void CarListColorShow(PPCContext& ctx, uint8_t* base);
void CarListColorShow(lifted::Regs& ctx, uint8_t* base);
void CarListColorSelect(PPCContext& ctx, uint8_t* base);
void CarListColorSelect(lifted::Regs& ctx, uint8_t* base);
void CarListColorSetArrows(PPCContext& ctx, uint8_t* base);
void CarListColorSetArrows(lifted::Regs& ctx, uint8_t* base);
void CarListColorShowSwatches(PPCContext& ctx, uint8_t* base);
void CarListColorShowSwatches(lifted::Regs& ctx, uint8_t* base);
void CarListColorPlaceCursor(PPCContext& ctx, uint8_t* base);
void CarListColorPlaceCursor(lifted::Regs& ctx, uint8_t* base);
void CarListColorSetCount(PPCContext& ctx, uint8_t* base);
void CarListColorSetCount(lifted::Regs& ctx, uint8_t* base);
void CarListColorSetSwatch(PPCContext& ctx, uint8_t* base);
void CarListColorSetSwatch(lifted::Regs& ctx, uint8_t* base);
void CarListColorSetSwatches(PPCContext& ctx, uint8_t* base);
void CarListColorSetSwatches(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetTexts(PPCContext& ctx, uint8_t* base);
void CarListCarSetTexts(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetButtons(PPCContext& ctx, uint8_t* base);
void CarListCarSetButtons(lifted::Regs& ctx, uint8_t* base);
void CarListCarPlaceCursor(PPCContext& ctx, uint8_t* base);
void CarListCarPlaceCursor(lifted::Regs& ctx, uint8_t* base);
void CarListCarSelect(PPCContext& ctx, uint8_t* base);
void CarListCarSelect(lifted::Regs& ctx, uint8_t* base);
void CarListCarStopArrows(PPCContext& ctx, uint8_t* base);
void CarListCarStopArrows(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetName(PPCContext& ctx, uint8_t* base);
void CarListCarSetName(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetStats(PPCContext& ctx, uint8_t* base);
void CarListCarSetStats(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetCarName(PPCContext& ctx, uint8_t* base);
void CarListCarSetCarName(lifted::Regs& ctx, uint8_t* base);
void CarListPanel7CShowArrows(PPCContext& ctx, uint8_t* base);
void CarListPanel7CShowArrows(lifted::Regs& ctx, uint8_t* base);
void CarListCarShowStats(PPCContext& ctx, uint8_t* base);
void CarListCarShowStats(lifted::Regs& ctx, uint8_t* base);
void CarListTabSetTab(PPCContext& ctx, uint8_t* base);
void CarListTabSetTab(lifted::Regs& ctx, uint8_t* base);
void CarListTabMove(PPCContext& ctx, uint8_t* base);
void CarListTabMove(lifted::Regs& ctx, uint8_t* base);
void CarListRowFill(PPCContext& ctx, uint8_t* base);
void CarListRowFill(lifted::Regs& ctx, uint8_t* base);
void CarListRowRefresh(PPCContext& ctx, uint8_t* base);
void CarListRowRefresh(lifted::Regs& ctx, uint8_t* base);
void CarListRowMove(PPCContext& ctx, uint8_t* base);
void CarListRowMove(lifted::Regs& ctx, uint8_t* base);
void CarListColorMove(PPCContext& ctx, uint8_t* base);
void CarListColorMove(lifted::Regs& ctx, uint8_t* base);
// The panels' answers (r3 the panel).
void CarListTabIsShown(PPCContext& ctx, uint8_t* base);
void CarListTabIsShown(lifted::Regs& ctx, uint8_t* base);
void CarListRowIsShown(PPCContext& ctx, uint8_t* base);
void CarListRowIsShown(lifted::Regs& ctx, uint8_t* base);
void CarListRowIsHidden(PPCContext& ctx, uint8_t* base);
void CarListRowIsHidden(lifted::Regs& ctx, uint8_t* base);
void CarListCarIsShown(PPCContext& ctx, uint8_t* base);
void CarListCarIsShown(lifted::Regs& ctx, uint8_t* base);
void CarListColorIsShown(PPCContext& ctx, uint8_t* base);
void CarListColorIsShown(lifted::Regs& ctx, uint8_t* base);
void CarListColorIsIdle(PPCContext& ctx, uint8_t* base);
void CarListColorIsIdle(lifted::Regs& ctx, uint8_t* base);
// The panels' shows, selections and moves.
void CarListTabShow(PPCContext& ctx, uint8_t* base);
void CarListTabShow(lifted::Regs& ctx, uint8_t* base);
void CarListTabSelect(PPCContext& ctx, uint8_t* base);
void CarListTabSelect(lifted::Regs& ctx, uint8_t* base);
void CarListRowShow(PPCContext& ctx, uint8_t* base);
void CarListRowShow(lifted::Regs& ctx, uint8_t* base);
void CarListRowSelect(PPCContext& ctx, uint8_t* base);
void CarListRowSelect(lifted::Regs& ctx, uint8_t* base);
void CarListCarShow(PPCContext& ctx, uint8_t* base);
void CarListCarShow(lifted::Regs& ctx, uint8_t* base);
void CarListCarMove(PPCContext& ctx, uint8_t* base);
void CarListCarMove(lifted::Regs& ctx, uint8_t* base);
void CarListCarPick(PPCContext& ctx, uint8_t* base);
void CarListCarPick(lifted::Regs& ctx, uint8_t* base);
// PowerGraph::SetEntry (r3 an entry, r4 the tab).
void CarListPowerGraphSetEntry(PPCContext& ctx, uint8_t* base);
void CarListPowerGraphSetEntry(lifted::Regs& ctx, uint8_t* base);

// PanelSet's leaves (r3 the set or its base, r4 a value).
void CarListSetReset(PPCContext& ctx, uint8_t* base);
void CarListSetReset(lifted::Regs& ctx, uint8_t* base);
void CarListSetPicked(PPCContext& ctx, uint8_t* base);
void CarListSetPicked(lifted::Regs& ctx, uint8_t* base);
void CarListSetWentBack(PPCContext& ctx, uint8_t* base);
void CarListSetWentBack(lifted::Regs& ctx, uint8_t* base);
void CarListSetWantsPowerGraph(PPCContext& ctx, uint8_t* base);
void CarListSetWantsPowerGraph(lifted::Regs& ctx, uint8_t* base);
void CarListSetOpenKind(PPCContext& ctx, uint8_t* base);
void CarListSetOpenKind(lifted::Regs& ctx, uint8_t* base);
void CarListSetPreviousTab(PPCContext& ctx, uint8_t* base);
void CarListSetPreviousTab(lifted::Regs& ctx, uint8_t* base);
void CarListSetPreviousEntry(PPCContext& ctx, uint8_t* base);
void CarListSetPreviousEntry(lifted::Regs& ctx, uint8_t* base);
void CarListSetPreviousColor(PPCContext& ctx, uint8_t* base);
void CarListSetPreviousColor(lifted::Regs& ctx, uint8_t* base);
void CarListPickedPage(PPCContext& ctx, uint8_t* base);
void CarListPickedPage(lifted::Regs& ctx, uint8_t* base);
void CarListSaveData(PPCContext& ctx, uint8_t* base);
void CarListSaveData(lifted::Regs& ctx, uint8_t* base);

// PanelSet's pages and steps (r3 the set or its base b, a step's r4 its
// first-pass word).
void CarListToTabs(PPCContext& ctx, uint8_t* base);
void CarListToTabs(lifted::Regs& ctx, uint8_t* base);
void CarListOpen(PPCContext& ctx, uint8_t* base);
void CarListOpen(lifted::Regs& ctx, uint8_t* base);
void CarListBaseFree(PPCContext& ctx, uint8_t* base);
void CarListBaseFree(lifted::Regs& ctx, uint8_t* base);
void CarListSetFree(PPCContext& ctx, uint8_t* base);
void CarListSetFree(lifted::Regs& ctx, uint8_t* base);
// The destructors (r3 the object; r4 the flags for the deleting one).
void CarListGroupBaseDestruct(PPCContext& ctx, uint8_t* base);
void CarListGroupBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListSetBaseDestruct(PPCContext& ctx, uint8_t* base);
void CarListSetBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListSetDestruct(PPCContext& ctx, uint8_t* base);
void CarListSetDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListSetInterfaceDeletingDestruct(PPCContext& ctx, uint8_t* base);
void CarListSetInterfaceDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListBaseHandleInput(PPCContext& ctx, uint8_t* base);
void CarListBaseHandleInput(lifted::Regs& ctx, uint8_t* base);
void CarListSetHandleInput(PPCContext& ctx, uint8_t* base);
void CarListSetHandleInput(lifted::Regs& ctx, uint8_t* base);
void CarListBackPage8(PPCContext& ctx, uint8_t* base);
void CarListBackPage8(lifted::Regs& ctx, uint8_t* base);
void CarListStep2(PPCContext& ctx, uint8_t* base);
void CarListStep2(lifted::Regs& ctx, uint8_t* base);
void CarListStep3(PPCContext& ctx, uint8_t* base);
void CarListStep3(lifted::Regs& ctx, uint8_t* base);
void CarListStep7(PPCContext& ctx, uint8_t* base);
void CarListStep7(lifted::Regs& ctx, uint8_t* base);
void CarListStep11(PPCContext& ctx, uint8_t* base);
void CarListStep11(lifted::Regs& ctx, uint8_t* base);
void CarListStep18(PPCContext& ctx, uint8_t* base);
void CarListStep18(lifted::Regs& ctx, uint8_t* base);
void CarListStep26(PPCContext& ctx, uint8_t* base);
void CarListStep26(lifted::Regs& ctx, uint8_t* base);
void CarListStep0(PPCContext& ctx, uint8_t* base);
void CarListStep0(lifted::Regs& ctx, uint8_t* base);
void CarListStep1(PPCContext& ctx, uint8_t* base);
void CarListStep1(lifted::Regs& ctx, uint8_t* base);
void CarListStep4(PPCContext& ctx, uint8_t* base);
void CarListStep4(lifted::Regs& ctx, uint8_t* base);
void CarListStep6(PPCContext& ctx, uint8_t* base);
void CarListStep6(lifted::Regs& ctx, uint8_t* base);
void CarListStep8(PPCContext& ctx, uint8_t* base);
void CarListStep8(lifted::Regs& ctx, uint8_t* base);
void CarListStep9(PPCContext& ctx, uint8_t* base);
void CarListStep9(lifted::Regs& ctx, uint8_t* base);
void CarListStep12(PPCContext& ctx, uint8_t* base);
void CarListStep12(lifted::Regs& ctx, uint8_t* base);
void CarListStep19(PPCContext& ctx, uint8_t* base);
void CarListStep19(lifted::Regs& ctx, uint8_t* base);
void CarListSetUpRows(PPCContext& ctx, uint8_t* base);
void CarListSetUpRows(lifted::Regs& ctx, uint8_t* base);
void CarListSetUpCarPage(PPCContext& ctx, uint8_t* base);
void CarListSetUpCarPage(lifted::Regs& ctx, uint8_t* base);
void CarListBaseBuild(PPCContext& ctx, uint8_t* base);
void CarListBaseBuild(lifted::Regs& ctx, uint8_t* base);
void CarListSetBuild(PPCContext& ctx, uint8_t* base);
void CarListSetBuild(lifted::Regs& ctx, uint8_t* base);
void CarListShowStandCar(PPCContext& ctx, uint8_t* base);
void CarListShowStandCar(lifted::Regs& ctx, uint8_t* base);
void CarListSetUpColors(PPCContext& ctx, uint8_t* base);
void CarListSetUpColors(lifted::Regs& ctx, uint8_t* base);
void CarListStep17(PPCContext& ctx, uint8_t* base);
void CarListStep17(lifted::Regs& ctx, uint8_t* base);
void CarListCarPage(PPCContext& ctx, uint8_t* base);
void CarListCarPage(lifted::Regs& ctx, uint8_t* base);
// PanelSet's update: GarageStages::DrawAll (no r3), PanelSetBase::
// UpdateStand and Update (r3 = b), PanelSet::Update (r3 the set).
void CarListDrawAll(PPCContext& ctx, uint8_t* base);
void CarListDrawAll(lifted::Regs& ctx, uint8_t* base);
void CarListUpdateStand(PPCContext& ctx, uint8_t* base);
void CarListUpdateStand(lifted::Regs& ctx, uint8_t* base);
void CarListBaseUpdate(PPCContext& ctx, uint8_t* base);
void CarListBaseUpdate(lifted::Regs& ctx, uint8_t* base);
void CarListSetUpdate(PPCContext& ctx, uint8_t* base);
void CarListSetUpdate(lifted::Regs& ctx, uint8_t* base);
void CarListTabsPage(PPCContext& ctx, uint8_t* base);
void CarListTabsPage(lifted::Regs& ctx, uint8_t* base);
void CarListRowsPage(PPCContext& ctx, uint8_t* base);
void CarListRowsPage(lifted::Regs& ctx, uint8_t* base);
void CarListColorsPage(PPCContext& ctx, uint8_t* base);
void CarListColorsPage(lifted::Regs& ctx, uint8_t* base);
void CarListPowerGraphPage(PPCContext& ctx, uint8_t* base);
void CarListPowerGraphPage(lifted::Regs& ctx, uint8_t* base);
// PanelSetPair (r3 the pair; the constructor's r4 to r10 and stack words,
// the deleting destructor's r4 the flags), the C panels' base (r3 the panel;
// SetCarName's r4 the car's id, r5 its kind), Panel6C::SetStats (r4 the
// record) and the confirm panel's constructor.
void CarListBaseSetCarName(PPCContext& ctx, uint8_t* base);
void CarListBaseSetCarName(lifted::Regs& ctx, uint8_t* base);
void CarListPanel6CSetStats(PPCContext& ctx, uint8_t* base);
void CarListPanel6CSetStats(lifted::Regs& ctx, uint8_t* base);
void CarListCarPanelBaseDestruct(PPCContext& ctx, uint8_t* base);
void CarListCarPanelBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListConfirmPanelConstruct(PPCContext& ctx, uint8_t* base);
void CarListConfirmPanelConstruct(lifted::Regs& ctx, uint8_t* base);
void CarListPairConstruct(PPCContext& ctx, uint8_t* base);
void CarListPairConstruct(lifted::Regs& ctx, uint8_t* base);
void CarListPairDestruct(PPCContext& ctx, uint8_t* base);
void CarListPairDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListPairDeletingDestruct(PPCContext& ctx, uint8_t* base);
void CarListPairDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListPairReset(PPCContext& ctx, uint8_t* base);
void CarListPairReset(lifted::Regs& ctx, uint8_t* base);
void CarListPairOpen(PPCContext& ctx, uint8_t* base);
void CarListPairOpen(lifted::Regs& ctx, uint8_t* base);
void CarListPairHandleInput(PPCContext& ctx, uint8_t* base);
void CarListPairHandleInput(lifted::Regs& ctx, uint8_t* base);
void CarListPairUpdate(PPCContext& ctx, uint8_t* base);
void CarListPairUpdate(lifted::Regs& ctx, uint8_t* base);
void CarListPairBuild(PPCContext& ctx, uint8_t* base);
void CarListPairBuild(lifted::Regs& ctx, uint8_t* base);
void CarListPairPicked(PPCContext& ctx, uint8_t* base);
void CarListPairPicked(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
