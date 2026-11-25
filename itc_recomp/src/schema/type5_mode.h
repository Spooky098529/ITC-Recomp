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
  static ModeType5 New() {
    return ModeType5(memory::MakeHandle(new ModeType5Data(), sizeof(ModeType5Data), "ModeType5", &kModeType5Layout));
  }
  template <uint32_t Stride = kSize>
  static ModeType5 NewArray(uint32_t count) {
    static_assert(sizeof(ModeType5Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeType5Data*>(::operator new(sizeof(ModeType5Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeType5Data();
    return ModeType5(memory::MakeHandle(p, sizeof(ModeType5Data) * count, "ModeType5", &kModeType5Layout, sizeof(ModeType5Data)));
  }
  ModeType5 at(uint32_t i) const { return ModeType5(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ModeType5Data* direct() const { return static_cast<ModeType5Data*>(memory::HandleOwn(address, &kModeType5Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeType5Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType5Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82006C24 (Type5ModeVtable)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer; the delegates clear it), not the mode's
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kModeType5Layout, 0, 1)); return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(address + 0))); }
  void set_answer_changed_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeType5Layout, 0, 1, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // the delegate at +8, 0 when it wraps to 0 (also the answer's next type +4 by the measure's join)
  uint32_t delegate_ptr_address() const { return address + 4; }
  uint32_t delegate_ptr_bits() const { if (auto* d = direct()) return d->delegate_ptr; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_delegate_ptr_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_ptr = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(delegate_ptr_bits()); }
  void set_delegate_ptr(const View& w) const { set_delegate_ptr_bits(w.address); }
  void set_delegate_ptr(uint32_t v) const { set_delegate_ptr_bits(v); }
  // the delegate (ModeDelegate): vtable 0x82010FA0
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable_bits() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_delegate_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t delegate_vtable() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_delegate_vtable(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // the delegate's object: the mode
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object_bits() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_delegate_object_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t delegate_object() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_delegate_object(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // a slot thunk: Mode::Slot8Thunk (WaitForLoads) after Init, Mode::NextModeThunk 0x82235C08 once loaded, Slot11Thunk to Slot14Thunk
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function_bits() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_delegate_function_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t delegate_function() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_delegate_function(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment_bits() const { if (auto* d = direct()) return d->delegate_adjustment; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_adjustment = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t delegate_adjustment() const { if (auto* d = direct()) return d->delegate_adjustment; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_delegate_adjustment(uint32_t v) const { if (auto* d = direct()) { d->delegate_adjustment = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the +32 interface's vtable 0x82006C18 (Interrupt, ForceExit, Resume)
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32_bits() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_vtable32_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t vtable32() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_vtable32(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers, the block's three machines (+9640, +0, +19280)
  uint32_t handlers_address() const { return address + 36; }
  uint8_t* handlers() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 36 : guest::Pointer(address + 36); }
  uint32_t handler_count_address() const { return address + 236; }
  uint32_t handler_count_bits() const { if (auto* d = direct()) return d->handler_count; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(address + 236))); }
  void set_handler_count_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_count = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(address + 236, v); }
  uint32_t handler_count() const { if (auto* d = direct()) return d->handler_count; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(address + 236))); }
  void set_handler_count(uint32_t v) const { if (auto* d = direct()) { d->handler_count = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(address + 236, v); }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint8_t* ring() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 240 : guest::Pointer(address + 240); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_count_bits() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 840) ? uint32_t(memory::OwnerLoad(address + 840, 4)) : guest::LoadU32(address + 840))); }
  void set_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 840)) memory::OwnerStore(address + 840, 4, v); else guest::StoreU32(address + 840, v); }
  uint32_t ring_count() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 840) ? uint32_t(memory::OwnerLoad(address + 840, 4)) : guest::LoadU32(address + 840))); }
  void set_ring_count(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 840)) memory::OwnerStore(address + 840, 4, v); else guest::StoreU32(address + 840, v); }
  uint32_t ring_start_address() const { return address + 844; }
  uint32_t ring_start_bits() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 844) ? uint32_t(memory::OwnerLoad(address + 844, 4)) : guest::LoadU32(address + 844))); }
  void set_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 844)) memory::OwnerStore(address + 844, 4, v); else guest::StoreU32(address + 844, v); }
  uint32_t ring_start() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 844) ? uint32_t(memory::OwnerLoad(address + 844, 4)) : guest::LoadU32(address + 844))); }
  void set_ring_start(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 844)) memory::OwnerStore(address + 844, 4, v); else guest::StoreU32(address + 844, v); }
  // measure fault: block (+848) through the +32 interface (Interrupt, ForceExit)
  uint32_t iface_block_address() const { return address + 816; }
  uint32_t iface_block_bits() const { if (auto* d = direct()) return d->iface_block; return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(address + 816))); }
  void set_iface_block_bits(uint32_t v) const { if (auto* d = direct()) { d->iface_block = v; return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(address + 816, v); }
  uint32_t iface_block() const { if (auto* d = direct()) return d->iface_block; return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(address + 816))); }
  void set_iface_block(uint32_t v) const { if (auto* d = direct()) { d->iface_block = v; return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(address + 816, v); }
  // VirtualAlloc(0, 28940, 0x1000, 4) by Init, freed by ModeType5::Release
  uint32_t block_address() const { return address + 848; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 848) ? uint32_t(memory::OwnerLoad(address + 848, 4)) : guest::LoadU32(address + 848))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 848)) memory::OwnerStore(address + 848, 4, v); else guest::StoreU32(address + 848, v); }
  template <typename T = Type5ModeBlock>
  T block() const { return T(block_bits()); }
  void set_block(const View& w) const { set_block_bits(w.address); }
  void set_block(uint32_t v) const { set_block_bits(v); }
};

