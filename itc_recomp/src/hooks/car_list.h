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
void CarListSetCaption(lifted::Regs& ctx, uint8_t* base);
void CarListPartFinishAll(lifted::Regs& ctx, uint8_t* base);
void CarListTabSetCursorY(lifted::Regs& ctx, uint8_t* base);
void CarListTabSetButtons(lifted::Regs& ctx, uint8_t* base);
void CarListRowSetRow(lifted::Regs& ctx, uint8_t* base);
void CarListRowSetArrows(lifted::Regs& ctx, uint8_t* base);
void CarListRowUnhighlight(lifted::Regs& ctx, uint8_t* base);
void CarListRowHide(lifted::Regs& ctx, uint8_t* base);
void CarListColorShow(lifted::Regs& ctx, uint8_t* base);
void CarListColorSelect(lifted::Regs& ctx, uint8_t* base);
void CarListColorSetArrows(lifted::Regs& ctx, uint8_t* base);
void CarListColorShowSwatches(lifted::Regs& ctx, uint8_t* base);
void CarListColorPlaceCursor(lifted::Regs& ctx, uint8_t* base);
void CarListColorSetCount(lifted::Regs& ctx, uint8_t* base);
void CarListColorSetSwatch(lifted::Regs& ctx, uint8_t* base);
void CarListColorSetSwatches(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetTexts(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetButtons(lifted::Regs& ctx, uint8_t* base);
void CarListCarPlaceCursor(lifted::Regs& ctx, uint8_t* base);
void CarListCarSelect(lifted::Regs& ctx, uint8_t* base);
void CarListCarStopArrows(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetName(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetStats(lifted::Regs& ctx, uint8_t* base);
void CarListCarSetCarName(lifted::Regs& ctx, uint8_t* base);
void CarListPanel7CShowArrows(lifted::Regs& ctx, uint8_t* base);
void CarListCarShowStats(lifted::Regs& ctx, uint8_t* base);
void CarListTabSetTab(lifted::Regs& ctx, uint8_t* base);
void CarListTabMove(lifted::Regs& ctx, uint8_t* base);
void CarListRowFill(lifted::Regs& ctx, uint8_t* base);
void CarListRowRefresh(lifted::Regs& ctx, uint8_t* base);
void CarListRowMove(lifted::Regs& ctx, uint8_t* base);
void CarListColorMove(lifted::Regs& ctx, uint8_t* base);
// The panels' answers (r3 the panel).
void CarListTabIsShown(lifted::Regs& ctx, uint8_t* base);
void CarListRowIsShown(lifted::Regs& ctx, uint8_t* base);
void CarListRowIsHidden(lifted::Regs& ctx, uint8_t* base);
void CarListCarIsShown(lifted::Regs& ctx, uint8_t* base);
void CarListColorIsShown(lifted::Regs& ctx, uint8_t* base);
void CarListColorIsIdle(lifted::Regs& ctx, uint8_t* base);
// The panels' shows, selections and moves.
void CarListTabShow(lifted::Regs& ctx, uint8_t* base);
void CarListTabSelect(lifted::Regs& ctx, uint8_t* base);
void CarListRowShow(lifted::Regs& ctx, uint8_t* base);
void CarListRowSelect(lifted::Regs& ctx, uint8_t* base);
void CarListCarShow(lifted::Regs& ctx, uint8_t* base);
void CarListCarMove(lifted::Regs& ctx, uint8_t* base);
void CarListCarPick(lifted::Regs& ctx, uint8_t* base);
// PowerGraph::SetEntry (r3 an entry, r4 the tab).
void CarListPowerGraphSetEntry(lifted::Regs& ctx, uint8_t* base);

// PanelSet's leaves (r3 the set or its base, r4 a value).
void CarListSetReset(lifted::Regs& ctx, uint8_t* base);
void CarListSetPicked(lifted::Regs& ctx, uint8_t* base);
void CarListSetWentBack(lifted::Regs& ctx, uint8_t* base);
void CarListSetWantsPowerGraph(lifted::Regs& ctx, uint8_t* base);
void CarListSetOpenKind(lifted::Regs& ctx, uint8_t* base);
void CarListSetPreviousTab(lifted::Regs& ctx, uint8_t* base);
void CarListSetPreviousEntry(lifted::Regs& ctx, uint8_t* base);
void CarListSetPreviousColor(lifted::Regs& ctx, uint8_t* base);
void CarListPickedPage(lifted::Regs& ctx, uint8_t* base);
void CarListSaveData(lifted::Regs& ctx, uint8_t* base);

// PanelSet's pages and steps (r3 the set or its base b, a step's r4 its
// first-pass word).
void CarListToTabs(lifted::Regs& ctx, uint8_t* base);
void CarListOpen(lifted::Regs& ctx, uint8_t* base);
void CarListBaseFree(lifted::Regs& ctx, uint8_t* base);
void CarListSetFree(lifted::Regs& ctx, uint8_t* base);
// The destructors (r3 the object; r4 the flags for the deleting one).
void CarListGroupBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListSetBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListSetDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListSetInterfaceDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListBaseHandleInput(lifted::Regs& ctx, uint8_t* base);
void CarListSetHandleInput(lifted::Regs& ctx, uint8_t* base);
void CarListBackPage8(lifted::Regs& ctx, uint8_t* base);
void CarListStep2(lifted::Regs& ctx, uint8_t* base);
void CarListStep3(lifted::Regs& ctx, uint8_t* base);
void CarListStep7(lifted::Regs& ctx, uint8_t* base);
void CarListStep11(lifted::Regs& ctx, uint8_t* base);
void CarListStep18(lifted::Regs& ctx, uint8_t* base);
void CarListStep26(lifted::Regs& ctx, uint8_t* base);
void CarListStep0(lifted::Regs& ctx, uint8_t* base);
void CarListStep1(lifted::Regs& ctx, uint8_t* base);
void CarListStep4(lifted::Regs& ctx, uint8_t* base);
void CarListStep6(lifted::Regs& ctx, uint8_t* base);
void CarListStep8(lifted::Regs& ctx, uint8_t* base);
void CarListStep9(lifted::Regs& ctx, uint8_t* base);
void CarListStep12(lifted::Regs& ctx, uint8_t* base);
void CarListStep19(lifted::Regs& ctx, uint8_t* base);
void CarListSetUpRows(lifted::Regs& ctx, uint8_t* base);
void CarListSetUpCarPage(lifted::Regs& ctx, uint8_t* base);
void CarListBaseBuild(lifted::Regs& ctx, uint8_t* base);
void CarListSetBuild(lifted::Regs& ctx, uint8_t* base);
void CarListShowStandCar(lifted::Regs& ctx, uint8_t* base);
void CarListSetUpColors(lifted::Regs& ctx, uint8_t* base);
void CarListStep17(lifted::Regs& ctx, uint8_t* base);
void CarListCarPage(lifted::Regs& ctx, uint8_t* base);
// PanelSet's update: GarageStages::DrawAll (no r3), PanelSetBase::
// UpdateStand and Update (r3 = b), PanelSet::Update (r3 the set).
void CarListDrawAll(lifted::Regs& ctx, uint8_t* base);
void CarListUpdateStand(lifted::Regs& ctx, uint8_t* base);
void CarListBaseUpdate(lifted::Regs& ctx, uint8_t* base);
void CarListSetUpdate(lifted::Regs& ctx, uint8_t* base);
void CarListTabsPage(lifted::Regs& ctx, uint8_t* base);
void CarListRowsPage(lifted::Regs& ctx, uint8_t* base);
void CarListColorsPage(lifted::Regs& ctx, uint8_t* base);
void CarListPowerGraphPage(lifted::Regs& ctx, uint8_t* base);
// PanelSetPair (r3 the pair; the constructor's r4 to r10 and stack words,
// the deleting destructor's r4 the flags), the C panels' base (r3 the panel;
// SetCarName's r4 the car's id, r5 its kind), Panel6C::SetStats (r4 the
// record) and the confirm panel's constructor.
void CarListBaseSetCarName(lifted::Regs& ctx, uint8_t* base);
void CarListPanel6CSetStats(lifted::Regs& ctx, uint8_t* base);
void CarListCarPanelBaseDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListConfirmPanelConstruct(lifted::Regs& ctx, uint8_t* base);
void CarListPairConstruct(lifted::Regs& ctx, uint8_t* base);
void CarListPairDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListPairDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void CarListPairReset(lifted::Regs& ctx, uint8_t* base);
void CarListPairOpen(lifted::Regs& ctx, uint8_t* base);
void CarListPairHandleInput(lifted::Regs& ctx, uint8_t* base);
void CarListPairUpdate(lifted::Regs& ctx, uint8_t* base);
void CarListPairBuild(lifted::Regs& ctx, uint8_t* base);
void CarListPairPicked(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
