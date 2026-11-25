// The views of docs/functions/movies.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/movies.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Movie, sub_820AE808.r3, sub_820AE818.r3, sub_820AE9F8.r3
#pragma pack(push, 1)
struct MovieData {
  uint32_t vtable = 0;  // +0
  uint32_t framework_interface = 0;  // +4
  uint32_t wrapper = 0;  // +8
  uint32_t command_vtable = 0;  // +12
  uint32_t command_vtable_4 = 0;  // +16
  uint32_t unknown_20 = 0;  // +20
  uint32_t command_status = 0;  // +24
  uint8_t unknown_28 = 0;  // +28
  uint8_t unknown_29 = 0;  // +29
  uint8_t unknown_30 = 0;  // +30
  uint8_t pad_31[1] = {};
  uint32_t unknown_32 = 0;  // +32
  uint32_t delegate_vtable = 0;  // +36
  uint32_t delegate_object = 0;  // +40
  uint32_t delegate_function = 0;  // +44
  uint8_t path[260] = {};  // +48, guest order
  uint32_t state = 0;  // +308
};
#pragma pack(pop)
static_assert(sizeof(MovieData) == 312);
inline constexpr memory::LayoutField kMovieFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {308, 4}};
inline constexpr memory::Layout kMovieLayout = {kMovieFields, 12};

