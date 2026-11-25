// The scene's loading objects' register steps (src/scene_loaders/,
// docs/functions/scene-loaders.md), shared by the adapters
// (src/hooks/scene_loaders.cpp, behind --native_scene_loaders) and the
// harness (without the flag). Each leaves r3 as the original does and runs
// the original where the plain C++ refuses. The ones that allocate run
// natively only while --native_heap routes the game's heap to the
// allocation layer (else the original).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "scene_loaders/pools.h"

#include <functional>

namespace hooks {

// ScenePools::MakeA sub_820C3C40: r3 = the set.
void SceneLoadersMakeA(lifted::Regs& ctx);
// sub_820D0960: r3 = where to store, r4 = the size; r3 = the pages.
void SceneLoadersAllocPages(lifted::Regs& ctx);
// PoolNode::PoolNode sub_82594050 (r3 = the node, left; r4 = the list) and
// PoolNode::~PoolNode sub_825940B0 (r3 = the node, left).
void SceneLoadersPoolNodeConstruct(lifted::Regs& ctx);
void SceneLoadersPoolNodeDestruct(lifted::Regs& ctx);
// The queries: r3 = the object; r3 = the answer.
void SceneLoadersCarModelIsLoaded(lifted::Regs& ctx);
void SceneLoadersPool4IsLoaded(lifted::Regs& ctx);
void SceneLoadersPartsIsLoaded(lifted::Regs& ctx);
void SceneLoadersInteriorIsLoaded(lifted::Regs& ctx);
// Accessories::AllLoaded sub_820F07D8 (no adapter: only Interior::IsLoaded
// calls it): r3 = the set; r3 = 1 or 0.
void SceneLoadersAccessoriesAllLoaded(lifted::Regs& ctx);
// Pool4Object::SetRecord sub_820D6A60: r3 = the object, r4 = the record;
// r3 = the copy's destination.
void SceneLoadersPool4SetRecord(lifted::Regs& ctx);
// CarParts::SlotOf sub_820D6D48 (no adapter: only CarParts::Part calls it):
// r4 = the kind, r5, r6; r3 = the slot (r3 left above 15).
void SceneLoadersPartsSlotOf(lifted::Regs& ctx);
// CarParts::Part sub_820D6E10: r3 = the parts, r4 = the kind, r5, r6; r3 =
// the part model or 0.
void SceneLoadersPartsPart(lifted::Regs& ctx);
// sub_820D2270 (no adapter: only CarParts::WalkMaterials calls it): r3 = a
// part model; r3 as the original leaves it.
void SceneLoadersPartWalkMaterials(lifted::Regs& ctx);
// CarParts::WalkMaterials sub_820D6EB8: r3 = the parts; r3 as the original
// leaves it.
void SceneLoadersPartsWalkMaterials(lifted::Regs& ctx);
// The paint walks (r3 = the parts or the cockpit; r4 to r9 the colors for
// SetPaint's; r3 as the original leaves it): CarParts' sub_820D6F20
// (ApplyPalette) and sub_820D7388 (SetPaint), the cockpit's sub_820D5FC8
// and sub_820D6328. Natively only with --native_material_passes (else the
// original).
void SceneLoadersPartsApplyPalette(lifted::Regs& ctx);
void SceneLoadersPartsSetPaint(lifted::Regs& ctx);
void SceneLoadersCockpitApplyPalette(lifted::Regs& ctx);
void SceneLoadersCockpitSetPaint(lifted::Regs& ctx);
// The interior's sub_820D7690 (ApplyPalette) and sub_820D7840 (SetPaint),
// the traffic car's sub_820D7BF8 (ApplyPalette): as the cockpit's.
void SceneLoadersInteriorApplyPalette(lifted::Regs& ctx);
void SceneLoadersInteriorSetPaint(lifted::Regs& ctx);
void SceneLoadersTrafficApplyPalette(lifted::Regs& ctx);
// Interior::Release sub_820D75F8 (r3 the interior; r3 as the original
// leaves it), ~Interior sub_820D76D8 and its deleting destructor
// sub_820D7960 (r4 the flags; r3 left), ScenePools::ReleaseA sub_820C4488
// (r3 the set, r4 the pool, r5 the index; r3 as the original leaves it):
// natively only while hooks::ModelsNativeLifetime() and the accessories'
// release is native (--native_accessories, AccessoriesNativeLoads), and for
// ReleaseA's pool 0 --native_car_models (else the original).
void SceneLoadersInteriorRelease(lifted::Regs& ctx);
void SceneLoadersInteriorDestruct(lifted::Regs& ctx);
void SceneLoadersInteriorDeletingDestruct(lifted::Regs& ctx);
void SceneLoadersReleaseA(lifted::Regs& ctx);
// Whether ReleaseA's plain C++ runs for `pool` (the conditions above), for
// native callers (the second set's releases).
bool SceneLoadersReleaseANative(uint32_t pool);
// CarModel::SelectedTexture sub_820D4588: r3 = the car model; r3 the
// texture or 0. Natively only with --native_models (else the original).
void SceneLoadersCarModelSelectedTexture(lifted::Regs& ctx);
// NameMatches sub_820CE800: r3, r4 = the strings; r3 = 1 or 0.
void SceneLoadersNameMatches(lifted::Regs& ctx);
// XPR::Count sub_82596F20 (r3 = the XPR) and XPR::RecordName sub_82596F40
// (r4 = the index), no adapters: only XPR::FindByName calls them.
void SceneLoadersXprCount(lifted::Regs& ctx);
void SceneLoadersXprRecordName(lifted::Regs& ctx);
// XPR::FindByName sub_820D09B8: r3 = the XPR, r4 = the name; r3 = the index
// or -1.
void SceneLoadersXprFindByName(lifted::Regs& ctx);

// CarModelLoads::CarModelLoads sub_820D3DE0 (r3 = the loads, left) and
// ~CarModelLoads sub_820D3E40 (r3 = the loads; r3 = the skeleton, as
// Skeleton::~Skeleton's adapter leaves it): only while
// hooks::ModelsNativeLifetime().
void SceneLoadersCarModelLoadsConstruct(lifted::Regs& ctx);
void SceneLoadersCarModelLoadsDestruct(lifted::Regs& ctx);

// CarModelLoads::Reset sub_820D3B58 (r3 = the loads) and CarModel::Reset
// sub_820D3E98 (r3 = the car model): r3 as the original leaves it; only
// while hooks::ModelsNativeLifetime().
void SceneLoadersCarModelLoadsReset(lifted::Regs& ctx);
void SceneLoadersCarModelReset(lifted::Regs& ctx);

// The car parts' and pool 4's lifetimes, only while
// hooks::ModelsNativeLifetime(): CarPartsLoads::CarPartsLoads sub_820D6F88
// (r3 = the loads, left), CarParts::CarParts sub_820D7428 (r3 = the parts,
// left; r4 = the list), CarParts::Release sub_820D6D00 (r3 = the parts),
// CarPartsLoads::~CarPartsLoads sub_820D6FE8 (r3 = the loads) and
// Pool4Object::Reset sub_820D6948 (r3 = the object): r3 as the original
// leaves it.
void SceneLoadersCarPartsLoadsConstruct(lifted::Regs& ctx);
void SceneLoadersCarPartsConstruct(lifted::Regs& ctx);
void SceneLoadersCarPartsRelease(lifted::Regs& ctx);
void SceneLoadersCarPartsLoadsDestruct(lifted::Regs& ctx);
void SceneLoadersPool4Reset(lifted::Regs& ctx);

// The cockpit body's and the interior's lifetimes, only while
// hooks::ModelsNativeLifetime(): CockpitLoads::CockpitLoads sub_820D6010 (r3
// = the loads, left; no adapter: only Cockpit::Cockpit calls it),
// Cockpit::Cockpit sub_820D63A0 and Interior::Interior sub_820D78B8 (r3 =
// the object, left; r4 = the list), Cockpit::Release sub_820D6238 (r3 = the
// cockpit) and CockpitLoads::~CockpitLoads sub_820D6090 (r3 = the loads): r3
// as the original leaves it.
void SceneLoadersCockpitLoadsConstruct(lifted::Regs& ctx);
void SceneLoadersCockpitConstruct(lifted::Regs& ctx);
void SceneLoadersCockpitRelease(lifted::Regs& ctx);
void SceneLoadersCockpitLoadsDestruct(lifted::Regs& ctx);
void SceneLoadersInteriorConstruct(lifted::Regs& ctx);

// The wheels' and the traffic body's lifetimes, only while
// hooks::ModelsNativeLifetime(): WheelsLoads::WheelsLoads sub_820D8F60 and
// TrafficLoads::TrafficLoads sub_820D7C40 (r3 = the loads, left);
// WheelsLoads::Reset sub_820D8AA8 (r3 = the loads; no adapter: only
// Wheels::Reset calls it), ~WheelsLoads sub_820D9010 and ~TrafficLoads
// sub_820D7CC0 (r3 = the loads), Wheels::Reset sub_820D8B58 (r3 = the
// wheels) and TrafficCar::Reset sub_820D7AB0 (r3 = the traffic body): r3 as
// the original leaves it.
void SceneLoadersWheelsLoadsConstruct(lifted::Regs& ctx);
void SceneLoadersWheelsLoadsReset(lifted::Regs& ctx);
void SceneLoadersWheelsLoadsDestruct(lifted::Regs& ctx);
void SceneLoadersWheelsReset(lifted::Regs& ctx);
void SceneLoadersTrafficLoadsConstruct(lifted::Regs& ctx);
void SceneLoadersTrafficLoadsDestruct(lifted::Regs& ctx);
void SceneLoadersTrafficReset(lifted::Regs& ctx);

// The lifetimes, only while hooks::ModelsNativeLifetime():
// Pool4Object::Pool4Object sub_820D6A70 and Wheels::Wheels sub_820D9570 (r3
// = the object, left; r4 = the list); Pool4Object::Release sub_820D6A20 and
// Wheels::Release sub_820D8E68 (r3 = the object; r3 as the original leaves
// it); ~Pool4Object sub_820D6AD0 and ~Wheels sub_820D90B8 (r3 = the object,
// left); the deleting destructors sub_820D6BC0 and sub_820D95E8 (r3 = the
// object, left; r4 = the flags: with bit 0 the original).
void SceneLoadersPool4Construct(lifted::Regs& ctx);
void SceneLoadersPool4Release(lifted::Regs& ctx);
void SceneLoadersPool4Destruct(lifted::Regs& ctx);
void SceneLoadersPool4DeletingDestruct(lifted::Regs& ctx);
void SceneLoadersWheelsConstruct(lifted::Regs& ctx);
void SceneLoadersWheelsRelease(lifted::Regs& ctx);
void SceneLoadersWheelsDestruct(lifted::Regs& ctx);
void SceneLoadersWheelsDeletingDestruct(lifted::Regs& ctx);

// CarModel::CarModel sub_820D5890 and TrafficCar::TrafficCar sub_820D8908
// (r3 = the object, left; r4 = the list); TrafficCar::Release sub_820D7B90
// (r3 = the traffic body; r3 as the original leaves it); the destructors
// ~CarModel sub_820D4750, ~CarParts sub_820D7038, ~Cockpit sub_820D64D0,
// ~TrafficCar sub_820D7E90 (r3 = the object, left) and their deleting
// destructors sub_820D5988, sub_820D74C0, sub_820D6798, sub_820D89B8 (r3 =
// the object, left; r4 = the flags: with bit 0 the original); only while
// hooks::ModelsNativeLifetime().
void SceneLoadersCarModelConstruct(lifted::Regs& ctx);
void SceneLoadersCarModelDestruct(lifted::Regs& ctx);
void SceneLoadersCarModelDeletingDestruct(lifted::Regs& ctx);
void SceneLoadersCarPartsDestruct(lifted::Regs& ctx);
void SceneLoadersCarPartsDeletingDestruct(lifted::Regs& ctx);
void SceneLoadersCockpitDestruct(lifted::Regs& ctx);
void SceneLoadersCockpitDeletingDestruct(lifted::Regs& ctx);
void SceneLoadersTrafficConstruct(lifted::Regs& ctx);
void SceneLoadersTrafficRelease(lifted::Regs& ctx);
void SceneLoadersTrafficDestruct(lifted::Regs& ctx);
void SceneLoadersTrafficDeletingDestruct(lifted::Regs& ctx);

// Cockpit::BindTextures sub_820D6108 and TrafficCar::BindTextures
// sub_820D7D38 (r3 = the object; r3 = 1, or 0 while loading), only with
// --native_models (their callees' plain C++).
void SceneLoadersCockpitBindTextures(lifted::Regs& ctx);
void SceneLoadersTrafficBindTextures(lifted::Regs& ctx);

// ModelPoint::Find sub_820D02D0 (r3 = the point record, r4 = the part
// model; r3 = 1 or 0), only with --native_models and native memcpy:
// Skeleton::UpdateMatrices' original from its frame where its plain C++
// declines.
void SceneLoadersModelPointFind(lifted::Regs& ctx);

// Cockpit::OnFileLoaded sub_820D6528 (r3 = the request, r4 = the buffer,
// r6 = the cockpit; r3 as the original leaves it), only while
// hooks::ModelsNativeLifetime(): its callees' originals from its frame where
// their plain C++ declines.
void SceneLoadersCockpitOnFileLoaded(lifted::Regs& ctx);

// TrafficCar::OnFileLoaded sub_820D8430 (r3 = the request, r6 = the
// traffic car; r3 as the original leaves it), only while
// hooks::ModelsNativeLifetime() and memcpy is native: its callees'
// originals from its frame where their plain C++ declines.
void SceneLoadersTrafficOnFileLoaded(lifted::Regs& ctx);

// ScenePools::CreateA sub_820C3FD8 (r3 = the set, r4 = the record; r3 as
// the original leaves it), only while hooks::ModelsNativeLifetime(): the
// constructors' originals called from its frame where their plain C++
// declines.
void SceneLoadersCreateA(lifted::Regs& ctx);
// For CreateA's native callers: `run` given the calls CreateA's adapter
// hands it (the constructors' originals from a 160-byte frame below ctx.r1,
// which is to be CreateA's caller's frame); its answer returned.
uint32_t SceneLoadersCreateAWith(
    lifted::Regs& ctx, const std::function<uint32_t(const scene_loaders::CreateCalls&)>& run);
// Interior::OnPartLoaded sub_820D7578 (r3 = the request, r6 = the interior;
// r3 as the original leaves it), only while hooks::ModelsNativeLifetime():
// Model::Load's original called from its frame where its plain C++ declines.
void SceneLoadersInteriorOnPartLoaded(lifted::Regs& ctx);

}  // namespace hooks
