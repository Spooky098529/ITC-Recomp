// The views of docs/functions/power-graph-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/power-graph-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<92>@PowerGraphScreen67::OnEnter
#pragma pack(push, 1)
struct PowerGraphScreenDataData {
  uint32_t movie_loaded = 0;  // +0
  uint32_t queued_state = 0;  // +4
  uint32_t car_value = 0;  // +8
  uint32_t rating = 0;  // +12
  uint32_t spec_word_0 = 0;  // +16
  uint32_t spec_word_1 = 0;  // +20
  uint32_t spec_word_2 = 0;  // +24
  uint32_t car_id = 0;  // +28
  uint32_t spec_byte_0 = 0;  // +32
  uint32_t spec_byte_1 = 0;  // +36
  uint32_t engine_class = 0;  // +40
  uint32_t spec_flag = 0;  // +44
  uint32_t spec_word_3 = 0;  // +48
  uint32_t curve_value_0 = 0;  // +52
  uint32_t curve_value_1 = 0;  // +56
  float peak_torque = 0;  // +60
  uint32_t peak_index = 0;  // +64
  uint32_t parts_word_0 = 0;  // +68
  uint32_t parts_word_1 = 0;  // +72
  uint32_t parts_word_2 = 0;  // +76
  uint32_t parts_word_3 = 0;  // +80
  uint32_t parts_word_4 = 0;  // +84
  uint32_t tab = 0;  // +88
};
#pragma pack(pop)
static_assert(sizeof(PowerGraphScreenDataData) == 92);
inline constexpr memory::LayoutField kPowerGraphScreenDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}};
inline constexpr memory::Layout kPowerGraphScreenDataLayout = {kPowerGraphScreenDataFields, 23};

