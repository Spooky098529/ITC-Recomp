// The car settings screen's register steps (src/car_settings/,
// docs/functions/car-settings.md), shared by the adapters
// (src/hooks/car_settings.cpp, behind --native_car_settings) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The machine's handlers (r3 the machine, r4 the mode where read).
void CarSettingsCreate(PPCContext& ctx, uint8_t* base);
void CarSettingsOpenMovie(PPCContext& ctx, uint8_t* base);
void CarSettingsWaitForMovie(PPCContext& ctx, uint8_t* base);
void CarSettingsClose(PPCContext& ctx, uint8_t* base);
void CarSettingsDestroy(PPCContext& ctx, uint8_t* base);
// The set-ups (r3 the machine).
void CarSettingsInit(PPCContext& ctx, uint8_t* base);
void CarSettingsInitType5(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
