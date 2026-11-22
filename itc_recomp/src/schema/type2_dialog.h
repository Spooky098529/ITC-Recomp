// The views of docs/functions/type2-dialog.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type2-dialog.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<68>@Type2Dialog::OnEnter#2
struct Type2DialogDataData {
  uint32_t movies_loaded = 0;  // +0
  uint32_t queued_state = 0;  // +4
  uint32_t movie_shown = 0;  // +8
};

struct Type2DialogData : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  static constexpr bool kHost = true;
  Type2DialogDataData* data() const { return static_cast<Type2DialogDataData*>(memory::HandleObject(address)); }
  static Type2DialogData New(uint8_t* base) {
    return Type2DialogData(base, memory::MakeHandle(new Type2DialogDataData(), kSize, "Type2DialogData"));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint32_t movies_loaded() const { return data()->movies_loaded; }
  void set_movies_loaded(uint32_t v) const { data()->movies_loaded = v; }
  uint32_t movies_loaded_bits() const { return data()->movies_loaded; }
  void set_movies_loaded_bits(uint32_t v) const { data()->movies_loaded = v; }
  // 21 none
  uint32_t queued_state() const { return data()->queued_state; }
  void set_queued_state(uint32_t v) const { data()->queued_state = v; }
  uint32_t queued_state_bits() const { return data()->queued_state; }
  void set_queued_state_bits(uint32_t v) const { data()->queued_state = v; }
  uint32_t movie_shown() const { return data()->movie_shown; }
  void set_movie_shown(uint32_t v) const { data()->movie_shown = v; }
  uint32_t movie_shown_bits() const { return data()->movie_shown; }
  void set_movie_shown_bits(uint32_t v) const { data()->movie_shown = v; }
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
