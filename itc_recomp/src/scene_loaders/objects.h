// The scene's loading objects' resets as plain C++ (docs/functions/
// scene-loaders.md; the fields in docs/known-data.md), store for store as
// the recompiled functions, over the members' plain C++ (car_models/,
// scene_models/, scene_loaders/loads.h). The ones that free run natively
// only while hooks::ModelsNativeLifetime() (the adapters' condition: the
// allocation layer and the memory blocks' host lock). The originals' stack
// temporaries (the matrices they build on their frames before memcpy) and
// saved registers aren't written. Adapters: src/hooks/scene_loaders.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace scene_loaders {

// The car model (7,264 bytes, pool 0): +32 the car record, +3728 the loads
// (loads.h), +7256 the first pool set.
constexpr uint32_t kCarModelBytes = 7264;
constexpr uint32_t kCarModelRecord = 32;
constexpr uint32_t kCarModelLoads = 3728;
constexpr uint32_t kCarModelSet = 7256;

// Whether CarModelReset runs natively (nothing changed): the car model below
// the physical views (its matrices are memcpy's copies there, which memcpy's
// override records for the renderer), and CarModelLoadsResettable.
bool CarModelResettable(uint8_t* base, uint32_t car_model);

// CarModel::Reset (sub_820D3E98): +7256 = [0x826C2E40]; CarRecord::Reset
// (+32); CarModelLoads::Reset (+3728); the four tire matrices (+6720 + 64i)
// the identity, then their positions (+6976 + 16i) (0, 0, 0, 1); the two
// seats' matrices (+7040 + 64i) and positions (+7168 + 16i) the same, one
// seat after the other; +7232 to +7252 = -1; +7200 = 0, +7216 = (0, 0, 0, 1).
// The 0.0 and 1.0 are the words at 0x82000A34 and 0x82000A30, as the
// original loads them. Returns the original's r3 (the last memcpy's
// destination, +7104); nullopt, with nothing done, where CarModelResettable
// says no.
std::optional<uint32_t> CarModelReset(uint8_t* base, uint32_t car_model);

}  // namespace scene_loaders