struct PowerGraphScreenData : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  PowerGraphScreenDataData* data() const { return static_cast<PowerGraphScreenDataData*>(memory::HandleObject(address)); }
  static PowerGraphScreenData New(uint8_t* base) {
    return PowerGraphScreenData(base, memory::MakeHandle(new PowerGraphScreenDataData(), sizeof(PowerGraphScreenDataData), "PowerGraphScreenData", &kPowerGraphScreenDataLayout));
  }
  template <uint32_t Stride = kSize>
  static PowerGraphScreenData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(PowerGraphScreenDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PowerGraphScreenDataData*>(::operator new(sizeof(PowerGraphScreenDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PowerGraphScreenDataData();
    return PowerGraphScreenData(base, memory::MakeHandle(p, sizeof(PowerGraphScreenDataData) * count, "PowerGraphScreenData", &kPowerGraphScreenDataLayout, sizeof(PowerGraphScreenDataData)));
  }
  PowerGraphScreenData at(uint32_t i) const { return PowerGraphScreenData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kPowerGraphScreenDataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kPowerGraphScreenDataLayout, offset, width, v); }
  // 1 once movie 9 is loaded (WaitForLoad)
  uint32_t movie_loaded_address() const { return address + 0; }
  uint32_t movie_loaded() const { return data()->movie_loaded; }
  void set_movie_loaded(uint32_t v) const { data()->movie_loaded = v; }
  uint32_t movie_loaded_bits() const { return data()->movie_loaded; }
  void set_movie_loaded_bits(uint32_t v) const { data()->movie_loaded = v; }
  // measure fault: SetParts' r4 is the data's +68 (parts), joined with the block; its byte +1 (the part icon), not the data's +1
  uint32_t part_icon_address() const { return address + 1; }
  uint8_t part_icon_bits() const { return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kPowerGraphScreenDataLayout, 1, 1)); }
  void set_part_icon_bits(uint8_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kPowerGraphScreenDataLayout, 1, 1, v); }
  uint8_t part_icon() const { return uint8_t(part_icon_bits()); }
  void set_part_icon(uint8_t v) const { set_part_icon_bits(uint8_t(v)); }
  // the machine's state Update applies; 21 none, 9 from ClosedPage
  uint32_t queued_state_address() const { return address + 4; }
  uint32_t queued_state() const { return data()->queued_state; }
  void set_queued_state(uint32_t v) const { data()->queued_state = v; }
  uint32_t queued_state_bits() const { return data()->queued_state; }
  void set_queued_state_bits(uint32_t v) const { data()->queued_state = v; }
  // the record (+8 to +92; its words below) +0: 0 (SetEntry) or int(the car's +1892 float) (SetFromCar); SetTexts' node 10
  uint32_t car_value_address() const { return address + 8; }
  uint32_t car_value() const { return data()->car_value; }
  void set_car_value(uint32_t v) const { data()->car_value = v; }
  uint32_t car_value_bits() const { return data()->car_value; }
  void set_car_value_bits(uint32_t v) const { data()->car_value = v; }
  // record +4: the spec's +712 (SetEntry) or a value from the car data (SetFromCar); node 11
  uint32_t rating_address() const { return address + 12; }
  uint32_t rating() const { return data()->rating; }
  void set_rating(uint32_t v) const { data()->rating = v; }
  uint32_t rating_bits() const { return data()->rating; }
  void set_rating_bits(uint32_t v) const { data()->rating = v; }
  // record +8: the spec's +716 (s +76); node 12
  uint32_t spec_word_0_address() const { return address + 16; }
  uint32_t spec_word_0() const { return data()->spec_word_0; }
  void set_spec_word_0(uint32_t v) const { data()->spec_word_0 = v; }
  uint32_t spec_word_0_bits() const { return data()->spec_word_0; }
  void set_spec_word_0_bits(uint32_t v) const { data()->spec_word_0 = v; }
  // record +12: the spec's +724 (s +84); node 13
  uint32_t spec_word_1_address() const { return address + 20; }
  uint32_t spec_word_1() const { return data()->spec_word_1; }
  void set_spec_word_1(uint32_t v) const { data()->spec_word_1 = v; }
  uint32_t spec_word_1_bits() const { return data()->spec_word_1; }
  void set_spec_word_1_bits(uint32_t v) const { data()->spec_word_1 = v; }
  // record +16: the spec's +720 (s +80); node 14
  uint32_t spec_word_2_address() const { return address + 24; }
  uint32_t spec_word_2() const { return data()->spec_word_2; }
  void set_spec_word_2(uint32_t v) const { data()->spec_word_2 = v; }
  uint32_t spec_word_2_bits() const { return data()->spec_word_2; }
  void set_spec_word_2_bits(uint32_t v) const { data()->spec_word_2 = v; }
  // record +20: the car id; strings 1646, 1790, 1718 + it
  uint32_t car_id_address() const { return address + 28; }
  uint32_t car_id() const { return data()->car_id; }
  void set_car_id(uint32_t v) const { data()->car_id = v; }
  uint32_t car_id_bits() const { return data()->car_id; }
  void set_car_id_bits(uint32_t v) const { data()->car_id = v; }
  // record +24: a spec byte (+769, s +129); string 1934 + it
  uint32_t spec_byte_0_address() const { return address + 32; }
  uint32_t spec_byte_0() const { return data()->spec_byte_0; }
  void set_spec_byte_0(uint32_t v) const { data()->spec_byte_0 = v; }
  uint32_t spec_byte_0_bits() const { return data()->spec_byte_0; }
  void set_spec_byte_0_bits(uint32_t v) const { data()->spec_byte_0 = v; }
  // record +28: a spec byte (+770, s +130); string 421 + it
  uint32_t spec_byte_1_address() const { return address + 36; }
  uint32_t spec_byte_1() const { return data()->spec_byte_1; }
  void set_spec_byte_1(uint32_t v) const { data()->spec_byte_1 = v; }
  uint32_t spec_byte_1_bits() const { return data()->spec_byte_1; }
  void set_spec_byte_1_bits(uint32_t v) const { data()->spec_byte_1 = v; }
  // record +32: a class 0 to 6 from the spec byte +708 (s +68); string 428 + it
  uint32_t engine_class_address() const { return address + 40; }
  uint32_t engine_class() const { return data()->engine_class; }
  void set_engine_class(uint32_t v) const { data()->engine_class = v; }
  uint32_t engine_class_bits() const { return data()->engine_class; }
  void set_engine_class_bits(uint32_t v) const { data()->engine_class = v; }
  // record +36: the spec byte +709 (s +69) set, 1 or 0; string 426 + it
  uint32_t spec_flag_address() const { return address + 44; }
  uint32_t spec_flag() const { return data()->spec_flag; }
  void set_spec_flag(uint32_t v) const { data()->spec_flag = v; }
  uint32_t spec_flag_bits() const { return data()->spec_flag; }
  void set_spec_flag_bits(uint32_t v) const { data()->spec_flag = v; }
  // record +40: the spec's +648 (s +8); node 39
  uint32_t spec_word_3_address() const { return address + 48; }
  uint32_t spec_word_3() const { return data()->spec_word_3; }
  void set_spec_word_3(uint32_t v) const { data()->spec_word_3 = v; }
  uint32_t spec_word_3_bits() const { return data()->spec_word_3; }
  void set_spec_word_3_bits(uint32_t v) const { data()->spec_word_3 = v; }
  // record +44: int(the power curve's +1056) (+652); node 33
  uint32_t curve_value_0_address() const { return address + 52; }
  uint32_t curve_value_0() const { return data()->curve_value_0; }
  void set_curve_value_0(uint32_t v) const { data()->curve_value_0 = v; }
  uint32_t curve_value_0_bits() const { return data()->curve_value_0; }
  void set_curve_value_0_bits(uint32_t v) const { data()->curve_value_0 = v; }
  // record +48: int(the curve's +1060) (+656); node 40
  uint32_t curve_value_1_address() const { return address + 56; }
  uint32_t curve_value_1() const { return data()->curve_value_1; }
  void set_curve_value_1(uint32_t v) const { data()->curve_value_1 = v; }
  uint32_t curve_value_1_bits() const { return data()->curve_value_1; }
  void set_curve_value_1_bits(uint32_t v) const { data()->curve_value_1 = v; }
  // record +52: the peak torque in kgf·m (probably); node 34
  uint32_t peak_torque_address() const { return address + 60; }
  float peak_torque() const { return data()->peak_torque; }
  void set_peak_torque(float v) const { data()->peak_torque = v; }
  uint32_t peak_torque_bits() const { return std::bit_cast<uint32_t>(data()->peak_torque); }
  void set_peak_torque_bits(uint32_t v) const { data()->peak_torque = std::bit_cast<float>(v); }
  // record +56: int(the curve's +1052), the peak's index; node 41
  uint32_t peak_index_address() const { return address + 64; }
  uint32_t peak_index() const { return data()->peak_index; }
  void set_peak_index(uint32_t v) const { data()->peak_index = v; }
  uint32_t peak_index_bits() const { return data()->peak_index; }
  void set_peak_index_bits(uint32_t v) const { data()->peak_index = v; }
  // record +60 to +76 (the parts): five words from the car's +1568 (SetFromCar only); SetParts reads their bytes +1 to +11 in guest order (each word's most significant byte first)
  uint32_t parts_word_0_address() const { return address + 68; }
  uint32_t parts_word_0() const { return data()->parts_word_0; }
  void set_parts_word_0(uint32_t v) const { data()->parts_word_0 = v; }
  uint32_t parts_word_0_bits() const { return data()->parts_word_0; }
  void set_parts_word_0_bits(uint32_t v) const { data()->parts_word_0 = v; }
  uint32_t parts_word_1_address() const { return address + 72; }
  uint32_t parts_word_1() const { return data()->parts_word_1; }
  void set_parts_word_1(uint32_t v) const { data()->parts_word_1 = v; }
  uint32_t parts_word_1_bits() const { return data()->parts_word_1; }
  void set_parts_word_1_bits(uint32_t v) const { data()->parts_word_1 = v; }
  uint32_t parts_word_2_address() const { return address + 76; }
  uint32_t parts_word_2() const { return data()->parts_word_2; }
  void set_parts_word_2(uint32_t v) const { data()->parts_word_2 = v; }
  uint32_t parts_word_2_bits() const { return data()->parts_word_2; }
  void set_parts_word_2_bits(uint32_t v) const { data()->parts_word_2 = v; }
  uint32_t parts_word_3_address() const { return address + 80; }
  uint32_t parts_word_3() const { return data()->parts_word_3; }
  void set_parts_word_3(uint32_t v) const { data()->parts_word_3 = v; }
  uint32_t parts_word_3_bits() const { return data()->parts_word_3; }
  void set_parts_word_3_bits(uint32_t v) const { data()->parts_word_3 = v; }
  uint32_t parts_word_4_address() const { return address + 84; }
  uint32_t parts_word_4() const { return data()->parts_word_4; }
  void set_parts_word_4(uint32_t v) const { data()->parts_word_4 = v; }
  uint32_t parts_word_4_bits() const { return data()->parts_word_4; }
  void set_parts_word_4_bits(uint32_t v) const { data()->parts_word_4 = v; }
  // record +80: the tab word the caller passes (r4); node 3 shown for 2 and 5
  uint32_t tab_address() const { return address + 88; }
  uint32_t tab() const { return data()->tab; }
  void set_tab(uint32_t v) const { data()->tab = v; }
  uint32_t tab_bits() const { return data()->tab; }
  void set_tab_bits(uint32_t v) const { data()->tab = v; }
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
