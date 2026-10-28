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

// Texture::Create sub_820ECF08: r3 = the format, r4, r5 = the size, r6, r7
// = where the header and data go; r3 = 1. Texture::Free sub_820ECFC8: r3,
// r4 = where the header and data are; r3 as the original leaves it.
// Vinyl::Compress sub_820ED090: r3, r4 = the size, r5, r6 = where the
// sheet's header and data are (replaced); r3 as the original leaves it.
// Natively only while --native_heap routes the game's heap to the layer,
// and Texture::Free and Vinyl::Compress with the header 0 or in its range.
void CarModelsTextureCreate(PPCContext& ctx, uint8_t* base);
void CarModelsTextureFree(PPCContext& ctx, uint8_t* base);
void CarModelsVinylCompress(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
