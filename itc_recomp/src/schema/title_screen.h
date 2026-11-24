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
  template <uint32_t Stride = kSize>
  static ModeType4 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeType4Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType4Data*>(::operator new(sizeof(ModeType4Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType4Data();
    return ModeType4(base, memory::MakeHandle(p, sizeof(ModeType4Data) * count, "ModeType4", &kModeType4Layout, sizeof(ModeType4Data)));
  }
  ModeType4 at(uint32_t i) const { return ModeType4(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ModeType4Data* direct() const { return static_cast<ModeType4Data*>(memory::HandleOwn(address, &kModeType4Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeType4Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006BE0
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer, the delegate functions' r3), not the mode's
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, 0, 1)); return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(base, address + 0))); }
  void set_answer_changed_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, 0, 1, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // the delegate at +8; 0 when its address wraps (never). Also the answer's next type +4 by the measure's join
  uint32_t delegate_ptr_address() const { return address + 4; }
  uint32_t delegate_ptr_bits() const { if (auto* d = direct()) return d->delegate_ptr; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_delegate_ptr_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_ptr = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, delegate_ptr_bits()); }
  void set_delegate_ptr(const View& w) const { set_delegate_ptr_bits(w.address); }
  void set_delegate_ptr(uint32_t v) const { set_delegate_ptr_bits(v); }
  // 0x82010FA0 (ModeDelegate::Invoke)
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable_bits() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_delegate_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_delegate_vtable(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the mode
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object_bits() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_delegate_object_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_delegate_object(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // Mode::Slot8Thunk (Restart) after Init, then NextModeThunk, Slot10Thunk, Slot11Thunk or Slot12Thunk
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function_bits() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_delegate_function_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_delegate_function(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // 0
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment_bits() const { if (auto* d = direct()) return d->delegate_adjustment; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_adjustment = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment() const { if (auto* d = direct()) return d->delegate_adjustment; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_delegate_adjustment(uint32_t v) const { if (auto* d = direct()) { d->delegate_adjustment = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // 0x82006BD4: Interrupt, ForceExit, Resume
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32_bits() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_vtable32_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_vtable32(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (the five machines, in Init's order)
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
  // VirtualAlloc'ed by Init (48,220 bytes)
  uint32_t block_address() const { return address + 848; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 848) ? uint32_t(memory::OwnerLoad(address + 848, 4)) : guest::LoadU32(base, address + 848))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 848)) memory::OwnerStore(address + 848, 4, v); else guest::StoreU32(base, address + 848, v); }
  template <typename T = Type4Block>
  T block() const { return T(base, block_bits()); }
  void set_block(const View& w) const { set_block_bits(w.address); }
  void set_block(uint32_t v) const { set_block_bits(v); }
  // measure fault: the +32 interface's +808 (ModeType4Interface32), the mode's +840
  uint32_t fault_interface_ring_count_address() const { return address + 808; }
  uint32_t fault_interface_ring_count_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, 808, 4)); return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_fault_interface_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, 808, 4, v); return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t fault_interface_ring_count() const { return uint32_t(fault_interface_ring_count_bits()); }
  void set_fault_interface_ring_count(uint32_t v) const { set_fault_interface_ring_count_bits(uint32_t(v)); }
  // measure fault: the +32 interface's +812, the mode's +844
  uint32_t fault_interface_ring_start_address() const { return address + 812; }
  uint32_t fault_interface_ring_start_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, 812, 4)); return uint32_t((memory::IsHandle(address + 812) ? uint32_t(memory::OwnerLoad(address + 812, 4)) : guest::LoadU32(base, address + 812))); }
  void set_fault_interface_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, 812, 4, v); return; } if (memory::IsHandle(address + 812)) memory::OwnerStore(address + 812, 4, v); else guest::StoreU32(base, address + 812, v); }
  uint32_t fault_interface_ring_start() const { return uint32_t(fault_interface_ring_start_bits()); }
  void set_fault_interface_ring_start(uint32_t v) const { set_fault_interface_ring_start_bits(uint32_t(v)); }
  // measure fault: the +32 interface's +816, the mode's +848
  uint32_t fault_interface_block_address() const { return address + 816; }
  uint32_t fault_interface_block_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, 816, 4)); return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(base, address + 816))); }
  void set_fault_interface_block_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Layout, 816, 4, v); return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(base, address + 816, v); }
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
  template <uint32_t Stride = kSize>
  static ModeType4Interface32 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeType4Interface32Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType4Interface32Data*>(::operator new(sizeof(ModeType4Interface32Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType4Interface32Data();
    return ModeType4Interface32(base, memory::MakeHandle(p, sizeof(ModeType4Interface32Data) * count, "ModeType4Interface32", &kModeType4Interface32Layout, sizeof(ModeType4Interface32Data)));
  }
  ModeType4Interface32 at(uint32_t i) const { return ModeType4Interface32(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ModeType4Interface32Data* direct() const { return static_cast<ModeType4Interface32Data*>(memory::HandleOwn(address, &kModeType4Interface32Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeType4Interface32Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType4Interface32Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006BD4
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the mode's ring (+240)
  uint32_t ring_address() const { return address + 208; }
  uint8_t* ring() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 208 : guest::Pointer(base, address + 208); }
  // the mode's +840
  uint32_t ring_count_address() const { return address + 808; }
  uint32_t ring_count_bits() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t ring_count() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_ring_count(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  // the mode's +844
  uint32_t ring_start_address() const { return address + 812; }
  uint32_t ring_start_bits() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 812) ? uint32_t(memory::OwnerLoad(address + 812, 4)) : guest::LoadU32(base, address + 812))); }
  void set_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 812)) memory::OwnerStore(address + 812, 4, v); else guest::StoreU32(base, address + 812, v); }
  uint32_t ring_start() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 812) ? uint32_t(memory::OwnerLoad(address + 812, 4)) : guest::LoadU32(base, address + 812))); }
  void set_ring_start(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 812)) memory::OwnerStore(address + 812, 4, v); else guest::StoreU32(base, address + 812, v); }
  // the mode's +848
  uint32_t block_address() const { return address + 816; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(base, address + 816))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(base, address + 816, v); }
  template <typename T = Type4Block>
  T block() const { return T(base, block_bits()); }
  void set_block(const View& w) const { set_block_bits(w.address); }
  void set_block(uint32_t v) const { set_block_bits(v); }
};

