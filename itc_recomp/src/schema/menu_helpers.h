// The views of docs/functions/menu-helpers.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/menu-helpers.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<68>@Type12Screen7::Create
struct FrontEndMenu : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  uint32_t cursor() const { return guest::LoadU32(base, address + 0); }
  void set_cursor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t cursor_address() const { return address + 0; }
  uint32_t previous() const { return guest::LoadU32(base, address + 4); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // set when the last Menu::Step moved the cursor
  uint32_t moved() const { return guest::LoadU32(base, address + 8); }
  void set_moved(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t moved_address() const { return address + 8; }
  // 0 in every menu seen
  uint32_t first_item() const { return guest::LoadU32(base, address + 12); }
  void set_first_item(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t first_item_address() const { return address + 12; }
  uint32_t last_item() const { return guest::LoadU32(base, address + 16); }
  void set_last_item(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t last_item_address() const { return address + 16; }
  // 1 when the cursor wraps (probably)
  uint32_t wraps() const { return guest::LoadU32(base, address + 20); }
  void set_wraps(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t wraps_address() const { return address + 20; }
  uint32_t up_down_locked() const { return guest::LoadU32(base, address + 24); }
  void set_up_down_locked(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t up_down_locked_address() const { return address + 24; }
  uint32_t left_right_steps() const { return guest::LoadU32(base, address + 28); }
  void set_left_right_steps(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t left_right_steps_address() const { return address + 28; }
  uint32_t shoulders_step() const { return guest::LoadU32(base, address + 32); }
  void set_shoulders_step(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t shoulders_step_address() const { return address + 32; }
  // -1 or 1 when left or right was asked
  int32_t side() const { return int32_t(guest::LoadU32(base, address + 36)); }
  void set_side(int32_t v) const { guest::StoreU32(base, address + 36, uint32_t(v)); }
  uint32_t side_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  // probably the player: the mapper entry's index
  uint32_t player() const { return guest::LoadU32(base, address + 64); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t player_address() const { return address + 64; }
};

// allocated; measure: new<28>@Type2Dialog::OnEnter, new<28>@Type12Screen7::Create
struct PageObject : View {
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

}  // namespace schema
