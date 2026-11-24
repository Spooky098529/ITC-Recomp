// The views of docs/functions/stand-models.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/stand-models.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: GarageStage, GarageStage.+0, GarageStage@+4512, GarageStage.+0@+4512
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

// allocated; measure: TrafficCar, SceneCockpit.+28, SceneTraffic.+9768, SceneModel.+5092.+9760
#pragma pack(push, 1)
struct TrafficCarData {
  uint32_t vtable = 0;  // +0
  uint32_t list = 0;  // +4
  uint32_t next = 0;  // +8
  uint32_t previous = 0;  // +12
  uint32_t state = 0;  // +16
  uint32_t index = 0;  // +20
  uint32_t joined_owner = 0;  // +24
  uint32_t joined_cockpit_28 = 0;  // +28
  uint32_t car_number = 0;  // +32
  uint8_t pad_36[12] = {};
  uint32_t model_block = 0;  // +48
  uint8_t pad_52[12] = {};
  uint64_t model_paint0_xy = 0;  // +64
  uint64_t model_paint0_zw = 0;  // +72
  uint64_t model_paint1_xy = 0;  // +80
  uint64_t model_paint1_zw = 0;  // +88
  uint64_t model_paint2_xy = 0;  // +96
  uint64_t model_paint2_zw = 0;  // +104
  float model_front_wheel_paint0_hue = 0;  // +112
  float model_front_wheel_paint0_saturation = 0;  // +116
  float model_front_wheel_paint0_value = 0;  // +120
  uint8_t pad_124[4] = {};
  float model_front_wheel_paint1_hue = 0;  // +128
  float model_front_wheel_paint1_saturation = 0;  // +132
  float model_front_wheel_paint1_value = 0;  // +136
  uint8_t pad_140[4] = {};
  float model_front_wheel_paint2_hue = 0;  // +144
  float model_front_wheel_paint2_saturation = 0;  // +148
  float model_front_wheel_paint2_value = 0;  // +152
  uint8_t pad_156[4] = {};
  float model_rear_wheel_paint0_hue = 0;  // +160
  float model_rear_wheel_paint0_saturation = 0;  // +164
  float model_rear_wheel_paint0_value = 0;  // +168
  uint8_t pad_172[4] = {};
  float model_rear_wheel_paint1_hue = 0;  // +176
  float model_rear_wheel_paint1_saturation = 0;  // +180
  float model_rear_wheel_paint1_value = 0;  // +184
  uint8_t pad_188[4] = {};
  uint32_t texture_name_count = 0;  // +192
  float model_rear_wheel_paint2_saturation = 0;  // +196
  float model_rear_wheel_paint2_value = 0;  // +200
  uint8_t pad_204[208] = {};
  uint32_t unknown_412 = 0;  // +412
  uint8_t pad_416[140] = {};
  uint32_t unknown_556 = 0;  // +556
  uint8_t pad_560[16] = {};
  uint32_t unknown_576 = 0;  // +576
  uint8_t pad_580[152] = {};
  uint32_t unknown_732 = 0;  // +732
  uint8_t pad_736[2352] = {};
  uint32_t unknown_3088 = 0;  // +3088
  uint8_t pad_3092[172] = {};
  uint32_t unknown_3264 = 0;  // +3264
  uint8_t pad_3268[236] = {};
  uint32_t unknown_3504 = 0;  // +3504
  uint8_t pad_3508[220] = {};
  uint32_t texture_word0 = 0;  // +3728
  uint32_t texture_word1 = 0;  // +3732
  uint32_t texture_word2 = 0;  // +3736
  uint32_t texture_word3 = 0;  // +3740
  uint32_t file_slot = 0;  // +3744
  uint8_t pad_3748[4] = {};
  uint32_t model_parts_aero2 = 0;  // +3752
  uint32_t model_parts_aero1 = 0;  // +3756
  uint32_t file_buffer = 0;  // +3760
  uint32_t file_state = 0;  // +3764
  uint32_t model_parts_aero_kit = 0;  // +3768
  uint8_t pad_3772[4] = {};
  uint32_t skeleton = 0;  // +3776
  uint8_t pad_3780[4] = {};
  uint32_t model_parts_aero9 = 0;  // +3784
  uint8_t pad_3788[12] = {};
  uint32_t model_parts_class = 0;  // +3800
  uint8_t pad_3804[4] = {};
  uint32_t model_interior_class = 0;  // +3808
  uint8_t pad_3812[84] = {};
  uint32_t model_interior_item2 = 0;  // +3896
  uint32_t model_interior_item1 = 0;  // +3900
  uint32_t model_interior_item0 = 0;  // +3904
  uint32_t model_interior_seats = 0;  // +3908
  uint32_t model_interior_has_seats = 0;  // +3912
  uint32_t model_accessory = 0;  // +3916
  uint32_t model_accessory_variant = 0;  // +3920
  uint8_t pad_3924[140] = {};
  uint32_t texture_provider = 0;  // +4064
  uint8_t pad_4068[8] = {};
  uint32_t model_wheels_word = 0;  // +4076
  uint32_t model_front_tire_grade = 0;  // +4080
  uint32_t model_front_wheel = 0;  // +4084
  float model_front_wheel_f28 = 0;  // +4088
  float model_front_wheel_f16 = 0;  // +4092
  uint32_t model_rear_tire_grade = 0;  // +4096
  uint32_t model_rear_wheel = 0;  // +4100
  float model_rear_wheel_f28 = 0;  // +4104
  float model_rear_wheel_f16 = 0;  // +4108
  uint32_t model_front_size = 0;  // +4112
  float model_front_size_a = 0;  // +4116
  float model_front_size_b = 0;  // +4120
  uint32_t slot0_state = 0;  // +4124
  float model_rear_size_a = 0;  // +4128
  float model_rear_size_b = 0;  // +4132
  uint32_t model_wheel_extra = 0;  // +4136
  uint32_t model_wheel_size_a = 0;  // +4140
  uint32_t model_wheel_size_b = 0;  // +4144
  uint8_t pad_4148[8] = {};
  uint32_t model_caliper_texture = 0;  // +4156
  uint8_t pad_4160[64] = {};
  uint32_t model_light_kit0_b = 0;  // +4224
  uint32_t model_light_kit0_a = 0;  // +4228
  float model_light_kit0_f8 = 0;  // +4232
  float model_light_kit0_f12 = 0;  // +4236
  float model_light_kit0_f16 = 0;  // +4240
  uint32_t model_light_kit1_a = 0;  // +4244
  float model_light_kit1_f8 = 0;  // +4248
  float model_light_kit1_f12 = 0;  // +4252
  float model_light_kit1_f16 = 0;  // +4256
  uint32_t model_light_kit2_a = 0;  // +4260
  float model_light_kit2_f8 = 0;  // +4264
  float model_light_kit2_f12 = 0;  // +4268
  float model_light_kit2_f16 = 0;  // +4272
  uint8_t pad_4276[12] = {};
  uint64_t model_draw_block_head = 0;  // +4288
  uint32_t model_draw_word = 0;  // +4296
  uint8_t pad_4300[4] = {};
  uint32_t unknown_4304 = 0;  // +4304
  uint8_t pad_4308[384] = {};
  uint32_t unknown_4692 = 0;  // +4692
  uint8_t pad_4696[20] = {};
  uint32_t unknown_4716 = 0;  // +4716
  uint8_t pad_4720[208] = {};
  float model_bonnet = 0;  // +4928
  uint8_t pad_4932[12] = {};
  uint64_t model_open0_xy = 0;  // +4944
  uint64_t model_open0_zw = 0;  // +4952
  uint64_t model_open1_xy = 0;  // +4960
  uint64_t model_open1_zw = 0;  // +4968
  uint32_t model_lights_on = 0;  // +4976
  float model_light_value = 0;  // +4980
  uint8_t pad_4984[32] = {};
  uint32_t model_word5016 = 0;  // +5016
  uint8_t pad_5020[12] = {};
  uint32_t model_second_wheels_claimed = 0;  // +5032
  uint8_t pad_5036[24] = {};
  uint32_t model_parts = 0;  // +5060
  uint8_t pad_5064[36] = {};
  uint32_t model_wheels = 0;  // +5100
  uint8_t pad_5104[1936] = {};
  uint32_t texture2_word0 = 0;  // +7040
  uint32_t texture2_word1 = 0;  // +7044
  uint32_t texture2_word2 = 0;  // +7048
  uint32_t texture2_word3 = 0;  // +7052
  uint32_t xpr_record = 0;  // +7056
  uint32_t xpr_record2 = 0;  // +7060
  uint8_t pad_7064[20] = {};
  float tire_position0_w = 0;  // +7084
  uint8_t pad_7088[60] = {};
  float tire_matrix0_row0_w = 0;  // +7148
  uint8_t pad_7152[12] = {};
  float tire_matrix0_row1_w = 0;  // +7164
  uint8_t pad_7168[12] = {};
  float tire_matrix0_row2_w = 0;  // +7180
  uint8_t pad_7184[12] = {};
  float tire_matrix0_row3_w = 0;  // +7196
  uint8_t pad_7200[192] = {};
  uint32_t first_set = 0;  // +7392
  uint8_t pad_7396[4056] = {};
  uint32_t unknown_11452 = 0;  // +11452
  uint8_t pad_11456[200] = {};
  uint32_t unknown_11656 = 0;  // +11656
  uint8_t pad_11660[4] = {};
  uint32_t unknown_11664 = 0;  // +11664
  uint8_t pad_11668[5108] = {};
  uint32_t unknown_16776 = 0;  // +16776
};
#pragma pack(pop)
static_assert(sizeof(TrafficCarData) == 16780);
inline constexpr memory::LayoutField kTrafficCarFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {48, 4}, {64, 8}, {72, 8}, {80, 8}, {88, 8}, {96, 8}, {104, 8}, {112, 4}, {116, 4}, {120, 4}, {128, 4}, {132, 4}, {136, 4}, {144, 4}, {148, 4}, {152, 4}, {160, 4}, {164, 4}, {168, 4}, {176, 4}, {180, 4}, {184, 4}, {192, 4}, {196, 4}, {200, 4}, {412, 4}, {556, 4}, {576, 4}, {732, 4}, {3088, 4}, {3264, 4}, {3504, 4}, {3728, 4}, {3732, 4}, {3736, 4}, {3740, 4}, {3744, 4}, {3752, 4}, {3756, 4}, {3760, 4}, {3764, 4}, {3768, 4}, {3776, 4}, {3784, 4}, {3800, 4}, {3808, 4}, {3896, 4}, {3900, 4}, {3904, 4}, {3908, 4}, {3912, 4}, {3916, 4}, {3920, 4}, {4064, 4}, {4076, 4}, {4080, 4}, {4084, 4}, {4088, 4}, {4092, 4}, {4096, 4}, {4100, 4}, {4104, 4}, {4108, 4}, {4112, 4}, {4116, 4}, {4120, 4}, {4124, 4}, {4128, 4}, {4132, 4}, {4136, 4}, {4140, 4}, {4144, 4}, {4156, 4}, {4224, 4}, {4228, 4}, {4232, 4}, {4236, 4}, {4240, 4}, {4244, 4}, {4248, 4}, {4252, 4}, {4256, 4}, {4260, 4}, {4264, 4}, {4268, 4}, {4272, 4}, {4288, 8}, {4296, 4}, {4304, 4}, {4692, 4}, {4716, 4}, {4928, 4}, {4944, 8}, {4952, 8}, {4960, 8}, {4968, 8}, {4976, 4}, {4980, 4}, {5016, 4}, {5032, 4}, {5060, 4}, {5100, 4}, {7040, 4}, {7044, 4}, {7048, 4}, {7052, 4}, {7056, 4}, {7060, 4}, {7084, 4}, {7148, 4}, {7164, 4}, {7180, 4}, {7196, 4}, {7392, 4}, {11452, 4}, {11656, 4}, {11664, 4}, {16776, 4}};
inline constexpr memory::Layout kTrafficCarLayout = {kTrafficCarFields, 127};

