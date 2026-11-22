// The views of docs/functions/network-mode.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/network-mode.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ModeType12, ModeType12::LeaveScreen.r4, ModeType12::NextMode.r4, ModeType12::Teardown.r4, ModeType12::WaitForLoads.r4, ModeType12::WaitForShown.r4
struct ModeType12 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr uint32_t kVtable = 0x82006C6C;
  // 0x82006C6C
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the next-mode answer's byte +0 (the delegate functions' r3), not the mode's
  uint8_t answer_changed() const { return guest::LoadU8(base, address + 0); }
  void set_answer_changed(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t answer_changed_address() const { return address + 0; }
  // the delegate at +8, 0 when none (also the answer's +4 by the measure's join)
  uint32_t delegate_ptr() const { return guest::LoadU32(base, address + 4); }
  void set_delegate_ptr(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_address() const { return address + 4; }
  // the delegate: vtable 0x82010FA0
  uint32_t delegate_vtable() const { return guest::LoadU32(base, address + 8); }
  void set_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable_address() const { return address + 8; }
  // the delegate's object: the mode
  uint32_t delegate_object() const { return guest::LoadU32(base, address + 12); }
  void set_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object_address() const { return address + 12; }
  // the delegate function's thunk (Mode::Slot8Thunk, NextModeThunk, Slot10Thunk to Slot12Thunk)
  uint32_t delegate_function() const { return guest::LoadU32(base, address + 16); }
  void set_delegate_function(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_adjustment() const { return guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  // the +32 interface's vtable 0x82006C60 (Interrupt, ForceExit, Resume)
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (the fourteen machines)
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // the dispatcher's ring: 50 messages of 12 bytes (id, two words); the +32 interface's +808 and +812 are ring_count and ring_start
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
  // the block, VirtualAlloc'ed by Init (the +32 interface's +816)
  template <typename T = NetModeBlock>
  T block() const { return T(base, guest::LoadU32(base, address + 848)); }
  void set_block(const View& v) const { guest::StoreU32(base, address + 848, v.address); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 848, v); }
  uint32_t block_address() const { return address + 848; }
  // measure fault: AllLoaded's r3 is the block (NetModeBlock): the mode select's loaded word
  uint32_t select_loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_select_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t select_loaded_address() const { return address + 9608; }
  // measure fault: the block's screen 1 (+9640) loaded word, read by AllLoaded
  uint32_t screen1_loaded() const { return guest::LoadU32(base, address + 19248); }
  void set_screen1_loaded(uint32_t v) const { guest::StoreU32(base, address + 19248, v); }
  uint32_t screen1_loaded_address() const { return address + 19248; }
  // measure fault: the block's screen 3 (+19280) loaded word, read by AllLoaded
  uint32_t screen3_loaded() const { return guest::LoadU32(base, address + 28888); }
  void set_screen3_loaded(uint32_t v) const { guest::StoreU32(base, address + 28888, v); }
  uint32_t screen3_loaded_address() const { return address + 28888; }
};

// allocated; measure: new<135208>@ModeType12::Init, ModeType12.+816, ModeType12::Teardown.r4.+848
struct NetModeBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 135208;
  // index 0, NetModeSelect
  uint32_t mode_select_address() const { return address + 0; }
  // index 1, Type12Screen1
  uint32_t screen1_address() const { return address + 9640; }
  // index 3, Type12Screen3
  uint32_t screen3_address() const { return address + 19280; }
  // index 4, NetRanking
  uint32_t ranking_address() const { return address + 28920; }
  // index 5, NetTaCourse
  uint32_t ta_course_address() const { return address + 38560; }
  // index 14, PowerGraphScreen12 (type2-screens.md)
  uint32_t power_graph_address() const { return address + 48200; }
  // index 6, Type12Screen6
  uint32_t screen6_address() const { return address + 57840; }
  // index 7, Type12Screen7
  uint32_t screen7_address() const { return address + 67500; }
  // index 10, Type12Dialog10
  uint32_t dialog10_address() const { return address + 77232; }
  // index 11, Type12Dialog11
  uint32_t dialog11_address() const { return address + 86880; }
  // index 8, Type12Screen8
  uint32_t screen8_address() const { return address + 96524; }
  // index 9, Type12Screen9
  uint32_t screen9_address() const { return address + 106192; }
  // index 12, Type12Dialog12
  uint32_t dialog12_address() const { return address + 115888; }
  // index 13, Type12Dialog13
  uint32_t dialog13_address() const { return address + 125536; }
  // +0x2100C: the shown machine's index (ModeType12::FindShown)
  uint32_t shown() const { return guest::LoadU32(base, address + 135180); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 135180, v); }
  uint32_t shown_address() const { return address + 135180; }
  // +0x21010: the first screen's index (Init)
  uint32_t first_shown() const { return guest::LoadU32(base, address + 135184); }
  void set_first_shown(uint32_t v) const { guest::StoreU32(base, address + 135184, v); }
  uint32_t first_shown_address() const { return address + 135184; }
  // +0x21014: the next mode's 8-byte answer (NextMode, handed over by Teardown)
  uint64_t next_answer() const { return guest::LoadU64(base, address + 135188); }
  void set_next_answer(uint64_t v) const { guest::StoreU64(base, address + 135188, v); }
  uint32_t next_answer_address() const { return address + 135188; }
  // the answer's first word (byte 1 the change, then the frame's bytes)
  uint32_t next_answer_hi() const { return guest::LoadU32(base, address + 135188); }
  void set_next_answer_hi(uint32_t v) const { guest::StoreU32(base, address + 135188, v); }
  uint32_t next_answer_hi_address() const { return address + 135188; }
  // the answer's second word: the next mode's type
  uint32_t next_answer_mode() const { return guest::LoadU32(base, address + 135192); }
  void set_next_answer_mode(uint32_t v) const { guest::StoreU32(base, address + 135192, v); }
  uint32_t next_answer_mode_address() const { return address + 135192; }
  // +0x21024: set by Interrupt (back to the title, the session freed)
  uint32_t session_made() const { return guest::LoadU32(base, address + 135204); }
  void set_session_made(uint32_t v) const { guest::StoreU32(base, address + 135204, v); }
  uint32_t session_made_address() const { return address + 135204; }
};

