// The views of docs/functions/car-makers.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-makers.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: SceneModelRecord
struct SceneModelRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 4244;
  // 1 by default; from a car: 2 the players' cars (flags 0x81), 1 the rivals (0x2), 3 traffic
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // the car record's +0 (the car record, 3,696 bytes, is embedded at +16): the model block, CarModels::BlockOf of the car id
  uint32_t block() const { return guest::LoadU32(base, address + 16); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t block_address() const { return address + 16; }
  // a hue, saturation, value triple (SetPaint)
  float paint0_hue() const { return guest::LoadFloat(base, address + 32); }
  void set_paint0_hue(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t paint0_hue_address() const { return address + 32; }
  float paint0_saturation() const { return guest::LoadFloat(base, address + 36); }
  void set_paint0_saturation(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t paint0_saturation_address() const { return address + 36; }
  float paint0_value() const { return guest::LoadFloat(base, address + 40); }
  void set_paint0_value(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t paint0_value_address() const { return address + 40; }
  float paint1_hue() const { return guest::LoadFloat(base, address + 48); }
  void set_paint1_hue(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t paint1_hue_address() const { return address + 48; }
  float paint1_saturation() const { return guest::LoadFloat(base, address + 52); }
  void set_paint1_saturation(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t paint1_saturation_address() const { return address + 52; }
  float paint1_value() const { return guest::LoadFloat(base, address + 56); }
  void set_paint1_value(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t paint1_value_address() const { return address + 56; }
  float paint2_hue() const { return guest::LoadFloat(base, address + 64); }
  void set_paint2_hue(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t paint2_hue_address() const { return address + 64; }
  float paint2_saturation() const { return guest::LoadFloat(base, address + 68); }
  void set_paint2_saturation(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t paint2_saturation_address() const { return address + 68; }
  float paint2_value() const { return guest::LoadFloat(base, address + 72); }
  void set_paint2_value(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t paint2_value_address() const { return address + 72; }
  // the front wheels' colors (SetPaint, GarageStage::SetWheel)
  float front_wheel_paint0_hue() const { return guest::LoadFloat(base, address + 80); }
  void set_front_wheel_paint0_hue(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t front_wheel_paint0_hue_address() const { return address + 80; }
  float front_wheel_paint0_saturation() const { return guest::LoadFloat(base, address + 84); }
  void set_front_wheel_paint0_saturation(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t front_wheel_paint0_saturation_address() const { return address + 84; }
  float front_wheel_paint0_value() const { return guest::LoadFloat(base, address + 88); }
  void set_front_wheel_paint0_value(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t front_wheel_paint0_value_address() const { return address + 88; }
  float front_wheel_paint1_hue() const { return guest::LoadFloat(base, address + 96); }
  void set_front_wheel_paint1_hue(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t front_wheel_paint1_hue_address() const { return address + 96; }
  float front_wheel_paint1_saturation() const { return guest::LoadFloat(base, address + 100); }
  void set_front_wheel_paint1_saturation(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t front_wheel_paint1_saturation_address() const { return address + 100; }
  float front_wheel_paint1_value() const { return guest::LoadFloat(base, address + 104); }
  void set_front_wheel_paint1_value(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t front_wheel_paint1_value_address() const { return address + 104; }
  float front_wheel_paint2_hue() const { return guest::LoadFloat(base, address + 112); }
  void set_front_wheel_paint2_hue(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t front_wheel_paint2_hue_address() const { return address + 112; }
  float front_wheel_paint2_saturation() const { return guest::LoadFloat(base, address + 116); }
  void set_front_wheel_paint2_saturation(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t front_wheel_paint2_saturation_address() const { return address + 116; }
  float front_wheel_paint2_value() const { return guest::LoadFloat(base, address + 120); }
  void set_front_wheel_paint2_value(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t front_wheel_paint2_value_address() const { return address + 120; }
  // the rear wheels' colors (SetPaint, GarageStage::SetWheel)
  float rear_wheel_paint0_hue() const { return guest::LoadFloat(base, address + 128); }
  void set_rear_wheel_paint0_hue(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t rear_wheel_paint0_hue_address() const { return address + 128; }
  float rear_wheel_paint0_saturation() const { return guest::LoadFloat(base, address + 132); }
  void set_rear_wheel_paint0_saturation(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t rear_wheel_paint0_saturation_address() const { return address + 132; }
  float rear_wheel_paint0_value() const { return guest::LoadFloat(base, address + 136); }
  void set_rear_wheel_paint0_value(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t rear_wheel_paint0_value_address() const { return address + 136; }
  float rear_wheel_paint1_hue() const { return guest::LoadFloat(base, address + 144); }
  void set_rear_wheel_paint1_hue(float v) const { guest::StoreFloat(base, address + 144, v); }
  uint32_t rear_wheel_paint1_hue_address() const { return address + 144; }
  float rear_wheel_paint1_saturation() const { return guest::LoadFloat(base, address + 148); }
  void set_rear_wheel_paint1_saturation(float v) const { guest::StoreFloat(base, address + 148, v); }
  uint32_t rear_wheel_paint1_saturation_address() const { return address + 148; }
  float rear_wheel_paint1_value() const { return guest::LoadFloat(base, address + 152); }
  void set_rear_wheel_paint1_value(float v) const { guest::StoreFloat(base, address + 152, v); }
  uint32_t rear_wheel_paint1_value_address() const { return address + 152; }
  float rear_wheel_paint2_hue() const { return guest::LoadFloat(base, address + 160); }
  void set_rear_wheel_paint2_hue(float v) const { guest::StoreFloat(base, address + 160, v); }
  uint32_t rear_wheel_paint2_hue_address() const { return address + 160; }
  float rear_wheel_paint2_saturation() const { return guest::LoadFloat(base, address + 164); }
  void set_rear_wheel_paint2_saturation(float v) const { guest::StoreFloat(base, address + 164, v); }
  uint32_t rear_wheel_paint2_saturation_address() const { return address + 164; }
  float rear_wheel_paint2_value() const { return guest::LoadFloat(base, address + 168); }
  void set_rear_wheel_paint2_value(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t rear_wheel_paint2_value_address() const { return address + 168; }
  // traffic (kind 3): the paint pick
  uint32_t traffic_paint() const { return guest::LoadU32(base, address + 176); }
  void set_traffic_paint(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t traffic_paint_address() const { return address + 176; }
  // 64-byte entries (CarMakersRecordDecal): +192 three and +384 three (the car's six at +96), then eight groups of six slots from +576, five used (the car's forty at +288)
  uint32_t decals_address() const { return address + 192; }
  // the car record's +3632: twelve words (CarRecord::ResetBytes, SetBytes from the car's bytes +1568)
  uint32_t car_record_bytes_address() const { return address + 3648; }
  // the car's +1812 (kind 2)
  uint32_t interior_item3() const { return guest::LoadU32(base, address + 3696); }
  void set_interior_item3(uint32_t v) const { guest::StoreU32(base, address + 3696, v); }
  uint32_t interior_item3_address() const { return address + 3696; }
  // the car's +1792
  uint32_t exterior_item1() const { return guest::LoadU32(base, address + 3704); }
  void set_exterior_item1(uint32_t v) const { guest::StoreU32(base, address + 3704, v); }
  uint32_t exterior_item1_address() const { return address + 3704; }
  // 1 when the car's bonnet color kind (+1764) is 1
  uint32_t bonnet_painted() const { return guest::LoadU32(base, address + 3708); }
  void set_bonnet_painted(uint32_t v) const { guest::StoreU32(base, address + 3708, v); }
  uint32_t bonnet_painted_address() const { return address + 3708; }
  // the parts' 60 bytes start here: the car's +1744, or the aero kit's entry +0
  uint32_t parts_aero0() const { return guest::LoadU32(base, address + 3712); }
  void set_parts_aero0(uint32_t v) const { guest::StoreU32(base, address + 3712, v); }
  uint32_t parts_aero0_address() const { return address + 3712; }
  // the car's +1664
  uint32_t parts_power() const { return guest::LoadU32(base, address + 3716); }
  void set_parts_power(uint32_t v) const { guest::StoreU32(base, address + 3716, v); }
  uint32_t parts_power_address() const { return address + 3716; }
  // the car's +1752, or the kit's +4
  uint32_t parts_aero2() const { return guest::LoadU32(base, address + 3720); }
  void set_parts_aero2(uint32_t v) const { guest::StoreU32(base, address + 3720, v); }
  uint32_t parts_aero2_address() const { return address + 3720; }
  // the car's +1748, or the kit's +8
  uint32_t parts_aero1() const { return guest::LoadU32(base, address + 3724); }
  void set_parts_aero1(uint32_t v) const { guest::StoreU32(base, address + 3724, v); }
  uint32_t parts_aero1_address() const { return address + 3724; }
  // the car's +1756 (-1 gives 13), or the kit's +20
  uint32_t parts_aero3() const { return guest::LoadU32(base, address + 3728); }
  void set_parts_aero3(uint32_t v) const { guest::StoreU32(base, address + 3728, v); }
  uint32_t parts_aero3_address() const { return address + 3728; }
  // the car's +1760, or the kit's +16
  uint32_t parts_bonnet() const { return guest::LoadU32(base, address + 3732); }
  void set_parts_bonnet(uint32_t v) const { guest::StoreU32(base, address + 3732, v); }
  uint32_t parts_bonnet_address() const { return address + 3732; }
  // the car's aero kit (+1784) when the kit's flag 0x8, else 0
  uint32_t parts_aero_kit() const { return guest::LoadU32(base, address + 3736); }
  void set_parts_aero_kit(uint32_t v) const { guest::StoreU32(base, address + 3736, v); }
  uint32_t parts_aero_kit_address() const { return address + 3736; }
  // the car's +1768
  uint32_t parts_aero6() const { return guest::LoadU32(base, address + 3740); }
  void set_parts_aero6(uint32_t v) const { guest::StoreU32(base, address + 3740, v); }
  uint32_t parts_aero6_address() const { return address + 3740; }
  // the car's +1772
  uint32_t parts_aero7() const { return guest::LoadU32(base, address + 3744); }
  void set_parts_aero7(uint32_t v) const { guest::StoreU32(base, address + 3744, v); }
  uint32_t parts_aero7_address() const { return address + 3744; }
  // the car's +1776
  uint32_t parts_aero8() const { return guest::LoadU32(base, address + 3748); }
  void set_parts_aero8(uint32_t v) const { guest::StoreU32(base, address + 3748, v); }
  uint32_t parts_aero8_address() const { return address + 3748; }
  // the car's +1780, or the kit's +12
  uint32_t parts_aero9() const { return guest::LoadU32(base, address + 3752); }
  void set_parts_aero9(uint32_t v) const { guest::StoreU32(base, address + 3752, v); }
  uint32_t parts_aero9_address() const { return address + 3752; }
  // the car's +1656; picks the sound assets' index (SceneModel::SetUp)
  uint32_t parts_engine_stage() const { return guest::LoadU32(base, address + 3756); }
  void set_parts_engine_stage(uint32_t v) const { guest::StoreU32(base, address + 3756, v); }
  uint32_t parts_engine_stage_address() const { return address + 3756; }
  // the car's +1668
  uint32_t parts_cooling() const { return guest::LoadU32(base, address + 3760); }
  void set_parts_cooling(uint32_t v) const { guest::StoreU32(base, address + 3760, v); }
  uint32_t parts_cooling_address() const { return address + 3760; }
  // 1 when the car's halfword +4 (or the stand's class) is 1, 2 or 4
  uint32_t parts_class() const { return guest::LoadU32(base, address + 3768); }
  void set_parts_class(uint32_t v) const { guest::StoreU32(base, address + 3768, v); }
  uint32_t parts_class_address() const { return address + 3768; }
  // the interior's 120 bytes start at +3772: 0, 1 or 2 by the engine stage (3 and 7 the steps)
  uint32_t interior_engine_grade() const { return guest::LoadU32(base, address + 3776); }
  void set_interior_engine_grade(uint32_t v) const { guest::StoreU32(base, address + 3776, v); }
  uint32_t interior_engine_grade_address() const { return address + 3776; }
  // seven 12-byte choices (CarMakersRecordChoice, SetChoice): +3780, +3792 or +3804, +3816 from the car's +1860, +1872; +3828, +3840, +3852 from +1848, +1836, +1824
  uint32_t interior_choices_address() const { return address + 3780; }
  // the car's +1808
  uint32_t interior_item2() const { return guest::LoadU32(base, address + 3864); }
  void set_interior_item2(uint32_t v) const { guest::StoreU32(base, address + 3864, v); }
  uint32_t interior_item2_address() const { return address + 3864; }
  // the car's +1804
  uint32_t interior_item1() const { return guest::LoadU32(base, address + 3868); }
  void set_interior_item1(uint32_t v) const { guest::StoreU32(base, address + 3868, v); }
  uint32_t interior_item1_address() const { return address + 3868; }
  // the car's +1800: a scene part model's index (SceneInterior::Apply)
  uint32_t interior_item0() const { return guest::LoadU32(base, address + 3872); }
  void set_interior_item0(uint32_t v) const { guest::StoreU32(base, address + 3872, v); }
  uint32_t interior_item0_address() const { return address + 3872; }
  // 0 to 2, by the car's body level (+1708) and block 2's +232
  uint32_t interior_seats() const { return guest::LoadU32(base, address + 3876); }
  void set_interior_seats(uint32_t v) const { guest::StoreU32(base, address + 3876, v); }
  uint32_t interior_seats_address() const { return address + 3876; }
  uint32_t interior_has_seats() const { return guest::LoadU32(base, address + 3880); }
  void set_interior_has_seats(uint32_t v) const { guest::StoreU32(base, address + 3880, v); }
  uint32_t interior_has_seats_address() const { return address + 3880; }
  // the car's +1816 unless 3 (none)
  uint32_t accessory() const { return guest::LoadU32(base, address + 3884); }
  void set_accessory(uint32_t v) const { guest::StoreU32(base, address + 3884, v); }
  uint32_t accessory_address() const { return address + 3884; }
  // the car's +1820, with the accessory
  uint32_t accessory_variant() const { return guest::LoadU32(base, address + 3888); }
  void set_accessory_variant(uint32_t v) const { guest::StoreU32(base, address + 3888, v); }
  uint32_t accessory_variant_address() const { return address + 3888; }
  // the cockpit's 144 bytes start here (its interior part at +24): the car's +1760
  uint32_t cockpit_bonnet() const { return guest::LoadU32(base, address + 3892); }
  void set_cockpit_bonnet(uint32_t v) const { guest::StoreU32(base, address + 3892, v); }
  uint32_t cockpit_bonnet_address() const { return address + 3892; }
  // the car's +1760 again
  uint32_t cockpit_bonnet_again() const { return guest::LoadU32(base, address + 3896); }
  void set_cockpit_bonnet_again(uint32_t v) const { guest::StoreU32(base, address + 3896, v); }
  uint32_t cockpit_bonnet_again_address() const { return address + 3896; }
  // the car's +1768
  uint32_t cockpit_aero6() const { return guest::LoadU32(base, address + 3900); }
  void set_cockpit_aero6(uint32_t v) const { guest::StoreU32(base, address + 3900, v); }
  uint32_t cockpit_aero6_address() const { return address + 3900; }
  // the car's +1772
  uint32_t cockpit_aero7() const { return guest::LoadU32(base, address + 3904); }
  void set_cockpit_aero7(uint32_t v) const { guest::StoreU32(base, address + 3904, v); }
  uint32_t cockpit_aero7_address() const { return address + 3904; }
  // the car's +1780
  uint32_t cockpit_aero9() const { return guest::LoadU32(base, address + 3908); }
  void set_cockpit_aero9(uint32_t v) const { guest::StoreU32(base, address + 3908, v); }
  uint32_t cockpit_aero9_address() const { return address + 3908; }
  // the wheels' 156 bytes start at +4036; 1 after SetPaint
  uint32_t wheels_painted() const { return guest::LoadU32(base, address + 4040); }
  void set_wheels_painted(uint32_t v) const { guest::StoreU32(base, address + 4040, v); }
  uint32_t wheels_painted_address() const { return address + 4040; }
  // 0 to 2, from the car's front tire grade (+1716) through {0, 1, 1, 2, 2}
  uint32_t front_tire_grade() const { return guest::LoadU32(base, address + 4048); }
  void set_front_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 4048, v); }
  uint32_t front_tire_grade_address() const { return address + 4048; }
  // the car's front wheel number - 1 (-1 stock)
  uint32_t front_wheel() const { return guest::LoadU32(base, address + 4052); }
  void set_front_wheel(uint32_t v) const { guest::StoreU32(base, address + 4052, v); }
  uint32_t front_wheel_address() const { return address + 4052; }
  // the car's front wheel copy's +28 (+3296)
  float front_wheel_f28() const { return guest::LoadFloat(base, address + 4056); }
  void set_front_wheel_f28(float v) const { guest::StoreFloat(base, address + 4056, v); }
  uint32_t front_wheel_f28_address() const { return address + 4056; }
  // the car's front wheel copy's +16 (+3284)
  float front_wheel_f16() const { return guest::LoadFloat(base, address + 4060); }
  void set_front_wheel_f16(float v) const { guest::StoreFloat(base, address + 4060, v); }
  uint32_t front_wheel_f16_address() const { return address + 4060; }
  // as front_tire_grade, from +1724
  uint32_t rear_tire_grade() const { return guest::LoadU32(base, address + 4064); }
  void set_rear_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 4064, v); }
  uint32_t rear_tire_grade_address() const { return address + 4064; }
  // the car's rear wheel number - 1
  uint32_t rear_wheel() const { return guest::LoadU32(base, address + 4068); }
  void set_rear_wheel(uint32_t v) const { guest::StoreU32(base, address + 4068, v); }
  uint32_t rear_wheel_address() const { return address + 4068; }
  // the car's +3344
  float rear_wheel_f28() const { return guest::LoadFloat(base, address + 4072); }
  void set_rear_wheel_f28(float v) const { guest::StoreFloat(base, address + 4072, v); }
  uint32_t rear_wheel_f28_address() const { return address + 4072; }
  // the car's +3332
  float rear_wheel_f16() const { return guest::LoadFloat(base, address + 4076); }
  void set_rear_wheel_f16(float v) const { guest::StoreFloat(base, address + 4076, v); }
  uint32_t rear_wheel_f16_address() const { return address + 4076; }
  // the front tire copy's +80, truncated (probably a size)
  uint32_t front_size() const { return guest::LoadU32(base, address + 4080); }
  void set_front_size(uint32_t v) const { guest::StoreU32(base, address + 4080, v); }
  uint32_t front_size_address() const { return address + 4080; }
  // the front tire copy's +48 x [0x82001BDC]
  float front_size_a() const { return guest::LoadFloat(base, address + 4084); }
  void set_front_size_a(float v) const { guest::StoreFloat(base, address + 4084, v); }
  uint32_t front_size_a_address() const { return address + 4084; }
  // the front tire copy's +84 x 100
  float front_size_b() const { return guest::LoadFloat(base, address + 4088); }
  void set_front_size_b(float v) const { guest::StoreFloat(base, address + 4088, v); }
  uint32_t front_size_b_address() const { return address + 4088; }
  // as front_size, from the rear tire copy
  uint32_t rear_size() const { return guest::LoadU32(base, address + 4092); }
  void set_rear_size(uint32_t v) const { guest::StoreU32(base, address + 4092, v); }
  uint32_t rear_size_address() const { return address + 4092; }
  float rear_size_a() const { return guest::LoadFloat(base, address + 4096); }
  void set_rear_size_a(float v) const { guest::StoreFloat(base, address + 4096, v); }
  uint32_t rear_size_a_address() const { return address + 4096; }
  float rear_size_b() const { return guest::LoadFloat(base, address + 4100); }
  void set_rear_size_b(float v) const { guest::StoreFloat(base, address + 4100, v); }
  uint32_t rear_size_b_address() const { return address + 4100; }
  // the car's +1696 (GarageStage::SetWheelExtras)
  uint32_t wheel_extra() const { return guest::LoadU32(base, address + 4104); }
  void set_wheel_extra(uint32_t v) const { guest::StoreU32(base, address + 4104, v); }
  uint32_t wheel_extra_address() const { return address + 4104; }
  // the car's +1692, or block 2's +236 when set (probably the calipers' texture)
  uint32_t caliper_texture() const { return guest::LoadU32(base, address + 4124); }
  void set_caliper_texture(uint32_t v) const { guest::StoreU32(base, address + 4124, v); }
  uint32_t caliper_texture_address() const { return address + 4124; }
  // pool 4's 52 bytes start here: the car's +1592
  uint32_t layer0_b() const { return guest::LoadU32(base, address + 4192); }
  void set_layer0_b(uint32_t v) const { guest::StoreU32(base, address + 4192, v); }
  uint32_t layer0_b_address() const { return address + 4192; }
  // the car's +1588
  uint32_t layer0_a() const { return guest::LoadU32(base, address + 4196); }
  void set_layer0_a(uint32_t v) const { guest::StoreU32(base, address + 4196, v); }
  uint32_t layer0_a_address() const { return address + 4196; }
  // the car's +1596
  float layer0_f8() const { return guest::LoadFloat(base, address + 4200); }
  void set_layer0_f8(float v) const { guest::StoreFloat(base, address + 4200, v); }
  uint32_t layer0_f8_address() const { return address + 4200; }
  float layer0_f12() const { return guest::LoadFloat(base, address + 4204); }
  void set_layer0_f12(float v) const { guest::StoreFloat(base, address + 4204, v); }
  uint32_t layer0_f12_address() const { return address + 4204; }
  float layer0_f16() const { return guest::LoadFloat(base, address + 4208); }
  void set_layer0_f16(float v) const { guest::StoreFloat(base, address + 4208, v); }
  uint32_t layer0_f16_address() const { return address + 4208; }
  // the car's +1608
  uint32_t layer1_a() const { return guest::LoadU32(base, address + 4212); }
  void set_layer1_a(uint32_t v) const { guest::StoreU32(base, address + 4212, v); }
  uint32_t layer1_a_address() const { return address + 4212; }
  // the car's +1616
  float layer1_f8() const { return guest::LoadFloat(base, address + 4216); }
  void set_layer1_f8(float v) const { guest::StoreFloat(base, address + 4216, v); }
  uint32_t layer1_f8_address() const { return address + 4216; }
  float layer1_f12() const { return guest::LoadFloat(base, address + 4220); }
  void set_layer1_f12(float v) const { guest::StoreFloat(base, address + 4220, v); }
  uint32_t layer1_f12_address() const { return address + 4220; }
  float layer1_f16() const { return guest::LoadFloat(base, address + 4224); }
  void set_layer1_f16(float v) const { guest::StoreFloat(base, address + 4224, v); }
  uint32_t layer1_f16_address() const { return address + 4224; }
  // the car's +1628
  uint32_t layer2_a() const { return guest::LoadU32(base, address + 4228); }
  void set_layer2_a(uint32_t v) const { guest::StoreU32(base, address + 4228, v); }
  uint32_t layer2_a_address() const { return address + 4228; }
  // the car's +1636
  float layer2_f8() const { return guest::LoadFloat(base, address + 4232); }
  void set_layer2_f8(float v) const { guest::StoreFloat(base, address + 4232, v); }
  uint32_t layer2_f8_address() const { return address + 4232; }
  float layer2_f12() const { return guest::LoadFloat(base, address + 4236); }
  void set_layer2_f12(float v) const { guest::StoreFloat(base, address + 4236, v); }
  uint32_t layer2_f12_address() const { return address + 4236; }
  float layer2_f16() const { return guest::LoadFloat(base, address + 4240); }
  void set_layer2_f16(float v) const { guest::StoreFloat(base, address + 4240, v); }
  uint32_t layer2_f16_address() const { return address + 4240; }
};

// embedded; measure: -
struct CarMakersRecordDecal : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the car decal's +24 (an image id or car model, -1 none)
  uint32_t image() const { return guest::LoadU32(base, address + 0); }
  void set_image(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t image_address() const { return address + 0; }
  // the car decal's +28
  uint32_t mirrored() const { return guest::LoadU32(base, address + 4); }
  void set_mirrored(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t mirrored_address() const { return address + 4; }
  // the car decal's +8
  float x() const { return guest::LoadFloat(base, address + 16); }
  void set_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t x_address() const { return address + 16; }
  // the car decal's +12
  float y() const { return guest::LoadFloat(base, address + 20); }
  void set_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t y_address() const { return address + 20; }
  // the car decal's +16
  float angle() const { return guest::LoadFloat(base, address + 32); }
  void set_angle(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t angle_address() const { return address + 32; }
  // the car decal's +0
  float flag_scale() const { return guest::LoadFloat(base, address + 48); }
  void set_flag_scale(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t flag_scale_address() const { return address + 48; }
  // the car decal's +4
  float scale() const { return guest::LoadFloat(base, address + 52); }
  void set_scale(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t scale_address() const { return address + 52; }
};

// embedded; measure: -
struct CarMakersRecordChoice : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 0 none, else 1
  uint32_t set() const { return guest::LoadU32(base, address + 0); }
  void set_set(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t set_address() const { return address + 0; }
  // the value's c - 1
  uint32_t index() const { return guest::LoadU32(base, address + 4); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t index_address() const { return address + 4; }
  // 3a - 3 + k
  uint32_t variant() const { return guest::LoadU32(base, address + 8); }
  void set_variant(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t variant_address() const { return address + 8; }
};

// embedded; measure: SceneModelRecord::FromCar.r4, SceneModelRecord::SetPaint.r4
struct CarMakersCar : View {
  using View::View;
  static constexpr uint32_t kSize = 3896;
  // the paint (the fields below); the measure also files SetPaint's indexed reads of the paint copy (+3376 + 20 x pick) here
  uint32_t head_address() const { return address + 0; }
  // the paint pick SetPaint takes for -1 (-2: the car's own paint below)
  uint16_t paint_pick() const { return guest::LoadU16(base, address + 0); }
  void set_paint_pick(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t paint_pick_address() const { return address + 0; }
  uint16_t unknown_2() const { return guest::LoadU16(base, address + 2); }
  void set_unknown_2(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  // -1 after the reset; 1, 2 or 4 set the record's +3768
  uint16_t body_class() const { return guest::LoadU16(base, address + 4); }
  void set_body_class(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t body_class_address() const { return address + 4; }
  // 0 or 1: how SetPaint fills the third triple
  uint16_t paint_kind() const { return guest::LoadU16(base, address + 6); }
  void set_paint_kind(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t paint_kind_address() const { return address + 6; }
  uint16_t paint_hue() const { return guest::LoadU16(base, address + 8); }
  void set_paint_hue(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t paint_hue_address() const { return address + 8; }
  uint8_t paint_saturation() const { return guest::LoadU8(base, address + 10); }
  void set_paint_saturation(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t paint_saturation_address() const { return address + 10; }
  uint8_t paint_value() const { return guest::LoadU8(base, address + 11); }
  void set_paint_value(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t paint_value_address() const { return address + 11; }
  uint32_t car_id() const { return guest::LoadU32(base, address + 12); }
  void set_car_id(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t car_id_address() const { return address + 12; }
  // six then forty 32-byte decals (CarMakersCarDecal): +0 and +4 scales, +8, +12, +16 floats, +24 an image or -1, +28 mirrored
  uint32_t decals_address() const { return address + 96; }
  // 20 bytes (CarRecord::SetBytes reads +0 to +11; +0 exterior item 2)
  uint32_t setting_bytes_address() const { return address + 1568; }
  // three 20-byte entries from +1588 (probably the vinyl layers): two words, three floats
  uint32_t layer0_a() const { return guest::LoadU32(base, address + 1588); }
  void set_layer0_a(uint32_t v) const { guest::StoreU32(base, address + 1588, v); }
  uint32_t layer0_a_address() const { return address + 1588; }
  uint32_t layer0_b() const { return guest::LoadU32(base, address + 1592); }
  void set_layer0_b(uint32_t v) const { guest::StoreU32(base, address + 1592, v); }
  uint32_t layer0_b_address() const { return address + 1592; }
  float layer0_f8() const { return guest::LoadFloat(base, address + 1596); }
  void set_layer0_f8(float v) const { guest::StoreFloat(base, address + 1596, v); }
  uint32_t layer0_f8_address() const { return address + 1596; }
  float layer0_f12() const { return guest::LoadFloat(base, address + 1600); }
  void set_layer0_f12(float v) const { guest::StoreFloat(base, address + 1600, v); }
  uint32_t layer0_f12_address() const { return address + 1600; }
  float layer0_f16() const { return guest::LoadFloat(base, address + 1604); }
  void set_layer0_f16(float v) const { guest::StoreFloat(base, address + 1604, v); }
  uint32_t layer0_f16_address() const { return address + 1604; }
  uint32_t layer1_a() const { return guest::LoadU32(base, address + 1608); }
  void set_layer1_a(uint32_t v) const { guest::StoreU32(base, address + 1608, v); }
  uint32_t layer1_a_address() const { return address + 1608; }
  float layer1_f8() const { return guest::LoadFloat(base, address + 1616); }
  void set_layer1_f8(float v) const { guest::StoreFloat(base, address + 1616, v); }
  uint32_t layer1_f8_address() const { return address + 1616; }
  float layer1_f12() const { return guest::LoadFloat(base, address + 1620); }
  void set_layer1_f12(float v) const { guest::StoreFloat(base, address + 1620, v); }
  uint32_t layer1_f12_address() const { return address + 1620; }
  float layer1_f16() const { return guest::LoadFloat(base, address + 1624); }
  void set_layer1_f16(float v) const { guest::StoreFloat(base, address + 1624, v); }
  uint32_t layer1_f16_address() const { return address + 1624; }
  uint32_t layer2_a() const { return guest::LoadU32(base, address + 1628); }
  void set_layer2_a(uint32_t v) const { guest::StoreU32(base, address + 1628, v); }
  uint32_t layer2_a_address() const { return address + 1628; }
  float layer2_f8() const { return guest::LoadFloat(base, address + 1636); }
  void set_layer2_f8(float v) const { guest::StoreFloat(base, address + 1636, v); }
  uint32_t layer2_f8_address() const { return address + 1636; }
  float layer2_f12() const { return guest::LoadFloat(base, address + 1640); }
  void set_layer2_f12(float v) const { guest::StoreFloat(base, address + 1640, v); }
  uint32_t layer2_f12_address() const { return address + 1640; }
  float layer2_f16() const { return guest::LoadFloat(base, address + 1644); }
  void set_layer2_f16(float v) const { guest::StoreFloat(base, address + 1644, v); }
  uint32_t layer2_f16_address() const { return address + 1644; }
  uint32_t engine_stage() const { return guest::LoadU32(base, address + 1656); }
  void set_engine_stage(uint32_t v) const { guest::StoreU32(base, address + 1656, v); }
  uint32_t engine_stage_address() const { return address + 1656; }
  uint32_t power_part() const { return guest::LoadU32(base, address + 1664); }
  void set_power_part(uint32_t v) const { guest::StoreU32(base, address + 1664, v); }
  uint32_t power_part_address() const { return address + 1664; }
  // the cooling module's type
  uint32_t cooling() const { return guest::LoadU32(base, address + 1668); }
  void set_cooling(uint32_t v) const { guest::StoreU32(base, address + 1668, v); }
  uint32_t cooling_address() const { return address + 1668; }
  // a power train level (probably the gear count); the record's caliper_texture
  uint32_t power_train3() const { return guest::LoadU32(base, address + 1692); }
  void set_power_train3(uint32_t v) const { guest::StoreU32(base, address + 1692, v); }
  uint32_t power_train3_address() const { return address + 1692; }
  // the record's wheel_extra
  uint32_t power_train4() const { return guest::LoadU32(base, address + 1696); }
  void set_power_train4(uint32_t v) const { guest::StoreU32(base, address + 1696, v); }
  uint32_t power_train4_address() const { return address + 1696; }
  // picks the record's seats (1 to 2, 3 to 5, 6)
  uint32_t body_level() const { return guest::LoadU32(base, address + 1708); }
  void set_body_level(uint32_t v) const { guest::StoreU32(base, address + 1708, v); }
  uint32_t body_level_address() const { return address + 1708; }
  uint32_t front_tire_grade() const { return guest::LoadU32(base, address + 1716); }
  void set_front_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 1716, v); }
  uint32_t front_tire_grade_address() const { return address + 1716; }
  uint32_t rear_tire_grade() const { return guest::LoadU32(base, address + 1724); }
  void set_rear_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 1724, v); }
  uint32_t rear_tire_grade_address() const { return address + 1724; }
  // the aero items from +1744 to +1784
  uint32_t aero_item0() const { return guest::LoadU32(base, address + 1744); }
  void set_aero_item0(uint32_t v) const { guest::StoreU32(base, address + 1744, v); }
  uint32_t aero_item0_address() const { return address + 1744; }
  uint32_t aero_item1() const { return guest::LoadU32(base, address + 1748); }
  void set_aero_item1(uint32_t v) const { guest::StoreU32(base, address + 1748, v); }
  uint32_t aero_item1_address() const { return address + 1748; }
  uint32_t aero_item2() const { return guest::LoadU32(base, address + 1752); }
  void set_aero_item2(uint32_t v) const { guest::StoreU32(base, address + 1752, v); }
  uint32_t aero_item2_address() const { return address + 1752; }
  uint32_t aero_item3() const { return guest::LoadU32(base, address + 1756); }
  void set_aero_item3(uint32_t v) const { guest::StoreU32(base, address + 1756, v); }
  uint32_t aero_item3_address() const { return address + 1756; }
  // the aero items' bonnet
  uint32_t bonnet() const { return guest::LoadU32(base, address + 1760); }
  void set_bonnet(uint32_t v) const { guest::StoreU32(base, address + 1760, v); }
  uint32_t bonnet_address() const { return address + 1760; }
  // 0 to 2
  uint32_t bonnet_color_kind() const { return guest::LoadU32(base, address + 1764); }
  void set_bonnet_color_kind(uint32_t v) const { guest::StoreU32(base, address + 1764, v); }
  uint32_t bonnet_color_kind_address() const { return address + 1764; }
  uint32_t aero_item6() const { return guest::LoadU32(base, address + 1768); }
  void set_aero_item6(uint32_t v) const { guest::StoreU32(base, address + 1768, v); }
  uint32_t aero_item6_address() const { return address + 1768; }
  uint32_t aero_item7() const { return guest::LoadU32(base, address + 1772); }
  void set_aero_item7(uint32_t v) const { guest::StoreU32(base, address + 1772, v); }
  uint32_t aero_item7_address() const { return address + 1772; }
  uint32_t aero_item8() const { return guest::LoadU32(base, address + 1776); }
  void set_aero_item8(uint32_t v) const { guest::StoreU32(base, address + 1776, v); }
  uint32_t aero_item8_address() const { return address + 1776; }
  uint32_t aero_item9() const { return guest::LoadU32(base, address + 1780); }
  void set_aero_item9(uint32_t v) const { guest::StoreU32(base, address + 1780, v); }
  uint32_t aero_item9_address() const { return address + 1780; }
  // 0 none, else the kit (entry block x 2 + kit - 1 of [0x820093E0])
  uint32_t aero_kit() const { return guest::LoadU32(base, address + 1784); }
  void set_aero_kit(uint32_t v) const { guest::StoreU32(base, address + 1784, v); }
  uint32_t aero_kit_address() const { return address + 1784; }
  uint32_t exterior_item1() const { return guest::LoadU32(base, address + 1792); }
  void set_exterior_item1(uint32_t v) const { guest::StoreU32(base, address + 1792, v); }
  uint32_t exterior_item1_address() const { return address + 1792; }
  uint32_t interior_item0() const { return guest::LoadU32(base, address + 1800); }
  void set_interior_item0(uint32_t v) const { guest::StoreU32(base, address + 1800, v); }
  uint32_t interior_item0_address() const { return address + 1800; }
  uint32_t interior_item1() const { return guest::LoadU32(base, address + 1804); }
  void set_interior_item1(uint32_t v) const { guest::StoreU32(base, address + 1804, v); }
  uint32_t interior_item1_address() const { return address + 1804; }
  uint32_t interior_item2() const { return guest::LoadU32(base, address + 1808); }
  void set_interior_item2(uint32_t v) const { guest::StoreU32(base, address + 1808, v); }
  uint32_t interior_item2_address() const { return address + 1808; }
  uint32_t interior_item3() const { return guest::LoadU32(base, address + 1812); }
  void set_interior_item3(uint32_t v) const { guest::StoreU32(base, address + 1812, v); }
  uint32_t interior_item3_address() const { return address + 1812; }
  // the accessory (3 none)
  uint32_t accessory() const { return guest::LoadU32(base, address + 1816); }
  void set_accessory(uint32_t v) const { guest::StoreU32(base, address + 1816, v); }
  uint32_t accessory_address() const { return address + 1816; }
  // the accessory's variant
  uint32_t accessory_variant() const { return guest::LoadU32(base, address + 1820); }
  void set_accessory_variant(uint32_t v) const { guest::StoreU32(base, address + 1820, v); }
  uint32_t accessory_variant_address() const { return address + 1820; }
  // five 12-byte values {a, b, c} (SetChoice's r4:r5), read as a doubleword and a word
  uint64_t choice0() const { return guest::LoadU64(base, address + 1824); }
  void set_choice0(uint64_t v) const { guest::StoreU64(base, address + 1824, v); }
  uint32_t choice0_address() const { return address + 1824; }
  uint32_t choice0_a() const { return guest::LoadU32(base, address + 1824); }
  void set_choice0_a(uint32_t v) const { guest::StoreU32(base, address + 1824, v); }
  uint32_t choice0_a_address() const { return address + 1824; }
  uint32_t choice0_b() const { return guest::LoadU32(base, address + 1828); }
  void set_choice0_b(uint32_t v) const { guest::StoreU32(base, address + 1828, v); }
  uint32_t choice0_b_address() const { return address + 1828; }
  uint32_t choice0_c() const { return guest::LoadU32(base, address + 1832); }
  void set_choice0_c(uint32_t v) const { guest::StoreU32(base, address + 1832, v); }
  uint32_t choice0_c_address() const { return address + 1832; }
  uint64_t choice1() const { return guest::LoadU64(base, address + 1836); }
  void set_choice1(uint64_t v) const { guest::StoreU64(base, address + 1836, v); }
  uint32_t choice1_address() const { return address + 1836; }
  uint32_t choice1_a() const { return guest::LoadU32(base, address + 1836); }
  void set_choice1_a(uint32_t v) const { guest::StoreU32(base, address + 1836, v); }
  uint32_t choice1_a_address() const { return address + 1836; }
  uint32_t choice1_b() const { return guest::LoadU32(base, address + 1840); }
  void set_choice1_b(uint32_t v) const { guest::StoreU32(base, address + 1840, v); }
  uint32_t choice1_b_address() const { return address + 1840; }
  uint32_t choice1_c() const { return guest::LoadU32(base, address + 1844); }
  void set_choice1_c(uint32_t v) const { guest::StoreU32(base, address + 1844, v); }
  uint32_t choice1_c_address() const { return address + 1844; }
  uint64_t choice2() const { return guest::LoadU64(base, address + 1848); }
  void set_choice2(uint64_t v) const { guest::StoreU64(base, address + 1848, v); }
  uint32_t choice2_address() const { return address + 1848; }
  uint32_t choice2_a() const { return guest::LoadU32(base, address + 1848); }
  void set_choice2_a(uint32_t v) const { guest::StoreU32(base, address + 1848, v); }
  uint32_t choice2_a_address() const { return address + 1848; }
  uint32_t choice2_b() const { return guest::LoadU32(base, address + 1852); }
  void set_choice2_b(uint32_t v) const { guest::StoreU32(base, address + 1852, v); }
  uint32_t choice2_b_address() const { return address + 1852; }
  uint32_t choice2_c() const { return guest::LoadU32(base, address + 1856); }
  void set_choice2_c(uint32_t v) const { guest::StoreU32(base, address + 1856, v); }
  uint32_t choice2_c_address() const { return address + 1856; }
  uint64_t choice3() const { return guest::LoadU64(base, address + 1860); }
  void set_choice3(uint64_t v) const { guest::StoreU64(base, address + 1860, v); }
  uint32_t choice3_address() const { return address + 1860; }
  uint32_t choice3_a() const { return guest::LoadU32(base, address + 1860); }
  void set_choice3_a(uint32_t v) const { guest::StoreU32(base, address + 1860, v); }
  uint32_t choice3_a_address() const { return address + 1860; }
  uint32_t choice3_b() const { return guest::LoadU32(base, address + 1864); }
  void set_choice3_b(uint32_t v) const { guest::StoreU32(base, address + 1864, v); }
  uint32_t choice3_b_address() const { return address + 1864; }
  uint32_t choice3_c() const { return guest::LoadU32(base, address + 1868); }
  void set_choice3_c(uint32_t v) const { guest::StoreU32(base, address + 1868, v); }
  uint32_t choice3_c_address() const { return address + 1868; }
  uint64_t choice4() const { return guest::LoadU64(base, address + 1872); }
  void set_choice4(uint64_t v) const { guest::StoreU64(base, address + 1872, v); }
  uint32_t choice4_address() const { return address + 1872; }
  uint32_t choice4_a() const { return guest::LoadU32(base, address + 1872); }
  void set_choice4_a(uint32_t v) const { guest::StoreU32(base, address + 1872, v); }
  uint32_t choice4_a_address() const { return address + 1872; }
  uint32_t choice4_b() const { return guest::LoadU32(base, address + 1876); }
  void set_choice4_b(uint32_t v) const { guest::StoreU32(base, address + 1876, v); }
  uint32_t choice4_b_address() const { return address + 1876; }
  uint32_t choice4_c() const { return guest::LoadU32(base, address + 1880); }
  void set_choice4_c(uint32_t v) const { guest::StoreU32(base, address + 1880, v); }
  uint32_t choice4_c_address() const { return address + 1880; }
  // the front tire copy (+3084, inched up)'s +48
  float front_tire_48() const { return guest::LoadFloat(base, address + 3132); }
  void set_front_tire_48(float v) const { guest::StoreFloat(base, address + 3132, v); }
  uint32_t front_tire_48_address() const { return address + 3132; }
  float front_tire_80() const { return guest::LoadFloat(base, address + 3164); }
  void set_front_tire_80(float v) const { guest::StoreFloat(base, address + 3164, v); }
  uint32_t front_tire_80_address() const { return address + 3164; }
  float front_tire_84() const { return guest::LoadFloat(base, address + 3168); }
  void set_front_tire_84(float v) const { guest::StoreFloat(base, address + 3168, v); }
  uint32_t front_tire_84_address() const { return address + 3168; }
  // the rear tire copy (+3176, inched up)'s +48
  float rear_tire_48() const { return guest::LoadFloat(base, address + 3224); }
  void set_rear_tire_48(float v) const { guest::StoreFloat(base, address + 3224, v); }
  uint32_t rear_tire_48_address() const { return address + 3224; }
  float rear_tire_80() const { return guest::LoadFloat(base, address + 3256); }
  void set_rear_tire_80(float v) const { guest::StoreFloat(base, address + 3256, v); }
  uint32_t rear_tire_80_address() const { return address + 3256; }
  float rear_tire_84() const { return guest::LoadFloat(base, address + 3260); }
  void set_rear_tire_84(float v) const { guest::StoreFloat(base, address + 3260, v); }
  uint32_t rear_tire_84_address() const { return address + 3260; }
  // the front wheel copy (+3268): three colors, a hue halfword, saturation and value bytes
  uint16_t front_wheel_color0_hue() const { return guest::LoadU16(base, address + 3276); }
  void set_front_wheel_color0_hue(uint16_t v) const { guest::StoreU16(base, address + 3276, v); }
  uint32_t front_wheel_color0_hue_address() const { return address + 3276; }
  uint8_t front_wheel_color0_saturation() const { return guest::LoadU8(base, address + 3278); }
  void set_front_wheel_color0_saturation(uint8_t v) const { guest::StoreU8(base, address + 3278, v); }
  uint32_t front_wheel_color0_saturation_address() const { return address + 3278; }
  uint8_t front_wheel_color0_value() const { return guest::LoadU8(base, address + 3279); }
  void set_front_wheel_color0_value(uint8_t v) const { guest::StoreU8(base, address + 3279, v); }
  uint32_t front_wheel_color0_value_address() const { return address + 3279; }
  uint16_t front_wheel_color1_hue() const { return guest::LoadU16(base, address + 3288); }
  void set_front_wheel_color1_hue(uint16_t v) const { guest::StoreU16(base, address + 3288, v); }
  uint32_t front_wheel_color1_hue_address() const { return address + 3288; }
  uint8_t front_wheel_color1_saturation() const { return guest::LoadU8(base, address + 3290); }
  void set_front_wheel_color1_saturation(uint8_t v) const { guest::StoreU8(base, address + 3290, v); }
  uint32_t front_wheel_color1_saturation_address() const { return address + 3290; }
  uint8_t front_wheel_color1_value() const { return guest::LoadU8(base, address + 3291); }
  void set_front_wheel_color1_value(uint8_t v) const { guest::StoreU8(base, address + 3291, v); }
  uint32_t front_wheel_color1_value_address() const { return address + 3291; }
  uint16_t front_wheel_color2_hue() const { return guest::LoadU16(base, address + 3292); }
  void set_front_wheel_color2_hue(uint16_t v) const { guest::StoreU16(base, address + 3292, v); }
  uint32_t front_wheel_color2_hue_address() const { return address + 3292; }
  uint8_t front_wheel_color2_saturation() const { return guest::LoadU8(base, address + 3294); }
  void set_front_wheel_color2_saturation(uint8_t v) const { guest::StoreU8(base, address + 3294, v); }
  uint32_t front_wheel_color2_saturation_address() const { return address + 3294; }
  uint8_t front_wheel_color2_value() const { return guest::LoadU8(base, address + 3295); }
  void set_front_wheel_color2_value(uint8_t v) const { guest::StoreU8(base, address + 3295, v); }
  uint32_t front_wheel_color2_value_address() const { return address + 3295; }
  float front_wheel_f16() const { return guest::LoadFloat(base, address + 3284); }
  void set_front_wheel_f16(float v) const { guest::StoreFloat(base, address + 3284, v); }
  uint32_t front_wheel_f16_address() const { return address + 3284; }
  float front_wheel_f28() const { return guest::LoadFloat(base, address + 3296); }
  void set_front_wheel_f28(float v) const { guest::StoreFloat(base, address + 3296, v); }
  uint32_t front_wheel_f28_address() const { return address + 3296; }
  // 1 + the wheel's number (0 stock)
  uint32_t front_wheel_number() const { return guest::LoadU32(base, address + 3308); }
  void set_front_wheel_number(uint32_t v) const { guest::StoreU32(base, address + 3308, v); }
  uint32_t front_wheel_number_address() const { return address + 3308; }
  // the rear wheel copy (+3316): three colors, a hue halfword, saturation and value bytes
  uint16_t rear_wheel_color0_hue() const { return guest::LoadU16(base, address + 3324); }
  void set_rear_wheel_color0_hue(uint16_t v) const { guest::StoreU16(base, address + 3324, v); }
  uint32_t rear_wheel_color0_hue_address() const { return address + 3324; }
  uint8_t rear_wheel_color0_saturation() const { return guest::LoadU8(base, address + 3326); }
  void set_rear_wheel_color0_saturation(uint8_t v) const { guest::StoreU8(base, address + 3326, v); }
  uint32_t rear_wheel_color0_saturation_address() const { return address + 3326; }
  uint8_t rear_wheel_color0_value() const { return guest::LoadU8(base, address + 3327); }
  void set_rear_wheel_color0_value(uint8_t v) const { guest::StoreU8(base, address + 3327, v); }
  uint32_t rear_wheel_color0_value_address() const { return address + 3327; }
  uint16_t rear_wheel_color1_hue() const { return guest::LoadU16(base, address + 3336); }
  void set_rear_wheel_color1_hue(uint16_t v) const { guest::StoreU16(base, address + 3336, v); }
  uint32_t rear_wheel_color1_hue_address() const { return address + 3336; }
  uint8_t rear_wheel_color1_saturation() const { return guest::LoadU8(base, address + 3338); }
  void set_rear_wheel_color1_saturation(uint8_t v) const { guest::StoreU8(base, address + 3338, v); }
  uint32_t rear_wheel_color1_saturation_address() const { return address + 3338; }
  uint8_t rear_wheel_color1_value() const { return guest::LoadU8(base, address + 3339); }
  void set_rear_wheel_color1_value(uint8_t v) const { guest::StoreU8(base, address + 3339, v); }
  uint32_t rear_wheel_color1_value_address() const { return address + 3339; }
  uint16_t rear_wheel_color2_hue() const { return guest::LoadU16(base, address + 3340); }
  void set_rear_wheel_color2_hue(uint16_t v) const { guest::StoreU16(base, address + 3340, v); }
  uint32_t rear_wheel_color2_hue_address() const { return address + 3340; }
  uint8_t rear_wheel_color2_saturation() const { return guest::LoadU8(base, address + 3342); }
  void set_rear_wheel_color2_saturation(uint8_t v) const { guest::StoreU8(base, address + 3342, v); }
  uint32_t rear_wheel_color2_saturation_address() const { return address + 3342; }
  uint8_t rear_wheel_color2_value() const { return guest::LoadU8(base, address + 3343); }
  void set_rear_wheel_color2_value(uint8_t v) const { guest::StoreU8(base, address + 3343, v); }
  uint32_t rear_wheel_color2_value_address() const { return address + 3343; }
  float rear_wheel_f16() const { return guest::LoadFloat(base, address + 3332); }
  void set_rear_wheel_f16(float v) const { guest::StoreFloat(base, address + 3332, v); }
  uint32_t rear_wheel_f16_address() const { return address + 3332; }
  float rear_wheel_f28() const { return guest::LoadFloat(base, address + 3344); }
  void set_rear_wheel_f28(float v) const { guest::StoreFloat(base, address + 3344, v); }
  uint32_t rear_wheel_f28_address() const { return address + 3344; }
  // 1 + the wheel's number (0 stock)
  uint32_t rear_wheel_number() const { return guest::LoadU32(base, address + 3356); }
  void set_rear_wheel_number(uint32_t v) const { guest::StoreU32(base, address + 3356, v); }
  uint32_t rear_wheel_number_address() const { return address + 3356; }
  // the paint's copy: 20-byte entries from +3376 (three colors each, a hue halfword and two bytes), picked by SetPaint
  uint32_t paint_copy_address() const { return address + 3364; }
  // the copy's first entry (SetPaint's -2)
  uint16_t paint_copy0_hue0() const { return guest::LoadU16(base, address + 3376); }
  void set_paint_copy0_hue0(uint16_t v) const { guest::StoreU16(base, address + 3376, v); }
  uint32_t paint_copy0_hue0_address() const { return address + 3376; }
  uint8_t paint_copy0_saturation0() const { return guest::LoadU8(base, address + 3378); }
  void set_paint_copy0_saturation0(uint8_t v) const { guest::StoreU8(base, address + 3378, v); }
  uint32_t paint_copy0_saturation0_address() const { return address + 3378; }
  uint8_t paint_copy0_value0() const { return guest::LoadU8(base, address + 3379); }
  void set_paint_copy0_value0(uint8_t v) const { guest::StoreU8(base, address + 3379, v); }
  uint32_t paint_copy0_value0_address() const { return address + 3379; }
  uint16_t paint_copy0_hue1() const { return guest::LoadU16(base, address + 3380); }
  void set_paint_copy0_hue1(uint16_t v) const { guest::StoreU16(base, address + 3380, v); }
  uint32_t paint_copy0_hue1_address() const { return address + 3380; }
  uint8_t paint_copy0_saturation1() const { return guest::LoadU8(base, address + 3382); }
  void set_paint_copy0_saturation1(uint8_t v) const { guest::StoreU8(base, address + 3382, v); }
  uint32_t paint_copy0_saturation1_address() const { return address + 3382; }
  uint8_t paint_copy0_value1() const { return guest::LoadU8(base, address + 3383); }
  void set_paint_copy0_value1(uint8_t v) const { guest::StoreU8(base, address + 3383, v); }
  uint32_t paint_copy0_value1_address() const { return address + 3383; }
  uint16_t paint_copy0_hue2() const { return guest::LoadU16(base, address + 3384); }
  void set_paint_copy0_hue2(uint16_t v) const { guest::StoreU16(base, address + 3384, v); }
  uint32_t paint_copy0_hue2_address() const { return address + 3384; }
  uint8_t paint_copy0_saturation2() const { return guest::LoadU8(base, address + 3386); }
  void set_paint_copy0_saturation2(uint8_t v) const { guest::StoreU8(base, address + 3386, v); }
  uint32_t paint_copy0_saturation2_address() const { return address + 3386; }
  uint8_t paint_copy0_value2() const { return guest::LoadU8(base, address + 3387); }
  void set_paint_copy0_value2(uint8_t v) const { guest::StoreU8(base, address + 3387, v); }
  uint32_t paint_copy0_value2_address() const { return address + 3387; }
  // 0x1 and 0x80 the players' cars, 0x2 the rivals, 0x4 traffic
  uint32_t flags() const { return guest::LoadU32(base, address + 3888); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 3888, v); }
  uint32_t flags_address() const { return address + 3888; }
  // 1 after Car::Init
  uint32_t unknown_3892() const { return guest::LoadU32(base, address + 3892); }
  void set_unknown_3892(uint32_t v) const { guest::StoreU32(base, address + 3892, v); }
  uint32_t unknown_3892_address() const { return address + 3892; }
};

// embedded; measure: -
struct CarMakersCarDecal : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // negative: mirrored
  float flag_scale() const { return guest::LoadFloat(base, address + 0); }
  void set_flag_scale(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t flag_scale_address() const { return address + 0; }
  float scale() const { return guest::LoadFloat(base, address + 4); }
  void set_scale(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t scale_address() const { return address + 4; }
  float x() const { return guest::LoadFloat(base, address + 8); }
  void set_x(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t x_address() const { return address + 8; }
  float y() const { return guest::LoadFloat(base, address + 12); }
  void set_y(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t y_address() const { return address + 12; }
  float angle() const { return guest::LoadFloat(base, address + 16); }
  void set_angle(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t angle_address() const { return address + 16; }
  // packed from a halfword and two bytes
  uint32_t packed() const { return guest::LoadU32(base, address + 20); }
  void set_packed(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t packed_address() const { return address + 20; }
  // an image id or car model, -1 none
  uint32_t image() const { return guest::LoadU32(base, address + 24); }
  void set_image(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t image_address() const { return address + 24; }
  uint32_t mirrored() const { return guest::LoadU32(base, address + 28); }
  void set_mirrored(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t mirrored_address() const { return address + 28; }
};

// embedded; measure: -
struct CarMakersCarChoice : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // a and b, read as one doubleword (r4)
  uint64_t ab() const { return guest::LoadU64(base, address + 0); }
  void set_ab(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t ab_address() const { return address + 0; }
  uint32_t a() const { return guest::LoadU32(base, address + 0); }
  void set_a(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t a_address() const { return address + 0; }
  uint32_t b() const { return guest::LoadU32(base, address + 4); }
  void set_b(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t b_address() const { return address + 4; }
  // 0 none
  uint32_t c() const { return guest::LoadU32(base, address + 8); }
  void set_c(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t c_address() const { return address + 8; }
};

// embedded; measure: GarageCar
struct GarageCar : View {
  using View::View;
  static constexpr uint32_t kSize = 1968;
  // a car object's first 1,884 bytes (CarMakersCar)
  uint32_t car_head_address() const { return address + 0; }
  // 3, or the spec's +748 (Car::SetModelAndClass)
  uint32_t spec_748() const { return guest::LoadU32(base, address + 1884); }
  void set_spec_748(uint32_t v) const { guest::StoreU32(base, address + 1884, v); }
  uint32_t spec_748_address() const { return address + 1884; }
  // 0.0 at the reset
  float unknown_1888() const { return guest::LoadFloat(base, address + 1888); }
  void set_unknown_1888(float v) const { guest::StoreFloat(base, address + 1888, v); }
  uint32_t unknown_1888_address() const { return address + 1888; }
  // 0.0 at the reset; Racers::Add's f1 (the racer record's +3920)
  float racer_float() const { return guest::LoadFloat(base, address + 1892); }
  void set_racer_float(float v) const { guest::StoreFloat(base, address + 1892, v); }
  uint32_t racer_float_address() const { return address + 1892; }
  uint32_t unknown_1896() const { return guest::LoadU32(base, address + 1896); }
  void set_unknown_1896(uint32_t v) const { guest::StoreU32(base, address + 1896, v); }
  uint32_t unknown_1896_address() const { return address + 1896; }
  // the spec's +744 (Car::SetModelAndClass)
  uint32_t spec_744() const { return guest::LoadU32(base, address + 1900); }
  void set_spec_744(uint32_t v) const { guest::StoreU32(base, address + 1900, v); }
  uint32_t spec_744_address() const { return address + 1900; }
  // a bitset of the owned parts: 0x826C3044's 60 bytes at the reset
  uint32_t owned_parts_address() const { return address + 1904; }
  // 1 at the reset
  uint32_t unknown_1964() const { return guest::LoadU32(base, address + 1964); }
  void set_unknown_1964(uint32_t v) const { guest::StoreU32(base, address + 1964, v); }
  uint32_t unknown_1964_address() const { return address + 1964; }
};

// embedded; measure: -
struct GarageCarSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 1972;
  // a GarageCar
  uint32_t car_address() const { return address + 0; }
  // 0 at the static init
  uint32_t unknown_1968() const { return guest::LoadU32(base, address + 1968); }
  void set_unknown_1968(uint32_t v) const { guest::StoreU32(base, address + 1968, v); }
  uint32_t unknown_1968_address() const { return address + 1968; }
};

// embedded; measure: sub_82267CD0.r3
struct CarMakersNetMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9660;
  // a StateMachine (game-modes.toml)
  uint32_t machine_address() const { return address + 0; }
  // 1: back (Destroy goes to state 14)
  uint32_t back() const { return guest::LoadU32(base, address + 9624); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 9624, v); }
  uint32_t back_address() const { return address + 9624; }
  uint32_t unknown_9628() const { return guest::LoadU32(base, address + 9628); }
  void set_unknown_9628(uint32_t v) const { guest::StoreU32(base, address + 9628, v); }
  uint32_t unknown_9628_address() const { return address + 9628; }
  // 1: Destroy goes to state 15
  uint32_t leave() const { return guest::LoadU32(base, address + 9632); }
  void set_leave(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t leave_address() const { return address + 9632; }
  // set by ModeType12::Interrupt on the shown machine
  uint32_t shown() const { return guest::LoadU32(base, address + 9636); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 9636, v); }
  uint32_t shown_address() const { return address + 9636; }
  uint32_t unknown_9640() const { return guest::LoadU32(base, address + 9640); }
  void set_unknown_9640(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t unknown_9640_address() const { return address + 9640; }
  uint32_t unknown_9648() const { return guest::LoadU32(base, address + 9648); }
  void set_unknown_9648(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t unknown_9648_address() const { return address + 9648; }
  uint32_t unknown_9652() const { return guest::LoadU32(base, address + 9652); }
  void set_unknown_9652(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t unknown_9652_address() const { return address + 9652; }
  uint32_t unknown_9656() const { return guest::LoadU32(base, address + 9656); }
  void set_unknown_9656(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t unknown_9656_address() const { return address + 9656; }
};

// The image words docs/functions/car-makers.md's code uses.
struct CarMakersGlobals {
  // the stand scene's holder: three 4-byte blocks, each holding a garage stage at +0
  static View stand_holder(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
  // GarageStage::ReloadModel's calls since its last remake (the remake waits for 4)
  static uint32_t reload_count(uint8_t* base) { return guest::LoadU32(base, 0x826D83E4u); }
  static void set_reload_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E4u, v); }
  static constexpr uint32_t reload_count_address() { return 0x826D83E4u; }
  // two GarageCarSlot (1,972 bytes each)
  static constexpr uint32_t garage_cars_address() { return 0x82814EB8u; }
  // the aero kits' 24-byte entries, two a model block (block x 2 + kit - 1): +0, +4, +8 the record's +3712, +3720, +3724, +12 its +3752, +16 its +3732, +20 its +3728; how many not known
  static constexpr uint32_t aero_kits_address() { return 0x820093E0u; }
  // 1/255 (SetPaint)
  static float byte_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002200u); }
  static void set_byte_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002200u, v); }
  static constexpr uint32_t byte_scale_address() { return 0x82002200u; }
  // SetPaint's paint kind 1 hue factor (value not read)
  static float hue_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002A14u); }
  static void set_hue_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A14u, v); }
  static constexpr uint32_t hue_scale_address() { return 0x82002A14u; }
  // SetPaint's third value for paint kind 0 (not read)
  static float value72(uint8_t* base) { return guest::LoadFloat(base, 0x82009788u); }
  static void set_value72(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009788u, v); }
  static constexpr uint32_t value72_address() { return 0x82009788u; }
  // SetPaint's third value for paint kind 1 (0.8)
  static float value72_kind1(uint8_t* base) { return guest::LoadFloat(base, 0x82001070u); }
  static void set_value72_kind1(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001070u, v); }
  static constexpr uint32_t value72_kind1_address() { return 0x82001070u; }
  // SetWheels' tire factor (not read)
  static float tire_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001BDCu); }
  static void set_tire_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BDCu, v); }
  static constexpr uint32_t tire_scale_address() { return 0x82001BDCu; }
  // 100.0
  static float hundred(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_hundred(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t hundred_address() { return 0x82002A0Cu; }
  // pi / 180 (GarageStage::FitCar)
  static float radians_per_degree(uint8_t* base) { return guest::LoadFloat(base, 0x82002948u); }
  static void set_radians_per_degree(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002948u, v); }
  static constexpr uint32_t radians_per_degree_address() { return 0x82002948u; }
};

}  // namespace schema
