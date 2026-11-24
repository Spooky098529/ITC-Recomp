// The views of docs/functions/race-setup.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-setup.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Car, RacerRecord
struct Car : View {
  using View::View;
  static constexpr uint32_t kSize = 3936;
  // measure fault: the parts' resets (Car::ResetSettings, Car::ResetDecals6 and 40, sub_820CCC70, given car + 16, + 96, + 288, + 1568) are filed at +0 (up to the settings' +71), as are Car::DrawUpdate's scene model (+32) and Car::TickUpdate's car slots (+0, +16)
  uint32_t fault_head_address() const { return address + 0; }
  // the paint (probably): -2 for event 1's rival; a build's +2792
  uint16_t paint_pick() const { return guest::LoadU16(base, address + 0); }
  void set_paint_pick(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t paint_pick_address() const { return address + 0; }
  // a build's +2792 too
  uint16_t paint_2() const { return guest::LoadU16(base, address + 2); }
  void set_paint_2(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t paint_2_address() const { return address + 2; }
  // -1 after the reset
  uint16_t body_class() const { return guest::LoadU16(base, address + 4); }
  void set_body_class(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t body_class_address() const { return address + 4; }
  // a build's +2796; 1 for event 1's rival
  uint16_t paint_kind() const { return guest::LoadU16(base, address + 6); }
  void set_paint_kind(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t paint_kind_address() const { return address + 6; }
  // a build's +2800; 234 for event 1's rival
  uint16_t paint_hue() const { return guest::LoadU16(base, address + 8); }
  void set_paint_hue(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t paint_hue_address() const { return address + 8; }
  // a build's +2804; 92 for event 1's rival
  uint8_t paint_saturation() const { return guest::LoadU8(base, address + 10); }
  void set_paint_saturation(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t paint_saturation_address() const { return address + 10; }
  // a build's +2808; 64 for event 1's rival
  uint8_t paint_value() const { return guest::LoadU8(base, address + 11); }
  void set_paint_value(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t paint_value_address() const { return address + 11; }
  uint32_t car_id() const { return guest::LoadU32(base, address + 12); }
  void set_car_id(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t car_id_address() const { return address + 12; }
  // nine halfwords (Car::ResetSettings: 0); what they are isn't known
  uint16_t setting_half_0() const { return guest::LoadU16(base, address + 16); }
  void set_setting_half_0(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t setting_half_0_address() const { return address + 16; }
  uint16_t setting_half_1() const { return guest::LoadU16(base, address + 18); }
  void set_setting_half_1(uint16_t v) const { guest::StoreU16(base, address + 18, v); }
  uint32_t setting_half_1_address() const { return address + 18; }
  uint16_t setting_half_2() const { return guest::LoadU16(base, address + 20); }
  void set_setting_half_2(uint16_t v) const { guest::StoreU16(base, address + 20, v); }
  uint32_t setting_half_2_address() const { return address + 20; }
  uint16_t setting_half_3() const { return guest::LoadU16(base, address + 22); }
  void set_setting_half_3(uint16_t v) const { guest::StoreU16(base, address + 22, v); }
  uint32_t setting_half_3_address() const { return address + 22; }
  uint16_t setting_half_4() const { return guest::LoadU16(base, address + 24); }
  void set_setting_half_4(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t setting_half_4_address() const { return address + 24; }
  uint16_t setting_half_5() const { return guest::LoadU16(base, address + 26); }
  void set_setting_half_5(uint16_t v) const { guest::StoreU16(base, address + 26, v); }
  uint32_t setting_half_5_address() const { return address + 26; }
  uint16_t setting_half_6() const { return guest::LoadU16(base, address + 28); }
  void set_setting_half_6(uint16_t v) const { guest::StoreU16(base, address + 28, v); }
  uint32_t setting_half_6_address() const { return address + 28; }
  uint16_t setting_half_7() const { return guest::LoadU16(base, address + 30); }
  void set_setting_half_7(uint16_t v) const { guest::StoreU16(base, address + 30, v); }
  uint32_t setting_half_7_address() const { return address + 30; }
  uint16_t setting_half_8() const { return guest::LoadU16(base, address + 32); }
  void set_setting_half_8(uint16_t v) const { guest::StoreU16(base, address + 32, v); }
  uint32_t setting_half_8_address() const { return address + 32; }
  // the spec's block 1 +372; a build scales it
  float final_drive() const { return guest::LoadFloat(base, address + 36); }
  void set_final_drive(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t final_drive_address() const { return address + 36; }
  // the spec's block 1 +388 + 4i; a build scales the first six
  float gear_ratio_0() const { return guest::LoadFloat(base, address + 40); }
  void set_gear_ratio_0(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t gear_ratio_0_address() const { return address + 40; }
  float gear_ratio_1() const { return guest::LoadFloat(base, address + 44); }
  void set_gear_ratio_1(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t gear_ratio_1_address() const { return address + 44; }
  float gear_ratio_2() const { return guest::LoadFloat(base, address + 48); }
  void set_gear_ratio_2(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t gear_ratio_2_address() const { return address + 48; }
  float gear_ratio_3() const { return guest::LoadFloat(base, address + 52); }
  void set_gear_ratio_3(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t gear_ratio_3_address() const { return address + 52; }
  float gear_ratio_4() const { return guest::LoadFloat(base, address + 56); }
  void set_gear_ratio_4(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t gear_ratio_4_address() const { return address + 56; }
  float gear_ratio_5() const { return guest::LoadFloat(base, address + 60); }
  void set_gear_ratio_5(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t gear_ratio_5_address() const { return address + 60; }
  float gear_ratio_6() const { return guest::LoadFloat(base, address + 64); }
  void set_gear_ratio_6(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t gear_ratio_6_address() const { return address + 64; }
  float gear_ratio_7() const { return guest::LoadFloat(base, address + 68); }
  void set_gear_ratio_7(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t gear_ratio_7_address() const { return address + 68; }
  // signed setting bytes +72 to +94 (sub_820C05C8); spec +796, then +804
  uint8_t setting_72() const { return guest::LoadU8(base, address + 72); }
  void set_setting_72(uint8_t v) const { guest::StoreU8(base, address + 72, v); }
  uint32_t setting_72_address() const { return address + 72; }
  uint8_t setting_73() const { return guest::LoadU8(base, address + 73); }
  void set_setting_73(uint8_t v) const { guest::StoreU8(base, address + 73, v); }
  uint32_t setting_73_address() const { return address + 73; }
  // spec +800
  uint8_t setting_74() const { return guest::LoadU8(base, address + 74); }
  void set_setting_74(uint8_t v) const { guest::StoreU8(base, address + 74, v); }
  uint32_t setting_74_address() const { return address + 74; }
  // spec +808
  uint8_t setting_75() const { return guest::LoadU8(base, address + 75); }
  void set_setting_75(uint8_t v) const { guest::StoreU8(base, address + 75, v); }
  uint32_t setting_75_address() const { return address + 75; }
  // spec +632; 3 at the reset
  uint8_t setting_76() const { return guest::LoadU8(base, address + 76); }
  void set_setting_76(uint8_t v) const { guest::StoreU8(base, address + 76, v); }
  uint32_t setting_76_address() const { return address + 76; }
  // spec +636; 7 at the reset
  uint8_t setting_77() const { return guest::LoadU8(base, address + 77); }
  void set_setting_77(uint8_t v) const { guest::StoreU8(base, address + 77, v); }
  uint32_t setting_77_address() const { return address + 77; }
  uint8_t setting_78() const { return guest::LoadU8(base, address + 78); }
  void set_setting_78(uint8_t v) const { guest::StoreU8(base, address + 78, v); }
  uint32_t setting_78_address() const { return address + 78; }
  uint8_t setting_79() const { return guest::LoadU8(base, address + 79); }
  void set_setting_79(uint8_t v) const { guest::StoreU8(base, address + 79, v); }
  uint32_t setting_79_address() const { return address + 79; }
  // probably the front camber
  uint8_t camber_front() const { return guest::LoadU8(base, address + 80); }
  void set_camber_front(uint8_t v) const { guest::StoreU8(base, address + 80, v); }
  uint32_t camber_front_address() const { return address + 80; }
  // probably the rear camber
  uint8_t camber_rear() const { return guest::LoadU8(base, address + 81); }
  void set_camber_rear(uint8_t v) const { guest::StoreU8(base, address + 81, v); }
  uint32_t camber_rear_address() const { return address + 81; }
  uint8_t setting_82() const { return guest::LoadU8(base, address + 82); }
  void set_setting_82(uint8_t v) const { guest::StoreU8(base, address + 82, v); }
  uint32_t setting_82_address() const { return address + 82; }
  uint8_t setting_83() const { return guest::LoadU8(base, address + 83); }
  void set_setting_83(uint8_t v) const { guest::StoreU8(base, address + 83, v); }
  uint32_t setting_83_address() const { return address + 83; }
  uint8_t setting_84() const { return guest::LoadU8(base, address + 84); }
  void set_setting_84(uint8_t v) const { guest::StoreU8(base, address + 84, v); }
  uint32_t setting_84_address() const { return address + 84; }
  uint8_t setting_85() const { return guest::LoadU8(base, address + 85); }
  void set_setting_85(uint8_t v) const { guest::StoreU8(base, address + 85, v); }
  uint32_t setting_85_address() const { return address + 85; }
  uint8_t setting_86() const { return guest::LoadU8(base, address + 86); }
  void set_setting_86(uint8_t v) const { guest::StoreU8(base, address + 86, v); }
  uint32_t setting_86_address() const { return address + 86; }
  uint8_t setting_87() const { return guest::LoadU8(base, address + 87); }
  void set_setting_87(uint8_t v) const { guest::StoreU8(base, address + 87, v); }
  uint32_t setting_87_address() const { return address + 87; }
  uint8_t setting_88() const { return guest::LoadU8(base, address + 88); }
  void set_setting_88(uint8_t v) const { guest::StoreU8(base, address + 88, v); }
  uint32_t setting_88_address() const { return address + 88; }
  uint8_t setting_89() const { return guest::LoadU8(base, address + 89); }
  void set_setting_89(uint8_t v) const { guest::StoreU8(base, address + 89, v); }
  uint32_t setting_89_address() const { return address + 89; }
  uint8_t setting_90() const { return guest::LoadU8(base, address + 90); }
  void set_setting_90(uint8_t v) const { guest::StoreU8(base, address + 90, v); }
  uint32_t setting_90_address() const { return address + 90; }
  uint8_t setting_91() const { return guest::LoadU8(base, address + 91); }
  void set_setting_91(uint8_t v) const { guest::StoreU8(base, address + 91, v); }
  uint32_t setting_91_address() const { return address + 91; }
  // 1 for drive types other than 0 and 1
  uint8_t split_drive() const { return guest::LoadU8(base, address + 92); }
  void set_split_drive(uint8_t v) const { guest::StoreU8(base, address + 92, v); }
  uint32_t split_drive_address() const { return address + 92; }
  // probably the rear's torque share; 10 at the reset
  uint8_t rear_torque_share() const { return guest::LoadU8(base, address + 93); }
  void set_rear_torque_share(uint8_t v) const { guest::StoreU8(base, address + 93, v); }
  uint32_t rear_torque_share_address() const { return address + 93; }
  uint8_t setting_94() const { return guest::LoadU8(base, address + 94); }
  void set_setting_94(uint8_t v) const { guest::StoreU8(base, address + 94, v); }
  uint32_t setting_94_address() const { return address + 94; }
  // six 32-byte decals (CarDecal), probably the vinyls
  uint32_t decals6_address() const { return address + 96; }
  // forty 32-byte decals (CarDecal), probably the stickers
  uint32_t decals40_address() const { return address + 288; }
  // 20 bytes (sub_820CCC70; a build's +2768 as five words); what they are isn't known
  uint32_t extras_address() const { return address + 1568; }
  uint8_t exterior_item_2() const { return guest::LoadU8(base, address + 1568); }
  void set_exterior_item_2(uint8_t v) const { guest::StoreU8(base, address + 1568, v); }
  uint32_t exterior_item_2_address() const { return address + 1568; }
  // the bytes +1568 to +1571 as one word (Car::FromBuild)
  uint32_t extras_word_0() const { return guest::LoadU32(base, address + 1568); }
  void set_extras_word_0(uint32_t v) const { guest::StoreU32(base, address + 1568, v); }
  uint32_t extras_word_0_address() const { return address + 1568; }
  // 0 to 13 at random (Car::SetModel); Career::PickFromCar's pick
  uint8_t unknown_1569() const { return guest::LoadU8(base, address + 1569); }
  void set_unknown_1569(uint8_t v) const { guest::StoreU8(base, address + 1569, v); }
  uint32_t unknown_1569_address() const { return address + 1569; }
  // 5 or 3 by the displacement and sizes (Car::SetModel)
  uint8_t unknown_1570() const { return guest::LoadU8(base, address + 1570); }
  void set_unknown_1570(uint8_t v) const { guest::StoreU8(base, address + 1570, v); }
  uint32_t unknown_1570_address() const { return address + 1570; }
  // 0 or 3 by the displacement and sizes (Car::SetModel)
  uint8_t unknown_1571() const { return guest::LoadU8(base, address + 1571); }
  void set_unknown_1571(uint8_t v) const { guest::StoreU8(base, address + 1571, v); }
  uint32_t unknown_1571_address() const { return address + 1571; }
  uint8_t unknown_1572() const { return guest::LoadU8(base, address + 1572); }
  void set_unknown_1572(uint8_t v) const { guest::StoreU8(base, address + 1572, v); }
  uint32_t unknown_1572_address() const { return address + 1572; }
  uint32_t extras_word_1() const { return guest::LoadU32(base, address + 1572); }
  void set_extras_word_1(uint32_t v) const { guest::StoreU32(base, address + 1572, v); }
  uint32_t extras_word_1_address() const { return address + 1572; }
  // 1 to 10 at random (Car::SetModel)
  uint8_t unknown_1573() const { return guest::LoadU8(base, address + 1573); }
  void set_unknown_1573(uint8_t v) const { guest::StoreU8(base, address + 1573, v); }
  uint32_t unknown_1573_address() const { return address + 1573; }
  // 1 to 10 at random (Car::SetModel)
  uint8_t unknown_1574() const { return guest::LoadU8(base, address + 1574); }
  void set_unknown_1574(uint8_t v) const { guest::StoreU8(base, address + 1574, v); }
  uint32_t unknown_1574_address() const { return address + 1574; }
  // 1 to 10 at random (Car::SetModel)
  uint8_t unknown_1575() const { return guest::LoadU8(base, address + 1575); }
  void set_unknown_1575(uint8_t v) const { guest::StoreU8(base, address + 1575, v); }
  uint32_t unknown_1575_address() const { return address + 1575; }
  // 1 to 9 at random (Car::SetModel)
  uint8_t unknown_1576() const { return guest::LoadU8(base, address + 1576); }
  void set_unknown_1576(uint8_t v) const { guest::StoreU8(base, address + 1576, v); }
  uint32_t unknown_1576_address() const { return address + 1576; }
  uint8_t unknown_1577() const { return guest::LoadU8(base, address + 1577); }
  void set_unknown_1577(uint8_t v) const { guest::StoreU8(base, address + 1577, v); }
  uint32_t unknown_1577_address() const { return address + 1577; }
  uint8_t unknown_1578() const { return guest::LoadU8(base, address + 1578); }
  void set_unknown_1578(uint8_t v) const { guest::StoreU8(base, address + 1578, v); }
  uint32_t unknown_1578_address() const { return address + 1578; }
  uint8_t unknown_1579() const { return guest::LoadU8(base, address + 1579); }
  void set_unknown_1579(uint8_t v) const { guest::StoreU8(base, address + 1579, v); }
  uint32_t unknown_1579_address() const { return address + 1579; }
  uint32_t extras_word_2() const { return guest::LoadU32(base, address + 1576); }
  void set_extras_word_2(uint32_t v) const { guest::StoreU32(base, address + 1576, v); }
  uint32_t extras_word_2_address() const { return address + 1576; }
  // 1 for cars 56, 64 to 68 and 70
  uint8_t model_flag() const { return guest::LoadU8(base, address + 1580); }
  void set_model_flag(uint8_t v) const { guest::StoreU8(base, address + 1580, v); }
  uint32_t model_flag_address() const { return address + 1580; }
  uint32_t extras_word_3() const { return guest::LoadU32(base, address + 1580); }
  void set_extras_word_3(uint32_t v) const { guest::StoreU32(base, address + 1580, v); }
  uint32_t extras_word_3_address() const { return address + 1580; }
  uint32_t extras_word_4() const { return guest::LoadU32(base, address + 1584); }
  void set_extras_word_4(uint32_t v) const { guest::StoreU32(base, address + 1584, v); }
  uint32_t extras_word_4_address() const { return address + 1584; }
  // three 20-byte entries (CarLayer; probably the vinyl layers): two words, three floats
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
  uint32_t layer1_b() const { return guest::LoadU32(base, address + 1612); }
  void set_layer1_b(uint32_t v) const { guest::StoreU32(base, address + 1612, v); }
  uint32_t layer1_b_address() const { return address + 1612; }
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
  uint32_t layer2_b() const { return guest::LoadU32(base, address + 1632); }
  void set_layer2_b(uint32_t v) const { guest::StoreU32(base, address + 1632, v); }
  uint32_t layer2_b_address() const { return address + 1632; }
  float layer2_f8() const { return guest::LoadFloat(base, address + 1636); }
  void set_layer2_f8(float v) const { guest::StoreFloat(base, address + 1636, v); }
  uint32_t layer2_f8_address() const { return address + 1636; }
  float layer2_f12() const { return guest::LoadFloat(base, address + 1640); }
  void set_layer2_f12(float v) const { guest::StoreFloat(base, address + 1640, v); }
  uint32_t layer2_f12_address() const { return address + 1640; }
  float layer2_f16() const { return guest::LoadFloat(base, address + 1644); }
  void set_layer2_f16(float v) const { guest::StoreFloat(base, address + 1644, v); }
  uint32_t layer2_f16_address() const { return address + 1644; }
  // the tuning (CarTuning, 240 bytes)
  uint32_t tuning_address() const { return address + 1648; }
  // 0 at the reset; Car::SetModelAndClass stores it
  uint32_t tuning_head() const { return guest::LoadU32(base, address + 1648); }
  void set_tuning_head(uint32_t v) const { guest::StoreU32(base, address + 1648, v); }
  uint32_t tuning_head_address() const { return address + 1648; }
  // the spec's byte +709 != 0 (CarTuning::Reset's flag)
  uint32_t spec_flag() const { return guest::LoadU32(base, address + 1652); }
  void set_spec_flag(uint32_t v) const { guest::StoreU32(base, address + 1652, v); }
  uint32_t spec_flag_address() const { return address + 1652; }
  // Car::CopySpec copies spec + 980 + 204 x it
  uint32_t engine_stage() const { return guest::LoadU32(base, address + 1656); }
  void set_engine_stage(uint32_t v) const { guest::StoreU32(base, address + 1656, v); }
  uint32_t engine_stage_address() const { return address + 1656; }
  // (+4 == 1) at the reset
  uint32_t spec_flag_copy() const { return guest::LoadU32(base, address + 1660); }
  void set_spec_flag_copy(uint32_t v) const { guest::StoreU32(base, address + 1660, v); }
  uint32_t spec_flag_copy_address() const { return address + 1660; }
  // a power item's level
  uint32_t power_part_a() const { return guest::LoadU32(base, address + 1664); }
  void set_power_part_a(uint32_t v) const { guest::StoreU32(base, address + 1664, v); }
  uint32_t power_part_a_address() const { return address + 1664; }
  // the cooling module's type
  uint32_t cooling() const { return guest::LoadU32(base, address + 1668); }
  void set_cooling(uint32_t v) const { guest::StoreU32(base, address + 1668, v); }
  uint32_t cooling_address() const { return address + 1668; }
  // a power item's level
  uint32_t power_part_b() const { return guest::LoadU32(base, address + 1672); }
  void set_power_part_b(uint32_t v) const { guest::StoreU32(base, address + 1672, v); }
  uint32_t power_part_b_address() const { return address + 1672; }
  // a power item's level (probably the nitrous: the vehicle's +2416, +2420)
  uint32_t nitrous() const { return guest::LoadU32(base, address + 1676); }
  void set_nitrous(uint32_t v) const { guest::StoreU32(base, address + 1676, v); }
  uint32_t nitrous_address() const { return address + 1676; }
  // a power train level (spec +780)
  uint32_t drivetrain_32() const { return guest::LoadU32(base, address + 1680); }
  void set_drivetrain_32(uint32_t v) const { guest::StoreU32(base, address + 1680, v); }
  uint32_t drivetrain_32_address() const { return address + 1680; }
  // a power train level (spec +784)
  uint32_t drivetrain_36() const { return guest::LoadU32(base, address + 1684); }
  void set_drivetrain_36(uint32_t v) const { guest::StoreU32(base, address + 1684, v); }
  uint32_t drivetrain_36_address() const { return address + 1684; }
  // a power train level
  uint32_t drivetrain_40() const { return guest::LoadU32(base, address + 1688); }
  void set_drivetrain_40(uint32_t v) const { guest::StoreU32(base, address + 1688, v); }
  uint32_t drivetrain_40_address() const { return address + 1688; }
  // a power train level (probably the gear count)
  uint32_t gear_count() const { return guest::LoadU32(base, address + 1692); }
  void set_gear_count(uint32_t v) const { guest::StoreU32(base, address + 1692, v); }
  uint32_t gear_count_address() const { return address + 1692; }
  // a power train level
  uint32_t drivetrain_48() const { return guest::LoadU32(base, address + 1696); }
  void set_drivetrain_48(uint32_t v) const { guest::StoreU32(base, address + 1696, v); }
  uint32_t drivetrain_48_address() const { return address + 1696; }
  // the LSD's level split (spec +788)
  uint32_t lsd_a() const { return guest::LoadU32(base, address + 1700); }
  void set_lsd_a(uint32_t v) const { guest::StoreU32(base, address + 1700, v); }
  uint32_t lsd_a_address() const { return address + 1700; }
  // the LSD's level split (spec +792)
  uint32_t lsd_b() const { return guest::LoadU32(base, address + 1704); }
  void set_lsd_b(uint32_t v) const { guest::StoreU32(base, address + 1704, v); }
  uint32_t lsd_b_address() const { return address + 1704; }
  uint32_t body_level() const { return guest::LoadU32(base, address + 1708); }
  void set_body_level(uint32_t v) const { guest::StoreU32(base, address + 1708, v); }
  uint32_t body_level_address() const { return address + 1708; }
  // spec +884
  uint32_t front_tire_type() const { return guest::LoadU32(base, address + 1712); }
  void set_front_tire_type(uint32_t v) const { guest::StoreU32(base, address + 1712, v); }
  uint32_t front_tire_type_address() const { return address + 1712; }
  // spec +888
  uint32_t front_tire_grade() const { return guest::LoadU32(base, address + 1716); }
  void set_front_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 1716, v); }
  uint32_t front_tire_grade_address() const { return address + 1716; }
  // spec +892
  uint32_t rear_tire_type() const { return guest::LoadU32(base, address + 1720); }
  void set_rear_tire_type(uint32_t v) const { guest::StoreU32(base, address + 1720, v); }
  uint32_t rear_tire_type_address() const { return address + 1720; }
  // spec +896
  uint32_t rear_tire_grade() const { return guest::LoadU32(base, address + 1724); }
  void set_rear_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 1724, v); }
  uint32_t rear_tire_grade_address() const { return address + 1724; }
  // the front wheel number (0 stock)
  uint32_t front_wheel() const { return guest::LoadU32(base, address + 1728); }
  void set_front_wheel(uint32_t v) const { guest::StoreU32(base, address + 1728, v); }
  uint32_t front_wheel_address() const { return address + 1728; }
  uint32_t front_inch_up() const { return guest::LoadU32(base, address + 1732); }
  void set_front_inch_up(uint32_t v) const { guest::StoreU32(base, address + 1732, v); }
  uint32_t front_inch_up_address() const { return address + 1732; }
  // the rear wheel number (0 stock)
  uint32_t rear_wheel() const { return guest::LoadU32(base, address + 1736); }
  void set_rear_wheel(uint32_t v) const { guest::StoreU32(base, address + 1736, v); }
  uint32_t rear_wheel_address() const { return address + 1736; }
  uint32_t rear_inch_up() const { return guest::LoadU32(base, address + 1740); }
  void set_rear_inch_up(uint32_t v) const { guest::StoreU32(base, address + 1740, v); }
  uint32_t rear_inch_up_address() const { return address + 1740; }
  // an aero item's type (-1 read as 0 by Car::CopyAeroParts)
  uint32_t aero_0() const { return guest::LoadU32(base, address + 1744); }
  void set_aero_0(uint32_t v) const { guest::StoreU32(base, address + 1744, v); }
  uint32_t aero_0_address() const { return address + 1744; }
  // an aero item's type
  uint32_t aero_1() const { return guest::LoadU32(base, address + 1748); }
  void set_aero_1(uint32_t v) const { guest::StoreU32(base, address + 1748, v); }
  uint32_t aero_1_address() const { return address + 1748; }
  // an aero item's type
  uint32_t aero_2() const { return guest::LoadU32(base, address + 1752); }
  void set_aero_2(uint32_t v) const { guest::StoreU32(base, address + 1752, v); }
  uint32_t aero_2_address() const { return address + 1752; }
  // an aero item's type
  uint32_t aero_3() const { return guest::LoadU32(base, address + 1756); }
  void set_aero_3(uint32_t v) const { guest::StoreU32(base, address + 1756, v); }
  uint32_t aero_3_address() const { return address + 1756; }
  // the bonnet's type
  uint32_t bonnet() const { return guest::LoadU32(base, address + 1760); }
  void set_bonnet(uint32_t v) const { guest::StoreU32(base, address + 1760, v); }
  uint32_t bonnet_address() const { return address + 1760; }
  // 0 to 2: Car::CopyAeroParts' table 78 or 79
  uint32_t bonnet_color_kind() const { return guest::LoadU32(base, address + 1764); }
  void set_bonnet_color_kind(uint32_t v) const { guest::StoreU32(base, address + 1764, v); }
  uint32_t bonnet_color_kind_address() const { return address + 1764; }
  // an aero item's type
  uint32_t aero_6() const { return guest::LoadU32(base, address + 1768); }
  void set_aero_6(uint32_t v) const { guest::StoreU32(base, address + 1768, v); }
  uint32_t aero_6_address() const { return address + 1768; }
  // an aero item's type
  uint32_t aero_7() const { return guest::LoadU32(base, address + 1772); }
  void set_aero_7(uint32_t v) const { guest::StoreU32(base, address + 1772, v); }
  uint32_t aero_7_address() const { return address + 1772; }
  // an aero item's type
  uint32_t aero_8() const { return guest::LoadU32(base, address + 1776); }
  void set_aero_8(uint32_t v) const { guest::StoreU32(base, address + 1776, v); }
  uint32_t aero_8_address() const { return address + 1776; }
  // an aero item's type
  uint32_t aero_9() const { return guest::LoadU32(base, address + 1780); }
  void set_aero_9(uint32_t v) const { guest::StoreU32(base, address + 1780, v); }
  uint32_t aero_9_address() const { return address + 1780; }
  // an aero item's type
  uint32_t aero_10() const { return guest::LoadU32(base, address + 1784); }
  void set_aero_10(uint32_t v) const { guest::StoreU32(base, address + 1784, v); }
  uint32_t aero_10_address() const { return address + 1784; }
  // from a car build's +240 + 4 (Car::FromBuild)
  uint32_t unknown_140() const { return guest::LoadU32(base, address + 1788); }
  void set_unknown_140(uint32_t v) const { guest::StoreU32(base, address + 1788, v); }
  uint32_t unknown_140_address() const { return address + 1788; }
  uint32_t exterior_item_1() const { return guest::LoadU32(base, address + 1792); }
  void set_exterior_item_1(uint32_t v) const { guest::StoreU32(base, address + 1792, v); }
  uint32_t exterior_item_1_address() const { return address + 1792; }
  // spec +776
  uint32_t exterior_item_3() const { return guest::LoadU32(base, address + 1796); }
  void set_exterior_item_3(uint32_t v) const { guest::StoreU32(base, address + 1796, v); }
  uint32_t exterior_item_3_address() const { return address + 1796; }
  uint32_t interior_item_0() const { return guest::LoadU32(base, address + 1800); }
  void set_interior_item_0(uint32_t v) const { guest::StoreU32(base, address + 1800, v); }
  uint32_t interior_item_0_address() const { return address + 1800; }
  uint32_t interior_item_1() const { return guest::LoadU32(base, address + 1804); }
  void set_interior_item_1(uint32_t v) const { guest::StoreU32(base, address + 1804, v); }
  uint32_t interior_item_1_address() const { return address + 1804; }
  uint32_t interior_item_2() const { return guest::LoadU32(base, address + 1808); }
  void set_interior_item_2(uint32_t v) const { guest::StoreU32(base, address + 1808, v); }
  uint32_t interior_item_2_address() const { return address + 1808; }
  uint32_t interior_item_3() const { return guest::LoadU32(base, address + 1812); }
  void set_interior_item_3(uint32_t v) const { guest::StoreU32(base, address + 1812, v); }
  uint32_t interior_item_3_address() const { return address + 1812; }
  // 3 at the reset
  uint32_t interior_item_4() const { return guest::LoadU32(base, address + 1816); }
  void set_interior_item_4(uint32_t v) const { guest::StoreU32(base, address + 1816, v); }
  uint32_t interior_item_4_address() const { return address + 1816; }
  uint32_t interior_item_5() const { return guest::LoadU32(base, address + 1820); }
  void set_interior_item_5(uint32_t v) const { guest::StoreU32(base, address + 1820, v); }
  uint32_t interior_item_5_address() const { return address + 1820; }
  // one of five 12-byte entries (probably the stickers): 1 at the reset; spec +900 + 8i's +4 / 3 + 1
  uint32_t choice0_a() const { return guest::LoadU32(base, address + 1824); }
  void set_choice0_a(uint32_t v) const { guest::StoreU32(base, address + 1824, v); }
  uint32_t choice0_a_address() const { return address + 1824; }
  // spec +904 + 8i % 3
  uint32_t choice0_b() const { return guest::LoadU32(base, address + 1828); }
  void set_choice0_b(uint32_t v) const { guest::StoreU32(base, address + 1828, v); }
  uint32_t choice0_b_address() const { return address + 1828; }
  // spec +900 + 8i (0 none)
  uint32_t choice0_c() const { return guest::LoadU32(base, address + 1832); }
  void set_choice0_c(uint32_t v) const { guest::StoreU32(base, address + 1832, v); }
  uint32_t choice0_c_address() const { return address + 1832; }
  uint32_t choice1_a() const { return guest::LoadU32(base, address + 1836); }
  void set_choice1_a(uint32_t v) const { guest::StoreU32(base, address + 1836, v); }
  uint32_t choice1_a_address() const { return address + 1836; }
  uint32_t choice1_b() const { return guest::LoadU32(base, address + 1840); }
  void set_choice1_b(uint32_t v) const { guest::StoreU32(base, address + 1840, v); }
  uint32_t choice1_b_address() const { return address + 1840; }
  uint32_t choice1_c() const { return guest::LoadU32(base, address + 1844); }
  void set_choice1_c(uint32_t v) const { guest::StoreU32(base, address + 1844, v); }
  uint32_t choice1_c_address() const { return address + 1844; }
  uint32_t choice2_a() const { return guest::LoadU32(base, address + 1848); }
  void set_choice2_a(uint32_t v) const { guest::StoreU32(base, address + 1848, v); }
  uint32_t choice2_a_address() const { return address + 1848; }
  uint32_t choice2_b() const { return guest::LoadU32(base, address + 1852); }
  void set_choice2_b(uint32_t v) const { guest::StoreU32(base, address + 1852, v); }
  uint32_t choice2_b_address() const { return address + 1852; }
  uint32_t choice2_c() const { return guest::LoadU32(base, address + 1856); }
  void set_choice2_c(uint32_t v) const { guest::StoreU32(base, address + 1856, v); }
  uint32_t choice2_c_address() const { return address + 1856; }
  uint32_t choice3_a() const { return guest::LoadU32(base, address + 1860); }
  void set_choice3_a(uint32_t v) const { guest::StoreU32(base, address + 1860, v); }
  uint32_t choice3_a_address() const { return address + 1860; }
  uint32_t choice3_b() const { return guest::LoadU32(base, address + 1864); }
  void set_choice3_b(uint32_t v) const { guest::StoreU32(base, address + 1864, v); }
  uint32_t choice3_b_address() const { return address + 1864; }
  uint32_t choice3_c() const { return guest::LoadU32(base, address + 1868); }
  void set_choice3_c(uint32_t v) const { guest::StoreU32(base, address + 1868, v); }
  uint32_t choice3_c_address() const { return address + 1868; }
  uint32_t choice4_a() const { return guest::LoadU32(base, address + 1872); }
  void set_choice4_a(uint32_t v) const { guest::StoreU32(base, address + 1872, v); }
  uint32_t choice4_a_address() const { return address + 1872; }
  uint32_t choice4_b() const { return guest::LoadU32(base, address + 1876); }
  void set_choice4_b(uint32_t v) const { guest::StoreU32(base, address + 1876, v); }
  uint32_t choice4_b_address() const { return address + 1876; }
  uint32_t choice4_c() const { return guest::LoadU32(base, address + 1880); }
  void set_choice4_c(uint32_t v) const { guest::StoreU32(base, address + 1880, v); }
  uint32_t choice4_c_address() const { return address + 1880; }
  // a garage car's word (3, or the spec's +748, Car::SetModelAndClass); the measure files garage cars here
  uint32_t garage_class() const { return guest::LoadU32(base, address + 1884); }
  void set_garage_class(uint32_t v) const { guest::StoreU32(base, address + 1884, v); }
  uint32_t garage_class_address() const { return address + 1884; }
  // the spec's block 2 (Car::Refresh)
  uint32_t block2_address() const { return address + 1888; }
  // a garage car's word: the spec's +744, the price (Car::SetModelAndClass)
  uint32_t garage_price() const { return guest::LoadU32(base, address + 1900); }
  void set_garage_price(uint32_t v) const { guest::StoreU32(base, address + 1900, v); }
  uint32_t garage_price_address() const { return address + 1900; }
  // the spec's block 1
  uint32_t block1_address() const { return address + 2240; }
  // the spec's engine stage
  uint32_t engine_address() const { return address + 2880; }
  // the front tire, inched up
  uint32_t front_tire_address() const { return address + 3084; }
  float front_tire_width() const { return guest::LoadFloat(base, address + 3132); }
  void set_front_tire_width(float v) const { guest::StoreFloat(base, address + 3132, v); }
  uint32_t front_tire_width_address() const { return address + 3132; }
  // the rear tire, inched up
  uint32_t rear_tire_address() const { return address + 3176; }
  float rear_tire_width() const { return guest::LoadFloat(base, address + 3224); }
  void set_rear_tire_width(float v) const { guest::StoreFloat(base, address + 3224, v); }
  uint32_t rear_tire_width_address() const { return address + 3224; }
  uint32_t front_wheel_spec_address() const { return address + 3268; }
  uint32_t rear_wheel_spec_address() const { return address + 3316; }
  // the spec's paints
  uint32_t paints_address() const { return address + 3364; }
  // the aero parts (Car::CopyAeroParts)
  uint32_t aero_parts_address() const { return address + 3636; }
  // how much the inch-up widened the front tire
  float front_tire_widening() const { return guest::LoadFloat(base, address + 3876); }
  void set_front_tire_widening(float v) const { guest::StoreFloat(base, address + 3876, v); }
  uint32_t front_tire_widening_address() const { return address + 3876; }
  float rear_tire_widening() const { return guest::LoadFloat(base, address + 3880); }
  void set_rear_tire_widening(float v) const { guest::StoreFloat(base, address + 3880, v); }
  uint32_t rear_tire_widening_address() const { return address + 3880; }
  // 0x1 and 0x80 the human cars (the first and second added), 0x2 a rival or event racer, 0x4 traffic, 0x100 and 0x200 (event entries, rivals), 0x400 Race::AddRacer's flag, 0x800 a found rival
  uint32_t flags() const { return guest::LoadU32(base, address + 3888); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 3888, v); }
  uint32_t flags_address() const { return address + 3888; }
  // 1 after Car::Init; the save data's +0x1F870 for the player
  uint32_t unknown_3892() const { return guest::LoadU32(base, address + 3892); }
  void set_unknown_3892(uint32_t v) const { guest::StoreU32(base, address + 3892, v); }
  uint32_t unknown_3892_address() const { return address + 3892; }
  // a racer record's start position (section, slice); an event entry's +4
  uint32_t start_position() const { return guest::LoadU32(base, address + 3904); }
  void set_start_position(uint32_t v) const { guest::StoreU32(base, address + 3904, v); }
  uint32_t start_position_address() const { return address + 3904; }
  // -1 after RacerRecord::Init
  uint16_t start_section() const { return guest::LoadU16(base, address + 3904); }
  void set_start_section(uint16_t v) const { guest::StoreU16(base, address + 3904, v); }
  uint32_t start_section_address() const { return address + 3904; }
  // -1 after RacerRecord::Init
  uint16_t start_slice() const { return guest::LoadU16(base, address + 3906); }
  void set_start_slice(uint16_t v) const { guest::StoreU16(base, address + 3906, v); }
  uint32_t start_slice_address() const { return address + 3906; }
  // [0x8200692C], or 0.0 for stage types 2 and 3 (Racers::SetStart)
  float start_float() const { return guest::LoadFloat(base, address + 3908); }
  void set_start_float(float v) const { guest::StoreFloat(base, address + 3908, v); }
  uint32_t start_float_address() const { return address + 3908; }
  // the data tables' item (-1 none)
  uint32_t item_id() const { return guest::LoadU32(base, address + 3912); }
  void set_item_id(uint32_t v) const { guest::StoreU32(base, address + 3912, v); }
  uint32_t item_id_address() const { return address + 3912; }
  // the event's entry (-1 none)
  uint32_t entry_index() const { return guest::LoadU32(base, address + 3916); }
  void set_entry_index(uint32_t v) const { guest::StoreU32(base, address + 3916, v); }
  uint32_t entry_index_address() const { return address + 3916; }
  // Racers::Add's f1 (the player's: the garage car's +1892)
  float racer_value() const { return guest::LoadFloat(base, address + 3920); }
  void set_racer_value(float v) const { guest::StoreFloat(base, address + 3920, v); }
  uint32_t racer_value_address() const { return address + 3920; }
  // Racers::Add's r6: an item's byte +37, the player's save byte +8
  uint32_t racer_word() const { return guest::LoadU32(base, address + 3924); }
  void set_racer_word(uint32_t v) const { guest::StoreU32(base, address + 3924, v); }
  uint32_t racer_word_address() const { return address + 3924; }
  // measure fault: Car::DrawUpdate's scene model (+4288 its 736-byte block, to +5120) and Car::TickUpdate's car slots (+4288), filed as Car
  uint32_t fault_scene_model_address() const { return address + 4288; }
};

// embedded; measure: sub_820CC470.ret
struct CarDecal : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // 1.0 at the reset; a build's negative +0 mirrored (+28 = 1)
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
  // Object344::SetFloat16
  float angle() const { return guest::LoadFloat(base, address + 16); }
  void set_angle(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t angle_address() const { return address + 16; }
  // a build's halfword +22 << 16 | byte +27 << 8 | byte +31
  uint32_t packed() const { return guest::LoadU32(base, address + 20); }
  void set_packed(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t packed_address() const { return address + 20; }
  uint16_t packed_high() const { return guest::LoadU16(base, address + 20); }
  void set_packed_high(uint16_t v) const { guest::StoreU16(base, address + 20, v); }
  uint32_t packed_high_address() const { return address + 20; }
  uint8_t packed_byte_22() const { return guest::LoadU8(base, address + 22); }
  void set_packed_byte_22(uint8_t v) const { guest::StoreU8(base, address + 22, v); }
  uint32_t packed_byte_22_address() const { return address + 22; }
  uint8_t packed_byte_23() const { return guest::LoadU8(base, address + 23); }
  void set_packed_byte_23(uint8_t v) const { guest::StoreU8(base, address + 23, v); }
  uint32_t packed_byte_23_address() const { return address + 23; }
  // an image id or car model, -1 none
  uint32_t image() const { return guest::LoadU32(base, address + 24); }
  void set_image(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t image_address() const { return address + 24; }
  uint32_t mirrored() const { return guest::LoadU32(base, address + 28); }
  void set_mirrored(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t mirrored_address() const { return address + 28; }
};

// embedded; measure: -
struct CarLayer : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t a() const { return guest::LoadU32(base, address + 0); }
  void set_a(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t a_address() const { return address + 0; }
  uint32_t b() const { return guest::LoadU32(base, address + 4); }
  void set_b(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t b_address() const { return address + 4; }
  float f8() const { return guest::LoadFloat(base, address + 8); }
  void set_f8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t f8_address() const { return address + 8; }
  float f12() const { return guest::LoadFloat(base, address + 12); }
  void set_f12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t f12_address() const { return address + 12; }
  float f16() const { return guest::LoadFloat(base, address + 16); }
  void set_f16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t f16_address() const { return address + 16; }
};

// embedded; measure: -
struct CarChoice : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 1 at the reset; the spec's next word / 3 + 1
  uint32_t a() const { return guest::LoadU32(base, address + 0); }
  void set_a(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t a_address() const { return address + 0; }
  // the spec's next word % 3
  uint32_t b() const { return guest::LoadU32(base, address + 4); }
  void set_b(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t b_address() const { return address + 4; }
  // the spec's word (0 none)
  uint32_t c() const { return guest::LoadU32(base, address + 8); }
  void set_c(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t c_address() const { return address + 8; }
};

// embedded; measure: Car@+1648
struct CarTuning : View {
  using View::View;
  static constexpr uint32_t kSize = 240;
  // 0 at the reset; Car::SetModelAndClass stores it
  uint32_t tuning_head() const { return guest::LoadU32(base, address + 0); }
  void set_tuning_head(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tuning_head_address() const { return address + 0; }
  // the spec's byte +709 != 0 (CarTuning::Reset's flag)
  uint32_t spec_flag() const { return guest::LoadU32(base, address + 4); }
  void set_spec_flag(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t spec_flag_address() const { return address + 4; }
  // Car::CopySpec copies spec + 980 + 204 x it
  uint32_t engine_stage() const { return guest::LoadU32(base, address + 8); }
  void set_engine_stage(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t engine_stage_address() const { return address + 8; }
  // (+4 == 1) at the reset
  uint32_t spec_flag_copy() const { return guest::LoadU32(base, address + 12); }
  void set_spec_flag_copy(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t spec_flag_copy_address() const { return address + 12; }
  // a power item's level
  uint32_t power_part_a() const { return guest::LoadU32(base, address + 16); }
  void set_power_part_a(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t power_part_a_address() const { return address + 16; }
  // the cooling module's type
  uint32_t cooling() const { return guest::LoadU32(base, address + 20); }
  void set_cooling(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t cooling_address() const { return address + 20; }
  // a power item's level
  uint32_t power_part_b() const { return guest::LoadU32(base, address + 24); }
  void set_power_part_b(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t power_part_b_address() const { return address + 24; }
  // a power item's level (probably the nitrous: the vehicle's +2416, +2420)
  uint32_t nitrous() const { return guest::LoadU32(base, address + 28); }
  void set_nitrous(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t nitrous_address() const { return address + 28; }
  // a power train level (spec +780)
  uint32_t drivetrain_32() const { return guest::LoadU32(base, address + 32); }
  void set_drivetrain_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t drivetrain_32_address() const { return address + 32; }
  // a power train level (spec +784)
  uint32_t drivetrain_36() const { return guest::LoadU32(base, address + 36); }
  void set_drivetrain_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t drivetrain_36_address() const { return address + 36; }
  // a power train level
  uint32_t drivetrain_40() const { return guest::LoadU32(base, address + 40); }
  void set_drivetrain_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t drivetrain_40_address() const { return address + 40; }
  // a power train level (probably the gear count)
  uint32_t gear_count() const { return guest::LoadU32(base, address + 44); }
  void set_gear_count(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t gear_count_address() const { return address + 44; }
  // a power train level
  uint32_t drivetrain_48() const { return guest::LoadU32(base, address + 48); }
  void set_drivetrain_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t drivetrain_48_address() const { return address + 48; }
  // the LSD's level split (spec +788)
  uint32_t lsd_a() const { return guest::LoadU32(base, address + 52); }
  void set_lsd_a(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t lsd_a_address() const { return address + 52; }
  // the LSD's level split (spec +792)
  uint32_t lsd_b() const { return guest::LoadU32(base, address + 56); }
  void set_lsd_b(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t lsd_b_address() const { return address + 56; }
  uint32_t body_level() const { return guest::LoadU32(base, address + 60); }
  void set_body_level(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t body_level_address() const { return address + 60; }
  // spec +884
  uint32_t front_tire_type() const { return guest::LoadU32(base, address + 64); }
  void set_front_tire_type(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t front_tire_type_address() const { return address + 64; }
  // spec +888
  uint32_t front_tire_grade() const { return guest::LoadU32(base, address + 68); }
  void set_front_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t front_tire_grade_address() const { return address + 68; }
  // spec +892
  uint32_t rear_tire_type() const { return guest::LoadU32(base, address + 72); }
  void set_rear_tire_type(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t rear_tire_type_address() const { return address + 72; }
  // spec +896
  uint32_t rear_tire_grade() const { return guest::LoadU32(base, address + 76); }
  void set_rear_tire_grade(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t rear_tire_grade_address() const { return address + 76; }
  // the front wheel number (0 stock)
  uint32_t front_wheel() const { return guest::LoadU32(base, address + 80); }
  void set_front_wheel(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t front_wheel_address() const { return address + 80; }
  uint32_t front_inch_up() const { return guest::LoadU32(base, address + 84); }
  void set_front_inch_up(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t front_inch_up_address() const { return address + 84; }
  // the rear wheel number (0 stock)
  uint32_t rear_wheel() const { return guest::LoadU32(base, address + 88); }
  void set_rear_wheel(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t rear_wheel_address() const { return address + 88; }
  uint32_t rear_inch_up() const { return guest::LoadU32(base, address + 92); }
  void set_rear_inch_up(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t rear_inch_up_address() const { return address + 92; }
  // an aero item's type (-1 read as 0 by Car::CopyAeroParts)
  uint32_t aero_0() const { return guest::LoadU32(base, address + 96); }
  void set_aero_0(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t aero_0_address() const { return address + 96; }
  // an aero item's type
  uint32_t aero_1() const { return guest::LoadU32(base, address + 100); }
  void set_aero_1(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t aero_1_address() const { return address + 100; }
  // an aero item's type
  uint32_t aero_2() const { return guest::LoadU32(base, address + 104); }
  void set_aero_2(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t aero_2_address() const { return address + 104; }
  // an aero item's type
  uint32_t aero_3() const { return guest::LoadU32(base, address + 108); }
  void set_aero_3(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t aero_3_address() const { return address + 108; }
  // the bonnet's type
  uint32_t bonnet() const { return guest::LoadU32(base, address + 112); }
  void set_bonnet(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t bonnet_address() const { return address + 112; }
  // 0 to 2: Car::CopyAeroParts' table 78 or 79
  uint32_t bonnet_color_kind() const { return guest::LoadU32(base, address + 116); }
  void set_bonnet_color_kind(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t bonnet_color_kind_address() const { return address + 116; }
  // an aero item's type
  uint32_t aero_6() const { return guest::LoadU32(base, address + 120); }
  void set_aero_6(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t aero_6_address() const { return address + 120; }
  // an aero item's type
  uint32_t aero_7() const { return guest::LoadU32(base, address + 124); }
  void set_aero_7(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t aero_7_address() const { return address + 124; }
  // an aero item's type
  uint32_t aero_8() const { return guest::LoadU32(base, address + 128); }
  void set_aero_8(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t aero_8_address() const { return address + 128; }
  // an aero item's type
  uint32_t aero_9() const { return guest::LoadU32(base, address + 132); }
  void set_aero_9(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t aero_9_address() const { return address + 132; }
  // an aero item's type
  uint32_t aero_10() const { return guest::LoadU32(base, address + 136); }
  void set_aero_10(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t aero_10_address() const { return address + 136; }
  // from a car build's +240 + 4 (Car::FromBuild)
  uint32_t unknown_140() const { return guest::LoadU32(base, address + 140); }
  void set_unknown_140(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  uint32_t exterior_item_1() const { return guest::LoadU32(base, address + 144); }
  void set_exterior_item_1(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t exterior_item_1_address() const { return address + 144; }
  // spec +776
  uint32_t exterior_item_3() const { return guest::LoadU32(base, address + 148); }
  void set_exterior_item_3(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t exterior_item_3_address() const { return address + 148; }
  uint32_t interior_item_0() const { return guest::LoadU32(base, address + 152); }
  void set_interior_item_0(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t interior_item_0_address() const { return address + 152; }
  uint32_t interior_item_1() const { return guest::LoadU32(base, address + 156); }
  void set_interior_item_1(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t interior_item_1_address() const { return address + 156; }
  uint32_t interior_item_2() const { return guest::LoadU32(base, address + 160); }
  void set_interior_item_2(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t interior_item_2_address() const { return address + 160; }
  uint32_t interior_item_3() const { return guest::LoadU32(base, address + 164); }
  void set_interior_item_3(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t interior_item_3_address() const { return address + 164; }
  // 3 at the reset
  uint32_t interior_item_4() const { return guest::LoadU32(base, address + 168); }
  void set_interior_item_4(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t interior_item_4_address() const { return address + 168; }
  uint32_t interior_item_5() const { return guest::LoadU32(base, address + 172); }
  void set_interior_item_5(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t interior_item_5_address() const { return address + 172; }
  // one of five 12-byte entries (probably the stickers): 1 at the reset; spec +900 + 8i's +4 / 3 + 1
  uint32_t choice0_a() const { return guest::LoadU32(base, address + 176); }
  void set_choice0_a(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t choice0_a_address() const { return address + 176; }
  // spec +904 + 8i % 3
  uint32_t choice0_b() const { return guest::LoadU32(base, address + 180); }
  void set_choice0_b(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t choice0_b_address() const { return address + 180; }
  // spec +900 + 8i (0 none)
  uint32_t choice0_c() const { return guest::LoadU32(base, address + 184); }
  void set_choice0_c(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t choice0_c_address() const { return address + 184; }
  uint32_t choice1_a() const { return guest::LoadU32(base, address + 188); }
  void set_choice1_a(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t choice1_a_address() const { return address + 188; }
  uint32_t choice1_b() const { return guest::LoadU32(base, address + 192); }
  void set_choice1_b(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t choice1_b_address() const { return address + 192; }
  uint32_t choice1_c() const { return guest::LoadU32(base, address + 196); }
  void set_choice1_c(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t choice1_c_address() const { return address + 196; }
  uint32_t choice2_a() const { return guest::LoadU32(base, address + 200); }
  void set_choice2_a(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t choice2_a_address() const { return address + 200; }
  uint32_t choice2_b() const { return guest::LoadU32(base, address + 204); }
  void set_choice2_b(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t choice2_b_address() const { return address + 204; }
  uint32_t choice2_c() const { return guest::LoadU32(base, address + 208); }
  void set_choice2_c(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t choice2_c_address() const { return address + 208; }
  uint32_t choice3_a() const { return guest::LoadU32(base, address + 212); }
  void set_choice3_a(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t choice3_a_address() const { return address + 212; }
  uint32_t choice3_b() const { return guest::LoadU32(base, address + 216); }
  void set_choice3_b(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t choice3_b_address() const { return address + 216; }
  uint32_t choice3_c() const { return guest::LoadU32(base, address + 220); }
  void set_choice3_c(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t choice3_c_address() const { return address + 220; }
  uint32_t choice4_a() const { return guest::LoadU32(base, address + 224); }
  void set_choice4_a(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t choice4_a_address() const { return address + 224; }
  uint32_t choice4_b() const { return guest::LoadU32(base, address + 228); }
  void set_choice4_b(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t choice4_b_address() const { return address + 228; }
  uint32_t choice4_c() const { return guest::LoadU32(base, address + 232); }
  void set_choice4_c(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t choice4_c_address() const { return address + 232; }
};

// embedded; measure: [826C5B2C]@+4, Route::Reset.r3
struct Route : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // a course route's index (CourseRoutes::Get)
  uint32_t course_route() const { return guest::LoadU32(base, address + 0); }
  void set_course_route(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t course_route_address() const { return address + 0; }
  // the index + 20 when given 48; 20 at the reset
  uint32_t location() const { return guest::LoadU32(base, address + 4); }
  void set_location(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t location_address() const { return address + 4; }
  // the course route's first position (section, slice)
  uint32_t first_position() const { return guest::LoadU32(base, address + 8); }
  void set_first_position(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t first_position_address() const { return address + 8; }
  // the course route's last position (sub_820FFEB0)
  uint16_t last_section() const { return guest::LoadU16(base, address + 12); }
  void set_last_section(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t last_section_address() const { return address + 12; }
  uint16_t last_slice() const { return guest::LoadU16(base, address + 14); }
  void set_last_slice(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t last_slice_address() const { return address + 14; }
};

// embedded; measure: Route::Set.ret
struct RouteSetPosition : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // both halfwords as one word
  uint32_t position() const { return guest::LoadU32(base, address + 0); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t position_address() const { return address + 0; }
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  uint16_t slice() const { return guest::LoadU16(base, address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
};

// embedded; measure: RunningMean, RunningMean::Merge.r3
struct RunningMean : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  float mean() const { return guest::LoadFloat(base, address + 0); }
  void set_mean(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t mean_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(base, address + 4); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t count_address() const { return address + 4; }
};

// embedded; measure: sub_820FF558.r3
struct LapRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  // cleared at the reset (memset)
  uint32_t head_address() const { return address + 0; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(base, address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(base, address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(base, address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
};

// embedded; measure: sub_82101D20.r3, sub_82101F68.r3@+4
struct RaceDataEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 972;
  // -1 at the reset
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // -1 at the reset
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // -1 at the reset
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // ten LapRecords
  uint32_t laps_address() const { return address + 20; }
  uint32_t unknown_780() const { return guest::LoadU32(base, address + 780); }
  void set_unknown_780(uint32_t v) const { guest::StoreU32(base, address + 780, v); }
  uint32_t unknown_780_address() const { return address + 780; }
  // 0, then 12000 (sub_82101F68)
  uint32_t unknown_784() const { return guest::LoadU32(base, address + 784); }
  void set_unknown_784(uint32_t v) const { guest::StoreU32(base, address + 784, v); }
  uint32_t unknown_784_address() const { return address + 784; }
  // 12000 (sub_82101F68)
  uint32_t unknown_788() const { return guest::LoadU32(base, address + 788); }
  void set_unknown_788(uint32_t v) const { guest::StoreU32(base, address + 788, v); }
  uint32_t unknown_788_address() const { return address + 788; }
  // the running mean as one doubleword
  uint64_t mean_pair() const { return guest::LoadU64(base, address + 792); }
  void set_mean_pair(uint64_t v) const { guest::StoreU64(base, address + 792, v); }
  uint32_t mean_pair_address() const { return address + 792; }
  // a RunningMean: (0.0, 1) at the reset
  float mean() const { return guest::LoadFloat(base, address + 792); }
  void set_mean(float v) const { guest::StoreFloat(base, address + 792, v); }
  uint32_t mean_address() const { return address + 792; }
  uint32_t mean_count() const { return guest::LoadU32(base, address + 796); }
  void set_mean_count(uint32_t v) const { guest::StoreU32(base, address + 796, v); }
  uint32_t mean_count_address() const { return address + 796; }
  float unknown_800() const { return guest::LoadFloat(base, address + 800); }
  void set_unknown_800(float v) const { guest::StoreFloat(base, address + 800, v); }
  uint32_t unknown_800_address() const { return address + 800; }
  uint32_t unknown_804() const { return guest::LoadU32(base, address + 804); }
  void set_unknown_804(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t unknown_804_address() const { return address + 804; }
  uint32_t unknown_808() const { return guest::LoadU32(base, address + 808); }
  void set_unknown_808(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t unknown_808_address() const { return address + 808; }
  uint32_t unknown_812() const { return guest::LoadU32(base, address + 812); }
  void set_unknown_812(uint32_t v) const { guest::StoreU32(base, address + 812, v); }
  uint32_t unknown_812_address() const { return address + 812; }
  float unknown_816() const { return guest::LoadFloat(base, address + 816); }
  void set_unknown_816(float v) const { guest::StoreFloat(base, address + 816, v); }
  uint32_t unknown_816_address() const { return address + 816; }
  float unknown_820() const { return guest::LoadFloat(base, address + 820); }
  void set_unknown_820(float v) const { guest::StoreFloat(base, address + 820, v); }
  uint32_t unknown_820_address() const { return address + 820; }
  float unknown_824() const { return guest::LoadFloat(base, address + 824); }
  void set_unknown_824(float v) const { guest::StoreFloat(base, address + 824, v); }
  uint32_t unknown_824_address() const { return address + 824; }
  float unknown_828() const { return guest::LoadFloat(base, address + 828); }
  void set_unknown_828(float v) const { guest::StoreFloat(base, address + 828, v); }
  uint32_t unknown_828_address() const { return address + 828; }
  uint32_t unknown_832() const { return guest::LoadU32(base, address + 832); }
  void set_unknown_832(uint32_t v) const { guest::StoreU32(base, address + 832, v); }
  uint32_t unknown_832_address() const { return address + 832; }
  uint16_t unknown_964() const { return guest::LoadU16(base, address + 964); }
  void set_unknown_964(uint16_t v) const { guest::StoreU16(base, address + 964, v); }
  uint32_t unknown_964_address() const { return address + 964; }
  uint32_t unknown_968() const { return guest::LoadU32(base, address + 968); }
  void set_unknown_968(uint32_t v) const { guest::StoreU32(base, address + 968, v); }
  uint32_t unknown_968_address() const { return address + 968; }
};

// embedded; measure: sub_82101F68.r3
struct RaceDataRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 3936;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // four RaceDataEntries
  uint32_t entries_address() const { return address + 4; }
  // entry 0's +784 (12000 at the reset)
  uint32_t entry0_784() const { return guest::LoadU32(base, address + 788); }
  void set_entry0_784(uint32_t v) const { guest::StoreU32(base, address + 788, v); }
  uint32_t entry0_784_address() const { return address + 788; }
  // entry 0's +788 (12000 at the reset)
  uint32_t entry0_788() const { return guest::LoadU32(base, address + 792); }
  void set_entry0_788(uint32_t v) const { guest::StoreU32(base, address + 792, v); }
  uint32_t entry0_788_address() const { return address + 792; }
  // a Route
  uint32_t route_address() const { return address + 3892; }
  // 1 at the reset
  uint32_t unknown_3908() const { return guest::LoadU32(base, address + 3908); }
  void set_unknown_3908(uint32_t v) const { guest::StoreU32(base, address + 3908, v); }
  uint32_t unknown_3908_address() const { return address + 3908; }
  uint32_t unknown_3912() const { return guest::LoadU32(base, address + 3912); }
  void set_unknown_3912(uint32_t v) const { guest::StoreU32(base, address + 3912, v); }
  uint32_t unknown_3912_address() const { return address + 3912; }
  uint32_t unknown_3920() const { return guest::LoadU32(base, address + 3920); }
  void set_unknown_3920(uint32_t v) const { guest::StoreU32(base, address + 3920, v); }
  uint32_t unknown_3920_address() const { return address + 3920; }
  uint32_t unknown_3924() const { return guest::LoadU32(base, address + 3924); }
  void set_unknown_3924(uint32_t v) const { guest::StoreU32(base, address + 3924, v); }
  uint32_t unknown_3924_address() const { return address + 3924; }
  uint32_t unknown_3932() const { return guest::LoadU32(base, address + 3932); }
  void set_unknown_3932(uint32_t v) const { guest::StoreU32(base, address + 3932, v); }
  uint32_t unknown_3932_address() const { return address + 3932; }
};

// embedded; measure: Car@+1568
struct CarAt1568 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
  uint8_t unknown_2() const { return guest::LoadU8(base, address + 2); }
  void set_unknown_2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint8_t unknown_3() const { return guest::LoadU8(base, address + 3); }
  void set_unknown_3(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t unknown_3_address() const { return address + 3; }
  uint8_t unknown_4() const { return guest::LoadU8(base, address + 4); }
  void set_unknown_4(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint8_t unknown_5() const { return guest::LoadU8(base, address + 5); }
  void set_unknown_5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t unknown_5_address() const { return address + 5; }
  uint8_t unknown_6() const { return guest::LoadU8(base, address + 6); }
  void set_unknown_6(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint8_t unknown_7() const { return guest::LoadU8(base, address + 7); }
  void set_unknown_7(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t unknown_7_address() const { return address + 7; }
  uint8_t unknown_8() const { return guest::LoadU8(base, address + 8); }
  void set_unknown_8(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint8_t unknown_9() const { return guest::LoadU8(base, address + 9); }
  void set_unknown_9(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t unknown_9_address() const { return address + 9; }
  uint8_t unknown_10() const { return guest::LoadU8(base, address + 10); }
  void set_unknown_10(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
  uint8_t unknown_11() const { return guest::LoadU8(base, address + 11); }
  void set_unknown_11(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t unknown_11_address() const { return address + 11; }
  uint8_t unknown_12() const { return guest::LoadU8(base, address + 12); }
  void set_unknown_12(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: Car@+16
struct CarAt16 : View {
  using View::View;
  static constexpr uint32_t kSize = 79;
  uint16_t unknown_0() const { return guest::LoadU16(base, address + 0); }
  void set_unknown_0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint8_t unknown_56() const { return guest::LoadU8(base, address + 56); }
  void set_unknown_56(uint8_t v) const { guest::StoreU8(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint8_t unknown_57() const { return guest::LoadU8(base, address + 57); }
  void set_unknown_57(uint8_t v) const { guest::StoreU8(base, address + 57, v); }
  uint32_t unknown_57_address() const { return address + 57; }
  uint8_t unknown_58() const { return guest::LoadU8(base, address + 58); }
  void set_unknown_58(uint8_t v) const { guest::StoreU8(base, address + 58, v); }
  uint32_t unknown_58_address() const { return address + 58; }
  uint8_t unknown_59() const { return guest::LoadU8(base, address + 59); }
  void set_unknown_59(uint8_t v) const { guest::StoreU8(base, address + 59, v); }
  uint32_t unknown_59_address() const { return address + 59; }
  uint8_t unknown_60() const { return guest::LoadU8(base, address + 60); }
  void set_unknown_60(uint8_t v) const { guest::StoreU8(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint8_t unknown_61() const { return guest::LoadU8(base, address + 61); }
  void set_unknown_61(uint8_t v) const { guest::StoreU8(base, address + 61, v); }
  uint32_t unknown_61_address() const { return address + 61; }
  uint8_t unknown_62() const { return guest::LoadU8(base, address + 62); }
  void set_unknown_62(uint8_t v) const { guest::StoreU8(base, address + 62, v); }
  uint32_t unknown_62_address() const { return address + 62; }
  uint8_t unknown_63() const { return guest::LoadU8(base, address + 63); }
  void set_unknown_63(uint8_t v) const { guest::StoreU8(base, address + 63, v); }
  uint32_t unknown_63_address() const { return address + 63; }
  uint8_t unknown_64() const { return guest::LoadU8(base, address + 64); }
  void set_unknown_64(uint8_t v) const { guest::StoreU8(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint8_t unknown_65() const { return guest::LoadU8(base, address + 65); }
  void set_unknown_65(uint8_t v) const { guest::StoreU8(base, address + 65, v); }
  uint32_t unknown_65_address() const { return address + 65; }
  uint8_t unknown_66() const { return guest::LoadU8(base, address + 66); }
  void set_unknown_66(uint8_t v) const { guest::StoreU8(base, address + 66, v); }
  uint32_t unknown_66_address() const { return address + 66; }
  uint8_t unknown_67() const { return guest::LoadU8(base, address + 67); }
  void set_unknown_67(uint8_t v) const { guest::StoreU8(base, address + 67, v); }
  uint32_t unknown_67_address() const { return address + 67; }
  uint8_t unknown_68() const { return guest::LoadU8(base, address + 68); }
  void set_unknown_68(uint8_t v) const { guest::StoreU8(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint8_t unknown_69() const { return guest::LoadU8(base, address + 69); }
  void set_unknown_69(uint8_t v) const { guest::StoreU8(base, address + 69, v); }
  uint32_t unknown_69_address() const { return address + 69; }
  uint8_t unknown_70() const { return guest::LoadU8(base, address + 70); }
  void set_unknown_70(uint8_t v) const { guest::StoreU8(base, address + 70, v); }
  uint32_t unknown_70_address() const { return address + 70; }
  uint8_t unknown_71() const { return guest::LoadU8(base, address + 71); }
  void set_unknown_71(uint8_t v) const { guest::StoreU8(base, address + 71, v); }
  uint32_t unknown_71_address() const { return address + 71; }
  uint8_t unknown_72() const { return guest::LoadU8(base, address + 72); }
  void set_unknown_72(uint8_t v) const { guest::StoreU8(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint8_t unknown_73() const { return guest::LoadU8(base, address + 73); }
  void set_unknown_73(uint8_t v) const { guest::StoreU8(base, address + 73, v); }
  uint32_t unknown_73_address() const { return address + 73; }
  uint8_t unknown_74() const { return guest::LoadU8(base, address + 74); }
  void set_unknown_74(uint8_t v) const { guest::StoreU8(base, address + 74, v); }
  uint32_t unknown_74_address() const { return address + 74; }
  uint8_t unknown_75() const { return guest::LoadU8(base, address + 75); }
  void set_unknown_75(uint8_t v) const { guest::StoreU8(base, address + 75, v); }
  uint32_t unknown_75_address() const { return address + 75; }
  uint8_t unknown_76() const { return guest::LoadU8(base, address + 76); }
  void set_unknown_76(uint8_t v) const { guest::StoreU8(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint8_t unknown_77() const { return guest::LoadU8(base, address + 77); }
  void set_unknown_77(uint8_t v) const { guest::StoreU8(base, address + 77, v); }
  uint32_t unknown_77_address() const { return address + 77; }
  uint8_t unknown_78() const { return guest::LoadU8(base, address + 78); }
  void set_unknown_78(uint8_t v) const { guest::StoreU8(base, address + 78, v); }
  uint32_t unknown_78_address() const { return address + 78; }
};

// embedded; measure: Car@+288
struct CarAt288 : View {
  using View::View;
  static constexpr uint32_t kSize = 1152;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint16_t unknown_20() const { return guest::LoadU16(base, address + 20); }
  void set_unknown_20(uint16_t v) const { guest::StoreU16(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint8_t unknown_22() const { return guest::LoadU8(base, address + 22); }
  void set_unknown_22(uint8_t v) const { guest::StoreU8(base, address + 22, v); }
  uint32_t unknown_22_address() const { return address + 22; }
  uint8_t unknown_23() const { return guest::LoadU8(base, address + 23); }
  void set_unknown_23(uint8_t v) const { guest::StoreU8(base, address + 23, v); }
  uint32_t unknown_23_address() const { return address + 23; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  float unknown_160() const { return guest::LoadFloat(base, address + 160); }
  void set_unknown_160(float v) const { guest::StoreFloat(base, address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  float unknown_164() const { return guest::LoadFloat(base, address + 164); }
  void set_unknown_164(float v) const { guest::StoreFloat(base, address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  float unknown_168() const { return guest::LoadFloat(base, address + 168); }
  void set_unknown_168(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t unknown_168_address() const { return address + 168; }
  float unknown_172() const { return guest::LoadFloat(base, address + 172); }
  void set_unknown_172(float v) const { guest::StoreFloat(base, address + 172, v); }
  uint32_t unknown_172_address() const { return address + 172; }
  float unknown_176() const { return guest::LoadFloat(base, address + 176); }
  void set_unknown_176(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
  uint16_t unknown_180() const { return guest::LoadU16(base, address + 180); }
  void set_unknown_180(uint16_t v) const { guest::StoreU16(base, address + 180, v); }
  uint32_t unknown_180_address() const { return address + 180; }
  uint8_t unknown_182() const { return guest::LoadU8(base, address + 182); }
  void set_unknown_182(uint8_t v) const { guest::StoreU8(base, address + 182, v); }
  uint32_t unknown_182_address() const { return address + 182; }
  uint8_t unknown_183() const { return guest::LoadU8(base, address + 183); }
  void set_unknown_183(uint8_t v) const { guest::StoreU8(base, address + 183, v); }
  uint32_t unknown_183_address() const { return address + 183; }
  uint32_t unknown_184() const { return guest::LoadU32(base, address + 184); }
  void set_unknown_184(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t unknown_184_address() const { return address + 184; }
  uint32_t unknown_188() const { return guest::LoadU32(base, address + 188); }
  void set_unknown_188(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t unknown_188_address() const { return address + 188; }
  float unknown_320() const { return guest::LoadFloat(base, address + 320); }
  void set_unknown_320(float v) const { guest::StoreFloat(base, address + 320, v); }
  uint32_t unknown_320_address() const { return address + 320; }
  float unknown_324() const { return guest::LoadFloat(base, address + 324); }
  void set_unknown_324(float v) const { guest::StoreFloat(base, address + 324, v); }
  uint32_t unknown_324_address() const { return address + 324; }
  float unknown_328() const { return guest::LoadFloat(base, address + 328); }
  void set_unknown_328(float v) const { guest::StoreFloat(base, address + 328, v); }
  uint32_t unknown_328_address() const { return address + 328; }
  float unknown_332() const { return guest::LoadFloat(base, address + 332); }
  void set_unknown_332(float v) const { guest::StoreFloat(base, address + 332, v); }
  uint32_t unknown_332_address() const { return address + 332; }
  float unknown_336() const { return guest::LoadFloat(base, address + 336); }
  void set_unknown_336(float v) const { guest::StoreFloat(base, address + 336, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  uint16_t unknown_340() const { return guest::LoadU16(base, address + 340); }
  void set_unknown_340(uint16_t v) const { guest::StoreU16(base, address + 340, v); }
  uint32_t unknown_340_address() const { return address + 340; }
  uint8_t unknown_342() const { return guest::LoadU8(base, address + 342); }
  void set_unknown_342(uint8_t v) const { guest::StoreU8(base, address + 342, v); }
  uint32_t unknown_342_address() const { return address + 342; }
  uint8_t unknown_343() const { return guest::LoadU8(base, address + 343); }
  void set_unknown_343(uint8_t v) const { guest::StoreU8(base, address + 343, v); }
  uint32_t unknown_343_address() const { return address + 343; }
  uint32_t unknown_344() const { return guest::LoadU32(base, address + 344); }
  void set_unknown_344(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t unknown_344_address() const { return address + 344; }
  uint32_t unknown_348() const { return guest::LoadU32(base, address + 348); }
  void set_unknown_348(uint32_t v) const { guest::StoreU32(base, address + 348, v); }
  uint32_t unknown_348_address() const { return address + 348; }
  float unknown_480() const { return guest::LoadFloat(base, address + 480); }
  void set_unknown_480(float v) const { guest::StoreFloat(base, address + 480, v); }
  uint32_t unknown_480_address() const { return address + 480; }
  float unknown_484() const { return guest::LoadFloat(base, address + 484); }
  void set_unknown_484(float v) const { guest::StoreFloat(base, address + 484, v); }
  uint32_t unknown_484_address() const { return address + 484; }
  float unknown_488() const { return guest::LoadFloat(base, address + 488); }
  void set_unknown_488(float v) const { guest::StoreFloat(base, address + 488, v); }
  uint32_t unknown_488_address() const { return address + 488; }
  float unknown_492() const { return guest::LoadFloat(base, address + 492); }
  void set_unknown_492(float v) const { guest::StoreFloat(base, address + 492, v); }
  uint32_t unknown_492_address() const { return address + 492; }
  float unknown_496() const { return guest::LoadFloat(base, address + 496); }
  void set_unknown_496(float v) const { guest::StoreFloat(base, address + 496, v); }
  uint32_t unknown_496_address() const { return address + 496; }
  uint16_t unknown_500() const { return guest::LoadU16(base, address + 500); }
  void set_unknown_500(uint16_t v) const { guest::StoreU16(base, address + 500, v); }
  uint32_t unknown_500_address() const { return address + 500; }
  uint8_t unknown_502() const { return guest::LoadU8(base, address + 502); }
  void set_unknown_502(uint8_t v) const { guest::StoreU8(base, address + 502, v); }
  uint32_t unknown_502_address() const { return address + 502; }
  uint8_t unknown_503() const { return guest::LoadU8(base, address + 503); }
  void set_unknown_503(uint8_t v) const { guest::StoreU8(base, address + 503, v); }
  uint32_t unknown_503_address() const { return address + 503; }
  uint32_t unknown_504() const { return guest::LoadU32(base, address + 504); }
  void set_unknown_504(uint32_t v) const { guest::StoreU32(base, address + 504, v); }
  uint32_t unknown_504_address() const { return address + 504; }
  uint32_t unknown_508() const { return guest::LoadU32(base, address + 508); }
  void set_unknown_508(uint32_t v) const { guest::StoreU32(base, address + 508, v); }
  uint32_t unknown_508_address() const { return address + 508; }
  float unknown_640() const { return guest::LoadFloat(base, address + 640); }
  void set_unknown_640(float v) const { guest::StoreFloat(base, address + 640, v); }
  uint32_t unknown_640_address() const { return address + 640; }
  float unknown_644() const { return guest::LoadFloat(base, address + 644); }
  void set_unknown_644(float v) const { guest::StoreFloat(base, address + 644, v); }
  uint32_t unknown_644_address() const { return address + 644; }
  float unknown_648() const { return guest::LoadFloat(base, address + 648); }
  void set_unknown_648(float v) const { guest::StoreFloat(base, address + 648, v); }
  uint32_t unknown_648_address() const { return address + 648; }
  float unknown_652() const { return guest::LoadFloat(base, address + 652); }
  void set_unknown_652(float v) const { guest::StoreFloat(base, address + 652, v); }
  uint32_t unknown_652_address() const { return address + 652; }
  float unknown_656() const { return guest::LoadFloat(base, address + 656); }
  void set_unknown_656(float v) const { guest::StoreFloat(base, address + 656, v); }
  uint32_t unknown_656_address() const { return address + 656; }
  uint16_t unknown_660() const { return guest::LoadU16(base, address + 660); }
  void set_unknown_660(uint16_t v) const { guest::StoreU16(base, address + 660, v); }
  uint32_t unknown_660_address() const { return address + 660; }
  uint8_t unknown_662() const { return guest::LoadU8(base, address + 662); }
  void set_unknown_662(uint8_t v) const { guest::StoreU8(base, address + 662, v); }
  uint32_t unknown_662_address() const { return address + 662; }
  uint8_t unknown_663() const { return guest::LoadU8(base, address + 663); }
  void set_unknown_663(uint8_t v) const { guest::StoreU8(base, address + 663, v); }
  uint32_t unknown_663_address() const { return address + 663; }
  uint32_t unknown_664() const { return guest::LoadU32(base, address + 664); }
  void set_unknown_664(uint32_t v) const { guest::StoreU32(base, address + 664, v); }
  uint32_t unknown_664_address() const { return address + 664; }
  uint32_t unknown_668() const { return guest::LoadU32(base, address + 668); }
  void set_unknown_668(uint32_t v) const { guest::StoreU32(base, address + 668, v); }
  uint32_t unknown_668_address() const { return address + 668; }
  float unknown_800() const { return guest::LoadFloat(base, address + 800); }
  void set_unknown_800(float v) const { guest::StoreFloat(base, address + 800, v); }
  uint32_t unknown_800_address() const { return address + 800; }
  float unknown_804() const { return guest::LoadFloat(base, address + 804); }
  void set_unknown_804(float v) const { guest::StoreFloat(base, address + 804, v); }
  uint32_t unknown_804_address() const { return address + 804; }
  float unknown_808() const { return guest::LoadFloat(base, address + 808); }
  void set_unknown_808(float v) const { guest::StoreFloat(base, address + 808, v); }
  uint32_t unknown_808_address() const { return address + 808; }
  float unknown_812() const { return guest::LoadFloat(base, address + 812); }
  void set_unknown_812(float v) const { guest::StoreFloat(base, address + 812, v); }
  uint32_t unknown_812_address() const { return address + 812; }
  float unknown_816() const { return guest::LoadFloat(base, address + 816); }
  void set_unknown_816(float v) const { guest::StoreFloat(base, address + 816, v); }
  uint32_t unknown_816_address() const { return address + 816; }
  uint16_t unknown_820() const { return guest::LoadU16(base, address + 820); }
  void set_unknown_820(uint16_t v) const { guest::StoreU16(base, address + 820, v); }
  uint32_t unknown_820_address() const { return address + 820; }
  uint8_t unknown_822() const { return guest::LoadU8(base, address + 822); }
  void set_unknown_822(uint8_t v) const { guest::StoreU8(base, address + 822, v); }
  uint32_t unknown_822_address() const { return address + 822; }
  uint8_t unknown_823() const { return guest::LoadU8(base, address + 823); }
  void set_unknown_823(uint8_t v) const { guest::StoreU8(base, address + 823, v); }
  uint32_t unknown_823_address() const { return address + 823; }
  uint32_t unknown_824() const { return guest::LoadU32(base, address + 824); }
  void set_unknown_824(uint32_t v) const { guest::StoreU32(base, address + 824, v); }
  uint32_t unknown_824_address() const { return address + 824; }
  uint32_t unknown_828() const { return guest::LoadU32(base, address + 828); }
  void set_unknown_828(uint32_t v) const { guest::StoreU32(base, address + 828, v); }
  uint32_t unknown_828_address() const { return address + 828; }
  float unknown_960() const { return guest::LoadFloat(base, address + 960); }
  void set_unknown_960(float v) const { guest::StoreFloat(base, address + 960, v); }
  uint32_t unknown_960_address() const { return address + 960; }
  float unknown_964() const { return guest::LoadFloat(base, address + 964); }
  void set_unknown_964(float v) const { guest::StoreFloat(base, address + 964, v); }
  uint32_t unknown_964_address() const { return address + 964; }
  float unknown_968() const { return guest::LoadFloat(base, address + 968); }
  void set_unknown_968(float v) const { guest::StoreFloat(base, address + 968, v); }
  uint32_t unknown_968_address() const { return address + 968; }
  float unknown_972() const { return guest::LoadFloat(base, address + 972); }
  void set_unknown_972(float v) const { guest::StoreFloat(base, address + 972, v); }
  uint32_t unknown_972_address() const { return address + 972; }
  float unknown_976() const { return guest::LoadFloat(base, address + 976); }
  void set_unknown_976(float v) const { guest::StoreFloat(base, address + 976, v); }
  uint32_t unknown_976_address() const { return address + 976; }
  uint16_t unknown_980() const { return guest::LoadU16(base, address + 980); }
  void set_unknown_980(uint16_t v) const { guest::StoreU16(base, address + 980, v); }
  uint32_t unknown_980_address() const { return address + 980; }
  uint8_t unknown_982() const { return guest::LoadU8(base, address + 982); }
  void set_unknown_982(uint8_t v) const { guest::StoreU8(base, address + 982, v); }
  uint32_t unknown_982_address() const { return address + 982; }
  uint8_t unknown_983() const { return guest::LoadU8(base, address + 983); }
  void set_unknown_983(uint8_t v) const { guest::StoreU8(base, address + 983, v); }
  uint32_t unknown_983_address() const { return address + 983; }
  uint32_t unknown_984() const { return guest::LoadU32(base, address + 984); }
  void set_unknown_984(uint32_t v) const { guest::StoreU32(base, address + 984, v); }
  uint32_t unknown_984_address() const { return address + 984; }
  uint32_t unknown_988() const { return guest::LoadU32(base, address + 988); }
  void set_unknown_988(uint32_t v) const { guest::StoreU32(base, address + 988, v); }
  uint32_t unknown_988_address() const { return address + 988; }
  float unknown_1120() const { return guest::LoadFloat(base, address + 1120); }
  void set_unknown_1120(float v) const { guest::StoreFloat(base, address + 1120, v); }
  uint32_t unknown_1120_address() const { return address + 1120; }
  float unknown_1124() const { return guest::LoadFloat(base, address + 1124); }
  void set_unknown_1124(float v) const { guest::StoreFloat(base, address + 1124, v); }
  uint32_t unknown_1124_address() const { return address + 1124; }
  float unknown_1128() const { return guest::LoadFloat(base, address + 1128); }
  void set_unknown_1128(float v) const { guest::StoreFloat(base, address + 1128, v); }
  uint32_t unknown_1128_address() const { return address + 1128; }
  float unknown_1132() const { return guest::LoadFloat(base, address + 1132); }
  void set_unknown_1132(float v) const { guest::StoreFloat(base, address + 1132, v); }
  uint32_t unknown_1132_address() const { return address + 1132; }
  float unknown_1136() const { return guest::LoadFloat(base, address + 1136); }
  void set_unknown_1136(float v) const { guest::StoreFloat(base, address + 1136, v); }
  uint32_t unknown_1136_address() const { return address + 1136; }
  uint16_t unknown_1140() const { return guest::LoadU16(base, address + 1140); }
  void set_unknown_1140(uint16_t v) const { guest::StoreU16(base, address + 1140, v); }
  uint32_t unknown_1140_address() const { return address + 1140; }
  uint8_t unknown_1142() const { return guest::LoadU8(base, address + 1142); }
  void set_unknown_1142(uint8_t v) const { guest::StoreU8(base, address + 1142, v); }
  uint32_t unknown_1142_address() const { return address + 1142; }
  uint8_t unknown_1143() const { return guest::LoadU8(base, address + 1143); }
  void set_unknown_1143(uint8_t v) const { guest::StoreU8(base, address + 1143, v); }
  uint32_t unknown_1143_address() const { return address + 1143; }
  uint32_t unknown_1144() const { return guest::LoadU32(base, address + 1144); }
  void set_unknown_1144(uint32_t v) const { guest::StoreU32(base, address + 1144, v); }
  uint32_t unknown_1144_address() const { return address + 1144; }
  uint32_t unknown_1148() const { return guest::LoadU32(base, address + 1148); }
  void set_unknown_1148(uint32_t v) const { guest::StoreU32(base, address + 1148, v); }
  uint32_t unknown_1148_address() const { return address + 1148; }
};

// embedded; measure: Car@+96
struct CarAt96 : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint16_t unknown_20() const { return guest::LoadU16(base, address + 20); }
  void set_unknown_20(uint16_t v) const { guest::StoreU16(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint8_t unknown_22() const { return guest::LoadU8(base, address + 22); }
  void set_unknown_22(uint8_t v) const { guest::StoreU8(base, address + 22, v); }
  uint32_t unknown_22_address() const { return address + 22; }
  uint8_t unknown_23() const { return guest::LoadU8(base, address + 23); }
  void set_unknown_23(uint8_t v) const { guest::StoreU8(base, address + 23, v); }
  uint32_t unknown_23_address() const { return address + 23; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  float unknown_96() const { return guest::LoadFloat(base, address + 96); }
  void set_unknown_96(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  float unknown_100() const { return guest::LoadFloat(base, address + 100); }
  void set_unknown_100(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  float unknown_104() const { return guest::LoadFloat(base, address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  float unknown_108() const { return guest::LoadFloat(base, address + 108); }
  void set_unknown_108(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  float unknown_112() const { return guest::LoadFloat(base, address + 112); }
  void set_unknown_112(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint16_t unknown_116() const { return guest::LoadU16(base, address + 116); }
  void set_unknown_116(uint16_t v) const { guest::StoreU16(base, address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  uint8_t unknown_118() const { return guest::LoadU8(base, address + 118); }
  void set_unknown_118(uint8_t v) const { guest::StoreU8(base, address + 118, v); }
  uint32_t unknown_118_address() const { return address + 118; }
  uint8_t unknown_119() const { return guest::LoadU8(base, address + 119); }
  void set_unknown_119(uint8_t v) const { guest::StoreU8(base, address + 119, v); }
  uint32_t unknown_119_address() const { return address + 119; }
  uint32_t unknown_120() const { return guest::LoadU32(base, address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  uint32_t unknown_124() const { return guest::LoadU32(base, address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
};

// embedded; measure: RaceScene@+16
#pragma pack(push, 1)
struct CarAt16_2Data {
  uint32_t unknown_0 = 0;  // +0
  uint8_t pad_4[12] = {};
  uint32_t unknown_16 = 0;  // +16
  uint8_t pad_20[4268] = {};
  uint32_t unknown_4288 = 0;  // +4288
};
#pragma pack(pop)
static_assert(sizeof(CarAt16_2Data) == 4292);
inline constexpr memory::LayoutField kCarAt16_2Fields[] = {{0, 4}, {16, 4}, {4288, 4}};
inline constexpr memory::Layout kCarAt16_2Layout = {kCarAt16_2Fields, 3};

struct CarAt16_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 4292;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  CarAt16_2Data* data() const { return static_cast<CarAt16_2Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static CarAt16_2 New(uint8_t* base) {
    return CarAt16_2(base, memory::MakeHandle(new CarAt16_2Data(), sizeof(CarAt16_2Data), "CarAt16_2", &kCarAt16_2Layout));
  }
  template <uint32_t Stride = kSize>
  static CarAt16_2 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarAt16_2Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarAt16_2Data*>(::operator new(sizeof(CarAt16_2Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarAt16_2Data();
    return CarAt16_2(base, memory::MakeHandle(p, sizeof(CarAt16_2Data) * count, "CarAt16_2", &kCarAt16_2Layout, sizeof(CarAt16_2Data)));
  }
  CarAt16_2 at(uint32_t i) const { return CarAt16_2(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarAt16_2Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarAt16_2Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0() const { return on_host() ? uint32_t(data()->unknown_0) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_unknown_0(uint32_t v) const { if (on_host()) data()->unknown_0 = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_bits() const { return on_host() ? data()->unknown_0 : guest::LoadU32(base, address + 0); }
  void set_unknown_0_bits(uint32_t v) const { if (on_host()) data()->unknown_0 = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_16() const { return on_host() ? uint32_t(data()->unknown_16) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_unknown_16(uint32_t v) const { if (on_host()) data()->unknown_16 = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_bits() const { return on_host() ? data()->unknown_16 : guest::LoadU32(base, address + 16); }
  void set_unknown_16_bits(uint32_t v) const { if (on_host()) data()->unknown_16 = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_4288_address() const { return address + 4288; }
  uint32_t unknown_4288() const { return on_host() ? uint32_t(data()->unknown_4288) : uint32_t(guest::LoadU32(base, address + 4288)); }
  void set_unknown_4288(uint32_t v) const { if (on_host()) data()->unknown_4288 = v; else guest::StoreU32(base, address + 4288, v); }
  uint32_t unknown_4288_bits() const { return on_host() ? data()->unknown_4288 : guest::LoadU32(base, address + 4288); }
  void set_unknown_4288_bits(uint32_t v) const { if (on_host()) data()->unknown_4288 = v; else guest::StoreU32(base, address + 4288, v); }
};

// The image words docs/functions/race-setup.md's code uses.
struct RaceSetupGlobals {
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 100.0
  static float hundred(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_hundred(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t hundred_address() { return 0x82002A0Cu; }
  // 20.0
  static float twenty(uint8_t* base) { return guest::LoadFloat(base, 0x82002568u); }
  static void set_twenty(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002568u, v); }
  static constexpr uint32_t twenty_address() { return 0x82002568u; }
  // Car::SetModel's last factor (not read)
  static float torque_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8200222Cu); }
  static void set_torque_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200222Cu, v); }
  static constexpr uint32_t torque_scale_address() { return 0x8200222Cu; }
  // Car::FromBuild's ratio factor (not read)
  static float gear_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002204u); }
  static void set_gear_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002204u, v); }
  static constexpr uint32_t gear_scale_address() { return 0x82002204u; }
  // the racers' start float (Racers::SetStart; not read)
  static float traffic_start(uint8_t* base) { return guest::LoadFloat(base, 0x8200692Cu); }
  static void set_traffic_start(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200692Cu, v); }
  static constexpr uint32_t traffic_start_address() { return 0x8200692Cu; }
  // two halfword bits for each event % 7 (Race::SetUp; -1 none)
  static constexpr uint32_t event_bits_address() { return 0x82008114u; }
  // the race data (race-states.toml; +4 the route, +48 the racer records, +0x59620 the stage list, +0x59AE8 the 77 records)
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race event (RaceEvent::Current)
  template <typename T = RaceEvent>
  static T event(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C6078u)); }
  static void set_event(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C6078u, v.address); }
  static void set_event(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6078u, v); }
  static constexpr uint32_t event_address() { return 0x826C6078u; }
};

}  // namespace schema
