// The views of docs/functions/todays-result.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/todays-result.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ModeType9, ModeType9::WaitForLoads.r4, ModeType9::WaitForShown.r4, ModeType9::LeaveScreen.r4, ModeType9::NextMode.r4, ModeType9::Leaving.r4
#pragma pack(push, 1)
struct ModeType9Data {
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
static_assert(sizeof(ModeType9Data) == 852);
inline constexpr memory::LayoutField kModeType9Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {816, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kModeType9Layout = {kModeType9Fields, 12};

struct ModeType9 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x82006CAC;
  ModeType9Data* data() const { return static_cast<ModeType9Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeType9 New(uint8_t* base) {
    return ModeType9(base, memory::MakeHandle(new ModeType9Data(), sizeof(ModeType9Data), "ModeType9", &kModeType9Layout));
  }
  template <uint32_t Stride = kSize>
  static ModeType9 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeType9Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType9Data*>(::operator new(sizeof(ModeType9Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType9Data();
    return ModeType9(base, memory::MakeHandle(p, sizeof(ModeType9Data) * count, "ModeType9", &kModeType9Layout, sizeof(ModeType9Data)));
  }
  ModeType9 at(uint32_t i) const { return ModeType9(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeType9Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006CAC (14 slots)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), the delegates' r3
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { return on_host() ? uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9Layout, 0, 1)) : guest::LoadU8(base, address + 0); }
  void set_answer_changed_bits(uint8_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9Layout, 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the next-mode answer's 8 bytes (ModeAnswer)
  uint32_t answer_address() const { return address + 0; }
  uint64_t answer_bits() const { return on_host() ? uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9Layout, 0, 8)) : guest::LoadU64(base, address + 0); }
  void set_answer_bits(uint64_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9Layout, 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  uint64_t answer() const { return uint64_t(answer_bits()); }
  void set_answer(uint64_t v) const { set_answer_bits(uint64_t(v)); }
  // the delegate at +8, 0 none (also the answer's next type +4 by the measure's join)
  uint32_t delegate_ptr_address() const { return address + 4; }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, on_host() ? data()->delegate_ptr : guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  void set_delegate_ptr(uint32_t v) const { if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_bits() const { return on_host() ? data()->delegate_ptr : guest::LoadU32(base, address + 4); }
  void set_delegate_ptr_bits(uint32_t v) const { if (on_host()) data()->delegate_ptr = v; else guest::StoreU32(base, address + 4, v); }
  // 0x82010FA0 (ModeDelegate::Invoke)
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable() const { return on_host() ? uint32_t(data()->delegate_vtable) : uint32_t(guest::LoadU32(base, address + 8)); }
  void set_delegate_vtable(uint32_t v) const { if (on_host()) data()->delegate_vtable = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable_bits() const { return on_host() ? data()->delegate_vtable : guest::LoadU32(base, address + 8); }
  void set_delegate_vtable_bits(uint32_t v) const { if (on_host()) data()->delegate_vtable = v; else guest::StoreU32(base, address + 8, v); }
  // the mode
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object() const { return on_host() ? uint32_t(data()->delegate_object) : uint32_t(guest::LoadU32(base, address + 12)); }
  void set_delegate_object(uint32_t v) const { if (on_host()) data()->delegate_object = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object_bits() const { return on_host() ? data()->delegate_object : guest::LoadU32(base, address + 12); }
  void set_delegate_object_bits(uint32_t v) const { if (on_host()) data()->delegate_object = v; else guest::StoreU32(base, address + 12, v); }
  // a Mode::SlotNThunk or Mode::NextModeThunk 0x82235C08
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
  // 0x82006CA0: IsLoaded, ForceExit, an empty function
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32() const { return on_host() ? uint32_t(data()->vtable32) : uint32_t(guest::LoadU32(base, address + 32)); }
  void set_vtable32(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_bits() const { return on_host() ? data()->vtable32 : guest::LoadU32(base, address + 32); }
  void set_vtable32_bits(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (the two machines)
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
  // measure fault: IsLoaded's and ForceExit's r3 is the mode + 32, so this is the mode's +848 (block)
  uint32_t iface_block_address() const { return address + 816; }
  template <typename T = ModeType9Block>
  T iface_block() const { return T(base, on_host() ? data()->iface_block : guest::LoadU32(base, address + 816)); }
  void set_iface_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  void set_iface_block(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t iface_block_bits() const { return on_host() ? data()->iface_block : guest::LoadU32(base, address + 816); }
  void set_iface_block_bits(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  // VirtualAlloc'ed by Init, freed by Release
  uint32_t block_address() const { return address + 848; }
  template <typename T = ModeType9Block>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 848)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 848); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
};

// allocated; measure: new<19300>@ModeType9::Init, ModeType9.+816, ModeType9::WaitForLoads.r4.+848, ModeType9::NextMode.r4.+848, ModeType9::Leaving.r4.+848
#pragma pack(push, 1)
struct ModeType9BlockData {
  uint8_t pad_0[9604] = {};
  uint32_t result_state = 0;  // +9604
  uint32_t result_loaded = 0;  // +9608
  uint32_t result_reenter = 0;  // +9612
  uint32_t result_delete_movie = 0;  // +9616
  uint8_t pad_9620[9624] = {};
  uint32_t event_state = 0;  // +19244
  uint32_t event_loaded = 0;  // +19248
  uint32_t event_reenter = 0;  // +19252
  uint32_t event_delete_movie = 0;  // +19256
  uint8_t pad_19260[20] = {};
  uint32_t shown = 0;  // +19280
  uint32_t shown_at_init = 0;  // +19284
  uint8_t pad_19288[4] = {};
  uint64_t answer = 0;  // +19292
};
#pragma pack(pop)
static_assert(sizeof(ModeType9BlockData) == 19300);
inline constexpr memory::LayoutField kModeType9BlockFields[] = {{9600, 4}, {9604, 4}, {9604, 4}, {9608, 4}, {9608, 4}, {9612, 4}, {9612, 4}, {9616, 4}, {9616, 4}, {19240, 4}, {19244, 4}, {19244, 4}, {19248, 4}, {19248, 4}, {19252, 4}, {19252, 4}, {19256, 4}, {19256, 4}, {19280, 4}, {19284, 4}, {19292, 8}};
inline constexpr memory::Layout kModeType9BlockLayout = {kModeType9BlockFields, 21};

struct ModeType9Block : View {
  using View::View;
  static constexpr uint32_t kSize = 19300;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ModeType9BlockData* data() const { return static_cast<ModeType9BlockData*>(memory::HandleObject(address)); }
  static ModeType9Block New(uint8_t* base) {
    return ModeType9Block(base, memory::MakeHandle(new ModeType9BlockData(), sizeof(ModeType9BlockData), "ModeType9Block", &kModeType9BlockLayout));
  }
  template <uint32_t Stride = kSize>
  static ModeType9Block NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeType9BlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType9BlockData*>(::operator new(sizeof(ModeType9BlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType9BlockData();
    return ModeType9Block(base, memory::MakeHandle(p, sizeof(ModeType9BlockData) * count, "ModeType9Block", &kModeType9BlockLayout, sizeof(ModeType9BlockData)));
  }
  ModeType9Block at(uint32_t i) const { return ModeType9Block(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeType9BlockLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9BlockLayout, offset, width, v); }
  // today's result's machine (its +9604 state, +9608 loaded, +9612 reenter 1 at Init, +9616 delete_movie 0 at Init)
  template <typename T = TodaysResultMachine>
  T result() const { return T(base, address + 0); }
  // today's event's machine (the same words at +19244 to +19256)
  template <typename T = TodaysEventMachine>
  T event() const { return T(base, address + 9640); }
  // today's result's machine's state
  uint32_t result_state_address() const { return address + 9604; }
  uint32_t result_state() const { return data()->result_state; }
  void set_result_state(uint32_t v) const { data()->result_state = v; }
  uint32_t result_state_bits() const { return data()->result_state; }
  void set_result_state_bits(uint32_t v) const { data()->result_state = v; }
  // movie 27 loaded
  uint32_t result_loaded_address() const { return address + 9608; }
  uint32_t result_loaded() const { return data()->result_loaded; }
  void set_result_loaded(uint32_t v) const { data()->result_loaded = v; }
  uint32_t result_loaded_bits() const { return data()->result_loaded; }
  void set_result_loaded_bits(uint32_t v) const { data()->result_loaded = v; }
  // 1 at Init
  uint32_t result_reenter_address() const { return address + 9612; }
  uint32_t result_reenter() const { return data()->result_reenter; }
  void set_result_reenter(uint32_t v) const { data()->result_reenter = v; }
  uint32_t result_reenter_bits() const { return data()->result_reenter; }
  void set_result_reenter_bits(uint32_t v) const { data()->result_reenter = v; }
  // 0 at Init
  uint32_t result_delete_movie_address() const { return address + 9616; }
  uint32_t result_delete_movie() const { return data()->result_delete_movie; }
  void set_result_delete_movie(uint32_t v) const { data()->result_delete_movie = v; }
  uint32_t result_delete_movie_bits() const { return data()->result_delete_movie; }
  void set_result_delete_movie_bits(uint32_t v) const { data()->result_delete_movie = v; }
  // today's event's machine's state
  uint32_t event_state_address() const { return address + 19244; }
  uint32_t event_state() const { return data()->event_state; }
  void set_event_state(uint32_t v) const { data()->event_state = v; }
  uint32_t event_state_bits() const { return data()->event_state; }
  void set_event_state_bits(uint32_t v) const { data()->event_state = v; }
  // movie 0 and the images loaded
  uint32_t event_loaded_address() const { return address + 19248; }
  uint32_t event_loaded() const { return data()->event_loaded; }
  void set_event_loaded(uint32_t v) const { data()->event_loaded = v; }
  uint32_t event_loaded_bits() const { return data()->event_loaded; }
  void set_event_loaded_bits(uint32_t v) const { data()->event_loaded = v; }
  // 1 at Init
  uint32_t event_reenter_address() const { return address + 19252; }
  uint32_t event_reenter() const { return data()->event_reenter; }
  void set_event_reenter(uint32_t v) const { data()->event_reenter = v; }
  uint32_t event_reenter_bits() const { return data()->event_reenter; }
  void set_event_reenter_bits(uint32_t v) const { data()->event_reenter = v; }
  // 0 at Init
  uint32_t event_delete_movie_address() const { return address + 19256; }
  uint32_t event_delete_movie() const { return data()->event_delete_movie; }
  void set_event_delete_movie(uint32_t v) const { data()->event_delete_movie = v; }
  uint32_t event_delete_movie_bits() const { return data()->event_delete_movie; }
  void set_event_delete_movie_bits(uint32_t v) const { data()->event_delete_movie = v; }
  // the shown machine's index: 27 today's result, 0 today's event (FindShown)
  uint32_t shown_address() const { return address + 19280; }
  uint32_t shown() const { return data()->shown; }
  void set_shown(uint32_t v) const { data()->shown = v; }
  uint32_t shown_bits() const { return data()->shown; }
  void set_shown_bits(uint32_t v) const { data()->shown = v; }
  // the same at Init (readers not looked for)
  uint32_t shown_at_init_address() const { return address + 19284; }
  uint32_t shown_at_init() const { return data()->shown_at_init; }
  void set_shown_at_init(uint32_t v) const { data()->shown_at_init = v; }
  uint32_t shown_at_init_bits() const { return data()->shown_at_init; }
  void set_shown_at_init_bits(uint32_t v) const { data()->shown_at_init = v; }
  // the next mode's 8-byte answer ForceExit writes and NextModeAnswer hands to Modes::Tick (ModeAnswer)
  uint32_t answer_address() const { return address + 19292; }
  uint64_t answer() const { return data()->answer; }
  void set_answer(uint64_t v) const { data()->answer = v; }
  uint64_t answer_bits() const { return data()->answer; }
  void set_answer_bits(uint64_t v) const { data()->answer = v; }
  // the answer's first word (changed and three bytes), as NextModeAnswer copies it
  uint32_t answer_word0_address() const { return address + 19292; }
  uint32_t answer_word0_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9BlockLayout, 19292, 4)); }
  void set_answer_word0_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9BlockLayout, 19292, 4, v); }
  uint32_t answer_word0() const { return uint32_t(answer_word0_bits()); }
  void set_answer_word0(uint32_t v) const { set_answer_word0_bits(uint32_t(v)); }
  // the answer's next mode type
  uint32_t answer_next_type_address() const { return address + 19296; }
  uint32_t answer_next_type_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9BlockLayout, 19296, 4)); }
  void set_answer_next_type_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType9BlockLayout, 19296, 4, v); }
  uint32_t answer_next_type() const { return uint32_t(answer_next_type_bits()); }
  void set_answer_next_type(uint32_t v) const { set_answer_next_type_bits(uint32_t(v)); }
};

