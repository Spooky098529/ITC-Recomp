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

}  // namespace hooks
