// The views of docs/functions/race-screens.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-screens.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<?>@MenuList::Init
struct MenuListEnabled : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // item 0's word (0 disabled); item i's at +4i
  uint32_t item_0() const { return guest::LoadU32(base, address + 0); }
  void set_item_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t item_0_address() const { return address + 0; }
};

// allocated; measure: MenuList, new<72>@PauseMenu::Create, new<72>@AfterRaceMenu::Create, new<72>@RaceDialog::Create, new<72>@PauseMenu4::Create, new<72>@SaveScreen::OnEnter, new<72>@TitleScreen::Create, new<72>@ReplayTheater::Create, new<72>@PlateEditor::Create, new<72>@BuildScreen::Create, new<72>@BuildScreen::Create#2, new<72>@GarageScreen::Create, new<72>@GarageScreen::Create#2, new<72>@RaceScreens::Make1004, new<72>@CarDealer::Create#2
struct MenuList : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  // the item count
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the first item shown
  uint32_t scroll() const { return guest::LoadU32(base, address + 4); }
  void set_scroll(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t scroll_address() const { return address + 4; }
  uint32_t scroll_before() const { return guest::LoadU32(base, address + 8); }
  void set_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t scroll_before_address() const { return address + 8; }
  // the items shown
  uint32_t shown() const { return guest::LoadU32(base, address + 12); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t shown_address() const { return address + 12; }
  // relative to the scroll
  uint32_t selection() const { return guest::LoadU32(base, address + 16); }
  void set_selection(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t selection_address() const { return address + 16; }
  uint32_t selection_before() const { return guest::LoadU32(base, address + 20); }
  void set_selection_before(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t selection_before_address() const { return address + 20; }
  // the last move: 1 down, -1 up, 0 none
  uint32_t move() const { return guest::LoadU32(base, address + 24); }
  void set_move(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t move_address() const { return address + 24; }
  // left or right asked: -1, 1, else 0
  uint32_t side() const { return guest::LoadU32(base, address + 28); }
  void set_side(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t side_address() const { return address + 28; }
  // set when the move wrapped or skipped
  uint32_t wrapped() const { return guest::LoadU32(base, address + 32); }
  void set_wrapped(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t wrapped_address() const { return address + 32; }
  uint32_t down_action() const { return guest::LoadU32(base, address + 36); }
  void set_down_action(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t down_action_address() const { return address + 36; }
  uint32_t up_action() const { return guest::LoadU32(base, address + 40); }
  void set_up_action(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t up_action_address() const { return address + 40; }
  uint32_t left_action() const { return guest::LoadU32(base, address + 44); }
  void set_left_action(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t left_action_address() const { return address + 44; }
  uint32_t right_action() const { return guest::LoadU32(base, address + 48); }
  void set_right_action(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t right_action_address() const { return address + 48; }
  uint32_t accept_action() const { return guest::LoadU32(base, address + 52); }
  void set_accept_action(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t accept_action_address() const { return address + 52; }
  uint32_t back_action() const { return guest::LoadU32(base, address + 56); }
  void set_back_action(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t back_action_address() const { return address + 56; }
  // an index into [0x826BABA4], then the mapper entry at 0x826BAB80
  uint32_t player() const { return guest::LoadU32(base, address + 60); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t player_address() const { return address + 60; }
  uint32_t wraps() const { return guest::LoadU32(base, address + 64); }
  void set_wraps(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t wraps_address() const { return address + 64; }
  // a word an item, 0 for a disabled item
  template <typename T = MenuListEnabled>
  T enabled() const { return T(base, guest::LoadU32(base, address + 68)); }
  void set_enabled(const View& v) const { guest::StoreU32(base, address + 68, v.address); }
  void set_enabled(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t enabled_address() const { return address + 68; }
  // measure fault: PanelSet::Open's PanelSetBase (car-list.toml), taken for a menu list; its +228
  uint32_t fault_panel_set_228() const { return guest::LoadU32(base, address + 228); }
  void set_fault_panel_set_228(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t fault_panel_set_228_address() const { return address + 228; }
  // measure fault: as fault_panel_set_228
  uint32_t fault_panel_set_232() const { return guest::LoadU32(base, address + 232); }
  void set_fault_panel_set_232(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t fault_panel_set_232_address() const { return address + 232; }
  // measure fault: as fault_panel_set_228 (b + 272, list 0)
  uint32_t fault_panel_set_1820() const { return guest::LoadU32(base, address + 1820); }
  void set_fault_panel_set_1820(uint32_t v) const { guest::StoreU32(base, address + 1820, v); }
  uint32_t fault_panel_set_1820_address() const { return address + 1820; }
  // measure fault: as fault_panel_set_228
  uint32_t fault_panel_set_1900() const { return guest::LoadU32(base, address + 1900); }
  void set_fault_panel_set_1900(uint32_t v) const { guest::StoreU32(base, address + 1900, v); }
  uint32_t fault_panel_set_1900_address() const { return address + 1900; }
};

// allocated; measure: new<4>@PauseMenu::Create, PauseMenu
struct PauseMenuScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // UI movie 4 (UiMovies::Find)
  View movie() const { return View(base, guest::LoadU32(base, address + 0)); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t movie_address() const { return address + 0; }
};

// allocated; measure: new<160>@PauseMenu::Create
struct PauseMenuDataData {
  uint32_t hud_option_0 = 0;  // +0
  uint32_t hud_option_1 = 0;  // +4
  uint32_t hud_option_2 = 0;  // +8
  uint32_t hud_option_3 = 0;  // +12
  uint32_t music_volume_saved = 0;  // +16
  uint32_t effects_volume_saved = 0;  // +20
  uint32_t adjust_saved_0 = 0;  // +24
  uint32_t adjust_saved_1 = 0;  // +28
  uint32_t adjust_saved_2 = 0;  // +32
  uint32_t hud_edit_0 = 0;  // +36
  uint32_t hud_edit_1 = 0;  // +40
  uint32_t hud_edit_2 = 0;  // +44
  uint32_t hud_edit_3 = 0;  // +48
  uint32_t music_volume = 0;  // +52
  uint32_t effects_volume = 0;  // +56
  uint32_t adjust_0 = 0;  // +60
  uint32_t adjust_1 = 0;  // +64
  uint32_t adjust_2 = 0;  // +68
  uint32_t hud_default_0 = 0;  // +72
  uint32_t hud_default_1 = 0;  // +76
  uint32_t hud_default_2 = 0;  // +80
  uint32_t hud_default_3 = 0;  // +84
  uint32_t music_volume_default = 0;  // +88
  uint32_t effects_volume_default = 0;  // +92
  uint32_t adjust_default_0 = 0;  // +96
  uint32_t adjust_default_1 = 0;  // +100
  uint32_t adjust_default_2 = 0;  // +104
  uint32_t money = 0;  // +108
  uint32_t weekday = 0;  // +112
  uint32_t day = 0;  // +116
  uint32_t win_streak = 0;  // +120
  uint32_t car_value_a = 0;  // +124
  uint32_t car_value_b = 0;  // +128
  uint32_t kind = 0;  // +132
  uint32_t item_count = 0;  // +136
  uint32_t answer = 0;  // +140
  uint32_t last_item_ok = 0;  // +144
  uint32_t main_selection = 0;  // +148
  uint32_t run_count = 0;  // +152
  uint32_t first_day = 0;  // +156
};

struct PauseMenuData : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  static constexpr bool kHost = true;
  PauseMenuDataData* data() const { return static_cast<PauseMenuDataData*>(memory::HandleObject(address)); }
  static PauseMenuData New(uint8_t* base) {
    return PauseMenuData(base, memory::MakeHandle(new PauseMenuDataData(), kSize, "PauseMenuData"));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  // the four HUD options (the save's options words +0 to +12)
  uint32_t hud_option_0() const { return data()->hud_option_0; }
  void set_hud_option_0(uint32_t v) const { data()->hud_option_0 = v; }
  uint32_t hud_option_0_bits() const { return data()->hud_option_0; }
  void set_hud_option_0_bits(uint32_t v) const { data()->hud_option_0 = v; }
  uint32_t hud_option_1() const { return data()->hud_option_1; }
  void set_hud_option_1(uint32_t v) const { data()->hud_option_1 = v; }
  uint32_t hud_option_1_bits() const { return data()->hud_option_1; }
  void set_hud_option_1_bits(uint32_t v) const { data()->hud_option_1 = v; }
  uint32_t hud_option_2() const { return data()->hud_option_2; }
  void set_hud_option_2(uint32_t v) const { data()->hud_option_2 = v; }
  uint32_t hud_option_2_bits() const { return data()->hud_option_2; }
  void set_hud_option_2_bits(uint32_t v) const { data()->hud_option_2 = v; }
  uint32_t hud_option_3() const { return data()->hud_option_3; }
  void set_hud_option_3(uint32_t v) const { data()->hud_option_3 = v; }
  uint32_t hud_option_3_bits() const { return data()->hud_option_3; }
  void set_hud_option_3_bits(uint32_t v) const { data()->hud_option_3 = v; }
  // the options' byte +28, sign-extended (the music's: VolumePage sets SetMusicVolume with it)
  uint32_t music_volume_saved() const { return data()->music_volume_saved; }
  void set_music_volume_saved(uint32_t v) const { data()->music_volume_saved = v; }
  uint32_t music_volume_saved_bits() const { return data()->music_volume_saved; }
  void set_music_volume_saved_bits(uint32_t v) const { data()->music_volume_saved = v; }
  // the options' byte +29
  uint32_t effects_volume_saved() const { return data()->effects_volume_saved; }
  void set_effects_volume_saved(uint32_t v) const { data()->effects_volume_saved = v; }
  uint32_t effects_volume_saved_bits() const { return data()->effects_volume_saved; }
  void set_effects_volume_saved_bits(uint32_t v) const { data()->effects_volume_saved = v; }
  // three values -10 to 10 (the options' bytes +36 to +38)
  uint32_t adjust_saved_0() const { return data()->adjust_saved_0; }
  void set_adjust_saved_0(uint32_t v) const { data()->adjust_saved_0 = v; }
  uint32_t adjust_saved_0_bits() const { return data()->adjust_saved_0; }
  void set_adjust_saved_0_bits(uint32_t v) const { data()->adjust_saved_0 = v; }
  uint32_t adjust_saved_1() const { return data()->adjust_saved_1; }
  void set_adjust_saved_1(uint32_t v) const { data()->adjust_saved_1 = v; }
  uint32_t adjust_saved_1_bits() const { return data()->adjust_saved_1; }
  void set_adjust_saved_1_bits(uint32_t v) const { data()->adjust_saved_1 = v; }
  uint32_t adjust_saved_2() const { return data()->adjust_saved_2; }
  void set_adjust_saved_2(uint32_t v) const { data()->adjust_saved_2 = v; }
  uint32_t adjust_saved_2_bits() const { return data()->adjust_saved_2; }
  void set_adjust_saved_2_bits(uint32_t v) const { data()->adjust_saved_2 = v; }
  // the HUD options' edits (page 1)
  uint32_t hud_edit_0() const { return data()->hud_edit_0; }
  void set_hud_edit_0(uint32_t v) const { data()->hud_edit_0 = v; }
  uint32_t hud_edit_0_bits() const { return data()->hud_edit_0; }
  void set_hud_edit_0_bits(uint32_t v) const { data()->hud_edit_0 = v; }
  uint32_t hud_edit_1() const { return data()->hud_edit_1; }
  void set_hud_edit_1(uint32_t v) const { data()->hud_edit_1 = v; }
  uint32_t hud_edit_1_bits() const { return data()->hud_edit_1; }
  void set_hud_edit_1_bits(uint32_t v) const { data()->hud_edit_1 = v; }
  uint32_t hud_edit_2() const { return data()->hud_edit_2; }
  void set_hud_edit_2(uint32_t v) const { data()->hud_edit_2 = v; }
  uint32_t hud_edit_2_bits() const { return data()->hud_edit_2; }
  void set_hud_edit_2_bits(uint32_t v) const { data()->hud_edit_2 = v; }
  uint32_t hud_edit_3() const { return data()->hud_edit_3; }
  void set_hud_edit_3(uint32_t v) const { data()->hud_edit_3 = v; }
  uint32_t hud_edit_3_bits() const { return data()->hud_edit_3; }
  void set_hud_edit_3_bits(uint32_t v) const { data()->hud_edit_3 = v; }
  // the volumes' edits (page 2), 0 to 15: SoundOptions::SetMusicVolume's (+52) and SetEffectsVolume's (+56)
  uint32_t music_volume() const { return data()->music_volume; }
  void set_music_volume(uint32_t v) const { data()->music_volume = v; }
  uint32_t music_volume_bits() const { return data()->music_volume; }
  void set_music_volume_bits(uint32_t v) const { data()->music_volume = v; }
  uint32_t effects_volume() const { return data()->effects_volume; }
  void set_effects_volume(uint32_t v) const { data()->effects_volume = v; }
  uint32_t effects_volume_bits() const { return data()->effects_volume; }
  void set_effects_volume_bits(uint32_t v) const { data()->effects_volume = v; }
  // the three values' edits (page 3), -10 to 10
  uint32_t adjust_0() const { return data()->adjust_0; }
  void set_adjust_0(uint32_t v) const { data()->adjust_0 = v; }
  uint32_t adjust_0_bits() const { return data()->adjust_0; }
  void set_adjust_0_bits(uint32_t v) const { data()->adjust_0 = v; }
  uint32_t adjust_1() const { return data()->adjust_1; }
  void set_adjust_1(uint32_t v) const { data()->adjust_1 = v; }
  uint32_t adjust_1_bits() const { return data()->adjust_1; }
  void set_adjust_1_bits(uint32_t v) const { data()->adjust_1 = v; }
  uint32_t adjust_2() const { return data()->adjust_2; }
  void set_adjust_2(uint32_t v) const { data()->adjust_2 = v; }
  uint32_t adjust_2_bits() const { return data()->adjust_2; }
  void set_adjust_2_bits(uint32_t v) const { data()->adjust_2 = v; }
  // the HUD options' defaults (the last item's Default), 1 at LoadData
  uint32_t hud_default_0() const { return data()->hud_default_0; }
  void set_hud_default_0(uint32_t v) const { data()->hud_default_0 = v; }
  uint32_t hud_default_0_bits() const { return data()->hud_default_0; }
  void set_hud_default_0_bits(uint32_t v) const { data()->hud_default_0 = v; }
  // 1 at LoadData
  uint32_t hud_default_1() const { return data()->hud_default_1; }
  void set_hud_default_1(uint32_t v) const { data()->hud_default_1 = v; }
  uint32_t hud_default_1_bits() const { return data()->hud_default_1; }
  void set_hud_default_1_bits(uint32_t v) const { data()->hud_default_1 = v; }
  // 1 at LoadData
  uint32_t hud_default_2() const { return data()->hud_default_2; }
  void set_hud_default_2(uint32_t v) const { data()->hud_default_2 = v; }
  uint32_t hud_default_2_bits() const { return data()->hud_default_2; }
  void set_hud_default_2_bits(uint32_t v) const { data()->hud_default_2 = v; }
  // 1 at LoadData
  uint32_t hud_default_3() const { return data()->hud_default_3; }
  void set_hud_default_3(uint32_t v) const { data()->hud_default_3 = v; }
  uint32_t hud_default_3_bits() const { return data()->hud_default_3; }
  void set_hud_default_3_bits(uint32_t v) const { data()->hud_default_3 = v; }
  // the volumes' defaults, 15 at LoadData
  uint32_t music_volume_default() const { return data()->music_volume_default; }
  void set_music_volume_default(uint32_t v) const { data()->music_volume_default = v; }
  uint32_t music_volume_default_bits() const { return data()->music_volume_default; }
  void set_music_volume_default_bits(uint32_t v) const { data()->music_volume_default = v; }
  // 15 at LoadData (probably)
  uint32_t effects_volume_default() const { return data()->effects_volume_default; }
  void set_effects_volume_default(uint32_t v) const { data()->effects_volume_default = v; }
  uint32_t effects_volume_default_bits() const { return data()->effects_volume_default; }
  void set_effects_volume_default_bits(uint32_t v) const { data()->effects_volume_default = v; }
  // the three values' defaults, 0 at LoadData
  uint32_t adjust_default_0() const { return data()->adjust_default_0; }
  void set_adjust_default_0(uint32_t v) const { data()->adjust_default_0 = v; }
  uint32_t adjust_default_0_bits() const { return data()->adjust_default_0; }
  void set_adjust_default_0_bits(uint32_t v) const { data()->adjust_default_0 = v; }
  // 0 at LoadData
  uint32_t adjust_default_1() const { return data()->adjust_default_1; }
  void set_adjust_default_1(uint32_t v) const { data()->adjust_default_1 = v; }
  uint32_t adjust_default_1_bits() const { return data()->adjust_default_1; }
  void set_adjust_default_1_bits(uint32_t v) const { data()->adjust_default_1 = v; }
  // 0 at LoadData
  uint32_t adjust_default_2() const { return data()->adjust_default_2; }
  void set_adjust_default_2(uint32_t v) const { data()->adjust_default_2 = v; }
  uint32_t adjust_default_2_bits() const { return data()->adjust_default_2; }
  void set_adjust_default_2_bits(uint32_t v) const { data()->adjust_default_2 = v; }
  // the save data's +6116 (kinds 0 to 2)
  uint32_t money() const { return data()->money; }
  void set_money(uint32_t v) const { data()->money = v; }
  uint32_t money_bits() const { return data()->money; }
  void set_money_bits(uint32_t v) const { data()->money = v; }
  // the day (save +6100) mod 7
  uint32_t weekday() const { return data()->weekday; }
  void set_weekday(uint32_t v) const { data()->weekday = v; }
  uint32_t weekday_bits() const { return data()->weekday; }
  void set_weekday_bits(uint32_t v) const { data()->weekday = v; }
  // the day, at most 999
  uint32_t day() const { return data()->day; }
  void set_day(uint32_t v) const { data()->day = v; }
  uint32_t day_bits() const { return data()->day; }
  void set_day_bits(uint32_t v) const { data()->day = v; }
  // Stats::WinStreak, 0 to 999
  uint32_t win_streak() const { return data()->win_streak; }
  void set_win_streak(uint32_t v) const { data()->win_streak = v; }
  uint32_t win_streak_bits() const { return data()->win_streak; }
  void set_win_streak_bits(uint32_t v) const { data()->win_streak = v; }
  // the car's float +6248, clamped and truncated
  uint32_t car_value_a() const { return data()->car_value_a; }
  void set_car_value_a(uint32_t v) const { data()->car_value_a = v; }
  uint32_t car_value_a_bits() const { return data()->car_value_a; }
  void set_car_value_a_bits(uint32_t v) const { data()->car_value_a = v; }
  // the car's float +6244, clamped and truncated
  uint32_t car_value_b() const { return data()->car_value_b; }
  void set_car_value_b(uint32_t v) const { data()->car_value_b = v; }
  uint32_t car_value_b_bits() const { return data()->car_value_b; }
  void set_car_value_b_bits(uint32_t v) const { data()->car_value_b = v; }
  // 0 to 8: which items (the table at 0x8200CBF0)
  uint32_t kind() const { return data()->kind; }
  void set_kind(uint32_t v) const { data()->kind = v; }
  uint32_t kind_bits() const { return data()->kind; }
  void set_kind_bits(uint32_t v) const { data()->kind = v; }
  // the kind's items, at most 6
  uint32_t item_count() const { return data()->item_count; }
  void set_item_count(uint32_t v) const { data()->item_count = v; }
  uint32_t item_count_bits() const { return data()->item_count; }
  void set_item_count_bits(uint32_t v) const { data()->item_count = v; }
  // 1 the resume, 2 to 9 the items ending or restarting the race
  uint32_t answer() const { return data()->answer; }
  void set_answer(uint32_t v) const { data()->answer = v; }
  uint32_t answer_bits() const { return data()->answer; }
  void set_answer_bits(uint32_t v) const { data()->answer = v; }
  // the sub-pages' last item: 1 OK (the edits kept), 0 Default (the defaults); the arrow's row
  uint32_t last_item_ok() const { return data()->last_item_ok; }
  void set_last_item_ok(uint32_t v) const { data()->last_item_ok = v; }
  uint32_t last_item_ok_bits() const { return data()->last_item_ok; }
  void set_last_item_ok_bits(uint32_t v) const { data()->last_item_ok = v; }
  // the main list's selection kept while a sub-page shows
  uint32_t main_selection() const { return data()->main_selection; }
  void set_main_selection(uint32_t v) const { data()->main_selection = v; }
  uint32_t main_selection_bits() const { return data()->main_selection; }
  void set_main_selection_bits(uint32_t v) const { data()->main_selection = v; }
  // PauseMenu::RunTen's count, done at 10
  uint32_t run_count() const { return data()->run_count; }
  void set_run_count(uint32_t v) const { data()->run_count = v; }
  uint32_t run_count_bits() const { return data()->run_count; }
  void set_run_count_bits(uint32_t v) const { data()->run_count = v; }
  // Open's flag (RaceHudMachine::IsFirstDay): the items of id 4 disabled
  uint32_t first_day() const { return data()->first_day; }
  void set_first_day(uint32_t v) const { data()->first_day = v; }
  uint32_t first_day_bits() const { return data()->first_day; }
  void set_first_day_bits(uint32_t v) const { data()->first_day = v; }
};

// allocated; measure: new<40>@RaceDialog::Create
struct RaceDialogData : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // 1 when the question was answered yes
  uint32_t yes() const { return guest::LoadU32(base, address + 0); }
  void set_yes(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t yes_address() const { return address + 0; }
  // AskSlot's slot (0 a new one)
  uint32_t slot() const { return guest::LoadU32(base, address + 4); }
  void set_slot(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_address() const { return address + 4; }
  // 0 before Replays::Delete (Question's message 5)
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // the message (ShowMessage's jump table, 0 to 11), 13 the question
  uint32_t message() const { return guest::LoadU32(base, address + 12); }
  void set_message(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t message_address() const { return address + 12; }
  // how OpenAnim and ShowMessage open it: 0, 1 (with the list) or 2
  uint32_t open_style() const { return guest::LoadU32(base, address + 16); }
  void set_open_style(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t open_style_address() const { return address + 16; }
  // open_style after OpenAnim
  uint32_t next_open_style() const { return guest::LoadU32(base, address + 20); }
  void set_next_open_style(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t next_open_style_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // the list's selection at opening
  uint32_t selection() const { return guest::LoadU32(base, address + 28); }
  void set_selection(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t selection_address() const { return address + 28; }
  // 1 while the dialog shows (RaceDialog::Answer clears it)
  uint32_t showing() const { return guest::LoadU32(base, address + 32); }
  void set_showing(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t showing_address() const { return address + 32; }
  // 1: Update goes to page 5
  uint32_t close_asked() const { return guest::LoadU32(base, address + 36); }
  void set_close_asked(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t close_asked_address() const { return address + 36; }
};

// allocated; measure: CareerResults
struct CareerResultsScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 212;
  // 0 to 2 by the race's result; the transitions' r3 (the step word, +208) is also measured here (a measure fault)
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // the opponents' prizes summed
  uint32_t opponent_prize() const { return guest::LoadU32(base, address + 4); }
  void set_opponent_prize(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t opponent_prize_address() const { return address + 4; }
  // the win's prize by the race's time
  uint32_t win_prize() const { return guest::LoadU32(base, address + 8); }
  void set_win_prize(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t win_prize_address() const { return address + 8; }
  // by the day's win streak
  float streak_factor() const { return guest::LoadFloat(base, address + 12); }
  void set_streak_factor(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t streak_factor_address() const { return address + 12; }
  // (opponent_prize + win_prize) x streak_factor
  uint32_t prize() const { return guest::LoadU32(base, address + 16); }
  void set_prize(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t prize_address() const { return address + 16; }
  // the opponents' item bonuses summed
  uint32_t item_bonus() const { return guest::LoadU32(base, address + 20); }
  void set_item_bonus(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t item_bonus_address() const { return address + 20; }
  // the result's float +844; the entries' first
  float entry_value() const { return guest::LoadFloat(base, address + 24); }
  void set_entry_value(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t entry_value_address() const { return address + 24; }
  // entry_value x the scale, truncated
  uint32_t entry_points() const { return guest::LoadU32(base, address + 28); }
  void set_entry_points(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t entry_points_address() const { return address + 28; }
  // (float, int) pairs, 8 bytes each, pair_count of them
  uint32_t pairs_address() const { return address + 32; }
  uint32_t pair_count() const { return guest::LoadU32(base, address + 160); }
  void set_pair_count(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t pair_count_address() const { return address + 160; }
  // the save data's +6116 before the prize
  uint32_t money() const { return guest::LoadU32(base, address + 164); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t money_address() const { return address + 164; }
  // prize + item_bonus + entry_points + money, at least 0
  uint32_t total() const { return guest::LoadU32(base, address + 168); }
  void set_total(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t total_address() const { return address + 168; }
  // the entry NextEntry shows
  uint32_t entry() const { return guest::LoadU32(base, address + 172); }
  void set_entry(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t entry_address() const { return address + 172; }
  // UI movie 6
  View movie() const { return View(base, guest::LoadU32(base, address + 180)); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t movie_address() const { return address + 180; }
  // its page object (PageObject) at +184: the page
  uint32_t pages_page() const { return guest::LoadU32(base, address + 184); }
  void set_pages_page(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t pages_page_address() const { return address + 184; }
  uint32_t pages_next_page() const { return guest::LoadU32(base, address + 188); }
  void set_pages_next_page(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t pages_next_page_address() const { return address + 188; }
  uint32_t pages_previous_page() const { return guest::LoadU32(base, address + 192); }
  void set_pages_previous_page(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t pages_previous_page_address() const { return address + 192; }
  // the running transition, -1 none
  uint32_t pages_step() const { return guest::LoadU32(base, address + 196); }
  void set_pages_step(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t pages_step_address() const { return address + 196; }
  uint32_t pages_queued_step() const { return guest::LoadU32(base, address + 200); }
  void set_pages_queued_step(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t pages_queued_step_address() const { return address + 200; }
  uint32_t pages_unknown_20() const { return guest::LoadU32(base, address + 204); }
  void set_pages_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t pages_unknown_20_address() const { return address + 204; }
  uint32_t pages_step_started() const { return guest::LoadU32(base, address + 208); }
  void set_pages_step_started(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t pages_step_started_address() const { return address + 208; }
};

// allocated; measure: new<4>@AfterRaceMenu::Create, AfterRaceMenu
struct AfterRaceMenuScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // UI movie 9 (UiMovies::Find)
  View movie() const { return View(base, guest::LoadU32(base, address + 0)); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t movie_address() const { return address + 0; }
  // measure fault: a replay record's halfwords +0 to +10, the date and time (probably a SYSTEMTIME's)
  uint16_t fault_replay_year() const { return guest::LoadU16(base, address + 0); }
  void set_fault_replay_year(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_replay_year_address() const { return address + 0; }
  // measure fault
  uint16_t fault_replay_month() const { return guest::LoadU16(base, address + 2); }
  void set_fault_replay_month(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t fault_replay_month_address() const { return address + 2; }
  // measure fault
  uint16_t fault_replay_day() const { return guest::LoadU16(base, address + 6); }
  void set_fault_replay_day(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t fault_replay_day_address() const { return address + 6; }
  // measure fault
  uint16_t fault_replay_hour() const { return guest::LoadU16(base, address + 8); }
  void set_fault_replay_hour(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t fault_replay_hour_address() const { return address + 8; }
  // measure fault
  uint16_t fault_replay_minute() const { return guest::LoadU16(base, address + 10); }
  void set_fault_replay_minute(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t fault_replay_minute_address() const { return address + 10; }
  // measure fault: minutes, seconds and thousandths packed
  uint32_t fault_replay_time() const { return guest::LoadU32(base, address + 16); }
  void set_fault_replay_time(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t fault_replay_time_address() const { return address + 16; }
  // measure fault
  uint32_t fault_replay_mode() const { return guest::LoadU32(base, address + 20); }
  void set_fault_replay_mode(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t fault_replay_mode_address() const { return address + 20; }
  // measure fault
  uint32_t fault_replay_course() const { return guest::LoadU32(base, address + 24); }
  void set_fault_replay_course(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t fault_replay_course_address() const { return address + 24; }
  // measure fault: the replay's number, 0 for none
  uint32_t fault_replay_number() const { return guest::LoadU32(base, address + 28); }
  void set_fault_replay_number(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t fault_replay_number_address() const { return address + 28; }
  // measure fault
  uint32_t fault_replay_result() const { return guest::LoadU32(base, address + 32); }
  void set_fault_replay_result(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t fault_replay_result_address() const { return address + 32; }
};

// allocated; measure: new<24>@AfterRaceMenu::Create
struct AfterRaceMenuData : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // 0 to 6: which items (the table at 0x82010428)
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // the kind's items, at most 4
  uint32_t item_count() const { return guest::LoadU32(base, address + 4); }
  void set_item_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t item_count_address() const { return address + 4; }
  // Open's flag: items 1 and 2 (the replay's) disabled
  uint32_t no_replay() const { return guest::LoadU32(base, address + 8); }
  void set_no_replay(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t no_replay_address() const { return address + 8; }
  uint32_t answer() const { return guest::LoadU32(base, address + 12); }
  void set_answer(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t answer_address() const { return address + 12; }
  // the main list's selection kept
  uint32_t selection() const { return guest::LoadU32(base, address + 16); }
  void set_selection(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t selection_address() const { return address + 16; }
  // 0 at Open; 1 from sub_82211E90, 0 from sub_82211F18
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
};

// The image words docs/functions/race-screens.md's code uses.
struct RaceScreensGlobals {
  template <typename T = PauseMenuScreen>
  static T pause_screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84D0u)); }
  static void set_pause_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84D0u, v.address); }
  static void set_pause_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84D0u, v); }
  static constexpr uint32_t pause_screen_address() { return 0x826D84D0u; }
  template <typename T = PauseMenuData>
  static T pause_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84D4u)); }
  static void set_pause_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84D4u, v.address); }
  static void set_pause_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84D4u, v); }
  static constexpr uint32_t pause_data_address() { return 0x826D84D4u; }
  template <typename T = PageObject>
  static T pause_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84D8u)); }
  static void set_pause_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84D8u, v.address); }
  static void set_pause_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84D8u, v); }
  static constexpr uint32_t pause_pages_address() { return 0x826D84D8u; }
  template <typename T = MenuList>
  static T pause_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84DCu)); }
  static void set_pause_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84DCu, v.address); }
  static void set_pause_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84DCu, v); }
  static constexpr uint32_t pause_list_address() { return 0x826D84DCu; }
  template <typename T = MenuList>
  static T dialog_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84F4u)); }
  static void set_dialog_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84F4u, v.address); }
  static void set_dialog_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84F4u, v); }
  static constexpr uint32_t dialog_list_address() { return 0x826D84F4u; }
  template <typename T = PageObject>
  static T dialog_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84F8u)); }
  static void set_dialog_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84F8u, v.address); }
  static void set_dialog_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84F8u, v); }
  static constexpr uint32_t dialog_pages_address() { return 0x826D84F8u; }
  template <typename T = RaceDialogData>
  static T dialog_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84FCu)); }
  static void set_dialog_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84FCu, v.address); }
  static void set_dialog_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84FCu, v); }
  static constexpr uint32_t dialog_data_address() { return 0x826D84FCu; }
  template <typename T = CareerResultsScreen>
  static T career_results(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8500u)); }
  static void set_career_results(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8500u, v.address); }
  static void set_career_results(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8500u, v); }
  static constexpr uint32_t career_results_address() { return 0x826D8500u; }
  template <typename T = AfterRaceMenuScreen>
  static T after_screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8510u)); }
  static void set_after_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8510u, v.address); }
  static void set_after_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8510u, v); }
  static constexpr uint32_t after_screen_address() { return 0x826D8510u; }
  template <typename T = AfterRaceMenuData>
  static T after_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8514u)); }
  static void set_after_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8514u, v.address); }
  static void set_after_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8514u, v); }
  static constexpr uint32_t after_data_address() { return 0x826D8514u; }
  template <typename T = PageObject>
  static T after_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8518u)); }
  static void set_after_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8518u, v.address); }
  static void set_after_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8518u, v); }
  static constexpr uint32_t after_pages_address() { return 0x826D8518u; }
  template <typename T = MenuList>
  static T after_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D851Cu)); }
  static void set_after_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D851Cu, v.address); }
  static void set_after_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D851Cu, v); }
  static constexpr uint32_t after_list_address() { return 0x826D851Cu; }
};

}  // namespace schema
