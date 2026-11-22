// The views of docs/functions/save-data.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/save-data.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Progress@+109796
struct BitSet400 : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // 13 big-endian words; bits 0 to 399 used, 400 to 415 stay 0
  uint32_t words_address() const { return address + 0; }
};

// embedded; measure: ControlMap::SetDefault.r3
struct ControlMap : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // default 18
  uint32_t steering() const { return guest::LoadU32(base, address + 0); }
  void set_steering(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t steering_address() const { return address + 0; }
  // default 14
  uint32_t throttle() const { return guest::LoadU32(base, address + 4); }
  void set_throttle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t throttle_address() const { return address + 4; }
  // default 13
  uint32_t brake() const { return guest::LoadU32(base, address + 8); }
  void set_brake(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t brake_address() const { return address + 8; }
  // the car input's +10; default 9
  uint32_t input_10() const { return guest::LoadU32(base, address + 12); }
  void set_input_10(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t input_10_address() const { return address + 12; }
  // a button; default 0
  uint32_t button_16() const { return guest::LoadU32(base, address + 16); }
  void set_button_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t button_16_address() const { return address + 16; }
  // a button; default 10
  uint32_t button_20() const { return guest::LoadU32(base, address + 20); }
  void set_button_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t button_20_address() const { return address + 20; }
  // a button; default 16
  uint32_t button_24() const { return guest::LoadU32(base, address + 24); }
  void set_button_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t button_24_address() const { return address + 24; }
  // a button; default 15
  uint32_t button_28() const { return guest::LoadU32(base, address + 28); }
  void set_button_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t button_28_address() const { return address + 28; }
  // a button; default 24
  uint32_t button_32() const { return guest::LoadU32(base, address + 32); }
  void set_button_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t button_32_address() const { return address + 32; }
  // a button; default 7
  uint32_t button_36() const { return guest::LoadU32(base, address + 36); }
  void set_button_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t button_36_address() const { return address + 36; }
  // a button; default 17
  uint32_t button_40() const { return guest::LoadU32(base, address + 40); }
  void set_button_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t button_40_address() const { return address + 40; }
  // default 0
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  // the car input's +12; default 25
  uint32_t input_12() const { return guest::LoadU32(base, address + 48); }
  void set_input_12(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t input_12_address() const { return address + 48; }
  // a button; default 8
  uint32_t button_52() const { return guest::LoadU32(base, address + 52); }
  void set_button_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t button_52_address() const { return address + 52; }
  // a button; default 11
  uint32_t button_56() const { return guest::LoadU32(base, address + 56); }
  void set_button_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t button_56_address() const { return address + 56; }
};

// embedded; measure: CourseRecords
struct CourseRecords : View {
  using View::View;
  static constexpr uint32_t kSize = 8996;
  // 0 at the reset
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // 0 at the reset
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // 0 at the reset
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // 0 at the reset
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // 0 at the reset
  uint16_t unknown_16() const { return guest::LoadU16(base, address + 16); }
  void set_unknown_16(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // 0 at the reset
  uint16_t unknown_18() const { return guest::LoadU16(base, address + 18); }
  void set_unknown_18(uint16_t v) const { guest::StoreU16(base, address + 18, v); }
  uint32_t unknown_18_address() const { return address + 18; }
  // 22 CourseRecords of 408 bytes, course x 2 + traffic
  uint32_t records_address() const { return address + 20; }
};

// embedded; measure: CourseRecords::Of.r4
struct CourseRecordKey : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 0 to 10
  uint32_t course() const { return guest::LoadU32(base, address + 0); }
  void set_course(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t course_address() const { return address + 0; }
  // 0 or 1
  uint32_t traffic() const { return guest::LoadU32(base, address + 4); }
  void set_traffic(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t traffic_address() const { return address + 4; }
};

// The image words docs/functions/save-data.md's code uses.
struct SaveDataGlobals {
  // 0.0 (loaded with lfs, stored with stfs)
  static uint32_t zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // player 0's save data; player i's at + 4 x i (SaveData::Of)
  template <typename T = SaveData>
  static T slots(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B58u)); }
  static void set_slots(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B58u, v.address); }
  static void set_slots(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B58u, v); }
  static constexpr uint32_t slots_address() { return 0x826C5B58u; }
  // player 1's slot: the two-player block SaveData::CreateTwoPlayerBlock makes (0x3AB74 bytes, a player's half 0x1D52C)
  static uint32_t two_player_block(uint8_t* base) { return guest::LoadU32(base, 0x826C5B5Cu); }
  static void set_two_player_block(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B5Cu, v); }
  static constexpr uint32_t two_player_block_address() { return 0x826C5B5Cu; }
  // the play time's frame counter (Progress::SetDefaults copies it to +6088)
  static uint32_t play_time_frames(uint8_t* base) { return guest::LoadU32(base, 0x826C5B90u); }
  static void set_play_time_frames(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B90u, v); }
  static constexpr uint32_t play_time_frames_address() { return 0x826C5B90u; }
  // Defaults::Table50A8: copied into the save data's +0x19930
  static constexpr uint32_t defaults_50a8_address() { return 0x826C50A8u; }
  // Defaults::Table3044: less Table3E34's bits into +0x1996C
  static constexpr uint32_t defaults_3044_address() { return 0x826C3044u; }
  // Defaults::Table3E34: 15 words of bits taken from Table3044's
  static constexpr uint32_t defaults_3e34_address() { return 0x826C3E34u; }
  // Defaults::Table3C24: copied into +0x199A8
  static constexpr uint32_t defaults_3c24_address() { return 0x826C3C24u; }
  // Defaults::Table50E4: copied into +0x199C4
  static constexpr uint32_t defaults_50e4_address() { return 0x826C50E4u; }
  // the mode's first four system checks (mode-tick.md), 1 each from TitleScreen::ResetPlayer
  static constexpr uint32_t mode_checks_address() { return 0x826BAB40u; }
  // the player-to-controller table: 4 words, 0 to 3 from Players::ResetControllers
  static constexpr uint32_t controllers_address() { return 0x826BABA4u; }
  // 1 asks for the profile read
  static uint32_t profile_read_asked(uint8_t* base) { return guest::LoadU32(base, 0x826C7938u); }
  static void set_profile_read_asked(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7938u, v); }
  static constexpr uint32_t profile_read_asked_address() { return 0x826C7938u; }
  // the title screen's menu list
  template <typename T = MenuList>
  static T title_menu_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E30u)); }
  static void set_title_menu_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E30u, v.address); }
  static void set_title_menu_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E30u, v); }
  static constexpr uint32_t title_menu_list_address() { return 0x82814E30u; }
  // the title screen object (title-screen.toml)
  template <typename T = TitleScreenData>
  static T title_screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E34u)); }
  static void set_title_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E34u, v.address); }
  static void set_title_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E34u, v); }
  static constexpr uint32_t title_screen_address() { return 0x82814E34u; }
};

}  // namespace schema
