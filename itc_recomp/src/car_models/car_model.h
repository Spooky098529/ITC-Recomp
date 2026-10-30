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

}  // namespace car_models