struct Movie : View {
  using View::View;
  static constexpr uint32_t kSize = 312;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82002910;
  MovieData* data() const { return static_cast<MovieData*>(memory::HandleObject(address)); }
  static Movie New() {
    return Movie(memory::MakeHandle(new MovieData(), sizeof(MovieData), "Movie", &kMovieLayout));
  }
  template <uint32_t Stride = kSize>
  static Movie NewArray(uint32_t count) {
    static_assert(sizeof(MovieData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MovieData*>(::operator new(sizeof(MovieData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MovieData();
    return Movie(memory::MakeHandle(p, sizeof(MovieData) * count, "Movie", &kMovieLayout, sizeof(MovieData)));
  }
  Movie at(uint32_t i) const { return Movie(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MovieData* direct() const { return static_cast<MovieData*>(memory::HandleOwn(address, &kMovieLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMovieLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMovieLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82002910
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = MovieVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // the set-up block's +4 (the app's +8): slot +8 by Movie::Play, +12 by OnTick, +16 by Open and Release
  uint32_t framework_interface_address() const { return address + 4; }
  uint32_t framework_interface_bits() const { if (auto* d = direct()) return d->framework_interface; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_framework_interface_bits(uint32_t v) const { if (auto* d = direct()) { d->framework_interface = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = AppFrameworkInterface>
  T framework_interface() const { return T(framework_interface_bits()); }
  void set_framework_interface(const View& w) const { set_framework_interface_bits(w.address); }
  void set_framework_interface(uint32_t v) const { set_framework_interface_bits(v); }
  // the 8-byte MovieWrapper, or 0
  uint32_t wrapper_address() const { return address + 8; }
  uint32_t wrapper_bits() const { if (auto* d = direct()) return d->wrapper; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_wrapper_bits(uint32_t v) const { if (auto* d = direct()) { d->wrapper = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  template <typename T = MovieWrapper>
  T wrapper() const { return T(wrapper_bits()); }
  void set_wrapper(const View& w) const { set_wrapper_bits(w.address); }
  void set_wrapper(uint32_t v) const { set_wrapper_bits(v); }
  // the open's command: 0x820066DC (0x82000EB8 put back by the destructor)
  uint32_t command_vtable_address() const { return address + 12; }
  uint32_t command_vtable_bits() const { if (auto* d = direct()) return d->command_vtable; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_command_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->command_vtable = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t command_vtable() const { if (auto* d = direct()) return d->command_vtable; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_command_vtable(uint32_t v) const { if (auto* d = direct()) { d->command_vtable = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // 0x82000AB4 then 0x82002908 (0x82000BA8, then 0x82000AB4 put back)
  uint32_t command_vtable_4_address() const { return address + 16; }
  uint32_t command_vtable_4_bits() const { if (auto* d = direct()) return d->command_vtable_4; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_command_vtable_4_bits(uint32_t v) const { if (auto* d = direct()) { d->command_vtable_4 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t command_vtable_4() const { if (auto* d = direct()) return d->command_vtable_4; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_command_vtable_4(uint32_t v) const { if (auto* d = direct()) { d->command_vtable_4 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // the command's +8: 0
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_20_bits() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_unknown_20_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t unknown_20() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_unknown_20(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the command's +12: 0, 3 once done (the slots test it)
  uint32_t command_status_address() const { return address + 24; }
  uint32_t command_status_bits() const { if (auto* d = direct()) return d->command_status; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_command_status_bits(uint32_t v) const { if (auto* d = direct()) { d->command_status = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t command_status() const { if (auto* d = direct()) return d->command_status; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_command_status(uint32_t v) const { if (auto* d = direct()) { d->command_status = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  // the command's +16: 0
  uint32_t unknown_28_address() const { return address + 28; }
  uint8_t unknown_28_bits() const { if (auto* d = direct()) return d->unknown_28; return uint8_t((memory::IsHandle(address + 28) ? uint8_t(memory::OwnerLoad(address + 28, 1)) : guest::LoadU8(address + 28))); }
  void set_unknown_28_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 1, v); else guest::StoreU8(address + 28, v); }
  uint8_t unknown_28() const { if (auto* d = direct()) return d->unknown_28; return uint8_t((memory::IsHandle(address + 28) ? uint8_t(memory::OwnerLoad(address + 28, 1)) : guest::LoadU8(address + 28))); }
  void set_unknown_28(uint8_t v) const { if (auto* d = direct()) { d->unknown_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 1, v); else guest::StoreU8(address + 28, v); }
  // the command's +17: 1
  uint32_t unknown_29_address() const { return address + 29; }
  uint8_t unknown_29_bits() const { if (auto* d = direct()) return d->unknown_29; return uint8_t((memory::IsHandle(address + 29) ? uint8_t(memory::OwnerLoad(address + 29, 1)) : guest::LoadU8(address + 29))); }
  void set_unknown_29_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_29 = v; return; } if (memory::IsHandle(address + 29)) memory::OwnerStore(address + 29, 1, v); else guest::StoreU8(address + 29, v); }
  uint8_t unknown_29() const { if (auto* d = direct()) return d->unknown_29; return uint8_t((memory::IsHandle(address + 29) ? uint8_t(memory::OwnerLoad(address + 29, 1)) : guest::LoadU8(address + 29))); }
  void set_unknown_29(uint8_t v) const { if (auto* d = direct()) { d->unknown_29 = v; return; } if (memory::IsHandle(address + 29)) memory::OwnerStore(address + 29, 1, v); else guest::StoreU8(address + 29, v); }
  // the command's +18: 0
  uint32_t unknown_30_address() const { return address + 30; }
  uint8_t unknown_30_bits() const { if (auto* d = direct()) return d->unknown_30; return uint8_t((memory::IsHandle(address + 30) ? uint8_t(memory::OwnerLoad(address + 30, 1)) : guest::LoadU8(address + 30))); }
  void set_unknown_30_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_30 = v; return; } if (memory::IsHandle(address + 30)) memory::OwnerStore(address + 30, 1, v); else guest::StoreU8(address + 30, v); }
  uint8_t unknown_30() const { if (auto* d = direct()) return d->unknown_30; return uint8_t((memory::IsHandle(address + 30) ? uint8_t(memory::OwnerLoad(address + 30, 1)) : guest::LoadU8(address + 30))); }
  void set_unknown_30(uint8_t v) const { if (auto* d = direct()) { d->unknown_30 = v; return; } if (memory::IsHandle(address + 30)) memory::OwnerStore(address + 30, 1, v); else guest::StoreU8(address + 30, v); }
  // the command's +20: Movie::Play stores a word of its frame (+100)
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_32_bits() const { if (auto* d = direct()) return d->unknown_32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_unknown_32_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t unknown_32() const { if (auto* d = direct()) return d->unknown_32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_unknown_32(uint32_t v) const { if (auto* d = direct()) { d->unknown_32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // the command's delegate: 0x82002850 (0x82000ABC put back)
  uint32_t delegate_vtable_address() const { return address + 36; }
  uint32_t delegate_vtable_bits() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_delegate_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t delegate_vtable() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_delegate_vtable(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  // 0, then the movie (Movie::Play)
  uint32_t delegate_object_address() const { return address + 40; }
  uint32_t delegate_object_bits() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_delegate_object_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = Movie>
  T delegate_object() const { return T(delegate_object_bits()); }
  void set_delegate_object(const View& w) const { set_delegate_object_bits(w.address); }
  void set_delegate_object(uint32_t v) const { set_delegate_object_bits(v); }
  // 0, then 0x820AE7F8 (calls the movie's slot 8, Movie::Open)
  uint32_t delegate_function_address() const { return address + 44; }
  uint32_t delegate_function_bits() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_delegate_function_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t delegate_function() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_delegate_function(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  // the movie's path (strncpy'd by Movie::Play)
  uint32_t path_address() const { return address + 48; }
  uint8_t* path() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 48 : guest::Pointer(address + 48); }
  // the path as the C runtime's string functions reach it (the measure's Movie@+48)
  template <typename T = MovieAt48>
  T path_string() const { return T(address + 48); }
  // 0 none, 1 loaded or ended, 2 playing, 3 paused
  uint32_t state_address() const { return address + 308; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 308) ? uint32_t(memory::OwnerLoad(address + 308, 4)) : guest::LoadU32(address + 308))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 308)) memory::OwnerStore(address + 308, 4, v); else guest::StoreU32(address + 308, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 308) ? uint32_t(memory::OwnerLoad(address + 308, 4)) : guest::LoadU32(address + 308))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 308)) memory::OwnerStore(address + 308, 4, v); else guest::StoreU32(address + 308, v); }
};

// image; measure: [826C1CDC].+0, sub_820AE808.r3.+0, sub_820AE818.r3.+0
struct MovieVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // sub_820AE958 Movie::RenderFrame
  uint32_t render_frame() const { return guest::LoadU32(address + 0); }
  void set_render_frame(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t render_frame_address() const { return address + 0; }
  // sub_820AEC10 Movie::Play
  uint32_t play() const { return guest::LoadU32(address + 4); }
  void set_play(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t play_address() const { return address + 4; }
  // sub_820AE4E0 Movie::Start
  uint32_t start() const { return guest::LoadU32(address + 8); }
  void set_start(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t start_address() const { return address + 8; }
  // sub_820AE828 Movie::Stop
  uint32_t stop() const { return guest::LoadU32(address + 12); }
  void set_stop(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t stop_address() const { return address + 12; }
  // sub_820AE898 Movie::Pause
  uint32_t pause() const { return guest::LoadU32(address + 16); }
  void set_pause(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t pause_address() const { return address + 16; }
  // sub_820AE8F8 Movie::Resume
  uint32_t resume() const { return guest::LoadU32(address + 20); }
  void set_resume(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t resume_address() const { return address + 20; }
  // sub_820AEA80 Movie::State
  uint32_t state() const { return guest::LoadU32(address + 24); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t state_address() const { return address + 24; }
  // sub_820AE528 Movie::Release
  uint32_t release() const { return guest::LoadU32(address + 28); }
  void set_release(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t release_address() const { return address + 28; }
  // sub_820AE5D0 Movie::Open
  uint32_t open() const { return guest::LoadU32(address + 32); }
  void set_open(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t open_address() const { return address + 32; }
  // sub_820AE688 Movie::OnTick
  uint32_t on_tick() const { return guest::LoadU32(address + 36); }
  void set_on_tick(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t on_tick_address() const { return address + 36; }
  // sub_820AE780 Movie::OnMessage
  uint32_t on_message() const { return guest::LoadU32(address + 40); }
  void set_on_message(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t on_message_address() const { return address + 40; }
};

// allocated; measure: MovieWrapper, Movie.+8, sub_820AE9F8.r3.+8
#pragma pack(push, 1)
struct MovieWrapperData {
  uint32_t vtable = 0;  // +0
  uint32_t block = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(MovieWrapperData) == 8);
inline constexpr memory::LayoutField kMovieWrapperFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kMovieWrapperLayout = {kMovieWrapperFields, 2};

struct MovieWrapper : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x8206CC90;
  MovieWrapperData* data() const { return static_cast<MovieWrapperData*>(memory::HandleObject(address)); }
  static MovieWrapper New() {
    return MovieWrapper(memory::MakeHandle(new MovieWrapperData(), sizeof(MovieWrapperData), "MovieWrapper", &kMovieWrapperLayout));
  }
  template <uint32_t Stride = kSize>
  static MovieWrapper NewArray(uint32_t count) {
    static_assert(sizeof(MovieWrapperData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MovieWrapperData*>(::operator new(sizeof(MovieWrapperData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MovieWrapperData();
    return MovieWrapper(memory::MakeHandle(p, sizeof(MovieWrapperData) * count, "MovieWrapper", &kMovieWrapperLayout, sizeof(MovieWrapperData)));
  }
  MovieWrapper at(uint32_t i) const { return MovieWrapper(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MovieWrapperData* direct() const { return static_cast<MovieWrapperData*>(memory::HandleOwn(address, &kMovieWrapperLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMovieWrapperLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMovieWrapperLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x8206CC90
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = MovieWrapperVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // the 32-byte block (operator new), or 0
  uint32_t block_address() const { return address + 4; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = MovieWrapperBlock>
  T block() const { return T(block_bits()); }
  void set_block(const View& w) const { set_block_bits(w.address); }
  void set_block(uint32_t v) const { set_block_bits(v); }
};

// image; measure: Movie.+8.+0, sub_820AE9F8.r3.+8.+0
struct MovieWrapperVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // sub_82593E78 (probably the open)
  uint32_t open() const { return guest::LoadU32(address + 0); }
  void set_open(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t open_address() const { return address + 0; }
  // sub_82106540, empty
  uint32_t slot_1() const { return guest::LoadU32(address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
  // sub_82593D88 (slots 2 to 4)
  uint32_t sync_2() const { return guest::LoadU32(address + 8); }
  void set_sync_2(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t sync_2_address() const { return address + 8; }
  uint32_t sync_3() const { return guest::LoadU32(address + 12); }
  void set_sync_3(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t sync_3_address() const { return address + 12; }
  uint32_t sync_4() const { return guest::LoadU32(address + 16); }
  void set_sync_4(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t sync_4_address() const { return address + 16; }
  // sub_82593D78: the player
  uint32_t player() const { return guest::LoadU32(address + 20); }
  void set_player(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t player_address() const { return address + 20; }
  // sub_82593E30: the player released
  uint32_t release() const { return guest::LoadU32(address + 24); }
  void set_release(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t release_address() const { return address + 24; }
  // sub_82593F60
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 28); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t deleting_destructor_address() const { return address + 28; }
};

// allocated; measure: MovieWrapper.+4
#pragma pack(push, 1)
struct MovieWrapperBlockData {
  uint8_t critical_section[28] = {};  // +0, guest order
  uint32_t player = 0;  // +28
};
#pragma pack(pop)
static_assert(sizeof(MovieWrapperBlockData) == 32);
inline constexpr memory::LayoutField kMovieWrapperBlockFields[] = {{28, 4}};
inline constexpr memory::Layout kMovieWrapperBlockLayout = {kMovieWrapperBlockFields, 1};

struct MovieWrapperBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  MovieWrapperBlockData* data() const { return static_cast<MovieWrapperBlockData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static MovieWrapperBlock New() {
    return MovieWrapperBlock(memory::MakeHandle(new MovieWrapperBlockData(), sizeof(MovieWrapperBlockData), "MovieWrapperBlock", &kMovieWrapperBlockLayout));
  }
  template <uint32_t Stride = kSize>
  static MovieWrapperBlock NewArray(uint32_t count) {
    static_assert(sizeof(MovieWrapperBlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MovieWrapperBlockData*>(::operator new(sizeof(MovieWrapperBlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MovieWrapperBlockData();
    return MovieWrapperBlock(memory::MakeHandle(p, sizeof(MovieWrapperBlockData) * count, "MovieWrapperBlock", &kMovieWrapperBlockLayout, sizeof(MovieWrapperBlockData)));
  }
  MovieWrapperBlock at(uint32_t i) const { return MovieWrapperBlock(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MovieWrapperBlockData* direct() const { return static_cast<MovieWrapperBlockData*>(memory::HandleOwn(address, &kMovieWrapperBlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMovieWrapperBlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMovieWrapperBlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a host mutex since 2026-10-30 (app_start_up::MovieWrapperMutex): its bytes left as operator new leaves them
  uint32_t critical_section_address() const { return address + 0; }
  uint8_t* critical_section() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  // the library's player, 0 before the open
  uint32_t player_address() const { return address + 28; }
  uint32_t player_bits() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_player_bits(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  template <typename T = MoviePlayer>
  T player() const { return T(player_bits()); }
  void set_player(const View& w) const { set_player_bits(w.address); }
  void set_player(uint32_t v) const { set_player_bits(v); }
};

// allocated; measure: MoviePlayer, MovieWrapper.+4.+28
struct MoviePlayer : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = MoviePlayerVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: MoviePlayer.+0, MovieWrapper.+4.+28.+0
struct MoviePlayerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 120;
  // probably: called by sub_820A7BD0 (the player's release)
  uint32_t release() const { return guest::LoadU32(address + 8); }
  void set_release(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t release_address() const { return address + 8; }
  // probably: sub_82481B88's (r4 1, r5 0x82106540, r6 the wrapper)
  uint32_t set_callback() const { return guest::LoadU32(address + 56); }
  void set_set_callback(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t set_callback_address() const { return address + 56; }
  // MoviePlayer::RenderNextFrame's
  uint32_t render_next_frame() const { return guest::LoadU32(address + 80); }
  void set_render_next_frame(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t render_next_frame_address() const { return address + 80; }
  // MoviePlayer::Pause's (probably)
  uint32_t pause() const { return guest::LoadU32(address + 88); }
  void set_pause(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t pause_address() const { return address + 88; }
  // MoviePlayer::Resume's (probably)
  uint32_t resume() const { return guest::LoadU32(address + 92); }
  void set_resume(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t resume_address() const { return address + 92; }
  // MoviePlayer::Stop's (probably)
  uint32_t stop() const { return guest::LoadU32(address + 96); }
  void set_stop(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t stop_address() const { return address + 96; }
  // MoviePlayer::SetRect's (probably)
  uint32_t set_rect() const { return guest::LoadU32(address + 116); }
  void set_set_rect(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t set_rect_address() const { return address + 116; }
};

// embedded; measure: new<?>@sub_82485460
struct MovieLibNew82485460 : View {
  using View::View;
  static constexpr uint32_t kSize = 680;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  float unknown_80() const { return guest::LoadFloat(address + 80); }
  void set_unknown_80(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint32_t unknown_124() const { return guest::LoadU32(address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  uint32_t unknown_444() const { return guest::LoadU32(address + 444); }
  void set_unknown_444(uint32_t v) const { guest::StoreU32(address + 444, v); }
  uint32_t unknown_444_address() const { return address + 444; }
  uint32_t unknown_448() const { return guest::LoadU32(address + 448); }
  void set_unknown_448(uint32_t v) const { guest::StoreU32(address + 448, v); }
  uint32_t unknown_448_address() const { return address + 448; }
  uint32_t unknown_660() const { return guest::LoadU32(address + 660); }
  void set_unknown_660(uint32_t v) const { guest::StoreU32(address + 660, v); }
  uint32_t unknown_660_address() const { return address + 660; }
  uint32_t unknown_664() const { return guest::LoadU32(address + 664); }
  void set_unknown_664(uint32_t v) const { guest::StoreU32(address + 664, v); }
  uint32_t unknown_664_address() const { return address + 664; }
  uint32_t unknown_668() const { return guest::LoadU32(address + 668); }
  void set_unknown_668(uint32_t v) const { guest::StoreU32(address + 668, v); }
  uint32_t unknown_668_address() const { return address + 668; }
  uint32_t unknown_672() const { return guest::LoadU32(address + 672); }
  void set_unknown_672(uint32_t v) const { guest::StoreU32(address + 672, v); }
  uint32_t unknown_672_address() const { return address + 672; }
  uint32_t unknown_676() const { return guest::LoadU32(address + 676); }
  void set_unknown_676(uint32_t v) const { guest::StoreU32(address + 676, v); }
  uint32_t unknown_676_address() const { return address + 676; }
};

// embedded; measure: new<?>@sub_824847D0
struct MovieLibNew824847D0 : View {
  using View::View;
  static constexpr uint32_t kSize = 548;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  float unknown_72() const { return guest::LoadFloat(address + 72); }
  void set_unknown_72(float v) const { guest::StoreFloat(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint64_t unknown_128() const { return guest::LoadU64(address + 128); }
  void set_unknown_128(uint64_t v) const { guest::StoreU64(address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  uint32_t unknown_136() const { return guest::LoadU32(address + 136); }
  void set_unknown_136(uint32_t v) const { guest::StoreU32(address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  uint32_t unknown_284() const { return guest::LoadU32(address + 284); }
  void set_unknown_284(uint32_t v) const { guest::StoreU32(address + 284, v); }
  uint32_t unknown_284_address() const { return address + 284; }
  uint32_t unknown_500() const { return guest::LoadU32(address + 500); }
  void set_unknown_500(uint32_t v) const { guest::StoreU32(address + 500, v); }
  uint32_t unknown_500_address() const { return address + 500; }
  uint32_t unknown_504() const { return guest::LoadU32(address + 504); }
  void set_unknown_504(uint32_t v) const { guest::StoreU32(address + 504, v); }
  uint32_t unknown_504_address() const { return address + 504; }
  uint32_t unknown_508() const { return guest::LoadU32(address + 508); }
  void set_unknown_508(uint32_t v) const { guest::StoreU32(address + 508, v); }
  uint32_t unknown_508_address() const { return address + 508; }
  uint32_t unknown_512() const { return guest::LoadU32(address + 512); }
  void set_unknown_512(uint32_t v) const { guest::StoreU32(address + 512, v); }
  uint32_t unknown_512_address() const { return address + 512; }
  uint32_t unknown_520() const { return guest::LoadU32(address + 520); }
  void set_unknown_520(uint32_t v) const { guest::StoreU32(address + 520, v); }
  uint32_t unknown_520_address() const { return address + 520; }
  uint32_t unknown_524() const { return guest::LoadU32(address + 524); }
  void set_unknown_524(uint32_t v) const { guest::StoreU32(address + 524, v); }
  uint32_t unknown_524_address() const { return address + 524; }
  uint64_t unknown_528() const { return guest::LoadU64(address + 528); }
  void set_unknown_528(uint64_t v) const { guest::StoreU64(address + 528, v); }
  uint32_t unknown_528_address() const { return address + 528; }
  uint32_t unknown_536() const { return guest::LoadU32(address + 536); }
  void set_unknown_536(uint32_t v) const { guest::StoreU32(address + 536, v); }
  uint32_t unknown_536_address() const { return address + 536; }
  uint32_t unknown_540() const { return guest::LoadU32(address + 540); }
  void set_unknown_540(uint32_t v) const { guest::StoreU32(address + 540, v); }
  uint32_t unknown_540_address() const { return address + 540; }
  uint32_t unknown_544() const { return guest::LoadU32(address + 544); }
  void set_unknown_544(uint32_t v) const { guest::StoreU32(address + 544, v); }
  uint32_t unknown_544_address() const { return address + 544; }
};

// embedded; measure: new<?>@sub_824875A8
struct MovieLibNew824875A8 : View {
  using View::View;
  static constexpr uint32_t kSize = 296;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t unknown_80() const { return guest::LoadU32(address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint32_t unknown_84() const { return guest::LoadU32(address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  uint32_t unknown_88() const { return guest::LoadU32(address + 88); }
  void set_unknown_88(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  uint32_t unknown_92() const { return guest::LoadU32(address + 92); }
  void set_unknown_92(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  uint32_t unknown_100() const { return guest::LoadU32(address + 100); }
  void set_unknown_100(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  uint32_t unknown_104() const { return guest::LoadU32(address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t unknown_108() const { return guest::LoadU32(address + 108); }
  void set_unknown_108(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  uint32_t unknown_112() const { return guest::LoadU32(address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  uint32_t unknown_120() const { return guest::LoadU32(address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  uint32_t unknown_124() const { return guest::LoadU32(address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  uint32_t unknown_128() const { return guest::LoadU32(address + 128); }
  void set_unknown_128(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  uint32_t unknown_132() const { return guest::LoadU32(address + 132); }
  void set_unknown_132(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  uint32_t unknown_136() const { return guest::LoadU32(address + 136); }
  void set_unknown_136(uint32_t v) const { guest::StoreU32(address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  uint32_t unknown_140() const { return guest::LoadU32(address + 140); }
  void set_unknown_140(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  uint32_t unknown_144() const { return guest::LoadU32(address + 144); }
  void set_unknown_144(uint32_t v) const { guest::StoreU32(address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  uint32_t unknown_148() const { return guest::LoadU32(address + 148); }
  void set_unknown_148(uint32_t v) const { guest::StoreU32(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  uint32_t unknown_152() const { return guest::LoadU32(address + 152); }
  void set_unknown_152(uint32_t v) const { guest::StoreU32(address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  uint32_t unknown_156() const { return guest::LoadU32(address + 156); }
  void set_unknown_156(uint32_t v) const { guest::StoreU32(address + 156, v); }
  uint32_t unknown_156_address() const { return address + 156; }
  uint32_t unknown_160() const { return guest::LoadU32(address + 160); }
  void set_unknown_160(uint32_t v) const { guest::StoreU32(address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  uint32_t unknown_164() const { return guest::LoadU32(address + 164); }
  void set_unknown_164(uint32_t v) const { guest::StoreU32(address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  uint32_t unknown_168() const { return guest::LoadU32(address + 168); }
  void set_unknown_168(uint32_t v) const { guest::StoreU32(address + 168, v); }
  uint32_t unknown_168_address() const { return address + 168; }
  uint32_t unknown_172() const { return guest::LoadU32(address + 172); }
  void set_unknown_172(uint32_t v) const { guest::StoreU32(address + 172, v); }
  uint32_t unknown_172_address() const { return address + 172; }
  uint32_t unknown_176() const { return guest::LoadU32(address + 176); }
  void set_unknown_176(uint32_t v) const { guest::StoreU32(address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
  uint32_t unknown_180() const { return guest::LoadU32(address + 180); }
  void set_unknown_180(uint32_t v) const { guest::StoreU32(address + 180, v); }
  uint32_t unknown_180_address() const { return address + 180; }
  uint32_t unknown_184() const { return guest::LoadU32(address + 184); }
  void set_unknown_184(uint32_t v) const { guest::StoreU32(address + 184, v); }
  uint32_t unknown_184_address() const { return address + 184; }
  uint32_t unknown_188() const { return guest::LoadU32(address + 188); }
  void set_unknown_188(uint32_t v) const { guest::StoreU32(address + 188, v); }
  uint32_t unknown_188_address() const { return address + 188; }
  uint32_t unknown_192() const { return guest::LoadU32(address + 192); }
  void set_unknown_192(uint32_t v) const { guest::StoreU32(address + 192, v); }
  uint32_t unknown_192_address() const { return address + 192; }
  uint32_t unknown_196() const { return guest::LoadU32(address + 196); }
  void set_unknown_196(uint32_t v) const { guest::StoreU32(address + 196, v); }
  uint32_t unknown_196_address() const { return address + 196; }
  uint32_t unknown_200() const { return guest::LoadU32(address + 200); }
  void set_unknown_200(uint32_t v) const { guest::StoreU32(address + 200, v); }
  uint32_t unknown_200_address() const { return address + 200; }
  uint32_t unknown_204() const { return guest::LoadU32(address + 204); }
  void set_unknown_204(uint32_t v) const { guest::StoreU32(address + 204, v); }
  uint32_t unknown_204_address() const { return address + 204; }
  uint32_t unknown_208() const { return guest::LoadU32(address + 208); }
  void set_unknown_208(uint32_t v) const { guest::StoreU32(address + 208, v); }
  uint32_t unknown_208_address() const { return address + 208; }
  uint32_t unknown_212() const { return guest::LoadU32(address + 212); }
  void set_unknown_212(uint32_t v) const { guest::StoreU32(address + 212, v); }
  uint32_t unknown_212_address() const { return address + 212; }
  uint32_t unknown_216() const { return guest::LoadU32(address + 216); }
  void set_unknown_216(uint32_t v) const { guest::StoreU32(address + 216, v); }
  uint32_t unknown_216_address() const { return address + 216; }
  uint32_t unknown_220() const { return guest::LoadU32(address + 220); }
  void set_unknown_220(uint32_t v) const { guest::StoreU32(address + 220, v); }
  uint32_t unknown_220_address() const { return address + 220; }
  uint32_t unknown_224() const { return guest::LoadU32(address + 224); }
  void set_unknown_224(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  uint32_t unknown_228() const { return guest::LoadU32(address + 228); }
  void set_unknown_228(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  uint32_t unknown_232() const { return guest::LoadU32(address + 232); }
  void set_unknown_232(uint32_t v) const { guest::StoreU32(address + 232, v); }
  uint32_t unknown_232_address() const { return address + 232; }
  uint32_t unknown_236() const { return guest::LoadU32(address + 236); }
  void set_unknown_236(uint32_t v) const { guest::StoreU32(address + 236, v); }
  uint32_t unknown_236_address() const { return address + 236; }
  uint32_t unknown_240() const { return guest::LoadU32(address + 240); }
  void set_unknown_240(uint32_t v) const { guest::StoreU32(address + 240, v); }
  uint32_t unknown_240_address() const { return address + 240; }
  uint32_t unknown_244() const { return guest::LoadU32(address + 244); }
  void set_unknown_244(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  uint32_t unknown_248() const { return guest::LoadU32(address + 248); }
  void set_unknown_248(uint32_t v) const { guest::StoreU32(address + 248, v); }
  uint32_t unknown_248_address() const { return address + 248; }
  uint32_t unknown_252() const { return guest::LoadU32(address + 252); }
  void set_unknown_252(uint32_t v) const { guest::StoreU32(address + 252, v); }
  uint32_t unknown_252_address() const { return address + 252; }
  uint32_t unknown_256() const { return guest::LoadU32(address + 256); }
  void set_unknown_256(uint32_t v) const { guest::StoreU32(address + 256, v); }
  uint32_t unknown_256_address() const { return address + 256; }
  uint32_t unknown_260() const { return guest::LoadU32(address + 260); }
  void set_unknown_260(uint32_t v) const { guest::StoreU32(address + 260, v); }
  uint32_t unknown_260_address() const { return address + 260; }
  uint32_t unknown_264() const { return guest::LoadU32(address + 264); }
  void set_unknown_264(uint32_t v) const { guest::StoreU32(address + 264, v); }
  uint32_t unknown_264_address() const { return address + 264; }
  uint32_t unknown_268() const { return guest::LoadU32(address + 268); }
  void set_unknown_268(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t unknown_268_address() const { return address + 268; }
  uint32_t unknown_272() const { return guest::LoadU32(address + 272); }
  void set_unknown_272(uint32_t v) const { guest::StoreU32(address + 272, v); }
  uint32_t unknown_272_address() const { return address + 272; }
  uint32_t unknown_276() const { return guest::LoadU32(address + 276); }
  void set_unknown_276(uint32_t v) const { guest::StoreU32(address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  float unknown_280() const { return guest::LoadFloat(address + 280); }
  void set_unknown_280(float v) const { guest::StoreFloat(address + 280, v); }
  uint32_t unknown_280_address() const { return address + 280; }
  uint64_t unknown_288() const { return guest::LoadU64(address + 288); }
  void set_unknown_288(uint64_t v) const { guest::StoreU64(address + 288, v); }
  uint32_t unknown_288_address() const { return address + 288; }
};

// embedded; measure: new<?>@sub_82483DC0
struct MovieLibNew82483DC0 : View {
  using View::View;
  static constexpr uint32_t kSize = 376;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_288() const { return guest::LoadU32(address + 288); }
  void set_unknown_288(uint32_t v) const { guest::StoreU32(address + 288, v); }
  uint32_t unknown_288_address() const { return address + 288; }
  uint32_t unknown_324() const { return guest::LoadU32(address + 324); }
  void set_unknown_324(uint32_t v) const { guest::StoreU32(address + 324, v); }
  uint32_t unknown_324_address() const { return address + 324; }
  uint32_t unknown_328() const { return guest::LoadU32(address + 328); }
  void set_unknown_328(uint32_t v) const { guest::StoreU32(address + 328, v); }
  uint32_t unknown_328_address() const { return address + 328; }
  uint32_t unknown_332() const { return guest::LoadU32(address + 332); }
  void set_unknown_332(uint32_t v) const { guest::StoreU32(address + 332, v); }
  uint32_t unknown_332_address() const { return address + 332; }
  uint32_t unknown_336() const { return guest::LoadU32(address + 336); }
  void set_unknown_336(uint32_t v) const { guest::StoreU32(address + 336, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  uint32_t unknown_340() const { return guest::LoadU32(address + 340); }
  void set_unknown_340(uint32_t v) const { guest::StoreU32(address + 340, v); }
  uint32_t unknown_340_address() const { return address + 340; }
  uint32_t unknown_344() const { return guest::LoadU32(address + 344); }
  void set_unknown_344(uint32_t v) const { guest::StoreU32(address + 344, v); }
  uint32_t unknown_344_address() const { return address + 344; }
  uint32_t unknown_348() const { return guest::LoadU32(address + 348); }
  void set_unknown_348(uint32_t v) const { guest::StoreU32(address + 348, v); }
  uint32_t unknown_348_address() const { return address + 348; }
  uint32_t unknown_352() const { return guest::LoadU32(address + 352); }
  void set_unknown_352(uint32_t v) const { guest::StoreU32(address + 352, v); }
  uint32_t unknown_352_address() const { return address + 352; }
  uint32_t unknown_356() const { return guest::LoadU32(address + 356); }
  void set_unknown_356(uint32_t v) const { guest::StoreU32(address + 356, v); }
  uint32_t unknown_356_address() const { return address + 356; }
  uint32_t unknown_360() const { return guest::LoadU32(address + 360); }
  void set_unknown_360(uint32_t v) const { guest::StoreU32(address + 360, v); }
  uint32_t unknown_360_address() const { return address + 360; }
  uint32_t unknown_364() const { return guest::LoadU32(address + 364); }
  void set_unknown_364(uint32_t v) const { guest::StoreU32(address + 364, v); }
  uint32_t unknown_364_address() const { return address + 364; }
  uint32_t unknown_368() const { return guest::LoadU32(address + 368); }
  void set_unknown_368(uint32_t v) const { guest::StoreU32(address + 368, v); }
  uint32_t unknown_368_address() const { return address + 368; }
  uint32_t unknown_372() const { return guest::LoadU32(address + 372); }
  void set_unknown_372(uint32_t v) const { guest::StoreU32(address + 372, v); }
  uint32_t unknown_372_address() const { return address + 372; }
};

// embedded; measure: new<?>@sub_82486810, sub_8248F218.r3
struct MovieLibNew82486810 : View {
  using View::View;
  static constexpr uint32_t kSize = 344;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint64_t unknown_288() const { return guest::LoadU64(address + 288); }
  void set_unknown_288(uint64_t v) const { guest::StoreU64(address + 288, v); }
  uint32_t unknown_288_address() const { return address + 288; }
  uint64_t unknown_296() const { return guest::LoadU64(address + 296); }
  void set_unknown_296(uint64_t v) const { guest::StoreU64(address + 296, v); }
  uint32_t unknown_296_address() const { return address + 296; }
  uint64_t unknown_304() const { return guest::LoadU64(address + 304); }
  void set_unknown_304(uint64_t v) const { guest::StoreU64(address + 304, v); }
  uint32_t unknown_304_address() const { return address + 304; }
  uint32_t unknown_312() const { return guest::LoadU32(address + 312); }
  void set_unknown_312(uint32_t v) const { guest::StoreU32(address + 312, v); }
  uint32_t unknown_312_address() const { return address + 312; }
  uint32_t unknown_316() const { return guest::LoadU32(address + 316); }
  void set_unknown_316(uint32_t v) const { guest::StoreU32(address + 316, v); }
  uint32_t unknown_316_address() const { return address + 316; }
  uint32_t unknown_320() const { return guest::LoadU32(address + 320); }
  void set_unknown_320(uint32_t v) const { guest::StoreU32(address + 320, v); }
  uint32_t unknown_320_address() const { return address + 320; }
  uint32_t unknown_324() const { return guest::LoadU32(address + 324); }
  void set_unknown_324(uint32_t v) const { guest::StoreU32(address + 324, v); }
  uint32_t unknown_324_address() const { return address + 324; }
  uint32_t unknown_328() const { return guest::LoadU32(address + 328); }
  void set_unknown_328(uint32_t v) const { guest::StoreU32(address + 328, v); }
  uint32_t unknown_328_address() const { return address + 328; }
  uint32_t unknown_332() const { return guest::LoadU32(address + 332); }
  void set_unknown_332(uint32_t v) const { guest::StoreU32(address + 332, v); }
  uint32_t unknown_332_address() const { return address + 332; }
  float unknown_336() const { return guest::LoadFloat(address + 336); }
  void set_unknown_336(float v) const { guest::StoreFloat(address + 336, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  float unknown_340() const { return guest::LoadFloat(address + 340); }
  void set_unknown_340(float v) const { guest::StoreFloat(address + 340, v); }
  uint32_t unknown_340_address() const { return address + 340; }
};

// embedded; measure: new<?>@sub_82487660
struct MovieLibNew82487660 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint64_t unknown_48() const { return guest::LoadU64(address + 48); }
  void set_unknown_48(uint64_t v) const { guest::StoreU64(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
};

// embedded; measure: new<?>@sub_824878B0
struct MovieLibNew824878B0 : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: new<?>@sub_82487BE8
struct MovieLibNew82487BE8 : View {
  using View::View;
  static constexpr uint32_t kSize = 136;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint64_t unknown_56() const { return guest::LoadU64(address + 56); }
  void set_unknown_56(uint64_t v) const { guest::StoreU64(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint64_t unknown_64() const { return guest::LoadU64(address + 64); }
  void set_unknown_64(uint64_t v) const { guest::StoreU64(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint64_t unknown_72() const { return guest::LoadU64(address + 72); }
  void set_unknown_72(uint64_t v) const { guest::StoreU64(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint64_t unknown_80() const { return guest::LoadU64(address + 80); }
  void set_unknown_80(uint64_t v) const { guest::StoreU64(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint64_t unknown_88() const { return guest::LoadU64(address + 88); }
  void set_unknown_88(uint64_t v) const { guest::StoreU64(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  uint64_t unknown_96() const { return guest::LoadU64(address + 96); }
  void set_unknown_96(uint64_t v) const { guest::StoreU64(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  uint32_t unknown_104() const { return guest::LoadU32(address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t unknown_108() const { return guest::LoadU32(address + 108); }
  void set_unknown_108(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  uint32_t unknown_112() const { return guest::LoadU32(address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  uint32_t unknown_120() const { return guest::LoadU32(address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  uint32_t unknown_124() const { return guest::LoadU32(address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  uint32_t unknown_128() const { return guest::LoadU32(address + 128); }
  void set_unknown_128(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  uint32_t unknown_132() const { return guest::LoadU32(address + 132); }
  void set_unknown_132(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
};

// embedded; measure: VertexBuffer::Lock.ret
struct MovieLibLockedVertices : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint16_t unknown_0() const { return guest::LoadU16(address + 0); }
  void set_unknown_0(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_2() const { return guest::LoadU16(address + 2); }
  void set_unknown_2(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint16_t unknown_4() const { return guest::LoadU16(address + 4); }
  void set_unknown_4(uint16_t v) const { guest::StoreU16(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint16_t unknown_6() const { return guest::LoadU16(address + 6); }
  void set_unknown_6(uint16_t v) const { guest::StoreU16(address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint16_t unknown_8() const { return guest::LoadU16(address + 8); }
  void set_unknown_8(uint16_t v) const { guest::StoreU16(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint16_t unknown_10() const { return guest::LoadU16(address + 10); }
  void set_unknown_10(uint16_t v) const { guest::StoreU16(address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
};

// embedded; measure: new<?>@sub_82483460, sub_82483770.r3
struct MovieLibNew82483460 : View {
  using View::View;
  static constexpr uint32_t kSize = 1488;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_1468() const { return guest::LoadU32(address + 1468); }
  void set_unknown_1468(uint32_t v) const { guest::StoreU32(address + 1468, v); }
  uint32_t unknown_1468_address() const { return address + 1468; }
  uint32_t unknown_1472() const { return guest::LoadU32(address + 1472); }
  void set_unknown_1472(uint32_t v) const { guest::StoreU32(address + 1472, v); }
  uint32_t unknown_1472_address() const { return address + 1472; }
  uint32_t unknown_1476() const { return guest::LoadU32(address + 1476); }
  void set_unknown_1476(uint32_t v) const { guest::StoreU32(address + 1476, v); }
  uint32_t unknown_1476_address() const { return address + 1476; }
  uint32_t unknown_1480() const { return guest::LoadU32(address + 1480); }
  void set_unknown_1480(uint32_t v) const { guest::StoreU32(address + 1480, v); }
  uint32_t unknown_1480_address() const { return address + 1480; }
  uint32_t unknown_1484() const { return guest::LoadU32(address + 1484); }
  void set_unknown_1484(uint32_t v) const { guest::StoreU32(address + 1484, v); }
  uint32_t unknown_1484_address() const { return address + 1484; }
};

// embedded; measure: new<?>@sub_824847D0.+136
struct MovieLibNew824847D0At136 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
};

// embedded; measure: new<?>@sub_82485460.+124
struct MovieLibNew82485460At124 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
};

// embedded; measure: new<?>@sub_82487660.+0
struct MovieLibNew82487660At0 : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
};

// embedded; measure: new<?>@sub_824878B0.+0
struct MovieLibNew824878B0At0 : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
};

// embedded; measure: new<?>@sub_82487BE8.+0
struct MovieLibNew82487BE8At0 : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
};

// embedded; measure: sub_82481B98.r4
struct MovieLib82481B98R4 : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
};

// embedded; measure: sub_82481B98.r7
struct MovieLib82481B98R7 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_82482068.r4
struct MovieLib82482068R4 : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  // also read 8 bytes wide
  uint64_t unknown_40_u64() const { return guest::LoadU64(address + 40); }
  void set_unknown_40_u64(uint64_t v) const { guest::StoreU64(address + 40, v); }
  uint32_t unknown_40_u64_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint64_t unknown_48() const { return guest::LoadU64(address + 48); }
  void set_unknown_48(uint64_t v) const { guest::StoreU64(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
};

// embedded; measure: sub_82482068.r5
struct MovieLib82482068R5 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_82482780.r3
struct MovieLib82482780R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 288;
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t unknown_80() const { return guest::LoadU32(address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint32_t unknown_84() const { return guest::LoadU32(address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  uint32_t unknown_88() const { return guest::LoadU32(address + 88); }
  void set_unknown_88(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  uint32_t unknown_92() const { return guest::LoadU32(address + 92); }
  void set_unknown_92(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  uint32_t unknown_100() const { return guest::LoadU32(address + 100); }
  void set_unknown_100(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  uint32_t unknown_104() const { return guest::LoadU32(address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t unknown_108() const { return guest::LoadU32(address + 108); }
  void set_unknown_108(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  uint32_t unknown_112() const { return guest::LoadU32(address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  uint32_t unknown_120() const { return guest::LoadU32(address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  uint32_t unknown_124() const { return guest::LoadU32(address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  uint32_t unknown_128() const { return guest::LoadU32(address + 128); }
  void set_unknown_128(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  float unknown_132() const { return guest::LoadFloat(address + 132); }
  void set_unknown_132(float v) const { guest::StoreFloat(address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  float unknown_136() const { return guest::LoadFloat(address + 136); }
  void set_unknown_136(float v) const { guest::StoreFloat(address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  float unknown_140() const { return guest::LoadFloat(address + 140); }
  void set_unknown_140(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  uint32_t unknown_264() const { return guest::LoadU32(address + 264); }
  void set_unknown_264(uint32_t v) const { guest::StoreU32(address + 264, v); }
  uint32_t unknown_264_address() const { return address + 264; }
  uint32_t unknown_268() const { return guest::LoadU32(address + 268); }
  void set_unknown_268(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t unknown_268_address() const { return address + 268; }
  uint32_t unknown_272() const { return guest::LoadU32(address + 272); }
  void set_unknown_272(uint32_t v) const { guest::StoreU32(address + 272, v); }
  uint32_t unknown_272_address() const { return address + 272; }
  uint32_t unknown_276() const { return guest::LoadU32(address + 276); }
  void set_unknown_276(uint32_t v) const { guest::StoreU32(address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  uint32_t unknown_280() const { return guest::LoadU32(address + 280); }
  void set_unknown_280(uint32_t v) const { guest::StoreU32(address + 280, v); }
  uint32_t unknown_280_address() const { return address + 280; }
  uint32_t unknown_284() const { return guest::LoadU32(address + 284); }
  void set_unknown_284(uint32_t v) const { guest::StoreU32(address + 284, v); }
  uint32_t unknown_284_address() const { return address + 284; }
};

// embedded; measure: sub_82482B18.r3
struct MovieLib82482B18R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  float unknown_132() const { return guest::LoadFloat(address + 132); }
  void set_unknown_132(float v) const { guest::StoreFloat(address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  float unknown_136() const { return guest::LoadFloat(address + 136); }
  void set_unknown_136(float v) const { guest::StoreFloat(address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  float unknown_140() const { return guest::LoadFloat(address + 140); }
  void set_unknown_140(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
};

// embedded; measure: sub_82482B18.r3.+0
struct MovieLib82482B18R3At0 : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
};

// embedded; measure: sub_82482B18.r3.+56
struct MovieLib82482B18R3At56 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// embedded; measure: sub_82482CA8.r3
struct MovieLib82482CA8R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
};

// embedded; measure: sub_82482DA8.r3
struct MovieLib82482DA8R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 252;
  uint32_t unknown_80() const { return guest::LoadU32(address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint32_t unknown_84() const { return guest::LoadU32(address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  uint32_t unknown_88() const { return guest::LoadU32(address + 88); }
  void set_unknown_88(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  uint32_t unknown_92() const { return guest::LoadU32(address + 92); }
  void set_unknown_92(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  uint32_t unknown_100() const { return guest::LoadU32(address + 100); }
  void set_unknown_100(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  uint32_t unknown_104() const { return guest::LoadU32(address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t unknown_108() const { return guest::LoadU32(address + 108); }
  void set_unknown_108(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  uint32_t unknown_112() const { return guest::LoadU32(address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  float unknown_144() const { return guest::LoadFloat(address + 144); }
  void set_unknown_144(float v) const { guest::StoreFloat(address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  float unknown_148() const { return guest::LoadFloat(address + 148); }
  void set_unknown_148(float v) const { guest::StoreFloat(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  float unknown_164() const { return guest::LoadFloat(address + 164); }
  void set_unknown_164(float v) const { guest::StoreFloat(address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  float unknown_168() const { return guest::LoadFloat(address + 168); }
  void set_unknown_168(float v) const { guest::StoreFloat(address + 168, v); }
  uint32_t unknown_168_address() const { return address + 168; }
  float unknown_184() const { return guest::LoadFloat(address + 184); }
  void set_unknown_184(float v) const { guest::StoreFloat(address + 184, v); }
  uint32_t unknown_184_address() const { return address + 184; }
  float unknown_188() const { return guest::LoadFloat(address + 188); }
  void set_unknown_188(float v) const { guest::StoreFloat(address + 188, v); }
  uint32_t unknown_188_address() const { return address + 188; }
  float unknown_204() const { return guest::LoadFloat(address + 204); }
  void set_unknown_204(float v) const { guest::StoreFloat(address + 204, v); }
  uint32_t unknown_204_address() const { return address + 204; }
  float unknown_208() const { return guest::LoadFloat(address + 208); }
  void set_unknown_208(float v) const { guest::StoreFloat(address + 208, v); }
  uint32_t unknown_208_address() const { return address + 208; }
  float unknown_224() const { return guest::LoadFloat(address + 224); }
  void set_unknown_224(float v) const { guest::StoreFloat(address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  float unknown_228() const { return guest::LoadFloat(address + 228); }
  void set_unknown_228(float v) const { guest::StoreFloat(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  float unknown_244() const { return guest::LoadFloat(address + 244); }
  void set_unknown_244(float v) const { guest::StoreFloat(address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  float unknown_248() const { return guest::LoadFloat(address + 248); }
  void set_unknown_248(float v) const { guest::StoreFloat(address + 248, v); }
  uint32_t unknown_248_address() const { return address + 248; }
};

// embedded; measure: sub_82483130.r3
struct MovieLib82483130R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: sub_82483130.r3.+0
struct MovieLib82483130R3At0 : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
};

// embedded; measure: sub_82483770.r3.+0
struct MovieLib82483770R3At0 : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
};

// embedded; measure: sub_82483770.r4
struct MovieLib82483770R4 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: sub_82489048.r3
struct MovieLib82489048R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  template <typename T = MovieLib82489048R3>
  T unknown_4() const { return T(guest::LoadU32(address + 4)); }
  void set_unknown_4(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  template <typename T = MovieLib82489048R3>
  T unknown_8() const { return T(guest::LoadU32(address + 8)); }
  void set_unknown_8(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: sub_824892A8.r3
struct MovieLib824892A8R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// embedded; measure: sub_824892D0.r3
struct MovieLib824892D0R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// embedded; measure: sub_82489300.r3
struct MovieLib82489300R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// embedded; measure: sub_8248CB58.r3
struct MovieLib8248CB58R3 : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_112() const { return guest::LoadU32(address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  uint32_t unknown_120() const { return guest::LoadU32(address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
};

// embedded; measure: Movie@+48
#pragma pack(push, 1)
struct MovieAt48Data {
  uint8_t unknown_0 = 0;  // +0
  uint8_t unknown_1 = 0;  // +1
};
#pragma pack(pop)
static_assert(sizeof(MovieAt48Data) == 2);
inline constexpr memory::Layout kMovieAt48Layout = {nullptr, 0};

struct MovieAt48 : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  MovieAt48Data* data() const { return static_cast<MovieAt48Data*>(memory::HandleObject(address)); }
  static MovieAt48 New() {
    return MovieAt48(memory::MakeHandle(new MovieAt48Data(), sizeof(MovieAt48Data), "MovieAt48", &kMovieAt48Layout));
  }
  template <uint32_t Stride = kSize>
  static MovieAt48 NewArray(uint32_t count) {
    static_assert(sizeof(MovieAt48Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MovieAt48Data*>(::operator new(sizeof(MovieAt48Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MovieAt48Data();
    return MovieAt48(memory::MakeHandle(p, sizeof(MovieAt48Data) * count, "MovieAt48", &kMovieAt48Layout, sizeof(MovieAt48Data)));
  }
  MovieAt48 at(uint32_t i) const { return MovieAt48(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  MovieAt48Data* direct() const { return static_cast<MovieAt48Data*>(memory::HandleOwn(address, &kMovieAt48Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kMovieAt48Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kMovieAt48Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint8_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(address + 0))); }
  void set_unknown_0_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(address + 0, v); }
  uint8_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(address + 0))); }
  void set_unknown_0(uint8_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(address + 0, v); }
  uint32_t unknown_1_address() const { return address + 1; }
  uint8_t unknown_1_bits() const { if (auto* d = direct()) return d->unknown_1; return uint8_t((memory::IsHandle(address + 1) ? uint8_t(memory::OwnerLoad(address + 1, 1)) : guest::LoadU8(address + 1))); }
  void set_unknown_1_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_1 = v; return; } if (memory::IsHandle(address + 1)) memory::OwnerStore(address + 1, 1, v); else guest::StoreU8(address + 1, v); }
  uint8_t unknown_1() const { if (auto* d = direct()) return d->unknown_1; return uint8_t((memory::IsHandle(address + 1) ? uint8_t(memory::OwnerLoad(address + 1, 1)) : guest::LoadU8(address + 1))); }
  void set_unknown_1(uint8_t v) const { if (auto* d = direct()) { d->unknown_1 = v; return; } if (memory::IsHandle(address + 1)) memory::OwnerStore(address + 1, 1, v); else guest::StoreU8(address + 1, v); }
};

// embedded; measure: new<?>@sub_824847D0@+136
struct MovieLibNew824847D0At136_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  float unknown_72() const { return guest::LoadFloat(address + 72); }
  void set_unknown_72(float v) const { guest::StoreFloat(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
};

// embedded; measure: new<?>@sub_824847D0@+212
struct MovieLibNew824847D0At212 : View {
  using View::View;
  static constexpr uint32_t kSize = 288;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_284() const { return guest::LoadU32(address + 284); }
  void set_unknown_284(uint32_t v) const { guest::StoreU32(address + 284, v); }
  uint32_t unknown_284_address() const { return address + 284; }
};

// embedded; measure: new<?>@sub_82485460@+124
struct MovieLibNew82485460At124_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  float unknown_80() const { return guest::LoadFloat(address + 80); }
  void set_unknown_80(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
};

// embedded; measure: new<?>@sub_82485460@+208
struct MovieLibNew82485460At208 : View {
  using View::View;
  static constexpr uint32_t kSize = 452;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_444() const { return guest::LoadU32(address + 444); }
  void set_unknown_444(uint32_t v) const { guest::StoreU32(address + 444, v); }
  uint32_t unknown_444_address() const { return address + 444; }
  uint32_t unknown_448() const { return guest::LoadU32(address + 448); }
  void set_unknown_448(uint32_t v) const { guest::StoreU32(address + 448, v); }
  uint32_t unknown_448_address() const { return address + 448; }
};

// The image words docs/functions/movies.md's code uses.
struct MoviesGlobals {
  // the movie class, stored by Movies::Set (the app + 128), cleared by Movies::Clear
  template <typename T = Movie>
  static T movie_class() { return T(guest::LoadU32(0x826C1CDCu)); }
  static void set_movie_class(const View& v) { guest::StoreU32(0x826C1CDCu, v.address); }
  static void set_movie_class(uint32_t v) { guest::StoreU32(0x826C1CDCu, v); }
  static constexpr uint32_t movie_class_address() { return 0x826C1CDCu; }
};

}  // namespace schema