// embedded; measure: TodaysResult
#pragma pack(push, 1)
struct TodaysResultMachineData {
  uint8_t transitions[9600] = {};  // +0, guest order
  uint32_t transition_count = 0;  // +9600
  uint32_t state = 0;  // +9604
  uint32_t loaded = 0;  // +9608
  uint32_t reenter = 0;  // +9612
  uint32_t delete_movie = 0;  // +9616
};
#pragma pack(pop)
static_assert(sizeof(TodaysResultMachineData) == 9620);
inline constexpr memory::LayoutField kTodaysResultMachineFields[] = {{9600, 4}, {9604, 4}, {9608, 4}, {9612, 4}, {9616, 4}};
inline constexpr memory::Layout kTodaysResultMachineLayout = {kTodaysResultMachineFields, 5};

struct TodaysResultMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TodaysResultMachineData* data() const { return static_cast<TodaysResultMachineData*>(memory::HandleObject(address)); }
  static TodaysResultMachine New(uint8_t* base) {
    return TodaysResultMachine(base, memory::MakeHandle(new TodaysResultMachineData(), sizeof(TodaysResultMachineData), "TodaysResultMachine", &kTodaysResultMachineLayout));
  }
  template <uint32_t Stride = kSize>
  static TodaysResultMachine NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TodaysResultMachineData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TodaysResultMachineData*>(::operator new(sizeof(TodaysResultMachineData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TodaysResultMachineData();
    return TodaysResultMachine(base, memory::MakeHandle(p, sizeof(TodaysResultMachineData) * count, "TodaysResultMachine", &kTodaysResultMachineLayout, sizeof(TodaysResultMachineData)));
  }
  TodaysResultMachine at(uint32_t i) const { return TodaysResultMachine(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kTodaysResultMachineLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysResultMachineLayout, offset, width, v); }
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  uint8_t* transitions() const { return reinterpret_cast<uint8_t*>(data()) + 0; }
  // measure fault: the transitions' r3 is the page object's +24 (PageObject.step_started)
  uint32_t fault_step_started_address() const { return address + 0; }
  uint32_t fault_step_started_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysResultMachineLayout, 0, 4)); }
  void set_fault_step_started_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysResultMachineLayout, 0, 4, v); }
  uint32_t fault_step_started() const { return uint32_t(fault_step_started_bits()); }
  void set_fault_step_started(uint32_t v) const { set_fault_step_started_bits(uint32_t(v)); }
  // measure fault: LeavePage's r3 is the dispatcher (ModeDispatcher.ring_count)
  uint32_t fault_ring_count_address() const { return address + 804; }
  uint32_t fault_ring_count_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysResultMachineLayout, 804, 4)); }
  void set_fault_ring_count_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysResultMachineLayout, 804, 4, v); }
  uint32_t fault_ring_count() const { return uint32_t(fault_ring_count_bits()); }
  void set_fault_ring_count(uint32_t v) const { set_fault_ring_count_bits(uint32_t(v)); }
  // measure fault: LeavePage's r3 is the dispatcher (ModeDispatcher.ring_start)
  uint32_t fault_ring_start_address() const { return address + 808; }
  uint32_t fault_ring_start_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysResultMachineLayout, 808, 4)); }
  void set_fault_ring_start_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysResultMachineLayout, 808, 4, v); }
  uint32_t fault_ring_start() const { return uint32_t(fault_ring_start_bits()); }
  void set_fault_ring_start(uint32_t v) const { set_fault_ring_start_bits(uint32_t(v)); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t transition_count() const { return data()->transition_count; }
  void set_transition_count(uint32_t v) const { data()->transition_count = v; }
  uint32_t transition_count_bits() const { return data()->transition_count; }
  void set_transition_count_bits(uint32_t v) const { data()->transition_count = v; }
  // 1 or 9 at Create, 4 shown, 7 leaving, 10 unloaded, 13 done, 20 done with a movie queued
  uint32_t state_address() const { return address + 9604; }
  uint32_t state() const { return data()->state; }
  void set_state(uint32_t v) const { data()->state = v; }
  uint32_t state_bits() const { return data()->state; }
  void set_state_bits(uint32_t v) const { data()->state = v; }
  // movie 27 loaded
  uint32_t loaded_address() const { return address + 9608; }
  uint32_t loaded() const { return data()->loaded; }
  void set_loaded(uint32_t v) const { data()->loaded = v; }
  uint32_t loaded_bits() const { return data()->loaded; }
  void set_loaded_bits(uint32_t v) const { data()->loaded = v; }
  // nonzero: Create and WaitForMovie go to state 9 (open on message 6)
  uint32_t reenter_address() const { return address + 9612; }
  uint32_t reenter() const { return data()->reenter; }
  void set_reenter(uint32_t v) const { data()->reenter = v; }
  uint32_t reenter_bits() const { return data()->reenter; }
  void set_reenter_bits(uint32_t v) const { data()->reenter = v; }
  // nonzero: Unload deletes movie 27 and clears loaded
  uint32_t delete_movie_address() const { return address + 9616; }
  uint32_t delete_movie() const { return data()->delete_movie; }
  void set_delete_movie(uint32_t v) const { data()->delete_movie = v; }
  uint32_t delete_movie_bits() const { return data()->delete_movie; }
  void set_delete_movie_bits(uint32_t v) const { data()->delete_movie = v; }
};

