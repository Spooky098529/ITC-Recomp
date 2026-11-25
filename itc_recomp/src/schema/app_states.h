// The views of docs/functions/app-states.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/app-states.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: App, AppStates.+4, App::FrameDriver.r3
#pragma pack(push, 1)
struct AppData {
  uint32_t vtable = 0;  // +0
  uint32_t vtable_4 = 0;  // +4
  uint32_t vtable_8 = 0;  // +8
  uint32_t vtable_12 = 0;  // +12
  uint32_t vtable_16 = 0;  // +16
  uint32_t interface_vtable = 0;  // +20
  uint32_t framework_input = 0;  // +24
  uint32_t handlers = 0;  // +28
  uint32_t handlers_head = 0;  // +32
  uint32_t handlers_size = 0;  // +36
  uint32_t toggled_handlers = 0;  // +40
  uint32_t toggled_handlers_head = 0;  // +44
  uint32_t toggled_handlers_size = 0;  // +48
  uint32_t always_handlers = 0;  // +52
  uint32_t always_handlers_head = 0;  // +56
  uint32_t always_handlers_size = 0;  // +60
  uint32_t listeners = 0;  // +64
  uint32_t listeners_head = 0;  // +68
  uint32_t listeners_size = 0;  // +72
  uint32_t messages = 0;  // +76
  uint32_t messages_first = 0;  // +80
  uint32_t messages_last = 0;  // +84
  uint32_t messages_end = 0;  // +88
  uint32_t message = 0;  // +92
  uint32_t requests = 0;  // +96
  uint32_t requests_first = 0;  // +100
  uint32_t requests_last = 0;  // +104
  uint32_t requests_end = 0;  // +108
  uint8_t toggled = 0;  // +112
  uint8_t step = 0;  // +113
  uint8_t unknown_114 = 0;  // +114
  uint8_t pad_115[1] = {};
  uint32_t states = 0;  // +116
  uint32_t camera = 0;  // +120
  uint32_t sound_manager = 0;  // +124
  uint32_t movie = 0;  // +128
};
#pragma pack(pop)
static_assert(sizeof(AppData) == 132);
inline constexpr memory::LayoutField kAppFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}};
inline constexpr memory::Layout kAppLayout = {kAppFields, 32};

