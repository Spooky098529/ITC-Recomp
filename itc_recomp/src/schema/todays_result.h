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
  ModeType9Data* direct() const { return static_cast<ModeType9Data*>(memory::HandleOwn(address, &kModeType9Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeType9Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType9Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82006CAC (14 slots)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), the delegates' r3
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType9Layout, 0, 1)); return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(base, address + 0))); }
  void set_answer_changed_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType9Layout, 0, 1, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the next-mode answer's 8 bytes (ModeAnswer)
  uint32_t answer_address() const { return address + 0; }
  uint64_t answer_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType9Layout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(base, address + 0))); }
  void set_answer_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType9Layout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  uint64_t answer() const { return uint64_t(answer_bits()); }
  void set_answer(uint64_t v) const { set_answer_bits(uint64_t(v)); }
  // the delegate at +8, 0 none (also the answer's next type +4 by the measure's join)
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
  // a Mode::SlotNThunk or Mode::NextModeThunk 0x82235C08
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
  // 0x82006CA0: IsLoaded, ForceExit, an empty function
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32_bits() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_vtable32_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_vtable32(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (the two machines)
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
  // measure fault: IsLoaded's and ForceExit's r3 is the mode + 32, so this is the mode's +848 (block)
  uint32_t iface_block_address() const { return address + 816; }
  uint32_t iface_block_bits() const { if (auto* d = direct()) return d->iface_block; return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(base, address + 816))); }
  void set_iface_block_bits(uint32_t v) const { if (auto* d = direct()) { d->iface_block = v; return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(base, address + 816, v); }
  template <typename T = ModeType9Block>
  T iface_block() const { return T(base, iface_block_bits()); }
  void set_iface_block(const View& w) const { set_iface_block_bits(w.address); }
  void set_iface_block(uint32_t v) const { set_iface_block_bits(v); }
  // VirtualAlloc'ed by Init, freed by Release
  uint32_t block_address() const { return address + 848; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 848) ? uint32_t(memory::OwnerLoad(address + 848, 4)) : guest::LoadU32(base, address + 848))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 848)) memory::OwnerStore(address + 848, 4, v); else guest::StoreU32(base, address + 848, v); }
  template <typename T = ModeType9Block>
  T block() const { return T(base, block_bits()); }
  void set_block(const View& w) const { set_block_bits(w.address); }
  void set_block(uint32_t v) const { set_block_bits(v); }
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
inline constexpr memory::LayoutField kModeType9BlockFields[] = {{9600, 4}, {9604, 4}, {9608, 4}, {9612, 4}, {9616, 4}, {19240, 4}, {19244, 4}, {19248, 4}, {19252, 4}, {19256, 4}, {19280, 4}, {19284, 4}, {19292, 8}};
inline constexpr memory::Layout kModeType9BlockLayout = {kModeType9BlockFields, 13};

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
  ModeType9BlockData* direct() const { return static_cast<ModeType9BlockData*>(memory::HandleOwn(address, &kModeType9BlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeType9BlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType9BlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // today's result's machine (its +9604 state, +9608 loaded, +9612 reenter 1 at Init, +9616 delete_movie 0 at Init)
  template <typename T = TodaysResultMachine>
  T result() const { return T(base, address + 0); }
  // today's event's machine (the same words at +19244 to +19256)
  template <typename T = TodaysEventMachine>
  T event() const { return T(base, address + 9640); }
  // today's result's machine's state
  uint32_t result_state_address() const { return address + 9604; }
  uint32_t result_state_bits() const { if (auto* d = direct()) return d->result_state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_result_state_bits(uint32_t v) const { if (auto* d = direct()) { d->result_state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  uint32_t result_state() const { if (auto* d = direct()) return d->result_state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_result_state(uint32_t v) const { if (auto* d = direct()) { d->result_state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  // movie 27 loaded
  uint32_t result_loaded_address() const { return address + 9608; }
  uint32_t result_loaded_bits() const { if (auto* d = direct()) return d->result_loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_result_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->result_loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  uint32_t result_loaded() const { if (auto* d = direct()) return d->result_loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_result_loaded(uint32_t v) const { if (auto* d = direct()) { d->result_loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  // 1 at Init
  uint32_t result_reenter_address() const { return address + 9612; }
  uint32_t result_reenter_bits() const { if (auto* d = direct()) return d->result_reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_result_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->result_reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  uint32_t result_reenter() const { if (auto* d = direct()) return d->result_reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_result_reenter(uint32_t v) const { if (auto* d = direct()) { d->result_reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  // 0 at Init
  uint32_t result_delete_movie_address() const { return address + 9616; }
  uint32_t result_delete_movie_bits() const { if (auto* d = direct()) return d->result_delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_result_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->result_delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
  uint32_t result_delete_movie() const { if (auto* d = direct()) return d->result_delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_result_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->result_delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
  // today's event's machine's state
  uint32_t event_state_address() const { return address + 19244; }
  uint32_t event_state_bits() const { if (auto* d = direct()) return d->event_state; return uint32_t((memory::IsHandle(address + 19244) ? uint32_t(memory::OwnerLoad(address + 19244, 4)) : guest::LoadU32(base, address + 19244))); }
  void set_event_state_bits(uint32_t v) const { if (auto* d = direct()) { d->event_state = v; return; } if (memory::IsHandle(address + 19244)) memory::OwnerStore(address + 19244, 4, v); else guest::StoreU32(base, address + 19244, v); }
  uint32_t event_state() const { if (auto* d = direct()) return d->event_state; return uint32_t((memory::IsHandle(address + 19244) ? uint32_t(memory::OwnerLoad(address + 19244, 4)) : guest::LoadU32(base, address + 19244))); }
  void set_event_state(uint32_t v) const { if (auto* d = direct()) { d->event_state = v; return; } if (memory::IsHandle(address + 19244)) memory::OwnerStore(address + 19244, 4, v); else guest::StoreU32(base, address + 19244, v); }
  // movie 0 and the images loaded
  uint32_t event_loaded_address() const { return address + 19248; }
  uint32_t event_loaded_bits() const { if (auto* d = direct()) return d->event_loaded; return uint32_t((memory::IsHandle(address + 19248) ? uint32_t(memory::OwnerLoad(address + 19248, 4)) : guest::LoadU32(base, address + 19248))); }
  void set_event_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->event_loaded = v; return; } if (memory::IsHandle(address + 19248)) memory::OwnerStore(address + 19248, 4, v); else guest::StoreU32(base, address + 19248, v); }
  uint32_t event_loaded() const { if (auto* d = direct()) return d->event_loaded; return uint32_t((memory::IsHandle(address + 19248) ? uint32_t(memory::OwnerLoad(address + 19248, 4)) : guest::LoadU32(base, address + 19248))); }
  void set_event_loaded(uint32_t v) const { if (auto* d = direct()) { d->event_loaded = v; return; } if (memory::IsHandle(address + 19248)) memory::OwnerStore(address + 19248, 4, v); else guest::StoreU32(base, address + 19248, v); }
  // 1 at Init
  uint32_t event_reenter_address() const { return address + 19252; }
  uint32_t event_reenter_bits() const { if (auto* d = direct()) return d->event_reenter; return uint32_t((memory::IsHandle(address + 19252) ? uint32_t(memory::OwnerLoad(address + 19252, 4)) : guest::LoadU32(base, address + 19252))); }
  void set_event_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->event_reenter = v; return; } if (memory::IsHandle(address + 19252)) memory::OwnerStore(address + 19252, 4, v); else guest::StoreU32(base, address + 19252, v); }
  uint32_t event_reenter() const { if (auto* d = direct()) return d->event_reenter; return uint32_t((memory::IsHandle(address + 19252) ? uint32_t(memory::OwnerLoad(address + 19252, 4)) : guest::LoadU32(base, address + 19252))); }
  void set_event_reenter(uint32_t v) const { if (auto* d = direct()) { d->event_reenter = v; return; } if (memory::IsHandle(address + 19252)) memory::OwnerStore(address + 19252, 4, v); else guest::StoreU32(base, address + 19252, v); }
  // 0 at Init
  uint32_t event_delete_movie_address() const { return address + 19256; }
  uint32_t event_delete_movie_bits() const { if (auto* d = direct()) return d->event_delete_movie; return uint32_t((memory::IsHandle(address + 19256) ? uint32_t(memory::OwnerLoad(address + 19256, 4)) : guest::LoadU32(base, address + 19256))); }
  void set_event_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->event_delete_movie = v; return; } if (memory::IsHandle(address + 19256)) memory::OwnerStore(address + 19256, 4, v); else guest::StoreU32(base, address + 19256, v); }
  uint32_t event_delete_movie() const { if (auto* d = direct()) return d->event_delete_movie; return uint32_t((memory::IsHandle(address + 19256) ? uint32_t(memory::OwnerLoad(address + 19256, 4)) : guest::LoadU32(base, address + 19256))); }
  void set_event_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->event_delete_movie = v; return; } if (memory::IsHandle(address + 19256)) memory::OwnerStore(address + 19256, 4, v); else guest::StoreU32(base, address + 19256, v); }
  // the shown machine's index: 27 today's result, 0 today's event (FindShown)
  uint32_t shown_address() const { return address + 19280; }
  uint32_t shown_bits() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 19280) ? uint32_t(memory::OwnerLoad(address + 19280, 4)) : guest::LoadU32(base, address + 19280))); }
  void set_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 19280)) memory::OwnerStore(address + 19280, 4, v); else guest::StoreU32(base, address + 19280, v); }
  uint32_t shown() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 19280) ? uint32_t(memory::OwnerLoad(address + 19280, 4)) : guest::LoadU32(base, address + 19280))); }
  void set_shown(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 19280)) memory::OwnerStore(address + 19280, 4, v); else guest::StoreU32(base, address + 19280, v); }
  // the same at Init (readers not looked for)
  uint32_t shown_at_init_address() const { return address + 19284; }
  uint32_t shown_at_init_bits() const { if (auto* d = direct()) return d->shown_at_init; return uint32_t((memory::IsHandle(address + 19284) ? uint32_t(memory::OwnerLoad(address + 19284, 4)) : guest::LoadU32(base, address + 19284))); }
  void set_shown_at_init_bits(uint32_t v) const { if (auto* d = direct()) { d->shown_at_init = v; return; } if (memory::IsHandle(address + 19284)) memory::OwnerStore(address + 19284, 4, v); else guest::StoreU32(base, address + 19284, v); }
  uint32_t shown_at_init() const { if (auto* d = direct()) return d->shown_at_init; return uint32_t((memory::IsHandle(address + 19284) ? uint32_t(memory::OwnerLoad(address + 19284, 4)) : guest::LoadU32(base, address + 19284))); }
  void set_shown_at_init(uint32_t v) const { if (auto* d = direct()) { d->shown_at_init = v; return; } if (memory::IsHandle(address + 19284)) memory::OwnerStore(address + 19284, 4, v); else guest::StoreU32(base, address + 19284, v); }
  // the next mode's 8-byte answer ForceExit writes and NextModeAnswer hands to Modes::Tick (ModeAnswer)
  uint32_t answer_address() const { return address + 19292; }
  uint64_t answer_bits() const { if (auto* d = direct()) return d->answer; return uint64_t((memory::IsHandle(address + 19292) ? uint64_t(memory::OwnerLoad(address + 19292, 8)) : guest::LoadU64(base, address + 19292))); }
  void set_answer_bits(uint64_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 19292)) memory::OwnerStore(address + 19292, 8, v); else guest::StoreU64(base, address + 19292, v); }
  uint64_t answer() const { if (auto* d = direct()) return d->answer; return uint64_t((memory::IsHandle(address + 19292) ? uint64_t(memory::OwnerLoad(address + 19292, 8)) : guest::LoadU64(base, address + 19292))); }
  void set_answer(uint64_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 19292)) memory::OwnerStore(address + 19292, 8, v); else guest::StoreU64(base, address + 19292, v); }
  // the answer's first word (changed and three bytes), as NextModeAnswer copies it
  uint32_t answer_word0_address() const { return address + 19292; }
  uint32_t answer_word0_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType9BlockLayout, 19292, 4)); return uint32_t((memory::IsHandle(address + 19292) ? uint32_t(memory::OwnerLoad(address + 19292, 4)) : guest::LoadU32(base, address + 19292))); }
  void set_answer_word0_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType9BlockLayout, 19292, 4, v); return; } if (memory::IsHandle(address + 19292)) memory::OwnerStore(address + 19292, 4, v); else guest::StoreU32(base, address + 19292, v); }
  uint32_t answer_word0() const { return uint32_t(answer_word0_bits()); }
  void set_answer_word0(uint32_t v) const { set_answer_word0_bits(uint32_t(v)); }
  // the answer's next mode type
  uint32_t answer_next_type_address() const { return address + 19296; }
  uint32_t answer_next_type_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType9BlockLayout, 19296, 4)); return uint32_t((memory::IsHandle(address + 19296) ? uint32_t(memory::OwnerLoad(address + 19296, 4)) : guest::LoadU32(base, address + 19296))); }
  void set_answer_next_type_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType9BlockLayout, 19296, 4, v); return; } if (memory::IsHandle(address + 19296)) memory::OwnerStore(address + 19296, 4, v); else guest::StoreU32(base, address + 19296, v); }
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
  TodaysResultMachineData* direct() const { return static_cast<TodaysResultMachineData*>(memory::HandleOwn(address, &kTodaysResultMachineLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTodaysResultMachineLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysResultMachineLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  uint8_t* transitions() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  // measure fault: the transitions' r3 is the page object's +24 (PageObject.step_started)
  uint32_t fault_step_started_address() const { return address + 0; }
  uint32_t fault_step_started_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysResultMachineLayout, 0, 4)); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_fault_step_started_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysResultMachineLayout, 0, 4, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t fault_step_started() const { return uint32_t(fault_step_started_bits()); }
  void set_fault_step_started(uint32_t v) const { set_fault_step_started_bits(uint32_t(v)); }
  // measure fault: LeavePage's r3 is the dispatcher (ModeDispatcher.ring_count)
  uint32_t fault_ring_count_address() const { return address + 804; }
  uint32_t fault_ring_count_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysResultMachineLayout, 804, 4)); return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_fault_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysResultMachineLayout, 804, 4, v); return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  uint32_t fault_ring_count() const { return uint32_t(fault_ring_count_bits()); }
  void set_fault_ring_count(uint32_t v) const { set_fault_ring_count_bits(uint32_t(v)); }
  // measure fault: LeavePage's r3 is the dispatcher (ModeDispatcher.ring_start)
  uint32_t fault_ring_start_address() const { return address + 808; }
  uint32_t fault_ring_start_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysResultMachineLayout, 808, 4)); return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_fault_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysResultMachineLayout, 808, 4, v); return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t fault_ring_start() const { return uint32_t(fault_ring_start_bits()); }
  void set_fault_ring_start(uint32_t v) const { set_fault_ring_start_bits(uint32_t(v)); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t transition_count_bits() const { if (auto* d = direct()) return d->transition_count; return uint32_t((memory::IsHandle(address + 9600) ? uint32_t(memory::OwnerLoad(address + 9600, 4)) : guest::LoadU32(base, address + 9600))); }
  void set_transition_count_bits(uint32_t v) const { if (auto* d = direct()) { d->transition_count = v; return; } if (memory::IsHandle(address + 9600)) memory::OwnerStore(address + 9600, 4, v); else guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count() const { if (auto* d = direct()) return d->transition_count; return uint32_t((memory::IsHandle(address + 9600) ? uint32_t(memory::OwnerLoad(address + 9600, 4)) : guest::LoadU32(base, address + 9600))); }
  void set_transition_count(uint32_t v) const { if (auto* d = direct()) { d->transition_count = v; return; } if (memory::IsHandle(address + 9600)) memory::OwnerStore(address + 9600, 4, v); else guest::StoreU32(base, address + 9600, v); }
  // 1 or 9 at Create, 4 shown, 7 leaving, 10 unloaded, 13 done, 20 done with a movie queued
  uint32_t state_address() const { return address + 9604; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  // movie 27 loaded
  uint32_t loaded_address() const { return address + 9608; }
  uint32_t loaded_bits() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_loaded(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  // nonzero: Create and WaitForMovie go to state 9 (open on message 6)
  uint32_t reenter_address() const { return address + 9612; }
  uint32_t reenter_bits() const { if (auto* d = direct()) return d->reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter() const { if (auto* d = direct()) return d->reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_reenter(uint32_t v) const { if (auto* d = direct()) { d->reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  // nonzero: Unload deletes movie 27 and clears loaded
  uint32_t delete_movie_address() const { return address + 9616; }
  uint32_t delete_movie_bits() const { if (auto* d = direct()) return d->delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie() const { if (auto* d = direct()) return d->delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
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
  TodaysEventMachineData* direct() const { return static_cast<TodaysEventMachineData*>(memory::HandleOwn(address, &kTodaysEventMachineLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTodaysEventMachineLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  uint8_t* transitions() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  // measure fault: the transitions' r3 (PageObject.step_started), and the data's +0 (TodaysEventData.ready)
  uint32_t fault_step_started_address() const { return address + 0; }
  uint32_t fault_step_started_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 0, 4)); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_fault_step_started_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 0, 4, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t fault_step_started() const { return uint32_t(fault_step_started_bits()); }
  void set_fault_step_started(uint32_t v) const { set_fault_step_started_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.images_a's first record
  uint32_t fault_data_image_records_a_address() const { return address + 4; }
  uint32_t fault_data_image_records_a_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 4, 4)); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_fault_data_image_records_a_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 4, 4, v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t fault_data_image_records_a() const { return uint32_t(fault_data_image_records_a_bits()); }
  void set_fault_data_image_records_a(uint32_t v) const { set_fault_data_image_records_a_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.image_count_a
  uint32_t fault_data_image_count_a_address() const { return address + 188; }
  uint32_t fault_data_image_count_a_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 188, 4)); return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(base, address + 188))); }
  void set_fault_data_image_count_a_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 188, 4, v); return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(base, address + 188, v); }
  uint32_t fault_data_image_count_a() const { return uint32_t(fault_data_image_count_a_bits()); }
  void set_fault_data_image_count_a(uint32_t v) const { set_fault_data_image_count_a_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.images_b's first record
  uint32_t fault_data_image_records_b_address() const { return address + 192; }
  uint32_t fault_data_image_records_b_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 192, 4)); return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(base, address + 192))); }
  void set_fault_data_image_records_b_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 192, 4, v); return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(base, address + 192, v); }
  uint32_t fault_data_image_records_b() const { return uint32_t(fault_data_image_records_b_bits()); }
  void set_fault_data_image_records_b(uint32_t v) const { set_fault_data_image_records_b_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.image_count_b
  uint32_t fault_data_image_count_b_address() const { return address + 376; }
  uint32_t fault_data_image_count_b_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 376, 4)); return uint32_t((memory::IsHandle(address + 376) ? uint32_t(memory::OwnerLoad(address + 376, 4)) : guest::LoadU32(base, address + 376))); }
  void set_fault_data_image_count_b_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 376, 4, v); return; } if (memory::IsHandle(address + 376)) memory::OwnerStore(address + 376, 4, v); else guest::StoreU32(base, address + 376, v); }
  uint32_t fault_data_image_count_b() const { return uint32_t(fault_data_image_count_b_bits()); }
  void set_fault_data_image_count_b(uint32_t v) const { set_fault_data_image_count_b_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.image_bits_0
  uint32_t fault_data_image_bits_0_address() const { return address + 380; }
  uint32_t fault_data_image_bits_0_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 380, 4)); return uint32_t((memory::IsHandle(address + 380) ? uint32_t(memory::OwnerLoad(address + 380, 4)) : guest::LoadU32(base, address + 380))); }
  void set_fault_data_image_bits_0_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 380, 4, v); return; } if (memory::IsHandle(address + 380)) memory::OwnerStore(address + 380, 4, v); else guest::StoreU32(base, address + 380, v); }
  uint32_t fault_data_image_bits_0() const { return uint32_t(fault_data_image_bits_0_bits()); }
  void set_fault_data_image_bits_0(uint32_t v) const { set_fault_data_image_bits_0_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.image_bits_1
  uint32_t fault_data_image_bits_1_address() const { return address + 384; }
  uint32_t fault_data_image_bits_1_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 384, 4)); return uint32_t((memory::IsHandle(address + 384) ? uint32_t(memory::OwnerLoad(address + 384, 4)) : guest::LoadU32(base, address + 384))); }
  void set_fault_data_image_bits_1_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 384, 4, v); return; } if (memory::IsHandle(address + 384)) memory::OwnerStore(address + 384, 4, v); else guest::StoreU32(base, address + 384, v); }
  uint32_t fault_data_image_bits_1() const { return uint32_t(fault_data_image_bits_1_bits()); }
  void set_fault_data_image_bits_1(uint32_t v) const { set_fault_data_image_bits_1_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.unlock_table
  uint32_t fault_data_unlock_table_address() const { return address + 392; }
  uint32_t fault_data_unlock_table_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 392, 4)); return uint32_t((memory::IsHandle(address + 392) ? uint32_t(memory::OwnerLoad(address + 392, 4)) : guest::LoadU32(base, address + 392))); }
  void set_fault_data_unlock_table_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 392, 4, v); return; } if (memory::IsHandle(address + 392)) memory::OwnerStore(address + 392, 4, v); else guest::StoreU32(base, address + 392, v); }
  uint32_t fault_data_unlock_table() const { return uint32_t(fault_data_unlock_table_bits()); }
  void set_fault_data_unlock_table(uint32_t v) const { set_fault_data_unlock_table_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.unlock_entry
  uint32_t fault_data_unlock_entry_address() const { return address + 396; }
  uint32_t fault_data_unlock_entry_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 396, 4)); return uint32_t((memory::IsHandle(address + 396) ? uint32_t(memory::OwnerLoad(address + 396, 4)) : guest::LoadU32(base, address + 396))); }
  void set_fault_data_unlock_entry_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 396, 4, v); return; } if (memory::IsHandle(address + 396)) memory::OwnerStore(address + 396, 4, v); else guest::StoreU32(base, address + 396, v); }
  uint32_t fault_data_unlock_entry() const { return uint32_t(fault_data_unlock_entry_bits()); }
  void set_fault_data_unlock_entry(uint32_t v) const { set_fault_data_unlock_entry_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.movie_object_36
  uint32_t fault_data_movie_object_36_address() const { return address + 400; }
  uint32_t fault_data_movie_object_36_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 400, 4)); return uint32_t((memory::IsHandle(address + 400) ? uint32_t(memory::OwnerLoad(address + 400, 4)) : guest::LoadU32(base, address + 400))); }
  void set_fault_data_movie_object_36_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 400, 4, v); return; } if (memory::IsHandle(address + 400)) memory::OwnerStore(address + 400, 4, v); else guest::StoreU32(base, address + 400, v); }
  uint32_t fault_data_movie_object_36() const { return uint32_t(fault_data_movie_object_36_bits()); }
  void set_fault_data_movie_object_36(uint32_t v) const { set_fault_data_movie_object_36_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.movie_object_32
  uint32_t fault_data_movie_object_32_address() const { return address + 404; }
  uint32_t fault_data_movie_object_32_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 404, 4)); return uint32_t((memory::IsHandle(address + 404) ? uint32_t(memory::OwnerLoad(address + 404, 4)) : guest::LoadU32(base, address + 404))); }
  void set_fault_data_movie_object_32_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 404, 4, v); return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 4, v); else guest::StoreU32(base, address + 404, v); }
  uint32_t fault_data_movie_object_32() const { return uint32_t(fault_data_movie_object_32_bits()); }
  void set_fault_data_movie_object_32(uint32_t v) const { set_fault_data_movie_object_32_bits(uint32_t(v)); }
  // measure fault: TodaysEventData.movies_queued
  uint32_t fault_data_movies_queued_address() const { return address + 408; }
  uint32_t fault_data_movies_queued_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 408, 4)); return uint32_t((memory::IsHandle(address + 408) ? uint32_t(memory::OwnerLoad(address + 408, 4)) : guest::LoadU32(base, address + 408))); }
  void set_fault_data_movies_queued_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 408, 4, v); return; } if (memory::IsHandle(address + 408)) memory::OwnerStore(address + 408, 4, v); else guest::StoreU32(base, address + 408, v); }
  uint32_t fault_data_movies_queued() const { return uint32_t(fault_data_movies_queued_bits()); }
  void set_fault_data_movies_queued(uint32_t v) const { set_fault_data_movies_queued_bits(uint32_t(v)); }
  // measure fault: EndPage's r3 is the dispatcher (ModeDispatcher.ring_count)
  uint32_t fault_ring_count_address() const { return address + 804; }
  uint32_t fault_ring_count_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 804, 4)); return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_fault_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 804, 4, v); return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  uint32_t fault_ring_count() const { return uint32_t(fault_ring_count_bits()); }
  void set_fault_ring_count(uint32_t v) const { set_fault_ring_count_bits(uint32_t(v)); }
  // measure fault: EndPage's r3 is the dispatcher (ModeDispatcher.ring_start)
  uint32_t fault_ring_start_address() const { return address + 808; }
  uint32_t fault_ring_start_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 808, 4)); return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_fault_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventMachineLayout, 808, 4, v); return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t fault_ring_start() const { return uint32_t(fault_ring_start_bits()); }
  void set_fault_ring_start(uint32_t v) const { set_fault_ring_start_bits(uint32_t(v)); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t transition_count_bits() const { if (auto* d = direct()) return d->transition_count; return uint32_t((memory::IsHandle(address + 9600) ? uint32_t(memory::OwnerLoad(address + 9600, 4)) : guest::LoadU32(base, address + 9600))); }
  void set_transition_count_bits(uint32_t v) const { if (auto* d = direct()) { d->transition_count = v; return; } if (memory::IsHandle(address + 9600)) memory::OwnerStore(address + 9600, 4, v); else guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count() const { if (auto* d = direct()) return d->transition_count; return uint32_t((memory::IsHandle(address + 9600) ? uint32_t(memory::OwnerLoad(address + 9600, 4)) : guest::LoadU32(base, address + 9600))); }
  void set_transition_count(uint32_t v) const { if (auto* d = direct()) { d->transition_count = v; return; } if (memory::IsHandle(address + 9600)) memory::OwnerStore(address + 9600, 4, v); else guest::StoreU32(base, address + 9600, v); }
  // 1 or 9 at Create (9 with no unlock), 4 shown, 7 leaving, 9 after Unloaded, 10 at the end page, 20 done with movies queued
  uint32_t state_address() const { return address + 9604; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  // movie 0 and both image lists loaded
  uint32_t loaded_address() const { return address + 9608; }
  uint32_t loaded_bits() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_loaded(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  // nonzero: Create and WaitForMovie go to state 9
  uint32_t reenter_address() const { return address + 9612; }
  uint32_t reenter_bits() const { if (auto* d = direct()) return d->reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter() const { if (auto* d = direct()) return d->reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_reenter(uint32_t v) const { if (auto* d = direct()) { d->reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  // nonzero: Unload deletes movie 0 and clears loaded
  uint32_t delete_movie_address() const { return address + 9616; }
  uint32_t delete_movie_bits() const { if (auto* d = direct()) return d->delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie() const { if (auto* d = direct()) return d->delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(base, address + 9616))); }
  void set_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(base, address + 9616, v); }
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
  TodaysResultDataData* direct() const { return static_cast<TodaysResultDataData*>(memory::HandleOwn(address, &kTodaysResultDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTodaysResultDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysResultDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 1 once the texts are set (SetUpMovie)
  uint32_t ready_address() const { return address + 0; }
  uint32_t ready_bits() const { if (auto* d = direct()) return d->ready; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->ready = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t ready() const { if (auto* d = direct()) return d->ready; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_ready(uint32_t v) const { if (auto* d = direct()) { d->ready = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // 1 when LeavePage queued a movie; Destroy then goes to state 20
  uint32_t movie_queued_address() const { return address + 4; }
  uint32_t movie_queued_bits() const { if (auto* d = direct()) return d->movie_queued; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_movie_queued_bits(uint32_t v) const { if (auto* d = direct()) { d->movie_queued = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t movie_queued() const { if (auto* d = direct()) return d->movie_queued; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_movie_queued(uint32_t v) const { if (auto* d = direct()) { d->movie_queued = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t day_wins_address() const { return address + 8; }
  uint32_t day_wins_bits() const { if (auto* d = direct()) return d->day_wins; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_day_wins_bits(uint32_t v) const { if (auto* d = direct()) { d->day_wins = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t day_wins() const { if (auto* d = direct()) return d->day_wins; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_day_wins(uint32_t v) const { if (auto* d = direct()) { d->day_wins = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t day_losses_address() const { return address + 12; }
  uint32_t day_losses_bits() const { if (auto* d = direct()) return d->day_losses; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_day_losses_bits(uint32_t v) const { if (auto* d = direct()) { d->day_losses = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t day_losses() const { if (auto* d = direct()) return d->day_losses; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_day_losses(uint32_t v) const { if (auto* d = direct()) { d->day_losses = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the day's outcome 3 count
  uint32_t day_outcome3_address() const { return address + 16; }
  uint32_t day_outcome3_bits() const { if (auto* d = direct()) return d->day_outcome3; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_day_outcome3_bits(uint32_t v) const { if (auto* d = direct()) { d->day_outcome3 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t day_outcome3() const { if (auto* d = direct()) return d->day_outcome3; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_day_outcome3(uint32_t v) const { if (auto* d = direct()) { d->day_outcome3 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // the day's win rate in percent
  uint32_t day_win_rate_address() const { return address + 20; }
  uint32_t day_win_rate_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->day_win_rate); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_day_win_rate_bits(uint32_t v) const { if (auto* d = direct()) { d->day_win_rate = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  float day_win_rate() const { return std::bit_cast<float>(day_win_rate_bits()); }
  void set_day_win_rate(float v) const { set_day_win_rate_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t day_win_streak_address() const { return address + 24; }
  uint32_t day_win_streak_bits() const { if (auto* d = direct()) return d->day_win_streak; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_day_win_streak_bits(uint32_t v) const { if (auto* d = direct()) { d->day_win_streak = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t day_win_streak() const { if (auto* d = direct()) return d->day_win_streak; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_day_win_streak(uint32_t v) const { if (auto* d = direct()) { d->day_win_streak = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // the day block's +4832 (DayBlock::Word4832)
  uint32_t day_word_4832_address() const { return address + 28; }
  uint32_t day_word_4832_bits() const { if (auto* d = direct()) return d->day_word_4832; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_day_word_4832_bits(uint32_t v) const { if (auto* d = direct()) { d->day_word_4832 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t day_word_4832() const { if (auto* d = direct()) return d->day_word_4832; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_day_word_4832(uint32_t v) const { if (auto* d = direct()) { d->day_word_4832 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t career_wins_address() const { return address + 32; }
  uint32_t career_wins_bits() const { if (auto* d = direct()) return d->career_wins; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_career_wins_bits(uint32_t v) const { if (auto* d = direct()) { d->career_wins = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t career_wins() const { if (auto* d = direct()) return d->career_wins; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_career_wins(uint32_t v) const { if (auto* d = direct()) { d->career_wins = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t career_losses_address() const { return address + 36; }
  uint32_t career_losses_bits() const { if (auto* d = direct()) return d->career_losses; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_career_losses_bits(uint32_t v) const { if (auto* d = direct()) { d->career_losses = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t career_losses() const { if (auto* d = direct()) return d->career_losses; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_career_losses(uint32_t v) const { if (auto* d = direct()) { d->career_losses = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t career_outcome3_address() const { return address + 40; }
  uint32_t career_outcome3_bits() const { if (auto* d = direct()) return d->career_outcome3; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_career_outcome3_bits(uint32_t v) const { if (auto* d = direct()) { d->career_outcome3 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t career_outcome3() const { if (auto* d = direct()) return d->career_outcome3; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_career_outcome3(uint32_t v) const { if (auto* d = direct()) { d->career_outcome3 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  // in percent
  uint32_t career_win_rate_address() const { return address + 44; }
  uint32_t career_win_rate_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->career_win_rate); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_career_win_rate_bits(uint32_t v) const { if (auto* d = direct()) { d->career_win_rate = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  float career_win_rate() const { return std::bit_cast<float>(career_win_rate_bits()); }
  void set_career_win_rate(float v) const { set_career_win_rate_bits(std::bit_cast<uint32_t>(v)); }
  // the stats' +5968
  uint32_t best_win_streak_address() const { return address + 48; }
  uint32_t best_win_streak_bits() const { if (auto* d = direct()) return d->best_win_streak; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_best_win_streak_bits(uint32_t v) const { if (auto* d = direct()) { d->best_win_streak = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  uint32_t best_win_streak() const { if (auto* d = direct()) return d->best_win_streak; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_best_win_streak(uint32_t v) const { if (auto* d = direct()) { d->best_win_streak = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  // the save's +6116
  uint32_t money_address() const { return address + 52; }
  uint32_t money_bits() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_money_bits(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  uint32_t money() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_money(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  // the save's +6104
  uint32_t mode_address() const { return address + 56; }
  uint32_t mode_bits() const { if (auto* d = direct()) return d->mode; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_mode_bits(uint32_t v) const { if (auto* d = direct()) { d->mode = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  uint32_t mode() const { if (auto* d = direct()) return d->mode; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_mode(uint32_t v) const { if (auto* d = direct()) { d->mode = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  // the day % 7
  uint32_t weekday_address() const { return address + 60; }
  uint32_t weekday_bits() const { if (auto* d = direct()) return d->weekday; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_weekday_bits(uint32_t v) const { if (auto* d = direct()) { d->weekday = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t weekday() const { if (auto* d = direct()) return d->weekday; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_weekday(uint32_t v) const { if (auto* d = direct()) { d->weekday = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  // the save's +6100, at most 999
  uint32_t day_address() const { return address + 64; }
  uint32_t day_bits() const { if (auto* d = direct()) return d->day; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(base, address + 64))); }
  void set_day_bits(uint32_t v) const { if (auto* d = direct()) { d->day = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(base, address + 64, v); }
  uint32_t day() const { if (auto* d = direct()) return d->day; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(base, address + 64))); }
  void set_day(uint32_t v) const { if (auto* d = direct()) { d->day = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(base, address + 64, v); }
  // a copy of the save's 20-byte pick at +8 (five words)
  uint32_t pick_address() const { return address + 68; }
  uint8_t* pick() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 68 : guest::Pointer(base, address + 68); }
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
inline constexpr memory::LayoutField kTodaysEventDataFields[] = {{0, 4}, {188, 4}, {376, 4}, {380, 4}, {384, 4}, {388, 4}, {392, 4}, {396, 4}, {400, 4}, {404, 4}, {408, 4}};
inline constexpr memory::Layout kTodaysEventDataLayout = {kTodaysEventDataFields, 11};

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
  TodaysEventDataData* direct() const { return static_cast<TodaysEventDataData*>(memory::HandleOwn(address, &kTodaysEventDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTodaysEventDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 1 once the texts are set (SetUpTexts); 0 at Create
  uint32_t ready_address() const { return address + 0; }
  uint32_t ready_bits() const { if (auto* d = direct()) return d->ready; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_ready_bits(uint32_t v) const { if (auto* d = direct()) { d->ready = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t ready() const { if (auto* d = direct()) return d->ready; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_ready(uint32_t v) const { if (auto* d = direct()) { d->ready = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the first images' list (46 record pointers, the count at the data's +188); assets 22696 + i
  template <typename T = TodaysEventImageList>
  T images_a() const { return T(base, address + 4); }
  // the second images' list (the count at the data's +376); assets 22741 + i
  template <typename T = TodaysEventImageList>
  T images_b() const { return T(base, address + 192); }
  // list A's count
  uint32_t image_count_a_address() const { return address + 188; }
  uint32_t image_count_a_bits() const { if (auto* d = direct()) return d->image_count_a; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(base, address + 188))); }
  void set_image_count_a_bits(uint32_t v) const { if (auto* d = direct()) { d->image_count_a = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(base, address + 188, v); }
  uint32_t image_count_a() const { if (auto* d = direct()) return d->image_count_a; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(base, address + 188))); }
  void set_image_count_a(uint32_t v) const { if (auto* d = direct()) { d->image_count_a = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(base, address + 188, v); }
  // list B's count
  uint32_t image_count_b_address() const { return address + 376; }
  uint32_t image_count_b_bits() const { if (auto* d = direct()) return d->image_count_b; return uint32_t((memory::IsHandle(address + 376) ? uint32_t(memory::OwnerLoad(address + 376, 4)) : guest::LoadU32(base, address + 376))); }
  void set_image_count_b_bits(uint32_t v) const { if (auto* d = direct()) { d->image_count_b = v; return; } if (memory::IsHandle(address + 376)) memory::OwnerStore(address + 376, 4, v); else guest::StoreU32(base, address + 376, v); }
  uint32_t image_count_b() const { if (auto* d = direct()) return d->image_count_b; return uint32_t((memory::IsHandle(address + 376) ? uint32_t(memory::OwnerLoad(address + 376, 4)) : guest::LoadU32(base, address + 376))); }
  void set_image_count_b(uint32_t v) const { if (auto* d = direct()) { d->image_count_b = v; return; } if (memory::IsHandle(address + 376)) memory::OwnerStore(address + 376, 4, v); else guest::StoreU32(base, address + 376, v); }
  // the images to load: bit i of 46 (MarkImages)
  uint32_t image_bits_0_address() const { return address + 380; }
  uint32_t image_bits_0_bits() const { if (auto* d = direct()) return d->image_bits_0; return uint32_t((memory::IsHandle(address + 380) ? uint32_t(memory::OwnerLoad(address + 380, 4)) : guest::LoadU32(base, address + 380))); }
  void set_image_bits_0_bits(uint32_t v) const { if (auto* d = direct()) { d->image_bits_0 = v; return; } if (memory::IsHandle(address + 380)) memory::OwnerStore(address + 380, 4, v); else guest::StoreU32(base, address + 380, v); }
  uint32_t image_bits_0() const { if (auto* d = direct()) return d->image_bits_0; return uint32_t((memory::IsHandle(address + 380) ? uint32_t(memory::OwnerLoad(address + 380, 4)) : guest::LoadU32(base, address + 380))); }
  void set_image_bits_0(uint32_t v) const { if (auto* d = direct()) { d->image_bits_0 = v; return; } if (memory::IsHandle(address + 380)) memory::OwnerStore(address + 380, 4, v); else guest::StoreU32(base, address + 380, v); }
  uint32_t image_bits_1_address() const { return address + 384; }
  uint32_t image_bits_1_bits() const { if (auto* d = direct()) return d->image_bits_1; return uint32_t((memory::IsHandle(address + 384) ? uint32_t(memory::OwnerLoad(address + 384, 4)) : guest::LoadU32(base, address + 384))); }
  void set_image_bits_1_bits(uint32_t v) const { if (auto* d = direct()) { d->image_bits_1 = v; return; } if (memory::IsHandle(address + 384)) memory::OwnerStore(address + 384, 4, v); else guest::StoreU32(base, address + 384, v); }
  uint32_t image_bits_1() const { if (auto* d = direct()) return d->image_bits_1; return uint32_t((memory::IsHandle(address + 384) ? uint32_t(memory::OwnerLoad(address + 384, 4)) : guest::LoadU32(base, address + 384))); }
  void set_image_bits_1(uint32_t v) const { if (auto* d = direct()) { d->image_bits_1 = v; return; } if (memory::IsHandle(address + 384)) memory::OwnerStore(address + 384, 4, v); else guest::StoreU32(base, address + 384, v); }
  // the unlock entry shown: a copy of the save's 12-byte entry
  uint32_t unlock_shown_address() const { return address + 388; }
  uint8_t* unlock_shown() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 388 : guest::Pointer(base, address + 388); }
  // the entry's +0 (its shown word)
  uint32_t unlock_word_address() const { return address + 388; }
  uint32_t unlock_word_bits() const { if (auto* d = direct()) return d->unlock_word; return uint32_t((memory::IsHandle(address + 388) ? uint32_t(memory::OwnerLoad(address + 388, 4)) : guest::LoadU32(base, address + 388))); }
  void set_unlock_word_bits(uint32_t v) const { if (auto* d = direct()) { d->unlock_word = v; return; } if (memory::IsHandle(address + 388)) memory::OwnerStore(address + 388, 4, v); else guest::StoreU32(base, address + 388, v); }
  uint32_t unlock_word() const { if (auto* d = direct()) return d->unlock_word; return uint32_t((memory::IsHandle(address + 388) ? uint32_t(memory::OwnerLoad(address + 388, 4)) : guest::LoadU32(base, address + 388))); }
  void set_unlock_word(uint32_t v) const { if (auto* d = direct()) { d->unlock_word = v; return; } if (memory::IsHandle(address + 388)) memory::OwnerStore(address + 388, 4, v); else guest::StoreU32(base, address + 388, v); }
  // the data table
  uint32_t unlock_table_address() const { return address + 392; }
  uint32_t unlock_table_bits() const { if (auto* d = direct()) return d->unlock_table; return uint32_t((memory::IsHandle(address + 392) ? uint32_t(memory::OwnerLoad(address + 392, 4)) : guest::LoadU32(base, address + 392))); }
  void set_unlock_table_bits(uint32_t v) const { if (auto* d = direct()) { d->unlock_table = v; return; } if (memory::IsHandle(address + 392)) memory::OwnerStore(address + 392, 4, v); else guest::StoreU32(base, address + 392, v); }
  uint32_t unlock_table() const { if (auto* d = direct()) return d->unlock_table; return uint32_t((memory::IsHandle(address + 392) ? uint32_t(memory::OwnerLoad(address + 392, 4)) : guest::LoadU32(base, address + 392))); }
  void set_unlock_table(uint32_t v) const { if (auto* d = direct()) { d->unlock_table = v; return; } if (memory::IsHandle(address + 392)) memory::OwnerStore(address + 392, 4, v); else guest::StoreU32(base, address + 392, v); }
  // its 56-byte entry, -1 none; moved along the entry's pages
  uint32_t unlock_entry_address() const { return address + 396; }
  uint32_t unlock_entry_bits() const { if (auto* d = direct()) return d->unlock_entry; return uint32_t((memory::IsHandle(address + 396) ? uint32_t(memory::OwnerLoad(address + 396, 4)) : guest::LoadU32(base, address + 396))); }
  void set_unlock_entry_bits(uint32_t v) const { if (auto* d = direct()) { d->unlock_entry = v; return; } if (memory::IsHandle(address + 396)) memory::OwnerStore(address + 396, 4, v); else guest::StoreU32(base, address + 396, v); }
  uint32_t unlock_entry() const { if (auto* d = direct()) return d->unlock_entry; return uint32_t((memory::IsHandle(address + 396) ? uint32_t(memory::OwnerLoad(address + 396, 4)) : guest::LoadU32(base, address + 396))); }
  void set_unlock_entry(uint32_t v) const { if (auto* d = direct()) { d->unlock_entry = v; return; } if (memory::IsHandle(address + 396)) memory::OwnerStore(address + 396, 4, v); else guest::StoreU32(base, address + 396, v); }
  // movie 0's +28 object's +36 as Start found it (put back by Unload and Leave)
  uint32_t movie_object_36_address() const { return address + 400; }
  uint32_t movie_object_36_bits() const { if (auto* d = direct()) return d->movie_object_36; return uint32_t((memory::IsHandle(address + 400) ? uint32_t(memory::OwnerLoad(address + 400, 4)) : guest::LoadU32(base, address + 400))); }
  void set_movie_object_36_bits(uint32_t v) const { if (auto* d = direct()) { d->movie_object_36 = v; return; } if (memory::IsHandle(address + 400)) memory::OwnerStore(address + 400, 4, v); else guest::StoreU32(base, address + 400, v); }
  uint32_t movie_object_36() const { if (auto* d = direct()) return d->movie_object_36; return uint32_t((memory::IsHandle(address + 400) ? uint32_t(memory::OwnerLoad(address + 400, 4)) : guest::LoadU32(base, address + 400))); }
  void set_movie_object_36(uint32_t v) const { if (auto* d = direct()) { d->movie_object_36 = v; return; } if (memory::IsHandle(address + 400)) memory::OwnerStore(address + 400, 4, v); else guest::StoreU32(base, address + 400, v); }
  // movie 0's +28 object's +32 as Start found it
  uint32_t movie_object_32_address() const { return address + 404; }
  uint32_t movie_object_32_bits() const { if (auto* d = direct()) return d->movie_object_32; return uint32_t((memory::IsHandle(address + 404) ? uint32_t(memory::OwnerLoad(address + 404, 4)) : guest::LoadU32(base, address + 404))); }
  void set_movie_object_32_bits(uint32_t v) const { if (auto* d = direct()) { d->movie_object_32 = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 4, v); else guest::StoreU32(base, address + 404, v); }
  uint32_t movie_object_32() const { if (auto* d = direct()) return d->movie_object_32; return uint32_t((memory::IsHandle(address + 404) ? uint32_t(memory::OwnerLoad(address + 404, 4)) : guest::LoadU32(base, address + 404))); }
  void set_movie_object_32(uint32_t v) const { if (auto* d = direct()) { d->movie_object_32 = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 4, v); else guest::StoreU32(base, address + 404, v); }
  // 1 when EndPage queued movies; Destroy then goes to state 20
  uint32_t movies_queued_address() const { return address + 408; }
  uint32_t movies_queued_bits() const { if (auto* d = direct()) return d->movies_queued; return uint32_t((memory::IsHandle(address + 408) ? uint32_t(memory::OwnerLoad(address + 408, 4)) : guest::LoadU32(base, address + 408))); }
  void set_movies_queued_bits(uint32_t v) const { if (auto* d = direct()) { d->movies_queued = v; return; } if (memory::IsHandle(address + 408)) memory::OwnerStore(address + 408, 4, v); else guest::StoreU32(base, address + 408, v); }
  uint32_t movies_queued() const { if (auto* d = direct()) return d->movies_queued; return uint32_t((memory::IsHandle(address + 408) ? uint32_t(memory::OwnerLoad(address + 408, 4)) : guest::LoadU32(base, address + 408))); }
  void set_movies_queued(uint32_t v) const { if (auto* d = direct()) { d->movies_queued = v; return; } if (memory::IsHandle(address + 408)) memory::OwnerStore(address + 408, 4, v); else guest::StoreU32(base, address + 408, v); }
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
  TodaysEventImageListData* direct() const { return static_cast<TodaysEventImageListData*>(memory::HandleOwn(address, &kTodaysEventImageListLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kTodaysEventImageListLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kTodaysEventImageListLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 46 Type2HubImage pointers (type2-screens.toml)
  uint32_t records_address() const { return address + 0; }
  uint8_t* records() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  uint32_t count_address() const { return address + 184; }
  uint32_t count_bits() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(base, address + 184))); }
  void set_count_bits(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(base, address + 184, v); }
  uint32_t count() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(base, address + 184))); }
  void set_count(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(base, address + 184, v); }
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
