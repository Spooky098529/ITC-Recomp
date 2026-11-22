// The views of docs/functions/title-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/title-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ModeType4, ModeType4::Restart.r4, ModeType4::WaitForLeave.r4, ModeType4::LeaveScreen.r4, ModeType4::NextMode.r4, ModeType4::Leaving.r4, ModeType4::NextModeAnswer.r4
#pragma pack(push, 1)
struct ModeType4Data {
  uint32_t vtable = 0;  // +0
  uint32_t delegate_ptr = 0;  // +4
  uint32_t delegate_vtable = 0;  // +8
  uint32_t delegate_object = 0;  // +12
  uint32_t delegate_function = 0;  // +16
  uint32_t delegate_adjustment = 0;  // +20
  uint8_t pad_24[8] = {};
  uint32_t vtable32 = 0;  // +32
  uint8_t handlers[200] = {};  // +36, guest order
  uint32_t handler_count = 0;  // +236
  uint8_t ring[600] = {};  // +240, guest order
  uint32_t ring_count = 0;  // +840
  uint32_t ring_start = 0;  // +844
  uint32_t block = 0;  // +848
};
#pragma pack(pop)
static_assert(sizeof(ModeType4Data) == 852);
inline constexpr memory::LayoutField kModeType4Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kModeType4Layout = {kModeType4Fields, 11};