// embedded; measure: TodaysEvent, TodaysEvent::EndPage.r4
#pragma pack(push, 1)
struct TodaysEventMachineData {
  uint8_t transitions[9600] = {};  // +0, guest order
  uint32_t transition_count = 0;  // +9600
  uint32_t state = 0;  // +9604
  uint32_t loaded = 0;  // +9608
  uint32_t reenter = 0;  // +9612
  uint32_t delete_movie = 0;  // +9616
};
#pragma pack(pop)
static_assert(sizeof(TodaysEventMachineData) == 9620);
inline constexpr memory::LayoutField kTodaysEventMachineFields[] = {{9600, 4}, {9604, 4}, {9608, 4}, {9612, 4}, {9616, 4}};
inline constexpr memory::Layout kTodaysEventMachineLayout = {kTodaysEventMachineFields, 5};

struct TodaysEventMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TodaysEventMachineData* data() const { return static_cast<TodaysEventMachineData*>(memory::HandleObject(address)); }
  static TodaysEventMachine New(uint8_t* base) {
    return TodaysEventMachine(base, memory::MakeHandle(new TodaysEventMachineData(), sizeof(TodaysEventMachineData), "TodaysEventMachine", &kTodaysEventMachineLayout));
  }
  template <uint32_t Stride = kSize>
  static TodaysEventMachine NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TodaysEventMachineData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TodaysEventMachineData*>(::operator new(sizeof(TodaysEventMachineData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TodaysEventMachineData();
    return TodaysEventMachine(base, memory::MakeHandle(p, sizeof(TodaysEventMachineData) * count, "TodaysEventMachine", &kTodaysEventMachineLayout, sizeof(TodaysEventMachineData)));
  }
  TodaysEventMachine at(uint32_t i) const { return TodaysEventMachine(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kTodaysEventMachineLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, offset, width, v); }
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  uint8_t* transitions() const { return reinterpret_cast<uint8_t*>(data()) + 0; }
  // measure fault: the transitions' r3 (PageObject.step_started), and the data's +0 (TodaysEventData.ready)
  uint32_t fault_step_started_address() const { return address + 0; }
  uint32_t fault_step_started_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 0, 4)); }
  void set_fault_step_started_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 0, 4, v); }
  uint32_t fault_step_started() const { return uint32_t(fault_step_started_bits()); }
  void set_fault_step_started(uint32_t v) const { set_fault_step_started_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.images_a's first record
  uint32_t fault_data_image_records_a_address() const { return address + 4; }
  uint32_t fault_data_image_records_a_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 4, 4)); }
  void set_fault_data_image_records_a_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 4, 4, v); }
  uint32_t fault_data_image_records_a() const { return uint32_t(fault_data_image_records_a_bits()); }
  void set_fault_data_image_records_a(uint32_t v) const { set_fault_data_image_records_a_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.image_count_a
  uint32_t fault_data_image_count_a_address() const { return address + 188; }
  uint32_t fault_data_image_count_a_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 188, 4)); }
  void set_fault_data_image_count_a_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 188, 4, v); }
  uint32_t fault_data_image_count_a() const { return uint32_t(fault_data_image_count_a_bits()); }
  void set_fault_data_image_count_a(uint32_t v) const { set_fault_data_image_count_a_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.images_b's first record
  uint32_t fault_data_image_records_b_address() const { return address + 192; }
  uint32_t fault_data_image_records_b_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 192, 4)); }
  void set_fault_data_image_records_b_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 192, 4, v); }
  uint32_t fault_data_image_records_b() const { return uint32_t(fault_data_image_records_b_bits()); }
  void set_fault_data_image_records_b(uint32_t v) const { set_fault_data_image_records_b_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.image_count_b
  uint32_t fault_data_image_count_b_address() const { return address + 376; }
  uint32_t fault_data_image_count_b_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 376, 4)); }
  void set_fault_data_image_count_b_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 376, 4, v); }
  uint32_t fault_data_image_count_b() const { return uint32_t(fault_data_image_count_b_bits()); }
  void set_fault_data_image_count_b(uint32_t v) const { set_fault_data_image_count_b_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.image_bits_0
  uint32_t fault_data_image_bits_0_address() const { return address + 380; }
  uint32_t fault_data_image_bits_0_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 380, 4)); }
  void set_fault_data_image_bits_0_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 380, 4, v); }
  uint32_t fault_data_image_bits_0() const { return uint32_t(fault_data_image_bits_0_bits()); }
  void set_fault_data_image_bits_0(uint32_t v) const { set_fault_data_image_bits_0_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.image_bits_1
  uint32_t fault_data_image_bits_1_address() const { return address + 384; }
  uint32_t fault_data_image_bits_1_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 384, 4)); }
  void set_fault_data_image_bits_1_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 384, 4, v); }
  uint32_t fault_data_image_bits_1() const { return uint32_t(fault_data_image_bits_1_bits()); }
  void set_fault_data_image_bits_1(uint32_t v) const { set_fault_data_image_bits_1_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.unlock_table
  uint32_t fault_data_unlock_table_address() const { return address + 392; }
  uint32_t fault_data_unlock_table_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 392, 4)); }
  void set_fault_data_unlock_table_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 392, 4, v); }
  uint32_t fault_data_unlock_table() const { return uint32_t(fault_data_unlock_table_bits()); }
  void set_fault_data_unlock_table(uint32_t v) const { set_fault_data_unlock_table_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.unlock_entry
  uint32_t fault_data_unlock_entry_address() const { return address + 396; }
  uint32_t fault_data_unlock_entry_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 396, 4)); }
  void set_fault_data_unlock_entry_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 396, 4, v); }
  uint32_t fault_data_unlock_entry() const { return uint32_t(fault_data_unlock_entry_bits()); }
  void set_fault_data_unlock_entry(uint32_t v) const { set_fault_data_unlock_entry_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.movie_object_36
  uint32_t fault_data_movie_object_36_address() const { return address + 400; }
  uint32_t fault_data_movie_object_36_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 400, 4)); }
  void set_fault_data_movie_object_36_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 400, 4, v); }
  uint32_t fault_data_movie_object_36() const { return uint32_t(fault_data_movie_object_36_bits()); }
  void set_fault_data_movie_object_36(uint32_t v) const { set_fault_data_movie_object_36_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.movie_object_32
  uint32_t fault_data_movie_object_32_address() const { return address + 404; }
  uint32_t fault_data_movie_object_32_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 404, 4)); }
  void set_fault_data_movie_object_32_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 404, 4, v); }
  uint32_t fault_data_movie_object_32() const { return uint32_t(fault_data_movie_object_32_bits()); }
  void set_fault_data_movie_object_32(uint32_t v) const { set_fault_data_movie_object_32_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.movies_queued
  uint32_t fault_data_movies_queued_address() const { return address + 408; }
  uint32_t fault_data_movies_queued_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 408, 4)); }
  void set_fault_data_movies_queued_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 408, 4, v); }
  uint32_t fault_data_movies_queued() const { return uint32_t(fault_data_movies_queued_bits()); }
  void set_fault_data_movies_queued(uint32_t v) const { set_fault_data_movies_queued_bits(uint32_t(v)); }
  // measure fault: EndPage's r3 is the dispatcher (ModeDispatcher.ring_count)
  uint32_t fault_ring_count_address() const { return address + 804; }
  uint32_t fault_ring_count_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 804, 4)); }
  void set_fault_ring_count_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 804, 4, v); }
  uint32_t fault_ring_count() const { return uint32_t(fault_ring_count_bits()); }
  void set_fault_ring_count(uint32_t v) const { set_fault_ring_count_bits(uint32_t(v)); }
  // measure fault: EndPage's r3 is the dispatcher (ModeDispatcher.ring_start)
  uint32_t fault_ring_start_address() const { return address + 808; }
  uint32_t fault_ring_start_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 808, 4)); }
  void set_fault_ring_start_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventMachineLayout, 808, 4, v); }
  uint32_t fault_ring_start() const { return uint32_t(fault_ring_start_bits()); }
  void set_fault_ring_start(uint32_t v) const { set_fault_ring_start_bits(uint32_t(v)); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t transition_count() const { return data()->transition_count; }
  void set_transition_count(uint32_t v) const { data()->transition_count = v; }
  uint32_t transition_count_bits() const { return data()->transition_count; }
  void set_transition_count_bits(uint32_t v) const { data()->transition_count = v; }
  // 1 or 9 at Create (9 with no unlock), 4 shown, 7 leaving, 9 after Unloaded, 10 at the end page, 20 done with movies queued
  uint32_t state_address() const { return address + 9604; }
  uint32_t state() const { return data()->state; }
  void set_state(uint32_t v) const { data()->state = v; }
  uint32_t state_bits() const { return data()->state; }
  void set_state_bits(uint32_t v) const { data()->state = v; }
  // movie 0 and both image lists loaded
  uint32_t loaded_address() const { return address + 9608; }
  uint32_t loaded() const { return data()->loaded; }
  void set_loaded(uint32_t v) const { data()->loaded = v; }
  uint32_t loaded_bits() const { return data()->loaded; }
  void set_loaded_bits(uint32_t v) const { data()->loaded = v; }
  // nonzero: Create and WaitForMovie go to state 9
  uint32_t reenter_address() const { return address + 9612; }
  uint32_t reenter() const { return data()->reenter; }
  void set_reenter(uint32_t v) const { data()->reenter = v; }
  uint32_t reenter_bits() const { return data()->reenter; }
  void set_reenter_bits(uint32_t v) const { data()->reenter = v; }
  // nonzero: Unload deletes movie 0 and clears loaded
  uint32_t delete_movie_address() const { return address + 9616; }
  uint32_t delete_movie() const { return data()->delete_movie; }
  void set_delete_movie(uint32_t v) const { data()->delete_movie = v; }
  uint32_t delete_movie_bits() const { return data()->delete_movie; }
  void set_delete_movie_bits(uint32_t v) const { data()->delete_movie = v; }
};

