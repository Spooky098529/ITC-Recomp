// The views of docs/functions/scene-loaders.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/scene-loaders.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ScenePools, new<140>@sub_820C3C40, new<140>@ScenePools::MakeA, CarInterior.+3092, TrafficCar.+576
#pragma pack(push, 1)
struct ScenePoolsData {
  uint32_t teardown_count = 0;  // +0
  uint32_t state = 0;  // +4
  uint32_t model_set = 0;  // +8
  uint32_t car_model_count = 0;  // +12
  uint32_t car_parts_count = 0;  // +16
  uint32_t interior_count = 0;  // +20
  uint32_t cockpit_count = 0;  // +24
  uint32_t pool4_count = 0;  // +28
  uint32_t wheels_count = 0;  // +32
  uint32_t unused_sum = 0;  // +36
  uint32_t traffic_count = 0;  // +40
  uint8_t pad_44[8] = {};
  uint32_t deferred_releases = 0;  // +52
  uint8_t pools[84] = {};  // +56, guest order
  uint8_t pad_140[4896] = {};
  uint32_t car_scene_pool7_count = 0;  // +5036
};
#pragma pack(pop)
static_assert(sizeof(ScenePoolsData) == 5040);
inline constexpr memory::LayoutField kScenePoolsFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {52, 4}, {5036, 4}};
inline constexpr memory::Layout kScenePoolsLayout = {kScenePoolsFields, 13};