struct ModeType4 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x82006BE0;
  ModeType4Data* data() const { return static_cast<ModeType4Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeType4 New(uint8_t* base) {
    return ModeType4(base, memory::MakeHandle(new ModeType4Data(), sizeof(ModeType4Data), "ModeType4", &kModeType4Layout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeType4Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006BE0
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer, the delegate functions' r3), not the mode's
  uint8_t answer_changed_bits() const { return on_host() ? uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, 0, 1)) : guest::LoadU8(base, address + 0); }
  void set_answer_changed_bits(uint8_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // the delegate at +8; 0 when its address wraps (never). Also the answer's next type +4 by the measure's join
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, on_host() ? data()->delegate_ptr : guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  void set_delegate_ptr(uint32_t v) const { if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_bits() const { return on_host() ? data()->delegate_ptr : guest::LoadU32(base, address + 4); }
  void set_delegate_ptr_bits(uint32_t v) const { if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  // 0x82010FA0 (ModeDelegate::Invoke)
  uint32_t delegate_vtable() const { return on_host() ? uint32_t(data()->delegate_vtable) : uint32_t(guest::LoadU32(base, address + 8)); }
  void set_delegate_vtable(uint32_t v) const { if (on_host()) data()->delegate_vtable = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable_bits() const { return on_host() ? data()->delegate_vtable : guest::LoadU32(base, address + 8); }
  void set_delegate_vtable_bits(uint32_t v) const { if (on_host()) data()->delegate_vtable = v; else guest::StoreU32(base, address + 8, v); }
  // the mode
  uint32_t delegate_object() const { return on_host() ? uint32_t(data()->delegate_object) : uint32_t(guest::LoadU32(base, address + 12)); }
  void set_delegate_object(uint32_t v) const { if (on_host()) data()->delegate_object = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object_bits() const { return on_host() ? data()->delegate_object : guest::LoadU32(base, address + 12); }
  void set_delegate_object_bits(uint32_t v) const { if (on_host()) data()->delegate_object = v; else guest::StoreU32(base, address + 12, v); }
  // Mode::Slot8Thunk (Restart) after Init, then NextModeThunk, Slot10Thunk, Slot11Thunk or Slot12Thunk
  uint32_t delegate_function() const { return on_host() ? uint32_t(data()->delegate_function) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_delegate_function(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_bits() const { return on_host() ? data()->delegate_function : guest::LoadU32(base, address + 16); }
  void set_delegate_function_bits(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  // 0
  uint32_t delegate_adjustment() const { return on_host() ? uint32_t(data()->delegate_adjustment) : uint32_t(guest::LoadU32(base, address + 20)); }
  void set_delegate_adjustment(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_bits() const { return on_host() ? data()->delegate_adjustment : guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  // 0x82006BD4: Interrupt, ForceExit, Resume
  uint32_t vtable32() const { return on_host() ? uint32_t(data()->vtable32) : uint32_t(guest::LoadU32(base, address + 32)); }
  void set_vtable32(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_bits() const { return on_host() ? data()->vtable32 : guest::LoadU32(base, address + 32); }
  void set_vtable32_bits(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (the five machines, in Init's order)
  uint8_t* handlers() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 36 : guest::Pointer(base, address + 36); }
  uint32_t handler_count() const { return on_host() ? uint32_t(data()->handler_count) : uint32_t(guest::LoadU32(base, address + 236)); }
  void set_handler_count(uint32_t v) const { if (on_host()) data()->handler_count = v; else guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_bits() const { return on_host() ? data()->handler_count : guest::LoadU32(base, address + 236); }
  void set_handler_count_bits(uint32_t v) const { if (on_host()) data()->handler_count = v; else guest::StoreU32(base, address + 236, v); }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint8_t* ring() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 240 : guest::Pointer(base, address + 240); }
  uint32_t ring_count() const { return on_host() ? uint32_t(data()->ring_count) : uint32_t(guest::LoadU32(base, address + 840)); }
  void set_ring_count(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_bits() const { return on_host() ? data()->ring_count : guest::LoadU32(base, address + 840); }
  void set_ring_count_bits(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_start() const { return on_host() ? uint32_t(data()->ring_start) : uint32_t(guest::LoadU32(base, address + 844)); }
  void set_ring_start(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_bits() const { return on_host() ? data()->ring_start : guest::LoadU32(base, address + 844); }
  void set_ring_start_bits(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 844, v); }
  // VirtualAlloc'ed by Init (48,220 bytes)
  template <typename T = Type4Block>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 848)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 848); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  // measure fault: the +32 interface's +808 (ModeType4Interface32), the mode's +840
  uint32_t fault_interface_ring_count_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, 808, 4)) : guest::LoadU32(base, address + 808); }
  void set_fault_interface_ring_count_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t fault_interface_ring_count() const { return uint32_t(fault_interface_ring_count_bits()); }
  void set_fault_interface_ring_count(uint32_t v) const { set_fault_interface_ring_count_bits(uint32_t(v)); }
  // measure fault: the +32 interface's +812, the mode's +844
  uint32_t fault_interface_ring_start_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, 812, 4)) : guest::LoadU32(base, address + 812); }
  void set_fault_interface_ring_start_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, 812, 4, v); else guest::StoreU32(base, address + 812, v); }
  uint32_t fault_interface_ring_start() const { return uint32_t(fault_interface_ring_start_bits()); }
  void set_fault_interface_ring_start(uint32_t v) const { set_fault_interface_ring_start_bits(uint32_t(v)); }
  // measure fault: the +32 interface's +816, the mode's +848
  uint32_t fault_interface_block_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, 816, 4)) : guest::LoadU32(base, address + 816); }
  void set_fault_interface_block_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Layout, 816, 4, v); else guest::StoreU32(base, address + 816, v); }
  uint32_t fault_interface_block() const { return uint32_t(fault_interface_block_bits()); }
  void set_fault_interface_block(uint32_t v) const { set_fault_interface_block_bits(uint32_t(v)); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct ModeType4Interface32Data {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[204] = {};
  uint8_t ring[600] = {};  // +208, guest order
  uint32_t ring_count = 0;  // +808
  uint32_t ring_start = 0;  // +812
  uint32_t block = 0;  // +816
};
#pragma pack(pop)
static_assert(sizeof(ModeType4Interface32Data) == 820);
inline constexpr memory::LayoutField kModeType4Interface32Fields[] = {{0, 4}, {808, 4}, {812, 4}, {816, 4}};
inline constexpr memory::Layout kModeType4Interface32Layout = {kModeType4Interface32Fields, 4};

struct ModeType4Interface32 : View {
  using View::View;
  static constexpr uint32_t kSize = 820;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ModeType4Interface32Data* data() const { return static_cast<ModeType4Interface32Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeType4Interface32 New(uint8_t* base) {
    return ModeType4Interface32(base, memory::MakeHandle(new ModeType4Interface32Data(), sizeof(ModeType4Interface32Data), "ModeType4Interface32", &kModeType4Interface32Layout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeType4Interface32Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType4Interface32Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006BD4
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // the mode's ring (+240)
  uint8_t* ring() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 208 : guest::Pointer(base, address + 208); }
  // the mode's +840
  uint32_t ring_count() const { return on_host() ? uint32_t(data()->ring_count) : uint32_t(guest::LoadU32(base, address + 808)); }
  void set_ring_count(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 808, v); }
  uint32_t ring_count_bits() const { return on_host() ? data()->ring_count : guest::LoadU32(base, address + 808); }
  void set_ring_count_bits(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 808, v); }
  // the mode's +844
  uint32_t ring_start() const { return on_host() ? uint32_t(data()->ring_start) : uint32_t(guest::LoadU32(base, address + 812)); }
  void set_ring_start(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 812, v); }
  uint32_t ring_start_bits() const { return on_host() ? data()->ring_start : guest::LoadU32(base, address + 812); }
  void set_ring_start_bits(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 812, v); }
  // the mode's +848
  template <typename T = Type4Block>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 816)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 816, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 816); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 816, v); }
};

