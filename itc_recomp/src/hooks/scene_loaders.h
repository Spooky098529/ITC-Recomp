// The scene's loading objects' register steps (src/scene_loaders/,
// docs/functions/scene-loaders.md), shared by the adapters
// (src/hooks/scene_loaders.cpp, behind --native_scene_loaders) and the
// harness (without the flag). Each leaves r3 as the original does and runs
// the original where the plain C++ refuses. The ones that allocate run
// natively only while --native_heap routes the game's heap to the
// allocation layer (else the original).

#pragma once

#include "guest.h"

namespace hooks {

// ScenePools::MakeA sub_820C3C40: r3 = the set.
void SceneLoadersMakeA(PPCContext& ctx, uint8_t* base);
// sub_820D0960: r3 = where to store, r4 = the size; r3 = the pages.
void SceneLoadersAllocPages(PPCContext& ctx, uint8_t* base);
// PoolNode::PoolNode sub_82594050 (r3 = the node, left; r4 = the list) and
// PoolNode::~PoolNode sub_825940B0 (r3 = the node, left).
void SceneLoadersPoolNodeConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersPoolNodeDestruct(PPCContext& ctx, uint8_t* base);
// The queries: r3 = the object; r3 = the answer.
void SceneLoadersCarModelIsLoaded(PPCContext& ctx, uint8_t* base);
void SceneLoadersPool4IsLoaded(PPCContext& ctx, uint8_t* base);
void SceneLoadersPartsIsLoaded(PPCContext& ctx, uint8_t* base);
void SceneLoadersInteriorIsLoaded(PPCContext& ctx, uint8_t* base);
// Accessories::AllLoaded sub_820F07D8 (no adapter: only Interior::IsLoaded
// calls it): r3 = the set; r3 = 1 or 0.
void SceneLoadersAccessoriesAllLoaded(PPCContext& ctx, uint8_t* base);
// Pool4Object::SetRecord sub_820D6A60: r3 = the object, r4 = the record;
// r3 = the copy's destination.
void SceneLoadersPool4SetRecord(PPCContext& ctx, uint8_t* base);
// CarParts::SlotOf sub_820D6D48 (no adapter: only CarParts::Part calls it):
// r4 = the kind, r5, r6; r3 = the slot (r3 left above 15).
void SceneLoadersPartsSlotOf(PPCContext& ctx, uint8_t* base);
// CarParts::Part sub_820D6E10: r3 = the parts, r4 = the kind, r5, r6; r3 =
// the part model or 0.
void SceneLoadersPartsPart(PPCContext& ctx, uint8_t* base);
// sub_820D2270 (no adapter: only CarParts::WalkMaterials calls it): r3 = a
// part model; r3 as the original leaves it.
void SceneLoadersPartWalkMaterials(PPCContext& ctx, uint8_t* base);
// CarParts::WalkMaterials sub_820D6EB8: r3 = the parts; r3 as the original
// leaves it.
void SceneLoadersPartsWalkMaterials(PPCContext& ctx, uint8_t* base);
// NameMatches sub_820CE800: r3, r4 = the strings; r3 = 1 or 0.
void SceneLoadersNameMatches(PPCContext& ctx, uint8_t* base);
// XPR::Count sub_82596F20 (r3 = the XPR) and XPR::RecordName sub_82596F40
// (r4 = the index), no adapters: only XPR::FindByName calls them.
void SceneLoadersXprCount(PPCContext& ctx, uint8_t* base);
void SceneLoadersXprRecordName(PPCContext& ctx, uint8_t* base);
// XPR::FindByName sub_820D09B8: r3 = the XPR, r4 = the name; r3 = the index
// or -1.
void SceneLoadersXprFindByName(PPCContext& ctx, uint8_t* base);

// CarModelLoads::CarModelLoads sub_820D3DE0 (r3 = the loads, left) and
// ~CarModelLoads sub_820D3E40 (r3 = the loads; r3 = the skeleton, as
// Skeleton::~Skeleton's adapter leaves it): only while
// hooks::ModelsNativeLifetime().
void SceneLoadersCarModelLoadsConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarModelLoadsDestruct(PPCContext& ctx, uint8_t* base);

// CarModelLoads::Reset sub_820D3B58 (r3 = the loads) and CarModel::Reset
// sub_820D3E98 (r3 = the car model): r3 as the original leaves it; only
// while hooks::ModelsNativeLifetime().
void SceneLoadersCarModelLoadsReset(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarModelReset(PPCContext& ctx, uint8_t* base);

// The car parts' and pool 4's lifetimes, only while
// hooks::ModelsNativeLifetime(): CarPartsLoads::CarPartsLoads sub_820D6F88
// (r3 = the loads, left), CarParts::CarParts sub_820D7428 (r3 = the parts,
// left; r4 = the list), CarParts::Release sub_820D6D00 (r3 = the parts),
// CarPartsLoads::~CarPartsLoads sub_820D6FE8 (r3 = the loads) and
// Pool4Object::Reset sub_820D6948 (r3 = the object): r3 as the original
// leaves it.
void SceneLoadersCarPartsLoadsConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarPartsConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarPartsRelease(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarPartsLoadsDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersPool4Reset(PPCContext& ctx, uint8_t* base);

// The cockpit body's and the interior's lifetimes, only while
// hooks::ModelsNativeLifetime(): CockpitLoads::CockpitLoads sub_820D6010 (r3
// = the loads, left; no adapter: only Cockpit::Cockpit calls it),
// Cockpit::Cockpit sub_820D63A0 and Interior::Interior sub_820D78B8 (r3 =
// the object, left; r4 = the list), Cockpit::Release sub_820D6238 (r3 = the
// cockpit) and CockpitLoads::~CockpitLoads sub_820D6090 (r3 = the loads): r3
// as the original leaves it.
void SceneLoadersCockpitLoadsConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCockpitConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCockpitRelease(PPCContext& ctx, uint8_t* base);
void SceneLoadersCockpitLoadsDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersInteriorConstruct(PPCContext& ctx, uint8_t* base);

// The wheels' and the traffic body's lifetimes, only while
// hooks::ModelsNativeLifetime(): WheelsLoads::WheelsLoads sub_820D8F60 and
// TrafficLoads::TrafficLoads sub_820D7C40 (r3 = the loads, left);
// WheelsLoads::Reset sub_820D8AA8 (r3 = the loads; no adapter: only
// Wheels::Reset calls it), ~WheelsLoads sub_820D9010 and ~TrafficLoads
// sub_820D7CC0 (r3 = the loads), Wheels::Reset sub_820D8B58 (r3 = the
// wheels) and TrafficCar::Reset sub_820D7AB0 (r3 = the traffic body): r3 as
// the original leaves it.
void SceneLoadersWheelsLoadsConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersWheelsLoadsReset(PPCContext& ctx, uint8_t* base);
void SceneLoadersWheelsLoadsDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersWheelsReset(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficLoadsConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficLoadsDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficReset(PPCContext& ctx, uint8_t* base);

// The lifetimes, only while hooks::ModelsNativeLifetime():
// Pool4Object::Pool4Object sub_820D6A70 and Wheels::Wheels sub_820D9570 (r3
// = the object, left; r4 = the list); Pool4Object::Release sub_820D6A20 and
// Wheels::Release sub_820D8E68 (r3 = the object; r3 as the original leaves
// it); ~Pool4Object sub_820D6AD0 and ~Wheels sub_820D90B8 (r3 = the object,
// left); the deleting destructors sub_820D6BC0 and sub_820D95E8 (r3 = the
// object, left; r4 = the flags: with bit 0 the original).
void SceneLoadersPool4Construct(PPCContext& ctx, uint8_t* base);
void SceneLoadersPool4Release(PPCContext& ctx, uint8_t* base);
void SceneLoadersPool4Destruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersPool4DeletingDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersWheelsConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersWheelsRelease(PPCContext& ctx, uint8_t* base);
void SceneLoadersWheelsDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersWheelsDeletingDestruct(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
