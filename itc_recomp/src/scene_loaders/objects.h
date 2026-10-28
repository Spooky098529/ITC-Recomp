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

#include "models/model_load.h"

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

// The guest calls Interior::OnPartLoaded makes: Model::Load's own (`model_load`),
// and Model::Load's original (`load(user, model, file)`, r5 = 0, from a frame
// as the callback's) where its plain C++ declines.
struct PartLoadedCalls {
  models::LoadCalls model_load;
  void (*load)(void* user, uint32_t model, uint32_t file) = nullptr;
  void* user = nullptr;
};

// Interior::OnPartLoaded (sub_820D7578, Archive::Load's callback for
// CarInterior::LoadParts): r3 = the request, r6 = the interior. The first of
// the six slots whose request (+164 + 24i) is `request`: Model::Load(+304 +
// 464i, the slot's buffer +176 + 24i, 0), LoadSlot::ReleaseBuffer (+160 +
// 24i), state +180 + 24i = 3; nothing when none matches. Returns the
// original's r3 (ReleaseBuffer's, or `request`); nullopt, with nothing done,
// where the slot's buffer isn't the layer's pages.
std::optional<uint32_t> InteriorOnPartLoaded(uint8_t* base, uint32_t request, uint32_t interior,
                                             const PartLoadedCalls& calls);

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

// --- The lifetimes: constructors, releases, destructors ---
//
// The destructors leave r3 the object (as PoolNode::~PoolNode leaves it).
// The deleting destructors (each class's vtable slot 0; ScenePools' teardown
// calls it with flags 0 before freeing the pages) run the destructor; with
// bit 0 of the flags (operator delete after it, never seen in play) they
// return false with nothing done, and the original runs.

// The pool objects' base vtable, set before PoolNode::~PoolNode.
constexpr uint32_t kPoolObjectVtable = 0x82004F90;
constexpr uint32_t kPool4Vtable = 0x82005064;
constexpr uint32_t kWheelsVtable = 0x82005100;

// Pool4Object::Pool4Object (sub_820D6A70): PoolNode::PoolNode (r4 = the
// list), +16 = 0, the vtable, +20 = -1; the part model (PartModel::PartModel,
// +112); Pool4Object::Reset. False, with nothing done, where a callee would
// go to the original.
bool Pool4Construct(uint8_t* base, uint32_t object, uint32_t list);
// Pool4Object::Release (sub_820D6A20): LoadSlot::ReleaseBuffer (+80),
// Model::Release (+112), Pool4Object::Reset. Returns the original's r3
// (Reset's); nullopt, with nothing done, where a callee would go to the
// original.
std::optional<uint32_t> Pool4Release(uint8_t* base, uint32_t object);
// Pool4Object::~Pool4Object (sub_820D6AD0): the vtable, ReleaseBuffer (+80),
// Model::Release (+112), Pool4Object::Reset, PartModel::~PartModel (+112),
// the base vtable, PoolNode::~PoolNode. False, with nothing done, where a
// callee would go to the original.
bool Pool4Destruct(uint8_t* base, uint32_t object);
// sub_820D6BC0 (the deleting destructor): Pool4Destruct (see above).
bool Pool4DeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags);

// Wheels::Wheels (sub_820D9570): PoolNode::PoolNode (r4 = the list), +16 =
// 0, the vtable, +20 = -1; sub_820D3B38 on +72 and +84 (the record's +44 and
// +56); WheelsLoads::WheelsLoads (+192); Wheels::Reset. False, with nothing
// done, where a callee would go to the original.
bool WheelsConstruct(uint8_t* base, uint32_t wheels, uint32_t list);
// Wheels::Release (sub_820D8E68): LoadSlot::ReleaseBuffer on +192, +1168,
// +2144, +2280, then the same + 24; Wheels::Reset. Returns the original's r3
// (Reset's); nullopt, with nothing done, where a callee would go to the
// original (two slots freeing the same pages included).
std::optional<uint32_t> WheelsRelease(uint8_t* base, uint32_t wheels);
// Wheels::~Wheels (sub_820D90B8): the vtable, Wheels::Release,
// WheelsLoads::~WheelsLoads (+192), the base vtable, PoolNode::~PoolNode.
// False, with nothing done, where a callee would go to the original.
bool WheelsDestruct(uint8_t* base, uint32_t wheels);
// sub_820D95E8 (the deleting destructor): WheelsDestruct.
bool WheelsDeletingDestruct(uint8_t* base, uint32_t wheels, uint32_t flags);