// allocated; measure: new<48220>@ModeType4::Init, TitleScreen, ModeType4::NextModeAnswer.r4.+848, ModeType4::NextMode.r4.+848
#pragma pack(push, 1)
struct Type4BlockData {
  uint8_t pad_0[9604] = {};
  uint32_t options_state = 0;  // +9604
  uint32_t options_loaded = 0;  // +9608
  uint32_t options_reenter = 0;  // +9612
  uint32_t options_delete_movie = 0;  // +9616
  uint8_t pad_9620[9624] = {};
  uint32_t controller_options_state = 0;  // +19244
  uint32_t controller_options_loaded = 0;  // +19248
  uint32_t controller_options_reenter = 0;  // +19252
  uint32_t controller_options_delete_movie = 0;  // +19256
  uint8_t pad_19260[9624] = {};
  uint32_t save_screen_state = 0;  // +28884
  uint32_t save_screen_loaded = 0;  // +28888
  uint32_t save_screen_reenter = 0;  // +28892
  uint32_t save_screen_delete_movie = 0;  // +28896
  uint8_t pad_28900[9624] = {};
  uint32_t title_load_state = 0;  // +38524
  uint32_t title_load_loaded = 0;  // +38528
  uint32_t title_load_reenter = 0;  // +38532
  uint32_t title_load_delete_movie = 0;  // +38536
  uint8_t pad_38540[9624] = {};
  uint32_t title_screen_state = 0;  // +48164
  uint32_t title_screen_loaded = 0;  // +48168
  uint32_t title_screen_reenter = 0;  // +48172
  uint32_t title_screen_delete_movie = 0;  // +48176
  uint8_t pad_48180[20] = {};
  uint32_t shown = 0;  // +48200
  uint8_t pad_48204[8] = {};
  uint32_t next_answer = 0;  // +48212
  uint32_t next_type = 0;  // +48216
};
#pragma pack(pop)
static_assert(sizeof(Type4BlockData) == 48220);
inline constexpr memory::LayoutField kType4BlockFields[] = {{9604, 4}, {9608, 4}, {9612, 4}, {9616, 4}, {19244, 4}, {19248, 4}, {19252, 4}, {19256, 4}, {28884, 4}, {28888, 4}, {28892, 4}, {28896, 4}, {38524, 4}, {38528, 4}, {38532, 4}, {38536, 4}, {48164, 4}, {48168, 4}, {48172, 4}, {48176, 4}, {48200, 4}, {48212, 4}, {48216, 4}};
inline constexpr memory::Layout kType4BlockLayout = {kType4BlockFields, 23};

