// The scene's loading objects' register steps (src/scene_loaders/,
// docs/functions/scene-loaders.md), shared by the adapters
// (src/hooks/scene_loaders.cpp, behind --native_scene_loaders) and the
// harness (without the flag). Each leaves r3 as the original does and runs
// the original where the plain C++ refuses. The ones that allocate run
// natively only while --native_heap routes the game's heap to the
// allocation layer (else the original).

#pragma once

#include "guest.h"
#include "scene_loaders/pools.h"

#include <functional>

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
// The paint walks (r3 = the parts or the cockpit; r4 to r9 the colors for
// SetPaint's; r3 as the original leaves it): CarParts' sub_820D6F20
// (ApplyPalette) and sub_820D7388 (SetPaint), the cockpit's sub_820D5FC8
// and sub_820D6328. Natively only with --native_material_passes (else the
// original).
void SceneLoadersPartsApplyPalette(PPCContext& ctx, uint8_t* base);
void SceneLoadersPartsSetPaint(PPCContext& ctx, uint8_t* base);
void SceneLoadersCockpitApplyPalette(PPCContext& ctx, uint8_t* base);
void SceneLoadersCockpitSetPaint(PPCContext& ctx, uint8_t* base);
// The interior's sub_820D7690 (ApplyPalette) and sub_820D7840 (SetPaint),
// the traffic car's sub_820D7BF8 (ApplyPalette): as the cockpit's.
void SceneLoadersInteriorApplyPalette(PPCContext& ctx, uint8_t* base);
void SceneLoadersInteriorSetPaint(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficApplyPalette(PPCContext& ctx, uint8_t* base);
// Interior::Release sub_820D75F8 (r3 the interior; r3 as the original
// leaves it), ~Interior sub_820D76D8 and its deleting destructor
// sub_820D7960 (r4 the flags; r3 left), ScenePools::ReleaseA sub_820C4488
// (r3 the set, r4 the pool, r5 the index; r3 as the original leaves it):
// natively only while hooks::ModelsNativeLifetime() and the accessories'
// release is native (--native_accessories, AccessoriesNativeLoads), and for
// ReleaseA's pool 0 --native_car_models (else the original).
void SceneLoadersInteriorRelease(PPCContext& ctx, uint8_t* base);
void SceneLoadersInteriorDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersInteriorDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersReleaseA(PPCContext& ctx, uint8_t* base);
// Whether ReleaseA's plain C++ runs for `pool` (the conditions above), for
// native callers (the second set's releases).
bool SceneLoadersReleaseANative(uint32_t pool);
// CarModel::SelectedTexture sub_820D4588: r3 = the car model; r3 the
// texture or 0. Natively only with --native_models (else the original).
void SceneLoadersCarModelSelectedTexture(PPCContext& ctx, uint8_t* base);
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

// CarModel::CarModel sub_820D5890 and TrafficCar::TrafficCar sub_820D8908
// (r3 = the object, left; r4 = the list); TrafficCar::Release sub_820D7B90
// (r3 = the traffic body; r3 as the original leaves it); the destructors
// ~CarModel sub_820D4750, ~CarParts sub_820D7038, ~Cockpit sub_820D64D0,
// ~TrafficCar sub_820D7E90 (r3 = the object, left) and their deleting
// destructors sub_820D5988, sub_820D74C0, sub_820D6798, sub_820D89B8 (r3 =
// the object, left; r4 = the flags: with bit 0 the original); only while
// hooks::ModelsNativeLifetime().
void SceneLoadersCarModelConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarModelDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarModelDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarPartsDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCarPartsDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCockpitDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersCockpitDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficConstruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficRelease(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficDestruct(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficDeletingDestruct(PPCContext& ctx, uint8_t* base);

// Cockpit::BindTextures sub_820D6108 and TrafficCar::BindTextures
// sub_820D7D38 (r3 = the object; r3 = 1, or 0 while loading), only with
// --native_models (their callees' plain C++).
void SceneLoadersCockpitBindTextures(PPCContext& ctx, uint8_t* base);
void SceneLoadersTrafficBindTextures(PPCContext& ctx, uint8_t* base);

// ModelPoint::Find sub_820D02D0 (r3 = the point record, r4 = the part
// model; r3 = 1 or 0), only with --native_models and native memcpy:
// Skeleton::UpdateMatrices' original from its frame where its plain C++
// declines.
void SceneLoadersModelPointFind(PPCContext& ctx, uint8_t* base);

// Cockpit::OnFileLoaded sub_820D6528 (r3 = the request, r4 = the buffer,
// r6 = the cockpit; r3 as the original leaves it), only while
// hooks::ModelsNativeLifetime(): its callees' originals from its frame where
// their plain C++ declines.
void SceneLoadersCockpitOnFileLoaded(PPCContext& ctx, uint8_t* base);

// TrafficCar::OnFileLoaded sub_820D8430 (r3 = the request, r6 = the
// traffic car; r3 as the original leaves it), only while
// hooks::ModelsNativeLifetime() and memcpy is native: its callees'
// originals from its frame where their plain C++ declines.
void SceneLoadersTrafficOnFileLoaded(PPCContext& ctx, uint8_t* base);

// ScenePools::CreateA sub_820C3FD8 (r3 = the set, r4 = the record; r3 as
// the original leaves it), only while hooks::ModelsNativeLifetime(): the
// constructors' originals called from its frame where their plain C++
// declines.
void SceneLoadersCreateA(PPCContext& ctx, uint8_t* base);
// For CreateA's native callers: `run` given the calls CreateA's adapter
// hands it (the constructors' originals from a 160-byte frame below ctx.r1,
// which is to be CreateA's caller's frame); its answer returned.
uint32_t SceneLoadersCreateAWith(
    PPCContext& ctx, uint8_t* base,
    const std::function<uint32_t(const scene_loaders::CreateCalls&)>& run);
// Interior::OnPartLoaded sub_820D7578 (r3 = the request, r6 = the interior;
// r3 as the original leaves it), only while hooks::ModelsNativeLifetime():
// Model::Load's original called from its frame where its plain C++ declines.
void SceneLoadersInteriorOnPartLoaded(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
