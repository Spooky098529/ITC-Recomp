// The views of docs/functions/action-mappers.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/action-mappers.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ActionMapper, ActionMapper.+24, ActionMapper.+180
struct ActionMapper : View {
  using View::View;
  static constexpr uint32_t kSize = 2896;
  static constexpr uint32_t kVtable = 0x82007F78;
  // 0x82007F78
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the client node's type: 2
  uint32_t client_type() const { return guest::LoadU32(base, address + 4); }
  void set_client_type(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t client_type_address() const { return address + 4; }
  // the next node of the client list
  template <typename T = InputClientNode>
  T next_client() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_next_client(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_next_client(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_client_address() const { return address + 8; }
  // 30 words: each action's kind (1, digital)
  uint32_t kinds() const { return guest::LoadU32(base, address + 12); }
  void set_kinds(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t kinds_address() const { return address + 12; }
  // 30 words: each action's analog kind (2 for actions 24-29, else 1)
  uint32_t analog_kinds() const { return guest::LoadU32(base, address + 132); }
  void set_analog_kinds(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t analog_kinds_address() const { return address + 132; }
  // 30 words: each action's linked action (probably each direction's opposite; 30 none)
  uint32_t links() const { return guest::LoadU32(base, address + 252); }
  void set_links(uint32_t v) const { guest::StoreU32(base, address + 252, v); }
  uint32_t links_address() const { return address + 252; }
  // an ActionMapperConfig (the kinds and links pointers), then at +380 the analog stage's
  uint32_t config() const { return guest::LoadU32(base, address + 372); }
  void set_config(uint32_t v) const { guest::StoreU32(base, address + 372, v); }
  uint32_t config_address() const { return address + 372; }
  // the config's links pointer (mapper + 252)
  uint32_t config_links() const { return guest::LoadU32(base, address + 376); }
  void set_config_links(uint32_t v) const { guest::StoreU32(base, address + 376, v); }
  uint32_t config_links_address() const { return address + 376; }
  // the analog stage's ActionMapperConfig: its kinds pointer (mapper + 132)
  uint32_t analog_config() const { return guest::LoadU32(base, address + 380); }
  void set_analog_config(uint32_t v) const { guest::StoreU32(base, address + 380, v); }
  uint32_t analog_config_address() const { return address + 380; }
  // its links pointer (mapper + 252)
  uint32_t analog_config_links() const { return guest::LoadU32(base, address + 384); }
  void set_analog_config_links(uint32_t v) const { guest::StoreU32(base, address + 384, v); }
  uint32_t analog_config_links_address() const { return address + 384; }
  // 30 u16s: this tick's raw value per action; [0] Back (151)
  uint16_t raw() const { return guest::LoadU16(base, address + 388); }
  void set_raw(uint16_t v) const { guest::StoreU16(base, address + 388, v); }
  uint32_t raw_address() const { return address + 388; }
  // action 1 (152)
  uint16_t raw_start() const { return guest::LoadU16(base, address + 390); }
  void set_raw_start(uint16_t v) const { guest::StoreU16(base, address + 390, v); }
  uint32_t raw_start_address() const { return address + 390; }
  // action 2 (153)
  uint16_t raw_dpad_up() const { return guest::LoadU16(base, address + 392); }
  void set_raw_dpad_up(uint16_t v) const { guest::StoreU16(base, address + 392, v); }
  uint32_t raw_dpad_up_address() const { return address + 392; }
  // action 3 (154)
  uint16_t raw_dpad_down() const { return guest::LoadU16(base, address + 394); }
  void set_raw_dpad_down(uint16_t v) const { guest::StoreU16(base, address + 394, v); }
  uint32_t raw_dpad_down_address() const { return address + 394; }
  // action 4 (155)
  uint16_t raw_dpad_left() const { return guest::LoadU16(base, address + 396); }
  void set_raw_dpad_left(uint16_t v) const { guest::StoreU16(base, address + 396, v); }
  uint32_t raw_dpad_left_address() const { return address + 396; }
  // action 5 (156)
  uint16_t raw_dpad_right() const { return guest::LoadU16(base, address + 398); }
  void set_raw_dpad_right(uint16_t v) const { guest::StoreU16(base, address + 398, v); }
  uint32_t raw_dpad_right_address() const { return address + 398; }
  // action 6 (157)
  uint16_t raw_y() const { return guest::LoadU16(base, address + 400); }
  void set_raw_y(uint16_t v) const { guest::StoreU16(base, address + 400, v); }
  uint32_t raw_y_address() const { return address + 400; }
  // action 7 (158)
  uint16_t raw_b() const { return guest::LoadU16(base, address + 402); }
  void set_raw_b(uint16_t v) const { guest::StoreU16(base, address + 402, v); }
  uint32_t raw_b_address() const { return address + 402; }
  // action 8 (159)
  uint16_t raw_a() const { return guest::LoadU16(base, address + 404); }
  void set_raw_a(uint16_t v) const { guest::StoreU16(base, address + 404, v); }
  uint32_t raw_a_address() const { return address + 404; }
  // action 9 (160)
  uint16_t raw_x() const { return guest::LoadU16(base, address + 406); }
  void set_raw_x(uint16_t v) const { guest::StoreU16(base, address + 406, v); }
  uint32_t raw_x_address() const { return address + 406; }
  // action 10 (161)
  uint16_t raw_left_shoulder() const { return guest::LoadU16(base, address + 408); }
  void set_raw_left_shoulder(uint16_t v) const { guest::StoreU16(base, address + 408, v); }
  uint32_t raw_left_shoulder_address() const { return address + 408; }
  // action 11 (162)
  uint16_t raw_right_shoulder() const { return guest::LoadU16(base, address + 410); }
  void set_raw_right_shoulder(uint16_t v) const { guest::StoreU16(base, address + 410, v); }
  uint32_t raw_right_shoulder_address() const { return address + 410; }
  // action 12
  uint16_t raw_left_trigger() const { return guest::LoadU16(base, address + 412); }
  void set_raw_left_trigger(uint16_t v) const { guest::StoreU16(base, address + 412, v); }
  uint32_t raw_left_trigger_address() const { return address + 412; }
  // action 13
  uint16_t raw_right_trigger() const { return guest::LoadU16(base, address + 414); }
  void set_raw_right_trigger(uint16_t v) const { guest::StoreU16(base, address + 414, v); }
  uint32_t raw_right_trigger_address() const { return address + 414; }
  // action 14 (165)
  uint16_t raw_left_stick_click() const { return guest::LoadU16(base, address + 416); }
  void set_raw_left_stick_click(uint16_t v) const { guest::StoreU16(base, address + 416, v); }
  uint32_t raw_left_stick_click_address() const { return address + 416; }
  // action 15 (166)
  uint16_t raw_right_stick_click() const { return guest::LoadU16(base, address + 418); }
  void set_raw_right_stick_click(uint16_t v) const { guest::StoreU16(base, address + 418, v); }
  uint32_t raw_right_stick_click_address() const { return address + 418; }
  // action 16: the left stick's y above 0
  uint16_t raw_left_y_plus() const { return guest::LoadU16(base, address + 420); }
  void set_raw_left_y_plus(uint16_t v) const { guest::StoreU16(base, address + 420, v); }
  uint32_t raw_left_y_plus_address() const { return address + 420; }
  // action 17
  uint16_t raw_left_y_minus() const { return guest::LoadU16(base, address + 422); }
  void set_raw_left_y_minus(uint16_t v) const { guest::StoreU16(base, address + 422, v); }
  uint32_t raw_left_y_minus_address() const { return address + 422; }
  // action 18
  uint16_t raw_left_x_minus() const { return guest::LoadU16(base, address + 424); }
  void set_raw_left_x_minus(uint16_t v) const { guest::StoreU16(base, address + 424, v); }
  uint32_t raw_left_x_minus_address() const { return address + 424; }
  // action 19
  uint16_t raw_left_x_plus() const { return guest::LoadU16(base, address + 426); }
  void set_raw_left_x_plus(uint16_t v) const { guest::StoreU16(base, address + 426, v); }
  uint32_t raw_left_x_plus_address() const { return address + 426; }
  // action 20
  uint16_t raw_right_y_plus() const { return guest::LoadU16(base, address + 428); }
  void set_raw_right_y_plus(uint16_t v) const { guest::StoreU16(base, address + 428, v); }
  uint32_t raw_right_y_plus_address() const { return address + 428; }
  // action 21
  uint16_t raw_right_y_minus() const { return guest::LoadU16(base, address + 430); }
  void set_raw_right_y_minus(uint16_t v) const { guest::StoreU16(base, address + 430, v); }
  uint32_t raw_right_y_minus_address() const { return address + 430; }
  // action 22
  uint16_t raw_right_x_minus() const { return guest::LoadU16(base, address + 432); }
  void set_raw_right_x_minus(uint16_t v) const { guest::StoreU16(base, address + 432, v); }
  uint32_t raw_right_x_minus_address() const { return address + 432; }
  // action 23
  uint16_t raw_right_x_plus() const { return guest::LoadU16(base, address + 434); }
  void set_raw_right_x_plus(uint16_t v) const { guest::StoreU16(base, address + 434, v); }
  uint32_t raw_right_x_plus_address() const { return address + 434; }
  // action 24: the d-pad's axis from actions 4 and 5
  uint16_t raw_dpad_x() const { return guest::LoadU16(base, address + 436); }
  void set_raw_dpad_x(uint16_t v) const { guest::StoreU16(base, address + 436, v); }
  uint32_t raw_dpad_x_address() const { return address + 436; }
  // action 25: from actions 2 and 3
  uint16_t raw_dpad_y() const { return guest::LoadU16(base, address + 438); }
  void set_raw_dpad_y(uint16_t v) const { guest::StoreU16(base, address + 438, v); }
  uint32_t raw_dpad_y_address() const { return address + 438; }
  // action 26: the left stick's axis from actions 18 and 19
  uint16_t raw_left_x() const { return guest::LoadU16(base, address + 440); }
  void set_raw_left_x(uint16_t v) const { guest::StoreU16(base, address + 440, v); }
  uint32_t raw_left_x_address() const { return address + 440; }
  // action 27: from actions 16 and 17
  uint16_t raw_left_y() const { return guest::LoadU16(base, address + 442); }
  void set_raw_left_y(uint16_t v) const { guest::StoreU16(base, address + 442, v); }
  uint32_t raw_left_y_address() const { return address + 442; }
  // action 28: from actions 22 and 23
  uint16_t raw_right_x() const { return guest::LoadU16(base, address + 444); }
  void set_raw_right_x(uint16_t v) const { guest::StoreU16(base, address + 444, v); }
  uint32_t raw_right_x_address() const { return address + 444; }
  // action 29: from actions 20 and 21
  uint16_t raw_right_y() const { return guest::LoadU16(base, address + 446); }
  void set_raw_right_y(uint16_t v) const { guest::StoreU16(base, address + 446, v); }
  uint32_t raw_right_y_address() const { return address + 446; }
  // the input state (an ActionMapperState) starts here: bit i, action i held
  uint32_t held() const { return guest::LoadU32(base, address + 448); }
  void set_held(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t held_address() const { return address + 448; }
  uint32_t held_before() const { return guest::LoadU32(base, address + 452); }
  void set_held_before(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t held_before_address() const { return address + 452; }
  uint32_t pressed() const { return guest::LoadU32(base, address + 456); }
  void set_pressed(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t pressed_address() const { return address + 456; }
  uint32_t released() const { return guest::LoadU32(base, address + 460); }
  void set_released(uint32_t v) const { guest::StoreU32(base, address + 460, v); }
  uint32_t released_address() const { return address + 460; }
  // pressed or repeating
  uint32_t fired() const { return guest::LoadU32(base, address + 464); }
  void set_fired(uint32_t v) const { guest::StoreU32(base, address + 464, v); }
  uint32_t fired_address() const { return address + 464; }
  // ORed into the next update's held before and pressed
  uint32_t pending() const { return guest::LoadU32(base, address + 468); }
  void set_pending(uint32_t v) const { guest::StoreU32(base, address + 468, v); }
  uint32_t pending_address() const { return address + 468; }
  // 30 pointers to the repeat objects
  template <typename T = ActionRepeat>
  T repeat_pointers() const { return T(base, guest::LoadU32(base, address + 472)); }
  void set_repeat_pointers(const View& v) const { guest::StoreU32(base, address + 472, v.address); }
  void set_repeat_pointers(uint32_t v) const { guest::StoreU32(base, address + 472, v); }
  uint32_t repeat_pointers_address() const { return address + 472; }
  // 30 ActionRepeats (16 bytes): the first's vtable
  uint32_t repeats() const { return guest::LoadU32(base, address + 592); }
  void set_repeats(uint32_t v) const { guest::StoreU32(base, address + 592, v); }
  uint32_t repeats_address() const { return address + 592; }
  // the analog stage (an ActionMapperAnalog) starts here: 30 u16s
  uint16_t analog_raw() const { return guest::LoadU16(base, address + 1072); }
  void set_analog_raw(uint16_t v) const { guest::StoreU16(base, address + 1072, v); }
  uint32_t analog_raw_address() const { return address + 1072; }
  // 30 floats: the analog values through each action's curve
  float analog_values() const { return guest::LoadFloat(base, address + 1132); }
  void set_analog_values(float v) const { guest::StoreFloat(base, address + 1132, v); }
  uint32_t analog_values_address() const { return address + 1132; }
  // 30 pointers to the curve objects
  template <typename T = ActionCurve>
  T curve_pointers() const { return T(base, guest::LoadU32(base, address + 1252)); }
  void set_curve_pointers(const View& v) const { guest::StoreU32(base, address + 1252, v.address); }
  void set_curve_pointers(uint32_t v) const { guest::StoreU32(base, address + 1252, v); }
  uint32_t curve_pointers_address() const { return address + 1252; }
  // 30 ActionCurves (16 bytes): the first's vtable
  uint32_t curves() const { return guest::LoadU32(base, address + 1372); }
  void set_curves(uint32_t v) const { guest::StoreU32(base, address + 1372, v); }
  uint32_t curves_address() const { return address + 1372; }
  // 30 ActionThresholds (20 bytes): the first's on
  float thresholds() const { return guest::LoadFloat(base, address + 1852); }
  void set_thresholds(float v) const { guest::StoreFloat(base, address + 1852, v); }
  uint32_t thresholds_address() const { return address + 1852; }
  uint32_t unknown_2452_address() const { return address + 2452; }
  // the listener: an input processor (vtable 0x8206C0C4, input.md)
  uint32_t listener_address() const { return address + 2464; }
  // measure fault: the analog stage's raw[0] (ActionMapperAnalog +0, mapper + 1072)
  uint16_t fault_analog_raw() const { return guest::LoadU16(base, address + 0); }
  void set_fault_analog_raw(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_analog_raw_address() const { return address + 0; }
  // measure fault: ActionMapperState's fired (mapper + 464)
  uint32_t fault_state_fired() const { return guest::LoadU32(base, address + 16); }
  void set_fault_state_fired(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t fault_state_fired_address() const { return address + 16; }
  // measure fault: ActionMapperState's pending (mapper + 468)
  uint32_t fault_state_pending() const { return guest::LoadU32(base, address + 20); }
  void set_fault_state_pending(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t fault_state_pending_address() const { return address + 20; }
  // measure fault: ActionMapperState's repeat pointers (mapper + 472)
  uint32_t fault_state_repeat_pointers() const { return guest::LoadU32(base, address + 24); }
  void set_fault_state_repeat_pointers(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t fault_state_repeat_pointers_address() const { return address + 24; }
  // measure fault: the analog stage's values (mapper + 1132)
  float fault_analog_values() const { return guest::LoadFloat(base, address + 60); }
  void set_fault_analog_values(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t fault_analog_values_address() const { return address + 60; }
  // measure fault: ActionMapperState's first repeat's vtable (mapper + 592)
  uint32_t fault_state_repeats() const { return guest::LoadU32(base, address + 144); }
  void set_fault_state_repeats(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t fault_state_repeats_address() const { return address + 144; }
  // measure fault: the first repeat's delay (mapper + 600)
  uint32_t fault_state_repeat_delay() const { return guest::LoadU32(base, address + 152); }
  void set_fault_state_repeat_delay(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t fault_state_repeat_delay_address() const { return address + 152; }
  // measure fault: the first repeat's period (mapper + 604)
  uint32_t fault_state_repeat_period() const { return guest::LoadU32(base, address + 156); }
  void set_fault_state_repeat_period(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t fault_state_repeat_period_address() const { return address + 156; }
  // measure fault: the analog stage's curve pointers (mapper + 1252)
  uint32_t fault_analog_curve_pointers() const { return guest::LoadU32(base, address + 180); }
  void set_fault_analog_curve_pointers(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t fault_analog_curve_pointers_address() const { return address + 180; }
  // measure fault: the analog stage's first curve's vtable (mapper + 1372)
  uint32_t fault_analog_curves() const { return guest::LoadU32(base, address + 300); }
  void set_fault_analog_curves(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t fault_analog_curves_address() const { return address + 300; }
  // measure fault: the first curve's center (mapper + 1376)
  float fault_analog_curve_center() const { return guest::LoadFloat(base, address + 304); }
  void set_fault_analog_curve_center(float v) const { guest::StoreFloat(base, address + 304, v); }
  uint32_t fault_analog_curve_center_address() const { return address + 304; }
  // measure fault: the first curve's dead zone (mapper + 1380)
  float fault_analog_curve_dead_zone() const { return guest::LoadFloat(base, address + 308); }
  void set_fault_analog_curve_dead_zone(float v) const { guest::StoreFloat(base, address + 308, v); }
  uint32_t fault_analog_curve_dead_zone_address() const { return address + 308; }
  // measure fault: the first curve's maximum (mapper + 1384)
  float fault_analog_curve_maximum() const { return guest::LoadFloat(base, address + 312); }
  void set_fault_analog_curve_maximum(float v) const { guest::StoreFloat(base, address + 312, v); }
  uint32_t fault_analog_curve_maximum_address() const { return address + 312; }
  // measure fault: ActionMapperState +684, the analog stage's values (mapper + 1132)
  float fault_state_analog_values() const { return guest::LoadFloat(base, address + 684); }
  void set_fault_state_analog_values(float v) const { guest::StoreFloat(base, address + 684, v); }
  uint32_t fault_state_analog_values_address() const { return address + 684; }
  // measure fault: ActionMapperState +1404, the first threshold's on (mapper + 1852)
  float fault_threshold_on() const { return guest::LoadFloat(base, address + 1404); }
  void set_fault_threshold_on(float v) const { guest::StoreFloat(base, address + 1404, v); }
  uint32_t fault_threshold_on_address() const { return address + 1404; }
  // measure fault: the first threshold's off (mapper + 1856)
  float fault_threshold_off() const { return guest::LoadFloat(base, address + 1408); }
  void set_fault_threshold_off(float v) const { guest::StoreFloat(base, address + 1408, v); }
  uint32_t fault_threshold_off_address() const { return address + 1408; }
  // measure fault: the first threshold's suppress ticks (mapper + 1860)
  uint32_t fault_threshold_suppress_ticks() const { return guest::LoadU32(base, address + 1412); }
  void set_fault_threshold_suppress_ticks(uint32_t v) const { guest::StoreU32(base, address + 1412, v); }
  uint32_t fault_threshold_suppress_ticks_address() const { return address + 1412; }
  // measure fault: the first threshold's counter (mapper + 1864)
  uint32_t fault_threshold_counter() const { return guest::LoadU32(base, address + 1416); }
  void set_fault_threshold_counter(uint32_t v) const { guest::StoreU32(base, address + 1416, v); }
  uint32_t fault_threshold_counter_address() const { return address + 1416; }
  // measure fault: the first threshold's state (mapper + 1868)
  uint8_t fault_threshold_state() const { return guest::LoadU8(base, address + 1420); }
  void set_fault_threshold_state(uint8_t v) const { guest::StoreU8(base, address + 1420, v); }
  uint32_t fault_threshold_state_address() const { return address + 1420; }
  // the logical pad's 40 values (input manager + 44 + 196 × player)
  template <typename T = ActionPadValues>
  T pad_values() const { return T(base, guest::LoadU32(base, address + 2880)); }
  void set_pad_values(const View& v) const { guest::StoreU32(base, address + 2880, v.address); }
  void set_pad_values(uint32_t v) const { guest::StoreU32(base, address + 2880, v); }
  uint32_t pad_values_address() const { return address + 2880; }
  uint8_t bound() const { return guest::LoadU8(base, address + 2884); }
  void set_bound(uint8_t v) const { guest::StoreU8(base, address + 2884, v); }
  uint32_t bound_address() const { return address + 2884; }
  // 0-3
  uint32_t player() const { return guest::LoadU32(base, address + 2888); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 2888, v); }
  uint32_t player_address() const { return address + 2888; }
  // the input object it's bound to
  template <typename T = ActionMapperInputManager>
  T input_manager() const { return T(base, guest::LoadU32(base, address + 2892)); }
  void set_input_manager(const View& v) const { guest::StoreU32(base, address + 2892, v.address); }
  void set_input_manager(uint32_t v) const { guest::StoreU32(base, address + 2892, v); }
  uint32_t input_manager_address() const { return address + 2892; }
};

// embedded; measure: -
struct ActionMapperState : View {
  using View::View;
  static constexpr uint32_t kSize = 2004;
  // bit i, action i held
  uint32_t held() const { return guest::LoadU32(base, address + 0); }
  void set_held(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t held_address() const { return address + 0; }
  uint32_t held_before() const { return guest::LoadU32(base, address + 4); }
  void set_held_before(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t held_before_address() const { return address + 4; }
  uint32_t pressed() const { return guest::LoadU32(base, address + 8); }
  void set_pressed(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pressed_address() const { return address + 8; }
  uint32_t released() const { return guest::LoadU32(base, address + 12); }
  void set_released(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t released_address() const { return address + 12; }
  // pressed or repeating
  uint32_t fired() const { return guest::LoadU32(base, address + 16); }
  void set_fired(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t fired_address() const { return address + 16; }
  // ORed into the next update's held before and pressed
  uint32_t pending() const { return guest::LoadU32(base, address + 20); }
  void set_pending(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t pending_address() const { return address + 20; }
  // 30 pointers
  template <typename T = ActionRepeat>
  T repeat_pointers() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_repeat_pointers(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_repeat_pointers(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t repeat_pointers_address() const { return address + 24; }
  // 30 ActionRepeats (16 bytes): the first's vtable
  uint32_t repeats() const { return guest::LoadU32(base, address + 144); }
  void set_repeats(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t repeats_address() const { return address + 144; }
  // the analog stage (an ActionMapperAnalog, mapper + 1072): its first raw value
  uint16_t analog() const { return guest::LoadU16(base, address + 624); }
  void set_analog(uint16_t v) const { guest::StoreU16(base, address + 624, v); }
  uint32_t analog_address() const { return address + 624; }
  // the analog stage's 30 floats (its +60)
  float analog_values() const { return guest::LoadFloat(base, address + 684); }
  void set_analog_values(float v) const { guest::StoreFloat(base, address + 684, v); }
  uint32_t analog_values_address() const { return address + 684; }
  // 30 ActionThresholds (20 bytes): the first's on
  float thresholds() const { return guest::LoadFloat(base, address + 1404); }
  void set_thresholds(float v) const { guest::StoreFloat(base, address + 1404, v); }
  uint32_t thresholds_address() const { return address + 1404; }
};

// embedded; measure: -
struct ActionMapperAnalog : View {
  using View::View;
  static constexpr uint32_t kSize = 780;
  // 30 u16s: the raw values the stage took
  uint16_t raw() const { return guest::LoadU16(base, address + 0); }
  void set_raw(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t raw_address() const { return address + 0; }
  // 30 floats: through each action's curve
  float values() const { return guest::LoadFloat(base, address + 60); }
  void set_values(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t values_address() const { return address + 60; }
  // 30 pointers
  template <typename T = ActionCurve>
  T curve_pointers() const { return T(base, guest::LoadU32(base, address + 180)); }
  void set_curve_pointers(const View& v) const { guest::StoreU32(base, address + 180, v.address); }
  void set_curve_pointers(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t curve_pointers_address() const { return address + 180; }
  // 30 ActionCurves (16 bytes): the first's vtable
  uint32_t curves() const { return guest::LoadU32(base, address + 300); }
  void set_curves(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t curves_address() const { return address + 300; }
};

// embedded; measure: -
struct ActionThreshold : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  float on() const { return guest::LoadFloat(base, address + 0); }
  void set_on(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t on_address() const { return address + 0; }
  float off() const { return guest::LoadFloat(base, address + 4); }
  void set_off(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t off_address() const { return address + 4; }
  // set into a linked action's counter when this one turns on
  uint32_t suppress_ticks() const { return guest::LoadU32(base, address + 8); }
  void set_suppress_ticks(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t suppress_ticks_address() const { return address + 8; }
  // while nonzero, counts down and holds the state at 0
  uint32_t counter() const { return guest::LoadU32(base, address + 12); }
  void set_counter(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t counter_address() const { return address + 12; }
  uint8_t state() const { return guest::LoadU8(base, address + 16); }
  void set_state(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
};

// embedded; measure: ActionMapper::Hysteresis.r6
struct ActionMapperConfig : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the address of 30 kinds
  uint32_t kinds() const { return guest::LoadU32(base, address + 0); }
  void set_kinds(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kinds_address() const { return address + 0; }
  // the address of 30 links (30 none)
  uint32_t links() const { return guest::LoadU32(base, address + 4); }
  void set_links(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t links_address() const { return address + 4; }
};

// embedded; measure: ActionRepeat
struct ActionRepeat : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82007F68;
  template <typename T = ActionObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // ticks held
  uint32_t count() const { return guest::LoadU32(base, address + 4); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t count_address() const { return address + 4; }
  uint32_t delay() const { return guest::LoadU32(base, address + 8); }
  void set_delay(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t delay_address() const { return address + 8; }
  uint32_t period() const { return guest::LoadU32(base, address + 12); }
  void set_period(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t period_address() const { return address + 12; }
};

// embedded; measure: ActionCurve
struct ActionCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82007F70;
  template <typename T = ActionObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  float center() const { return guest::LoadFloat(base, address + 4); }
  void set_center(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t center_address() const { return address + 4; }
  float dead_zone() const { return guest::LoadFloat(base, address + 8); }
  void set_dead_zone(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t dead_zone_address() const { return address + 8; }
  float maximum() const { return guest::LoadFloat(base, address + 12); }
  void set_maximum(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t maximum_address() const { return address + 12; }
};

// image; measure: ActionMapper.+144, ActionMapper.+300, ActionMapper.+24.+0, ActionMapper.+180.+0
struct ActionObjectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0: sub_8243DC88 (repeats: the count 0) or sub_82106540 (curves: nothing)
  uint32_t reset() const { return guest::LoadU32(base, address + 0); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t reset_address() const { return address + 0; }
  // slot 1: ActionRepeat::Update sub_8210B568 or ActionCurve::Apply sub_8210B5B8
  uint32_t update() const { return guest::LoadU32(base, address + 4); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t update_address() const { return address + 4; }
};

// embedded; measure: ActionMapper.+2880
struct ActionPadValues : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  float left_x() const { return guest::LoadFloat(base, address + 0); }
  void set_left_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t left_x_address() const { return address + 0; }
  float left_y() const { return guest::LoadFloat(base, address + 4); }
  void set_left_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t left_y_address() const { return address + 4; }
  float left_trigger() const { return guest::LoadFloat(base, address + 8); }
  void set_left_trigger(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t left_trigger_address() const { return address + 8; }
  // 0
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float right_x() const { return guest::LoadFloat(base, address + 16); }
  void set_right_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t right_x_address() const { return address + 16; }
  float right_y() const { return guest::LoadFloat(base, address + 20); }
  void set_right_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t right_y_address() const { return address + 20; }
  float right_trigger() const { return guest::LoadFloat(base, address + 24); }
  void set_right_trigger(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t right_trigger_address() const { return address + 24; }
};

// allocated; measure: ActionMapper.+2892, ActionMapper::Bind.r4
struct ActionMapperInputManager : View {
  using View::View;
  static constexpr uint32_t kSize = 3984;
  template <typename T = ActionMapperInputManagerVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: ActionMapper.+2892.+0, ActionMapper::Bind.r4.+0
struct ActionMapperInputManagerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // slot 2: InputManager::GetLogicalPad 0x82098CD0
  uint32_t get_logical_pad() const { return guest::LoadU32(base, address + 8); }
  void set_get_logical_pad(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t get_logical_pad_address() const { return address + 8; }
  // slot 3: InputManager::AddListener 0x82099430
  uint32_t add_listener() const { return guest::LoadU32(base, address + 12); }
  void set_add_listener(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t add_listener_address() const { return address + 12; }
};

// allocated; measure: ClientList, InputClientList
struct InputClientList : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  static constexpr uint32_t kVtable = 0x82007F18;
  // 0x820713C0, then 0x82007F18
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = InputClientNode>
  T first() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  template <typename T = InputClientNode>
  T last() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_last(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  // 3 words: the nodes added of types 0-2
  uint32_t added_counts() const { return guest::LoadU32(base, address + 12); }
  void set_added_counts(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t added_counts_address() const { return address + 12; }
  uint32_t added_count_1() const { return guest::LoadU32(base, address + 16); }
  void set_added_count_1(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t added_count_1_address() const { return address + 16; }
  // the mappers
  uint32_t added_count_2() const { return guest::LoadU32(base, address + 20); }
  void set_added_count_2(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t added_count_2_address() const { return address + 20; }
  // 3 words: those bound, types 0-2
  uint32_t bound_counts() const { return guest::LoadU32(base, address + 24); }
  void set_bound_counts(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t bound_counts_address() const { return address + 24; }
  uint32_t bound_count_1() const { return guest::LoadU32(base, address + 28); }
  void set_bound_count_1(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t bound_count_1_address() const { return address + 28; }
  // the mappers bound
  uint32_t bound_count_2() const { return guest::LoadU32(base, address + 32); }
  void set_bound_count_2(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t bound_count_2_address() const { return address + 32; }
  // the object ActionMappers::Init was given
  template <typename T = ActionMappersOwner>
  T manager() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_manager(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_manager(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t manager_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
};

// embedded; measure: ClientNode, InputClient
struct InputClientNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr uint32_t kVtable = 0x82007ED8;
  // 0x82071380, then 0x82007ED8, then the derived class's
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 2 for an action mapper
  uint32_t type() const { return guest::LoadU32(base, address + 4); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t type_address() const { return address + 4; }
  template <typename T = InputClientNode>
  T next() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_address() const { return address + 8; }
};

// allocated; measure: InputClientList.+36
struct ActionMappersOwner : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = ActionMappersOwnerVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: InputClientList.+36.+0
struct ActionMappersOwnerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // probably: what InputClientList::BindAll binds the mappers to
  uint32_t get_input_manager() const { return guest::LoadU32(base, address + 36); }
  void set_get_input_manager(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t get_input_manager_address() const { return address + 36; }
};

// embedded; measure: InputProcessor::AllHeld.r4, InputProcessor::TestHeld.r4
struct ActionButtonMask : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t mask() const { return guest::LoadU32(base, address + 0); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t mask_address() const { return address + 0; }
};

// image; measure: -
struct ActionSettings : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  float on() const { return guest::LoadFloat(base, address + 0); }
  void set_on(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t on_address() const { return address + 0; }
  float off() const { return guest::LoadFloat(base, address + 4); }
  void set_off(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t off_address() const { return address + 4; }
  uint32_t suppress_ticks() const { return guest::LoadU32(base, address + 8); }
  void set_suppress_ticks(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t suppress_ticks_address() const { return address + 8; }
  uint32_t repeat_delay() const { return guest::LoadU32(base, address + 12); }
  void set_repeat_delay(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t repeat_delay_address() const { return address + 12; }
  uint32_t repeat_period() const { return guest::LoadU32(base, address + 16); }
  void set_repeat_period(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t repeat_period_address() const { return address + 16; }
  float curve_center() const { return guest::LoadFloat(base, address + 20); }
  void set_curve_center(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t curve_center_address() const { return address + 20; }
  float curve_dead_zone() const { return guest::LoadFloat(base, address + 24); }
  void set_curve_dead_zone(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t curve_dead_zone_address() const { return address + 24; }
  float curve_maximum() const { return guest::LoadFloat(base, address + 28); }
  void set_curve_maximum(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t curve_maximum_address() const { return address + 28; }
};

// embedded; measure: ActionMapper::Words.r4@+8
struct ActionMapperConfigAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// The image words docs/functions/action-mappers.md's code uses.
struct ActionMappersGlobals {
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // probably 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // probably -1.0
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // probably 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // a new mapper's threshold off
  static float threshold_off(uint8_t* base) { return guest::LoadFloat(base, 0x8200296Cu); }
  static void set_threshold_off(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200296Cu, v); }
  static constexpr uint32_t threshold_off_address() { return 0x8200296Cu; }
  // a new mapper's threshold on
  static float threshold_on(uint8_t* base) { return guest::LoadFloat(base, 0x82002A10u); }
  static void set_threshold_on(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A10u, v); }
  static constexpr uint32_t threshold_on_address() { return 0x82002A10u; }
  // probably 1/32767: CurveUnsigned's raw value to the curve
  static float unsigned_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002BD4u); }
  static void set_unsigned_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002BD4u, v); }
  static constexpr uint32_t unsigned_scale_address() { return 0x82002BD4u; }
  // a pad value to a raw u16 (probably 32767)
  static float pad_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82007F58u); }
  static void set_pad_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82007F58u, v); }
  static constexpr uint32_t pad_scale_address() { return 0x82007F58u; }
  // the axes' factor on the direction pairs
  static float axis_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82007F5Cu); }
  static void set_axis_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82007F5Cu, v); }
  static constexpr uint32_t axis_scale_address() { return 0x82007F5Cu; }
  // CurveCentered's factor
  static float centered_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82007F60u); }
  static void set_centered_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82007F60u, v); }
  static constexpr uint32_t centered_scale_address() { return 0x82007F60u; }
  // CurveCentered's center, taken from the raw value
  static float centered_offset(uint8_t* base) { return guest::LoadFloat(base, 0x82007F64u); }
  static void set_centered_offset(uint8_t* base, float v) { guest::StoreFloat(base, 0x82007F64u, v); }
  static constexpr uint32_t centered_offset_address() { return 0x82007F64u; }
  // the object ActionMappers::Init was given
  template <typename T = ActionMappersOwner>
  static T owner(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826BAB78u)); }
  static void set_owner(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826BAB78u, v.address); }
  static void set_owner(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB78u, v); }
  static constexpr uint32_t owner_address() { return 0x826BAB78u; }
  template <typename T = InputClientList>
  static T client_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826BAB7Cu)); }
  static void set_client_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826BAB7Cu, v.address); }
  static void set_client_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB7Cu, v); }
  static constexpr uint32_t client_list_address() { return 0x826BAB7Cu; }
  // 4 entries of 8 bytes: +0 a mapper (players 0-3), +4 a word the menus test
  template <typename T = ActionMapper>
  static T mappers(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826BAB80u)); }
  static void set_mappers(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826BAB80u, v.address); }
  static void set_mappers(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB80u, v); }
  static constexpr uint32_t mappers_address() { return 0x826BAB80u; }
  // 1 at boot
  static uint32_t settings_index(uint8_t* base) { return guest::LoadU32(base, 0x82600B84u); }
  static void set_settings_index(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600B84u, v); }
  static constexpr uint32_t settings_index_address() { return 0x82600B84u; }
  // ActionSettings entries of 32 bytes, picked by the index
  static constexpr uint32_t settings_address() { return 0x82600B88u; }
  // ClientList's one list (the lookups read it here)
  template <typename T = InputClientList>
  static T the_client_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82970C14u)); }
  static void set_the_client_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82970C14u, v.address); }
  static void set_the_client_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970C14u, v); }
  static constexpr uint32_t the_client_list_address() { return 0x82970C14u; }
};

}  // namespace schema
