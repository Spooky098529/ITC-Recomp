// The views of docs/functions/type2-dialog.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type2-dialog.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<68>@Type2Dialog::OnEnter#2
#pragma pack(push, 1)
struct Type2DialogDataData {
  uint32_t movies_loaded = 0;  // +0
  uint32_t queued_state = 0;  // +4
  uint32_t movie_shown = 0;  // +8
  uint8_t pad_12[56] = {};
};
#pragma pack(pop)
static_assert(sizeof(Type2DialogDataData) == 68);
inline constexpr memory::LayoutField kType2DialogDataFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kType2DialogDataLayout = {kType2DialogDataFields, 3};

struct Type2DialogData : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  Type2DialogDataData* data() const { return static_cast<Type2DialogDataData*>(memory::HandleObject(address)); }
  static Type2DialogData New(uint8_t* base) {
    return Type2DialogData(base, memory::MakeHandle(new Type2DialogDataData(), sizeof(Type2DialogDataData), "Type2DialogData", &kType2DialogDataLayout));
  }
  template <uint32_t Stride = kSize>
  static Type2DialogData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(Type2DialogDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<Type2DialogDataData*>(::operator new(sizeof(Type2DialogDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) Type2DialogDataData();
    return Type2DialogData(base, memory::MakeHandle(p, sizeof(Type2DialogDataData) * count, "Type2DialogData", &kType2DialogDataLayout, sizeof(Type2DialogDataData)));
  }
  Type2DialogData at(uint32_t i) const { return Type2DialogData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  Type2DialogDataData* direct() const { return static_cast<Type2DialogDataData*>(memory::HandleOwn(address, &kType2DialogDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kType2DialogDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kType2DialogDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t movies_loaded_address() const { return address + 0; }
  uint32_t movies_loaded_bits() const { if (auto* d = direct()) return d->movies_loaded; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_movies_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->movies_loaded = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t movies_loaded() const { if (auto* d = direct()) return d->movies_loaded; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_movies_loaded(uint32_t v) const { if (auto* d = direct()) { d->movies_loaded = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // 21 none
  uint32_t queued_state_address() const { return address + 4; }
  uint32_t queued_state_bits() const { if (auto* d = direct()) return d->queued_state; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_queued_state_bits(uint32_t v) const { if (auto* d = direct()) { d->queued_state = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t queued_state() const { if (auto* d = direct()) return d->queued_state; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_queued_state(uint32_t v) const { if (auto* d = direct()) { d->queued_state = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t movie_shown_address() const { return address + 8; }
  uint32_t movie_shown_bits() const { if (auto* d = direct()) return d->movie_shown; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_movie_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->movie_shown = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t movie_shown() const { if (auto* d = direct()) return d->movie_shown; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_movie_shown(uint32_t v) const { if (auto* d = direct()) { d->movie_shown = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
};

// The image words docs/functions/type2-dialog.md's code uses.
struct Type2DialogGlobals {
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83F8u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83F8u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83F8u, v); }
  static constexpr uint32_t pages_address() { return 0x826D83F8u; }
  // a front-end menu, items 0 to 1
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83FCu)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83FCu, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83FCu, v); }
  static constexpr uint32_t menu_address() { return 0x826D83FCu; }
  template <typename T = Type2DialogData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8400u)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8400u, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8400u, v); }
  static constexpr uint32_t data_address() { return 0x826D8400u; }
  // the dialog showing, 0 to 3
  static uint32_t showing(uint8_t* base) { return guest::LoadU32(base, 0x82636200u); }
  static void set_showing(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82636200u, v); }
  static constexpr uint32_t showing_address() { return 0x82636200u; }
  // -1 loading, 3 idle, 5 an open asked, 0 just shown, 4 asked by TakesInput, 1 taking input, 2 cancelled
  static uint32_t status(uint8_t* base) { return guest::LoadU32(base, 0x82636204u); }
  static void set_status(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82636204u, v); }
  static constexpr uint32_t status_address() { return 0x82636204u; }
};

}  // namespace schema
