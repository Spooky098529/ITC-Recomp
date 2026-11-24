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
  template <uint32_t Stride = kSize>
  static ModeType2 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeType2Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType2Data*>(::operator new(sizeof(ModeType2Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType2Data();
    return ModeType2(base, memory::MakeHandle(p, sizeof(ModeType2Data) * count, "ModeType2", &kModeType2Layout, sizeof(ModeType2Data)));
  }
  ModeType2 at(uint32_t i) const { return ModeType2(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ModeType2Data* direct() const { return static_cast<ModeType2Data*>(memory::HandleOwn(address, &kModeType2Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeType2Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType2Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006B9C (ModeType2Vtable)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), not the mode's
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType2Layout, 0, 1)); return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(base, address + 0))); }
  void set_answer_changed_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType2Layout, 0, 1, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the next-mode answer's first word (ModeAnswer; TearDown copies the block's into it), not the mode's
  uint32_t answer_word0_address() const { return address + 0; }
  uint32_t answer_word0_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType2Layout, 0, 4)); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_answer_word0_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType2Layout, 0, 4, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t answer_word0() const { return uint32_t(answer_word0_bits()); }
  void set_answer_word0(uint32_t v) const { set_answer_word0_bits(uint32_t(v)); }
  // the delegate at +8 (also the answer's next type +4 by the measure's join)
  uint32_t delegate_ptr_address() const { return address + 4; }
  uint32_t delegate_ptr_bits() const { if (auto* d = direct()) return d->delegate_ptr; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_delegate_ptr_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_ptr = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, delegate_ptr_bits()); }
  void set_delegate_ptr(const View& w) const { set_delegate_ptr_bits(w.address); }
  void set_delegate_ptr(uint32_t v) const { set_delegate_ptr_bits(v); }
  // the delegate (ModeDelegate): vtable 0x82010FA0
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable_bits() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_delegate_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_delegate_vtable(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the delegate's object: the mode
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object_bits() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_delegate_object_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_delegate_object(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // a slot thunk: Mode::Slot9Thunk (WaitForLoads) after Init, Mode::NextModeThunk 0x82235C08 once loaded, Slot10Thunk to 13
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function_bits() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_delegate_function_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_delegate_function(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment_bits() const { if (auto* d = direct()) return d->delegate_adjustment; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_adjustment = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment() const { if (auto* d = direct()) return d->delegate_adjustment; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_delegate_adjustment(uint32_t v) const { if (auto* d = direct()) { d->delegate_adjustment = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // the +32 interface's vtable 0x82006B90 (Interrupt, ForceExit, Resume)
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32_bits() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_vtable32_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_vtable32(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers, the block's 14 machines
  uint32_t handlers_address() const { return address + 36; }
  uint8_t* handlers() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 36 : guest::Pointer(base, address + 36); }
  uint32_t handler_count_address() const { return address + 236; }
  uint32_t handler_count_bits() const { if (auto* d = direct()) return d->handler_count; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(base, address + 236))); }
  void set_handler_count_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_count = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count() const { if (auto* d = direct()) return d->handler_count; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(base, address + 236))); }
  void set_handler_count(uint32_t v) const { if (auto* d = direct()) { d->handler_count = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(base, address + 236, v); }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint8_t* ring() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 240 : guest::Pointer(base, address + 240); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_count_bits() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 840) ? uint32_t(memory::OwnerLoad(address + 840, 4)) : guest::LoadU32(base, address + 840))); }
  void set_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 840)) memory::OwnerStore(address + 840, 4, v); else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 840) ? uint32_t(memory::OwnerLoad(address + 840, 4)) : guest::LoadU32(base, address + 840))); }
  void set_ring_count(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 840)) memory::OwnerStore(address + 840, 4, v); else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_start_address() const { return address + 844; }
  uint32_t ring_start_bits() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 844) ? uint32_t(memory::OwnerLoad(address + 844, 4)) : guest::LoadU32(base, address + 844))); }
  void set_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 844)) memory::OwnerStore(address + 844, 4, v); else guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 844) ? uint32_t(memory::OwnerLoad(address + 844, 4)) : guest::LoadU32(base, address + 844))); }
  void set_ring_start(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 844)) memory::OwnerStore(address + 844, 4, v); else guest::StoreU32(base, address + 844, v); }
  // measure fault: ring_count (+840) through the +32 interface (Interrupt)
  uint32_t iface_ring_count_address() const { return address + 808; }
  uint32_t iface_ring_count_bits() const { if (auto* d = direct()) return d->iface_ring_count; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_iface_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { d->iface_ring_count = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t iface_ring_count() const { if (auto* d = direct()) return d->iface_ring_count; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_iface_ring_count(uint32_t v) const { if (auto* d = direct()) { d->iface_ring_count = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  // measure fault: ring_start (+844) through the +32 interface (Interrupt)
  uint32_t iface_ring_start_address() const { return address + 812; }
  uint32_t iface_ring_start_bits() const { if (auto* d = direct()) return d->iface_ring_start; return uint32_t((memory::IsHandle(address + 812) ? uint32_t(memory::OwnerLoad(address + 812, 4)) : guest::LoadU32(base, address + 812))); }
  void set_iface_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { d->iface_ring_start = v; return; } if (memory::IsHandle(address + 812)) memory::OwnerStore(address + 812, 4, v); else guest::StoreU32(base, address + 812, v); }
  uint32_t iface_ring_start() const { if (auto* d = direct()) return d->iface_ring_start; return uint32_t((memory::IsHandle(address + 812) ? uint32_t(memory::OwnerLoad(address + 812, 4)) : guest::LoadU32(base, address + 812))); }
  void set_iface_ring_start(uint32_t v) const { if (auto* d = direct()) { d->iface_ring_start = v; return; } if (memory::IsHandle(address + 812)) memory::OwnerStore(address + 812, 4, v); else guest::StoreU32(base, address + 812, v); }
  // measure fault: block (+848) through the +32 interface (Interrupt, ForceExit)
  uint32_t iface_block_address() const { return address + 816; }
  uint32_t iface_block_bits() const { if (auto* d = direct()) return d->iface_block; return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(base, address + 816))); }
  void set_iface_block_bits(uint32_t v) const { if (auto* d = direct()) { d->iface_block = v; return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(base, address + 816, v); }
  uint32_t iface_block() const { if (auto* d = direct()) return d->iface_block; return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(base, address + 816))); }
  void set_iface_block(uint32_t v) const { if (auto* d = direct()) { d->iface_block = v; return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(base, address + 816, v); }
  // VirtualAlloc(0, 162688, 0x1000, 4) by Init, freed by ModeType2::Release
  uint32_t block_address() const { return address + 848; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 848) ? uint32_t(memory::OwnerLoad(address + 848, 4)) : guest::LoadU32(base, address + 848))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 848)) memory::OwnerStore(address + 848, 4, v); else guest::StoreU32(base, address + 848, v); }
  template <typename T = Type2ModeBlock>
  T block() const { return T(base, block_bits()); }
  void set_block(const View& w) const { set_block_bits(w.address); }
  void set_block(uint32_t v) const { set_block_bits(v); }
};

