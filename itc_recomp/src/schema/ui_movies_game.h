// The views of docs/functions/ui-movies-game.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/ui-movies-game.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<4096>@UiMovie::GetText
struct UiMovieTextBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 4096;
  static UiMovieTextBuffer New(uint32_t size) { return UiMovieTextBuffer(memory::NewBlob(size, "UiMovieTextBuffer")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  // the first character
  uint16_t char_0() const { return guest::LoadU16(address + 0); }
  void set_char_0(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t char_0_address() const { return address + 0; }
  uint16_t char_1() const { return guest::LoadU16(address + 2); }
  void set_char_1(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t char_1_address() const { return address + 2; }
};

// allocated; measure: -
struct UiMovieArchiveFile : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static UiMovieArchiveFile New(uint32_t size) { return UiMovieArchiveFile(memory::NewBlob(size, "UiMovieArchiveFile")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// allocated; measure: new<28>@UiMovie::GetText, TextMap::Find.ret.+0, sub_8211AC90.r3.+39888
#pragma pack(push, 1)
struct UiMovieTextData {
  uint32_t buffer = 0;  // +0
  uint32_t scrolling = 0;  // +4
  uint32_t scroll_counter = 0;  // +8
  uint32_t scroll_delay = 0;  // +12
  float scroll_speed = 0;  // +16
  uint32_t page = 0;  // +20
  uint32_t first_line = 0;  // +24
};
#pragma pack(pop)
static_assert(sizeof(UiMovieTextData) == 28);
inline constexpr memory::LayoutField kUiMovieTextFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}};
inline constexpr memory::Layout kUiMovieTextLayout = {kUiMovieTextFields, 7};

struct UiMovieText : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  UiMovieTextData* data() const { return static_cast<UiMovieTextData*>(memory::HandleObject(address)); }
  static UiMovieText New() {
    return UiMovieText(memory::MakeHandle(new UiMovieTextData(), sizeof(UiMovieTextData), "UiMovieText", &kUiMovieTextLayout));
  }
  template <uint32_t Stride = kSize>
  static UiMovieText NewArray(uint32_t count) {
    static_assert(sizeof(UiMovieTextData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<UiMovieTextData*>(::operator new(sizeof(UiMovieTextData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) UiMovieTextData();
    return UiMovieText(memory::MakeHandle(p, sizeof(UiMovieTextData) * count, "UiMovieText", &kUiMovieTextLayout, sizeof(UiMovieTextData)));
  }
  UiMovieText at(uint32_t i) const { return UiMovieText(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  UiMovieTextData* direct() const { return static_cast<UiMovieTextData*>(memory::HandleOwn(address, &kUiMovieTextLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kUiMovieTextLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kUiMovieTextLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a 4 KB UTF-16 buffer (pages from sub_82295398)
  uint32_t buffer_address() const { return address + 0; }
  uint32_t buffer_bits() const { if (auto* d = direct()) return d->buffer; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->buffer = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = UiMovieTextBuffer>
  T buffer() const { return T(buffer_bits()); }
  void set_buffer(const View& w) const { set_buffer_bits(w.address); }
  void set_buffer(uint32_t v) const { set_buffer_bits(v); }
  uint32_t scrolling_address() const { return address + 4; }
  uint32_t scrolling_bits() const { if (auto* d = direct()) return d->scrolling; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_scrolling_bits(uint32_t v) const { if (auto* d = direct()) { d->scrolling = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t scrolling() const { if (auto* d = direct()) return d->scrolling; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_scrolling(uint32_t v) const { if (auto* d = direct()) { d->scrolling = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // ticks
  uint32_t scroll_counter_address() const { return address + 8; }
  uint32_t scroll_counter_bits() const { if (auto* d = direct()) return d->scroll_counter; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_scroll_counter_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_counter = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t scroll_counter() const { if (auto* d = direct()) return d->scroll_counter; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_scroll_counter(uint32_t v) const { if (auto* d = direct()) { d->scroll_counter = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // signed
  uint32_t scroll_delay_address() const { return address + 12; }
  uint32_t scroll_delay_bits() const { if (auto* d = direct()) return d->scroll_delay; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_scroll_delay_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_delay = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t scroll_delay() const { if (auto* d = direct()) return d->scroll_delay; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_scroll_delay(uint32_t v) const { if (auto* d = direct()) { d->scroll_delay = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t scroll_speed_address() const { return address + 16; }
  uint32_t scroll_speed_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->scroll_speed); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_scroll_speed_bits(uint32_t v) const { if (auto* d = direct()) { d->scroll_speed = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float scroll_speed() const { return std::bit_cast<float>(scroll_speed_bits()); }
  void set_scroll_speed(float v) const { set_scroll_speed_bits(std::bit_cast<uint32_t>(v)); }
  // scroll_speed as a word (ScrollText stores the default's word)
  uint32_t scroll_speed_word_address() const { return address + 16; }
  uint32_t scroll_speed_word_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kUiMovieTextLayout, 16, 4)); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_scroll_speed_word_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kUiMovieTextLayout, 16, 4, v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t scroll_speed_word() const { return uint32_t(scroll_speed_word_bits()); }
  void set_scroll_speed_word(uint32_t v) const { set_scroll_speed_word_bits(uint32_t(v)); }
  // the page shown (probably: the characters shown)
  uint32_t page_address() const { return address + 20; }
  uint32_t page_bits() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_page_bits(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t page() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_page(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // passed to the formatting (UiMovie::SetFirstLine, FirstLine)
  uint32_t first_line_address() const { return address + 24; }
  uint32_t first_line_bits() const { if (auto* d = direct()) return d->first_line; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_first_line_bits(uint32_t v) const { if (auto* d = direct()) { d->first_line = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t first_line() const { if (auto* d = direct()) return d->first_line; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_first_line(uint32_t v) const { if (auto* d = direct()) { d->first_line = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
};

// embedded; measure: TextItem, sub_8211AED0.r3, sub_8211AED0.r4, sub_8259E620.r4, UiMovie.+40, sub_8259E620.r3
struct UiMovieTextItem : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // the first item's: operator new[]'s count (512) in the array's 16-byte header
  uint32_t array_count() const { return guest::LoadU32(address + -16); }
  void set_array_count(uint32_t v) const { guest::StoreU32(address + -16, v); }
  uint32_t array_count_address() const { return address + -16; }
  // the TXT node drawn
  uint32_t node() const { return guest::LoadU32(address + 0); }
  void set_node(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t node_address() const { return address + 0; }
  // a 4x4 matrix (the identity when made)
  uint32_t matrix_address() const { return address + 16; }
  float matrix_3() const { return guest::LoadFloat(address + 28); }
  void set_matrix_3(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t matrix_3_address() const { return address + 28; }
  float matrix_7() const { return guest::LoadFloat(address + 44); }
  void set_matrix_7(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t matrix_7_address() const { return address + 44; }
  float matrix_11() const { return guest::LoadFloat(address + 60); }
  void set_matrix_11(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t matrix_11_address() const { return address + 60; }
  float matrix_15() const { return guest::LoadFloat(address + 76); }
  void set_matrix_15(float v) const { guest::StoreFloat(address + 76, v); }
  uint32_t matrix_15_address() const { return address + 76; }
  // a float, 0.0 when made (copied as words)
  uint32_t depth() const { return guest::LoadU32(address + 80); }
  void set_depth(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t depth_address() const { return address + 80; }
  // the color's scales (+84 to +96), floats copied as words; 0.0 when made
  uint32_t red() const { return guest::LoadU32(address + 84); }
  void set_red(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t red_address() const { return address + 84; }
  uint32_t green() const { return guest::LoadU32(address + 88); }
  void set_green(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t green_address() const { return address + 88; }
  uint32_t blue() const { return guest::LoadU32(address + 92); }
  void set_blue(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t blue_address() const { return address + 92; }
  uint32_t alpha() const { return guest::LoadU32(address + 96); }
  void set_alpha(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t alpha_address() const { return address + 96; }
  // the clip rectangle (+100 to +112)
  uint32_t clip_0() const { return guest::LoadU32(address + 100); }
  void set_clip_0(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t clip_0_address() const { return address + 100; }
  uint32_t clip_1() const { return guest::LoadU32(address + 104); }
  void set_clip_1(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t clip_1_address() const { return address + 104; }
  uint32_t clip_2() const { return guest::LoadU32(address + 108); }
  void set_clip_2(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t clip_2_address() const { return address + 108; }
  uint32_t clip_3() const { return guest::LoadU32(address + 112); }
  void set_clip_3(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t clip_3_address() const { return address + 112; }
};

// allocated; measure: new<65552>@sub_8211B018
struct UiMovieTextItemArray : View {
  using View::View;
  static constexpr uint32_t kSize = 65552;
  static UiMovieTextItemArray New(uint32_t size) { return UiMovieTextItemArray(memory::NewBlob(size, "UiMovieTextItemArray")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  // 512
  uint32_t count() const { return guest::LoadU32(address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t count_address() const { return address + 0; }
};

// allocated; measure: UiMovie, sub_8211A9E8.r3, sub_8211AC90.r3, sub_8211E378.r3, sub_8211C4F0.r3, sub_8211C5F8.r3, UiMovie::OnLoaded.r6, UiMovies::Find.ret.+0, AnimationList::Get.r4, RaceHud.+604
#pragma pack(push, 1)
struct UiMovieData {
  uint32_t file_0 = 0;  // +0
  uint32_t file_1 = 0;  // +4
  uint32_t file_2 = 0;  // +8
  uint32_t texture_set = 0;  // +12
  uint32_t live_scene = 0;  // +16
  uint32_t saved_scene = 0;  // +20
  uint32_t file_scene = 0;  // +24
  uint32_t textures = 0;  // +28
  uint32_t texture_capacity = 0;  // +32
  uint32_t texture_count = 0;  // +36
  uint32_t text_items = 0;  // +40
  uint32_t text_item_capacity = 0;  // +44
  uint32_t text_item_count = 0;  // +48
  uint32_t unknown_52 = 0;  // +52
  uint32_t unknown_56 = 0;  // +56
  uint32_t unknown_60 = 0;  // +60
  uint8_t draw_context[112] = {};  // +64, guest order
  uint8_t named_animations[36864] = {};  // +176, guest order
  uint32_t named_count = 0;  // +37040
  uint8_t pad_37044[128] = {};
  uint16_t queue_free = 0;  // +37172
  uint8_t pad_37174[2] = {};
  uint16_t queue_first = 0;  // +37176
  uint8_t pad_37178[2] = {};
  uint16_t queue_count = 0;  // +37180
  uint8_t pad_37182[1410] = {};
  uint16_t looping_free = 0;  // +38592
  uint8_t pad_38594[2] = {};
  uint16_t looping_first = 0;  // +38596
  uint8_t pad_38598[2] = {};
  uint16_t looping_count = 0;  // +38600
  uint8_t pad_38602[1282] = {};
  uint32_t text_0_id = 0;  // +39884
  uint32_t text_0 = 0;  // +39888
  uint8_t pad_39892[1016] = {};
  uint32_t text_count = 0;  // +40908
  uint32_t loaded = 0;  // +40912
  uint32_t id = 0;  // +40916
  uint8_t last_started[40] = {};  // +40920, guest order
  uint8_t pad_40960[16] = {};
};
#pragma pack(pop)
static_assert(sizeof(UiMovieData) == 40976);
inline constexpr memory::LayoutField kUiMovieFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {37040, 4}, {37172, 2}, {37176, 2}, {37180, 2}, {38592, 2}, {38596, 2}, {38600, 2}, {39884, 4}, {39888, 4}, {40908, 4}, {40912, 4}, {40916, 4}};
inline constexpr memory::Layout kUiMovieLayout = {kUiMovieFields, 28};

struct UiMovie : View {
  using View::View;
  static constexpr uint32_t kSize = 40976;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  UiMovieData* data() const { return static_cast<UiMovieData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static UiMovie New() {
    return UiMovie(memory::MakeHandle(new UiMovieData(), sizeof(UiMovieData), "UiMovie", &kUiMovieLayout));
  }
  template <uint32_t Stride = kSize>
  static UiMovie NewArray(uint32_t count) {
    static_assert(sizeof(UiMovieData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<UiMovieData*>(::operator new(sizeof(UiMovieData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) UiMovieData();
    return UiMovie(memory::MakeHandle(p, sizeof(UiMovieData) * count, "UiMovie", &kUiMovieLayout, sizeof(UiMovieData)));
  }
  UiMovie at(uint32_t i) const { return UiMovie(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  UiMovieData* direct() const { return static_cast<UiMovieData*>(memory::HandleOwn(address, &kUiMovieLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kUiMovieLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kUiMovieLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the first of three copies of its movie file (pages from sub_82295398); the live scene's
  uint32_t file_0_address() const { return address + 0; }
  uint32_t file_0_bits() const { if (auto* d = direct()) return d->file_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_file_0_bits(uint32_t v) const { if (auto* d = direct()) { d->file_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t file_0() const { if (auto* d = direct()) return d->file_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_file_0(uint32_t v) const { if (auto* d = direct()) { d->file_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the second copy (the saved scene's)
  uint32_t file_1_address() const { return address + 4; }
  uint32_t file_1_bits() const { if (auto* d = direct()) return d->file_1; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_file_1_bits(uint32_t v) const { if (auto* d = direct()) { d->file_1 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t file_1() const { if (auto* d = direct()) return d->file_1; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_file_1(uint32_t v) const { if (auto* d = direct()) { d->file_1 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the third copy (the file's own scene, for resets)
  uint32_t file_2_address() const { return address + 8; }
  uint32_t file_2_bits() const { if (auto* d = direct()) return d->file_2; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_file_2_bits(uint32_t v) const { if (auto* d = direct()) { d->file_2 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t file_2() const { if (auto* d = direct()) return d->file_2; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_file_2(uint32_t v) const { if (auto* d = direct()) { d->file_2 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // its texture set (24 bytes, the XPR, sub_82596E68)
  uint32_t texture_set_address() const { return address + 12; }
  uint32_t texture_set_bits() const { if (auto* d = direct()) return d->texture_set; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_texture_set_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_set = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t texture_set() const { if (auto* d = direct()) return d->texture_set; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_texture_set(uint32_t v) const { if (auto* d = direct()) { d->texture_set = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the live scene (animated and drawn)
  uint32_t live_scene_address() const { return address + 16; }
  uint32_t live_scene_bits() const { if (auto* d = direct()) return d->live_scene; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_live_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->live_scene = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t live_scene() const { if (auto* d = direct()) return d->live_scene; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_live_scene(uint32_t v) const { if (auto* d = direct()) { d->live_scene = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // the live pose saved at each update's start
  uint32_t saved_scene_address() const { return address + 20; }
  uint32_t saved_scene_bits() const { if (auto* d = direct()) return d->saved_scene; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_saved_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->saved_scene = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t saved_scene() const { if (auto* d = direct()) return d->saved_scene; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_saved_scene(uint32_t v) const { if (auto* d = direct()) { d->saved_scene = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the file's scene (for resets)
  uint32_t file_scene_address() const { return address + 24; }
  uint32_t file_scene_bits() const { if (auto* d = direct()) return d->file_scene; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_file_scene_bits(uint32_t v) const { if (auto* d = direct()) { d->file_scene = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t file_scene() const { if (auto* d = direct()) return d->file_scene; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_file_scene(uint32_t v) const { if (auto* d = direct()) { d->file_scene = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  // the textures' word vector (UiMovieWordVector): its data, 12-byte entries' addresses
  uint32_t textures_address() const { return address + 28; }
  uint32_t textures_bits() const { if (auto* d = direct()) return d->textures; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_textures_bits(uint32_t v) const { if (auto* d = direct()) { d->textures = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t textures() const { if (auto* d = direct()) return d->textures; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_textures(uint32_t v) const { if (auto* d = direct()) { d->textures = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  // 512
  uint32_t texture_capacity_address() const { return address + 32; }
  uint32_t texture_capacity_bits() const { if (auto* d = direct()) return d->texture_capacity; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_texture_capacity_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_capacity = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t texture_capacity() const { if (auto* d = direct()) return d->texture_capacity; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_texture_capacity(uint32_t v) const { if (auto* d = direct()) { d->texture_capacity = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t texture_count_address() const { return address + 36; }
  uint32_t texture_count_bits() const { if (auto* d = direct()) return d->texture_count; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_texture_count_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_count = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t texture_count() const { if (auto* d = direct()) return d->texture_count; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_texture_count(uint32_t v) const { if (auto* d = direct()) { d->texture_count = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  // the text list (ui-movies.toml's UiMovieDrawTextList): its items, 128 bytes each, 512 made
  uint32_t text_items_address() const { return address + 40; }
  uint32_t text_items_bits() const { if (auto* d = direct()) return d->text_items; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_text_items_bits(uint32_t v) const { if (auto* d = direct()) { d->text_items = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = UiMovieTextItem>
  T text_items() const { return T(text_items_bits()); }
  void set_text_items(const View& w) const { set_text_items_bits(w.address); }
  void set_text_items(uint32_t v) const { set_text_items_bits(v); }
  uint32_t text_item_capacity_address() const { return address + 44; }
  uint32_t text_item_capacity_bits() const { if (auto* d = direct()) return d->text_item_capacity; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_text_item_capacity_bits(uint32_t v) const { if (auto* d = direct()) { d->text_item_capacity = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t text_item_capacity() const { if (auto* d = direct()) return d->text_item_capacity; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_text_item_capacity(uint32_t v) const { if (auto* d = direct()) { d->text_item_capacity = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t text_item_count_address() const { return address + 48; }
  uint32_t text_item_count_bits() const { if (auto* d = direct()) return d->text_item_count; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_text_item_count_bits(uint32_t v) const { if (auto* d = direct()) { d->text_item_count = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t text_item_count() const { if (auto* d = direct()) return d->text_item_count; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_text_item_count(uint32_t v) const { if (auto* d = direct()) { d->text_item_count = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // measure fault: sub_825A0E40 reads a movie file's X2D and 005 tags (+8 to +15) through a file it takes for a movie
  uint32_t fault_file_tags_address() const { return address + 8; }
  uint8_t* fault_file_tags() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 8 : guest::Pointer(address + 8); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_52_bits() const { if (auto* d = direct()) return d->unknown_52; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_unknown_52_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_52 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t unknown_52() const { if (auto* d = direct()) return d->unknown_52; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_unknown_52(uint32_t v) const { if (auto* d = direct()) { d->unknown_52 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_56_bits() const { if (auto* d = direct()) return d->unknown_56; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_unknown_56_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_56 = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t unknown_56() const { if (auto* d = direct()) return d->unknown_56; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_unknown_56(uint32_t v) const { if (auto* d = direct()) { d->unknown_56 = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_60_bits() const { if (auto* d = direct()) return d->unknown_60; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_unknown_60_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_60 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t unknown_60() const { if (auto* d = direct()) return d->unknown_60; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_unknown_60(uint32_t v) const { if (auto* d = direct()) { d->unknown_60 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // the draw context (known-data.md: UI movie draw context)
  uint32_t draw_context_address() const { return address + 64; }
  uint8_t* draw_context() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 64 : guest::Pointer(address + 64); }
  // 1,024 UiMovieNamedAnimation entries of 36 bytes
  uint32_t named_animations_address() const { return address + 176; }
  uint8_t* named_animations() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 176 : guest::Pointer(address + 176); }
  // the named animations in use
  uint32_t named_count_address() const { return address + 37040; }
  uint32_t named_count_bits() const { if (auto* d = direct()) return d->named_count; return uint32_t((memory::IsHandle(address + 37040) ? uint32_t(memory::OwnerLoad(address + 37040, 4)) : guest::LoadU32(address + 37040))); }
  void set_named_count_bits(uint32_t v) const { if (auto* d = direct()) { d->named_count = v; return; } if (memory::IsHandle(address + 37040)) memory::OwnerStore(address + 37040, 4, v); else guest::StoreU32(address + 37040, v); }
  uint32_t named_count() const { if (auto* d = direct()) return d->named_count; return uint32_t((memory::IsHandle(address + 37040) ? uint32_t(memory::OwnerLoad(address + 37040, 4)) : guest::LoadU32(address + 37040))); }
  void set_named_count(uint32_t v) const { if (auto* d = direct()) { d->named_count = v; return; } if (memory::IsHandle(address + 37040)) memory::OwnerStore(address + 37040, 4, v); else guest::StoreU32(address + 37040, v); }
  // the animation queue (a UiMovieAnimationList), played in chained groups
  uint32_t queue_address() const { return address + 37044; }
  uint8_t* queue() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 37044 : guest::Pointer(address + 37044); }
  // the queue's free chain's first link
  uint32_t queue_free_address() const { return address + 37172; }
  uint16_t queue_free_bits() const { if (auto* d = direct()) return d->queue_free; return uint16_t((memory::IsHandle(address + 37172) ? uint16_t(memory::OwnerLoad(address + 37172, 2)) : guest::LoadU16(address + 37172))); }
  void set_queue_free_bits(uint16_t v) const { if (auto* d = direct()) { d->queue_free = v; return; } if (memory::IsHandle(address + 37172)) memory::OwnerStore(address + 37172, 2, v); else guest::StoreU16(address + 37172, v); }
  uint16_t queue_free() const { if (auto* d = direct()) return d->queue_free; return uint16_t((memory::IsHandle(address + 37172) ? uint16_t(memory::OwnerLoad(address + 37172, 2)) : guest::LoadU16(address + 37172))); }
  void set_queue_free(uint16_t v) const { if (auto* d = direct()) { d->queue_free = v; return; } if (memory::IsHandle(address + 37172)) memory::OwnerStore(address + 37172, 2, v); else guest::StoreU16(address + 37172, v); }
  // the queue's first link in use (-1 none)
  uint32_t queue_first_address() const { return address + 37176; }
  uint16_t queue_first_bits() const { if (auto* d = direct()) return d->queue_first; return uint16_t((memory::IsHandle(address + 37176) ? uint16_t(memory::OwnerLoad(address + 37176, 2)) : guest::LoadU16(address + 37176))); }
  void set_queue_first_bits(uint16_t v) const { if (auto* d = direct()) { d->queue_first = v; return; } if (memory::IsHandle(address + 37176)) memory::OwnerStore(address + 37176, 2, v); else guest::StoreU16(address + 37176, v); }
  uint16_t queue_first() const { if (auto* d = direct()) return d->queue_first; return uint16_t((memory::IsHandle(address + 37176) ? uint16_t(memory::OwnerLoad(address + 37176, 2)) : guest::LoadU16(address + 37176))); }
  void set_queue_first(uint16_t v) const { if (auto* d = direct()) { d->queue_first = v; return; } if (memory::IsHandle(address + 37176)) memory::OwnerStore(address + 37176, 2, v); else guest::StoreU16(address + 37176, v); }
  uint32_t queue_count_address() const { return address + 37180; }
  uint16_t queue_count_bits() const { if (auto* d = direct()) return d->queue_count; return uint16_t((memory::IsHandle(address + 37180) ? uint16_t(memory::OwnerLoad(address + 37180, 2)) : guest::LoadU16(address + 37180))); }
  void set_queue_count_bits(uint16_t v) const { if (auto* d = direct()) { d->queue_count = v; return; } if (memory::IsHandle(address + 37180)) memory::OwnerStore(address + 37180, 2, v); else guest::StoreU16(address + 37180, v); }
  uint16_t queue_count() const { if (auto* d = direct()) return d->queue_count; return uint16_t((memory::IsHandle(address + 37180) ? uint16_t(memory::OwnerLoad(address + 37180, 2)) : guest::LoadU16(address + 37180))); }
  void set_queue_count(uint16_t v) const { if (auto* d = direct()) { d->queue_count = v; return; } if (memory::IsHandle(address + 37180)) memory::OwnerStore(address + 37180, 2, v); else guest::StoreU16(address + 37180, v); }
  // the looping animations (a UiMovieAnimationList)
  uint32_t looping_address() const { return address + 38464; }
  uint8_t* looping() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 38464 : guest::Pointer(address + 38464); }
  uint32_t looping_free_address() const { return address + 38592; }
  uint16_t looping_free_bits() const { if (auto* d = direct()) return d->looping_free; return uint16_t((memory::IsHandle(address + 38592) ? uint16_t(memory::OwnerLoad(address + 38592, 2)) : guest::LoadU16(address + 38592))); }
  void set_looping_free_bits(uint16_t v) const { if (auto* d = direct()) { d->looping_free = v; return; } if (memory::IsHandle(address + 38592)) memory::OwnerStore(address + 38592, 2, v); else guest::StoreU16(address + 38592, v); }
  uint16_t looping_free() const { if (auto* d = direct()) return d->looping_free; return uint16_t((memory::IsHandle(address + 38592) ? uint16_t(memory::OwnerLoad(address + 38592, 2)) : guest::LoadU16(address + 38592))); }
  void set_looping_free(uint16_t v) const { if (auto* d = direct()) { d->looping_free = v; return; } if (memory::IsHandle(address + 38592)) memory::OwnerStore(address + 38592, 2, v); else guest::StoreU16(address + 38592, v); }
  uint32_t looping_first_address() const { return address + 38596; }
  uint16_t looping_first_bits() const { if (auto* d = direct()) return d->looping_first; return uint16_t((memory::IsHandle(address + 38596) ? uint16_t(memory::OwnerLoad(address + 38596, 2)) : guest::LoadU16(address + 38596))); }
  void set_looping_first_bits(uint16_t v) const { if (auto* d = direct()) { d->looping_first = v; return; } if (memory::IsHandle(address + 38596)) memory::OwnerStore(address + 38596, 2, v); else guest::StoreU16(address + 38596, v); }
  uint16_t looping_first() const { if (auto* d = direct()) return d->looping_first; return uint16_t((memory::IsHandle(address + 38596) ? uint16_t(memory::OwnerLoad(address + 38596, 2)) : guest::LoadU16(address + 38596))); }
  void set_looping_first(uint16_t v) const { if (auto* d = direct()) { d->looping_first = v; return; } if (memory::IsHandle(address + 38596)) memory::OwnerStore(address + 38596, 2, v); else guest::StoreU16(address + 38596, v); }
  uint32_t looping_count_address() const { return address + 38600; }
  uint16_t looping_count_bits() const { if (auto* d = direct()) return d->looping_count; return uint16_t((memory::IsHandle(address + 38600) ? uint16_t(memory::OwnerLoad(address + 38600, 2)) : guest::LoadU16(address + 38600))); }
  void set_looping_count_bits(uint16_t v) const { if (auto* d = direct()) { d->looping_count = v; return; } if (memory::IsHandle(address + 38600)) memory::OwnerStore(address + 38600, 2, v); else guest::StoreU16(address + 38600, v); }
  uint16_t looping_count() const { if (auto* d = direct()) return d->looping_count; return uint16_t((memory::IsHandle(address + 38600) ? uint16_t(memory::OwnerLoad(address + 38600, 2)) : guest::LoadU16(address + 38600))); }
  void set_looping_count(uint16_t v) const { if (auto* d = direct()) { d->looping_count = v; return; } if (memory::IsHandle(address + 38600)) memory::OwnerStore(address + 38600, 2, v); else guest::StoreU16(address + 38600, v); }
  // 128 {id, text} pairs (UiMoviePair; the texts are UiMovieText records)
  uint32_t texts_address() const { return address + 39884; }
  uint8_t* texts() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 39884 : guest::Pointer(address + 39884); }
  // the first pair's id
  uint32_t text_0_id_address() const { return address + 39884; }
  uint32_t text_0_id_bits() const { if (auto* d = direct()) return d->text_0_id; return uint32_t((memory::IsHandle(address + 39884) ? uint32_t(memory::OwnerLoad(address + 39884, 4)) : guest::LoadU32(address + 39884))); }
  void set_text_0_id_bits(uint32_t v) const { if (auto* d = direct()) { d->text_0_id = v; return; } if (memory::IsHandle(address + 39884)) memory::OwnerStore(address + 39884, 4, v); else guest::StoreU32(address + 39884, v); }
  uint32_t text_0_id() const { if (auto* d = direct()) return d->text_0_id; return uint32_t((memory::IsHandle(address + 39884) ? uint32_t(memory::OwnerLoad(address + 39884, 4)) : guest::LoadU32(address + 39884))); }
  void set_text_0_id(uint32_t v) const { if (auto* d = direct()) { d->text_0_id = v; return; } if (memory::IsHandle(address + 39884)) memory::OwnerStore(address + 39884, 4, v); else guest::StoreU32(address + 39884, v); }
  // the first pair's text
  uint32_t text_0_address() const { return address + 39888; }
  uint32_t text_0_bits() const { if (auto* d = direct()) return d->text_0; return uint32_t((memory::IsHandle(address + 39888) ? uint32_t(memory::OwnerLoad(address + 39888, 4)) : guest::LoadU32(address + 39888))); }
  void set_text_0_bits(uint32_t v) const { if (auto* d = direct()) { d->text_0 = v; return; } if (memory::IsHandle(address + 39888)) memory::OwnerStore(address + 39888, 4, v); else guest::StoreU32(address + 39888, v); }
  template <typename T = UiMovieText>
  T text_0() const { return T(text_0_bits()); }
  void set_text_0(const View& w) const { set_text_0_bits(w.address); }
  void set_text_0(uint32_t v) const { set_text_0_bits(v); }
  // the pairs in use; also numbers new texts
  uint32_t text_count_address() const { return address + 40908; }
  uint32_t text_count_bits() const { if (auto* d = direct()) return d->text_count; return uint32_t((memory::IsHandle(address + 40908) ? uint32_t(memory::OwnerLoad(address + 40908, 4)) : guest::LoadU32(address + 40908))); }
  void set_text_count_bits(uint32_t v) const { if (auto* d = direct()) { d->text_count = v; return; } if (memory::IsHandle(address + 40908)) memory::OwnerStore(address + 40908, 4, v); else guest::StoreU32(address + 40908, v); }
  uint32_t text_count() const { if (auto* d = direct()) return d->text_count; return uint32_t((memory::IsHandle(address + 40908) ? uint32_t(memory::OwnerLoad(address + 40908, 4)) : guest::LoadU32(address + 40908))); }
  void set_text_count(uint32_t v) const { if (auto* d = direct()) { d->text_count = v; return; } if (memory::IsHandle(address + 40908)) memory::OwnerStore(address + 40908, 4, v); else guest::StoreU32(address + 40908, v); }
  // set at the end of UiMovie::OnLoaded
  uint32_t loaded_address() const { return address + 40912; }
  uint32_t loaded_bits() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 40912) ? uint32_t(memory::OwnerLoad(address + 40912, 4)) : guest::LoadU32(address + 40912))); }
  void set_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 40912)) memory::OwnerStore(address + 40912, 4, v); else guest::StoreU32(address + 40912, v); }
  uint32_t loaded() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 40912) ? uint32_t(memory::OwnerLoad(address + 40912, 4)) : guest::LoadU32(address + 40912))); }
  void set_loaded(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 40912)) memory::OwnerStore(address + 40912, 4, v); else guest::StoreU32(address + 40912, v); }
  // its id in the movie table (UiMovies::Open)
  uint32_t id_address() const { return address + 40916; }
  uint32_t id_bits() const { if (auto* d = direct()) return d->id; return uint32_t((memory::IsHandle(address + 40916) ? uint32_t(memory::OwnerLoad(address + 40916, 4)) : guest::LoadU32(address + 40916))); }
  void set_id_bits(uint32_t v) const { if (auto* d = direct()) { d->id = v; return; } if (memory::IsHandle(address + 40916)) memory::OwnerStore(address + 40916, 4, v); else guest::StoreU32(address + 40916, v); }
  uint32_t id() const { if (auto* d = direct()) return d->id; return uint32_t((memory::IsHandle(address + 40916) ? uint32_t(memory::OwnerLoad(address + 40916, 4)) : guest::LoadU32(address + 40916))); }
  void set_id(uint32_t v) const { if (auto* d = direct()) { d->id = v; return; } if (memory::IsHandle(address + 40916)) memory::OwnerStore(address + 40916, 4, v); else guest::StoreU32(address + 40916, v); }
  // a copy of the last started animation's slot (UiMovieAnimationSlot)
  uint32_t last_started_address() const { return address + 40920; }
  uint8_t* last_started() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 40920 : guest::Pointer(address + 40920); }
  // measure fault: LocationScreen::Page0's r31 is an image base, r31 + 20044 the global 0x82814E4C (the location's menu), not a field (from UiMovieObject, tuning-pages)
  uint32_t fault_location_menu_address() const { return address + 20044; }
  uint32_t fault_location_menu_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kUiMovieLayout, 20044, 4)); return uint32_t((memory::IsHandle(address + 20044) ? uint32_t(memory::OwnerLoad(address + 20044, 4)) : guest::LoadU32(address + 20044))); }
  void set_fault_location_menu_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kUiMovieLayout, 20044, 4, v); return; } if (memory::IsHandle(address + 20044)) memory::OwnerStore(address + 20044, 4, v); else guest::StoreU32(address + 20044, v); }
  uint32_t fault_location_menu() const { return uint32_t(fault_location_menu_bits()); }
  void set_fault_location_menu(uint32_t v) const { set_fault_location_menu_bits(uint32_t(v)); }
};

// embedded; measure: UiMovie@+37044, AnimationList::RemoveAt.r3
struct UiMovieAnimationList : View {
  using View::View;
  static constexpr uint32_t kSize = 1420;
  // 32 links: +0 the next (16-bit), +2 its slot
  uint32_t links_address() const { return address + 0; }
  // the free chain's first link
  uint16_t free() const { return guest::LoadU16(address + 128); }
  void set_free(uint16_t v) const { guest::StoreU16(address + 128, v); }
  uint32_t free_address() const { return address + 128; }
  // set to -1, not read
  uint16_t unknown_130() const { return guest::LoadU16(address + 130); }
  void set_unknown_130(uint16_t v) const { guest::StoreU16(address + 130, v); }
  uint32_t unknown_130_address() const { return address + 130; }
  // the first link in use (-1 none)
  uint16_t first() const { return guest::LoadU16(address + 132); }
  void set_first(uint16_t v) const { guest::StoreU16(address + 132, v); }
  uint32_t first_address() const { return address + 132; }
  // set to -1, not read
  uint16_t unknown_134() const { return guest::LoadU16(address + 134); }
  void set_unknown_134(uint16_t v) const { guest::StoreU16(address + 134, v); }
  uint32_t unknown_134_address() const { return address + 134; }
  uint16_t count() const { return guest::LoadU16(address + 136); }
  void set_count(uint16_t v) const { guest::StoreU16(address + 136, v); }
  uint32_t count_address() const { return address + 136; }
  // 32 UiMovieAnimationSlot of 40 bytes
  uint32_t slots_address() const { return address + 140; }
};

// embedded; measure: -
struct UiMovieAnimationLink : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the next link's index (signed; -1 ends a chain)
  uint16_t next() const { return guest::LoadU16(address + 0); }
  void set_next(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  // its slot's index (signed)
  uint16_t slot() const { return guest::LoadU16(address + 2); }
  void set_slot(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t slot_address() const { return address + 2; }
};

// embedded; measure: -
struct UiMovieAnimationSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // the animation's index in its scene (-1 for a named one)
  uint32_t index() const { return guest::LoadU32(address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t index_address() const { return address + 0; }
  // the sub-scene's index (-1 the live scene; -1 for a named one)
  uint32_t sub_scene() const { return guest::LoadU32(address + 4); }
  void set_sub_scene(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t sub_scene_address() const { return address + 4; }
  // the named animation's index (-1 otherwise)
  uint32_t named() const { return guest::LoadU32(address + 8); }
  void set_named(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t named_address() const { return address + 8; }
  // the scene node
  uint32_t scene() const { return guest::LoadU32(address + 12); }
  void set_scene(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t scene_address() const { return address + 12; }
  // the animation (its +12 the length)
  uint32_t animation() const { return guest::LoadU32(address + 16); }
  void set_animation(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t animation_address() const { return address + 16; }
  // a float, copied as its word
  uint32_t time() const { return guest::LoadU32(address + 20); }
  void set_time(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t time_address() const { return address + 20; }
  // chained with the next slot
  uint32_t chained() const { return guest::LoadU32(address + 24); }
  void set_chained(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t chained_address() const { return address + 24; }
  // the time steps down from the length
  uint32_t reversed() const { return guest::LoadU32(address + 28); }
  void set_reversed(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t reversed_address() const { return address + 28; }
  // probably keep its end pose once it ends
  uint32_t keep_end() const { return guest::LoadU32(address + 32); }
  void set_keep_end(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t keep_end_address() const { return address + 32; }
  // the time isn't stepped
  uint32_t paused() const { return guest::LoadU32(address + 36); }
  void set_paused(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t paused_address() const { return address + 36; }
};

// embedded; measure: UiMovies.+0, sub_8211C168.r3.+0, sub_8211B458.r3.+0, sub_8211B458.r4, new<4294967295>@sub_8211B160
#pragma pack(push, 1)
struct UiMoviePairData {
  uint32_t key = 0;  // +0
  uint32_t value = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(UiMoviePairData) == 8);
inline constexpr memory::LayoutField kUiMoviePairFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kUiMoviePairLayout = {kUiMoviePairFields, 2};

struct UiMoviePair : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  UiMoviePairData* data() const { return static_cast<UiMoviePairData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static UiMoviePair New() {
    return UiMoviePair(memory::MakeHandle(new UiMoviePairData(), sizeof(UiMoviePairData), "UiMoviePair", &kUiMoviePairLayout));
  }
  template <uint32_t Stride = kSize>
  static UiMoviePair NewArray(uint32_t count) {
    static_assert(sizeof(UiMoviePairData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<UiMoviePairData*>(::operator new(sizeof(UiMoviePairData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) UiMoviePairData();
    return UiMoviePair(memory::MakeHandle(p, sizeof(UiMoviePairData) * count, "UiMoviePair", &kUiMoviePairLayout, sizeof(UiMoviePairData)));
  }
  UiMoviePair at(uint32_t i) const { return UiMoviePair(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  UiMoviePairData* direct() const { return static_cast<UiMoviePairData*>(memory::HandleOwn(address, &kUiMoviePairLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kUiMoviePairLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kUiMoviePairLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t key_address() const { return address + 0; }
  uint32_t key_bits() const { if (auto* d = direct()) return d->key; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_key_bits(uint32_t v) const { if (auto* d = direct()) { d->key = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t key() const { if (auto* d = direct()) return d->key; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_key(uint32_t v) const { if (auto* d = direct()) { d->key = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 4; }
  uint32_t value_bits() const { if (auto* d = direct()) return d->value; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_value_bits(uint32_t v) const { if (auto* d = direct()) { d->value = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t value() const { if (auto* d = direct()) return d->value; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_value(uint32_t v) const { if (auto* d = direct()) { d->value = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
};

// embedded; measure: TextMap::Set.r3, TextMap::Find.r3
struct UiMovieTextMap : View {
  using View::View;
  static constexpr uint32_t kSize = 1028;
  // 128 UiMoviePair {id, text}
  uint32_t pairs_address() const { return address + 0; }
  uint32_t pair_0_key() const { return guest::LoadU32(address + 0); }
  void set_pair_0_key(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t pair_0_key_address() const { return address + 0; }
  // the pairs in use (the movie's text_count)
  uint32_t count() const { return guest::LoadU32(address + 1024); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 1024, v); }
  uint32_t count_address() const { return address + 1024; }
};

// embedded; measure: UiMovies, sub_8211C168.r3, sub_8211B458.r3
struct UiMoviePairVector : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = UiMoviePair>
  T pairs() const { return T(guest::LoadU32(address + 0)); }
  void set_pairs(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_pairs(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t pairs_address() const { return address + 0; }
  uint32_t capacity() const { return guest::LoadU32(address + 4); }
  void set_capacity(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t capacity_address() const { return address + 4; }
  uint32_t count() const { return guest::LoadU32(address + 8); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t count_address() const { return address + 8; }
};

// embedded; measure: TextMap::Find.ret, TextMap::Find.r4, TextMap::Set.r4, TextMap::Set.r5, UiMovie::GetText.r4, UiMovies::Find.r4, sub_8211C168.r4, sub_8211C168.r5, sub_8211AE68.r4
struct UiMovieWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: UiMovie::OnLoaded.r6@+28, sub_8211A6C0.ret@+28, sub_8211A6C0.r3
struct UiMovieWordVector : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t words() const { return guest::LoadU32(address + 0); }
  void set_words(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t words_address() const { return address + 0; }
  uint32_t capacity() const { return guest::LoadU32(address + 4); }
  void set_capacity(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t capacity_address() const { return address + 4; }
  uint32_t count() const { return guest::LoadU32(address + 8); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t count_address() const { return address + 8; }
  // measure fault: sub_8211A6C0's r3 is the movie (its +16 the live scene), not this vector (the movie's +28)
  uint32_t fault_live_scene() const { return guest::LoadU32(address + 16); }
  void set_fault_live_scene(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t fault_live_scene_address() const { return address + 16; }
};

// embedded; measure: sub_8211E318.r3, sub_8211E318.r4
struct UiMovieNamedAnimation : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // a std::string, "scene.animation"
  uint32_t name_address() const { return address + 0; }
  uint32_t scene() const { return guest::LoadU32(address + 28); }
  void set_scene(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t scene_address() const { return address + 28; }
  uint32_t animation() const { return guest::LoadU32(address + 32); }
  void set_animation(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t animation_address() const { return address + 32; }
};

// embedded; measure: Quad, sub_8259DAD0.r3, sub_8259E098.r3, sub_8259EBE8.r3
struct UiMovieQuad : View {
  using View::View;
  static constexpr uint32_t kSize = 116;
  // the identity when made
  uint32_t matrix_address() const { return address + 0; }
  float matrix_3() const { return guest::LoadFloat(address + 12); }
  void set_matrix_3(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t matrix_3_address() const { return address + 12; }
  float matrix_7() const { return guest::LoadFloat(address + 28); }
  void set_matrix_7(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t matrix_7_address() const { return address + 28; }
  float matrix_11() const { return guest::LoadFloat(address + 44); }
  void set_matrix_11(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t matrix_11_address() const { return address + 44; }
  float matrix_15() const { return guest::LoadFloat(address + 60); }
  void set_matrix_15(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t matrix_15_address() const { return address + 60; }
  // 0 when made
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  // 0 to 4, 5 and up none; 0 when made
  uint32_t blend_mode() const { return guest::LoadU32(address + 68); }
  void set_blend_mode(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t blend_mode_address() const { return address + 68; }
  // 2 when made
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t vertex_count() const { return guest::LoadU32(address + 80); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t vertex_count_address() const { return address + 80; }
  uint32_t texture() const { return guest::LoadU32(address + 84); }
  void set_texture(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t texture_address() const { return address + 84; }
  // 16 bytes a vertex
  uint32_t positions() const { return guest::LoadU32(address + 88); }
  void set_positions(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t positions_address() const { return address + 88; }
  // 16 bytes a vertex
  uint32_t colors() const { return guest::LoadU32(address + 92); }
  void set_colors(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t colors_address() const { return address + 92; }
  // the texture coordinates, 16 bytes a vertex
  uint32_t coords() const { return guest::LoadU32(address + 96); }
  void set_coords(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t coords_address() const { return address + 96; }
  uint32_t clipped() const { return guest::LoadU32(address + 100); }
  void set_clipped(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t clipped_address() const { return address + 100; }
  // the clip rectangle (+104 to +110)
  uint16_t clip_0() const { return guest::LoadU16(address + 104); }
  void set_clip_0(uint16_t v) const { guest::StoreU16(address + 104, v); }
  uint32_t clip_0_address() const { return address + 104; }
  uint16_t clip_1() const { return guest::LoadU16(address + 106); }
  void set_clip_1(uint16_t v) const { guest::StoreU16(address + 106, v); }
  uint32_t clip_1_address() const { return address + 106; }
  uint16_t clip_2() const { return guest::LoadU16(address + 108); }
  void set_clip_2(uint16_t v) const { guest::StoreU16(address + 108, v); }
  uint32_t clip_2_address() const { return address + 108; }
  uint16_t clip_3() const { return guest::LoadU16(address + 110); }
  void set_clip_3(uint16_t v) const { guest::StoreU16(address + 110, v); }
  uint32_t clip_3_address() const { return address + 110; }
  // the texture stage flag
  uint32_t texture_stage() const { return guest::LoadU32(address + 112); }
  void set_texture_stage(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t texture_stage_address() const { return address + 112; }
};

// embedded; measure: UiMovie::ScrollOffset.r4
struct UiMovieTextParams : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  // integer
  uint32_t x() const { return guest::LoadU32(address + 0); }
  void set_x(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  uint32_t y() const { return guest::LoadU32(address + 4); }
  void set_y(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  // the node's pivot
  uint32_t origin_x() const { return guest::LoadU32(address + 8); }
  void set_origin_x(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t origin_x_address() const { return address + 8; }
  uint32_t origin_y() const { return guest::LoadU32(address + 12); }
  void set_origin_y(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t origin_y_address() const { return address + 12; }
  float angle() const { return guest::LoadFloat(address + 16); }
  void set_angle(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t angle_address() const { return address + 16; }
  uint8_t red() const { return guest::LoadU8(address + 20); }
  void set_red(uint8_t v) const { guest::StoreU8(address + 20, v); }
  uint32_t red_address() const { return address + 20; }
  uint8_t green() const { return guest::LoadU8(address + 21); }
  void set_green(uint8_t v) const { guest::StoreU8(address + 21, v); }
  uint32_t green_address() const { return address + 21; }
  uint8_t blue() const { return guest::LoadU8(address + 22); }
  void set_blue(uint8_t v) const { guest::StoreU8(address + 22, v); }
  uint32_t blue_address() const { return address + 22; }
  float alpha() const { return guest::LoadFloat(address + 24); }
  void set_alpha(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t alpha_address() const { return address + 24; }
  float scale_x() const { return guest::LoadFloat(address + 28); }
  void set_scale_x(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t scale_x_address() const { return address + 28; }
  float scale_y() const { return guest::LoadFloat(address + 32); }
  void set_scale_y(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t scale_y_address() const { return address + 32; }
  float glyph_scale_x() const { return guest::LoadFloat(address + 36); }
  void set_glyph_scale_x(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t glyph_scale_x_address() const { return address + 36; }
  float glyph_scale_y() const { return guest::LoadFloat(address + 40); }
  void set_glyph_scale_y(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t glyph_scale_y_address() const { return address + 40; }
  // the node's clip rectangle (+44 to +56)
  uint32_t clip_0() const { return guest::LoadU32(address + 44); }
  void set_clip_0(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t clip_0_address() const { return address + 44; }
  uint32_t clip_1() const { return guest::LoadU32(address + 48); }
  void set_clip_1(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t clip_1_address() const { return address + 48; }
  uint32_t clip_2() const { return guest::LoadU32(address + 52); }
  void set_clip_2(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t clip_2_address() const { return address + 52; }
  uint32_t clip_3() const { return guest::LoadU32(address + 56); }
  void set_clip_3(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t clip_3_address() const { return address + 56; }
  // 0 left, 1, 2, 3 (the node's flags)
  uint32_t alignment() const { return guest::LoadU32(address + 60); }
  void set_alignment(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t alignment_address() const { return address + 60; }
  // 24
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  // the node's +100 (integer): the scrolling text's gap
  uint32_t gap() const { return guest::LoadU32(address + 68); }
  void set_gap(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t gap_address() const { return address + 68; }
  // the node's +104 (integer)
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  // the node's +124
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  // the node's +128
  uint32_t unknown_80() const { return guest::LoadU32(address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  // the node's +116 (integer)
  uint32_t unknown_84() const { return guest::LoadU32(address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  // the node's +120 (integer)
  uint32_t unknown_88() const { return guest::LoadU32(address + 88); }
  void set_unknown_88(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  // 128
  uint8_t unknown_92() const { return guest::LoadU8(address + 92); }
  void set_unknown_92(uint8_t v) const { guest::StoreU8(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  // 128
  uint8_t unknown_93() const { return guest::LoadU8(address + 93); }
  void set_unknown_93(uint8_t v) const { guest::StoreU8(address + 93, v); }
  uint32_t unknown_93_address() const { return address + 93; }
  // 128
  uint8_t unknown_94() const { return guest::LoadU8(address + 94); }
  void set_unknown_94(uint8_t v) const { guest::StoreU8(address + 94, v); }
  uint32_t unknown_94_address() const { return address + 94; }
  float unknown_96() const { return guest::LoadFloat(address + 96); }
  void set_unknown_96(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  float unknown_100() const { return guest::LoadFloat(address + 100); }
  void set_unknown_100(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  float unknown_104() const { return guest::LoadFloat(address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  // 0
  uint32_t unknown_112() const { return guest::LoadU32(address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  // the node's layer (+72)
  uint32_t layer() const { return guest::LoadU32(address + 116); }
  void set_layer(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t layer_address() const { return address + 116; }
  // the node's text id (+108)
  uint32_t text_id() const { return guest::LoadU32(address + 120); }
  void set_text_id(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t text_id_address() const { return address + 120; }
};

// embedded; measure: UiMovie::SetNameText.r5
struct UiMovieNameRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // 0, 1, 4 or 5
  uint32_t kind() const { return guest::LoadU32(address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // signed
  uint16_t index() const { return guest::LoadU16(address + 8); }
  void set_index(uint16_t v) const { guest::StoreU16(address + 8, v); }
  uint32_t index_address() const { return address + 8; }
  uint8_t flag() const { return guest::LoadU8(address + 10); }
  void set_flag(uint8_t v) const { guest::StoreU8(address + 10, v); }
  uint32_t flag_address() const { return address + 10; }
  // 2 or 3
  uint32_t second_kind() const { return guest::LoadU32(address + 12); }
  void set_second_kind(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t second_kind_address() const { return address + 12; }
  // signed
  uint16_t second_index() const { return guest::LoadU16(address + 16); }
  void set_second_index(uint16_t v) const { guest::StoreU16(address + 16, v); }
  uint32_t second_index_address() const { return address + 16; }
  uint8_t second_flag() const { return guest::LoadU8(address + 18); }
  void set_second_flag(uint8_t v) const { guest::StoreU8(address + 18, v); }
  uint32_t second_flag_address() const { return address + 18; }
};

// file; measure: sub_8259D670.ret
struct UiMovieFileAnimation : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  float length() const { return guest::LoadFloat(address + 12); }
  void set_length(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t length_address() const { return address + 12; }
  // its name's first byte (a string in place)
  uint8_t name() const { return guest::LoadU8(address + 16); }
  void set_name(uint8_t v) const { guest::StoreU8(address + 16, v); }
  uint32_t name_address() const { return address + 16; }
};

// file; measure: sub_8259D648.ret
struct UiMoviePictureTexture : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the index in the movie's textures
  uint32_t texture() const { return guest::LoadU32(address + 0); }
  void set_texture(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t texture_address() const { return address + 0; }
};

// file; measure: sub_825A0CD0.ret
struct UiMovieXmdEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // entry 1's: the XPR's texture count
  uint32_t texture_count() const { return guest::LoadU32(address + 12); }
  void set_texture_count(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t texture_count_address() const { return address + 12; }
};

// file; measure: sub_82108A70.r5
struct UiMovieCharText : View {
  using View::View;
  static constexpr uint32_t kSize = 3;
  uint8_t char_0() const { return guest::LoadU8(address + 0); }
  void set_char_0(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t char_0_address() const { return address + 0; }
  uint8_t char_1() const { return guest::LoadU8(address + 1); }
  void set_char_1(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t char_1_address() const { return address + 1; }
  uint8_t char_2() const { return guest::LoadU8(address + 2); }
  void set_char_2(uint8_t v) const { guest::StoreU8(address + 2, v); }
  uint32_t char_2_address() const { return address + 2; }
};

// embedded; measure: UiMovie@+176
struct UiMovieAt176 : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
};

// embedded; measure: UiMovie@+28
struct UiMovieAt28 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: UiMovie@+40
struct UiMovieAt40 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: UiMovie@+64
struct UiMovieAt64 : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_12_4i() const { return guest::LoadU32(address + 12); }
  void set_unknown_12_4i(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_4i_address() const { return address + 12; }
  float unknown_28() const { return guest::LoadFloat(address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  float unknown_44() const { return guest::LoadFloat(address + 44); }
  void set_unknown_44(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  float unknown_60() const { return guest::LoadFloat(address + 60); }
  void set_unknown_60(float v) const { guest::StoreFloat(address + 60, v); }
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
  float unknown_84() const { return guest::LoadFloat(address + 84); }
  void set_unknown_84(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  float unknown_88() const { return guest::LoadFloat(address + 88); }
  void set_unknown_88(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  float unknown_92() const { return guest::LoadFloat(address + 92); }
  void set_unknown_92(float v) const { guest::StoreFloat(address + 92, v); }
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
};

// embedded; measure: new<65552>@sub_8211B018@+16
struct UiMovieTextItemAt16 : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_28() const { return guest::LoadFloat(address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  float unknown_44() const { return guest::LoadFloat(address + 44); }
  void set_unknown_44(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  float unknown_60() const { return guest::LoadFloat(address + 60); }
  void set_unknown_60(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  float unknown_76() const { return guest::LoadFloat(address + 76); }
  void set_unknown_76(float v) const { guest::StoreFloat(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  float unknown_80() const { return guest::LoadFloat(address + 80); }
  void set_unknown_80(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  float unknown_84() const { return guest::LoadFloat(address + 84); }
  void set_unknown_84(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  float unknown_88() const { return guest::LoadFloat(address + 88); }
  void set_unknown_88(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  float unknown_92() const { return guest::LoadFloat(address + 92); }
  void set_unknown_92(float v) const { guest::StoreFloat(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  float unknown_96() const { return guest::LoadFloat(address + 96); }
  void set_unknown_96(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
};

// The image words docs/functions/ui-movies-game.md's code uses.
struct UiMoviesGameGlobals {
  // the UI movie color table: 12-byte entries (r, g, b integers)
  static constexpr uint32_t colors_address() { return 0x82008410u; }
  // the empty string a text without a buffer gives
  static uint16_t empty_string() { return guest::LoadU16(0x82008434u); }
  static void set_empty_string(uint16_t v) { guest::StoreU16(0x82008434u, v); }
  static constexpr uint32_t empty_string_address() { return 0x82008434u; }
  // the float 0.0 (copied as its word)
  static uint32_t zero() { return guest::LoadU32(0x82000A34u); }
  static void set_zero(uint32_t v) { guest::StoreU32(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // the time AnimationTime answers for none
  static uint32_t no_time() { return guest::LoadU32(0x82001BE0u); }
  static void set_no_time(uint32_t v) { guest::StoreU32(0x82001BE0u, v); }
  static constexpr uint32_t no_time_address() { return 0x82001BE0u; }
  // the named animations' "."
  static uint8_t name_separator() { return guest::LoadU8(0x82001DE8u); }
  static void set_name_separator(uint8_t v) { guest::StoreU8(0x82001DE8u, v); }
  static constexpr uint32_t name_separator_address() { return 0x82001DE8u; }
  // the scroll speed ScrollText sets (a float, copied as its word)
  static uint32_t scroll_speed() { return guest::LoadU32(0x820021F8u); }
  static void set_scroll_speed(uint32_t v) { guest::StoreU32(0x820021F8u, v); }
  static constexpr uint32_t scroll_speed_address() { return 0x820021F8u; }
  // cleared by sub_8211AD00's work
  static uint32_t screen_flag() { return guest::LoadU32(0x826C5CB0u); }
  static void set_screen_flag(uint32_t v) { guest::StoreU32(0x826C5CB0u, v); }
  static constexpr uint32_t screen_flag_address() { return 0x826C5CB0u; }
  // the UI movie table: a UiMoviePairVector of {id, movie} pairs
  template <typename T = UiMoviePair>
  static T movie_table_pairs() { return T(guest::LoadU32(0x826C5CCCu)); }
  static void set_movie_table_pairs(const View& v) { guest::StoreU32(0x826C5CCCu, v.address); }
  static void set_movie_table_pairs(uint32_t v) { guest::StoreU32(0x826C5CCCu, v); }
  static constexpr uint32_t movie_table_pairs_address() { return 0x826C5CCCu; }
  // 64
  static uint32_t movie_table_capacity() { return guest::LoadU32(0x826C5CD0u); }
  static void set_movie_table_capacity(uint32_t v) { guest::StoreU32(0x826C5CD0u, v); }
  static constexpr uint32_t movie_table_capacity_address() { return 0x826C5CD0u; }
  static uint32_t movie_table_count() { return guest::LoadU32(0x826C5CD4u); }
  static void set_movie_table_count(uint32_t v) { guest::StoreU32(0x826C5CD4u, v); }
  static constexpr uint32_t movie_table_count_address() { return 0x826C5CD4u; }
  // what the texts' lookup answers the address of for a missing key
  static uint32_t text_none() { return guest::LoadU32(0x826C5CD8u); }
  static void set_text_none(uint32_t v) { guest::StoreU32(0x826C5CD8u, v); }
  static constexpr uint32_t text_none_address() { return 0x826C5CD8u; }
  // what the movie table's lookup answers the address of for a missing key
  static uint32_t movie_none() { return guest::LoadU32(0x826C5CDCu); }
  static void set_movie_none(uint32_t v) { guest::StoreU32(0x826C5CDCu, v); }
  static constexpr uint32_t movie_none_address() { return 0x826C5CDCu; }
  // cleared by sub_8211AD00
  static uint32_t unknown_826C5CA4() { return guest::LoadU32(0x826C5CA4u); }
  static void set_unknown_826C5CA4(uint32_t v) { guest::StoreU32(0x826C5CA4u, v); }
  static constexpr uint32_t unknown_826C5CA4_address() { return 0x826C5CA4u; }
  // cleared by sub_8211AD00
  static uint32_t unknown_826C5CA8() { return guest::LoadU32(0x826C5CA8u); }
  static void set_unknown_826C5CA8(uint32_t v) { guest::StoreU32(0x826C5CA8u, v); }
  static constexpr uint32_t unknown_826C5CA8_address() { return 0x826C5CA8u; }
  // cleared by sub_8211AD00
  static uint32_t unknown_826C5CAC() { return guest::LoadU32(0x826C5CACu); }
  static void set_unknown_826C5CAC(uint32_t v) { guest::StoreU32(0x826C5CACu, v); }
  static constexpr uint32_t unknown_826C5CAC_address() { return 0x826C5CACu; }
  // cleared by sub_8211AD00
  static uint32_t unknown_826C5CB4() { return guest::LoadU32(0x826C5CB4u); }
  static void set_unknown_826C5CB4(uint32_t v) { guest::StoreU32(0x826C5CB4u, v); }
  static constexpr uint32_t unknown_826C5CB4_address() { return 0x826C5CB4u; }
  // cleared by sub_8211AD00
  static uint32_t unknown_826C5CB8() { return guest::LoadU32(0x826C5CB8u); }
  static void set_unknown_826C5CB8(uint32_t v) { guest::StoreU32(0x826C5CB8u, v); }
  static constexpr uint32_t unknown_826C5CB8_address() { return 0x826C5CB8u; }
  // cleared by sub_8211AD00
  static uint32_t unknown_826C5CBC() { return guest::LoadU32(0x826C5CBCu); }
  static void set_unknown_826C5CBC(uint32_t v) { guest::StoreU32(0x826C5CBCu, v); }
  static constexpr uint32_t unknown_826C5CBC_address() { return 0x826C5CBCu; }
  // cleared by sub_8211AD00
  static uint32_t unknown_826C5CC0() { return guest::LoadU32(0x826C5CC0u); }
  static void set_unknown_826C5CC0(uint32_t v) { guest::StoreU32(0x826C5CC0u, v); }
  static constexpr uint32_t unknown_826C5CC0_address() { return 0x826C5CC0u; }
};

}  // namespace schema
