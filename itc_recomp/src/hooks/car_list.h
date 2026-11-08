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

}  // namespace hooks
