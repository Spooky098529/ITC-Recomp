// The views of docs/functions/stand-models.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/stand-models.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: GarageStage, GarageLights
struct GarageStage : View {
  using View::View;
  static constexpr uint32_t kSize = 6208;
  // the stage's index (0 to 2, GarageStage::Init). Measure fault: a holder block's +0, the stage pointer, is filed here
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
  // -1 when nothing is pending (GarageStage::IsBusy), else the model block GarageStage::ReloadModel remakes the scene model for
  uint32_t pending_block() const { return guest::LoadU32(base, address + 4); }
  void set_pending_block(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t pending_block_address() const { return address + 4; }
  // the car's scene model's index in pool 7 of the car scene (-1 none)
  uint32_t model_index() const { return guest::LoadU32(base, address + 8); }
  void set_model_index(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t model_index_address() const { return address + 8; }
  // 0.0 from GarageStage::Init
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // set while it holds a claimed model (GarageStage::MakeModel sets 1; ReloadModel releases it first)
  uint32_t model_claimed() const { return guest::LoadU32(base, address + 16); }
  void set_model_claimed(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t model_claimed_address() const { return address + 16; }
  // measure fault: GarageLights::Init and SetMode's eleven 96-byte light records and SetLights' reads (r3 the lights, stage + 4512: its +16 to +111), and ResetPose's vectors (r3 stage + 32), filed at the stage's own offsets
  uint32_t joined_records_address() const { return address + 16; }
  // a value opened and closed by animation types 2 and 3 (probably the bonnet), copied to the scene model's +4928
  float bonnet() const { return guest::LoadFloat(base, address + 32); }
  void set_bonnet(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t bonnet_address() const { return address + 32; }
  // +48 to +63 copied to the scene model's +4944 and +4960 (+52, +56 and +96 opened and closed by animation types 4 and 5, probably the doors)
  float open_x() const { return guest::LoadFloat(base, address + 48); }
  void set_open_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t open_x_address() const { return address + 48; }
  float open_y() const { return guest::LoadFloat(base, address + 52); }
  void set_open_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t open_y_address() const { return address + 52; }
  float open_z() const { return guest::LoadFloat(base, address + 56); }
  void set_open_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t open_z_address() const { return address + 56; }
  // +80 to +95 the car's angles (from a preset's +48, GarageStage::ApplyPreset)
  float angle_x() const { return guest::LoadFloat(base, address + 80); }
  void set_angle_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t angle_x_address() const { return address + 80; }
  // the car's yaw, turned 0.5 degrees a call (GarageStage::Draw)
  float yaw() const { return guest::LoadFloat(base, address + 84); }
  void set_yaw(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t yaw_address() const { return address + 84; }
  float angle_z() const { return guest::LoadFloat(base, address + 88); }
  void set_angle_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t angle_z_address() const { return address + 88; }
  // the doors' word (animation types 4 and 5; 0 from ResetPose)
  uint32_t doors() const { return guest::LoadU32(base, address + 96); }
  void set_doors(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t doors_address() const { return address + 96; }
  // the garage camera (docs/known-data.md "garage camera"; GarageCamera::Init, Update). Also covers the lights' records +304 to +399 and +1072 to +1087 the measure files at the stage's offsets (GarageLights::Init, SetLights)
  uint32_t camera_address() const { return address + 112; }
  // the camera's +0: 0 until the presets are loaded (GarageStage::Draw does nothing until then)
  uint32_t camera_set_up() const { return guest::LoadU32(base, address + 112); }
  void set_camera_set_up(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t camera_set_up_address() const { return address + 112; }
  // the camera's +100 (a preset's +4)
  float camera_elevation() const { return guest::LoadFloat(base, address + 212); }
  void set_camera_elevation(float v) const { guest::StoreFloat(base, address + 212, v); }
  uint32_t camera_elevation_address() const { return address + 212; }
  // the camera's +108: 640 / tan(field of view)
  float camera_zoom() const { return guest::LoadFloat(base, address + 220); }
  void set_camera_zoom(float v) const { guest::StoreFloat(base, address + 220, v); }
  uint32_t camera_zoom_address() const { return address + 220; }
  // the camera's +176
  float camera_orbit_angle() const { return guest::LoadFloat(base, address + 288); }
  void set_camera_orbit_angle(float v) const { guest::StoreFloat(base, address + 288, v); }
  uint32_t camera_orbit_angle_address() const { return address + 288; }
  // the camera's +180 (a preset's +8)
  float camera_distance() const { return guest::LoadFloat(base, address + 292); }
  void set_camera_distance(float v) const { guest::StoreFloat(base, address + 292, v); }
  uint32_t camera_distance_address() const { return address + 292; }
  // the camera's +224, degrees (a preset's +12)
  float camera_fov() const { return guest::LoadFloat(base, address + 336); }
  void set_camera_fov(float v) const { guest::StoreFloat(base, address + 336, v); }
  uint32_t camera_fov_address() const { return address + 336; }
  // the camera's +292: the presets' index table (0x82619428, 290 slots a car)
  uint32_t camera_preset_table() const { return guest::LoadU32(base, address + 404); }
  void set_camera_preset_table(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t camera_preset_table_address() const { return address + 404; }
  // the camera's +4392
  uint32_t camera_preset_count() const { return guest::LoadU32(base, address + 4504); }
  void set_camera_preset_count(uint32_t v) const { guest::StoreU32(base, address + 4504, v); }
  uint32_t camera_preset_count_address() const { return address + 4504; }
  // the lights (GarageLights::Init, SetMode, GarageStage::SetLights): +16 + 96i eleven records (+32 + 96i three lights, +304 + 96i four spots), +1072 the 16 bytes for 0x8265ED00
  uint32_t lights_address() const { return address + 4512; }
  // 16 entries of 28 bytes (+0 the type, +4 the steps, +8 a word, +12 the value's address, +16 the scene model's index, +20 done, +24 a float)
  uint32_t animations_address() const { return address + 5600; }
  uint32_t animation_count() const { return guest::LoadU32(base, address + 6048); }
  void set_animation_count(uint32_t v) const { guest::StoreU32(base, address + 6048, v); }
  uint32_t animation_count_address() const { return address + 6048; }
  // the reflection drawn (two passes); 1 from Init
  uint32_t reflection() const { return guest::LoadU32(base, address + 6052); }
  void set_reflection(uint32_t v) const { guest::StoreU32(base, address + 6052, v); }
  uint32_t reflection_address() const { return address + 6052; }
  // the scene model's draw word (its +4296)
  uint32_t draw_word() const { return guest::LoadU32(base, address + 6056); }
  void set_draw_word(uint32_t v) const { guest::StoreU32(base, address + 6056, v); }
  uint32_t draw_word_address() const { return address + 6056; }
  // the light kit switched off every frame (GarageStage::SetDark)
  uint32_t dark() const { return guest::LoadU32(base, address + 6060); }
  void set_dark(uint32_t v) const { guest::StoreU32(base, address + 6060, v); }
  uint32_t dark_address() const { return address + 6060; }
  // +6064 to +6084 0 from Init
  uint32_t unknown_6064() const { return guest::LoadU32(base, address + 6064); }
  void set_unknown_6064(uint32_t v) const { guest::StoreU32(base, address + 6064, v); }
  uint32_t unknown_6064_address() const { return address + 6064; }
  uint32_t unknown_6068() const { return guest::LoadU32(base, address + 6068); }
  void set_unknown_6068(uint32_t v) const { guest::StoreU32(base, address + 6068, v); }
  uint32_t unknown_6068_address() const { return address + 6068; }
  uint32_t unknown_6072() const { return guest::LoadU32(base, address + 6072); }
  void set_unknown_6072(uint32_t v) const { guest::StoreU32(base, address + 6072, v); }
  uint32_t unknown_6072_address() const { return address + 6072; }
  uint32_t unknown_6076() const { return guest::LoadU32(base, address + 6076); }
  void set_unknown_6076(uint32_t v) const { guest::StoreU32(base, address + 6076, v); }
  uint32_t unknown_6076_address() const { return address + 6076; }
  uint32_t unknown_6080() const { return guest::LoadU32(base, address + 6080); }
  void set_unknown_6080(uint32_t v) const { guest::StoreU32(base, address + 6080, v); }
  uint32_t unknown_6080_address() const { return address + 6080; }
  uint32_t unknown_6084() const { return guest::LoadU32(base, address + 6084); }
  void set_unknown_6084(uint32_t v) const { guest::StoreU32(base, address + 6084, v); }
  uint32_t unknown_6084_address() const { return address + 6084; }
  // a camera preset waiting (-1 none)
  uint32_t waiting_preset() const { return guest::LoadU32(base, address + 6088); }
  void set_waiting_preset(uint32_t v) const { guest::StoreU32(base, address + 6088, v); }
  uint32_t waiting_preset_address() const { return address + 6088; }
  // the number plates shown (1 from Init; GarageStage::SetPlates)
  uint32_t plates_shown() const { return guest::LoadU32(base, address + 6092); }
  void set_plates_shown(uint32_t v) const { guest::StoreU32(base, address + 6092, v); }
  uint32_t plates_shown_address() const { return address + 6092; }
  // the car data's +1736 (GarageStage::FitCar)
  float front_axle_height() const { return guest::LoadFloat(base, address + 6096); }
  void set_front_axle_height(float v) const { guest::StoreFloat(base, address + 6096, v); }
  uint32_t front_axle_height_address() const { return address + 6096; }
  // the car data's +1756
  float rear_axle_height() const { return guest::LoadFloat(base, address + 6100); }
  void set_rear_axle_height(float v) const { guest::StoreFloat(base, address + 6100, v); }
  uint32_t rear_axle_height_address() const { return address + 6100; }
  // the front axle record's +52
  float front_wheel_height() const { return guest::LoadFloat(base, address + 6104); }
  void set_front_wheel_height(float v) const { guest::StoreFloat(base, address + 6104, v); }
  uint32_t front_wheel_height_address() const { return address + 6104; }
  // the rear axle record's +52
  float rear_wheel_height() const { return guest::LoadFloat(base, address + 6108); }
  void set_rear_wheel_height(float v) const { guest::StoreFloat(base, address + 6108, v); }
  uint32_t rear_wheel_height_address() const { return address + 6108; }
  // the front axle record's +208 (probably)
  float front_camber() const { return guest::LoadFloat(base, address + 6112); }
  void set_front_camber(float v) const { guest::StoreFloat(base, address + 6112, v); }
  uint32_t front_camber_address() const { return address + 6112; }
  float rear_camber() const { return guest::LoadFloat(base, address + 6116); }
  void set_rear_camber(float v) const { guest::StoreFloat(base, address + 6116, v); }
  uint32_t rear_camber_address() const { return address + 6116; }
  // the front axle record's +212 (probably)
  float front_toe() const { return guest::LoadFloat(base, address + 6120); }
  void set_front_toe(float v) const { guest::StoreFloat(base, address + 6120, v); }
  uint32_t front_toe_address() const { return address + 6120; }
  float rear_toe() const { return guest::LoadFloat(base, address + 6124); }
  void set_rear_toe(float v) const { guest::StoreFloat(base, address + 6124, v); }
  uint32_t rear_toe_address() const { return address + 6124; }
  // sin(+208 x [0x82002948]) x +52
  float front_camber_offset() const { return guest::LoadFloat(base, address + 6128); }
  void set_front_camber_offset(float v) const { guest::StoreFloat(base, address + 6128, v); }
  uint32_t front_camber_offset_address() const { return address + 6128; }
  float rear_camber_offset() const { return guest::LoadFloat(base, address + 6132); }
  void set_rear_camber_offset(float v) const { guest::StoreFloat(base, address + 6132, v); }
  uint32_t rear_camber_offset_address() const { return address + 6132; }
  // +6104 - +6096 (probably the front height)
  float front_height() const { return guest::LoadFloat(base, address + 6136); }
  void set_front_height(float v) const { guest::StoreFloat(base, address + 6136, v); }
  uint32_t front_height_address() const { return address + 6136; }
  // +6108 - +6100
  float rear_height() const { return guest::LoadFloat(base, address + 6140); }
  void set_rear_height(float v) const { guest::StoreFloat(base, address + 6140, v); }
  uint32_t rear_height_address() const { return address + 6140; }
  // GarageStage::PoseCar
  float mean_height() const { return guest::LoadFloat(base, address + 6144); }
  void set_mean_height(float v) const { guest::StoreFloat(base, address + 6144, v); }
  uint32_t mean_height_address() const { return address + 6144; }
  // GarageStage::PoseCar
  float pitch() const { return guest::LoadFloat(base, address + 6148); }
  void set_pitch(float v) const { guest::StoreFloat(base, address + 6148, v); }
  uint32_t pitch_address() const { return address + 6148; }
  // the car data's +4
  float wheelbase() const { return guest::LoadFloat(base, address + 6152); }
  void set_wheelbase(float v) const { guest::StoreFloat(base, address + 6152, v); }
  uint32_t wheelbase_address() const { return address + 6152; }
  // the car data's +8 (probably)
  float front_track() const { return guest::LoadFloat(base, address + 6156); }
  void set_front_track(float v) const { guest::StoreFloat(base, address + 6156, v); }
  uint32_t front_track_address() const { return address + 6156; }
  // the car data's +12 (probably)
  float rear_track() const { return guest::LoadFloat(base, address + 6160); }
  void set_rear_track(float v) const { guest::StoreFloat(base, address + 6160, v); }
  uint32_t rear_track_address() const { return address + 6160; }
  // +6176 to +6188 the front axle record's +0 to +8 and 1.0 (GarageStage::FitCar, FitAxles)
  float front_axle_x() const { return guest::LoadFloat(base, address + 6176); }
  void set_front_axle_x(float v) const { guest::StoreFloat(base, address + 6176, v); }
  uint32_t front_axle_x_address() const { return address + 6176; }
  float front_axle_y() const { return guest::LoadFloat(base, address + 6180); }
  void set_front_axle_y(float v) const { guest::StoreFloat(base, address + 6180, v); }
  uint32_t front_axle_y_address() const { return address + 6180; }
  // probably the front half track
  float front_axle_z() const { return guest::LoadFloat(base, address + 6184); }
  void set_front_axle_z(float v) const { guest::StoreFloat(base, address + 6184, v); }
  uint32_t front_axle_z_address() const { return address + 6184; }
  float front_axle_w() const { return guest::LoadFloat(base, address + 6188); }
  void set_front_axle_w(float v) const { guest::StoreFloat(base, address + 6188, v); }
  uint32_t front_axle_w_address() const { return address + 6188; }
  // +6192 to +6204 the rear axle record's +0 to +8 and 1.0
  float rear_axle_x() const { return guest::LoadFloat(base, address + 6192); }
  void set_rear_axle_x(float v) const { guest::StoreFloat(base, address + 6192, v); }
  uint32_t rear_axle_x_address() const { return address + 6192; }
  float rear_axle_y() const { return guest::LoadFloat(base, address + 6196); }
  void set_rear_axle_y(float v) const { guest::StoreFloat(base, address + 6196, v); }
  uint32_t rear_axle_y_address() const { return address + 6196; }
  // probably the rear half track
  float rear_axle_z() const { return guest::LoadFloat(base, address + 6200); }
  void set_rear_axle_z(float v) const { guest::StoreFloat(base, address + 6200, v); }
  uint32_t rear_axle_z_address() const { return address + 6200; }
  float rear_axle_w() const { return guest::LoadFloat(base, address + 6204); }
  void set_rear_axle_w(float v) const { guest::StoreFloat(base, address + 6204, v); }
  uint32_t rear_axle_w_address() const { return address + 6204; }
  // GarageStage::Init's doubleword copies
  uint64_t front_axle_xy() const { return guest::LoadU64(base, address + 6176); }
  void set_front_axle_xy(uint64_t v) const { guest::StoreU64(base, address + 6176, v); }
  uint32_t front_axle_xy_address() const { return address + 6176; }
  uint64_t front_axle_zw() const { return guest::LoadU64(base, address + 6184); }
  void set_front_axle_zw(uint64_t v) const { guest::StoreU64(base, address + 6184, v); }
  uint32_t front_axle_zw_address() const { return address + 6184; }
  uint64_t rear_axle_xy() const { return guest::LoadU64(base, address + 6192); }
  void set_rear_axle_xy(uint64_t v) const { guest::StoreU64(base, address + 6192, v); }
  uint32_t rear_axle_xy_address() const { return address + 6192; }
  uint64_t rear_axle_zw() const { return guest::LoadU64(base, address + 6200); }
  void set_rear_axle_zw(uint64_t v) const { guest::StoreU64(base, address + 6200, v); }
  uint32_t rear_axle_zw_address() const { return address + 6200; }
};

// allocated; measure: TrafficCar, SceneModel.+5100.+16780
struct TrafficCar : View {
  using View::View;
  static constexpr uint32_t kSize = 16780;
  static constexpr uint32_t kVtable = 0x820050B4;
  // 0x820050B4 (0x82004F90 in the destructor's end)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the pool node's (0 when made)
  uint32_t state() const { return guest::LoadU32(base, address + 16); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  // -1 when made. Measure fault: also SceneCockpit, SceneTraffic and SceneWheels::Release's pool index
  uint32_t index() const { return guest::LoadU32(base, address + 20); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t index_address() const { return address + 20; }
  // measure fault: the second set's pool objects' +24 (SceneCockpit, SceneParts, ScenePool4, SceneWheels::SetUp; SceneModel::ApplyRecord, Build)
  uint32_t joined_owner() const { return guest::LoadU32(base, address + 24); }
  void set_joined_owner(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t joined_owner_address() const { return address + 24; }
  // measure fault: SceneCockpit::SetUp's store
  uint32_t joined_cockpit_28() const { return guest::LoadU32(base, address + 28); }
  void set_joined_cockpit_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t joined_cockpit_28_address() const { return address + 28; }
  // the car record's +0: the traffic car's number, which picks its name at [0x82601980 + 4 x it]. Measure fault: also SceneCockpit::SetUp's store
  uint32_t car_number() const { return guest::LoadU32(base, address + 32); }
  void set_car_number(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t car_number_address() const { return address + 32; }
  // measure fault: the stand's scene model's car record +0, the model block (GarageAnims, GarageStage::ApplyPreset, SetAeroSet)
  uint32_t model_block() const { return guest::LoadU32(base, address + 48); }
  void set_model_block(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t model_block_address() const { return address + 48; }
  // measure fault: the stand model's record +32 to +79, three 16-byte hue, saturation, value rows, copied as doublewords by GarageStage::SetColor0 to SetColor5
  uint64_t model_paint0_xy() const { return guest::LoadU64(base, address + 64); }
  void set_model_paint0_xy(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t model_paint0_xy_address() const { return address + 64; }
  uint64_t model_paint0_zw() const { return guest::LoadU64(base, address + 72); }
  void set_model_paint0_zw(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t model_paint0_zw_address() const { return address + 72; }
  uint64_t model_paint1_xy() const { return guest::LoadU64(base, address + 80); }
  void set_model_paint1_xy(uint64_t v) const { guest::StoreU64(base, address + 80, v); }
  uint32_t model_paint1_xy_address() const { return address + 80; }
  uint64_t model_paint1_zw() const { return guest::LoadU64(base, address + 88); }
  void set_model_paint1_zw(uint64_t v) const { guest::StoreU64(base, address + 88, v); }
  uint32_t model_paint1_zw_address() const { return address + 88; }
  uint64_t model_paint2_xy() const { return guest::LoadU64(base, address + 96); }
  void set_model_paint2_xy(uint64_t v) const { guest::StoreU64(base, address + 96, v); }
  uint32_t model_paint2_xy_address() const { return address + 96; }
  uint64_t model_paint2_zw() const { return guest::LoadU64(base, address + 104); }
  void set_model_paint2_zw(uint64_t v) const { guest::StoreU64(base, address + 104, v); }
  uint32_t model_paint2_zw_address() const { return address + 104; }
  // measure fault: the stand model's record +80 to +120, the front wheels' colors (GarageStage::SetWheel)
  float model_front_wheel_paint0_hue() const { return guest::LoadFloat(base, address + 112); }
  void set_model_front_wheel_paint0_hue(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t model_front_wheel_paint0_hue_address() const { return address + 112; }
  float model_front_wheel_paint0_saturation() const { return guest::LoadFloat(base, address + 116); }
  void set_model_front_wheel_paint0_saturation(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t model_front_wheel_paint0_saturation_address() const { return address + 116; }
  float model_front_wheel_paint0_value() const { return guest::LoadFloat(base, address + 120); }
  void set_model_front_wheel_paint0_value(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t model_front_wheel_paint0_value_address() const { return address + 120; }
  float model_front_wheel_paint1_hue() const { return guest::LoadFloat(base, address + 128); }
  void set_model_front_wheel_paint1_hue(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t model_front_wheel_paint1_hue_address() const { return address + 128; }
  float model_front_wheel_paint1_saturation() const { return guest::LoadFloat(base, address + 132); }
  void set_model_front_wheel_paint1_saturation(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t model_front_wheel_paint1_saturation_address() const { return address + 132; }
  float model_front_wheel_paint1_value() const { return guest::LoadFloat(base, address + 136); }
  void set_model_front_wheel_paint1_value(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t model_front_wheel_paint1_value_address() const { return address + 136; }
  float model_front_wheel_paint2_hue() const { return guest::LoadFloat(base, address + 144); }
  void set_model_front_wheel_paint2_hue(float v) const { guest::StoreFloat(base, address + 144, v); }
  uint32_t model_front_wheel_paint2_hue_address() const { return address + 144; }
  float model_front_wheel_paint2_saturation() const { return guest::LoadFloat(base, address + 148); }
  void set_model_front_wheel_paint2_saturation(float v) const { guest::StoreFloat(base, address + 148, v); }
  uint32_t model_front_wheel_paint2_saturation_address() const { return address + 148; }
  float model_front_wheel_paint2_value() const { return guest::LoadFloat(base, address + 152); }
  void set_model_front_wheel_paint2_value(float v) const { guest::StoreFloat(base, address + 152, v); }
  uint32_t model_front_wheel_paint2_value_address() const { return address + 152; }
  // measure fault: the stand model's record +128 to +168, the rear wheels' colors
  float model_rear_wheel_paint0_hue() const { return guest::LoadFloat(base, address + 160); }
  void set_model_rear_wheel_paint0_hue(float v) const { guest::StoreFloat(base, address + 160, v); }
  uint32_t model_rear_wheel_paint0_hue_address() const { return address + 160; }
  float model_rear_wheel_paint0_saturation() const { return guest::LoadFloat(base, address + 164); }
  void set_model_rear_wheel_paint0_saturation(float v) const { guest::StoreFloat(base, address + 164, v); }
  uint32_t model_rear_wheel_paint0_saturation_address() const { return address + 164; }
  float model_rear_wheel_paint0_value() const { return guest::LoadFloat(base, address + 168); }
  void set_model_rear_wheel_paint0_value(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t model_rear_wheel_paint0_value_address() const { return address + 168; }
  float model_rear_wheel_paint1_hue() const { return guest::LoadFloat(base, address + 176); }
  void set_model_rear_wheel_paint1_hue(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t model_rear_wheel_paint1_hue_address() const { return address + 176; }
  float model_rear_wheel_paint1_saturation() const { return guest::LoadFloat(base, address + 180); }
  void set_model_rear_wheel_paint1_saturation(float v) const { guest::StoreFloat(base, address + 180, v); }
  uint32_t model_rear_wheel_paint1_saturation_address() const { return address + 180; }
  float model_rear_wheel_paint1_value() const { return guest::LoadFloat(base, address + 184); }
  void set_model_rear_wheel_paint1_value(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t model_rear_wheel_paint1_value_address() const { return address + 184; }
  // the car record's +160: a count or choice passed to the texture name's format (TrafficCar::BindTextures). The stand model: its record +160, the rear wheels' third hue (SetWheel, a float)
  uint32_t texture_name_count() const { return guest::LoadU32(base, address + 192); }
  void set_texture_name_count(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t texture_name_count_address() const { return address + 192; }
  float model_rear_wheel_paint2_saturation() const { return guest::LoadFloat(base, address + 196); }
  void set_model_rear_wheel_paint2_saturation(float v) const { guest::StoreFloat(base, address + 196, v); }
  uint32_t model_rear_wheel_paint2_saturation_address() const { return address + 196; }
  float model_rear_wheel_paint2_value() const { return guest::LoadFloat(base, address + 200); }
  void set_model_rear_wheel_paint2_value(float v) const { guest::StoreFloat(base, address + 200, v); }
  uint32_t model_rear_wheel_paint2_value_address() const { return address + 200; }
  // measure fault: SceneTraffic::Draw's read (not read)
  uint32_t unknown_412() const { return guest::LoadU32(base, address + 412); }
  void set_unknown_412(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t unknown_412_address() const { return address + 412; }
  // sub_820D6328's read (not read)
  uint32_t unknown_556() const { return guest::LoadU32(base, address + 556); }
  void set_unknown_556(uint32_t v) const { guest::StoreU32(base, address + 556, v); }
  uint32_t unknown_556_address() const { return address + 556; }
  // sub_820D6B48's read (not read)
  uint32_t unknown_576() const { return guest::LoadU32(base, address + 576); }
  void set_unknown_576(uint32_t v) const { guest::StoreU32(base, address + 576, v); }
  uint32_t unknown_576_address() const { return address + 576; }
  // measure fault: GarageStage::PoseCar's store (not read)
  uint32_t unknown_732() const { return guest::LoadU32(base, address + 732); }
  void set_unknown_732(uint32_t v) const { guest::StoreU32(base, address + 732, v); }
  uint32_t unknown_732_address() const { return address + 732; }
  // measure fault: SceneInterior::SetUp's store
  uint32_t unknown_3088() const { return guest::LoadU32(base, address + 3088); }
  void set_unknown_3088(uint32_t v) const { guest::StoreU32(base, address + 3088, v); }
  uint32_t unknown_3088_address() const { return address + 3088; }
  // measure fault: SceneWheels::SetUp's store
  uint32_t unknown_3264() const { return guest::LoadU32(base, address + 3264); }
  void set_unknown_3264(uint32_t v) const { guest::StoreU32(base, address + 3264, v); }
  uint32_t unknown_3264_address() const { return address + 3264; }
  // measure fault: SceneCockpit::Finish's read
  uint32_t unknown_3504() const { return guest::LoadU32(base, address + 3504); }
  void set_unknown_3504(uint32_t v) const { guest::StoreU32(base, address + 3504, v); }
  uint32_t unknown_3504_address() const { return address + 3504; }
  // +3728 to +3740 a 16 x 16 A8R8G8B8 texture's words (Texture::Create; TrafficLoads at +3728). The stand model: record +3696, the aero word (GarageStage::SetAero)
  uint32_t texture_word0() const { return guest::LoadU32(base, address + 3728); }
  void set_texture_word0(uint32_t v) const { guest::StoreU32(base, address + 3728, v); }
  uint32_t texture_word0_address() const { return address + 3728; }
  // the stand model: record +3700 (GarageStage::SetAero2)
  uint32_t texture_word1() const { return guest::LoadU32(base, address + 3732); }
  void set_texture_word1(uint32_t v) const { guest::StoreU32(base, address + 3732, v); }
  uint32_t texture_word1_address() const { return address + 3732; }
  // the stand model: record +3704, the door kind (GarageStage::SetDoorKind, read by GarageAnims' doors)
  uint32_t texture_word2() const { return guest::LoadU32(base, address + 3736); }
  void set_texture_word2(uint32_t v) const { guest::StoreU32(base, address + 3736, v); }
  uint32_t texture_word2_address() const { return address + 3736; }
  // the stand model: record +3708, the decal word (GarageStage::SetDecalWord)
  uint32_t texture_word3() const { return guest::LoadU32(base, address + 3740); }
  void set_texture_word3(uint32_t v) const { guest::StoreU32(base, address + 3740, v); }
  uint32_t texture_word3_address() const { return address + 3740; }
  // the file's slot (+3748 the request). The stand model: record +3712, the parts record's +0 (GarageStage::SetParts, SetAeroSet, SetPart)
  uint32_t file_slot() const { return guest::LoadU32(base, address + 3744); }
  void set_file_slot(uint32_t v) const { guest::StoreU32(base, address + 3744, v); }
  uint32_t file_slot_address() const { return address + 3744; }
  // measure fault: the stand model's record +3720
  uint32_t model_parts_aero2() const { return guest::LoadU32(base, address + 3752); }
  void set_model_parts_aero2(uint32_t v) const { guest::StoreU32(base, address + 3752, v); }
  uint32_t model_parts_aero2_address() const { return address + 3752; }
  // measure fault: the stand model's record +3724
  uint32_t model_parts_aero1() const { return guest::LoadU32(base, address + 3756); }
  void set_model_parts_aero1(uint32_t v) const { guest::StoreU32(base, address + 3756, v); }
  uint32_t model_parts_aero1_address() const { return address + 3756; }
  // the file's buffer (a container: entry 0 an XPR2, 1 the node file, 2-7 the six parts). The stand model: record +3728, parts_aero3
  uint32_t file_buffer() const { return guest::LoadU32(base, address + 3760); }
  void set_file_buffer(uint32_t v) const { guest::StoreU32(base, address + 3760, v); }
  uint32_t file_buffer_address() const { return address + 3760; }
  // the file's state (TrafficCar::BindTextures). The stand model: record +3732, parts_bonnet
  uint32_t file_state() const { return guest::LoadU32(base, address + 3764); }
  void set_file_state(uint32_t v) const { guest::StoreU32(base, address + 3764, v); }
  uint32_t file_state_address() const { return address + 3764; }
  // measure fault: the stand model's record +3736 (SetAeroSet: the set + 1 when its flag 8, else 0)
  uint32_t model_parts_aero_kit() const { return guest::LoadU32(base, address + 3768); }
  void set_model_parts_aero_kit(uint32_t v) const { guest::StoreU32(base, address + 3768, v); }
  uint32_t model_parts_aero_kit_address() const { return address + 3768; }
  // the skeleton (+3860 its node names, +3924 its matrices). Measure fault: SceneModel::BeamColor's read of a scene model's +3776 (record +3744)
  uint32_t skeleton() const { return guest::LoadU32(base, address + 3776); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(base, address + 3776, v); }
  uint32_t skeleton_address() const { return address + 3776; }
  // measure fault: the stand model's record +3752
  uint32_t model_parts_aero9() const { return guest::LoadU32(base, address + 3784); }
  void set_model_parts_aero9(uint32_t v) const { guest::StoreU32(base, address + 3784, v); }
  uint32_t model_parts_aero9_address() const { return address + 3784; }
  // measure fault: the stand model's record +3768 (GarageStage::SetCarClass)
  uint32_t model_parts_class() const { return guest::LoadU32(base, address + 3800); }
  void set_model_parts_class(uint32_t v) const { guest::StoreU32(base, address + 3800, v); }
  uint32_t model_parts_class_address() const { return address + 3800; }
  // measure fault: the stand model's record +3776, 0 to 2 (GarageStage::SetInteriorClass)
  uint32_t model_interior_class() const { return guest::LoadU32(base, address + 3808); }
  void set_model_interior_class(uint32_t v) const { guest::StoreU32(base, address + 3808, v); }
  uint32_t model_interior_class_address() const { return address + 3808; }
  // measure fault: the stand model's record +3864 (GarageStage::SetInteriorWord kind 5)
  uint32_t model_interior_item2() const { return guest::LoadU32(base, address + 3896); }
  void set_model_interior_item2(uint32_t v) const { guest::StoreU32(base, address + 3896, v); }
  uint32_t model_interior_item2_address() const { return address + 3896; }
  // measure fault: the stand model's record +3868 (SetInteriorWord kind 4, SetInteriorPair)
  uint32_t model_interior_item1() const { return guest::LoadU32(base, address + 3900); }
  void set_model_interior_item1(uint32_t v) const { guest::StoreU32(base, address + 3900, v); }
  uint32_t model_interior_item1_address() const { return address + 3900; }
  // measure fault: the stand model's record +3872 (SetInteriorPair)
  uint32_t model_interior_item0() const { return guest::LoadU32(base, address + 3904); }
  void set_model_interior_item0(uint32_t v) const { guest::StoreU32(base, address + 3904, v); }
  uint32_t model_interior_item0_address() const { return address + 3904; }
  // measure fault: the stand model's record +3876 (SetInteriorWord kind 1, SetInteriorLevel)
  uint32_t model_interior_seats() const { return guest::LoadU32(base, address + 3908); }
  void set_model_interior_seats(uint32_t v) const { guest::StoreU32(base, address + 3908, v); }
  uint32_t model_interior_seats_address() const { return address + 3908; }
  // measure fault: the stand model's record +3880 (SetInteriorLevel)
  uint32_t model_interior_has_seats() const { return guest::LoadU32(base, address + 3912); }
  void set_model_interior_has_seats(uint32_t v) const { guest::StoreU32(base, address + 3912, v); }
  uint32_t model_interior_has_seats_address() const { return address + 3912; }
  // measure fault: the stand model's record +3884 (GarageStage::SetAccessory)
  uint32_t model_accessory() const { return guest::LoadU32(base, address + 3916); }
  void set_model_accessory(uint32_t v) const { guest::StoreU32(base, address + 3916, v); }
  uint32_t model_accessory_address() const { return address + 3916; }
  // measure fault: the stand model's record +3888
  uint32_t model_accessory_variant() const { return guest::LoadU32(base, address + 3920); }
  void set_model_accessory_variant(uint32_t v) const { guest::StoreU32(base, address + 3920, v); }
  uint32_t model_accessory_variant_address() const { return address + 3920; }
  // a texture provider (TrafficCar::Reset; another at +4084)
  uint32_t texture_provider() const { return guest::LoadU32(base, address + 4064); }
  void set_texture_provider(uint32_t v) const { guest::StoreU32(base, address + 4064, v); }
  uint32_t texture_provider_address() const { return address + 4064; }
  // measure fault: the stand model's record +4044, the wheels' +8 (GarageStage::SetWheelWord)
  uint32_t model_wheels_word() const { return guest::LoadU32(base, address + 4076); }
  void set_model_wheels_word(uint32_t v) const { guest::StoreU32(base, address + 4076, v); }
  uint32_t model_wheels_word_address() const { return address + 4076; }
  // measure fault: the stand model's record +4048 (GarageStage::SetTireGrade)
  uint32_t model_front_tire_grade() const { return guest::LoadU32(base, address + 4080); }
  void set_model_front_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 4080, v); }
  uint32_t model_front_tire_grade_address() const { return address + 4080; }
  // measure fault: the stand model's record +4052, the wheel number - 1 (GarageStage::SetWheel)
  uint32_t model_front_wheel() const { return guest::LoadU32(base, address + 4084); }
  void set_model_front_wheel(uint32_t v) const { guest::StoreU32(base, address + 4084, v); }
  uint32_t model_front_wheel_address() const { return address + 4084; }
  // measure fault: the stand model's record +4056
  float model_front_wheel_f28() const { return guest::LoadFloat(base, address + 4088); }
  void set_model_front_wheel_f28(float v) const { guest::StoreFloat(base, address + 4088, v); }
  uint32_t model_front_wheel_f28_address() const { return address + 4088; }
  // measure fault: the stand model's record +4060
  float model_front_wheel_f16() const { return guest::LoadFloat(base, address + 4092); }
  void set_model_front_wheel_f16(float v) const { guest::StoreFloat(base, address + 4092, v); }
  uint32_t model_front_wheel_f16_address() const { return address + 4092; }
  // measure fault: the stand model's record +4064
  uint32_t model_rear_tire_grade() const { return guest::LoadU32(base, address + 4096); }
  void set_model_rear_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 4096, v); }
  uint32_t model_rear_tire_grade_address() const { return address + 4096; }
  // measure fault: the stand model's record +4068
  uint32_t model_rear_wheel() const { return guest::LoadU32(base, address + 4100); }
  void set_model_rear_wheel(uint32_t v) const { guest::StoreU32(base, address + 4100, v); }
  uint32_t model_rear_wheel_address() const { return address + 4100; }
  // six 24-byte slots (+4104 + 24i, not loaded by the file callback). The stand model: record +4072 to +4215, the wheels' and light kit's words below
  uint32_t slots_address() const { return address + 4104; }
  // measure fault: the stand model's record +4072
  float model_rear_wheel_f28() const { return guest::LoadFloat(base, address + 4104); }
  void set_model_rear_wheel_f28(float v) const { guest::StoreFloat(base, address + 4104, v); }
  uint32_t model_rear_wheel_f28_address() const { return address + 4104; }
  // measure fault: the stand model's record +4076
  float model_rear_wheel_f16() const { return guest::LoadFloat(base, address + 4108); }
  void set_model_rear_wheel_f16(float v) const { guest::StoreFloat(base, address + 4108, v); }
  uint32_t model_rear_wheel_f16_address() const { return address + 4108; }
  // measure fault: the stand model's record +4080, the front tire copy's +80 truncated
  uint32_t model_front_size() const { return guest::LoadU32(base, address + 4112); }
  void set_model_front_size(uint32_t v) const { guest::StoreU32(base, address + 4112, v); }
  uint32_t model_front_size_address() const { return address + 4112; }
  // measure fault: the record's +4080 and +4084 passed to SceneWheels::SetSizes
  uint64_t model_front_size_pair() const { return guest::LoadU64(base, address + 4112); }
  void set_model_front_size_pair(uint64_t v) const { guest::StoreU64(base, address + 4112, v); }
  uint32_t model_front_size_pair_address() const { return address + 4112; }
  // measure fault: the stand model's record +4084
  float model_front_size_a() const { return guest::LoadFloat(base, address + 4116); }
  void set_model_front_size_a(float v) const { guest::StoreFloat(base, address + 4116, v); }
  uint32_t model_front_size_a_address() const { return address + 4116; }
  // measure fault: the stand model's record +4088
  float model_front_size_b() const { return guest::LoadFloat(base, address + 4120); }
  void set_model_front_size_b(float v) const { guest::StoreFloat(base, address + 4120, v); }
  uint32_t model_front_size_b_address() const { return address + 4120; }
  // slot 0's state (TrafficCar::SetPaint). The stand model: record +4092, the rear tire copy's +80 truncated
  uint32_t slot0_state() const { return guest::LoadU32(base, address + 4124); }
  void set_slot0_state(uint32_t v) const { guest::StoreU32(base, address + 4124, v); }
  uint32_t slot0_state_address() const { return address + 4124; }
  // measure fault: the record's +4092 and +4096 passed to SceneWheels::SetSizes
  uint64_t model_rear_size_pair() const { return guest::LoadU64(base, address + 4124); }
  void set_model_rear_size_pair(uint64_t v) const { guest::StoreU64(base, address + 4124, v); }
  uint32_t model_rear_size_pair_address() const { return address + 4124; }
  // measure fault: the stand model's record +4096
  float model_rear_size_a() const { return guest::LoadFloat(base, address + 4128); }
  void set_model_rear_size_a(float v) const { guest::StoreFloat(base, address + 4128, v); }
  uint32_t model_rear_size_a_address() const { return address + 4128; }
  // measure fault: the stand model's record +4100
  float model_rear_size_b() const { return guest::LoadFloat(base, address + 4132); }
  void set_model_rear_size_b(float v) const { guest::StoreFloat(base, address + 4132, v); }
  uint32_t model_rear_size_b_address() const { return address + 4132; }
  // measure fault: the stand model's record +4104, the wheels' +68 (GarageStage::SetWheelExtras)
  uint32_t model_wheel_extra() const { return guest::LoadU32(base, address + 4136); }
  void set_model_wheel_extra(uint32_t v) const { guest::StoreU32(base, address + 4136, v); }
  uint32_t model_wheel_extra_address() const { return address + 4136; }
  // measure fault: the stand model's record +4108, the wheels' +72 (SceneWheels::SetSizes' a)
  uint32_t model_wheel_size_a() const { return guest::LoadU32(base, address + 4140); }
  void set_model_wheel_size_a(uint32_t v) const { guest::StoreU32(base, address + 4140, v); }
  uint32_t model_wheel_size_a_address() const { return address + 4140; }
  // measure fault: the stand model's record +4112, the wheels' +76 (SetSizes' b)
  uint32_t model_wheel_size_b() const { return guest::LoadU32(base, address + 4144); }
  void set_model_wheel_size_b(uint32_t v) const { guest::StoreU32(base, address + 4144, v); }
  uint32_t model_wheel_size_b_address() const { return address + 4144; }
  // measure fault: the stand model's record +4124, the wheels' +88 (SetWheelExtras)
  uint32_t model_caliper_texture() const { return guest::LoadU32(base, address + 4156); }
  void set_model_caliper_texture(uint32_t v) const { guest::StoreU32(base, address + 4156, v); }
  uint32_t model_caliper_texture_address() const { return address + 4156; }
  // measure fault: the stand model's record +4192 to +4240, the light kit's 52 bytes (GarageStage::SetLightKit, ClearLightKit)
  uint32_t model_light_kit0_b() const { return guest::LoadU32(base, address + 4224); }
  void set_model_light_kit0_b(uint32_t v) const { guest::StoreU32(base, address + 4224, v); }
  uint32_t model_light_kit0_b_address() const { return address + 4224; }
  uint32_t model_light_kit0_a() const { return guest::LoadU32(base, address + 4228); }
  void set_model_light_kit0_a(uint32_t v) const { guest::StoreU32(base, address + 4228, v); }
  uint32_t model_light_kit0_a_address() const { return address + 4228; }
  float model_light_kit0_f8() const { return guest::LoadFloat(base, address + 4232); }
  void set_model_light_kit0_f8(float v) const { guest::StoreFloat(base, address + 4232, v); }
  uint32_t model_light_kit0_f8_address() const { return address + 4232; }
  float model_light_kit0_f12() const { return guest::LoadFloat(base, address + 4236); }
  void set_model_light_kit0_f12(float v) const { guest::StoreFloat(base, address + 4236, v); }
  uint32_t model_light_kit0_f12_address() const { return address + 4236; }
  float model_light_kit0_f16() const { return guest::LoadFloat(base, address + 4240); }
  void set_model_light_kit0_f16(float v) const { guest::StoreFloat(base, address + 4240, v); }
  uint32_t model_light_kit0_f16_address() const { return address + 4240; }
  uint32_t model_light_kit1_a() const { return guest::LoadU32(base, address + 4244); }
  void set_model_light_kit1_a(uint32_t v) const { guest::StoreU32(base, address + 4244, v); }
  uint32_t model_light_kit1_a_address() const { return address + 4244; }
  float model_light_kit1_f8() const { return guest::LoadFloat(base, address + 4248); }
  void set_model_light_kit1_f8(float v) const { guest::StoreFloat(base, address + 4248, v); }
  uint32_t model_light_kit1_f8_address() const { return address + 4248; }
  float model_light_kit1_f12() const { return guest::LoadFloat(base, address + 4252); }
  void set_model_light_kit1_f12(float v) const { guest::StoreFloat(base, address + 4252, v); }
  uint32_t model_light_kit1_f12_address() const { return address + 4252; }
  float model_light_kit1_f16() const { return guest::LoadFloat(base, address + 4256); }
  void set_model_light_kit1_f16(float v) const { guest::StoreFloat(base, address + 4256, v); }
  uint32_t model_light_kit1_f16_address() const { return address + 4256; }
  uint32_t model_light_kit2_a() const { return guest::LoadU32(base, address + 4260); }
  void set_model_light_kit2_a(uint32_t v) const { guest::StoreU32(base, address + 4260, v); }
  uint32_t model_light_kit2_a_address() const { return address + 4260; }
  float model_light_kit2_f8() const { return guest::LoadFloat(base, address + 4264); }
  void set_model_light_kit2_f8(float v) const { guest::StoreFloat(base, address + 4264, v); }
  uint32_t model_light_kit2_f8_address() const { return address + 4264; }
  float model_light_kit2_f12() const { return guest::LoadFloat(base, address + 4268); }
  void set_model_light_kit2_f12(float v) const { guest::StoreFloat(base, address + 4268, v); }
  uint32_t model_light_kit2_f12_address() const { return address + 4268; }
  float model_light_kit2_f16() const { return guest::LoadFloat(base, address + 4272); }
  void set_model_light_kit2_f16(float v) const { guest::StoreFloat(base, address + 4272, v); }
  uint32_t model_light_kit2_f16_address() const { return address + 4272; }
  // the six part models (+4256 + 464i)
  uint32_t part_models_address() const { return address + 4256; }
  // measure fault: a scene model's draw block +0 (SceneModel::Beam, BeamShape, BeamColor)
  uint64_t model_draw_block_head() const { return guest::LoadU64(base, address + 4288); }
  void set_model_draw_block_head(uint64_t v) const { guest::StoreU64(base, address + 4288, v); }
  uint32_t model_draw_block_head_address() const { return address + 4288; }
  // measure fault: the stand model's draw word (the draw block's +8; GarageStage::Draw, MakeModel, ReloadModel, SetDrawWord, StandScene::PollTeardown). The traffic body: a part model's word
  uint32_t model_draw_word() const { return guest::LoadU32(base, address + 4296); }
  void set_model_draw_word(uint32_t v) const { guest::StoreU32(base, address + 4296, v); }
  uint32_t model_draw_word_address() const { return address + 4296; }
  // part model 0's +48 (TrafficCar::BindTextures)
  uint32_t unknown_4304() const { return guest::LoadU32(base, address + 4304); }
  void set_unknown_4304(uint32_t v) const { guest::StoreU32(base, address + 4304, v); }
  uint32_t unknown_4304_address() const { return address + 4304; }
  // part model 0's +436 (TrafficCar::BindTextures)
  uint32_t unknown_4692() const { return guest::LoadU32(base, address + 4692); }
  void set_unknown_4692(uint32_t v) const { guest::StoreU32(base, address + 4692, v); }
  uint32_t unknown_4692_address() const { return address + 4692; }
  // part model 0's +460 (TrafficCar::BindTextures)
  uint32_t unknown_4716() const { return guest::LoadU32(base, address + 4716); }
  void set_unknown_4716(uint32_t v) const { guest::StoreU32(base, address + 4716, v); }
  uint32_t unknown_4716_address() const { return address + 4716; }
  // measure fault: the stand model's draw block +640, the stage's +32 (GarageStage::Draw)
  float model_bonnet() const { return guest::LoadFloat(base, address + 4928); }
  void set_model_bonnet(float v) const { guest::StoreFloat(base, address + 4928, v); }
  uint32_t model_bonnet_address() const { return address + 4928; }
  // measure fault: the stand model's +4944 and +4960, the stage's +48 to +63 twice (GarageStage::Draw)
  uint64_t model_open0_xy() const { return guest::LoadU64(base, address + 4944); }
  void set_model_open0_xy(uint64_t v) const { guest::StoreU64(base, address + 4944, v); }
  uint32_t model_open0_xy_address() const { return address + 4944; }
  uint64_t model_open0_zw() const { return guest::LoadU64(base, address + 4952); }
  void set_model_open0_zw(uint64_t v) const { guest::StoreU64(base, address + 4952, v); }
  uint32_t model_open0_zw_address() const { return address + 4952; }
  uint64_t model_open1_xy() const { return guest::LoadU64(base, address + 4960); }
  void set_model_open1_xy(uint64_t v) const { guest::StoreU64(base, address + 4960, v); }
  uint32_t model_open1_xy_address() const { return address + 4960; }
  uint64_t model_open1_zw() const { return guest::LoadU64(base, address + 4968); }
  void set_model_open1_zw(uint64_t v) const { guest::StoreU64(base, address + 4968, v); }
  uint32_t model_open1_zw_address() const { return address + 4968; }
  // measure fault: the stand model's lights on (GarageStage::SetLightsOn)
  uint32_t model_lights_on() const { return guest::LoadU32(base, address + 4976); }
  void set_model_lights_on(uint32_t v) const { guest::StoreU32(base, address + 4976, v); }
  uint32_t model_lights_on_address() const { return address + 4976; }
  // measure fault: 0.0 when on, else SceneParts::LightValue
  float model_light_value() const { return guest::LoadFloat(base, address + 4980); }
  void set_model_light_value(float v) const { guest::StoreFloat(base, address + 4980, v); }
  uint32_t model_light_value_address() const { return address + 4980; }
  // measure fault: GarageStage::SetWord5016's word
  uint32_t model_word5016() const { return guest::LoadU32(base, address + 5016); }
  void set_model_word5016(uint32_t v) const { guest::StoreU32(base, address + 5016, v); }
  uint32_t model_word5016_address() const { return address + 5016; }
  // measure fault: the stand model's SceneModel::AddSecondWheels ran
  uint32_t model_second_wheels_claimed() const { return guest::LoadU32(base, address + 5032); }
  void set_model_second_wheels_claimed(uint32_t v) const { guest::StoreU32(base, address + 5032, v); }
  uint32_t model_second_wheels_claimed_address() const { return address + 5032; }
  // measure fault: the stand model's pool 1 object (SceneParts)
  uint32_t model_parts() const { return guest::LoadU32(base, address + 5060); }
  void set_model_parts(uint32_t v) const { guest::StoreU32(base, address + 5060, v); }
  uint32_t model_parts_address() const { return address + 5060; }
  // measure fault: the stand model's pool 5 object (SceneWheels)
  uint32_t model_wheels() const { return guest::LoadU32(base, address + 5100); }
  void set_model_wheels(uint32_t v) const { guest::StoreU32(base, address + 5100, v); }
  uint32_t model_wheels_address() const { return address + 5100; }
  // +7040 to +7052 a second texture's words (CarModel::MakePlate)
  uint32_t texture2_word0() const { return guest::LoadU32(base, address + 7040); }
  void set_texture2_word0(uint32_t v) const { guest::StoreU32(base, address + 7040, v); }
  uint32_t texture2_word0_address() const { return address + 7040; }
  uint32_t texture2_word1() const { return guest::LoadU32(base, address + 7044); }
  void set_texture2_word1(uint32_t v) const { guest::StoreU32(base, address + 7044, v); }
  uint32_t texture2_word1_address() const { return address + 7044; }
  uint32_t texture2_word2() const { return guest::LoadU32(base, address + 7048); }
  void set_texture2_word2(uint32_t v) const { guest::StoreU32(base, address + 7048, v); }
  uint32_t texture2_word2_address() const { return address + 7048; }
  uint32_t texture2_word3() const { return guest::LoadU32(base, address + 7052); }
  void set_texture2_word3(uint32_t v) const { guest::StoreU32(base, address + 7052, v); }
  uint32_t texture2_word3_address() const { return address + 7052; }
  // the XPR record named by the first formatted name (-1 none)
  uint32_t xpr_record() const { return guest::LoadU32(base, address + 7056); }
  void set_xpr_record(uint32_t v) const { guest::StoreU32(base, address + 7056, v); }
  uint32_t xpr_record_address() const { return address + 7056; }
  // another (TrafficCar::BindTextures)
  uint32_t xpr_record2() const { return guest::LoadU32(base, address + 7060); }
  void set_xpr_record2(uint32_t v) const { guest::StoreU32(base, address + 7060, v); }
  uint32_t xpr_record2_address() const { return address + 7060; }
  // +7072 + 16i the four tires' positions (their w 1.0 from the constructor)
  float tire_position0_w() const { return guest::LoadFloat(base, address + 7084); }
  void set_tire_position0_w(float v) const { guest::StoreFloat(base, address + 7084, v); }
  uint32_t tire_position0_w_address() const { return address + 7084; }
  // +7136 + 64i the tires' matrices (each row's w 1.0 from the constructor)
  float tire_matrix0_row0_w() const { return guest::LoadFloat(base, address + 7148); }
  void set_tire_matrix0_row0_w(float v) const { guest::StoreFloat(base, address + 7148, v); }
  uint32_t tire_matrix0_row0_w_address() const { return address + 7148; }
  float tire_matrix0_row1_w() const { return guest::LoadFloat(base, address + 7164); }
  void set_tire_matrix0_row1_w(float v) const { guest::StoreFloat(base, address + 7164, v); }
  uint32_t tire_matrix0_row1_w_address() const { return address + 7164; }
  float tire_matrix0_row2_w() const { return guest::LoadFloat(base, address + 7180); }
  void set_tire_matrix0_row2_w(float v) const { guest::StoreFloat(base, address + 7180, v); }
  uint32_t tire_matrix0_row2_w_address() const { return address + 7180; }
  float tire_matrix0_row3_w() const { return guest::LoadFloat(base, address + 7196); }
  void set_tire_matrix0_row3_w(float v) const { guest::StoreFloat(base, address + 7196, v); }
  uint32_t tire_matrix0_row3_w_address() const { return address + 7196; }
  // the first set ([0x826C2E40])
  template <typename T = ScenePools>
  T first_set() const { return T(base, guest::LoadU32(base, address + 7392)); }
  void set_first_set(const View& v) const { guest::StoreU32(base, address + 7392, v.address); }
  void set_first_set(uint32_t v) const { guest::StoreU32(base, address + 7392, v); }
  uint32_t first_set_address() const { return address + 7392; }
  // measure fault: sub_820E3658's store (another pool object)
  uint32_t unknown_11452() const { return guest::LoadU32(base, address + 11452); }
  void set_unknown_11452(uint32_t v) const { guest::StoreU32(base, address + 11452, v); }
  uint32_t unknown_11452_address() const { return address + 11452; }
  // measure fault: SceneCockpit::SetUp's store
  uint32_t unknown_11656() const { return guest::LoadU32(base, address + 11656); }
  void set_unknown_11656(uint32_t v) const { guest::StoreU32(base, address + 11656, v); }
  uint32_t unknown_11656_address() const { return address + 11656; }
  // measure fault: SceneCockpit::Finish's store
  uint32_t unknown_11664() const { return guest::LoadU32(base, address + 11664); }
  void set_unknown_11664(uint32_t v) const { guest::StoreU32(base, address + 11664, v); }
  uint32_t unknown_11664_address() const { return address + 11664; }
  // measure fault: sub_820E7378's store (a SceneWheels' +16776)
  uint32_t unknown_16776() const { return guest::LoadU32(base, address + 16776); }
  void set_unknown_16776(uint32_t v) const { guest::StoreU32(base, address + 16776, v); }
  uint32_t unknown_16776_address() const { return address + 16776; }
};

// file; measure: Params::GetWheel.ret
struct StandWheelSource : View {
  using View::View;
  static constexpr uint32_t kSize = 8412;
  // into the record's +80 (front) or +128 (rear) as floats
  uint16_t color0_hue() const { return guest::LoadU16(base, address + 8); }
  void set_color0_hue(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t color0_hue_address() const { return address + 8; }
  uint8_t color0_saturation() const { return guest::LoadU8(base, address + 10); }
  void set_color0_saturation(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t color0_saturation_address() const { return address + 10; }
  uint8_t color0_value() const { return guest::LoadU8(base, address + 11); }
  void set_color0_value(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t color0_value_address() const { return address + 11; }
  // into the record's +4060 (front) or +4076 (rear)
  float f16() const { return guest::LoadFloat(base, address + 16); }
  void set_f16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t f16_address() const { return address + 16; }
  uint16_t color1_hue() const { return guest::LoadU16(base, address + 20); }
  void set_color1_hue(uint16_t v) const { guest::StoreU16(base, address + 20, v); }
  uint32_t color1_hue_address() const { return address + 20; }
  uint8_t color1_saturation() const { return guest::LoadU8(base, address + 22); }
  void set_color1_saturation(uint8_t v) const { guest::StoreU8(base, address + 22, v); }
  uint32_t color1_saturation_address() const { return address + 22; }
  uint8_t color1_value() const { return guest::LoadU8(base, address + 23); }
  void set_color1_value(uint8_t v) const { guest::StoreU8(base, address + 23, v); }
  uint32_t color1_value_address() const { return address + 23; }
  uint16_t color2_hue() const { return guest::LoadU16(base, address + 24); }
  void set_color2_hue(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t color2_hue_address() const { return address + 24; }
  uint8_t color2_saturation() const { return guest::LoadU8(base, address + 26); }
  void set_color2_saturation(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t color2_saturation_address() const { return address + 26; }
  uint8_t color2_value() const { return guest::LoadU8(base, address + 27); }
  void set_color2_value(uint8_t v) const { guest::StoreU8(base, address + 27, v); }
  uint32_t color2_value_address() const { return address + 27; }
  // into the record's +4056 (front) or +4072 (rear)
  float f28() const { return guest::LoadFloat(base, address + 28); }
  void set_f28(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t f28_address() const { return address + 28; }
  // measure fault: the car spec's stock wheel (+8380), its +8 to +28
  uint16_t stock_color0_hue() const { return guest::LoadU16(base, address + 8388); }
  void set_stock_color0_hue(uint16_t v) const { guest::StoreU16(base, address + 8388, v); }
  uint32_t stock_color0_hue_address() const { return address + 8388; }
  uint8_t stock_color0_saturation() const { return guest::LoadU8(base, address + 8390); }
  void set_stock_color0_saturation(uint8_t v) const { guest::StoreU8(base, address + 8390, v); }
  uint32_t stock_color0_saturation_address() const { return address + 8390; }
  uint8_t stock_color0_value() const { return guest::LoadU8(base, address + 8391); }
  void set_stock_color0_value(uint8_t v) const { guest::StoreU8(base, address + 8391, v); }
  uint32_t stock_color0_value_address() const { return address + 8391; }
  float stock_f16() const { return guest::LoadFloat(base, address + 8396); }
  void set_stock_f16(float v) const { guest::StoreFloat(base, address + 8396, v); }
  uint32_t stock_f16_address() const { return address + 8396; }
  uint16_t stock_color1_hue() const { return guest::LoadU16(base, address + 8400); }
  void set_stock_color1_hue(uint16_t v) const { guest::StoreU16(base, address + 8400, v); }
  uint32_t stock_color1_hue_address() const { return address + 8400; }
  uint8_t stock_color1_saturation() const { return guest::LoadU8(base, address + 8402); }
  void set_stock_color1_saturation(uint8_t v) const { guest::StoreU8(base, address + 8402, v); }
  uint32_t stock_color1_saturation_address() const { return address + 8402; }
  uint8_t stock_color1_value() const { return guest::LoadU8(base, address + 8403); }
  void set_stock_color1_value(uint8_t v) const { guest::StoreU8(base, address + 8403, v); }
  uint32_t stock_color1_value_address() const { return address + 8403; }
  uint16_t stock_color2_hue() const { return guest::LoadU16(base, address + 8404); }
  void set_stock_color2_hue(uint16_t v) const { guest::StoreU16(base, address + 8404, v); }
  uint32_t stock_color2_hue_address() const { return address + 8404; }
  uint8_t stock_color2_saturation() const { return guest::LoadU8(base, address + 8406); }
  void set_stock_color2_saturation(uint8_t v) const { guest::StoreU8(base, address + 8406, v); }
  uint32_t stock_color2_saturation_address() const { return address + 8406; }
  uint8_t stock_color2_value() const { return guest::LoadU8(base, address + 8407); }
  void set_stock_color2_value(uint8_t v) const { guest::StoreU8(base, address + 8407, v); }
  uint32_t stock_color2_value_address() const { return address + 8407; }
  float stock_f28() const { return guest::LoadFloat(base, address + 8408); }
  void set_stock_f28(float v) const { guest::StoreFloat(base, address + 8408, v); }
  uint32_t stock_f28_address() const { return address + 8408; }
};

// allocated; measure: new<8448>@sub_82274DA8, new<8448>@sub_82274DA8#2
struct StandModelsSpecCopy : View {
  using View::View;
  static constexpr uint32_t kSize = 8448;
  // the spec's byte +771: 0 or 1 picks the list, else the copy is freed
  uint8_t unknown_771() const { return guest::LoadU8(base, address + 771); }
  void set_unknown_771(uint8_t v) const { guest::StoreU8(base, address + 771, v); }
  uint32_t unknown_771_address() const { return address + 771; }
  uint32_t car_id() const { return guest::LoadU32(base, address + 8432); }
  void set_car_id(uint32_t v) const { guest::StoreU32(base, address + 8432, v); }
  uint32_t car_id_address() const { return address + 8432; }
};

// The image words docs/functions/stand-models.md's code uses.
struct StandModelsGlobals {
  // the car scene (the second set of scene pools)
  template <typename T = CarScene>
  static T scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // the stand scene's holder (StandScene::Create): +0 to +8 three 4-byte blocks, each holding a garage stage at +0; +20 a stand model was made
  static View stand_holder(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
};

}  // namespace schema