// allocated; measure: new<28940>@ModeType5::Init, ModeType5.+816, ModeType5::NextMode.r4.+848, ModeType5::WaitForLoads.r4.+848, ModeType5::Leaving.r4.+848, FrontEndMode::NextMode.r4.+848
#pragma pack(push, 1)
struct Type5ModeBlockData {
  uint8_t pad_0[9604] = {};
  uint32_t car_settings_state = 0;  // +9604
  uint32_t car_settings_loaded = 0;  // +9608
  uint32_t car_settings_reenter = 0;  // +9612
  uint32_t car_settings_delete_movie = 0;  // +9616
  uint8_t pad_9620[9624] = {};
  uint32_t location_state = 0;  // +19244
  uint32_t location_loaded = 0;  // +19248
  uint32_t location_reenter = 0;  // +19252
  uint32_t location_delete_movie = 0;  // +19256
  uint8_t pad_19260[9624] = {};
  uint32_t dialog_state = 0;  // +28884
  uint32_t dialog_loaded = 0;  // +28888
  uint32_t dialog_reenter = 0;  // +28892
  uint32_t dialog_delete_movie = 0;  // +28896
  uint8_t pad_28900[20] = {};
  uint32_t shown = 0;  // +28920
  uint32_t first_shown = 0;  // +28924
  uint64_t answer = 0;  // +28928
  uint8_t pad_28936[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(Type5ModeBlockData) == 28940);
inline constexpr memory::LayoutField kType5ModeBlockFields[] = {{9604, 4}, {9608, 4}, {9612, 4}, {9616, 4}, {19244, 4}, {19248, 4}, {19252, 4}, {19256, 4}, {28884, 4}, {28888, 4}, {28892, 4}, {28896, 4}, {28920, 4}, {28924, 4}, {28928, 8}};
inline constexpr memory::Layout kType5ModeBlockLayout = {kType5ModeBlockFields, 15};

struct Type5ModeBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 28940;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  Type5ModeBlockData* data() const { return static_cast<Type5ModeBlockData*>(memory::HandleObject(address)); }
  static Type5ModeBlock New() {
    return Type5ModeBlock(memory::MakeHandle(new Type5ModeBlockData(), sizeof(Type5ModeBlockData), "Type5ModeBlock", &kType5ModeBlockLayout));
  }
  template <uint32_t Stride = kSize>
  static Type5ModeBlock NewArray(uint32_t count) {
    static_assert(sizeof(Type5ModeBlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Type5ModeBlockData*>(::operator new(sizeof(Type5ModeBlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Type5ModeBlockData();
    return Type5ModeBlock(memory::MakeHandle(p, sizeof(Type5ModeBlockData) * count, "Type5ModeBlock", &kType5ModeBlockLayout, sizeof(Type5ModeBlockData)));
  }
  Type5ModeBlock at(uint32_t i) const { return Type5ModeBlock(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  Type5ModeBlockData* direct() const { return static_cast<Type5ModeBlockData*>(memory::HandleOwn(address, &kType5ModeBlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kType5ModeBlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType5ModeBlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the car settings' machine (car-settings.md, CarSettings::InitType5); id 3
  uint32_t car_settings_address() const { return address + 0; }
  uint8_t* car_settings() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  // the car settings' machine state
  uint32_t car_settings_state_address() const { return address + 9604; }
  uint32_t car_settings_state_bits() const { if (auto* d = direct()) return d->car_settings_state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(address + 9604))); }
  void set_car_settings_state_bits(uint32_t v) const { if (auto* d = direct()) { d->car_settings_state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(address + 9604, v); }
  uint32_t car_settings_state() const { if (auto* d = direct()) return d->car_settings_state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(address + 9604))); }
  void set_car_settings_state(uint32_t v) const { if (auto* d = direct()) { d->car_settings_state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(address + 9604, v); }
  // its movies loaded (AllLoaded)
  uint32_t car_settings_loaded_address() const { return address + 9608; }
  uint32_t car_settings_loaded_bits() const { if (auto* d = direct()) return d->car_settings_loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(address + 9608))); }
  void set_car_settings_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->car_settings_loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(address + 9608, v); }
  uint32_t car_settings_loaded() const { if (auto* d = direct()) return d->car_settings_loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(address + 9608))); }
  void set_car_settings_loaded(uint32_t v) const { if (auto* d = direct()) { d->car_settings_loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(address + 9608, v); }
  // open on its message: 1 from Init
  uint32_t car_settings_reenter_address() const { return address + 9612; }
  uint32_t car_settings_reenter_bits() const { if (auto* d = direct()) return d->car_settings_reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(address + 9612))); }
  void set_car_settings_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->car_settings_reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(address + 9612, v); }
  uint32_t car_settings_reenter() const { if (auto* d = direct()) return d->car_settings_reenter; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(address + 9612))); }
  void set_car_settings_reenter(uint32_t v) const { if (auto* d = direct()) { d->car_settings_reenter = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(address + 9612, v); }
  // 0 from Init
  uint32_t car_settings_delete_movie_address() const { return address + 9616; }
  uint32_t car_settings_delete_movie_bits() const { if (auto* d = direct()) return d->car_settings_delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(address + 9616))); }
  void set_car_settings_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->car_settings_delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(address + 9616, v); }
  uint32_t car_settings_delete_movie() const { if (auto* d = direct()) return d->car_settings_delete_movie; return uint32_t((memory::IsHandle(address + 9616) ? uint32_t(memory::OwnerLoad(address + 9616, 4)) : guest::LoadU32(address + 9616))); }
  void set_car_settings_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->car_settings_delete_movie = v; return; } if (memory::IsHandle(address + 9616)) memory::OwnerStore(address + 9616, 4, v); else guest::StoreU32(address + 9616, v); }
  // the parking area's machine (location-screen.md's LocationScreenMachine); id 21
  uint32_t location_screen_address() const { return address + 9640; }
  uint8_t* location_screen() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 9640 : guest::Pointer(address + 9640); }
  // the parking area's machine state: 8 a leave asked, 10 leave (message 4), 11 a race set up, 13 today's result
  uint32_t location_state_address() const { return address + 19244; }
  uint32_t location_state_bits() const { if (auto* d = direct()) return d->location_state; return uint32_t((memory::IsHandle(address + 19244) ? uint32_t(memory::OwnerLoad(address + 19244, 4)) : guest::LoadU32(address + 19244))); }
  void set_location_state_bits(uint32_t v) const { if (auto* d = direct()) { d->location_state = v; return; } if (memory::IsHandle(address + 19244)) memory::OwnerStore(address + 19244, 4, v); else guest::StoreU32(address + 19244, v); }
  uint32_t location_state() const { if (auto* d = direct()) return d->location_state; return uint32_t((memory::IsHandle(address + 19244) ? uint32_t(memory::OwnerLoad(address + 19244, 4)) : guest::LoadU32(address + 19244))); }
  void set_location_state(uint32_t v) const { if (auto* d = direct()) { d->location_state = v; return; } if (memory::IsHandle(address + 19244)) memory::OwnerStore(address + 19244, 4, v); else guest::StoreU32(address + 19244, v); }
  // its movies loaded (AllLoaded)
  uint32_t location_loaded_address() const { return address + 19248; }
  uint32_t location_loaded_bits() const { if (auto* d = direct()) return d->location_loaded; return uint32_t((memory::IsHandle(address + 19248) ? uint32_t(memory::OwnerLoad(address + 19248, 4)) : guest::LoadU32(address + 19248))); }
  void set_location_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->location_loaded = v; return; } if (memory::IsHandle(address + 19248)) memory::OwnerStore(address + 19248, 4, v); else guest::StoreU32(address + 19248, v); }
  uint32_t location_loaded() const { if (auto* d = direct()) return d->location_loaded; return uint32_t((memory::IsHandle(address + 19248) ? uint32_t(memory::OwnerLoad(address + 19248, 4)) : guest::LoadU32(address + 19248))); }
  void set_location_loaded(uint32_t v) const { if (auto* d = direct()) { d->location_loaded = v; return; } if (memory::IsHandle(address + 19248)) memory::OwnerStore(address + 19248, 4, v); else guest::StoreU32(address + 19248, v); }
  // open on its message: 1 from Init
  uint32_t location_reenter_address() const { return address + 19252; }
  uint32_t location_reenter_bits() const { if (auto* d = direct()) return d->location_reenter; return uint32_t((memory::IsHandle(address + 19252) ? uint32_t(memory::OwnerLoad(address + 19252, 4)) : guest::LoadU32(address + 19252))); }
  void set_location_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->location_reenter = v; return; } if (memory::IsHandle(address + 19252)) memory::OwnerStore(address + 19252, 4, v); else guest::StoreU32(address + 19252, v); }
  uint32_t location_reenter() const { if (auto* d = direct()) return d->location_reenter; return uint32_t((memory::IsHandle(address + 19252) ? uint32_t(memory::OwnerLoad(address + 19252, 4)) : guest::LoadU32(address + 19252))); }
  void set_location_reenter(uint32_t v) const { if (auto* d = direct()) { d->location_reenter = v; return; } if (memory::IsHandle(address + 19252)) memory::OwnerStore(address + 19252, 4, v); else guest::StoreU32(address + 19252, v); }
  // 0 from Init
  uint32_t location_delete_movie_address() const { return address + 19256; }
  uint32_t location_delete_movie_bits() const { if (auto* d = direct()) return d->location_delete_movie; return uint32_t((memory::IsHandle(address + 19256) ? uint32_t(memory::OwnerLoad(address + 19256, 4)) : guest::LoadU32(address + 19256))); }
  void set_location_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->location_delete_movie = v; return; } if (memory::IsHandle(address + 19256)) memory::OwnerStore(address + 19256, 4, v); else guest::StoreU32(address + 19256, v); }
  uint32_t location_delete_movie() const { if (auto* d = direct()) return d->location_delete_movie; return uint32_t((memory::IsHandle(address + 19256) ? uint32_t(memory::OwnerLoad(address + 19256, 4)) : guest::LoadU32(address + 19256))); }
  void set_location_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->location_delete_movie = v; return; } if (memory::IsHandle(address + 19256)) memory::OwnerStore(address + 19256, 4, v); else guest::StoreU32(address + 19256, v); }
  // the mode's dialog's machine (Type5Dialog); id 16
  uint32_t dialog_address() const { return address + 19280; }
  uint8_t* dialog() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 19280 : guest::Pointer(address + 19280); }
  // the dialog's machine state (9 idle: Interrupt answers 1)
  uint32_t dialog_state_address() const { return address + 28884; }
  uint32_t dialog_state_bits() const { if (auto* d = direct()) return d->dialog_state; return uint32_t((memory::IsHandle(address + 28884) ? uint32_t(memory::OwnerLoad(address + 28884, 4)) : guest::LoadU32(address + 28884))); }
  void set_dialog_state_bits(uint32_t v) const { if (auto* d = direct()) { d->dialog_state = v; return; } if (memory::IsHandle(address + 28884)) memory::OwnerStore(address + 28884, 4, v); else guest::StoreU32(address + 28884, v); }
  uint32_t dialog_state() const { if (auto* d = direct()) return d->dialog_state; return uint32_t((memory::IsHandle(address + 28884) ? uint32_t(memory::OwnerLoad(address + 28884, 4)) : guest::LoadU32(address + 28884))); }
  void set_dialog_state(uint32_t v) const { if (auto* d = direct()) { d->dialog_state = v; return; } if (memory::IsHandle(address + 28884)) memory::OwnerStore(address + 28884, 4, v); else guest::StoreU32(address + 28884, v); }
  // its movies loaded (AllLoaded)
  uint32_t dialog_loaded_address() const { return address + 28888; }
  uint32_t dialog_loaded_bits() const { if (auto* d = direct()) return d->dialog_loaded; return uint32_t((memory::IsHandle(address + 28888) ? uint32_t(memory::OwnerLoad(address + 28888, 4)) : guest::LoadU32(address + 28888))); }
  void set_dialog_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->dialog_loaded = v; return; } if (memory::IsHandle(address + 28888)) memory::OwnerStore(address + 28888, 4, v); else guest::StoreU32(address + 28888, v); }
  uint32_t dialog_loaded() const { if (auto* d = direct()) return d->dialog_loaded; return uint32_t((memory::IsHandle(address + 28888) ? uint32_t(memory::OwnerLoad(address + 28888, 4)) : guest::LoadU32(address + 28888))); }
  void set_dialog_loaded(uint32_t v) const { if (auto* d = direct()) { d->dialog_loaded = v; return; } if (memory::IsHandle(address + 28888)) memory::OwnerStore(address + 28888, 4, v); else guest::StoreU32(address + 28888, v); }
  // open on its message: 1 from Init
  uint32_t dialog_reenter_address() const { return address + 28892; }
  uint32_t dialog_reenter_bits() const { if (auto* d = direct()) return d->dialog_reenter; return uint32_t((memory::IsHandle(address + 28892) ? uint32_t(memory::OwnerLoad(address + 28892, 4)) : guest::LoadU32(address + 28892))); }
  void set_dialog_reenter_bits(uint32_t v) const { if (auto* d = direct()) { d->dialog_reenter = v; return; } if (memory::IsHandle(address + 28892)) memory::OwnerStore(address + 28892, 4, v); else guest::StoreU32(address + 28892, v); }
  uint32_t dialog_reenter() const { if (auto* d = direct()) return d->dialog_reenter; return uint32_t((memory::IsHandle(address + 28892) ? uint32_t(memory::OwnerLoad(address + 28892, 4)) : guest::LoadU32(address + 28892))); }
  void set_dialog_reenter(uint32_t v) const { if (auto* d = direct()) { d->dialog_reenter = v; return; } if (memory::IsHandle(address + 28892)) memory::OwnerStore(address + 28892, 4, v); else guest::StoreU32(address + 28892, v); }
  // 0 from Init
  uint32_t dialog_delete_movie_address() const { return address + 28896; }
  uint32_t dialog_delete_movie_bits() const { if (auto* d = direct()) return d->dialog_delete_movie; return uint32_t((memory::IsHandle(address + 28896) ? uint32_t(memory::OwnerLoad(address + 28896, 4)) : guest::LoadU32(address + 28896))); }
  void set_dialog_delete_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->dialog_delete_movie = v; return; } if (memory::IsHandle(address + 28896)) memory::OwnerStore(address + 28896, 4, v); else guest::StoreU32(address + 28896, v); }
  uint32_t dialog_delete_movie() const { if (auto* d = direct()) return d->dialog_delete_movie; return uint32_t((memory::IsHandle(address + 28896) ? uint32_t(memory::OwnerLoad(address + 28896, 4)) : guest::LoadU32(address + 28896))); }
  void set_dialog_delete_movie(uint32_t v) const { if (auto* d = direct()) { d->dialog_delete_movie = v; return; } if (memory::IsHandle(address + 28896)) memory::OwnerStore(address + 28896, 4, v); else guest::StoreU32(address + 28896, v); }
  // the shown machine's id (3, 16 or 21; FindShown; 21 at Init)
  uint32_t shown_address() const { return address + 28920; }
  uint32_t shown_bits() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 28920) ? uint32_t(memory::OwnerLoad(address + 28920, 4)) : guest::LoadU32(address + 28920))); }
  void set_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 28920)) memory::OwnerStore(address + 28920, 4, v); else guest::StoreU32(address + 28920, v); }
  uint32_t shown() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 28920) ? uint32_t(memory::OwnerLoad(address + 28920, 4)) : guest::LoadU32(address + 28920))); }
  void set_shown(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 28920)) memory::OwnerStore(address + 28920, 4, v); else guest::StoreU32(address + 28920, v); }
  // 21 at Init (readers not looked for)
  uint32_t first_shown_address() const { return address + 28924; }
  uint32_t first_shown_bits() const { if (auto* d = direct()) return d->first_shown; return uint32_t((memory::IsHandle(address + 28924) ? uint32_t(memory::OwnerLoad(address + 28924, 4)) : guest::LoadU32(address + 28924))); }
  void set_first_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->first_shown = v; return; } if (memory::IsHandle(address + 28924)) memory::OwnerStore(address + 28924, 4, v); else guest::StoreU32(address + 28924, v); }
  uint32_t first_shown() const { if (auto* d = direct()) return d->first_shown; return uint32_t((memory::IsHandle(address + 28924) ? uint32_t(memory::OwnerLoad(address + 28924, 4)) : guest::LoadU32(address + 28924))); }
  void set_first_shown(uint32_t v) const { if (auto* d = direct()) { d->first_shown = v; return; } if (memory::IsHandle(address + 28924)) memory::OwnerStore(address + 28924, 4, v); else guest::StoreU32(address + 28924, v); }
  // the next mode's 8-byte answer (ModeAnswer), written whole by NextMode and ForceExit
  uint32_t answer_address() const { return address + 28928; }
  uint64_t answer_bits() const { if (auto* d = direct()) return d->answer; return uint64_t((memory::IsHandle(address + 28928) ? uint64_t(memory::OwnerLoad(address + 28928, 8)) : guest::LoadU64(address + 28928))); }
  void set_answer_bits(uint64_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 28928)) memory::OwnerStore(address + 28928, 8, v); else guest::StoreU64(address + 28928, v); }
  uint64_t answer() const { if (auto* d = direct()) return d->answer; return uint64_t((memory::IsHandle(address + 28928) ? uint64_t(memory::OwnerLoad(address + 28928, 8)) : guest::LoadU64(address + 28928))); }
  void set_answer(uint64_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 28928)) memory::OwnerStore(address + 28928, 8, v); else guest::StoreU64(address + 28928, v); }
  // the answer's first word: its changed byte and three bytes
  uint32_t answer_word0_address() const { return address + 28928; }
  uint32_t answer_word0_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kType5ModeBlockLayout, 28928, 4)); return uint32_t((memory::IsHandle(address + 28928) ? uint32_t(memory::OwnerLoad(address + 28928, 4)) : guest::LoadU32(address + 28928))); }
  void set_answer_word0_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType5ModeBlockLayout, 28928, 4, v); return; } if (memory::IsHandle(address + 28928)) memory::OwnerStore(address + 28928, 4, v); else guest::StoreU32(address + 28928, v); }
  uint32_t answer_word0() const { return uint32_t(answer_word0_bits()); }
  void set_answer_word0(uint32_t v) const { set_answer_word0_bits(uint32_t(v)); }
  // the answer's next mode type (1 the race, 9 today's result, 4 the title, 12)
  uint32_t answer_next_type_address() const { return address + 28932; }
  uint32_t answer_next_type_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kType5ModeBlockLayout, 28932, 4)); return uint32_t((memory::IsHandle(address + 28932) ? uint32_t(memory::OwnerLoad(address + 28932, 4)) : guest::LoadU32(address + 28932))); }
  void set_answer_next_type_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType5ModeBlockLayout, 28932, 4, v); return; } if (memory::IsHandle(address + 28932)) memory::OwnerStore(address + 28932, 4, v); else guest::StoreU32(address + 28932, v); }
  uint32_t answer_next_type() const { return uint32_t(answer_next_type_bits()); }
  void set_answer_next_type(uint32_t v) const { set_answer_next_type_bits(uint32_t(v)); }
};

