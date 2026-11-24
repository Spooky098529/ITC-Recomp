// The views of docs/functions/type10-mode.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type10-mode.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ModeType10, ModeType10::WaitForLoads.r4, ModeType10::WaitForShown.r4, ModeType10::LeaveScreen.r4, ModeType10::NextMode.r4, ModeType10::Leaving.r4, ModeType9::NextModeAnswer.r4
#pragma pack(push, 1)
struct ModeType10Data {
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
static_assert(sizeof(ModeType10Data) == 852);
inline constexpr memory::LayoutField kModeType10Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kModeType10Layout = {kModeType10Fields, 11};

struct ModeType10 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x82006CF0;
  ModeType10Data* data() const { return static_cast<ModeType10Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeType10 New(uint8_t* base) {
    return ModeType10(base, memory::MakeHandle(new ModeType10Data(), sizeof(ModeType10Data), "ModeType10", &kModeType10Layout));
  }
  template <uint32_t Stride = kSize>
  static ModeType10 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeType10Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType10Data*>(::operator new(sizeof(ModeType10Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType10Data();
    return ModeType10(base, memory::MakeHandle(p, sizeof(ModeType10Data) * count, "ModeType10", &kModeType10Layout, sizeof(ModeType10Data)));
  }
  ModeType10 at(uint32_t i) const { return ModeType10(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeType10Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006CF0 (14 slots)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer, the delegate functions' r3), not the mode's
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { return on_host() ? uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 0, 1)) : guest::LoadU8(base, address + 0); }
  void set_answer_changed_bits(uint8_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the next-mode answer's 8 bytes (ModeAnswer)
  uint32_t answer_address() const { return address + 0; }
  uint64_t answer_bits() const { return on_host() ? uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 0, 8)) : guest::LoadU64(base, address + 0); }
  void set_answer_bits(uint64_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  uint64_t answer() const { return uint64_t(answer_bits()); }
  void set_answer(uint64_t v) const { set_answer_bits(uint64_t(v)); }
  // the delegate at +8; 0 when its address wraps (never). Also the answer's next type +4 by the measure's join
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
  // Mode::Slot8Thunk (WaitForLoads) after Init, then NextModeThunk, Slot10Thunk, Slot11Thunk, Slot12Thunk or Slot13Thunk
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function() const { return on_host() ? uint32_t(data()->delegate_function) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_delegate_function(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_bits() const { return on_host() ? data()->delegate_function : guest::LoadU32(base, address + 16); }
  void set_delegate_function_bits(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  // 0
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment() const { return on_host() ? uint32_t(data()->delegate_adjustment) : uint32_t(guest::LoadU32(base, address + 20)); }
  void set_delegate_adjustment(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_bits() const { return on_host() ? data()->delegate_adjustment : guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  // 0x82006CE4: Interrupt, ModeType9::ForceExit, Resume
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32() const { return on_host() ? uint32_t(data()->vtable32) : uint32_t(guest::LoadU32(base, address + 32)); }
  void set_vtable32(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_bits() const { return on_host() ? data()->vtable32 : guest::LoadU32(base, address + 32); }
  void set_vtable32_bits(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (the theater, then the save screen)
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
  // VirtualAlloc'ed by Init (19,300 bytes), freed by Release
  uint32_t block_address() const { return address + 848; }
  template <typename T = ModeType10Block>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 848)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 848); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  // measure fault: the +32 interface's +808 (ModeType10Interface32), the mode's +840
  uint32_t fault_interface_ring_count_address() const { return address + 808; }
  uint32_t fault_interface_ring_count_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 808, 4)) : guest::LoadU32(base, address + 808); }
  void set_fault_interface_ring_count_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t fault_interface_ring_count() const { return uint32_t(fault_interface_ring_count_bits()); }
  void set_fault_interface_ring_count(uint32_t v) const { set_fault_interface_ring_count_bits(uint32_t(v)); }
  // measure fault: the +32 interface's +812, the mode's +844
  uint32_t fault_interface_ring_start_address() const { return address + 812; }
  uint32_t fault_interface_ring_start_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 812, 4)) : guest::LoadU32(base, address + 812); }
  void set_fault_interface_ring_start_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 812, 4, v); else guest::StoreU32(base, address + 812, v); }
  uint32_t fault_interface_ring_start() const { return uint32_t(fault_interface_ring_start_bits()); }
  void set_fault_interface_ring_start(uint32_t v) const { set_fault_interface_ring_start_bits(uint32_t(v)); }
  // measure fault: the +32 interface's +816, the mode's +848
  uint32_t fault_interface_block_address() const { return address + 816; }
  uint32_t fault_interface_block_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 816, 4)) : guest::LoadU32(base, address + 816); }
  void set_fault_interface_block_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Layout, 816, 4, v); else guest::StoreU32(base, address + 816, v); }
  uint32_t fault_interface_block() const { return uint32_t(fault_interface_block_bits()); }
  void set_fault_interface_block(uint32_t v) const { set_fault_interface_block_bits(uint32_t(v)); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct ModeType10Interface32Data {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[204] = {};
  uint8_t ring[600] = {};  // +208, guest order
  uint32_t ring_count = 0;  // +808
  uint32_t ring_start = 0;  // +812
  uint32_t block = 0;  // +816
};
#pragma pack(pop)
static_assert(sizeof(ModeType10Interface32Data) == 820);
inline constexpr memory::LayoutField kModeType10Interface32Fields[] = {{0, 4}, {808, 4}, {812, 4}, {816, 4}};
inline constexpr memory::Layout kModeType10Interface32Layout = {kModeType10Interface32Fields, 4};

struct ModeType10Interface32 : View {
  using View::View;
  static constexpr uint32_t kSize = 820;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ModeType10Interface32Data* data() const { return static_cast<ModeType10Interface32Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeType10Interface32 New(uint8_t* base) {
    return ModeType10Interface32(base, memory::MakeHandle(new ModeType10Interface32Data(), sizeof(ModeType10Interface32Data), "ModeType10Interface32", &kModeType10Interface32Layout));
  }
  template <uint32_t Stride = kSize>
  static ModeType10Interface32 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeType10Interface32Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType10Interface32Data*>(::operator new(sizeof(ModeType10Interface32Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType10Interface32Data();
    return ModeType10Interface32(base, memory::MakeHandle(p, sizeof(ModeType10Interface32Data) * count, "ModeType10Interface32", &kModeType10Interface32Layout, sizeof(ModeType10Interface32Data)));
  }
  ModeType10Interface32 at(uint32_t i) const { return ModeType10Interface32(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kModeType10Interface32Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kModeType10Interface32Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006CE4
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // the mode's ring (+240)
  uint32_t ring_address() const { return address + 208; }
  uint8_t* ring() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 208 : guest::Pointer(base, address + 208); }
  // the mode's +840
  uint32_t ring_count_address() const { return address + 808; }
  uint32_t ring_count() const { return on_host() ? uint32_t(data()->ring_count) : uint32_t(guest::LoadU32(base, address + 808)); }
  void set_ring_count(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 808, v); }
  uint32_t ring_count_bits() const { return on_host() ? data()->ring_count : guest::LoadU32(base, address + 808); }
  void set_ring_count_bits(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 808, v); }
  // the mode's +844
  uint32_t ring_start_address() const { return address + 812; }
  uint32_t ring_start() const { return on_host() ? uint32_t(data()->ring_start) : uint32_t(guest::LoadU32(base, address + 812)); }
  void set_ring_start(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 812, v); }
  uint32_t ring_start_bits() const { return on_host() ? data()->ring_start : guest::LoadU32(base, address + 812); }
  void set_ring_start_bits(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 812, v); }
  // the mode's +848
  uint32_t block_address() const { return address + 816; }
  template <typename T = ModeType10Block>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 816)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 816, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 816); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 816, v); }
};