// allocated; measure: new<68>@TodaysResult::Create#2
#pragma pack(push, 1)
struct TodaysResultDataData {
  uint32_t ready = 0;  // +0
  uint32_t movie_queued = 0;  // +4
  uint32_t day_wins = 0;  // +8
  uint32_t day_losses = 0;  // +12
  uint32_t day_outcome3 = 0;  // +16
  float day_win_rate = 0;  // +20
  uint32_t day_win_streak = 0;  // +24
  uint32_t day_word_4832 = 0;  // +28
  uint32_t career_wins = 0;  // +32
  uint32_t career_losses = 0;  // +36
  uint32_t career_outcome3 = 0;  // +40
  float career_win_rate = 0;  // +44
  uint32_t best_win_streak = 0;  // +48
  uint32_t money = 0;  // +52
  uint32_t mode = 0;  // +56
  uint32_t weekday = 0;  // +60
  uint32_t day = 0;  // +64
  uint8_t pick[20] = {};  // +68, guest order
};
#pragma pack(pop)
static_assert(sizeof(TodaysResultDataData) == 88);
inline constexpr memory::LayoutField kTodaysResultDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}};
inline constexpr memory::Layout kTodaysResultDataLayout = {kTodaysResultDataFields, 17};

struct TodaysResultData : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TodaysResultDataData* data() const { return static_cast<TodaysResultDataData*>(memory::HandleObject(address)); }
  static TodaysResultData New(uint8_t* base) {
    return TodaysResultData(base, memory::MakeHandle(new TodaysResultDataData(), sizeof(TodaysResultDataData), "TodaysResultData", &kTodaysResultDataLayout));
  }
  template <uint32_t Stride = kSize>
  static TodaysResultData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TodaysResultDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TodaysResultDataData*>(::operator new(sizeof(TodaysResultDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TodaysResultDataData();
    return TodaysResultData(base, memory::MakeHandle(p, sizeof(TodaysResultDataData) * count, "TodaysResultData", &kTodaysResultDataLayout, sizeof(TodaysResultDataData)));
  }
  TodaysResultData at(uint32_t i) const { return TodaysResultData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kTodaysResultDataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysResultDataLayout, offset, width, v); }
  // 1 once the texts are set (SetUpMovie)
  uint32_t ready_address() const { return address + 0; }
  uint32_t ready() const { return data()->ready; }
  void set_ready(uint32_t v) const { data()->ready = v; }
  uint32_t ready_bits() const { return data()->ready; }
  void set_ready_bits(uint32_t v) const { data()->ready = v; }
  // 1 when LeavePage queued a movie; Destroy then goes to state 20
  uint32_t movie_queued_address() const { return address + 4; }
  uint32_t movie_queued() const { return data()->movie_queued; }
  void set_movie_queued(uint32_t v) const { data()->movie_queued = v; }
  uint32_t movie_queued_bits() const { return data()->movie_queued; }
  void set_movie_queued_bits(uint32_t v) const { data()->movie_queued = v; }
  uint32_t day_wins_address() const { return address + 8; }
  uint32_t day_wins() const { return data()->day_wins; }
  void set_day_wins(uint32_t v) const { data()->day_wins = v; }
  uint32_t day_wins_bits() const { return data()->day_wins; }
  void set_day_wins_bits(uint32_t v) const { data()->day_wins = v; }
  uint32_t day_losses_address() const { return address + 12; }
  uint32_t day_losses() const { return data()->day_losses; }
  void set_day_losses(uint32_t v) const { data()->day_losses = v; }
  uint32_t day_losses_bits() const { return data()->day_losses; }
  void set_day_losses_bits(uint32_t v) const { data()->day_losses = v; }
  // the day's outcome 3 count
  uint32_t day_outcome3_address() const { return address + 16; }
  uint32_t day_outcome3() const { return data()->day_outcome3; }
  void set_day_outcome3(uint32_t v) const { data()->day_outcome3 = v; }
  uint32_t day_outcome3_bits() const { return data()->day_outcome3; }
  void set_day_outcome3_bits(uint32_t v) const { data()->day_outcome3 = v; }
  // the day's win rate in percent
  uint32_t day_win_rate_address() const { return address + 20; }
  float day_win_rate() const { return data()->day_win_rate; }
  void set_day_win_rate(float v) const { data()->day_win_rate = v; }
  uint32_t day_win_rate_bits() const { return std::bit_cast<uint32_t>(data()->day_win_rate); }
  void set_day_win_rate_bits(uint32_t v) const { data()->day_win_rate = std::bit_cast<float>(v); }
  uint32_t day_win_streak_address() const { return address + 24; }
  uint32_t day_win_streak() const { return data()->day_win_streak; }
  void set_day_win_streak(uint32_t v) const { data()->day_win_streak = v; }
  uint32_t day_win_streak_bits() const { return data()->day_win_streak; }
  void set_day_win_streak_bits(uint32_t v) const { data()->day_win_streak = v; }
  // the day block's +4832 (DayBlock::Word4832)
  uint32_t day_word_4832_address() const { return address + 28; }
  uint32_t day_word_4832() const { return data()->day_word_4832; }
  void set_day_word_4832(uint32_t v) const { data()->day_word_4832 = v; }
  uint32_t day_word_4832_bits() const { return data()->day_word_4832; }
  void set_day_word_4832_bits(uint32_t v) const { data()->day_word_4832 = v; }
  uint32_t career_wins_address() const { return address + 32; }
  uint32_t career_wins() const { return data()->career_wins; }
  void set_career_wins(uint32_t v) const { data()->career_wins = v; }
  uint32_t career_wins_bits() const { return data()->career_wins; }
  void set_career_wins_bits(uint32_t v) const { data()->career_wins = v; }
  uint32_t career_losses_address() const { return address + 36; }
  uint32_t career_losses() const { return data()->career_losses; }
  void set_career_losses(uint32_t v) const { data()->career_losses = v; }
  uint32_t career_losses_bits() const { return data()->career_losses; }
  void set_career_losses_bits(uint32_t v) const { data()->career_losses = v; }
  uint32_t career_outcome3_address() const { return address + 40; }
  uint32_t career_outcome3() const { return data()->career_outcome3; }
  void set_career_outcome3(uint32_t v) const { data()->career_outcome3 = v; }
  uint32_t career_outcome3_bits() const { return data()->career_outcome3; }
  void set_career_outcome3_bits(uint32_t v) const { data()->career_outcome3 = v; }
  // in percent
  uint32_t career_win_rate_address() const { return address + 44; }
  float career_win_rate() const { return data()->career_win_rate; }
  void set_career_win_rate(float v) const { data()->career_win_rate = v; }
  uint32_t career_win_rate_bits() const { return std::bit_cast<uint32_t>(data()->career_win_rate); }
  void set_career_win_rate_bits(uint32_t v) const { data()->career_win_rate = std::bit_cast<float>(v); }
  // the stats' +5968
  uint32_t best_win_streak_address() const { return address + 48; }
  uint32_t best_win_streak() const { return data()->best_win_streak; }
  void set_best_win_streak(uint32_t v) const { data()->best_win_streak = v; }
  uint32_t best_win_streak_bits() const { return data()->best_win_streak; }
  void set_best_win_streak_bits(uint32_t v) const { data()->best_win_streak = v; }
  // the save's +6116
  uint32_t money_address() const { return address + 52; }
  uint32_t money() const { return data()->money; }
  void set_money(uint32_t v) const { data()->money = v; }
  uint32_t money_bits() const { return data()->money; }
  void set_money_bits(uint32_t v) const { data()->money = v; }
  // the save's +6104
  uint32_t mode_address() const { return address + 56; }
  uint32_t mode() const { return data()->mode; }
  void set_mode(uint32_t v) const { data()->mode = v; }
  uint32_t mode_bits() const { return data()->mode; }
  void set_mode_bits(uint32_t v) const { data()->mode = v; }
  // the day % 7
  uint32_t weekday_address() const { return address + 60; }
  uint32_t weekday() const { return data()->weekday; }
  void set_weekday(uint32_t v) const { data()->weekday = v; }
  uint32_t weekday_bits() const { return data()->weekday; }
  void set_weekday_bits(uint32_t v) const { data()->weekday = v; }
  // the save's +6100, at most 999
  uint32_t day_address() const { return address + 64; }
  uint32_t day() const { return data()->day; }
  void set_day(uint32_t v) const { data()->day = v; }
  uint32_t day_bits() const { return data()->day; }
  void set_day_bits(uint32_t v) const { data()->day = v; }
  // a copy of the save's 20-byte pick at +8 (five words)
  uint32_t pick_address() const { return address + 68; }
  uint8_t* pick() const { return reinterpret_cast<uint8_t*>(data()) + 68; }
};