struct Type4Block : View {
  using View::View;
  static constexpr uint32_t kSize = 48220;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  Type4BlockData* data() const { return static_cast<Type4BlockData*>(memory::HandleObject(address)); }
  static Type4Block New(uint8_t* base) {
    return Type4Block(base, memory::MakeHandle(new Type4BlockData(), sizeof(Type4BlockData), "Type4Block", &kType4BlockLayout));
  }
  template <uint32_t Stride = kSize>
  static Type4Block NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(Type4BlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Type4BlockData*>(::operator new(sizeof(Type4BlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Type4BlockData();
    return Type4Block(base, memory::MakeHandle(p, sizeof(Type4BlockData) * count, "Type4Block", &kType4BlockLayout, sizeof(Type4BlockData)));
  }
  Type4Block at(uint32_t i) const { return Type4Block(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  Type4BlockData* direct() const { return static_cast<Type4BlockData*>(memory::HandleOwn(address, &kType4BlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kType4BlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType4BlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // the options' machine (Type4Options::Init, options-screen.md)
  uint32_t options_address() const { return address + 0; }
  uint8_t* options() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  uint32_t options_state_address() const { return address + 9604; }
  uint32_t options_state_bits() const { if (auto* d = direct()) return d->options_state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_options_state_bits(uint32_t v) const { if (auto* d = direct()) { d->options_state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  uint32_t options_state() const { if (auto* d = direct()) return d->options_state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_options_state(uint32_t v) const { if (auto* d = direct()) { d->options_state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  uint32_t options_loaded_address() const { return address + 9608; }
  uint32_t options_loaded_bits() const { if (auto* d = direct()) return d->options_loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_options_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->options_loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  uint32_t options_loaded() const { if (auto* d = direct()) return d->options_loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_options_loaded(uint32_t v) const { if (auto* d = direct()) { d->options_loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  // 1 from Init
  uint32_t options_reenter_address() const { return address + 9612; }
  uint32_t options_reenter_bits() const { if (auto* d = direct()) return d->options_reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_options_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->options_reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  uint32_t options_reenter() const { if (auto* d = direct()) return d->options_reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_options_reenter(uint32_t v) const { if (auto* d = direct()) { d->options_reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  // 0 from Init
  uint32_t options_delete_movie_address() const { return address + 9616; }
  uint32_t options_delete_movie_bits() const { if (auto* d = direct()) return d->options_delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_options_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->options_delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
  uint32_t options_delete_movie() const { if (auto* d = direct()) return d->options_delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_options_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->options_delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
  // the controller options' machine (Type4ControllerOptions::Init, options-screen.md)
  uint32_t controller_options_address() const { return address + 9640; }
  uint8_t* controller_options() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 9640 : guest::Pointer(base, address + 9640); }
  uint32_t controller_options_state_address() const { return address + 19244; }
  uint32_t controller_options_state_bits() const { if (auto* d = direct()) return d->controller_options_state; return uint32_t((memory::IsHandle(address + 19244) ? uint32_t(memory::OwnerLoad(address + 19244, 4)) : guest::LoadU32(base, address + 19244))); }
  void set_controller_options_state_bits(uint32_t v) const { if (auto* d = direct()) { d->controller_options_state = v; return; } if (memory::IsHandle(address + 19244)) memory::OwnerStore(address + 19244, 4, v); else guest::StoreU32(base, address + 19244, v); }
  uint32_t controller_options_state() const { if (auto* d = direct()) return d->controller_options_state; return uint32_t((memory::IsHandle(address + 19244) ? uint32_t(memory::OwnerLoad(address + 19244, 4)) : guest::LoadU32(base, address + 19244))); }
  void set_controller_options_state(uint32_t v) const { if (auto* d = direct()) { d->controller_options_state = v; return; } if (memory::IsHandle(address + 19244)) memory::OwnerStore(address + 19244, 4, v); else guest::StoreU32(base, address + 19244, v); }
  uint32_t controller_options_loaded_address() const { return address + 19248; }
  uint32_t controller_options_loaded_bits() const { if (auto* d = direct()) return d->controller_options_loaded; return uint32_t((memory::IsHandle(address + 19248) ? uint32_t(memory::OwnerLoad(address + 19248, 4)) : guest::LoadU32(base, address + 19248))); }
  void set_controller_options_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->controller_options_loaded = v; return; } if (memory::IsHandle(address + 19248)) memory::OwnerStore(address + 19248, 4, v); else guest::StoreU32(base, address + 19248, v); }
  uint32_t controller_options_loaded() const { if (auto* d = direct()) return d->controller_options_loaded; return uint32_t((memory::IsHandle(address + 19248) ? uint32_t(memory::OwnerLoad(address + 19248, 4)) : guest::LoadU32(base, address + 19248))); }
  void set_controller_options_loaded(uint32_t v) const { if (auto* d = direct()) { d->controller_options_loaded = v; return; } if (memory::IsHandle(address + 19248)) memory::OwnerStore(address + 19248, 4, v); else guest::StoreU32(base, address + 19248, v); }
  // 0 from Init
  uint32_t controller_options_reenter_address() const { return address + 19252; }
  uint32_t controller_options_reenter_bits() const { if (auto* d = direct()) return d->controller_options_reenter; return uint32_t((memory::IsHandle(address + 19252) ? uint32_t(memory::OwnerLoad(address + 19252, 4)) : guest::LoadU32(base, address + 19252))); }
  void set_controller_options_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->controller_options_reenter = v; return; } if (memory::IsHandle(address + 19252)) memory::OwnerStore(address + 19252, 4, v); else guest::StoreU32(base, address + 19252, v); }
  uint32_t controller_options_reenter() const { if (auto* d = direct()) return d->controller_options_reenter; return uint32_t((memory::IsHandle(address + 19252) ? uint32_t(memory::OwnerLoad(address + 19252, 4)) : guest::LoadU32(base, address + 19252))); }
  void set_controller_options_reenter(uint32_t v) const { if (auto* d = direct()) { d->controller_options_reenter = v; return; } if (memory::IsHandle(address + 19252)) memory::OwnerStore(address + 19252, 4, v); else guest::StoreU32(base, address + 19252, v); }
  // 1 from Init
  uint32_t controller_options_delete_movie_address() const { return address + 19256; }
  uint32_t controller_options_delete_movie_bits() const { if (auto* d = direct()) return d->controller_options_delete_movie; return uint32_t((memory::IsHandle(address + 19256) ? uint32_t(memory::OwnerLoad(address + 19256, 4)) : guest::LoadU32(base, address + 19256))); }
  void set_controller_options_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->controller_options_delete_movie = v; return; } if (memory::IsHandle(address + 19256)) memory::OwnerStore(address + 19256, 4, v); else guest::StoreU32(base, address + 19256, v); }
  uint32_t controller_options_delete_movie() const { if (auto* d = direct()) return d->controller_options_delete_movie; return uint32_t((memory::IsHandle(address + 19256) ? uint32_t(memory::OwnerLoad(address + 19256, 4)) : guest::LoadU32(base, address + 19256))); }
  void set_controller_options_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->controller_options_delete_movie = v; return; } if (memory::IsHandle(address + 19256)) memory::OwnerStore(address + 19256, 4, v); else guest::StoreU32(base, address + 19256, v); }
  // the save screen's machine (Type4SaveScreen::Init, save-screen.md)
  uint32_t save_screen_address() const { return address + 19280; }
  uint8_t* save_screen() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 19280 : guest::Pointer(base, address + 19280); }
  // 5 shown (Interrupt)
  uint32_t save_screen_state_address() const { return address + 28884; }
  uint32_t save_screen_state_bits() const { if (auto* d = direct()) return d->save_screen_state; return uint32_t((memory::IsHandle(address + 28884) ? uint32_t(memory::OwnerLoad(address + 28884, 4)) : guest::LoadU32(base, address + 28884))); }
  void set_save_screen_state_bits(uint32_t v) const { if (auto* d = direct()) { d->save_screen_state = v; return; } if (memory::IsHandle(address + 28884)) memory::OwnerStore(address + 28884, 4, v); else guest::StoreU32(base, address + 28884, v); }
  uint32_t save_screen_state() const { if (auto* d = direct()) return d->save_screen_state; return uint32_t((memory::IsHandle(address + 28884) ? uint32_t(memory::OwnerLoad(address + 28884, 4)) : guest::LoadU32(base, address + 28884))); }
  void set_save_screen_state(uint32_t v) const { if (auto* d = direct()) { d->save_screen_state = v; return; } if (memory::IsHandle(address + 28884)) memory::OwnerStore(address + 28884, 4, v); else guest::StoreU32(base, address + 28884, v); }
  uint32_t save_screen_loaded_address() const { return address + 28888; }
  uint32_t save_screen_loaded_bits() const { if (auto* d = direct()) return d->save_screen_loaded; return uint32_t((memory::IsHandle(address + 28888) ? uint32_t(memory::OwnerLoad(address + 28888, 4)) : guest::LoadU32(base, address + 28888))); }
  void set_save_screen_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->save_screen_loaded = v; return; } if (memory::IsHandle(address + 28888)) memory::OwnerStore(address + 28888, 4, v); else guest::StoreU32(base, address + 28888, v); }
  uint32_t save_screen_loaded() const { if (auto* d = direct()) return d->save_screen_loaded; return uint32_t((memory::IsHandle(address + 28888) ? uint32_t(memory::OwnerLoad(address + 28888, 4)) : guest::LoadU32(base, address + 28888))); }
  void set_save_screen_loaded(uint32_t v) const { if (auto* d = direct()) { d->save_screen_loaded = v; return; } if (memory::IsHandle(address + 28888)) memory::OwnerStore(address + 28888, 4, v); else guest::StoreU32(base, address + 28888, v); }
  // 1 from Init
  uint32_t save_screen_reenter_address() const { return address + 28892; }
  uint32_t save_screen_reenter_bits() const { if (auto* d = direct()) return d->save_screen_reenter; return uint32_t((memory::IsHandle(address + 28892) ? uint32_t(memory::OwnerLoad(address + 28892, 4)) : guest::LoadU32(base, address + 28892))); }
  void set_save_screen_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->save_screen_reenter = v; return; } if (memory::IsHandle(address + 28892)) memory::OwnerStore(address + 28892, 4, v); else guest::StoreU32(base, address + 28892, v); }
  uint32_t save_screen_reenter() const { if (auto* d = direct()) return d->save_screen_reenter; return uint32_t((memory::IsHandle(address + 28892) ? uint32_t(memory::OwnerLoad(address + 28892, 4)) : guest::LoadU32(base, address + 28892))); }
  void set_save_screen_reenter(uint32_t v) const { if (auto* d = direct()) { d->save_screen_reenter = v; return; } if (memory::IsHandle(address + 28892)) memory::OwnerStore(address + 28892, 4, v); else guest::StoreU32(base, address + 28892, v); }
  // 0 from Init
  uint32_t save_screen_delete_movie_address() const { return address + 28896; }
  uint32_t save_screen_delete_movie_bits() const { if (auto* d = direct()) return d->save_screen_delete_movie; return uint32_t((memory::IsHandle(address + 28896) ? uint32_t(memory::OwnerLoad(address + 28896, 4)) : guest::LoadU32(base, address + 28896))); }
  void set_save_screen_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->save_screen_delete_movie = v; return; } if (memory::IsHandle(address + 28896)) memory::OwnerStore(address + 28896, 4, v); else guest::StoreU32(base, address + 28896, v); }
  uint32_t save_screen_delete_movie() const { if (auto* d = direct()) return d->save_screen_delete_movie; return uint32_t((memory::IsHandle(address + 28896) ? uint32_t(memory::OwnerLoad(address + 28896, 4)) : guest::LoadU32(base, address + 28896))); }
  void set_save_screen_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->save_screen_delete_movie = v; return; } if (memory::IsHandle(address + 28896)) memory::OwnerStore(address + 28896, 4, v); else guest::StoreU32(base, address + 28896, v); }
  // the title load's machine (TitleLoad::Init, Type4Machine)
  uint32_t title_load_address() const { return address + 28920; }
  uint8_t* title_load() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 28920 : guest::Pointer(base, address + 28920); }
  uint32_t title_load_state_address() const { return address + 38524; }
  uint32_t title_load_state_bits() const { if (auto* d = direct()) return d->title_load_state; return uint32_t((memory::IsHandle(address + 38524) ? uint32_t(memory::OwnerLoad(address + 38524, 4)) : guest::LoadU32(base, address + 38524))); }
  void set_title_load_state_bits(uint32_t v) const { if (auto* d = direct()) { d->title_load_state = v; return; } if (memory::IsHandle(address + 38524)) memory::OwnerStore(address + 38524, 4, v); else guest::StoreU32(base, address + 38524, v); }
  uint32_t title_load_state() const { if (auto* d = direct()) return d->title_load_state; return uint32_t((memory::IsHandle(address + 38524) ? uint32_t(memory::OwnerLoad(address + 38524, 4)) : guest::LoadU32(base, address + 38524))); }
  void set_title_load_state(uint32_t v) const { if (auto* d = direct()) { d->title_load_state = v; return; } if (memory::IsHandle(address + 38524)) memory::OwnerStore(address + 38524, 4, v); else guest::StoreU32(base, address + 38524, v); }
  uint32_t title_load_loaded_address() const { return address + 38528; }
  uint32_t title_load_loaded_bits() const { if (auto* d = direct()) return d->title_load_loaded; return uint32_t((memory::IsHandle(address + 38528) ? uint32_t(memory::OwnerLoad(address + 38528, 4)) : guest::LoadU32(base, address + 38528))); }
  void set_title_load_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->title_load_loaded = v; return; } if (memory::IsHandle(address + 38528)) memory::OwnerStore(address + 38528, 4, v); else guest::StoreU32(base, address + 38528, v); }
  uint32_t title_load_loaded() const { if (auto* d = direct()) return d->title_load_loaded; return uint32_t((memory::IsHandle(address + 38528) ? uint32_t(memory::OwnerLoad(address + 38528, 4)) : guest::LoadU32(base, address + 38528))); }
  void set_title_load_loaded(uint32_t v) const { if (auto* d = direct()) { d->title_load_loaded = v; return; } if (memory::IsHandle(address + 38528)) memory::OwnerStore(address + 38528, 4, v); else guest::StoreU32(base, address + 38528, v); }
  // 1 from Init
  uint32_t title_load_reenter_address() const { return address + 38532; }
  uint32_t title_load_reenter_bits() const { if (auto* d = direct()) return d->title_load_reenter; return uint32_t((memory::IsHandle(address + 38532) ? uint32_t(memory::OwnerLoad(address + 38532, 4)) : guest::LoadU32(base, address + 38532))); }
  void set_title_load_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->title_load_reenter = v; return; } if (memory::IsHandle(address + 38532)) memory::OwnerStore(address + 38532, 4, v); else guest::StoreU32(base, address + 38532, v); }
  uint32_t title_load_reenter() const { if (auto* d = direct()) return d->title_load_reenter; return uint32_t((memory::IsHandle(address + 38532) ? uint32_t(memory::OwnerLoad(address + 38532, 4)) : guest::LoadU32(base, address + 38532))); }
  void set_title_load_reenter(uint32_t v) const { if (auto* d = direct()) { d->title_load_reenter = v; return; } if (memory::IsHandle(address + 38532)) memory::OwnerStore(address + 38532, 4, v); else guest::StoreU32(base, address + 38532, v); }
  // 0 from Init
  uint32_t title_load_delete_movie_address() const { return address + 38536; }
  uint32_t title_load_delete_movie_bits() const { if (auto* d = direct()) return d->title_load_delete_movie; return uint32_t((memory::IsHandle(address + 38536) ? uint32_t(memory::OwnerLoad(address + 38536, 4)) : guest::LoadU32(base, address + 38536))); }
  void set_title_load_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->title_load_delete_movie = v; return; } if (memory::IsHandle(address + 38536)) memory::OwnerStore(address + 38536, 4, v); else guest::StoreU32(base, address + 38536, v); }
  uint32_t title_load_delete_movie() const { if (auto* d = direct()) return d->title_load_delete_movie; return uint32_t((memory::IsHandle(address + 38536) ? uint32_t(memory::OwnerLoad(address + 38536, 4)) : guest::LoadU32(base, address + 38536))); }
  void set_title_load_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->title_load_delete_movie = v; return; } if (memory::IsHandle(address + 38536)) memory::OwnerStore(address + 38536, 4, v); else guest::StoreU32(base, address + 38536, v); }
  // the title screen's machine (TitleScreen::Init, Type4Machine)
  uint32_t title_screen_address() const { return address + 38560; }
  uint8_t* title_screen() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 38560 : guest::Pointer(base, address + 38560); }
  uint32_t title_screen_state_address() const { return address + 48164; }
  uint32_t title_screen_state_bits() const { if (auto* d = direct()) return d->title_screen_state; return uint32_t((memory::IsHandle(address + 48164) ? uint32_t(memory::OwnerLoad(address + 48164, 4)) : guest::LoadU32(base, address + 48164))); }
  void set_title_screen_state_bits(uint32_t v) const { if (auto* d = direct()) { d->title_screen_state = v; return; } if (memory::IsHandle(address + 48164)) memory::OwnerStore(address + 48164, 4, v); else guest::StoreU32(base, address + 48164, v); }
  uint32_t title_screen_state() const { if (auto* d = direct()) return d->title_screen_state; return uint32_t((memory::IsHandle(address + 48164) ? uint32_t(memory::OwnerLoad(address + 48164, 4)) : guest::LoadU32(base, address + 48164))); }
  void set_title_screen_state(uint32_t v) const { if (auto* d = direct()) { d->title_screen_state = v; return; } if (memory::IsHandle(address + 48164)) memory::OwnerStore(address + 48164, 4, v); else guest::StoreU32(base, address + 48164, v); }
  uint32_t title_screen_loaded_address() const { return address + 48168; }
  uint32_t title_screen_loaded_bits() const { if (auto* d = direct()) return d->title_screen_loaded; return uint32_t((memory::IsHandle(address + 48168) ? uint32_t(memory::OwnerLoad(address + 48168, 4)) : guest::LoadU32(base, address + 48168))); }
  void set_title_screen_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->title_screen_loaded = v; return; } if (memory::IsHandle(address + 48168)) memory::OwnerStore(address + 48168, 4, v); else guest::StoreU32(base, address + 48168, v); }
  uint32_t title_screen_loaded() const { if (auto* d = direct()) return d->title_screen_loaded; return uint32_t((memory::IsHandle(address + 48168) ? uint32_t(memory::OwnerLoad(address + 48168, 4)) : guest::LoadU32(base, address + 48168))); }
  void set_title_screen_loaded(uint32_t v) const { if (auto* d = direct()) { d->title_screen_loaded = v; return; } if (memory::IsHandle(address + 48168)) memory::OwnerStore(address + 48168, 4, v); else guest::StoreU32(base, address + 48168, v); }
  // 1 from Init
  uint32_t title_screen_reenter_address() const { return address + 48172; }
  uint32_t title_screen_reenter_bits() const { if (auto* d = direct()) return d->title_screen_reenter; return uint32_t((memory::IsHandle(address + 48172) ? uint32_t(memory::OwnerLoad(address + 48172, 4)) : guest::LoadU32(base, address + 48172))); }
  void set_title_screen_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->title_screen_reenter = v; return; } if (memory::IsHandle(address + 48172)) memory::OwnerStore(address + 48172, 4, v); else guest::StoreU32(base, address + 48172, v); }
  uint32_t title_screen_reenter() const { if (auto* d = direct()) return d->title_screen_reenter; return uint32_t((memory::IsHandle(address + 48172) ? uint32_t(memory::OwnerLoad(address + 48172, 4)) : guest::LoadU32(base, address + 48172))); }
  void set_title_screen_reenter(uint32_t v) const { if (auto* d = direct()) { d->title_screen_reenter = v; return; } if (memory::IsHandle(address + 48172)) memory::OwnerStore(address + 48172, 4, v); else guest::StoreU32(base, address + 48172, v); }
  // 0 from Init
  uint32_t title_screen_delete_movie_address() const { return address + 48176; }
  uint32_t title_screen_delete_movie_bits() const { if (auto* d = direct()) return d->title_screen_delete_movie; return uint32_t((memory::IsHandle(address + 48176) ? uint32_t(memory::OwnerLoad(address + 48176, 4)) : guest::LoadU32(base, address + 48176))); }
  void set_title_screen_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->title_screen_delete_movie = v; return; } if (memory::IsHandle(address + 48176)) memory::OwnerStore(address + 48176, 4, v); else guest::StoreU32(base, address + 48176, v); }
  uint32_t title_screen_delete_movie() const { if (auto* d = direct()) return d->title_screen_delete_movie; return uint32_t((memory::IsHandle(address + 48176) ? uint32_t(memory::OwnerLoad(address + 48176, 4)) : guest::LoadU32(base, address + 48176))); }
  void set_title_screen_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->title_screen_delete_movie = v; return; } if (memory::IsHandle(address + 48176)) memory::OwnerStore(address + 48176, 4, v); else guest::StoreU32(base, address + 48176, v); }
  // the last machine (MachineOf's index, 0 to 4) in state 4 or 5 (FindShown)
  uint32_t shown_address() const { return address + 48200; }
  uint32_t shown_bits() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 48200) ? uint32_t(memory::OwnerLoad(address + 48200, 4)) : guest::LoadU32(base, address + 48200))); }
  void set_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 48200)) memory::OwnerStore(address + 48200, 4, v); else guest::StoreU32(base, address + 48200, v); }
  uint32_t shown() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 48200) ? uint32_t(memory::OwnerLoad(address + 48200, 4)) : guest::LoadU32(base, address + 48200))); }
  void set_shown(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 48200)) memory::OwnerStore(address + 48200, 4, v); else guest::StoreU32(base, address + 48200, v); }
  // the next mode's 8-byte answer (NextMode, ForceExit; NextModeAnswer copies it): its first word, the byte 1 and three bytes
  uint32_t next_answer_address() const { return address + 48212; }
  uint32_t next_answer_bits() const { if (auto* d = direct()) return d->next_answer; return uint32_t((memory::IsHandle(address + 48212) ? uint32_t(memory::OwnerLoad(address + 48212, 4)) : guest::LoadU32(base, address + 48212))); }
  void set_next_answer_bits(uint32_t v) const { if (auto* d = direct()) { d->next_answer = v; return; } if (memory::IsHandle(address + 48212)) memory::OwnerStore(address + 48212, 4, v); else guest::StoreU32(base, address + 48212, v); }
  uint32_t next_answer() const { if (auto* d = direct()) return d->next_answer; return uint32_t((memory::IsHandle(address + 48212) ? uint32_t(memory::OwnerLoad(address + 48212, 4)) : guest::LoadU32(base, address + 48212))); }
  void set_next_answer(uint32_t v) const { if (auto* d = direct()) { d->next_answer = v; return; } if (memory::IsHandle(address + 48212)) memory::OwnerStore(address + 48212, 4, v); else guest::StoreU32(base, address + 48212, v); }
  // the answer's next mode type (1, 2, 3, 6, 7, 8, 10, 12; 4 or 12 from ForceExit)
  uint32_t next_type_address() const { return address + 48216; }
  uint32_t next_type_bits() const { if (auto* d = direct()) return d->next_type; return uint32_t((memory::IsHandle(address + 48216) ? uint32_t(memory::OwnerLoad(address + 48216, 4)) : guest::LoadU32(base, address + 48216))); }
  void set_next_type_bits(uint32_t v) const { if (auto* d = direct()) { d->next_type = v; return; } if (memory::IsHandle(address + 48216)) memory::OwnerStore(address + 48216, 4, v); else guest::StoreU32(base, address + 48216, v); }
  uint32_t next_type() const { if (auto* d = direct()) return d->next_type; return uint32_t((memory::IsHandle(address + 48216) ? uint32_t(memory::OwnerLoad(address + 48216, 4)) : guest::LoadU32(base, address + 48216))); }
  void set_next_type(uint32_t v) const { if (auto* d = direct()) { d->next_type = v; return; } if (memory::IsHandle(address + 48216)) memory::OwnerStore(address + 48216, 4, v); else guest::StoreU32(base, address + 48216, v); }
};

