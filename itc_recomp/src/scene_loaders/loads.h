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

// The members three loads share at their own offsets: the skeleton, the
// two XPRs, the two texture providers.
struct MembersLayout {
  uint32_t skeleton;
  uint32_t xprs[2];
  uint32_t providers[2];
};
constexpr MembersLayout kCarModelMembers{32, {272, 296}, {320, 340}};  // and the cockpit's
constexpr MembersLayout kTrafficMembers{48, {288, 312}, {336, 356}};

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
// Whether CarModelLoadsDestruct runs natively (its checks, nothing changed;
// the pending frees for the XPRs' four nodes included).
bool CarModelLoadsDestructible(uint8_t* base, uint32_t loads);
// Whether each of the loads' providers (at `m`'s offsets) has
// TextureProvider::ClearAll as its slot 2 (the resets call it through the
// vtable).
bool ProvidersClearAll(uint8_t* base, uint32_t loads, const MembersLayout& m);

// CockpitLoads (the cockpit's +176): CarModelLoads' members at the same
// offsets, then six part models from +512.
constexpr uint32_t kCockpitLoadsParts = 512;

// CockpitLoads::CockpitLoads (sub_820D6010): Skeleton::Skeleton (+32),
// XPR::XPR (+272, +296), TextureProvider::TextureProvider (+320, +340), the
// six part models (PartModel::PartModel, +512 + 464i). False, with nothing
// done, where a constructor would go to the original.
bool CockpitLoadsConstruct(uint8_t* base, uint32_t loads);

// CockpitLoads::~CockpitLoads (sub_820D6090): PartModel::~PartModel on the
// six part models (last first), the providers' destructors (+340, +320),
// XPR::~XPR (+296, +272), Skeleton::~Skeleton (+32). Returns the original's
// r3 (the skeleton, +32); nullopt, with nothing done, where a destructor
// would go to the original.
std::optional<uint32_t> CockpitLoadsDestruct(uint8_t* base, uint32_t loads);

// TrafficLoads (the traffic body's +3728): the members at kTrafficMembers,
// six part models from +528. Constructed and destroyed as the cockpit's.
constexpr uint32_t kTrafficLoadsParts = 528;
// TrafficLoads::TrafficLoads (sub_820D7C40) and ~TrafficLoads (sub_820D7CC0,
// r3 = the skeleton, +48): as CockpitLoads'.
bool TrafficLoadsConstruct(uint8_t* base, uint32_t loads);
std::optional<uint32_t> TrafficLoadsDestruct(uint8_t* base, uint32_t loads);

// WheelsLoads (the wheels' +192): for front and rear (+24i, +464i) the
// tires' slots (+0) and models (+48), the rims' (+976, +1024), the XPR2s'
// slots (+1952), XPRs (+2000) and providers (+2048, 20 bytes apart), the
// calipers' slots (+2088) and models (+2144).
constexpr uint32_t kWheelsTires = 48;
constexpr uint32_t kWheelsRims = 1024;
constexpr uint32_t kWheelsXprs = 2000;
constexpr uint32_t kWheelsProviders = 2048;
constexpr uint32_t kWheelsCalipers = 2144;

// WheelsLoads::WheelsLoads (sub_820D8F60): the tires' and rims' part models,
// XPR::XPR (both), the providers (64 slots each), the calipers' part models.
// False, with nothing done, where a constructor would go to the original.
bool WheelsLoadsConstruct(uint8_t* base, uint32_t loads);
// WheelsLoads::Reset (sub_820D8AA8): for front and rear, the tire's slot
// reset, Model::Release and PartModel::Clear on its model; the rim's the
// same; the XPR2's slot reset, XPR::Release, the provider's slot 2
// (ClearAll); the caliper's slot and model as the tire's. Returns the
// original's r3 (the rear caliper, +2608); nullopt, with nothing done,
// where a callee would go to the original.
std::optional<uint32_t> WheelsLoadsReset(uint8_t* base, uint32_t loads);
// WheelsLoads::~WheelsLoads (sub_820D9010): PartModel::~PartModel on the
// calipers (rear first), the providers' destructors, XPR::~XPR (both, rear
// first), then the rims and the tires the same. Returns the original's r3
// (the front tire's destructor's); nullopt, with nothing done, where a
// destructor would go to the original.
std::optional<uint32_t> WheelsLoadsDestruct(uint8_t* base, uint32_t loads);

// Whether WheelsLoadsDestruct's members run natively (the six part models'
// members and the two providers' storage; the pending frees apart): for the
// wheels' destructor, which checks before its release.
bool WheelsLoadsMembersDestructible(uint8_t* base, uint32_t loads);

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