// allocated; measure: new<19300>@ModeType10::Init, ModeType10.+816, ModeType10::WaitForLoads.r4.+848, ModeType10::WaitForShown.r4.+848, ModeType10::NextMode.r4.+848, ModeType10::Leaving.r4.+848, ModeType9::NextModeAnswer.r4.+848
struct ModeType10Block : View {
  using View::View;
  static constexpr uint32_t kSize = 19300;
  // the replay theater's machine (ReplayTheaterMachine)
  uint32_t theater_address() const { return address + 0; }
  uint32_t theater_state() const { return guest::LoadU32(base, address + 9604); }
  void set_theater_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t theater_state_address() const { return address + 9604; }
  // movie 45 loaded
  uint32_t theater_loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_theater_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t theater_loaded_address() const { return address + 9608; }
  // 1 at Init: open on message 5
  uint32_t theater_reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_theater_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t theater_reenter_address() const { return address + 9612; }
  // 0 at Init
  uint32_t theater_delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_theater_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t theater_delete_movie_address() const { return address + 9616; }
  // the save screen's machine (Type10SaveScreen::Init, save-screen.md)
  uint32_t save_screen_address() const { return address + 9640; }
  // 5 shown (Interrupt)
  uint32_t save_screen_state() const { return guest::LoadU32(base, address + 19244); }
  void set_save_screen_state(uint32_t v) const { guest::StoreU32(base, address + 19244, v); }
  uint32_t save_screen_state_address() const { return address + 19244; }
  uint32_t save_screen_loaded() const { return guest::LoadU32(base, address + 19248); }
  void set_save_screen_loaded(uint32_t v) const { guest::StoreU32(base, address + 19248, v); }
  uint32_t save_screen_loaded_address() const { return address + 19248; }
  // 1 at Init
  uint32_t save_screen_reenter() const { return guest::LoadU32(base, address + 19252); }
  void set_save_screen_reenter(uint32_t v) const { guest::StoreU32(base, address + 19252, v); }
  uint32_t save_screen_reenter_address() const { return address + 19252; }
  // 0 at Init
  uint32_t save_screen_delete_movie() const { return guest::LoadU32(base, address + 19256); }
  void set_save_screen_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 19256, v); }
  uint32_t save_screen_delete_movie_address() const { return address + 19256; }
  // the shown machine's index: 0 the theater, 1 the save screen (FindShown)
  uint32_t shown() const { return guest::LoadU32(base, address + 19280); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 19280, v); }
  uint32_t shown_address() const { return address + 19280; }
  // 0 at Init (readers not looked for)
  uint32_t shown_at_init() const { return guest::LoadU32(base, address + 19284); }
  void set_shown_at_init(uint32_t v) const { guest::StoreU32(base, address + 19284, v); }
  uint32_t shown_at_init_address() const { return address + 19284; }
  // the next mode's 8-byte answer ForceExit writes and NextModeAnswer hands to Modes::Tick (ModeAnswer)
  uint64_t answer() const { return guest::LoadU64(base, address + 19292); }
  void set_answer(uint64_t v) const { guest::StoreU64(base, address + 19292, v); }
  uint32_t answer_address() const { return address + 19292; }
  // the answer's first word (changed and three bytes), as NextModeAnswer copies it
  uint32_t answer_word0() const { return guest::LoadU32(base, address + 19292); }
  void set_answer_word0(uint32_t v) const { guest::StoreU32(base, address + 19292, v); }
  uint32_t answer_word0_address() const { return address + 19292; }
  // the answer's next mode type (4 or 12 from ForceExit)
  uint32_t answer_next_type() const { return guest::LoadU32(base, address + 19296); }
  void set_answer_next_type(uint32_t v) const { guest::StoreU32(base, address + 19296, v); }
  uint32_t answer_next_type_address() const { return address + 19296; }
};

