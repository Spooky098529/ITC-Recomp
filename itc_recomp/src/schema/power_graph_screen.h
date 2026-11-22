// The views of docs/functions/power-graph-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/power-graph-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<92>@PowerGraphScreen67::OnEnter
struct PowerGraphScreenData : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  // 1 once movie 9 is loaded (WaitForLoad)
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 0); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t movie_loaded_address() const { return address + 0; }
  // measure fault: SetParts' r4 is the data's +68 (parts), joined with the block; its byte +1 (the part icon), not the data's +1
  uint8_t part_icon() const { return guest::LoadU8(base, address + 1); }
  void set_part_icon(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t part_icon_address() const { return address + 1; }
  // the machine's state Update applies; 21 none, 9 from ClosedPage
  uint32_t queued_state() const { return guest::LoadU32(base, address + 4); }
  void set_queued_state(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t queued_state_address() const { return address + 4; }
  // the record (+8 to +92)
  uint32_t record_address() const { return address + 8; }
  // record +0: 0 (SetEntry) or int(the car's +1892 float) (SetFromCar); SetTexts' node 10
  uint32_t car_value() const { return guest::LoadU32(base, address + 8); }
  void set_car_value(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t car_value_address() const { return address + 8; }
  // record +4: the spec's +712 (SetEntry) or a value from the car data (SetFromCar); node 11
  uint32_t rating() const { return guest::LoadU32(base, address + 12); }
  void set_rating(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t rating_address() const { return address + 12; }
  // record +8: the spec's +716 (s +76); node 12
  uint32_t spec_word_0() const { return guest::LoadU32(base, address + 16); }
  void set_spec_word_0(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t spec_word_0_address() const { return address + 16; }
  // record +12: the spec's +724 (s +84); node 13
  uint32_t spec_word_1() const { return guest::LoadU32(base, address + 20); }
  void set_spec_word_1(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t spec_word_1_address() const { return address + 20; }
  // record +16: the spec's +720 (s +80); node 14
  uint32_t spec_word_2() const { return guest::LoadU32(base, address + 24); }
  void set_spec_word_2(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t spec_word_2_address() const { return address + 24; }
  // record +20: the car id; strings 1646, 1790, 1718 + it
  uint32_t car_id() const { return guest::LoadU32(base, address + 28); }
  void set_car_id(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t car_id_address() const { return address + 28; }
  // record +24: a spec byte (+769, s +129); string 1934 + it
  uint32_t spec_byte_0() const { return guest::LoadU32(base, address + 32); }
  void set_spec_byte_0(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t spec_byte_0_address() const { return address + 32; }
  // record +28: a spec byte (+770, s +130); string 421 + it
  uint32_t spec_byte_1() const { return guest::LoadU32(base, address + 36); }
  void set_spec_byte_1(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t spec_byte_1_address() const { return address + 36; }
  // record +32: a class 0 to 6 from the spec byte +708 (s +68); string 428 + it
  uint32_t engine_class() const { return guest::LoadU32(base, address + 40); }
  void set_engine_class(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t engine_class_address() const { return address + 40; }
  // record +36: the spec byte +709 (s +69) set, 1 or 0; string 426 + it
  uint32_t spec_flag() const { return guest::LoadU32(base, address + 44); }
  void set_spec_flag(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t spec_flag_address() const { return address + 44; }
  // record +40: the spec's +648 (s +8); node 39
  uint32_t spec_word_3() const { return guest::LoadU32(base, address + 48); }
  void set_spec_word_3(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t spec_word_3_address() const { return address + 48; }
  // record +44: int(the power curve's +1056) (+652); node 33
  uint32_t curve_value_0() const { return guest::LoadU32(base, address + 52); }
  void set_curve_value_0(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t curve_value_0_address() const { return address + 52; }
  // record +48: int(the curve's +1060) (+656); node 40
  uint32_t curve_value_1() const { return guest::LoadU32(base, address + 56); }
  void set_curve_value_1(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t curve_value_1_address() const { return address + 56; }
  // record +52: the peak torque in kgf·m (probably); node 34
  float peak_torque() const { return guest::LoadFloat(base, address + 60); }
  void set_peak_torque(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t peak_torque_address() const { return address + 60; }
  // record +56: int(the curve's +1052), the peak's index; node 41
  uint32_t peak_index() const { return guest::LoadU32(base, address + 64); }
  void set_peak_index(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t peak_index_address() const { return address + 64; }
  // record +60 to +76: five words from the car's +1568 (SetFromCar only); SetParts reads its bytes +1 to +11
  uint32_t parts_address() const { return address + 68; }
  uint32_t parts_word_0() const { return guest::LoadU32(base, address + 68); }
  void set_parts_word_0(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t parts_word_0_address() const { return address + 68; }
  uint32_t parts_word_1() const { return guest::LoadU32(base, address + 72); }
  void set_parts_word_1(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t parts_word_1_address() const { return address + 72; }
  uint32_t parts_word_2() const { return guest::LoadU32(base, address + 76); }
  void set_parts_word_2(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t parts_word_2_address() const { return address + 76; }
  uint32_t parts_word_3() const { return guest::LoadU32(base, address + 80); }
  void set_parts_word_3(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t parts_word_3_address() const { return address + 80; }
  uint32_t parts_word_4() const { return guest::LoadU32(base, address + 84); }
  void set_parts_word_4(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t parts_word_4_address() const { return address + 84; }
  // record +80: the tab word the caller passes (r4); node 3 shown for 2 and 5
  uint32_t tab() const { return guest::LoadU32(base, address + 88); }
  void set_tab(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t tab_address() const { return address + 88; }
};

// embedded; measure: PowerGraphScreen67
struct PowerGraphScreen67 : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 0x82010A68 (type 6), 0x82010FD0 (type 7)
  template <typename T = PowerGraphScreen67Vtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the machine (StateMachine)
  uint32_t machine_address() const { return address + 4; }
  // the machine's state (its +9604)
  uint32_t machine_state() const { return guest::LoadU32(base, address + 9608); }
  void set_machine_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t machine_state_address() const { return address + 9608; }
  // the machine's loaded word (its +9608)
  uint32_t machine_loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_machine_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t machine_loaded_address() const { return address + 9612; }
  // the mode's word the entry reads (the machine's +9612): set, state 1, else 9
  uint32_t machine_reenter() const { return guest::LoadU32(base, address + 9616); }
  void set_machine_reenter(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t machine_reenter_address() const { return address + 9616; }
};

// image; measure: PowerGraphScreen67.+0
struct PowerGraphScreen67Vtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t slot0() const { return guest::LoadU32(base, address + 0); }
  void set_slot0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t slot0_address() const { return address + 0; }
  // slot 1: message 4 (type 7: 3), Leave
  uint32_t post_leave() const { return guest::LoadU32(base, address + 4); }
  void set_post_leave(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t post_leave_address() const { return address + 4; }
  // slot 2: message 23 (20), from MenuPage
  uint32_t post_closing() const { return guest::LoadU32(base, address + 8); }
  void set_post_closing(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t post_closing_address() const { return address + 8; }
  // slot 3: message 24 (21), from ClosedPage
  uint32_t post_closed() const { return guest::LoadU32(base, address + 12); }
  void set_post_closed(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t post_closed_address() const { return address + 12; }
};

// embedded; measure: PowerGraphScreen67::Step0.r4, PowerGraphScreen67::Step1.r4
struct PowerGraphStepStarted : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 on a step's first call
  uint32_t started() const { return guest::LoadU32(base, address + 0); }
  void set_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t started_address() const { return address + 0; }
};

// embedded; measure: PowerGraphScreen67::SetPartRow1.r4, PowerGraphScreen67::SetPartRow2.r4
struct PowerGraphPartValues : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // indexed: 10 hides a row 2 node, 37 and above a row 1 node
  int32_t value() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_value(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: new<92>@PowerGraphScreen67::OnEnter@+68
struct PowerGraphScreenDataAt68 : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
};

// The image words docs/functions/power-graph-screen.md's code uses.
struct PowerGraphScreenGlobals {
  // the page object, VirtualAlloc'ed by OnEnter
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82815EB4u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82815EB4u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815EB4u, v); }
  static constexpr uint32_t pages_address() { return 0x82815EB4u; }
  // a front-end menu, +16 = 7
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82815EB8u)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82815EB8u, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815EB8u, v); }
  static constexpr uint32_t menu_address() { return 0x82815EB8u; }
  template <typename T = PowerGraphScreenData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82815EBCu)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82815EBCu, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815EBCu, v); }
  static constexpr uint32_t data_address() { return 0x82815EBCu; }
  // the graph drawn while set (Step0's end); 0 from OnEnter, Open, Shown and Step1
  static uint32_t shown(uint8_t* base) { return guest::LoadU32(base, 0x82815EC0u); }
  static void set_shown(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815EC0u, v); }
  static constexpr uint32_t shown_address() { return 0x82815EC0u; }
};

}  // namespace schema