// allocated; measure: new<162688>@ModeType2::Init, ModeType2::TearDown.r4.+848, ModeType2::WaitForLoads.r4.+848, ModeType2::NextMode.r4.+848, ModeType2.+816
#pragma pack(push, 1)
struct Type2ModeBlockData {
  uint8_t pad_0[9604] = {};
  uint32_t hub_state = 0;  // +9604
  uint32_t hub_loaded = 0;  // +9608
  uint32_t hub_reenter = 0;  // +9612
  uint32_t hub_delete_movie = 0;  // +9616
  uint8_t pad_9620[20] = {};
  uint8_t race_menu[10152] = {};  // +9640, guest order
  uint8_t tuning_machine[36832] = {};  // +19792, guest order
  uint8_t spec_sheet[9640] = {};  // +56624, guest order
  uint8_t user_info[9640] = {};  // +66264, guest order
  uint8_t options[9640] = {};  // +75904, guest order
  uint8_t controller_options[9640] = {};  // +85544, guest order
  uint8_t car_settings[9640] = {};  // +95184, guest order
  uint8_t car_dealer[9640] = {};  // +104824, guest order
  uint8_t build_screen[9640] = {};  // +114464, guest order
  uint8_t dialog[9640] = {};  // +124104, guest order
  uint8_t plate_editor[9640] = {};  // +133744, guest order
  uint8_t garage_screen[9640] = {};  // +143384, guest order
  uint8_t pad_153024[9604] = {};
  uint32_t save_screen_state = 0;  // +162628
  uint32_t save_screen_loaded = 0;  // +162632
  uint8_t pad_162636[28] = {};
  uint32_t shown = 0;  // +162664
  uint32_t first_shown = 0;  // +162668
  uint8_t pad_162672[4] = {};
  uint8_t answer_changed = 0;  // +162676
  uint8_t pad_162677[3] = {};
  uint32_t answer_next_type = 0;  // +162680
  uint8_t pad_162684[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(Type2ModeBlockData) == 162688);
inline constexpr memory::LayoutField kType2ModeBlockFields[] = {{9604, 4}, {9608, 4}, {9612, 4}, {9616, 4}, {162628, 4}, {162632, 4}, {162664, 4}, {162668, 4}, {162680, 4}};
inline constexpr memory::Layout kType2ModeBlockLayout = {kType2ModeBlockFields, 9};

struct Type2ModeBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 162688;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  Type2ModeBlockData* data() const { return static_cast<Type2ModeBlockData*>(memory::HandleObject(address)); }
  static Type2ModeBlock New(uint8_t* base) {
    return Type2ModeBlock(base, memory::MakeHandle(new Type2ModeBlockData(), sizeof(Type2ModeBlockData), "Type2ModeBlock", &kType2ModeBlockLayout));
  }
  template <uint32_t Stride = kSize>
  static Type2ModeBlock NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(Type2ModeBlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Type2ModeBlockData*>(::operator new(sizeof(Type2ModeBlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Type2ModeBlockData();
    return Type2ModeBlock(base, memory::MakeHandle(p, sizeof(Type2ModeBlockData) * count, "Type2ModeBlock", &kType2ModeBlockLayout, sizeof(Type2ModeBlockData)));
  }
  Type2ModeBlock at(uint32_t i) const { return Type2ModeBlock(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  Type2ModeBlockData* direct() const { return static_cast<Type2ModeBlockData*>(memory::HandleOwn(address, &kType2ModeBlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kType2ModeBlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType2ModeBlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // Type2Hub's machine (type2-screens.md); MachineOf 0
  uint32_t hub_address() const { return address + 0; }
  uint8_t* hub() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  // the hub's machine state (14 picks the next mode 4)
  uint32_t hub_state_address() const { return address + 9604; }
  uint32_t hub_state_bits() const { if (auto* d = direct()) return d->hub_state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_hub_state_bits(uint32_t v) const { if (auto* d = direct()) { d->hub_state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  uint32_t hub_state() const { if (auto* d = direct()) return d->hub_state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_hub_state(uint32_t v) const { if (auto* d = direct()) { d->hub_state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  // the hub's movie loaded (AllLoaded)
  uint32_t hub_loaded_address() const { return address + 9608; }
  uint32_t hub_loaded_bits() const { if (auto* d = direct()) return d->hub_loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_hub_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->hub_loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  uint32_t hub_loaded() const { if (auto* d = direct()) return d->hub_loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_hub_loaded(uint32_t v) const { if (auto* d = direct()) { d->hub_loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  // open on its message: 1 from Init
  uint32_t hub_reenter_address() const { return address + 9612; }
  uint32_t hub_reenter_bits() const { if (auto* d = direct()) return d->hub_reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_hub_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->hub_reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  uint32_t hub_reenter() const { if (auto* d = direct()) return d->hub_reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_hub_reenter(uint32_t v) const { if (auto* d = direct()) { d->hub_reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  // 0 from Init
  uint32_t hub_delete_movie_address() const { return address + 9616; }
  uint32_t hub_delete_movie_bits() const { if (auto* d = direct()) return d->hub_delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_hub_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->hub_delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
  uint32_t hub_delete_movie() const { if (auto* d = direct()) return d->hub_delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_hub_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->hub_delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
  // RaceMenu (race-menu.md); MachineOf 1
  uint32_t race_menu_address() const { return address + 9640; }
  uint8_t* race_menu() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 9640 : guest::Pointer(base, address + 9640); }
  // the tuning screen (tuning-machine.md); MachineOf 2
  uint32_t tuning_machine_address() const { return address + 19792; }
  uint8_t* tuning_machine() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 19792 : guest::Pointer(base, address + 19792); }
  // PowerGraphScreen2, the spec sheet (type2-screens.md); MachineOf 9
  uint32_t spec_sheet_address() const { return address + 56624; }
  uint8_t* spec_sheet() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 56624 : guest::Pointer(base, address + 56624); }
  // UserInfo (user-info.md); MachineOf 10
  uint32_t user_info_address() const { return address + 66264; }
  uint8_t* user_info() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 66264 : guest::Pointer(base, address + 66264); }
  // Type2Options (options-screen.md); MachineOf 7
  uint32_t options_address() const { return address + 75904; }
  uint8_t* options() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 75904 : guest::Pointer(base, address + 75904); }
  // Type2ControllerOptions (options-screen.md); MachineOf 12
  uint32_t controller_options_address() const { return address + 85544; }
  uint8_t* controller_options() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 85544 : guest::Pointer(base, address + 85544); }
  // CarSettings (car-settings.md); MachineOf 3
  uint32_t car_settings_address() const { return address + 95184; }
  uint8_t* car_settings() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 95184 : guest::Pointer(base, address + 95184); }
  // CarDealer (car-dealer.md); MachineOf 4; its state 14 picks the next mode 4
  uint32_t car_dealer_address() const { return address + 104824; }
  uint8_t* car_dealer() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 104824 : guest::Pointer(base, address + 104824); }
  // BuildScreen (type2-screens.md); MachineOf 6
  uint32_t build_screen_address() const { return address + 114464; }
  uint8_t* build_screen() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 114464 : guest::Pointer(base, address + 114464); }
  // Type2Dialog (type2-dialog.md); MachineOf 16
  uint32_t dialog_address() const { return address + 124104; }
  uint8_t* dialog() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 124104 : guest::Pointer(base, address + 124104); }
  // PlateEditor (type2-screens.md); MachineOf 26
  uint32_t plate_editor_address() const { return address + 133744; }
  uint8_t* plate_editor() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 133744 : guest::Pointer(base, address + 133744); }
  // GarageScreen (garage-screen.md); MachineOf 5
  uint32_t garage_screen_address() const { return address + 143384; }
  uint8_t* garage_screen() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 143384 : guest::Pointer(base, address + 143384); }
  // Type2SaveScreen (save-screen.md); MachineOf 44
  uint32_t save_screen_address() const { return address + 153024; }
  uint8_t* save_screen() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 153024 : guest::Pointer(base, address + 153024); }
  // the save screen's machine state (+0x255C0 + 9604; Interrupt: 5)
  uint32_t save_screen_state_address() const { return address + 162628; }
  uint32_t save_screen_state_bits() const { if (auto* d = direct()) return d->save_screen_state; return uint32_t((memory::IsHandle(address + 162628) ? uint32_t(memory::OwnerLoad(address + 162628, 4)) : guest::LoadU32(base, address + 162628))); }
  void set_save_screen_state_bits(uint32_t v) const { if (auto* d = direct()) { d->save_screen_state = v; return; } if (memory::IsHandle(address + 162628)) memory::OwnerStore(address + 162628, 4, v); else guest::StoreU32(base, address + 162628, v); }
  uint32_t save_screen_state() const { if (auto* d = direct()) return d->save_screen_state; return uint32_t((memory::IsHandle(address + 162628) ? uint32_t(memory::OwnerLoad(address + 162628, 4)) : guest::LoadU32(base, address + 162628))); }
  void set_save_screen_state(uint32_t v) const { if (auto* d = direct()) { d->save_screen_state = v; return; } if (memory::IsHandle(address + 162628)) memory::OwnerStore(address + 162628, 4, v); else guest::StoreU32(base, address + 162628, v); }
  // the save screen's +9608 (+0x255C0 + 9608), the last word AllLoaded waits on
  uint32_t save_screen_loaded_address() const { return address + 162632; }
  uint32_t save_screen_loaded_bits() const { if (auto* d = direct()) return d->save_screen_loaded; return uint32_t((memory::IsHandle(address + 162632) ? uint32_t(memory::OwnerLoad(address + 162632, 4)) : guest::LoadU32(base, address + 162632))); }
  void set_save_screen_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->save_screen_loaded = v; return; } if (memory::IsHandle(address + 162632)) memory::OwnerStore(address + 162632, 4, v); else guest::StoreU32(base, address + 162632, v); }
  uint32_t save_screen_loaded() const { if (auto* d = direct()) return d->save_screen_loaded; return uint32_t((memory::IsHandle(address + 162632) ? uint32_t(memory::OwnerLoad(address + 162632, 4)) : guest::LoadU32(base, address + 162632))); }
  void set_save_screen_loaded(uint32_t v) const { if (auto* d = direct()) { d->save_screen_loaded = v; return; } if (memory::IsHandle(address + 162632)) memory::OwnerStore(address + 162632, 4, v); else guest::StoreU32(base, address + 162632, v); }
  // the shown screen's index (0 to 7, FindShown; MachineOf's index)
  uint32_t shown_address() const { return address + 162664; }
  uint32_t shown_bits() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 162664) ? uint32_t(memory::OwnerLoad(address + 162664, 4)) : guest::LoadU32(base, address + 162664))); }
  void set_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 162664)) memory::OwnerStore(address + 162664, 4, v); else guest::StoreU32(base, address + 162664, v); }
  uint32_t shown() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 162664) ? uint32_t(memory::OwnerLoad(address + 162664, 4)) : guest::LoadU32(base, address + 162664))); }
  void set_shown(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 162664)) memory::OwnerStore(address + 162664, 4, v); else guest::StoreU32(base, address + 162664, v); }
  // Init's first screen: 0 the hub, 3 the car settings, 4 the car dealer
  uint32_t first_shown_address() const { return address + 162668; }
  uint32_t first_shown_bits() const { if (auto* d = direct()) return d->first_shown; return uint32_t((memory::IsHandle(address + 162668) ? uint32_t(memory::OwnerLoad(address + 162668, 4)) : guest::LoadU32(base, address + 162668))); }
  void set_first_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->first_shown = v; return; } if (memory::IsHandle(address + 162668)) memory::OwnerStore(address + 162668, 4, v); else guest::StoreU32(base, address + 162668, v); }
  uint32_t first_shown() const { if (auto* d = direct()) return d->first_shown; return uint32_t((memory::IsHandle(address + 162668) ? uint32_t(memory::OwnerLoad(address + 162668, 4)) : guest::LoadU32(base, address + 162668))); }
  void set_first_shown(uint32_t v) const { if (auto* d = direct()) { d->first_shown = v; return; } if (memory::IsHandle(address + 162668)) memory::OwnerStore(address + 162668, 4, v); else guest::StoreU32(base, address + 162668, v); }
  // the next mode's 8-byte answer (ModeAnswer) TearDown returns: byte 1
  uint32_t answer_changed_address() const { return address + 162676; }
  uint8_t answer_changed_bits() const { if (auto* d = direct()) return d->answer_changed; return uint8_t((memory::IsHandle(address + 162676) ? uint8_t(memory::OwnerLoad(address + 162676, 1)) : guest::LoadU8(base, address + 162676))); }
  void set_answer_changed_bits(uint8_t v) const { if (auto* d = direct()) { d->answer_changed = v; return; } if (memory::IsHandle(address + 162676)) memory::OwnerStore(address + 162676, 1, v); else guest::StoreU8(base, address + 162676, v); }
  uint8_t answer_changed() const { if (auto* d = direct()) return d->answer_changed; return uint8_t((memory::IsHandle(address + 162676) ? uint8_t(memory::OwnerLoad(address + 162676, 1)) : guest::LoadU8(base, address + 162676))); }
  void set_answer_changed(uint8_t v) const { if (auto* d = direct()) { d->answer_changed = v; return; } if (memory::IsHandle(address + 162676)) memory::OwnerStore(address + 162676, 1, v); else guest::StoreU8(base, address + 162676, v); }
  // the answer's first word as copied: byte 1 and three frame bytes
  uint32_t answer_word0_address() const { return address + 162676; }
  uint32_t answer_word0_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kType2ModeBlockLayout, 162676, 4)); return uint32_t((memory::IsHandle(address + 162676) ? uint32_t(memory::OwnerLoad(address + 162676, 4)) : guest::LoadU32(base, address + 162676))); }
  void set_answer_word0_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType2ModeBlockLayout, 162676, 4, v); return; } if (memory::IsHandle(address + 162676)) memory::OwnerStore(address + 162676, 4, v); else guest::StoreU32(base, address + 162676, v); }
  uint32_t answer_word0() const { return uint32_t(answer_word0_bits()); }
  void set_answer_word0(uint32_t v) const { set_answer_word0_bits(uint32_t(v)); }
  // the next mode: 1 the race, 3, 4, 5 the parking area or 12
  uint32_t answer_next_type_address() const { return address + 162680; }
  uint32_t answer_next_type_bits() const { if (auto* d = direct()) return d->answer_next_type; return uint32_t((memory::IsHandle(address + 162680) ? uint32_t(memory::OwnerLoad(address + 162680, 4)) : guest::LoadU32(base, address + 162680))); }
  void set_answer_next_type_bits(uint32_t v) const { if (auto* d = direct()) { d->answer_next_type = v; return; } if (memory::IsHandle(address + 162680)) memory::OwnerStore(address + 162680, 4, v); else guest::StoreU32(base, address + 162680, v); }
  uint32_t answer_next_type() const { if (auto* d = direct()) return d->answer_next_type; return uint32_t((memory::IsHandle(address + 162680) ? uint32_t(memory::OwnerLoad(address + 162680, 4)) : guest::LoadU32(base, address + 162680))); }
  void set_answer_next_type(uint32_t v) const { if (auto* d = direct()) { d->answer_next_type = v; return; } if (memory::IsHandle(address + 162680)) memory::OwnerStore(address + 162680, 4, v); else guest::StoreU32(base, address + 162680, v); }
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
  template <uint32_t Stride = kSize>
  static ModeSlotObject NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeSlotObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeSlotObjectData*>(::operator new(sizeof(ModeSlotObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeSlotObjectData();
    return ModeSlotObject(base, memory::MakeHandle(p, sizeof(ModeSlotObjectData) * count, "ModeSlotObject", &kModeSlotObjectLayout, sizeof(ModeSlotObjectData)));
  }
  ModeSlotObject at(uint32_t i) const { return ModeSlotObject(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ModeSlotObjectData* direct() const { return static_cast<ModeSlotObjectData*>(memory::HandleOwn(address, &kModeSlotObjectLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeSlotObjectLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeSlotObjectLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006B9C for the type 2 mode
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  template <typename T = ModeType2Vtable>
  T vtable() const { return T(base, vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
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