// allocated; measure: TitleScreen, ModeType4::NextModeAnswer.r4.+848, ModeType4::NextMode.r4.+848
struct Type4Block : View {
  using View::View;
  static constexpr uint32_t kSize = 48220;
  // the options' machine (Type4Options::Init, options-screen.md)
  uint32_t options_address() const { return address + 0; }
  uint32_t options_state() const { return guest::LoadU32(base, address + 9604); }
  void set_options_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t options_state_address() const { return address + 9604; }
  uint32_t options_loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_options_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t options_loaded_address() const { return address + 9608; }
  // 1 from Init
  uint32_t options_reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_options_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t options_reenter_address() const { return address + 9612; }
  // 0 from Init
  uint32_t options_delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_options_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t options_delete_movie_address() const { return address + 9616; }
  // the controller options' machine (Type4ControllerOptions::Init, options-screen.md)
  uint32_t controller_options_address() const { return address + 9640; }
  uint32_t controller_options_state() const { return guest::LoadU32(base, address + 19244); }
  void set_controller_options_state(uint32_t v) const { guest::StoreU32(base, address + 19244, v); }
  uint32_t controller_options_state_address() const { return address + 19244; }
  uint32_t controller_options_loaded() const { return guest::LoadU32(base, address + 19248); }
  void set_controller_options_loaded(uint32_t v) const { guest::StoreU32(base, address + 19248, v); }
  uint32_t controller_options_loaded_address() const { return address + 19248; }
  // 0 from Init
  uint32_t controller_options_reenter() const { return guest::LoadU32(base, address + 19252); }
  void set_controller_options_reenter(uint32_t v) const { guest::StoreU32(base, address + 19252, v); }
  uint32_t controller_options_reenter_address() const { return address + 19252; }
  // 1 from Init
  uint32_t controller_options_delete_movie() const { return guest::LoadU32(base, address + 19256); }
  void set_controller_options_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 19256, v); }
  uint32_t controller_options_delete_movie_address() const { return address + 19256; }
  // the save screen's machine (Type4SaveScreen::Init, save-screen.md)
  uint32_t save_screen_address() const { return address + 19280; }
  // 5 shown (Interrupt)
  uint32_t save_screen_state() const { return guest::LoadU32(base, address + 28884); }
  void set_save_screen_state(uint32_t v) const { guest::StoreU32(base, address + 28884, v); }
  uint32_t save_screen_state_address() const { return address + 28884; }
  uint32_t save_screen_loaded() const { return guest::LoadU32(base, address + 28888); }
  void set_save_screen_loaded(uint32_t v) const { guest::StoreU32(base, address + 28888, v); }
  uint32_t save_screen_loaded_address() const { return address + 28888; }
  // 1 from Init
  uint32_t save_screen_reenter() const { return guest::LoadU32(base, address + 28892); }
  void set_save_screen_reenter(uint32_t v) const { guest::StoreU32(base, address + 28892, v); }
  uint32_t save_screen_reenter_address() const { return address + 28892; }
  // 0 from Init
  uint32_t save_screen_delete_movie() const { return guest::LoadU32(base, address + 28896); }
  void set_save_screen_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 28896, v); }
  uint32_t save_screen_delete_movie_address() const { return address + 28896; }
  // the title load's machine (TitleLoad::Init, Type4Machine)
  uint32_t title_load_address() const { return address + 28920; }
  uint32_t title_load_state() const { return guest::LoadU32(base, address + 38524); }
  void set_title_load_state(uint32_t v) const { guest::StoreU32(base, address + 38524, v); }
  uint32_t title_load_state_address() const { return address + 38524; }
  uint32_t title_load_loaded() const { return guest::LoadU32(base, address + 38528); }
  void set_title_load_loaded(uint32_t v) const { guest::StoreU32(base, address + 38528, v); }
  uint32_t title_load_loaded_address() const { return address + 38528; }
  // 1 from Init
  uint32_t title_load_reenter() const { return guest::LoadU32(base, address + 38532); }
  void set_title_load_reenter(uint32_t v) const { guest::StoreU32(base, address + 38532, v); }
  uint32_t title_load_reenter_address() const { return address + 38532; }
  // 0 from Init
  uint32_t title_load_delete_movie() const { return guest::LoadU32(base, address + 38536); }
  void set_title_load_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 38536, v); }
  uint32_t title_load_delete_movie_address() const { return address + 38536; }
  // the title screen's machine (TitleScreen::Init, Type4Machine)
  uint32_t title_screen_address() const { return address + 38560; }
  uint32_t title_screen_state() const { return guest::LoadU32(base, address + 48164); }
  void set_title_screen_state(uint32_t v) const { guest::StoreU32(base, address + 48164, v); }
  uint32_t title_screen_state_address() const { return address + 48164; }
  uint32_t title_screen_loaded() const { return guest::LoadU32(base, address + 48168); }
  void set_title_screen_loaded(uint32_t v) const { guest::StoreU32(base, address + 48168, v); }
  uint32_t title_screen_loaded_address() const { return address + 48168; }
  // 1 from Init
  uint32_t title_screen_reenter() const { return guest::LoadU32(base, address + 48172); }
  void set_title_screen_reenter(uint32_t v) const { guest::StoreU32(base, address + 48172, v); }
  uint32_t title_screen_reenter_address() const { return address + 48172; }
  // 0 from Init
  uint32_t title_screen_delete_movie() const { return guest::LoadU32(base, address + 48176); }
  void set_title_screen_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 48176, v); }
  uint32_t title_screen_delete_movie_address() const { return address + 48176; }
  // the last machine (MachineOf's index, 0 to 4) in state 4 or 5 (FindShown)
  uint32_t shown() const { return guest::LoadU32(base, address + 48200); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 48200, v); }
  uint32_t shown_address() const { return address + 48200; }
  // the next mode's 8-byte answer (NextMode, ForceExit; NextModeAnswer copies it): its first word, the byte 1 and three bytes
  uint32_t next_answer() const { return guest::LoadU32(base, address + 48212); }
  void set_next_answer(uint32_t v) const { guest::StoreU32(base, address + 48212, v); }
  uint32_t next_answer_address() const { return address + 48212; }
  // the answer's next mode type (1, 2, 3, 6, 7, 8, 10, 12; 4 or 12 from ForceExit)
  uint32_t next_type() const { return guest::LoadU32(base, address + 48216); }
  void set_next_type(uint32_t v) const { guest::StoreU32(base, address + 48216, v); }
  uint32_t next_type_address() const { return address + 48216; }
};

