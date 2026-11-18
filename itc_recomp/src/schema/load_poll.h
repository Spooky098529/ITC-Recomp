// The views of docs/functions/load-poll.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/load-poll.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: SceneParts
struct SceneParts : View {
  using View::View;
  static constexpr uint32_t kSize = 37296;
  static constexpr uint32_t kVtable = 0x82006024;
  // 0x82006024
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the PoolNode's list
  uint32_t list() const { return guest::LoadU32(base, address + 4); }
  void set_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t list_address() const { return address + 4; }
  // the PoolNode's links
  uint32_t link_8() const { return guest::LoadU32(base, address + 8); }
  void set_link_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t link_8_address() const { return address + 8; }
  uint32_t link_12() const { return guest::LoadU32(base, address + 12); }
  void set_link_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t link_12_address() const { return address + 12; }
  // 3 in use
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // its index in the pool (-1 from the constructor)
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  // the second set's car model (pool 0; SceneParts::Link): its +24 the first set's car model
  template <typename T = CarModelsSceneCarModel>
  T scene_car_model() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_scene_car_model(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_scene_car_model(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t scene_car_model_address() const { return address + 24; }
  // the car model's loading object's +3760, the node file's skeleton (SceneParts::Link)
  uint32_t skeleton() const { return guest::LoadU32(base, address + 28); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t skeleton_address() const { return address + 28; }
  // the 17 part models by kind (+32 + 4k; CarParts::Part, SceneParts::Build; cleared by Reset): PartModel, material-passes.md
  uint32_t part_model_0() const { return guest::LoadU32(base, address + 32); }
  void set_part_model_0(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t part_model_0_address() const { return address + 32; }
  uint32_t part_model_1() const { return guest::LoadU32(base, address + 36); }
  void set_part_model_1(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t part_model_1_address() const { return address + 36; }
  uint32_t part_model_2() const { return guest::LoadU32(base, address + 40); }
  void set_part_model_2(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t part_model_2_address() const { return address + 40; }
  uint32_t part_model_3() const { return guest::LoadU32(base, address + 44); }
  void set_part_model_3(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t part_model_3_address() const { return address + 44; }
  // 0 without the call when the record's +124 is 13
  uint32_t part_model_4() const { return guest::LoadU32(base, address + 48); }
  void set_part_model_4(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t part_model_4_address() const { return address + 48; }
  uint32_t part_model_5() const { return guest::LoadU32(base, address + 52); }
  void set_part_model_5(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t part_model_5_address() const { return address + 52; }
  uint32_t part_model_6() const { return guest::LoadU32(base, address + 56); }
  void set_part_model_6(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t part_model_6_address() const { return address + 56; }
  uint32_t part_model_7() const { return guest::LoadU32(base, address + 60); }
  void set_part_model_7(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t part_model_7_address() const { return address + 60; }
  uint32_t part_model_8() const { return guest::LoadU32(base, address + 64); }
  void set_part_model_8(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t part_model_8_address() const { return address + 64; }
  uint32_t part_model_9() const { return guest::LoadU32(base, address + 68); }
  void set_part_model_9(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t part_model_9_address() const { return address + 68; }
  uint32_t part_model_10() const { return guest::LoadU32(base, address + 72); }
  void set_part_model_10(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t part_model_10_address() const { return address + 72; }
  uint32_t part_model_11() const { return guest::LoadU32(base, address + 76); }
  void set_part_model_11(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t part_model_11_address() const { return address + 76; }
  uint32_t part_model_12() const { return guest::LoadU32(base, address + 80); }
  void set_part_model_12(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t part_model_12_address() const { return address + 80; }
  uint32_t part_model_13() const { return guest::LoadU32(base, address + 84); }
  void set_part_model_13(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t part_model_13_address() const { return address + 84; }
  uint32_t part_model_14() const { return guest::LoadU32(base, address + 88); }
  void set_part_model_14(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t part_model_14_address() const { return address + 88; }
  // the measure's SceneParts.+92 (its +452 the selected texture)
  uint32_t part_model_15() const { return guest::LoadU32(base, address + 92); }
  void set_part_model_15(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t part_model_15_address() const { return address + 92; }
  uint32_t part_model_16() const { return guest::LoadU32(base, address + 96); }
  void set_part_model_16(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t part_model_16_address() const { return address + 96; }
  // CarParts::IsLoaded's answer (SceneParts::Finish); 1 once loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 100); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t loaded_address() const { return address + 100; }
  // the first set's parts (CarParts, scene-loaders.md; pool 1 of [0x826C2E40])
  uint32_t first_set_parts() const { return guest::LoadU32(base, address + 104); }
  void set_first_set_parts(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t first_set_parts_address() const { return address + 104; }
  // the 60-byte parts record (SceneParts::SetUp, Apply); its words pick each kind's part
  uint32_t record_address() const { return address + 108; }
  // kind 0's CarParts::Part r5 (and kind 1's r6)
  uint32_t choice_0() const { return guest::LoadU32(base, address + 108); }
  void set_choice_0(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t choice_0_address() const { return address + 108; }
  // kind 1's r5
  uint32_t choice_1() const { return guest::LoadU32(base, address + 112); }
  void set_choice_1(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t choice_1_address() const { return address + 112; }
  // kind 2's
  uint32_t choice_2() const { return guest::LoadU32(base, address + 116); }
  void set_choice_2(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t choice_2_address() const { return address + 116; }
  // kind 3's
  uint32_t choice_3() const { return guest::LoadU32(base, address + 120); }
  void set_choice_3(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t choice_3_address() const { return address + 120; }
  // kind 4's; 13: no part
  uint32_t choice_4() const { return guest::LoadU32(base, address + 124); }
  void set_choice_4(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t choice_4_address() const { return address + 124; }
  // kind 5's
  uint32_t choice_5() const { return guest::LoadU32(base, address + 128); }
  void set_choice_5(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t choice_5_address() const { return address + 128; }
  // kinds 6 and 7's r5, 8 and 9's r6
  uint32_t choice_6() const { return guest::LoadU32(base, address + 132); }
  void set_choice_6(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t choice_6_address() const { return address + 132; }
  // kinds 8 and 9's r5
  uint32_t choice_8() const { return guest::LoadU32(base, address + 136); }
  void set_choice_8(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t choice_8_address() const { return address + 136; }
  // kind 10's
  uint32_t choice_10() const { return guest::LoadU32(base, address + 140); }
  void set_choice_10(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t choice_10_address() const { return address + 140; }
  // kind 11's
  uint32_t choice_11() const { return guest::LoadU32(base, address + 144); }
  void set_choice_11(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t choice_11_address() const { return address + 144; }
  // kind 12's
  uint32_t choice_12() const { return guest::LoadU32(base, address + 148); }
  void set_choice_12(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t choice_12_address() const { return address + 148; }
  // kind 14's
  uint32_t choice_14() const { return guest::LoadU32(base, address + 152); }
  void set_choice_14(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t choice_14_address() const { return address + 152; }
  // kind 15's
  uint32_t choice_15() const { return guest::LoadU32(base, address + 156); }
  void set_choice_15(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t choice_15_address() const { return address + 156; }
  // the point block (PartsPoints, second-set.md; groups by part in known-data.md)
  uint32_t points_address() const { return address + 176; }
  // the point block's +27488, in the headlights' group (SceneParts::LightValue)
  float light_value() const { return guest::LoadFloat(base, address + 27664); }
  void set_light_value(float v) const { guest::StoreFloat(base, address + 27664, v); }
  uint32_t light_value_address() const { return address + 27664; }
  // the point block's +37076: the first set's car model's car record (its +32; SceneParts::Build)
  uint32_t points_car_record() const { return guest::LoadU32(base, address + 37252); }
  void set_points_car_record(uint32_t v) const { guest::StoreU32(base, address + 37252, v); }
  uint32_t points_car_record_address() const { return address + 37252; }
  // the point block's +37080: the record (+108)
  uint32_t points_record() const { return guest::LoadU32(base, address + 37256); }
  void set_points_record(uint32_t v) const { guest::StoreU32(base, address + 37256, v); }
  uint32_t points_record_address() const { return address + 37256; }
  // the point block's +37084: the second set's car model (+24)
  uint32_t points_scene_car_model() const { return guest::LoadU32(base, address + 37260); }
  void set_points_scene_car_model(uint32_t v) const { guest::StoreU32(base, address + 37260, v); }
  uint32_t points_scene_car_model_address() const { return address + 37260; }
  // the point block's +37092: the first set's car model's skeleton (its +3760)
  uint32_t points_skeleton() const { return guest::LoadU32(base, address + 37268); }
  void set_points_skeleton(uint32_t v) const { guest::StoreU32(base, address + 37268, v); }
  uint32_t points_skeleton_address() const { return address + 37268; }
  // 0 from SceneParts::Reset
  uint32_t unknown_37280() const { return guest::LoadU32(base, address + 37280); }
  void set_unknown_37280(uint32_t v) const { guest::StoreU32(base, address + 37280, v); }
  uint32_t unknown_37280_address() const { return address + 37280; }
  // the car scene ([0x826C2E44]); its +4 the model set
  template <typename T = CarScene>
  T scene() const { return T(base, guest::LoadU32(base, address + 37284)); }
  void set_scene(const View& v) const { guest::StoreU32(base, address + 37284, v.address); }
  void set_scene(uint32_t v) const { guest::StoreU32(base, address + 37284, v); }
  uint32_t scene_address() const { return address + 37284; }
};

// embedded; measure: InteriorMeters
struct InteriorMeters : View {
  using View::View;
  static constexpr uint32_t kSize = 5360;
  // the interior's 120-byte record (SceneInterior +40): gauge i's entry at +8 + 12i (present, kind, texture index)
  template <typename T = InteriorRecord>
  T record() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_record(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t record_address() const { return address + 0; }
  // the car's block (SceneInterior::Finish); 5: gauge 1 is the turbo gauge
  uint32_t car_block() const { return guest::LoadU32(base, address + 4); }
  void set_car_block(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t car_block_address() const { return address + 4; }
  // gauge i's needle model (its point record's +4; InteriorMeters::SetGauge's PartModel::SetTexture)
  uint32_t needle0_model() const { return guest::LoadU32(base, address + 1700); }
  void set_needle0_model(uint32_t v) const { guest::StoreU32(base, address + 1700, v); }
  uint32_t needle0_model_address() const { return address + 1700; }
  // its shape (the record's +228)
  uint32_t needle0_shape() const { return guest::LoadU32(base, address + 1924); }
  void set_needle0_shape(uint32_t v) const { guest::StoreU32(base, address + 1924, v); }
  uint32_t needle0_shape_address() const { return address + 1924; }
  uint32_t needle1_model() const { return guest::LoadU32(base, address + 1940); }
  void set_needle1_model(uint32_t v) const { guest::StoreU32(base, address + 1940, v); }
  uint32_t needle1_model_address() const { return address + 1940; }
  uint32_t needle1_shape() const { return guest::LoadU32(base, address + 2164); }
  void set_needle1_shape(uint32_t v) const { guest::StoreU32(base, address + 2164, v); }
  uint32_t needle1_shape_address() const { return address + 2164; }
  uint32_t needle2_model() const { return guest::LoadU32(base, address + 2660); }
  void set_needle2_model(uint32_t v) const { guest::StoreU32(base, address + 2660, v); }
  uint32_t needle2_model_address() const { return address + 2660; }
  uint32_t needle2_shape() const { return guest::LoadU32(base, address + 2884); }
  void set_needle2_shape(uint32_t v) const { guest::StoreU32(base, address + 2884, v); }
  uint32_t needle2_shape_address() const { return address + 2884; }
  uint32_t needle3_model() const { return guest::LoadU32(base, address + 2900); }
  void set_needle3_model(uint32_t v) const { guest::StoreU32(base, address + 2900, v); }
  uint32_t needle3_model_address() const { return address + 2900; }
  uint32_t needle3_shape() const { return guest::LoadU32(base, address + 3124); }
  void set_needle3_shape(uint32_t v) const { guest::StoreU32(base, address + 3124, v); }
  uint32_t needle3_shape_address() const { return address + 3124; }
  uint32_t needle4_model() const { return guest::LoadU32(base, address + 3620); }
  void set_needle4_model(uint32_t v) const { guest::StoreU32(base, address + 3620, v); }
  uint32_t needle4_model_address() const { return address + 3620; }
  uint32_t needle4_shape() const { return guest::LoadU32(base, address + 3844); }
  void set_needle4_shape(uint32_t v) const { guest::StoreU32(base, address + 3844, v); }
  uint32_t needle4_shape_address() const { return address + 3844; }
  uint32_t needle5_model() const { return guest::LoadU32(base, address + 3860); }
  void set_needle5_model(uint32_t v) const { guest::StoreU32(base, address + 3860, v); }
  uint32_t needle5_model_address() const { return address + 3860; }
  uint32_t needle5_shape() const { return guest::LoadU32(base, address + 4084); }
  void set_needle5_shape(uint32_t v) const { guest::StoreU32(base, address + 4084, v); }
  uint32_t needle5_shape_address() const { return address + 4084; }
  uint32_t needle6_model() const { return guest::LoadU32(base, address + 4100); }
  void set_needle6_model(uint32_t v) const { guest::StoreU32(base, address + 4100, v); }
  uint32_t needle6_model_address() const { return address + 4100; }
  uint32_t needle6_shape() const { return guest::LoadU32(base, address + 4324); }
  void set_needle6_shape(uint32_t v) const { guest::StoreU32(base, address + 4324, v); }
  uint32_t needle6_shape_address() const { return address + 4324; }
  // the gauges' scene entries (the scene + 4864 + 20 x the record's kind; InteriorMeters::SetGauge): their texture providers
  uint32_t gauge0_provider() const { return guest::LoadU32(base, address + 5296); }
  void set_gauge0_provider(uint32_t v) const { guest::StoreU32(base, address + 5296, v); }
  uint32_t gauge0_provider_address() const { return address + 5296; }
  uint32_t gauge1_provider() const { return guest::LoadU32(base, address + 5300); }
  void set_gauge1_provider(uint32_t v) const { guest::StoreU32(base, address + 5300, v); }
  uint32_t gauge1_provider_address() const { return address + 5300; }
  uint32_t gauge2_provider() const { return guest::LoadU32(base, address + 5304); }
  void set_gauge2_provider(uint32_t v) const { guest::StoreU32(base, address + 5304, v); }
  uint32_t gauge2_provider_address() const { return address + 5304; }
  uint32_t gauge3_provider() const { return guest::LoadU32(base, address + 5308); }
  void set_gauge3_provider(uint32_t v) const { guest::StoreU32(base, address + 5308, v); }
  uint32_t gauge3_provider_address() const { return address + 5308; }
  uint32_t gauge4_provider() const { return guest::LoadU32(base, address + 5312); }
  void set_gauge4_provider(uint32_t v) const { guest::StoreU32(base, address + 5312, v); }
  uint32_t gauge4_provider_address() const { return address + 5312; }
  uint32_t gauge5_provider() const { return guest::LoadU32(base, address + 5316); }
  void set_gauge5_provider(uint32_t v) const { guest::StoreU32(base, address + 5316, v); }
  uint32_t gauge5_provider_address() const { return address + 5316; }
  uint32_t gauge6_provider() const { return guest::LoadU32(base, address + 5320); }
  void set_gauge6_provider(uint32_t v) const { guest::StoreU32(base, address + 5320, v); }
  uint32_t gauge6_provider_address() const { return address + 5320; }
  // gauge 1's in mode 5 (SceneInterior +8636: the interior's texture provider, SceneInterior::Finish)
  uint32_t turbo_provider() const { return guest::LoadU32(base, address + 5324); }
  void set_turbo_provider(uint32_t v) const { guest::StoreU32(base, address + 5324, v); }
  uint32_t turbo_provider_address() const { return address + 5324; }
  // the needles' angle words (sub_820D3658: 0x82003600 + 16 x (2 x kind + (texture index >= 3)))
  uint32_t gauge0_angle() const { return guest::LoadU32(base, address + 5328); }
  void set_gauge0_angle(uint32_t v) const { guest::StoreU32(base, address + 5328, v); }
  uint32_t gauge0_angle_address() const { return address + 5328; }
  uint32_t gauge1_angle() const { return guest::LoadU32(base, address + 5332); }
  void set_gauge1_angle(uint32_t v) const { guest::StoreU32(base, address + 5332, v); }
  uint32_t gauge1_angle_address() const { return address + 5332; }
  uint32_t gauge2_angle() const { return guest::LoadU32(base, address + 5336); }
  void set_gauge2_angle(uint32_t v) const { guest::StoreU32(base, address + 5336, v); }
  uint32_t gauge2_angle_address() const { return address + 5336; }
  uint32_t gauge3_angle() const { return guest::LoadU32(base, address + 5340); }
  void set_gauge3_angle(uint32_t v) const { guest::StoreU32(base, address + 5340, v); }
  uint32_t gauge3_angle_address() const { return address + 5340; }
  uint32_t gauge4_angle() const { return guest::LoadU32(base, address + 5344); }
  void set_gauge4_angle(uint32_t v) const { guest::StoreU32(base, address + 5344, v); }
  uint32_t gauge4_angle_address() const { return address + 5344; }
  uint32_t gauge5_angle() const { return guest::LoadU32(base, address + 5348); }
  void set_gauge5_angle(uint32_t v) const { guest::StoreU32(base, address + 5348, v); }
  uint32_t gauge5_angle_address() const { return address + 5348; }
  uint32_t gauge6_angle() const { return guest::LoadU32(base, address + 5352); }
  void set_gauge6_angle(uint32_t v) const { guest::StoreU32(base, address + 5352, v); }
  uint32_t gauge6_angle_address() const { return address + 5352; }
};

// The image words docs/functions/load-poll.md's code uses.
struct LoadPollGlobals {
  // the car scene: +4 the model set (0 front end, 1 race), +208 and +296 the tires' and calipers' texture providers, +1064 and +1132 the digits'
  template <typename T = CarScene>
  static T scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
};

}  // namespace schema
