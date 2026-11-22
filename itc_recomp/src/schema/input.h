// The views of docs/functions/input.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/input.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: InputManager, Input, InputManager::AddListener.r3
struct InputManager : View {
  using View::View;
  static constexpr uint32_t kSize = 3984;
  static constexpr uint32_t kVtable = 0x82000F48;
  // 0x82000F48: 0 sub_82098FF8, 1 GetProcessor, 2 GetLogicalPad, 3 AddListener, 4 sub_82099048, 5 GetPhysicalPad
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the set-up block's +4: a pointer to the framework's +16
  uint32_t framework_part() const { return guest::LoadU32(base, address + 4); }
  void set_framework_part(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_part_address() const { return address + 4; }
  // five LogicalPads (196 bytes): 0-3 the players', 4 the any pad
  uint32_t logical_pads_address() const { return address + 8; }
  // four physical pads (212 bytes, model-animation.toml's PhysicalPad)
  uint32_t physical_pads_address() const { return address + 988; }
  // five InputProcessors (416 bytes)
  uint32_t processors_address() const { return address + 1840; }
  // processor 0's vtable (Input::Update's read)
  template <typename T = InputProcessorVtable>
  T processor0_vtable() const { return T(base, guest::LoadU32(base, address + 1840)); }
  void set_processor0_vtable(const View& v) const { guest::StoreU32(base, address + 1840, v.address); }
  void set_processor0_vtable(uint32_t v) const { guest::StoreU32(base, address + 1840, v); }
  uint32_t processor0_vtable_address() const { return address + 1840; }
  // five PtrLists (12 bytes) of listeners, one per logical pad
  uint32_t listener_lists_address() const { return address + 3920; }
  // list 0's head node (Input::Update's read)
  template <typename T = PtrListNode>
  T listeners0_head() const { return T(base, guest::LoadU32(base, address + 3924)); }
  void set_listeners0_head(const View& v) const { guest::StoreU32(base, address + 3924, v.address); }
  void set_listeners0_head(uint32_t v) const { guest::StoreU32(base, address + 3924, v); }
  uint32_t listeners0_head_address() const { return address + 3924; }
  // list 0's size
  uint32_t listeners0_size() const { return guest::LoadU32(base, address + 3928); }
  void set_listeners0_size(uint32_t v) const { guest::StoreU32(base, address + 3928, v); }
  uint32_t listeners0_size_address() const { return address + 3928; }
  // not set by the constructor
  uint32_t unknown_3980() const { return guest::LoadU32(base, address + 3980); }
  void set_unknown_3980(uint32_t v) const { guest::StoreU32(base, address + 3980, v); }
  uint32_t unknown_3980_address() const { return address + 3980; }
};

// embedded; measure: InputManager::InputManager.r4
struct InputManagerArgs : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the message dispatcher
  template <typename T = InputDispatcher>
  T dispatcher() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_dispatcher(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_dispatcher(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t dispatcher_address() const { return address + 0; }
  // a pointer to the framework's +16, kept at the manager's +4
  uint32_t framework_part() const { return guest::LoadU32(base, address + 4); }
  void set_framework_part(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_part_address() const { return address + 4; }
};

// embedded; measure: InputManager::InputManager.r4.+0
struct InputDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = InputDispatcherVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the vtable's slot +4 (InputDispatcherVtable), read through r11 after the vtable's load
  uint32_t fault_vtable_4() const { return guest::LoadU32(base, address + 4); }
  void set_fault_vtable_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t fault_vtable_4_address() const { return address + 4; }
  // measure fault: the vtable's slot +8, as fault_vtable_4
  uint32_t fault_vtable_8() const { return guest::LoadU32(base, address + 8); }
  void set_fault_vtable_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fault_vtable_8_address() const { return address + 8; }
};

// image; measure: -
struct InputDispatcherVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // slot +4: registers message 7's handler (sub_82098CE8)
  uint32_t register_7() const { return guest::LoadU32(base, address + 4); }
  void set_register_7(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t register_7_address() const { return address + 4; }
  // slot +8: registers message 5's handler (Input::Update sub_820990F0)
  uint32_t register_5() const { return guest::LoadU32(base, address + 8); }
  void set_register_5(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t register_5_address() const { return address + 8; }
};

// embedded; measure: InputProcessor::Update.r4, LogicalPad::Bind.r3, InputManager@+8
struct LogicalPad : View {
  using View::View;
  static constexpr uint32_t kSize = 196;
  static constexpr uint32_t kVtable = 0x8206CBEC;
  // 0x8206CBEC (InputProcessor::Update's r4: any device, a logical pad in play)
  template <typename T = InputDeviceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // four InputPadBindings (8 bytes), cleared word by word by the constructor
  uint32_t bindings_address() const { return address + 4; }
  // the 40 values (to +195): 0-7 the sticks and triggers (LX, LY, LT, 0, RX, RY, RT, 0), 8-39 the buttons
  float values() const { return guest::LoadFloat(base, address + 36); }
  void set_values(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t values_address() const { return address + 36; }
};

// embedded; measure: -
struct InputPadBinding : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t enabled() const { return guest::LoadU8(base, address + 0); }
  void set_enabled(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t enabled_address() const { return address + 0; }
  // a physical pad, or for the any pad a logical pad 0-3
  uint32_t device() const { return guest::LoadU32(base, address + 4); }
  void set_device(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t device_address() const { return address + 4; }
};

// image; measure: InputProcessor::Update.r4.+0
struct InputDeviceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0, probably the destructor (sub_82591898, sub_82592058)
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: the values' address (LogicalPad::Values sub_82591B30, PhysicalPad::Values sub_820988B8)
  uint32_t values() const { return guest::LoadU32(base, address + 4); }
  void set_values(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t values_address() const { return address + 4; }
};

// embedded; measure: InputProcessor, InputProcessorBase, InputProcessor::Assign.r4, InputProcessor::InputProcessor.r3
struct InputProcessor : View {
  using View::View;
  static constexpr uint32_t kSize = 416;
  static constexpr uint32_t kVtable = 0x8206C0C4;
  // 0x8206C0C4 (the base's 0x8206CC00 first)
  template <typename T = InputProcessorVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0.5 at construction; the four dead zones apply to both sticks
  float dead_zone_x() const { return guest::LoadFloat(base, address + 16); }
  void set_dead_zone_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t dead_zone_x_address() const { return address + 16; }
  uint64_t dead_zone_x_pair() const { return guest::LoadU64(base, address + 16); }
  void set_dead_zone_x_pair(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t dead_zone_x_pair_address() const { return address + 16; }
  // 0.5
  float dead_zone_y() const { return guest::LoadFloat(base, address + 20); }
  void set_dead_zone_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t dead_zone_y_address() const { return address + 20; }
  // 0.0
  float dead_zone_trigger() const { return guest::LoadFloat(base, address + 24); }
  void set_dead_zone_trigger(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t dead_zone_trigger_address() const { return address + 24; }
  uint64_t dead_zone_trigger_pair() const { return guest::LoadU64(base, address + 24); }
  void set_dead_zone_trigger_pair(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t dead_zone_trigger_pair_address() const { return address + 24; }
  // 0.0
  float dead_zone_unused() const { return guest::LoadFloat(base, address + 28); }
  void set_dead_zone_unused(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t dead_zone_unused_address() const { return address + 28; }
  // the pad's values 0-7 (to +63)
  float raw_lx() const { return guest::LoadFloat(base, address + 32); }
  void set_raw_lx(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t raw_lx_address() const { return address + 32; }
  uint64_t raw_lx_pair() const { return guest::LoadU64(base, address + 32); }
  void set_raw_lx_pair(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t raw_lx_pair_address() const { return address + 32; }
  float raw_ly() const { return guest::LoadFloat(base, address + 36); }
  void set_raw_ly(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t raw_ly_address() const { return address + 36; }
  float raw_lt() const { return guest::LoadFloat(base, address + 40); }
  void set_raw_lt(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t raw_lt_address() const { return address + 40; }
  uint64_t raw_lt_pair() const { return guest::LoadU64(base, address + 40); }
  void set_raw_lt_pair(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t raw_lt_pair_address() const { return address + 40; }
  // 1.0 at construction (a vector's w)
  float raw_3() const { return guest::LoadFloat(base, address + 44); }
  void set_raw_3(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t raw_3_address() const { return address + 44; }
  float raw_rx() const { return guest::LoadFloat(base, address + 48); }
  void set_raw_rx(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t raw_rx_address() const { return address + 48; }
  uint64_t raw_rx_pair() const { return guest::LoadU64(base, address + 48); }
  void set_raw_rx_pair(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t raw_rx_pair_address() const { return address + 48; }
  float raw_ry() const { return guest::LoadFloat(base, address + 52); }
  void set_raw_ry(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t raw_ry_address() const { return address + 52; }
  float raw_rt() const { return guest::LoadFloat(base, address + 56); }
  void set_raw_rt(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t raw_rt_address() const { return address + 56; }
  uint64_t raw_rt_pair() const { return guest::LoadU64(base, address + 56); }
  void set_raw_rt_pair(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t raw_rt_pair_address() const { return address + 56; }
  // 1.0 at construction
  float raw_7() const { return guest::LoadFloat(base, address + 60); }
  void set_raw_7(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t raw_7_address() const { return address + 60; }
  // the raw values with the dead zone taken out (to +95)
  float adjusted_lx() const { return guest::LoadFloat(base, address + 64); }
  void set_adjusted_lx(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t adjusted_lx_address() const { return address + 64; }
  uint64_t adjusted_lx_pair() const { return guest::LoadU64(base, address + 64); }
  void set_adjusted_lx_pair(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t adjusted_lx_pair_address() const { return address + 64; }
  float adjusted_ly() const { return guest::LoadFloat(base, address + 68); }
  void set_adjusted_ly(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t adjusted_ly_address() const { return address + 68; }
  float adjusted_lt() const { return guest::LoadFloat(base, address + 72); }
  void set_adjusted_lt(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t adjusted_lt_address() const { return address + 72; }
  uint64_t adjusted_lt_pair() const { return guest::LoadU64(base, address + 72); }
  void set_adjusted_lt_pair(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t adjusted_lt_pair_address() const { return address + 72; }
  // 1.0 at construction
  float adjusted_3() const { return guest::LoadFloat(base, address + 76); }
  void set_adjusted_3(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t adjusted_3_address() const { return address + 76; }
  float adjusted_rx() const { return guest::LoadFloat(base, address + 80); }
  void set_adjusted_rx(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t adjusted_rx_address() const { return address + 80; }
  uint64_t adjusted_rx_pair() const { return guest::LoadU64(base, address + 80); }
  void set_adjusted_rx_pair(uint64_t v) const { guest::StoreU64(base, address + 80, v); }
  uint32_t adjusted_rx_pair_address() const { return address + 80; }
  float adjusted_ry() const { return guest::LoadFloat(base, address + 84); }
  void set_adjusted_ry(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t adjusted_ry_address() const { return address + 84; }
  float adjusted_rt() const { return guest::LoadFloat(base, address + 88); }
  void set_adjusted_rt(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t adjusted_rt_address() const { return address + 88; }
  uint64_t adjusted_rt_pair() const { return guest::LoadU64(base, address + 88); }
  void set_adjusted_rt_pair(uint64_t v) const { guest::StoreU64(base, address + 88, v); }
  uint32_t adjusted_rt_pair_address() const { return address + 88; }
  // 1.0 at construction
  float adjusted_7() const { return guest::LoadFloat(base, address + 92); }
  void set_adjusted_7(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t adjusted_7_address() const { return address + 92; }
  // the left stick's (x, y) normalized, or 0 (stvx: 16 bytes)
  float left_direction_x() const { return guest::LoadFloat(base, address + 96); }
  void set_left_direction_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t left_direction_x_address() const { return address + 96; }
  uint64_t left_direction_x_pair() const { return guest::LoadU64(base, address + 96); }
  void set_left_direction_x_pair(uint64_t v) const { guest::StoreU64(base, address + 96, v); }
  uint32_t left_direction_x_pair_address() const { return address + 96; }
  float left_direction_y() const { return guest::LoadFloat(base, address + 100); }
  void set_left_direction_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t left_direction_y_address() const { return address + 100; }
  float left_direction_z() const { return guest::LoadFloat(base, address + 104); }
  void set_left_direction_z(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t left_direction_z_address() const { return address + 104; }
  uint64_t left_direction_z_pair() const { return guest::LoadU64(base, address + 104); }
  void set_left_direction_z_pair(uint64_t v) const { guest::StoreU64(base, address + 104, v); }
  uint32_t left_direction_z_pair_address() const { return address + 104; }
  // 1.0 at construction
  float left_direction_w() const { return guest::LoadFloat(base, address + 108); }
  void set_left_direction_w(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t left_direction_w_address() const { return address + 108; }
  // the right stick's, as the left's
  float right_direction_x() const { return guest::LoadFloat(base, address + 112); }
  void set_right_direction_x(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t right_direction_x_address() const { return address + 112; }
  uint64_t right_direction_x_pair() const { return guest::LoadU64(base, address + 112); }
  void set_right_direction_x_pair(uint64_t v) const { guest::StoreU64(base, address + 112, v); }
  uint32_t right_direction_x_pair_address() const { return address + 112; }
  float right_direction_y() const { return guest::LoadFloat(base, address + 116); }
  void set_right_direction_y(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t right_direction_y_address() const { return address + 116; }
  float right_direction_z() const { return guest::LoadFloat(base, address + 120); }
  void set_right_direction_z(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t right_direction_z_address() const { return address + 120; }
  uint64_t right_direction_z_pair() const { return guest::LoadU64(base, address + 120); }
  void set_right_direction_z_pair(uint64_t v) const { guest::StoreU64(base, address + 120, v); }
  uint32_t right_direction_z_pair_address() const { return address + 120; }
  // 1.0 at construction
  float right_direction_w() const { return guest::LoadFloat(base, address + 124); }
  void set_right_direction_w(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t right_direction_w_address() const { return address + 124; }
  float left_length() const { return guest::LoadFloat(base, address + 128); }
  void set_left_length(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t left_length_address() const { return address + 128; }
  float right_length() const { return guest::LoadFloat(base, address + 132); }
  void set_right_length(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t right_length_address() const { return address + 132; }
  // the pad's values 8-39 (32 floats, to +263)
  float buttons() const { return guest::LoadFloat(base, address + 136); }
  void set_buttons(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t buttons_address() const { return address + 136; }
  // bits newly set this tick
  uint32_t pressed() const { return guest::LoadU32(base, address + 264); }
  void set_pressed(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t pressed_address() const { return address + 264; }
  // bits newly clear this tick
  uint32_t released() const { return guest::LoadU32(base, address + 268); }
  void set_released(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t released_address() const { return address + 268; }
  // bit i: buttons[i] above 0.5
  uint32_t held() const { return guest::LoadU32(base, address + 272); }
  void set_held(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t held_address() const { return address + 272; }
  // 20 at construction
  int32_t repeat_delay() const { return int32_t(guest::LoadU32(base, address + 276)); }
  void set_repeat_delay(int32_t v) const { guest::StoreU32(base, address + 276, uint32_t(v)); }
  uint32_t repeat_delay_address() const { return address + 276; }
  // 4 at construction
  int32_t repeat_period() const { return int32_t(guest::LoadU32(base, address + 280)); }
  void set_repeat_period(int32_t v) const { guest::StoreU32(base, address + 280, uint32_t(v)); }
  uint32_t repeat_period_address() const { return address + 280; }
  // 32 counters (s32): the ticks each bit has been held
  uint32_t held_ticks_address() const { return address + 284; }
};

// image; measure: Input.+1840
struct InputProcessorVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0: sub_8258CF40 (the base's sub_825925A8)
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: InputProcessor::Update sub_82592188
  uint32_t update() const { return guest::LoadU32(base, address + 4); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t update_address() const { return address + 4; }
};

// embedded; measure: InputManager::GetProcessor.r4, InputManager::GetLogicalPad.r4, InputManager::GetPhysicalPad.r4, InputManager::AddListener.r4, LogicalPad::Bind.r4
struct InputIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: InputProcessor::TestPressed.r4, InputProcessor::TestRepeat.r4, InputProcessor::AnyPressed.r4, InputProcessor::PressedOrRepeat.r4
struct InputButtonMask : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t mask() const { return guest::LoadU32(base, address + 0); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t mask_address() const { return address + 0; }
};

// embedded; measure: PtrList::Insert.r5
struct PtrListInsertValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// allocated; measure: new<?>@PtrList::AllocateNodes
struct PtrListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = PtrListNode>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = PtrListNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // a listener (an InputProcessor) in the input manager's lists
  uint32_t value() const { return guest::LoadU32(base, address + 8); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t value_address() const { return address + 8; }
  // measure fault: AllocateNodes is the 12-byte allocator RequestVector::InsertN (framework.md) also uses: +12 to +23 are its block's second request
  uint32_t fault_request_12() const { return guest::LoadU32(base, address + 12); }
  void set_fault_request_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fault_request_12_address() const { return address + 12; }
  // measure fault: as fault_request_12
  uint32_t fault_request_16() const { return guest::LoadU32(base, address + 16); }
  void set_fault_request_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t fault_request_16_address() const { return address + 16; }
  // measure fault: as fault_request_12
  uint32_t fault_request_20() const { return guest::LoadU32(base, address + 20); }
  void set_fault_request_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t fault_request_20_address() const { return address + 20; }
};

}  // namespace schema