// allocated; measure: -
#pragma pack(push, 1)
struct TodaysEventDataData {
  uint32_t ready = 0;  // +0
  uint8_t pad_4[184] = {};
  uint32_t image_count_a = 0;  // +188
  uint8_t pad_192[184] = {};
  uint32_t image_count_b = 0;  // +376
  uint32_t image_bits_0 = 0;  // +380
  uint32_t image_bits_1 = 0;  // +384
  uint32_t unlock_word = 0;  // +388
  uint32_t unlock_table = 0;  // +392
  uint32_t unlock_entry = 0;  // +396
  uint32_t movie_object_36 = 0;  // +400
  uint32_t movie_object_32 = 0;  // +404
  uint32_t movies_queued = 0;  // +408
};
#pragma pack(pop)
static_assert(sizeof(TodaysEventDataData) == 412);
inline constexpr memory::LayoutField kTodaysEventDataFields[] = {{0, 4}, {188, 4}, {188, 4}, {376, 4}, {376, 4}, {380, 4}, {384, 4}, {388, 4}, {392, 4}, {396, 4}, {400, 4}, {404, 4}, {408, 4}};
inline constexpr memory::Layout kTodaysEventDataLayout = {kTodaysEventDataFields, 13};

struct TodaysEventData : View {
  using View::View;
  static constexpr uint32_t kSize = 412;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TodaysEventDataData* data() const { return static_cast<TodaysEventDataData*>(memory::HandleObject(address)); }
  static TodaysEventData New(uint8_t* base) {
    return TodaysEventData(base, memory::MakeHandle(new TodaysEventDataData(), sizeof(TodaysEventDataData), "TodaysEventData", &kTodaysEventDataLayout));
  }
  template <uint32_t Stride = kSize>
  static TodaysEventData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TodaysEventDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TodaysEventDataData*>(::operator new(sizeof(TodaysEventDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TodaysEventDataData();
    return TodaysEventData(base, memory::MakeHandle(p, sizeof(TodaysEventDataData) * count, "TodaysEventData", &kTodaysEventDataLayout, sizeof(TodaysEventDataData)));
  }
  TodaysEventData at(uint32_t i) const { return TodaysEventData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kTodaysEventDataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventDataLayout, offset, width, v); }
  // 1 once the texts are set (SetUpTexts); 0 at Create
  uint32_t ready_address() const { return address + 0; }
  uint32_t ready() const { return data()->ready; }
  void set_ready(uint32_t v) const { data()->ready = v; }
  uint32_t ready_bits() const { return data()->ready; }
  void set_ready_bits(uint32_t v) const { data()->ready = v; }
  // the first images' list (46 record pointers, the count at the data's +188); assets 22696 + i
  template <typename T = TodaysEventImageList>
  T images_a() const { return T(base, address + 4); }
  // the second images' list (the count at the data's +376); assets 22741 + i
  template <typename T = TodaysEventImageList>
  T images_b() const { return T(base, address + 192); }
  // list A's count
  uint32_t image_count_a_address() const { return address + 188; }
  uint32_t image_count_a() const { return data()->image_count_a; }
  void set_image_count_a(uint32_t v) const { data()->image_count_a = v; }
  uint32_t image_count_a_bits() const { return data()->image_count_a; }
  void set_image_count_a_bits(uint32_t v) const { data()->image_count_a = v; }
  // list B's count
  uint32_t image_count_b_address() const { return address + 376; }
  uint32_t image_count_b() const { return data()->image_count_b; }
  void set_image_count_b(uint32_t v) const { data()->image_count_b = v; }
  uint32_t image_count_b_bits() const { return data()->image_count_b; }
  void set_image_count_b_bits(uint32_t v) const { data()->image_count_b = v; }
  // the images to load: bit i of 46 (MarkImages)
  uint32_t image_bits_0_address() const { return address + 380; }
  uint32_t image_bits_0() const { return data()->image_bits_0; }
  void set_image_bits_0(uint32_t v) const { data()->image_bits_0 = v; }
  uint32_t image_bits_0_bits() const { return data()->image_bits_0; }
  void set_image_bits_0_bits(uint32_t v) const { data()->image_bits_0 = v; }
  uint32_t image_bits_1_address() const { return address + 384; }
  uint32_t image_bits_1() const { return data()->image_bits_1; }
  void set_image_bits_1(uint32_t v) const { data()->image_bits_1 = v; }
  uint32_t image_bits_1_bits() const { return data()->image_bits_1; }
  void set_image_bits_1_bits(uint32_t v) const { data()->image_bits_1 = v; }
  // the unlock entry shown: a copy of the save's 12-byte entry
  uint32_t unlock_shown_address() const { return address + 388; }
  uint8_t* unlock_shown() const { return reinterpret_cast<uint8_t*>(data()) + 388; }
  // the entry's +0 (its shown word)
  uint32_t unlock_word_address() const { return address + 388; }
  uint32_t unlock_word() const { return data()->unlock_word; }
  void set_unlock_word(uint32_t v) const { data()->unlock_word = v; }
  uint32_t unlock_word_bits() const { return data()->unlock_word; }
  void set_unlock_word_bits(uint32_t v) const { data()->unlock_word = v; }
  // the data table
  uint32_t unlock_table_address() const { return address + 392; }
  uint32_t unlock_table() const { return data()->unlock_table; }
  void set_unlock_table(uint32_t v) const { data()->unlock_table = v; }
  uint32_t unlock_table_bits() const { return data()->unlock_table; }
  void set_unlock_table_bits(uint32_t v) const { data()->unlock_table = v; }
  // its 56-byte entry, -1 none; moved along the entry's pages
  uint32_t unlock_entry_address() const { return address + 396; }
  uint32_t unlock_entry() const { return data()->unlock_entry; }
  void set_unlock_entry(uint32_t v) const { data()->unlock_entry = v; }
  uint32_t unlock_entry_bits() const { return data()->unlock_entry; }
  void set_unlock_entry_bits(uint32_t v) const { data()->unlock_entry = v; }
  // movie 0's +28 object's +36 as Start found it (put back by Unload and Leave)
  uint32_t movie_object_36_address() const { return address + 400; }
  uint32_t movie_object_36() const { return data()->movie_object_36; }
  void set_movie_object_36(uint32_t v) const { data()->movie_object_36 = v; }
  uint32_t movie_object_36_bits() const { return data()->movie_object_36; }
  void set_movie_object_36_bits(uint32_t v) const { data()->movie_object_36 = v; }
  // movie 0's +28 object's +32 as Start found it
  uint32_t movie_object_32_address() const { return address + 404; }
  uint32_t movie_object_32() const { return data()->movie_object_32; }
  void set_movie_object_32(uint32_t v) const { data()->movie_object_32 = v; }
  uint32_t movie_object_32_bits() const { return data()->movie_object_32; }
  void set_movie_object_32_bits(uint32_t v) const { data()->movie_object_32 = v; }
  // 1 when EndPage queued movies; Destroy then goes to state 20
  uint32_t movies_queued_address() const { return address + 408; }
  uint32_t movies_queued() const { return data()->movies_queued; }
  void set_movies_queued(uint32_t v) const { data()->movies_queued = v; }
  uint32_t movies_queued_bits() const { return data()->movies_queued; }
  void set_movies_queued_bits(uint32_t v) const { data()->movies_queued = v; }
};

// embedded; measure: -
#pragma pack(push, 1)
struct TodaysEventImageListData {
  uint8_t records[184] = {};  // +0, guest order
  uint32_t count = 0;  // +184
};
#pragma pack(pop)
static_assert(sizeof(TodaysEventImageListData) == 188);
inline constexpr memory::LayoutField kTodaysEventImageListFields[] = {{184, 4}};
inline constexpr memory::Layout kTodaysEventImageListLayout = {kTodaysEventImageListFields, 1};

struct TodaysEventImageList : View {
  using View::View;
  static constexpr uint32_t kSize = 188;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TodaysEventImageListData* data() const { return static_cast<TodaysEventImageListData*>(memory::HandleObject(address)); }
  static TodaysEventImageList New(uint8_t* base) {
    return TodaysEventImageList(base, memory::MakeHandle(new TodaysEventImageListData(), sizeof(TodaysEventImageListData), "TodaysEventImageList", &kTodaysEventImageListLayout));
  }
  template <uint32_t Stride = kSize>
  static TodaysEventImageList NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TodaysEventImageListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TodaysEventImageListData*>(::operator new(sizeof(TodaysEventImageListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TodaysEventImageListData();
    return TodaysEventImageList(base, memory::MakeHandle(p, sizeof(TodaysEventImageListData) * count, "TodaysEventImageList", &kTodaysEventImageListLayout, sizeof(TodaysEventImageListData)));
  }
  TodaysEventImageList at(uint32_t i) const { return TodaysEventImageList(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kTodaysEventImageListLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kTodaysEventImageListLayout, offset, width, v); }
  // 46 Type2HubImage pointers (type2-screens.toml)
  uint32_t records_address() const { return address + 0; }
  uint8_t* records() const { return reinterpret_cast<uint8_t*>(data()) + 0; }
  uint32_t count_address() const { return address + 184; }
  uint32_t count() const { return data()->count; }
  void set_count(uint32_t v) const { data()->count = v; }
  uint32_t count_bits() const { return data()->count; }
  void set_count_bits(uint32_t v) const { data()->count = v; }
};

// The image words docs/functions/todays-result.md's code uses.
struct TodaysResultGlobals {
  // today's result's page object (28 bytes)
  template <typename T = PageObject>
  static T result_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160C8u)); }
  static void set_result_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160C8u, v.address); }
  static void set_result_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160C8u, v); }
  static constexpr uint32_t result_pages_address() { return 0x828160C8u; }
  // today's result's menu (68 bytes; +16 = 4)
  template <typename T = FrontEndMenu>
  static T result_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160CCu)); }
  static void set_result_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160CCu, v.address); }
  static void set_result_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160CCu, v); }
  static constexpr uint32_t result_menu_address() { return 0x828160CCu; }
  template <typename T = TodaysResultData>
  static T result_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160D0u)); }
  static void set_result_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160D0u, v.address); }
  static void set_result_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160D0u, v); }
  static constexpr uint32_t result_data_address() { return 0x828160D0u; }
  // today's event's page object
  template <typename T = PageObject>
  static T event_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160D4u)); }
  static void set_event_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160D4u, v.address); }
  static void set_event_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160D4u, v); }
  static constexpr uint32_t event_pages_address() { return 0x828160D4u; }
  // today's event's menu
  template <typename T = FrontEndMenu>
  static T event_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160D8u)); }
  static void set_event_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160D8u, v.address); }
  static void set_event_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160D8u, v); }
  static constexpr uint32_t event_menu_address() { return 0x828160D8u; }
  template <typename T = TodaysEventData>
  static T event_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160DCu)); }
  static void set_event_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160DCu, v.address); }
  static void set_event_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160DCu, v); }
  static constexpr uint32_t event_data_address() { return 0x828160DCu; }
  // the string table Strings::Load gets (file 171)
  static uint32_t string_table(uint8_t* base) { return guest::LoadU32(base, 0x826C5C34u); }
  static void set_string_table(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t string_table_address() { return 0x826C5C34u; }
  // the passes word ScreenFades::FadeIn is given
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // the passes word ScreenFades::FadeOut is given
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // the fades' counts the FadeOut transitions wait on
  static uint32_t fade_count_0(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_count_0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_count_0_address() { return 0x826A50E4u; }
  static uint32_t fade_count_1(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_count_1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_count_1_address() { return 0x826A50DCu; }
  // the boot movies' five ids (11 ending the list)
  static constexpr uint32_t boot_movie_ids_address() { return 0x826C5BACu; }
  // the mode type after the boot movies' list
  static uint32_t boot_next_type(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC0u); }
  static void set_boot_next_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC0u, v); }
  static constexpr uint32_t boot_next_type_address() { return 0x826C5BC0u; }
  static uint32_t boot_skippable(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC4u); }
  static void set_boot_skippable(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC4u, v); }
  static constexpr uint32_t boot_skippable_address() { return 0x826C5BC4u; }
  static uint32_t boot_movie_index(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC8u); }
  static void set_boot_movie_index(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC8u, v); }
  static constexpr uint32_t boot_movie_index_address() { return 0x826C5BC8u; }
  // the sound manager (AudioManager): slot 14 IsCuePlaying, 13 StopCue
  static uint32_t sound_manager(uint8_t* base) { return guest::LoadU32(base, 0x826C5C80u); }
  static void set_sound_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C80u, v); }
  static constexpr uint32_t sound_manager_address() { return 0x826C5C80u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 100.0
  static float hundred(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_hundred(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t hundred_address() { return 0x82002A0Cu; }
};

}  // namespace schema
