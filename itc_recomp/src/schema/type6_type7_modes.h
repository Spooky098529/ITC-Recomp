// The views of docs/functions/type6-type7-modes.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type6-type7-modes.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Type6Mode, Type6Mode::Leaving.r4, Type6Mode::NextMode.r4, Type6Mode::TearDown.r4, Type6Mode::WaitForLoads.r4
#pragma pack(push, 1)
struct Type6ModeData {
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
static_assert(sizeof(Type6ModeData) == 852);
inline constexpr memory::LayoutField kType6ModeFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {808, 4}, {812, 4}, {816, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kType6ModeLayout = {kType6ModeFields, 14};

struct Type6Mode : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x82010A48;
  Type6ModeData* data() const { return static_cast<Type6ModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static Type6Mode New(uint8_t* base) {
    return Type6Mode(base, memory::MakeHandle(new Type6ModeData(), sizeof(Type6ModeData), "Type6Mode", &kType6ModeLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kType6ModeLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType6ModeLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82010A48
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), the delegate functions' r3
  uint8_t answer_changed_bits() const { return on_host() ? uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kType6ModeLayout, 0, 1)) : guest::LoadU8(base, address + 0); }
  void set_answer_changed_bits(uint8_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType6ModeLayout, 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the answer's first word (ModeAnswer.word0), TearDown's copy of the block's answer
  uint32_t answer_word0_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kType6ModeLayout, 0, 4)) : guest::LoadU32(base, address + 0); }
  void set_answer_word0_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType6ModeLayout, 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t answer_word0() const { return uint32_t(answer_word0_bits()); }
  void set_answer_word0(uint32_t v) const { set_answer_word0_bits(uint32_t(v)); }
  // the delegate at +8 (also the answer's next type +4 by the measure's join)
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
  // WaitForLoads, then Mode::NextModeThunk (NextMode), Leaving, TearDown
  uint32_t delegate_function() const { return on_host() ? uint32_t(data()->delegate_function) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_delegate_function(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_bits() const { return on_host() ? data()->delegate_function : guest::LoadU32(base, address + 16); }
  void set_delegate_function_bits(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  // 0
  uint32_t delegate_adjustment() const { return on_host() ? uint32_t(data()->delegate_adjustment) : uint32_t(guest::LoadU32(base, address + 20)); }
  void set_delegate_adjustment(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_bits() const { return on_host() ? data()->delegate_adjustment : guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  // 0x82010A3C: Interrupt, ForceExit, Resume
  uint32_t vtable32() const { return on_host() ? uint32_t(data()->vtable32) : uint32_t(guest::LoadU32(base, address + 32)); }
  void set_vtable32(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_bits() const { return on_host() ? data()->vtable32 : guest::LoadU32(base, address + 32); }
  void set_vtable32_bits(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (nine machines, in Init's order)
  uint8_t* handlers() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 36 : guest::Pointer(base, address + 36); }
  uint32_t handler_count() const { return on_host() ? uint32_t(data()->handler_count) : uint32_t(guest::LoadU32(base, address + 236)); }
  void set_handler_count(uint32_t v) const { if (on_host()) data()->handler_count = v; else guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_bits() const { return on_host() ? data()->handler_count : guest::LoadU32(base, address + 236); }
  void set_handler_count_bits(uint32_t v) const { if (on_host()) data()->handler_count = v; else guest::StoreU32(base, address + 236, v); }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint8_t* ring() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 240 : guest::Pointer(base, address + 240); }
  // measure fault: Interrupt's r3 is the mode + 32, so this is the mode's +840 (ring_count)
  uint32_t iface_ring_count() const { return on_host() ? uint32_t(data()->iface_ring_count) : uint32_t(guest::LoadU32(base, address + 808)); }
  void set_iface_ring_count(uint32_t v) const { if (on_host()) data()->iface_ring_count = v; else guest::StoreU32(base, address + 808, v); }
  uint32_t iface_ring_count_bits() const { return on_host() ? data()->iface_ring_count : guest::LoadU32(base, address + 808); }
  void set_iface_ring_count_bits(uint32_t v) const { if (on_host()) data()->iface_ring_count = v; else guest::StoreU32(base, address + 808, v); }
  // measure fault: the mode's +844 (ring_start) through the +32 interface
  uint32_t iface_ring_start() const { return on_host() ? uint32_t(data()->iface_ring_start) : uint32_t(guest::LoadU32(base, address + 812)); }
  void set_iface_ring_start(uint32_t v) const { if (on_host()) data()->iface_ring_start = v; else guest::StoreU32(base, address + 812, v); }
  uint32_t iface_ring_start_bits() const { return on_host() ? data()->iface_ring_start : guest::LoadU32(base, address + 812); }
  void set_iface_ring_start_bits(uint32_t v) const { if (on_host()) data()->iface_ring_start = v; else guest::StoreU32(base, address + 812, v); }
  // measure fault: the mode's +848 (block) through the +32 interface
  template <typename T = Type6Screens>
  T iface_block() const { return T(base, on_host() ? data()->iface_block : guest::LoadU32(base, address + 816)); }
  void set_iface_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  void set_iface_block(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t iface_block_bits() const { return on_host() ? data()->iface_block : guest::LoadU32(base, address + 816); }
  void set_iface_block_bits(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t ring_count() const { return on_host() ? uint32_t(data()->ring_count) : uint32_t(guest::LoadU32(base, address + 840)); }
  void set_ring_count(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_bits() const { return on_host() ? data()->ring_count : guest::LoadU32(base, address + 840); }
  void set_ring_count_bits(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_start() const { return on_host() ? uint32_t(data()->ring_start) : uint32_t(guest::LoadU32(base, address + 844)); }
  void set_ring_start(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_bits() const { return on_host() ? data()->ring_start : guest::LoadU32(base, address + 844); }
  void set_ring_start_bits(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 844, v); }
  // the screen block (91,272 bytes, VirtualAlloc'ed by Init, freed by StandMode::Release)
  template <typename T = Type6Screens>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 848)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 848); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
};

// allocated; measure: Type7Mode, Type7Mode::Leaving.r4, Type7Mode::NextMode.r4, Type7Mode::TearDown.r4, Type7Mode::WaitForLoads.r4
#pragma pack(push, 1)
struct Type7ModeData {
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
static_assert(sizeof(Type7ModeData) == 852);
inline constexpr memory::LayoutField kType7ModeFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {808, 4}, {812, 4}, {816, 4}, {840, 4}, {844, 4}, {848, 4}};
inline constexpr memory::Layout kType7ModeLayout = {kType7ModeFields, 14};

struct Type7Mode : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x82010FB0;
  Type7ModeData* data() const { return static_cast<Type7ModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static Type7Mode New(uint8_t* base) {
    return Type7Mode(base, memory::MakeHandle(new Type7ModeData(), sizeof(Type7ModeData), "Type7Mode", &kType7ModeLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kType7ModeLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType7ModeLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82010FB0
  uint32_t vtable() const { return on_host() ? uint32_t(data()->vtable) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), the delegate functions' r3
  uint8_t answer_changed_bits() const { return on_host() ? uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kType7ModeLayout, 0, 1)) : guest::LoadU8(base, address + 0); }
  void set_answer_changed_bits(uint8_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType7ModeLayout, 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the answer's first word (ModeAnswer.word0), TearDown's copy of the block's answer
  uint32_t answer_word0_bits() const { return on_host() ? uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kType7ModeLayout, 0, 4)) : guest::LoadU32(base, address + 0); }
  void set_answer_word0_bits(uint32_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType7ModeLayout, 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t answer_word0() const { return uint32_t(answer_word0_bits()); }
  void set_answer_word0(uint32_t v) const { set_answer_word0_bits(uint32_t(v)); }
  // the delegate at +8 (also the answer's next type +4 by the measure's join)
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
  // WaitForLoads, then Mode::NextModeThunk (NextMode), Leaving, TearDown
  uint32_t delegate_function() const { return on_host() ? uint32_t(data()->delegate_function) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_delegate_function(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_bits() const { return on_host() ? data()->delegate_function : guest::LoadU32(base, address + 16); }
  void set_delegate_function_bits(uint32_t v) const { if (on_host()) data()->delegate_function = v; else guest::StoreU32(base, address + 16, v); }
  // 0
  uint32_t delegate_adjustment() const { return on_host() ? uint32_t(data()->delegate_adjustment) : uint32_t(guest::LoadU32(base, address + 20)); }
  void set_delegate_adjustment(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_bits() const { return on_host() ? data()->delegate_adjustment : guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (on_host()) data()->delegate_adjustment = v; else guest::StoreU32(base, address + 20, v); }
  // 0x82010FA4: Interrupt, ForceExit, Resume
  uint32_t vtable32() const { return on_host() ? uint32_t(data()->vtable32) : uint32_t(guest::LoadU32(base, address + 32)); }
  void set_vtable32(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_bits() const { return on_host() ? data()->vtable32 : guest::LoadU32(base, address + 32); }
  void set_vtable32_bits(uint32_t v) const { if (on_host()) data()->vtable32 = v; else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (seven machines, in Init's order)
  uint8_t* handlers() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 36 : guest::Pointer(base, address + 36); }
  uint32_t handler_count() const { return on_host() ? uint32_t(data()->handler_count) : uint32_t(guest::LoadU32(base, address + 236)); }
  void set_handler_count(uint32_t v) const { if (on_host()) data()->handler_count = v; else guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_bits() const { return on_host() ? data()->handler_count : guest::LoadU32(base, address + 236); }
  void set_handler_count_bits(uint32_t v) const { if (on_host()) data()->handler_count = v; else guest::StoreU32(base, address + 236, v); }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint8_t* ring() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 240 : guest::Pointer(base, address + 240); }
  // measure fault: Interrupt's r3 is the mode + 32, so this is the mode's +840 (ring_count)
  uint32_t iface_ring_count() const { return on_host() ? uint32_t(data()->iface_ring_count) : uint32_t(guest::LoadU32(base, address + 808)); }
  void set_iface_ring_count(uint32_t v) const { if (on_host()) data()->iface_ring_count = v; else guest::StoreU32(base, address + 808, v); }
  uint32_t iface_ring_count_bits() const { return on_host() ? data()->iface_ring_count : guest::LoadU32(base, address + 808); }
  void set_iface_ring_count_bits(uint32_t v) const { if (on_host()) data()->iface_ring_count = v; else guest::StoreU32(base, address + 808, v); }
  // measure fault: the mode's +844 (ring_start) through the +32 interface
  uint32_t iface_ring_start() const { return on_host() ? uint32_t(data()->iface_ring_start) : uint32_t(guest::LoadU32(base, address + 812)); }
  void set_iface_ring_start(uint32_t v) const { if (on_host()) data()->iface_ring_start = v; else guest::StoreU32(base, address + 812, v); }
  uint32_t iface_ring_start_bits() const { return on_host() ? data()->iface_ring_start : guest::LoadU32(base, address + 812); }
  void set_iface_ring_start_bits(uint32_t v) const { if (on_host()) data()->iface_ring_start = v; else guest::StoreU32(base, address + 812, v); }
  // measure fault: the mode's +848 (block) through the +32 interface
  template <typename T = Type7Screens>
  T iface_block() const { return T(base, on_host() ? data()->iface_block : guest::LoadU32(base, address + 816)); }
  void set_iface_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  void set_iface_block(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t iface_block_bits() const { return on_host() ? data()->iface_block : guest::LoadU32(base, address + 816); }
  void set_iface_block_bits(uint32_t v) const { if (on_host()) data()->iface_block = v; else guest::StoreU32(base, address + 816, v); }
  uint32_t ring_count() const { return on_host() ? uint32_t(data()->ring_count) : uint32_t(guest::LoadU32(base, address + 840)); }
  void set_ring_count(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_bits() const { return on_host() ? data()->ring_count : guest::LoadU32(base, address + 840); }
  void set_ring_count_bits(uint32_t v) const { if (on_host()) data()->ring_count = v; else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_start() const { return on_host() ? uint32_t(data()->ring_start) : uint32_t(guest::LoadU32(base, address + 844)); }
  void set_ring_start(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_bits() const { return on_host() ? data()->ring_start : guest::LoadU32(base, address + 844); }
  void set_ring_start_bits(uint32_t v) const { if (on_host()) data()->ring_start = v; else guest::StoreU32(base, address + 844, v); }
  // the screen block (71,292 bytes, VirtualAlloc'ed by Init, freed by StandMode::Release)
  template <typename T = Type7Screens>
  T block() const { return T(base, on_host() ? data()->block : guest::LoadU32(base, address + 848)); }
  void set_block(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 848); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
};

// allocated; measure: StandMode
#pragma pack(push, 1)
struct StandModeData {
  uint8_t pad_0[848] = {};
  uint32_t block = 0;  // +848
};
#pragma pack(pop)
static_assert(sizeof(StandModeData) == 852);
inline constexpr memory::LayoutField kStandModeFields[] = {{848, 4}};
inline constexpr memory::Layout kStandModeLayout = {kStandModeFields, 1};

struct StandMode : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  StandModeData* data() const { return static_cast<StandModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static StandMode New(uint8_t* base) {
    return StandMode(base, memory::MakeHandle(new StandModeData(), sizeof(StandModeData), "StandMode", &kStandModeLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kStandModeLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kStandModeLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // the screen block (Type6Screens or Type7Screens), freed and 0
  uint32_t block() const { return on_host() ? uint32_t(data()->block) : uint32_t(guest::LoadU32(base, address + 848)); }
  void set_block(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
  uint32_t block_bits() const { return on_host() ? data()->block : guest::LoadU32(base, address + 848); }
  void set_block_bits(uint32_t v) const { if (on_host()) data()->block = v; else guest::StoreU32(base, address + 848, v); }
};

// embedded; measure: Type6Screen0
struct Type6Screen0 : View {
  using View::View;
  static constexpr uint32_t kSize = 9668;
  // the machine's transitions (StateMachine, game-modes.toml)
  uint32_t machine_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 enter, 1 open the movie, 2 wait for it, 4 show, 5 shown, 7 leaving, 6 done, 8 suspended, 9 wait for the mode, 10 leave the mode
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movie 22 loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // open at once (OnEnter: state 1, else 9); 1 from the mode's Init
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // Suspend deletes movie 22 (0 from the mode's Init: kept)
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
  // VirtualAlloc(76) by OnEnter
  template <typename T = Type67Screen0Words>
  T words() const { return T(base, guest::LoadU32(base, address + 9640)); }
  void set_words(const View& v) const { guest::StoreU32(base, address + 9640, v.address); }
  void set_words(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t words_address() const { return address + 9640; }
  // VirtualAlloc(28) by OnEnter: the page state
  template <typename T = PageObject>
  T pages() const { return T(base, guest::LoadU32(base, address + 9644)); }
  void set_pages(const View& v) const { guest::StoreU32(base, address + 9644, v.address); }
  void set_pages(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t pages_address() const { return address + 9644; }
  // VirtualAlloc(68) by OnEnter: items 0 to 4
  template <typename T = FrontEndMenu>
  T menu() const { return T(base, guest::LoadU32(base, address + 9648)); }
  void set_menu(const View& v) const { guest::StoreU32(base, address + 9648, v.address); }
  void set_menu(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t menu_address() const { return address + 9648; }
  // the item accepted (the menu's cursor; Show puts the cursor back on it)
  uint32_t item_picked() const { return guest::LoadU32(base, address + 9652); }
  void set_item_picked(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t item_picked_address() const { return address + 9652; }
  // 0 when a quit confirm is asked (MenuPage, OnDialogClosed); readers not found
  uint32_t unknown_9656() const { return guest::LoadU32(base, address + 9656); }
  void set_unknown_9656(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t unknown_9656_address() const { return address + 9656; }
  // 4 the first quit confirm, 5 the second, -1 none (OnDialogClosed)
  uint32_t confirm_asked() const { return guest::LoadU32(base, address + 9664); }
  void set_confirm_asked(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t confirm_asked_address() const { return address + 9664; }
};

// embedded; measure: Type7Screen0
struct Type7Screen0 : View {
  using View::View;
  static constexpr uint32_t kSize = 9660;
  static constexpr uint32_t kVtable = 0x82010B5C;
  // 0x82010B5C (the empty base)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the machine's transitions (StateMachine, game-modes.toml)
  uint32_t machine_address() const { return address + 4; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9604); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t transition_count_address() const { return address + 9604; }
  // as Type6Screen0's
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  // movie 22 loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t loaded_address() const { return address + 9612; }
  // open at once (OnEnter: state 1, else 9)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9616); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t reenter_address() const { return address + 9616; }
  // Suspend deletes movie 22
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9620); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t delete_movie_address() const { return address + 9620; }
  // VirtualAlloc(76) by OnEnter
  template <typename T = Type67Screen0Words>
  T words() const { return T(base, guest::LoadU32(base, address + 9644)); }
  void set_words(const View& v) const { guest::StoreU32(base, address + 9644, v.address); }
  void set_words(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t words_address() const { return address + 9644; }
  // VirtualAlloc(28) by OnEnter: the page state
  template <typename T = PageObject>
  T pages() const { return T(base, guest::LoadU32(base, address + 9648)); }
  void set_pages(const View& v) const { guest::StoreU32(base, address + 9648, v.address); }
  void set_pages(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t pages_address() const { return address + 9648; }
  // VirtualAlloc(68) by OnEnter: items 0 to 3
  template <typename T = FrontEndMenu>
  T menu() const { return T(base, guest::LoadU32(base, address + 9652)); }
  void set_menu(const View& v) const { guest::StoreU32(base, address + 9652, v.address); }
  void set_menu(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t menu_address() const { return address + 9652; }
  // the item accepted
  uint32_t item_picked() const { return guest::LoadU32(base, address + 9656); }
  void set_item_picked(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t item_picked_address() const { return address + 9656; }
};

// allocated; measure: new<76>@Type6Screen0::OnEnter, new<76>@Type7Screen0::OnEnter
#pragma pack(push, 1)
struct Type67Screen0WordsData {
  uint32_t loading_shown = 0;  // +0
  uint8_t pad_4[64] = {};
  uint32_t unknown_68 = 0;  // +68
  uint32_t pick = 0;  // +72
};
#pragma pack(pop)
static_assert(sizeof(Type67Screen0WordsData) == 76);
inline constexpr memory::LayoutField kType67Screen0WordsFields[] = {{0, 4}, {68, 4}, {72, 4}};
inline constexpr memory::Layout kType67Screen0WordsLayout = {kType67Screen0WordsFields, 3};

struct Type67Screen0Words : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  Type67Screen0WordsData* data() const { return static_cast<Type67Screen0WordsData*>(memory::HandleObject(address)); }
  static Type67Screen0Words New(uint8_t* base) {
    return Type67Screen0Words(base, memory::MakeHandle(new Type67Screen0WordsData(), sizeof(Type67Screen0WordsData), "Type67Screen0Words", &kType67Screen0WordsLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kType67Screen0WordsLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kType67Screen0WordsLayout, offset, width, v); }
  // the stand's loading animation shown (named 44 to 46; Update)
  uint32_t loading_shown() const { return data()->loading_shown; }
  void set_loading_shown(uint32_t v) const { data()->loading_shown = v; }
  uint32_t loading_shown_bits() const { return data()->loading_shown; }
  void set_loading_shown_bits(uint32_t v) const { data()->loading_shown = v; }
  // 0 from OnEnter; readers not found
  uint32_t unknown_68() const { return data()->unknown_68; }
  void set_unknown_68(uint32_t v) const { data()->unknown_68 = v; }
  uint32_t unknown_68_bits() const { return data()->unknown_68; }
  void set_unknown_68_bits(uint32_t v) const { data()->unknown_68 = v; }
  // the dialog's answer: PickYes 1, PickNo 0, PickBack; 2 the power graph; -1 none
  uint32_t pick() const { return data()->pick; }
  void set_pick(uint32_t v) const { data()->pick = v; }
  uint32_t pick_bits() const { return data()->pick; }
  void set_pick_bits(uint32_t v) const { data()->pick = v; }
};

// embedded; measure: Type6Dialog, Type7Dialog, Dialog, Type6Mode.+816, Type7Mode.+816
struct Type67Dialog : View {
  using View::View;
  static constexpr uint32_t kSize = 9764;
  // the machine's transitions (StateMachine, game-modes.toml)
  uint32_t machine_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 load, 4 show, 5 shown, 7 leaving, 9 waiting
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movies 16 to 19 loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // open at once (OnEnter: state 1, else 9)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // an embedded page object (PageObject's layout)
  uint32_t page_state_address() const { return address + 9640; }
  // 0 the yes/no, 1 the notice, -1 while a step runs
  uint32_t page() const { return guest::LoadU32(base, address + 9640); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t page_address() const { return address + 9640; }
  uint32_t next_page() const { return guest::LoadU32(base, address + 9644); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t next_page_address() const { return address + 9644; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 9648); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t previous_page_address() const { return address + 9648; }
  // 0 open the yes/no, 1 close it, 2 open the notice, 3 close it, 4 move the cursor; -1 none
  uint32_t step() const { return guest::LoadU32(base, address + 9652); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t step_address() const { return address + 9652; }
  // -1 none
  uint32_t queued_step() const { return guest::LoadU32(base, address + 9656); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t queued_step_address() const { return address + 9656; }
  // the page object's +20: -1 at an open, 0 when a queued step starts
  uint32_t page_word_20() const { return guest::LoadU32(base, address + 9660); }
  void set_page_word_20(uint32_t v) const { guest::StoreU32(base, address + 9660, v); }
  uint32_t page_word_20_address() const { return address + 9660; }
  // the step's first frame done
  uint32_t step_started() const { return guest::LoadU32(base, address + 9664); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 9664, v); }
  uint32_t step_started_address() const { return address + 9664; }
  // an embedded front-end menu (FrontEndMenu's layout), items 0 to 1
  uint32_t menu_address() const { return address + 9668; }
  // 0 yes, 1 no (from movie 16's +40960 at the open)
  uint32_t menu_cursor() const { return guest::LoadU32(base, address + 9668); }
  void set_menu_cursor(uint32_t v) const { guest::StoreU32(base, address + 9668, v); }
  uint32_t menu_cursor_address() const { return address + 9668; }
  uint32_t menu_previous() const { return guest::LoadU32(base, address + 9672); }
  void set_menu_previous(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t menu_previous_address() const { return address + 9672; }
  uint32_t menu_moved() const { return guest::LoadU32(base, address + 9676); }
  void set_menu_moved(uint32_t v) const { guest::StoreU32(base, address + 9676, v); }
  uint32_t menu_moved_address() const { return address + 9676; }
  // 0
  uint32_t menu_first_item() const { return guest::LoadU32(base, address + 9680); }
  void set_menu_first_item(uint32_t v) const { guest::StoreU32(base, address + 9680, v); }
  uint32_t menu_first_item_address() const { return address + 9680; }
  // 1
  uint32_t menu_last_item() const { return guest::LoadU32(base, address + 9684); }
  void set_menu_last_item(uint32_t v) const { guest::StoreU32(base, address + 9684, v); }
  uint32_t menu_last_item_address() const { return address + 9684; }
  // 1
  uint32_t menu_wraps() const { return guest::LoadU32(base, address + 9688); }
  void set_menu_wraps(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t menu_wraps_address() const { return address + 9688; }
  uint32_t menu_up_down_locked() const { return guest::LoadU32(base, address + 9692); }
  void set_menu_up_down_locked(uint32_t v) const { guest::StoreU32(base, address + 9692, v); }
  uint32_t menu_up_down_locked_address() const { return address + 9692; }
  uint32_t menu_left_right_steps() const { return guest::LoadU32(base, address + 9696); }
  void set_menu_left_right_steps(uint32_t v) const { guest::StoreU32(base, address + 9696, v); }
  uint32_t menu_left_right_steps_address() const { return address + 9696; }
  uint32_t menu_shoulders_step() const { return guest::LoadU32(base, address + 9700); }
  void set_menu_shoulders_step(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t menu_shoulders_step_address() const { return address + 9700; }
  uint32_t menu_side() const { return guest::LoadU32(base, address + 9704); }
  void set_menu_side(uint32_t v) const { guest::StoreU32(base, address + 9704, v); }
  uint32_t menu_side_address() const { return address + 9704; }
  uint32_t menu_wrapped() const { return guest::LoadU32(base, address + 9708); }
  void set_menu_wrapped(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t menu_wrapped_address() const { return address + 9708; }
  uint32_t menu_visible_row() const { return guest::LoadU32(base, address + 9716); }
  void set_menu_visible_row(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t menu_visible_row_address() const { return address + 9716; }
  uint32_t menu_previous_row() const { return guest::LoadU32(base, address + 9720); }
  void set_menu_previous_row(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t menu_previous_row_address() const { return address + 9720; }
  uint32_t menu_player() const { return guest::LoadU32(base, address + 9732); }
  void set_menu_player(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t menu_player_address() const { return address + 9732; }
  // 0 from OnEnter, 1 when WaitForMovies finds the movies already loaded; readers not found
  uint32_t unknown_9736() const { return guest::LoadU32(base, address + 9736); }
  void set_unknown_9736(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t unknown_9736_address() const { return address + 9736; }
  // the state Update applies (9: the closed message posted); 21 none
  uint32_t queued_state() const { return guest::LoadU32(base, address + 9740); }
  void set_queued_state(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t queued_state_address() const { return address + 9740; }
  // the movie shown: 16 the yes/no, 19 the notice
  uint32_t movie() const { return guest::LoadU32(base, address + 9744); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t movie_address() const { return address + 9744; }
  // a reopen queued (QueueReopen) for Dialog::DrawWaiting
  uint32_t reopen() const { return guest::LoadU32(base, address + 9748); }
  void set_reopen(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t reopen_address() const { return address + 9748; }
  // 0 the yes/no, 1 the notice (OpenConfirm, OpenNotice; Cancel's choice)
  uint32_t is_notice() const { return guest::LoadU32(base, address + 9752); }
  void set_is_notice(uint32_t v) const { guest::StoreU32(base, address + 9752, v); }
  uint32_t is_notice_address() const { return address + 9752; }
  // type 6's status: 5 an open asked, 0 just shown, 4 asked by IsTakingInput, 1 taking input, 2 cancelled, 3 hidden, -1 loading
  uint32_t type6_status() const { return guest::LoadU32(base, address + 9756); }
  void set_type6_status(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t type6_status_address() const { return address + 9756; }
  // type 7's status (as type6_status)
  uint32_t type7_status() const { return guest::LoadU32(base, address + 9760); }
  void set_type7_status(uint32_t v) const { guest::StoreU32(base, address + 9760, v); }
  uint32_t type7_status_address() const { return address + 9760; }
  // measure fault: Type7Mode::Interrupt reads the block's +19424 (the dialog at +9664's type7_status), not the dialog's
  uint32_t fault_block_type7_status() const { return guest::LoadU32(base, address + 19424); }
  void set_fault_block_type7_status(uint32_t v) const { guest::StoreU32(base, address + 19424, v); }
  uint32_t fault_block_type7_status_address() const { return address + 19424; }
  // measure fault: Type6Mode::Interrupt reads the block's +19428 (the dialog at +9672's type6_status), not the dialog's
  uint32_t fault_block_type6_status() const { return guest::LoadU32(base, address + 19428); }
  void set_fault_block_type6_status(uint32_t v) const { guest::StoreU32(base, address + 19428, v); }
  uint32_t fault_block_type6_status_address() const { return address + 19428; }
};

// embedded; measure: ControllerOptions
struct ControllerOptions : View {
  using View::View;
  static constexpr uint32_t kSize = 9612;
  // measure fault: the steps' r3 is the page object's started word (controller_page's +24), filed here
  uint32_t fault_step_started() const { return guest::LoadU32(base, address + 0); }
  void set_fault_step_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_step_started_address() const { return address + 0; }
  // the machine's transitions (StateMachine, game-modes.toml)
  uint32_t machine_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 4 from Open, 7 from Leave
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 from OnEnter
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
};

// embedded; measure: ScreenMachine
struct ScreenMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9640;
  // 200 transitions of 48 bytes (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 9 from Wait; Reenter: 4 when loaded, else 1
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // the screen's movies loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // open at once
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // delete the movies when suspended
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
  // 0 at construction; the owner's
  uint32_t word_9620() const { return guest::LoadU32(base, address + 9620); }
  void set_word_9620(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t word_9620_address() const { return address + 9620; }
  // 0 at construction; the owner's
  uint32_t word_9624() const { return guest::LoadU32(base, address + 9624); }
  void set_word_9624(uint32_t v) const { guest::StoreU32(base, address + 9624, v); }
  uint32_t word_9624_address() const { return address + 9624; }
  // 0 at construction; the owner's
  uint32_t word_9628() const { return guest::LoadU32(base, address + 9628); }
  void set_word_9628(uint32_t v) const { guest::StoreU32(base, address + 9628, v); }
  uint32_t word_9628_address() const { return address + 9628; }
  // 0 at construction; the owner's
  uint32_t word_9632() const { return guest::LoadU32(base, address + 9632); }
  void set_word_9632(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t word_9632_address() const { return address + 9632; }
  // 0 at construction; the owner's
  uint32_t word_9636() const { return guest::LoadU32(base, address + 9636); }
  void set_word_9636(uint32_t v) const { guest::StoreU32(base, address + 9636, v); }
  uint32_t word_9636_address() const { return address + 9636; }
};

// embedded; measure: Screen
struct Type67OffsetScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  // measure fault: LeaveToHub's r3 is the machine itself, so this is its state (+9604)
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9604); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t transition_count_address() const { return address + 9604; }
  // 9 from Wait; Reenter: 4 when loaded, else 1
  uint32_t state() const { return guest::LoadU32(base, address + 9608); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t state_address() const { return address + 9608; }
  uint32_t loaded() const { return guest::LoadU32(base, address + 9612); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t loaded_address() const { return address + 9612; }
};

// allocated; measure: StandSlot
struct StandSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the garage stage (6,208 bytes, VirtualAlloc'ed by StandSlot::Init), 0 none
  template <typename T = GarageStage>
  T stage() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_stage(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_stage(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t stage_address() const { return address + 0; }
};

// embedded; measure: GarageCamera::OnPresetsLoaded.r6, GarageStage.+0@+112, GarageCamera::Reset.r3, GarageStage@+112
struct GarageCamera : View {
  using View::View;
  static constexpr uint32_t kSize = 4396;
  // 0 until the presets are loaded, then 1 (OnPresetsLoaded, on the loader thread)
  uint32_t set_up() const { return guest::LoadU32(base, address + 0); }
  void set_set_up(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t set_up_address() const { return address + 0; }
  // the eye's start (0, 0, 0, 1 at reset)
  uint32_t eye_address() const { return address + 16; }
  float eye_x() const { return guest::LoadFloat(base, address + 16); }
  void set_eye_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t eye_x_address() const { return address + 16; }
  float eye_y() const { return guest::LoadFloat(base, address + 20); }
  void set_eye_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t eye_y_address() const { return address + 20; }
  float eye_z() const { return guest::LoadFloat(base, address + 24); }
  void set_eye_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t eye_z_address() const { return address + 24; }
  // the target (0, 0, 0, 1 at reset)
  uint32_t target_address() const { return address + 32; }
  float target_x() const { return guest::LoadFloat(base, address + 32); }
  void set_target_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t target_x_address() const { return address + 32; }
  float target_y() const { return guest::LoadFloat(base, address + 36); }
  void set_target_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t target_y_address() const { return address + 36; }
  float target_z() const { return guest::LoadFloat(base, address + 40); }
  void set_target_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t target_z_address() const { return address + 40; }
  // the eye less the target
  uint32_t eye_offset_address() const { return address + 48; }
  // (0, 0, 0, 1) at reset; what it is isn't known
  uint32_t vector_64_address() const { return address + 64; }
  // (0, 0, 0, 1) at reset; what it is isn't known
  uint32_t vector_80_address() const { return address + 80; }
  // 0.0 at reset
  float unknown_96() const { return guest::LoadFloat(base, address + 96); }
  void set_unknown_96(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  // the elevation (0.4014 at reset; commands 0 and 1, clamped)
  float tilt() const { return guest::LoadFloat(base, address + 100); }
  void set_tilt(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t tilt_address() const { return address + 100; }
  // 0.0 at reset
  float unknown_104() const { return guest::LoadFloat(base, address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  // 640 / tan(60 degrees)
  float zoom() const { return guest::LoadFloat(base, address + 108); }
  void set_zoom(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t zoom_address() const { return address + 108; }
  // a matrix: the identity turned about x by the tilt
  uint32_t rotation_address() const { return address + 112; }
  // the orbit angle (commands 2 and 3, wrapped)
  float orbit() const { return guest::LoadFloat(base, address + 176); }
  void set_orbit(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t orbit_address() const { return address + 176; }
  // 5.1499 at reset (commands 4 and 5, clamped)
  float distance() const { return guest::LoadFloat(base, address + 180); }
  void set_distance(float v) const { guest::StoreFloat(base, address + 180, v); }
  uint32_t distance_address() const { return address + 180; }
  // from (orbit, tilt, 0)
  uint32_t quaternion_address() const { return address + 192; }
  // the field of view (60.0; commands 6 and 7, clamped)
  float fov() const { return guest::LoadFloat(base, address + 224); }
  void set_fov(float v) const { guest::StoreFloat(base, address + 224, v); }
  uint32_t fov_address() const { return address + 224; }
  // (0, pi, 0, 1) at reset; commands 8 to 13, wrapped
  uint32_t angles_address() const { return address + 240; }
  float angle_x() const { return guest::LoadFloat(base, address + 240); }
  void set_angle_x(float v) const { guest::StoreFloat(base, address + 240, v); }
  uint32_t angle_x_address() const { return address + 240; }
  float angle_y() const { return guest::LoadFloat(base, address + 244); }
  void set_angle_y(float v) const { guest::StoreFloat(base, address + 244, v); }
  uint32_t angle_y_address() const { return address + 244; }
  float angle_z() const { return guest::LoadFloat(base, address + 248); }
  void set_angle_z(float v) const { guest::StoreFloat(base, address + 248, v); }
  uint32_t angle_z_address() const { return address + 248; }
  // (1.35, 0.4, 0, 1) at reset; commands 14 to 19
  uint32_t translation_address() const { return address + 256; }
  float translation_x() const { return guest::LoadFloat(base, address + 256); }
  void set_translation_x(float v) const { guest::StoreFloat(base, address + 256, v); }
  uint32_t translation_x_address() const { return address + 256; }
  float translation_y() const { return guest::LoadFloat(base, address + 260); }
  void set_translation_y(float v) const { guest::StoreFloat(base, address + 260, v); }
  uint32_t translation_y_address() const { return address + 260; }
  float translation_z() const { return guest::LoadFloat(base, address + 264); }
  void set_translation_z(float v) const { guest::StoreFloat(base, address + 264, v); }
  uint32_t translation_z_address() const { return address + 264; }
  // 1 after a tilt, turn or distance command; 2 then 0 around a reset
  uint32_t moved() const { return guest::LoadU32(base, address + 272); }
  void set_moved(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t moved_address() const { return address + 272; }
  // the presets' index table (0x82619428, 290 slots a car)
  uint32_t preset_table() const { return guest::LoadU32(base, address + 292); }
  void set_preset_table(uint32_t v) const { guest::StoreU32(base, address + 292, v); }
  uint32_t preset_table_address() const { return address + 292; }
  // 1,024 pointers to CameraPreset
  uint32_t presets_address() const { return address + 296; }
  uint32_t preset_count() const { return guest::LoadU32(base, address + 4392); }
  void set_preset_count(uint32_t v) const { guest::StoreU32(base, address + 4392, v); }
  uint32_t preset_count_address() const { return address + 4392; }
};

// allocated; measure: GarageCamera::OnPresetsLoaded.r4
struct GaragePresetsData : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the records' count
  uint32_t count() const { return guest::LoadU32(base, address + 4); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t count_address() const { return address + 4; }
  // the records' offset from the data (64 bytes each, CameraPreset's layout)
  uint32_t records_offset() const { return guest::LoadU32(base, address + 8); }
  void set_records_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t records_offset_address() const { return address + 8; }
};

// allocated; measure: CameraPreset
#pragma pack(push, 1)
struct CameraPresetData {
  uint8_t pad_0[4] = {};
  float elevation = 0;  // +4
  float value_8 = 0;  // +8
  float value_12 = 0;  // +12
  float vector_16_x = 0;  // +16
  float vector_16_y = 0;  // +20
  float vector_16_z = 0;  // +24
  float vector_16_w = 0;  // +28
  float vector_32_x = 0;  // +32
  float vector_32_y = 0;  // +36
  float vector_32_z = 0;  // +40
  float vector_32_w = 0;  // +44
  float angles_x = 0;  // +48
  float angles_y = 0;  // +52
  float angles_z = 0;  // +56
  float angles_w = 0;  // +60
};
#pragma pack(pop)
static_assert(sizeof(CameraPresetData) == 64);
inline constexpr memory::LayoutField kCameraPresetFields[] = {{4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}};
inline constexpr memory::Layout kCameraPresetLayout = {kCameraPresetFields, 15};

struct CameraPreset : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CameraPresetData* data() const { return static_cast<CameraPresetData*>(memory::HandleObject(address)); }
  static CameraPreset New(uint8_t* base) {
    return CameraPreset(base, memory::MakeHandle(new CameraPresetData(), sizeof(CameraPresetData), "CameraPreset", &kCameraPresetLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCameraPresetLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCameraPresetLayout, offset, width, v); }
  // the camera's tilt (GarageStage::ApplyPreset)
  float elevation() const { return data()->elevation; }
  void set_elevation(float v) const { data()->elevation = v; }
  uint32_t elevation_bits() const { return std::bit_cast<uint32_t>(data()->elevation); }
  void set_elevation_bits(uint32_t v) const { data()->elevation = std::bit_cast<float>(v); }
  float value_8() const { return data()->value_8; }
  void set_value_8(float v) const { data()->value_8 = v; }
  uint32_t value_8_bits() const { return std::bit_cast<uint32_t>(data()->value_8); }
  void set_value_8_bits(uint32_t v) const { data()->value_8 = std::bit_cast<float>(v); }
  float value_12() const { return data()->value_12; }
  void set_value_12(float v) const { data()->value_12 = v; }
  uint32_t value_12_bits() const { return std::bit_cast<uint32_t>(data()->value_12); }
  void set_value_12_bits(uint32_t v) const { data()->value_12 = std::bit_cast<float>(v); }
  uint8_t* vector_16() const { return reinterpret_cast<uint8_t*>(data()) + 16; }
  float vector_16_x() const { return data()->vector_16_x; }
  void set_vector_16_x(float v) const { data()->vector_16_x = v; }
  uint32_t vector_16_x_bits() const { return std::bit_cast<uint32_t>(data()->vector_16_x); }
  void set_vector_16_x_bits(uint32_t v) const { data()->vector_16_x = std::bit_cast<float>(v); }
  float vector_16_y() const { return data()->vector_16_y; }
  void set_vector_16_y(float v) const { data()->vector_16_y = v; }
  uint32_t vector_16_y_bits() const { return std::bit_cast<uint32_t>(data()->vector_16_y); }
  void set_vector_16_y_bits(uint32_t v) const { data()->vector_16_y = std::bit_cast<float>(v); }
  float vector_16_z() const { return data()->vector_16_z; }
  void set_vector_16_z(float v) const { data()->vector_16_z = v; }
  uint32_t vector_16_z_bits() const { return std::bit_cast<uint32_t>(data()->vector_16_z); }
  void set_vector_16_z_bits(uint32_t v) const { data()->vector_16_z = std::bit_cast<float>(v); }
  float vector_16_w() const { return data()->vector_16_w; }
  void set_vector_16_w(float v) const { data()->vector_16_w = v; }
  uint32_t vector_16_w_bits() const { return std::bit_cast<uint32_t>(data()->vector_16_w); }
  void set_vector_16_w_bits(uint32_t v) const { data()->vector_16_w = std::bit_cast<float>(v); }
  uint8_t* vector_32() const { return reinterpret_cast<uint8_t*>(data()) + 32; }
  float vector_32_x() const { return data()->vector_32_x; }
  void set_vector_32_x(float v) const { data()->vector_32_x = v; }
  uint32_t vector_32_x_bits() const { return std::bit_cast<uint32_t>(data()->vector_32_x); }
  void set_vector_32_x_bits(uint32_t v) const { data()->vector_32_x = std::bit_cast<float>(v); }
  float vector_32_y() const { return data()->vector_32_y; }
  void set_vector_32_y(float v) const { data()->vector_32_y = v; }
  uint32_t vector_32_y_bits() const { return std::bit_cast<uint32_t>(data()->vector_32_y); }
  void set_vector_32_y_bits(uint32_t v) const { data()->vector_32_y = std::bit_cast<float>(v); }
  float vector_32_z() const { return data()->vector_32_z; }
  void set_vector_32_z(float v) const { data()->vector_32_z = v; }
  uint32_t vector_32_z_bits() const { return std::bit_cast<uint32_t>(data()->vector_32_z); }
  void set_vector_32_z_bits(uint32_t v) const { data()->vector_32_z = std::bit_cast<float>(v); }
  float vector_32_w() const { return data()->vector_32_w; }
  void set_vector_32_w(float v) const { data()->vector_32_w = v; }
  uint32_t vector_32_w_bits() const { return std::bit_cast<uint32_t>(data()->vector_32_w); }
  void set_vector_32_w_bits(uint32_t v) const { data()->vector_32_w = std::bit_cast<float>(v); }
  // the car's angles (the stage's +80, GarageStage::ApplyPreset)
  uint8_t* angles() const { return reinterpret_cast<uint8_t*>(data()) + 48; }
  float angles_x() const { return data()->angles_x; }
  void set_angles_x(float v) const { data()->angles_x = v; }
  uint32_t angles_x_bits() const { return std::bit_cast<uint32_t>(data()->angles_x); }
  void set_angles_x_bits(uint32_t v) const { data()->angles_x = std::bit_cast<float>(v); }
  float angles_y() const { return data()->angles_y; }
  void set_angles_y(float v) const { data()->angles_y = v; }
  uint32_t angles_y_bits() const { return std::bit_cast<uint32_t>(data()->angles_y); }
  void set_angles_y_bits(uint32_t v) const { data()->angles_y = std::bit_cast<float>(v); }
  float angles_z() const { return data()->angles_z; }
  void set_angles_z(float v) const { data()->angles_z = v; }
  uint32_t angles_z_bits() const { return std::bit_cast<uint32_t>(data()->angles_z); }
  void set_angles_z_bits(uint32_t v) const { data()->angles_z = std::bit_cast<float>(v); }
  float angles_w() const { return data()->angles_w; }
  void set_angles_w(float v) const { data()->angles_w = v; }
  uint32_t angles_w_bits() const { return std::bit_cast<uint32_t>(data()->angles_w); }
  void set_angles_w_bits(uint32_t v) const { data()->angles_w = std::bit_cast<float>(v); }
};

// embedded; measure: Type6Screens.+52692, Type7Screens.+42352
struct Type67PowerGraphHead : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x82010A68 (type 6), 0x82010FD0 (type 7)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// embedded; measure: PowerGraph
struct PowerGraphSource : View {
  using View::View;
  static constexpr uint32_t kSize = 8436;
  // a garage car's id (SetFromCar)
  uint32_t car_id() const { return guest::LoadU32(base, address + 12); }
  void set_car_id(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t car_id_address() const { return address + 12; }
  // a car list entry's block 2 +68 (SetEntry's class switch)
  uint8_t engine_type() const { return guest::LoadU8(base, address + 708); }
  void set_engine_type(uint8_t v) const { guest::StoreU8(base, address + 708, v); }
  uint32_t engine_type_address() const { return address + 708; }
  // a car list entry's block 2 +69
  uint8_t turbo() const { return guest::LoadU8(base, address + 709); }
  void set_turbo(uint8_t v) const { guest::StoreU8(base, address + 709, v); }
  uint32_t turbo_address() const { return address + 709; }
  // a car list entry's: the record's +4
  uint32_t stat_712() const { return guest::LoadU32(base, address + 712); }
  void set_stat_712(uint32_t v) const { guest::StoreU32(base, address + 712, v); }
  uint32_t stat_712_address() const { return address + 712; }
  // the record's +8
  uint32_t stat_716() const { return guest::LoadU32(base, address + 716); }
  void set_stat_716(uint32_t v) const { guest::StoreU32(base, address + 716, v); }
  uint32_t stat_716_address() const { return address + 716; }
  // the record's +16
  uint32_t stat_720() const { return guest::LoadU32(base, address + 720); }
  void set_stat_720(uint32_t v) const { guest::StoreU32(base, address + 720, v); }
  uint32_t stat_720_address() const { return address + 720; }
  // the record's +12
  uint32_t stat_724() const { return guest::LoadU32(base, address + 724); }
  void set_stat_724(uint32_t v) const { guest::StoreU32(base, address + 724, v); }
  uint32_t stat_724_address() const { return address + 724; }
  // a car list entry's maker (the record's +24)
  uint8_t maker() const { return guest::LoadU8(base, address + 769); }
  void set_maker(uint8_t v) const { guest::StoreU8(base, address + 769, v); }
  uint32_t maker_address() const { return address + 769; }
  // the record's +28
  uint8_t byte_770() const { return guest::LoadU8(base, address + 770); }
  void set_byte_770(uint8_t v) const { guest::StoreU8(base, address + 770, v); }
  uint32_t byte_770_address() const { return address + 770; }
  // a garage car's +1568 (five words to the record's +60)
  uint32_t parts() const { return guest::LoadU32(base, address + 1568); }
  void set_parts(uint32_t v) const { guest::StoreU32(base, address + 1568, v); }
  uint32_t parts_address() const { return address + 1568; }
  // a garage car's +1892 (the record's +0, as an int)
  float power() const { return guest::LoadFloat(base, address + 1892); }
  void set_power(float v) const { guest::StoreFloat(base, address + 1892, v); }
  uint32_t power_address() const { return address + 1892; }
  // a car list entry's car id (the record's +20)
  uint32_t entry_car_id() const { return guest::LoadU32(base, address + 8432); }
  void set_entry_car_id(uint32_t v) const { guest::StoreU32(base, address + 8432, v); }
  uint32_t entry_car_id_address() const { return address + 8432; }
};

// image; measure: -
struct Type67Car : View {
  using View::View;
  static constexpr uint32_t kSize = 1984;
  // a garage car (GarageCar)
  uint32_t garage_car_address() const { return address + 0; }
  // the stand's model made for it (GarageStage::ReloadModel's answer); 0 from Show
  uint32_t model_made() const { return guest::LoadU32(base, address + 1968); }
  void set_model_made(uint32_t v) const { guest::StoreU32(base, address + 1968, v); }
  uint32_t model_made_address() const { return address + 1968; }
  // where from: -1 none (made again), 0 stock, 2 the garage
  uint32_t source() const { return guest::LoadU32(base, address + 1972); }
  void set_source(uint32_t v) const { guest::StoreU32(base, address + 1972, v); }
  uint32_t source_address() const { return address + 1972; }
  // the garage list position it came from (the save's selected_car)
  uint32_t garage_index() const { return guest::LoadU32(base, address + 1976); }
  void set_garage_index(uint32_t v) const { guest::StoreU32(base, address + 1976, v); }
  uint32_t garage_index_address() const { return address + 1976; }
  // the garage car's first halfword (its paint pick), sign-extended; 0 for a stock car
  uint32_t paint_word() const { return guest::LoadU32(base, address + 1980); }
  void set_paint_word(uint32_t v) const { guest::StoreU32(base, address + 1980, v); }
  uint32_t paint_word_address() const { return address + 1980; }
};

// embedded; measure: Type6Mode.+816@+9672
struct Type67DialogAt9672 : View {
  using View::View;
  static constexpr uint32_t kSize = 9760;
  uint32_t unknown_9756() const { return guest::LoadU32(base, address + 9756); }
  void set_unknown_9756(uint32_t v) const { guest::StoreU32(base, address + 9756, v); }
  uint32_t unknown_9756_address() const { return address + 9756; }
};

// embedded; measure: Type6Screens@+4
struct Type6Screen0At4 : View {
  using View::View;
  static constexpr uint32_t kSize = 9608;
  uint32_t unknown_9604() const { return guest::LoadU32(base, address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
};

// embedded; measure: Type7Mode.+816@+9664
struct Type67DialogAt9664 : View {
  using View::View;
  static constexpr uint32_t kSize = 9764;
  uint32_t unknown_9760() const { return guest::LoadU32(base, address + 9760); }
  void set_unknown_9760(uint32_t v) const { guest::StoreU32(base, address + 9760, v); }
  uint32_t unknown_9760_address() const { return address + 9760; }
};

// embedded; measure: Type7Screens@+4
struct Type7Screen0At4 : View {
  using View::View;
  static constexpr uint32_t kSize = 9612;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_9608() const { return guest::LoadU32(base, address + 9608); }
  void set_unknown_9608(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t unknown_9608_address() const { return address + 9608; }
};

// The image words docs/functions/type6-type7-modes.md's code uses.
struct Type6Type7ModesGlobals {
  // the current mode's type (ControllerOptions::Leave posts by it)
  static uint32_t mode_type(uint8_t* base) { return guest::LoadU32(base, 0x826032DCu); }
  static void set_mode_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032DCu, v); }
  static constexpr uint32_t mode_type_address() { return 0x826032DCu; }
  // the controller pages' page object (28 bytes, ControllerOptions::OnEnter)
  template <typename T = PageObject>
  static T controller_page(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D848Cu)); }
  static void set_controller_page(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D848Cu, v.address); }
  static void set_controller_page(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D848Cu, v); }
  static constexpr uint32_t controller_page_address() { return 0x826D848Cu; }
  // the controller pages' menu (68 bytes; +64 = 1, player 1's pad)
  template <typename T = FrontEndMenu>
  static T controller_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8490u)); }
  static void set_controller_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8490u, v.address); }
  static void set_controller_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8490u, v); }
  static constexpr uint32_t controller_menu_address() { return 0x826D8490u; }
  // the settings the controller pages edit ([0x826D8478]'s block)
  template <typename T = OptionsSettings>
  static T controller_settings(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8494u)); }
  static void set_controller_settings(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8494u, v.address); }
  static void set_controller_settings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8494u, v); }
  static constexpr uint32_t controller_settings_address() { return 0x826D8494u; }
  // the stand scene's holder (StandScene::Create, Release)
  template <typename T = StandHolder>
  static T stand_holder(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83E0u, v.address); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
  // 0 once NextMode leaves (how the title is entered)
  static uint32_t leaving_flag(uint8_t* base) { return guest::LoadU32(base, 0x826364ECu); }
  static void set_leaving_flag(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826364ECu, v); }
  static constexpr uint32_t leaving_flag_address() { return 0x826364ECu; }
  // type 6's car: a garage car (1,968 bytes), +1968 the model made, +1972 where from (-1 none, 0 stock, 2 the garage), +1976 the garage index, +1980 the car's halfword
  static constexpr uint32_t type6_car_address() { return 0x826367E8u; }
  // type 7's car (type6_car's layout)
  static constexpr uint32_t type7_car_address() { return 0x826371D0u; }
  // Type7Screen0::Draw's page table (member pointers), built once
  static constexpr uint32_t type7_draw_table_address() { return 0x82815E20u; }
  // its guard (bit 0)
  static uint32_t type7_draw_guard(uint8_t* base) { return guest::LoadU32(base, 0x82815E38u); }
  static void set_type7_draw_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815E38u, v); }
  static constexpr uint32_t type7_draw_guard_address() { return 0x82815E38u; }
  // Type7Screen0::Update's step table (member pointers), built once
  static constexpr uint32_t type7_steps_address() { return 0x82815E3Cu; }
  // its guard (bit 0)
  static uint32_t type7_steps_guard(uint8_t* base) { return guest::LoadU32(base, 0x82815E5Cu); }
  static void set_type7_steps_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82815E5Cu, v); }
  static constexpr uint32_t type7_steps_guard_address() { return 0x82815E5Cu; }
  // Type6Screen0::Draw's page table (4 handlers)
  static constexpr uint32_t type6_draw_table_address() { return 0x826367B8u; }
  // Type6Screen0::Update's step table
  static constexpr uint32_t type6_steps_address() { return 0x826367C8u; }
};

}  // namespace schema
