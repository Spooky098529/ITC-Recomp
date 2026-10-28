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

// --- The car parts (pool 1) ---

// The car parts (106,016 bytes): +96 the loads (216 slots, then the part
// models at the loads' +5184, the parts' +5280), +28 the parts record,
// +106000 the first pool set.
constexpr uint32_t kCarPartsVtable = 0x82005094;
constexpr uint32_t kCarPartsLoads = 96;

// CarPartsLoads::CarPartsLoads (sub_820D6F88): the 216 part models
// (PartModel::PartModel, +5184 + 464i), then the 30 words at +105436 + 16i
// = the word at 0x82000A30 (1.0, as the original loads it). False, with
// nothing done, where a part model's constructor would go to the original.
bool CarPartsLoadsConstruct(uint8_t* base, uint32_t loads);

// CarParts::CarParts (sub_820D7428): PoolNode::PoolNode (r4 = the list),
// +20 = -1, the vtable, +16 = 0; CarPartsLoads::CarPartsLoads (+96); +24 =
// 0; for the 216 parts the slot (+96 + 24i) reset, Model::Release and
// PartModel::Clear on the model (+5280 + 464i); the parts record's defaults
// (sub_820D3558, +28); +106000 = [0x826C2E40]. False, with nothing done,
// where a callee would go to the original.
bool CarPartsConstruct(uint8_t* base, uint32_t parts, uint32_t list);

// CarParts::Release (sub_820D6D00): for the 216 parts,
// LoadSlot::ReleaseBuffer (+96 + 24i), Model::Release and PartModel::Clear
// (+5280 + 464i). Returns the original's r3 (the last model); nullopt, with
// nothing done, where a callee would go to the original.
std::optional<uint32_t> CarPartsRelease(uint8_t* base, uint32_t parts);

// CarPartsLoads::~CarPartsLoads (sub_820D6FE8): PartModel::~PartModel on
// the 216 part models, last first. Returns the original's r3 (the last
// destructor's); nullopt, with nothing done, where one would go to the
// original.
std::optional<uint32_t> CarPartsLoadsDestruct(uint8_t* base, uint32_t loads);

// --- The cockpit body (pool 2) and the interior (pool 3) ---

constexpr uint32_t kCockpitVtable = 0x82005020;
constexpr uint32_t kCockpitLoads = 176;
constexpr uint32_t kInteriorVtable = 0x8200509C;

// Cockpit::Cockpit (sub_820D63A0): PoolNode::PoolNode (r4 = the list), +20
// = -1, the vtable, +16 = 0; CockpitLoads::CockpitLoads (+176); +3500 = 1.0,
// +24 = 0, +3508 = [0x826C2E40]; Skeleton::FreeNodeValues (+208); the
// cockpit record's defaults (sub_820D34B8, +28); +3472 = 0, +3488 = (0, 0,
// 0, 1), +3504 = 0; the node file's slot (+176) reset; the two XPR2s' slots
// (+400 + 24i) reset, XPR::Release (+448 + 24i) and the providers' (+496 +
// 20i) slot 2; the six parts' slots (+536 + 24i) reset, Model::Release and
// PartModel::Clear (+688 + 464i). The 0.0 and 1.0 the words at 0x82000A34
// and 0x82000A30. False, with nothing done, where a callee would go to the
// original (the providers' slot 2 not ClearAll included).
bool CockpitConstruct(uint8_t* base, uint32_t cockpit, uint32_t list);

// Cockpit::Release (sub_820D6238): the node file's slot (+176)
// ReleaseBuffer and Reset, Skeleton::FreeNodeValues (+208); the XPR2s'
// slots (+400 + 24i) released and reset, XPR::Release (+448 + 24i), the
// providers' (+496 + 20i) slot 2; +3508 = 0, +24 = 0; the record's defaults
// (+28); +3472 = 0, +3488 = (0, 0, 0, 1), +3504 = 0; the six parts' slots
// (+536 + 24i) released and reset. Returns the original's r3 (the last
// slot, +656); nullopt, with nothing done, where a callee would go to the
// original.
std::optional<uint32_t> CockpitRelease(uint8_t* base, uint32_t cockpit);

// Interior::Interior (sub_820D78B8): PoolNode::PoolNode (r4 = the list),
// +16 = 0, the vtable, +20 = -1; six part models (PartModel::PartModel,
// +304 + 464i); +24 = 0; the six slots (+160 + 24i) reset, Model::Release
// and PartModel::Clear on the models; the interior record's defaults
// (sub_820D3840, +28); +3088 = 0, +3092 = [0x826C2E40]. False, with nothing
// done, where a callee would go to the original.
bool InteriorConstruct(uint8_t* base, uint32_t interior, uint32_t list);

// --- The wheels (pool 5) and the traffic body (pool 6) ---

constexpr uint32_t kWheelsLoads = 192;
constexpr uint32_t kTrafficLoads = 3728;

// Wheels::Reset (sub_820D8B58): +24 = 0, +3268 = [0x826C2E40], the wheels
// record's defaults (sub_820D3900, +28), WheelsLoads::Reset (+192). Returns
// the original's r3 (WheelsLoads::Reset's); nullopt, with nothing done,
// where that would go to the original.
std::optional<uint32_t> WheelsReset(uint8_t* base, uint32_t wheels);

// TrafficCar::Reset (sub_820D7AB0): +7392 = [0x826C2E40]; CarRecord::Reset
// (+32); +7056 = -1; the node file's slot (+3744) reset,
// Skeleton::FreeNodeValues (+3776); the two XPR2s' slots (+3968 + 24i)
// reset, XPR::Release (+4016 + 24i), the providers' (+4064 + 20i) slot 2;
// +3728 to +3740 and +7040 to +7052 = 0; the six slots (+4104 + 24i) reset,
// Model::Release and PartModel::Clear (+4256 + 464i). Returns the
// original's r3 (the last model, +6576); nullopt, with nothing done, where a
// callee would go to the original.
std::optional<uint32_t> TrafficReset(uint8_t* base, uint32_t traffic);

// --- Pool 4's object ---

// Pool4Object::Reset (sub_820D6948): +24 = 0; the record's defaults
// (sub_820D34E0, +28); the slot (+80) reset; Model::Release and
// PartModel::Clear on the model (+112); +576 = [0x826C2E40]. Returns the
// original's r3 (the model); nullopt, with nothing done, where
// Model::Release would go to the original.
std::optional<uint32_t> Pool4Reset(uint8_t* base, uint32_t object);

}  // namespace scene_loaders