// embedded; measure: Type5Dialog
struct Type5Dialog : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  // measure fault: the steps' r3, the page object's step_started (+24), not the machine's
  uint32_t fault_step_started() const { return guest::LoadU32(address + 0); }
  void set_fault_step_started(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t fault_step_started_address() const { return address + 0; }
  // measure fault: the pages' r3 is the dispatcher (ModeDispatcher's ring_count), not the machine
  uint32_t fault_ring_count() const { return guest::LoadU32(address + 804); }
  void set_fault_ring_count(uint32_t v) const { guest::StoreU32(address + 804, v); }
  uint32_t fault_ring_count_address() const { return address + 804; }
  // measure fault: the dispatcher's ring_start, as above
  uint32_t fault_ring_start() const { return guest::LoadU32(address + 808); }
  void set_fault_ring_start(uint32_t v) const { guest::StoreU32(address + 808, v); }
  uint32_t fault_ring_start_address() const { return address + 808; }
  uint32_t transition_count() const { return guest::LoadU32(address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 enter, 1 load the movies, 2 wait for them, 4 a dialog opening, 9 idle
  uint32_t state() const { return guest::LoadU32(address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movies 16 to 19 loaded (WaitForMovies)
  uint32_t loaded() const { return guest::LoadU32(address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // open on its message: 1 from ModeType5::Init (OnEnter goes to 1, else 9)
  uint32_t reenter() const { return guest::LoadU32(address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // 0 from ModeType5::Init
  uint32_t delete_movie() const { return guest::LoadU32(address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(address + 9616, v); }
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
  static Type5DialogData New() {
    return Type5DialogData(memory::MakeHandle(new Type5DialogDataData(), sizeof(Type5DialogDataData), "Type5DialogData", &kType5DialogDataLayout));
  }
  template <uint32_t Stride = kSize>
  static Type5DialogData NewArray(uint32_t count) {
    static_assert(sizeof(Type5DialogDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Type5DialogDataData*>(::operator new(sizeof(Type5DialogDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Type5DialogDataData();
    return Type5DialogData(memory::MakeHandle(p, sizeof(Type5DialogDataData) * count, "Type5DialogData", &kType5DialogDataLayout, sizeof(Type5DialogDataData)));
  }
  Type5DialogData at(uint32_t i) const { return Type5DialogData(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  Type5DialogDataData* direct() const { return static_cast<Type5DialogDataData*>(memory::HandleOwn(address, &kType5DialogDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kType5DialogDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType5DialogDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 1 once movies 16 to 19 are loaded
  uint32_t movies_loaded_address() const { return address + 0; }
  uint32_t movies_loaded_bits() const { if (auto* d = direct()) return d->movies_loaded; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_movies_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->movies_loaded = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t movies_loaded() const { if (auto* d = direct()) return d->movies_loaded; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_movies_loaded(uint32_t v) const { if (auto* d = direct()) { d->movies_loaded = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the machine's state queued by the close steps (9); 21 none
  uint32_t queued_state_address() const { return address + 4; }
  uint32_t queued_state_bits() const { if (auto* d = direct()) return d->queued_state; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_queued_state_bits(uint32_t v) const { if (auto* d = direct()) { d->queued_state = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t queued_state() const { if (auto* d = direct()) return d->queued_state; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_queued_state(uint32_t v) const { if (auto* d = direct()) { d->queued_state = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the movie shown, 16 to 19
  uint32_t movie_shown_address() const { return address + 8; }
  uint32_t movie_shown_bits() const { if (auto* d = direct()) return d->movie_shown; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_movie_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->movie_shown = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t movie_shown() const { if (auto* d = direct()) return d->movie_shown; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_movie_shown(uint32_t v) const { if (auto* d = direct()) { d->movie_shown = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // set by message 18 (OnInterrupt); the pages close with message 20
  uint32_t interrupted_address() const { return address + 12; }
  uint32_t interrupted_bits() const { if (auto* d = direct()) return d->interrupted; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_interrupted_bits(uint32_t v) const { if (auto* d = direct()) { d->interrupted = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t interrupted() const { if (auto* d = direct()) return d->interrupted; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_interrupted(uint32_t v) const { if (auto* d = direct()) { d->interrupted = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
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
  static Type5SlotObject New() {
    return Type5SlotObject(memory::MakeHandle(new Type5SlotObjectData(), sizeof(Type5SlotObjectData), "Type5SlotObject", &kType5SlotObjectLayout));
  }
  template <uint32_t Stride = kSize>
  static Type5SlotObject NewArray(uint32_t count) {
    static_assert(sizeof(Type5SlotObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Type5SlotObjectData*>(::operator new(sizeof(Type5SlotObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Type5SlotObjectData();
    return Type5SlotObject(memory::MakeHandle(p, sizeof(Type5SlotObjectData) * count, "Type5SlotObject", &kType5SlotObjectLayout, sizeof(Type5SlotObjectData)));
  }
  Type5SlotObject at(uint32_t i) const { return Type5SlotObject(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  Type5SlotObjectData* direct() const { return static_cast<Type5SlotObjectData*>(memory::HandleOwn(address, &kType5SlotObjectLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kType5SlotObjectLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType5SlotObjectLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82006C24
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = Type5ModeVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
};

// image; measure: Mode::Slot14Thunk.r4.+0
struct Type5ModeVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // slot 0: ModeType5::Init
  uint32_t init() const { return guest::LoadU32(address + 0); }
  void set_init(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t init_address() const { return address + 0; }
  // slot 1: ModeType5::Release
  uint32_t release() const { return guest::LoadU32(address + 4); }
  void set_release(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t release_address() const { return address + 4; }
  // slots 2 to 5: Mode::NoNextMode
  uint32_t slot2() const { return guest::LoadU32(address + 8); }
  void set_slot2(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t slot2_address() const { return address + 8; }
  uint32_t slot3() const { return guest::LoadU32(address + 12); }
  void set_slot3(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t slot3_address() const { return address + 12; }
  uint32_t slot4() const { return guest::LoadU32(address + 16); }
  void set_slot4(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t slot4_address() const { return address + 16; }
  uint32_t slot5() const { return guest::LoadU32(address + 20); }
  void set_slot5(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t slot5_address() const { return address + 20; }
  // slot 6: ModeType5::NextMode
  uint32_t next_mode() const { return guest::LoadU32(address + 24); }
  void set_next_mode(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t next_mode_address() const { return address + 24; }
  // slot 7: Mode::DeletingDestructor
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 28); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t deleting_destructor_address() const { return address + 28; }
  // slot 8: ModeType5::WaitForLoads (Mode::Slot8Thunk)
  uint32_t wait_for_loads() const { return guest::LoadU32(address + 32); }
  void set_wait_for_loads(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t wait_for_loads_address() const { return address + 32; }
  // slots 9 and 10: Mode::NoNextMode
  uint32_t slot9() const { return guest::LoadU32(address + 36); }
  void set_slot9(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t slot9_address() const { return address + 36; }
  uint32_t slot10() const { return guest::LoadU32(address + 40); }
  void set_slot10(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t slot10_address() const { return address + 40; }
  // slot 11: ModeType5::WaitForShown (Mode::Slot11Thunk)
  uint32_t wait_for_shown() const { return guest::LoadU32(address + 44); }
  void set_wait_for_shown(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t wait_for_shown_address() const { return address + 44; }
  // slot 12: ModeType5::LeaveScreen (Mode::Slot12Thunk)
  uint32_t leave_screen() const { return guest::LoadU32(address + 48); }
  void set_leave_screen(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t leave_screen_address() const { return address + 48; }
  // slot 13: FrontEndMode::NextMode sub_8222FCA0 (Mode::Slot13Thunk)
  uint32_t next_mode_answer() const { return guest::LoadU32(address + 52); }
  void set_next_mode_answer(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t next_mode_answer_address() const { return address + 52; }
  // slot 14: ModeType5::Leaving (Mode::Slot14Thunk)
  uint32_t leaving() const { return guest::LoadU32(address + 56); }
  void set_leaving(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t leaving_address() const { return address + 56; }
};

// The image words docs/functions/type5-mode.md's code uses.
struct Type5ModeGlobals {
  // the dialog's page object (pages 0 to 3, steps 0 to 9)
  template <typename T = PageObject>
  static T pages() { return T(guest::LoadU32(0x82814E3Cu)); }
  static void set_pages(const View& v) { guest::StoreU32(0x82814E3Cu, v.address); }
  static void set_pages(uint32_t v) { guest::StoreU32(0x82814E3Cu, v); }
  static constexpr uint32_t pages_address() { return 0x82814E3Cu; }
  // the dialog's menu, items 0 (yes) to 1 (no)
  template <typename T = FrontEndMenu>
  static T menu() { return T(guest::LoadU32(0x82814E40u)); }
  static void set_menu(const View& v) { guest::StoreU32(0x82814E40u, v.address); }
  static void set_menu(uint32_t v) { guest::StoreU32(0x82814E40u, v); }
  static constexpr uint32_t menu_address() { return 0x82814E40u; }
  template <typename T = Type5DialogData>
  static T data() { return T(guest::LoadU32(0x82814E44u)); }
  static void set_data(const View& v) { guest::StoreU32(0x82814E44u, v.address); }
  static void set_data(uint32_t v) { guest::StoreU32(0x82814E44u, v); }
  static constexpr uint32_t data_address() { return 0x82814E44u; }
  // the parking area's screen (location-screen.toml's): Resume sets its restart (+900)
  template <typename T = LocationScreen>
  static T location_screen() { return T(guest::LoadU32(0x82814E50u)); }
  static void set_location_screen(const View& v) { guest::StoreU32(0x82814E50u, v.address); }
  static void set_location_screen(uint32_t v) { guest::StoreU32(0x82814E50u, v); }
  static constexpr uint32_t location_screen_address() { return 0x82814E50u; }
  // how the title is entered: 1 from ForceExit (reasons 0 and 1)
  static uint32_t leaving_flag() { return guest::LoadU32(0x826364ECu); }
  static void set_leaving_flag(uint32_t v) { guest::StoreU32(0x826364ECu, v); }
  static constexpr uint32_t leaving_flag_address() { return 0x826364ECu; }
  // the string table Strings::Load gets
  static uint32_t string_table() { return guest::LoadU32(0x826C5C34u); }
  static void set_string_table(uint32_t v) { guest::StoreU32(0x826C5C34u, v); }
  static constexpr uint32_t string_table_address() { return 0x826C5C34u; }
  // the passes word ScreenFades::FadeIn is given
  static uint32_t fade_in_passes() { return guest::LoadU32(0x82001BECu); }
  static void set_fade_in_passes(uint32_t v) { guest::StoreU32(0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
};

}  // namespace schema