// embedded; measure: ModeType4::MachineOf.ret, TitleLoad, TitleScreen::LeavePage.r4
struct Type4Machine : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 Create, 1 OpenMovie, 2 WaitForMovie, 4 to start, 5 running, 7 leaving, 8 unloading, 9 waiting (Reenter), 10 to leave, 11 and up the next mode (Destroy)
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once its movie is loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // open at once: Create goes to 1 (else 9), WaitForMovie to 9 (else 4)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // Unload deletes the movie (and clears `loaded`)
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// allocated; measure: new<68>@TitleLoad::Create
#pragma pack(push, 1)
struct TitleLoadBlockData {
  uint32_t unknown_0 = 0;  // +0
  uint32_t tick_count = 0;  // +4
  uint32_t transition = 0;  // +8
  uint32_t page = 0;  // +12
  uint32_t transition_started = 0;  // +16
  uint8_t pad_20[48] = {};
};
#pragma pack(pop)
static_assert(sizeof(TitleLoadBlockData) == 68);
inline constexpr memory::LayoutField kTitleLoadBlockFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}};
inline constexpr memory::Layout kTitleLoadBlockLayout = {kTitleLoadBlockFields, 5};

struct TitleLoadBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TitleLoadBlockData* data() const { return static_cast<TitleLoadBlockData*>(memory::HandleObject(address)); }
  static TitleLoadBlock New(uint8_t* base) {
    return TitleLoadBlock(base, memory::MakeHandle(new TitleLoadBlockData(), sizeof(TitleLoadBlockData), "TitleLoadBlock", &kTitleLoadBlockLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kTitleLoadBlockLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTitleLoadBlockLayout, offset, width, v); }
  // 0 from Create; not read
  uint32_t unknown_0() const { return data()->unknown_0; }
  void set_unknown_0(uint32_t v) const { data()->unknown_0 = v; }
  uint32_t unknown_0_bits() const { return data()->unknown_0; }
  void set_unknown_0_bits(uint32_t v) const { data()->unknown_0 = v; }
  // the debug sequence's tick count
  uint32_t tick_count() const { return data()->tick_count; }
  void set_tick_count(uint32_t v) const { data()->tick_count = v; }
  uint32_t tick_count_bits() const { return data()->tick_count; }
  void set_tick_count_bits(uint32_t v) const { data()->tick_count = v; }
  // the transition Update runs (-1 none; 12 the logo)
  uint32_t transition() const { return data()->transition; }
  void set_transition(uint32_t v) const { data()->transition = v; }
  uint32_t transition_bits() const { return data()->transition; }
  void set_transition_bits(uint32_t v) const { data()->transition = v; }
  // the page Draw runs (-1 none; 4 posts the close, 5 waits for the archive)
  uint32_t page() const { return data()->page; }
  void set_page(uint32_t v) const { data()->page = v; }
  uint32_t page_bits() const { return data()->page; }
  void set_page_bits(uint32_t v) const { data()->page = v; }
  // the transition's started word
  uint32_t transition_started() const { return data()->transition_started; }
  void set_transition_started(uint32_t v) const { data()->transition_started = v; }
  uint32_t transition_started_bits() const { return data()->transition_started; }
  void set_transition_started_bits(uint32_t v) const { data()->transition_started = v; }
};

