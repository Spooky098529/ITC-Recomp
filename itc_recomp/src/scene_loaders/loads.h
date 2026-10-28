// The loading objects' embedded loads as plain C++ (docs/functions/
// scene-loaders.md): the members each class keeps (a skeleton, XPRs, texture
// providers), made and destroyed through the members' own plain C++
// (model_animation/model_animation.h, models/xpr.h,
// models/texture_provider.h). The providers allocate and free, and the
// XPRs put their blocks off under the memory blocks' host lock, so these run
// natively only while hooks::ModelsNativeLifetime() (the adapters'
// condition). Each checks first what its members would leave to the
// original, and returns false with nothing changed then. Adapters:
// src/hooks/scene_loaders.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace scene_loaders {

// CarModelLoads (the car model's +3728): +32 the skeleton, +272 and +296 the
// XPRs, +320 and +340 the texture providers.
constexpr uint32_t kCarModelLoadsSkeleton = 32;
constexpr uint32_t kCarModelLoadsXprs[] = {272, 296};
constexpr uint32_t kCarModelLoadsProviders[] = {320, 340};

// CarModelLoads::CarModelLoads (sub_820D3DE0): Skeleton::Skeleton (+32),
// XPR::XPR (+272, +296), TextureProvider::TextureProvider (+320, +340, 64
// slots each). False, with nothing done, where a provider's allocation
// would go to the original.
bool CarModelLoadsConstruct(uint8_t* base, uint32_t loads);

// CarModelLoads::~CarModelLoads (sub_820D3E40): the providers' destructors
// (+340, then +320), XPR::~XPR (+296, +272), Skeleton::~Skeleton (+32).
// False, with nothing done, where a member's destructor would go to the
// original (a provider's storage or the skeleton's arrays not from the
// allocation layer, no room in the pending frees for the XPRs' blocks).
bool CarModelLoadsDestruct(uint8_t* base, uint32_t loads);

// The texture provider's slot 2 (TextureProvider::ClearAll, sub_82594328),
// the one the loads' resets call through each provider's vtable.
constexpr uint32_t kTextureProviderClearAll = 0x82594328;

// Whether CarModelLoadsReset runs natively (nothing changed): the skeleton's
// node values (+32's +176) none or a block the layer gave, room in the
// pending frees for both XPRs' blocks, and each provider's slot 2
// TextureProvider::ClearAll.
bool CarModelLoadsResettable(uint8_t* base, uint32_t loads);

// CarModelLoads::Reset (sub_820D3B58): LoadSlot::Reset on the node file's
// slot (+0), Skeleton::FreeNodeValues (+32); for the two XPR2s their slot
// (+224 + 24i) reset, XPR::Release (+272 + 24i) and the provider's (+320 +
// 20i) slot 2 (ClearAll); +360 = 0; both 24-byte slots of the three layer
// records (+364 + 48i) and the six sticker records (+652 + 48i) reset;
// +2956, +2960, +2972 to +2984 = 0. Returns the original's r3 (the last
// slot, +916); nullopt, with nothing done, where CarModelLoadsResettable
// says no.
std::optional<uint32_t> CarModelLoadsReset(uint8_t* base, uint32_t loads);

}  // namespace scene_loaders
