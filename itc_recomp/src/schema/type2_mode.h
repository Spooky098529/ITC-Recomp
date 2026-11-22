// The views of docs/functions/type2-mode.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type2-mode.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ModeType2, ModeType2::AfterLeaveScreen.r4, ModeType2::LeaveScreen.r4, ModeType2::Leaving.r4, ModeType2::NextMode.r4, ModeType2::Restart.r4, ModeType2::TearDown.r4, ModeType2::WaitForLoads.r4
#pragma pack(push, 1)
struct ModeType2Data {
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
  uint8_t pad_240[568] = {};
  uint32_t iface_ring_count = 0;  // +808
  uint32_t iface_ring_start = 0;  // +812
  uint32_t iface_block = 0;  // +816
  uint8_t pad_820[20] = {};
  uint32_t ring_count = 0;  // +840
  uint32_t ring_start = 0;  // +844
  uint32_t block = 0;  // +848
};
#pragma pack(pop)
static_assert(sizeof(ModeType2Data) == 852);
inline constexpr memory::LayoutField kModeType2Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {808, 4}, {812, 4}, {816, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kModeType2Layout = {kModeType2Fields, 14};

struct ModeType2 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x82006B9C;
  ModeType2Data* data() const { return static_cast<ModeType2Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeType2 New(uint8_t* base) {
    return ModeType2(base, memory::MakeHandle(new ModeType2Data(), sizeof(ModeType2Data), "ModeType2", &kModeType2Layout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeType2Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType2Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006B9C (ModeType2Vtable)
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), not the mode's
  uint8_t answer_changed_bits() const { return on_host() ? uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType2Layout, 0, 1)) : guest::LoadU8(base, address + 0); }
  void set_answer_changed_bits(uint8_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType2Layout, 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the next-mode answer's first word (ModeAnswer; TearDown copies the block's into it), not the mode's
  uint32_t answer_word0_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType2Layout, 0, 4)) : guest::LoadU32(base, address + 0); }
  void set_answer_word0_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType2Layout, 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t answer_word0() const { return uint32_t(answer_word0_bits()); }
  void set_answer_word0(uint32_t v) const { set_answer_word0_bits(uint32_t(v)); }
  // the delegate at +8 (also the answer's next type +4 by the measure's join)
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, on_host() ? data()->delegate_ptr : guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  void set_delegate_ptr(uint32_t v) const { if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_bits() const { return on_host() ? data()->delegate_ptr : guest::LoadU32(base, address + 4); }
  void set_delegate_ptr_bits(uint32_t v) const { if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  // the delegate (ModeDelegate): vtable 0x82010FA0
  uint32_t delegate_vtable() const { return on_host() ? uint32_t(data()->delegate_vtable) : uint32_t(guest::LoadU32(base, address + 8)); }
  void set_delegate_vtable(uint32_t v) const { if (on_host()) data()->delegate_vtable = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable_bits() const { return on_host() ? data()->delegate_vtable : guest::LoadU32(base, address + 8); }
  void set_delegate_vtable_bits(uint32_t v) const { if (on_host()) data()->delegate_vtable = v; else guest::StoreU32(base, address + 8, v); }
  // the delegate's object: the mode
  uint32_t delegate_object() const { return on_host() ? uint32_t(data()->delegate_object) : uint32_t(guest::LoadU32(base, address + 12)); }
  void set_delegate_object(uint32_t v) const { if (on_host()) data()->delegate_object = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object_bits() const { return on_host() ? data()->delegate_object : guest::LoadU32(base, address + 12); }
  void set_delegate_object_bits(uint32_t v) const { if (on_host()) data()->delegate_object = v; else guest::StoreU32(base, address + 12, v); }
  // a slot thunk: Mode::Slot9Thunk (WaitForLoads) after Init, Mode::NextModeThunk 0x82235C08 once loaded, Slot10Thunk to 13
  uint32_t delegate_function() const { return on_host() ? uint32_t(data()->delegate_function) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_delegate_function(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_bits() const { return on_host() ? data()->delegate_function : guest::LoadU32(base, address + 16); }
  void set_delegate_function_bits(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_adjustment() const { return on_host() ? uint32_t(data()->delegate_adjustment) : uint32_t(guest::LoadU32(base, address + 20)); }
  void set_delegate_adjustment(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_bits() const { return on_host() ? data()->delegate_adjustment : guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  // the +32 interface's vtable 0x82006B90 (Interrupt, ForceExit, Resume)
  uint32_t vtable32() const { return on_host() ? uint32_t(data()->vtable32) : uint32_t(guest::LoadU32(base, address + 32)); }
  void set_vtable32(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_bits() const { return on_host() ? data()->vtable32 : guest::LoadU32(base, address + 32); }
  void set_vtable32_bits(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers, the block's 14 machines
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
  // measure fault: ring_count (+840) through the +32 interface (Interrupt)
  uint32_t iface_ring_count() const { return on_host() ? uint32_t(data()->iface_ring_count) : uint32_t(guest::LoadU32(base, address + 808)); }
  void set_iface_ring_count(uint32_t v) const { if (on_host()) data()->iface_ring_count = v; else guest::StoreU32(base, address + 808, v); }
  uint32_t iface_ring_count_bits() const { return on_host() ? data()->iface_ring_count : guest::LoadU32(base, address + 808); }
  void set_iface_ring_count_bits(uint32_t v) const { if (on_host()) data()->iface_ring_count = v; else guest::StoreU32(base, address + 808, v); }
  // measure fault: ring_start (+844) through the +32 interface (Interrupt)
  uint32_t iface_ring_start() const { return on_host() ? uint32_t(data()->iface_ring_start) : uint32_t(guest::LoadU32(base, address + 812)); }
  void set_iface_ring_start(uint32_t v) const { if (on_host()) data()->iface_ring_start = v; else guest::StoreU32(base, address + 812, v); }
  uint32_t iface_ring_start_bits() const { return on_host() ? data()->iface_ring_start : guest::LoadU32(base, address + 812); }
  void set_iface_ring_start_bits(uint32_t v) const { if (on_host()) data()->iface_ring_start = v; else guest::StoreU32(base, address + 812, v); }
  // measure fault: block (+848) through the +32 interface (Interrupt, ForceExit)
  uint32_t iface_block() const { return on_host() ? uint32_t(data()->iface_block) : uint32_t(guest::LoadU32(base, address + 816)); }
  void set_iface_block(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t iface_block_bits() const { return on_host() ? data()->iface_block : guest::LoadU32(base, address + 816); }
  void set_iface_block_bits(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  // VirtualAlloc(0, 162688, 0x1000, 4) by Init, freed by ModeType2::Release
  template <typename T = Type2ModeBlock>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 848)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 848); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
};

// allocated; measure: new<162688>@ModeType2::Init, ModeType2::TearDown.r4.+848, ModeType2::WaitForLoads.r4.+848, ModeType2::NextMode.r4.+848, ModeType2.+816
struct Type2ModeBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 162688;
  // Type2Hub's machine (type2-screens.md); MachineOf 0
  uint32_t hub_address() const { return address + 0; }
  // the hub's machine state (14 picks the next mode 4)
  uint32_t hub_state() const { return guest::LoadU32(base, address + 9604); }
  void set_hub_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t hub_state_address() const { return address + 9604; }
  // the hub's movie loaded (AllLoaded)
  uint32_t hub_loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_hub_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t hub_loaded_address() const { return address + 9608; }
  // open on its message: 1 from Init
  uint32_t hub_reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_hub_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t hub_reenter_address() const { return address + 9612; }
  // 0 from Init
  uint32_t hub_delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_hub_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t hub_delete_movie_address() const { return address + 9616; }
  // RaceMenu (race-menu.md); MachineOf 1
  uint32_t race_menu_address() const { return address + 9640; }
  // the tuning screen (tuning-machine.md); MachineOf 2
  uint32_t tuning_machine_address() const { return address + 19792; }
  // PowerGraphScreen2, the spec sheet (type2-screens.md); MachineOf 9
  uint32_t spec_sheet_address() const { return address + 56624; }
  // UserInfo (user-info.md); MachineOf 10
  uint32_t user_info_address() const { return address + 66264; }
  // Type2Options (options-screen.md); MachineOf 7
  uint32_t options_address() const { return address + 75904; }
  // Type2ControllerOptions (options-screen.md); MachineOf 12
  uint32_t controller_options_address() const { return address + 85544; }
  // CarSettings (car-settings.md); MachineOf 3
  uint32_t car_settings_address() const { return address + 95184; }
  // CarDealer (car-dealer.md); MachineOf 4; its state 14 picks the next mode 4
  uint32_t car_dealer_address() const { return address + 104824; }
  // BuildScreen (type2-screens.md); MachineOf 6
  uint32_t build_screen_address() const { return address + 114464; }
  // Type2Dialog (type2-dialog.md); MachineOf 16
  uint32_t dialog_address() const { return address + 124104; }
  // PlateEditor (type2-screens.md); MachineOf 26
  uint32_t plate_editor_address() const { return address + 133744; }
  // GarageScreen (garage-screen.md); MachineOf 5
  uint32_t garage_screen_address() const { return address + 143384; }
  // Type2SaveScreen (save-screen.md); MachineOf 44
  uint32_t save_screen_address() const { return address + 153024; }
  // the save screen's machine state (+0x255C0 + 9604; Interrupt: 5)
  uint32_t save_screen_state() const { return guest::LoadU32(base, address + 162628); }
  void set_save_screen_state(uint32_t v) const { guest::StoreU32(base, address + 162628, v); }
  uint32_t save_screen_state_address() const { return address + 162628; }
  // the save screen's +9608 (+0x255C0 + 9608), the last word AllLoaded waits on
  uint32_t save_screen_loaded() const { return guest::LoadU32(base, address + 162632); }
  void set_save_screen_loaded(uint32_t v) const { guest::StoreU32(base, address + 162632, v); }
  uint32_t save_screen_loaded_address() const { return address + 162632; }
  // the shown screen's index (0 to 7, FindShown; MachineOf's index)
  uint32_t shown() const { return guest::LoadU32(base, address + 162664); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 162664, v); }
  uint32_t shown_address() const { return address + 162664; }
  // Init's first screen: 0 the hub, 3 the car settings, 4 the car dealer
  uint32_t first_shown() const { return guest::LoadU32(base, address + 162668); }
  void set_first_shown(uint32_t v) const { guest::StoreU32(base, address + 162668, v); }
  uint32_t first_shown_address() const { return address + 162668; }
  // the next mode's 8-byte answer (ModeAnswer) TearDown returns: byte 1
  uint8_t answer_changed() const { return guest::LoadU8(base, address + 162676); }
  void set_answer_changed(uint8_t v) const { guest::StoreU8(base, address + 162676, v); }
  uint32_t answer_changed_address() const { return address + 162676; }
  // the answer's first word as copied: byte 1 and three frame bytes
  uint32_t answer_word0() const { return guest::LoadU32(base, address + 162676); }
  void set_answer_word0(uint32_t v) const { guest::StoreU32(base, address + 162676, v); }
  uint32_t answer_word0_address() const { return address + 162676; }
  // the next mode: 1 the race, 3, 4, 5 the parking area or 12
  uint32_t answer_next_type() const { return guest::LoadU32(base, address + 162680); }
  void set_answer_next_type(uint32_t v) const { guest::StoreU32(base, address + 162680, v); }
  uint32_t answer_next_type_address() const { return address + 162680; }
};

// embedded; measure: ModeType2::MachineOf.ret
struct Type2ScreenMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 4 starting, 5 running, 8 a leave asked, 10 leave all, 11, 12, 14, 20 pick the next mode; 6 done
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // the movie loaded (each machine's WaitForLoad); AllLoaded waits on it
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // open on its message: 1 from ModeType2::Init
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // delete the movies on unload: 0 from ModeType2::Init
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// embedded; measure: Mode::Slot8Thunk.r4, Mode::Slot9Thunk.r4, Mode::Slot10Thunk.r4, Mode::Slot11Thunk.r4, Mode::Slot12Thunk.r4, Mode::Slot13Thunk.r4
#pragma pack(push, 1)
struct ModeSlotObjectData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(ModeSlotObjectData) == 4);
inline constexpr memory::LayoutField kModeSlotObjectFields[] = {{0, 4}};
inline constexpr memory::Layout kModeSlotObjectLayout = {kModeSlotObjectFields, 1};

struct ModeSlotObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ModeSlotObjectData* data() const { return static_cast<ModeSlotObjectData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeSlotObject New(uint8_t* base) {
    return ModeSlotObject(base, memory::MakeHandle(new ModeSlotObjectData(), sizeof(ModeSlotObjectData), "ModeSlotObject", &kModeSlotObjectLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeSlotObjectLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeSlotObjectLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006B9C for the type 2 mode
  template <typename T = ModeType2Vtable>
  T vtable() const { return T(base, on_host() ? data()->vtable : guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
};

// image; measure: Mode::Slot8Thunk.r4.+0, Mode::Slot9Thunk.r4.+0, Mode::Slot10Thunk.r4.+0, Mode::Slot11Thunk.r4.+0, Mode::Slot12Thunk.r4.+0, Mode::Slot13Thunk.r4.+0
struct ModeType2Vtable : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // slot 0: ModeType2::Init
  uint32_t init() const { return guest::LoadU32(base, address + 0); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t init_address() const { return address + 0; }
  // slot 1: ModeType2::Release
  uint32_t release() const { return guest::LoadU32(base, address + 4); }
  void set_release(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t release_address() const { return address + 4; }
  // slots 2 to 5: Mode::NoNextMode
  uint32_t slot2() const { return guest::LoadU32(base, address + 8); }
  void set_slot2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot2_address() const { return address + 8; }
  uint32_t slot3() const { return guest::LoadU32(base, address + 12); }
  void set_slot3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot3_address() const { return address + 12; }
  uint32_t slot4() const { return guest::LoadU32(base, address + 16); }
  void set_slot4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot4_address() const { return address + 16; }
  uint32_t slot5() const { return guest::LoadU32(base, address + 20); }
  void set_slot5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot5_address() const { return address + 20; }
  // slot 6: ModeType2::NextMode
  uint32_t next_mode() const { return guest::LoadU32(base, address + 24); }
  void set_next_mode(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t next_mode_address() const { return address + 24; }
  // slot 7: Mode::DeletingDestructor
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 28); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t deleting_destructor_address() const { return address + 28; }
  // slot 8: ModeType2::Restart (Mode::Slot8Thunk)
  uint32_t restart() const { return guest::LoadU32(base, address + 32); }
  void set_restart(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t restart_address() const { return address + 32; }
  // slot 9: ModeType2::WaitForLoads (Mode::Slot9Thunk)
  uint32_t wait_for_loads() const { return guest::LoadU32(base, address + 36); }
  void set_wait_for_loads(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t wait_for_loads_address() const { return address + 36; }
  // slot 10: ModeType2::LeaveScreen (Mode::Slot10Thunk)
  uint32_t leave_screen() const { return guest::LoadU32(base, address + 40); }
  void set_leave_screen(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t leave_screen_address() const { return address + 40; }
  // slot 11: ModeType2::TearDown (Mode::Slot11Thunk)
  uint32_t tear_down() const { return guest::LoadU32(base, address + 44); }
  void set_tear_down(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t tear_down_address() const { return address + 44; }
  // slot 12: ModeType2::AfterLeaveScreen (Mode::Slot12Thunk)
  uint32_t after_leave_screen() const { return guest::LoadU32(base, address + 48); }
  void set_after_leave_screen(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t after_leave_screen_address() const { return address + 48; }
  // slot 13: ModeType2::Leaving (Mode::Slot13Thunk)
  uint32_t leaving() const { return guest::LoadU32(base, address + 52); }
  void set_leaving(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t leaving_address() const { return address + 52; }
};

// The image words docs/functions/type2-mode.md's code uses.
struct Type2ModeGlobals {
  // the pad blocked (known-data.md): NextMode queues no Draw when either word is set
  static uint32_t pad_blocked0(uint8_t* base) { return guest::LoadU32(base, 0x826C7960u); }
  static void set_pad_blocked0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7960u, v); }
  static constexpr uint32_t pad_blocked0_address() { return 0x826C7960u; }
  static uint32_t pad_blocked1(uint8_t* base) { return guest::LoadU32(base, 0x826C7964u); }
  static void set_pad_blocked1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7964u, v); }
  static constexpr uint32_t pad_blocked1_address() { return 0x826C7964u; }
  // 1 from ForceExit (0 or 1), 0 from NextMode when the hub or the car dealer picks mode 4
  static uint32_t leaving_flag(uint8_t* base) { return guest::LoadU32(base, 0x826364ECu); }
  static void set_leaving_flag(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826364ECu, v); }
  static constexpr uint32_t leaving_flag_address() { return 0x826364ECu; }
  // the mode before (1 the race): Init's first screen and Career::NextDay
  static uint32_t mode_before(uint8_t* base) { return guest::LoadU32(base, 0x826032E0u); }
  static void set_mode_before(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032E0u, v); }
  static constexpr uint32_t mode_before_address() { return 0x826032E0u; }
  // the race data (race-states.toml's RaceData): Init reads its race_mode +0xA3AF4
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the options' settings (options-screen.toml's OptionsSettings): Interrupt reads its save_page_closed +712
  template <typename T = OptionsSettings>
  static T options_settings(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8478u)); }
  static void set_options_settings(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8478u, v.address); }
  static void set_options_settings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8478u, v); }
  static constexpr uint32_t options_settings_address() { return 0x826D8478u; }
  // the type 2 dialog's status (type2-dialog.toml's status): Interrupt answers 1 at 3
  static uint32_t dialog_status(uint8_t* base) { return guest::LoadU32(base, 0x82636204u); }
  static void set_dialog_status(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82636204u, v); }
  static constexpr uint32_t dialog_status_address() { return 0x82636204u; }
  // the string table Strings::Load gets
  static uint32_t string_table(uint8_t* base) { return guest::LoadU32(base, 0x826C5C34u); }
  static void set_string_table(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t string_table_address() { return 0x826C5C34u; }
  // the passes word ScreenFades::FadeIn is given
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
};

}  // namespace schema
