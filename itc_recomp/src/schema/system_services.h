// The views of docs/functions/system-services.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/system-services.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<256>@Users::Create
struct SysUsers : View {
  using View::View;
  static constexpr uint32_t kSize = 256;
  static constexpr uint32_t kVtable = 0x82000FA0;
  // 0x82000FA0
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the four 40-byte user records from +8 (user n at +8 + 40n): XUserGetSigninState's answer (1 local, 2 Live)
  uint32_t user0_signin_state() const { return guest::LoadU32(base, address + 8); }
  void set_user0_signin_state(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t user0_signin_state_address() const { return address + 8; }
  // the record's +8: XUserGetXUID's answer
  uint64_t user0_xuid() const { return guest::LoadU64(base, address + 16); }
  void set_user0_xuid(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t user0_xuid_address() const { return address + 16; }
  // the record's +16: XUserGetName's 16 bytes
  uint32_t user0_name_address() const { return address + 24; }
  // the record's +32: 1 when signed in to Live (state 2)
  uint8_t user0_on_live() const { return guest::LoadU8(base, address + 40); }
  void set_user0_on_live(uint8_t v) const { guest::StoreU8(base, address + 40, v); }
  uint32_t user0_on_live_address() const { return address + 40; }
  // the Live users' list (8 bytes each: the record's address, then the user at +172)
  uint32_t live_users() const { return guest::LoadU32(base, address + 168); }
  void set_live_users(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t live_users_address() const { return address + 168; }
  // the first Live entry's user index
  uint32_t live_user0_index() const { return guest::LoadU32(base, address + 172); }
  void set_live_user0_index(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t live_user0_index_address() const { return address + 172; }
  // the signed-in users' indices (a word each, 4)
  uint32_t signed_in_users() const { return guest::LoadU32(base, address + 200); }
  void set_signed_in_users(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t signed_in_users_address() const { return address + 200; }
  uint32_t signed_in_count() const { return guest::LoadU32(base, address + 216); }
  void set_signed_in_count(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t signed_in_count_address() const { return address + 216; }
  uint32_t live_count() const { return guest::LoadU32(base, address + 220); }
  void set_live_count(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t live_count_address() const { return address + 220; }
  // the set-up block's +8
  uint32_t tick_key() const { return guest::LoadU32(base, address + 224); }
  void set_tick_key(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t tick_key_address() const { return address + 224; }
  // the set-up block's +12
  uint32_t listener_key() const { return guest::LoadU32(base, address + 228); }
  void set_listener_key(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t listener_key_address() const { return address + 228; }
  // the set-up block's +4
  uint32_t framework_interface() const { return guest::LoadU32(base, address + 232); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t framework_interface_address() const { return address + 232; }
  // set by sub_820998B8 (not read in detail)
  uint32_t unknown_236() const { return guest::LoadU32(base, address + 236); }
  void set_unknown_236(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t unknown_236_address() const { return address + 236; }
  // the notification listener (XNotifyCreateListener mask 3; user::kNoListener since B5)
  uint32_t listener() const { return guest::LoadU32(base, address + 240); }
  void set_listener(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t listener_address() const { return address + 240; }
  // set when the users were rebuilt (sign-in changed)
  uint32_t users_changed() const { return guest::LoadU32(base, address + 244); }
  void set_users_changed(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t users_changed_address() const { return address + 244; }
  // set by Live connection changed (never sent); App's Live connection state answer
  uint32_t live_connection_changed() const { return guest::LoadU32(base, address + 248); }
  void set_live_connection_changed(uint32_t v) const { guest::StoreU32(base, address + 248, v); }
  uint32_t live_connection_changed_address() const { return address + 248; }
  // Live connection changed's parameter
  uint32_t live_connection_state() const { return guest::LoadU32(base, address + 252); }
  void set_live_connection_state(uint32_t v) const { guest::StoreU32(base, address + 252, v); }
  uint32_t live_connection_state_address() const { return address + 252; }
};

// embedded; measure: sub_820998B8.r4
struct SysUsersSetUp : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the run loop (frame-driver.toml's FrameDriverRunLoopRef: its slots 2 and 1)
  template <typename T = FrameDriverRunLoopRef>
  T run_loop() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_run_loop(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_run_loop(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t run_loop_address() const { return address + 0; }
  // into the users' +232
  uint32_t framework_interface() const { return guest::LoadU32(base, address + 4); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_interface_address() const { return address + 4; }
  // into the users' +224
  uint32_t tick_key() const { return guest::LoadU32(base, address + 8); }
  void set_tick_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t tick_key_address() const { return address + 8; }
  // into the users' +228
  uint32_t listener_key() const { return guest::LoadU32(base, address + 12); }
  void set_listener_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t listener_key_address() const { return address + 12; }
};

// image; measure: Network, sub_8214FDF8.r3, sub_8214FF78.r3, sub_8214F790.r3, sub_8214FA20.r3, sub_8214FC40.r3, sub_8214F938.r3, [826C74A0]
struct NetworkManager : View {
  using View::View;
  static constexpr uint32_t kSize = 460;
  // XHV's engine (0: not made)
  template <typename T = VoiceEngine>
  T voice_engine() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_voice_engine(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_voice_engine(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t voice_engine_address() const { return address + 0; }
  // +4 to +32 cleared by the constructor
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the local chat data (the engine's GetLocalChatData, sub_8214FA78)
  uint32_t chat_buffer_address() const { return address + 36; }
  // the chat data's count
  uint32_t chat_bytes() const { return guest::LoadU32(base, address + 436); }
  void set_chat_bytes(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t chat_bytes_address() const { return address + 436; }
  // the tick count at the voice set-up (sub_8214F6E8)
  uint32_t voice_ticks() const { return guest::LoadU32(base, address + 444); }
  void set_voice_ticks(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t voice_ticks_address() const { return address + 444; }
  // where the machine id is: the user table (0x826C7670), from the voice set-up's r4
  template <typename T = NetUserTable>
  T machine_id() const { return T(base, guest::LoadU32(base, address + 456)); }
  void set_machine_id(const View& v) const { guest::StoreU32(base, address + 456, v.address); }
  void set_machine_id(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t machine_id_address() const { return address + 456; }
};

// allocated; measure: sub_8214FDF8.r3.+0, sub_8214FF78.r3.+0, sub_8214F790.r3.+0, sub_8214F938.r3.+0, sub_8214FA20.r3.+0, sub_8214FC40.r3.+0, [826C74A0].+0
struct VoiceEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 2240;
  // the engine's table (0x829576D0)
  uint32_t table() const { return guest::LoadU32(base, address + 0); }
  void set_table(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t table_address() const { return address + 0; }
  // measure fault: the table's +20 (sub_8257B0A8, probably StartRemoteProcessingModes)
  uint32_t slot_start_remote_processing() const { return guest::LoadU32(base, address + 20); }
  void set_slot_start_remote_processing(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot_start_remote_processing_address() const { return address + 20; }
  // measure fault: the table's +40 (sub_8257B188)
  uint32_t slot_register_remote_talker() const { return guest::LoadU32(base, address + 40); }
  void set_slot_register_remote_talker(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t slot_register_remote_talker_address() const { return address + 40; }
  // measure fault: the table's +44 (sub_8257B290)
  uint32_t slot_unregister_remote_talker() const { return guest::LoadU32(base, address + 44); }
  void set_slot_unregister_remote_talker(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t slot_unregister_remote_talker_address() const { return address + 44; }
  // measure fault: the table's +48 (sub_8257B2F0)
  uint32_t slot_get_remote_talkers() const { return guest::LoadU32(base, address + 48); }
  void set_slot_get_remote_talkers(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot_get_remote_talkers_address() const { return address + 48; }
  // measure fault: the table's +52 (sub_8257A790)
  uint32_t slot_is_headset_present() const { return guest::LoadU32(base, address + 52); }
  void set_slot_is_headset_present(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t slot_is_headset_present_address() const { return address + 52; }
  // measure fault: the table's +80 (sub_8257B4F8)
  uint32_t slot_set_playback_priority() const { return guest::LoadU32(base, address + 80); }
  void set_slot_set_playback_priority(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t slot_set_playback_priority_address() const { return address + 80; }
  // measure fault: the table's +84 (sub_8257B568)
  uint32_t slot_submit_incoming_chat_data() const { return guest::LoadU32(base, address + 84); }
  void set_slot_submit_incoming_chat_data(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t slot_submit_incoming_chat_data_address() const { return address + 84; }
};

// image; measure: sub_8214FDF8.r3.+456
struct NetUserTable : View {
  using View::View;
  static constexpr uint32_t kSize = 384;
  // XNetXnAddrToMachineId's out (never written: not signed in to Live)
  uint64_t machine_id() const { return guest::LoadU64(base, address + 0); }
  void set_machine_id(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t machine_id_address() const { return address + 0; }
  // the signed-in users
  uint8_t count() const { return guest::LoadU8(base, address + 12); }
  void set_count(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t count_address() const { return address + 12; }
  // 4 32-byte UTF-16 names, in order
  uint32_t names_address() const { return address + 14; }
  // 4 XUIDs, in order
  uint32_t xuids_address() const { return address + 144; }
  // the gamercard region (profile setting 3) after a read
  uint32_t region_slot0() const { return guest::LoadU32(base, address + 316); }
  void set_region_slot0(uint32_t v) const { guest::StoreU32(base, address + 316, v); }
  uint32_t region_slot0_address() const { return address + 316; }
  uint32_t region_slot1() const { return guest::LoadU32(base, address + 320); }
  void set_region_slot1(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t region_slot1_address() const { return address + 320; }
  uint32_t region_slot2() const { return guest::LoadU32(base, address + 324); }
  void set_region_slot2(uint32_t v) const { guest::StoreU32(base, address + 324, v); }
  uint32_t region_slot2_address() const { return address + 324; }
  uint32_t region_slot3() const { return guest::LoadU32(base, address + 328); }
  void set_region_slot3(uint32_t v) const { guest::StoreU32(base, address + 328, v); }
  uint32_t region_slot3_address() const { return address + 328; }
  // a word per signed-in slot (achievements.md: probably the network battles' wins)
  uint32_t slot_counts_address() const { return address + 364; }
};

// allocated; measure: new<?>@sub_82149178
struct SysProfileResults : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the settings' count (26)
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the settings (0 while missing)
  template <typename T = SysProfileSettings>
  T settings() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_settings(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_settings(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t settings_address() const { return address + 4; }
};

// allocated; measure: new<?>@sub_82149178.+4
struct SysProfileSettings : View {
  using View::View;
  static constexpr uint32_t kSize = 1040;
  // setting 3's source (0 when missing): the gamercard region
  uint32_t region_source() const { return guest::LoadU32(base, address + 120); }
  void set_region_source(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t region_source_address() const { return address + 120; }
  // setting 3's value
  uint32_t region_value() const { return guest::LoadU32(base, address + 152); }
  void set_region_value(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t region_value_address() const { return address + 152; }
  // setting 15's value (save-screen.md)
  uint32_t setting15_value() const { return guest::LoadU32(base, address + 632); }
  void set_setting15_value(uint32_t v) const { guest::StoreU32(base, address + 632, v); }
  uint32_t setting15_value_address() const { return address + 632; }
  // setting 16's value
  uint32_t setting16_value() const { return guest::LoadU32(base, address + 672); }
  void set_setting16_value(uint32_t v) const { guest::StoreU32(base, address + 672, v); }
  uint32_t setting16_value_address() const { return address + 672; }
  // setting 17's value
  uint32_t setting17_value() const { return guest::LoadU32(base, address + 712); }
  void set_setting17_value(uint32_t v) const { guest::StoreU32(base, address + 712, v); }
  uint32_t setting17_value_address() const { return address + 712; }
  // setting 18's value
  uint32_t setting18_value() const { return guest::LoadU32(base, address + 752); }
  void set_setting18_value(uint32_t v) const { guest::StoreU32(base, address + 752, v); }
  uint32_t setting18_value_address() const { return address + 752; }
};

// image; measure: -
struct SysXOverlapped : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // InternalLow: 997 while pending, then the result
  uint32_t result() const { return guest::LoadU32(base, address + 0); }
  void set_result(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t result_address() const { return address + 0; }
  // InternalHigh: 0, or -1 for an error
  uint32_t length() const { return guest::LoadU32(base, address + 4); }
  void set_length(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t length_address() const { return address + 4; }
  uint32_t context() const { return guest::LoadU32(base, address + 8); }
  void set_context(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t context_address() const { return address + 8; }
  uint32_t event() const { return guest::LoadU32(base, address + 12); }
  void set_event(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t event_address() const { return address + 12; }
  uint32_t completion_routine() const { return guest::LoadU32(base, address + 16); }
  void set_completion_routine(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t completion_routine_address() const { return address + 16; }
  uint32_t completion_context() const { return guest::LoadU32(base, address + 20); }
  void set_completion_context(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t completion_context_address() const { return address + 20; }
  uint32_t extended_error() const { return guest::LoadU32(base, address + 24); }
  void set_extended_error(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t extended_error_address() const { return address + 24; }
};

// image; measure: XNet, sub_82577788.r3@+28.+20
struct XNetConfig : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  // bit 0: Configure fails (0x80004005)
  uint32_t flags() const { return guest::LoadU32(base, address + 8); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t flags_address() const { return address + 8; }
  // the whole size; what's left after the tables goes to +56
  uint32_t total_bytes() const { return guest::LoadU32(base, address + 20); }
  void set_total_bytes(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t total_bytes_address() const { return address + 20; }
  // times +26: the area from +64 to +68
  uint16_t table_d_count() const { return guest::LoadU16(base, address + 24); }
  void set_table_d_count(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t table_d_count_address() const { return address + 24; }
  uint16_t table_d_entry_bytes() const { return guest::LoadU16(base, address + 26); }
  void set_table_d_entry_bytes(uint16_t v) const { guest::StoreU16(base, address + 26, v); }
  uint32_t table_d_entry_bytes_address() const { return address + 26; }
  // halfwords: the area from +68 to +72
  uint16_t table_e_count() const { return guest::LoadU16(base, address + 32); }
  void set_table_e_count(uint16_t v) const { guest::StoreU16(base, address + 32, v); }
  uint32_t table_e_count_address() const { return address + 32; }
  // the optional area at +60
  uint16_t block_a_bytes() const { return guest::LoadU16(base, address + 38); }
  void set_block_a_bytes(uint16_t v) const { guest::StoreU16(base, address + 38, v); }
  uint32_t block_a_bytes_address() const { return address + 38; }
  // a word each at +44, entries of +42 bytes at +48
  uint16_t table_b_count() const { return guest::LoadU16(base, address + 40); }
  void set_table_b_count(uint16_t v) const { guest::StoreU16(base, address + 40, v); }
  uint32_t table_b_count_address() const { return address + 40; }
  uint16_t table_b_entry_bytes() const { return guest::LoadU16(base, address + 42); }
  void set_table_b_entry_bytes(uint16_t v) const { guest::StoreU16(base, address + 42, v); }
  uint32_t table_b_entry_bytes_address() const { return address + 42; }
  // after the header (and the +60 area)
  uint32_t table_b_index() const { return guest::LoadU32(base, address + 44); }
  void set_table_b_index(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t table_b_index_address() const { return address + 44; }
  uint32_t table_b() const { return guest::LoadU32(base, address + 48); }
  void set_table_b(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t table_b_address() const { return address + 48; }
  // after table_b
  uint32_t heap() const { return guest::LoadU32(base, address + 52); }
  void set_heap(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t heap_address() const { return address + 52; }
  // total_bytes less the header and the tables
  uint32_t heap_bytes() const { return guest::LoadU32(base, address + 56); }
  void set_heap_bytes(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t heap_bytes_address() const { return address + 56; }
  // the source + 44, when block_a_bytes isn't 0
  uint32_t block_a() const { return guest::LoadU32(base, address + 60); }
  void set_block_a(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t block_a_address() const { return address + 60; }
  // heap + heap_bytes
  uint32_t table_d() const { return guest::LoadU32(base, address + 64); }
  void set_table_d(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t table_d_address() const { return address + 64; }
  uint32_t table_e() const { return guest::LoadU32(base, address + 68); }
  void set_table_e(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t table_e_address() const { return address + 68; }
  uint32_t end() const { return guest::LoadU32(base, address + 72); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t end_address() const { return address + 72; }
};

// image; measure: SaveDevices.+0
struct SaveDevicesVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // slot 7: asked about a slot whose device state isn't 0-3 (cleared under 5)
  uint32_t check_slot_device() const { return guest::LoadU32(base, address + 28); }
  void set_check_slot_device(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t check_slot_device_address() const { return address + 28; }
};

// embedded; measure: SaveDevices.+12
struct SaveDeviceSource : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = SaveDeviceSourceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: SaveDevices.+12.+0
struct SaveDeviceSourceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 1: the device list (its slot 2 answers a slot's device state)
  uint32_t device_list() const { return guest::LoadU32(base, address + 4); }
  void set_device_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t device_list_address() const { return address + 4; }
};

// embedded; measure: sub_82149888.r3, sub_821498F0.r3, sub_8214C828.r4, sub_8214BCE0.r4, sub_8214A8E0.r3, sub_8214B2C8.r3, sub_8214B188.r4, sub_82149BF0.ret.+0, sub_82149CA0.ret.+0, sub_8214A950.ret.+0
struct SysLinkIdSet : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the head node (its parent the root)
  template <typename T = SysLinkIdSetNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
};

// allocated; measure: new<?>@sub_82149A30, new<?>@sub_82149A30.+4.+0.+8.+4, sub_821496F8.r3.+4, sub_821496F8.r3.+4.+0, sub_821496F8.r3.+4.+8, sub_821497A8.r3.+4, sub_821497A8.r3.+4.+0, sub_821497A8.r3.+4.+8, sub_82149888.r3.+4, sub_821498F0.r3.+4, sub_82149888.r4, sub_82149888.r4.+4, sub_82149888.r4.+8, sub_82149888.r4.+8.+0, sub_82149AA8.r4, sub_82149AA8.r4.+0, sub_82149CA0.ret.+0.+4, sub_82149CA0.ret.+0.+4.+0, sub_82149CA0.ret.+0.+4.+0.+8, sub_8214A950.ret.+0.+4, sub_8214A950.ret.+0.+4.+8, sub_8214A950.ret.+0.+4.+8.+0, sub_8214C708.r4.+4, sub_8214C708.r4.+4.+0, sub_8214C708.r4.+4.+0.+8, sub_8214C828.r4.+4, sub_8214C828.r4.+4.+0, sub_8214C828.r4.+4.+8, sub_8214BCE0.r4.+4
struct SysLinkIdSetNode : View {
  using View::View;
  static constexpr uint32_t kSize = 26;
  template <typename T = SysLinkIdSetNode>
  T left() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_left(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t left_address() const { return address + 0; }
  template <typename T = SysLinkIdSetNode>
  T parent() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_parent(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_parent(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t parent_address() const { return address + 4; }
  template <typename T = SysLinkIdSetNode>
  T right() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_right(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_right(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t right_address() const { return address + 8; }
  uint64_t id() const { return guest::LoadU64(base, address + 16); }
  void set_id(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t id_address() const { return address + 16; }
  uint8_t color() const { return guest::LoadU8(base, address + 24); }
  void set_color(uint8_t v) const { guest::StoreU8(base, address + 24, v); }
  uint32_t color_address() const { return address + 24; }
  uint8_t is_nil() const { return guest::LoadU8(base, address + 25); }
  void set_is_nil(uint8_t v) const { guest::StoreU8(base, address + 25, v); }
  uint32_t is_nil_address() const { return address + 25; }
};

// embedded; measure: sub_821496F8.r3, sub_821497A8.r3, sub_82149BF0.ret, sub_82149CA0.r3, sub_82149CA0.ret, sub_8214A950.r3, sub_8214A950.ret, sub_8214BCE0.ret, sub_8214BCE0.r3, sub_8214FCC8.r3, sub_8214FCC8.ret, sub_8214FCC8.r4, sub_8214C708.r4, sub_8214C828.r3
struct SysLinkIdSetIter : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // both words copied as one doubleword
  uint64_t whole() const { return guest::LoadU64(base, address + 0); }
  void set_whole(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t whole_address() const { return address + 0; }
  template <typename T = SysLinkIdSet>
  T container() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_container(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_container(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t container_address() const { return address + 0; }
  template <typename T = SysLinkIdSetNode>
  T node() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_node(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_address() const { return address + 4; }
};

// embedded; measure: sub_8214A9B0.r3
struct SysLinkIdSetRange : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  template <typename T = SysLinkIdSet>
  T first_container() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_first_container(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_first_container(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_container_address() const { return address + 0; }
  uint32_t first_node() const { return guest::LoadU32(base, address + 4); }
  void set_first_node(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_node_address() const { return address + 4; }
  template <typename T = SysLinkIdSet>
  T last_container() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_last_container(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_last_container(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_container_address() const { return address + 8; }
  uint32_t last_node() const { return guest::LoadU32(base, address + 12); }
  void set_last_node(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t last_node_address() const { return address + 12; }
};

// embedded; measure: sub_82260468.r3
struct SysLiveBlock1592 : View {
  using View::View;
  static constexpr uint32_t kSize = 1592;
  uint16_t unknown_0() const { return guest::LoadU16(base, address + 0); }
  void set_unknown_0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_2() const { return guest::LoadU16(base, address + 2); }
  void set_unknown_2(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint16_t unknown_4() const { return guest::LoadU16(base, address + 4); }
  void set_unknown_4(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint64_t unknown_288() const { return guest::LoadU64(base, address + 288); }
  void set_unknown_288(uint64_t v) const { guest::StoreU64(base, address + 288, v); }
  uint32_t unknown_288_address() const { return address + 288; }
  uint64_t unknown_768() const { return guest::LoadU64(base, address + 768); }
  void set_unknown_768(uint64_t v) const { guest::StoreU64(base, address + 768, v); }
  uint32_t unknown_768_address() const { return address + 768; }
  uint64_t unknown_928() const { return guest::LoadU64(base, address + 928); }
  void set_unknown_928(uint64_t v) const { guest::StoreU64(base, address + 928, v); }
  uint32_t unknown_928_address() const { return address + 928; }
  uint64_t unknown_1088() const { return guest::LoadU64(base, address + 1088); }
  void set_unknown_1088(uint64_t v) const { guest::StoreU64(base, address + 1088, v); }
  uint32_t unknown_1088_address() const { return address + 1088; }
  uint32_t unknown_1568() const { return guest::LoadU32(base, address + 1568); }
  void set_unknown_1568(uint32_t v) const { guest::StoreU32(base, address + 1568, v); }
  uint32_t unknown_1568_address() const { return address + 1568; }
  uint32_t unknown_1588() const { return guest::LoadU32(base, address + 1588); }
  void set_unknown_1588(uint32_t v) const { guest::StoreU32(base, address + 1588, v); }
  uint32_t unknown_1588_address() const { return address + 1588; }
};

// embedded; measure: sub_822606B8.r3
struct SysLiveBlock32 : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint64_t unknown_0() const { return guest::LoadU64(base, address + 0); }
  void set_unknown_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint64_t unknown_8() const { return guest::LoadU64(base, address + 8); }
  void set_unknown_8(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint64_t unknown_16() const { return guest::LoadU64(base, address + 16); }
  void set_unknown_16(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint64_t unknown_24() const { return guest::LoadU64(base, address + 24); }
  void set_unknown_24(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
};

// embedded; measure: sub_8214C708.r3
struct SysLinkIdSetInsertResult : View {
  using View::View;
  static constexpr uint32_t kSize = 9;
  // the iterator copied as one doubleword
  uint64_t whole() const { return guest::LoadU64(base, address + 0); }
  void set_whole(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t whole_address() const { return address + 0; }
  template <typename T = SysLinkIdSet>
  T container() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_container(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_container(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t container_address() const { return address + 0; }
  template <typename T = SysLinkIdSetNode>
  T node() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_node(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_address() const { return address + 4; }
  uint8_t inserted() const { return guest::LoadU8(base, address + 8); }
  void set_inserted(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t inserted_address() const { return address + 8; }
};

// embedded; measure: sub_82149830.r4, sub_82149960.r4, sub_82149BF0.r3, sub_82149BF0.r4, sub_8214B188.r3, sub_8214BCE0.r7, sub_8214BF20.r3, sub_8214C708.r5, sub_8214CD20.r3, sub_8214FCC8.r5
struct SysLinkId : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint64_t id() const { return guest::LoadU64(base, address + 0); }
  void set_id(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
};

// embedded; measure: sub_8214A3B0.r3, sub_8214CFB8.r3, sub_82148768.r3, sub_82148F88.r3
struct SysLinkPacket : View {
  using View::View;
  static constexpr uint32_t kSize = 1277;
  uint8_t unknown_2() const { return guest::LoadU8(base, address + 2); }
  void set_unknown_2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  // probably the sender's id (unaligned)
  uint64_t sender_id() const { return guest::LoadU64(base, address + 3); }
  void set_sender_id(uint64_t v) const { guest::StoreU64(base, address + 3, v); }
  uint32_t sender_id_address() const { return address + 3; }
  uint8_t unknown_11() const { return guest::LoadU8(base, address + 11); }
  void set_unknown_11(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t unknown_11_address() const { return address + 11; }
  uint32_t unknown_192() const { return guest::LoadU32(base, address + 192); }
  void set_unknown_192(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t unknown_192_address() const { return address + 192; }
  uint16_t unknown_1275() const { return guest::LoadU16(base, address + 1275); }
  void set_unknown_1275(uint16_t v) const { guest::StoreU16(base, address + 1275, v); }
  uint32_t unknown_1275_address() const { return address + 1275; }
};

// embedded; measure: sub_8214A3B0.r3@+11
struct SysLinkMessage3 : View {
  using View::View;
  static constexpr uint32_t kSize = 3;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_1() const { return guest::LoadU16(base, address + 1); }
  void set_unknown_1(uint16_t v) const { guest::StoreU16(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
};

// embedded; measure: sub_8214FDF8.r4
struct SysLinkMessage9 : View {
  using View::View;
  static constexpr uint32_t kSize = 9;
  uint8_t unknown_8() const { return guest::LoadU8(base, address + 8); }
  void set_unknown_8(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: sub_8214A8E0.ret, sub_821492B8.r3, sub_8214E908.r4, sub_8214EB28.r4
struct SysLinkRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 304;
  uint64_t unknown_0() const { return guest::LoadU64(base, address + 0); }
  void set_unknown_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint8_t unknown_12() const { return guest::LoadU8(base, address + 12); }
  void set_unknown_12(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint64_t unknown_144() const { return guest::LoadU64(base, address + 144); }
  void set_unknown_144(uint64_t v) const { guest::StoreU64(base, address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  uint32_t unknown_176() const { return guest::LoadU32(base, address + 176); }
  void set_unknown_176(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
  double unknown_192() const { return std::bit_cast<double>(guest::LoadU64(base, address + 192)); }
  void set_unknown_192(double v) const { guest::StoreU64(base, address + 192, std::bit_cast<uint64_t>(v)); }
  uint32_t unknown_192_address() const { return address + 192; }
  double unknown_224() const { return std::bit_cast<double>(guest::LoadU64(base, address + 224)); }
  void set_unknown_224(double v) const { guest::StoreU64(base, address + 224, std::bit_cast<uint64_t>(v)); }
  uint32_t unknown_224_address() const { return address + 224; }
  uint32_t unknown_292() const { return guest::LoadU32(base, address + 292); }
  void set_unknown_292(uint32_t v) const { guest::StoreU32(base, address + 292, v); }
  uint32_t unknown_292_address() const { return address + 292; }
  uint32_t unknown_300() const { return guest::LoadU32(base, address + 300); }
  void set_unknown_300(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t unknown_300_address() const { return address + 300; }
};

// embedded; measure: sub_8214CEF0.r3, sub_8214CD20.r4
struct SysLinkVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // probably
  uint32_t first() const { return guest::LoadU32(base, address + 4); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  // probably
  uint32_t last() const { return guest::LoadU32(base, address + 8); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  // probably
  uint32_t end() const { return guest::LoadU32(base, address + 12); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
};

// embedded; measure: sub_8214F910.r3
struct SysLinkBlock20 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: sub_8214F910.r4
struct SysLinkBlock16 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: sub_8214E6C8.r3, sub_8214E778.r3, sub_8214E9C8.r3, sub_8214EB28.r3, sub_8214ED30.r3, sub_8214E850.r3, GetLastError_82295F18.ret
struct SysLiveSession : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint64_t unknown_72() const { return guest::LoadU64(base, address + 72); }
  void set_unknown_72(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_80() const { return guest::LoadU32(base, address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint32_t unknown_84() const { return guest::LoadU32(base, address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  uint32_t unknown_88() const { return guest::LoadU32(base, address + 88); }
  void set_unknown_88(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  uint32_t unknown_120() const { return guest::LoadU32(base, address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  uint32_t unknown_124() const { return guest::LoadU32(base, address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  uint32_t unknown_128() const { return guest::LoadU32(base, address + 128); }
  void set_unknown_128(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  uint32_t unknown_132() const { return guest::LoadU32(base, address + 132); }
  void set_unknown_132(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  uint32_t unknown_136() const { return guest::LoadU32(base, address + 136); }
  void set_unknown_136(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  uint32_t unknown_140() const { return guest::LoadU32(base, address + 140); }
  void set_unknown_140(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
};

// embedded; measure: sub_822950C0.r7
struct SysXMsgBlock8 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: recvfrom.ret
struct SysRecvFromAnswer : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// allocated; measure: new<8>@Type12Screen7::Create
struct NetScreen7DataData {
  uint32_t unknown_0 = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
};

struct NetScreen7Data : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  NetScreen7DataData* data() const { return static_cast<NetScreen7DataData*>(memory::HandleObject(address)); }
  static NetScreen7Data New(uint8_t* base) {
    return NetScreen7Data(base, memory::MakeHandle(new NetScreen7DataData(), kSize, "NetScreen7Data"));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint32_t unknown_0() const { return data()->unknown_0; }
  void set_unknown_0(uint32_t v) const { data()->unknown_0 = v; }
  uint32_t unknown_0_bits() const { return data()->unknown_0; }
  void set_unknown_0_bits(uint32_t v) const { data()->unknown_0 = v; }
  uint32_t unknown_4() const { return data()->unknown_4; }
  void set_unknown_4(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_4_bits() const { return data()->unknown_4; }
  void set_unknown_4_bits(uint32_t v) const { data()->unknown_4 = v; }
};

// embedded; measure: sub_82577788.r3@+28, sub_82577850.r3@+8, sub_82577850.r3, sub_82577850.r3@+28
struct XNetContext : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  template <typename T = XNetConfig>
  T config() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_config(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_config(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t config_address() const { return address + 0; }
  template <typename T = XNetBuffer>
  T buffer() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_buffer(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_buffer(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t buffer_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
};

// embedded; measure: sub_825767A0.r3, sub_82577850.r3.+4
struct XNetBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 516;
  uint32_t unknown_512() const { return guest::LoadU32(base, address + 512); }
  void set_unknown_512(uint32_t v) const { guest::StoreU32(base, address + 512, v); }
  uint32_t unknown_512_address() const { return address + 512; }
};

// embedded; measure: sub_825776A8.r3
struct XNetBlock76 : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(base, address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  template <typename T = SysOutWord>
  T unknown_72() const { return T(base, guest::LoadU32(base, address + 72)); }
  void set_unknown_72(const View& v) const { guest::StoreU32(base, address + 72, v.address); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
};

// embedded; measure: sub_82577788.r3, sub_825778D8.r3
struct XNetRequest : View {
  using View::View;
  static constexpr uint32_t kSize = 1084;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(base, address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint64_t unknown_64() const { return guest::LoadU64(base, address + 64); }
  void set_unknown_64(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_1080() const { return guest::LoadU32(base, address + 1080); }
  void set_unknown_1080(uint32_t v) const { guest::StoreU32(base, address + 1080, v); }
  uint32_t unknown_1080_address() const { return address + 1080; }
};

// embedded; measure: sub_825780D0.r3, sub_82578500.r3
struct XNetEntry20 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: sub_825780F0.r3, sub_82578118.r3, sub_82577190.r3
struct XNetEntry16 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: sub_82577528.r3
struct XNetRecord26 : View {
  using View::View;
  static constexpr uint32_t kSize = 26;
  uint16_t unknown_24() const { return guest::LoadU16(base, address + 24); }
  void set_unknown_24(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
};

// embedded; measure: sub_82576BE0.r7
struct XLiveRequestArgs : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
};

// embedded; measure: sub_825783C8.r4
struct XNetU64Arg : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint64_t value() const { return guest::LoadU64(base, address + 0); }
  void set_value(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: sub_82149B68.r5, sub_822950C0.r8, sub_82575B68.r4, sub_82575BF0.r7, sub_82575BF0.r8, sub_82575BF0.r9, sub_82575D78.r10, sub_825763D0.r6, sub_82577528.r5, sub_82577850.r5, sub_82577850.r6, sub_825778D8.r5, sub_82578118.r4, sub_82578118.r5, sub_82578340.r4, sub_82578608.r5, sub_825786B8.r7, WideCharToMultiByte.r10, sub_825776A8.r3.+72
struct SysOutWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: XLaunchNewImage.r3, sub_82577140.r4, sub_825786B8.r6
struct SysOutByte : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t value() const { return guest::LoadU8(base, address + 0); }
  void set_value(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: sub_825782C0.r4, sub_82578588.r8
struct SysOutHalf : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint16_t value() const { return guest::LoadU16(base, address + 0); }
  void set_value(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
  // sub_82578588's r8
  uint8_t byte1() const { return guest::LoadU8(base, address + 1); }
  void set_byte1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t byte1_address() const { return address + 1; }
};

// embedded; measure: sub_82297340.r3, sub_82297540.r5
struct Utf8Bytes : View {
  using View::View;
  static constexpr uint32_t kSize = 6;
  uint8_t byte0() const { return guest::LoadU8(base, address + 0); }
  void set_byte0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t byte0_address() const { return address + 0; }
  uint8_t byte1() const { return guest::LoadU8(base, address + 1); }
  void set_byte1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t byte1_address() const { return address + 1; }
  uint8_t byte2() const { return guest::LoadU8(base, address + 2); }
  void set_byte2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t byte2_address() const { return address + 2; }
  uint8_t byte3() const { return guest::LoadU8(base, address + 3); }
  void set_byte3(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t byte3_address() const { return address + 3; }
  uint8_t byte4() const { return guest::LoadU8(base, address + 4); }
  void set_byte4(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t byte4_address() const { return address + 4; }
  uint8_t byte5() const { return guest::LoadU8(base, address + 5); }
  void set_byte5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t byte5_address() const { return address + 5; }
};

// embedded; measure: sub_82297340.r5, sub_82297540.r3
struct Utf16Chars : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint16_t char0() const { return guest::LoadU16(base, address + 0); }
  void set_char0(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t char0_address() const { return address + 0; }
  uint16_t char1() const { return guest::LoadU16(base, address + 2); }
  void set_char1(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t char1_address() const { return address + 2; }
};

// The image words docs/functions/system-services.md's code uses.
struct SystemServicesGlobals {
  // the profile read's XOVERLAPPED (SysXOverlapped): 997 while a read is pending
  static uint32_t profile_overlapped(uint8_t* base) { return guest::LoadU32(base, 0x826C6080u); }
  static void set_profile_overlapped(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6080u, v); }
  static constexpr uint32_t profile_overlapped_address() { return 0x826C6080u; }
  // the profile read's results
  template <typename T = SysProfileResults>
  static T profile_results(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C6AD0u)); }
  static void set_profile_results(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C6AD0u, v.address); }
  static void set_profile_results(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6AD0u, v); }
  static constexpr uint32_t profile_results_address() { return 0x826C6AD0u; }
  // the system link socket (INVALID_SOCKET since B5: not made)
  static uint32_t system_link_socket(uint8_t* base) { return guest::LoadU32(base, 0x826C6AA0u); }
  static void set_system_link_socket(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6AA0u, v); }
  static constexpr uint32_t system_link_socket_address() { return 0x826C6AA0u; }
  // the network manager (NetworkManager, 460 bytes)
  static constexpr uint32_t network_manager_address() { return 0x826C74A0u; }
  // the user table (NetUserTable, 384 bytes)
  static constexpr uint32_t user_table_address() { return 0x826C7670u; }
  // where the gamercard region is also copied after a read
  static uint32_t region_copy_6728(uint8_t* base) { return guest::LoadU32(base, 0x826C6728u); }
  static void set_region_copy_6728(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6728u, v); }
  static constexpr uint32_t region_copy_6728_address() { return 0x826C6728u; }
  // where the gamercard region is also copied after a read
  static uint32_t region_copy_7288(uint8_t* base) { return guest::LoadU32(base, 0x826C7288u); }
  static void set_region_copy_7288(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7288u, v); }
  static constexpr uint32_t region_copy_7288_address() { return 0x826C7288u; }
  // 1 asks for the profile read (set by the controller assignments)
  static uint32_t profile_requested(uint8_t* base) { return guest::LoadU32(base, 0x826C7938u); }
  static void set_profile_requested(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7938u, v); }
  static constexpr uint32_t profile_requested_address() { return 0x826C7938u; }
  // set after a profile read, until its region is copied
  static uint32_t profile_was_read(uint8_t* base) { return guest::LoadU32(base, 0x826C793Cu); }
  static void set_profile_was_read(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C793Cu, v); }
  static constexpr uint32_t profile_was_read_address() { return 0x826C793Cu; }
  static uint32_t sign_in_polled(uint8_t* base) { return guest::LoadU32(base, 0x826C7950u); }
  static void set_sign_in_polled(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7950u, v); }
  static constexpr uint32_t sign_in_polled_address() { return 0x826C7950u; }
  static uint32_t signed_in_mask(uint8_t* base) { return guest::LoadU32(base, 0x826C7954u); }
  static void set_signed_in_mask(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7954u, v); }
  static constexpr uint32_t signed_in_mask_address() { return 0x826C7954u; }
  static uint32_t signed_in_count(uint8_t* base) { return guest::LoadU32(base, 0x826C7958u); }
  static void set_signed_in_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7958u, v); }
  static constexpr uint32_t signed_in_count_address() { return 0x826C7958u; }
  // the users on Live (0 from B5's stub)
  static uint32_t live_mask(uint8_t* base) { return guest::LoadU32(base, 0x826C795Cu); }
  static void set_live_mask(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C795Cu, v); }
  static constexpr uint32_t live_mask_address() { return 0x826C795Cu; }
  static uint32_t sign_in_checked(uint8_t* base) { return guest::LoadU32(base, 0x826C796Cu); }
  static void set_sign_in_checked(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C796Cu, v); }
  static constexpr uint32_t sign_in_checked_address() { return 0x826C796Cu; }
  // set by the stub's first poll
  static uint32_t users_polled(uint8_t* base) { return guest::LoadU32(base, 0x826C7998u); }
  static void set_users_polled(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7998u, v); }
  static constexpr uint32_t users_polled_address() { return 0x826C7998u; }
  // the first signed-in user (-1 none)
  static uint32_t first_user(uint8_t* base) { return guest::LoadU32(base, 0x82618B84u); }
  static void set_first_user(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82618B84u, v); }
  static constexpr uint32_t first_user_address() { return 0x82618B84u; }
  // the user on player 1's controller
  static uint32_t player_controller(uint8_t* base) { return guest::LoadU32(base, 0x826BABA4u); }
  static void set_player_controller(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABA4u, v); }
  static constexpr uint32_t player_controller_address() { return 0x826BABA4u; }
  // the heap xstart makes (sub_822997B0's handle; heap.toml's ProcessHeap)
  static uint32_t heap(uint8_t* base) { return guest::LoadU32(base, 0x8297149Cu); }
  static void set_heap(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8297149Cu, v); }
  static constexpr uint32_t heap_address() { return 0x8297149Cu; }
  // a C runtime word xstart sets to -1 first (not read in detail)
  static uint32_t crt_word_a(uint8_t* base) { return guest::LoadU32(base, 0x82978330u); }
  static void set_crt_word_a(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82978330u, v); }
  static constexpr uint32_t crt_word_a_address() { return 0x82978330u; }
  // the second, also -1
  static uint32_t crt_word_b(uint8_t* base) { return guest::LoadU32(base, 0x82978334u); }
  static void set_crt_word_b(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82978334u, v); }
  static constexpr uint32_t crt_word_b_address() { return 0x82978334u; }
  // xstart splits the command line into main's arguments when set (nothing sets it)
  static uint32_t command_line_set(uint8_t* base) { return guest::LoadU32(base, 0x829537B8u); }
  static void set_command_line_set(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x829537B8u, v); }
  static constexpr uint32_t command_line_set_address() { return 0x829537B8u; }
};

}  // namespace schema
