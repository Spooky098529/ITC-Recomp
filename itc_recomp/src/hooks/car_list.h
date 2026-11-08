// The car list's register steps (src/car_list/, docs/functions/car-list.md),
// shared by the adapters (src/hooks/car_list.cpp, behind --native_car_list)
// and the harness (without the flag). Each takes the registers its original
// takes and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The panel groups and panels (r3 the group or panel, r4 to r7 their
// arguments).
void CarListSetCaption(PPCContext& ctx, uint8_t* base);
void CarListPartFinishAll(PPCContext& ctx, uint8_t* base);
void CarListTabSetCursorY(PPCContext& ctx, uint8_t* base);
void CarListTabSetButtons(PPCContext& ctx, uint8_t* base);
void CarListRowSetRow(PPCContext& ctx, uint8_t* base);
void CarListRowSetArrows(PPCContext& ctx, uint8_t* base);
void CarListRowUnhighlight(PPCContext& ctx, uint8_t* base);
void CarListRowHide(PPCContext& ctx, uint8_t* base);
void CarListColorShow(PPCContext& ctx, uint8_t* base);
void CarListColorSelect(PPCContext& ctx, uint8_t* base);
void CarListColorSetArrows(PPCContext& ctx, uint8_t* base);
void CarListColorShowSwatches(PPCContext& ctx, uint8_t* base);
void CarListColorPlaceCursor(PPCContext& ctx, uint8_t* base);
void CarListColorSetCount(PPCContext& ctx, uint8_t* base);
void CarListColorSetSwatch(PPCContext& ctx, uint8_t* base);
void CarListColorSetSwatches(PPCContext& ctx, uint8_t* base);
void CarListCarSetTexts(PPCContext& ctx, uint8_t* base);
void CarListCarSetButtons(PPCContext& ctx, uint8_t* base);
void CarListCarPlaceCursor(PPCContext& ctx, uint8_t* base);
void CarListCarSelect(PPCContext& ctx, uint8_t* base);
void CarListCarStopArrows(PPCContext& ctx, uint8_t* base);
void CarListCarSetName(PPCContext& ctx, uint8_t* base);
void CarListCarSetStats(PPCContext& ctx, uint8_t* base);
void CarListCarSetCarName(PPCContext& ctx, uint8_t* base);
void CarListPanel7CShowArrows(PPCContext& ctx, uint8_t* base);
void CarListCarShowStats(PPCContext& ctx, uint8_t* base);
void CarListTabSetTab(PPCContext& ctx, uint8_t* base);
void CarListTabMove(PPCContext& ctx, uint8_t* base);
void CarListRowFill(PPCContext& ctx, uint8_t* base);
void CarListRowRefresh(PPCContext& ctx, uint8_t* base);
void CarListRowMove(PPCContext& ctx, uint8_t* base);
void CarListColorMove(PPCContext& ctx, uint8_t* base);
// PowerGraph::SetEntry (r3 an entry, r4 the tab).
void CarListPowerGraphSetEntry(PPCContext& ctx, uint8_t* base);

// PanelSet's leaves (r3 the set or its base, r4 a value).
void CarListSetReset(PPCContext& ctx, uint8_t* base);
void CarListSetPicked(PPCContext& ctx, uint8_t* base);
void CarListSetWentBack(PPCContext& ctx, uint8_t* base);
void CarListSetWantsPowerGraph(PPCContext& ctx, uint8_t* base);
void CarListSetOpenKind(PPCContext& ctx, uint8_t* base);
void CarListSetPreviousTab(PPCContext& ctx, uint8_t* base);
void CarListSetPreviousEntry(PPCContext& ctx, uint8_t* base);
void CarListSetPreviousColor(PPCContext& ctx, uint8_t* base);
void CarListPickedPage(PPCContext& ctx, uint8_t* base);
void CarListSaveData(PPCContext& ctx, uint8_t* base);

// PanelSet's pages and steps (r3 the set or its base b, a step's r4 its
// first-pass word).
void CarListToTabs(PPCContext& ctx, uint8_t* base);
void CarListOpen(PPCContext& ctx, uint8_t* base);
void CarListBaseFree(PPCContext& ctx, uint8_t* base);
void CarListSetFree(PPCContext& ctx, uint8_t* base);
void CarListBaseHandleInput(PPCContext& ctx, uint8_t* base);
void CarListSetHandleInput(PPCContext& ctx, uint8_t* base);
void CarListBackPage8(PPCContext& ctx, uint8_t* base);
void CarListStep2(PPCContext& ctx, uint8_t* base);
void CarListStep3(PPCContext& ctx, uint8_t* base);
void CarListStep7(PPCContext& ctx, uint8_t* base);
void CarListStep11(PPCContext& ctx, uint8_t* base);
void CarListStep18(PPCContext& ctx, uint8_t* base);
void CarListStep26(PPCContext& ctx, uint8_t* base);
void CarListSetUpRows(PPCContext& ctx, uint8_t* base);
void CarListSetUpCarPage(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