struct ScenePools : View {
  using View::View;
  static constexpr uint32_t kSize = 140;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ScenePoolsData* data() const { return static_cast<ScenePoolsData*>(memory::HandleObject(address)); }
  static ScenePools New(uint8_t* base) {
    return ScenePools(base, memory::MakeHandle(new ScenePoolsData(), sizeof(ScenePoolsData), "ScenePools", &kScenePoolsLayout));
  }
  template <uint32_t Stride = kSize>
  static ScenePools NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ScenePoolsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ScenePoolsData*>(::operator new(sizeof(ScenePoolsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ScenePoolsData();
    return ScenePools(base, memory::MakeHandle(p, sizeof(ScenePoolsData) * count, "ScenePools", &kScenePoolsLayout, sizeof(ScenePoolsData)));
  }
  ScenePools at(uint32_t i) const { return ScenePools(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ScenePoolsData* direct() const { return static_cast<ScenePoolsData*>(memory::HandleOwn(address, &kScenePoolsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kScenePoolsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kScenePoolsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // the teardown's count (2, ScenePools::StartTeardownA); the measure's store here is AllocPages' (sub_820D0960) into its out word, the set's +52
  uint32_t teardown_count_address() const { return address + 0; }
  uint32_t teardown_count_bits() const { if (auto* d = direct()) return d->teardown_count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_teardown_count_bits(uint32_t v) const { if (auto* d = direct()) { d->teardown_count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t teardown_count() const { if (auto* d = direct()) return d->teardown_count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_teardown_count(uint32_t v) const { if (auto* d = direct()) { d->teardown_count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // 0 when made, 3 once filled, 4 tearing down
  uint32_t state_address() const { return address + 4; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // 0 set a, 1 set b (the loading objects read it as [[their first set] + 8])
  uint32_t model_set_address() const { return address + 8; }
  uint32_t model_set_bits() const { if (auto* d = direct()) return d->model_set; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_model_set_bits(uint32_t v) const { if (auto* d = direct()) { d->model_set = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t model_set() const { if (auto* d = direct()) return d->model_set; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_model_set(uint32_t v) const { if (auto* d = direct()) { d->model_set = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // pool 0's count
  uint32_t car_model_count_address() const { return address + 12; }
  uint32_t car_model_count_bits() const { if (auto* d = direct()) return d->car_model_count; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_car_model_count_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model_count = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t car_model_count() const { if (auto* d = direct()) return d->car_model_count; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_car_model_count(uint32_t v) const { if (auto* d = direct()) { d->car_model_count = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // pool 1's
  uint32_t car_parts_count_address() const { return address + 16; }
  uint32_t car_parts_count_bits() const { if (auto* d = direct()) return d->car_parts_count; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_car_parts_count_bits(uint32_t v) const { if (auto* d = direct()) { d->car_parts_count = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t car_parts_count() const { if (auto* d = direct()) return d->car_parts_count; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_car_parts_count(uint32_t v) const { if (auto* d = direct()) { d->car_parts_count = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // pool 3's
  uint32_t interior_count_address() const { return address + 20; }
  uint32_t interior_count_bits() const { if (auto* d = direct()) return d->interior_count; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_interior_count_bits(uint32_t v) const { if (auto* d = direct()) { d->interior_count = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t interior_count() const { if (auto* d = direct()) return d->interior_count; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_interior_count(uint32_t v) const { if (auto* d = direct()) { d->interior_count = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // pool 2's
  uint32_t cockpit_count_address() const { return address + 24; }
  uint32_t cockpit_count_bits() const { if (auto* d = direct()) return d->cockpit_count; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_cockpit_count_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit_count = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t cockpit_count() const { if (auto* d = direct()) return d->cockpit_count; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_cockpit_count(uint32_t v) const { if (auto* d = direct()) { d->cockpit_count = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // pool 4's
  uint32_t pool4_count_address() const { return address + 28; }
  uint32_t pool4_count_bits() const { if (auto* d = direct()) return d->pool4_count; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_pool4_count_bits(uint32_t v) const { if (auto* d = direct()) { d->pool4_count = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t pool4_count() const { if (auto* d = direct()) return d->pool4_count; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_pool4_count(uint32_t v) const { if (auto* d = direct()) { d->pool4_count = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // pool 5's
  uint32_t wheels_count_address() const { return address + 32; }
  uint32_t wheels_count_bits() const { if (auto* d = direct()) return d->wheels_count; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_wheels_count_bits(uint32_t v) const { if (auto* d = direct()) { d->wheels_count = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t wheels_count() const { if (auto* d = direct()) return d->wheels_count; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_wheels_count(uint32_t v) const { if (auto* d = direct()) { d->wheels_count = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the record's +32 + +28, not used as a count
  uint32_t unused_sum_address() const { return address + 36; }
  uint32_t unused_sum_bits() const { if (auto* d = direct()) return d->unused_sum; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_unused_sum_bits(uint32_t v) const { if (auto* d = direct()) { d->unused_sum = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t unused_sum() const { if (auto* d = direct()) return d->unused_sum; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_unused_sum(uint32_t v) const { if (auto* d = direct()) { d->unused_sum = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  // pool 6's
  uint32_t traffic_count_address() const { return address + 40; }
  uint32_t traffic_count_bits() const { if (auto* d = direct()) return d->traffic_count; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_traffic_count_bits(uint32_t v) const { if (auto* d = direct()) { d->traffic_count = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t traffic_count() const { if (auto* d = direct()) return d->traffic_count; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_traffic_count(uint32_t v) const { if (auto* d = direct()) { d->traffic_count = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  // 4 x (+12 + +40) records of 20 bytes
  uint32_t deferred_releases_address() const { return address + 52; }
  uint32_t deferred_releases_bits() const { if (auto* d = direct()) return d->deferred_releases; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_deferred_releases_bits(uint32_t v) const { if (auto* d = direct()) { d->deferred_releases = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  template <typename T = CarModelsDeferredRelease>
  T deferred_releases() const { return T(base, deferred_releases_bits()); }
  void set_deferred_releases(const View& w) const { set_deferred_releases_bits(w.address); }
  void set_deferred_releases(uint32_t v) const { set_deferred_releases_bits(v); }
  // seven lists of PoolNodes (PoolList), 12 bytes each, by pool
  uint32_t pools_address() const { return address + 56; }
  uint8_t* pools() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 56 : guest::Pointer(base, address + 56); }
  // a measure fault: ScenePools::FindModel's r3 is the car scene ([0x826C2E44]), whose +5036 is pool 7's count
  uint32_t car_scene_pool7_count_address() const { return address + 5036; }
  uint32_t car_scene_pool7_count_bits() const { if (auto* d = direct()) return d->car_scene_pool7_count; return uint32_t((memory::IsHandle(address + 5036) ? uint32_t(memory::OwnerLoad(address + 5036, 4)) : guest::LoadU32(base, address + 5036))); }
  void set_car_scene_pool7_count_bits(uint32_t v) const { if (auto* d = direct()) { d->car_scene_pool7_count = v; return; } if (memory::IsHandle(address + 5036)) memory::OwnerStore(address + 5036, 4, v); else guest::StoreU32(base, address + 5036, v); }
  uint32_t car_scene_pool7_count() const { if (auto* d = direct()) return d->car_scene_pool7_count; return uint32_t((memory::IsHandle(address + 5036) ? uint32_t(memory::OwnerLoad(address + 5036, 4)) : guest::LoadU32(base, address + 5036))); }
  void set_car_scene_pool7_count(uint32_t v) const { if (auto* d = direct()) { d->car_scene_pool7_count = v; return; } if (memory::IsHandle(address + 5036)) memory::OwnerStore(address + 5036, 4, v); else guest::StoreU32(base, address + 5036, v); }
};

// file; measure: ScenePools::CreateA.r4
struct ScenePoolsCreateRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  uint32_t model_set() const { return guest::LoadU32(base, address + 0); }
  void set_model_set(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t model_set_address() const { return address + 0; }
  // pool 0
  uint32_t car_models() const { return guest::LoadU32(base, address + 4); }
  void set_car_models(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t car_models_address() const { return address + 4; }
  // pool 1
  uint32_t car_parts() const { return guest::LoadU32(base, address + 8); }
  void set_car_parts(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t car_parts_address() const { return address + 8; }
  // pool 3's count is +12 + +16 + +32
  uint32_t interiors() const { return guest::LoadU32(base, address + 12); }
  void set_interiors(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t interiors_address() const { return address + 12; }
  // pool 2
  uint32_t cockpits() const { return guest::LoadU32(base, address + 16); }
  void set_cockpits(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t cockpits_address() const { return address + 16; }
  // pool 4
  uint32_t pool4() const { return guest::LoadU32(base, address + 20); }
  void set_pool4(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t pool4_address() const { return address + 20; }
  // pool 5's count is +24 + +32 + +28
  uint32_t wheels() const { return guest::LoadU32(base, address + 24); }
  void set_wheels(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t wheels_address() const { return address + 24; }
  // added to the wheels' count and to the set's +36
  uint32_t extra_wheels() const { return guest::LoadU32(base, address + 28); }
  void set_extra_wheels(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t extra_wheels_address() const { return address + 28; }
  // pool 6; added to the interiors' and the wheels' counts
  uint32_t traffic() const { return guest::LoadU32(base, address + 32); }
  void set_traffic(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t traffic_address() const { return address + 32; }
};

// embedded; measure: PoolNode, PoolNode::PoolNode.r4.+4, PoolNode.+4.+8.+7, PoolNode.+8.+12.+0, PoolNode.+8.+12.+4
struct PoolNode : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  static constexpr uint32_t kVtable = 0x8206CCB0;
  // 0x8206CCB0 for the node itself; each class's own, then 0x82004F90 before the node's destructor
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = PoolList>
  T list() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_list(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t list_address() const { return address + 4; }
  template <typename T = PoolNode>
  T next() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_address() const { return address + 8; }
  template <typename T = PoolNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t previous_address() const { return address + 12; }
  // the pool object's: 0 free, 3 in use
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // the pool object's index in its pool (-1 from the constructors)
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
};

// embedded; measure: PoolNode::PoolNode.r4, PoolNode.+8.+12
struct PoolList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = PoolNode>
  T first() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  template <typename T = PoolNode>
  T last() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_last(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t last_address() const { return address + 4; }
  uint32_t count() const { return guest::LoadU32(base, address + 8); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t count_address() const { return address + 8; }
};

// file; measure: NameMatches.r3, NameMatches.r4, Cockpit::OnFileLoaded.r6.+292, TrafficCar::OnFileLoaded.r6.+3860
struct SceneLoadersName : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  // NUL-terminated
  uint32_t text_address() const { return address + 0; }
};

// allocated; measure: CarParts, SceneModel.+5060.+104, CarParts::OnPartLoadedA.r6, CarParts::OnPartLoadedB.r6
#pragma pack(push, 1)
struct CarPartsData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint8_t record[60] = {};  // +28, guest order
  uint8_t pad_88[8] = {};
  uint8_t loads[105904] = {};  // +96, guest order
  uint32_t first_set = 0;  // +106000
  uint8_t pad_106004[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarPartsData) == 106016);
inline constexpr memory::LayoutField kCarPartsFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {106000, 4}};
inline constexpr memory::Layout kCarPartsLayout = {kCarPartsFields, 8};

struct CarParts : View {
  using View::View;
  static constexpr uint32_t kSize = 106016;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82005094;
  CarPartsData* data() const { return static_cast<CarPartsData*>(memory::HandleObject(address)); }
  static CarParts New(uint8_t* base) {
    return CarParts(base, memory::MakeHandle(new CarPartsData(), sizeof(CarPartsData), "CarParts", &kCarPartsLayout));
  }
  template <uint32_t Stride = kSize>
  static CarParts NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarPartsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarPartsData*>(::operator new(sizeof(CarPartsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarPartsData();
    return CarParts(base, memory::MakeHandle(p, sizeof(CarPartsData) * count, "CarParts", &kCarPartsLayout, sizeof(CarPartsData)));
  }
  CarParts at(uint32_t i) const { return CarParts(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarPartsData* direct() const { return static_cast<CarPartsData*>(memory::HandleOwn(address, &kCarPartsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarPartsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarPartsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82005094 (the pool node's 0x82004F90 while it's torn down)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the PoolNode's list (its pool's, in the first set)
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the PoolNode's links
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the pool object's: 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // its index in the pool
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  template <typename T = CarModel>
  T car_model() const { return T(base, car_model_bits()); }
  void set_car_model(const View& w) const { set_car_model_bits(w.address); }
  void set_car_model(uint32_t v) const { set_car_model_bits(v); }
  // a copy of its record (CarParts::LoadSetB reads +28 to +76)
  uint32_t record_address() const { return address + 28; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 28 : guest::Pointer(base, address + 28); }
  // the loads (CarPartsLoads)
  uint32_t loads_address() const { return address + 96; }
  uint8_t* loads() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 96 : guest::Pointer(base, address + 96); }
  // 216 LoadSlots of 24 bytes (+116 + 24i their states); set b uses the first 17
  uint32_t slots_address() const { return address + 96; }
  uint8_t* slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 96 : guest::Pointer(base, address + 96); }
  // 216 PartModels of 464 bytes (the measure's +412 is a model's shape count, +5328 the first's +48, its texture provider)
  uint32_t part_models_address() const { return address + 5280; }
  uint8_t* part_models() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 5280 : guest::Pointer(base, address + 5280); }
  uint32_t first_set_address() const { return address + 106000; }
  uint32_t first_set_bits() const { if (auto* d = direct()) return d->first_set; return uint32_t((memory::IsHandle(address + 106000) ? uint32_t(memory::OwnerLoad(address + 106000, 4)) : guest::LoadU32(base, address + 106000))); }
  void set_first_set_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set = v; return; } if (memory::IsHandle(address + 106000)) memory::OwnerStore(address + 106000, 4, v); else guest::StoreU32(base, address + 106000, v); }
  template <typename T = ScenePools>
  T first_set() const { return T(base, first_set_bits()); }
  void set_first_set(const View& w) const { set_first_set_bits(w.address); }
  void set_first_set(uint32_t v) const { set_first_set_bits(v); }
};

// embedded; measure: CarParts@+96
struct CarPartsLoads : View {
  using View::View;
  static constexpr uint32_t kSize = 105904;
  // 216 LoadSlots
  uint32_t slots_address() const { return address + 0; }
  // 216 PartModels of 464 bytes
  uint32_t part_models_address() const { return address + 5184; }
  // 30 float4s, w (+105436 + 16i) 1.0 from the constructor
  uint32_t points_address() const { return address + 105424; }
};

// allocated; measure: Cockpit, Cockpit::OnFileLoaded.r6
#pragma pack(push, 1)
struct CockpitData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint8_t pad_28[20] = {};
  uint32_t texture_number = 0;  // +48
  uint8_t pad_52[128] = {};
  uint32_t node_file_request = 0;  // +180
  uint8_t pad_184[8] = {};
  uint32_t node_file_buffer = 0;  // +192
  uint32_t node_file_state = 0;  // +196
  uint8_t pad_200[92] = {};
  uint32_t skeleton_node_names = 0;  // +292
  uint8_t pad_296[60] = {};
  uint32_t skeleton_node_matrices = 0;  // +356
  uint8_t pad_360[44] = {};
  uint32_t xpr_slot_request = 0;  // +404
  uint8_t pad_408[12] = {};
  uint32_t xpr_slot_state = 0;  // +420
  uint8_t pad_424[24] = {};
  uint8_t xprs[48] = {};  // +448, guest order
  uint32_t provider_vtable = 0;  // +496
  uint8_t pad_500[40] = {};
  uint32_t part_slot_request = 0;  // +540
  uint8_t pad_544[8] = {};
  uint32_t part_slot_buffer = 0;  // +552
  uint32_t part_slot_state = 0;  // +556
  uint8_t pad_560[176] = {};
  uint32_t part_provider = 0;  // +736
  uint8_t pad_740[400] = {};
  uint32_t part_texture = 0;  // +1140
  uint8_t pad_1144[2328] = {};
  uint32_t accessory_node = 0;  // +3472
  uint8_t pad_3476[24] = {};
  float accessory_point_w = 0;  // +3500
  uint32_t texture = 0;  // +3504
  uint32_t first_set = 0;  // +3508
  uint8_t pad_3512[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(CockpitData) == 3520);
inline constexpr memory::LayoutField kCockpitFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {48, 4}, {180, 4}, {192, 4}, {196, 4}, {292, 4}, {356, 4}, {404, 4}, {420, 4}, {496, 4}, {540, 4}, {552, 4}, {556, 4}, {736, 4}, {1140, 4}, {3472, 4}, {3500, 4}, {3504, 4}, {3508, 4}};
inline constexpr memory::Layout kCockpitLayout = {kCockpitFields, 25};

struct Cockpit : View {
  using View::View;
  static constexpr uint32_t kSize = 3520;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82005020;
  CockpitData* data() const { return static_cast<CockpitData*>(memory::HandleObject(address)); }
  static Cockpit New(uint8_t* base) {
    return Cockpit(base, memory::MakeHandle(new CockpitData(), sizeof(CockpitData), "Cockpit", &kCockpitLayout));
  }
  template <uint32_t Stride = kSize>
  static Cockpit NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CockpitData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CockpitData*>(::operator new(sizeof(CockpitData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CockpitData();
    return Cockpit(base, memory::MakeHandle(p, sizeof(CockpitData) * count, "Cockpit", &kCockpitLayout, sizeof(CockpitData)));
  }
  Cockpit at(uint32_t i) const { return Cockpit(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CockpitData* direct() const { return static_cast<CockpitData*>(memory::HandleOwn(address, &kCockpitLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCockpitLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCockpitLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82005020 (the pool node's 0x82004F90 while it's torn down)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the PoolNode's list (its pool's, in the first set)
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the PoolNode's links
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the pool object's: 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // its index in the pool
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // its +32 the block
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  template <typename T = CarModel>
  T car_model() const { return T(base, car_model_bits()); }
  void set_car_model(const View& w) const { set_car_model_bits(w.address); }
  void set_car_model(uint32_t v) const { set_car_model_bits(v); }
  // a copy of its 144-byte record
  uint32_t record_address() const { return address + 28; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 28 : guest::Pointer(base, address + 28); }
  // the record's +20: 0 names the texture "%s_inte_on", else "%s_inte_on_0%d" with it
  uint32_t texture_number_address() const { return address + 48; }
  uint32_t texture_number_bits() const { if (auto* d = direct()) return d->texture_number; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_texture_number_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_number = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  uint32_t texture_number() const { if (auto* d = direct()) return d->texture_number; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_texture_number(uint32_t v) const { if (auto* d = direct()) { d->texture_number = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  // the loads (CockpitLoads: CarModelLoads' members, then six part models from +512)
  uint32_t loads_address() const { return address + 176; }
  uint8_t* loads() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 176 : guest::Pointer(base, address + 176); }
  // the node file's LoadSlot
  uint32_t node_file_slot_address() const { return address + 176; }
  uint8_t* node_file_slot() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 176 : guest::Pointer(base, address + 176); }
  uint32_t node_file_request_address() const { return address + 180; }
  uint32_t node_file_request_bits() const { if (auto* d = direct()) return d->node_file_request; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(base, address + 180))); }
  void set_node_file_request_bits(uint32_t v) const { if (auto* d = direct()) { d->node_file_request = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(base, address + 180, v); }
  uint32_t node_file_request() const { if (auto* d = direct()) return d->node_file_request; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(base, address + 180))); }
  void set_node_file_request(uint32_t v) const { if (auto* d = direct()) { d->node_file_request = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(base, address + 180, v); }
  uint32_t node_file_buffer_address() const { return address + 192; }
  uint32_t node_file_buffer_bits() const { if (auto* d = direct()) return d->node_file_buffer; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(base, address + 192))); }
  void set_node_file_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->node_file_buffer = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(base, address + 192, v); }
  uint32_t node_file_buffer() const { if (auto* d = direct()) return d->node_file_buffer; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(base, address + 192))); }
  void set_node_file_buffer(uint32_t v) const { if (auto* d = direct()) { d->node_file_buffer = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(base, address + 192, v); }
  // 2 loading, 3 arrived
  uint32_t node_file_state_address() const { return address + 196; }
  uint32_t node_file_state_bits() const { if (auto* d = direct()) return d->node_file_state; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(base, address + 196))); }
  void set_node_file_state_bits(uint32_t v) const { if (auto* d = direct()) { d->node_file_state = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(base, address + 196, v); }
  uint32_t node_file_state() const { if (auto* d = direct()) return d->node_file_state; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(base, address + 196))); }
  void set_node_file_state(uint32_t v) const { if (auto* d = direct()) { d->node_file_state = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(base, address + 196, v); }
  // the node file's Skeleton (+176 its node values)
  uint32_t skeleton_address() const { return address + 208; }
  uint8_t* skeleton() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 208 : guest::Pointer(base, address + 208); }
  // the skeleton's +84: 160-byte names
  uint32_t skeleton_node_names_address() const { return address + 292; }
  uint32_t skeleton_node_names_bits() const { if (auto* d = direct()) return d->skeleton_node_names; return uint32_t((memory::IsHandle(address + 292) ? uint32_t(memory::OwnerLoad(address + 292, 4)) : guest::LoadU32(base, address + 292))); }
  void set_skeleton_node_names_bits(uint32_t v) const { if (auto* d = direct()) { d->skeleton_node_names = v; return; } if (memory::IsHandle(address + 292)) memory::OwnerStore(address + 292, 4, v); else guest::StoreU32(base, address + 292, v); }
  template <typename T = SceneLoadersName>
  T skeleton_node_names() const { return T(base, skeleton_node_names_bits()); }
  void set_skeleton_node_names(const View& w) const { set_skeleton_node_names_bits(w.address); }
  void set_skeleton_node_names(uint32_t v) const { set_skeleton_node_names_bits(v); }
  // the skeleton's +148: 64 bytes a node
  uint32_t skeleton_node_matrices_address() const { return address + 356; }
  uint32_t skeleton_node_matrices_bits() const { if (auto* d = direct()) return d->skeleton_node_matrices; return uint32_t((memory::IsHandle(address + 356) ? uint32_t(memory::OwnerLoad(address + 356, 4)) : guest::LoadU32(base, address + 356))); }
  void set_skeleton_node_matrices_bits(uint32_t v) const { if (auto* d = direct()) { d->skeleton_node_matrices = v; return; } if (memory::IsHandle(address + 356)) memory::OwnerStore(address + 356, 4, v); else guest::StoreU32(base, address + 356, v); }
  template <typename T = SkeletonMatrix>
  T skeleton_node_matrices() const { return T(base, skeleton_node_matrices_bits()); }
  void set_skeleton_node_matrices(const View& w) const { set_skeleton_node_matrices_bits(w.address); }
  void set_skeleton_node_matrices(uint32_t v) const { set_skeleton_node_matrices_bits(v); }
  // the two XPR2s' LoadSlots
  uint32_t xpr_slots_address() const { return address + 400; }
  uint8_t* xpr_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 400 : guest::Pointer(base, address + 400); }
  // the first's request (+24i)
  uint32_t xpr_slot_request_address() const { return address + 404; }
  uint32_t xpr_slot_request_bits() const { if (auto* d = direct()) return d->xpr_slot_request; return uint32_t((memory::IsHandle(address + 404) ? uint32_t(memory::OwnerLoad(address + 404, 4)) : guest::LoadU32(base, address + 404))); }
  void set_xpr_slot_request_bits(uint32_t v) const { if (auto* d = direct()) { d->xpr_slot_request = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 4, v); else guest::StoreU32(base, address + 404, v); }
  uint32_t xpr_slot_request() const { if (auto* d = direct()) return d->xpr_slot_request; return uint32_t((memory::IsHandle(address + 404) ? uint32_t(memory::OwnerLoad(address + 404, 4)) : guest::LoadU32(base, address + 404))); }
  void set_xpr_slot_request(uint32_t v) const { if (auto* d = direct()) { d->xpr_slot_request = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 4, v); else guest::StoreU32(base, address + 404, v); }
  // the first's state (+24i)
  uint32_t xpr_slot_state_address() const { return address + 420; }
  uint32_t xpr_slot_state_bits() const { if (auto* d = direct()) return d->xpr_slot_state; return uint32_t((memory::IsHandle(address + 420) ? uint32_t(memory::OwnerLoad(address + 420, 4)) : guest::LoadU32(base, address + 420))); }
  void set_xpr_slot_state_bits(uint32_t v) const { if (auto* d = direct()) { d->xpr_slot_state = v; return; } if (memory::IsHandle(address + 420)) memory::OwnerStore(address + 420, 4, v); else guest::StoreU32(base, address + 420, v); }
  uint32_t xpr_slot_state() const { if (auto* d = direct()) return d->xpr_slot_state; return uint32_t((memory::IsHandle(address + 420) ? uint32_t(memory::OwnerLoad(address + 420, 4)) : guest::LoadU32(base, address + 420))); }
  void set_xpr_slot_state(uint32_t v) const { if (auto* d = direct()) { d->xpr_slot_state = v; return; } if (memory::IsHandle(address + 420)) memory::OwnerStore(address + 420, 4, v); else guest::StoreU32(base, address + 420, v); }
  // two XPR objects, 24 bytes each
  uint32_t xprs_address() const { return address + 448; }
  uint8_t* xprs() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 448 : guest::Pointer(base, address + 448); }
  // two texture providers, 20 bytes each
  uint32_t providers_address() const { return address + 496; }
  uint8_t* providers() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 496 : guest::Pointer(base, address + 496); }
  // the first provider's vtable word
  uint32_t provider_vtable_address() const { return address + 496; }
  uint32_t provider_vtable_bits() const { if (auto* d = direct()) return d->provider_vtable; return uint32_t((memory::IsHandle(address + 496) ? uint32_t(memory::OwnerLoad(address + 496, 4)) : guest::LoadU32(base, address + 496))); }
  void set_provider_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->provider_vtable = v; return; } if (memory::IsHandle(address + 496)) memory::OwnerStore(address + 496, 4, v); else guest::StoreU32(base, address + 496, v); }
  template <typename T = TextureProviderVtable>
  T provider_vtable() const { return T(base, provider_vtable_bits()); }
  void set_provider_vtable(const View& w) const { set_provider_vtable_bits(w.address); }
  void set_provider_vtable(uint32_t v) const { set_provider_vtable_bits(v); }
  // the six parts' LoadSlots
  uint32_t part_slots_address() const { return address + 536; }
  uint8_t* part_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 536 : guest::Pointer(base, address + 536); }
  // the first's request (+24i)
  uint32_t part_slot_request_address() const { return address + 540; }
  uint32_t part_slot_request_bits() const { if (auto* d = direct()) return d->part_slot_request; return uint32_t((memory::IsHandle(address + 540) ? uint32_t(memory::OwnerLoad(address + 540, 4)) : guest::LoadU32(base, address + 540))); }
  void set_part_slot_request_bits(uint32_t v) const { if (auto* d = direct()) { d->part_slot_request = v; return; } if (memory::IsHandle(address + 540)) memory::OwnerStore(address + 540, 4, v); else guest::StoreU32(base, address + 540, v); }
  uint32_t part_slot_request() const { if (auto* d = direct()) return d->part_slot_request; return uint32_t((memory::IsHandle(address + 540) ? uint32_t(memory::OwnerLoad(address + 540, 4)) : guest::LoadU32(base, address + 540))); }
  void set_part_slot_request(uint32_t v) const { if (auto* d = direct()) { d->part_slot_request = v; return; } if (memory::IsHandle(address + 540)) memory::OwnerStore(address + 540, 4, v); else guest::StoreU32(base, address + 540, v); }
  // the first's buffer (+24i)
  uint32_t part_slot_buffer_address() const { return address + 552; }
  uint32_t part_slot_buffer_bits() const { if (auto* d = direct()) return d->part_slot_buffer; return uint32_t((memory::IsHandle(address + 552) ? uint32_t(memory::OwnerLoad(address + 552, 4)) : guest::LoadU32(base, address + 552))); }
  void set_part_slot_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->part_slot_buffer = v; return; } if (memory::IsHandle(address + 552)) memory::OwnerStore(address + 552, 4, v); else guest::StoreU32(base, address + 552, v); }
  uint32_t part_slot_buffer() const { if (auto* d = direct()) return d->part_slot_buffer; return uint32_t((memory::IsHandle(address + 552) ? uint32_t(memory::OwnerLoad(address + 552, 4)) : guest::LoadU32(base, address + 552))); }
  void set_part_slot_buffer(uint32_t v) const { if (auto* d = direct()) { d->part_slot_buffer = v; return; } if (memory::IsHandle(address + 552)) memory::OwnerStore(address + 552, 4, v); else guest::StoreU32(base, address + 552, v); }
  // the first's state (+24i)
  uint32_t part_slot_state_address() const { return address + 556; }
  uint32_t part_slot_state_bits() const { if (auto* d = direct()) return d->part_slot_state; return uint32_t((memory::IsHandle(address + 556) ? uint32_t(memory::OwnerLoad(address + 556, 4)) : guest::LoadU32(base, address + 556))); }
  void set_part_slot_state_bits(uint32_t v) const { if (auto* d = direct()) { d->part_slot_state = v; return; } if (memory::IsHandle(address + 556)) memory::OwnerStore(address + 556, 4, v); else guest::StoreU32(base, address + 556, v); }
  uint32_t part_slot_state() const { if (auto* d = direct()) return d->part_slot_state; return uint32_t((memory::IsHandle(address + 556) ? uint32_t(memory::OwnerLoad(address + 556, 4)) : guest::LoadU32(base, address + 556))); }
  void set_part_slot_state(uint32_t v) const { if (auto* d = direct()) { d->part_slot_state = v; return; } if (memory::IsHandle(address + 556)) memory::OwnerStore(address + 556, 4, v); else guest::StoreU32(base, address + 556, v); }
  // six PartModels of 464 bytes
  uint32_t part_models_address() const { return address + 688; }
  uint8_t* part_models() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 688 : guest::Pointer(base, address + 688); }
  // the first part model's +48, its texture provider (+496; +464i)
  uint32_t part_provider_address() const { return address + 736; }
  uint32_t part_provider_bits() const { if (auto* d = direct()) return d->part_provider; return uint32_t((memory::IsHandle(address + 736) ? uint32_t(memory::OwnerLoad(address + 736, 4)) : guest::LoadU32(base, address + 736))); }
  void set_part_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->part_provider = v; return; } if (memory::IsHandle(address + 736)) memory::OwnerStore(address + 736, 4, v); else guest::StoreU32(base, address + 736, v); }
  template <typename T = TextureProvider>
  T part_provider() const { return T(base, part_provider_bits()); }
  void set_part_provider(const View& w) const { set_part_provider_bits(w.address); }
  void set_part_provider(uint32_t v) const { set_part_provider_bits(v); }
  // the first part model's +452, the texture from the formatted name (+464i)
  uint32_t part_texture_address() const { return address + 1140; }
  uint32_t part_texture_bits() const { if (auto* d = direct()) return d->part_texture; return uint32_t((memory::IsHandle(address + 1140) ? uint32_t(memory::OwnerLoad(address + 1140, 4)) : guest::LoadU32(base, address + 1140))); }
  void set_part_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->part_texture = v; return; } if (memory::IsHandle(address + 1140)) memory::OwnerStore(address + 1140, 4, v); else guest::StoreU32(base, address + 1140, v); }
  uint32_t part_texture() const { if (auto* d = direct()) return d->part_texture; return uint32_t((memory::IsHandle(address + 1140) ? uint32_t(memory::OwnerLoad(address + 1140, 4)) : guest::LoadU32(base, address + 1140))); }
  void set_part_texture(uint32_t v) const { if (auto* d = direct()) { d->part_texture = v; return; } if (memory::IsHandle(address + 1140)) memory::OwnerStore(address + 1140, 4, v); else guest::StoreU32(base, address + 1140, v); }
  // the matrix of the node named "acce_null" (0x82004FDC), probably the accessory point
  uint32_t accessory_node_address() const { return address + 3472; }
  uint32_t accessory_node_bits() const { if (auto* d = direct()) return d->accessory_node; return uint32_t((memory::IsHandle(address + 3472) ? uint32_t(memory::OwnerLoad(address + 3472, 4)) : guest::LoadU32(base, address + 3472))); }
  void set_accessory_node_bits(uint32_t v) const { if (auto* d = direct()) { d->accessory_node = v; return; } if (memory::IsHandle(address + 3472)) memory::OwnerStore(address + 3472, 4, v); else guest::StoreU32(base, address + 3472, v); }
  template <typename T = SkeletonMatrix>
  T accessory_node() const { return T(base, accessory_node_bits()); }
  void set_accessory_node(const View& w) const { set_accessory_node_bits(w.address); }
  void set_accessory_node(uint32_t v) const { set_accessory_node_bits(v); }
  // its position: row 3, x, y, z times 0.1, w 1.0, turned a quarter about y
  uint32_t accessory_point_address() const { return address + 3488; }
  uint8_t* accessory_point() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3488 : guest::Pointer(base, address + 3488); }
  uint32_t accessory_point_w_address() const { return address + 3500; }
  uint32_t accessory_point_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->accessory_point_w); return uint32_t((memory::IsHandle(address + 3500) ? uint32_t(memory::OwnerLoad(address + 3500, 4)) : guest::LoadU32(base, address + 3500))); }
  void set_accessory_point_w_bits(uint32_t v) const { if (auto* d = direct()) { d->accessory_point_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 3500)) memory::OwnerStore(address + 3500, 4, v); else guest::StoreU32(base, address + 3500, v); }
  float accessory_point_w() const { return std::bit_cast<float>(accessory_point_w_bits()); }
  void set_accessory_point_w(float v) const { set_accessory_point_w_bits(std::bit_cast<uint32_t>(v)); }
  // a texture from the first XPR by the formatted name
  uint32_t texture_address() const { return address + 3504; }
  uint32_t texture_bits() const { if (auto* d = direct()) return d->texture; return uint32_t((memory::IsHandle(address + 3504) ? uint32_t(memory::OwnerLoad(address + 3504, 4)) : guest::LoadU32(base, address + 3504))); }
  void set_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->texture = v; return; } if (memory::IsHandle(address + 3504)) memory::OwnerStore(address + 3504, 4, v); else guest::StoreU32(base, address + 3504, v); }
  uint32_t texture() const { if (auto* d = direct()) return d->texture; return uint32_t((memory::IsHandle(address + 3504) ? uint32_t(memory::OwnerLoad(address + 3504, 4)) : guest::LoadU32(base, address + 3504))); }
  void set_texture(uint32_t v) const { if (auto* d = direct()) { d->texture = v; return; } if (memory::IsHandle(address + 3504)) memory::OwnerStore(address + 3504, 4, v); else guest::StoreU32(base, address + 3504, v); }
  uint32_t first_set_address() const { return address + 3508; }
  uint32_t first_set_bits() const { if (auto* d = direct()) return d->first_set; return uint32_t((memory::IsHandle(address + 3508) ? uint32_t(memory::OwnerLoad(address + 3508, 4)) : guest::LoadU32(base, address + 3508))); }
  void set_first_set_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set = v; return; } if (memory::IsHandle(address + 3508)) memory::OwnerStore(address + 3508, 4, v); else guest::StoreU32(base, address + 3508, v); }
  template <typename T = ScenePools>
  T first_set() const { return T(base, first_set_bits()); }
  void set_first_set(const View& w) const { set_first_set_bits(w.address); }
  void set_first_set(uint32_t v) const { set_first_set_bits(v); }
};

// allocated; measure: Interior, Interior::OnPartLoaded.r6
#pragma pack(push, 1)
struct InteriorData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint8_t pad_28[112] = {};
  uint32_t accessory = 0;  // +140
  uint32_t accessory_arg = 0;  // +144
  uint8_t pad_148[16] = {};
  uint32_t part_slot_request = 0;  // +164
  uint8_t pad_168[12] = {};
  uint32_t part_slot_state = 0;  // +180
  uint8_t pad_184[120] = {};
  uint8_t part_models[2784] = {};  // +304, guest order
  uint32_t cockpit = 0;  // +3088
  uint32_t first_set = 0;  // +3092
  uint8_t pad_3096[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(InteriorData) == 3104);
inline constexpr memory::LayoutField kInteriorFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {140, 4}, {144, 4}, {164, 4}, {180, 4}, {3088, 4}, {3092, 4}};
inline constexpr memory::Layout kInteriorLayout = {kInteriorFields, 13};

struct Interior : View {
  using View::View;
  static constexpr uint32_t kSize = 3104;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x8200509C;
  InteriorData* data() const { return static_cast<InteriorData*>(memory::HandleObject(address)); }
  static Interior New(uint8_t* base) {
    return Interior(base, memory::MakeHandle(new InteriorData(), sizeof(InteriorData), "Interior", &kInteriorLayout));
  }
  template <uint32_t Stride = kSize>
  static Interior NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(InteriorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<InteriorData*>(::operator new(sizeof(InteriorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) InteriorData();
    return Interior(base, memory::MakeHandle(p, sizeof(InteriorData) * count, "Interior", &kInteriorLayout, sizeof(InteriorData)));
  }
  Interior at(uint32_t i) const { return Interior(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  InteriorData* direct() const { return static_cast<InteriorData*>(memory::HandleOwn(address, &kInteriorLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kInteriorLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInteriorLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x8200509C (the pool node's 0x82004F90 while it's torn down)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the PoolNode's list (its pool's, in the first set)
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the PoolNode's links
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the pool object's: 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // its index in the pool
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  template <typename T = CarModel>
  T car_model() const { return T(base, car_model_bits()); }
  void set_car_model(const View& w) const { set_car_model_bits(w.address); }
  void set_car_model(uint32_t v) const { set_car_model_bits(v); }
  // a copy of its 120-byte record
  uint32_t record_address() const { return address + 28; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 28 : guest::Pointer(base, address + 28); }
  // the record's +112: the accessory picked (set b)
  uint32_t accessory_address() const { return address + 140; }
  uint32_t accessory_bits() const { if (auto* d = direct()) return d->accessory; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(base, address + 140))); }
  void set_accessory_bits(uint32_t v) const { if (auto* d = direct()) { d->accessory = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(base, address + 140, v); }
  uint32_t accessory() const { if (auto* d = direct()) return d->accessory; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(base, address + 140))); }
  void set_accessory(uint32_t v) const { if (auto* d = direct()) { d->accessory = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(base, address + 140, v); }
  // the record's +116
  uint32_t accessory_arg_address() const { return address + 144; }
  uint32_t accessory_arg_bits() const { if (auto* d = direct()) return d->accessory_arg; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(base, address + 144))); }
  void set_accessory_arg_bits(uint32_t v) const { if (auto* d = direct()) { d->accessory_arg = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(base, address + 144, v); }
  uint32_t accessory_arg() const { if (auto* d = direct()) return d->accessory_arg; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(base, address + 144))); }
  void set_accessory_arg(uint32_t v) const { if (auto* d = direct()) { d->accessory_arg = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(base, address + 144, v); }
  // the six parts' LoadSlots
  uint32_t part_slots_address() const { return address + 160; }
  uint8_t* part_slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 160 : guest::Pointer(base, address + 160); }
  // the first's request (+24i)
  uint32_t part_slot_request_address() const { return address + 164; }
  uint32_t part_slot_request_bits() const { if (auto* d = direct()) return d->part_slot_request; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(base, address + 164))); }
  void set_part_slot_request_bits(uint32_t v) const { if (auto* d = direct()) { d->part_slot_request = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(base, address + 164, v); }
  uint32_t part_slot_request() const { if (auto* d = direct()) return d->part_slot_request; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(base, address + 164))); }
  void set_part_slot_request(uint32_t v) const { if (auto* d = direct()) { d->part_slot_request = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(base, address + 164, v); }
  // the first's state (+24i)
  uint32_t part_slot_state_address() const { return address + 180; }
  uint32_t part_slot_state_bits() const { if (auto* d = direct()) return d->part_slot_state; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(base, address + 180))); }
  void set_part_slot_state_bits(uint32_t v) const { if (auto* d = direct()) { d->part_slot_state = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(base, address + 180, v); }
  uint32_t part_slot_state() const { if (auto* d = direct()) return d->part_slot_state; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(base, address + 180))); }
  void set_part_slot_state(uint32_t v) const { if (auto* d = direct()) { d->part_slot_state = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(base, address + 180, v); }
  // six PartModels of 464 bytes
  uint32_t part_models_address() const { return address + 304; }
  uint8_t* part_models() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 304 : guest::Pointer(base, address + 304); }
  // nonzero: the cockpit set (c)
  uint32_t cockpit_address() const { return address + 3088; }
  uint32_t cockpit_bits() const { if (auto* d = direct()) return d->cockpit; return uint32_t((memory::IsHandle(address + 3088) ? uint32_t(memory::OwnerLoad(address + 3088, 4)) : guest::LoadU32(base, address + 3088))); }
  void set_cockpit_bits(uint32_t v) const { if (auto* d = direct()) { d->cockpit = v; return; } if (memory::IsHandle(address + 3088)) memory::OwnerStore(address + 3088, 4, v); else guest::StoreU32(base, address + 3088, v); }
  uint32_t cockpit() const { if (auto* d = direct()) return d->cockpit; return uint32_t((memory::IsHandle(address + 3088) ? uint32_t(memory::OwnerLoad(address + 3088, 4)) : guest::LoadU32(base, address + 3088))); }
  void set_cockpit(uint32_t v) const { if (auto* d = direct()) { d->cockpit = v; return; } if (memory::IsHandle(address + 3088)) memory::OwnerStore(address + 3088, 4, v); else guest::StoreU32(base, address + 3088, v); }
  uint32_t first_set_address() const { return address + 3092; }
  uint32_t first_set_bits() const { if (auto* d = direct()) return d->first_set; return uint32_t((memory::IsHandle(address + 3092) ? uint32_t(memory::OwnerLoad(address + 3092, 4)) : guest::LoadU32(base, address + 3092))); }
  void set_first_set_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set = v; return; } if (memory::IsHandle(address + 3092)) memory::OwnerStore(address + 3092, 4, v); else guest::StoreU32(base, address + 3092, v); }
  template <typename T = ScenePools>
  T first_set() const { return T(base, first_set_bits()); }
  void set_first_set(const View& w) const { set_first_set_bits(w.address); }
  void set_first_set(uint32_t v) const { set_first_set_bits(v); }
};

// allocated; measure: Pool4Object, sub_820D69B8.r6, SceneLights.+13152
#pragma pack(push, 1)
struct Pool4ObjectData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t car_model = 0;  // +24
  uint8_t record[52] = {};  // +28, guest order
  uint8_t pad_80[20] = {};
  uint32_t slot_state = 0;  // +100
  uint8_t pad_104[8] = {};
  uint8_t model[464] = {};  // +112, guest order
  uint32_t first_set = 0;  // +576
  uint8_t pad_580[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(Pool4ObjectData) == 592);
inline constexpr memory::LayoutField kPool4ObjectFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {100, 4}, {576, 4}};
inline constexpr memory::Layout kPool4ObjectLayout = {kPool4ObjectFields, 9};

struct Pool4Object : View {
  using View::View;
  static constexpr uint32_t kSize = 592;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82005064;
  Pool4ObjectData* data() const { return static_cast<Pool4ObjectData*>(memory::HandleObject(address)); }
  static Pool4Object New(uint8_t* base) {
    return Pool4Object(base, memory::MakeHandle(new Pool4ObjectData(), sizeof(Pool4ObjectData), "Pool4Object", &kPool4ObjectLayout));
  }
  template <uint32_t Stride = kSize>
  static Pool4Object NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(Pool4ObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Pool4ObjectData*>(::operator new(sizeof(Pool4ObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Pool4ObjectData();
    return Pool4Object(base, memory::MakeHandle(p, sizeof(Pool4ObjectData) * count, "Pool4Object", &kPool4ObjectLayout, sizeof(Pool4ObjectData)));
  }
  Pool4Object at(uint32_t i) const { return Pool4Object(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  Pool4ObjectData* direct() const { return static_cast<Pool4ObjectData*>(memory::HandleOwn(address, &kPool4ObjectLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kPool4ObjectLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPool4ObjectLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82005064 (the pool node's 0x82004F90 while it's torn down)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the PoolNode's list (its pool's, in the first set)
  uint32_t list_address() const { return address + 4; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t list() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_list(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the PoolNode's links
  uint32_t next_address() const { return address + 8; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t next() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_next(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t previous_address() const { return address + 12; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the pool object's: 3 in use
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // its index in the pool
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t car_model_address() const { return address + 24; }
  uint32_t car_model_bits() const { if (auto* d = direct()) return d->car_model; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_car_model_bits(uint32_t v) const { if (auto* d = direct()) { d->car_model = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  template <typename T = CarModel>
  T car_model() const { return T(base, car_model_bits()); }
  void set_car_model(const View& w) const { set_car_model_bits(w.address); }
  void set_car_model(uint32_t v) const { set_car_model_bits(v); }
  // a copy of its 52-byte record (Pool4Object::SetRecord); the measure's +48 store is SceneLights::Finish's on the model's +48 (a measure fault)
  uint32_t record_address() const { return address + 28; }
  uint8_t* record() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 28 : guest::Pointer(base, address + 28); }
  // the model's LoadSlot
  uint32_t slot_address() const { return address + 80; }
  uint8_t* slot() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 80 : guest::Pointer(base, address + 80); }
  // 2 loading
  uint32_t slot_state_address() const { return address + 100; }
  uint32_t slot_state_bits() const { if (auto* d = direct()) return d->slot_state; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_slot_state_bits(uint32_t v) const { if (auto* d = direct()) { d->slot_state = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  uint32_t slot_state() const { if (auto* d = direct()) return d->slot_state; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_slot_state(uint32_t v) const { if (auto* d = direct()) { d->slot_state = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  // a PartModel
  uint32_t model_address() const { return address + 112; }
  uint8_t* model() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 112 : guest::Pointer(base, address + 112); }
  uint32_t first_set_address() const { return address + 576; }
  uint32_t first_set_bits() const { if (auto* d = direct()) return d->first_set; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(base, address + 576))); }
  void set_first_set_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(base, address + 576, v); }
  template <typename T = ScenePools>
  T first_set() const { return T(base, first_set_bits()); }
  void set_first_set(const View& w) const { set_first_set_bits(w.address); }
  void set_first_set(uint32_t v) const { set_first_set_bits(v); }
};

// embedded; measure: CarModel@+3728
struct CarModelLoads : View {
  using View::View;
  static constexpr uint32_t kSize = 2988;
  // the node file's LoadSlot
  uint32_t node_file_slot_address() const { return address + 0; }
  // a Skeleton (+176 its node values)
  uint32_t skeleton_address() const { return address + 32; }
  // the two XPR2s' LoadSlots
  uint32_t xpr_slots_address() const { return address + 224; }
  // two XPR objects
  uint32_t xprs_address() const { return address + 272; }
  // two texture providers, 20 bytes each
  uint32_t providers_address() const { return address + 320; }
  // the first provider's vtable word
  template <typename T = TextureProviderVtable>
  T provider_vtable() const { return T(base, guest::LoadU32(base, address + 320)); }
  void set_provider_vtable(const View& v) const { guest::StoreU32(base, address + 320, v.address); }
  void set_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t provider_vtable_address() const { return address + 320; }
  // the car model's +4088: 2 loading, 3 done, 0 reset
  uint32_t decals_state() const { return guest::LoadU32(base, address + 360); }
  void set_decals_state(uint32_t v) const { guest::StoreU32(base, address + 360, v); }
  uint32_t decals_state_address() const { return address + 360; }
  // six layers, two 24-byte LoadSlots each (the reset clears the first three)
  uint32_t layer_slots_address() const { return address + 364; }
  // 48 stickers, two LoadSlots each (the reset clears the first six)
  uint32_t sticker_slots_address() const { return address + 652; }
  // the car model's +6684: the vinyl sheet's texture
  uint32_t vinyl_sheet() const { return guest::LoadU32(base, address + 2956); }
  void set_vinyl_sheet(uint32_t v) const { guest::StoreU32(base, address + 2956, v); }
  uint32_t vinyl_sheet_address() const { return address + 2956; }
  // its pixels
  uint32_t vinyl_sheet_data() const { return guest::LoadU32(base, address + 2960); }
  void set_vinyl_sheet_data(uint32_t v) const { guest::StoreU32(base, address + 2960, v); }
  uint32_t vinyl_sheet_data_address() const { return address + 2960; }
  // passed with them (the car model's +6692)
  uint32_t vinyl_sheet_second() const { return guest::LoadU32(base, address + 2964); }
  void set_vinyl_sheet_second(uint32_t v) const { guest::StoreU32(base, address + 2964, v); }
  uint32_t vinyl_sheet_second_address() const { return address + 2964; }
  uint32_t vinyl_sheet_second_data() const { return guest::LoadU32(base, address + 2968); }
  void set_vinyl_sheet_second_data(uint32_t v) const { guest::StoreU32(base, address + 2968, v); }
  uint32_t vinyl_sheet_second_data_address() const { return address + 2968; }
  // the plate's first texture (the car model's +6700)
  uint32_t plate_first() const { return guest::LoadU32(base, address + 2972); }
  void set_plate_first(uint32_t v) const { guest::StoreU32(base, address + 2972, v); }
  uint32_t plate_first_address() const { return address + 2972; }
  uint32_t plate_first_data() const { return guest::LoadU32(base, address + 2976); }
  void set_plate_first_data(uint32_t v) const { guest::StoreU32(base, address + 2976, v); }
  uint32_t plate_first_data_address() const { return address + 2976; }
  // the plate's second texture
  uint32_t plate_second() const { return guest::LoadU32(base, address + 2980); }
  void set_plate_second(uint32_t v) const { guest::StoreU32(base, address + 2980, v); }
  uint32_t plate_second_address() const { return address + 2980; }
  uint32_t plate_second_data() const { return guest::LoadU32(base, address + 2984); }
  void set_plate_second_data(uint32_t v) const { guest::StoreU32(base, address + 2984, v); }
  uint32_t plate_second_data_address() const { return address + 2984; }
};

// embedded; measure: Wheels@+192
struct WheelsLoads : View {
  using View::View;
  static constexpr uint32_t kSize = 3072;
  uint32_t tire_slots_address() const { return address + 0; }
  // two PartModels
  uint32_t tires_address() const { return address + 48; }
  // rims or brakes
  uint32_t rim_slots_address() const { return address + 976; }
  uint32_t rims_address() const { return address + 1024; }
  uint32_t xpr_slots_address() const { return address + 1952; }
  uint32_t xprs_address() const { return address + 2000; }
  // two texture providers, 20 bytes each
  uint32_t providers_address() const { return address + 2048; }
  // the front provider's vtable word
  template <typename T = TextureProviderVtable>
  T provider_vtable() const { return T(base, guest::LoadU32(base, address + 2048)); }
  void set_provider_vtable(const View& v) const { guest::StoreU32(base, address + 2048, v.address); }
  void set_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 2048, v); }
  uint32_t provider_vtable_address() const { return address + 2048; }
  uint32_t caliper_slots_address() const { return address + 2088; }
  uint32_t calipers_address() const { return address + 2144; }
};

// embedded; measure: CarParts@+5280
struct CarPartsAt5280 : View {
  using View::View;
  static constexpr uint32_t kSize = 416;
  uint32_t unknown_412() const { return guest::LoadU32(base, address + 412); }
  void set_unknown_412(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t unknown_412_address() const { return address + 412; }
};

// embedded; measure: ScenePools@+52
struct ScenePoolsAt52 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// The image words docs/functions/scene-loaders.md's code uses.
struct SceneLoadersGlobals {
  template <typename T = ScenePools>
  static T first_set(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E40u)); }
  static void set_first_set(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E40u, v.address); }
  static void set_first_set(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E40u, v); }
  static constexpr uint32_t first_set_address() { return 0x826C2E40u; }
  // 1.0's bits
  static uint32_t one(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_one(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0's bits
  static uint32_t zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 0.1
  static float tenth(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_tenth(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t tenth_address() { return 0x82001FBCu; }
  // -pi / 2
  static float quarter_turn(uint8_t* base) { return guest::LoadFloat(base, 0x82002A50u); }
  static void set_quarter_turn(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A50u, v); }
  static constexpr uint32_t quarter_turn_address() { return 0x82002A50u; }
  // "%s%s"
  static constexpr uint32_t two_strings_address() { return 0x82004FE8u; }
  // "%s%s%d"
  static constexpr uint32_t numbered_address() { return 0x82004FA4u; }
  // "_inte_on"
  static constexpr uint32_t inte_on_address() { return 0x82005008u; }
  // "_inte_on_0"
  static constexpr uint32_t inte_on_numbered_address() { return 0x82004FFCu; }
  // "_ex_0"
  static constexpr uint32_t traffic_suffix_address() { return 0x820050ACu; }
  // the traffic body's file name's suffix
  static constexpr uint32_t traffic_file_suffix_address() { return 0x820050B8u; }
  // "acce_null"
  static constexpr uint32_t accessory_node_name_address() { return 0x82004FDCu; }
  // "tire_fl", "tire_fr", "tire_rl", "tire_rr", 8 bytes apart
  static constexpr uint32_t tire_names_address() { return 0x82004FACu; }
  // the cars' names by block (45 pointers)
  static constexpr uint32_t cockpit_names_address() { return 0x826018C8u; }
  // the traffic cars' names by number (pointers)
  static constexpr uint32_t traffic_names_address() { return 0x82601980u; }
};

}  // namespace schema