constexpr uint32_t kCarModelVtable = 0x82004F94;
constexpr uint32_t kTrafficVtable = 0x820050B4;

// CarModel::CarModel (sub_820D5890): PoolNode::PoolNode (r4 = the list), +16
// = 0, the vtable, +20 = -1; sub_820D45B0 on the car record (+32);
// CarModelLoads::CarModelLoads (+3728); the w of each row of the matrices at
// +6720 + 64i (four), of the positions +6976 + 16i (four), of the matrices'
// rows +7040 + 64i (two) and the positions +7168 + 16i (two), and +7228 =
// the word at 0x82000A30 (1.0); CarModel::Reset. False, with nothing done,
// where a callee would go to the original (a car model in the physical
// views included, as CarModel::Reset).
bool CarModelConstruct(uint8_t* base, uint32_t car_model, uint32_t list);
// CarModel::~CarModel (sub_820D4750): the vtable, CarModel::Release,
// CarModelLoads::~CarModelLoads (+3728), the base vtable,
// PoolNode::~PoolNode. False, with nothing done, where a callee would go to
// the original.
bool CarModelDestruct(uint8_t* base, uint32_t car_model);
// sub_820D5988 (the deleting destructor): CarModelDestruct.
bool CarModelDeletingDestruct(uint8_t* base, uint32_t car_model, uint32_t flags);

// CarParts::~CarParts (sub_820D7038): the vtable, CarParts::Release,
// CarPartsLoads::~CarPartsLoads (+96), the base vtable, PoolNode::~PoolNode.
// False, with nothing done, where a callee would go to the original.
bool CarPartsDestruct(uint8_t* base, uint32_t parts);
// sub_820D74C0 (the deleting destructor): CarPartsDestruct.
bool CarPartsDeletingDestruct(uint8_t* base, uint32_t parts, uint32_t flags);

// Cockpit::~Cockpit (sub_820D64D0): the vtable, Cockpit::Release,
// CockpitLoads::~CockpitLoads (+176), the base vtable, PoolNode::~PoolNode.
// False, with nothing done, where a callee would go to the original.
bool CockpitDestruct(uint8_t* base, uint32_t cockpit);
// sub_820D6798 (the deleting destructor): CockpitDestruct.
bool CockpitDeletingDestruct(uint8_t* base, uint32_t cockpit, uint32_t flags);

// TrafficCar::TrafficCar (sub_820D8908): PoolNode::PoolNode (r4 = the list),
// +16 = 0, the vtable, +20 = -1; sub_820D45B0 on the car record (+32);
// TrafficLoads::TrafficLoads (+3728); the w of the positions +7072 + 16i
// (four) and of each row of the matrices +7136 + 64i (four) = the word at
// 0x82000A30 (1.0); TrafficCar::Reset. False, with nothing done, where a
// callee would go to the original.
bool TrafficConstruct(uint8_t* base, uint32_t traffic, uint32_t list);
// TrafficCar::Release (sub_820D7B90): the two textures' words (+3728 to
// +3740, +7040 to +7052) put off (DeferRelease on [+7392], read for each),
// LoadSlot::ReleaseBuffer (+3744), TrafficCar::Reset. Returns the original's
// r3 (Reset's); nullopt, with nothing done, where a callee would go to the
// original.
std::optional<uint32_t> TrafficRelease(uint8_t* base, uint32_t traffic);
// TrafficCar::~TrafficCar (sub_820D7E90): the vtable, TrafficCar::Release,
// TrafficLoads::~TrafficLoads (+3728), the base vtable, PoolNode::~PoolNode.
// False, with nothing done, where a callee would go to the original.
bool TrafficDestruct(uint8_t* base, uint32_t traffic);
// sub_820D89B8 (the deleting destructor): TrafficDestruct.
bool TrafficDeletingDestruct(uint8_t* base, uint32_t traffic, uint32_t flags);

}  // namespace scene_loaders
