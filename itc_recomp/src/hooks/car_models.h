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
// LoadSlot::LoadShared sub_820D0860 and LoadSlot::Load sub_820D07C8: r3 =
// the slot, r4 = the asset id, r5 = the callback's context, r6 = the
// callback; r3 1 when loading, else 0. LoadSlot::Load natively only while
// --native_heap routes VirtualAlloc to the layer.
void CarModelsLoadSlotLoadShared(PPCContext& ctx, uint8_t* base);
void CarModelsLoadSlotLoad(PPCContext& ctx, uint8_t* base);
// The team emblem sub_820D3CF8: r4 = the block; r3 the asset id or -1 (r11,
// r0, r12 and ctr as the original's jump table leaves them).
void CarModelsTeamEmblem(PPCContext& ctx, uint8_t* base);
// CarModel::LoadDecals sub_820D5B50: r3 = the car model; r3 = 1.
void CarModelsLoadDecals(PPCContext& ctx, uint8_t* base);
// CarModel::LoadExterior sub_820D5EC8: r3 = the car model, r4 = its set-up;
// r3 = 1. Natively only while --native_heap routes the game's heap to the
// layer, and where memcpy's copy is native.
void CarModelsLoadExterior(PPCContext& ctx, uint8_t* base);
// CarModel::SetVinylTextures sub_820D5F88: r3 = the scene model; r3 the last
// part model.
void CarModelsSetVinylTextures(PPCContext& ctx, uint8_t* base);
// The wheels' set-up sub_820D9110: r3 = the wheels, r4 = the set-up; r3 the
// last LoadSlot::LoadShared's (the original where memcpy's copy can't be
// native).
void CarModelsWheelsSetUp(PPCContext& ctx, uint8_t* base);
// CarInterior::LoadSetCBody sub_820D67E8: r3 = the object, r4 = its set-up;
// r3 the last LoadShared's. Natively only while --native_heap routes
// VirtualAlloc to the layer, and where memcpy's copy is native.
void CarModelsCockpitLoadSetC(PPCContext& ctx, uint8_t* base);
// CarInterior::Load sub_820D79B0: r3 = the object, r4 = its set-up; r3 the
// last accessories' slot +4 call's (memcpy's for other sets). The slot +4
// calls are guest calls, from a 112-byte frame as the original's; the
// original where memcpy's copy can't be native.
void CarModelsInteriorLoad(PPCContext& ctx, uint8_t* base);
// CarModel::MakePlate sub_820ED6D0: r3 to r6 where the textures go, r7 to
// r10 and the caller's stack the 48-byte struct; r3 as the original leaves
// it. Resource::Release a guest call from a 944-byte frame as the
// original's. Natively only while --native_heap routes the allocations to
// the layer and memcpy is native.
void CarModelsMakePlate(PPCContext& ctx, uint8_t* base);
// CarModel::OnFileLoaded sub_820D47A8 and OnFileLoadedB sub_820D4F80:
// r3 = the request's serial, r4 = its buffer, r6 = the car model (r3 left
// as it came: the archive's finish doesn't read it). Their guest calls from
// a 1,552-byte frame as the original's; the original for a request
// FileLoadedNative refuses, or without the allocation layer, --native_models
// or a native memcpy.
void CarModelsOnFileLoaded(PPCContext& ctx, uint8_t* base);
void CarModelsOnFileLoadedB(PPCContext& ctx, uint8_t* base);
// sub_820C3F50: r3 = the set (left), r4 to r7 the record's words.
void CarModelsDeferRelease(PPCContext& ctx, uint8_t* base);

// CarModel::Release sub_820D4508: r3 = the car model; r3 as the original
// leaves it. Natively only while hooks::ModelsNativeLifetime().
void CarModelsCarModelRelease(PPCContext& ctx, uint8_t* base);

// Texture::Create sub_820ECF08: r3 = the format, r4, r5 = the size, r6, r7
// = where the header and data go; r3 = 1. Texture::Free sub_820ECFC8: r3,
// r4 = where the header and data are; r3 as the original leaves it.
// Natively only while --native_heap routes the game's heap to the layer,
// and Texture::Free with the header 0 or in its range.
void CarModelsTextureCreate(PPCContext& ctx, uint8_t* base);
void CarModelsTextureFree(PPCContext& ctx, uint8_t* base);

// The decals' TGA header's clear sub_820EEBC0: r3 = the header (left), r11
// = 0. TGA::CreateImage sub_820ED020: r3 = the header out, r4 = the size,
// r5 = where the file's buffer is, r6 = where the pixels go; r3 = 1.
// Natively only while --native_heap routes the game's heap to the layer,
// and for the files car_models::TgaDecodable takes.
void CarModelsTgaHeaderClear(PPCContext& ctx, uint8_t* base);
void CarModelsTgaCreateImage(PPCContext& ctx, uint8_t* base);

// Vinyl::DrawDecal sub_820EE638: r3 = the region, r4 = mirrored, r5 to r7
// the TGA header, r8, r9 the record's +16 to +31, f1 the rotation, the rest
// in the caller's frame; r3 as the original leaves it. Natively for regions
// 0 to 9 (car_models::DecalRegionPorted).
void CarModelsDrawDecal(PPCContext& ctx, uint8_t* base);

// Vinyl::Compress sub_820ED090's register steps from a frame below ctx.r1
// (its entry stack), for its native caller (CarModel::OnDecalLoaded's
// adapter) and the harness: the size, where the sheet's header and data are
// (replaced); the sheet at `header_in` must be car_models::TextureFreeable.
// Returns its r3.
uint32_t CarModelsVinylCompressAt(PPCContext& ctx, uint8_t* base, uint32_t width,
                                  uint32_t height, uint32_t header_in, uint32_t data_in);

// CarModel::OnDecalLoaded sub_820D5750 (Archive::Load's callback): r3 = the
// request, r6 = the car model; r3 as the original leaves it. Natively only
// while --native_heap routes the game's heap to the layer, and where
// car_models::DecalsNative agrees.
void CarModelsOnDecalLoaded(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