// embedded; measure: NetModeSelect, NetRanking, NetTaCourse, Type12Screen3, Type12Screen6, Type12Screen7, Type12Screen8, Type12Screen9, Type12Dialog10, Type12Dialog11, Type12Dialog12, Type12Dialog13, ModeType12::Machine.ret, sub_8225E418.r3, sub_8226A660.r3, sub_82263D80.r3, sub_822657D8.r3, sub_82265930.r3, sub_82266028.r3, sub_822662F0.r3, sub_822663E0.r3, sub_82266548.r3, sub_82266658.r3, sub_822681D0.r3, sub_82268720.r3, sub_82269988.r3, sub_8226CB40.r3, sub_822702C0.r3, sub_82277178.r3, sub_822800C0.r3, sub_82282630.r3, sub_822659F0.ret, sub_822681D0.ret
struct NetMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9732;
  // 200 transitions of 48 bytes (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 to 15: 5 running, 7 left, 8 unloading, 9 and 10 opening, 11 the race, 13 the garage, 14 the title, 15 the mode again
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // its movie loaded (WaitForMovie)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // open on its message: Create goes to state 1, else 9 (1 from Init)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // delete the movie on unload (0 from Init)
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
  // 1: back to the title (Destroy goes to state 14)
  uint32_t back() const { return guest::LoadU32(base, address + 9624); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 9624, v); }
  uint32_t back_address() const { return address + 9624; }
  // 1: to the race (screen 7's Destroy goes to state 11)
  uint32_t race() const { return guest::LoadU32(base, address + 9628); }
  void set_race(uint32_t v) const { guest::StoreU32(base, address + 9628, v); }
  uint32_t race_address() const { return address + 9628; }
  // 1: the mode again (Destroy goes to state 15); 0 from Resume
  uint32_t again() const { return guest::LoadU32(base, address + 9632); }
  void set_again(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t again_address() const { return address + 9632; }
  // set by ModeType12::Interrupt on the shown machine
  uint32_t interrupted() const { return guest::LoadU32(base, address + 9636); }
  void set_interrupted(uint32_t v) const { guest::StoreU32(base, address + 9636, v); }
  uint32_t interrupted_address() const { return address + 9636; }
  // machines 6 to 13's own words from here (0 or 1 from their Creates)
  uint32_t unknown_9640() const { return guest::LoadU32(base, address + 9640); }
  void set_unknown_9640(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t unknown_9640_address() const { return address + 9640; }
  uint32_t unknown_9644() const { return guest::LoadU32(base, address + 9644); }
  void set_unknown_9644(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t unknown_9644_address() const { return address + 9644; }
  uint32_t unknown_9648() const { return guest::LoadU32(base, address + 9648); }
  void set_unknown_9648(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t unknown_9648_address() const { return address + 9648; }
  uint32_t unknown_9652() const { return guest::LoadU32(base, address + 9652); }
  void set_unknown_9652(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t unknown_9652_address() const { return address + 9652; }
  uint32_t unknown_9656() const { return guest::LoadU32(base, address + 9656); }
  void set_unknown_9656(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t unknown_9656_address() const { return address + 9656; }
  uint32_t unknown_9660() const { return guest::LoadU32(base, address + 9660); }
  void set_unknown_9660(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t unknown_9660_address() const { return address + 9660; }
  uint32_t unknown_9664() const { return guest::LoadU32(base, address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
  uint32_t unknown_9668() const { return guest::LoadU32(base, address + 9668); }
  void set_unknown_9668(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t unknown_9668_address() const { return address + 9668; }
  uint32_t unknown_9672() const { return guest::LoadU32(base, address + 9672); }
  void set_unknown_9672(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t unknown_9672_address() const { return address + 9672; }
  uint32_t unknown_9676() const { return guest::LoadU32(base, address + 9676); }
  void set_unknown_9676(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t unknown_9676_address() const { return address + 9676; }
  uint32_t unknown_9680() const { return guest::LoadU32(base, address + 9680); }
  void set_unknown_9680(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t unknown_9680_address() const { return address + 9680; }
  uint32_t unknown_9684() const { return guest::LoadU32(base, address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  uint32_t unknown_9688() const { return guest::LoadU32(base, address + 9688); }
  void set_unknown_9688(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t unknown_9688_address() const { return address + 9688; }
  uint32_t unknown_9692() const { return guest::LoadU32(base, address + 9692); }
  void set_unknown_9692(uint32_t v) const { guest::StoreU32(base, address + 9692, v); }
  uint32_t unknown_9692_address() const { return address + 9692; }
  uint32_t unknown_9696() const { return guest::LoadU32(base, address + 9696); }
  void set_unknown_9696(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t unknown_9696_address() const { return address + 9696; }
  uint32_t unknown_9700() const { return guest::LoadU32(base, address + 9700); }
  void set_unknown_9700(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t unknown_9700_address() const { return address + 9700; }
  uint32_t unknown_9704() const { return guest::LoadU32(base, address + 9704); }
  void set_unknown_9704(uint32_t v) const { guest::StoreU32(base, address + 9704, v); }
  uint32_t unknown_9704_address() const { return address + 9704; }
  uint32_t unknown_9708() const { return guest::LoadU32(base, address + 9708); }
  void set_unknown_9708(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t unknown_9708_address() const { return address + 9708; }
  uint32_t unknown_9712() const { return guest::LoadU32(base, address + 9712); }
  void set_unknown_9712(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t unknown_9712_address() const { return address + 9712; }
  uint32_t unknown_9716() const { return guest::LoadU32(base, address + 9716); }
  void set_unknown_9716(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t unknown_9716_address() const { return address + 9716; }
  uint32_t unknown_9720() const { return guest::LoadU32(base, address + 9720); }
  void set_unknown_9720(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t unknown_9720_address() const { return address + 9720; }
  uint32_t unknown_9724() const { return guest::LoadU32(base, address + 9724); }
  void set_unknown_9724(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t unknown_9724_address() const { return address + 9724; }
  uint32_t unknown_9728() const { return guest::LoadU32(base, address + 9728); }
  void set_unknown_9728(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t unknown_9728_address() const { return address + 9728; }
};

// embedded; measure: NetModeSelect::Leave.r4, NetModeSelect::Unloaded.r4, NetRanking::Leave.r4, NetTaCourse::Leave.r4, Type12Dialog10::Leave.r4, Type12Dialog11::Leave.r4, Type12Dialog12::Leave.r4, Type12Dialog13::Leave.r4, Type12Screen1::Leave.r4, Type12Screen3::Leave.r4, Type12Screen6::Leave.r4, Type12Screen7::Leave.r4, Type12Screen8::Leave.r4, Type12Screen9::Leave.r4, sub_8225E418.r4, sub_82285630.r4
struct NetModeDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 handler pointers (the machines)
  uint32_t handlers_address() const { return address + 0; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 200); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t handler_count_address() const { return address + 200; }
  // 50 messages of 12 bytes (id, two words)
  uint32_t ring_address() const { return address + 204; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count_address() const { return address + 804; }
  // the ring's first slot
  uint32_t ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start_address() const { return address + 808; }
};

// allocated; measure: new<36>@NetModeSelect::Create
struct NetModeSelectDataData {
  uint32_t shown_before = 0;  // +0
  uint32_t back = 0;  // +4
  uint32_t back_to_title = 0;  // +8
  uint32_t sign_in_ui_shown = 0;  // +12
  uint32_t privilege_dialog = 0;  // +16
  uint32_t dialog_page = 0;  // +20
  uint32_t dialog_transition = 0;  // +24
  uint32_t sign_in_ticks = 0;  // +28
  uint32_t sign_in_continues = 0;  // +32
};

struct NetModeSelectData : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  static constexpr bool kHost = true;
  NetModeSelectDataData* data() const { return static_cast<NetModeSelectDataData*>(memory::HandleObject(address)); }
  static NetModeSelectData New(uint8_t* base) {
    return NetModeSelectData(base, memory::MakeHandle(new NetModeSelectDataData(), kSize, "NetModeSelectData"));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  // 1 once set up (SetUp): Open starts with ShowCursor, not Intro
  uint32_t shown_before() const { return data()->shown_before; }
  void set_shown_before(uint32_t v) const { data()->shown_before = v; }
  uint32_t shown_before_bits() const { return data()->shown_before; }
  void set_shown_before_bits(uint32_t v) const { data()->shown_before = v; }
  // back to the title (Destroy goes to state 14)
  uint32_t back() const { return data()->back; }
  void set_back(uint32_t v) const { data()->back = v; }
  uint32_t back_bits() const { return data()->back; }
  void set_back_bits(uint32_t v) const { data()->back = v; }
  // set by BackPage: Unloaded goes to state 14
  uint32_t back_to_title() const { return data()->back_to_title; }
  void set_back_to_title(uint32_t v) const { data()->back_to_title = v; }
  uint32_t back_to_title_bits() const { return data()->back_to_title; }
  void set_back_to_title_bits(uint32_t v) const { data()->back_to_title = v; }
  uint32_t sign_in_ui_shown() const { return data()->sign_in_ui_shown; }
  void set_sign_in_ui_shown(uint32_t v) const { data()->sign_in_ui_shown = v; }
  uint32_t sign_in_ui_shown_bits() const { return data()->sign_in_ui_shown; }
  void set_sign_in_ui_shown_bits(uint32_t v) const { data()->sign_in_ui_shown = v; }
  // dialog 12 (no privilege) up
  uint32_t privilege_dialog() const { return data()->privilege_dialog; }
  void set_privilege_dialog(uint32_t v) const { data()->privilege_dialog = v; }
  uint32_t privilege_dialog_bits() const { return data()->privilege_dialog; }
  void set_privilege_dialog_bits(uint32_t v) const { data()->privilege_dialog = v; }
  // the page a dialog's close goes to (-1 none)
  uint32_t dialog_page() const { return data()->dialog_page; }
  void set_dialog_page(uint32_t v) const { data()->dialog_page = v; }
  uint32_t dialog_page_bits() const { return data()->dialog_page; }
  void set_dialog_page_bits(uint32_t v) const { data()->dialog_page = v; }
  // the transition a dialog's close runs (-1 none)
  uint32_t dialog_transition() const { return data()->dialog_transition; }
  void set_dialog_transition(uint32_t v) const { data()->dialog_transition = v; }
  uint32_t dialog_transition_bits() const { return data()->dialog_transition; }
  void set_dialog_transition_bits(uint32_t v) const { data()->dialog_transition = v; }
  // the sign-in pages' tick count, to 10
  uint32_t sign_in_ticks() const { return data()->sign_in_ticks; }
  void set_sign_in_ticks(uint32_t v) const { data()->sign_in_ticks = v; }
  uint32_t sign_in_ticks_bits() const { return data()->sign_in_ticks; }
  void set_sign_in_ticks_bits(uint32_t v) const { data()->sign_in_ticks = v; }
  // 1 from Create and SetUp, 0 from Interrupt: a signed-in Live user goes on to the battle or time attack page
  uint32_t sign_in_continues() const { return data()->sign_in_continues; }
  void set_sign_in_continues(uint32_t v) const { data()->sign_in_continues = v; }
  uint32_t sign_in_continues_bits() const { return data()->sign_in_continues; }
  void set_sign_in_continues_bits(uint32_t v) const { data()->sign_in_continues = v; }
};

// allocated; measure: new<108>@NetRanking::Create, sub_822822C0.r3, sub_822822C0.ret
struct NetRankingData : View {
  using View::View;
  static constexpr uint32_t kSize = 108;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // back to the title (Destroy goes to state 14)
  uint32_t back() const { return guest::LoadU32(base, address + 4); }
  void set_back(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t back_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // read by ModeType12::Interrupt (0: message 43 posted)
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // -1 at Create
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // -1 at Create
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
  // 1 at Create
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  // -1 at Create; the leaderboard's rank (Leaderboard::Rank's answer, probably)
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(base, address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  // the first row shown (sub_822822C0, probably)
  uint32_t first_row() const { return guest::LoadU32(base, address + 68); }
  void set_first_row(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t first_row_address() const { return address + 68; }
  // the row after the last shown: first + 7, kept within the rows (probably)
  uint32_t last_row() const { return guest::LoadU32(base, address + 72); }
  void set_last_row(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t last_row_address() const { return address + 72; }
  // the last row the window may reach (probably)
  uint32_t row_limit() const { return guest::LoadU32(base, address + 76); }
  void set_row_limit(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t row_limit_address() const { return address + 76; }
  // the rows read, -1 at Create (probably)
  uint32_t row_count() const { return guest::LoadU32(base, address + 80); }
  void set_row_count(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t row_count_address() const { return address + 80; }
  uint32_t unknown_84() const { return guest::LoadU32(base, address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  // 0 at Create; the same pointer as +96 and +100 (from [0x82816E48])
  uint32_t unknown_92() const { return guest::LoadU32(base, address + 92); }
  void set_unknown_92(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint32_t unknown_96() const { return guest::LoadU32(base, address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  uint32_t unknown_100() const { return guest::LoadU32(base, address + 100); }
  void set_unknown_100(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  // -1 at Create
  uint32_t unknown_104() const { return guest::LoadU32(base, address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
};

// embedded; measure: sub_82282150.r4
struct NetRankingTime : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t milliseconds() const { return guest::LoadU32(base, address + 0); }
  void set_milliseconds(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t milliseconds_address() const { return address + 0; }
  uint32_t seconds() const { return guest::LoadU32(base, address + 4); }
  void set_seconds(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t seconds_address() const { return address + 4; }
  uint32_t minutes() const { return guest::LoadU32(base, address + 8); }
  void set_minutes(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t minutes_address() const { return address + 8; }
};

// allocated; measure: Session, Leaderboard
struct Session : View {
  using View::View;
  static constexpr uint32_t kSize = 1297744;
  // a leaderboard read started (Kind6Menu::RankingPage)
  uint32_t read_started() const { return guest::LoadU32(base, address + 144); }
  void set_read_started(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t read_started_address() const { return address + 144; }
  uint32_t unknown_160() const { return guest::LoadU32(base, address + 160); }
  void set_unknown_160(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  // the player's XUID
  uint64_t xuid() const { return guest::LoadU64(base, address + 168); }
  void set_xuid(uint64_t v) const { guest::StoreU64(base, address + 168, v); }
  uint32_t xuid_address() const { return address + 168; }
  // the leaderboard read's state: 2 done, 3 a write, 4 and up an error
  uint32_t read_state() const { return guest::LoadU32(base, address + 180); }
  void set_read_state(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t read_state_address() const { return address + 180; }
  // the leaderboard's view id (1 from Init; Network::SetLiveState)
  uint32_t view_id() const { return guest::LoadU32(base, address + 188); }
  void set_view_id(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t view_id_address() const { return address + 188; }
  // 1 from Init
  uint32_t unknown_192() const { return guest::LoadU32(base, address + 192); }
  void set_unknown_192(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t unknown_192_address() const { return address + 192; }
  // the race's time in milliseconds (3,600,000 from Init)
  uint64_t race_time() const { return guest::LoadU64(base, address + 200); }
  void set_race_time(uint64_t v) const { guest::StoreU64(base, address + 200, v); }
  uint32_t race_time_address() const { return address + 200; }
  // the best time in milliseconds (3,600,000 from Init)
  uint64_t best_time() const { return guest::LoadU64(base, address + 208); }
  void set_best_time(uint64_t v) const { guest::StoreU64(base, address + 208, v); }
  uint32_t best_time_address() const { return address + 208; }
  // [0x82816694] (the time attack car's +12), from NextMode's time attack
  uint32_t ta_car_word() const { return guest::LoadU32(base, address + 216); }
  void set_ta_car_word(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t ta_car_word_address() const { return address + 216; }
  // -1 from Init
  uint32_t unknown_220() const { return guest::LoadU32(base, address + 220); }
  void set_unknown_220(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t unknown_220_address() const { return address + 220; }
  uint32_t unknown_224() const { return guest::LoadU32(base, address + 224); }
  void set_unknown_224(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  // the multiplayer privilege check's answer (NetModeSelect::TimeAttackPage)
  uint32_t privilege() const { return guest::LoadU32(base, address + 228); }
  void set_privilege(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t privilege_address() const { return address + 228; }
  uint32_t unknown_232() const { return guest::LoadU32(base, address + 232); }
  void set_unknown_232(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t unknown_232_address() const { return address + 232; }
  // the upload's state
  uint32_t upload_state() const { return guest::LoadU32(base, address + 236); }
  void set_upload_state(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t upload_state_address() const { return address + 236; }
  // set: Init opens screen 3 after a race
  uint32_t screen3_after_race() const { return guest::LoadU32(base, address + 240); }
  void set_screen3_after_race(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t screen3_after_race_address() const { return address + 240; }
  // set: Init opens the ranking after a race
  uint32_t ranking_after_race() const { return guest::LoadU32(base, address + 244); }
  void set_ranking_after_race(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t ranking_after_race_address() const { return address + 244; }
  // cleared by Init (XMemSet)
  uint32_t cleared_address() const { return address + 248; }
  // 1 from Init
  uint32_t unknown_1297712() const { return guest::LoadU32(base, address + 1297712); }
  void set_unknown_1297712(uint32_t v) const { guest::StoreU32(base, address + 1297712, v); }
  uint32_t unknown_1297712_address() const { return address + 1297712; }
  // +0x13CD34: a buffer Session::Release frees (operator delete)
  uint32_t rows() const { return guest::LoadU32(base, address + 1297716); }
  void set_rows(uint32_t v) const { guest::StoreU32(base, address + 1297716, v); }
  uint32_t rows_address() const { return address + 1297716; }
};

// embedded; measure: ModeType12.+816@+115888
struct NetModeBlockAt115888 : View {
  using View::View;
  static constexpr uint32_t kSize = 9648;
  uint32_t unknown_9644() const { return guest::LoadU32(base, address + 9644); }
  void set_unknown_9644(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t unknown_9644_address() const { return address + 9644; }
};

// embedded; measure: ModeType12.+816@+125536
struct NetModeBlockAt125536 : View {
  using View::View;
  static constexpr uint32_t kSize = 9644;
  uint32_t unknown_9640() const { return guest::LoadU32(base, address + 9640); }
  void set_unknown_9640(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t unknown_9640_address() const { return address + 9640; }
};

// embedded; measure: ModeType12.+816@+86880
struct NetModeBlockAt86880 : View {
  using View::View;
  static constexpr uint32_t kSize = 9644;
  uint32_t unknown_9640() const { return guest::LoadU32(base, address + 9640); }
  void set_unknown_9640(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t unknown_9640_address() const { return address + 9640; }
};

// The image words docs/functions/network-mode.md's code uses.
struct NetworkModeGlobals {
  // the game mode before (1 the race)
  static uint32_t mode_before(uint8_t* base) { return guest::LoadU32(base, 0x826032E0u); }
  static void set_mode_before(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032E0u, v); }
  static constexpr uint32_t mode_before_address() { return 0x826032E0u; }
  // the network session, 0 when none (0 in play)
  template <typename T = Session>
  static T session(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816E3Cu)); }
  static void set_session(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816E3Cu, v.address); }
  static void set_session(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816E3Cu, v); }
  static constexpr uint32_t session_address() { return 0x82816E3Cu; }
  // picks screen 7 (1) or 9 (0 after a race) at Init
  static uint32_t network_state(uint8_t* base) { return guest::LoadU32(base, 0x8281611Cu); }
  static void set_network_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8281611Cu, v); }
  static constexpr uint32_t network_state_address() { return 0x8281611Cu; }
  // the string table (Strings::Load's r3)
  static View string_table(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826C5C34u)); }
  static void set_string_table(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t string_table_address() { return 0x826C5C34u; }
  // the fades' table word FadeIn gets
  static uint32_t fades_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fades_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fades_passes_address() { return 0x82001BECu; }
  // the fades' table word FadeOut gets
  static uint32_t fades_out(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fades_out(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fades_out_address() { return 0x82001BF0u; }
  // a fade's count (0 done)
  static uint32_t fade_count0(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_count0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_count0_address() { return 0x826A50E4u; }
  // the other fade's count (0 done)
  static uint32_t fade_count1(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_count1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_count1_address() { return 0x826A50DCu; }
  // the sound manager
  template <typename T = AudioManager>
  static T sound_manager(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5C80u)); }
  static void set_sound_manager(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5C80u, v.address); }
  static void set_sound_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C80u, v); }
  static constexpr uint32_t sound_manager_address() { return 0x826C5C80u; }
  // the sign-in watch's "not on Live" check (0x826BAB40 + 16): 0 turns it on
  static uint32_t sign_in_live_check(uint8_t* base) { return guest::LoadU32(base, 0x826BAB50u); }
  static void set_sign_in_live_check(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB50u, v); }
  static constexpr uint32_t sign_in_live_check_address() { return 0x826BAB50u; }
  // set by NextMode when leaving for the title with the session made here
  static uint32_t title_flag(uint8_t* base) { return guest::LoadU32(base, 0x826364ECu); }
  static void set_title_flag(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826364ECu, v); }
  static constexpr uint32_t title_flag_address() { return 0x826364ECu; }
  // the leaderboard read's results (operator delete[] by Session::Release)
  static View leaderboard_results(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826C6AD4u)); }
  static void set_leaderboard_results(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6AD4u, v); }
  static constexpr uint32_t leaderboard_results_address() { return 0x826C6AD4u; }
  // cleared by Session::Init (Session::IsActive reads it)
  static uint32_t session_state(uint8_t* base) { return guest::LoadU32(base, 0x826C6AA8u); }
  static void set_session_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6AA8u, v); }
  static constexpr uint32_t session_state_address() { return 0x826C6AA8u; }
  // the race data
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the time attack's route
  static uint32_t ta_route(uint8_t* base) { return guest::LoadU32(base, 0x82816660u); }
  static void set_ta_route(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816660u, v); }
  static constexpr uint32_t ta_route_address() { return 0x82816660u; }
  // set: ta_kind picks the race data's +0x59618 word
  static uint32_t ta_has_kind(uint8_t* base) { return guest::LoadU32(base, 0x82816680u); }
  static void set_ta_has_kind(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816680u, v); }
  static constexpr uint32_t ta_has_kind_address() { return 0x82816680u; }
  static uint32_t ta_kind(uint8_t* base) { return guest::LoadU32(base, 0x82816684u); }
  static void set_ta_kind(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816684u, v); }
  static constexpr uint32_t ta_kind_address() { return 0x82816684u; }
  // the time attack's car, copied into the race's
  static constexpr uint32_t ta_car_address() { return 0x82816688u; }
  // ta_car's +12, into the session's +216
  static uint32_t ta_session_word(uint8_t* base) { return guest::LoadU32(base, 0x82816694u); }
  static void set_ta_session_word(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816694u, v); }
  static constexpr uint32_t ta_session_word_address() { return 0x82816694u; }
  // into the race car's +3892
  static uint32_t ta_car_word(uint8_t* base) { return guest::LoadU32(base, 0x82816E38u); }
  static void set_ta_car_word(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816E38u, v); }
  static constexpr uint32_t ta_car_word_address() { return 0x82816E38u; }
  // the mode select's page object
  template <typename T = PageObject>
  static T select_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816190u)); }
  static void set_select_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816190u, v.address); }
  static void set_select_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816190u, v); }
  static constexpr uint32_t select_pages_address() { return 0x82816190u; }
  // the mode select's menu (2 items)
  template <typename T = FrontEndMenu>
  static T select_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816194u)); }
  static void set_select_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816194u, v.address); }
  static void set_select_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816194u, v); }
  static constexpr uint32_t select_menu_address() { return 0x82816194u; }
  template <typename T = NetModeSelectData>
  static T select_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816198u)); }
  static void set_select_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816198u, v.address); }
  static void set_select_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816198u, v); }
  static constexpr uint32_t select_data_address() { return 0x82816198u; }
  template <typename T = PageObject>
  static T screen1_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8281616Cu)); }
  static void set_screen1_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8281616Cu, v.address); }
  static void set_screen1_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8281616Cu, v); }
  static constexpr uint32_t screen1_pages_address() { return 0x8281616Cu; }
  // 2 items
  template <typename T = FrontEndMenu>
  static T screen1_menu0(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816170u)); }
  static void set_screen1_menu0(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816170u, v.address); }
  static void set_screen1_menu0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816170u, v); }
  static constexpr uint32_t screen1_menu0_address() { return 0x82816170u; }
  // 2 items
  template <typename T = FrontEndMenu>
  static T screen1_menu1(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816174u)); }
  static void set_screen1_menu1(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816174u, v.address); }
  static void set_screen1_menu1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816174u, v); }
  static constexpr uint32_t screen1_menu1_address() { return 0x82816174u; }
  // 1 item
  template <typename T = FrontEndMenu>
  static T screen1_menu2(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816178u)); }
  static void set_screen1_menu2(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816178u, v.address); }
  static void set_screen1_menu2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816178u, v); }
  static constexpr uint32_t screen1_menu2_address() { return 0x82816178u; }
  // 1 item
  template <typename T = FrontEndMenu>
  static T screen1_menu3(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8281617Cu)); }
  static void set_screen1_menu3(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8281617Cu, v.address); }
  static void set_screen1_menu3(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8281617Cu, v); }
  static constexpr uint32_t screen1_menu3_address() { return 0x8281617Cu; }
  // screen 1's data (2,820 bytes: +4 back, +8 set by the mode select's pick)
  static View screen1_data(uint8_t* base) { return View(base, guest::LoadU32(base, 0x82816180u)); }
  static void set_screen1_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816180u, v); }
  static constexpr uint32_t screen1_data_address() { return 0x82816180u; }
  // the "Now Loading" print's counter
  static uint32_t screen1_loading_count(uint8_t* base) { return guest::LoadU32(base, 0x82816184u); }
  static void set_screen1_loading_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816184u, v); }
  static constexpr uint32_t screen1_loading_count_address() { return 0x82816184u; }
  template <typename T = PageObject>
  static T screen3_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160E0u)); }
  static void set_screen3_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160E0u, v.address); }
  static void set_screen3_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160E0u, v); }
  static constexpr uint32_t screen3_pages_address() { return 0x828160E0u; }
  // 3 items
  template <typename T = FrontEndMenu>
  static T screen3_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160E4u)); }
  static void set_screen3_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160E4u, v.address); }
  static void set_screen3_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160E4u, v); }
  static constexpr uint32_t screen3_menu_address() { return 0x828160E4u; }
  // 24 bytes: +4 back
  static View screen3_data(uint8_t* base) { return View(base, guest::LoadU32(base, 0x828160E8u)); }
  static void set_screen3_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160E8u, v); }
  static constexpr uint32_t screen3_data_address() { return 0x828160E8u; }
  static uint32_t screen3_loading_count(uint8_t* base) { return guest::LoadU32(base, 0x828160ECu); }
  static void set_screen3_loading_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160ECu, v); }
  static constexpr uint32_t screen3_loading_count_address() { return 0x828160ECu; }
  template <typename T = PageObject>
  static T screen8_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816144u)); }
  static void set_screen8_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816144u, v.address); }
  static void set_screen8_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816144u, v); }
  static constexpr uint32_t screen8_pages_address() { return 0x82816144u; }
  template <typename T = FrontEndMenu>
  static T screen8_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816148u)); }
  static void set_screen8_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816148u, v.address); }
  static void set_screen8_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816148u, v); }
  static constexpr uint32_t screen8_menu_address() { return 0x82816148u; }
  template <typename T = PageObject>
  static T screen9_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8281614Cu)); }
  static void set_screen9_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8281614Cu, v.address); }
  static void set_screen9_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8281614Cu, v); }
  static constexpr uint32_t screen9_pages_address() { return 0x8281614Cu; }
  template <typename T = FrontEndMenu>
  static T screen9_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816150u)); }
  static void set_screen9_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816150u, v.address); }
  static void set_screen9_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816150u, v); }
  static constexpr uint32_t screen9_menu_address() { return 0x82816150u; }
  template <typename T = PageObject>
  static T screen6_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816154u)); }
  static void set_screen6_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816154u, v.address); }
  static void set_screen6_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816154u, v); }
  static constexpr uint32_t screen6_pages_address() { return 0x82816154u; }
  // 2 items
  template <typename T = FrontEndMenu>
  static T screen6_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816158u)); }
  static void set_screen6_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816158u, v.address); }
  static void set_screen6_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816158u, v); }
  static constexpr uint32_t screen6_menu_address() { return 0x82816158u; }
  // 0 from Create
  static uint32_t screen6_word(uint8_t* base) { return guest::LoadU32(base, 0x8281615Cu); }
  static void set_screen6_word(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8281615Cu, v); }
  static constexpr uint32_t screen6_word_address() { return 0x8281615Cu; }
  template <typename T = PageObject>
  static T screen7_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816160u)); }
  static void set_screen7_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816160u, v.address); }
  static void set_screen7_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816160u, v); }
  static constexpr uint32_t screen7_pages_address() { return 0x82816160u; }
  // 3 items
  template <typename T = FrontEndMenu>
  static T screen7_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816164u)); }
  static void set_screen7_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816164u, v.address); }
  static void set_screen7_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816164u, v); }
  static constexpr uint32_t screen7_menu_address() { return 0x82816164u; }
  // 8 bytes: +0, +4 0 from Create
  template <typename T = NetScreen7Data>
  static T screen7_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816168u)); }
  static void set_screen7_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816168u, v.address); }
  static void set_screen7_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816168u, v); }
  static constexpr uint32_t screen7_data_address() { return 0x82816168u; }
  template <typename T = PageObject>
  static T dialog11_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82816188u)); }
  static void set_dialog11_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82816188u, v.address); }
  static void set_dialog11_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816188u, v); }
  static constexpr uint32_t dialog11_pages_address() { return 0x82816188u; }
  template <typename T = FrontEndMenu>
  static T dialog11_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8281618Cu)); }
  static void set_dialog11_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8281618Cu, v.address); }
  static void set_dialog11_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8281618Cu, v); }
  static constexpr uint32_t dialog11_menu_address() { return 0x8281618Cu; }
  template <typename T = PageObject>
  static T dialog13_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8281619Cu)); }
  static void set_dialog13_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8281619Cu, v.address); }
  static void set_dialog13_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8281619Cu, v); }
  static constexpr uint32_t dialog13_pages_address() { return 0x8281619Cu; }
  template <typename T = FrontEndMenu>
  static T dialog13_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161A0u)); }
  static void set_dialog13_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161A0u, v.address); }
  static void set_dialog13_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161A0u, v); }
  static constexpr uint32_t dialog13_menu_address() { return 0x828161A0u; }
  // the message dialog 13 shows (7: no Live user)
  static uint32_t dialog13_reason(uint8_t* base) { return guest::LoadU32(base, 0x828161A4u); }
  static void set_dialog13_reason(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161A4u, v); }
  static constexpr uint32_t dialog13_reason_address() { return 0x828161A4u; }
  template <typename T = PageObject>
  static T dialog12_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161A8u)); }
  static void set_dialog12_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161A8u, v.address); }
  static void set_dialog12_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161A8u, v); }
  static constexpr uint32_t dialog12_pages_address() { return 0x828161A8u; }
  template <typename T = FrontEndMenu>
  static T dialog12_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161ACu)); }
  static void set_dialog12_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161ACu, v.address); }
  static void set_dialog12_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161ACu, v); }
  static constexpr uint32_t dialog12_menu_address() { return 0x828161ACu; }
  // the message dialog 12 shows (10: the privilege check failed)
  static uint32_t dialog12_reason(uint8_t* base) { return guest::LoadU32(base, 0x828161B0u); }
  static void set_dialog12_reason(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161B0u, v); }
  static constexpr uint32_t dialog12_reason_address() { return 0x828161B0u; }
  template <typename T = PageObject>
  static T ta_course_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161B4u)); }
  static void set_ta_course_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161B4u, v.address); }
  static void set_ta_course_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161B4u, v); }
  static constexpr uint32_t ta_course_pages_address() { return 0x828161B4u; }
  // 7 items
  template <typename T = FrontEndMenu>
  static T ta_course_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161B8u)); }
  static void set_ta_course_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161B8u, v.address); }
  static void set_ta_course_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161B8u, v); }
  static constexpr uint32_t ta_course_menu_address() { return 0x828161B8u; }
  // 48 bytes: +4 back
  static View ta_course_data(uint8_t* base) { return View(base, guest::LoadU32(base, 0x828161BCu)); }
  static void set_ta_course_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161BCu, v); }
  static constexpr uint32_t ta_course_data_address() { return 0x828161BCu; }
  static uint32_t ta_course_loading_count(uint8_t* base) { return guest::LoadU32(base, 0x828161C0u); }
  static void set_ta_course_loading_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161C0u, v); }
  static constexpr uint32_t ta_course_loading_count_address() { return 0x828161C0u; }
  template <typename T = PageObject>
  static T ranking_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161C4u)); }
  static void set_ranking_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161C4u, v.address); }
  static void set_ranking_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161C4u, v); }
  static constexpr uint32_t ranking_pages_address() { return 0x828161C4u; }
  // 7 items
  template <typename T = FrontEndMenu>
  static T ranking_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161C8u)); }
  static void set_ranking_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161C8u, v.address); }
  static void set_ranking_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161C8u, v); }
  static constexpr uint32_t ranking_menu_address() { return 0x828161C8u; }
  // 1 item
  template <typename T = FrontEndMenu>
  static T ranking_menu2(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161CCu)); }
  static void set_ranking_menu2(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161CCu, v.address); }
  static void set_ranking_menu2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161CCu, v); }
  static constexpr uint32_t ranking_menu2_address() { return 0x828161CCu; }
  template <typename T = NetRankingData>
  static T ranking_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161D0u)); }
  static void set_ranking_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161D0u, v.address); }
  static void set_ranking_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161D0u, v); }
  static constexpr uint32_t ranking_data_address() { return 0x828161D0u; }
  template <typename T = PageObject>
  static T dialog10_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161D4u)); }
  static void set_dialog10_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161D4u, v.address); }
  static void set_dialog10_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161D4u, v); }
  static constexpr uint32_t dialog10_pages_address() { return 0x828161D4u; }
  template <typename T = FrontEndMenu>
  static T dialog10_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161D8u)); }
  static void set_dialog10_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161D8u, v.address); }
  static void set_dialog10_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161D8u, v); }
  static constexpr uint32_t dialog10_menu_address() { return 0x828161D8u; }
  // 0 from Create
  static uint32_t dialog10_word(uint8_t* base) { return guest::LoadU32(base, 0x828161DCu); }
  static void set_dialog10_word(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161DCu, v); }
  static constexpr uint32_t dialog10_word_address() { return 0x828161DCu; }
  // the Live users' mask (0 from B5's Network::Update stub)
  static uint32_t live_mask(uint8_t* base) { return guest::LoadU32(base, 0x826C795Cu); }
  static void set_live_mask(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C795Cu, v); }
  static constexpr uint32_t live_mask_address() { return 0x826C795Cu; }
  // player 1's controller (0 to 3)
  static uint32_t player_controller(uint8_t* base) { return guest::LoadU32(base, 0x826BABA4u); }
  static void set_player_controller(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABA4u, v); }
  static constexpr uint32_t player_controller_address() { return 0x826BABA4u; }
  // the system UI's words
  static uint32_t system_ui0(uint8_t* base) { return guest::LoadU32(base, 0x826C7960u); }
  static void set_system_ui0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7960u, v); }
  static constexpr uint32_t system_ui0_address() { return 0x826C7960u; }
  static uint32_t system_ui1(uint8_t* base) { return guest::LoadU32(base, 0x826C7964u); }
  static void set_system_ui1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7964u, v); }
  static constexpr uint32_t system_ui1_address() { return 0x826C7964u; }
};

}  // namespace schema
