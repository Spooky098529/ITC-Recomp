// The car model's release as plain C++ (docs/functions/car-models.md,
// CarModel::Release), over the load slots (load_slots.h), the skeleton's
// plain C++ and CarModel::Reset's (scene_loaders/objects.h). It frees, so it
// runs natively only while hooks::ModelsNativeLifetime() (the adapter's
// condition). Adapter: src/hooks/car_models.cpp.

#pragma once

#include "archive/archive.h"

#include <cstdint>
#include <optional>

namespace car_models {

// CarModel::Release (sub_820D4508): LoadSlot::ReleaseBuffer on the node
// file's slot (+3728), Skeleton::FreeNodeValues (+3760), ReleaseBuffer on
// the two XPR2s' slots (+3952, +3976); the two vinyl sheets (+6684 to +6696,
// +6700 to +6712) put off (DeferRelease on [+7256]); then CarModel::Reset.
// Returns the original's r3 (CarModel::Reset's); nullopt, with nothing
// done, where a callee would go to the original (a buffer that isn't the
// layer's pages, node values the layer didn't give, CarModel::Reset's
// checks).
std::optional<uint32_t> CarModelRelease(uint8_t* base, uint32_t car_model);
// Whether CarModelRelease runs natively (its checks, nothing changed; the
// pending frees' room for CarModel::Reset's four nodes included).
bool CarModelReleasable(uint8_t* base, uint32_t car_model);

// The callbacks the sets' loads hand Archive::Load: CarModel::OnFileLoaded
// (set a) and OnFileLoadedB (set b).
constexpr uint32_t kOnFileLoaded = 0x820D47A8;
constexpr uint32_t kOnFileLoadedB = 0x820D4F80;

// CarModel::LoadSetA (sub_820D59D8): the node file 893 + 226 x block (+32)
// into the slot at +3728 (+0 the id, +8 the size, +16 pages of that size
// from sub_820C3CA0; when the size is above 0, +4 = Archive::Load(id, the
// pages, OnFileLoaded, the car model) after +20 = 2), then the body XPR2
// 891 + 226 x block into +3952 the same way, +20 = 1 when its size is 0.
// Returns the original's r3 (the last load's serial, sign-extended as
// Archive::Load's r3, or the last pages).
// Only while --native_heap routes VirtualAlloc to the layer.
int64_t CarModelLoadSetA(uint8_t* base, uint32_t car_model, const archive::LoadCall& load);

// CarModel::LoadSetB (sub_820D5AC0): LoadSlot::Load of the node file 11063
// + 197 x block into +3728 (OnFileLoadedB, the car model), then the body
// XPR2 11061 + 197 x block into the shared load buffer ([0x826C2E44] +
// 5056, read after the first load): +3952 the id, +3960 the size (not
// compared), +3956 = Archive::Load(id, the buffer, OnFileLoadedB, the car
// model), +3972 = 2. Returns the serial (r3, sign-extended). Only while --native_heap
// routes VirtualAlloc to the layer.
int64_t CarModelLoadSetB(uint8_t* base, uint32_t car_model, uint32_t pcr,
                          const archive::LoadCall& load);

// sub_820D3CF8 (the team emblem): for blocks 18 to 44 the emblem's asset id
// (168 to 188, by the original's table), -1 for the others.
int32_t TeamEmblem(uint32_t block);

// The callback the decals' loads hand Archive::Load: CarModel::OnDecalLoaded.
constexpr uint32_t kOnDecalLoaded = 0x820D5750;

// CarModel::LoadDecals (sub_820D5B50): the six vinyl layers (+208, 64 bytes
// apart; asset index + 163 for layers 0-2, + 189 for 3-5) through
// LoadSlot::LoadShared into +4092 + 48i, then the 48 stickers from 47 down
// to 0 (sticker j: group j / 6's records, 64 bytes apart, at +1360, +976,
// +1744, +2128, +3280, +2896, +2512, +592 for groups 0 to 7, record j % 6;
// index + 383, or the index itself when the record's +8 is 9) into +4380 +
// 48j, with OnDecalLoaded and the car model; a negative index leaves the
// slot's +20 = 1. +4088 = 2, or 3 when nothing was asked for. Returns 1.
uint32_t CarModelLoadDecals(uint8_t* base, uint32_t car_model, const archive::LoadCall& load);

// CarModel::LoadExterior (sub_820D5EC8): the 3,696-byte set-up copied to
// +32 (memcpy, native::Buffers_Memcpy), the vinyl sheet made (Texture::Create
// of A8R8G8B8 at the size in 0x82602570, into +6684, +6688; `stack` the
// guest r1 at entry, its frame 112 bytes below), for blocks 18 and up the
// team emblem in sticker group 7's first record (+592 the id, +600 = 9),
// set a (LoadSetA) or set b (LoadSetB) by [[+7256] + 8] (0, 1; neither
// above), then LoadDecals. Returns 1 (LoadDecals' r3); nullopt, with nothing
// done, where the copy can't be native (the original runs). Only while
// --native_heap routes the game's heap to the layer.
std::optional<uint32_t> CarModelLoadExterior(uint8_t* base, uint32_t car_model, uint32_t setup,
                                             uint32_t stack, uint32_t pcr,
                                             const archive::LoadCall& load);

// CarModel::ReloadDecals (sub_820D5D20): the old sheet's four words (+6684
// to +6696) put off for release (sub_820C3F50 on [+7256]), +4088 = 0, the
// three layer slots (+4092) and six sticker slots (+4380) reset
// (LoadSlot::Reset), a new sheet made (Texture::Create into +6684, +6688;
// `stack` the guest r1 at entry, its frame 320 bytes below), for blocks 18
// and up the team emblem in sticker group 7's first record, then
// LoadDecals. Returns 1 (LoadDecals' r3). Not where +3724 is 1 (an image
// drawn into the sheet, sub_820C4E68, never seen in play: the adapter runs
// the original). Only while --native_heap routes the game's heap to the
// layer.
uint32_t CarModelReloadDecals(uint8_t* base, uint32_t car_model, uint32_t stack,
                              const archive::LoadCall& load);

// CarModel::SetVinylTextures (sub_820D5F88): r3 = the scene model (its car
// model at +24): the six part models at +688, 464 bytes apart, given the car
// model's vinyl sheet (Model::SetVinylTextures with [+6684], [+6692], the
// car model reread each time). Returns the last part model (its r3).
uint32_t CarModelSetVinylTextures(uint8_t* base, uint32_t scene_model);

}  // namespace car_models
