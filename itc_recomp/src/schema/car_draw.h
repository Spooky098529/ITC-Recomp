// The views of docs/functions/car-draw.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-draw.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: CarScene, Car.+5116, CarScene::OnFileLoaded.r6, VirtualFree.ret, SceneParts@+176.+37088
struct CarScene : View {
  using View::View;
  static constexpr uint32_t kSize = 5536;
  // 0 made, 3 set up (CarScene::SetUp), 4 tearing down
  uint32_t state() const { return guest::LoadU32(address + 0); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t state_address() const { return address + 0; }
  // the set-up record's +0: 0 the front end (set a), 1 the race and parking area (set b)
  uint32_t model_set() const { return guest::LoadU32(address + 4); }
  void set_model_set(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t model_set_address() const { return address + 4; }
  // pool 0's count (the set-up record's +4)
  uint32_t car_model_count() const { return guest::LoadU32(address + 8); }
  void set_car_model_count(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t car_model_count_address() const { return address + 8; }
  // pool 1's count
  uint32_t parts_count() const { return guest::LoadU32(address + 12); }
  void set_parts_count(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t parts_count_address() const { return address + 12; }
  // pool 3's count (record +12 + +16 + +32)
  uint32_t interior_count() const { return guest::LoadU32(address + 16); }
  void set_interior_count(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t interior_count_address() const { return address + 16; }
  // pool 2's count
  uint32_t cockpit_count() const { return guest::LoadU32(address + 20); }
  void set_cockpit_count(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t cockpit_count_address() const { return address + 20; }
  // pool 4's count
  uint32_t light_kit_count() const { return guest::LoadU32(address + 24); }
  void set_light_kit_count(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t light_kit_count_address() const { return address + 24; }
  // pool 5's count (record +24 + +32 + +28)
  uint32_t wheels_count() const { return guest::LoadU32(address + 28); }
  void set_wheels_count(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t wheels_count_address() const { return address + 28; }
  // pool 7's count
  uint32_t scene_model_count() const { return guest::LoadU32(address + 32); }
  void set_scene_model_count(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t scene_model_count_address() const { return address + 32; }
  // pool 6's count
  uint32_t traffic_count() const { return guest::LoadU32(address + 36); }
  void set_traffic_count(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t traffic_count_address() const { return address + 36; }
  // the scene's own load buffer, 0 for the archive's shared one
  uint32_t own_load_buffer() const { return guest::LoadU32(address + 40); }
  void set_own_load_buffer(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t own_load_buffer_address() const { return address + 40; }
  uint32_t own_load_buffer_size() const { return guest::LoadU32(address + 44); }
  void set_own_load_buffer_size(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t own_load_buffer_size_address() const { return address + 44; }
  // 2 updates
  uint32_t teardown_count() const { return guest::LoadU32(address + 48); }
  void set_teardown_count(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t teardown_count_address() const { return address + 48; }
  // the shared assets' slot +0 (asset 141): its +20, 2 loading, 3 done
  uint32_t asset141_state() const { return guest::LoadU32(address + 84); }
  void set_asset141_state(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t asset141_state_address() const { return address + 84; }
  // the shared assets' slot +48 (asset 586)
  uint32_t asset586_state() const { return guest::LoadU32(address + 132); }
  void set_asset586_state(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t asset586_state_address() const { return address + 132; }
  // the shared assets' slot +72
  uint32_t asset_slot72_state() const { return guest::LoadU32(address + 156); }
  void set_asset_slot72_state(uint32_t v) const { guest::StoreU32(address + 156, v); }
  uint32_t asset_slot72_state_address() const { return address + 156; }
  // the shared assets' slot +184 (asset 888)
  uint32_t asset888_state() const { return guest::LoadU32(address + 268); }
  void set_asset888_state(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t asset888_state_address() const { return address + 268; }
  // the texture provider of the slot at the assets' +320 (asset 145)
  uint32_t texture_provider_145() const { return guest::LoadU32(address + 432); }
  void set_texture_provider_145(uint32_t v) const { guest::StoreU32(address + 432, v); }
  uint32_t texture_provider_145_address() const { return address + 432; }
  // the slot at the assets' +388 (asset 142, the glow model's part file)
  uint32_t glow_part_file_state() const { return guest::LoadU32(address + 472); }
  void set_glow_part_file_state(uint32_t v) const { guest::StoreU32(address + 472, v); }
  uint32_t glow_part_file_state_address() const { return address + 472; }
  // the shared glow model (a part model, the assets' +416)
  uint32_t glow_model_address() const { return address + 480; }
  // the glow model's +412: its shape count
  uint32_t glow_model_shapes() const { return guest::LoadU32(address + 892); }
  void set_glow_model_shapes(uint32_t v) const { guest::StoreU32(address + 892, v); }
  uint32_t glow_model_shapes_address() const { return address + 892; }
  // the slot at the assets' +880 (asset 143, the glow model's textures)
  uint32_t glow_textures_state() const { return guest::LoadU32(address + 964); }
  void set_glow_textures_state(uint32_t v) const { guest::StoreU32(address + 964, v); }
  uint32_t glow_textures_state_address() const { return address + 964; }
  // the TGA image's header (assets +928)
  uint32_t tga_header_address() const { return address + 992; }
  // the TGA image's pixels (assets +948, CarScene::GetTga)
  uint32_t tga_pixels() const { return guest::LoadU32(address + 1012); }
  void set_tga_pixels(uint32_t v) const { guest::StoreU32(address + 1012, v); }
  uint32_t tga_pixels_address() const { return address + 1012; }
  // the slot at the assets' +4072 (asset 109, set a only)
  uint32_t asset109_state() const { return guest::LoadU32(address + 4156); }
  void set_asset109_state(uint32_t v) const { guest::StoreU32(address + 4156, v); }
  uint32_t asset109_state_address() const { return address + 4156; }
  // asset 105's XPR (assets +4584): the number plates' blank texture
  uint32_t plate_xpr_address() const { return address + 4648; }
  // the fourth bundle's slot (assets +4680)
  uint32_t bundle3_state() const { return guest::LoadU32(address + 4764); }
  void set_bundle3_state(uint32_t v) const { guest::StoreU32(address + 4764, v); }
  uint32_t bundle3_state_address() const { return address + 4764; }
  // the second set's lists, 12 bytes a pool: +0 the first node, +8 the count
  uint32_t car_models_first() const { return guest::LoadU32(address + 4944); }
  void set_car_models_first(uint32_t v) const { guest::StoreU32(address + 4944, v); }
  uint32_t car_models_first_address() const { return address + 4944; }
  uint32_t car_models_count() const { return guest::LoadU32(address + 4952); }
  void set_car_models_count(uint32_t v) const { guest::StoreU32(address + 4952, v); }
  uint32_t car_models_count_address() const { return address + 4952; }
  uint32_t parts_first() const { return guest::LoadU32(address + 4956); }
  void set_parts_first(uint32_t v) const { guest::StoreU32(address + 4956, v); }
  uint32_t parts_first_address() const { return address + 4956; }
  uint32_t parts_list_count() const { return guest::LoadU32(address + 4964); }
  void set_parts_list_count(uint32_t v) const { guest::StoreU32(address + 4964, v); }
  uint32_t parts_list_count_address() const { return address + 4964; }
  uint32_t cockpits_first() const { return guest::LoadU32(address + 4968); }
  void set_cockpits_first(uint32_t v) const { guest::StoreU32(address + 4968, v); }
  uint32_t cockpits_first_address() const { return address + 4968; }
  uint32_t cockpits_count() const { return guest::LoadU32(address + 4976); }
  void set_cockpits_count(uint32_t v) const { guest::StoreU32(address + 4976, v); }
  uint32_t cockpits_count_address() const { return address + 4976; }
  uint32_t interiors_first() const { return guest::LoadU32(address + 4980); }
  void set_interiors_first(uint32_t v) const { guest::StoreU32(address + 4980, v); }
  uint32_t interiors_first_address() const { return address + 4980; }
  uint32_t interiors_count() const { return guest::LoadU32(address + 4988); }
  void set_interiors_count(uint32_t v) const { guest::StoreU32(address + 4988, v); }
  uint32_t interiors_count_address() const { return address + 4988; }
  uint32_t light_kits_first() const { return guest::LoadU32(address + 4992); }
  void set_light_kits_first(uint32_t v) const { guest::StoreU32(address + 4992, v); }
  uint32_t light_kits_first_address() const { return address + 4992; }
  uint32_t light_kits_count() const { return guest::LoadU32(address + 5000); }
  void set_light_kits_count(uint32_t v) const { guest::StoreU32(address + 5000, v); }
  uint32_t light_kits_count_address() const { return address + 5000; }
  uint32_t wheels_first() const { return guest::LoadU32(address + 5004); }
  void set_wheels_first(uint32_t v) const { guest::StoreU32(address + 5004, v); }
  uint32_t wheels_first_address() const { return address + 5004; }
  uint32_t wheels_list_count() const { return guest::LoadU32(address + 5012); }
  void set_wheels_list_count(uint32_t v) const { guest::StoreU32(address + 5012, v); }
  uint32_t wheels_list_count_address() const { return address + 5012; }
  uint32_t traffic_first() const { return guest::LoadU32(address + 5016); }
  void set_traffic_first(uint32_t v) const { guest::StoreU32(address + 5016, v); }
  uint32_t traffic_first_address() const { return address + 5016; }
  uint32_t traffic_list_count() const { return guest::LoadU32(address + 5024); }
  void set_traffic_list_count(uint32_t v) const { guest::StoreU32(address + 5024, v); }
  uint32_t traffic_list_count_address() const { return address + 5024; }
  uint32_t scene_models_first() const { return guest::LoadU32(address + 5028); }
  void set_scene_models_first(uint32_t v) const { guest::StoreU32(address + 5028, v); }
  uint32_t scene_models_first_address() const { return address + 5028; }
  uint32_t scene_models_count() const { return guest::LoadU32(address + 5036); }
  void set_scene_models_count(uint32_t v) const { guest::StoreU32(address + 5036, v); }
  uint32_t scene_models_count_address() const { return address + 5036; }
  // the first set ([0x826C2E40], ScenePools::MakeA's)
  uint32_t first_set() const { return guest::LoadU32(address + 5040); }
  void set_first_set(uint32_t v) const { guest::StoreU32(address + 5040, v); }
  uint32_t first_set_address() const { return address + 5040; }
  // CarScene::SetUp's r5; set: asset 141 loaded in set b too, [0x8296B8F0] from its XPR in the race path
  uint32_t set_up_r5() const { return guest::LoadU32(address + 5044); }
  void set_set_up_r5(uint32_t v) const { guest::StoreU32(address + 5044, v); }
  uint32_t set_up_r5_address() const { return address + 5044; }
  // 0: the light kits' blink timers advance (Race::ToggleEffects)
  uint32_t blink_timers_held() const { return guest::LoadU32(address + 5048); }
  void set_blink_timers_held(uint32_t v) const { guest::StoreU32(address + 5048, v); }
  uint32_t blink_timers_held_address() const { return address + 5048; }
  // 0: CarLighting::Gather writes the light slots; 1 in the parking area (LocationScreen::Update)
  uint32_t lighting_fixed() const { return guest::LoadU32(address + 5052); }
  void set_lighting_fixed(uint32_t v) const { guest::StoreU32(address + 5052, v); }
  uint32_t lighting_fixed_address() const { return address + 5052; }
  // the load buffer the scene uses (its own or the archive's shared one)
  uint32_t load_buffer() const { return guest::LoadU32(address + 5056); }
  void set_load_buffer(uint32_t v) const { guest::StoreU32(address + 5056, v); }
  uint32_t load_buffer_address() const { return address + 5056; }
  uint32_t load_buffer_size() const { return guest::LoadU32(address + 5060); }
  void set_load_buffer_size(uint32_t v) const { guest::StoreU32(address + 5060, v); }
  uint32_t load_buffer_size_address() const { return address + 5060; }
  // the pass description (+5072 to +5199, set by the scene's draw before each Car::DrawUpdate): +0 the camera pass index (0x826BAC00 + 592 x it)
  uint32_t pass_camera() const { return guest::LoadU32(address + 5072); }
  void set_pass_camera(uint32_t v) const { guest::StoreU32(address + 5072, v); }
  uint32_t pass_camera_address() const { return address + 5072; }
  // +4: the plain shadows' and the tires' depth camera pass
  uint32_t pass_shadow_camera_a() const { return guest::LoadU32(address + 5076); }
  void set_pass_shadow_camera_a(uint32_t v) const { guest::StoreU32(address + 5076, v); }
  uint32_t pass_shadow_camera_a_address() const { return address + 5076; }
  // +8: the second shadow set's camera pass
  uint32_t pass_shadow_camera_b() const { return guest::LoadU32(address + 5080); }
  void set_pass_shadow_camera_b(uint32_t v) const { guest::StoreU32(address + 5080, v); }
  uint32_t pass_shadow_camera_b_address() const { return address + 5080; }
  // +12: the light kit's camera pass
  uint32_t pass_light_kit_camera() const { return guest::LoadU32(address + 5084); }
  void set_pass_light_kit_camera(uint32_t v) const { guest::StoreU32(address + 5084, v); }
  uint32_t pass_light_kit_camera_address() const { return address + 5084; }
  // +16: the parts' second depth draw's camera pass (probably)
  uint32_t pass_parts_shadow_camera() const { return guest::LoadU32(address + 5088); }
  void set_pass_parts_shadow_camera(uint32_t v) const { guest::StoreU32(address + 5088, v); }
  uint32_t pass_parts_shadow_camera_address() const { return address + 5088; }
  // +20: copied to 0x8296B90C (Model::Draw's group 0 bucket)
  uint32_t pass_key_group0() const { return guest::LoadU32(address + 5092); }
  void set_pass_key_group0(uint32_t v) const { guest::StoreU32(address + 5092, v); }
  uint32_t pass_key_group0_address() const { return address + 5092; }
  // +24: copied to 0x8296B900
  uint32_t pass_key_group1() const { return guest::LoadU32(address + 5096); }
  void set_pass_key_group1(uint32_t v) const { guest::StoreU32(address + 5096, v); }
  uint32_t pass_key_group1_address() const { return address + 5096; }
  // +28: copied to 0x8296B908
  uint32_t pass_key_depth() const { return guest::LoadU32(address + 5100); }
  void set_pass_key_depth(uint32_t v) const { guest::StoreU32(address + 5100, v); }
  uint32_t pass_key_depth_address() const { return address + 5100; }
  // +32: the plain shadows' (and the tires' depth) key
  uint32_t pass_shadow_key_a() const { return guest::LoadU32(address + 5104); }
  void set_pass_shadow_key_a(uint32_t v) const { guest::StoreU32(address + 5104, v); }
  uint32_t pass_shadow_key_a_address() const { return address + 5104; }
  // +36: the second shadow set's key
  uint32_t pass_shadow_key_b() const { return guest::LoadU32(address + 5108); }
  void set_pass_shadow_key_b(uint32_t v) const { guest::StoreU32(address + 5108, v); }
  uint32_t pass_shadow_key_b_address() const { return address + 5108; }
  // +40 + 4i: the planar shadows' keys, lights 0 to 2
  uint32_t pass_light0_shadow_key() const { return guest::LoadU32(address + 5112); }
  void set_pass_light0_shadow_key(uint32_t v) const { guest::StoreU32(address + 5112, v); }
  uint32_t pass_light0_shadow_key_address() const { return address + 5112; }
  uint32_t pass_light1_shadow_key() const { return guest::LoadU32(address + 5116); }
  void set_pass_light1_shadow_key(uint32_t v) const { guest::StoreU32(address + 5116, v); }
  uint32_t pass_light1_shadow_key_address() const { return address + 5116; }
  uint32_t pass_light2_shadow_key() const { return guest::LoadU32(address + 5120); }
  void set_pass_light2_shadow_key(uint32_t v) const { guest::StoreU32(address + 5120, v); }
  uint32_t pass_light2_shadow_key_address() const { return address + 5120; }
  // +52
  uint32_t pass_light_kit_key() const { return guest::LoadU32(address + 5124); }
  void set_pass_light_kit_key(uint32_t v) const { guest::StoreU32(address + 5124, v); }
  uint32_t pass_light_kit_key_address() const { return address + 5124; }
  // +56
  uint32_t pass_parts_shadow_key() const { return guest::LoadU32(address + 5128); }
  void set_pass_parts_shadow_key(uint32_t v) const { guest::StoreU32(address + 5128, v); }
  uint32_t pass_parts_shadow_key_address() const { return address + 5128; }
  // +60: 0x2 the interior's body r6, 0x4 and 0x10 the plain shadow sets and the tires' depth pass, 0x8 the light kit, 0x20 << i the planar shadows from light i, 0x100 the parts' second depth draw; 0xF8 cleared for a car far in height from the player's
  uint32_t pass_mask() const { return guest::LoadU32(address + 5132); }
  void set_pass_mask(uint32_t v) const { guest::StoreU32(address + 5132, v); }
  uint32_t pass_mask_address() const { return address + 5132; }
  // +64: [0x8296B90C] while a car draws
  uint32_t pass_saved_key_group0() const { return guest::LoadU32(address + 5136); }
  void set_pass_saved_key_group0(uint32_t v) const { guest::StoreU32(address + 5136, v); }
  uint32_t pass_saved_key_group0_address() const { return address + 5136; }
  // +68: [0x8296B900]
  uint32_t pass_saved_key_group1() const { return guest::LoadU32(address + 5140); }
  void set_pass_saved_key_group1(uint32_t v) const { guest::StoreU32(address + 5140, v); }
  uint32_t pass_saved_key_group1_address() const { return address + 5140; }
  // +72: [0x8296B908]
  uint32_t pass_saved_key_depth() const { return guest::LoadU32(address + 5144); }
  void set_pass_saved_key_depth(uint32_t v) const { guest::StoreU32(address + 5144, v); }
  uint32_t pass_saved_key_depth_address() const { return address + 5144; }
  // +76: [0x8296B8FC] around a depth draw
  uint32_t pass_saved_depth_key() const { return guest::LoadU32(address + 5148); }
  void set_pass_saved_depth_key(uint32_t v) const { guest::StoreU32(address + 5148, v); }
  uint32_t pass_saved_depth_key_address() const { return address + 5148; }
  // +80: [0x8296B8F8] around a depth draw
  uint32_t pass_saved_other_key() const { return guest::LoadU32(address + 5152); }
  void set_pass_saved_other_key(uint32_t v) const { guest::StoreU32(address + 5152, v); }
  uint32_t pass_saved_other_key_address() const { return address + 5152; }
  // +96: the planar shadow's light direction, normalized (probably)
  uint32_t pass_light_direction_address() const { return address + 5168; }
  // 1.0 when made
  float pass_light_direction_w() const { return guest::LoadFloat(address + 5180); }
  void set_pass_light_direction_w(float v) const { guest::StoreFloat(address + 5180, v); }
  uint32_t pass_light_direction_w_address() const { return address + 5180; }
  // +112: the planar shadow's projection (probably)
  uint32_t pass_light_matrix_address() const { return address + 5184; }
  // its rows' w's, 1.0 when made
  float pass_light_matrix_w0() const { return guest::LoadFloat(address + 5196); }
  void set_pass_light_matrix_w0(float v) const { guest::StoreFloat(address + 5196, v); }
  uint32_t pass_light_matrix_w0_address() const { return address + 5196; }
  float pass_light_matrix_w1() const { return guest::LoadFloat(address + 5212); }
  void set_pass_light_matrix_w1(float v) const { guest::StoreFloat(address + 5212, v); }
  uint32_t pass_light_matrix_w1_address() const { return address + 5212; }
  float pass_light_matrix_w2() const { return guest::LoadFloat(address + 5228); }
  void set_pass_light_matrix_w2(float v) const { guest::StoreFloat(address + 5228, v); }
  uint32_t pass_light_matrix_w2_address() const { return address + 5228; }
  float pass_light_matrix_w3() const { return guest::LoadFloat(address + 5244); }
  void set_pass_light_matrix_w3(float v) const { guest::StoreFloat(address + 5244, v); }
  uint32_t pass_light_matrix_w3_address() const { return address + 5244; }
  // the lighting block (+5248 to +5520, CarScene::InitLighting): the other cars' lights' first color factor
  uint32_t car_light_factor_a_address() const { return address + 5248; }
  float car_light_factor_a_w() const { return guest::LoadFloat(address + 5260); }
  void set_car_light_factor_a_w(float v) const { guest::StoreFloat(address + 5260, v); }
  uint32_t car_light_factor_a_w_address() const { return address + 5260; }
  // their second color factor
  uint32_t car_light_factor_b_address() const { return address + 5264; }
  // a vector copied to 0x8265ED00 by the scene's draw (two doublewords)
  uint64_t light_vector_lo() const { return guest::LoadU64(address + 5280); }
  void set_light_vector_lo(uint64_t v) const { guest::StoreU64(address + 5280, v); }
  uint32_t light_vector_lo_address() const { return address + 5280; }
  uint64_t light_vector_hi() const { return guest::LoadU64(address + 5288); }
  void set_light_vector_hi(uint64_t v) const { guest::StoreU64(address + 5288, v); }
  uint32_t light_vector_hi_address() const { return address + 5288; }
  // read by Scenery::RecordHeadlightProjectors
  float projector_a_x() const { return guest::LoadFloat(address + 5296); }
  void set_projector_a_x(float v) const { guest::StoreFloat(address + 5296, v); }
  uint32_t projector_a_x_address() const { return address + 5296; }
  float projector_a_y() const { return guest::LoadFloat(address + 5300); }
  void set_projector_a_y(float v) const { guest::StoreFloat(address + 5300, v); }
  uint32_t projector_a_y_address() const { return address + 5300; }
  float projector_a_z() const { return guest::LoadFloat(address + 5304); }
  void set_projector_a_z(float v) const { guest::StoreFloat(address + 5304, v); }
  uint32_t projector_a_z_address() const { return address + 5304; }
  float projector_b_x() const { return guest::LoadFloat(address + 5312); }
  void set_projector_b_x(float v) const { guest::StoreFloat(address + 5312, v); }
  uint32_t projector_b_x_address() const { return address + 5312; }
  float projector_b_y() const { return guest::LoadFloat(address + 5316); }
  void set_projector_b_y(float v) const { guest::StoreFloat(address + 5316, v); }
  uint32_t projector_b_y_address() const { return address + 5316; }
  float projector_b_z() const { return guest::LoadFloat(address + 5320); }
  void set_projector_b_z(float v) const { guest::StoreFloat(address + 5320, v); }
  uint32_t projector_b_z_address() const { return address + 5320; }
  uint64_t projector_c_lo() const { return guest::LoadU64(address + 5328); }
  void set_projector_c_lo(uint64_t v) const { guest::StoreU64(address + 5328, v); }
  uint32_t projector_c_lo_address() const { return address + 5328; }
  uint64_t projector_c_hi() const { return guest::LoadU64(address + 5336); }
  void set_projector_c_hi(uint64_t v) const { guest::StoreU64(address + 5336, v); }
  uint32_t projector_c_hi_address() const { return address + 5336; }
  // CarLighting::Gather's lighting vectors
  uint64_t gather_vector_a_lo() const { return guest::LoadU64(address + 5344); }
  void set_gather_vector_a_lo(uint64_t v) const { guest::StoreU64(address + 5344, v); }
  uint32_t gather_vector_a_lo_address() const { return address + 5344; }
  uint64_t gather_vector_a_hi() const { return guest::LoadU64(address + 5352); }
  void set_gather_vector_a_hi(uint64_t v) const { guest::StoreU64(address + 5352, v); }
  uint32_t gather_vector_a_hi_address() const { return address + 5352; }
  uint64_t gather_vector_b_lo() const { return guest::LoadU64(address + 5360); }
  void set_gather_vector_b_lo(uint64_t v) const { guest::StoreU64(address + 5360, v); }
  uint32_t gather_vector_b_lo_address() const { return address + 5360; }
  uint64_t gather_vector_b_hi() const { return guest::LoadU64(address + 5368); }
  void set_gather_vector_b_hi(uint64_t v) const { guest::StoreU64(address + 5368, v); }
  uint32_t gather_vector_b_hi_address() const { return address + 5368; }
  // the headlight's position in the model's space
  uint64_t headlight_position_lo() const { return guest::LoadU64(address + 5376); }
  void set_headlight_position_lo(uint64_t v) const { guest::StoreU64(address + 5376, v); }
  uint32_t headlight_position_lo_address() const { return address + 5376; }
  uint64_t headlight_position_hi() const { return guest::LoadU64(address + 5384); }
  void set_headlight_position_hi(uint64_t v) const { guest::StoreU64(address + 5384, v); }
  uint32_t headlight_position_hi_address() const { return address + 5384; }
  float headlight_position_w() const { return guest::LoadFloat(address + 5388); }
  void set_headlight_position_w(float v) const { guest::StoreFloat(address + 5388, v); }
  uint32_t headlight_position_w_address() const { return address + 5388; }
  // the beam's direction, low beams (SceneModel::Beam)
  uint32_t low_beam_direction_address() const { return address + 5392; }
  float low_beam_direction_w() const { return guest::LoadFloat(address + 5404); }
  void set_low_beam_direction_w(float v) const { guest::StoreFloat(address + 5404, v); }
  uint32_t low_beam_direction_w_address() const { return address + 5404; }
  // the beam's direction, high beams
  uint64_t high_beam_direction_lo() const { return guest::LoadU64(address + 5408); }
  void set_high_beam_direction_lo(uint64_t v) const { guest::StoreU64(address + 5408, v); }
  uint32_t high_beam_direction_lo_address() const { return address + 5408; }
  uint64_t high_beam_direction_hi() const { return guest::LoadU64(address + 5416); }
  void set_high_beam_direction_hi(uint64_t v) const { guest::StoreU64(address + 5416, v); }
  uint32_t high_beam_direction_hi_address() const { return address + 5416; }
  float high_beam_direction_w() const { return guest::LoadFloat(address + 5420); }
  void set_high_beam_direction_w(float v) const { guest::StoreFloat(address + 5420, v); }
  uint32_t high_beam_direction_w_address() const { return address + 5420; }
  // the vector Car::DrawUpdate puts in the light slots 2 to 5
  uint64_t light_slot_vector_lo() const { return guest::LoadU64(address + 5424); }
  void set_light_slot_vector_lo(uint64_t v) const { guest::StoreU64(address + 5424, v); }
  uint32_t light_slot_vector_lo_address() const { return address + 5424; }
  uint64_t light_slot_vector_hi() const { return guest::LoadU64(address + 5432); }
  void set_light_slot_vector_hi(uint64_t v) const { guest::StoreU64(address + 5432, v); }
  uint32_t light_slot_vector_hi_address() const { return address + 5432; }
  // the headlight projector's field of view (also the light slot vector's w)
  float projector_fov() const { return guest::LoadFloat(address + 5436); }
  void set_projector_fov(float v) const { guest::StoreFloat(address + 5436, v); }
  uint32_t projector_fov_address() const { return address + 5436; }
  // the light slots' sixth vector
  uint32_t light_slot_sixth_address() const { return address + 5440; }
  float light_slot_sixth_w() const { return guest::LoadFloat(address + 5452); }
  void set_light_slot_sixth_w(float v) const { guest::StoreFloat(address + 5452, v); }
  uint32_t light_slot_sixth_w_address() const { return address + 5452; }
  // the beam's third vector, low beams (SceneModel::BeamShape)
  uint32_t low_beam_third_address() const { return address + 5456; }
  float low_beam_third_w() const { return guest::LoadFloat(address + 5468); }
  void set_low_beam_third_w(float v) const { guest::StoreFloat(address + 5468, v); }
  uint32_t low_beam_third_w_address() const { return address + 5468; }
  // the beam's third vector, high beams
  uint64_t high_beam_third_lo() const { return guest::LoadU64(address + 5472); }
  void set_high_beam_third_lo(uint64_t v) const { guest::StoreU64(address + 5472, v); }
  uint32_t high_beam_third_lo_address() const { return address + 5472; }
  uint64_t high_beam_third_hi() const { return guest::LoadU64(address + 5480); }
  void set_high_beam_third_hi(uint64_t v) const { guest::StoreU64(address + 5480, v); }
  uint32_t high_beam_third_hi_address() const { return address + 5480; }
  float high_beam_third_w() const { return guest::LoadFloat(address + 5484); }
  void set_high_beam_third_w(float v) const { guest::StoreFloat(address + 5484, v); }
  uint32_t high_beam_third_w_address() const { return address + 5484; }
  // CarLighting::Gather's cones
  uint64_t cone_a_lo() const { return guest::LoadU64(address + 5488); }
  void set_cone_a_lo(uint64_t v) const { guest::StoreU64(address + 5488, v); }
  uint32_t cone_a_lo_address() const { return address + 5488; }
  uint64_t cone_a_hi() const { return guest::LoadU64(address + 5496); }
  void set_cone_a_hi(uint64_t v) const { guest::StoreU64(address + 5496, v); }
  uint32_t cone_a_hi_address() const { return address + 5496; }
  // a light cone's angle (CarLighting::Gather)
  float cone_angle() const { return guest::LoadFloat(address + 5500); }
  void set_cone_angle(float v) const { guest::StoreFloat(address + 5500, v); }
  uint32_t cone_angle_address() const { return address + 5500; }
  uint64_t cone_b_lo() const { return guest::LoadU64(address + 5504); }
  void set_cone_b_lo(uint64_t v) const { guest::StoreU64(address + 5504, v); }
  uint32_t cone_b_lo_address() const { return address + 5504; }
  uint64_t cone_b_hi() const { return guest::LoadU64(address + 5512); }
  void set_cone_b_hi(uint64_t v) const { guest::StoreU64(address + 5512, v); }
  uint32_t cone_b_hi_address() const { return address + 5512; }
  float cone_b_w() const { return guest::LoadFloat(address + 5516); }
  void set_cone_b_w(float v) const { guest::StoreFloat(address + 5516, v); }
  uint32_t cone_b_w_address() const { return address + 5516; }
  // InitLighting's last float (0x8200256C), read by CarLighting::Gather
  float lighting_end() const { return guest::LoadFloat(address + 5520); }
  void set_lighting_end(float v) const { guest::StoreFloat(address + 5520, v); }
  uint32_t lighting_end_address() const { return address + 5520; }
  // the SceneAssets (scene.toml) at +64: CarScene::OnFileLoaded's r6 reaches its slots' requests and states through the scene (scene's report)
  uint32_t assets_address() const { return address + 64; }
};

// embedded; measure: CarPart
struct CarPart : View {
  using View::View;
  static constexpr uint32_t kSize = 240;
  // its node record ([+0] + 136: the skeleton's entries)
  uint32_t node() const { return guest::LoadU32(address + 0); }
  void set_node(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t node_address() const { return address + 0; }
  // the part model (0 when not found)
  uint32_t model() const { return guest::LoadU32(address + 4); }
  void set_model(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t model_address() const { return address + 4; }
  uint32_t matrix_address() const { return address + 16; }
  uint32_t inverse_address() const { return address + 80; }
  // the glows' node position, two doublewords
  uint64_t position_lo() const { return guest::LoadU64(address + 144); }
  void set_position_lo(uint64_t v) const { guest::StoreU64(address + 144, v); }
  uint32_t position_lo_address() const { return address + 144; }
  uint64_t position_hi() const { return guest::LoadU64(address + 152); }
  void set_position_hi(uint64_t v) const { guest::StoreU64(address + 152, v); }
  uint32_t position_hi_address() const { return address + 152; }
  uint32_t node_index() const { return guest::LoadU32(address + 224); }
  void set_node_index(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t node_index_address() const { return address + 224; }
  // the shape's index in the model
  uint32_t shape() const { return guest::LoadU32(address + 228); }
  void set_shape(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t shape_address() const { return address + 228; }
};

// embedded; measure: PartsPoints, PartsPoints::DrawFrontBumper.r3, PartsPoints::DrawRearBumper.r3, PartsPoints::DrawHeadlights.r3, PartsPoints::DrawDoorWindows02.r3, PartsPoints::DrawDoorWindows01.r3, PartsPoints::DrawTailLights.r3, PartsPoints::DrawHeadlightBody.r3, PartsPoints::DrawFrontBumperLights.r3, PartsPoints::ResetRearBumper.r3, PartsPoints::DrawCabinWindow02.r3, PartsPoints::DrawCabinWindow01.r3, PartsPoints::ResetShadows.r3, PartsPoints::DrawBonnet.r3
struct PartsPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 37108;
  uint32_t record0_node() const { return guest::LoadU32(address + 0); }
  void set_record0_node(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t record0_node_address() const { return address + 0; }
  uint32_t record0_model() const { return guest::LoadU32(address + 4); }
  void set_record0_model(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t record0_model_address() const { return address + 4; }
  // the mufflers' group: four matrices at +0, their rows' w's 1.0 (InitMufflers)
  float mufflers_row0_w() const { return guest::LoadFloat(address + 12); }
  void set_mufflers_row0_w(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t mufflers_row0_w_address() const { return address + 12; }
  float mufflers_row1_w() const { return guest::LoadFloat(address + 28); }
  void set_mufflers_row1_w(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t mufflers_row1_w_address() const { return address + 28; }
  float mufflers_row2_w() const { return guest::LoadFloat(address + 44); }
  void set_mufflers_row2_w(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t mufflers_row2_w_address() const { return address + 44; }
  float mufflers_row3_w() const { return guest::LoadFloat(address + 60); }
  void set_mufflers_row3_w(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t mufflers_row3_w_address() const { return address + 60; }
  // record 0's position, two doublewords (the glows)
  uint64_t record0_position_lo() const { return guest::LoadU64(address + 144); }
  void set_record0_position_lo(uint64_t v) const { guest::StoreU64(address + 144, v); }
  uint32_t record0_position_lo_address() const { return address + 144; }
  uint64_t record0_position_hi() const { return guest::LoadU64(address + 152); }
  void set_record0_position_hi(uint64_t v) const { guest::StoreU64(address + 152, v); }
  uint32_t record0_position_hi_address() const { return address + 152; }
  uint32_t record0_node_index() const { return guest::LoadU32(address + 224); }
  void set_record0_node_index(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t record0_node_index_address() const { return address + 224; }
  uint32_t record0_shape() const { return guest::LoadU32(address + 228); }
  void set_record0_shape(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t record0_shape_address() const { return address + 228; }
  uint32_t record1_model() const { return guest::LoadU32(address + 244); }
  void set_record1_model(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t record1_model_address() const { return address + 244; }
  // record 1's position (the headlights' glows)
  uint64_t record1_position_lo() const { return guest::LoadU64(address + 384); }
  void set_record1_position_lo(uint64_t v) const { guest::StoreU64(address + 384, v); }
  uint32_t record1_position_lo_address() const { return address + 384; }
  uint64_t record1_position_hi() const { return guest::LoadU64(address + 392); }
  void set_record1_position_hi(uint64_t v) const { guest::StoreU64(address + 392, v); }
  uint32_t record1_position_hi_address() const { return address + 392; }
  uint32_t record1_shape() const { return guest::LoadU32(address + 468); }
  void set_record1_shape(uint32_t v) const { guest::StoreU32(address + 468, v); }
  uint32_t record1_shape_address() const { return address + 468; }
  uint32_t record2_model() const { return guest::LoadU32(address + 484); }
  void set_record2_model(uint32_t v) const { guest::StoreU32(address + 484, v); }
  uint32_t record2_model_address() const { return address + 484; }
  uint32_t record2_shape() const { return guest::LoadU32(address + 708); }
  void set_record2_shape(uint32_t v) const { guest::StoreU32(address + 708, v); }
  uint32_t record2_shape_address() const { return address + 708; }
  uint32_t record3_model() const { return guest::LoadU32(address + 724); }
  void set_record3_model(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t record3_model_address() const { return address + 724; }
  uint32_t record3_shape() const { return guest::LoadU32(address + 948); }
  void set_record3_shape(uint32_t v) const { guest::StoreU32(address + 948, v); }
  uint32_t record3_shape_address() const { return address + 948; }
  // the shadows' group: 0 (ResetShadows), after its four records
  uint32_t shadows_word_960() const { return guest::LoadU32(address + 960); }
  void set_shadows_word_960(uint32_t v) const { guest::StoreU32(address + 960, v); }
  uint32_t shadows_word_960_address() const { return address + 960; }
  uint32_t record4_model() const { return guest::LoadU32(address + 964); }
  void set_record4_model(uint32_t v) const { guest::StoreU32(address + 964, v); }
  uint32_t record4_model_address() const { return address + 964; }
  uint32_t record4_shape() const { return guest::LoadU32(address + 1188); }
  void set_record4_shape(uint32_t v) const { guest::StoreU32(address + 1188, v); }
  uint32_t record4_shape_address() const { return address + 1188; }
  uint32_t record5_model() const { return guest::LoadU32(address + 1204); }
  void set_record5_model(uint32_t v) const { guest::StoreU32(address + 1204, v); }
  uint32_t record5_model_address() const { return address + 1204; }
  uint32_t record5_shape() const { return guest::LoadU32(address + 1428); }
  void set_record5_shape(uint32_t v) const { guest::StoreU32(address + 1428, v); }
  uint32_t record5_shape_address() const { return address + 1428; }
  // the rear bumper's: the number plate's model
  uint32_t record6_model() const { return guest::LoadU32(address + 1444); }
  void set_record6_model(uint32_t v) const { guest::StoreU32(address + 1444, v); }
  uint32_t record6_model_address() const { return address + 1444; }
  uint32_t record6_shape() const { return guest::LoadU32(address + 1668); }
  void set_record6_shape(uint32_t v) const { guest::StoreU32(address + 1668, v); }
  uint32_t record6_shape_address() const { return address + 1668; }
  // the rear bumper's group: 1 when made (the plate's texture set)
  uint32_t rear_plate_set() const { return guest::LoadU32(address + 1680); }
  void set_rear_plate_set(uint32_t v) const { guest::StoreU32(address + 1680, v); }
  uint32_t rear_plate_set_address() const { return address + 1680; }
  uint32_t record7_model() const { return guest::LoadU32(address + 1684); }
  void set_record7_model(uint32_t v) const { guest::StoreU32(address + 1684, v); }
  uint32_t record7_model_address() const { return address + 1684; }
  // the rear bumper's group (after its seven records): the plate's texture
  uint32_t rear_plate_texture() const { return guest::LoadU32(address + 1684); }
  void set_rear_plate_texture(uint32_t v) const { guest::StoreU32(address + 1684, v); }
  uint32_t rear_plate_texture_address() const { return address + 1684; }
  uint32_t record7_shape() const { return guest::LoadU32(address + 1908); }
  void set_record7_shape(uint32_t v) const { guest::StoreU32(address + 1908, v); }
  uint32_t record7_shape_address() const { return address + 1908; }
  // the front bumper's: the number plate's model
  uint32_t record8_model() const { return guest::LoadU32(address + 1924); }
  void set_record8_model(uint32_t v) const { guest::StoreU32(address + 1924, v); }
  uint32_t record8_model_address() const { return address + 1924; }
  uint32_t record8_shape() const { return guest::LoadU32(address + 2148); }
  void set_record8_shape(uint32_t v) const { guest::StoreU32(address + 2148, v); }
  uint32_t record8_shape_address() const { return address + 2148; }
  uint32_t record9_model() const { return guest::LoadU32(address + 2164); }
  void set_record9_model(uint32_t v) const { guest::StoreU32(address + 2164, v); }
  uint32_t record9_model_address() const { return address + 2164; }
  uint32_t record9_shape() const { return guest::LoadU32(address + 2388); }
  void set_record9_shape(uint32_t v) const { guest::StoreU32(address + 2388, v); }
  uint32_t record9_shape_address() const { return address + 2388; }
  uint32_t record10_model() const { return guest::LoadU32(address + 2404); }
  void set_record10_model(uint32_t v) const { guest::StoreU32(address + 2404, v); }
  uint32_t record10_model_address() const { return address + 2404; }
  uint32_t record10_shape() const { return guest::LoadU32(address + 2628); }
  void set_record10_shape(uint32_t v) const { guest::StoreU32(address + 2628, v); }
  uint32_t record10_shape_address() const { return address + 2628; }
  // the front bumper's group (after its eleven records): the plate's texture set
  uint32_t front_plate_set() const { return guest::LoadU32(address + 2640); }
  void set_front_plate_set(uint32_t v) const { guest::StoreU32(address + 2640, v); }
  uint32_t front_plate_set_address() const { return address + 2640; }
  uint32_t record11_model() const { return guest::LoadU32(address + 2644); }
  void set_record11_model(uint32_t v) const { guest::StoreU32(address + 2644, v); }
  uint32_t record11_model_address() const { return address + 2644; }
  // the front bumper's group: the plate's texture (the car model's loading object's +6700)
  uint32_t front_plate_texture() const { return guest::LoadU32(address + 2644); }
  void set_front_plate_texture(uint32_t v) const { guest::StoreU32(address + 2644, v); }
  uint32_t front_plate_texture_address() const { return address + 2644; }
  // the front bumper's group: the car record (+3684 the plate's record index)
  uint32_t front_car_record() const { return guest::LoadU32(address + 2648); }
  void set_front_car_record(uint32_t v) const { guest::StoreU32(address + 2648, v); }
  uint32_t front_car_record_address() const { return address + 2648; }
  uint32_t record11_shape() const { return guest::LoadU32(address + 2868); }
  void set_record11_shape(uint32_t v) const { guest::StoreU32(address + 2868, v); }
  uint32_t record11_shape_address() const { return address + 2868; }
  // the headlights' group: the pop-up's matrix (DrawHeadlightBody, for DrawHeadlights)
  uint32_t headlights_popup_matrix_address() const { return address + 3120; }
  uint32_t record13_model() const { return guest::LoadU32(address + 3124); }
  void set_record13_model(uint32_t v) const { guest::StoreU32(address + 3124, v); }
  uint32_t record13_model_address() const { return address + 3124; }
  // the headlights' group: the pop-up's largest angle (FindHeadlights)
  float headlights_popup_max() const { return guest::LoadFloat(address + 3184); }
  void set_headlights_popup_max(float v) const { guest::StoreFloat(address + 3184, v); }
  uint32_t headlights_popup_max_address() const { return address + 3184; }
  uint32_t record13_shape() const { return guest::LoadU32(address + 3348); }
  void set_record13_shape(uint32_t v) const { guest::StoreU32(address + 3348, v); }
  uint32_t record13_shape_address() const { return address + 3348; }
  // the block's: the rear bumper's plate texture (+2896 + 1684)
  uint32_t block_rear_plate_texture() const { return guest::LoadU32(address + 4580); }
  void set_block_rear_plate_texture(uint32_t v) const { guest::StoreU32(address + 4580, v); }
  uint32_t block_rear_plate_texture_address() const { return address + 4580; }
  // set b's doors' group: the parts record (+12 the body's kind, +28 the windows')
  template <typename T = PartsRecord>
  T doors_parts_record() const { return T(guest::LoadU32(address + 5520)); }
  void set_doors_parts_record(const View& v) const { guest::StoreU32(address + 5520, v.address); }
  void set_doors_parts_record(uint32_t v) const { guest::StoreU32(address + 5520, v); }
  uint32_t doors_parts_record_address() const { return address + 5520; }
  // a door's group: the first-set car model's skeleton (PartsPoints::Find)
  uint32_t door_skeleton() const { return guest::LoadU32(address + 6000); }
  void set_door_skeleton(uint32_t v) const { guest::StoreU32(address + 6000, v); }
  uint32_t door_skeleton_address() const { return address + 6000; }
  // a door's group: the door's matrix (DrawLeftDoor, DrawRightDoor)
  uint32_t door_matrix_address() const { return address + 6016; }
  // a door's group: the car record (+3688 scissor doors)
  uint32_t door_car_record() const { return guest::LoadU32(address + 6080); }
  void set_door_car_record(uint32_t v) const { guest::StoreU32(address + 6080, v); }
  uint32_t door_car_record_address() const { return address + 6080; }
  // a door's group: the parts record (+12, +28)
  template <typename T = PartsRecord>
  T door_parts_record() const { return T(guest::LoadU32(address + 6084)); }
  void set_door_parts_record(const View& v) const { guest::StoreU32(address + 6084, v.address); }
  void set_door_parts_record(uint32_t v) const { guest::StoreU32(address + 6084, v); }
  uint32_t door_parts_record_address() const { return address + 6084; }
  // the block's: the left door's group's +6000
  uint32_t block_left_door_skeleton() const { return guest::LoadU32(address + 12032); }
  void set_block_left_door_skeleton(uint32_t v) const { guest::StoreU32(address + 12032, v); }
  uint32_t block_left_door_skeleton_address() const { return address + 12032; }
  uint32_t block_left_door_car_record() const { return guest::LoadU32(address + 12112); }
  void set_block_left_door_car_record(uint32_t v) const { guest::StoreU32(address + 12112, v); }
  uint32_t block_left_door_car_record_address() const { return address + 12112; }
  uint32_t block_left_door_parts_record() const { return guest::LoadU32(address + 12116); }
  void set_block_left_door_parts_record(uint32_t v) const { guest::StoreU32(address + 12116, v); }
  uint32_t block_left_door_parts_record_address() const { return address + 12116; }
  uint32_t block_right_door_skeleton() const { return guest::LoadU32(address + 18128); }
  void set_block_right_door_skeleton(uint32_t v) const { guest::StoreU32(address + 18128, v); }
  uint32_t block_right_door_skeleton_address() const { return address + 18128; }
  uint32_t block_right_door_car_record() const { return guest::LoadU32(address + 18208); }
  void set_block_right_door_car_record(uint32_t v) const { guest::StoreU32(address + 18208, v); }
  uint32_t block_right_door_car_record_address() const { return address + 18208; }
  uint32_t block_right_door_parts_record() const { return guest::LoadU32(address + 18212); }
  void set_block_right_door_parts_record(uint32_t v) const { guest::StoreU32(address + 18212, v); }
  uint32_t block_right_door_parts_record_address() const { return address + 18212; }
  // the block's: set b's doors' group's +5520
  uint32_t block_doors_parts_record() const { return guest::LoadU32(address + 23744); }
  void set_block_doors_parts_record(uint32_t v) const { guest::StoreU32(address + 23744, v); }
  uint32_t block_doors_parts_record_address() const { return address + 23744; }
  uint32_t scene_model_record() const { return guest::LoadU32(address + 37072); }
  void set_scene_model_record(uint32_t v) const { guest::StoreU32(address + 37072, v); }
  uint32_t scene_model_record_address() const { return address + 37072; }
  // the first-set car model's car record (+0 the headlights' color index, +3688 scissor doors)
  uint32_t car_record() const { return guest::LoadU32(address + 37076); }
  void set_car_record(uint32_t v) const { guest::StoreU32(address + 37076, v); }
  uint32_t car_record_address() const { return address + 37076; }
  // the parts object's 60-byte record (+108)
  template <typename T = PartsRecord>
  T parts_record() const { return T(guest::LoadU32(address + 37080)); }
  void set_parts_record(const View& v) const { guest::StoreU32(address + 37080, v.address); }
  void set_parts_record(uint32_t v) const { guest::StoreU32(address + 37080, v); }
  uint32_t parts_record_address() const { return address + 37080; }
  // the first-set car model (the parts object's +24)
  uint32_t car_model() const { return guest::LoadU32(address + 37084); }
  void set_car_model(uint32_t v) const { guest::StoreU32(address + 37084, v); }
  uint32_t car_model_address() const { return address + 37084; }
  template <typename T = CarScene>
  T scene() const { return T(guest::LoadU32(address + 37088)); }
  void set_scene(const View& v) const { guest::StoreU32(address + 37088, v.address); }
  void set_scene(uint32_t v) const { guest::StoreU32(address + 37088, v); }
  uint32_t scene_address() const { return address + 37088; }
  // the first-set car model's skeleton
  uint32_t skeleton() const { return guest::LoadU32(address + 37092); }
  void set_skeleton(uint32_t v) const { guest::StoreU32(address + 37092, v); }
  uint32_t skeleton_address() const { return address + 37092; }
};

// embedded; measure: sub_820D3558.r3, PartsPoints::DrawRace.ret@+176.+37080, SceneParts::Draw.r3@+176.+37080, SceneParts::Draw.r3@+176@+18224.+5520, PartsPoints::DrawRace.ret@+176@+6032.+6084, PartsPoints::DrawRace.ret@+176@+12128.+6084
struct PartsRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // the doors' body record (12 + it, drawn when below 11, signed)
  uint32_t doors_body_kind() const { return guest::LoadU32(address + 12); }
  void set_doors_body_kind(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t doors_body_kind_address() const { return address + 12; }
  // the door panels' and windows' record (7 + it, 3 + it)
  uint32_t door_panel_kind() const { return guest::LoadU32(address + 28); }
  void set_door_panel_kind(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t door_panel_kind_address() const { return address + 28; }
  // the headlights' color table sub-index
  uint32_t headlight_color() const { return guest::LoadU32(address + 32); }
  void set_headlight_color(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t headlight_color_address() const { return address + 32; }
  // handed to the front end's and race's draws as r24's word (not read in detail)
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_36() const { return guest::LoadU32(address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  // sub_820D3558's store (it writes all 15 words; not read in detail)
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// embedded; measure: SceneLights@+144, SceneLights@+4480, SceneLights@+8816, SceneLights@+96, LightPoints::FindGroup.r3, LightPoints::ResetFront.r3, LightPoints::ResetSides.r3, LightPoints::ResetRear.r3
struct LightPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 13056;
  // the light kit's 52-byte record (a group's: 0 when reset)
  template <typename T = LightKitRecord>
  T record() const { return T(guest::LoadU32(address + 0)); }
  void set_record(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t record_address() const { return address + 0; }
  // the block's: BlinkTimers::Advance's counters, frames
  uint64_t blink_counter() const { return guest::LoadU64(address + 8); }
  void set_blink_counter(uint64_t v) const { guest::StoreU64(address + 8, v); }
  uint32_t blink_counter_address() const { return address + 8; }
  uint64_t front_counter() const { return guest::LoadU64(address + 16); }
  void set_front_counter(uint64_t v) const { guest::StoreU64(address + 16, v); }
  uint32_t front_counter_address() const { return address + 16; }
  // a group's record 0 (the neon): its model
  uint32_t neon_model() const { return guest::LoadU32(address + 20); }
  void set_neon_model(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t neon_model_address() const { return address + 20; }
  uint64_t sides_counter() const { return guest::LoadU64(address + 24); }
  void set_sides_counter(uint64_t v) const { guest::StoreU64(address + 24, v); }
  uint32_t sides_counter_address() const { return address + 24; }
  uint64_t rear_counter() const { return guest::LoadU64(address + 32); }
  void set_rear_counter(uint64_t v) const { guest::StoreU64(address + 32, v); }
  uint32_t rear_counter_address() const { return address + 32; }
  uint32_t neon_shape() const { return guest::LoadU32(address + 244); }
  void set_neon_shape(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t neon_shape_address() const { return address + 244; }
  // the LEDs' records (not seen in play)
  uint32_t led1_model() const { return guest::LoadU32(address + 260); }
  void set_led1_model(uint32_t v) const { guest::StoreU32(address + 260, v); }
  uint32_t led1_model_address() const { return address + 260; }
  uint32_t led1_shape() const { return guest::LoadU32(address + 484); }
  void set_led1_shape(uint32_t v) const { guest::StoreU32(address + 484, v); }
  uint32_t led1_shape_address() const { return address + 484; }
  uint32_t led2_model() const { return guest::LoadU32(address + 500); }
  void set_led2_model(uint32_t v) const { guest::StoreU32(address + 500, v); }
  uint32_t led2_model_address() const { return address + 500; }
  uint32_t led2_shape() const { return guest::LoadU32(address + 724); }
  void set_led2_shape(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t led2_shape_address() const { return address + 724; }
  // read by the LEDs' draws (not read in detail)
  uint32_t unknown_760() const { return guest::LoadU32(address + 760); }
  void set_unknown_760(uint32_t v) const { guest::StoreU32(address + 760, v); }
  uint32_t unknown_760_address() const { return address + 760; }
  // read by the LEDs' draws (not read in detail)
  uint32_t unknown_984() const { return guest::LoadU32(address + 984); }
  void set_unknown_984(uint32_t v) const { guest::StoreU32(address + 984, v); }
  uint32_t unknown_984_address() const { return address + 984; }
  // record 12 (the first spot; not seen in play)
  uint32_t spot_model() const { return guest::LoadU32(address + 2900); }
  void set_spot_model(uint32_t v) const { guest::StoreU32(address + 2900, v); }
  uint32_t spot_model_address() const { return address + 2900; }
  uint32_t spot_shape() const { return guest::LoadU32(address + 3124); }
  void set_spot_shape(uint32_t v) const { guest::StoreU32(address + 3124, v); }
  uint32_t spot_shape_address() const { return address + 3124; }
};

// embedded; measure: sub_820D34E0.r3, SceneLights@+144.+0, SceneLights@+4480.+0, SceneLights@+8816.+0, SceneLights@+96.+0
struct LightKitRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // PartModel::SetTexture's texture (SceneLights::Finish)
  uint32_t texture() const { return guest::LoadU32(address + 0); }
  void set_texture(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t texture_address() const { return address + 0; }
  // 0 none, 1 neon, 2 to 6 LEDs, 7 to 9 spots (blinking from 2)
  uint32_t front_mode() const { return guest::LoadU32(address + 4); }
  void set_front_mode(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t front_mode_address() const { return address + 4; }
  // the on fraction
  float front_on() const { return guest::LoadFloat(address + 8); }
  void set_front_on(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t front_on_address() const { return address + 8; }
  float front_fade() const { return guest::LoadFloat(address + 12); }
  void set_front_fade(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t front_fade_address() const { return address + 12; }
  float front_rate() const { return guest::LoadFloat(address + 16); }
  void set_front_rate(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t front_rate_address() const { return address + 16; }
  // spots up to 11
  uint32_t sides_mode() const { return guest::LoadU32(address + 20); }
  void set_sides_mode(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t sides_mode_address() const { return address + 20; }
  float sides_on() const { return guest::LoadFloat(address + 24); }
  void set_sides_on(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t sides_on_address() const { return address + 24; }
  float sides_fade() const { return guest::LoadFloat(address + 28); }
  void set_sides_fade(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t sides_fade_address() const { return address + 28; }
  float sides_rate() const { return guest::LoadFloat(address + 32); }
  void set_sides_rate(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t sides_rate_address() const { return address + 32; }
  uint32_t rear_mode() const { return guest::LoadU32(address + 36); }
  void set_rear_mode(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t rear_mode_address() const { return address + 36; }
  float rear_on() const { return guest::LoadFloat(address + 40); }
  void set_rear_on(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t rear_on_address() const { return address + 40; }
  float rear_fade() const { return guest::LoadFloat(address + 44); }
  void set_rear_fade(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t rear_fade_address() const { return address + 44; }
  float rear_rate() const { return guest::LoadFloat(address + 48); }
  void set_rear_rate(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t rear_rate_address() const { return address + 48; }
};

// embedded; measure: TrafficPoints::Draw.r3@+16, TrafficPoints::Draw.r3@+1472, TrafficPoints::Find.r3@+3152, TrafficPoints::Draw.r3@+3152, TrafficPoints::Find.r3@+1472, SceneTraffic@+3728
struct TrafficPoints : View {
  using View::View;
  static constexpr uint32_t kSize = 6032;
  // the block's: 0 (TrafficPoints::Reset; not read in detail)
  uint32_t word_0() const { return guest::LoadU32(address + 0); }
  void set_word_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t word_0_address() const { return address + 0; }
  uint32_t record0_model() const { return guest::LoadU32(address + 4); }
  void set_record0_model(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t record0_model_address() const { return address + 4; }
  uint64_t record0_position_lo() const { return guest::LoadU64(address + 144); }
  void set_record0_position_lo(uint64_t v) const { guest::StoreU64(address + 144, v); }
  uint32_t record0_position_lo_address() const { return address + 144; }
  uint64_t record0_position_hi() const { return guest::LoadU64(address + 152); }
  void set_record0_position_hi(uint64_t v) const { guest::StoreU64(address + 152, v); }
  uint32_t record0_position_hi_address() const { return address + 152; }
  // the body's: the front plate
  uint32_t record2_model() const { return guest::LoadU32(address + 484); }
  void set_record2_model(uint32_t v) const { guest::StoreU32(address + 484, v); }
  uint32_t record2_model_address() const { return address + 484; }
  // the body's: the rear plate
  uint32_t record3_model() const { return guest::LoadU32(address + 724); }
  void set_record3_model(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t record3_model_address() const { return address + 724; }
  // the body's: the window (or the headlights' add-on)
  uint32_t record5_model() const { return guest::LoadU32(address + 1204); }
  void set_record5_model(uint32_t v) const { guest::StoreU32(address + 1204, v); }
  uint32_t record5_model_address() const { return address + 1204; }
  uint32_t record5_shape() const { return guest::LoadU32(address + 1428); }
  void set_record5_shape(uint32_t v) const { guest::StoreU32(address + 1428, v); }
  uint32_t record5_shape_address() const { return address + 1428; }
  // the body's records' +1440 (the body's +7040)
  uint32_t front_plate_texture() const { return guest::LoadU32(address + 1440); }
  void set_front_plate_texture(uint32_t v) const { guest::StoreU32(address + 1440, v); }
  uint32_t front_plate_texture_address() const { return address + 1440; }
  // the body's records' +1444 (the body's +7048)
  uint32_t rear_plate_texture() const { return guest::LoadU32(address + 1444); }
  void set_rear_plate_texture(uint32_t v) const { guest::StoreU32(address + 1444, v); }
  uint32_t rear_plate_texture_address() const { return address + 1444; }
  // the headlights' cover
  uint32_t record6_model() const { return guest::LoadU32(address + 1444); }
  void set_record6_model(uint32_t v) const { guest::StoreU32(address + 1444, v); }
  uint32_t record6_model_address() const { return address + 1444; }
  uint32_t record6_shape() const { return guest::LoadU32(address + 1668); }
  void set_record6_shape(uint32_t v) const { guest::StoreU32(address + 1668, v); }
  uint32_t record6_shape_address() const { return address + 1668; }
  uint32_t record9_model() const { return guest::LoadU32(address + 2164); }
  void set_record9_model(uint32_t v) const { guest::StoreU32(address + 2164, v); }
  uint32_t record9_model_address() const { return address + 2164; }
  uint32_t record9_shape() const { return guest::LoadU32(address + 2388); }
  void set_record9_shape(uint32_t v) const { guest::StoreU32(address + 2388, v); }
  uint32_t record9_shape_address() const { return address + 2388; }
  uint32_t record10_model() const { return guest::LoadU32(address + 2404); }
  void set_record10_model(uint32_t v) const { guest::StoreU32(address + 2404, v); }
  uint32_t record10_model_address() const { return address + 2404; }
  uint32_t record10_shape() const { return guest::LoadU32(address + 2628); }
  void set_record10_shape(uint32_t v) const { guest::StoreU32(address + 2628, v); }
  uint32_t record10_shape_address() const { return address + 2628; }
};

// embedded; measure: sub_820D3840.r3, SceneInterior@+160@+240.+0, SceneInterior@+160@+9488.+0, SceneInterior@+160@+10464.+0, SceneInterior@+160.+9488, SceneInterior@+160.+11444, SceneInterior@+160@+3152.+0
struct InteriorRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 120;
  // 0 face record 4, else 5
  uint32_t speedometer() const { return guest::LoadU32(address + 0); }
  void set_speedometer(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t speedometer_address() const { return address + 0; }
  // 0, 1, 2 face records 6, 7, 8, else none
  uint32_t tachometer() const { return guest::LoadU32(address + 4); }
  void set_tachometer(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t tachometer_address() const { return address + 4; }
  // the seven gauges, 12 bytes each: +8 present, +12 its kind
  uint32_t gauge0_present() const { return guest::LoadU32(address + 8); }
  void set_gauge0_present(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t gauge0_present_address() const { return address + 8; }
  uint32_t gauge0_kind() const { return guest::LoadU32(address + 12); }
  void set_gauge0_kind(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t gauge0_kind_address() const { return address + 12; }
  uint32_t gauge1_present() const { return guest::LoadU32(address + 20); }
  void set_gauge1_present(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t gauge1_present_address() const { return address + 20; }
  uint32_t gauge1_kind() const { return guest::LoadU32(address + 24); }
  void set_gauge1_kind(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t gauge1_kind_address() const { return address + 24; }
  uint32_t gauge2_present() const { return guest::LoadU32(address + 32); }
  void set_gauge2_present(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t gauge2_present_address() const { return address + 32; }
  uint32_t gauge2_kind() const { return guest::LoadU32(address + 36); }
  void set_gauge2_kind(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t gauge2_kind_address() const { return address + 36; }
  uint32_t gauge3_present() const { return guest::LoadU32(address + 44); }
  void set_gauge3_present(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t gauge3_present_address() const { return address + 44; }
  uint32_t gauge3_kind() const { return guest::LoadU32(address + 48); }
  void set_gauge3_kind(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t gauge3_kind_address() const { return address + 48; }
  uint32_t gauge4_present() const { return guest::LoadU32(address + 56); }
  void set_gauge4_present(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t gauge4_present_address() const { return address + 56; }
  uint32_t gauge4_kind() const { return guest::LoadU32(address + 60); }
  void set_gauge4_kind(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t gauge4_kind_address() const { return address + 60; }
  uint32_t gauge5_present() const { return guest::LoadU32(address + 68); }
  void set_gauge5_present(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t gauge5_present_address() const { return address + 68; }
  uint32_t gauge5_kind() const { return guest::LoadU32(address + 72); }
  void set_gauge5_kind(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t gauge5_kind_address() const { return address + 72; }
  uint32_t gauge6_present() const { return guest::LoadU32(address + 80); }
  void set_gauge6_present(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t gauge6_present_address() const { return address + 80; }
  uint32_t gauge6_kind() const { return guest::LoadU32(address + 84); }
  void set_gauge6_kind(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t gauge6_kind_address() const { return address + 84; }
  // the steering wheel's record (+8512 + 16 + 240 x it)
  uint32_t steering_wheel() const { return guest::LoadU32(address + 92); }
  void set_steering_wheel(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t steering_wheel_address() const { return address + 92; }
  uint32_t seat_texture() const { return guest::LoadU32(address + 96); }
  void set_seat_texture(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t seat_texture_address() const { return address + 96; }
  // 0 the stock seats, else the aftermarket seat model (the scene's +784 + 464 x it)
  uint32_t seat_model() const { return guest::LoadU32(address + 100); }
  void set_seat_model(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t seat_model_address() const { return address + 100; }
  // 1 record 0, 2 record 1, else none
  uint32_t roll_cage() const { return guest::LoadU32(address + 104); }
  void set_roll_cage(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t roll_cage_address() const { return address + 104; }
  // 0 record 2, else 3 (probably the strut tower bar)
  uint32_t strut_bar() const { return guest::LoadU32(address + 108); }
  void set_strut_bar(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t strut_bar_address() const { return address + 108; }
  // sub_820D3840's store (gauge 0's third word; not read in detail)
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // sub_820D3840's store (gauge 1's third word)
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  // sub_820D3840's store (gauge 2's third word)
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  // sub_820D3840's store (gauge 3's third word)
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  // sub_820D3840's store (gauge 4's third word)
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  // sub_820D3840's store (gauge 5's third word)
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  // sub_820D3840's store (gauge 6's third word)
  uint32_t unknown_88() const { return guest::LoadU32(address + 88); }
  void set_unknown_88(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  // sub_820D3840's store: the accessory word (scene-models' report)
  uint32_t accessory() const { return guest::LoadU32(address + 112); }
  void set_accessory(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t accessory_address() const { return address + 112; }
  // sub_820D3840's store
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
};

// embedded; measure: SceneInterior@+160@+240, SceneInterior@+400
struct InteriorDials : View {
  using View::View;
  static constexpr uint32_t kSize = 2912;
  template <typename T = InteriorRecord>
  T record() const { return T(guest::LoadU32(address + 0)); }
  void set_record(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t record_address() const { return address + 0; }
  // the car's block (InteriorDials::SetBlock)
  uint32_t car_block() const { return guest::LoadU32(address + 4); }
  void set_car_block(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t car_block_address() const { return address + 4; }
  // the digits' texture provider (the scene's +1132 in set b, +1064 in set a)
  uint32_t digit_provider() const { return guest::LoadU32(address + 8); }
  void set_digit_provider(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t digit_provider_address() const { return address + 8; }
  // record 0's model: Dash::PoseTachometer runs when set
  uint32_t tachometer_model() const { return guest::LoadU32(address + 20); }
  void set_tachometer_model(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t tachometer_model_address() const { return address + 20; }
  // record 9's model: DrawDigitalSpeed runs when set
  uint32_t digits_model() const { return guest::LoadU32(address + 2180); }
  void set_digits_model(uint32_t v) const { guest::StoreU32(address + 2180, v); }
  uint32_t digits_model_address() const { return address + 2180; }
  // record 9's shape
  uint32_t ones_shape() const { return guest::LoadU32(address + 2404); }
  void set_ones_shape(uint32_t v) const { guest::StoreU32(address + 2404, v); }
  uint32_t ones_shape_address() const { return address + 2404; }
  // read by InteriorPoints::FindDials (not read)
  uint32_t unknown_2420() const { return guest::LoadU32(address + 2420); }
  void set_unknown_2420(uint32_t v) const { guest::StoreU32(address + 2420, v); }
  uint32_t unknown_2420_address() const { return address + 2420; }
  // record 10's shape
  uint32_t tens_shape() const { return guest::LoadU32(address + 2644); }
  void set_tens_shape(uint32_t v) const { guest::StoreU32(address + 2644, v); }
  uint32_t tens_shape_address() const { return address + 2644; }
  // read by InteriorPoints::FindDials (not read)
  uint32_t unknown_2660() const { return guest::LoadU32(address + 2660); }
  void set_unknown_2660(uint32_t v) const { guest::StoreU32(address + 2660, v); }
  uint32_t unknown_2660_address() const { return address + 2660; }
  // record 11's shape
  uint32_t hundreds_shape() const { return guest::LoadU32(address + 2884); }
  void set_hundreds_shape(uint32_t v) const { guest::StoreU32(address + 2884, v); }
  uint32_t hundreds_shape_address() const { return address + 2884; }
  // a 48-byte record of the table at 0x82003680 (sub_820D3728 of the block)
  uint32_t gauge_record() const { return guest::LoadU32(address + 2896); }
  void set_gauge_record(uint32_t v) const { guest::StoreU32(address + 2896, v); }
  uint32_t gauge_record_address() const { return address + 2896; }
};

// embedded; measure: SceneInterior@+160@+3152
struct Dash : View {
  using View::View;
  static constexpr uint32_t kSize = 5356;
  template <typename T = InteriorRecord>
  T record() const { return T(guest::LoadU32(address + 0)); }
  void set_record(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t record_address() const { return address + 0; }
  // 5: gauge 1 is the turbo gauge (Dash::PoseDials; SceneInterior::Finish writes the block here)
  uint32_t mode() const { return guest::LoadU32(address + 4); }
  void set_mode(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t mode_address() const { return address + 4; }
  // the seven needles' models
  uint32_t needle0_model() const { return guest::LoadU32(address + 1700); }
  void set_needle0_model(uint32_t v) const { guest::StoreU32(address + 1700, v); }
  uint32_t needle0_model_address() const { return address + 1700; }
  uint32_t needle1_model() const { return guest::LoadU32(address + 1940); }
  void set_needle1_model(uint32_t v) const { guest::StoreU32(address + 1940, v); }
  uint32_t needle1_model_address() const { return address + 1940; }
  uint32_t needle2_model() const { return guest::LoadU32(address + 2660); }
  void set_needle2_model(uint32_t v) const { guest::StoreU32(address + 2660, v); }
  uint32_t needle2_model_address() const { return address + 2660; }
  // the dials' +2896 (PoseTachometer)
  uint32_t dials_gauge_record() const { return guest::LoadU32(address + 2896); }
  void set_dials_gauge_record(uint32_t v) const { guest::StoreU32(address + 2896, v); }
  uint32_t dials_gauge_record_address() const { return address + 2896; }
  uint32_t needle3_model() const { return guest::LoadU32(address + 2900); }
  void set_needle3_model(uint32_t v) const { guest::StoreU32(address + 2900, v); }
  uint32_t needle3_model_address() const { return address + 2900; }
  uint32_t needle4_model() const { return guest::LoadU32(address + 3620); }
  void set_needle4_model(uint32_t v) const { guest::StoreU32(address + 3620, v); }
  uint32_t needle4_model_address() const { return address + 3620; }
  uint32_t needle5_model() const { return guest::LoadU32(address + 3860); }
  void set_needle5_model(uint32_t v) const { guest::StoreU32(address + 3860, v); }
  uint32_t needle5_model_address() const { return address + 3860; }
  uint32_t needle6_model() const { return guest::LoadU32(address + 4100); }
  void set_needle6_model(uint32_t v) const { guest::StoreU32(address + 4100, v); }
  uint32_t needle6_model_address() const { return address + 4100; }
  // the gauges' texture providers, stored into the model's +48
  uint32_t gauge0_provider() const { return guest::LoadU32(address + 5296); }
  void set_gauge0_provider(uint32_t v) const { guest::StoreU32(address + 5296, v); }
  uint32_t gauge0_provider_address() const { return address + 5296; }
  uint32_t gauge1_provider() const { return guest::LoadU32(address + 5300); }
  void set_gauge1_provider(uint32_t v) const { guest::StoreU32(address + 5300, v); }
  uint32_t gauge1_provider_address() const { return address + 5300; }
  uint32_t gauge2_provider() const { return guest::LoadU32(address + 5304); }
  void set_gauge2_provider(uint32_t v) const { guest::StoreU32(address + 5304, v); }
  uint32_t gauge2_provider_address() const { return address + 5304; }
  uint32_t gauge3_provider() const { return guest::LoadU32(address + 5308); }
  void set_gauge3_provider(uint32_t v) const { guest::StoreU32(address + 5308, v); }
  uint32_t gauge3_provider_address() const { return address + 5308; }
  uint32_t gauge4_provider() const { return guest::LoadU32(address + 5312); }
  void set_gauge4_provider(uint32_t v) const { guest::StoreU32(address + 5312, v); }
  uint32_t gauge4_provider_address() const { return address + 5312; }
  uint32_t gauge5_provider() const { return guest::LoadU32(address + 5316); }
  void set_gauge5_provider(uint32_t v) const { guest::StoreU32(address + 5316, v); }
  uint32_t gauge5_provider_address() const { return address + 5316; }
  uint32_t gauge6_provider() const { return guest::LoadU32(address + 5320); }
  void set_gauge6_provider(uint32_t v) const { guest::StoreU32(address + 5320, v); }
  uint32_t gauge6_provider_address() const { return address + 5320; }
  // gauge 1's in mode 5
  uint32_t turbo_provider() const { return guest::LoadU32(address + 5324); }
  void set_turbo_provider(uint32_t v) const { guest::StoreU32(address + 5324, v); }
  uint32_t turbo_provider_address() const { return address + 5324; }
  // the needles' angle words (sub_820D3688's first argument)
  uint32_t gauge0_angle() const { return guest::LoadU32(address + 5328); }
  void set_gauge0_angle(uint32_t v) const { guest::StoreU32(address + 5328, v); }
  uint32_t gauge0_angle_address() const { return address + 5328; }
  uint32_t gauge1_angle() const { return guest::LoadU32(address + 5332); }
  void set_gauge1_angle(uint32_t v) const { guest::StoreU32(address + 5332, v); }
  uint32_t gauge1_angle_address() const { return address + 5332; }
  uint32_t gauge2_angle() const { return guest::LoadU32(address + 5336); }
  void set_gauge2_angle(uint32_t v) const { guest::StoreU32(address + 5336, v); }
  uint32_t gauge2_angle_address() const { return address + 5336; }
  uint32_t gauge3_angle() const { return guest::LoadU32(address + 5340); }
  void set_gauge3_angle(uint32_t v) const { guest::StoreU32(address + 5340, v); }
  uint32_t gauge3_angle_address() const { return address + 5340; }
  uint32_t gauge4_angle() const { return guest::LoadU32(address + 5344); }
  void set_gauge4_angle(uint32_t v) const { guest::StoreU32(address + 5344, v); }
  uint32_t gauge4_angle_address() const { return address + 5344; }
  uint32_t gauge5_angle() const { return guest::LoadU32(address + 5348); }
  void set_gauge5_angle(uint32_t v) const { guest::StoreU32(address + 5348, v); }
  uint32_t gauge5_angle_address() const { return address + 5348; }
  uint32_t gauge6_angle() const { return guest::LoadU32(address + 5352); }
  void set_gauge6_angle(uint32_t v) const { guest::StoreU32(address + 5352, v); }
  uint32_t gauge6_angle_address() const { return address + 5352; }
};

// embedded; measure: CockpitPoints::DrawCabinWindow.r4, InteriorDials::DrawDigitalSpeed.r4, InteriorPoints::DrawRace.r4, PartsPoints::DrawBonnet.r4, PartsPoints::DrawCabinWindow01.r4, PartsPoints::DrawCabinWindow02.r4, PartsPoints::DrawFenderLights.r4, PartsPoints::DrawFrontBumperLights.r4, PartsPoints::DrawFrontEnd.r4, PartsPoints::DrawHeadlightBody.r4, PartsPoints::DrawHeadlights.r4, PartsPoints::DrawLeftDoorPanel.r4, PartsPoints::DrawRace.r4, PartsPoints::DrawRearBumperLights.r4, PartsPoints::DrawTailLights.r4, TrafficPoints::DrawHeadlights.r4, WheelPoints::DrawRimAMorphed.r5, WheelPoints::DrawRimB.r5, SceneModel@+4288
struct CarDrawBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 736;
  // +0 to +7 read as a doubleword (the flags in its low word)
  uint64_t head() const { return guest::LoadU64(address + 0); }
  void set_head(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t head_address() const { return address + 0; }
  // 0x1 headlights on, 0x2 and 0x4 the left and right sides, 0x8, 0x10, 0x20 no glows, 0x40
  uint32_t flags() const { return guest::LoadU32(address + 4); }
  void set_flags(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t flags_address() const { return address + 4; }
  // the scene model's +4296: 0 draws nothing
  uint32_t drawn() const { return guest::LoadU32(address + 8); }
  void set_drawn(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t drawn_address() const { return address + 8; }
  // the scene model's +4300 (the interior's body r6)
  uint32_t cockpit_only() const { return guest::LoadU32(address + 12); }
  void set_cockpit_only(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t cockpit_only_address() const { return address + 12; }
  // the scene model's matrix (+4304)
  uint32_t world_address() const { return address + 16; }
  // the wheels' turns R_0 to R_3 and spins S_0 to S_3, 64 bytes each
  uint32_t wheel_r0_address() const { return address + 80; }
  uint32_t wheel_r1_address() const { return address + 144; }
  uint32_t wheel_r2_address() const { return address + 208; }
  uint32_t wheel_r3_address() const { return address + 272; }
  uint32_t wheel_s0_address() const { return address + 336; }
  uint32_t wheel_s1_address() const { return address + 400; }
  uint32_t wheel_s2_address() const { return address + 464; }
  uint32_t wheel_s3_address() const { return address + 528; }
  // set: the rear tires and rims drawn a second time, shifted
  uint32_t rear_second_matrix() const { return guest::LoadU32(address + 592); }
  void set_rear_second_matrix(uint32_t v) const { guest::StoreU32(address + 592, v); }
  uint32_t rear_second_matrix_address() const { return address + 592; }
  // set b's rims: 1 record 2, 2 record 4, else record 0
  uint32_t front_rim_look_a() const { return guest::LoadU32(address + 596); }
  void set_front_rim_look_a(uint32_t v) const { guest::StoreU32(address + 596, v); }
  uint32_t front_rim_look_a_address() const { return address + 596; }
  uint32_t front_rim_look_b() const { return guest::LoadU32(address + 600); }
  void set_front_rim_look_b(uint32_t v) const { guest::StoreU32(address + 600, v); }
  uint32_t front_rim_look_b_address() const { return address + 600; }
  uint32_t rear_rim_look_a() const { return guest::LoadU32(address + 604); }
  void set_rear_rim_look_a(uint32_t v) const { guest::StoreU32(address + 604, v); }
  uint32_t rear_rim_look_a_address() const { return address + 604; }
  uint32_t rear_rim_look_b() const { return guest::LoadU32(address + 608); }
  void set_rear_rim_look_b(uint32_t v) const { guest::StoreU32(address + 608, v); }
  uint32_t rear_rim_look_b_address() const { return address + 608; }
  // record 10's alpha on the brakes
  float front_brake_alpha_a() const { return guest::LoadFloat(address + 612); }
  void set_front_brake_alpha_a(float v) const { guest::StoreFloat(address + 612, v); }
  uint32_t front_brake_alpha_a_address() const { return address + 612; }
  float front_brake_alpha_b() const { return guest::LoadFloat(address + 616); }
  void set_front_brake_alpha_b(float v) const { guest::StoreFloat(address + 616, v); }
  uint32_t front_brake_alpha_b_address() const { return address + 616; }
  float rear_brake_alpha_a() const { return guest::LoadFloat(address + 620); }
  void set_rear_brake_alpha_a(float v) const { guest::StoreFloat(address + 620, v); }
  uint32_t rear_brake_alpha_a_address() const { return address + 620; }
  float rear_brake_alpha_b() const { return guest::LoadFloat(address + 624); }
  void set_rear_brake_alpha_b(float v) const { guest::StoreFloat(address + 624, v); }
  uint32_t rear_brake_alpha_b_address() const { return address + 624; }
  // the door windows 02, the cabin's window 02 and the cockpit's windows
  float window_alpha_02() const { return guest::LoadFloat(address + 628); }
  void set_window_alpha_02(float v) const { guest::StoreFloat(address + 628, v); }
  uint32_t window_alpha_02_address() const { return address + 628; }
  float window_alpha_01() const { return guest::LoadFloat(address + 632); }
  void set_window_alpha_01(float v) const { guest::StoreFloat(address + 632, v); }
  uint32_t window_alpha_01_address() const { return address + 632; }
  // the lamps' covers
  float lights_alpha() const { return guest::LoadFloat(address + 636); }
  void set_lights_alpha(float v) const { guest::StoreFloat(address + 636, v); }
  uint32_t lights_alpha_address() const { return address + 636; }
  // clamped to [0.0, pi]
  float bonnet_angle() const { return guest::LoadFloat(address + 640); }
  void set_bonnet_angle(float v) const { guest::StoreFloat(address + 640, v); }
  uint32_t bonnet_angle_address() const { return address + 640; }
  float left_door_theta() const { return guest::LoadFloat(address + 660); }
  void set_left_door_theta(float v) const { guest::StoreFloat(address + 660, v); }
  uint32_t left_door_theta_address() const { return address + 660; }
  float left_door_phi() const { return guest::LoadFloat(address + 664); }
  void set_left_door_phi(float v) const { guest::StoreFloat(address + 664, v); }
  uint32_t left_door_phi_address() const { return address + 664; }
  float right_door_theta() const { return guest::LoadFloat(address + 676); }
  void set_right_door_theta(float v) const { guest::StoreFloat(address + 676, v); }
  uint32_t right_door_theta_address() const { return address + 676; }
  float right_door_phi() const { return guest::LoadFloat(address + 680); }
  void set_right_door_phi(float v) const { guest::StoreFloat(address + 680, v); }
  uint32_t right_door_phi_address() const { return address + 680; }
  // set: the pop-up's angle from +692
  uint32_t popup_set() const { return guest::LoadU32(address + 688); }
  void set_popup_set(uint32_t v) const { guest::StoreU32(address + 688, v); }
  uint32_t popup_set_address() const { return address + 688; }
  float popup_angle() const { return guest::LoadFloat(address + 692); }
  void set_popup_angle(float v) const { guest::StoreFloat(address + 692, v); }
  uint32_t popup_angle_address() const { return address + 692; }
  // the digital speedometer's value
  float speed() const { return guest::LoadFloat(address + 696); }
  void set_speed(float v) const { guest::StoreFloat(address + 696, v); }
  uint32_t speed_address() const { return address + 696; }
  // the scene model's +4992, lowered by 1.0 a draw (the turbo gauge)
  float boost() const { return guest::LoadFloat(address + 704); }
  void set_boost(float v) const { guest::StoreFloat(address + 704, v); }
  uint32_t boost_address() const { return address + 704; }
  // set: the light kit's world the identity in the front end
  uint32_t identity_world() const { return guest::LoadU32(address + 724); }
  void set_identity_world(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t identity_world_address() const { return address + 724; }
  // 0: the front end's parts take one path (not read in detail)
  uint32_t word_728() const { return guest::LoadU32(address + 728); }
  void set_word_728(uint32_t v) const { guest::StoreU32(address + 728, v); }
  uint32_t word_728_address() const { return address + 728; }
  // set: the wheels' sides not moved by the group's side float4s
  uint32_t no_side_offset() const { return guest::LoadU32(address + 732); }
  void set_no_side_offset(uint32_t v) const { guest::StoreU32(address + 732, v); }
  uint32_t no_side_offset_address() const { return address + 732; }
  // 1.0 (the constructor) (from SceneModelBlock, second-set)
  float world_row0_w() const { return guest::LoadFloat(address + 28); }
  void set_world_row0_w(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t world_row0_w_address() const { return address + 28; }
  // from SceneModelBlock (second-set)
  float world_row1_w() const { return guest::LoadFloat(address + 44); }
  void set_world_row1_w(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t world_row1_w_address() const { return address + 44; }
  // from SceneModelBlock (second-set)
  float world_row2_w() const { return guest::LoadFloat(address + 60); }
  void set_world_row2_w(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t world_row2_w_address() const { return address + 60; }
  // from SceneModelBlock (second-set)
  float world_row3_w() const { return guest::LoadFloat(address + 76); }
  void set_world_row3_w(float v) const { guest::StoreFloat(address + 76, v); }
  uint32_t world_row3_w_address() const { return address + 76; }
  // the wheels' turns R_0 to R_3, 64 bytes each (from SceneModelBlock, second-set)
  uint32_t wheel_turns_address() const { return address + 80; }
  // 1.0 (the constructor; every row's w of the eight matrices) (from SceneModelBlock, second-set)
  float wheel_r0_row0_w() const { return guest::LoadFloat(address + 92); }
  void set_wheel_r0_row0_w(float v) const { guest::StoreFloat(address + 92, v); }
  uint32_t wheel_r0_row0_w_address() const { return address + 92; }
  // from SceneModelBlock (second-set)
  float wheel_r0_row1_w() const { return guest::LoadFloat(address + 108); }
  void set_wheel_r0_row1_w(float v) const { guest::StoreFloat(address + 108, v); }
  uint32_t wheel_r0_row1_w_address() const { return address + 108; }
  // from SceneModelBlock (second-set)
  float wheel_r0_row2_w() const { return guest::LoadFloat(address + 124); }
  void set_wheel_r0_row2_w(float v) const { guest::StoreFloat(address + 124, v); }
  uint32_t wheel_r0_row2_w_address() const { return address + 124; }
  // from SceneModelBlock (second-set)
  float wheel_r0_row3_w() const { return guest::LoadFloat(address + 140); }
  void set_wheel_r0_row3_w(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t wheel_r0_row3_w_address() const { return address + 140; }
  // the wheels' spins S_0 to S_3, 64 bytes each (from SceneModelBlock, second-set)
  uint32_t wheel_spins_address() const { return address + 336; }
  // from SceneModelBlock (second-set)
  float wheel_s0_row0_w() const { return guest::LoadFloat(address + 348); }
  void set_wheel_s0_row0_w(float v) const { guest::StoreFloat(address + 348, v); }
  uint32_t wheel_s0_row0_w_address() const { return address + 348; }
  // from SceneModelBlock (second-set)
  float wheel_s0_row1_w() const { return guest::LoadFloat(address + 364); }
  void set_wheel_s0_row1_w(float v) const { guest::StoreFloat(address + 364, v); }
  uint32_t wheel_s0_row1_w_address() const { return address + 364; }
  // from SceneModelBlock (second-set)
  float wheel_s0_row2_w() const { return guest::LoadFloat(address + 380); }
  void set_wheel_s0_row2_w(float v) const { guest::StoreFloat(address + 380, v); }
  uint32_t wheel_s0_row2_w_address() const { return address + 380; }
  // from SceneModelBlock (second-set)
  float wheel_s0_row3_w() const { return guest::LoadFloat(address + 396); }
  void set_wheel_s0_row3_w(float v) const { guest::StoreFloat(address + 396, v); }
  uint32_t wheel_s0_row3_w_address() const { return address + 396; }
  // four words (2 from Reset): set b's rims' records (from SceneModelBlock, second-set)
  uint32_t rim_looks_address() const { return address + 596; }
  // four floats (0.0 from Reset): record 10's alpha on the brakes (from SceneModelBlock, second-set)
  uint32_t brake_alphas_address() const { return address + 612; }
  // the left door's float4 (0.0 from Reset; w 1.0 from the constructor, then 0.0) (from SceneModelBlock, second-set)
  float left_door_x() const { return guest::LoadFloat(address + 656); }
  void set_left_door_x(float v) const { guest::StoreFloat(address + 656, v); }
  uint32_t left_door_x_address() const { return address + 656; }
  // from SceneModelBlock (second-set)
  float left_door_w() const { return guest::LoadFloat(address + 668); }
  void set_left_door_w(float v) const { guest::StoreFloat(address + 668, v); }
  uint32_t left_door_w_address() const { return address + 668; }
  // the right door's float4 (from SceneModelBlock, second-set)
  float right_door_x() const { return guest::LoadFloat(address + 672); }
  void set_right_door_x(float v) const { guest::StoreFloat(address + 672, v); }
  uint32_t right_door_x_address() const { return address + 672; }
  // from SceneModelBlock (second-set)
  float right_door_w() const { return guest::LoadFloat(address + 684); }
  void set_right_door_w(float v) const { guest::StoreFloat(address + 684, v); }
  uint32_t right_door_w_address() const { return address + 684; }
  // 0.0 from Reset (from SceneModelBlock, second-set)
  float unknown_700() const { return guest::LoadFloat(address + 700); }
  void set_unknown_700(float v) const { guest::StoreFloat(address + 700, v); }
  uint32_t unknown_700_address() const { return address + 700; }
};

// embedded; measure: CarLighting
struct CarLightingRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 688;
  // the gathered lights' words, +4i (set: light i casts a planar shadow)
  uint32_t light0() const { return guest::LoadU32(address + 0); }
  void set_light0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t light0_address() const { return address + 0; }
  uint32_t light1() const { return guest::LoadU32(address + 4); }
  void set_light1(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t light1_address() const { return address + 4; }
  uint32_t light2() const { return guest::LoadU32(address + 8); }
  void set_light2(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t light2_address() const { return address + 8; }
  // +48 + 64i: light i's shadow matrix (their w's 1.0 first)
  uint32_t shadow_matrices_address() const { return address + 48; }
};

// embedded; measure: Matrix::Shadow.r4, Matrix::Shadow.r5
struct ShadowVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(address + 0); }
  void set_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(address + 4); }
  void set_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(address + 8); }
  void set_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // the light's (the point has none)
  float w() const { return guest::LoadFloat(address + 12); }
  void set_w(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// embedded; measure: SceneModel::IsNearPlayerHeight.r4
struct NearHeightPosition : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(address + 0); }
  void set_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  // compared with the player's car's
  float y() const { return guest::LoadFloat(address + 4); }
  void set_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t y_address() const { return address + 4; }
};

// embedded; measure: CarScene@+5248
struct CarSceneAt5248 : View {
  using View::View;
  static constexpr uint32_t kSize = 276;
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +0, the scene's +5248
  uint64_t lighting_0() const { return guest::LoadU64(address + 0); }
  void set_lighting_0(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t lighting_0_address() const { return address + 0; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +8, the scene's +5256
  uint64_t lighting_8() const { return guest::LoadU64(address + 8); }
  void set_lighting_8(uint64_t v) const { guest::StoreU64(address + 8, v); }
  uint32_t lighting_8_address() const { return address + 8; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +16, the scene's +5264
  uint64_t lighting_16() const { return guest::LoadU64(address + 16); }
  void set_lighting_16(uint64_t v) const { guest::StoreU64(address + 16, v); }
  uint32_t lighting_16_address() const { return address + 16; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +24, the scene's +5272
  uint64_t lighting_24() const { return guest::LoadU64(address + 24); }
  void set_lighting_24(uint64_t v) const { guest::StoreU64(address + 24, v); }
  uint32_t lighting_24_address() const { return address + 24; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +32, the scene's +5280
  uint64_t lighting_32() const { return guest::LoadU64(address + 32); }
  void set_lighting_32(uint64_t v) const { guest::StoreU64(address + 32, v); }
  uint32_t lighting_32_address() const { return address + 32; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +40, the scene's +5288
  uint64_t lighting_40() const { return guest::LoadU64(address + 40); }
  void set_lighting_40(uint64_t v) const { guest::StoreU64(address + 40, v); }
  uint32_t lighting_40_address() const { return address + 40; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +48, the scene's +5296
  uint64_t lighting_48() const { return guest::LoadU64(address + 48); }
  void set_lighting_48(uint64_t v) const { guest::StoreU64(address + 48, v); }
  uint32_t lighting_48_address() const { return address + 48; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +56, the scene's +5304
  uint64_t lighting_56() const { return guest::LoadU64(address + 56); }
  void set_lighting_56(uint64_t v) const { guest::StoreU64(address + 56, v); }
  uint32_t lighting_56_address() const { return address + 56; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +64, the scene's +5312
  uint64_t lighting_64() const { return guest::LoadU64(address + 64); }
  void set_lighting_64(uint64_t v) const { guest::StoreU64(address + 64, v); }
  uint32_t lighting_64_address() const { return address + 64; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +72, the scene's +5320
  uint64_t lighting_72() const { return guest::LoadU64(address + 72); }
  void set_lighting_72(uint64_t v) const { guest::StoreU64(address + 72, v); }
  uint32_t lighting_72_address() const { return address + 72; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +80, the scene's +5328
  uint64_t lighting_80() const { return guest::LoadU64(address + 80); }
  void set_lighting_80(uint64_t v) const { guest::StoreU64(address + 80, v); }
  uint32_t lighting_80_address() const { return address + 80; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +88, the scene's +5336
  uint64_t lighting_88() const { return guest::LoadU64(address + 88); }
  void set_lighting_88(uint64_t v) const { guest::StoreU64(address + 88, v); }
  uint32_t lighting_88_address() const { return address + 88; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +96, the scene's +5344
  uint64_t lighting_96() const { return guest::LoadU64(address + 96); }
  void set_lighting_96(uint64_t v) const { guest::StoreU64(address + 96, v); }
  uint32_t lighting_96_address() const { return address + 96; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +104, the scene's +5352
  uint64_t lighting_104() const { return guest::LoadU64(address + 104); }
  void set_lighting_104(uint64_t v) const { guest::StoreU64(address + 104, v); }
  uint32_t lighting_104_address() const { return address + 104; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +112, the scene's +5360
  uint64_t lighting_112() const { return guest::LoadU64(address + 112); }
  void set_lighting_112(uint64_t v) const { guest::StoreU64(address + 112, v); }
  uint32_t lighting_112_address() const { return address + 112; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +120, the scene's +5368
  uint64_t lighting_120() const { return guest::LoadU64(address + 120); }
  void set_lighting_120(uint64_t v) const { guest::StoreU64(address + 120, v); }
  uint32_t lighting_120_address() const { return address + 120; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +128, the scene's +5376
  uint64_t lighting_128() const { return guest::LoadU64(address + 128); }
  void set_lighting_128(uint64_t v) const { guest::StoreU64(address + 128, v); }
  uint32_t lighting_128_address() const { return address + 128; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +136, the scene's +5384
  uint64_t lighting_136() const { return guest::LoadU64(address + 136); }
  void set_lighting_136(uint64_t v) const { guest::StoreU64(address + 136, v); }
  uint32_t lighting_136_address() const { return address + 136; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +144, the scene's +5392
  uint64_t lighting_144() const { return guest::LoadU64(address + 144); }
  void set_lighting_144(uint64_t v) const { guest::StoreU64(address + 144, v); }
  uint32_t lighting_144_address() const { return address + 144; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +152, the scene's +5400
  uint64_t lighting_152() const { return guest::LoadU64(address + 152); }
  void set_lighting_152(uint64_t v) const { guest::StoreU64(address + 152, v); }
  uint32_t lighting_152_address() const { return address + 152; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +160, the scene's +5408
  uint64_t lighting_160() const { return guest::LoadU64(address + 160); }
  void set_lighting_160(uint64_t v) const { guest::StoreU64(address + 160, v); }
  uint32_t lighting_160_address() const { return address + 160; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +168, the scene's +5416
  uint64_t lighting_168() const { return guest::LoadU64(address + 168); }
  void set_lighting_168(uint64_t v) const { guest::StoreU64(address + 168, v); }
  uint32_t lighting_168_address() const { return address + 168; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +176, the scene's +5424
  uint64_t lighting_176() const { return guest::LoadU64(address + 176); }
  void set_lighting_176(uint64_t v) const { guest::StoreU64(address + 176, v); }
  uint32_t lighting_176_address() const { return address + 176; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +184, the scene's +5432
  uint64_t lighting_184() const { return guest::LoadU64(address + 184); }
  void set_lighting_184(uint64_t v) const { guest::StoreU64(address + 184, v); }
  uint32_t lighting_184_address() const { return address + 184; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +192, the scene's +5440
  uint64_t lighting_192() const { return guest::LoadU64(address + 192); }
  void set_lighting_192(uint64_t v) const { guest::StoreU64(address + 192, v); }
  uint32_t lighting_192_address() const { return address + 192; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +200, the scene's +5448
  uint64_t lighting_200() const { return guest::LoadU64(address + 200); }
  void set_lighting_200(uint64_t v) const { guest::StoreU64(address + 200, v); }
  uint32_t lighting_200_address() const { return address + 200; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +208, the scene's +5456
  uint64_t lighting_208() const { return guest::LoadU64(address + 208); }
  void set_lighting_208(uint64_t v) const { guest::StoreU64(address + 208, v); }
  uint32_t lighting_208_address() const { return address + 208; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +216, the scene's +5464
  uint64_t lighting_216() const { return guest::LoadU64(address + 216); }
  void set_lighting_216(uint64_t v) const { guest::StoreU64(address + 216, v); }
  uint32_t lighting_216_address() const { return address + 216; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +224, the scene's +5472
  uint64_t lighting_224() const { return guest::LoadU64(address + 224); }
  void set_lighting_224(uint64_t v) const { guest::StoreU64(address + 224, v); }
  uint32_t lighting_224_address() const { return address + 224; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +232, the scene's +5480
  uint64_t lighting_232() const { return guest::LoadU64(address + 232); }
  void set_lighting_232(uint64_t v) const { guest::StoreU64(address + 232, v); }
  uint32_t lighting_232_address() const { return address + 232; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +240, the scene's +5488
  uint64_t lighting_240() const { return guest::LoadU64(address + 240); }
  void set_lighting_240(uint64_t v) const { guest::StoreU64(address + 240, v); }
  uint32_t lighting_240_address() const { return address + 240; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +248, the scene's +5496
  uint64_t lighting_248() const { return guest::LoadU64(address + 248); }
  void set_lighting_248(uint64_t v) const { guest::StoreU64(address + 248, v); }
  uint32_t lighting_248_address() const { return address + 248; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +256, the scene's +5504
  uint64_t lighting_256() const { return guest::LoadU64(address + 256); }
  void set_lighting_256(uint64_t v) const { guest::StoreU64(address + 256, v); }
  uint32_t lighting_256_address() const { return address + 256; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): CarScene::InitLighting's r3 is the scene + 5248 (the lighting block, car_light_factor_a on), joined with the scene at +0; this is the block's +264, the scene's +5512
  uint64_t lighting_264() const { return guest::LoadU64(address + 264); }
  void set_lighting_264(uint64_t v) const { guest::StoreU64(address + 264, v); }
  uint32_t lighting_264_address() const { return address + 264; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): InitLighting's last float, the scene's +5520 (lighting_end)
  float lighting_272() const { return guest::LoadFloat(address + 272); }
  void set_lighting_272(float v) const { guest::StoreFloat(address + 272, v); }
  uint32_t lighting_272_address() const { return address + 272; }
};

// embedded; measure: PartsPoints::DrawRace.ret@+176
struct PartsPointsAt176 : View {
  using View::View;
  static constexpr uint32_t kSize = 37084;
  uint32_t unknown_37076() const { return guest::LoadU32(address + 37076); }
  void set_unknown_37076(uint32_t v) const { guest::StoreU32(address + 37076, v); }
  uint32_t unknown_37076_address() const { return address + 37076; }
  uint32_t unknown_37080() const { return guest::LoadU32(address + 37080); }
  void set_unknown_37080(uint32_t v) const { guest::StoreU32(address + 37080, v); }
  uint32_t unknown_37080_address() const { return address + 37080; }
};

// embedded; measure: PartsPoints::DrawRace.ret@+176@+12128
struct PartsPointsAt12128 : View {
  using View::View;
  static constexpr uint32_t kSize = 6088;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_6080() const { return guest::LoadU32(address + 6080); }
  void set_unknown_6080(uint32_t v) const { guest::StoreU32(address + 6080, v); }
  uint32_t unknown_6080_address() const { return address + 6080; }
  uint32_t unknown_6084() const { return guest::LoadU32(address + 6084); }
  void set_unknown_6084(uint32_t v) const { guest::StoreU32(address + 6084, v); }
  uint32_t unknown_6084_address() const { return address + 6084; }
};

// embedded; measure: PartsPoints::DrawRace.ret@+176@+6032
struct PartsPointsAt6032 : View {
  using View::View;
  static constexpr uint32_t kSize = 6088;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_6080() const { return guest::LoadU32(address + 6080); }
  void set_unknown_6080(uint32_t v) const { guest::StoreU32(address + 6080, v); }
  uint32_t unknown_6080_address() const { return address + 6080; }
  uint32_t unknown_6084() const { return guest::LoadU32(address + 6084); }
  void set_unknown_6084(uint32_t v) const { guest::StoreU32(address + 6084, v); }
  uint32_t unknown_6084_address() const { return address + 6084; }
};

// embedded; measure: PartsPoints::PartsPoints.r3@+34640
struct PartsPointsAt34640 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  float unknown_12() const { return guest::LoadFloat(address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_28() const { return guest::LoadFloat(address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  float unknown_44() const { return guest::LoadFloat(address + 44); }
  void set_unknown_44(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  float unknown_60() const { return guest::LoadFloat(address + 60); }
  void set_unknown_60(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
};

// embedded; measure: SceneParts::Draw.r3@+176
struct PartsPointsAt176_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 37084;
  uint32_t unknown_37080() const { return guest::LoadU32(address + 37080); }
  void set_unknown_37080(uint32_t v) const { guest::StoreU32(address + 37080, v); }
  uint32_t unknown_37080_address() const { return address + 37080; }
};

// embedded; measure: SceneParts::Draw.r3@+176@+18224
struct PartsPointsAt18224 : View {
  using View::View;
  static constexpr uint32_t kSize = 5524;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_228() const { return guest::LoadU32(address + 228); }
  void set_unknown_228(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  uint32_t unknown_244() const { return guest::LoadU32(address + 244); }
  void set_unknown_244(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  uint32_t unknown_468() const { return guest::LoadU32(address + 468); }
  void set_unknown_468(uint32_t v) const { guest::StoreU32(address + 468, v); }
  uint32_t unknown_468_address() const { return address + 468; }
  uint32_t unknown_5520() const { return guest::LoadU32(address + 5520); }
  void set_unknown_5520(uint32_t v) const { guest::StoreU32(address + 5520, v); }
  uint32_t unknown_5520_address() const { return address + 5520; }
};

// embedded; measure: SceneParts::Draw.r3@+176@+36096
struct PartsPointsAt36096 : View {
  using View::View;
  static constexpr uint32_t kSize = 952;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_228() const { return guest::LoadU32(address + 228); }
  void set_unknown_228(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  uint32_t unknown_724() const { return guest::LoadU32(address + 724); }
  void set_unknown_724(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t unknown_724_address() const { return address + 724; }
  uint32_t unknown_948() const { return guest::LoadU32(address + 948); }
  void set_unknown_948(uint32_t v) const { guest::StoreU32(address + 948, v); }
  uint32_t unknown_948_address() const { return address + 948; }
};

// embedded; measure: SceneParts@+176
struct PartsPointsAt176_3 : View {
  using View::View;
  static constexpr uint32_t kSize = 37092;
  uint32_t unknown_724() const { return guest::LoadU32(address + 724); }
  void set_unknown_724(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t unknown_724_address() const { return address + 724; }
  uint32_t unknown_948() const { return guest::LoadU32(address + 948); }
  void set_unknown_948(uint32_t v) const { guest::StoreU32(address + 948, v); }
  uint32_t unknown_948_address() const { return address + 948; }
  uint32_t unknown_964() const { return guest::LoadU32(address + 964); }
  void set_unknown_964(uint32_t v) const { guest::StoreU32(address + 964, v); }
  uint32_t unknown_964_address() const { return address + 964; }
  uint32_t unknown_1188() const { return guest::LoadU32(address + 1188); }
  void set_unknown_1188(uint32_t v) const { guest::StoreU32(address + 1188, v); }
  uint32_t unknown_1188_address() const { return address + 1188; }
  uint32_t unknown_1204() const { return guest::LoadU32(address + 1204); }
  void set_unknown_1204(uint32_t v) const { guest::StoreU32(address + 1204, v); }
  uint32_t unknown_1204_address() const { return address + 1204; }
  uint32_t unknown_1428() const { return guest::LoadU32(address + 1428); }
  void set_unknown_1428(uint32_t v) const { guest::StoreU32(address + 1428, v); }
  uint32_t unknown_1428_address() const { return address + 1428; }
  uint32_t unknown_1924() const { return guest::LoadU32(address + 1924); }
  void set_unknown_1924(uint32_t v) const { guest::StoreU32(address + 1924, v); }
  uint32_t unknown_1924_address() const { return address + 1924; }
  uint32_t unknown_2644() const { return guest::LoadU32(address + 2644); }
  void set_unknown_2644(uint32_t v) const { guest::StoreU32(address + 2644, v); }
  uint32_t unknown_2644_address() const { return address + 2644; }
  uint32_t unknown_2648() const { return guest::LoadU32(address + 2648); }
  void set_unknown_2648(uint32_t v) const { guest::StoreU32(address + 2648, v); }
  uint32_t unknown_2648_address() const { return address + 2648; }
  uint32_t unknown_4580() const { return guest::LoadU32(address + 4580); }
  void set_unknown_4580(uint32_t v) const { guest::StoreU32(address + 4580, v); }
  uint32_t unknown_4580_address() const { return address + 4580; }
  uint32_t unknown_12032() const { return guest::LoadU32(address + 12032); }
  void set_unknown_12032(uint32_t v) const { guest::StoreU32(address + 12032, v); }
  uint32_t unknown_12032_address() const { return address + 12032; }
  uint32_t unknown_12112() const { return guest::LoadU32(address + 12112); }
  void set_unknown_12112(uint32_t v) const { guest::StoreU32(address + 12112, v); }
  uint32_t unknown_12112_address() const { return address + 12112; }
  uint32_t unknown_12116() const { return guest::LoadU32(address + 12116); }
  void set_unknown_12116(uint32_t v) const { guest::StoreU32(address + 12116, v); }
  uint32_t unknown_12116_address() const { return address + 12116; }
  uint32_t unknown_18128() const { return guest::LoadU32(address + 18128); }
  void set_unknown_18128(uint32_t v) const { guest::StoreU32(address + 18128, v); }
  uint32_t unknown_18128_address() const { return address + 18128; }
  uint32_t unknown_18208() const { return guest::LoadU32(address + 18208); }
  void set_unknown_18208(uint32_t v) const { guest::StoreU32(address + 18208, v); }
  uint32_t unknown_18208_address() const { return address + 18208; }
  uint32_t unknown_18212() const { return guest::LoadU32(address + 18212); }
  void set_unknown_18212(uint32_t v) const { guest::StoreU32(address + 18212, v); }
  uint32_t unknown_18212_address() const { return address + 18212; }
  uint32_t unknown_23744() const { return guest::LoadU32(address + 23744); }
  void set_unknown_23744(uint32_t v) const { guest::StoreU32(address + 23744, v); }
  uint32_t unknown_23744_address() const { return address + 23744; }
  uint32_t unknown_37088() const { return guest::LoadU32(address + 37088); }
  void set_unknown_37088(uint32_t v) const { guest::StoreU32(address + 37088, v); }
  uint32_t unknown_37088_address() const { return address + 37088; }
};

// embedded; measure: SceneParts@+176@+18224
struct PartsPointsAt18224_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 952;
  uint32_t unknown_724() const { return guest::LoadU32(address + 724); }
  void set_unknown_724(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t unknown_724_address() const { return address + 724; }
  uint32_t unknown_948() const { return guest::LoadU32(address + 948); }
  void set_unknown_948(uint32_t v) const { guest::StoreU32(address + 948, v); }
  uint32_t unknown_948_address() const { return address + 948; }
};

// embedded; measure: SceneParts@+176@+24480
struct PartsPointsAt24480 : View {
  using View::View;
  static constexpr uint32_t kSize = 3188;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_224() const { return guest::LoadU32(address + 224); }
  void set_unknown_224(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  uint32_t unknown_1684() const { return guest::LoadU32(address + 1684); }
  void set_unknown_1684(uint32_t v) const { guest::StoreU32(address + 1684, v); }
  uint32_t unknown_1684_address() const { return address + 1684; }
  uint32_t unknown_1908() const { return guest::LoadU32(address + 1908); }
  void set_unknown_1908(uint32_t v) const { guest::StoreU32(address + 1908, v); }
  uint32_t unknown_1908_address() const { return address + 1908; }
  uint32_t unknown_1924() const { return guest::LoadU32(address + 1924); }
  void set_unknown_1924(uint32_t v) const { guest::StoreU32(address + 1924, v); }
  uint32_t unknown_1924_address() const { return address + 1924; }
  uint32_t unknown_2148() const { return guest::LoadU32(address + 2148); }
  void set_unknown_2148(uint32_t v) const { guest::StoreU32(address + 2148, v); }
  uint32_t unknown_2148_address() const { return address + 2148; }
  uint32_t unknown_2164() const { return guest::LoadU32(address + 2164); }
  void set_unknown_2164(uint32_t v) const { guest::StoreU32(address + 2164, v); }
  uint32_t unknown_2164_address() const { return address + 2164; }
  uint32_t unknown_2388() const { return guest::LoadU32(address + 2388); }
  void set_unknown_2388(uint32_t v) const { guest::StoreU32(address + 2388, v); }
  uint32_t unknown_2388_address() const { return address + 2388; }
  uint32_t unknown_2404() const { return guest::LoadU32(address + 2404); }
  void set_unknown_2404(uint32_t v) const { guest::StoreU32(address + 2404, v); }
  uint32_t unknown_2404_address() const { return address + 2404; }
  uint32_t unknown_2628() const { return guest::LoadU32(address + 2628); }
  void set_unknown_2628(uint32_t v) const { guest::StoreU32(address + 2628, v); }
  uint32_t unknown_2628_address() const { return address + 2628; }
  float unknown_3184() const { return guest::LoadFloat(address + 3184); }
  void set_unknown_3184(float v) const { guest::StoreFloat(address + 3184, v); }
  uint32_t unknown_3184_address() const { return address + 3184; }
};

// embedded; measure: SceneParts@+176@+27680
struct PartsPointsAt27680 : View {
  using View::View;
  static constexpr uint32_t kSize = 2152;
  uint32_t unknown_1924() const { return guest::LoadU32(address + 1924); }
  void set_unknown_1924(uint32_t v) const { guest::StoreU32(address + 1924, v); }
  uint32_t unknown_1924_address() const { return address + 1924; }
  uint32_t unknown_2148() const { return guest::LoadU32(address + 2148); }
  void set_unknown_2148(uint32_t v) const { guest::StoreU32(address + 2148, v); }
  uint32_t unknown_2148_address() const { return address + 2148; }
};

// embedded; measure: SceneParts@+176@+2896
struct PartsPointsAt2896 : View {
  using View::View;
  static constexpr uint32_t kSize = 1688;
  uint32_t unknown_244() const { return guest::LoadU32(address + 244); }
  void set_unknown_244(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  uint32_t unknown_468() const { return guest::LoadU32(address + 468); }
  void set_unknown_468(uint32_t v) const { guest::StoreU32(address + 468, v); }
  uint32_t unknown_468_address() const { return address + 468; }
  uint32_t unknown_484() const { return guest::LoadU32(address + 484); }
  void set_unknown_484(uint32_t v) const { guest::StoreU32(address + 484, v); }
  uint32_t unknown_484_address() const { return address + 484; }
  uint32_t unknown_708() const { return guest::LoadU32(address + 708); }
  void set_unknown_708(uint32_t v) const { guest::StoreU32(address + 708, v); }
  uint32_t unknown_708_address() const { return address + 708; }
  uint32_t unknown_724() const { return guest::LoadU32(address + 724); }
  void set_unknown_724(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t unknown_724_address() const { return address + 724; }
  uint32_t unknown_948() const { return guest::LoadU32(address + 948); }
  void set_unknown_948(uint32_t v) const { guest::StoreU32(address + 948, v); }
  uint32_t unknown_948_address() const { return address + 948; }
  uint32_t unknown_964() const { return guest::LoadU32(address + 964); }
  void set_unknown_964(uint32_t v) const { guest::StoreU32(address + 964, v); }
  uint32_t unknown_964_address() const { return address + 964; }
  uint32_t unknown_1188() const { return guest::LoadU32(address + 1188); }
  void set_unknown_1188(uint32_t v) const { guest::StoreU32(address + 1188, v); }
  uint32_t unknown_1188_address() const { return address + 1188; }
  uint32_t unknown_1444() const { return guest::LoadU32(address + 1444); }
  void set_unknown_1444(uint32_t v) const { guest::StoreU32(address + 1444, v); }
  uint32_t unknown_1444_address() const { return address + 1444; }
  uint32_t unknown_1684() const { return guest::LoadU32(address + 1684); }
  void set_unknown_1684(uint32_t v) const { guest::StoreU32(address + 1684, v); }
  uint32_t unknown_1684_address() const { return address + 1684; }
};

// embedded; measure: SceneParts@+176@+31280
struct PartsPointsAt31280 : View {
  using View::View;
  static constexpr uint32_t kSize = 952;
  uint32_t unknown_724() const { return guest::LoadU32(address + 724); }
  void set_unknown_724(uint32_t v) const { guest::StoreU32(address + 724, v); }
  uint32_t unknown_724_address() const { return address + 724; }
  uint32_t unknown_948() const { return guest::LoadU32(address + 948); }
  void set_unknown_948(uint32_t v) const { guest::StoreU32(address + 948, v); }
  uint32_t unknown_948_address() const { return address + 948; }
};

// embedded; measure: SceneParts@+176@+32960
struct PartsPointsAt32960 : View {
  using View::View;
  static constexpr uint32_t kSize = 472;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_228() const { return guest::LoadU32(address + 228); }
  void set_unknown_228(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  uint32_t unknown_244() const { return guest::LoadU32(address + 244); }
  void set_unknown_244(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  uint32_t unknown_468() const { return guest::LoadU32(address + 468); }
  void set_unknown_468(uint32_t v) const { guest::StoreU32(address + 468, v); }
  uint32_t unknown_468_address() const { return address + 468; }
};

// embedded; measure: SceneParts@+176@+4832
struct PartsPointsAt4832 : View {
  using View::View;
  static constexpr uint32_t kSize = 712;
  uint32_t unknown_244() const { return guest::LoadU32(address + 244); }
  void set_unknown_244(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  uint32_t unknown_468() const { return guest::LoadU32(address + 468); }
  void set_unknown_468(uint32_t v) const { guest::StoreU32(address + 468, v); }
  uint32_t unknown_468_address() const { return address + 468; }
  uint32_t unknown_484() const { return guest::LoadU32(address + 484); }
  void set_unknown_484(uint32_t v) const { guest::StoreU32(address + 484, v); }
  uint32_t unknown_484_address() const { return address + 484; }
  uint32_t unknown_708() const { return guest::LoadU32(address + 708); }
  void set_unknown_708(uint32_t v) const { guest::StoreU32(address + 708, v); }
  uint32_t unknown_708_address() const { return address + 708; }
};

// embedded; measure: SceneParts@+176@+6032
struct PartsPointsAt6032_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 1912;
  uint32_t unknown_1684() const { return guest::LoadU32(address + 1684); }
  void set_unknown_1684(uint32_t v) const { guest::StoreU32(address + 1684, v); }
  uint32_t unknown_1684_address() const { return address + 1684; }
  uint32_t unknown_1908() const { return guest::LoadU32(address + 1908); }
  void set_unknown_1908(uint32_t v) const { guest::StoreU32(address + 1908, v); }
  uint32_t unknown_1908_address() const { return address + 1908; }
};

// The image words docs/functions/car-draw.md's code uses.
struct CarDrawGlobals {
  // the car scene
  template <typename T = CarScene>
  static T scene() { return T(guest::LoadU32(0x826C2E44u)); }
  static void set_scene(const View& v) { guest::StoreU32(0x826C2E44u, v.address); }
  static void set_scene(uint32_t v) { guest::StoreU32(0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // flipped around the light kit's and the glows' draws
  static uint32_t lights_flip() { return guest::LoadU32(0x826C5104u); }
  static void set_lights_flip(uint32_t v) { guest::StoreU32(0x826C5104u, v); }
  static constexpr uint32_t lights_flip_address() { return 0x826C5104u; }
  // set: the number plates drawn blank
  static uint32_t plate_blank() { return guest::LoadU32(0x82816E3Cu); }
  static void set_plate_blank(uint32_t v) { guest::StoreU32(0x82816E3Cu, v); }
  static constexpr uint32_t plate_blank_address() { return 0x82816E3Cu; }
  // the render queue's sort keys: Model::Draw's group 1 bucket
  static uint32_t key_group1() { return guest::LoadU32(0x8296B900u); }
  static void set_key_group1(uint32_t v) { guest::StoreU32(0x8296B900u, v); }
  static constexpr uint32_t key_group1_address() { return 0x8296B900u; }
  // Model::Draw's depth bucket
  static uint32_t key_depth() { return guest::LoadU32(0x8296B908u); }
  static void set_key_depth(uint32_t v) { guest::StoreU32(0x8296B908u, v); }
  static constexpr uint32_t key_depth_address() { return 0x8296B908u; }
  // Model::Draw's group 0 bucket
  static uint32_t key_group0() { return guest::LoadU32(0x8296B90Cu); }
  static void set_key_group0(uint32_t v) { guest::StoreU32(0x8296B90Cu, v); }
  static constexpr uint32_t key_group0_address() { return 0x8296B90Cu; }
  // the depth draws' key
  static uint32_t depth_key() { return guest::LoadU32(0x8296B8FCu); }
  static void set_depth_key(uint32_t v) { guest::StoreU32(0x8296B8FCu, v); }
  static constexpr uint32_t depth_key_address() { return 0x8296B8FCu; }
  // the depth draws' second key
  static uint32_t other_key() { return guest::LoadU32(0x8296B8F8u); }
  static void set_other_key(uint32_t v) { guest::StoreU32(0x8296B8F8u, v); }
  static constexpr uint32_t other_key_address() { return 0x8296B8F8u; }
  // the second pool maker (its slot 1 makes the morph's job)
  static uint32_t pool_maker() { return guest::LoadU32(0x8296B914u); }
  static void set_pool_maker(uint32_t v) { guest::StoreU32(0x8296B914u, v); }
  static constexpr uint32_t pool_maker_address() { return 0x8296B914u; }
};

}  // namespace schema
