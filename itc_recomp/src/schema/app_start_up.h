// The views of docs/functions/app-start-up.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/app-start-up.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: sub_82379338.r3, sub_82379350.r3, sub_820943B0.r3, sub_820943C8.r3, [826BDC90]
#pragma pack(push, 1)
struct AppInterface16Data {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[4] = {};
  uint32_t framework_interface = 0;  // +8
  uint8_t pad_12[96] = {};
  uint32_t sound_manager = 0;  // +108
  uint32_t movie = 0;  // +112
};
#pragma pack(pop)
static_assert(sizeof(AppInterface16Data) == 116);
inline constexpr memory::LayoutField kAppInterface16Fields[] = {{0, 4}, {8, 4}, {108, 4}, {112, 4}};
inline constexpr memory::Layout kAppInterface16Layout = {kAppInterface16Fields, 4};

struct AppInterface16 : View {
  using View::View;
  static constexpr uint32_t kSize = 116;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppInterface16Data* data() const { return static_cast<AppInterface16Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppInterface16 New(uint8_t* base) {
    return AppInterface16(base, memory::MakeHandle(new AppInterface16Data(), sizeof(AppInterface16Data), "AppInterface16", &kAppInterface16Layout));
  }
  template <uint32_t Stride = kSize>
  static AppInterface16 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AppInterface16Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppInterface16Data*>(::operator new(sizeof(AppInterface16Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppInterface16Data();
    return AppInterface16(base, memory::MakeHandle(p, sizeof(AppInterface16Data) * count, "AppInterface16", &kAppInterface16Layout, sizeof(AppInterface16Data)));
  }
  AppInterface16 at(uint32_t i) const { return AppInterface16(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppInterface16Data* direct() const { return static_cast<AppInterface16Data*>(memory::HandleOwn(address, &kAppInterface16Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppInterface16Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppInterface16Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82000E38
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // app + 124: the sound manager (AudioManager::Create, sound-cues.md), slot +48's answer
  uint32_t sound_manager_address() const { return address + 108; }
  uint32_t sound_manager_bits() const { if (auto* d = direct()) return d->sound_manager; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(base, address + 108))); }
  void set_sound_manager_bits(uint32_t v) const { if (auto* d = direct()) { d->sound_manager = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(base, address + 108, v); }
  uint32_t sound_manager() const { if (auto* d = direct()) return d->sound_manager; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(base, address + 108))); }
  void set_sound_manager(uint32_t v) const { if (auto* d = direct()) { d->sound_manager = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(base, address + 108, v); }
  // app + 128: the movie class (Movie::Movie, movies.md), slot +44's answer
  uint32_t movie_address() const { return address + 112; }
  uint32_t movie_bits() const { if (auto* d = direct()) return d->movie; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(base, address + 112))); }
  void set_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->movie = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(base, address + 112, v); }
  uint32_t movie() const { if (auto* d = direct()) return d->movie; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(base, address + 112))); }
  void set_movie(uint32_t v) const { if (auto* d = direct()) { d->movie = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(base, address + 112, v); }
  // app + 24: the framework's interface, read by the vtable's +8 users getter sub_820943C8 and +12 save-devices getter sub_820943B0 (save-interface's and system-services' reports)
  uint32_t framework_interface_address() const { return address + 8; }
  uint32_t framework_interface_bits() const { if (auto* d = direct()) return d->framework_interface; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_framework_interface_bits(uint32_t v) const { if (auto* d = direct()) { d->framework_interface = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  template <typename T = AppFrameworkInterface>
  T framework_interface() const { return T(base, framework_interface_bits()); }
  void set_framework_interface(const View& w) const { set_framework_interface_bits(w.address); }
  void set_framework_interface(uint32_t v) const { set_framework_interface_bits(v); }
};

// embedded; measure: sub_82094368.r3
#pragma pack(push, 1)
struct AppInterface8Data {
  uint8_t pad_0[16] = {};
  uint32_t framework_interface = 0;  // +16
};
#pragma pack(pop)
static_assert(sizeof(AppInterface8Data) == 20);
inline constexpr memory::LayoutField kAppInterface8Fields[] = {{16, 4}};
inline constexpr memory::Layout kAppInterface8Layout = {kAppInterface8Fields, 1};

struct AppInterface8 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppInterface8Data* data() const { return static_cast<AppInterface8Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppInterface8 New(uint8_t* base) {
    return AppInterface8(base, memory::MakeHandle(new AppInterface8Data(), sizeof(AppInterface8Data), "AppInterface8", &kAppInterface8Layout));
  }
  template <uint32_t Stride = kSize>
  static AppInterface8 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AppInterface8Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppInterface8Data*>(::operator new(sizeof(AppInterface8Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppInterface8Data();
    return AppInterface8(base, memory::MakeHandle(p, sizeof(AppInterface8Data) * count, "AppInterface8", &kAppInterface8Layout, sizeof(AppInterface8Data)));
  }
  AppInterface8 at(uint32_t i) const { return AppInterface8(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppInterface8Data* direct() const { return static_cast<AppInterface8Data*>(memory::HandleOwn(address, &kAppInterface8Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppInterface8Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppInterface8Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // app + 24: the framework's +36 interface
  uint32_t framework_interface_address() const { return address + 16; }
  uint32_t framework_interface_bits() const { if (auto* d = direct()) return d->framework_interface; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_framework_interface_bits(uint32_t v) const { if (auto* d = direct()) { d->framework_interface = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  template <typename T = AppFrameworkInterface>
  T framework_interface() const { return T(base, framework_interface_bits()); }
  void set_framework_interface(const View& w) const { set_framework_interface_bits(w.address); }
  void set_framework_interface(uint32_t v) const { set_framework_interface_bits(v); }
};

// embedded; measure: sub_82094368.r3.+16, sub_820943B0.r3.+8, sub_820943C8.r3.+8, App::SceneFromInterface16.r3@-8.+16
#pragma pack(push, 1)
struct AppFrameworkInterfaceData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(AppFrameworkInterfaceData) == 4);
inline constexpr memory::LayoutField kAppFrameworkInterfaceFields[] = {{0, 4}};
inline constexpr memory::Layout kAppFrameworkInterfaceLayout = {kAppFrameworkInterfaceFields, 1};

struct AppFrameworkInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppFrameworkInterfaceData* data() const { return static_cast<AppFrameworkInterfaceData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppFrameworkInterface New(uint8_t* base) {
    return AppFrameworkInterface(base, memory::MakeHandle(new AppFrameworkInterfaceData(), sizeof(AppFrameworkInterfaceData), "AppFrameworkInterface", &kAppFrameworkInterfaceLayout));
  }
  template <uint32_t Stride = kSize>
  static AppFrameworkInterface NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AppFrameworkInterfaceData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppFrameworkInterfaceData*>(::operator new(sizeof(AppFrameworkInterfaceData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppFrameworkInterfaceData();
    return AppFrameworkInterface(base, memory::MakeHandle(p, sizeof(AppFrameworkInterfaceData) * count, "AppFrameworkInterface", &kAppFrameworkInterfaceLayout, sizeof(AppFrameworkInterfaceData)));
  }
  AppFrameworkInterface at(uint32_t i) const { return AppFrameworkInterface(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AppFrameworkInterfaceData* direct() const { return static_cast<AppFrameworkInterfaceData*>(memory::HandleOwn(address, &kAppFrameworkInterfaceLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAppFrameworkInterfaceLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAppFrameworkInterfaceLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82000AFC
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  template <typename T = AppFrameworkInterfaceVtable>
  T vtable() const { return T(base, vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
};

// image; measure: sub_82094368.r3.+16.+0, sub_820943B0.r3.+8.+0, sub_820943C8.r3.+8.+0, App::SceneFromInterface16.r3@-8.+16.+0, App::FrameDriver.r3.+20.+0
struct AppFrameworkInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // sub_820908E0: the frame driver (framework + 76)
  uint32_t slot_2_frame_driver() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2_frame_driver(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_frame_driver_address() const { return address + 8; }
  // sub_82090908: the scene (framework + 80)
  uint32_t slot_3_scene() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3_scene(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_scene_address() const { return address + 12; }
  // sub_820908E8: the first worker pool (framework + 68)
  uint32_t slot_4_worker_pool() const { return guest::LoadU32(base, address + 16); }
  void set_slot_4_worker_pool(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot_4_worker_pool_address() const { return address + 16; }
  // sub_82493700: the users (sub_820943C8 calls it)
  uint32_t slot_9_users() const { return guest::LoadU32(base, address + 36); }
  void set_slot_9_users(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t slot_9_users_address() const { return address + 36; }
  // sub_82090788: the save devices (sub_820943B0 calls it)
  uint32_t slot_10_save_devices() const { return guest::LoadU32(base, address + 40); }
  void set_slot_10_save_devices(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t slot_10_save_devices_address() const { return address + 40; }
};

// embedded; measure: App::App.r4
struct AppSetUpBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = AppRunLoop>
  T run_loop() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_run_loop(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_run_loop(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t run_loop_address() const { return address + 0; }
  // kept at the app's +24
  template <typename T = AppFrameworkInterface>
  T framework_interface() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_framework_interface(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_interface_address() const { return address + 4; }
};

// embedded; measure: App::App.r4.+0, App::OnRunLoopEvent.r4
struct AppRunLoop : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  static constexpr uint32_t kVtable = 0x82000FE8;
  template <typename T = AppRunLoopVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a std::map of delegates by key (+8 its head node, +12 its size)
  uint32_t tick_handlers_address() const { return address + 4; }
  // a std::map of delegates by key
  uint32_t listeners_address() const { return address + 16; }
};

// image; measure: App::OnRunLoopEvent.r4.+0
struct AppRunLoopVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t slot_1_add_listener() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1_add_listener(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_add_listener_address() const { return address + 4; }
  uint32_t slot_2_add_tick_handler() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2_add_tick_handler(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_add_tick_handler_address() const { return address + 8; }
  uint32_t slot_3_remove_listener() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3_remove_listener(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_remove_listener_address() const { return address + 12; }
  uint32_t slot_4_remove_tick_handler() const { return guest::LoadU32(base, address + 16); }
  void set_slot_4_remove_tick_handler(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot_4_remove_tick_handler_address() const { return address + 16; }
};

// embedded; measure: App::OnRunLoopEvent.r5
struct AppRunLoopEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0: App::DeliverMessage(5); 1: message 6 and the app's handlers removed
  uint32_t code() const { return guest::LoadU32(base, address + 0); }
  void set_code(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t code_address() const { return address + 0; }
};

// image; measure: Tiling, [8260034C]
struct Tiling : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // 1280
  uint32_t width() const { return guest::LoadU32(base, address + 0); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t width_address() const { return address + 0; }
  // 720
  uint32_t height() const { return guest::LoadU32(base, address + 4); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t height_address() const { return address + 4; }
  // 2
  uint32_t multisample_type() const { return guest::LoadU32(base, address + 8); }
  void set_multisample_type(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t multisample_type_address() const { return address + 8; }
  // 3
  uint32_t tile_count() const { return guest::LoadU32(base, address + 12); }
  void set_tile_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t tile_count_address() const { return address + 12; }
  // TilingTile x tile_count, 16 bytes each
  uint32_t tiles_address() const { return address + 16; }
};

// embedded; measure: -
struct TilingTile : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t left() const { return guest::LoadU32(base, address + 0); }
  void set_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t left_address() const { return address + 0; }
  uint32_t top() const { return guest::LoadU32(base, address + 4); }
  void set_top(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t top_address() const { return address + 4; }
  uint32_t right() const { return guest::LoadU32(base, address + 8); }
  void set_right(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t right_address() const { return address + 8; }
  uint32_t bottom() const { return guest::LoadU32(base, address + 12); }
  void set_bottom(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t bottom_address() const { return address + 12; }
};

// embedded; measure: Tiling::LargestTile.r4
struct TilingTileSize : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the largest right - left
  uint32_t width() const { return guest::LoadU32(base, address + 0); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t width_address() const { return address + 0; }
  // the largest bottom - top
  uint32_t height() const { return guest::LoadU32(base, address + 4); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t height_address() const { return address + 4; }
};

// image; measure: CreateVertexDeclaration.r3
struct VertexElement : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 255 ends the list
  uint16_t stream() const { return guest::LoadU16(base, address + 0); }
  void set_stream(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t stream_address() const { return address + 0; }
  // probably
  uint16_t offset() const { return guest::LoadU16(base, address + 2); }
  void set_offset(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t offset_address() const { return address + 2; }
  // probably the format
  uint32_t element_type() const { return guest::LoadU32(base, address + 4); }
  void set_element_type(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t element_type_address() const { return address + 4; }
  // probably
  uint8_t method() const { return guest::LoadU8(base, address + 8); }
  void set_method(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t method_address() const { return address + 8; }
  // probably
  uint8_t usage() const { return guest::LoadU8(base, address + 9); }
  void set_usage(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t usage_address() const { return address + 9; }
  // probably
  uint8_t usage_index() const { return guest::LoadU8(base, address + 10); }
  void set_usage_index(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t usage_index_address() const { return address + 10; }
  // the next element's stream, as the count's loop reads it
  uint16_t next_stream() const { return guest::LoadU16(base, address + 12); }
  void set_next_stream(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t next_stream_address() const { return address + 12; }
};

// allocated; measure: new<?>@CreateVertexDeclaration, Device.+11552, Device::PatchVertexShader.r5
struct VertexDeclaration : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // 0x00100005 (probably the resource's type word)
  uint32_t common() const { return guest::LoadU32(base, address + 0); }
  void set_common(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t common_address() const { return address + 0; }
  // 1 (probably)
  uint32_t reference_count() const { return guest::LoadU32(base, address + 4); }
  void set_reference_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t reference_count_address() const { return address + 4; }
  // 0xFFFF0000
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t element_count() const { return guest::LoadU32(base, address + 24); }
  void set_element_count(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t element_count_address() const { return address + 24; }
  uint32_t highest_stream() const { return guest::LoadU32(base, address + 28); }
  void set_highest_stream(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t highest_stream_address() const { return address + 28; }
  // 255 for each stream used, else 0
  uint32_t stream_map_address() const { return address + 32; }
  // 0
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  // the 12-byte elements from +52: element 0's +9 (Device::PatchVertexShader; probably the usage)
  uint8_t element0_byte_9() const { return guest::LoadU8(base, address + 61); }
  void set_element0_byte_9(uint8_t v) const { guest::StoreU8(base, address + 61, v); }
  uint32_t element0_byte_9_address() const { return address + 61; }
  // element 0's +10 (Device::PatchVertexShader; probably the usage index)
  uint8_t element0_byte_10() const { return guest::LoadU8(base, address + 62); }
  void set_element0_byte_10(uint8_t v) const { guest::StoreU8(base, address + 62, v); }
  uint32_t element0_byte_10_address() const { return address + 62; }
};

// embedded; measure: GlobalMemoryStatus.r3
struct MemoryStatus : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint32_t length() const { return guest::LoadU32(base, address + 0); }
  void set_length(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t length_address() const { return address + 0; }
  uint32_t memory_load() const { return guest::LoadU32(base, address + 4); }
  void set_memory_load(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t memory_load_address() const { return address + 4; }
  uint32_t total_physical() const { return guest::LoadU32(base, address + 8); }
  void set_total_physical(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t total_physical_address() const { return address + 8; }
  uint32_t available_physical() const { return guest::LoadU32(base, address + 12); }
  void set_available_physical(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t available_physical_address() const { return address + 12; }
  uint32_t total_page_file() const { return guest::LoadU32(base, address + 16); }
  void set_total_page_file(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t total_page_file_address() const { return address + 16; }
  uint32_t available_page_file() const { return guest::LoadU32(base, address + 20); }
  void set_available_page_file(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t available_page_file_address() const { return address + 20; }
  uint32_t total_virtual() const { return guest::LoadU32(base, address + 24); }
  void set_total_virtual(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t total_virtual_address() const { return address + 24; }
  uint32_t available_virtual() const { return guest::LoadU32(base, address + 28); }
  void set_available_virtual(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t available_virtual_address() const { return address + 28; }
};

// image; measure: [82600A54], [82600A60]
struct BillboardShaderSource : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t first_char() const { return guest::LoadU8(base, address + 0); }
  void set_first_char(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t first_char_address() const { return address + 0; }
};

// The image words docs/functions/app-start-up.md's code uses.
struct AppStartUpGlobals {
  // ReadError::SetApp's: the +4 of the object at 0x82600340
  template <typename T = AppInterface16>
  static T read_error_app(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82600344u)); }
  static void set_read_error_app(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82600344u, v.address); }
  static void set_read_error_app(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600344u, v); }
  static constexpr uint32_t read_error_app_address() { return 0x82600344u; }
  // sub_8209AC30's
  template <typename T = AppInterface16>
  static T app_interface_84(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8266D084u)); }
  static void set_app_interface_84(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8266D084u, v.address); }
  static void set_app_interface_84(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8266D084u, v); }
  static constexpr uint32_t app_interface_84_address() { return 0x8266D084u; }
  // the video output's way in (its slot 7, then that object's slot 3)
  template <typename T = AppInterface16>
  static T scene_target_app(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8266D088u)); }
  static void set_scene_target_app(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8266D088u, v.address); }
  static void set_scene_target_app(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8266D088u, v); }
  static constexpr uint32_t scene_target_app_address() { return 0x8266D088u; }
  // the SceneTarget (32 bytes, scene-target.md), or 0
  static uint32_t scene_target(uint8_t* base) { return guest::LoadU32(base, 0x8266D08Cu); }
  static void set_scene_target(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8266D08Cu, v); }
  static constexpr uint32_t scene_target_address() { return 0x8266D08Cu; }
  // sub_8209F580's: the scene's render manager
  template <typename T = AppInterface16>
  static T render_app(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826BABA0u)); }
  static void set_render_app(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826BABA0u, v.address); }
  static void set_render_app(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BABA0u, v); }
  static constexpr uint32_t render_app_address() { return 0x826BABA0u; }
  template <typename T = Tiling>
  static T display_tiling(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8260034Cu)); }
  static void set_display_tiling(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8260034Cu, v.address); }
  static void set_display_tiling(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8260034Cu, v); }
  static constexpr uint32_t display_tiling_address() { return 0x8260034Cu; }
  // sub_8225FB70 clears it; readers not looked for
  static uint32_t word_82816110(uint8_t* base) { return guest::LoadU32(base, 0x82816110u); }
  static void set_word_82816110(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816110u, v); }
  static constexpr uint32_t word_82816110_address() { return 0x82816110u; }
  // sub_8225FB70 clears it; readers not looked for
  static uint32_t word_82816114(uint8_t* base) { return guest::LoadU32(base, 0x82816114u); }
  static void set_word_82816114(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82816114u, v); }
  static constexpr uint32_t word_82816114_address() { return 0x82816114u; }
  // flipped 0 / 1
  static uint32_t billboard_ring_index(uint8_t* base) { return guest::LoadU32(base, 0x826A5098u); }
  static void set_billboard_ring_index(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5098u, v); }
  static constexpr uint32_t billboard_ring_index_address() { return 0x826A5098u; }
  static uint32_t billboard_vertex_count(uint8_t* base) { return guest::LoadU32(base, 0x826A509Cu); }
  static void set_billboard_vertex_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A509Cu, v); }
  static constexpr uint32_t billboard_vertex_count_address() { return 0x826A509Cu; }
  static uint32_t billboard_vertex_shader(uint8_t* base) { return guest::LoadU32(base, 0x826A50A0u); }
  static void set_billboard_vertex_shader(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50A0u, v); }
  static constexpr uint32_t billboard_vertex_shader_address() { return 0x826A50A0u; }
  static uint32_t billboard_pixel_shader(uint8_t* base) { return guest::LoadU32(base, 0x826A50A4u); }
  static void set_billboard_pixel_shader(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50A4u, v); }
  static constexpr uint32_t billboard_pixel_shader_address() { return 0x826A50A4u; }
  template <typename T = VertexDeclaration>
  static T billboard_declaration(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826A50A8u)); }
  static void set_billboard_declaration(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826A50A8u, v.address); }
  static void set_billboard_declaration(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50A8u, v); }
  static constexpr uint32_t billboard_declaration_address() { return 0x826A50A8u; }
  template <typename T = BillboardShaderSource>
  static T billboard_vertex_source(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82600A54u)); }
  static void set_billboard_vertex_source(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82600A54u, v.address); }
  static void set_billboard_vertex_source(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600A54u, v); }
  static constexpr uint32_t billboard_vertex_source_address() { return 0x82600A54u; }
  template <typename T = BillboardShaderSource>
  static T billboard_pixel_source(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82600A60u)); }
  static void set_billboard_pixel_source(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82600A60u, v.address); }
  static void set_billboard_pixel_source(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600A60u, v); }
  static constexpr uint32_t billboard_pixel_source_address() { return 0x82600A60u; }
  // three VertexElements and the end
  static constexpr uint32_t billboard_elements_address() { return 0x82600A64u; }
  // a float Billboards::Init clears (copied as bits from 0x82000A34); readers not looked for
  static uint32_t billboard_float_8266D090(uint8_t* base) { return guest::LoadU32(base, 0x8266D090u); }
  static void set_billboard_float_8266D090(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8266D090u, v); }
  static constexpr uint32_t billboard_float_8266D090_address() { return 0x8266D090u; }
  // 0.0 in the image (as bits)
  static uint32_t float_zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_float_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t float_zero_address() { return 0x82000A34u; }
};

}  // namespace schema
