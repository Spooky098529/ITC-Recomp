// The car model's release as plain C++ (docs/functions/car-models.md,
// CarModel::Release), over the load slots (load_slots.h), the skeleton's
// plain C++ and CarModel::Reset's (scene_loaders/objects.h). It frees, so it
// runs natively only while hooks::ModelsNativeLifetime() (the adapter's
// condition). Adapter: src/hooks/car_models.cpp.

#pragma once

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

}  // namespace car_models