// allocated; measure: new<68>@TitleScreen::Create#2
struct TitleScreenData : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  // 0 from Create; not read
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // the main menu's item (0 to 6; 8 the attract race), which picks the next mode (Destroy)
  uint32_t main_item() const { return guest::LoadU32(base, address + 4); }
  void set_main_item(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t main_item_address() const { return address + 4; }
  // player 0's day and garage count nonzero
  uint32_t can_continue() const { return guest::LoadU32(base, address + 8); }
  void set_can_continue(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t can_continue_address() const { return address + 8; }
  // the career menu's third item shown (bit 0x20 of player 0's save +6092)
  uint32_t career_third_item() const { return guest::LoadU32(base, address + 12); }
  void set_career_third_item(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t career_third_item_address() const { return address + 12; }
  // the intro and press start played (from [0x826364EC], Start)
  uint32_t from_boot() const { return guest::LoadU32(base, address + 16); }
  void set_from_boot(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t from_boot_address() const { return address + 16; }
  // the career menu's first selection
  uint32_t career_selection() const { return guest::LoadU32(base, address + 20); }
  void set_career_selection(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t career_selection_address() const { return address + 20; }
  // Menu4's choice
  uint32_t menu4_choice() const { return guest::LoadU32(base, address + 24); }
  void set_menu4_choice(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t menu4_choice_address() const { return address + 24; }
  // item 5 refused (SaveSlots::State(0) 0)
  uint32_t save_item_refused() const { return guest::LoadU32(base, address + 28); }
  void set_save_item_refused(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t save_item_refused_address() const { return address + 28; }
  // item 4's pad state (player 1's controller connected)
  uint32_t pad_item_state() const { return guest::LoadU32(base, address + 32); }
  void set_pad_item_state(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t pad_item_state_address() const { return address + 32; }
  // press start's idle ticks; 600 starts the attract race
  uint32_t idle_ticks() const { return guest::LoadU32(base, address + 36); }
  void set_idle_ticks(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t idle_ticks_address() const { return address + 36; }
  // set by message 17 (OnInterrupt), cleared by 18 (OnResume)
  uint32_t interrupted() const { return guest::LoadU32(base, address + 40); }
  void set_interrupted(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t interrupted_address() const { return address + 40; }
};

// embedded; measure: sub_82222C90.r3, sub_82223050.r3, sub_822231B0.r3
struct TitleTransitionStarted : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 until the transition is set up, then 1
  uint32_t started() const { return guest::LoadU32(base, address + 0); }
  void set_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t started_address() const { return address + 0; }
};

// embedded; measure: new<48220>@ModeType4::Init
struct Type4BlockPart : View {
  using View::View;
  static constexpr uint32_t kSize = 48180;
  uint32_t unknown_9612() const { return guest::LoadU32(base, address + 9612); }
  void set_unknown_9612(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t unknown_9612_address() const { return address + 9612; }
  uint32_t unknown_9616() const { return guest::LoadU32(base, address + 9616); }
  void set_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t unknown_9616_address() const { return address + 9616; }
  uint32_t unknown_19252() const { return guest::LoadU32(base, address + 19252); }
  void set_unknown_19252(uint32_t v) const { guest::StoreU32(base, address + 19252, v); }
  uint32_t unknown_19252_address() const { return address + 19252; }
  uint32_t unknown_19256() const { return guest::LoadU32(base, address + 19256); }
  void set_unknown_19256(uint32_t v) const { guest::StoreU32(base, address + 19256, v); }
  uint32_t unknown_19256_address() const { return address + 19256; }
  uint32_t unknown_28892() const { return guest::LoadU32(base, address + 28892); }
  void set_unknown_28892(uint32_t v) const { guest::StoreU32(base, address + 28892, v); }
  uint32_t unknown_28892_address() const { return address + 28892; }
  uint32_t unknown_28896() const { return guest::LoadU32(base, address + 28896); }
  void set_unknown_28896(uint32_t v) const { guest::StoreU32(base, address + 28896, v); }
  uint32_t unknown_28896_address() const { return address + 28896; }
  uint32_t unknown_38532() const { return guest::LoadU32(base, address + 38532); }
  void set_unknown_38532(uint32_t v) const { guest::StoreU32(base, address + 38532, v); }
  uint32_t unknown_38532_address() const { return address + 38532; }
  uint32_t unknown_38536() const { return guest::LoadU32(base, address + 38536); }
  void set_unknown_38536(uint32_t v) const { guest::StoreU32(base, address + 38536, v); }
  uint32_t unknown_38536_address() const { return address + 38536; }
  uint32_t unknown_48172() const { return guest::LoadU32(base, address + 48172); }
  void set_unknown_48172(uint32_t v) const { guest::StoreU32(base, address + 48172, v); }
  uint32_t unknown_48172_address() const { return address + 48172; }
  uint32_t unknown_48176() const { return guest::LoadU32(base, address + 48176); }
  void set_unknown_48176(uint32_t v) const { guest::StoreU32(base, address + 48176, v); }
  uint32_t unknown_48176_address() const { return address + 48176; }
};

// embedded; measure: new<48220>@ModeType4::Init@+38560
struct Type4BlockAt38560 : View {
  using View::View;
  static constexpr uint32_t kSize = 9608;
  uint32_t unknown_9604() const { return guest::LoadU32(base, address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
};

// The image words docs/functions/title-screen.md's code uses.
struct TitleScreenGlobals {
  template <typename T = TitleLoadBlock>
  static T load_block(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E28u)); }
  static void set_load_block(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E28u, v.address); }
  static void set_load_block(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E28u, v); }
  static constexpr uint32_t load_block_address() { return 0x82814E28u; }
  // the title screen's page object (28 bytes)
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E2Cu)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E2Cu, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E2Cu, v); }
  static constexpr uint32_t pages_address() { return 0x82814E2Cu; }
  // the title screen's menu list (72 bytes)
  template <typename T = MenuList>
  static T list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E30u)); }
  static void set_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E30u, v.address); }
  static void set_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E30u, v); }
  static constexpr uint32_t list_address() { return 0x82814E30u; }
  template <typename T = TitleScreenData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E34u)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E34u, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E34u, v); }
  static constexpr uint32_t data_address() { return 0x82814E34u; }
  // Menu4's menu; Destroy frees it and leaves the pointer
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E38u)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E38u, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E38u, v); }
  static constexpr uint32_t menu_address() { return 0x82814E38u; }
  // how the title was entered: 0 the main menu at once, 1 from the boot (intro and press start), 2 the title load's message 6
  static uint32_t entered(uint8_t* base) { return guest::LoadU32(base, 0x826364ECu); }
  static void set_entered(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826364ECu, v); }
  static constexpr uint32_t entered_address() { return 0x826364ECu; }
  // the mode's four system checks OnResume and SetUpMainMenu clear
  static constexpr uint32_t system_checks_address() { return 0x826BAB40u; }
  // the players' mapper entries (8 bytes: the mapper, +4 blocked when 1)
  static constexpr uint32_t mapper_entries_address() { return 0x826BAB80u; }
  // the players' controllers (a word a player: its mapper entry's index; +16 a fifth slot)
  static constexpr uint32_t controllers_address() { return 0x826BABA4u; }
  // controllers' word 1
  static uint32_t player1_controller(uint8_t* base) { return guest::LoadU32(base, 0x826BABA8u); }
  static void set_player1_controller(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABA8u, v); }
  static constexpr uint32_t player1_controller_address() { return 0x826BABA8u; }
  // the object BootData::Load gets
  static uint32_t boot_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B60u); }
  static void set_boot_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B60u, v); }
  static constexpr uint32_t boot_data_address() { return 0x826C5B60u; }
  // the string table Strings::Load gets
  static uint32_t string_table(uint8_t* base) { return guest::LoadU32(base, 0x826C5C34u); }
  static void set_string_table(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t string_table_address() { return 0x826C5C34u; }
  // the sound manager (slot 14 IsCuePlaying)
  static uint32_t sound_manager(uint8_t* base) { return guest::LoadU32(base, 0x826C5C80u); }
  static void set_sound_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C80u, v); }
  static constexpr uint32_t sound_manager_address() { return 0x826C5C80u; }
  // 1 when a player's controller changes (the profile read asked for)
  static uint32_t profile_read(uint8_t* base) { return guest::LoadU32(base, 0x826C7938u); }
  static void set_profile_read(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7938u, v); }
  static constexpr uint32_t profile_read_address() { return 0x826C7938u; }
  // the pad blocked (known-data.md): either word set
  static uint32_t pad_blocked0(uint8_t* base) { return guest::LoadU32(base, 0x826C7960u); }
  static void set_pad_blocked0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7960u, v); }
  static constexpr uint32_t pad_blocked0_address() { return 0x826C7960u; }
  static uint32_t pad_blocked1(uint8_t* base) { return guest::LoadU32(base, 0x826C7964u); }
  static void set_pad_blocked1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7964u, v); }
  static constexpr uint32_t pad_blocked1_address() { return 0x826C7964u; }
  // the save screen's data (save-screen.toml's; +52 the controller pick OnControllerPicked reads)
  template <typename T = SaveScreenData>
  static T pick_block(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8454u)); }
  static void set_pick_block(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8454u, v.address); }
  static void set_pick_block(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8454u, v); }
  static constexpr uint32_t pick_block_address() { return 0x826D8454u; }
  // the options' settings (options-screen.toml's; +712 read by Interrupt)
  template <typename T = OptionsSettings>
  static T options_settings(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8478u)); }
  static void set_options_settings(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8478u, v.address); }
  static void set_options_settings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8478u, v); }
  static constexpr uint32_t options_settings_address() { return 0x826D8478u; }
  // the passes word ScreenFades::FadeIn gets
  static uint32_t fades_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fades_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fades_passes_address() { return 0x82001BECu; }
  // the cursor's position MoveMenu4Cursor copies when the menu wraps up
  static uint32_t menu4_wrap_up(uint8_t* base) { return guest::LoadU32(base, 0x82001FB0u); }
  static void set_menu4_wrap_up(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001FB0u, v); }
  static constexpr uint32_t menu4_wrap_up_address() { return 0x82001FB0u; }
  // its position when the menu wraps down
  static uint32_t menu4_wrap_down(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_menu4_wrap_down(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t menu4_wrap_down_address() { return 0x82000A34u; }
};

}  // namespace schema
