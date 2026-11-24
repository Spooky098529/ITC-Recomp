// The views of docs/functions/type5-mode.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type5-mode.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ModeType5, ModeType5::LeaveScreen.r4, ModeType5::Leaving.r4, ModeType5::NextMode.r4, ModeType5::WaitForLoads.r4, ModeType5::WaitForShown.r4, FrontEndMode::NextMode.r4
#pragma pack(push, 1)
struct ModeType5Data {
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
  uint8_t pad_240[576] = {};
  uint32_t iface_block = 0;  // +816
  uint8_t pad_820[20] = {};
  uint32_t ring_count = 0;  // +840
  uint32_t ring_start = 0;  // +844
  uint32_t block = 0;  // +848
};
#pragma pack(pop)
static_assert(sizeof(ModeType5Data) == 852);
inline constexpr memory::LayoutField kModeType5Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {816, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kModeType5Layout = {kModeType5Fields, 12};

struct ModeType5 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x82006C24;
  ModeType5Data* data() const { return static_cast<ModeType5Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeType5 New(uint8_t* base) {
    return ModeType5(base, memory::MakeHandle(new ModeType5Data(), sizeof(ModeType5Data), "ModeType5", &kModeType5Layout));
  }
  template <uint32_t Stride = kSize>
  static ModeType5 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeType5Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType5Data*>(::operator new(sizeof(ModeType5Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType5Data();
    return ModeType5(base, memory::MakeHandle(p, sizeof(ModeType5Data) * count, "ModeType5", &kModeType5Layout, sizeof(ModeType5Data)));
  }
  ModeType5 at(uint32_t i) const { return ModeType5(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeType5Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType5Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006C24 (Type5ModeVtable)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer; the delegates clear it), not the mode's
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { return on_host() ? uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType5Layout, 0, 1)) : guest::LoadU8(base, address + 0); }
  void set_answer_changed_bits(uint8_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType5Layout, 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // the delegate at +8, 0 when it wraps to 0 (also the answer's next type +4 by the measure's join)
  uint32_t delegate_ptr_address() const { return address + 4; }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, on_host() ? data()->delegate_ptr : guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  void set_delegate_ptr(uint32_t v) const { if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_bits() const { return on_host() ? data()->delegate_ptr : guest::LoadU32(base, address + 4); }
  void set_delegate_ptr_bits(uint32_t v) const { if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  // the delegate (ModeDelegate): vtable 0x82010FA0
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable() const { return on_host() ? uint32_t(data()->delegate_vtable) : uint32_t(guest::LoadU32(base, address + 8)); }
  void set_delegate_vtable(uint32_t v) const { if (on_host()) data()->delegate_vtable = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable_bits() const { return on_host() ? data()->delegate_vtable : guest::LoadU32(base, address + 8); }
  void set_delegate_vtable_bits(uint32_t v) const { if (on_host()) data()->delegate_vtable = v; else guest::StoreU32(base, address + 8, v); }
  // the delegate's object: the mode
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object() const { return on_host() ? uint32_t(data()->delegate_object) : uint32_t(guest::LoadU32(base, address + 12)); }
  void set_delegate_object(uint32_t v) const { if (on_host()) data()->delegate_object = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object_bits() const { return on_host() ? data()->delegate_object : guest::LoadU32(base, address + 12); }
  void set_delegate_object_bits(uint32_t v) const { if (on_host()) data()->delegate_object = v; else guest::StoreU32(base, address + 12, v); }
  // a slot thunk: Mode::Slot8Thunk (WaitForLoads) after Init, Mode::NextModeThunk 0x82235C08 once loaded, Slot11Thunk to Slot14Thunk
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function() const { return on_host() ? uint32_t(data()->delegate_function) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_delegate_function(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_bits() const { return on_host() ? data()->delegate_function : guest::LoadU32(base, address + 16); }
  void set_delegate_function_bits(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment() const { return on_host() ? uint32_t(data()->delegate_adjustment) : uint32_t(guest::LoadU32(base, address + 20)); }
  void set_delegate_adjustment(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_bits() const { return on_host() ? data()->delegate_adjustment : guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  // the +32 interface's vtable 0x82006C18 (Interrupt, ForceExit, Resume)
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32() const { return on_host() ? uint32_t(data()->vtable32) : uint32_t(guest::LoadU32(base, address + 32)); }
  void set_vtable32(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_bits() const { return on_host() ? data()->vtable32 : guest::LoadU32(base, address + 32); }
  void set_vtable32_bits(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers, the block's three machines (+9640, +0, +19280)
  uint32_t handlers_address() const { return address + 36; }
  uint8_t* handlers() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 36 : guest::Pointer(base, address + 36); }
  uint32_t handler_count_address() const { return address + 236; }
  uint32_t handler_count() const { return on_host() ? uint32_t(data()->handler_count) : uint32_t(guest::LoadU32(base, address + 236)); }
  void set_handler_count(uint32_t v) const { if (on_host()) data()->handler_count = v; else guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_bits() const { return on_host() ? data()->handler_count : guest::LoadU32(base, address + 236); }
  void set_handler_count_bits(uint32_t v) const { if (on_host()) data()->handler_count = v; else guest::StoreU32(base, address + 236, v); }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint8_t* ring() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 240 : guest::Pointer(base, address + 240); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_count() const { return on_host() ? uint32_t(data()->ring_count) : uint32_t(guest::LoadU32(base, address + 840)); }
  void set_ring_count(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_bits() const { return on_host() ? data()->ring_count : guest::LoadU32(base, address + 840); }
  void set_ring_count_bits(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_start_address() const { return address + 844; }
  uint32_t ring_start() const { return on_host() ? uint32_t(data()->ring_start) : uint32_t(guest::LoadU32(base, address + 844)); }
  void set_ring_start(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_bits() const { return on_host() ? data()->ring_start : guest::LoadU32(base, address + 844); }
  void set_ring_start_bits(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 844, v); }
  // measure fault: block (+848) through the +32 interface (Interrupt, ForceExit)
  uint32_t iface_block_address() const { return address + 816; }
  uint32_t iface_block() const { return on_host() ? uint32_t(data()->iface_block) : uint32_t(guest::LoadU32(base, address + 816)); }
  void set_iface_block(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t iface_block_bits() const { return on_host() ? data()->iface_block : guest::LoadU32(base, address + 816); }
  void set_iface_block_bits(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  // VirtualAlloc(0, 28940, 0x1000, 4) by Init, freed by ModeType5::Release
  uint32_t block_address() const { return address + 848; }
  template <typename T = Type5ModeBlock>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 848)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 848); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
};

// allocated; measure: ModeType5.+816, ModeType5::NextMode.r4.+848, ModeType5::WaitForLoads.r4.+848, ModeType5::Leaving.r4.+848, FrontEndMode::NextMode.r4.+848
struct Type5ModeBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 28940;
  // the car settings' machine (car-settings.md, CarSettings::InitType5); id 3
  uint32_t car_settings_address() const { return address + 0; }
  // the car settings' machine state
  uint32_t car_settings_state() const { return guest::LoadU32(base, address + 9604); }
  void set_car_settings_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t car_settings_state_address() const { return address + 9604; }
  // its movies loaded (AllLoaded)
  uint32_t car_settings_loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_car_settings_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t car_settings_loaded_address() const { return address + 9608; }
  // open on its message: 1 from Init
  uint32_t car_settings_reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_car_settings_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t car_settings_reenter_address() const { return address + 9612; }
  // 0 from Init
  uint32_t car_settings_delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_car_settings_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t car_settings_delete_movie_address() const { return address + 9616; }
  // the parking area's machine (location-screen.md's LocationScreenMachine); id 21
  uint32_t location_screen_address() const { return address + 9640; }
  // the parking area's machine state: 8 a leave asked, 10 leave (message 4), 11 a race set up, 13 today's result
  uint32_t location_state() const { return guest::LoadU32(base, address + 19244); }
  void set_location_state(uint32_t v) const { guest::StoreU32(base, address + 19244, v); }
  uint32_t location_state_address() const { return address + 19244; }
  // its movies loaded (AllLoaded)
  uint32_t location_loaded() const { return guest::LoadU32(base, address + 19248); }
  void set_location_loaded(uint32_t v) const { guest::StoreU32(base, address + 19248, v); }
  uint32_t location_loaded_address() const { return address + 19248; }
  // open on its message: 1 from Init
  uint32_t location_reenter() const { return guest::LoadU32(base, address + 19252); }
  void set_location_reenter(uint32_t v) const { guest::StoreU32(base, address + 19252, v); }
  uint32_t location_reenter_address() const { return address + 19252; }
  // 0 from Init
  uint32_t location_delete_movie() const { return guest::LoadU32(base, address + 19256); }
  void set_location_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 19256, v); }
  uint32_t location_delete_movie_address() const { return address + 19256; }
  // the mode's dialog's machine (Type5Dialog); id 16
  uint32_t dialog_address() const { return address + 19280; }
  // the dialog's machine state (9 idle: Interrupt answers 1)
  uint32_t dialog_state() const { return guest::LoadU32(base, address + 28884); }
  void set_dialog_state(uint32_t v) const { guest::StoreU32(base, address + 28884, v); }
  uint32_t dialog_state_address() const { return address + 28884; }
  // its movies loaded (AllLoaded)
  uint32_t dialog_loaded() const { return guest::LoadU32(base, address + 28888); }
  void set_dialog_loaded(uint32_t v) const { guest::StoreU32(base, address + 28888, v); }
  uint32_t dialog_loaded_address() const { return address + 28888; }
  // open on its message: 1 from Init
  uint32_t dialog_reenter() const { return guest::LoadU32(base, address + 28892); }
  void set_dialog_reenter(uint32_t v) const { guest::StoreU32(base, address + 28892, v); }
  uint32_t dialog_reenter_address() const { return address + 28892; }
  // 0 from Init
  uint32_t dialog_delete_movie() const { return guest::LoadU32(base, address + 28896); }
  void set_dialog_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 28896, v); }
  uint32_t dialog_delete_movie_address() const { return address + 28896; }
  // the shown machine's id (3, 16 or 21; FindShown; 21 at Init)
  uint32_t shown() const { return guest::LoadU32(base, address + 28920); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 28920, v); }
  uint32_t shown_address() const { return address + 28920; }
  // 21 at Init (readers not looked for)
  uint32_t first_shown() const { return guest::LoadU32(base, address + 28924); }
  void set_first_shown(uint32_t v) const { guest::StoreU32(base, address + 28924, v); }
  uint32_t first_shown_address() const { return address + 28924; }
  // the next mode's 8-byte answer (ModeAnswer), written whole by NextMode and ForceExit
  uint64_t answer() const { return guest::LoadU64(base, address + 28928); }
  void set_answer(uint64_t v) const { guest::StoreU64(base, address + 28928, v); }
  uint32_t answer_address() const { return address + 28928; }
  // the answer's first word: its changed byte and three bytes
  uint32_t answer_word0() const { return guest::LoadU32(base, address + 28928); }
  void set_answer_word0(uint32_t v) const { guest::StoreU32(base, address + 28928, v); }
  uint32_t answer_word0_address() const { return address + 28928; }
  // the answer's next mode type (1 the race, 9 today's result, 4 the title, 12)
  uint32_t answer_next_type() const { return guest::LoadU32(base, address + 28932); }
  void set_answer_next_type(uint32_t v) const { guest::StoreU32(base, address + 28932, v); }
  uint32_t answer_next_type_address() const { return address + 28932; }
};

// embedded; measure: Type5Dialog
struct Type5Dialog : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  // measure fault: the steps' r3, the page object's step_started (+24), not the machine's
  uint32_t fault_step_started() const { return guest::LoadU32(base, address + 0); }
  void set_fault_step_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_step_started_address() const { return address + 0; }
  // measure fault: the pages' r3 is the dispatcher (ModeDispatcher's ring_count), not the machine
  uint32_t fault_ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_fault_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t fault_ring_count_address() const { return address + 804; }
  // measure fault: the dispatcher's ring_start, as above
  uint32_t fault_ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_fault_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t fault_ring_start_address() const { return address + 808; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 enter, 1 load the movies, 2 wait for them, 4 a dialog opening, 9 idle
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movies 16 to 19 loaded (WaitForMovies)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // open on its message: 1 from ModeType5::Init (OnEnter goes to 1, else 9)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // 0 from ModeType5::Init
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// allocated; measure: new<68>@Type5Dialog::OnEnter#2
#pragma pack(push, 1)
struct Type5DialogDataData {
  uint32_t movies_loaded = 0;  // +0
  uint32_t queued_state = 0;  // +4
  uint32_t movie_shown = 0;  // +8
  uint32_t interrupted = 0;  // +12
  uint8_t pad_16[52] = {};
};
#pragma pack(pop)
static_assert(sizeof(Type5DialogDataData) == 68);
inline constexpr memory::LayoutField kType5DialogDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kType5DialogDataLayout = {kType5DialogDataFields, 4};

struct Type5DialogData : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  Type5DialogDataData* data() const { return static_cast<Type5DialogDataData*>(memory::HandleObject(address)); }
  static Type5DialogData New(uint8_t* base) {
    return Type5DialogData(base, memory::MakeHandle(new Type5DialogDataData(), sizeof(Type5DialogDataData), "Type5DialogData", &kType5DialogDataLayout));
  }
  template <uint32_t Stride = kSize>
  static Type5DialogData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(Type5DialogDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Type5DialogDataData*>(::operator new(sizeof(Type5DialogDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Type5DialogDataData();
    return Type5DialogData(base, memory::MakeHandle(p, sizeof(Type5DialogDataData) * count, "Type5DialogData", &kType5DialogDataLayout, sizeof(Type5DialogDataData)));
  }
  Type5DialogData at(uint32_t i) const { return Type5DialogData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kType5DialogDataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType5DialogDataLayout, offset, width, v); }
  // 1 once movies 16 to 19 are loaded
  uint32_t movies_loaded_address() const { return address + 0; }
  uint32_t movies_loaded() const { return data()->movies_loaded; }
  void set_movies_loaded(uint32_t v) const { data()->movies_loaded = v; }
  uint32_t movies_loaded_bits() const { return data()->movies_loaded; }
  void set_movies_loaded_bits(uint32_t v) const { data()->movies_loaded = v; }
  // the machine's state queued by the close steps (9); 21 none
  uint32_t queued_state_address() const { return address + 4; }
  uint32_t queued_state() const { return data()->queued_state; }
  void set_queued_state(uint32_t v) const { data()->queued_state = v; }
  uint32_t queued_state_bits() const { return data()->queued_state; }
  void set_queued_state_bits(uint32_t v) const { data()->queued_state = v; }
  // the movie shown, 16 to 19
  uint32_t movie_shown_address() const { return address + 8; }
  uint32_t movie_shown() const { return data()->movie_shown; }
  void set_movie_shown(uint32_t v) const { data()->movie_shown = v; }
  uint32_t movie_shown_bits() const { return data()->movie_shown; }
  void set_movie_shown_bits(uint32_t v) const { data()->movie_shown = v; }
  // set by message 18 (OnInterrupt); the pages close with message 20
  uint32_t interrupted_address() const { return address + 12; }
  uint32_t interrupted() const { return data()->interrupted; }
  void set_interrupted(uint32_t v) const { data()->interrupted = v; }
  uint32_t interrupted_bits() const { return data()->interrupted; }
  void set_interrupted_bits(uint32_t v) const { data()->interrupted = v; }
};

// embedded; measure: Mode::Slot14Thunk.r4
#pragma pack(push, 1)
struct Type5SlotObjectData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(Type5SlotObjectData) == 4);
inline constexpr memory::LayoutField kType5SlotObjectFields[] = {{0, 4}};
inline constexpr memory::Layout kType5SlotObjectLayout = {kType5SlotObjectFields, 1};

struct Type5SlotObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  Type5SlotObjectData* data() const { return static_cast<Type5SlotObjectData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static Type5SlotObject New(uint8_t* base) {
    return Type5SlotObject(base, memory::MakeHandle(new Type5SlotObjectData(), sizeof(Type5SlotObjectData), "Type5SlotObject", &kType5SlotObjectLayout));
  }
  template <uint32_t Stride = kSize>
  static Type5SlotObject NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(Type5SlotObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Type5SlotObjectData*>(::operator new(sizeof(Type5SlotObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Type5SlotObjectData();
    return Type5SlotObject(base, memory::MakeHandle(p, sizeof(Type5SlotObjectData) * count, "Type5SlotObject", &kType5SlotObjectLayout, sizeof(Type5SlotObjectData)));
  }
  Type5SlotObject at(uint32_t i) const { return Type5SlotObject(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kType5SlotObjectLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType5SlotObjectLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006C24
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = Type5ModeVtable>
  T vtable() const { return T(base, on_host() ? data()->vtable : guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
};

// image; measure: Mode::Slot14Thunk.r4.+0
struct Type5ModeVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // slot 0: ModeType5::Init
  uint32_t init() const { return guest::LoadU32(base, address + 0); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t init_address() const { return address + 0; }
  // slot 1: ModeType5::Release
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
  // slot 6: ModeType5::NextMode
  uint32_t next_mode() const { return guest::LoadU32(base, address + 24); }
  void set_next_mode(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t next_mode_address() const { return address + 24; }
  // slot 7: Mode::DeletingDestructor
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 28); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t deleting_destructor_address() const { return address + 28; }
  // slot 8: ModeType5::WaitForLoads (Mode::Slot8Thunk)
  uint32_t wait_for_loads() const { return guest::LoadU32(base, address + 32); }
  void set_wait_for_loads(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t wait_for_loads_address() const { return address + 32; }
  // slots 9 and 10: Mode::NoNextMode
  uint32_t slot9() const { return guest::LoadU32(base, address + 36); }
  void set_slot9(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t slot9_address() const { return address + 36; }
  uint32_t slot10() const { return guest::LoadU32(base, address + 40); }
  void set_slot10(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t slot10_address() const { return address + 40; }
  // slot 11: ModeType5::WaitForShown (Mode::Slot11Thunk)
  uint32_t wait_for_shown() const { return guest::LoadU32(base, address + 44); }
  void set_wait_for_shown(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t wait_for_shown_address() const { return address + 44; }
  // slot 12: ModeType5::LeaveScreen (Mode::Slot12Thunk)
  uint32_t leave_screen() const { return guest::LoadU32(base, address + 48); }
  void set_leave_screen(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t leave_screen_address() const { return address + 48; }
  // slot 13: FrontEndMode::NextMode sub_8222FCA0 (Mode::Slot13Thunk)
  uint32_t next_mode_answer() const { return guest::LoadU32(base, address + 52); }
  void set_next_mode_answer(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t next_mode_answer_address() const { return address + 52; }
  // slot 14: ModeType5::Leaving (Mode::Slot14Thunk)
  uint32_t leaving() const { return guest::LoadU32(base, address + 56); }
  void set_leaving(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t leaving_address() const { return address + 56; }
};

// The image words docs/functions/type5-mode.md's code uses.
struct Type5ModeGlobals {
  // the dialog's page object (pages 0 to 3, steps 0 to 9)
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E3Cu)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E3Cu, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E3Cu, v); }
  static constexpr uint32_t pages_address() { return 0x82814E3Cu; }
  // the dialog's menu, items 0 (yes) to 1 (no)
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E40u)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E40u, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E40u, v); }
  static constexpr uint32_t menu_address() { return 0x82814E40u; }
  template <typename T = Type5DialogData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E44u)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E44u, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E44u, v); }
  static constexpr uint32_t data_address() { return 0x82814E44u; }
  // the parking area's screen (location-screen.toml's): Resume sets its restart (+900)
  template <typename T = LocationScreen>
  static T location_screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E50u)); }
  static void set_location_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E50u, v.address); }
  static void set_location_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E50u, v); }
  static constexpr uint32_t location_screen_address() { return 0x82814E50u; }
  // how the title is entered: 1 from ForceExit (reasons 0 and 1)
  static uint32_t leaving_flag(uint8_t* base) { return guest::LoadU32(base, 0x826364ECu); }
  static void set_leaving_flag(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826364ECu, v); }
  static constexpr uint32_t leaving_flag_address() { return 0x826364ECu; }
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