struct App : View {
  using View::View;
  static constexpr uint32_t kSize = 132;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82000EA0;
  AppData* data() const { return static_cast<AppData*>(memory::HandleObject(address)); }
  static App New() {
    return App(memory::MakeHandle(new AppData(), sizeof(AppData), "App", &kAppLayout));
  }
  template <uint32_t Stride = kSize>
  static App NewArray(uint32_t count) {
    static_assert(sizeof(AppData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppData*>(::operator new(sizeof(AppData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppData();
    return App(memory::MakeHandle(p, sizeof(AppData) * count, "App", &kAppLayout, sizeof(AppData)));
  }
  App at(uint32_t i) const { return App(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppData* direct() const { return static_cast<AppData*>(memory::HandleOwn(address, &kAppLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the handler map run while the toggle is clear
  template <typename T = HandlerMap>
  T handlers_map() const { return T(address + 28); }
  // the handler map run while the toggle is set
  template <typename T = HandlerMap>
  T toggled_handlers_map() const { return T(address + 40); }
  // the handler map run every pass
  template <typename T = HandlerMap>
  T always_handlers_map() const { return T(address + 52); }
  // the listener map
  template <typename T = HandlerMap>
  T listeners_map() const { return T(address + 64); }
  // the message vector
  template <typename T = MessageVector>
  T messages_vector() const { return T(address + 76); }
  // the request vector
  template <typename T = RequestVector>
  T requests_vector() const { return T(address + 96); }
  // 0x82000EA0
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = AppVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // measure fault: App::ForEachMessage's and ForEachRequest's r3 is the result functor, not the app
  uint32_t fault_for_each_result_0_address() const { return address + 0; }
  uint64_t fault_for_each_result_0_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kAppLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(address + 0))); }
  void set_fault_for_each_result_0_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(address + 0, v); }
  uint64_t fault_for_each_result_0() const { return uint64_t(fault_for_each_result_0_bits()); }
  void set_fault_for_each_result_0(uint64_t v) const { set_fault_for_each_result_0_bits(uint64_t(v)); }
  // the +4 interface's: 0x82000E94 (base 0x82000ABC)
  uint32_t vtable_4_address() const { return address + 4; }
  uint32_t vtable_4_bits() const { if (auto* d = direct()) return d->vtable_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_vtable_4_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t vtable_4() const { if (auto* d = direct()) return d->vtable_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_vtable_4(uint32_t v) const { if (auto* d = direct()) { d->vtable_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the +8 interface's: 0x82000E80 (base 0x82000A74)
  uint32_t vtable_8_address() const { return address + 8; }
  uint32_t vtable_8_bits() const { if (auto* d = direct()) return d->vtable_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_vtable_8_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t vtable_8() const { if (auto* d = direct()) return d->vtable_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_vtable_8(uint32_t v) const { if (auto* d = direct()) { d->vtable_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // measure fault: as fault_for_each_result_0
  uint32_t fault_for_each_result_8_address() const { return address + 8; }
  uint64_t fault_for_each_result_8_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kAppLayout, 8, 8)); return uint64_t((memory::IsHandle(address + 8) ? uint64_t(memory::OwnerLoad(address + 8, 8)) : guest::LoadU64(address + 8))); }
  void set_fault_for_each_result_8_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppLayout, 8, 8, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 8, v); else guest::StoreU64(address + 8, v); }
  uint64_t fault_for_each_result_8() const { return uint64_t(fault_for_each_result_8_bits()); }
  void set_fault_for_each_result_8(uint64_t v) const { set_fault_for_each_result_8_bits(uint64_t(v)); }
  // the +12 interface's: 0x82000E70 (base 0x82000D48)
  uint32_t vtable_12_address() const { return address + 12; }
  uint32_t vtable_12_bits() const { if (auto* d = direct()) return d->vtable_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_vtable_12_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t vtable_12() const { if (auto* d = direct()) return d->vtable_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_vtable_12(uint32_t v) const { if (auto* d = direct()) { d->vtable_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the +16 interface's: 0x82000E38 (base 0x82000DD0); the owner a layer keeps (StateLayer +8)
  uint32_t vtable_16_address() const { return address + 16; }
  uint32_t vtable_16_bits() const { if (auto* d = direct()) return d->vtable_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_vtable_16_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t vtable_16() const { if (auto* d = direct()) return d->vtable_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_vtable_16(uint32_t v) const { if (auto* d = direct()) { d->vtable_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // the +20 interface's: 0x82000E08 (base 0x82000DA0); the interface the framework keeps at +120 (AppInterface)
  uint32_t interface_vtable_address() const { return address + 20; }
  uint32_t interface_vtable_bits() const { if (auto* d = direct()) return d->interface_vtable; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_interface_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->interface_vtable = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  template <typename T = AppInterfaceVtable>
  T interface_vtable() const { return T(interface_vtable_bits()); }
  void set_interface_vtable(const View& w) const { set_interface_vtable_bits(w.address); }
  void set_interface_vtable(uint32_t v) const { set_interface_vtable_bits(v); }
  // the framework's +36 interface (its slot 1 the input manager); also App::FrameDriver's +20 read (r3 = app + 4)
  uint32_t framework_input_address() const { return address + 24; }
  uint32_t framework_input_bits() const { if (auto* d = direct()) return d->framework_input; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_framework_input_bits(uint32_t v) const { if (auto* d = direct()) { d->framework_input = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  template <typename T = AppFrameworkIface>
  T framework_input() const { return T(framework_input_bits()); }
  void set_framework_input(const View& w) const { set_framework_input_bits(w.address); }
  void set_framework_input(uint32_t v) const { set_framework_input_bits(v); }
  // a handler map (std::map of delegates by key), run while the toggle is clear or a step was asked: its first word, not read
  uint32_t handlers_address() const { return address + 28; }
  uint32_t handlers_bits() const { if (auto* d = direct()) return d->handlers; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_handlers_bits(uint32_t v) const { if (auto* d = direct()) { d->handlers = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t handlers() const { if (auto* d = direct()) return d->handlers; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_handlers(uint32_t v) const { if (auto* d = direct()) { d->handlers = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t handlers_head_address() const { return address + 32; }
  uint32_t handlers_head_bits() const { if (auto* d = direct()) return d->handlers_head; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_handlers_head_bits(uint32_t v) const { if (auto* d = direct()) { d->handlers_head = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  template <typename T = AppHandlerMapHead>
  T handlers_head() const { return T(handlers_head_bits()); }
  void set_handlers_head(const View& w) const { set_handlers_head_bits(w.address); }
  void set_handlers_head(uint32_t v) const { set_handlers_head_bits(v); }
  uint32_t handlers_size_address() const { return address + 36; }
  uint32_t handlers_size_bits() const { if (auto* d = direct()) return d->handlers_size; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_handlers_size_bits(uint32_t v) const { if (auto* d = direct()) { d->handlers_size = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t handlers_size() const { if (auto* d = direct()) return d->handlers_size; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_handlers_size(uint32_t v) const { if (auto* d = direct()) { d->handlers_size = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  // the handler map run while the toggle is set: its first word, not read
  uint32_t toggled_handlers_address() const { return address + 40; }
  uint32_t toggled_handlers_bits() const { if (auto* d = direct()) return d->toggled_handlers; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_toggled_handlers_bits(uint32_t v) const { if (auto* d = direct()) { d->toggled_handlers = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t toggled_handlers() const { if (auto* d = direct()) return d->toggled_handlers; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_toggled_handlers(uint32_t v) const { if (auto* d = direct()) { d->toggled_handlers = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t toggled_handlers_head_address() const { return address + 44; }
  uint32_t toggled_handlers_head_bits() const { if (auto* d = direct()) return d->toggled_handlers_head; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_toggled_handlers_head_bits(uint32_t v) const { if (auto* d = direct()) { d->toggled_handlers_head = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  template <typename T = AppHandlerMapHead>
  T toggled_handlers_head() const { return T(toggled_handlers_head_bits()); }
  void set_toggled_handlers_head(const View& w) const { set_toggled_handlers_head_bits(w.address); }
  void set_toggled_handlers_head(uint32_t v) const { set_toggled_handlers_head_bits(v); }
  uint32_t toggled_handlers_size_address() const { return address + 48; }
  uint32_t toggled_handlers_size_bits() const { if (auto* d = direct()) return d->toggled_handlers_size; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_toggled_handlers_size_bits(uint32_t v) const { if (auto* d = direct()) { d->toggled_handlers_size = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t toggled_handlers_size() const { if (auto* d = direct()) return d->toggled_handlers_size; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_toggled_handlers_size(uint32_t v) const { if (auto* d = direct()) { d->toggled_handlers_size = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // the handler map run every pass: its first word, not read
  uint32_t always_handlers_address() const { return address + 52; }
  uint32_t always_handlers_bits() const { if (auto* d = direct()) return d->always_handlers; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_always_handlers_bits(uint32_t v) const { if (auto* d = direct()) { d->always_handlers = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t always_handlers() const { if (auto* d = direct()) return d->always_handlers; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_always_handlers(uint32_t v) const { if (auto* d = direct()) { d->always_handlers = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t always_handlers_head_address() const { return address + 56; }
  uint32_t always_handlers_head_bits() const { if (auto* d = direct()) return d->always_handlers_head; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_always_handlers_head_bits(uint32_t v) const { if (auto* d = direct()) { d->always_handlers_head = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  template <typename T = AppHandlerMapHead>
  T always_handlers_head() const { return T(always_handlers_head_bits()); }
  void set_always_handlers_head(const View& w) const { set_always_handlers_head_bits(w.address); }
  void set_always_handlers_head(uint32_t v) const { set_always_handlers_head_bits(v); }
  uint32_t always_handlers_size_address() const { return address + 60; }
  uint32_t always_handlers_size_bits() const { if (auto* d = direct()) return d->always_handlers_size; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_always_handlers_size_bits(uint32_t v) const { if (auto* d = direct()) { d->always_handlers_size = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t always_handlers_size() const { if (auto* d = direct()) return d->always_handlers_size; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_always_handlers_size(uint32_t v) const { if (auto* d = direct()) { d->always_handlers_size = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // the listener map (given the messages): its first word, not read
  uint32_t listeners_address() const { return address + 64; }
  uint32_t listeners_bits() const { if (auto* d = direct()) return d->listeners; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_listeners_bits(uint32_t v) const { if (auto* d = direct()) { d->listeners = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t listeners() const { if (auto* d = direct()) return d->listeners; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_listeners(uint32_t v) const { if (auto* d = direct()) { d->listeners = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t listeners_head_address() const { return address + 68; }
  uint32_t listeners_head_bits() const { if (auto* d = direct()) return d->listeners_head; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_listeners_head_bits(uint32_t v) const { if (auto* d = direct()) { d->listeners_head = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  template <typename T = AppHandlerMapHead>
  T listeners_head() const { return T(listeners_head_bits()); }
  void set_listeners_head(const View& w) const { set_listeners_head_bits(w.address); }
  void set_listeners_head(uint32_t v) const { set_listeners_head_bits(v); }
  uint32_t listeners_size_address() const { return address + 72; }
  uint32_t listeners_size_bits() const { if (auto* d = direct()) return d->listeners_size; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_listeners_size_bits(uint32_t v) const { if (auto* d = direct()) { d->listeners_size = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t listeners_size() const { if (auto* d = direct()) return d->listeners_size; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_listeners_size(uint32_t v) const { if (auto* d = direct()) { d->listeners_size = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  // the message vector (MessageVector) starts here: its first word, not read
  uint32_t messages_address() const { return address + 76; }
  uint32_t messages_bits() const { if (auto* d = direct()) return d->messages; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_messages_bits(uint32_t v) const { if (auto* d = direct()) { d->messages = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t messages() const { if (auto* d = direct()) return d->messages; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_messages(uint32_t v) const { if (auto* d = direct()) { d->messages = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t messages_first_address() const { return address + 80; }
  uint32_t messages_first_bits() const { if (auto* d = direct()) return d->messages_first; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_messages_first_bits(uint32_t v) const { if (auto* d = direct()) { d->messages_first = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  template <typename T = AppMessageWord>
  T messages_first() const { return T(messages_first_bits()); }
  void set_messages_first(const View& w) const { set_messages_first_bits(w.address); }
  void set_messages_first(uint32_t v) const { set_messages_first_bits(v); }
  uint32_t messages_last_address() const { return address + 84; }
  uint32_t messages_last_bits() const { if (auto* d = direct()) return d->messages_last; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_messages_last_bits(uint32_t v) const { if (auto* d = direct()) { d->messages_last = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  template <typename T = AppMessageWord>
  T messages_last() const { return T(messages_last_bits()); }
  void set_messages_last(const View& w) const { set_messages_last_bits(w.address); }
  void set_messages_last(uint32_t v) const { set_messages_last_bits(v); }
  uint32_t messages_end_address() const { return address + 88; }
  uint32_t messages_end_bits() const { if (auto* d = direct()) return d->messages_end; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_messages_end_bits(uint32_t v) const { if (auto* d = direct()) { d->messages_end = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  template <typename T = AppMessageWord>
  T messages_end() const { return T(messages_end_bits()); }
  void set_messages_end(const View& w) const { set_messages_end_bits(w.address); }
  void set_messages_end(uint32_t v) const { set_messages_end_bits(v); }
  // the message being delivered (the listeners get its address)
  uint32_t message_address() const { return address + 92; }
  uint32_t message_bits() const { if (auto* d = direct()) return d->message; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_message_bits(uint32_t v) const { if (auto* d = direct()) { d->message = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  uint32_t message() const { if (auto* d = direct()) return d->message; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_message(uint32_t v) const { if (auto* d = direct()) { d->message = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  // the request vector (12-byte requests {type, key, delegate}) starts here: its first word, not read by the app's code; the measure's +96 reads are the +20 interface's +96, the state stack (App::IsRunning, App::Start: a measure fault)
  uint32_t requests_address() const { return address + 96; }
  uint32_t requests_bits() const { if (auto* d = direct()) return d->requests; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_requests_bits(uint32_t v) const { if (auto* d = direct()) { d->requests = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t requests() const { if (auto* d = direct()) return d->requests; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_requests(uint32_t v) const { if (auto* d = direct()) { d->requests = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t requests_first_address() const { return address + 100; }
  uint32_t requests_first_bits() const { if (auto* d = direct()) return d->requests_first; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_requests_first_bits(uint32_t v) const { if (auto* d = direct()) { d->requests_first = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t requests_first() const { if (auto* d = direct()) return d->requests_first; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_requests_first(uint32_t v) const { if (auto* d = direct()) { d->requests_first = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t requests_last_address() const { return address + 104; }
  uint32_t requests_last_bits() const { if (auto* d = direct()) return d->requests_last; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_requests_last_bits(uint32_t v) const { if (auto* d = direct()) { d->requests_last = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t requests_last() const { if (auto* d = direct()) return d->requests_last; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_requests_last(uint32_t v) const { if (auto* d = direct()) { d->requests_last = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t requests_end_address() const { return address + 108; }
  uint32_t requests_end_bits() const { if (auto* d = direct()) return d->requests_end; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_requests_end_bits(uint32_t v) const { if (auto* d = direct()) { d->requests_end = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t requests_end() const { if (auto* d = direct()) return d->requests_end; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_requests_end(uint32_t v) const { if (auto* d = direct()) { d->requests_end = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  // the toggle player 1's Start flips (message 0 when set, 2 when cleared)
  uint32_t toggled_address() const { return address + 112; }
  uint8_t toggled_bits() const { if (auto* d = direct()) return d->toggled; return uint8_t((memory::IsHandle(address + 112) ? uint8_t(memory::OwnerLoad(address + 112, 1)) : guest::LoadU8(address + 112))); }
  void set_toggled_bits(uint8_t v) const { if (auto* d = direct()) { d->toggled = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 1, v); else guest::StoreU8(address + 112, v); }
  uint8_t toggled() const { if (auto* d = direct()) return d->toggled; return uint8_t((memory::IsHandle(address + 112) ? uint8_t(memory::OwnerLoad(address + 112, 1)) : guest::LoadU8(address + 112))); }
  void set_toggled(uint8_t v) const { if (auto* d = direct()) { d->toggled = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 1, v); else guest::StoreU8(address + 112, v); }
  // a step asked (Back while toggled, message 1); cleared after the pass
  uint32_t step_address() const { return address + 113; }
  uint8_t step_bits() const { if (auto* d = direct()) return d->step; return uint8_t((memory::IsHandle(address + 113) ? uint8_t(memory::OwnerLoad(address + 113, 1)) : guest::LoadU8(address + 113))); }
  void set_step_bits(uint8_t v) const { if (auto* d = direct()) { d->step = v; return; } if (memory::IsHandle(address + 113)) memory::OwnerStore(address + 113, 1, v); else guest::StoreU8(address + 113, v); }
  uint8_t step() const { if (auto* d = direct()) return d->step; return uint8_t((memory::IsHandle(address + 113) ? uint8_t(memory::OwnerLoad(address + 113, 1)) : guest::LoadU8(address + 113))); }
  void set_step(uint8_t v) const { if (auto* d = direct()) { d->step = v; return; } if (memory::IsHandle(address + 113)) memory::OwnerStore(address + 113, 1, v); else guest::StoreU8(address + 113, v); }
  // 0 at construction, not read
  uint32_t unknown_114_address() const { return address + 114; }
  uint8_t unknown_114_bits() const { if (auto* d = direct()) return d->unknown_114; return uint8_t((memory::IsHandle(address + 114) ? uint8_t(memory::OwnerLoad(address + 114, 1)) : guest::LoadU8(address + 114))); }
  void set_unknown_114_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_114 = v; return; } if (memory::IsHandle(address + 114)) memory::OwnerStore(address + 114, 1, v); else guest::StoreU8(address + 114, v); }
  uint8_t unknown_114() const { if (auto* d = direct()) return d->unknown_114; return uint8_t((memory::IsHandle(address + 114) ? uint8_t(memory::OwnerLoad(address + 114, 1)) : guest::LoadU8(address + 114))); }
  void set_unknown_114(uint8_t v) const { if (auto* d = direct()) { d->unknown_114 = v; return; } if (memory::IsHandle(address + 114)) memory::OwnerStore(address + 114, 1, v); else guest::StoreU8(address + 114, v); }
  // the state stack (AppStates::Create)
  uint32_t states_address() const { return address + 116; }
  uint32_t states_bits() const { if (auto* d = direct()) return d->states; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_states_bits(uint32_t v) const { if (auto* d = direct()) { d->states = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  template <typename T = AppStates>
  T states() const { return T(states_bits()); }
  void set_states(const View& w) const { set_states_bits(w.address); }
  void set_states(uint32_t v) const { set_states_bits(v); }
  // a second camera class (368 bytes, sub_820A8B88)
  uint32_t camera_address() const { return address + 120; }
  uint32_t camera_bits() const { if (auto* d = direct()) return d->camera; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_camera_bits(uint32_t v) const { if (auto* d = direct()) { d->camera = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t camera() const { if (auto* d = direct()) return d->camera; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_camera(uint32_t v) const { if (auto* d = direct()) { d->camera = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  // AudioManager::Create
  uint32_t sound_manager_address() const { return address + 124; }
  uint32_t sound_manager_bits() const { if (auto* d = direct()) return d->sound_manager; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_sound_manager_bits(uint32_t v) const { if (auto* d = direct()) { d->sound_manager = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  uint32_t sound_manager() const { if (auto* d = direct()) return d->sound_manager; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_sound_manager(uint32_t v) const { if (auto* d = direct()) { d->sound_manager = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  // 312 bytes, Movie::Movie
  uint32_t movie_address() const { return address + 128; }
  uint32_t movie_bits() const { if (auto* d = direct()) return d->movie; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->movie = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  uint32_t movie() const { if (auto* d = direct()) return d->movie; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_movie(uint32_t v) const { if (auto* d = direct()) { d->movie = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
};

// image; measure: AppStates.+4.+0
struct AppVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: the +20 interface (AppInterface)
  uint32_t interface() const { return guest::LoadU32(address + 4); }
  void set_interface(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t interface_address() const { return address + 4; }
  // slot 2: the interface at app + 16, a layer's owner
  uint32_t owner() const { return guest::LoadU32(address + 8); }
  void set_owner(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t owner_address() const { return address + 8; }
};

// embedded; measure: AppStates::OnMessage.r4
#pragma pack(push, 1)
struct AppInterfaceData {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[52] = {};
  uint32_t messages = 0;  // +56
  uint8_t pad_60[16] = {};
  uint32_t requests = 0;  // +76
  uint8_t pad_80[16] = {};
  uint32_t states = 0;  // +96
  uint8_t pad_100[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(AppInterfaceData) == 112);
inline constexpr memory::LayoutField kAppInterfaceFields[] = {{0, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {96, 4}};
inline constexpr memory::Layout kAppInterfaceLayout = {kAppInterfaceFields, 10};

struct AppInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppInterfaceData* data() const { return static_cast<AppInterfaceData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppInterface New() {
    return AppInterface(memory::MakeHandle(new AppInterfaceData(), sizeof(AppInterfaceData), "AppInterface", &kAppInterfaceLayout));
  }
  template <uint32_t Stride = kSize>
  static AppInterface NewArray(uint32_t count) {
    static_assert(sizeof(AppInterfaceData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppInterfaceData*>(::operator new(sizeof(AppInterfaceData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppInterfaceData();
    return AppInterface(memory::MakeHandle(p, sizeof(AppInterfaceData) * count, "AppInterface", &kAppInterfaceLayout, sizeof(AppInterfaceData)));
  }
  AppInterface at(uint32_t i) const { return AppInterface(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppInterfaceData* direct() const { return static_cast<AppInterfaceData*>(memory::HandleOwn(address, &kAppInterfaceLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppInterfaceLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppInterfaceLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the app's message vector
  template <typename T = MessageVector>
  T messages_vector() const { return T(address + 56); }
  // the app's request vector
  template <typename T = RequestVector>
  T requests_vector() const { return T(address + 76); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = AppInterfaceVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // the app's message vector (app + 76)
  uint32_t messages_address() const { return address + 56; }
  uint32_t messages_bits() const { if (auto* d = direct()) return d->messages; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_messages_bits(uint32_t v) const { if (auto* d = direct()) { d->messages = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t messages() const { if (auto* d = direct()) return d->messages; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_messages(uint32_t v) const { if (auto* d = direct()) { d->messages = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  // the app's request vector (app + 96)
  uint32_t requests_address() const { return address + 76; }
  uint32_t requests_bits() const { if (auto* d = direct()) return d->requests; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_requests_bits(uint32_t v) const { if (auto* d = direct()) { d->requests = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t requests() const { if (auto* d = direct()) return d->requests; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_requests(uint32_t v) const { if (auto* d = direct()) { d->requests = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  // the app's +116
  uint32_t states_address() const { return address + 96; }
  uint32_t states_bits() const { if (auto* d = direct()) return d->states; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_states_bits(uint32_t v) const { if (auto* d = direct()) { d->states = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  template <typename T = AppStates>
  T states() const { return T(states_bits()); }
  void set_states(const View& w) const { set_states_bits(w.address); }
  void set_states(uint32_t v) const { set_states_bits(v); }
};

// image; measure: App.+20, AppStates::OnMessage.r4.+0, App::FrameDriver.r3.+20
struct AppInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // measure fault: App::FrameDriver's read is the framework interface's vtable (r3 = app + 4, its +20 the app's +24)
  uint32_t fault_framework_vtable() const { return guest::LoadU32(address + 0); }
  void set_fault_framework_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t fault_framework_vtable_address() const { return address + 0; }
  // App::AddListener
  uint32_t add_listener() const { return guest::LoadU32(address + 4); }
  void set_add_listener(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t add_listener_address() const { return address + 4; }
  // App::AddAlwaysHandler
  uint32_t add_always_handler() const { return guest::LoadU32(address + 8); }
  void set_add_always_handler(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t add_always_handler_address() const { return address + 8; }
  // App::AddToggledHandler
  uint32_t add_toggled_handler() const { return guest::LoadU32(address + 12); }
  void set_add_toggled_handler(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t add_toggled_handler_address() const { return address + 12; }
  // App::AddHandler
  uint32_t add_handler() const { return guest::LoadU32(address + 16); }
  void set_add_handler(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t add_handler_address() const { return address + 16; }
  // probably (AppStates::OnMessage's teardown, listener 9); never ran
  uint32_t remove_listener() const { return guest::LoadU32(address + 20); }
  void set_remove_listener(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t remove_listener_address() const { return address + 20; }
  // probably; never ran
  uint32_t remove_always_handler() const { return guest::LoadU32(address + 24); }
  void set_remove_always_handler(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t remove_always_handler_address() const { return address + 24; }
  // probably (key 2); never ran
  uint32_t remove_toggled_handler() const { return guest::LoadU32(address + 28); }
  void set_remove_toggled_handler(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t remove_toggled_handler_address() const { return address + 28; }
  // probably (key 8); never ran
  uint32_t remove_handler() const { return guest::LoadU32(address + 32); }
  void set_remove_handler(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t remove_handler_address() const { return address + 32; }
  // App::Start
  uint32_t start() const { return guest::LoadU32(address + 36); }
  void set_start(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t start_address() const { return address + 36; }
  // App::SendMessage
  uint32_t send_message() const { return guest::LoadU32(address + 40); }
  void set_send_message(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t send_message_address() const { return address + 40; }
  // App::IsRunning
  uint32_t is_running() const { return guest::LoadU32(address + 44); }
  void set_is_running(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t is_running_address() const { return address + 44; }
};

// embedded; measure: App.+24
#pragma pack(push, 1)
struct AppFrameworkIfaceData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(AppFrameworkIfaceData) == 4);
inline constexpr memory::LayoutField kAppFrameworkIfaceFields[] = {{0, 4}};
inline constexpr memory::Layout kAppFrameworkIfaceLayout = {kAppFrameworkIfaceFields, 1};

struct AppFrameworkIface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppFrameworkIfaceData* data() const { return static_cast<AppFrameworkIfaceData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppFrameworkIface New() {
    return AppFrameworkIface(memory::MakeHandle(new AppFrameworkIfaceData(), sizeof(AppFrameworkIfaceData), "AppFrameworkIface", &kAppFrameworkIfaceLayout));
  }
  template <uint32_t Stride = kSize>
  static AppFrameworkIface NewArray(uint32_t count) {
    static_assert(sizeof(AppFrameworkIfaceData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppFrameworkIfaceData*>(::operator new(sizeof(AppFrameworkIfaceData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppFrameworkIfaceData();
    return AppFrameworkIface(memory::MakeHandle(p, sizeof(AppFrameworkIfaceData) * count, "AppFrameworkIface", &kAppFrameworkIfaceLayout, sizeof(AppFrameworkIfaceData)));
  }
  AppFrameworkIface at(uint32_t i) const { return AppFrameworkIface(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppFrameworkIfaceData* direct() const { return static_cast<AppFrameworkIfaceData*>(memory::HandleOwn(address, &kAppFrameworkIfaceLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppFrameworkIfaceLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppFrameworkIfaceLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = AppFrameworkIfaceVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
};

// image; measure: App.+24.+0
struct AppFrameworkIfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 1: the input manager
  uint32_t input_manager() const { return guest::LoadU32(address + 4); }
  void set_input_manager(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t input_manager_address() const { return address + 4; }
};

// allocated; measure: App.+32, App.+44, App.+56, App.+68
#pragma pack(push, 1)
struct AppHandlerMapHeadData {
  uint32_t left = 0;  // +0
  uint8_t pad_4[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(AppHandlerMapHeadData) == 12);
inline constexpr memory::LayoutField kAppHandlerMapHeadFields[] = {{0, 4}};
inline constexpr memory::Layout kAppHandlerMapHeadLayout = {kAppHandlerMapHeadFields, 1};

struct AppHandlerMapHead : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppHandlerMapHeadData* data() const { return static_cast<AppHandlerMapHeadData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppHandlerMapHead New() {
    return AppHandlerMapHead(memory::MakeHandle(new AppHandlerMapHeadData(), sizeof(AppHandlerMapHeadData), "AppHandlerMapHead", &kAppHandlerMapHeadLayout));
  }
  template <uint32_t Stride = kSize>
  static AppHandlerMapHead NewArray(uint32_t count) {
    static_assert(sizeof(AppHandlerMapHeadData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppHandlerMapHeadData*>(::operator new(sizeof(AppHandlerMapHeadData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppHandlerMapHeadData();
    return AppHandlerMapHead(memory::MakeHandle(p, sizeof(AppHandlerMapHeadData) * count, "AppHandlerMapHead", &kAppHandlerMapHeadLayout, sizeof(AppHandlerMapHeadData)));
  }
  AppHandlerMapHead at(uint32_t i) const { return AppHandlerMapHead(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppHandlerMapHeadData* direct() const { return static_cast<AppHandlerMapHeadData*>(memory::HandleOwn(address, &kAppHandlerMapHeadLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppHandlerMapHeadLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppHandlerMapHeadLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the leftmost node (begin)
  uint32_t left_address() const { return address + 0; }
  uint32_t left_bits() const { if (auto* d = direct()) return d->left; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_left_bits(uint32_t v) const { if (auto* d = direct()) { d->left = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t left() const { if (auto* d = direct()) return d->left; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_left(uint32_t v) const { if (auto* d = direct()) { d->left = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
};

// embedded; measure: App::AddHandler.r4, App::AddToggledHandler.r4, App::AddAlwaysHandler.r4, App::AddListener.r4
struct AppHandlerKey : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t key() const { return guest::LoadU32(address + 0); }
  void set_key(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t key_address() const { return address + 0; }
};

// embedded; measure: MessageVector::PushBack.r4, MessageVector::InsertN.r6, AppStates::OnMessage.r5, MessageVector.+8
struct AppMessageWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 toggle set, 1 a step, 2 toggle cleared, 5 and 6 the teardowns
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// allocated; measure: App::AddHandler.r5, App::AddToggledHandler.r5, App::AddAlwaysHandler.r5, App::AddListener.r5, new<12>@Delegate::Clone
#pragma pack(push, 1)
struct AppDelegateData {
  uint32_t vtable = 0;  // +0
  uint32_t object = 0;  // +4
  uint32_t function = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(AppDelegateData) == 12);
inline constexpr memory::LayoutField kAppDelegateFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kAppDelegateLayout = {kAppDelegateFields, 3};

struct AppDelegate : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppDelegateData* data() const { return static_cast<AppDelegateData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppDelegate New() {
    return AppDelegate(memory::MakeHandle(new AppDelegateData(), sizeof(AppDelegateData), "AppDelegate", &kAppDelegateLayout));
  }
  template <uint32_t Stride = kSize>
  static AppDelegate NewArray(uint32_t count) {
    static_assert(sizeof(AppDelegateData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppDelegateData*>(::operator new(sizeof(AppDelegateData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppDelegateData();
    return AppDelegate(memory::MakeHandle(p, sizeof(AppDelegateData) * count, "AppDelegate", &kAppDelegateLayout, sizeof(AppDelegateData)));
  }
  AppDelegate at(uint32_t i) const { return AppDelegate(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppDelegateData* direct() const { return static_cast<AppDelegateData*>(memory::HandleOwn(address, &kAppDelegateLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppDelegateLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppDelegateLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82002850 (base 0x82000ABC)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = AppDelegateVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  uint32_t object_address() const { return address + 4; }
  uint32_t object_bits() const { if (auto* d = direct()) return d->object; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_object_bits(uint32_t v) const { if (auto* d = direct()) { d->object = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t object() const { if (auto* d = direct()) return d->object; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_object(uint32_t v) const { if (auto* d = direct()) { d->object = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t function_address() const { return address + 8; }
  uint32_t function_bits() const { if (auto* d = direct()) return d->function; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_function_bits(uint32_t v) const { if (auto* d = direct()) { d->function = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t function() const { if (auto* d = direct()) return d->function; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_function(uint32_t v) const { if (auto* d = direct()) { d->function = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
};

// image; measure: App::AddHandler.r5.+0, App::AddToggledHandler.r5.+0, App::AddAlwaysHandler.r5.+0, App::AddListener.r5.+0
struct AppDelegateVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the base's sub_82094D20, never ran
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // Delegate::Invoke
  uint32_t invoke() const { return guest::LoadU32(address + 4); }
  void set_invoke(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t invoke_address() const { return address + 4; }
  // Delegate::Clone
  uint32_t clone() const { return guest::LoadU32(address + 8); }
  void set_clone(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t clone_address() const { return address + 8; }
  // sub_820AC9E8, never ran
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 12); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t deleting_destructor_address() const { return address + 12; }
};

// embedded; measure: MessageVector
#pragma pack(push, 1)
struct MessageVectorData {
  uint8_t pad_0[4] = {};
  uint32_t first = 0;  // +4
  uint32_t last = 0;  // +8
  uint32_t end = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(MessageVectorData) == 16);
inline constexpr memory::LayoutField kMessageVectorFields[] = {{4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kMessageVectorLayout = {kMessageVectorFields, 3};

struct MessageVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  MessageVectorData* data() const { return static_cast<MessageVectorData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static MessageVector New() {
    return MessageVector(memory::MakeHandle(new MessageVectorData(), sizeof(MessageVectorData), "MessageVector", &kMessageVectorLayout));
  }
  template <uint32_t Stride = kSize>
  static MessageVector NewArray(uint32_t count) {
    static_assert(sizeof(MessageVectorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MessageVectorData*>(::operator new(sizeof(MessageVectorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MessageVectorData();
    return MessageVector(memory::MakeHandle(p, sizeof(MessageVectorData) * count, "MessageVector", &kMessageVectorLayout, sizeof(MessageVectorData)));
  }
  MessageVector at(uint32_t i) const { return MessageVector(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MessageVectorData* direct() const { return static_cast<MessageVectorData*>(memory::HandleOwn(address, &kMessageVectorLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMessageVectorLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMessageVectorLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // measure fault: Erase's and Insert's r3 is the result iterator (vector, element), not the vector
  uint32_t fault_result_address() const { return address + 0; }
  uint64_t fault_result_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kMessageVectorLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(address + 0))); }
  void set_fault_result_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMessageVectorLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(address + 0, v); }
  uint64_t fault_result() const { return uint64_t(fault_result_bits()); }
  void set_fault_result(uint64_t v) const { set_fault_result_bits(uint64_t(v)); }
  uint32_t first_address() const { return address + 4; }
  uint32_t first_bits() const { if (auto* d = direct()) return d->first; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_first_bits(uint32_t v) const { if (auto* d = direct()) { d->first = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = AppMessageWord>
  T first() const { return T(first_bits()); }
  void set_first(const View& w) const { set_first_bits(w.address); }
  void set_first(uint32_t v) const { set_first_bits(v); }
  uint32_t last_address() const { return address + 8; }
  uint32_t last_bits() const { if (auto* d = direct()) return d->last; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_last_bits(uint32_t v) const { if (auto* d = direct()) { d->last = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  template <typename T = AppMessageWord>
  T last() const { return T(last_bits()); }
  void set_last(const View& w) const { set_last_bits(w.address); }
  void set_last(uint32_t v) const { set_last_bits(v); }
  uint32_t end_address() const { return address + 12; }
  uint32_t end_bits() const { if (auto* d = direct()) return d->end; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_end_bits(uint32_t v) const { if (auto* d = direct()) { d->end = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  template <typename T = AppMessageWord>
  T end() const { return T(end_bits()); }
  void set_end(const View& w) const { set_end_bits(w.address); }
  void set_end(uint32_t v) const { set_end_bits(v); }
};

// allocated; measure: AppStates, App.+96, sub_820AD8D0.ret
#pragma pack(push, 1)
struct AppStatesData {
  uint32_t vtable = 0;  // +0
  uint32_t app = 0;  // +4
  uint32_t root = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(AppStatesData) == 12);
inline constexpr memory::LayoutField kAppStatesFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kAppStatesLayout = {kAppStatesFields, 3};

struct AppStates : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x820028F8;
  AppStatesData* data() const { return static_cast<AppStatesData*>(memory::HandleObject(address)); }
  static AppStates New() {
    return AppStates(memory::MakeHandle(new AppStatesData(), sizeof(AppStatesData), "AppStates", &kAppStatesLayout));
  }
  template <uint32_t Stride = kSize>
  static AppStates NewArray(uint32_t count) {
    static_assert(sizeof(AppStatesData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppStatesData*>(::operator new(sizeof(AppStatesData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppStatesData();
    return AppStates(memory::MakeHandle(p, sizeof(AppStatesData) * count, "AppStates", &kAppStatesLayout, sizeof(AppStatesData)));
  }
  AppStates at(uint32_t i) const { return AppStates(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppStatesData* direct() const { return static_cast<AppStatesData*>(memory::HandleOwn(address, &kAppStatesLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppStatesLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppStatesLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = AppStatesVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  uint32_t app_address() const { return address + 4; }
  uint32_t app_bits() const { if (auto* d = direct()) return d->app; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_app_bits(uint32_t v) const { if (auto* d = direct()) { d->app = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = App>
  T app() const { return T(app_bits()); }
  void set_app(const View& w) const { set_app_bits(w.address); }
  void set_app(uint32_t v) const { set_app_bits(v); }
  // the root layer (no states; one child in play); sub_820AD8D0.ret's read is AppStates::OnMessage's r3, the stack (a measure fault)
  uint32_t root_address() const { return address + 8; }
  uint32_t root_bits() const { if (auto* d = direct()) return d->root; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_root_bits(uint32_t v) const { if (auto* d = direct()) { d->root = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  template <typename T = StateLayer>
  T root() const { return T(root_bits()); }
  void set_root(const View& w) const { set_root_bits(w.address); }
  void set_root(uint32_t v) const { set_root_bits(v); }
};

// image; measure: App.+96.+0
struct AppStatesVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // sub_820AE2F8, never ran
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // AppStates::AddLayer
  uint32_t add_layer() const { return guest::LoadU32(address + 4); }
  void set_add_layer(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t add_layer_address() const { return address + 4; }
  // sub_820AE198, never ran
  uint32_t count_states() const { return guest::LoadU32(address + 8); }
  void set_count_states(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t count_states_address() const { return address + 8; }
  // AppStates::IsEmpty
  uint32_t is_empty() const { return guest::LoadU32(address + 12); }
  void set_is_empty(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t is_empty_address() const { return address + 12; }
};

// embedded; measure: AppStates::AppStates.r4
struct AppStatesSetup : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = App>
  T app() const { return T(guest::LoadU32(address + 0)); }
  void set_app(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_app(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t app_address() const { return address + 0; }
};

// allocated; measure: StateLayer, AppStates.+8, sub_820ADBD8.r3, sub_820AD350.ret, sub_820AD5B0.r3, sub_820AD8D0.r3, sub_820ADA70.r3, sub_820ADB28.r3, sub_820ADB80.r3, sub_820ACD60.r4, sub_820ACD80.r4, sub_820ACDE8.r4, StateLayer::AddChild.r4
#pragma pack(push, 1)
struct StateLayerData {
  uint32_t vtable = 0;  // +0
  uint32_t state_vtable = 0;  // +4
  uint32_t owner = 0;  // +8
  uint32_t children = 0;  // +12
  uint32_t children_head = 0;  // +16
  uint32_t children_size = 0;  // +20
  uint32_t states = 0;  // +24
  uint32_t states_head = 0;  // +28
  uint32_t states_size = 0;  // +32
  uint32_t ops = 0;  // +36
  uint32_t ops_head = 0;  // +40
  uint32_t ops_size = 0;  // +44
};
#pragma pack(pop)
static_assert(sizeof(StateLayerData) == 48);
inline constexpr memory::LayoutField kStateLayerFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}};
inline constexpr memory::Layout kStateLayerLayout = {kStateLayerFields, 12};

struct StateLayer : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x820028E0;
  StateLayerData* data() const { return static_cast<StateLayerData*>(memory::HandleObject(address)); }
  static StateLayer New() {
    return StateLayer(memory::MakeHandle(new StateLayerData(), sizeof(StateLayerData), "StateLayer", &kStateLayerLayout));
  }
  template <uint32_t Stride = kSize>
  static StateLayer NewArray(uint32_t count) {
    static_assert(sizeof(StateLayerData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<StateLayerData*>(::operator new(sizeof(StateLayerData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) StateLayerData();
    return StateLayer(memory::MakeHandle(p, sizeof(StateLayerData) * count, "StateLayer", &kStateLayerLayout, sizeof(StateLayerData)));
  }
  StateLayer at(uint32_t i) const { return StateLayer(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  StateLayerData* direct() const { return static_cast<StateLayerData*>(memory::HandleOwn(address, &kStateLayerLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kStateLayerLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kStateLayerLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the child layers
  template <typename T = PtrList>
  T children_list() const { return T(address + 12); }
  // the states, the top at the front
  template <typename T = PtrList>
  T states_list() const { return T(address + 24); }
  // the queued operations
  template <typename T = OpList>
  T ops_list() const { return T(address + 36); }
  // 0x820028E0
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // 0x820028B0 (the state base's 0x82002880 first): the layer + 4 is a state
  uint32_t state_vtable_address() const { return address + 4; }
  uint32_t state_vtable_bits() const { if (auto* d = direct()) return d->state_vtable; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_state_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->state_vtable = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = AppStateVtable>
  T state_vtable() const { return T(state_vtable_bits()); }
  void set_state_vtable(const View& w) const { set_state_vtable_bits(w.address); }
  void set_state_vtable(uint32_t v) const { set_state_vtable_bits(v); }
  // the app's interface at app + 16 (the app's slot 2)
  uint32_t owner_address() const { return address + 8; }
  uint32_t owner_bits() const { if (auto* d = direct()) return d->owner; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_owner_bits(uint32_t v) const { if (auto* d = direct()) { d->owner = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t owner() const { if (auto* d = direct()) return d->owner; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_owner(uint32_t v) const { if (auto* d = direct()) { d->owner = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // the child layers (a PtrList): its first word, not read
  uint32_t children_address() const { return address + 12; }
  uint32_t children_bits() const { if (auto* d = direct()) return d->children; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_children_bits(uint32_t v) const { if (auto* d = direct()) { d->children = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t children() const { if (auto* d = direct()) return d->children; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_children(uint32_t v) const { if (auto* d = direct()) { d->children = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t children_head_address() const { return address + 16; }
  uint32_t children_head_bits() const { if (auto* d = direct()) return d->children_head; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_children_head_bits(uint32_t v) const { if (auto* d = direct()) { d->children_head = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  template <typename T = StateListNode>
  T children_head() const { return T(children_head_bits()); }
  void set_children_head(const View& w) const { set_children_head_bits(w.address); }
  void set_children_head(uint32_t v) const { set_children_head_bits(v); }
  uint32_t children_size_address() const { return address + 20; }
  uint32_t children_size_bits() const { if (auto* d = direct()) return d->children_size; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_children_size_bits(uint32_t v) const { if (auto* d = direct()) { d->children_size = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t children_size() const { if (auto* d = direct()) return d->children_size; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_children_size(uint32_t v) const { if (auto* d = direct()) { d->children_size = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the states (a PtrList, the top at the front): its first word, not read
  uint32_t states_address() const { return address + 24; }
  uint32_t states_bits() const { if (auto* d = direct()) return d->states; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_states_bits(uint32_t v) const { if (auto* d = direct()) { d->states = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t states() const { if (auto* d = direct()) return d->states; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_states(uint32_t v) const { if (auto* d = direct()) { d->states = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t states_head_address() const { return address + 28; }
  uint32_t states_head_bits() const { if (auto* d = direct()) return d->states_head; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_states_head_bits(uint32_t v) const { if (auto* d = direct()) { d->states_head = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  template <typename T = StateListNode>
  T states_head() const { return T(states_head_bits()); }
  void set_states_head(const View& w) const { set_states_head_bits(w.address); }
  void set_states_head(uint32_t v) const { set_states_head_bits(v); }
  uint32_t states_size_address() const { return address + 32; }
  uint32_t states_size_bits() const { if (auto* d = direct()) return d->states_size; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_states_size_bits(uint32_t v) const { if (auto* d = direct()) { d->states_size = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t states_size() const { if (auto* d = direct()) return d->states_size; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_states_size(uint32_t v) const { if (auto* d = direct()) { d->states_size = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // the queued operations (an OpList): its first word, not read
  uint32_t ops_address() const { return address + 36; }
  uint32_t ops_bits() const { if (auto* d = direct()) return d->ops; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_ops_bits(uint32_t v) const { if (auto* d = direct()) { d->ops = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t ops() const { if (auto* d = direct()) return d->ops; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_ops(uint32_t v) const { if (auto* d = direct()) { d->ops = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t ops_head_address() const { return address + 40; }
  uint32_t ops_head_bits() const { if (auto* d = direct()) return d->ops_head; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_ops_head_bits(uint32_t v) const { if (auto* d = direct()) { d->ops_head = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = AppOpListNode>
  T ops_head() const { return T(ops_head_bits()); }
  void set_ops_head(const View& w) const { set_ops_head_bits(w.address); }
  void set_ops_head(uint32_t v) const { set_ops_head_bits(v); }
  uint32_t ops_size_address() const { return address + 44; }
  uint32_t ops_size_bits() const { if (auto* d = direct()) return d->ops_size; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_ops_size_bits(uint32_t v) const { if (auto* d = direct()) { d->ops_size = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t ops_size() const { if (auto* d = direct()) return d->ops_size; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_ops_size(uint32_t v) const { if (auto* d = direct()) { d->ops_size = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
};

// allocated; measure: StateLayer.+28.+0.+8, sub_820AD5B0.r3.+28.+0.+8
struct AppState : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AppStateVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: StateLayer.+28.+0.+8.+0, sub_820AD5B0.r3.+28.+0.+8.+0, sub_820ACD60.r4.+4, sub_820ACD80.r4.+4, sub_820ACDE8.r4.+4, StateLayer::AddChild.r4.+4, AppStates.+8.+4
struct AppStateVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // sub_820ACCF0 in the base
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  uint32_t on_enter() const { return guest::LoadU32(address + 4); }
  void set_on_enter(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t on_enter_address() const { return address + 4; }
  uint32_t on_exit() const { return guest::LoadU32(address + 8); }
  void set_on_exit(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t on_exit_address() const { return address + 8; }
  // message 0
  uint32_t on_toggle_on() const { return guest::LoadU32(address + 12); }
  void set_on_toggle_on(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t on_toggle_on_address() const { return address + 12; }
  // message 1
  uint32_t on_step() const { return guest::LoadU32(address + 16); }
  void set_on_step(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t on_step_address() const { return address + 16; }
  // message 2
  uint32_t on_toggle_off() const { return guest::LoadU32(address + 20); }
  void set_on_toggle_off(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t on_toggle_off_address() const { return address + 20; }
  uint32_t on_covered() const { return guest::LoadU32(address + 24); }
  void set_on_covered(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t on_covered_address() const { return address + 24; }
  uint32_t on_uncovered() const { return guest::LoadU32(address + 28); }
  void set_on_uncovered(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t on_uncovered_address() const { return address + 28; }
  uint32_t update() const { return guest::LoadU32(address + 32); }
  void set_update(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t update_address() const { return address + 32; }
  uint32_t update_toggled() const { return guest::LoadU32(address + 36); }
  void set_update_toggled(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t update_toggled_address() const { return address + 36; }
  uint32_t tick() const { return guest::LoadU32(address + 40); }
  void set_tick(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t tick_address() const { return address + 40; }
  // sub_825C8390 (1) in the base
  uint32_t is_empty() const { return guest::LoadU32(address + 44); }
  void set_is_empty(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t is_empty_address() const { return address + 44; }
};

// embedded; measure: PtrList, PtrList::Erase.r4, PtrList::PtrList.r3
#pragma pack(push, 1)
struct PtrListData {
  uint8_t pad_0[4] = {};
  uint32_t head = 0;  // +4
  uint32_t size = 0;  // +8
  uint8_t pad_12[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(PtrListData) == 16);
inline constexpr memory::LayoutField kPtrListFields[] = {{4, 4}, {8, 4}};
inline constexpr memory::Layout kPtrListLayout = {kPtrListFields, 2};

struct PtrList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  PtrListData* data() const { return static_cast<PtrListData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static PtrList New() {
    return PtrList(memory::MakeHandle(new PtrListData(), sizeof(PtrListData), "PtrList", &kPtrListLayout));
  }
  template <uint32_t Stride = kSize>
  static PtrList NewArray(uint32_t count) {
    static_assert(sizeof(PtrListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PtrListData*>(::operator new(sizeof(PtrListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PtrListData();
    return PtrList(memory::MakeHandle(p, sizeof(PtrListData) * count, "PtrList", &kPtrListLayout, sizeof(PtrListData)));
  }
  PtrList at(uint32_t i) const { return PtrList(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  PtrListData* direct() const { return static_cast<PtrListData*>(memory::HandleOwn(address, &kPtrListLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kPtrListLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPtrListLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // measure fault: PtrList::ForEach's and Erase's r3 is the result (functor or iterator), not the list; HandleList::Remove's and sub_82118850's reads are an iterator's
  uint32_t fault_result_0_address() const { return address + 0; }
  uint64_t fault_result_0_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kPtrListLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(address + 0))); }
  void set_fault_result_0_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPtrListLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(address + 0, v); }
  uint64_t fault_result_0() const { return uint64_t(fault_result_0_bits()); }
  void set_fault_result_0(uint64_t v) const { set_fault_result_0_bits(uint64_t(v)); }
  // the head node (new<?>@PtrList::AllocateNodes, input's)
  uint32_t head_address() const { return address + 4; }
  uint32_t head_bits() const { if (auto* d = direct()) return d->head; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_head_bits(uint32_t v) const { if (auto* d = direct()) { d->head = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = StateListNode>
  T head() const { return T(head_bits()); }
  void set_head(const View& w) const { set_head_bits(w.address); }
  void set_head(uint32_t v) const { set_head_bits(v); }
  uint32_t size_address() const { return address + 8; }
  uint32_t size_bits() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_size_bits(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t size() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_size(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // measure fault: as fault_result_0
  uint32_t fault_result_8_address() const { return address + 8; }
  uint64_t fault_result_8_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kPtrListLayout, 8, 8)); return uint64_t((memory::IsHandle(address + 8) ? uint64_t(memory::OwnerLoad(address + 8, 8)) : guest::LoadU64(address + 8))); }
  void set_fault_result_8_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPtrListLayout, 8, 8, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 8, v); else guest::StoreU64(address + 8, v); }
  uint64_t fault_result_8() const { return uint64_t(fault_result_8_bits()); }
  void set_fault_result_8(uint64_t v) const { set_fault_result_8_bits(uint64_t(v)); }
};

// allocated; measure: StateLayer.+16, StateLayer.+28, StateLayer.+28.+0, sub_820AD5B0.r3.+16, sub_820AD5B0.r3.+28, sub_820AD5B0.r3.+28.+0, sub_820AD8D0.r3.+16, sub_820AD8D0.r3.+28, List::Clear_820ACF88.r3.+4, List::Clear_820ACF88.r3.+4.+0
struct StateListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = StateListNode>
  T next() const { return T(guest::LoadU32(address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = StateListNode>
  T previous() const { return T(guest::LoadU32(address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // a child layer (its +4 a state: AppState) or a state
  uint32_t value() const { return guest::LoadU32(address + 8); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t value_address() const { return address + 8; }
};

// embedded; measure: OpList, OpList::IncSize.r3
#pragma pack(push, 1)
struct OpListData {
  uint8_t pad_0[4] = {};
  uint32_t head = 0;  // +4
  uint32_t size = 0;  // +8
  uint8_t pad_12[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(OpListData) == 16);
inline constexpr memory::LayoutField kOpListFields[] = {{4, 4}, {8, 4}};
inline constexpr memory::Layout kOpListLayout = {kOpListFields, 2};

struct OpList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  OpListData* data() const { return static_cast<OpListData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static OpList New() {
    return OpList(memory::MakeHandle(new OpListData(), sizeof(OpListData), "OpList", &kOpListLayout));
  }
  template <uint32_t Stride = kSize>
  static OpList NewArray(uint32_t count) {
    static_assert(sizeof(OpListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<OpListData*>(::operator new(sizeof(OpListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) OpListData();
    return OpList(memory::MakeHandle(p, sizeof(OpListData) * count, "OpList", &kOpListLayout, sizeof(OpListData)));
  }
  OpList at(uint32_t i) const { return OpList(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  OpListData* direct() const { return static_cast<OpListData*>(memory::HandleOwn(address, &kOpListLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kOpListLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kOpListLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // measure fault: OpList::ForEach's r3 is the result functor, not the list
  uint32_t fault_result_0_address() const { return address + 0; }
  uint64_t fault_result_0_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kOpListLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(address + 0))); }
  void set_fault_result_0_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kOpListLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(address + 0, v); }
  uint64_t fault_result_0() const { return uint64_t(fault_result_0_bits()); }
  void set_fault_result_0(uint64_t v) const { set_fault_result_0_bits(uint64_t(v)); }
  uint32_t head_address() const { return address + 4; }
  uint32_t head_bits() const { if (auto* d = direct()) return d->head; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_head_bits(uint32_t v) const { if (auto* d = direct()) { d->head = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = AppOpListNode>
  T head() const { return T(head_bits()); }
  void set_head(const View& w) const { set_head_bits(w.address); }
  void set_head(uint32_t v) const { set_head_bits(v); }
  uint32_t size_address() const { return address + 8; }
  uint32_t size_bits() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_size_bits(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t size() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_size(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // measure fault: as fault_result_0
  uint32_t fault_result_8_address() const { return address + 8; }
  uint64_t fault_result_8_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kOpListLayout, 8, 8)); return uint64_t((memory::IsHandle(address + 8) ? uint64_t(memory::OwnerLoad(address + 8, 8)) : guest::LoadU64(address + 8))); }
  void set_fault_result_8_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kOpListLayout, 8, 8, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 8, v); else guest::StoreU64(address + 8, v); }
  uint64_t fault_result_8() const { return uint64_t(fault_result_8_bits()); }
  void set_fault_result_8(uint64_t v) const { set_fault_result_8_bits(uint64_t(v)); }
};

// allocated; measure: OpList::BuyNode.ret, OpList::BuyNode.ret.+4, StateLayer.+40, sub_820ADA70.r3.+40, sub_820ADB28.r3.+40, sub_820ADB80.r3.+40, new<?>@OpList::AllocateNodes
#pragma pack(push, 1)
struct AppOpListNodeData {
  uint32_t next = 0;  // +0
  uint32_t previous = 0;  // +4
  uint32_t op_type = 0;  // +8
  uint32_t op_state = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(AppOpListNodeData) == 16);
inline constexpr memory::LayoutField kAppOpListNodeFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kAppOpListNodeLayout = {kAppOpListNodeFields, 4};

struct AppOpListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  AppOpListNodeData* data() const { return static_cast<AppOpListNodeData*>(memory::HandleObject(address)); }
  static AppOpListNode New() {
    return AppOpListNode(memory::MakeHandle(new AppOpListNodeData(), sizeof(AppOpListNodeData), "AppOpListNode", &kAppOpListNodeLayout));
  }
  template <uint32_t Stride = kSize>
  static AppOpListNode NewArray(uint32_t count) {
    static_assert(sizeof(AppOpListNodeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppOpListNodeData*>(::operator new(sizeof(AppOpListNodeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppOpListNodeData();
    return AppOpListNode(memory::MakeHandle(p, sizeof(AppOpListNodeData) * count, "AppOpListNode", &kAppOpListNodeLayout, sizeof(AppOpListNodeData)));
  }
  AppOpListNode at(uint32_t i) const { return AppOpListNode(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppOpListNodeData* direct() const { return static_cast<AppOpListNodeData*>(memory::HandleOwn(address, &kAppOpListNodeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppOpListNodeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppOpListNodeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t next_address() const { return address + 0; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = AppOpListNode>
  T next() const { return T(next_bits()); }
  void set_next(const View& w) const { set_next_bits(w.address); }
  void set_next(uint32_t v) const { set_next_bits(v); }
  uint32_t previous_address() const { return address + 4; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = AppOpListNode>
  T previous() const { return T(previous_bits()); }
  void set_previous(const View& w) const { set_previous_bits(w.address); }
  void set_previous(uint32_t v) const { set_previous_bits(v); }
  // 0 add a child layer, 1 pop, 2 push, 3 replace
  uint32_t op_type_address() const { return address + 8; }
  uint32_t op_type_bits() const { if (auto* d = direct()) return d->op_type; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_op_type_bits(uint32_t v) const { if (auto* d = direct()) { d->op_type = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t op_type() const { if (auto* d = direct()) return d->op_type; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_op_type(uint32_t v) const { if (auto* d = direct()) { d->op_type = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t op_state_address() const { return address + 12; }
  uint32_t op_state_bits() const { if (auto* d = direct()) return d->op_state; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_op_state_bits(uint32_t v) const { if (auto* d = direct()) { d->op_state = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t op_state() const { if (auto* d = direct()) return d->op_state; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_op_state(uint32_t v) const { if (auto* d = direct()) { d->op_state = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the operation: its type (high word), then a state (in host memory the two words are the members: read them, not this doubleword)
  uint32_t op_address() const { return address + 8; }
  uint64_t op_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kAppOpListNodeLayout, 8, 8)); return uint64_t((memory::IsHandle(address + 8) ? uint64_t(memory::OwnerLoad(address + 8, 8)) : guest::LoadU64(address + 8))); }
  void set_op_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppOpListNodeLayout, 8, 8, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 8, v); else guest::StoreU64(address + 8, v); }
  uint64_t op() const { return uint64_t(op_bits()); }
  void set_op(uint64_t v) const { set_op_bits(uint64_t(v)); }
};

// embedded; measure: OpList::BuyNode.r6
struct AppStateOp : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 0 add a child layer, 1 pop, 2 push, 3 replace
  uint32_t type() const { return guest::LoadU32(address + 0); }
  void set_type(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  uint32_t state() const { return guest::LoadU32(address + 4); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t state_address() const { return address + 4; }
};

// embedded; measure: sub_820AD808.r3
struct AppMemberFunctor : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the function (high word) and the object
  uint64_t function_object() const { return guest::LoadU64(address + 0); }
  void set_function_object(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t function_object_address() const { return address + 0; }
  // the adjustment (high word)
  uint64_t adjust() const { return guest::LoadU64(address + 8); }
  void set_adjust(uint64_t v) const { guest::StoreU64(address + 8, v); }
  uint32_t adjust_address() const { return address + 8; }
};

// allocated; measure: sub_82098F30.r3, sub_82098FF8.ret@+792
struct InputManagerAtTeardown : View {
  using View::View;
  static constexpr uint32_t kSize = 3976;
  // 0x82000F48, then the base's 0x82000A38
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the last of its four lists' head (the lists from +3932, 12 bytes each), freed and cleared
  uint32_t list_head() const { return guest::LoadU32(address + 3972); }
  void set_list_head(uint32_t v) const { guest::StoreU32(address + 3972, v); }
  uint32_t list_head_address() const { return address + 3972; }
};

// allocated; measure: sub_8258CEA0.r3, sub_8258CF40.r3, sub_82592178.r3
struct InputProcessorAtTeardown : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x8206C0C4, then the base's 0x8206CC00
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// allocated; measure: sub_82591BB8.r3
struct PhysicalPadAtTeardown : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x8206CBF4, then 0x8206CBE4
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// embedded; measure: App::SceneFromInterface16.r3@-8
#pragma pack(push, 1)
struct AppOuter8Data {
  uint8_t pad_0[16] = {};
  uint32_t unknown_16 = 0;  // +16
};
#pragma pack(pop)
static_assert(sizeof(AppOuter8Data) == 20);
inline constexpr memory::LayoutField kAppOuter8Fields[] = {{16, 4}};
inline constexpr memory::Layout kAppOuter8Layout = {kAppOuter8Fields, 1};

struct AppOuter8 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppOuter8Data* data() const { return static_cast<AppOuter8Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppOuter8 New() {
    return AppOuter8(memory::MakeHandle(new AppOuter8Data(), sizeof(AppOuter8Data), "AppOuter8", &kAppOuter8Layout));
  }
  template <uint32_t Stride = kSize>
  static AppOuter8 NewArray(uint32_t count) {
    static_assert(sizeof(AppOuter8Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppOuter8Data*>(::operator new(sizeof(AppOuter8Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppOuter8Data();
    return AppOuter8(memory::MakeHandle(p, sizeof(AppOuter8Data) * count, "AppOuter8", &kAppOuter8Layout, sizeof(AppOuter8Data)));
  }
  AppOuter8 at(uint32_t i) const { return AppOuter8(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppOuter8Data* direct() const { return static_cast<AppOuter8Data*>(memory::HandleOwn(address, &kAppOuter8Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppOuter8Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppOuter8Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_16_bits() const { if (auto* d = direct()) return d->unknown_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_unknown_16_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t unknown_16() const { if (auto* d = direct()) return d->unknown_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_unknown_16(uint32_t v) const { if (auto* d = direct()) { d->unknown_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
};

// embedded; measure: App::SendMessage.r3@+56
#pragma pack(push, 1)
struct MessageVectorAt56Data {
  uint8_t pad_0[4] = {};
  uint32_t unknown_4 = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
  uint32_t unknown_12 = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(MessageVectorAt56Data) == 16);
inline constexpr memory::LayoutField kMessageVectorAt56Fields[] = {{4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kMessageVectorAt56Layout = {kMessageVectorAt56Fields, 3};

struct MessageVectorAt56 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  MessageVectorAt56Data* data() const { return static_cast<MessageVectorAt56Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static MessageVectorAt56 New() {
    return MessageVectorAt56(memory::MakeHandle(new MessageVectorAt56Data(), sizeof(MessageVectorAt56Data), "MessageVectorAt56", &kMessageVectorAt56Layout));
  }
  template <uint32_t Stride = kSize>
  static MessageVectorAt56 NewArray(uint32_t count) {
    static_assert(sizeof(MessageVectorAt56Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MessageVectorAt56Data*>(::operator new(sizeof(MessageVectorAt56Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MessageVectorAt56Data();
    return MessageVectorAt56(memory::MakeHandle(p, sizeof(MessageVectorAt56Data) * count, "MessageVectorAt56", &kMessageVectorAt56Layout, sizeof(MessageVectorAt56Data)));
  }
  MessageVectorAt56 at(uint32_t i) const { return MessageVectorAt56(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MessageVectorAt56Data* direct() const { return static_cast<MessageVectorAt56Data*>(memory::HandleOwn(address, &kMessageVectorAt56Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMessageVectorAt56Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMessageVectorAt56Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4_bits() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_4() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8_bits() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t unknown_8() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_12_bits() const { if (auto* d = direct()) return d->unknown_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_unknown_12_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t unknown_12() const { if (auto* d = direct()) return d->unknown_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_unknown_12(uint32_t v) const { if (auto* d = direct()) { d->unknown_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
};

// embedded; measure: App::SendMessage.r3@+56.+8@+4
struct AppMessageWordAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: App@+76
#pragma pack(push, 1)
struct MessageVectorAt76Data {
  uint8_t pad_0[4] = {};
  uint32_t unknown_4 = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(MessageVectorAt76Data) == 12);
inline constexpr memory::LayoutField kMessageVectorAt76Fields[] = {{4, 4}, {8, 4}};
inline constexpr memory::Layout kMessageVectorAt76Layout = {kMessageVectorAt76Fields, 2};

struct MessageVectorAt76 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  MessageVectorAt76Data* data() const { return static_cast<MessageVectorAt76Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static MessageVectorAt76 New() {
    return MessageVectorAt76(memory::MakeHandle(new MessageVectorAt76Data(), sizeof(MessageVectorAt76Data), "MessageVectorAt76", &kMessageVectorAt76Layout));
  }
  template <uint32_t Stride = kSize>
  static MessageVectorAt76 NewArray(uint32_t count) {
    static_assert(sizeof(MessageVectorAt76Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MessageVectorAt76Data*>(::operator new(sizeof(MessageVectorAt76Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MessageVectorAt76Data();
    return MessageVectorAt76(memory::MakeHandle(p, sizeof(MessageVectorAt76Data) * count, "MessageVectorAt76", &kMessageVectorAt76Layout, sizeof(MessageVectorAt76Data)));
  }
  MessageVectorAt76 at(uint32_t i) const { return MessageVectorAt76(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MessageVectorAt76Data* direct() const { return static_cast<MessageVectorAt76Data*>(memory::HandleOwn(address, &kMessageVectorAt76Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMessageVectorAt76Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMessageVectorAt76Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4_bits() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_4() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8_bits() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t unknown_8() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
};

// embedded; measure: StateLayer@+36
struct OpListAt36 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

}  // namespace schema

#include "schema/framework.h"
