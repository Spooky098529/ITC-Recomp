// The views of docs/functions/user-info.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/user-info.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: UserInfo
struct UserInfoMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  // measure fault: the transitions' r3 is the page object's +24 (PageObject.step_started), not the machine's
  uint32_t fault_step_started() const { return guest::LoadU32(base, address + 0); }
  void set_fault_step_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_step_started_address() const { return address + 0; }
  // measure fault: MenuPage's r3 is the dispatcher (UserInfoMode.message_count), not the machine
  uint32_t fault_message_count() const { return guest::LoadU32(base, address + 804); }
  void set_fault_message_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t fault_message_count_address() const { return address + 804; }
  // measure fault: the dispatcher's message_start, as above
  uint32_t fault_message_start() const { return guest::LoadU32(base, address + 808); }
  void set_fault_message_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t fault_message_start_address() const { return address + 808; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 at set-up, 1 open the movie, 2 its load, 9 waiting for message 27, 4 and 5 shown, 7 and 6 closed
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movie 10 is loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // set by the mode's Init: nonzero, Create goes to state 1, else 9
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
};

// embedded; measure: UserInfo::Close.r4
struct UserInfoMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint32_t message_count() const { return guest::LoadU32(base, address + 804); }
  void set_message_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t message_count_address() const { return address + 804; }
  // the ring's first slot
  uint32_t message_start() const { return guest::LoadU32(base, address + 808); }
  void set_message_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t message_start_address() const { return address + 808; }
};

// allocated; measure: new<60>@UserInfo::Create
struct UserInfoData : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // 1 once loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(base, address + 0); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t loaded_address() const { return address + 0; }
  // the state Update moves the machine to: 21 none, 9 from Closing
  uint32_t queued_state() const { return guest::LoadU32(base, address + 4); }
  void set_queued_state(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t queued_state_address() const { return address + 4; }
  // the stats' halfword +12 (node 8)
  uint32_t count_12() const { return guest::LoadU32(base, address + 8); }
  void set_count_12(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t count_12_address() const { return address + 8; }
  // the stats' halfword +14 (node 9)
  uint32_t count_14() const { return guest::LoadU32(base, address + 12); }
  void set_count_14(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t count_14_address() const { return address + 12; }
  // the stats' halfword +16 (node 10)
  uint32_t count_16() const { return guest::LoadU32(base, address + 16); }
  void set_count_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t count_16_address() const { return address + 16; }
  // +12 / +22 times 100 (node 11, string 453; probably the win rate)
  float ratio() const { return guest::LoadFloat(base, address + 20); }
  void set_ratio(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t ratio_address() const { return address + 20; }
  // the stats' +5968 (node 12)
  uint32_t stats_5968() const { return guest::LoadU32(base, address + 24); }
  void set_stats_5968(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t stats_5968_address() const { return address + 24; }
  // the stats' +5976, shown times 0.62138 (node 19, kilometers in miles)
  float distance_5976() const { return guest::LoadFloat(base, address + 28); }
  void set_distance_5976(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t distance_5976_address() const { return address + 28; }
  // the stats' +5904, shown times 0.62138 (node 20)
  float distance_5904() const { return guest::LoadFloat(base, address + 32); }
  void set_distance_5904(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t distance_5904_address() const { return address + 32; }
  // the stats' +5992 truncated, shown times 0.62138 (node 21)
  int32_t distance_5992() const { return int32_t(guest::LoadU32(base, address + 36)); }
  void set_distance_5992(int32_t v) const { guest::StoreU32(base, address + 36, uint32_t(v)); }
  uint32_t distance_5992_address() const { return address + 36; }
  // the stats' +6024 (node 22, SetNumberText)
  uint32_t stats_6024() const { return guest::LoadU32(base, address + 40); }
  void set_stats_6024(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t stats_6024_address() const { return address + 40; }
  // the play time's hours (PlayTime::Add; shown at most 9999)
  uint32_t play_hours() const { return guest::LoadU32(base, address + 44); }
  void set_play_hours(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t play_hours_address() const { return address + 44; }
  // PlayTime::Add's word +4: the minutes and seconds bytes
  uint32_t play_time_word() const { return guest::LoadU32(base, address + 48); }
  void set_play_time_word(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t play_time_word_address() const { return address + 48; }
  uint8_t play_minutes() const { return guest::LoadU8(base, address + 48); }
  void set_play_minutes(uint8_t v) const { guest::StoreU8(base, address + 48, v); }
  uint32_t play_minutes_address() const { return address + 48; }
  uint8_t play_seconds() const { return guest::LoadU8(base, address + 49); }
  void set_play_seconds(uint8_t v) const { guest::StoreU8(base, address + 49, v); }
  uint32_t play_seconds_address() const { return address + 49; }
  // the garage's count (node 24, string 444)
  uint32_t garage_count() const { return guest::LoadU32(base, address + 52); }
  void set_garage_count(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t garage_count_address() const { return address + 52; }
  // the save's +0x1991C (node 24)
  uint32_t save_1991C() const { return guest::LoadU32(base, address + 56); }
  void set_save_1991C(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t save_1991C_address() const { return address + 56; }
};

// embedded; measure: PlayTime::Add.ret
struct PlayTimeAnswer : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the stats' +6032
  uint32_t hours() const { return guest::LoadU32(base, address + 0); }
  void set_hours(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t hours_address() const { return address + 0; }
  // the stats' word +6036: the minutes, seconds and frames bytes
  uint32_t time_word() const { return guest::LoadU32(base, address + 4); }
  void set_time_word(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t time_word_address() const { return address + 4; }
};

// The image words docs/functions/user-info.md's code uses.
struct UserInfoGlobals {
  // pages 0 the menu page, 1 none (closing); steps 0 the opening, 1 the closing
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84C4u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84C4u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84C4u, v); }
  static constexpr uint32_t pages_address() { return 0x826D84C4u; }
  // a front-end menu, items 0 to 7, no wrap; stepped, its cursor not read
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84C8u)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84C8u, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84C8u, v); }
  static constexpr uint32_t menu_address() { return 0x826D84C8u; }
  template <typename T = UserInfoData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84CCu)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84CCu, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84CCu, v); }
  static constexpr uint32_t data_address() { return 0x826D84CCu; }
  // 0.62138 (kilometers to miles)
  static double miles_per_km(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x8200A510u)); }
  static void set_miles_per_km(uint8_t* base, double v) { guest::StoreU64(base, 0x8200A510u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t miles_per_km_address() { return 0x8200A510u; }
};

}  // namespace schema