// embedded; measure: ReplayTheater
struct ReplayTheaterMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  // measure fault: the transitions' r3 (PageObject.step_started), and ShowRecord's record's word_0 (SaveReplayRecord)
  uint32_t step_started() const { return guest::LoadU32(base, address + 0); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t step_started_address() const { return address + 0; }
  // measure fault: ShowRecord's record (SaveReplayRecord.year)
  uint16_t record_year() const { return guest::LoadU16(base, address + 0); }
  void set_record_year(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t record_year_address() const { return address + 0; }
  // measure fault: SaveReplayRecord.month
  uint16_t record_month() const { return guest::LoadU16(base, address + 2); }
  void set_record_month(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t record_month_address() const { return address + 2; }
  // measure fault: SaveReplayRecord.day
  uint16_t record_day() const { return guest::LoadU16(base, address + 6); }
  void set_record_day(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t record_day_address() const { return address + 6; }
  // measure fault: SaveReplayRecord.hour
  uint16_t record_hour() const { return guest::LoadU16(base, address + 8); }
  void set_record_hour(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t record_hour_address() const { return address + 8; }
  // measure fault: SaveReplayRecord.minute
  uint16_t record_minute() const { return guest::LoadU16(base, address + 10); }
  void set_record_minute(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t record_minute_address() const { return address + 10; }
  // measure fault: SaveReplayRecord.time
  uint32_t record_time() const { return guest::LoadU32(base, address + 16); }
  void set_record_time(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t record_time_address() const { return address + 16; }
  // measure fault: SaveReplayRecord.mode
  uint32_t record_mode() const { return guest::LoadU32(base, address + 20); }
  void set_record_mode(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t record_mode_address() const { return address + 20; }
  // measure fault: SaveReplayRecord.course
  uint32_t record_course() const { return guest::LoadU32(base, address + 24); }
  void set_record_course(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t record_course_address() const { return address + 24; }
  // measure fault: SaveReplayRecord.number
  uint32_t record_number() const { return guest::LoadU32(base, address + 28); }
  void set_record_number(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t record_number_address() const { return address + 28; }
  // measure fault: SaveReplayRecord.result
  uint32_t record_result() const { return guest::LoadU32(base, address + 32); }
  void set_record_result(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t record_result_address() const { return address + 32; }
  // measure fault: the pages' r3 is the dispatcher (ModeDispatcher.ring_count)
  uint32_t ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count_address() const { return address + 804; }
  // measure fault: the pages' r3 is the dispatcher (ModeDispatcher.ring_start)
  uint32_t ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start_address() const { return address + 808; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 or 9 at Create, 4 to start, 5 running, 7 leaving, 8 unloading, 9 waiting, 11 the race (a replay loaded), 14 the title (Destroy)
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movie 45 loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForMovie go to state 9 (open on message 5)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movie 45 and clears loaded
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// allocated; measure: new<16>@ReplayTheater::Create
#pragma pack(push, 1)
struct ReplayTheaterDataData {
  uint32_t started = 0;  // +0
  uint32_t asked = 0;  // +4
  uint32_t replay_loaded = 0;  // +8
  uint32_t interrupted = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(ReplayTheaterDataData) == 16);
inline constexpr memory::LayoutField kReplayTheaterDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kReplayTheaterDataLayout = {kReplayTheaterDataFields, 4};

struct ReplayTheaterData : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ReplayTheaterDataData* data() const { return static_cast<ReplayTheaterDataData*>(memory::HandleObject(address)); }
  static ReplayTheaterData New(uint8_t* base) {
    return ReplayTheaterData(base, memory::MakeHandle(new ReplayTheaterDataData(), sizeof(ReplayTheaterDataData), "ReplayTheaterData", &kReplayTheaterDataLayout));
  }
  template <uint32_t Stride = kSize>
  static ReplayTheaterData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ReplayTheaterDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ReplayTheaterDataData*>(::operator new(sizeof(ReplayTheaterDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ReplayTheaterDataData();
    return ReplayTheaterData(base, memory::MakeHandle(p, sizeof(ReplayTheaterDataData) * count, "ReplayTheaterData", &kReplayTheaterDataLayout, sizeof(ReplayTheaterDataData)));
  }
  ReplayTheaterData at(uint32_t i) const { return ReplayTheaterData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kReplayTheaterDataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kReplayTheaterDataLayout, offset, width, v); }
  // 0 from Create, 1 from StartPage
  uint32_t started_address() const { return address + 0; }
  uint32_t started() const { return data()->started; }
  void set_started(uint32_t v) const { data()->started = v; }
  uint32_t started_bits() const { return data()->started; }
  void set_started_bits(uint32_t v) const { data()->started = v; }
  // what was asked of the save screen: 0 the list, 1 a replay's load, 2 the device lost, 3 back, -1 after an interrupt
  uint32_t asked_address() const { return address + 4; }
  uint32_t asked() const { return data()->asked; }
  void set_asked(uint32_t v) const { data()->asked = v; }
  uint32_t asked_bits() const { return data()->asked; }
  void set_asked_bits(uint32_t v) const { data()->asked = v; }
  // 1 once a replay is loaded and the race set up from it; Destroy then goes to state 11 (the race)
  uint32_t replay_loaded_address() const { return address + 8; }
  uint32_t replay_loaded() const { return data()->replay_loaded; }
  void set_replay_loaded(uint32_t v) const { data()->replay_loaded = v; }
  uint32_t replay_loaded_bits() const { return data()->replay_loaded; }
  void set_replay_loaded_bits(uint32_t v) const { data()->replay_loaded = v; }
  // set by message 15 (OnInterrupt), cleared by 16 (OnResume)
  uint32_t interrupted_address() const { return address + 12; }
  uint32_t interrupted() const { return data()->interrupted; }
  void set_interrupted(uint32_t v) const { data()->interrupted = v; }
  uint32_t interrupted_bits() const { return data()->interrupted; }
  void set_interrupted_bits(uint32_t v) const { data()->interrupted = v; }
};

// allocated; measure: Replay
struct KeptReplay : View {
  using View::View;
  static constexpr uint32_t kSize = 1296624;
  // the race data's +4 (the course)
  uint32_t race_word_4() const { return guest::LoadU32(base, address + 0); }
  void set_race_word_4(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t race_word_4_address() const { return address + 0; }
  // the race data's +28
  uint32_t race_word_28() const { return guest::LoadU32(base, address + 4); }
  void set_race_word_28(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t race_word_28_address() const { return address + 4; }
  // the race data's +32 (the mode)
  uint32_t race_word_32() const { return guest::LoadU32(base, address + 8); }
  void set_race_word_32(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t race_word_32_address() const { return address + 8; }
  // the record word +12 of the four racers the stage lists (+12 to +24)
  uint32_t racer_record_words_address() const { return address + 12; }
  // the first of them
  uint32_t racer_record_word_12() const { return guest::LoadU32(base, address + 12); }
  void set_racer_record_word_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t racer_record_word_12_address() const { return address + 12; }
  // the stage's word +0
  uint32_t stage_word_0() const { return guest::LoadU32(base, address + 28); }
  void set_stage_word_0(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t stage_word_0_address() const { return address + 28; }
  // five words of player 0's save data from +8 (or defaults), +32 to +51: the first
  uint32_t save_pick() const { return guest::LoadU32(base, address + 32); }
  void set_save_pick(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t save_pick_address() const { return address + 32; }
  // the pick's first byte
  uint8_t save_pick_flag() const { return guest::LoadU8(base, address + 32); }
  void set_save_pick_flag(uint8_t v) const { guest::StoreU8(base, address + 32, v); }
  uint32_t save_pick_flag_address() const { return address + 32; }
  uint32_t save_pick_4() const { return guest::LoadU32(base, address + 36); }
  void set_save_pick_4(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t save_pick_4_address() const { return address + 36; }
  uint16_t save_pick_8() const { return guest::LoadU16(base, address + 40); }
  void set_save_pick_8(uint16_t v) const { guest::StoreU16(base, address + 40, v); }
  uint32_t save_pick_8_address() const { return address + 40; }
  uint8_t save_pick_10() const { return guest::LoadU8(base, address + 42); }
  void set_save_pick_10(uint8_t v) const { guest::StoreU8(base, address + 42, v); }
  uint32_t save_pick_10_address() const { return address + 42; }
  uint32_t save_pick_12() const { return guest::LoadU32(base, address + 44); }
  void set_save_pick_12(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t save_pick_12_address() const { return address + 44; }
  uint16_t save_pick_16() const { return guest::LoadU16(base, address + 48); }
  void set_save_pick_16(uint16_t v) const { guest::StoreU16(base, address + 48, v); }
  uint32_t save_pick_16_address() const { return address + 48; }
  uint8_t save_pick_18() const { return guest::LoadU8(base, address + 50); }
  void set_save_pick_18(uint8_t v) const { guest::StoreU8(base, address + 50, v); }
  uint32_t save_pick_18_address() const { return address + 50; }
  // the race data's +4 to +35 (to +83)
  uint32_t race_words_address() const { return address + 52; }
  // the race data's +0x59610 (to +0x5961C at +96)
  uint32_t race_word_59610() const { return guest::LoadU32(base, address + 84); }
  void set_race_word_59610(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t race_word_59610_address() const { return address + 84; }
  uint32_t race_word_59614() const { return guest::LoadU32(base, address + 88); }
  void set_race_word_59614(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t race_word_59614_address() const { return address + 88; }
  uint32_t race_word_59618() const { return guest::LoadU32(base, address + 92); }
  void set_race_word_59618(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t race_word_59618_address() const { return address + 92; }
  uint32_t race_word_5961C() const { return guest::LoadU32(base, address + 96); }
  void set_race_word_5961C(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t race_word_5961C_address() const { return address + 96; }
  // the 93 racer entries (1,908 bytes each from +100): entry 0's flags
  uint32_t racer_flags() const { return guest::LoadU32(base, address + 100); }
  void set_racer_flags(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t racer_flags_address() const { return address + 100; }
  // entry 0's +3892 (then the record's first 1,884 bytes from +108)
  uint32_t racer_word_3892() const { return guest::LoadU32(base, address + 104); }
  void set_racer_word_3892(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t racer_word_3892_address() const { return address + 104; }
  // entry 0's start position
  uint16_t racer_start_position() const { return guest::LoadU16(base, address + 1992); }
  void set_racer_start_position(uint16_t v) const { guest::StoreU16(base, address + 1992, v); }
  uint32_t racer_start_position_address() const { return address + 1992; }
  uint16_t racer_half_1894() const { return guest::LoadU16(base, address + 1994); }
  void set_racer_half_1894(uint16_t v) const { guest::StoreU16(base, address + 1994, v); }
  uint32_t racer_half_1894_address() const { return address + 1994; }
  // entry 0's +3908
  float racer_word_3908() const { return guest::LoadFloat(base, address + 1996); }
  void set_racer_word_3908(float v) const { guest::StoreFloat(base, address + 1996, v); }
  uint32_t racer_word_3908_address() const { return address + 1996; }
  // entry 0's racer id
  uint32_t racer_id() const { return guest::LoadU32(base, address + 2000); }
  void set_racer_id(uint32_t v) const { guest::StoreU32(base, address + 2000, v); }
  uint32_t racer_id_address() const { return address + 2000; }
  // entry 0's +3920
  float racer_word_3920() const { return guest::LoadFloat(base, address + 2004); }
  void set_racer_word_3920(float v) const { guest::StoreFloat(base, address + 2004, v); }
  uint32_t racer_word_3920_address() const { return address + 2004; }
  // the stage (80 bytes from +0x2B588): its first doubleword (seven copied by SetUpRace)
  uint64_t stage() const { return guest::LoadU64(base, address + 177544); }
  void set_stage(uint64_t v) const { guest::StoreU64(base, address + 177544, v); }
  uint32_t stage_address() const { return address + 177544; }
  // the stage's first word
  uint32_t stage_word_0_2B588() const { return guest::LoadU32(base, address + 177544); }
  void set_stage_word_0_2B588(uint32_t v) const { guest::StoreU32(base, address + 177544, v); }
  uint32_t stage_word_0_2B588_address() const { return address + 177544; }
  uint64_t stage_8() const { return guest::LoadU64(base, address + 177552); }
  void set_stage_8(uint64_t v) const { guest::StoreU64(base, address + 177552, v); }
  uint32_t stage_8_address() const { return address + 177552; }
  uint64_t stage_16() const { return guest::LoadU64(base, address + 177560); }
  void set_stage_16(uint64_t v) const { guest::StoreU64(base, address + 177560, v); }
  uint32_t stage_16_address() const { return address + 177560; }
  uint64_t stage_24() const { return guest::LoadU64(base, address + 177568); }
  void set_stage_24(uint64_t v) const { guest::StoreU64(base, address + 177568, v); }
  uint32_t stage_24_address() const { return address + 177568; }
  uint64_t stage_32() const { return guest::LoadU64(base, address + 177576); }
  void set_stage_32(uint64_t v) const { guest::StoreU64(base, address + 177576, v); }
  uint32_t stage_32_address() const { return address + 177576; }
  uint64_t stage_40() const { return guest::LoadU64(base, address + 177584); }
  void set_stage_40(uint64_t v) const { guest::StoreU64(base, address + 177584, v); }
  uint32_t stage_40_address() const { return address + 177584; }
  uint32_t stage_word_44() const { return guest::LoadU32(base, address + 177588); }
  void set_stage_word_44(uint32_t v) const { guest::StoreU32(base, address + 177588, v); }
  uint32_t stage_word_44_address() const { return address + 177588; }
  uint64_t stage_48() const { return guest::LoadU64(base, address + 177592); }
  void set_stage_48(uint64_t v) const { guest::StoreU64(base, address + 177592, v); }
  uint32_t stage_48_address() const { return address + 177592; }
  // the last kept result's +3916 (to +3928 at +0x2B5E4)
  uint32_t result_3916() const { return guest::LoadU32(base, address + 177624); }
  void set_result_3916(uint32_t v) const { guest::StoreU32(base, address + 177624, v); }
  uint32_t result_3916_address() const { return address + 177624; }
  uint32_t result_3920() const { return guest::LoadU32(base, address + 177628); }
  void set_result_3920(uint32_t v) const { guest::StoreU32(base, address + 177628, v); }
  uint32_t result_3920_address() const { return address + 177628; }
  uint32_t result_3924() const { return guest::LoadU32(base, address + 177632); }
  void set_result_3924(uint32_t v) const { guest::StoreU32(base, address + 177632, v); }
  uint32_t result_3924_address() const { return address + 177632; }
  uint32_t result_3928() const { return guest::LoadU32(base, address + 177636); }
  void set_result_3928(uint32_t v) const { guest::StoreU32(base, address + 177636, v); }
  uint32_t result_3928_address() const { return address + 177636; }
  // the race data's +0xA3AE0
  uint32_t race_word_A3AE0() const { return guest::LoadU32(base, address + 177640); }
  void set_race_word_A3AE0(uint32_t v) const { guest::StoreU32(base, address + 177640, v); }
  uint32_t race_word_A3AE0_address() const { return address + 177640; }
  // five words from the race data's +0xA3AE4: the first
  uint32_t race_words_A3AE4() const { return guest::LoadU32(base, address + 177644); }
  void set_race_words_A3AE4(uint32_t v) const { guest::StoreU32(base, address + 177644, v); }
  uint32_t race_words_A3AE4_address() const { return address + 177644; }
  // +0x2B600: 1 from Replay::Create, 0 once filled; SetUpRace returns 0 at once when nonzero
  uint32_t unfilled() const { return guest::LoadU32(base, address + 177664); }
  void set_unfilled(uint32_t v) const { guest::StoreU32(base, address + 177664, v); }
  uint32_t unfilled_address() const { return address + 177664; }
  // +0x2B610: a race snapshot (replay-snapshots.toml's ReplaySnapshot, block A's copy)
  uint32_t snapshot_address() const { return address + 177680; }
  // +0x34E20: player 0's input recording with its count (0x107AC4 bytes)
  uint32_t recording_address() const { return address + 216608; }
};

// The image words docs/functions/type10-mode.md's code uses.
struct Type10ModeGlobals {
  // the theater's page object (28 bytes)
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161E0u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161E0u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161E0u, v); }
  static constexpr uint32_t pages_address() { return 0x828161E0u; }
  // the replay list (72 bytes, a menu list)
  template <typename T = MenuList>
  static T list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161E4u)); }
  static void set_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161E4u, v.address); }
  static void set_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161E4u, v); }
  static constexpr uint32_t list_address() { return 0x828161E4u; }
  template <typename T = ReplayTheaterData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161E8u)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161E8u, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161E8u, v); }
  static constexpr uint32_t data_address() { return 0x828161E8u; }
  // the save layer's header (+12 the replays' 36-byte records, +20 their count)
  template <typename T = SaveBuffers>
  static T save_buffers(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161F0u)); }
  static void set_save_buffers(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161F0u, v.address); }
  static void set_save_buffers(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161F0u, v); }
  static constexpr uint32_t save_buffers_address() { return 0x828161F0u; }
  // the save screen's data (+40 the replay picked, +56 the prompts WaitPage shows)
  template <typename T = SaveScreenData>
  static T save_screen_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8454u)); }
  static void set_save_screen_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8454u, v.address); }
  static void set_save_screen_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8454u, v); }
  static constexpr uint32_t save_screen_data_address() { return 0x826D8454u; }
  // how the title is entered: NextMode's 14 clears it, ForceExit's 0 and 1 set it
  static uint32_t entered(uint8_t* base) { return guest::LoadU32(base, 0x826364ECu); }
  static void set_entered(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826364ECu, v); }
  static constexpr uint32_t entered_address() { return 0x826364ECu; }
  // the pad blocked (the system UI, probably): either word set
  static uint32_t pad_blocked0(uint8_t* base) { return guest::LoadU32(base, 0x826C7960u); }
  static void set_pad_blocked0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7960u, v); }
  static constexpr uint32_t pad_blocked0_address() { return 0x826C7960u; }
  static uint32_t pad_blocked1(uint8_t* base) { return guest::LoadU32(base, 0x826C7964u); }
  static void set_pad_blocked1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7964u, v); }
  static constexpr uint32_t pad_blocked1_address() { return 0x826C7964u; }
  // the race data (a pointer) OnDone hands to Replay::SetUpRace
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the input recordings (a pointer) OnDone hands to Replay::SetUpRace
  static uint32_t recordings(uint8_t* base) { return guest::LoadU32(base, 0x826C5B4Cu); }
  static void set_recordings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B4Cu, v); }
  static constexpr uint32_t recordings_address() { return 0x826C5B4Cu; }
  // the kept replay OnDone hands to Replay::SetUpRace
  template <typename T = KeptReplay>
  static T kept_replay(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B50u)); }
  static void set_kept_replay(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B50u, v.address); }
  static void set_kept_replay(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B50u, v); }
  static constexpr uint32_t kept_replay_address() { return 0x826C5B50u; }
  // the string table Strings::Load gets
  static uint32_t string_table(uint8_t* base) { return guest::LoadU32(base, 0x826C5C34u); }
  static void set_string_table(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t string_table_address() { return 0x826C5C34u; }
  // the fades' pass ScreenFades::FadeIn gets
  static uint32_t fades_in(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fades_in(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fades_in_address() { return 0x82001BECu; }
  // the fades' pass ScreenFades::FadeOut gets
  static uint32_t fades_out(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fades_out(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fades_out_address() { return 0x82001BF0u; }
  // a fades' count FadeOut waits on
  static uint32_t fade_count0(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_count0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_count0_address() { return 0x826A50E4u; }
  // the other fades' count FadeOut waits on
  static uint32_t fade_count1(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_count1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_count1_address() { return 0x826A50DCu; }
};

}  // namespace schema
