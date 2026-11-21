// The views of docs/functions/second-set.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/second-set.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: SceneCarModel
struct SceneCarModel : View {
  using View::View;
  static constexpr uint32_t kSize = 3744;
  static constexpr uint32_t kVtable = 0x82005130;
  // 0x82005130; the pool objects' base 0x82004F90 before ~PoolNode
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the pool object's: 0 free, 3 in use
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool (-1 from the constructor)
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  // the first set's car model (car-models.toml's CarModel); handed back by ScenePools::ReleaseA with its +20
  template <typename T = SecondSetFirstSetObject>
  T car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // the 3,696-byte car record (scene-models.toml's CarRecord)
  uint32_t car_record_address() const { return address + 32; }
  // the car scene (0 when reset)
  template <typename T = CarScene>
  T scene() const { return T(base, guest::LoadU32(base, address + 3728)); }
  void set_scene(const View& v) const { guest::StoreU32(base, address + 3728, v.address); }
  void set_scene(uint32_t v) const { guest::StoreU32(base, address + 3728, v); }
  uint32_t scene_address() const { return address + 3728; }
};

// embedded; measure: SceneCarModel.+24, SceneModel.+5084
struct SecondSetFirstSetObject : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // its index in its pool (ScenePools::ReleaseA's r5, ReleaseB's)
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
};