struct TrafficCar : View {
  using View::View;
  static constexpr uint32_t kSize = 16780;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x820050B4;
  TrafficCarData* data() const { return static_cast<TrafficCarData*>(memory::HandleObject(address)); }
  static TrafficCar New(uint8_t* base) {
    return TrafficCar(base, memory::MakeHandle(new TrafficCarData(), sizeof(TrafficCarData), "TrafficCar", &kTrafficCarLayout));
  }
  template <uint32_t Stride = kSize>
  static TrafficCar NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TrafficCarData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TrafficCarData*>(::operator new(sizeof(TrafficCarData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TrafficCarData();
    return TrafficCar(base, memory::MakeHandle(p, sizeof(TrafficCarData) * count, "TrafficCar", &kTrafficCarLayout, sizeof(TrafficCarData)));
  }
  TrafficCar at(uint32_t i) const { return TrafficCar(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  TrafficCarData* direct() const { return static_cast<TrafficCarData*>(memory::HandleOwn(address, &kTrafficCarLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTrafficCarLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTrafficCarLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x820050B4 (0x82004F90 in the destructor's end)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the PoolNode's list (pool 6's of the first set)
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
  // the pool node's (0 when made)
  uint32_t state_address() const { return address + 16; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // -1 when made. Measure fault: also SceneCockpit, SceneTraffic and SceneWheels::Release's pool index
  uint32_t index_address() const { return address + 20; }
  uint32_t index_bits() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index_bits(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t index() const { if (auto* d = direct()) return d->index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_index(uint32_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // measure fault: the second set's pool objects' +24 (SceneCockpit, SceneParts, ScenePool4, SceneWheels::SetUp; SceneModel::ApplyRecord, Build)
  uint32_t joined_owner_address() const { return address + 24; }
  uint32_t joined_owner_bits() const { if (auto* d = direct()) return d->joined_owner; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_joined_owner_bits(uint32_t v) const { if (auto* d = direct()) { d->joined_owner = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t joined_owner() const { if (auto* d = direct()) return d->joined_owner; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_joined_owner(uint32_t v) const { if (auto* d = direct()) { d->joined_owner = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // measure fault: SceneCockpit::SetUp's store
  uint32_t joined_cockpit_28_address() const { return address + 28; }
  uint32_t joined_cockpit_28_bits() const { if (auto* d = direct()) return d->joined_cockpit_28; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_joined_cockpit_28_bits(uint32_t v) const { if (auto* d = direct()) { d->joined_cockpit_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t joined_cockpit_28() const { if (auto* d = direct()) return d->joined_cockpit_28; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_joined_cockpit_28(uint32_t v) const { if (auto* d = direct()) { d->joined_cockpit_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // the car record's +0: the traffic car's number, which picks its name at [0x82601980 + 4 x it]. Measure fault: also SceneCockpit::SetUp's store
  uint32_t car_number_address() const { return address + 32; }
  uint32_t car_number_bits() const { if (auto* d = direct()) return d->car_number; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_car_number_bits(uint32_t v) const { if (auto* d = direct()) { d->car_number = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t car_number() const { if (auto* d = direct()) return d->car_number; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_car_number(uint32_t v) const { if (auto* d = direct()) { d->car_number = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // measure fault: the stand's scene model's car record +0, the model block (GarageAnims, GarageStage::ApplyPreset, SetAeroSet)
  uint32_t model_block_address() const { return address + 48; }
  uint32_t model_block_bits() const { if (auto* d = direct()) return d->model_block; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_model_block_bits(uint32_t v) const { if (auto* d = direct()) { d->model_block = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  uint32_t model_block() const { if (auto* d = direct()) return d->model_block; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_model_block(uint32_t v) const { if (auto* d = direct()) { d->model_block = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  // measure fault: the stand model's record +32 to +79, three 16-byte hue, saturation, value rows, copied as doublewords by GarageStage::SetColor0 to SetColor5
  uint32_t model_paint0_xy_address() const { return address + 64; }
  uint64_t model_paint0_xy_bits() const { if (auto* d = direct()) return d->model_paint0_xy; return uint64_t((memory::IsHandle(address + 64) ? uint64_t(memory::OwnerLoad(address + 64, 8)) : guest::LoadU64(base, address + 64))); }
  void set_model_paint0_xy_bits(uint64_t v) const { if (auto* d = direct()) { d->model_paint0_xy = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 8, v); else guest::StoreU64(base, address + 64, v); }
  uint64_t model_paint0_xy() const { if (auto* d = direct()) return d->model_paint0_xy; return uint64_t((memory::IsHandle(address + 64) ? uint64_t(memory::OwnerLoad(address + 64, 8)) : guest::LoadU64(base, address + 64))); }
  void set_model_paint0_xy(uint64_t v) const { if (auto* d = direct()) { d->model_paint0_xy = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 8, v); else guest::StoreU64(base, address + 64, v); }
  uint32_t model_paint0_zw_address() const { return address + 72; }
  uint64_t model_paint0_zw_bits() const { if (auto* d = direct()) return d->model_paint0_zw; return uint64_t((memory::IsHandle(address + 72) ? uint64_t(memory::OwnerLoad(address + 72, 8)) : guest::LoadU64(base, address + 72))); }
  void set_model_paint0_zw_bits(uint64_t v) const { if (auto* d = direct()) { d->model_paint0_zw = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 8, v); else guest::StoreU64(base, address + 72, v); }
  uint64_t model_paint0_zw() const { if (auto* d = direct()) return d->model_paint0_zw; return uint64_t((memory::IsHandle(address + 72) ? uint64_t(memory::OwnerLoad(address + 72, 8)) : guest::LoadU64(base, address + 72))); }
  void set_model_paint0_zw(uint64_t v) const { if (auto* d = direct()) { d->model_paint0_zw = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 8, v); else guest::StoreU64(base, address + 72, v); }
  uint32_t model_paint1_xy_address() const { return address + 80; }
  uint64_t model_paint1_xy_bits() const { if (auto* d = direct()) return d->model_paint1_xy; return uint64_t((memory::IsHandle(address + 80) ? uint64_t(memory::OwnerLoad(address + 80, 8)) : guest::LoadU64(base, address + 80))); }
  void set_model_paint1_xy_bits(uint64_t v) const { if (auto* d = direct()) { d->model_paint1_xy = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 8, v); else guest::StoreU64(base, address + 80, v); }
  uint64_t model_paint1_xy() const { if (auto* d = direct()) return d->model_paint1_xy; return uint64_t((memory::IsHandle(address + 80) ? uint64_t(memory::OwnerLoad(address + 80, 8)) : guest::LoadU64(base, address + 80))); }
  void set_model_paint1_xy(uint64_t v) const { if (auto* d = direct()) { d->model_paint1_xy = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 8, v); else guest::StoreU64(base, address + 80, v); }
  uint32_t model_paint1_zw_address() const { return address + 88; }
  uint64_t model_paint1_zw_bits() const { if (auto* d = direct()) return d->model_paint1_zw; return uint64_t((memory::IsHandle(address + 88) ? uint64_t(memory::OwnerLoad(address + 88, 8)) : guest::LoadU64(base, address + 88))); }
  void set_model_paint1_zw_bits(uint64_t v) const { if (auto* d = direct()) { d->model_paint1_zw = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 8, v); else guest::StoreU64(base, address + 88, v); }
  uint64_t model_paint1_zw() const { if (auto* d = direct()) return d->model_paint1_zw; return uint64_t((memory::IsHandle(address + 88) ? uint64_t(memory::OwnerLoad(address + 88, 8)) : guest::LoadU64(base, address + 88))); }
  void set_model_paint1_zw(uint64_t v) const { if (auto* d = direct()) { d->model_paint1_zw = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 8, v); else guest::StoreU64(base, address + 88, v); }
  uint32_t model_paint2_xy_address() const { return address + 96; }
  uint64_t model_paint2_xy_bits() const { if (auto* d = direct()) return d->model_paint2_xy; return uint64_t((memory::IsHandle(address + 96) ? uint64_t(memory::OwnerLoad(address + 96, 8)) : guest::LoadU64(base, address + 96))); }
  void set_model_paint2_xy_bits(uint64_t v) const { if (auto* d = direct()) { d->model_paint2_xy = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 8, v); else guest::StoreU64(base, address + 96, v); }
  uint64_t model_paint2_xy() const { if (auto* d = direct()) return d->model_paint2_xy; return uint64_t((memory::IsHandle(address + 96) ? uint64_t(memory::OwnerLoad(address + 96, 8)) : guest::LoadU64(base, address + 96))); }
  void set_model_paint2_xy(uint64_t v) const { if (auto* d = direct()) { d->model_paint2_xy = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 8, v); else guest::StoreU64(base, address + 96, v); }
  uint32_t model_paint2_zw_address() const { return address + 104; }
  uint64_t model_paint2_zw_bits() const { if (auto* d = direct()) return d->model_paint2_zw; return uint64_t((memory::IsHandle(address + 104) ? uint64_t(memory::OwnerLoad(address + 104, 8)) : guest::LoadU64(base, address + 104))); }
  void set_model_paint2_zw_bits(uint64_t v) const { if (auto* d = direct()) { d->model_paint2_zw = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 8, v); else guest::StoreU64(base, address + 104, v); }
  uint64_t model_paint2_zw() const { if (auto* d = direct()) return d->model_paint2_zw; return uint64_t((memory::IsHandle(address + 104) ? uint64_t(memory::OwnerLoad(address + 104, 8)) : guest::LoadU64(base, address + 104))); }
  void set_model_paint2_zw(uint64_t v) const { if (auto* d = direct()) { d->model_paint2_zw = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 8, v); else guest::StoreU64(base, address + 104, v); }
  // measure fault: the stand model's record +80 to +120, the front wheels' colors (GarageStage::SetWheel)
  uint32_t model_front_wheel_paint0_hue_address() const { return address + 112; }
  uint32_t model_front_wheel_paint0_hue_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint0_hue); return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(base, address + 112))); }
  void set_model_front_wheel_paint0_hue_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint0_hue = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(base, address + 112, v); }
  float model_front_wheel_paint0_hue() const { return std::bit_cast<float>(model_front_wheel_paint0_hue_bits()); }
  void set_model_front_wheel_paint0_hue(float v) const { set_model_front_wheel_paint0_hue_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_front_wheel_paint0_saturation_address() const { return address + 116; }
  uint32_t model_front_wheel_paint0_saturation_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint0_saturation); return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(base, address + 116))); }
  void set_model_front_wheel_paint0_saturation_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint0_saturation = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(base, address + 116, v); }
  float model_front_wheel_paint0_saturation() const { return std::bit_cast<float>(model_front_wheel_paint0_saturation_bits()); }
  void set_model_front_wheel_paint0_saturation(float v) const { set_model_front_wheel_paint0_saturation_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_front_wheel_paint0_value_address() const { return address + 120; }
  uint32_t model_front_wheel_paint0_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint0_value); return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(base, address + 120))); }
  void set_model_front_wheel_paint0_value_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint0_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(base, address + 120, v); }
  float model_front_wheel_paint0_value() const { return std::bit_cast<float>(model_front_wheel_paint0_value_bits()); }
  void set_model_front_wheel_paint0_value(float v) const { set_model_front_wheel_paint0_value_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_front_wheel_paint1_hue_address() const { return address + 128; }
  uint32_t model_front_wheel_paint1_hue_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint1_hue); return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(base, address + 128))); }
  void set_model_front_wheel_paint1_hue_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint1_hue = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(base, address + 128, v); }
  float model_front_wheel_paint1_hue() const { return std::bit_cast<float>(model_front_wheel_paint1_hue_bits()); }
  void set_model_front_wheel_paint1_hue(float v) const { set_model_front_wheel_paint1_hue_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_front_wheel_paint1_saturation_address() const { return address + 132; }
  uint32_t model_front_wheel_paint1_saturation_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint1_saturation); return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(base, address + 132))); }
  void set_model_front_wheel_paint1_saturation_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint1_saturation = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(base, address + 132, v); }
  float model_front_wheel_paint1_saturation() const { return std::bit_cast<float>(model_front_wheel_paint1_saturation_bits()); }
  void set_model_front_wheel_paint1_saturation(float v) const { set_model_front_wheel_paint1_saturation_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_front_wheel_paint1_value_address() const { return address + 136; }
  uint32_t model_front_wheel_paint1_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint1_value); return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(base, address + 136))); }
  void set_model_front_wheel_paint1_value_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint1_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(base, address + 136, v); }
  float model_front_wheel_paint1_value() const { return std::bit_cast<float>(model_front_wheel_paint1_value_bits()); }
  void set_model_front_wheel_paint1_value(float v) const { set_model_front_wheel_paint1_value_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_front_wheel_paint2_hue_address() const { return address + 144; }
  uint32_t model_front_wheel_paint2_hue_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint2_hue); return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(base, address + 144))); }
  void set_model_front_wheel_paint2_hue_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint2_hue = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(base, address + 144, v); }
  float model_front_wheel_paint2_hue() const { return std::bit_cast<float>(model_front_wheel_paint2_hue_bits()); }
  void set_model_front_wheel_paint2_hue(float v) const { set_model_front_wheel_paint2_hue_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_front_wheel_paint2_saturation_address() const { return address + 148; }
  uint32_t model_front_wheel_paint2_saturation_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint2_saturation); return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(base, address + 148))); }
  void set_model_front_wheel_paint2_saturation_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint2_saturation = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(base, address + 148, v); }
  float model_front_wheel_paint2_saturation() const { return std::bit_cast<float>(model_front_wheel_paint2_saturation_bits()); }
  void set_model_front_wheel_paint2_saturation(float v) const { set_model_front_wheel_paint2_saturation_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_front_wheel_paint2_value_address() const { return address + 152; }
  uint32_t model_front_wheel_paint2_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_paint2_value); return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(base, address + 152))); }
  void set_model_front_wheel_paint2_value_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_paint2_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(base, address + 152, v); }
  float model_front_wheel_paint2_value() const { return std::bit_cast<float>(model_front_wheel_paint2_value_bits()); }
  void set_model_front_wheel_paint2_value(float v) const { set_model_front_wheel_paint2_value_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's record +128 to +168, the rear wheels' colors
  uint32_t model_rear_wheel_paint0_hue_address() const { return address + 160; }
  uint32_t model_rear_wheel_paint0_hue_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_paint0_hue); return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(base, address + 160))); }
  void set_model_rear_wheel_paint0_hue_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_paint0_hue = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(base, address + 160, v); }
  float model_rear_wheel_paint0_hue() const { return std::bit_cast<float>(model_rear_wheel_paint0_hue_bits()); }
  void set_model_rear_wheel_paint0_hue(float v) const { set_model_rear_wheel_paint0_hue_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_rear_wheel_paint0_saturation_address() const { return address + 164; }
  uint32_t model_rear_wheel_paint0_saturation_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_paint0_saturation); return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(base, address + 164))); }
  void set_model_rear_wheel_paint0_saturation_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_paint0_saturation = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(base, address + 164, v); }
  float model_rear_wheel_paint0_saturation() const { return std::bit_cast<float>(model_rear_wheel_paint0_saturation_bits()); }
  void set_model_rear_wheel_paint0_saturation(float v) const { set_model_rear_wheel_paint0_saturation_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_rear_wheel_paint0_value_address() const { return address + 168; }
  uint32_t model_rear_wheel_paint0_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_paint0_value); return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(base, address + 168))); }
  void set_model_rear_wheel_paint0_value_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_paint0_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(base, address + 168, v); }
  float model_rear_wheel_paint0_value() const { return std::bit_cast<float>(model_rear_wheel_paint0_value_bits()); }
  void set_model_rear_wheel_paint0_value(float v) const { set_model_rear_wheel_paint0_value_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_rear_wheel_paint1_hue_address() const { return address + 176; }
  uint32_t model_rear_wheel_paint1_hue_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_paint1_hue); return uint32_t((memory::IsHandle(address + 176) ? uint32_t(memory::OwnerLoad(address + 176, 4)) : guest::LoadU32(base, address + 176))); }
  void set_model_rear_wheel_paint1_hue_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_paint1_hue = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 176)) memory::OwnerStore(address + 176, 4, v); else guest::StoreU32(base, address + 176, v); }
  float model_rear_wheel_paint1_hue() const { return std::bit_cast<float>(model_rear_wheel_paint1_hue_bits()); }
  void set_model_rear_wheel_paint1_hue(float v) const { set_model_rear_wheel_paint1_hue_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_rear_wheel_paint1_saturation_address() const { return address + 180; }
  uint32_t model_rear_wheel_paint1_saturation_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_paint1_saturation); return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(base, address + 180))); }
  void set_model_rear_wheel_paint1_saturation_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_paint1_saturation = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(base, address + 180, v); }
  float model_rear_wheel_paint1_saturation() const { return std::bit_cast<float>(model_rear_wheel_paint1_saturation_bits()); }
  void set_model_rear_wheel_paint1_saturation(float v) const { set_model_rear_wheel_paint1_saturation_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_rear_wheel_paint1_value_address() const { return address + 184; }
  uint32_t model_rear_wheel_paint1_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_paint1_value); return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(base, address + 184))); }
  void set_model_rear_wheel_paint1_value_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_paint1_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(base, address + 184, v); }
  float model_rear_wheel_paint1_value() const { return std::bit_cast<float>(model_rear_wheel_paint1_value_bits()); }
  void set_model_rear_wheel_paint1_value(float v) const { set_model_rear_wheel_paint1_value_bits(std::bit_cast<uint32_t>(v)); }
  // the car record's +160: a count or choice passed to the texture name's format (TrafficCar::BindTextures). The stand model: its record +160, the rear wheels' third hue (SetWheel, a float)
  uint32_t texture_name_count_address() const { return address + 192; }
  uint32_t texture_name_count_bits() const { if (auto* d = direct()) return d->texture_name_count; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(base, address + 192))); }
  void set_texture_name_count_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_name_count = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(base, address + 192, v); }
  uint32_t texture_name_count() const { if (auto* d = direct()) return d->texture_name_count; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(base, address + 192))); }
  void set_texture_name_count(uint32_t v) const { if (auto* d = direct()) { d->texture_name_count = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(base, address + 192, v); }
  uint32_t model_rear_wheel_paint2_saturation_address() const { return address + 196; }
  uint32_t model_rear_wheel_paint2_saturation_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_paint2_saturation); return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(base, address + 196))); }
  void set_model_rear_wheel_paint2_saturation_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_paint2_saturation = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(base, address + 196, v); }
  float model_rear_wheel_paint2_saturation() const { return std::bit_cast<float>(model_rear_wheel_paint2_saturation_bits()); }
  void set_model_rear_wheel_paint2_saturation(float v) const { set_model_rear_wheel_paint2_saturation_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_rear_wheel_paint2_value_address() const { return address + 200; }
  uint32_t model_rear_wheel_paint2_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_paint2_value); return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(base, address + 200))); }
  void set_model_rear_wheel_paint2_value_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_paint2_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(base, address + 200, v); }
  float model_rear_wheel_paint2_value() const { return std::bit_cast<float>(model_rear_wheel_paint2_value_bits()); }
  void set_model_rear_wheel_paint2_value(float v) const { set_model_rear_wheel_paint2_value_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: SceneTraffic::Draw's read (not read)
  uint32_t unknown_412_address() const { return address + 412; }
  uint32_t unknown_412_bits() const { if (auto* d = direct()) return d->unknown_412; return uint32_t((memory::IsHandle(address + 412) ? uint32_t(memory::OwnerLoad(address + 412, 4)) : guest::LoadU32(base, address + 412))); }
  void set_unknown_412_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_412 = v; return; } if (memory::IsHandle(address + 412)) memory::OwnerStore(address + 412, 4, v); else guest::StoreU32(base, address + 412, v); }
  uint32_t unknown_412() const { if (auto* d = direct()) return d->unknown_412; return uint32_t((memory::IsHandle(address + 412) ? uint32_t(memory::OwnerLoad(address + 412, 4)) : guest::LoadU32(base, address + 412))); }
  void set_unknown_412(uint32_t v) const { if (auto* d = direct()) { d->unknown_412 = v; return; } if (memory::IsHandle(address + 412)) memory::OwnerStore(address + 412, 4, v); else guest::StoreU32(base, address + 412, v); }
  // sub_820D6328's read (not read)
  uint32_t unknown_556_address() const { return address + 556; }
  uint32_t unknown_556_bits() const { if (auto* d = direct()) return d->unknown_556; return uint32_t((memory::IsHandle(address + 556) ? uint32_t(memory::OwnerLoad(address + 556, 4)) : guest::LoadU32(base, address + 556))); }
  void set_unknown_556_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_556 = v; return; } if (memory::IsHandle(address + 556)) memory::OwnerStore(address + 556, 4, v); else guest::StoreU32(base, address + 556, v); }
  uint32_t unknown_556() const { if (auto* d = direct()) return d->unknown_556; return uint32_t((memory::IsHandle(address + 556) ? uint32_t(memory::OwnerLoad(address + 556, 4)) : guest::LoadU32(base, address + 556))); }
  void set_unknown_556(uint32_t v) const { if (auto* d = direct()) { d->unknown_556 = v; return; } if (memory::IsHandle(address + 556)) memory::OwnerStore(address + 556, 4, v); else guest::StoreU32(base, address + 556, v); }
  // sub_820D6B48's read (not read)
  uint32_t unknown_576_address() const { return address + 576; }
  uint32_t unknown_576_bits() const { if (auto* d = direct()) return d->unknown_576; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(base, address + 576))); }
  void set_unknown_576_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_576 = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(base, address + 576, v); }
  uint32_t unknown_576() const { if (auto* d = direct()) return d->unknown_576; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(base, address + 576))); }
  void set_unknown_576(uint32_t v) const { if (auto* d = direct()) { d->unknown_576 = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(base, address + 576, v); }
  // measure fault: GarageStage::PoseCar's store (not read)
  uint32_t unknown_732_address() const { return address + 732; }
  uint32_t unknown_732_bits() const { if (auto* d = direct()) return d->unknown_732; return uint32_t((memory::IsHandle(address + 732) ? uint32_t(memory::OwnerLoad(address + 732, 4)) : guest::LoadU32(base, address + 732))); }
  void set_unknown_732_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_732 = v; return; } if (memory::IsHandle(address + 732)) memory::OwnerStore(address + 732, 4, v); else guest::StoreU32(base, address + 732, v); }
  uint32_t unknown_732() const { if (auto* d = direct()) return d->unknown_732; return uint32_t((memory::IsHandle(address + 732) ? uint32_t(memory::OwnerLoad(address + 732, 4)) : guest::LoadU32(base, address + 732))); }
  void set_unknown_732(uint32_t v) const { if (auto* d = direct()) { d->unknown_732 = v; return; } if (memory::IsHandle(address + 732)) memory::OwnerStore(address + 732, 4, v); else guest::StoreU32(base, address + 732, v); }
  // measure fault: SceneInterior::SetUp's store
  uint32_t unknown_3088_address() const { return address + 3088; }
  uint32_t unknown_3088_bits() const { if (auto* d = direct()) return d->unknown_3088; return uint32_t((memory::IsHandle(address + 3088) ? uint32_t(memory::OwnerLoad(address + 3088, 4)) : guest::LoadU32(base, address + 3088))); }
  void set_unknown_3088_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_3088 = v; return; } if (memory::IsHandle(address + 3088)) memory::OwnerStore(address + 3088, 4, v); else guest::StoreU32(base, address + 3088, v); }
  uint32_t unknown_3088() const { if (auto* d = direct()) return d->unknown_3088; return uint32_t((memory::IsHandle(address + 3088) ? uint32_t(memory::OwnerLoad(address + 3088, 4)) : guest::LoadU32(base, address + 3088))); }
  void set_unknown_3088(uint32_t v) const { if (auto* d = direct()) { d->unknown_3088 = v; return; } if (memory::IsHandle(address + 3088)) memory::OwnerStore(address + 3088, 4, v); else guest::StoreU32(base, address + 3088, v); }
  // measure fault: SceneWheels::SetUp's store
  uint32_t unknown_3264_address() const { return address + 3264; }
  uint32_t unknown_3264_bits() const { if (auto* d = direct()) return d->unknown_3264; return uint32_t((memory::IsHandle(address + 3264) ? uint32_t(memory::OwnerLoad(address + 3264, 4)) : guest::LoadU32(base, address + 3264))); }
  void set_unknown_3264_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_3264 = v; return; } if (memory::IsHandle(address + 3264)) memory::OwnerStore(address + 3264, 4, v); else guest::StoreU32(base, address + 3264, v); }
  uint32_t unknown_3264() const { if (auto* d = direct()) return d->unknown_3264; return uint32_t((memory::IsHandle(address + 3264) ? uint32_t(memory::OwnerLoad(address + 3264, 4)) : guest::LoadU32(base, address + 3264))); }
  void set_unknown_3264(uint32_t v) const { if (auto* d = direct()) { d->unknown_3264 = v; return; } if (memory::IsHandle(address + 3264)) memory::OwnerStore(address + 3264, 4, v); else guest::StoreU32(base, address + 3264, v); }
  // measure fault: SceneCockpit::Finish's read
  uint32_t unknown_3504_address() const { return address + 3504; }
  uint32_t unknown_3504_bits() const { if (auto* d = direct()) return d->unknown_3504; return uint32_t((memory::IsHandle(address + 3504) ? uint32_t(memory::OwnerLoad(address + 3504, 4)) : guest::LoadU32(base, address + 3504))); }
  void set_unknown_3504_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_3504 = v; return; } if (memory::IsHandle(address + 3504)) memory::OwnerStore(address + 3504, 4, v); else guest::StoreU32(base, address + 3504, v); }
  uint32_t unknown_3504() const { if (auto* d = direct()) return d->unknown_3504; return uint32_t((memory::IsHandle(address + 3504) ? uint32_t(memory::OwnerLoad(address + 3504, 4)) : guest::LoadU32(base, address + 3504))); }
  void set_unknown_3504(uint32_t v) const { if (auto* d = direct()) { d->unknown_3504 = v; return; } if (memory::IsHandle(address + 3504)) memory::OwnerStore(address + 3504, 4, v); else guest::StoreU32(base, address + 3504, v); }
  // +3728 to +3740 a 16 x 16 A8R8G8B8 texture's words (Texture::Create; TrafficLoads at +3728). The stand model: record +3696, the aero word (GarageStage::SetAero)
  uint32_t texture_word0_address() const { return address + 3728; }
  uint32_t texture_word0_bits() const { if (auto* d = direct()) return d->texture_word0; return uint32_t((memory::IsHandle(address + 3728) ? uint32_t(memory::OwnerLoad(address + 3728, 4)) : guest::LoadU32(base, address + 3728))); }
  void set_texture_word0_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_word0 = v; return; } if (memory::IsHandle(address + 3728)) memory::OwnerStore(address + 3728, 4, v); else guest::StoreU32(base, address + 3728, v); }
  uint32_t texture_word0() const { if (auto* d = direct()) return d->texture_word0; return uint32_t((memory::IsHandle(address + 3728) ? uint32_t(memory::OwnerLoad(address + 3728, 4)) : guest::LoadU32(base, address + 3728))); }
  void set_texture_word0(uint32_t v) const { if (auto* d = direct()) { d->texture_word0 = v; return; } if (memory::IsHandle(address + 3728)) memory::OwnerStore(address + 3728, 4, v); else guest::StoreU32(base, address + 3728, v); }
  // the stand model: record +3700 (GarageStage::SetAero2)
  uint32_t texture_word1_address() const { return address + 3732; }
  uint32_t texture_word1_bits() const { if (auto* d = direct()) return d->texture_word1; return uint32_t((memory::IsHandle(address + 3732) ? uint32_t(memory::OwnerLoad(address + 3732, 4)) : guest::LoadU32(base, address + 3732))); }
  void set_texture_word1_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_word1 = v; return; } if (memory::IsHandle(address + 3732)) memory::OwnerStore(address + 3732, 4, v); else guest::StoreU32(base, address + 3732, v); }
  uint32_t texture_word1() const { if (auto* d = direct()) return d->texture_word1; return uint32_t((memory::IsHandle(address + 3732) ? uint32_t(memory::OwnerLoad(address + 3732, 4)) : guest::LoadU32(base, address + 3732))); }
  void set_texture_word1(uint32_t v) const { if (auto* d = direct()) { d->texture_word1 = v; return; } if (memory::IsHandle(address + 3732)) memory::OwnerStore(address + 3732, 4, v); else guest::StoreU32(base, address + 3732, v); }
  // the stand model: record +3704, the door kind (GarageStage::SetDoorKind, read by GarageAnims' doors)
  uint32_t texture_word2_address() const { return address + 3736; }
  uint32_t texture_word2_bits() const { if (auto* d = direct()) return d->texture_word2; return uint32_t((memory::IsHandle(address + 3736) ? uint32_t(memory::OwnerLoad(address + 3736, 4)) : guest::LoadU32(base, address + 3736))); }
  void set_texture_word2_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_word2 = v; return; } if (memory::IsHandle(address + 3736)) memory::OwnerStore(address + 3736, 4, v); else guest::StoreU32(base, address + 3736, v); }
  uint32_t texture_word2() const { if (auto* d = direct()) return d->texture_word2; return uint32_t((memory::IsHandle(address + 3736) ? uint32_t(memory::OwnerLoad(address + 3736, 4)) : guest::LoadU32(base, address + 3736))); }
  void set_texture_word2(uint32_t v) const { if (auto* d = direct()) { d->texture_word2 = v; return; } if (memory::IsHandle(address + 3736)) memory::OwnerStore(address + 3736, 4, v); else guest::StoreU32(base, address + 3736, v); }
  // the stand model: record +3708, the decal word (GarageStage::SetDecalWord)
  uint32_t texture_word3_address() const { return address + 3740; }
  uint32_t texture_word3_bits() const { if (auto* d = direct()) return d->texture_word3; return uint32_t((memory::IsHandle(address + 3740) ? uint32_t(memory::OwnerLoad(address + 3740, 4)) : guest::LoadU32(base, address + 3740))); }
  void set_texture_word3_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_word3 = v; return; } if (memory::IsHandle(address + 3740)) memory::OwnerStore(address + 3740, 4, v); else guest::StoreU32(base, address + 3740, v); }
  uint32_t texture_word3() const { if (auto* d = direct()) return d->texture_word3; return uint32_t((memory::IsHandle(address + 3740) ? uint32_t(memory::OwnerLoad(address + 3740, 4)) : guest::LoadU32(base, address + 3740))); }
  void set_texture_word3(uint32_t v) const { if (auto* d = direct()) { d->texture_word3 = v; return; } if (memory::IsHandle(address + 3740)) memory::OwnerStore(address + 3740, 4, v); else guest::StoreU32(base, address + 3740, v); }
  // the file's slot (+3748 the request). The stand model: record +3712, the parts record's +0 (GarageStage::SetParts, SetAeroSet, SetPart)
  uint32_t file_slot_address() const { return address + 3744; }
  uint32_t file_slot_bits() const { if (auto* d = direct()) return d->file_slot; return uint32_t((memory::IsHandle(address + 3744) ? uint32_t(memory::OwnerLoad(address + 3744, 4)) : guest::LoadU32(base, address + 3744))); }
  void set_file_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->file_slot = v; return; } if (memory::IsHandle(address + 3744)) memory::OwnerStore(address + 3744, 4, v); else guest::StoreU32(base, address + 3744, v); }
  uint32_t file_slot() const { if (auto* d = direct()) return d->file_slot; return uint32_t((memory::IsHandle(address + 3744) ? uint32_t(memory::OwnerLoad(address + 3744, 4)) : guest::LoadU32(base, address + 3744))); }
  void set_file_slot(uint32_t v) const { if (auto* d = direct()) { d->file_slot = v; return; } if (memory::IsHandle(address + 3744)) memory::OwnerStore(address + 3744, 4, v); else guest::StoreU32(base, address + 3744, v); }
  // measure fault: the stand model's record +3720
  uint32_t model_parts_aero2_address() const { return address + 3752; }
  uint32_t model_parts_aero2_bits() const { if (auto* d = direct()) return d->model_parts_aero2; return uint32_t((memory::IsHandle(address + 3752) ? uint32_t(memory::OwnerLoad(address + 3752, 4)) : guest::LoadU32(base, address + 3752))); }
  void set_model_parts_aero2_bits(uint32_t v) const { if (auto* d = direct()) { d->model_parts_aero2 = v; return; } if (memory::IsHandle(address + 3752)) memory::OwnerStore(address + 3752, 4, v); else guest::StoreU32(base, address + 3752, v); }
  uint32_t model_parts_aero2() const { if (auto* d = direct()) return d->model_parts_aero2; return uint32_t((memory::IsHandle(address + 3752) ? uint32_t(memory::OwnerLoad(address + 3752, 4)) : guest::LoadU32(base, address + 3752))); }
  void set_model_parts_aero2(uint32_t v) const { if (auto* d = direct()) { d->model_parts_aero2 = v; return; } if (memory::IsHandle(address + 3752)) memory::OwnerStore(address + 3752, 4, v); else guest::StoreU32(base, address + 3752, v); }
  // measure fault: the stand model's record +3724
  uint32_t model_parts_aero1_address() const { return address + 3756; }
  uint32_t model_parts_aero1_bits() const { if (auto* d = direct()) return d->model_parts_aero1; return uint32_t((memory::IsHandle(address + 3756) ? uint32_t(memory::OwnerLoad(address + 3756, 4)) : guest::LoadU32(base, address + 3756))); }
  void set_model_parts_aero1_bits(uint32_t v) const { if (auto* d = direct()) { d->model_parts_aero1 = v; return; } if (memory::IsHandle(address + 3756)) memory::OwnerStore(address + 3756, 4, v); else guest::StoreU32(base, address + 3756, v); }
  uint32_t model_parts_aero1() const { if (auto* d = direct()) return d->model_parts_aero1; return uint32_t((memory::IsHandle(address + 3756) ? uint32_t(memory::OwnerLoad(address + 3756, 4)) : guest::LoadU32(base, address + 3756))); }
  void set_model_parts_aero1(uint32_t v) const { if (auto* d = direct()) { d->model_parts_aero1 = v; return; } if (memory::IsHandle(address + 3756)) memory::OwnerStore(address + 3756, 4, v); else guest::StoreU32(base, address + 3756, v); }
  // the file's buffer (a container: entry 0 an XPR2, 1 the node file, 2-7 the six parts). The stand model: record +3728, parts_aero3
  uint32_t file_buffer_address() const { return address + 3760; }
  uint32_t file_buffer_bits() const { if (auto* d = direct()) return d->file_buffer; return uint32_t((memory::IsHandle(address + 3760) ? uint32_t(memory::OwnerLoad(address + 3760, 4)) : guest::LoadU32(base, address + 3760))); }
  void set_file_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->file_buffer = v; return; } if (memory::IsHandle(address + 3760)) memory::OwnerStore(address + 3760, 4, v); else guest::StoreU32(base, address + 3760, v); }
  uint32_t file_buffer() const { if (auto* d = direct()) return d->file_buffer; return uint32_t((memory::IsHandle(address + 3760) ? uint32_t(memory::OwnerLoad(address + 3760, 4)) : guest::LoadU32(base, address + 3760))); }
  void set_file_buffer(uint32_t v) const { if (auto* d = direct()) { d->file_buffer = v; return; } if (memory::IsHandle(address + 3760)) memory::OwnerStore(address + 3760, 4, v); else guest::StoreU32(base, address + 3760, v); }
  // the file's state (TrafficCar::BindTextures). The stand model: record +3732, parts_bonnet
  uint32_t file_state_address() const { return address + 3764; }
  uint32_t file_state_bits() const { if (auto* d = direct()) return d->file_state; return uint32_t((memory::IsHandle(address + 3764) ? uint32_t(memory::OwnerLoad(address + 3764, 4)) : guest::LoadU32(base, address + 3764))); }
  void set_file_state_bits(uint32_t v) const { if (auto* d = direct()) { d->file_state = v; return; } if (memory::IsHandle(address + 3764)) memory::OwnerStore(address + 3764, 4, v); else guest::StoreU32(base, address + 3764, v); }
  uint32_t file_state() const { if (auto* d = direct()) return d->file_state; return uint32_t((memory::IsHandle(address + 3764) ? uint32_t(memory::OwnerLoad(address + 3764, 4)) : guest::LoadU32(base, address + 3764))); }
  void set_file_state(uint32_t v) const { if (auto* d = direct()) { d->file_state = v; return; } if (memory::IsHandle(address + 3764)) memory::OwnerStore(address + 3764, 4, v); else guest::StoreU32(base, address + 3764, v); }
  // measure fault: the stand model's record +3736 (SetAeroSet: the set + 1 when its flag 8, else 0)
  uint32_t model_parts_aero_kit_address() const { return address + 3768; }
  uint32_t model_parts_aero_kit_bits() const { if (auto* d = direct()) return d->model_parts_aero_kit; return uint32_t((memory::IsHandle(address + 3768) ? uint32_t(memory::OwnerLoad(address + 3768, 4)) : guest::LoadU32(base, address + 3768))); }
  void set_model_parts_aero_kit_bits(uint32_t v) const { if (auto* d = direct()) { d->model_parts_aero_kit = v; return; } if (memory::IsHandle(address + 3768)) memory::OwnerStore(address + 3768, 4, v); else guest::StoreU32(base, address + 3768, v); }
  uint32_t model_parts_aero_kit() const { if (auto* d = direct()) return d->model_parts_aero_kit; return uint32_t((memory::IsHandle(address + 3768) ? uint32_t(memory::OwnerLoad(address + 3768, 4)) : guest::LoadU32(base, address + 3768))); }
  void set_model_parts_aero_kit(uint32_t v) const { if (auto* d = direct()) { d->model_parts_aero_kit = v; return; } if (memory::IsHandle(address + 3768)) memory::OwnerStore(address + 3768, 4, v); else guest::StoreU32(base, address + 3768, v); }
  // the skeleton (+3860 its node names, +3924 its matrices). Measure fault: SceneModel::BeamColor's read of a scene model's +3776 (record +3744)
  uint32_t skeleton_address() const { return address + 3776; }
  uint32_t skeleton_bits() const { if (auto* d = direct()) return d->skeleton; return uint32_t((memory::IsHandle(address + 3776) ? uint32_t(memory::OwnerLoad(address + 3776, 4)) : guest::LoadU32(base, address + 3776))); }
  void set_skeleton_bits(uint32_t v) const { if (auto* d = direct()) { d->skeleton = v; return; } if (memory::IsHandle(address + 3776)) memory::OwnerStore(address + 3776, 4, v); else guest::StoreU32(base, address + 3776, v); }
  uint32_t skeleton() const { if (auto* d = direct()) return d->skeleton; return uint32_t((memory::IsHandle(address + 3776) ? uint32_t(memory::OwnerLoad(address + 3776, 4)) : guest::LoadU32(base, address + 3776))); }
  void set_skeleton(uint32_t v) const { if (auto* d = direct()) { d->skeleton = v; return; } if (memory::IsHandle(address + 3776)) memory::OwnerStore(address + 3776, 4, v); else guest::StoreU32(base, address + 3776, v); }
  // measure fault: the stand model's record +3752
  uint32_t model_parts_aero9_address() const { return address + 3784; }
  uint32_t model_parts_aero9_bits() const { if (auto* d = direct()) return d->model_parts_aero9; return uint32_t((memory::IsHandle(address + 3784) ? uint32_t(memory::OwnerLoad(address + 3784, 4)) : guest::LoadU32(base, address + 3784))); }
  void set_model_parts_aero9_bits(uint32_t v) const { if (auto* d = direct()) { d->model_parts_aero9 = v; return; } if (memory::IsHandle(address + 3784)) memory::OwnerStore(address + 3784, 4, v); else guest::StoreU32(base, address + 3784, v); }
  uint32_t model_parts_aero9() const { if (auto* d = direct()) return d->model_parts_aero9; return uint32_t((memory::IsHandle(address + 3784) ? uint32_t(memory::OwnerLoad(address + 3784, 4)) : guest::LoadU32(base, address + 3784))); }
  void set_model_parts_aero9(uint32_t v) const { if (auto* d = direct()) { d->model_parts_aero9 = v; return; } if (memory::IsHandle(address + 3784)) memory::OwnerStore(address + 3784, 4, v); else guest::StoreU32(base, address + 3784, v); }
  // measure fault: the stand model's record +3768 (GarageStage::SetCarClass)
  uint32_t model_parts_class_address() const { return address + 3800; }
  uint32_t model_parts_class_bits() const { if (auto* d = direct()) return d->model_parts_class; return uint32_t((memory::IsHandle(address + 3800) ? uint32_t(memory::OwnerLoad(address + 3800, 4)) : guest::LoadU32(base, address + 3800))); }
  void set_model_parts_class_bits(uint32_t v) const { if (auto* d = direct()) { d->model_parts_class = v; return; } if (memory::IsHandle(address + 3800)) memory::OwnerStore(address + 3800, 4, v); else guest::StoreU32(base, address + 3800, v); }
  uint32_t model_parts_class() const { if (auto* d = direct()) return d->model_parts_class; return uint32_t((memory::IsHandle(address + 3800) ? uint32_t(memory::OwnerLoad(address + 3800, 4)) : guest::LoadU32(base, address + 3800))); }
  void set_model_parts_class(uint32_t v) const { if (auto* d = direct()) { d->model_parts_class = v; return; } if (memory::IsHandle(address + 3800)) memory::OwnerStore(address + 3800, 4, v); else guest::StoreU32(base, address + 3800, v); }
  // measure fault: the stand model's record +3776, 0 to 2 (GarageStage::SetInteriorClass)
  uint32_t model_interior_class_address() const { return address + 3808; }
  uint32_t model_interior_class_bits() const { if (auto* d = direct()) return d->model_interior_class; return uint32_t((memory::IsHandle(address + 3808) ? uint32_t(memory::OwnerLoad(address + 3808, 4)) : guest::LoadU32(base, address + 3808))); }
  void set_model_interior_class_bits(uint32_t v) const { if (auto* d = direct()) { d->model_interior_class = v; return; } if (memory::IsHandle(address + 3808)) memory::OwnerStore(address + 3808, 4, v); else guest::StoreU32(base, address + 3808, v); }
  uint32_t model_interior_class() const { if (auto* d = direct()) return d->model_interior_class; return uint32_t((memory::IsHandle(address + 3808) ? uint32_t(memory::OwnerLoad(address + 3808, 4)) : guest::LoadU32(base, address + 3808))); }
  void set_model_interior_class(uint32_t v) const { if (auto* d = direct()) { d->model_interior_class = v; return; } if (memory::IsHandle(address + 3808)) memory::OwnerStore(address + 3808, 4, v); else guest::StoreU32(base, address + 3808, v); }
  // measure fault: the stand model's record +3864 (GarageStage::SetInteriorWord kind 5)
  uint32_t model_interior_item2_address() const { return address + 3896; }
  uint32_t model_interior_item2_bits() const { if (auto* d = direct()) return d->model_interior_item2; return uint32_t((memory::IsHandle(address + 3896) ? uint32_t(memory::OwnerLoad(address + 3896, 4)) : guest::LoadU32(base, address + 3896))); }
  void set_model_interior_item2_bits(uint32_t v) const { if (auto* d = direct()) { d->model_interior_item2 = v; return; } if (memory::IsHandle(address + 3896)) memory::OwnerStore(address + 3896, 4, v); else guest::StoreU32(base, address + 3896, v); }
  uint32_t model_interior_item2() const { if (auto* d = direct()) return d->model_interior_item2; return uint32_t((memory::IsHandle(address + 3896) ? uint32_t(memory::OwnerLoad(address + 3896, 4)) : guest::LoadU32(base, address + 3896))); }
  void set_model_interior_item2(uint32_t v) const { if (auto* d = direct()) { d->model_interior_item2 = v; return; } if (memory::IsHandle(address + 3896)) memory::OwnerStore(address + 3896, 4, v); else guest::StoreU32(base, address + 3896, v); }
  // measure fault: the stand model's record +3868 (SetInteriorWord kind 4, SetInteriorPair)
  uint32_t model_interior_item1_address() const { return address + 3900; }
  uint32_t model_interior_item1_bits() const { if (auto* d = direct()) return d->model_interior_item1; return uint32_t((memory::IsHandle(address + 3900) ? uint32_t(memory::OwnerLoad(address + 3900, 4)) : guest::LoadU32(base, address + 3900))); }
  void set_model_interior_item1_bits(uint32_t v) const { if (auto* d = direct()) { d->model_interior_item1 = v; return; } if (memory::IsHandle(address + 3900)) memory::OwnerStore(address + 3900, 4, v); else guest::StoreU32(base, address + 3900, v); }
  uint32_t model_interior_item1() const { if (auto* d = direct()) return d->model_interior_item1; return uint32_t((memory::IsHandle(address + 3900) ? uint32_t(memory::OwnerLoad(address + 3900, 4)) : guest::LoadU32(base, address + 3900))); }
  void set_model_interior_item1(uint32_t v) const { if (auto* d = direct()) { d->model_interior_item1 = v; return; } if (memory::IsHandle(address + 3900)) memory::OwnerStore(address + 3900, 4, v); else guest::StoreU32(base, address + 3900, v); }
  // measure fault: the stand model's record +3872 (SetInteriorPair)
  uint32_t model_interior_item0_address() const { return address + 3904; }
  uint32_t model_interior_item0_bits() const { if (auto* d = direct()) return d->model_interior_item0; return uint32_t((memory::IsHandle(address + 3904) ? uint32_t(memory::OwnerLoad(address + 3904, 4)) : guest::LoadU32(base, address + 3904))); }
  void set_model_interior_item0_bits(uint32_t v) const { if (auto* d = direct()) { d->model_interior_item0 = v; return; } if (memory::IsHandle(address + 3904)) memory::OwnerStore(address + 3904, 4, v); else guest::StoreU32(base, address + 3904, v); }
  uint32_t model_interior_item0() const { if (auto* d = direct()) return d->model_interior_item0; return uint32_t((memory::IsHandle(address + 3904) ? uint32_t(memory::OwnerLoad(address + 3904, 4)) : guest::LoadU32(base, address + 3904))); }
  void set_model_interior_item0(uint32_t v) const { if (auto* d = direct()) { d->model_interior_item0 = v; return; } if (memory::IsHandle(address + 3904)) memory::OwnerStore(address + 3904, 4, v); else guest::StoreU32(base, address + 3904, v); }
  // measure fault: the stand model's record +3876 (SetInteriorWord kind 1, SetInteriorLevel)
  uint32_t model_interior_seats_address() const { return address + 3908; }
  uint32_t model_interior_seats_bits() const { if (auto* d = direct()) return d->model_interior_seats; return uint32_t((memory::IsHandle(address + 3908) ? uint32_t(memory::OwnerLoad(address + 3908, 4)) : guest::LoadU32(base, address + 3908))); }
  void set_model_interior_seats_bits(uint32_t v) const { if (auto* d = direct()) { d->model_interior_seats = v; return; } if (memory::IsHandle(address + 3908)) memory::OwnerStore(address + 3908, 4, v); else guest::StoreU32(base, address + 3908, v); }
  uint32_t model_interior_seats() const { if (auto* d = direct()) return d->model_interior_seats; return uint32_t((memory::IsHandle(address + 3908) ? uint32_t(memory::OwnerLoad(address + 3908, 4)) : guest::LoadU32(base, address + 3908))); }
  void set_model_interior_seats(uint32_t v) const { if (auto* d = direct()) { d->model_interior_seats = v; return; } if (memory::IsHandle(address + 3908)) memory::OwnerStore(address + 3908, 4, v); else guest::StoreU32(base, address + 3908, v); }
  // measure fault: the stand model's record +3880 (SetInteriorLevel)
  uint32_t model_interior_has_seats_address() const { return address + 3912; }
  uint32_t model_interior_has_seats_bits() const { if (auto* d = direct()) return d->model_interior_has_seats; return uint32_t((memory::IsHandle(address + 3912) ? uint32_t(memory::OwnerLoad(address + 3912, 4)) : guest::LoadU32(base, address + 3912))); }
  void set_model_interior_has_seats_bits(uint32_t v) const { if (auto* d = direct()) { d->model_interior_has_seats = v; return; } if (memory::IsHandle(address + 3912)) memory::OwnerStore(address + 3912, 4, v); else guest::StoreU32(base, address + 3912, v); }
  uint32_t model_interior_has_seats() const { if (auto* d = direct()) return d->model_interior_has_seats; return uint32_t((memory::IsHandle(address + 3912) ? uint32_t(memory::OwnerLoad(address + 3912, 4)) : guest::LoadU32(base, address + 3912))); }
  void set_model_interior_has_seats(uint32_t v) const { if (auto* d = direct()) { d->model_interior_has_seats = v; return; } if (memory::IsHandle(address + 3912)) memory::OwnerStore(address + 3912, 4, v); else guest::StoreU32(base, address + 3912, v); }
  // measure fault: the stand model's record +3884 (GarageStage::SetAccessory)
  uint32_t model_accessory_address() const { return address + 3916; }
  uint32_t model_accessory_bits() const { if (auto* d = direct()) return d->model_accessory; return uint32_t((memory::IsHandle(address + 3916) ? uint32_t(memory::OwnerLoad(address + 3916, 4)) : guest::LoadU32(base, address + 3916))); }
  void set_model_accessory_bits(uint32_t v) const { if (auto* d = direct()) { d->model_accessory = v; return; } if (memory::IsHandle(address + 3916)) memory::OwnerStore(address + 3916, 4, v); else guest::StoreU32(base, address + 3916, v); }
  uint32_t model_accessory() const { if (auto* d = direct()) return d->model_accessory; return uint32_t((memory::IsHandle(address + 3916) ? uint32_t(memory::OwnerLoad(address + 3916, 4)) : guest::LoadU32(base, address + 3916))); }
  void set_model_accessory(uint32_t v) const { if (auto* d = direct()) { d->model_accessory = v; return; } if (memory::IsHandle(address + 3916)) memory::OwnerStore(address + 3916, 4, v); else guest::StoreU32(base, address + 3916, v); }
  // measure fault: the stand model's record +3888
  uint32_t model_accessory_variant_address() const { return address + 3920; }
  uint32_t model_accessory_variant_bits() const { if (auto* d = direct()) return d->model_accessory_variant; return uint32_t((memory::IsHandle(address + 3920) ? uint32_t(memory::OwnerLoad(address + 3920, 4)) : guest::LoadU32(base, address + 3920))); }
  void set_model_accessory_variant_bits(uint32_t v) const { if (auto* d = direct()) { d->model_accessory_variant = v; return; } if (memory::IsHandle(address + 3920)) memory::OwnerStore(address + 3920, 4, v); else guest::StoreU32(base, address + 3920, v); }
  uint32_t model_accessory_variant() const { if (auto* d = direct()) return d->model_accessory_variant; return uint32_t((memory::IsHandle(address + 3920) ? uint32_t(memory::OwnerLoad(address + 3920, 4)) : guest::LoadU32(base, address + 3920))); }
  void set_model_accessory_variant(uint32_t v) const { if (auto* d = direct()) { d->model_accessory_variant = v; return; } if (memory::IsHandle(address + 3920)) memory::OwnerStore(address + 3920, 4, v); else guest::StoreU32(base, address + 3920, v); }
  // a texture provider (TrafficCar::Reset; another at +4084)
  uint32_t texture_provider_address() const { return address + 4064; }
  uint32_t texture_provider_bits() const { if (auto* d = direct()) return d->texture_provider; return uint32_t((memory::IsHandle(address + 4064) ? uint32_t(memory::OwnerLoad(address + 4064, 4)) : guest::LoadU32(base, address + 4064))); }
  void set_texture_provider_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_provider = v; return; } if (memory::IsHandle(address + 4064)) memory::OwnerStore(address + 4064, 4, v); else guest::StoreU32(base, address + 4064, v); }
  uint32_t texture_provider() const { if (auto* d = direct()) return d->texture_provider; return uint32_t((memory::IsHandle(address + 4064) ? uint32_t(memory::OwnerLoad(address + 4064, 4)) : guest::LoadU32(base, address + 4064))); }
  void set_texture_provider(uint32_t v) const { if (auto* d = direct()) { d->texture_provider = v; return; } if (memory::IsHandle(address + 4064)) memory::OwnerStore(address + 4064, 4, v); else guest::StoreU32(base, address + 4064, v); }
  // measure fault: the stand model's record +4044, the wheels' +8 (GarageStage::SetWheelWord)
  uint32_t model_wheels_word_address() const { return address + 4076; }
  uint32_t model_wheels_word_bits() const { if (auto* d = direct()) return d->model_wheels_word; return uint32_t((memory::IsHandle(address + 4076) ? uint32_t(memory::OwnerLoad(address + 4076, 4)) : guest::LoadU32(base, address + 4076))); }
  void set_model_wheels_word_bits(uint32_t v) const { if (auto* d = direct()) { d->model_wheels_word = v; return; } if (memory::IsHandle(address + 4076)) memory::OwnerStore(address + 4076, 4, v); else guest::StoreU32(base, address + 4076, v); }
  uint32_t model_wheels_word() const { if (auto* d = direct()) return d->model_wheels_word; return uint32_t((memory::IsHandle(address + 4076) ? uint32_t(memory::OwnerLoad(address + 4076, 4)) : guest::LoadU32(base, address + 4076))); }
  void set_model_wheels_word(uint32_t v) const { if (auto* d = direct()) { d->model_wheels_word = v; return; } if (memory::IsHandle(address + 4076)) memory::OwnerStore(address + 4076, 4, v); else guest::StoreU32(base, address + 4076, v); }
  // measure fault: the stand model's record +4048 (GarageStage::SetTireGrade)
  uint32_t model_front_tire_grade_address() const { return address + 4080; }
  uint32_t model_front_tire_grade_bits() const { if (auto* d = direct()) return d->model_front_tire_grade; return uint32_t((memory::IsHandle(address + 4080) ? uint32_t(memory::OwnerLoad(address + 4080, 4)) : guest::LoadU32(base, address + 4080))); }
  void set_model_front_tire_grade_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_tire_grade = v; return; } if (memory::IsHandle(address + 4080)) memory::OwnerStore(address + 4080, 4, v); else guest::StoreU32(base, address + 4080, v); }
  uint32_t model_front_tire_grade() const { if (auto* d = direct()) return d->model_front_tire_grade; return uint32_t((memory::IsHandle(address + 4080) ? uint32_t(memory::OwnerLoad(address + 4080, 4)) : guest::LoadU32(base, address + 4080))); }
  void set_model_front_tire_grade(uint32_t v) const { if (auto* d = direct()) { d->model_front_tire_grade = v; return; } if (memory::IsHandle(address + 4080)) memory::OwnerStore(address + 4080, 4, v); else guest::StoreU32(base, address + 4080, v); }
  // measure fault: the stand model's record +4052, the wheel number - 1 (GarageStage::SetWheel)
  uint32_t model_front_wheel_address() const { return address + 4084; }
  uint32_t model_front_wheel_bits() const { if (auto* d = direct()) return d->model_front_wheel; return uint32_t((memory::IsHandle(address + 4084) ? uint32_t(memory::OwnerLoad(address + 4084, 4)) : guest::LoadU32(base, address + 4084))); }
  void set_model_front_wheel_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel = v; return; } if (memory::IsHandle(address + 4084)) memory::OwnerStore(address + 4084, 4, v); else guest::StoreU32(base, address + 4084, v); }
  uint32_t model_front_wheel() const { if (auto* d = direct()) return d->model_front_wheel; return uint32_t((memory::IsHandle(address + 4084) ? uint32_t(memory::OwnerLoad(address + 4084, 4)) : guest::LoadU32(base, address + 4084))); }
  void set_model_front_wheel(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel = v; return; } if (memory::IsHandle(address + 4084)) memory::OwnerStore(address + 4084, 4, v); else guest::StoreU32(base, address + 4084, v); }
  // measure fault: the stand model's record +4056
  uint32_t model_front_wheel_f28_address() const { return address + 4088; }
  uint32_t model_front_wheel_f28_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_f28); return uint32_t((memory::IsHandle(address + 4088) ? uint32_t(memory::OwnerLoad(address + 4088, 4)) : guest::LoadU32(base, address + 4088))); }
  void set_model_front_wheel_f28_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_f28 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4088)) memory::OwnerStore(address + 4088, 4, v); else guest::StoreU32(base, address + 4088, v); }
  float model_front_wheel_f28() const { return std::bit_cast<float>(model_front_wheel_f28_bits()); }
  void set_model_front_wheel_f28(float v) const { set_model_front_wheel_f28_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's record +4060
  uint32_t model_front_wheel_f16_address() const { return address + 4092; }
  uint32_t model_front_wheel_f16_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_wheel_f16); return uint32_t((memory::IsHandle(address + 4092) ? uint32_t(memory::OwnerLoad(address + 4092, 4)) : guest::LoadU32(base, address + 4092))); }
  void set_model_front_wheel_f16_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_wheel_f16 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4092)) memory::OwnerStore(address + 4092, 4, v); else guest::StoreU32(base, address + 4092, v); }
  float model_front_wheel_f16() const { return std::bit_cast<float>(model_front_wheel_f16_bits()); }
  void set_model_front_wheel_f16(float v) const { set_model_front_wheel_f16_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's record +4064
  uint32_t model_rear_tire_grade_address() const { return address + 4096; }
  uint32_t model_rear_tire_grade_bits() const { if (auto* d = direct()) return d->model_rear_tire_grade; return uint32_t((memory::IsHandle(address + 4096) ? uint32_t(memory::OwnerLoad(address + 4096, 4)) : guest::LoadU32(base, address + 4096))); }
  void set_model_rear_tire_grade_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_tire_grade = v; return; } if (memory::IsHandle(address + 4096)) memory::OwnerStore(address + 4096, 4, v); else guest::StoreU32(base, address + 4096, v); }
  uint32_t model_rear_tire_grade() const { if (auto* d = direct()) return d->model_rear_tire_grade; return uint32_t((memory::IsHandle(address + 4096) ? uint32_t(memory::OwnerLoad(address + 4096, 4)) : guest::LoadU32(base, address + 4096))); }
  void set_model_rear_tire_grade(uint32_t v) const { if (auto* d = direct()) { d->model_rear_tire_grade = v; return; } if (memory::IsHandle(address + 4096)) memory::OwnerStore(address + 4096, 4, v); else guest::StoreU32(base, address + 4096, v); }
  // measure fault: the stand model's record +4068
  uint32_t model_rear_wheel_address() const { return address + 4100; }
  uint32_t model_rear_wheel_bits() const { if (auto* d = direct()) return d->model_rear_wheel; return uint32_t((memory::IsHandle(address + 4100) ? uint32_t(memory::OwnerLoad(address + 4100, 4)) : guest::LoadU32(base, address + 4100))); }
  void set_model_rear_wheel_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel = v; return; } if (memory::IsHandle(address + 4100)) memory::OwnerStore(address + 4100, 4, v); else guest::StoreU32(base, address + 4100, v); }
  uint32_t model_rear_wheel() const { if (auto* d = direct()) return d->model_rear_wheel; return uint32_t((memory::IsHandle(address + 4100) ? uint32_t(memory::OwnerLoad(address + 4100, 4)) : guest::LoadU32(base, address + 4100))); }
  void set_model_rear_wheel(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel = v; return; } if (memory::IsHandle(address + 4100)) memory::OwnerStore(address + 4100, 4, v); else guest::StoreU32(base, address + 4100, v); }
  // six 24-byte slots (+4104 + 24i, not loaded by the file callback). The stand model: record +4072 to +4215, the wheels' and light kit's words below
  uint32_t slots_address() const { return address + 4104; }
  uint8_t* slots() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4104 : guest::Pointer(base, address + 4104); }
  // measure fault: the stand model's record +4072
  uint32_t model_rear_wheel_f28_address() const { return address + 4104; }
  uint32_t model_rear_wheel_f28_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_f28); return uint32_t((memory::IsHandle(address + 4104) ? uint32_t(memory::OwnerLoad(address + 4104, 4)) : guest::LoadU32(base, address + 4104))); }
  void set_model_rear_wheel_f28_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_f28 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4104)) memory::OwnerStore(address + 4104, 4, v); else guest::StoreU32(base, address + 4104, v); }
  float model_rear_wheel_f28() const { return std::bit_cast<float>(model_rear_wheel_f28_bits()); }
  void set_model_rear_wheel_f28(float v) const { set_model_rear_wheel_f28_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's record +4076
  uint32_t model_rear_wheel_f16_address() const { return address + 4108; }
  uint32_t model_rear_wheel_f16_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_wheel_f16); return uint32_t((memory::IsHandle(address + 4108) ? uint32_t(memory::OwnerLoad(address + 4108, 4)) : guest::LoadU32(base, address + 4108))); }
  void set_model_rear_wheel_f16_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_wheel_f16 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4108)) memory::OwnerStore(address + 4108, 4, v); else guest::StoreU32(base, address + 4108, v); }
  float model_rear_wheel_f16() const { return std::bit_cast<float>(model_rear_wheel_f16_bits()); }
  void set_model_rear_wheel_f16(float v) const { set_model_rear_wheel_f16_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's record +4080, the front tire copy's +80 truncated
  uint32_t model_front_size_address() const { return address + 4112; }
  uint32_t model_front_size_bits() const { if (auto* d = direct()) return d->model_front_size; return uint32_t((memory::IsHandle(address + 4112) ? uint32_t(memory::OwnerLoad(address + 4112, 4)) : guest::LoadU32(base, address + 4112))); }
  void set_model_front_size_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_size = v; return; } if (memory::IsHandle(address + 4112)) memory::OwnerStore(address + 4112, 4, v); else guest::StoreU32(base, address + 4112, v); }
  uint32_t model_front_size() const { if (auto* d = direct()) return d->model_front_size; return uint32_t((memory::IsHandle(address + 4112) ? uint32_t(memory::OwnerLoad(address + 4112, 4)) : guest::LoadU32(base, address + 4112))); }
  void set_model_front_size(uint32_t v) const { if (auto* d = direct()) { d->model_front_size = v; return; } if (memory::IsHandle(address + 4112)) memory::OwnerStore(address + 4112, 4, v); else guest::StoreU32(base, address + 4112, v); }
  // measure fault: the record's +4080 and +4084 passed to SceneWheels::SetSizes
  uint32_t model_front_size_pair_address() const { return address + 4112; }
  uint64_t model_front_size_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTrafficCarLayout, 4112, 8)); return uint64_t((memory::IsHandle(address + 4112) ? uint64_t(memory::OwnerLoad(address + 4112, 8)) : guest::LoadU64(base, address + 4112))); }
  void set_model_front_size_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTrafficCarLayout, 4112, 8, v); return; } if (memory::IsHandle(address + 4112)) memory::OwnerStore(address + 4112, 8, v); else guest::StoreU64(base, address + 4112, v); }
  uint64_t model_front_size_pair() const { return uint64_t(model_front_size_pair_bits()); }
  void set_model_front_size_pair(uint64_t v) const { set_model_front_size_pair_bits(uint64_t(v)); }
  // measure fault: the stand model's record +4084
  uint32_t model_front_size_a_address() const { return address + 4116; }
  uint32_t model_front_size_a_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_size_a); return uint32_t((memory::IsHandle(address + 4116) ? uint32_t(memory::OwnerLoad(address + 4116, 4)) : guest::LoadU32(base, address + 4116))); }
  void set_model_front_size_a_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_size_a = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4116)) memory::OwnerStore(address + 4116, 4, v); else guest::StoreU32(base, address + 4116, v); }
  float model_front_size_a() const { return std::bit_cast<float>(model_front_size_a_bits()); }
  void set_model_front_size_a(float v) const { set_model_front_size_a_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's record +4088
  uint32_t model_front_size_b_address() const { return address + 4120; }
  uint32_t model_front_size_b_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_front_size_b); return uint32_t((memory::IsHandle(address + 4120) ? uint32_t(memory::OwnerLoad(address + 4120, 4)) : guest::LoadU32(base, address + 4120))); }
  void set_model_front_size_b_bits(uint32_t v) const { if (auto* d = direct()) { d->model_front_size_b = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4120)) memory::OwnerStore(address + 4120, 4, v); else guest::StoreU32(base, address + 4120, v); }
  float model_front_size_b() const { return std::bit_cast<float>(model_front_size_b_bits()); }
  void set_model_front_size_b(float v) const { set_model_front_size_b_bits(std::bit_cast<uint32_t>(v)); }
  // slot 0's state (TrafficCar::SetPaint). The stand model: record +4092, the rear tire copy's +80 truncated
  uint32_t slot0_state_address() const { return address + 4124; }
  uint32_t slot0_state_bits() const { if (auto* d = direct()) return d->slot0_state; return uint32_t((memory::IsHandle(address + 4124) ? uint32_t(memory::OwnerLoad(address + 4124, 4)) : guest::LoadU32(base, address + 4124))); }
  void set_slot0_state_bits(uint32_t v) const { if (auto* d = direct()) { d->slot0_state = v; return; } if (memory::IsHandle(address + 4124)) memory::OwnerStore(address + 4124, 4, v); else guest::StoreU32(base, address + 4124, v); }
  uint32_t slot0_state() const { if (auto* d = direct()) return d->slot0_state; return uint32_t((memory::IsHandle(address + 4124) ? uint32_t(memory::OwnerLoad(address + 4124, 4)) : guest::LoadU32(base, address + 4124))); }
  void set_slot0_state(uint32_t v) const { if (auto* d = direct()) { d->slot0_state = v; return; } if (memory::IsHandle(address + 4124)) memory::OwnerStore(address + 4124, 4, v); else guest::StoreU32(base, address + 4124, v); }
  // measure fault: the record's +4092 and +4096 passed to SceneWheels::SetSizes
  uint32_t model_rear_size_pair_address() const { return address + 4124; }
  uint64_t model_rear_size_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTrafficCarLayout, 4124, 8)); return uint64_t((memory::IsHandle(address + 4124) ? uint64_t(memory::OwnerLoad(address + 4124, 8)) : guest::LoadU64(base, address + 4124))); }
  void set_model_rear_size_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTrafficCarLayout, 4124, 8, v); return; } if (memory::IsHandle(address + 4124)) memory::OwnerStore(address + 4124, 8, v); else guest::StoreU64(base, address + 4124, v); }
  uint64_t model_rear_size_pair() const { return uint64_t(model_rear_size_pair_bits()); }
  void set_model_rear_size_pair(uint64_t v) const { set_model_rear_size_pair_bits(uint64_t(v)); }
  // measure fault: the stand model's record +4096
  uint32_t model_rear_size_a_address() const { return address + 4128; }
  uint32_t model_rear_size_a_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_size_a); return uint32_t((memory::IsHandle(address + 4128) ? uint32_t(memory::OwnerLoad(address + 4128, 4)) : guest::LoadU32(base, address + 4128))); }
  void set_model_rear_size_a_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_size_a = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4128)) memory::OwnerStore(address + 4128, 4, v); else guest::StoreU32(base, address + 4128, v); }
  float model_rear_size_a() const { return std::bit_cast<float>(model_rear_size_a_bits()); }
  void set_model_rear_size_a(float v) const { set_model_rear_size_a_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's record +4100
  uint32_t model_rear_size_b_address() const { return address + 4132; }
  uint32_t model_rear_size_b_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_rear_size_b); return uint32_t((memory::IsHandle(address + 4132) ? uint32_t(memory::OwnerLoad(address + 4132, 4)) : guest::LoadU32(base, address + 4132))); }
  void set_model_rear_size_b_bits(uint32_t v) const { if (auto* d = direct()) { d->model_rear_size_b = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4132)) memory::OwnerStore(address + 4132, 4, v); else guest::StoreU32(base, address + 4132, v); }
  float model_rear_size_b() const { return std::bit_cast<float>(model_rear_size_b_bits()); }
  void set_model_rear_size_b(float v) const { set_model_rear_size_b_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's record +4104, the wheels' +68 (GarageStage::SetWheelExtras)
  uint32_t model_wheel_extra_address() const { return address + 4136; }
  uint32_t model_wheel_extra_bits() const { if (auto* d = direct()) return d->model_wheel_extra; return uint32_t((memory::IsHandle(address + 4136) ? uint32_t(memory::OwnerLoad(address + 4136, 4)) : guest::LoadU32(base, address + 4136))); }
  void set_model_wheel_extra_bits(uint32_t v) const { if (auto* d = direct()) { d->model_wheel_extra = v; return; } if (memory::IsHandle(address + 4136)) memory::OwnerStore(address + 4136, 4, v); else guest::StoreU32(base, address + 4136, v); }
  uint32_t model_wheel_extra() const { if (auto* d = direct()) return d->model_wheel_extra; return uint32_t((memory::IsHandle(address + 4136) ? uint32_t(memory::OwnerLoad(address + 4136, 4)) : guest::LoadU32(base, address + 4136))); }
  void set_model_wheel_extra(uint32_t v) const { if (auto* d = direct()) { d->model_wheel_extra = v; return; } if (memory::IsHandle(address + 4136)) memory::OwnerStore(address + 4136, 4, v); else guest::StoreU32(base, address + 4136, v); }
  // measure fault: the stand model's record +4108, the wheels' +72 (SceneWheels::SetSizes' a)
  uint32_t model_wheel_size_a_address() const { return address + 4140; }
  uint32_t model_wheel_size_a_bits() const { if (auto* d = direct()) return d->model_wheel_size_a; return uint32_t((memory::IsHandle(address + 4140) ? uint32_t(memory::OwnerLoad(address + 4140, 4)) : guest::LoadU32(base, address + 4140))); }
  void set_model_wheel_size_a_bits(uint32_t v) const { if (auto* d = direct()) { d->model_wheel_size_a = v; return; } if (memory::IsHandle(address + 4140)) memory::OwnerStore(address + 4140, 4, v); else guest::StoreU32(base, address + 4140, v); }
  uint32_t model_wheel_size_a() const { if (auto* d = direct()) return d->model_wheel_size_a; return uint32_t((memory::IsHandle(address + 4140) ? uint32_t(memory::OwnerLoad(address + 4140, 4)) : guest::LoadU32(base, address + 4140))); }
  void set_model_wheel_size_a(uint32_t v) const { if (auto* d = direct()) { d->model_wheel_size_a = v; return; } if (memory::IsHandle(address + 4140)) memory::OwnerStore(address + 4140, 4, v); else guest::StoreU32(base, address + 4140, v); }
  // measure fault: the stand model's record +4112, the wheels' +76 (SetSizes' b)
  uint32_t model_wheel_size_b_address() const { return address + 4144; }
  uint32_t model_wheel_size_b_bits() const { if (auto* d = direct()) return d->model_wheel_size_b; return uint32_t((memory::IsHandle(address + 4144) ? uint32_t(memory::OwnerLoad(address + 4144, 4)) : guest::LoadU32(base, address + 4144))); }
  void set_model_wheel_size_b_bits(uint32_t v) const { if (auto* d = direct()) { d->model_wheel_size_b = v; return; } if (memory::IsHandle(address + 4144)) memory::OwnerStore(address + 4144, 4, v); else guest::StoreU32(base, address + 4144, v); }
  uint32_t model_wheel_size_b() const { if (auto* d = direct()) return d->model_wheel_size_b; return uint32_t((memory::IsHandle(address + 4144) ? uint32_t(memory::OwnerLoad(address + 4144, 4)) : guest::LoadU32(base, address + 4144))); }
  void set_model_wheel_size_b(uint32_t v) const { if (auto* d = direct()) { d->model_wheel_size_b = v; return; } if (memory::IsHandle(address + 4144)) memory::OwnerStore(address + 4144, 4, v); else guest::StoreU32(base, address + 4144, v); }
  // measure fault: the stand model's record +4124, the wheels' +88 (SetWheelExtras)
  uint32_t model_caliper_texture_address() const { return address + 4156; }
  uint32_t model_caliper_texture_bits() const { if (auto* d = direct()) return d->model_caliper_texture; return uint32_t((memory::IsHandle(address + 4156) ? uint32_t(memory::OwnerLoad(address + 4156, 4)) : guest::LoadU32(base, address + 4156))); }
  void set_model_caliper_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->model_caliper_texture = v; return; } if (memory::IsHandle(address + 4156)) memory::OwnerStore(address + 4156, 4, v); else guest::StoreU32(base, address + 4156, v); }
  uint32_t model_caliper_texture() const { if (auto* d = direct()) return d->model_caliper_texture; return uint32_t((memory::IsHandle(address + 4156) ? uint32_t(memory::OwnerLoad(address + 4156, 4)) : guest::LoadU32(base, address + 4156))); }
  void set_model_caliper_texture(uint32_t v) const { if (auto* d = direct()) { d->model_caliper_texture = v; return; } if (memory::IsHandle(address + 4156)) memory::OwnerStore(address + 4156, 4, v); else guest::StoreU32(base, address + 4156, v); }
  // measure fault: the stand model's record +4192 to +4240, the light kit's 52 bytes (GarageStage::SetLightKit, ClearLightKit)
  uint32_t model_light_kit0_b_address() const { return address + 4224; }
  uint32_t model_light_kit0_b_bits() const { if (auto* d = direct()) return d->model_light_kit0_b; return uint32_t((memory::IsHandle(address + 4224) ? uint32_t(memory::OwnerLoad(address + 4224, 4)) : guest::LoadU32(base, address + 4224))); }
  void set_model_light_kit0_b_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit0_b = v; return; } if (memory::IsHandle(address + 4224)) memory::OwnerStore(address + 4224, 4, v); else guest::StoreU32(base, address + 4224, v); }
  uint32_t model_light_kit0_b() const { if (auto* d = direct()) return d->model_light_kit0_b; return uint32_t((memory::IsHandle(address + 4224) ? uint32_t(memory::OwnerLoad(address + 4224, 4)) : guest::LoadU32(base, address + 4224))); }
  void set_model_light_kit0_b(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit0_b = v; return; } if (memory::IsHandle(address + 4224)) memory::OwnerStore(address + 4224, 4, v); else guest::StoreU32(base, address + 4224, v); }
  uint32_t model_light_kit0_a_address() const { return address + 4228; }
  uint32_t model_light_kit0_a_bits() const { if (auto* d = direct()) return d->model_light_kit0_a; return uint32_t((memory::IsHandle(address + 4228) ? uint32_t(memory::OwnerLoad(address + 4228, 4)) : guest::LoadU32(base, address + 4228))); }
  void set_model_light_kit0_a_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit0_a = v; return; } if (memory::IsHandle(address + 4228)) memory::OwnerStore(address + 4228, 4, v); else guest::StoreU32(base, address + 4228, v); }
  uint32_t model_light_kit0_a() const { if (auto* d = direct()) return d->model_light_kit0_a; return uint32_t((memory::IsHandle(address + 4228) ? uint32_t(memory::OwnerLoad(address + 4228, 4)) : guest::LoadU32(base, address + 4228))); }
  void set_model_light_kit0_a(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit0_a = v; return; } if (memory::IsHandle(address + 4228)) memory::OwnerStore(address + 4228, 4, v); else guest::StoreU32(base, address + 4228, v); }
  uint32_t model_light_kit0_f8_address() const { return address + 4232; }
  uint32_t model_light_kit0_f8_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit0_f8); return uint32_t((memory::IsHandle(address + 4232) ? uint32_t(memory::OwnerLoad(address + 4232, 4)) : guest::LoadU32(base, address + 4232))); }
  void set_model_light_kit0_f8_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit0_f8 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4232)) memory::OwnerStore(address + 4232, 4, v); else guest::StoreU32(base, address + 4232, v); }
  float model_light_kit0_f8() const { return std::bit_cast<float>(model_light_kit0_f8_bits()); }
  void set_model_light_kit0_f8(float v) const { set_model_light_kit0_f8_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_light_kit0_f12_address() const { return address + 4236; }
  uint32_t model_light_kit0_f12_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit0_f12); return uint32_t((memory::IsHandle(address + 4236) ? uint32_t(memory::OwnerLoad(address + 4236, 4)) : guest::LoadU32(base, address + 4236))); }
  void set_model_light_kit0_f12_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit0_f12 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4236)) memory::OwnerStore(address + 4236, 4, v); else guest::StoreU32(base, address + 4236, v); }
  float model_light_kit0_f12() const { return std::bit_cast<float>(model_light_kit0_f12_bits()); }
  void set_model_light_kit0_f12(float v) const { set_model_light_kit0_f12_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_light_kit0_f16_address() const { return address + 4240; }
  uint32_t model_light_kit0_f16_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit0_f16); return uint32_t((memory::IsHandle(address + 4240) ? uint32_t(memory::OwnerLoad(address + 4240, 4)) : guest::LoadU32(base, address + 4240))); }
  void set_model_light_kit0_f16_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit0_f16 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4240)) memory::OwnerStore(address + 4240, 4, v); else guest::StoreU32(base, address + 4240, v); }
  float model_light_kit0_f16() const { return std::bit_cast<float>(model_light_kit0_f16_bits()); }
  void set_model_light_kit0_f16(float v) const { set_model_light_kit0_f16_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_light_kit1_a_address() const { return address + 4244; }
  uint32_t model_light_kit1_a_bits() const { if (auto* d = direct()) return d->model_light_kit1_a; return uint32_t((memory::IsHandle(address + 4244) ? uint32_t(memory::OwnerLoad(address + 4244, 4)) : guest::LoadU32(base, address + 4244))); }
  void set_model_light_kit1_a_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit1_a = v; return; } if (memory::IsHandle(address + 4244)) memory::OwnerStore(address + 4244, 4, v); else guest::StoreU32(base, address + 4244, v); }
  uint32_t model_light_kit1_a() const { if (auto* d = direct()) return d->model_light_kit1_a; return uint32_t((memory::IsHandle(address + 4244) ? uint32_t(memory::OwnerLoad(address + 4244, 4)) : guest::LoadU32(base, address + 4244))); }
  void set_model_light_kit1_a(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit1_a = v; return; } if (memory::IsHandle(address + 4244)) memory::OwnerStore(address + 4244, 4, v); else guest::StoreU32(base, address + 4244, v); }
  uint32_t model_light_kit1_f8_address() const { return address + 4248; }
  uint32_t model_light_kit1_f8_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit1_f8); return uint32_t((memory::IsHandle(address + 4248) ? uint32_t(memory::OwnerLoad(address + 4248, 4)) : guest::LoadU32(base, address + 4248))); }
  void set_model_light_kit1_f8_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit1_f8 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4248)) memory::OwnerStore(address + 4248, 4, v); else guest::StoreU32(base, address + 4248, v); }
  float model_light_kit1_f8() const { return std::bit_cast<float>(model_light_kit1_f8_bits()); }
  void set_model_light_kit1_f8(float v) const { set_model_light_kit1_f8_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_light_kit1_f12_address() const { return address + 4252; }
  uint32_t model_light_kit1_f12_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit1_f12); return uint32_t((memory::IsHandle(address + 4252) ? uint32_t(memory::OwnerLoad(address + 4252, 4)) : guest::LoadU32(base, address + 4252))); }
  void set_model_light_kit1_f12_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit1_f12 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4252)) memory::OwnerStore(address + 4252, 4, v); else guest::StoreU32(base, address + 4252, v); }
  float model_light_kit1_f12() const { return std::bit_cast<float>(model_light_kit1_f12_bits()); }
  void set_model_light_kit1_f12(float v) const { set_model_light_kit1_f12_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_light_kit1_f16_address() const { return address + 4256; }
  uint32_t model_light_kit1_f16_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit1_f16); return uint32_t((memory::IsHandle(address + 4256) ? uint32_t(memory::OwnerLoad(address + 4256, 4)) : guest::LoadU32(base, address + 4256))); }
  void set_model_light_kit1_f16_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit1_f16 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4256)) memory::OwnerStore(address + 4256, 4, v); else guest::StoreU32(base, address + 4256, v); }
  float model_light_kit1_f16() const { return std::bit_cast<float>(model_light_kit1_f16_bits()); }
  void set_model_light_kit1_f16(float v) const { set_model_light_kit1_f16_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_light_kit2_a_address() const { return address + 4260; }
  uint32_t model_light_kit2_a_bits() const { if (auto* d = direct()) return d->model_light_kit2_a; return uint32_t((memory::IsHandle(address + 4260) ? uint32_t(memory::OwnerLoad(address + 4260, 4)) : guest::LoadU32(base, address + 4260))); }
  void set_model_light_kit2_a_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit2_a = v; return; } if (memory::IsHandle(address + 4260)) memory::OwnerStore(address + 4260, 4, v); else guest::StoreU32(base, address + 4260, v); }
  uint32_t model_light_kit2_a() const { if (auto* d = direct()) return d->model_light_kit2_a; return uint32_t((memory::IsHandle(address + 4260) ? uint32_t(memory::OwnerLoad(address + 4260, 4)) : guest::LoadU32(base, address + 4260))); }
  void set_model_light_kit2_a(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit2_a = v; return; } if (memory::IsHandle(address + 4260)) memory::OwnerStore(address + 4260, 4, v); else guest::StoreU32(base, address + 4260, v); }
  uint32_t model_light_kit2_f8_address() const { return address + 4264; }
  uint32_t model_light_kit2_f8_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit2_f8); return uint32_t((memory::IsHandle(address + 4264) ? uint32_t(memory::OwnerLoad(address + 4264, 4)) : guest::LoadU32(base, address + 4264))); }
  void set_model_light_kit2_f8_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit2_f8 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4264)) memory::OwnerStore(address + 4264, 4, v); else guest::StoreU32(base, address + 4264, v); }
  float model_light_kit2_f8() const { return std::bit_cast<float>(model_light_kit2_f8_bits()); }
  void set_model_light_kit2_f8(float v) const { set_model_light_kit2_f8_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_light_kit2_f12_address() const { return address + 4268; }
  uint32_t model_light_kit2_f12_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit2_f12); return uint32_t((memory::IsHandle(address + 4268) ? uint32_t(memory::OwnerLoad(address + 4268, 4)) : guest::LoadU32(base, address + 4268))); }
  void set_model_light_kit2_f12_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit2_f12 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4268)) memory::OwnerStore(address + 4268, 4, v); else guest::StoreU32(base, address + 4268, v); }
  float model_light_kit2_f12() const { return std::bit_cast<float>(model_light_kit2_f12_bits()); }
  void set_model_light_kit2_f12(float v) const { set_model_light_kit2_f12_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t model_light_kit2_f16_address() const { return address + 4272; }
  uint32_t model_light_kit2_f16_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_kit2_f16); return uint32_t((memory::IsHandle(address + 4272) ? uint32_t(memory::OwnerLoad(address + 4272, 4)) : guest::LoadU32(base, address + 4272))); }
  void set_model_light_kit2_f16_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_kit2_f16 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4272)) memory::OwnerStore(address + 4272, 4, v); else guest::StoreU32(base, address + 4272, v); }
  float model_light_kit2_f16() const { return std::bit_cast<float>(model_light_kit2_f16_bits()); }
  void set_model_light_kit2_f16(float v) const { set_model_light_kit2_f16_bits(std::bit_cast<uint32_t>(v)); }
  // the six part models (+4256 + 464i)
  uint32_t part_models_address() const { return address + 4256; }
  uint8_t* part_models() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4256 : guest::Pointer(base, address + 4256); }
  // measure fault: a scene model's draw block +0 (SceneModel::Beam, BeamShape, BeamColor)
  uint32_t model_draw_block_head_address() const { return address + 4288; }
  uint64_t model_draw_block_head_bits() const { if (auto* d = direct()) return d->model_draw_block_head; return uint64_t((memory::IsHandle(address + 4288) ? uint64_t(memory::OwnerLoad(address + 4288, 8)) : guest::LoadU64(base, address + 4288))); }
  void set_model_draw_block_head_bits(uint64_t v) const { if (auto* d = direct()) { d->model_draw_block_head = v; return; } if (memory::IsHandle(address + 4288)) memory::OwnerStore(address + 4288, 8, v); else guest::StoreU64(base, address + 4288, v); }
  uint64_t model_draw_block_head() const { if (auto* d = direct()) return d->model_draw_block_head; return uint64_t((memory::IsHandle(address + 4288) ? uint64_t(memory::OwnerLoad(address + 4288, 8)) : guest::LoadU64(base, address + 4288))); }
  void set_model_draw_block_head(uint64_t v) const { if (auto* d = direct()) { d->model_draw_block_head = v; return; } if (memory::IsHandle(address + 4288)) memory::OwnerStore(address + 4288, 8, v); else guest::StoreU64(base, address + 4288, v); }
  // measure fault: the stand model's draw word (the draw block's +8; GarageStage::Draw, MakeModel, ReloadModel, SetDrawWord, StandScene::PollTeardown). The traffic body: a part model's word
  uint32_t model_draw_word_address() const { return address + 4296; }
  uint32_t model_draw_word_bits() const { if (auto* d = direct()) return d->model_draw_word; return uint32_t((memory::IsHandle(address + 4296) ? uint32_t(memory::OwnerLoad(address + 4296, 4)) : guest::LoadU32(base, address + 4296))); }
  void set_model_draw_word_bits(uint32_t v) const { if (auto* d = direct()) { d->model_draw_word = v; return; } if (memory::IsHandle(address + 4296)) memory::OwnerStore(address + 4296, 4, v); else guest::StoreU32(base, address + 4296, v); }
  uint32_t model_draw_word() const { if (auto* d = direct()) return d->model_draw_word; return uint32_t((memory::IsHandle(address + 4296) ? uint32_t(memory::OwnerLoad(address + 4296, 4)) : guest::LoadU32(base, address + 4296))); }
  void set_model_draw_word(uint32_t v) const { if (auto* d = direct()) { d->model_draw_word = v; return; } if (memory::IsHandle(address + 4296)) memory::OwnerStore(address + 4296, 4, v); else guest::StoreU32(base, address + 4296, v); }
  // part model 0's +48 (TrafficCar::BindTextures)
  uint32_t unknown_4304_address() const { return address + 4304; }
  uint32_t unknown_4304_bits() const { if (auto* d = direct()) return d->unknown_4304; return uint32_t((memory::IsHandle(address + 4304) ? uint32_t(memory::OwnerLoad(address + 4304, 4)) : guest::LoadU32(base, address + 4304))); }
  void set_unknown_4304_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4304 = v; return; } if (memory::IsHandle(address + 4304)) memory::OwnerStore(address + 4304, 4, v); else guest::StoreU32(base, address + 4304, v); }
  uint32_t unknown_4304() const { if (auto* d = direct()) return d->unknown_4304; return uint32_t((memory::IsHandle(address + 4304) ? uint32_t(memory::OwnerLoad(address + 4304, 4)) : guest::LoadU32(base, address + 4304))); }
  void set_unknown_4304(uint32_t v) const { if (auto* d = direct()) { d->unknown_4304 = v; return; } if (memory::IsHandle(address + 4304)) memory::OwnerStore(address + 4304, 4, v); else guest::StoreU32(base, address + 4304, v); }
  // part model 0's +436 (TrafficCar::BindTextures)
  uint32_t unknown_4692_address() const { return address + 4692; }
  uint32_t unknown_4692_bits() const { if (auto* d = direct()) return d->unknown_4692; return uint32_t((memory::IsHandle(address + 4692) ? uint32_t(memory::OwnerLoad(address + 4692, 4)) : guest::LoadU32(base, address + 4692))); }
  void set_unknown_4692_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4692 = v; return; } if (memory::IsHandle(address + 4692)) memory::OwnerStore(address + 4692, 4, v); else guest::StoreU32(base, address + 4692, v); }
  uint32_t unknown_4692() const { if (auto* d = direct()) return d->unknown_4692; return uint32_t((memory::IsHandle(address + 4692) ? uint32_t(memory::OwnerLoad(address + 4692, 4)) : guest::LoadU32(base, address + 4692))); }
  void set_unknown_4692(uint32_t v) const { if (auto* d = direct()) { d->unknown_4692 = v; return; } if (memory::IsHandle(address + 4692)) memory::OwnerStore(address + 4692, 4, v); else guest::StoreU32(base, address + 4692, v); }
  // part model 0's +460 (TrafficCar::BindTextures)
  uint32_t unknown_4716_address() const { return address + 4716; }
  uint32_t unknown_4716_bits() const { if (auto* d = direct()) return d->unknown_4716; return uint32_t((memory::IsHandle(address + 4716) ? uint32_t(memory::OwnerLoad(address + 4716, 4)) : guest::LoadU32(base, address + 4716))); }
  void set_unknown_4716_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4716 = v; return; } if (memory::IsHandle(address + 4716)) memory::OwnerStore(address + 4716, 4, v); else guest::StoreU32(base, address + 4716, v); }
  uint32_t unknown_4716() const { if (auto* d = direct()) return d->unknown_4716; return uint32_t((memory::IsHandle(address + 4716) ? uint32_t(memory::OwnerLoad(address + 4716, 4)) : guest::LoadU32(base, address + 4716))); }
  void set_unknown_4716(uint32_t v) const { if (auto* d = direct()) { d->unknown_4716 = v; return; } if (memory::IsHandle(address + 4716)) memory::OwnerStore(address + 4716, 4, v); else guest::StoreU32(base, address + 4716, v); }
  // measure fault: the stand model's draw block +640, the stage's +32 (GarageStage::Draw)
  uint32_t model_bonnet_address() const { return address + 4928; }
  uint32_t model_bonnet_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_bonnet); return uint32_t((memory::IsHandle(address + 4928) ? uint32_t(memory::OwnerLoad(address + 4928, 4)) : guest::LoadU32(base, address + 4928))); }
  void set_model_bonnet_bits(uint32_t v) const { if (auto* d = direct()) { d->model_bonnet = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4928)) memory::OwnerStore(address + 4928, 4, v); else guest::StoreU32(base, address + 4928, v); }
  float model_bonnet() const { return std::bit_cast<float>(model_bonnet_bits()); }
  void set_model_bonnet(float v) const { set_model_bonnet_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the stand model's +4944 and +4960, the stage's +48 to +63 twice (GarageStage::Draw)
  uint32_t model_open0_xy_address() const { return address + 4944; }
  uint64_t model_open0_xy_bits() const { if (auto* d = direct()) return d->model_open0_xy; return uint64_t((memory::IsHandle(address + 4944) ? uint64_t(memory::OwnerLoad(address + 4944, 8)) : guest::LoadU64(base, address + 4944))); }
  void set_model_open0_xy_bits(uint64_t v) const { if (auto* d = direct()) { d->model_open0_xy = v; return; } if (memory::IsHandle(address + 4944)) memory::OwnerStore(address + 4944, 8, v); else guest::StoreU64(base, address + 4944, v); }
  uint64_t model_open0_xy() const { if (auto* d = direct()) return d->model_open0_xy; return uint64_t((memory::IsHandle(address + 4944) ? uint64_t(memory::OwnerLoad(address + 4944, 8)) : guest::LoadU64(base, address + 4944))); }
  void set_model_open0_xy(uint64_t v) const { if (auto* d = direct()) { d->model_open0_xy = v; return; } if (memory::IsHandle(address + 4944)) memory::OwnerStore(address + 4944, 8, v); else guest::StoreU64(base, address + 4944, v); }
  uint32_t model_open0_zw_address() const { return address + 4952; }
  uint64_t model_open0_zw_bits() const { if (auto* d = direct()) return d->model_open0_zw; return uint64_t((memory::IsHandle(address + 4952) ? uint64_t(memory::OwnerLoad(address + 4952, 8)) : guest::LoadU64(base, address + 4952))); }
  void set_model_open0_zw_bits(uint64_t v) const { if (auto* d = direct()) { d->model_open0_zw = v; return; } if (memory::IsHandle(address + 4952)) memory::OwnerStore(address + 4952, 8, v); else guest::StoreU64(base, address + 4952, v); }
  uint64_t model_open0_zw() const { if (auto* d = direct()) return d->model_open0_zw; return uint64_t((memory::IsHandle(address + 4952) ? uint64_t(memory::OwnerLoad(address + 4952, 8)) : guest::LoadU64(base, address + 4952))); }
  void set_model_open0_zw(uint64_t v) const { if (auto* d = direct()) { d->model_open0_zw = v; return; } if (memory::IsHandle(address + 4952)) memory::OwnerStore(address + 4952, 8, v); else guest::StoreU64(base, address + 4952, v); }
  uint32_t model_open1_xy_address() const { return address + 4960; }
  uint64_t model_open1_xy_bits() const { if (auto* d = direct()) return d->model_open1_xy; return uint64_t((memory::IsHandle(address + 4960) ? uint64_t(memory::OwnerLoad(address + 4960, 8)) : guest::LoadU64(base, address + 4960))); }
  void set_model_open1_xy_bits(uint64_t v) const { if (auto* d = direct()) { d->model_open1_xy = v; return; } if (memory::IsHandle(address + 4960)) memory::OwnerStore(address + 4960, 8, v); else guest::StoreU64(base, address + 4960, v); }
  uint64_t model_open1_xy() const { if (auto* d = direct()) return d->model_open1_xy; return uint64_t((memory::IsHandle(address + 4960) ? uint64_t(memory::OwnerLoad(address + 4960, 8)) : guest::LoadU64(base, address + 4960))); }
  void set_model_open1_xy(uint64_t v) const { if (auto* d = direct()) { d->model_open1_xy = v; return; } if (memory::IsHandle(address + 4960)) memory::OwnerStore(address + 4960, 8, v); else guest::StoreU64(base, address + 4960, v); }
  uint32_t model_open1_zw_address() const { return address + 4968; }
  uint64_t model_open1_zw_bits() const { if (auto* d = direct()) return d->model_open1_zw; return uint64_t((memory::IsHandle(address + 4968) ? uint64_t(memory::OwnerLoad(address + 4968, 8)) : guest::LoadU64(base, address + 4968))); }
  void set_model_open1_zw_bits(uint64_t v) const { if (auto* d = direct()) { d->model_open1_zw = v; return; } if (memory::IsHandle(address + 4968)) memory::OwnerStore(address + 4968, 8, v); else guest::StoreU64(base, address + 4968, v); }
  uint64_t model_open1_zw() const { if (auto* d = direct()) return d->model_open1_zw; return uint64_t((memory::IsHandle(address + 4968) ? uint64_t(memory::OwnerLoad(address + 4968, 8)) : guest::LoadU64(base, address + 4968))); }
  void set_model_open1_zw(uint64_t v) const { if (auto* d = direct()) { d->model_open1_zw = v; return; } if (memory::IsHandle(address + 4968)) memory::OwnerStore(address + 4968, 8, v); else guest::StoreU64(base, address + 4968, v); }
  // measure fault: the stand model's lights on (GarageStage::SetLightsOn)
  uint32_t model_lights_on_address() const { return address + 4976; }
  uint32_t model_lights_on_bits() const { if (auto* d = direct()) return d->model_lights_on; return uint32_t((memory::IsHandle(address + 4976) ? uint32_t(memory::OwnerLoad(address + 4976, 4)) : guest::LoadU32(base, address + 4976))); }
  void set_model_lights_on_bits(uint32_t v) const { if (auto* d = direct()) { d->model_lights_on = v; return; } if (memory::IsHandle(address + 4976)) memory::OwnerStore(address + 4976, 4, v); else guest::StoreU32(base, address + 4976, v); }
  uint32_t model_lights_on() const { if (auto* d = direct()) return d->model_lights_on; return uint32_t((memory::IsHandle(address + 4976) ? uint32_t(memory::OwnerLoad(address + 4976, 4)) : guest::LoadU32(base, address + 4976))); }
  void set_model_lights_on(uint32_t v) const { if (auto* d = direct()) { d->model_lights_on = v; return; } if (memory::IsHandle(address + 4976)) memory::OwnerStore(address + 4976, 4, v); else guest::StoreU32(base, address + 4976, v); }
  // measure fault: 0.0 when on, else SceneParts::LightValue
  uint32_t model_light_value_address() const { return address + 4980; }
  uint32_t model_light_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->model_light_value); return uint32_t((memory::IsHandle(address + 4980) ? uint32_t(memory::OwnerLoad(address + 4980, 4)) : guest::LoadU32(base, address + 4980))); }
  void set_model_light_value_bits(uint32_t v) const { if (auto* d = direct()) { d->model_light_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4980)) memory::OwnerStore(address + 4980, 4, v); else guest::StoreU32(base, address + 4980, v); }
  float model_light_value() const { return std::bit_cast<float>(model_light_value_bits()); }
  void set_model_light_value(float v) const { set_model_light_value_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: GarageStage::SetWord5016's word
  uint32_t model_word5016_address() const { return address + 5016; }
  uint32_t model_word5016_bits() const { if (auto* d = direct()) return d->model_word5016; return uint32_t((memory::IsHandle(address + 5016) ? uint32_t(memory::OwnerLoad(address + 5016, 4)) : guest::LoadU32(base, address + 5016))); }
  void set_model_word5016_bits(uint32_t v) const { if (auto* d = direct()) { d->model_word5016 = v; return; } if (memory::IsHandle(address + 5016)) memory::OwnerStore(address + 5016, 4, v); else guest::StoreU32(base, address + 5016, v); }
  uint32_t model_word5016() const { if (auto* d = direct()) return d->model_word5016; return uint32_t((memory::IsHandle(address + 5016) ? uint32_t(memory::OwnerLoad(address + 5016, 4)) : guest::LoadU32(base, address + 5016))); }
  void set_model_word5016(uint32_t v) const { if (auto* d = direct()) { d->model_word5016 = v; return; } if (memory::IsHandle(address + 5016)) memory::OwnerStore(address + 5016, 4, v); else guest::StoreU32(base, address + 5016, v); }
  // measure fault: the stand model's SceneModel::AddSecondWheels ran
  uint32_t model_second_wheels_claimed_address() const { return address + 5032; }
  uint32_t model_second_wheels_claimed_bits() const { if (auto* d = direct()) return d->model_second_wheels_claimed; return uint32_t((memory::IsHandle(address + 5032) ? uint32_t(memory::OwnerLoad(address + 5032, 4)) : guest::LoadU32(base, address + 5032))); }
  void set_model_second_wheels_claimed_bits(uint32_t v) const { if (auto* d = direct()) { d->model_second_wheels_claimed = v; return; } if (memory::IsHandle(address + 5032)) memory::OwnerStore(address + 5032, 4, v); else guest::StoreU32(base, address + 5032, v); }
  uint32_t model_second_wheels_claimed() const { if (auto* d = direct()) return d->model_second_wheels_claimed; return uint32_t((memory::IsHandle(address + 5032) ? uint32_t(memory::OwnerLoad(address + 5032, 4)) : guest::LoadU32(base, address + 5032))); }
  void set_model_second_wheels_claimed(uint32_t v) const { if (auto* d = direct()) { d->model_second_wheels_claimed = v; return; } if (memory::IsHandle(address + 5032)) memory::OwnerStore(address + 5032, 4, v); else guest::StoreU32(base, address + 5032, v); }
  // measure fault: the stand model's pool 1 object (SceneParts)
  uint32_t model_parts_address() const { return address + 5060; }
  uint32_t model_parts_bits() const { if (auto* d = direct()) return d->model_parts; return uint32_t((memory::IsHandle(address + 5060) ? uint32_t(memory::OwnerLoad(address + 5060, 4)) : guest::LoadU32(base, address + 5060))); }
  void set_model_parts_bits(uint32_t v) const { if (auto* d = direct()) { d->model_parts = v; return; } if (memory::IsHandle(address + 5060)) memory::OwnerStore(address + 5060, 4, v); else guest::StoreU32(base, address + 5060, v); }
  uint32_t model_parts() const { if (auto* d = direct()) return d->model_parts; return uint32_t((memory::IsHandle(address + 5060) ? uint32_t(memory::OwnerLoad(address + 5060, 4)) : guest::LoadU32(base, address + 5060))); }
  void set_model_parts(uint32_t v) const { if (auto* d = direct()) { d->model_parts = v; return; } if (memory::IsHandle(address + 5060)) memory::OwnerStore(address + 5060, 4, v); else guest::StoreU32(base, address + 5060, v); }
  // measure fault: the stand model's pool 5 object (SceneWheels)
  uint32_t model_wheels_address() const { return address + 5100; }
  uint32_t model_wheels_bits() const { if (auto* d = direct()) return d->model_wheels; return uint32_t((memory::IsHandle(address + 5100) ? uint32_t(memory::OwnerLoad(address + 5100, 4)) : guest::LoadU32(base, address + 5100))); }
  void set_model_wheels_bits(uint32_t v) const { if (auto* d = direct()) { d->model_wheels = v; return; } if (memory::IsHandle(address + 5100)) memory::OwnerStore(address + 5100, 4, v); else guest::StoreU32(base, address + 5100, v); }
  uint32_t model_wheels() const { if (auto* d = direct()) return d->model_wheels; return uint32_t((memory::IsHandle(address + 5100) ? uint32_t(memory::OwnerLoad(address + 5100, 4)) : guest::LoadU32(base, address + 5100))); }
  void set_model_wheels(uint32_t v) const { if (auto* d = direct()) { d->model_wheels = v; return; } if (memory::IsHandle(address + 5100)) memory::OwnerStore(address + 5100, 4, v); else guest::StoreU32(base, address + 5100, v); }
  // +7040 to +7052 a second texture's words (CarModel::MakePlate)
  uint32_t texture2_word0_address() const { return address + 7040; }
  uint32_t texture2_word0_bits() const { if (auto* d = direct()) return d->texture2_word0; return uint32_t((memory::IsHandle(address + 7040) ? uint32_t(memory::OwnerLoad(address + 7040, 4)) : guest::LoadU32(base, address + 7040))); }
  void set_texture2_word0_bits(uint32_t v) const { if (auto* d = direct()) { d->texture2_word0 = v; return; } if (memory::IsHandle(address + 7040)) memory::OwnerStore(address + 7040, 4, v); else guest::StoreU32(base, address + 7040, v); }
  uint32_t texture2_word0() const { if (auto* d = direct()) return d->texture2_word0; return uint32_t((memory::IsHandle(address + 7040) ? uint32_t(memory::OwnerLoad(address + 7040, 4)) : guest::LoadU32(base, address + 7040))); }
  void set_texture2_word0(uint32_t v) const { if (auto* d = direct()) { d->texture2_word0 = v; return; } if (memory::IsHandle(address + 7040)) memory::OwnerStore(address + 7040, 4, v); else guest::StoreU32(base, address + 7040, v); }
  uint32_t texture2_word1_address() const { return address + 7044; }
  uint32_t texture2_word1_bits() const { if (auto* d = direct()) return d->texture2_word1; return uint32_t((memory::IsHandle(address + 7044) ? uint32_t(memory::OwnerLoad(address + 7044, 4)) : guest::LoadU32(base, address + 7044))); }
  void set_texture2_word1_bits(uint32_t v) const { if (auto* d = direct()) { d->texture2_word1 = v; return; } if (memory::IsHandle(address + 7044)) memory::OwnerStore(address + 7044, 4, v); else guest::StoreU32(base, address + 7044, v); }
  uint32_t texture2_word1() const { if (auto* d = direct()) return d->texture2_word1; return uint32_t((memory::IsHandle(address + 7044) ? uint32_t(memory::OwnerLoad(address + 7044, 4)) : guest::LoadU32(base, address + 7044))); }
  void set_texture2_word1(uint32_t v) const { if (auto* d = direct()) { d->texture2_word1 = v; return; } if (memory::IsHandle(address + 7044)) memory::OwnerStore(address + 7044, 4, v); else guest::StoreU32(base, address + 7044, v); }
  uint32_t texture2_word2_address() const { return address + 7048; }
  uint32_t texture2_word2_bits() const { if (auto* d = direct()) return d->texture2_word2; return uint32_t((memory::IsHandle(address + 7048) ? uint32_t(memory::OwnerLoad(address + 7048, 4)) : guest::LoadU32(base, address + 7048))); }
  void set_texture2_word2_bits(uint32_t v) const { if (auto* d = direct()) { d->texture2_word2 = v; return; } if (memory::IsHandle(address + 7048)) memory::OwnerStore(address + 7048, 4, v); else guest::StoreU32(base, address + 7048, v); }
  uint32_t texture2_word2() const { if (auto* d = direct()) return d->texture2_word2; return uint32_t((memory::IsHandle(address + 7048) ? uint32_t(memory::OwnerLoad(address + 7048, 4)) : guest::LoadU32(base, address + 7048))); }
  void set_texture2_word2(uint32_t v) const { if (auto* d = direct()) { d->texture2_word2 = v; return; } if (memory::IsHandle(address + 7048)) memory::OwnerStore(address + 7048, 4, v); else guest::StoreU32(base, address + 7048, v); }
  uint32_t texture2_word3_address() const { return address + 7052; }
  uint32_t texture2_word3_bits() const { if (auto* d = direct()) return d->texture2_word3; return uint32_t((memory::IsHandle(address + 7052) ? uint32_t(memory::OwnerLoad(address + 7052, 4)) : guest::LoadU32(base, address + 7052))); }
  void set_texture2_word3_bits(uint32_t v) const { if (auto* d = direct()) { d->texture2_word3 = v; return; } if (memory::IsHandle(address + 7052)) memory::OwnerStore(address + 7052, 4, v); else guest::StoreU32(base, address + 7052, v); }
  uint32_t texture2_word3() const { if (auto* d = direct()) return d->texture2_word3; return uint32_t((memory::IsHandle(address + 7052) ? uint32_t(memory::OwnerLoad(address + 7052, 4)) : guest::LoadU32(base, address + 7052))); }
  void set_texture2_word3(uint32_t v) const { if (auto* d = direct()) { d->texture2_word3 = v; return; } if (memory::IsHandle(address + 7052)) memory::OwnerStore(address + 7052, 4, v); else guest::StoreU32(base, address + 7052, v); }
  // the XPR record named by the first formatted name (-1 none)
  uint32_t xpr_record_address() const { return address + 7056; }
  uint32_t xpr_record_bits() const { if (auto* d = direct()) return d->xpr_record; return uint32_t((memory::IsHandle(address + 7056) ? uint32_t(memory::OwnerLoad(address + 7056, 4)) : guest::LoadU32(base, address + 7056))); }
  void set_xpr_record_bits(uint32_t v) const { if (auto* d = direct()) { d->xpr_record = v; return; } if (memory::IsHandle(address + 7056)) memory::OwnerStore(address + 7056, 4, v); else guest::StoreU32(base, address + 7056, v); }
  uint32_t xpr_record() const { if (auto* d = direct()) return d->xpr_record; return uint32_t((memory::IsHandle(address + 7056) ? uint32_t(memory::OwnerLoad(address + 7056, 4)) : guest::LoadU32(base, address + 7056))); }
  void set_xpr_record(uint32_t v) const { if (auto* d = direct()) { d->xpr_record = v; return; } if (memory::IsHandle(address + 7056)) memory::OwnerStore(address + 7056, 4, v); else guest::StoreU32(base, address + 7056, v); }
  // another (TrafficCar::BindTextures)
  uint32_t xpr_record2_address() const { return address + 7060; }
  uint32_t xpr_record2_bits() const { if (auto* d = direct()) return d->xpr_record2; return uint32_t((memory::IsHandle(address + 7060) ? uint32_t(memory::OwnerLoad(address + 7060, 4)) : guest::LoadU32(base, address + 7060))); }
  void set_xpr_record2_bits(uint32_t v) const { if (auto* d = direct()) { d->xpr_record2 = v; return; } if (memory::IsHandle(address + 7060)) memory::OwnerStore(address + 7060, 4, v); else guest::StoreU32(base, address + 7060, v); }
  uint32_t xpr_record2() const { if (auto* d = direct()) return d->xpr_record2; return uint32_t((memory::IsHandle(address + 7060) ? uint32_t(memory::OwnerLoad(address + 7060, 4)) : guest::LoadU32(base, address + 7060))); }
  void set_xpr_record2(uint32_t v) const { if (auto* d = direct()) { d->xpr_record2 = v; return; } if (memory::IsHandle(address + 7060)) memory::OwnerStore(address + 7060, 4, v); else guest::StoreU32(base, address + 7060, v); }
  // +7072 + 16i the four tires' positions (their w 1.0 from the constructor)
  uint32_t tire_position0_w_address() const { return address + 7084; }
  uint32_t tire_position0_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->tire_position0_w); return uint32_t((memory::IsHandle(address + 7084) ? uint32_t(memory::OwnerLoad(address + 7084, 4)) : guest::LoadU32(base, address + 7084))); }
  void set_tire_position0_w_bits(uint32_t v) const { if (auto* d = direct()) { d->tire_position0_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 7084)) memory::OwnerStore(address + 7084, 4, v); else guest::StoreU32(base, address + 7084, v); }
  float tire_position0_w() const { return std::bit_cast<float>(tire_position0_w_bits()); }
  void set_tire_position0_w(float v) const { set_tire_position0_w_bits(std::bit_cast<uint32_t>(v)); }
  // +7136 + 64i the tires' matrices (each row's w 1.0 from the constructor)
  uint32_t tire_matrix0_row0_w_address() const { return address + 7148; }
  uint32_t tire_matrix0_row0_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->tire_matrix0_row0_w); return uint32_t((memory::IsHandle(address + 7148) ? uint32_t(memory::OwnerLoad(address + 7148, 4)) : guest::LoadU32(base, address + 7148))); }
  void set_tire_matrix0_row0_w_bits(uint32_t v) const { if (auto* d = direct()) { d->tire_matrix0_row0_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 7148)) memory::OwnerStore(address + 7148, 4, v); else guest::StoreU32(base, address + 7148, v); }
  float tire_matrix0_row0_w() const { return std::bit_cast<float>(tire_matrix0_row0_w_bits()); }
  void set_tire_matrix0_row0_w(float v) const { set_tire_matrix0_row0_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t tire_matrix0_row1_w_address() const { return address + 7164; }
  uint32_t tire_matrix0_row1_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->tire_matrix0_row1_w); return uint32_t((memory::IsHandle(address + 7164) ? uint32_t(memory::OwnerLoad(address + 7164, 4)) : guest::LoadU32(base, address + 7164))); }
  void set_tire_matrix0_row1_w_bits(uint32_t v) const { if (auto* d = direct()) { d->tire_matrix0_row1_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 7164)) memory::OwnerStore(address + 7164, 4, v); else guest::StoreU32(base, address + 7164, v); }
  float tire_matrix0_row1_w() const { return std::bit_cast<float>(tire_matrix0_row1_w_bits()); }
  void set_tire_matrix0_row1_w(float v) const { set_tire_matrix0_row1_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t tire_matrix0_row2_w_address() const { return address + 7180; }
  uint32_t tire_matrix0_row2_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->tire_matrix0_row2_w); return uint32_t((memory::IsHandle(address + 7180) ? uint32_t(memory::OwnerLoad(address + 7180, 4)) : guest::LoadU32(base, address + 7180))); }
  void set_tire_matrix0_row2_w_bits(uint32_t v) const { if (auto* d = direct()) { d->tire_matrix0_row2_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 7180)) memory::OwnerStore(address + 7180, 4, v); else guest::StoreU32(base, address + 7180, v); }
  float tire_matrix0_row2_w() const { return std::bit_cast<float>(tire_matrix0_row2_w_bits()); }
  void set_tire_matrix0_row2_w(float v) const { set_tire_matrix0_row2_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t tire_matrix0_row3_w_address() const { return address + 7196; }
  uint32_t tire_matrix0_row3_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->tire_matrix0_row3_w); return uint32_t((memory::IsHandle(address + 7196) ? uint32_t(memory::OwnerLoad(address + 7196, 4)) : guest::LoadU32(base, address + 7196))); }
  void set_tire_matrix0_row3_w_bits(uint32_t v) const { if (auto* d = direct()) { d->tire_matrix0_row3_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 7196)) memory::OwnerStore(address + 7196, 4, v); else guest::StoreU32(base, address + 7196, v); }
  float tire_matrix0_row3_w() const { return std::bit_cast<float>(tire_matrix0_row3_w_bits()); }
  void set_tire_matrix0_row3_w(float v) const { set_tire_matrix0_row3_w_bits(std::bit_cast<uint32_t>(v)); }
  // the first set ([0x826C2E40])
  uint32_t first_set_address() const { return address + 7392; }
  uint32_t first_set_bits() const { if (auto* d = direct()) return d->first_set; return uint32_t((memory::IsHandle(address + 7392) ? uint32_t(memory::OwnerLoad(address + 7392, 4)) : guest::LoadU32(base, address + 7392))); }
  void set_first_set_bits(uint32_t v) const { if (auto* d = direct()) { d->first_set = v; return; } if (memory::IsHandle(address + 7392)) memory::OwnerStore(address + 7392, 4, v); else guest::StoreU32(base, address + 7392, v); }
  template <typename T = ScenePools>
  T first_set() const { return T(base, first_set_bits()); }
  void set_first_set(const View& w) const { set_first_set_bits(w.address); }
  void set_first_set(uint32_t v) const { set_first_set_bits(v); }
  // measure fault: sub_820E3658's store (another pool object)
  uint32_t unknown_11452_address() const { return address + 11452; }
  uint32_t unknown_11452_bits() const { if (auto* d = direct()) return d->unknown_11452; return uint32_t((memory::IsHandle(address + 11452) ? uint32_t(memory::OwnerLoad(address + 11452, 4)) : guest::LoadU32(base, address + 11452))); }
  void set_unknown_11452_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_11452 = v; return; } if (memory::IsHandle(address + 11452)) memory::OwnerStore(address + 11452, 4, v); else guest::StoreU32(base, address + 11452, v); }
  uint32_t unknown_11452() const { if (auto* d = direct()) return d->unknown_11452; return uint32_t((memory::IsHandle(address + 11452) ? uint32_t(memory::OwnerLoad(address + 11452, 4)) : guest::LoadU32(base, address + 11452))); }
  void set_unknown_11452(uint32_t v) const { if (auto* d = direct()) { d->unknown_11452 = v; return; } if (memory::IsHandle(address + 11452)) memory::OwnerStore(address + 11452, 4, v); else guest::StoreU32(base, address + 11452, v); }
  // measure fault: SceneCockpit::SetUp's store
  uint32_t unknown_11656_address() const { return address + 11656; }
  uint32_t unknown_11656_bits() const { if (auto* d = direct()) return d->unknown_11656; return uint32_t((memory::IsHandle(address + 11656) ? uint32_t(memory::OwnerLoad(address + 11656, 4)) : guest::LoadU32(base, address + 11656))); }
  void set_unknown_11656_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_11656 = v; return; } if (memory::IsHandle(address + 11656)) memory::OwnerStore(address + 11656, 4, v); else guest::StoreU32(base, address + 11656, v); }
  uint32_t unknown_11656() const { if (auto* d = direct()) return d->unknown_11656; return uint32_t((memory::IsHandle(address + 11656) ? uint32_t(memory::OwnerLoad(address + 11656, 4)) : guest::LoadU32(base, address + 11656))); }
  void set_unknown_11656(uint32_t v) const { if (auto* d = direct()) { d->unknown_11656 = v; return; } if (memory::IsHandle(address + 11656)) memory::OwnerStore(address + 11656, 4, v); else guest::StoreU32(base, address + 11656, v); }
  // measure fault: SceneCockpit::Finish's store
  uint32_t unknown_11664_address() const { return address + 11664; }
  uint32_t unknown_11664_bits() const { if (auto* d = direct()) return d->unknown_11664; return uint32_t((memory::IsHandle(address + 11664) ? uint32_t(memory::OwnerLoad(address + 11664, 4)) : guest::LoadU32(base, address + 11664))); }
  void set_unknown_11664_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_11664 = v; return; } if (memory::IsHandle(address + 11664)) memory::OwnerStore(address + 11664, 4, v); else guest::StoreU32(base, address + 11664, v); }
  uint32_t unknown_11664() const { if (auto* d = direct()) return d->unknown_11664; return uint32_t((memory::IsHandle(address + 11664) ? uint32_t(memory::OwnerLoad(address + 11664, 4)) : guest::LoadU32(base, address + 11664))); }
  void set_unknown_11664(uint32_t v) const { if (auto* d = direct()) { d->unknown_11664 = v; return; } if (memory::IsHandle(address + 11664)) memory::OwnerStore(address + 11664, 4, v); else guest::StoreU32(base, address + 11664, v); }
  // measure fault: sub_820E7378's store (a SceneWheels' +16776)
  uint32_t unknown_16776_address() const { return address + 16776; }
  uint32_t unknown_16776_bits() const { if (auto* d = direct()) return d->unknown_16776; return uint32_t((memory::IsHandle(address + 16776) ? uint32_t(memory::OwnerLoad(address + 16776, 4)) : guest::LoadU32(base, address + 16776))); }
  void set_unknown_16776_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_16776 = v; return; } if (memory::IsHandle(address + 16776)) memory::OwnerStore(address + 16776, 4, v); else guest::StoreU32(base, address + 16776, v); }
  uint32_t unknown_16776() const { if (auto* d = direct()) return d->unknown_16776; return uint32_t((memory::IsHandle(address + 16776) ? uint32_t(memory::OwnerLoad(address + 16776, 4)) : guest::LoadU32(base, address + 16776))); }
  void set_unknown_16776(uint32_t v) const { if (auto* d = direct()) { d->unknown_16776 = v; return; } if (memory::IsHandle(address + 16776)) memory::OwnerStore(address + 16776, 4, v); else guest::StoreU32(base, address + 16776, v); }
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

