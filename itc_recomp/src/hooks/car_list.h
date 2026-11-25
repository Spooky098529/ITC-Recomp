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
void CarListSetCaption(lifted::Regs& ctx);
void CarListPartFinishAll(lifted::Regs& ctx);
void CarListTabSetCursorY(lifted::Regs& ctx);
void CarListTabSetButtons(lifted::Regs& ctx);
void CarListRowSetRow(lifted::Regs& ctx);
void CarListRowSetArrows(lifted::Regs& ctx);
void CarListRowUnhighlight(lifted::Regs& ctx);
void CarListRowHide(lifted::Regs& ctx);
void CarListColorShow(lifted::Regs& ctx);
void CarListColorSelect(lifted::Regs& ctx);
void CarListColorSetArrows(lifted::Regs& ctx);
void CarListColorShowSwatches(lifted::Regs& ctx);
void CarListColorPlaceCursor(lifted::Regs& ctx);
void CarListColorSetCount(lifted::Regs& ctx);
void CarListColorSetSwatch(lifted::Regs& ctx);
void CarListColorSetSwatches(lifted::Regs& ctx);
void CarListCarSetTexts(lifted::Regs& ctx);
void CarListCarSetButtons(lifted::Regs& ctx);
void CarListCarPlaceCursor(lifted::Regs& ctx);
void CarListCarSelect(lifted::Regs& ctx);
void CarListCarStopArrows(lifted::Regs& ctx);
void CarListCarSetName(lifted::Regs& ctx);
void CarListCarSetStats(lifted::Regs& ctx);
void CarListCarSetCarName(lifted::Regs& ctx);
void CarListPanel7CShowArrows(lifted::Regs& ctx);
void CarListCarShowStats(lifted::Regs& ctx);
void CarListTabSetTab(lifted::Regs& ctx);
void CarListTabMove(lifted::Regs& ctx);
void CarListRowFill(lifted::Regs& ctx);
void CarListRowRefresh(lifted::Regs& ctx);
void CarListRowMove(lifted::Regs& ctx);
void CarListColorMove(lifted::Regs& ctx);
// The panels' answers (r3 the panel).
void CarListTabIsShown(lifted::Regs& ctx);
void CarListRowIsShown(lifted::Regs& ctx);
void CarListRowIsHidden(lifted::Regs& ctx);
void CarListCarIsShown(lifted::Regs& ctx);
void CarListColorIsShown(lifted::Regs& ctx);
void CarListColorIsIdle(lifted::Regs& ctx);
// The panels' shows, selections and moves.
void CarListTabShow(lifted::Regs& ctx);
void CarListTabSelect(lifted::Regs& ctx);
void CarListRowShow(lifted::Regs& ctx);
void CarListRowSelect(lifted::Regs& ctx);
void CarListCarShow(lifted::Regs& ctx);
void CarListCarMove(lifted::Regs& ctx);
void CarListCarPick(lifted::Regs& ctx);
// PowerGraph::SetEntry (r3 an entry, r4 the tab).
void CarListPowerGraphSetEntry(lifted::Regs& ctx);

// PanelSet's leaves (r3 the set or its base, r4 a value).
void CarListSetReset(lifted::Regs& ctx);
void CarListSetPicked(lifted::Regs& ctx);
void CarListSetWentBack(lifted::Regs& ctx);
void CarListSetWantsPowerGraph(lifted::Regs& ctx);
void CarListSetOpenKind(lifted::Regs& ctx);
void CarListSetPreviousTab(lifted::Regs& ctx);
void CarListSetPreviousEntry(lifted::Regs& ctx);
void CarListSetPreviousColor(lifted::Regs& ctx);
void CarListPickedPage(lifted::Regs& ctx);
void CarListSaveData(lifted::Regs& ctx);

// PanelSet's pages and steps (r3 the set or its base b, a step's r4 its
// first-pass word).
void CarListToTabs(lifted::Regs& ctx);
void CarListOpen(lifted::Regs& ctx);
void CarListBaseFree(lifted::Regs& ctx);
void CarListSetFree(lifted::Regs& ctx);
// The destructors (r3 the object; r4 the flags for the deleting one).
void CarListGroupBaseDestruct(lifted::Regs& ctx);
void CarListSetBaseDestruct(lifted::Regs& ctx);
void CarListSetDestruct(lifted::Regs& ctx);
void CarListSetInterfaceDeletingDestruct(lifted::Regs& ctx);
void CarListBaseHandleInput(lifted::Regs& ctx);
void CarListSetHandleInput(lifted::Regs& ctx);
void CarListBackPage8(lifted::Regs& ctx);
void CarListStep2(lifted::Regs& ctx);
void CarListStep3(lifted::Regs& ctx);
void CarListStep7(lifted::Regs& ctx);
void CarListStep11(lifted::Regs& ctx);
void CarListStep18(lifted::Regs& ctx);
void CarListStep26(lifted::Regs& ctx);
void CarListStep0(lifted::Regs& ctx);
void CarListStep1(lifted::Regs& ctx);
void CarListStep4(lifted::Regs& ctx);
void CarListStep6(lifted::Regs& ctx);
void CarListStep8(lifted::Regs& ctx);
void CarListStep9(lifted::Regs& ctx);
void CarListStep12(lifted::Regs& ctx);
void CarListStep19(lifted::Regs& ctx);
void CarListSetUpRows(lifted::Regs& ctx);
void CarListSetUpCarPage(lifted::Regs& ctx);
void CarListBaseBuild(lifted::Regs& ctx);
void CarListSetBuild(lifted::Regs& ctx);
void CarListShowStandCar(lifted::Regs& ctx);
void CarListSetUpColors(lifted::Regs& ctx);
void CarListStep17(lifted::Regs& ctx);
void CarListCarPage(lifted::Regs& ctx);
// PanelSet's update: GarageStages::DrawAll (no r3), PanelSetBase::
// UpdateStand and Update (r3 = b), PanelSet::Update (r3 the set).
void CarListDrawAll(lifted::Regs& ctx);
void CarListUpdateStand(lifted::Regs& ctx);
void CarListBaseUpdate(lifted::Regs& ctx);
void CarListSetUpdate(lifted::Regs& ctx);
void CarListTabsPage(lifted::Regs& ctx);
void CarListRowsPage(lifted::Regs& ctx);
void CarListColorsPage(lifted::Regs& ctx);
void CarListPowerGraphPage(lifted::Regs& ctx);
// PanelSetPair (r3 the pair; the constructor's r4 to r10 and stack words,
// the deleting destructor's r4 the flags), the C panels' base (r3 the panel;
// SetCarName's r4 the car's id, r5 its kind), Panel6C::SetStats (r4 the
// record) and the confirm panel's constructor.
void CarListBaseSetCarName(lifted::Regs& ctx);
void CarListPanel6CSetStats(lifted::Regs& ctx);
void CarListCarPanelBaseDestruct(lifted::Regs& ctx);
void CarListConfirmPanelConstruct(lifted::Regs& ctx);
void CarListPairConstruct(lifted::Regs& ctx);
void CarListPairDestruct(lifted::Regs& ctx);
void CarListPairDeletingDestruct(lifted::Regs& ctx);
void CarListPairReset(lifted::Regs& ctx);
void CarListPairOpen(lifted::Regs& ctx);
void CarListPairHandleInput(lifted::Regs& ctx);
void CarListPairUpdate(lifted::Regs& ctx);
void CarListPairBuild(lifted::Regs& ctx);
void CarListPairPicked(lifted::Regs& ctx);

}  // namespace hooks