// allocated; measure: SceneCockpit
struct SceneCockpit : View {
  using View::View;
  static constexpr uint32_t kSize = 4320;
  static constexpr uint32_t kVtable = 0x8200527C;
  // 0x8200527C; the pool objects' base 0x82004F90 before ~PoolNode. Measure fault: CockpitPoints::Find's r4 (the part models at +4288) is filed here, its +0 to +20 the six part models
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the PoolNode's list (measure fault: part model 1)
  uint32_t list() const { return guest::LoadU32(base, address + 4); }
  void set_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t list_address() const { return address + 4; }
  // the PoolNode's links (measure fault: part models 2 and 3)
  uint32_t next() const { return guest::LoadU32(base, address + 8); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_address() const { return address + 8; }
  uint32_t previous() const { return guest::LoadU32(base, address + 12); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t previous_address() const { return address + 12; }
  // the pool object's: 3 in use (measure fault: part model 4)
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool (measure fault: part model 5)
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  // the second set's car model (SceneCockpit::SetUp; 0 when reset)
  uint32_t car_model() const { return guest::LoadU32(base, address + 24); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // its own interior (pool 3 of the second set)
  template <typename T = SceneInterior>
  T interior() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_interior(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_interior(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t interior_address() const { return address + 28; }
  // the first set's cockpit body (pool 2 of [0x826C2E40]; scene-loaders.toml's Cockpit)
  uint32_t body() const { return guest::LoadU32(base, address + 32); }
  void set_body(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t body_address() const { return address + 32; }
  // the 144-byte cockpit record (scene-models.toml's SceneModelsCockpitRecord)
  uint32_t record_address() const { return address + 36; }
  // the point block (SecondSetCockpitPoints)
  uint32_t points_address() const { return address + 192; }
  // the record's address (+36)
  template <typename T = SceneModelsCockpitRecord>
  T record_address() const { return T(base, guest::LoadU32(base, address + 4272)); }
  void set_record_address(const View& v) const { guest::StoreU32(base, address + 4272, v.address); }
  void set_record_address(uint32_t v) const { guest::StoreU32(base, address + 4272, v); }
  uint32_t record_address_address() const { return address + 4272; }
  // the body's six part models (its +688 + 464i; SceneCockpit::Finish)
  uint32_t part_models_address() const { return address + 4288; }
  uint32_t part_model_0() const { return guest::LoadU32(base, address + 4288); }
  void set_part_model_0(uint32_t v) const { guest::StoreU32(base, address + 4288, v); }
  uint32_t part_model_0_address() const { return address + 4288; }
  // part models 0 and 1 (cleared as doublewords by Reset)
  uint64_t part_models_01() const { return guest::LoadU64(base, address + 4288); }
  void set_part_models_01(uint64_t v) const { guest::StoreU64(base, address + 4288, v); }
  uint32_t part_models_01_address() const { return address + 4288; }
  uint64_t part_models_23() const { return guest::LoadU64(base, address + 4296); }
  void set_part_models_23(uint64_t v) const { guest::StoreU64(base, address + 4296, v); }
  uint32_t part_models_23_address() const { return address + 4296; }
  uint64_t part_models_45() const { return guest::LoadU64(base, address + 4304); }
  void set_part_models_45(uint64_t v) const { guest::StoreU64(base, address + 4304, v); }
  uint32_t part_models_45_address() const { return address + 4304; }
  // SceneCockpit::Finish
  uint32_t finished() const { return guest::LoadU32(base, address + 4312); }
  void set_finished(uint32_t v) const { guest::StoreU32(base, address + 4312, v); }
  uint32_t finished_address() const { return address + 4312; }
  template <typename T = CarScene>
  T scene() const { return T(base, guest::LoadU32(base, address + 4316)); }
  void set_scene(const View& v) const { guest::StoreU32(base, address + 4316, v.address); }
  void set_scene(uint32_t v) const { guest::StoreU32(base, address + 4316, v); }
  uint32_t scene_address() const { return address + 4316; }
};

// embedded; measure: CockpitPoints
struct SecondSetCockpitPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 4084;
  // the fenders' 3 records
  uint32_t fenders_address() const { return address + 0; }
  // a group's record 0: its model (the doors' window, DrawDoorWindow)
  uint32_t record0_model() const { return guest::LoadU32(base, address + 4); }
  void set_record0_model(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t record0_model_address() const { return address + 4; }
  uint32_t record0_shape() const { return guest::LoadU32(base, address + 228); }
  void set_record0_shape(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t record0_shape_address() const { return address + 228; }
  // a group's record 1 (the cabin's window, DrawCabinWindow)
  uint32_t record1_model() const { return guest::LoadU32(base, address + 244); }
  void set_record1_model(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t record1_model_address() const { return address + 244; }
  uint32_t record1_shape() const { return guest::LoadU32(base, address + 468); }
  void set_record1_shape(uint32_t v) const { guest::StoreU32(base, address + 468, v); }
  uint32_t record1_shape_address() const { return address + 468; }
  // the cabin's 2 records
  uint32_t cabin_address() const { return address + 720; }
  // the bonnet's record
  uint32_t bonnet_address() const { return address + 1200; }
  // a group's record 5 (the headlights', FindHeadlights' scaled)
  uint32_t record5_model() const { return guest::LoadU32(base, address + 1204); }
  void set_record5_model(uint32_t v) const { guest::StoreU32(base, address + 1204, v); }
  uint32_t record5_model_address() const { return address + 1204; }
  uint32_t record5_shape() const { return guest::LoadU32(base, address + 1428); }
  void set_record5_shape(uint32_t v) const { guest::StoreU32(base, address + 1428, v); }
  uint32_t record5_shape_address() const { return address + 1428; }
  // the doors' 2 records
  uint32_t doors_address() const { return address + 1440; }
  uint32_t record6_model() const { return guest::LoadU32(base, address + 1444); }
  void set_record6_model(uint32_t v) const { guest::StoreU32(base, address + 1444, v); }
  uint32_t record6_model_address() const { return address + 1444; }
  uint32_t record6_shape() const { return guest::LoadU32(base, address + 1668); }
  void set_record6_shape(uint32_t v) const { guest::StoreU32(base, address + 1668, v); }
  uint32_t record6_shape_address() const { return address + 1668; }
  // the mirror's record
  uint32_t mirror_address() const { return address + 1920; }
  // the headlights' 8 records
  uint32_t headlights_address() const { return address + 2160; }
  // 0 (CockpitPoints::Reset; not read in detail)
  uint32_t word_4080() const { return guest::LoadU32(base, address + 4080); }
  void set_word_4080(uint32_t v) const { guest::StoreU32(base, address + 4080, v); }
  uint32_t word_4080_address() const { return address + 4080; }
};

// allocated; measure: SceneInterior, List::At.ret.+5068
struct SceneInterior : View {
  using View::View;
  static constexpr uint32_t kSize = 11680;
  static constexpr uint32_t kVtable = 0x820062E4;
  // 0x820062E4; the pool objects' base 0x82004F90 before ~PoolNode. Measure fault: InteriorPoints::Find's r4 (the part models at +11632) is filed here, its +0 to +20 the six part models
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the PoolNode's list (measure fault: part model 1)
  uint32_t list() const { return guest::LoadU32(base, address + 4); }
  void set_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t list_address() const { return address + 4; }
  // the PoolNode's links (measure fault: part models 2 and 3)
  uint32_t next() const { return guest::LoadU32(base, address + 8); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_address() const { return address + 8; }
  uint32_t previous() const { return guest::LoadU32(base, address + 12); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t previous_address() const { return address + 12; }
  // the pool object's: 3 in use (measure fault: part model 4)
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool (measure fault: part model 5)
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  // the second set's car model (SceneInterior::Link): its +24 the first set's
  template <typename T = CarModelsSceneCarModel>
  T car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // the first set's car model's +3760 (SceneInterior::Link; the cockpit body + 208 under a cockpit)
  uint32_t skeleton() const { return guest::LoadU32(base, address + 28); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t skeleton_address() const { return address + 28; }
  // the first set's car model's +4048 (SceneInterior::Link; the cockpit body + 496 under a cockpit)
  uint32_t texture_set() const { return guest::LoadU32(base, address + 32); }
  void set_texture_set(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t texture_set_address() const { return address + 32; }
  // the first set's interior (pool 3 of [0x826C2E40])
  template <typename T = Interior>
  T first_set_interior() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_first_set_interior(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_first_set_interior(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t first_set_interior_address() const { return address + 36; }
  // the 120-byte interior record (car-draw.toml's InteriorRecord)
  uint32_t record_address() const { return address + 40; }
  // the record's +8 (SceneInterior::SetGauges)
  uint32_t gauge0_present() const { return guest::LoadU32(base, address + 48); }
  void set_gauge0_present(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t gauge0_present_address() const { return address + 48; }
  // the record's +12
  uint32_t gauge0_kind() const { return guest::LoadU32(base, address + 52); }
  void set_gauge0_kind(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t gauge0_kind_address() const { return address + 52; }
  // the record's +100 (SceneInterior::Apply)
  uint32_t seat_model() const { return guest::LoadU32(base, address + 140); }
  void set_seat_model(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t seat_model_address() const { return address + 140; }
  // the record's +112: the accessory (3 none; SceneInterior::Draw)
  uint32_t accessory() const { return guest::LoadU32(base, address + 152); }
  void set_accessory(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t accessory_address() const { return address + 152; }
  // the record's +116
  uint32_t accessory_arg() const { return guest::LoadU32(base, address + 156); }
  void set_accessory_arg(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t accessory_arg_address() const { return address + 156; }
  // the point block (SecondSetInteriorPoints)
  uint32_t points_address() const { return address + 160; }
  // the point block's +3156 (the meters' +4): the car's block (SceneInterior::Finish)
  uint32_t meters_car_block() const { return guest::LoadU32(base, address + 3316); }
  void set_meters_car_block(uint32_t v) const { guest::StoreU32(base, address + 3316, v); }
  uint32_t meters_car_block_address() const { return address + 3316; }
  // the point block's +8476 (the meters' +5324) = +32 (SceneInterior::Finish)
  uint32_t meters_turbo_provider() const { return guest::LoadU32(base, address + 8636); }
  void set_meters_turbo_provider(uint32_t v) const { guest::StoreU32(base, address + 8636, v); }
  uint32_t meters_turbo_provider_address() const { return address + 8636; }
  // the point block's +9496: the scene + 784 + 464 x the record's +100 (SceneInterior::Apply)
  uint32_t points_seat_model() const { return guest::LoadU32(base, address + 9656); }
  void set_points_seat_model(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t points_seat_model_address() const { return address + 9656; }
  // the point block's +11440: the first set's car model's +7168 (SceneInterior::Finish)
  uint32_t points_seat_position() const { return guest::LoadU32(base, address + 11600); }
  void set_points_seat_position(uint32_t v) const { guest::StoreU32(base, address + 11600, v); }
  uint32_t points_seat_position_address() const { return address + 11600; }
  // the point block's +11444: the record's address (+40)
  template <typename T = InteriorRecord>
  T points_record() const { return T(base, guest::LoadU32(base, address + 11604)); }
  void set_points_record(const View& v) const { guest::StoreU32(base, address + 11604, v.address); }
  void set_points_record(uint32_t v) const { guest::StoreU32(base, address + 11604, v); }
  uint32_t points_record_address() const { return address + 11604; }
  // the point block's +11448: the first set's car model's +3760
  uint32_t points_skeleton() const { return guest::LoadU32(base, address + 11608); }
  void set_points_skeleton(uint32_t v) const { guest::StoreU32(base, address + 11608, v); }
  uint32_t points_skeleton_address() const { return address + 11608; }
  // the point block's +11452: 1 under a cockpit (sub_820E3658)
  uint32_t points_cockpit() const { return guest::LoadU32(base, address + 11612); }
  void set_points_cockpit(uint32_t v) const { guest::StoreU32(base, address + 11612, v); }
  uint32_t points_cockpit_address() const { return address + 11612; }
  // the point block's +11456: the accessory drawn (CarScene::SetInteriorWord; 1 from Reset)
  uint32_t points_accessory_shown() const { return guest::LoadU32(base, address + 11616); }
  void set_points_accessory_shown(uint32_t v) const { guest::StoreU32(base, address + 11616, v); }
  uint32_t points_accessory_shown_address() const { return address + 11616; }
  // six part models (the first set's interior's +304 + 464i)
  uint32_t part_models_address() const { return address + 11632; }
  uint32_t part_model_0() const { return guest::LoadU32(base, address + 11632); }
  void set_part_model_0(uint32_t v) const { guest::StoreU32(base, address + 11632, v); }
  uint32_t part_model_0_address() const { return address + 11632; }
  // 1 under a cockpit (the cockpit set c)
  uint32_t cockpit() const { return guest::LoadU32(base, address + 11656); }
  void set_cockpit(uint32_t v) const { guest::StoreU32(base, address + 11656, v); }
  uint32_t cockpit_address() const { return address + 11656; }
  // SceneInterior::Finish
  uint32_t finished() const { return guest::LoadU32(base, address + 11660); }
  void set_finished(uint32_t v) const { guest::StoreU32(base, address + 11660, v); }
  uint32_t finished_address() const { return address + 11660; }
  // the cockpit body's texture (SceneCockpit::Finish)
  uint32_t cockpit_texture() const { return guest::LoadU32(base, address + 11664); }
  void set_cockpit_texture(uint32_t v) const { guest::StoreU32(base, address + 11664, v); }
  uint32_t cockpit_texture_address() const { return address + 11664; }
  template <typename T = CarScene>
  T scene() const { return T(base, guest::LoadU32(base, address + 11668)); }
  void set_scene(const View& v) const { guest::StoreU32(base, address + 11668, v.address); }
  void set_scene(uint32_t v) const { guest::StoreU32(base, address + 11668, v); }
  uint32_t scene_address() const { return address + 11668; }
};

// embedded; measure: InteriorPoints
struct SecondSetInteriorPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 11460;
  // the body's record
  uint32_t body_address() const { return address + 0; }
  // a group's: the interior record (0 from the resets)
  template <typename T = InteriorRecord>
  T group_record() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_group_record(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_group_record(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t group_record_address() const { return address + 0; }
  // a group's: the car's block (-1 from the resets; FindDials' IsBlockSetA, B)
  uint32_t group_word_4() const { return guest::LoadU32(base, address + 4); }
  void set_group_word_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t group_word_4_address() const { return address + 4; }
  // the dials' digits' texture provider (0 from ResetDials)
  uint32_t group_word_8() const { return guest::LoadU32(base, address + 8); }
  void set_group_word_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t group_word_8_address() const { return address + 8; }
  // a group's records (+16 + 240k): the dials' 12, the meters' 22, the others' 4
  uint32_t group_records_address() const { return address + 16; }
  // the dials' group (car-draw.toml's InteriorDials)
  uint32_t dials_address() const { return address + 240; }
  template <typename T = InteriorRecord>
  T dials_record() const { return T(base, guest::LoadU32(base, address + 240)); }
  void set_dials_record(const View& v) const { guest::StoreU32(base, address + 240, v.address); }
  void set_dials_record(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t dials_record_address() const { return address + 240; }
  // a group's record 8 (+16 + 240k): the dials' digits
  uint32_t record8_model() const { return guest::LoadU32(base, address + 2180); }
  void set_record8_model(uint32_t v) const { guest::StoreU32(base, address + 2180, v); }
  uint32_t record8_model_address() const { return address + 2180; }
  uint32_t record8_shape() const { return guest::LoadU32(base, address + 2404); }
  void set_record8_shape(uint32_t v) const { guest::StoreU32(base, address + 2404, v); }
  uint32_t record8_shape_address() const { return address + 2404; }
  uint32_t record9_model() const { return guest::LoadU32(base, address + 2420); }
  void set_record9_model(uint32_t v) const { guest::StoreU32(base, address + 2420, v); }
  uint32_t record9_model_address() const { return address + 2420; }
  uint32_t record9_shape() const { return guest::LoadU32(base, address + 2644); }
  void set_record9_shape(uint32_t v) const { guest::StoreU32(base, address + 2644, v); }
  uint32_t record9_shape_address() const { return address + 2644; }
  uint32_t record10_model() const { return guest::LoadU32(base, address + 2660); }
  void set_record10_model(uint32_t v) const { guest::StoreU32(base, address + 2660, v); }
  uint32_t record10_model_address() const { return address + 2660; }
  uint32_t record10_shape() const { return guest::LoadU32(base, address + 2884); }
  void set_record10_shape(uint32_t v) const { guest::StoreU32(base, address + 2884, v); }
  uint32_t record10_shape_address() const { return address + 2884; }
  // the meters' group (load-poll.toml's InteriorMeters)
  uint32_t meters_address() const { return address + 3152; }
  template <typename T = InteriorRecord>
  T meters_record() const { return T(base, guest::LoadU32(base, address + 3152)); }
  void set_meters_record(const View& v) const { guest::StoreU32(base, address + 3152, v.address); }
  void set_meters_record(uint32_t v) const { guest::StoreU32(base, address + 3152, v); }
  uint32_t meters_record_address() const { return address + 3152; }
  // the meters' group: seven gauge texture providers (0 from ResetMeters)
  uint32_t gauge_providers_address() const { return address + 5296; }
  uint32_t gauge0_provider() const { return guest::LoadU32(base, address + 5296); }
  void set_gauge0_provider(uint32_t v) const { guest::StoreU32(base, address + 5296, v); }
  uint32_t gauge0_provider_address() const { return address + 5296; }
  // the meters' group: gauge 1's in mode 5
  uint32_t turbo_provider() const { return guest::LoadU32(base, address + 5324); }
  void set_turbo_provider(uint32_t v) const { guest::StoreU32(base, address + 5324, v); }
  uint32_t turbo_provider_address() const { return address + 5324; }
  // the steering wheel's group
  uint32_t steering_address() const { return address + 8512; }
  template <typename T = InteriorRecord>
  T steering_record() const { return T(base, guest::LoadU32(base, address + 8512)); }
  void set_steering_record(const View& v) const { guest::StoreU32(base, address + 8512, v.address); }
  void set_steering_record(uint32_t v) const { guest::StoreU32(base, address + 8512, v); }
  uint32_t steering_record_address() const { return address + 8512; }
  // the seats' group
  uint32_t seats_address() const { return address + 9488; }
  template <typename T = InteriorRecord>
  T seats_record() const { return T(base, guest::LoadU32(base, address + 9488)); }
  void set_seats_record(const View& v) const { guest::StoreU32(base, address + 9488, v.address); }
  void set_seats_record(uint32_t v) const { guest::StoreU32(base, address + 9488, v); }
  uint32_t seats_record_address() const { return address + 9488; }
  // the seats' +4: +11440
  uint32_t seat_position() const { return guest::LoadU32(base, address + 9492); }
  void set_seat_position(uint32_t v) const { guest::StoreU32(base, address + 9492, v); }
  uint32_t seat_position_address() const { return address + 9492; }
  // the seats' +8: the scene + 784 + 464 x the record's +100, when that's set
  uint32_t seat_model() const { return guest::LoadU32(base, address + 9496); }
  void set_seat_model(uint32_t v) const { guest::StoreU32(base, address + 9496, v); }
  uint32_t seat_model_address() const { return address + 9496; }
  // the roll cage's group
  uint32_t roll_cage_address() const { return address + 10464; }
  template <typename T = InteriorRecord>
  T roll_cage_record() const { return T(base, guest::LoadU32(base, address + 10464)); }
  void set_roll_cage_record(const View& v) const { guest::StoreU32(base, address + 10464, v.address); }
  void set_roll_cage_record(uint32_t v) const { guest::StoreU32(base, address + 10464, v); }
  uint32_t roll_cage_record_address() const { return address + 10464; }
  // the first set's car model's +7168 (SceneInterior::Finish; 0 from Reset)
  uint32_t seat_position_source() const { return guest::LoadU32(base, address + 11440); }
  void set_seat_position_source(uint32_t v) const { guest::StoreU32(base, address + 11440, v); }
  uint32_t seat_position_source_address() const { return address + 11440; }
  // the interior's record (its +40)
  template <typename T = InteriorRecord>
  T record() const { return T(base, guest::LoadU32(base, address + 11444)); }
  void set_record(const View& v) const { guest::StoreU32(base, address + 11444, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 11444, v); }
  uint32_t record_address() const { return address + 11444; }
  // the first set's car model's +3760
  uint32_t skeleton() const { return guest::LoadU32(base, address + 11448); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(base, address + 11448, v); }
  uint32_t skeleton_address() const { return address + 11448; }
  // 1 under a cockpit (sub_820E3658; DrawRace)
  uint32_t cockpit() const { return guest::LoadU32(base, address + 11452); }
  void set_cockpit(uint32_t v) const { guest::StoreU32(base, address + 11452, v); }
  uint32_t cockpit_address() const { return address + 11452; }
  // 1 from Reset (CarScene::SetInteriorWord)
  uint32_t accessory_shown() const { return guest::LoadU32(base, address + 11456); }
  void set_accessory_shown(uint32_t v) const { guest::StoreU32(base, address + 11456, v); }
  uint32_t accessory_shown_address() const { return address + 11456; }
};

// allocated; measure: SceneWheels
struct SceneWheels : View {
  using View::View;
  static constexpr uint32_t kSize = 16800;
  static constexpr uint32_t kVtable = 0x82006638;
  // 0x82006638; the pool objects' base 0x82004F90 before ~PoolNode
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
  // the second set's car model (0 for traffic)
  uint32_t car_model() const { return guest::LoadU32(base, address + 24); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  // the first set's wheels (pool 5 of [0x826C2E40]; car-models.toml's CarWheels)
  uint32_t first_set_wheels() const { return guest::LoadU32(base, address + 28); }
  void set_first_set_wheels(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t first_set_wheels_address() const { return address + 28; }
  // the 156-byte wheels record (scene-models.toml's SceneModelsWheelsRecord: +n here is its +n - 32)
  uint32_t record_address() const { return address + 32; }
  // the record's +0
  uint32_t kind() const { return guest::LoadU32(base, address + 32); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t kind_address() const { return address + 32; }
  // the record's +8: 0 morphed once at set-up (SceneWheels::Draw)
  uint32_t morph_each_draw() const { return guest::LoadU32(base, address + 40); }
  void set_morph_each_draw(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t morph_each_draw_address() const { return address + 40; }
  // the record's +12 (SceneWheels::SetTireGrades)
  uint32_t front_tire_grade() const { return guest::LoadU32(base, address + 44); }
  void set_front_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t front_tire_grade_address() const { return address + 44; }
  // the record's +16
  uint32_t front_wheel() const { return guest::LoadU32(base, address + 48); }
  void set_front_wheel(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t front_wheel_address() const { return address + 48; }
  // the record's +28
  uint32_t rear_tire_grade() const { return guest::LoadU32(base, address + 60); }
  void set_rear_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t rear_tire_grade_address() const { return address + 60; }
  // the record's +32
  uint32_t rear_wheel() const { return guest::LoadU32(base, address + 64); }
  void set_rear_wheel(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t rear_wheel_address() const { return address + 64; }
  // the record's +44 to +64: the tires' sizes (SceneWheels::SetSizes)
  uint32_t front_size_index() const { return guest::LoadU32(base, address + 76); }
  void set_front_size_index(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t front_size_index_address() const { return address + 76; }
  uint32_t front_size_a() const { return guest::LoadU32(base, address + 80); }
  void set_front_size_a(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t front_size_a_address() const { return address + 80; }
  uint32_t front_size_b() const { return guest::LoadU32(base, address + 84); }
  void set_front_size_b(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t front_size_b_address() const { return address + 84; }
  uint32_t rear_size_index() const { return guest::LoadU32(base, address + 88); }
  void set_rear_size_index(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t rear_size_index_address() const { return address + 88; }
  uint32_t rear_size_a() const { return guest::LoadU32(base, address + 92); }
  void set_rear_size_a(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t rear_size_a_address() const { return address + 92; }
  uint32_t rear_size_b() const { return guest::LoadU32(base, address + 96); }
  void set_rear_size_b(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t rear_size_b_address() const { return address + 96; }
  // the record's +68
  uint32_t brake_kind() const { return guest::LoadU32(base, address + 100); }
  void set_brake_kind(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t brake_kind_address() const { return address + 100; }
  // the record's +72 (SetSizes' r4)
  uint32_t unknown_104() const { return guest::LoadU32(base, address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  // the record's +76 (SetSizes' r5)
  uint32_t unknown_108() const { return guest::LoadU32(base, address + 108); }
  void set_unknown_108(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  // the record's +80 to +120: from the table at 0x820044B0 (SceneWheels::SetUp)
  uint32_t table_112() const { return guest::LoadU32(base, address + 112); }
  void set_table_112(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t table_112_address() const { return address + 112; }
  uint32_t table_116() const { return guest::LoadU32(base, address + 116); }
  void set_table_116(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t table_116_address() const { return address + 116; }
  // the record's +88 (SceneWheels::SetCaliperTexture)
  uint32_t caliper_texture() const { return guest::LoadU32(base, address + 120); }
  void set_caliper_texture(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t caliper_texture_address() const { return address + 120; }
  uint32_t table_124() const { return guest::LoadU32(base, address + 124); }
  void set_table_124(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t table_124_address() const { return address + 124; }
  uint32_t table_128() const { return guest::LoadU32(base, address + 128); }
  void set_table_128(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t table_128_address() const { return address + 128; }
  // the record's +100
  float front_brake_turn() const { return guest::LoadFloat(base, address + 132); }
  void set_front_brake_turn(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t front_brake_turn_address() const { return address + 132; }
  // the record's +104
  float rear_brake_turn() const { return guest::LoadFloat(base, address + 136); }
  void set_rear_brake_turn(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t rear_brake_turn_address() const { return address + 136; }
  // the record's +108
  uint32_t front_brake_size() const { return guest::LoadU32(base, address + 140); }
  void set_front_brake_size(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t front_brake_size_address() const { return address + 140; }
  // the record's +112
  uint32_t rear_brake_size() const { return guest::LoadU32(base, address + 144); }
  void set_rear_brake_size(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t rear_brake_size_address() const { return address + 144; }
  // the record's +116: the entry's +40 (tire 584 for aftermarket wheels)
  uint32_t front_tire() const { return guest::LoadU32(base, address + 148); }
  void set_front_tire(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t front_tire_address() const { return address + 148; }
  // the record's +120
  uint32_t rear_tire() const { return guest::LoadU32(base, address + 152); }
  void set_rear_tire(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t rear_tire_address() const { return address + 152; }
  // the point block (SecondSetWheelPoints)
  uint32_t points_address() const { return address + 192; }
  // the point block's +0 (SceneWheels::SetUpPoints)
  uint32_t points_car_model() const { return guest::LoadU32(base, address + 192); }
  void set_points_car_model(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t points_car_model_address() const { return address + 192; }
  // the point block's +4: the traffic record
  template <typename T = CarModelsTrafficIndex>
  T points_car_record() const { return T(base, guest::LoadU32(base, address + 196)); }
  void set_points_car_record(const View& v) const { guest::StoreU32(base, address + 196, v.address); }
  void set_points_car_record(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t points_car_record_address() const { return address + 196; }
  // the point block's +8: the traffic body's tire positions
  uint32_t points_matrix() const { return guest::LoadU32(base, address + 200); }
  void set_points_matrix(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t points_matrix_address() const { return address + 200; }
  // the point block's +12: the record's address (+32)
  template <typename T = SceneModelsWheelsRecord>
  T points_record() const { return T(base, guest::LoadU32(base, address + 204)); }
  void set_points_record(const View& v) const { guest::StoreU32(base, address + 204, v.address); }
  void set_points_record(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t points_record_address() const { return address + 204; }
  // the point block's +16: the first set's wheels' + 240
  uint32_t points_tire_model() const { return guest::LoadU32(base, address + 208); }
  void set_points_tire_model(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t points_tire_model_address() const { return address + 208; }
  // the point block's +20: + 1216
  uint32_t points_rim_model() const { return guest::LoadU32(base, address + 212); }
  void set_points_rim_model(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t points_rim_model_address() const { return address + 212; }
  // the point block's +24: + 2336
  uint32_t points_brake_model() const { return guest::LoadU32(base, address + 216); }
  void set_points_brake_model(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t points_brake_model_address() const { return address + 216; }
  // SceneWheels::Finish
  uint32_t textures_bound() const { return guest::LoadU32(base, address + 16768); }
  void set_textures_bound(uint32_t v) const { guest::StoreU32(base, address + 16768, v); }
  uint32_t textures_bound_address() const { return address + 16768; }
  // SceneWheels::SetUpPoints
  uint32_t points_set_up() const { return guest::LoadU32(base, address + 16772); }
  void set_points_set_up(uint32_t v) const { guest::StoreU32(base, address + 16772, v); }
  uint32_t points_set_up_address() const { return address + 16772; }
  // the traffic record (its +0 the traffic car's index)
  uint32_t traffic_record() const { return guest::LoadU32(base, address + 16776); }
  void set_traffic_record(uint32_t v) const { guest::StoreU32(base, address + 16776, v); }
  uint32_t traffic_record_address() const { return address + 16776; }
  // the traffic body's tire positions (its +7072; SceneModel::PollLoad)
  uint32_t tire_positions() const { return guest::LoadU32(base, address + 16780); }
  void set_tire_positions(uint32_t v) const { guest::StoreU32(base, address + 16780, v); }
  uint32_t tire_positions_address() const { return address + 16780; }
  template <typename T = CarScene>
  T scene() const { return T(base, guest::LoadU32(base, address + 16784)); }
  void set_scene(const View& v) const { guest::StoreU32(base, address + 16784, v.address); }
  void set_scene(uint32_t v) const { guest::StoreU32(base, address + 16784, v); }
  uint32_t scene_address() const { return address + 16784; }
};

// embedded; measure: WheelPoints
struct SecondSetWheelPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 13648;
  // the block's: the second set's car model (SceneCarModel; its +24 the first set's)
  uint32_t car_model() const { return guest::LoadU32(base, address + 0); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t car_model_address() const { return address + 0; }
  // a group's: the car's matrix (the first set's car model's +6976, or the block's +8)
  template <typename T = SecondSetWheelMatrix>
  T matrix() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_matrix(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_matrix(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t matrix_address() const { return address + 0; }
  // measure fault: sub_820E6F28's and sub_820E7010's entries (r3 a tire + 12) filed at +0 to +20
  float entry_value() const { return guest::LoadFloat(base, address + 0); }
  void set_entry_value(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t entry_value_address() const { return address + 0; }
  // the block's (traffic) and a group's: the car record (its +0 the block)
  template <typename T = CarRecord>
  T car_record() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_car_record(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_car_record(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t car_record_address() const { return address + 4; }
  // a group's: the wheels record; the block's: the traffic body's tire positions (a matrix)
  template <typename T = SceneModelsWheelsRecord>
  T wheels_record() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_wheels_record(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t wheels_record_address() const { return address + 8; }
  // the block's: the wheels record; a rim's or the brakes': the front flag (1 from the resets)
  uint32_t word_12() const { return guest::LoadU32(base, address + 12); }
  void set_word_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t word_12_address() const { return address + 12; }
  // a tire's: twelve 24-byte entries (sub_820E6F28, sub_820E7010; cleared by ResetTire)
  uint32_t tire_entries_address() const { return address + 12; }
  // the block's: the tire model (the rear's + 464)
  uint32_t word_16() const { return guest::LoadU32(base, address + 16); }
  void set_word_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t word_16_address() const { return address + 16; }
  // a rim's or the brakes': the morph's weight
  float weight() const { return guest::LoadFloat(base, address + 16); }
  void set_weight(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t weight_address() const { return address + 16; }
  // the block's: the rim model
  uint32_t word_20() const { return guest::LoadU32(base, address + 20); }
  void set_word_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t word_20_address() const { return address + 20; }
  // a rim's: the tire's +356 copied (WheelPoints::SetUp)
  float float_20() const { return guest::LoadFloat(base, address + 20); }
  void set_float_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t float_20_address() const { return address + 20; }
  // the block's: the brake and caliper model
  uint32_t brake_model() const { return guest::LoadU32(base, address + 24); }
  void set_brake_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t brake_model_address() const { return address + 24; }
  // a rim's seven or the brakes' twelve records
  uint32_t group_records_address() const { return address + 32; }
  // the front tire's group
  uint32_t front_tire_address() const { return address + 32; }
  // also a rim's or the brakes' record 0's skeleton
  uint32_t front_tire_matrix() const { return guest::LoadU32(base, address + 32); }
  void set_front_tire_matrix(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t front_tire_matrix_address() const { return address + 32; }
  // also a rim's record 0's model (+32 + 4)
  uint32_t front_tire_car_record() const { return guest::LoadU32(base, address + 36); }
  void set_front_tire_car_record(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t front_tire_car_record_address() const { return address + 36; }
  template <typename T = SceneModelsWheelsRecord>
  T front_tire_wheels_record() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_front_tire_wheels_record(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_front_tire_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t front_tire_wheels_record_address() const { return address + 40; }
  // a rim's or the brakes' record 0's shape (+32 + 228)
  uint32_t rim_record0_shape() const { return guest::LoadU32(base, address + 260); }
  void set_rim_record0_shape(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t rim_record0_shape_address() const { return address + 260; }
  // a tire's: 1 the front (ResetTire), 0 the rear (WheelPoints::Reset)
  uint32_t tire_front() const { return guest::LoadU32(base, address + 300); }
  void set_tire_front(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t tire_front_address() const { return address + 300; }
  // a tire's: the morph's six shape indices (-1 from ResetTire; records 1 to 5's +228)
  uint32_t tire_shapes_address() const { return address + 304; }
  uint32_t tire_shape0() const { return guest::LoadU32(base, address + 304); }
  void set_tire_shape0(uint32_t v) const { guest::StoreU32(base, address + 304, v); }
  uint32_t tire_shape0_address() const { return address + 304; }
  // a tire's: the morph's six weights (sub_820E7010's float4)
  uint32_t tire_weights_address() const { return address + 328; }
  float tire_weight0() const { return guest::LoadFloat(base, address + 328); }
  void set_tire_weight0(float v) const { guest::StoreFloat(base, address + 328, v); }
  uint32_t tire_weight0_address() const { return address + 328; }
  // a tire's: sub_820E7010's out1
  float tire_out1() const { return guest::LoadFloat(base, address + 352); }
  void set_tire_out1(float v) const { guest::StoreFloat(base, address + 352, v); }
  uint32_t tire_out1_address() const { return address + 352; }
  // a tire's: sub_820E7010's out2
  float tire_out2() const { return guest::LoadFloat(base, address + 356); }
  void set_tire_out2(float v) const { guest::StoreFloat(base, address + 356, v); }
  uint32_t tire_out2_address() const { return address + 356; }
  // a tire's six records
  uint32_t tire_records_address() const { return address + 368; }
  // a tire's record 0's model (+368 + 4)
  uint32_t tire_record0_model() const { return guest::LoadU32(base, address + 372); }
  void set_tire_record0_model(uint32_t v) const { guest::StoreU32(base, address + 372, v); }
  uint32_t tire_record0_model_address() const { return address + 372; }
  // the block's: the front tire's +352
  float front_tire_out1() const { return guest::LoadFloat(base, address + 384); }
  void set_front_tire_out1(float v) const { guest::StoreFloat(base, address + 384, v); }
  uint32_t front_tire_out1_address() const { return address + 384; }
  // the block's: the front tire's +356
  float front_tire_out2() const { return guest::LoadFloat(base, address + 388); }
  void set_front_tire_out2(float v) const { guest::StoreFloat(base, address + 388, v); }
  uint32_t front_tire_out2_address() const { return address + 388; }
  // a rim's or the brakes' record 1's shape (the morph's target)
  uint32_t rim_record1_shape() const { return guest::LoadU32(base, address + 500); }
  void set_rim_record1_shape(uint32_t v) const { guest::StoreU32(base, address + 500, v); }
  uint32_t rim_record1_shape_address() const { return address + 500; }
  // a tire's record 0's shape: the morph's base
  uint32_t tire_base_shape() const { return guest::LoadU32(base, address + 596); }
  void set_tire_base_shape(uint32_t v) const { guest::StoreU32(base, address + 596, v); }
  uint32_t tire_base_shape_address() const { return address + 596; }
  uint32_t rim_record2_shape() const { return guest::LoadU32(base, address + 740); }
  void set_rim_record2_shape(uint32_t v) const { guest::StoreU32(base, address + 740, v); }
  uint32_t rim_record2_shape_address() const { return address + 740; }
  // a tire's records 1 to 5's shapes at +836 + 240k
  uint32_t tire_record1_shape() const { return guest::LoadU32(base, address + 836); }
  void set_tire_record1_shape(uint32_t v) const { guest::StoreU32(base, address + 836, v); }
  uint32_t tire_record1_shape_address() const { return address + 836; }
  uint32_t rim_record3_shape() const { return guest::LoadU32(base, address + 980); }
  void set_rim_record3_shape(uint32_t v) const { guest::StoreU32(base, address + 980, v); }
  uint32_t rim_record3_shape_address() const { return address + 980; }
  uint32_t rim_record4_shape() const { return guest::LoadU32(base, address + 1220); }
  void set_rim_record4_shape(uint32_t v) const { guest::StoreU32(base, address + 1220, v); }
  uint32_t rim_record4_shape_address() const { return address + 1220; }
  uint32_t rim_record5_shape() const { return guest::LoadU32(base, address + 1460); }
  void set_rim_record5_shape(uint32_t v) const { guest::StoreU32(base, address + 1460, v); }
  uint32_t rim_record5_shape_address() const { return address + 1460; }
  uint32_t rim_record6_shape() const { return guest::LoadU32(base, address + 1700); }
  void set_rim_record6_shape(uint32_t v) const { guest::StoreU32(base, address + 1700, v); }
  uint32_t rim_record6_shape_address() const { return address + 1700; }
  // a rim's two rows (32 bytes of the matrix's first or second half; (0, 0, 0, 1) from the resets)
  uint32_t rim_rows_address() const { return address + 1712; }
  float rim_row0_x() const { return guest::LoadFloat(base, address + 1712); }
  void set_rim_row0_x(float v) const { guest::StoreFloat(base, address + 1712, v); }
  uint32_t rim_row0_x_address() const { return address + 1712; }
  uint64_t rim_row0_xy() const { return guest::LoadU64(base, address + 1712); }
  void set_rim_row0_xy(uint64_t v) const { guest::StoreU64(base, address + 1712, v); }
  uint32_t rim_row0_xy_address() const { return address + 1712; }
  float rim_row0_y() const { return guest::LoadFloat(base, address + 1716); }
  void set_rim_row0_y(float v) const { guest::StoreFloat(base, address + 1716, v); }
  uint32_t rim_row0_y_address() const { return address + 1716; }
  float rim_row0_z() const { return guest::LoadFloat(base, address + 1720); }
  void set_rim_row0_z(float v) const { guest::StoreFloat(base, address + 1720, v); }
  uint32_t rim_row0_z_address() const { return address + 1720; }
  uint64_t rim_row0_zw() const { return guest::LoadU64(base, address + 1720); }
  void set_rim_row0_zw(uint64_t v) const { guest::StoreU64(base, address + 1720, v); }
  uint32_t rim_row0_zw_address() const { return address + 1720; }
  float rim_row0_w() const { return guest::LoadFloat(base, address + 1724); }
  void set_rim_row0_w(float v) const { guest::StoreFloat(base, address + 1724, v); }
  uint32_t rim_row0_w_address() const { return address + 1724; }
  uint64_t rim_row1_xy() const { return guest::LoadU64(base, address + 1728); }
  void set_rim_row1_xy(uint64_t v) const { guest::StoreU64(base, address + 1728, v); }
  uint32_t rim_row1_xy_address() const { return address + 1728; }
  uint64_t rim_row1_zw() const { return guest::LoadU64(base, address + 1736); }
  void set_rim_row1_zw(uint64_t v) const { guest::StoreU64(base, address + 1736, v); }
  uint32_t rim_row1_zw_address() const { return address + 1736; }
  // a tire's two rows
  uint32_t tire_rows_address() const { return address + 1808; }
  float tire_row0_x() const { return guest::LoadFloat(base, address + 1808); }
  void set_tire_row0_x(float v) const { guest::StoreFloat(base, address + 1808, v); }
  uint32_t tire_row0_x_address() const { return address + 1808; }
  uint64_t tire_row0_xy() const { return guest::LoadU64(base, address + 1808); }
  void set_tire_row0_xy(uint64_t v) const { guest::StoreU64(base, address + 1808, v); }
  uint32_t tire_row0_xy_address() const { return address + 1808; }
  float tire_row0_y() const { return guest::LoadFloat(base, address + 1812); }
  void set_tire_row0_y(float v) const { guest::StoreFloat(base, address + 1812, v); }
  uint32_t tire_row0_y_address() const { return address + 1812; }
  float tire_row0_z() const { return guest::LoadFloat(base, address + 1816); }
  void set_tire_row0_z(float v) const { guest::StoreFloat(base, address + 1816, v); }
  uint32_t tire_row0_z_address() const { return address + 1816; }
  uint64_t tire_row0_zw() const { return guest::LoadU64(base, address + 1816); }
  void set_tire_row0_zw(uint64_t v) const { guest::StoreU64(base, address + 1816, v); }
  uint32_t tire_row0_zw_address() const { return address + 1816; }
  float tire_row0_w() const { return guest::LoadFloat(base, address + 1820); }
  void set_tire_row0_w(float v) const { guest::StoreFloat(base, address + 1820, v); }
  uint32_t tire_row0_w_address() const { return address + 1820; }
  uint64_t tire_row1_xy() const { return guest::LoadU64(base, address + 1824); }
  void set_tire_row1_xy(uint64_t v) const { guest::StoreU64(base, address + 1824, v); }
  uint32_t tire_row1_xy_address() const { return address + 1824; }
  uint64_t tire_row1_zw() const { return guest::LoadU64(base, address + 1832); }
  void set_tire_row1_zw(uint64_t v) const { guest::StoreU64(base, address + 1832, v); }
  uint32_t tire_row1_zw_address() const { return address + 1832; }
  // the rear tire's group
  uint32_t rear_tire_address() const { return address + 1872; }
  uint32_t rear_tire_matrix() const { return guest::LoadU32(base, address + 1872); }
  void set_rear_tire_matrix(uint32_t v) const { guest::StoreU32(base, address + 1872, v); }
  uint32_t rear_tire_matrix_address() const { return address + 1872; }
  uint32_t rear_tire_car_record() const { return guest::LoadU32(base, address + 1876); }
  void set_rear_tire_car_record(uint32_t v) const { guest::StoreU32(base, address + 1876, v); }
  uint32_t rear_tire_car_record_address() const { return address + 1876; }
  template <typename T = SceneModelsWheelsRecord>
  T rear_tire_wheels_record() const { return T(base, guest::LoadU32(base, address + 1880)); }
  void set_rear_tire_wheels_record(const View& v) const { guest::StoreU32(base, address + 1880, v.address); }
  void set_rear_tire_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 1880, v); }
  uint32_t rear_tire_wheels_record_address() const { return address + 1880; }
  uint32_t brake_record7_shape() const { return guest::LoadU32(base, address + 1940); }
  void set_brake_record7_shape(uint32_t v) const { guest::StoreU32(base, address + 1940, v); }
  uint32_t brake_record7_shape_address() const { return address + 1940; }
  // the block's: the rear tire's +300 (0)
  uint32_t rear_tire_front() const { return guest::LoadU32(base, address + 2172); }
  void set_rear_tire_front(uint32_t v) const { guest::StoreU32(base, address + 2172, v); }
  uint32_t rear_tire_front_address() const { return address + 2172; }
  uint32_t brake_record8_shape() const { return guest::LoadU32(base, address + 2180); }
  void set_brake_record8_shape(uint32_t v) const { guest::StoreU32(base, address + 2180, v); }
  uint32_t brake_record8_shape_address() const { return address + 2180; }
  // the block's: the rear tire's +352
  float rear_tire_out1() const { return guest::LoadFloat(base, address + 2224); }
  void set_rear_tire_out1(float v) const { guest::StoreFloat(base, address + 2224, v); }
  uint32_t rear_tire_out1_address() const { return address + 2224; }
  // the block's: the rear tire's +356
  float rear_tire_out2() const { return guest::LoadFloat(base, address + 2228); }
  void set_rear_tire_out2(float v) const { guest::StoreFloat(base, address + 2228, v); }
  uint32_t rear_tire_out2_address() const { return address + 2228; }
  uint32_t brake_record9_shape() const { return guest::LoadU32(base, address + 2420); }
  void set_brake_record9_shape(uint32_t v) const { guest::StoreU32(base, address + 2420, v); }
  uint32_t brake_record9_shape_address() const { return address + 2420; }
  // the brakes' record 10 (scaled by 1.0)
  uint32_t brake_record10_model() const { return guest::LoadU32(base, address + 2436); }
  void set_brake_record10_model(uint32_t v) const { guest::StoreU32(base, address + 2436, v); }
  uint32_t brake_record10_model_address() const { return address + 2436; }
  uint32_t brake_record10_shape() const { return guest::LoadU32(base, address + 2660); }
  void set_brake_record10_shape(uint32_t v) const { guest::StoreU32(base, address + 2660, v); }
  uint32_t brake_record10_shape_address() const { return address + 2660; }
  uint32_t brake_record11_model() const { return guest::LoadU32(base, address + 2676); }
  void set_brake_record11_model(uint32_t v) const { guest::StoreU32(base, address + 2676, v); }
  uint32_t brake_record11_model_address() const { return address + 2676; }
  uint32_t brake_record11_shape() const { return guest::LoadU32(base, address + 2900); }
  void set_brake_record11_shape(uint32_t v) const { guest::StoreU32(base, address + 2900, v); }
  uint32_t brake_record11_shape_address() const { return address + 2900; }
  // the brakes' two rows
  uint32_t brake_rows_address() const { return address + 2912; }
  float brake_row0_x() const { return guest::LoadFloat(base, address + 2912); }
  void set_brake_row0_x(float v) const { guest::StoreFloat(base, address + 2912, v); }
  uint32_t brake_row0_x_address() const { return address + 2912; }
  uint64_t brake_row0_xy() const { return guest::LoadU64(base, address + 2912); }
  void set_brake_row0_xy(uint64_t v) const { guest::StoreU64(base, address + 2912, v); }
  uint32_t brake_row0_xy_address() const { return address + 2912; }
  float brake_row0_y() const { return guest::LoadFloat(base, address + 2916); }
  void set_brake_row0_y(float v) const { guest::StoreFloat(base, address + 2916, v); }
  uint32_t brake_row0_y_address() const { return address + 2916; }
  float brake_row0_z() const { return guest::LoadFloat(base, address + 2920); }
  void set_brake_row0_z(float v) const { guest::StoreFloat(base, address + 2920, v); }
  uint32_t brake_row0_z_address() const { return address + 2920; }
  uint64_t brake_row0_zw() const { return guest::LoadU64(base, address + 2920); }
  void set_brake_row0_zw(uint64_t v) const { guest::StoreU64(base, address + 2920, v); }
  uint32_t brake_row0_zw_address() const { return address + 2920; }
  float brake_row0_w() const { return guest::LoadFloat(base, address + 2924); }
  void set_brake_row0_w(float v) const { guest::StoreFloat(base, address + 2924, v); }
  uint32_t brake_row0_w_address() const { return address + 2924; }
  uint64_t brake_row1_xy() const { return guest::LoadU64(base, address + 2928); }
  void set_brake_row1_xy(uint64_t v) const { guest::StoreU64(base, address + 2928, v); }
  uint32_t brake_row1_xy_address() const { return address + 2928; }
  uint64_t brake_row1_zw() const { return guest::LoadU64(base, address + 2936); }
  void set_brake_row1_zw(uint64_t v) const { guest::StoreU64(base, address + 2936, v); }
  uint32_t brake_row1_zw_address() const { return address + 2936; }
  // set a's front rim's group
  uint32_t rim_a_front_address() const { return address + 3712; }
  uint32_t rim_a_front_matrix() const { return guest::LoadU32(base, address + 3712); }
  void set_rim_a_front_matrix(uint32_t v) const { guest::StoreU32(base, address + 3712, v); }
  uint32_t rim_a_front_matrix_address() const { return address + 3712; }
  uint32_t rim_a_front_wheels_record() const { return guest::LoadU32(base, address + 3720); }
  void set_rim_a_front_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 3720, v); }
  uint32_t rim_a_front_wheels_record_address() const { return address + 3720; }
  // the front tire's +352
  float rim_a_front_weight() const { return guest::LoadFloat(base, address + 3728); }
  void set_rim_a_front_weight(float v) const { guest::StoreFloat(base, address + 3728, v); }
  uint32_t rim_a_front_weight_address() const { return address + 3728; }
  float rim_a_front_float_20() const { return guest::LoadFloat(base, address + 3732); }
  void set_rim_a_front_float_20(float v) const { guest::StoreFloat(base, address + 3732, v); }
  uint32_t rim_a_front_float_20_address() const { return address + 3732; }
  // set a's rear rim's group
  uint32_t rim_a_rear_address() const { return address + 5456; }
  // measure fault: the matrix word, filed as pointing to the wheels record
  template <typename T = SceneModelsWheelsRecord>
  T rim_a_rear_matrix() const { return T(base, guest::LoadU32(base, address + 5456)); }
  void set_rim_a_rear_matrix(const View& v) const { guest::StoreU32(base, address + 5456, v.address); }
  void set_rim_a_rear_matrix(uint32_t v) const { guest::StoreU32(base, address + 5456, v); }
  uint32_t rim_a_rear_matrix_address() const { return address + 5456; }
  template <typename T = SceneModelsWheelsRecord>
  T rim_a_rear_wheels_record() const { return T(base, guest::LoadU32(base, address + 5464)); }
  void set_rim_a_rear_wheels_record(const View& v) const { guest::StoreU32(base, address + 5464, v.address); }
  void set_rim_a_rear_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 5464, v); }
  uint32_t rim_a_rear_wheels_record_address() const { return address + 5464; }
  // 0 (WheelPoints::Reset)
  uint32_t rim_a_rear_front() const { return guest::LoadU32(base, address + 5468); }
  void set_rim_a_rear_front(uint32_t v) const { guest::StoreU32(base, address + 5468, v); }
  uint32_t rim_a_rear_front_address() const { return address + 5468; }
  float rim_a_rear_weight() const { return guest::LoadFloat(base, address + 5472); }
  void set_rim_a_rear_weight(float v) const { guest::StoreFloat(base, address + 5472, v); }
  uint32_t rim_a_rear_weight_address() const { return address + 5472; }
  float rim_a_rear_float_20() const { return guest::LoadFloat(base, address + 5476); }
  void set_rim_a_rear_float_20(float v) const { guest::StoreFloat(base, address + 5476, v); }
  uint32_t rim_a_rear_float_20_address() const { return address + 5476; }
  // set b's front rim's group
  uint32_t rim_b_front_address() const { return address + 7200; }
  uint32_t rim_b_front_matrix() const { return guest::LoadU32(base, address + 7200); }
  void set_rim_b_front_matrix(uint32_t v) const { guest::StoreU32(base, address + 7200, v); }
  uint32_t rim_b_front_matrix_address() const { return address + 7200; }
  uint32_t rim_b_front_wheels_record() const { return guest::LoadU32(base, address + 7208); }
  void set_rim_b_front_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 7208, v); }
  uint32_t rim_b_front_wheels_record_address() const { return address + 7208; }
  float rim_b_front_weight() const { return guest::LoadFloat(base, address + 7216); }
  void set_rim_b_front_weight(float v) const { guest::StoreFloat(base, address + 7216, v); }
  uint32_t rim_b_front_weight_address() const { return address + 7216; }
  float rim_b_front_float_20() const { return guest::LoadFloat(base, address + 7220); }
  void set_rim_b_front_float_20(float v) const { guest::StoreFloat(base, address + 7220, v); }
  uint32_t rim_b_front_float_20_address() const { return address + 7220; }
  // set b's rear rim's group
  uint32_t rim_b_rear_address() const { return address + 8944; }
  uint32_t rim_b_rear_matrix() const { return guest::LoadU32(base, address + 8944); }
  void set_rim_b_rear_matrix(uint32_t v) const { guest::StoreU32(base, address + 8944, v); }
  uint32_t rim_b_rear_matrix_address() const { return address + 8944; }
  uint32_t rim_b_rear_wheels_record() const { return guest::LoadU32(base, address + 8952); }
  void set_rim_b_rear_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 8952, v); }
  uint32_t rim_b_rear_wheels_record_address() const { return address + 8952; }
  // 0 (WheelPoints::Reset)
  uint32_t rim_b_rear_front() const { return guest::LoadU32(base, address + 8956); }
  void set_rim_b_rear_front(uint32_t v) const { guest::StoreU32(base, address + 8956, v); }
  uint32_t rim_b_rear_front_address() const { return address + 8956; }
  float rim_b_rear_weight() const { return guest::LoadFloat(base, address + 8960); }
  void set_rim_b_rear_weight(float v) const { guest::StoreFloat(base, address + 8960, v); }
  uint32_t rim_b_rear_weight_address() const { return address + 8960; }
  float rim_b_rear_float_20() const { return guest::LoadFloat(base, address + 8964); }
  void set_rim_b_rear_float_20(float v) const { guest::StoreFloat(base, address + 8964, v); }
  uint32_t rim_b_rear_float_20_address() const { return address + 8964; }
  // the front brakes' group
  uint32_t brakes_front_address() const { return address + 10688; }
  uint32_t brakes_front_matrix() const { return guest::LoadU32(base, address + 10688); }
  void set_brakes_front_matrix(uint32_t v) const { guest::StoreU32(base, address + 10688, v); }
  uint32_t brakes_front_matrix_address() const { return address + 10688; }
  uint32_t brakes_front_car_record() const { return guest::LoadU32(base, address + 10692); }
  void set_brakes_front_car_record(uint32_t v) const { guest::StoreU32(base, address + 10692, v); }
  uint32_t brakes_front_car_record_address() const { return address + 10692; }
  uint32_t brakes_front_wheels_record() const { return guest::LoadU32(base, address + 10696); }
  void set_brakes_front_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 10696, v); }
  uint32_t brakes_front_wheels_record_address() const { return address + 10696; }
  // the rear brakes' group (to +16576, past the block's measured 13,648)
  uint32_t brakes_rear_address() const { return address + 13632; }
  uint32_t brakes_rear_matrix() const { return guest::LoadU32(base, address + 13632); }
  void set_brakes_rear_matrix(uint32_t v) const { guest::StoreU32(base, address + 13632, v); }
  uint32_t brakes_rear_matrix_address() const { return address + 13632; }
  uint32_t brakes_rear_car_record() const { return guest::LoadU32(base, address + 13636); }
  void set_brakes_rear_car_record(uint32_t v) const { guest::StoreU32(base, address + 13636, v); }
  uint32_t brakes_rear_car_record_address() const { return address + 13636; }
  uint32_t brakes_rear_wheels_record() const { return guest::LoadU32(base, address + 13640); }
  void set_brakes_rear_wheels_record(uint32_t v) const { guest::StoreU32(base, address + 13640, v); }
  uint32_t brakes_rear_wheels_record_address() const { return address + 13640; }
  // 0 (WheelPoints::Reset)
  uint32_t brakes_rear_front() const { return guest::LoadU32(base, address + 13644); }
  void set_brakes_rear_front(uint32_t v) const { guest::StoreU32(base, address + 13644, v); }
  uint32_t brakes_rear_front_address() const { return address + 13644; }
};

// embedded; measure: WheelPoints.+0
struct SecondSetWheelMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint64_t row0_xy() const { return guest::LoadU64(base, address + 0); }
  void set_row0_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t row0_xy_address() const { return address + 0; }
  uint64_t row0_zw() const { return guest::LoadU64(base, address + 8); }
  void set_row0_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t row0_zw_address() const { return address + 8; }
  uint64_t row1_xy() const { return guest::LoadU64(base, address + 16); }
  void set_row1_xy(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t row1_xy_address() const { return address + 16; }
  uint64_t row1_zw() const { return guest::LoadU64(base, address + 24); }
  void set_row1_zw(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t row1_zw_address() const { return address + 24; }
  // measure fault: the block's +0's +24 (WheelPoints::SetUp)
  uint32_t car_model() const { return guest::LoadU32(base, address + 24); }
  void set_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t car_model_address() const { return address + 24; }
  uint64_t row2_xy() const { return guest::LoadU64(base, address + 32); }
  void set_row2_xy(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t row2_xy_address() const { return address + 32; }
  uint64_t row2_zw() const { return guest::LoadU64(base, address + 40); }
  void set_row2_zw(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t row2_zw_address() const { return address + 40; }
  uint64_t row3_xy() const { return guest::LoadU64(base, address + 48); }
  void set_row3_xy(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t row3_xy_address() const { return address + 48; }
  uint64_t row3_zw() const { return guest::LoadU64(base, address + 56); }
  void set_row3_zw(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t row3_zw_address() const { return address + 56; }
};

// allocated; measure: SceneTraffic
struct SceneTraffic : View {
  using View::View;
  static constexpr uint32_t kSize = 9808;
  static constexpr uint32_t kVtable = 0x82006424;
  // 0x82006424; the pool objects' base 0x82004F90 before ~PoolNode. Measure fault: TrafficPoints::Find's r4 (the part models at +9768) is filed here, its +0 to +20 the six part models
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the PoolNode's list (measure fault: part model 1)
  uint32_t list() const { return guest::LoadU32(base, address + 4); }
  void set_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t list_address() const { return address + 4; }
  // the PoolNode's links (measure fault: part models 2 and 3)
  uint32_t next() const { return guest::LoadU32(base, address + 8); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_address() const { return address + 8; }
  uint32_t previous() const { return guest::LoadU32(base, address + 12); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t previous_address() const { return address + 12; }
  // the pool object's: 3 in use (measure fault: part model 4)
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool (measure fault: part model 5)
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  // the 3,696-byte car record (scene-models.toml's CarRecord)
  uint32_t car_record_address() const { return address + 32; }
  // the point block (car-draw.toml's TrafficPoints)
  uint32_t points_address() const { return address + 3728; }
  // the point block's +1456: the first set's body's +7040 (SceneTraffic::Finish)
  uint32_t points_front_plate_texture() const { return guest::LoadU32(base, address + 5184); }
  void set_points_front_plate_texture(uint32_t v) const { guest::StoreU32(base, address + 5184, v); }
  uint32_t points_front_plate_texture_address() const { return address + 5184; }
  // the point block's +1460: the body's +7048
  uint32_t points_rear_plate_texture() const { return guest::LoadU32(base, address + 5188); }
  void set_points_rear_plate_texture(uint32_t v) const { guest::StoreU32(base, address + 5188, v); }
  uint32_t points_rear_plate_texture_address() const { return address + 5188; }
  // the first set's traffic body (pool 6 of [0x826C2E40])
  uint32_t body() const { return guest::LoadU32(base, address + 9760); }
  void set_body(uint32_t v) const { guest::StoreU32(base, address + 9760, v); }
  uint32_t body_address() const { return address + 9760; }
  // the body's +3776
  uint32_t skeleton() const { return guest::LoadU32(base, address + 9764); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(base, address + 9764, v); }
  uint32_t skeleton_address() const { return address + 9764; }
  // the body's six part models (its +4256 + 464i)
  uint32_t part_models_address() const { return address + 9768; }
  uint32_t part_model_0() const { return guest::LoadU32(base, address + 9768); }
  void set_part_model_0(uint32_t v) const { guest::StoreU32(base, address + 9768, v); }
  uint32_t part_model_0_address() const { return address + 9768; }
  // checked by SceneTraffic::Finish, never set there
  uint32_t finished() const { return guest::LoadU32(base, address + 9792); }
  void set_finished(uint32_t v) const { guest::StoreU32(base, address + 9792, v); }
  uint32_t finished_address() const { return address + 9792; }
  template <typename T = CarScene>
  T scene() const { return T(base, guest::LoadU32(base, address + 9796)); }
  void set_scene(const View& v) const { guest::StoreU32(base, address + 9796, v.address); }
  void set_scene(uint32_t v) const { guest::StoreU32(base, address + 9796, v); }
  uint32_t scene_address() const { return address + 9796; }
};

// embedded; measure: PointRecord, ModelPoint
struct SecondSetPointRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 240;
  // the skeleton (ModelPoint::Find; 0 from Clear)
  uint32_t skeleton() const { return guest::LoadU32(base, address + 0); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t skeleton_address() const { return address + 0; }
  // the model found (0 until found)
  uint32_t model() const { return guest::LoadU32(base, address + 4); }
  void set_model(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t model_address() const { return address + 4; }
  // 0, or (FindShape) the shape found (544 bytes, the model's +40)
  uint32_t shape() const { return guest::LoadU32(base, address + 8); }
  void set_shape(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t shape_address() const { return address + 8; }
  // the node's matrix (the identity from Clear)
  uint32_t matrix_address() const { return address + 16; }
  // 1.0 (PointRecord::PointRecord)
  float matrix_row0_w() const { return guest::LoadFloat(base, address + 28); }
  void set_matrix_row0_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t matrix_row0_w_address() const { return address + 28; }
  float matrix_row1_w() const { return guest::LoadFloat(base, address + 44); }
  void set_matrix_row1_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t matrix_row1_w_address() const { return address + 44; }
  float matrix_row2_w() const { return guest::LoadFloat(base, address + 60); }
  void set_matrix_row2_w(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t matrix_row2_w_address() const { return address + 60; }
  // the translation (ModelPoint::Find scales it)
  float matrix_row3_x() const { return guest::LoadFloat(base, address + 64); }
  void set_matrix_row3_x(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t matrix_row3_x_address() const { return address + 64; }
  uint64_t matrix_row3_xy() const { return guest::LoadU64(base, address + 64); }
  void set_matrix_row3_xy(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t matrix_row3_xy_address() const { return address + 64; }
  float matrix_row3_y() const { return guest::LoadFloat(base, address + 68); }
  void set_matrix_row3_y(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t matrix_row3_y_address() const { return address + 68; }
  float matrix_row3_z() const { return guest::LoadFloat(base, address + 72); }
  void set_matrix_row3_z(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t matrix_row3_z_address() const { return address + 72; }
  uint64_t matrix_row3_zw() const { return guest::LoadU64(base, address + 72); }
  void set_matrix_row3_zw(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t matrix_row3_zw_address() const { return address + 72; }
  float matrix_row3_w() const { return guest::LoadFloat(base, address + 76); }
  void set_matrix_row3_w(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t matrix_row3_w_address() const { return address + 76; }
  // its inverse
  uint32_t inverse_address() const { return address + 80; }
  float inverse_row0_w() const { return guest::LoadFloat(base, address + 92); }
  void set_inverse_row0_w(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t inverse_row0_w_address() const { return address + 92; }
  float inverse_row1_w() const { return guest::LoadFloat(base, address + 108); }
  void set_inverse_row1_w(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t inverse_row1_w_address() const { return address + 108; }
  float inverse_row2_w() const { return guest::LoadFloat(base, address + 124); }
  void set_inverse_row2_w(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t inverse_row2_w_address() const { return address + 124; }
  float inverse_row3_w() const { return guest::LoadFloat(base, address + 140); }
  void set_inverse_row3_w(float v) const { guest::StoreFloat(base, address + 140, v); }
  uint32_t inverse_row3_w_address() const { return address + 140; }
  // its translation ((0, 0, 0, 1) from Clear)
  uint32_t position_address() const { return address + 144; }
  uint64_t position_xy() const { return guest::LoadU64(base, address + 144); }
  void set_position_xy(uint64_t v) const { guest::StoreU64(base, address + 144, v); }
  uint32_t position_xy_address() const { return address + 144; }
  uint64_t position_zw() const { return guest::LoadU64(base, address + 152); }
  void set_position_zw(uint64_t v) const { guest::StoreU64(base, address + 152, v); }
  uint32_t position_zw_address() const { return address + 152; }
  // 1.0 (PointRecord::PointRecord)
  float position_w() const { return guest::LoadFloat(base, address + 156); }
  void set_position_w(float v) const { guest::StoreFloat(base, address + 156, v); }
  uint32_t position_w_address() const { return address + 156; }
  // the node's name (SetNode; NameMatches)
  uint32_t node_name_address() const { return address + 160; }
  uint8_t node_name_0() const { return guest::LoadU8(base, address + 160); }
  void set_node_name_0(uint8_t v) const { guest::StoreU8(base, address + 160, v); }
  uint32_t node_name_0_address() const { return address + 160; }
  // the shape's name (SetShape)
  uint32_t shape_name_address() const { return address + 192; }
  uint8_t shape_name_0() const { return guest::LoadU8(base, address + 192); }
  void set_shape_name_0(uint8_t v) const { guest::StoreU8(base, address + 192, v); }
  uint32_t shape_name_0_address() const { return address + 192; }
  // -1 from Clear
  uint32_t node_index() const { return guest::LoadU32(base, address + 224); }
  void set_node_index(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t node_index_address() const { return address + 224; }
  // -1, or (FindShape) the shape's index (the node's +40)
  uint32_t shape_index() const { return guest::LoadU32(base, address + 228); }
  void set_shape_index(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t shape_index_address() const { return address + 228; }
  // measure fault: the parts' headlights group's +3120 matrix (InitHeadlights)
  float headlights_matrix_row0_w() const { return guest::LoadFloat(base, address + 3132); }
  void set_headlights_matrix_row0_w(float v) const { guest::StoreFloat(base, address + 3132, v); }
  uint32_t headlights_matrix_row0_w_address() const { return address + 3132; }
  float headlights_matrix_row1_w() const { return guest::LoadFloat(base, address + 3148); }
  void set_headlights_matrix_row1_w(float v) const { guest::StoreFloat(base, address + 3148, v); }
  uint32_t headlights_matrix_row1_w_address() const { return address + 3148; }
  float headlights_matrix_row2_w() const { return guest::LoadFloat(base, address + 3164); }
  void set_headlights_matrix_row2_w(float v) const { guest::StoreFloat(base, address + 3164, v); }
  uint32_t headlights_matrix_row2_w_address() const { return address + 3164; }
  float headlights_matrix_row3_w() const { return guest::LoadFloat(base, address + 3180); }
  void set_headlights_matrix_row3_w(float v) const { guest::StoreFloat(base, address + 3180, v); }
  uint32_t headlights_matrix_row3_w_address() const { return address + 3180; }
  // measure fault: the parts' doors' group's +6016 matrix (InitLeftDoor, InitRightDoor)
  float door_matrix_row0_w() const { return guest::LoadFloat(base, address + 6028); }
  void set_door_matrix_row0_w(float v) const { guest::StoreFloat(base, address + 6028, v); }
  uint32_t door_matrix_row0_w_address() const { return address + 6028; }
  float door_matrix_row1_w() const { return guest::LoadFloat(base, address + 6044); }
  void set_door_matrix_row1_w(float v) const { guest::StoreFloat(base, address + 6044, v); }
  uint32_t door_matrix_row1_w_address() const { return address + 6044; }
  float door_matrix_row2_w() const { return guest::LoadFloat(base, address + 6060); }
  void set_door_matrix_row2_w(float v) const { guest::StoreFloat(base, address + 6060, v); }
  uint32_t door_matrix_row2_w_address() const { return address + 6060; }
  float door_matrix_row3_w() const { return guest::LoadFloat(base, address + 6076); }
  void set_door_matrix_row3_w(float v) const { guest::StoreFloat(base, address + 6076, v); }
  uint32_t door_matrix_row3_w_address() const { return address + 6076; }
};

// embedded; measure: PointRecord::SetNode.r4, PointRecord::SetShape.r4
struct SecondSetPointName : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  // its characters, to the 0
  uint8_t first() const { return guest::LoadU8(base, address + 0); }
  void set_first(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
};

// embedded; measure: ModelPoint::FindShape.r4
struct SecondSetFindModel : View {
  using View::View;
  static constexpr uint32_t kSize = 412;
  // 192 bytes a node
  template <typename T = SecondSetModelNode>
  T nodes() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_nodes(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_nodes(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t nodes_address() const { return address + 36; }
  // 544 bytes a shape
  template <typename T = ModelShape>
  T shapes() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_shapes(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_shapes(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t shapes_address() const { return address + 40; }
  uint32_t node_count() const { return guest::LoadU32(base, address + 408); }
  void set_node_count(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t node_count_address() const { return address + 408; }
};

// embedded; measure: ModelPoint::FindShape.r4.+36
struct SecondSetModelNode : View {
  using View::View;
  static constexpr uint32_t kSize = 192;
  // its shape's index (-1 none)
  uint32_t shape() const { return guest::LoadU32(base, address + 40); }
  void set_shape(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t shape_address() const { return address + 40; }
};

// embedded; measure: PartsPoints.+1444, PartsPoints.+1924
struct SecondSetPlateModel : View {
  using View::View;
  static constexpr uint32_t kSize = 460;
  // set by the bumpers' draws (not read in detail)
  uint32_t unknown_432() const { return guest::LoadU32(base, address + 432); }
  void set_unknown_432(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t unknown_432_address() const { return address + 432; }
  // the plate's texture (the group's +1684 or +2644)
  uint32_t texture() const { return guest::LoadU32(base, address + 456); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t texture_address() const { return address + 456; }
};

// embedded; measure: Morph, Morph::Run.r4, Morph::SetUp.r4
struct Morph : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  // the base shape's first stream ([shape + 400] + 16)
  uint32_t base_stream() const { return guest::LoadU32(base, address + 0); }
  void set_base_stream(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t base_stream_address() const { return address + 0; }
  // its second ([shape + 404] + 16)
  uint32_t base_stream2() const { return guest::LoadU32(base, address + 4); }
  void set_base_stream2(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t base_stream2_address() const { return address + 4; }
  // the first output (Morph::Run)
  template <typename T = MorphStream>
  T out1() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_out1(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_out1(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t out1_address() const { return address + 8; }
  // the second
  template <typename T = MorphStream>
  T out2() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_out2(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_out2(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t out2_address() const { return address + 12; }
  // up to seven targets' first streams
  uint32_t targets_address() const { return address + 16; }
  uint32_t target0() const { return guest::LoadU32(base, address + 16); }
  void set_target0(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t target0_address() const { return address + 16; }
  // the seventh target's (Morph::SetUp reads it back)
  uint32_t target6() const { return guest::LoadU32(base, address + 40); }
  void set_target6(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t target6_address() const { return address + 40; }
  // the base's second stream, once a target
  uint32_t target_seconds_address() const { return address + 48; }
  template <typename T = MorphStream>
  T target_second0() const { return T(base, guest::LoadU32(base, address + 48)); }
  void set_target_second0(const View& v) const { guest::StoreU32(base, address + 48, v.address); }
  void set_target_second0(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t target_second0_address() const { return address + 48; }
  // the targets' weights
  uint32_t weights_address() const { return address + 80; }
  float weight0() const { return guest::LoadFloat(base, address + 80); }
  void set_weight0(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t weight0_address() const { return address + 80; }
  // the targets (8 and above taken as 7)
  uint32_t count() const { return guest::LoadU32(base, address + 112); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t count_address() const { return address + 112; }
  // [[shape + 400]]: the vertices (16 bytes each)
  uint32_t vertices() const { return guest::LoadU32(base, address + 116); }
  void set_vertices(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t vertices_address() const { return address + 116; }
  // [[shape + 404] + 4]: 6 Blend6, 7 Blend7, else 8 (sub_8258B7E0, never ran)
  uint32_t kind() const { return guest::LoadU32(base, address + 120); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t kind_address() const { return address + 120; }
};

// embedded; measure: Morph.+12, Morph.+48
struct MorphStream : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // sub_8258B7E0 (never ran; not read in detail)
  float unknown_28() const { return guest::LoadFloat(base, address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
};

// The image words docs/functions/second-set.md's code uses.
struct SecondSetGlobals {
  // the car scene (its second set at +4944)
  template <typename T = CarScene>
  static T scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // the first set of scene pools (ScenePools::ReleaseA's r3)
  template <typename T = ScenePools>
  static T first_set(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E40u)); }
  static void set_first_set(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E40u, v.address); }
  static void set_first_set(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E40u, v); }
  static constexpr uint32_t first_set_address() { return 0x826C2E40u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 3.0 (the traffic body's lights' scale)
  static float three(uint8_t* base) { return guest::LoadFloat(base, 0x82006420u); }
  static void set_three(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006420u, v); }
  static constexpr uint32_t three_address() { return 0x82006420u; }
  // 5.0 (the parts' scale)
  static float five(uint8_t* base) { return guest::LoadFloat(base, 0x82005FE4u); }
  static void set_five(uint8_t* base, float v) { guest::StoreFloat(base, 0x82005FE4u, v); }
  static constexpr uint32_t five_address() { return 0x82005FE4u; }
  // 1/9 (probably): the brakes' morph weight a step
  static float brake_step(uint8_t* base) { return guest::LoadFloat(base, 0x82006634u); }
  static void set_brake_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006634u, v); }
  static constexpr uint32_t brake_step_address() { return 0x82006634u; }
  // 24 bytes an entry by the block (+1080 for traffic): the tire's index, value and float, front then rear
  static constexpr uint32_t tire_table_address() { return 0x82003EF8u; }
  // 44 bytes an entry by the block (+1980 for traffic)
  static constexpr uint32_t index_table_address() { return 0x820044B0u; }
  // the parts', interior's, wheels' and traffic body's name tables (string pointers, to 0x82602A1B)
  static constexpr uint32_t parts_names_address() { return 0x826025F4u; }
  // the cockpit's name tables (to 0x8200527B)
  static constexpr uint32_t cockpit_names_address() { return 0x82005238u; }
  // the light kit's three 18-name tables (to 0x820056C7)
  static constexpr uint32_t light_names_address() { return 0x820055F0u; }
};

}  // namespace schema
