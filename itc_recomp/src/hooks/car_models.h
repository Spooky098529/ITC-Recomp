// The car models' register steps (src/car_models/,
// docs/functions/car-models.md), shared by the adapters
// (src/hooks/car_models.cpp, behind --native_car_models) and the harness
// (without the flag). Each leaves r3 as the original does and runs the
// original where the plain C++ refuses.

#pragma once

#include "guest.h"

namespace hooks {

// LoadSlot::Reset sub_820D0730: r3 = the slot (left).
void CarModelsLoadSlotReset(PPCContext& ctx, uint8_t* base);
// LoadSlot::ReleaseBuffer sub_820D0758: r3 = the slot; r3 as the original
// leaves it. Natively only while --native_heap routes VirtualFree to the
// allocation layer.
void CarModelsLoadSlotReleaseBuffer(PPCContext& ctx, uint8_t* base);
// sub_820C3F50: r3 = the set (left), r4 to r7 the record's words.
void CarModelsDeferRelease(PPCContext& ctx, uint8_t* base);

// CarModel::Release sub_820D4508: r3 = the car model; r3 as the original
// leaves it. Natively only while hooks::ModelsNativeLifetime().
void CarModelsCarModelRelease(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
