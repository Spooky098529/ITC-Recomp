// The views of docs/functions/scene-loaders.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/scene-loaders.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ScenePools, new<140>@sub_820C3C40
struct ScenePools : View {
  using View::View;
  static constexpr uint32_t kSize = 140;
  // the teardown's count (2, ScenePools::StartTeardownA); the measure's store here is AllocPages' (sub_820D0960) into its out word, the set's +52
  uint32_t teardown_count() const { return guest::LoadU32(base, address + 0); }
  void set_teardown_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t teardown_count_address() const { return address + 0; }
  // 0 when made, 3 once filled, 4 tearing down
  uint32_t state() const { return guest::LoadU32(base, address + 4); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t state_address() const { return address + 4; }
  // 0 set a, 1 set b (the loading objects read it as [[their first set] + 8])
  uint32_t model_set() const { return guest::LoadU32(base, address + 8); }
  void set_model_set(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t model_set_address() const { return address + 8; }
  // pool 0's count
  uint32_t car_model_count() const { return guest::LoadU32(base, address + 12); }
  void set_car_model_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t car_model_count_address() const { return address + 12; }
  // pool 1's
  uint32_t car_parts_count() const { return guest::LoadU32(base, address + 16); }
  void set_car_parts_count(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t car_parts_count_address() const { return address + 16; }
  // pool 3's
  uint32_t interior_count() const { return guest::LoadU32(base, address + 20); }
  void set_interior_count(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t interior_count_address() const { return address + 20; }
  // pool 2's
  uint32_t cockpit_count() const { return guest::LoadU32(base, address + 24); }
  void set_cockpit_count(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t cockpit_count_address() const { return address + 24; }
  // pool 4's
  uint32_t pool4_count() const { return guest::LoadU32(base, address + 28); }
  void set_pool4_count(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t pool4_count_address() const { return address + 28; }
  // pool 5's
  uint32_t wheels_count() const { return guest::LoadU32(base, address + 32); }
  void set_wheels_count(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t wheels_count_address() const { return address + 32; }
  // the record's +32 + +28, not used as a count
  uint32_t unused_sum() const { return guest::LoadU32(base, address + 36); }
  void set_unused_sum(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unused_sum_address() const { return address + 36; }
  // pool 6's
  uint32_t traffic_count() const { return guest::LoadU32(base, address + 40); }
  void set_traffic_count(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t traffic_count_address() const { return address + 40; }
  // 4 x (+12 + +40) records of 20 bytes
  template <typename T = CarModelsDeferredRelease>
  T deferred_releases() const { return T(base, guest::LoadU32(base, address + 52)); }
  void set_deferred_releases(const View& v) const { guest::StoreU32(base, address + 52, v.address); }
  void set_deferred_releases(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t deferred_releases_address() const { return address + 52; }
  // seven lists of PoolNodes (PoolList), 12 bytes each, by pool
  uint32_t pools_address() const { return address + 56; }
  // a measure fault: ScenePools::FindModel's r3 is the car scene ([0x826C2E44]), whose +5036 is pool 7's count
  uint32_t car_scene_pool7_count() const { return guest::LoadU32(base, address + 5036); }
  void set_car_scene_pool7_count(uint32_t v) const { guest::StoreU32(base, address + 5036, v); }
  uint32_t car_scene_pool7_count_address() const { return address + 5036; }
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

// allocated; measure: CarParts, SceneModel.+5060.+104
struct CarParts : View {
  using View::View;
  static constexpr uint32_t kSize = 106016;
  static constexpr uint32_t kVtable = 0x82005094;
  // 0x82005094 (the pool node's 0x82004F90 while it's torn down)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the pool object's: 3 in use
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  template <typename T = CarModel>
  T car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // a copy of its record (CarParts::LoadSetB reads +28 to +76)
  uint32_t record_address() const { return address + 28; }
  // the loads (CarPartsLoads)
  uint32_t loads_address() const { return address + 96; }
  // 216 LoadSlots of 24 bytes (+116 + 24i their states); set b uses the first 17
  uint32_t slots_address() const { return address + 96; }
  // 216 PartModels of 464 bytes (the measure's +412 is a model's shape count, +5328 the first's +48, its texture provider)
  uint32_t part_models_address() const { return address + 5280; }
  template <typename T = ScenePools>
  T first_set() const { return T(base, guest::LoadU32(base, address + 106000)); }
  void set_first_set(const View& v) const { guest::StoreU32(base, address + 106000, v.address); }
  void set_first_set(uint32_t v) const { guest::StoreU32(base, address + 106000, v); }
  uint32_t first_set_address() const { return address + 106000; }
};

// embedded; measure: CarPartsLoads
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
struct Cockpit : View {
  using View::View;
  static constexpr uint32_t kSize = 3520;
  static constexpr uint32_t kVtable = 0x82005020;
  // 0x82005020 (the pool node's 0x82004F90 while it's torn down)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the pool object's: 3 in use
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  // its +32 the block
  template <typename T = CarModel>
  T car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // a copy of its 144-byte record
  uint32_t record_address() const { return address + 28; }
  // the record's +20: 0 names the texture "%s_inte_on", else "%s_inte_on_0%d" with it
  uint32_t texture_number() const { return guest::LoadU32(base, address + 48); }
  void set_texture_number(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t texture_number_address() const { return address + 48; }
  // the loads (CockpitLoads: CarModelLoads' members, then six part models from +512)
  uint32_t loads_address() const { return address + 176; }
  // the node file's LoadSlot
  uint32_t node_file_slot_address() const { return address + 176; }
  uint32_t node_file_request() const { return guest::LoadU32(base, address + 180); }
  void set_node_file_request(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t node_file_request_address() const { return address + 180; }
  uint32_t node_file_buffer() const { return guest::LoadU32(base, address + 192); }
  void set_node_file_buffer(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t node_file_buffer_address() const { return address + 192; }
  // 2 loading, 3 arrived
  uint32_t node_file_state() const { return guest::LoadU32(base, address + 196); }
  void set_node_file_state(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t node_file_state_address() const { return address + 196; }
  // the node file's Skeleton (+176 its node values)
  uint32_t skeleton_address() const { return address + 208; }
  // the skeleton's +84: 160-byte names
  template <typename T = SceneLoadersName>
  T skeleton_node_names() const { return T(base, guest::LoadU32(base, address + 292)); }
  void set_skeleton_node_names(const View& v) const { guest::StoreU32(base, address + 292, v.address); }
  void set_skeleton_node_names(uint32_t v) const { guest::StoreU32(base, address + 292, v); }
  uint32_t skeleton_node_names_address() const { return address + 292; }
  // the skeleton's +148: 64 bytes a node
  template <typename T = SkeletonMatrix>
  T skeleton_node_matrices() const { return T(base, guest::LoadU32(base, address + 356)); }
  void set_skeleton_node_matrices(const View& v) const { guest::StoreU32(base, address + 356, v.address); }
  void set_skeleton_node_matrices(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t skeleton_node_matrices_address() const { return address + 356; }
  // the two XPR2s' LoadSlots
  uint32_t xpr_slots_address() const { return address + 400; }
  // the first's request (+24i)
  uint32_t xpr_slot_request() const { return guest::LoadU32(base, address + 404); }
  void set_xpr_slot_request(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t xpr_slot_request_address() const { return address + 404; }
  // the first's state (+24i)
  uint32_t xpr_slot_state() const { return guest::LoadU32(base, address + 420); }
  void set_xpr_slot_state(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t xpr_slot_state_address() const { return address + 420; }
  // two XPR objects, 24 bytes each
  uint32_t xprs_address() const { return address + 448; }
  // two texture providers, 20 bytes each
  uint32_t providers_address() const { return address + 496; }
  // the first provider's vtable word
  template <typename T = TextureProviderVtable>
  T provider_vtable() const { return T(base, guest::LoadU32(base, address + 496)); }
  void set_provider_vtable(const View& v) const { guest::StoreU32(base, address + 496, v.address); }
  void set_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 496, v); }
  uint32_t provider_vtable_address() const { return address + 496; }
  // the six parts' LoadSlots
  uint32_t part_slots_address() const { return address + 536; }
  // the first's request (+24i)
  uint32_t part_slot_request() const { return guest::LoadU32(base, address + 540); }
  void set_part_slot_request(uint32_t v) const { guest::StoreU32(base, address + 540, v); }
  uint32_t part_slot_request_address() const { return address + 540; }
  // the first's buffer (+24i)
  uint32_t part_slot_buffer() const { return guest::LoadU32(base, address + 552); }
  void set_part_slot_buffer(uint32_t v) const { guest::StoreU32(base, address + 552, v); }
  uint32_t part_slot_buffer_address() const { return address + 552; }
  // the first's state (+24i)
  uint32_t part_slot_state() const { return guest::LoadU32(base, address + 556); }
  void set_part_slot_state(uint32_t v) const { guest::StoreU32(base, address + 556, v); }
  uint32_t part_slot_state_address() const { return address + 556; }
  // six PartModels of 464 bytes
  uint32_t part_models_address() const { return address + 688; }
  // the first part model's +48, its texture provider (+496; +464i)
  template <typename T = TextureProvider>
  T part_provider() const { return T(base, guest::LoadU32(base, address + 736)); }
  void set_part_provider(const View& v) const { guest::StoreU32(base, address + 736, v.address); }
  void set_part_provider(uint32_t v) const { guest::StoreU32(base, address + 736, v); }
  uint32_t part_provider_address() const { return address + 736; }
  // the first part model's +452, the texture from the formatted name (+464i)
  uint32_t part_texture() const { return guest::LoadU32(base, address + 1140); }
  void set_part_texture(uint32_t v) const { guest::StoreU32(base, address + 1140, v); }
  uint32_t part_texture_address() const { return address + 1140; }
  // the matrix of the node named "acce_null" (0x82004FDC), probably the accessory point
  template <typename T = SkeletonMatrix>
  T accessory_node() const { return T(base, guest::LoadU32(base, address + 3472)); }
  void set_accessory_node(const View& v) const { guest::StoreU32(base, address + 3472, v.address); }
  void set_accessory_node(uint32_t v) const { guest::StoreU32(base, address + 3472, v); }
  uint32_t accessory_node_address() const { return address + 3472; }
  // its position: row 3, x, y, z times 0.1, w 1.0, turned a quarter about y
  uint32_t accessory_point_address() const { return address + 3488; }
  float accessory_point_w() const { return guest::LoadFloat(base, address + 3500); }
  void set_accessory_point_w(float v) const { guest::StoreFloat(base, address + 3500, v); }
  uint32_t accessory_point_w_address() const { return address + 3500; }
  // a texture from the first XPR by the formatted name
  uint32_t texture() const { return guest::LoadU32(base, address + 3504); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 3504, v); }
  uint32_t texture_address() const { return address + 3504; }
  template <typename T = ScenePools>
  T first_set() const { return T(base, guest::LoadU32(base, address + 3508)); }
  void set_first_set(const View& v) const { guest::StoreU32(base, address + 3508, v.address); }
  void set_first_set(uint32_t v) const { guest::StoreU32(base, address + 3508, v); }
  uint32_t first_set_address() const { return address + 3508; }
};

// allocated; measure: Interior, Interior::OnPartLoaded.r6
struct Interior : View {
  using View::View;
  static constexpr uint32_t kSize = 3104;
  static constexpr uint32_t kVtable = 0x8200509C;
  // 0x8200509C (the pool node's 0x82004F90 while it's torn down)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the pool object's: 3 in use
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  template <typename T = CarModel>
  T car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // a copy of its 120-byte record
  uint32_t record_address() const { return address + 28; }
  // the record's +112: the accessory picked (set b)
  uint32_t accessory() const { return guest::LoadU32(base, address + 140); }
  void set_accessory(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t accessory_address() const { return address + 140; }
  // the record's +116
  uint32_t accessory_arg() const { return guest::LoadU32(base, address + 144); }
  void set_accessory_arg(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t accessory_arg_address() const { return address + 144; }
  // the six parts' LoadSlots
  uint32_t part_slots_address() const { return address + 160; }
  // the first's request (+24i)
  uint32_t part_slot_request() const { return guest::LoadU32(base, address + 164); }
  void set_part_slot_request(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t part_slot_request_address() const { return address + 164; }
  // the first's state (+24i)
  uint32_t part_slot_state() const { return guest::LoadU32(base, address + 180); }
  void set_part_slot_state(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t part_slot_state_address() const { return address + 180; }
  // six PartModels of 464 bytes
  uint32_t part_models_address() const { return address + 304; }
  // nonzero: the cockpit set (c)
  uint32_t cockpit() const { return guest::LoadU32(base, address + 3088); }
  void set_cockpit(uint32_t v) const { guest::StoreU32(base, address + 3088, v); }
  uint32_t cockpit_address() const { return address + 3088; }
  template <typename T = ScenePools>
  T first_set() const { return T(base, guest::LoadU32(base, address + 3092)); }
  void set_first_set(const View& v) const { guest::StoreU32(base, address + 3092, v.address); }
  void set_first_set(uint32_t v) const { guest::StoreU32(base, address + 3092, v); }
  uint32_t first_set_address() const { return address + 3092; }
};

// allocated; measure: Pool4Object
struct Pool4Object : View {
  using View::View;
  static constexpr uint32_t kSize = 592;
  static constexpr uint32_t kVtable = 0x82005064;
  // 0x82005064 (the pool node's 0x82004F90 while it's torn down)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the pool object's: 3 in use
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  template <typename T = CarModel>
  T car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // a copy of its 52-byte record (Pool4Object::SetRecord); the measure's +48 store is SceneLights::Finish's on the model's +48 (a measure fault)
  uint32_t record_address() const { return address + 28; }
  // the model's LoadSlot
  uint32_t slot_address() const { return address + 80; }
  // 2 loading
  uint32_t slot_state() const { return guest::LoadU32(base, address + 100); }
  void set_slot_state(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t slot_state_address() const { return address + 100; }
  // a PartModel
  uint32_t model_address() const { return address + 112; }
  template <typename T = ScenePools>
  T first_set() const { return T(base, guest::LoadU32(base, address + 576)); }
  void set_first_set(const View& v) const { guest::StoreU32(base, address + 576, v.address); }
  void set_first_set(uint32_t v) const { guest::StoreU32(base, address + 576, v); }
  uint32_t first_set_address() const { return address + 576; }
};

// embedded; measure: CarModelLoads
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

// embedded; measure: WheelsLoads
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
