// The views of docs/functions/menu-helpers.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/menu-helpers.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Menu, new<68>@Type2Dialog::OnEnter, new<68>@Type12Screen7::Create, new<68>@NetRanking::Create, new<68>@TitleScreen::Create, new<68>@TodaysResult::Create, new<68>@TodaysEvent::Create, new<68>@TuningMachine::Create, new<68>@Type2Hub::Create, new<68>@PowerGraphScreen::Create, new<68>@Type5Dialog::OnEnter, new<68>@Type6Screen0::OnEnter, new<68>@Type7Screen0::OnEnter, new<68>@ControllerOptions::OnEnter, new<68>@UserInfo::Create, new<68>@Type12Screen1::Create#2, new<68>@Type12Screen1::Create#3, new<68>@Type12Screen1::Create#4, TuningScreen.+10044, TuningScreen.+10040
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
  // 1 when the last Menu::Step wrapped
  uint32_t wrapped() const { return guest::LoadU32(base, address + 40); }
  void set_wrapped(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t wrapped_address() const { return address + 40; }
  // 1 when the last Menu::StepList wrapped (probably; tuning-pages.md)
  uint32_t list_wrapped() const { return guest::LoadU32(base, address + 44); }
  void set_list_wrapped(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t list_wrapped_address() const { return address + 44; }
  // the visible row, kept within first_row to rows_shown - 1 (probably)
  uint32_t visible_row() const { return guest::LoadU32(base, address + 48); }
  void set_visible_row(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t visible_row_address() const { return address + 48; }
  // the row before the last up or down step (probably)
  uint32_t previous_row() const { return guest::LoadU32(base, address + 52); }
  void set_previous_row(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t previous_row_address() const { return address + 52; }
  // probably
  uint32_t first_row() const { return guest::LoadU32(base, address + 56); }
  void set_first_row(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t first_row_address() const { return address + 56; }
  // 8 where seen (probably)
  uint32_t rows_shown() const { return guest::LoadU32(base, address + 60); }
  void set_rows_shown(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t rows_shown_address() const { return address + 60; }
  // the player: an index into [0x826BABA4], then the mapper entry at 0x826BAB80
  uint32_t player() const { return guest::LoadU32(base, address + 64); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t player_address() const { return address + 64; }
};

// allocated; measure: new<28>@Type2Dialog::OnEnter, new<28>@Type12Screen7::Create, ScreenPages, new<28>@PauseMenu::Create, new<28>@RaceDialog::Create, new<28>@AfterRaceMenu::Create, new<28>@PauseMenu4::Create, new<28>@Type12Screen6::Create, new<28>@Type12Screen9::Create, new<28>@Type12Dialog11::Create, new<28>@NetRanking::Create, new<28>@TitleScreen::Create, new<28>@TodaysResult::Create, new<28>@TodaysEvent::Create, new<28>@TuningMachine::Create, new<28>@Type2Hub::Create, new<28>@BuildScreen::Create, new<28>@PowerGraphScreen::Create, new<28>@ReplayTheater::Create, new<28>@Type5Dialog::OnEnter, new<28>@Type6Screen0::OnEnter, new<28>@Type7Screen0::OnEnter, new<28>@ControllerOptions::OnEnter, new<28>@UserInfo::Create, ScreenPages::Finish.r3
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
