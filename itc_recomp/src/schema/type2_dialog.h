// The views of docs/functions/type2-dialog.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type2-dialog.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<28>@Type2Dialog::OnEnter
struct Type2DialogPages : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the page Draw runs; -1 while a step runs
  uint32_t page() const { return guest::LoadU32(base, address + 0); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t page_address() const { return address + 0; }
  // the page after the step
  uint32_t next_page() const { return guest::LoadU32(base, address + 4); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t next_page_address() const { return address + 4; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 8); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t previous_page_address() const { return address + 8; }
  // the step Update runs; -1 none
  uint32_t step() const { return guest::LoadU32(base, address + 12); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t step_address() const { return address + 12; }
  // the step after it
  uint32_t queued_step() const { return guest::LoadU32(base, address + 16); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t queued_step_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // the step's started word
  uint32_t step_started() const { return guest::LoadU32(base, address + 24); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t step_started_address() const { return address + 24; }
};

// allocated; measure: new<68>@Type2Dialog::OnEnter
struct Type2DialogData : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  uint32_t movies_loaded() const { return guest::LoadU32(base, address + 0); }
  void set_movies_loaded(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t movies_loaded_address() const { return address + 0; }
  // 21 none
  uint32_t queued_state() const { return guest::LoadU32(base, address + 4); }
  void set_queued_state(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t queued_state_address() const { return address + 4; }
  uint32_t movie_shown() const { return guest::LoadU32(base, address + 8); }
  void set_movie_shown(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t movie_shown_address() const { return address + 8; }
};

// The image words docs/functions/type2-dialog.md's code uses.
struct Type2DialogGlobals {
  template <typename T = Type2DialogPages>
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
