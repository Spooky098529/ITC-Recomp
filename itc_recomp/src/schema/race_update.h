// The views of docs/functions/race-update.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-update.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: RaceScene
struct RaceScene : View {
  using View::View;
  static constexpr uint32_t kSize = 31168;
  // the course scenery's manager ([0x826C551C], set by RaceScene::LoadScenery)
  template <typename T = Scenery>
  T scenery() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_scenery(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_scenery(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t scenery_address() const { return address + 0; }
  // the car slots' count (7); Car::TickUpdate's r3 is this word's address
  uint32_t car_slot_count() const { return guest::LoadU32(base, address + 16); }
  void set_car_slot_count(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t car_slot_count_address() const { return address + 16; }
  // seven RaceCarSlots of 4,288 bytes, by vehicle index
  uint32_t car_slots_address() const { return address + 32; }
  // the light set (RaceLights)
  uint32_t lights_address() const { return address + 30064; }
  // the race's screen object (1024 x 576; 0 when not made)
  template <typename T = ScreenObject>
  T screen_object() const { return T(base, guest::LoadU32(base, address + 31152)); }
  void set_screen_object(const View& v) const { guest::StoreU32(base, address + 31152, v.address); }
  void set_screen_object(uint32_t v) const { guest::StoreU32(base, address + 31152, v); }
  uint32_t screen_object_address() const { return address + 31152; }
  // split screen: player 1's screen object
  template <typename T = ScreenObject>
  T screen_object_first() const { return T(base, guest::LoadU32(base, address + 31156)); }
  void set_screen_object_first(const View& v) const { guest::StoreU32(base, address + 31156, v.address); }
  void set_screen_object_first(uint32_t v) const { guest::StoreU32(base, address + 31156, v); }
  uint32_t screen_object_first_address() const { return address + 31156; }
  // split screen: player 2's screen object
  template <typename T = ScreenObject>
  T screen_object_second() const { return T(base, guest::LoadU32(base, address + 31160)); }
  void set_screen_object_second(const View& v) const { guest::StoreU32(base, address + 31160, v.address); }
  void set_screen_object_second(uint32_t v) const { guest::StoreU32(base, address + 31160, v); }
  uint32_t screen_object_second_address() const { return address + 31160; }
  // measure fault: Car::TickUpdate's halfword +0 of the car (the racer record, race-setup's Car: the paint pick), joined with the scene
  uint16_t fault_car_pick() const { return guest::LoadU16(base, address + 0); }
  void set_fault_car_pick(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_car_pick_address() const { return address + 0; }
  // measure fault: RaceScene::VaryCar's r3 is a car (the racer record, race-setup's Car), its word +12 (64 picks the +1580 byte), joined with the scene
  uint32_t fault_car_12() const { return guest::LoadU32(base, address + 12); }
  void set_fault_car_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fault_car_12_address() const { return address + 12; }
};

// embedded; measure: RaceScene::RemakeCarModel.r5
struct RaceCarSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4288;
  // its pool 7 scene model (ScenePools::SetUpB's; 0 none)
  template <typename T = SceneModel>
  T model() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_model(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_model(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t model_address() const { return address + 0; }
  // its scene model record (car-makers.toml's SceneModelRecord, from SceneModelRecord::FromCar)
  uint32_t record_address() const { return address + 16; }
  // the record's +0: 3 traffic
  uint32_t record_kind() const { return guest::LoadU32(base, address + 16); }
  void set_record_kind(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t record_kind_address() const { return address + 16; }
  // the record's +16 (the model block): 0 or 7 on traffic sets the block's +592
  uint32_t record_block() const { return guest::LoadU32(base, address + 32); }
  void set_record_block(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t record_block_address() const { return address + 32; }
  // its pool 7 index (ScenePools::ClaimB; -1 none)
  int32_t pool_index() const { return int32_t(guest::LoadU32(base, address + 4272)); }
  void set_pool_index(int32_t v) const { guest::StoreU32(base, address + 4272, uint32_t(v)); }
  uint32_t pool_index_address() const { return address + 4272; }
};

// embedded; measure: RaceLights
struct RaceLights : View {
  using View::View;
  static constexpr uint32_t kSize = 1076;
  // the entries of kind 1 or 2 (11)
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // 3
  uint32_t kind1_count() const { return guest::LoadU32(base, address + 4); }
  void set_kind1_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind1_count_address() const { return address + 4; }
  // 8
  uint32_t kind2_count() const { return guest::LoadU32(base, address + 8); }
  void set_kind2_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t kind2_count_address() const { return address + 8; }
  // eleven RaceLightEntries of 96 bytes
  uint32_t entries_address() const { return address + 16; }
  // 0 at SetUp
  uint32_t unknown_1072() const { return guest::LoadU32(base, address + 1072); }
  void set_unknown_1072(uint32_t v) const { guest::StoreU32(base, address + 1072, v); }
  uint32_t unknown_1072_address() const { return address + 1072; }
};

// embedded; measure: -
struct RaceLightEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  // 1 directional (entries 0 to 2), 2 (3 to 10; probably spot lights)
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // the direction (earlier taken for the color): normalized into the scene lights' 0x8296B950 by RaceLights::Update
  uint32_t direction_address() const { return address + 16; }
  // copied to 0x8296B980
  uint32_t second_address() const { return address + 32; }
  // copied to 0x8296B9B0
  uint32_t third_address() const { return address + 48; }
  // kind 2
  uint32_t fourth_address() const { return address + 64; }
  // kind 2: the cosines of two angles (probably the cone's outer and inner)
  uint32_t cone_address() const { return address + 80; }
};

// The image words docs/functions/race-update.md's code uses.
struct RaceUpdateGlobals {
  // the race data: +4 the course, +32 the mode, +48 the racer records (3,936 bytes), +0x59614 the human cars, +0x59620 the stages, +0x59AE8 the racer records' list, +0xA3AF0 the split mode (4 split screen)
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race's scenery configurations' two words, 8 bytes an entry (0 to 2 by mode, 3 courses 8 to 10)
  static constexpr uint32_t scenery_configs_address() { return 0x82602EA0u; }
  // the scene lights' directions, 16 bytes a kind 1 entry
  static constexpr uint32_t light_directions_address() { return 0x8296B950u; }
  // the kind 1 entries' +32
  static constexpr uint32_t light_second_address() { return 0x8296B980u; }
  // the kind 1 entries' +48
  static constexpr uint32_t light_third_address() { return 0x8296B9B0u; }
  // (0, 0, 0, 1) from RaceLights::Update
  static constexpr uint32_t light_origin_address() { return 0x8265ED00u; }
  // camera pass 0 (cameras.toml's CameraPass): +400 and +416 the two points Car::TickUpdate takes a direction of
  static constexpr uint32_t camera_pass0_address() { return 0x826BAC00u; }
  // the car scene (car-draw.toml's CarScene)
  template <typename T = CarScene>
  static T car_scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_car_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_car_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t car_scene_address() { return 0x826C2E44u; }
  // the racers' manager: +2308 the racers
  template <typename T = RaceManager>
  static T racers(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82F4u)); }
  static void set_racers(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82F4u, v.address); }
  static void set_racers(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_address() { return 0x826D82F4u; }
  // the viewports (camera-modes.toml's Viewports): +8 nonzero in split screen
  template <typename T = Viewports>
  static T viewports(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2CE8u)); }
  static void set_viewports(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2CE8u, v.address); }
  static void set_viewports(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // the vehicles' input records' pointers by vehicle index (+0 the flags: 0x8 brake, 0x100 high beams)
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // 0 for player 2 followed (two human cars)
  static uint32_t followed(uint8_t* base) { return guest::LoadU32(base, 0x826C7824u); }
  static void set_followed(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7824u, v); }
  static constexpr uint32_t followed_address() { return 0x826C7824u; }
  // 2.0 (probably): the wheels' load scale
  static float wheel_load_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_wheel_load_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t wheel_load_scale_address() { return 0x820021E8u; }
  // a wheel's slip above it: level 2 (not read from the image)
  static float slip_high(uint8_t* base) { return guest::LoadFloat(base, 0x8200979Cu); }
  static void set_slip_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200979Cu, v); }
  static constexpr uint32_t slip_high_address() { return 0x8200979Cu; }
  // a wheel's slip above it: level 1 (not read from the image)
  static float slip_low(uint8_t* base) { return guest::LoadFloat(base, 0x820097A0u); }
  static void set_slip_low(uint8_t* base, float v) { guest::StoreFloat(base, 0x820097A0u, v); }
  static constexpr uint32_t slip_low_address() { return 0x820097A0u; }
  // 1/300: the screen object's speed from the km/h
  static float speed_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820086ECu); }
  static void set_speed_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820086ECu, v); }
  static constexpr uint32_t speed_scale_address() { return 0x820086ECu; }
  // 1.0 (copied as a word by RaceLights::Update and UploadDirectional)
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
};

}  // namespace schema