// embedded; measure: ModeType4::MachineOf.ret, TitleLoad, TitleScreen::LeavePage.r4, new<48220>@ModeType4::Init@+38560
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
  template <uint32_t Stride = kSize>
  static TitleLoadBlock NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TitleLoadBlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TitleLoadBlockData*>(::operator new(sizeof(TitleLoadBlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TitleLoadBlockData();
    return TitleLoadBlock(base, memory::MakeHandle(p, sizeof(TitleLoadBlockData) * count, "TitleLoadBlock", &kTitleLoadBlockLayout, sizeof(TitleLoadBlockData)));
  }
  TitleLoadBlock at(uint32_t i) const { return TitleLoadBlock(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  TitleLoadBlockData* direct() const { return static_cast<TitleLoadBlockData*>(memory::HandleOwn(address, &kTitleLoadBlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTitleLoadBlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTitleLoadBlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0 from Create; not read
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_unknown_0_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_unknown_0(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the debug sequence's tick count
  uint32_t tick_count_address() const { return address + 4; }
  uint32_t tick_count_bits() const { if (auto* d = direct()) return d->tick_count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_tick_count_bits(uint32_t v) const { if (auto* d = direct()) { d->tick_count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t tick_count() const { if (auto* d = direct()) return d->tick_count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_tick_count(uint32_t v) const { if (auto* d = direct()) { d->tick_count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the transition Update runs (-1 none; 12 the logo)
  uint32_t transition_address() const { return address + 8; }
  uint32_t transition_bits() const { if (auto* d = direct()) return d->transition; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_transition_bits(uint32_t v) const { if (auto* d = direct()) { d->transition = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t transition() const { if (auto* d = direct()) return d->transition; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_transition(uint32_t v) const { if (auto* d = direct()) { d->transition = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the page Draw runs (-1 none; 4 posts the close, 5 waits for the archive)
  uint32_t page_address() const { return address + 12; }
  uint32_t page_bits() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_page_bits(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t page() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_page(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the transition's started word
  uint32_t transition_started_address() const { return address + 16; }
  uint32_t transition_started_bits() const { if (auto* d = direct()) return d->transition_started; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_transition_started_bits(uint32_t v) const { if (auto* d = direct()) { d->transition_started = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t transition_started() const { if (auto* d = direct()) return d->transition_started; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_transition_started(uint32_t v) const { if (auto* d = direct()) { d->transition_started = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
};

// allocated; measure: new<68>@TitleScreen::Create#2
#pragma pack(push, 1)
struct TitleScreenDataData {
  uint32_t unknown_0 = 0;  // +0
  uint32_t main_item = 0;  // +4
  uint32_t can_continue = 0;  // +8
  uint32_t career_third_item = 0;  // +12
  uint32_t from_boot = 0;  // +16
  uint32_t career_selection = 0;  // +20
  uint32_t menu4_choice = 0;  // +24
  uint32_t save_item_refused = 0;  // +28
  uint32_t pad_item_state = 0;  // +32
  uint32_t idle_ticks = 0;  // +36
  uint32_t interrupted = 0;  // +40
  uint8_t pad_44[24] = {};
};
#pragma pack(pop)
static_assert(sizeof(TitleScreenDataData) == 68);
inline constexpr memory::LayoutField kTitleScreenDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}};
inline constexpr memory::Layout kTitleScreenDataLayout = {kTitleScreenDataFields, 11};

struct TitleScreenData : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TitleScreenDataData* data() const { return static_cast<TitleScreenDataData*>(memory::HandleObject(address)); }
  static TitleScreenData New(uint8_t* base) {
    return TitleScreenData(base, memory::MakeHandle(new TitleScreenDataData(), sizeof(TitleScreenDataData), "TitleScreenData", &kTitleScreenDataLayout));
  }
  template <uint32_t Stride = kSize>
  static TitleScreenData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(TitleScreenDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<TitleScreenDataData*>(::operator new(sizeof(TitleScreenDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) TitleScreenDataData();
    return TitleScreenData(base, memory::MakeHandle(p, sizeof(TitleScreenDataData) * count, "TitleScreenData", &kTitleScreenDataLayout, sizeof(TitleScreenDataData)));
  }
  TitleScreenData at(uint32_t i) const { return TitleScreenData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  TitleScreenDataData* direct() const { return static_cast<TitleScreenDataData*>(memory::HandleOwn(address, &kTitleScreenDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTitleScreenDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTitleScreenDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0 from Create; not read
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_unknown_0_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_unknown_0(uint32_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the main menu's item (0 to 6; 8 the attract race), which picks the next mode (Destroy)
  uint32_t main_item_address() const { return address + 4; }
  uint32_t main_item_bits() const { if (auto* d = direct()) return d->main_item; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_main_item_bits(uint32_t v) const { if (auto* d = direct()) { d->main_item = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t main_item() const { if (auto* d = direct()) return d->main_item; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_main_item(uint32_t v) const { if (auto* d = direct()) { d->main_item = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // player 0's day and garage count nonzero
  uint32_t can_continue_address() const { return address + 8; }
  uint32_t can_continue_bits() const { if (auto* d = direct()) return d->can_continue; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_can_continue_bits(uint32_t v) const { if (auto* d = direct()) { d->can_continue = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t can_continue() const { if (auto* d = direct()) return d->can_continue; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_can_continue(uint32_t v) const { if (auto* d = direct()) { d->can_continue = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the career menu's third item shown (bit 0x20 of player 0's save +6092)
  uint32_t career_third_item_address() const { return address + 12; }
  uint32_t career_third_item_bits() const { if (auto* d = direct()) return d->career_third_item; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_career_third_item_bits(uint32_t v) const { if (auto* d = direct()) { d->career_third_item = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t career_third_item() const { if (auto* d = direct()) return d->career_third_item; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_career_third_item(uint32_t v) const { if (auto* d = direct()) { d->career_third_item = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the intro and press start played (from [0x826364EC], Start)
  uint32_t from_boot_address() const { return address + 16; }
  uint32_t from_boot_bits() const { if (auto* d = direct()) return d->from_boot; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_from_boot_bits(uint32_t v) const { if (auto* d = direct()) { d->from_boot = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t from_boot() const { if (auto* d = direct()) return d->from_boot; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_from_boot(uint32_t v) const { if (auto* d = direct()) { d->from_boot = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // the career menu's first selection
  uint32_t career_selection_address() const { return address + 20; }
  uint32_t career_selection_bits() const { if (auto* d = direct()) return d->career_selection; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_career_selection_bits(uint32_t v) const { if (auto* d = direct()) { d->career_selection = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t career_selection() const { if (auto* d = direct()) return d->career_selection; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_career_selection(uint32_t v) const { if (auto* d = direct()) { d->career_selection = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // Menu4's choice
  uint32_t menu4_choice_address() const { return address + 24; }
  uint32_t menu4_choice_bits() const { if (auto* d = direct()) return d->menu4_choice; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_menu4_choice_bits(uint32_t v) const { if (auto* d = direct()) { d->menu4_choice = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t menu4_choice() const { if (auto* d = direct()) return d->menu4_choice; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_menu4_choice(uint32_t v) const { if (auto* d = direct()) { d->menu4_choice = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // item 5 refused (SaveSlots::State(0) 0)
  uint32_t save_item_refused_address() const { return address + 28; }
  uint32_t save_item_refused_bits() const { if (auto* d = direct()) return d->save_item_refused; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_save_item_refused_bits(uint32_t v) const { if (auto* d = direct()) { d->save_item_refused = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t save_item_refused() const { if (auto* d = direct()) return d->save_item_refused; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_save_item_refused(uint32_t v) const { if (auto* d = direct()) { d->save_item_refused = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // item 4's pad state (player 1's controller connected)
  uint32_t pad_item_state_address() const { return address + 32; }
  uint32_t pad_item_state_bits() const { if (auto* d = direct()) return d->pad_item_state; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_pad_item_state_bits(uint32_t v) const { if (auto* d = direct()) { d->pad_item_state = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t pad_item_state() const { if (auto* d = direct()) return d->pad_item_state; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_pad_item_state(uint32_t v) const { if (auto* d = direct()) { d->pad_item_state = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // press start's idle ticks; 600 starts the attract race
  uint32_t idle_ticks_address() const { return address + 36; }
  uint32_t idle_ticks_bits() const { if (auto* d = direct()) return d->idle_ticks; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_idle_ticks_bits(uint32_t v) const { if (auto* d = direct()) { d->idle_ticks = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t idle_ticks() const { if (auto* d = direct()) return d->idle_ticks; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_idle_ticks(uint32_t v) const { if (auto* d = direct()) { d->idle_ticks = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  // set by message 17 (OnInterrupt), cleared by 18 (OnResume)
  uint32_t interrupted_address() const { return address + 40; }
  uint32_t interrupted_bits() const { if (auto* d = direct()) return d->interrupted; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_interrupted_bits(uint32_t v) const { if (auto* d = direct()) { d->interrupted = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t interrupted() const { if (auto* d = direct()) return d->interrupted; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_interrupted(uint32_t v) const { if (auto* d = direct()) { d->interrupted = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
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
