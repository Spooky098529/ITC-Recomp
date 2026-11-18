// The views of docs/functions/options-load-save.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/options-load-save.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<28>@Options::OnEnter#2, new<28>@Options::OnEnter#4
struct OptionsSubPageBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the sub-page: 0 the device check, 1 the question, 2 a note, 3 the wait, 4 the close, 5 the cancel's, 6 the device's answer (load only)
  uint32_t page() const { return guest::LoadU32(base, address + 0); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t page_address() const { return address + 0; }
  // the page after the step (the options' page block layout)
  uint32_t next_page() const { return guest::LoadU32(base, address + 4); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t next_page_address() const { return address + 4; }
  // the sub-page before, set unless the same
  uint32_t previous_page() const { return guest::LoadU32(base, address + 8); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t previous_page_address() const { return address + 8; }
  // -1 here
  uint32_t step() const { return guest::LoadU32(base, address + 12); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t step_address() const { return address + 12; }
  // -1 here
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

// embedded; measure: LoadPage, SavePage
struct OptionsLeaveStarted : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 on the leave step's first pass, then 1
  uint32_t started() const { return guest::LoadU32(base, address + 0); }
  void set_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t started_address() const { return address + 0; }
};

// allocated; measure: new<129400>@SaveBuffers::Create
struct OptionsLoadedGameData : View {
  using View::View;
  static constexpr uint32_t kSize = 129400;
  // the garage list's count; 0 is kind 12's note (probably not the game's data)
  uint16_t garage_count() const { return guest::LoadU16(base, address + 6328); }
  void set_garage_count(uint16_t v) const { guest::StoreU16(base, address + 6328, v); }
  uint32_t garage_count_address() const { return address + 6328; }
};

// The image words docs/functions/options-load-save.md's code uses.
struct OptionsLoadSaveGlobals {
  // page 9's (the load's) sub-page block
  template <typename T = OptionsSubPageBlock>
  static T load_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D847Cu)); }
  static void set_load_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D847Cu, v.address); }
  static void set_load_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D847Cu, v); }
  static constexpr uint32_t load_pages_address() { return 0x826D847Cu; }
  // page 10's (the save's) sub-page block
  template <typename T = OptionsSubPageBlock>
  static T save_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8488u)); }
  static void set_save_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8488u, v.address); }
  static void set_save_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8488u, v); }
  static constexpr uint32_t save_pages_address() { return 0x826D8488u; }
};

}  // namespace schema