// embedded; measure: GarageStage.+0@+32
struct GarageStageAt32 : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  float unknown_48() const { return guest::LoadFloat(base, address + 48); }
  void set_unknown_48(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  float unknown_52() const { return guest::LoadFloat(base, address + 52); }
  void set_unknown_52(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  float unknown_56() const { return guest::LoadFloat(base, address + 56); }
  void set_unknown_56(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: TrafficCar@+160
struct TrafficCarAt160 : View {
  using View::View;
  static constexpr uint32_t kSize = 11456;
  uint32_t unknown_11452() const { return guest::LoadU32(base, address + 11452); }
  void set_unknown_11452(uint32_t v) const { guest::StoreU32(base, address + 11452, v); }
  uint32_t unknown_11452_address() const { return address + 11452; }
};

// embedded; measure: GarageStage@+32
struct GarageStageAt32_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint64_t unknown_16() const { return guest::LoadU64(base, address + 16); }
  void set_unknown_16(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint64_t unknown_24() const { return guest::LoadU64(base, address + 24); }
  void set_unknown_24(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint64_t unknown_32() const { return guest::LoadU64(base, address + 32); }
  void set_unknown_32(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint64_t unknown_40() const { return guest::LoadU64(base, address + 40); }
  void set_unknown_40(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint64_t unknown_48() const { return guest::LoadU64(base, address + 48); }
  void set_unknown_48(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint64_t unknown_56() const { return guest::LoadU64(base, address + 56); }
  void set_unknown_56(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_64() const { return guest::LoadU32(base, address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
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
