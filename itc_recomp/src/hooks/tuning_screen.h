// The tuning screen's register steps (src/tuning_screen/,
// docs/functions/tuning-screen.md), shared by the adapters
// (src/hooks/tuning_screen.cpp, behind --native_tuning_screen) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// The part tables: r3 = the screen. TuningScreen::CategoryItemCount
// sub_821CC6C0, IsItemAvailable sub_821CDB18 (r4 = an item),
// WheelNumber sub_821CDED8 (r4 = a brand, r5 = a model), GetPrice
// sub_821CD160, GetCurrentChoice sub_821CD868, GetChoiceFlag sub_821CDC38
// (r4 = a choice).
void TuningCategoryItemCount(PPCContext& ctx, uint8_t* base);
void TuningIsItemAvailable(PPCContext& ctx, uint8_t* base);
void TuningWheelNumber(PPCContext& ctx, uint8_t* base);
void TuningGetPrice(PPCContext& ctx, uint8_t* base);
void TuningGetCurrentChoice(PPCContext& ctx, uint8_t* base);
void TuningGetChoiceFlag(PPCContext& ctx, uint8_t* base);

// The debug state handlers (r3 = the screen): DebugMainMenu sub_821CBE08,
// DebugLevelSelect sub_821CBEE8, DebugTypeSelect sub_821CBFC0,
// DebugOptionSelect sub_821CC248, DebugConfirmBuy sub_821CC3A8,
// DebugConfirmEquip sub_821CC488, DebugCategoryMenu sub_821CFCE8,
// DebugWheelPartSelect sub_821E8068, DebugWheelKindSelect sub_821E8128,
// DebugWheelInchSelect sub_821E8298, DebugDecalTopMenu sub_821FD710,
// DebugLayerSelect sub_821FD880, DebugLayerEdit sub_821FDCB8, DebugLayerSet
// sub_821FE080, DebugEditPosition sub_821FE100, DebugEditView sub_821FE178,
// DebugPaintConfirm sub_821FE190.
void TuningDebugMainMenu(PPCContext& ctx, uint8_t* base);
void TuningDebugLevelSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugTypeSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugOptionSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugConfirmBuy(PPCContext& ctx, uint8_t* base);
void TuningDebugConfirmEquip(PPCContext& ctx, uint8_t* base);
void TuningDebugCategoryMenu(PPCContext& ctx, uint8_t* base);
void TuningDebugWheelPartSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugWheelKindSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugWheelInchSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugDecalTopMenu(PPCContext& ctx, uint8_t* base);
void TuningDebugLayerSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugLayerEdit(PPCContext& ctx, uint8_t* base);
void TuningDebugLayerSet(PPCContext& ctx, uint8_t* base);
void TuningDebugEditPosition(PPCContext& ctx, uint8_t* base);
void TuningDebugEditView(PPCContext& ctx, uint8_t* base);
void TuningDebugPaintConfirm(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
