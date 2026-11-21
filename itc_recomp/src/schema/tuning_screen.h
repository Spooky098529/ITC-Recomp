// The views of docs/functions/tuning-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/tuning-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: TuningScreen
struct TuningScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 91824;
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::PlaceStickerNode)
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::PlaceStickerNode)
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::PlaceStickerNode)
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::PlaceStickerNode)
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::PlaceStickerNode)
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // the vinyl editor's scroll (state 28, [0x826D84AC]; Command23 reads it, then 0)
  uint32_t scroll_direction() const { return guest::LoadU32(base, address + 9640); }
  void set_scroll_direction(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t scroll_direction_address() const { return address + 9640; }
  // the vinyl editor's animation pair (Command21 sets it, the update passes it to Command20)
  uint32_t vinyl_animation_a() const { return guest::LoadU32(base, address + 9644); }
  void set_vinyl_animation_a(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t vinyl_animation_a_address() const { return address + 9644; }
  // the pair's second
  uint32_t vinyl_animation_b() const { return guest::LoadU32(base, address + 9648); }
  void set_vinyl_animation_b(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t vinyl_animation_b_address() const { return address + 9648; }
  // the state machine (3 to 58; 61 picks movie 13 or 14, no handler)
  uint32_t state() const { return guest::LoadU32(base, address + 9652); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t state_address() const { return address + 9652; }
  // 0 power, 1 power train, 2 body, 3 tire, 4 aero, 5 exterior, 6 interior
  uint32_t category() const { return guest::LoadU32(base, address + 9656); }
  void set_category(uint32_t v) const { guest::StoreU32(base, address + 9656, v); }
  uint32_t category_address() const { return address + 9656; }
  // the item in the category
  uint8_t item() const { return guest::LoadU8(base, address + 9660); }
  void set_item(uint8_t v) const { guest::StoreU8(base, address + 9660, v); }
  uint32_t item_address() const { return address + 9660; }
  // the level pick
  uint8_t level() const { return guest::LoadU8(base, address + 9661); }
  void set_level(uint8_t v) const { guest::StoreU8(base, address + 9661, v); }
  uint32_t level_address() const { return address + 9661; }
  // the type pick
  uint8_t type() const { return guest::LoadU8(base, address + 9662); }
  void set_type(uint8_t v) const { guest::StoreU8(base, address + 9662, v); }
  uint32_t type_address() const { return address + 9662; }
  // the bonnet's choice (the second type pick)
  uint8_t bonnet_choice() const { return guest::LoadU8(base, address + 9663); }
  void set_bonnet_choice(uint8_t v) const { guest::StoreU8(base, address + 9663, v); }
  uint32_t bonnet_choice_address() const { return address + 9663; }
  // the option pick (color, hardness or door type)
  uint8_t option() const { return guest::LoadU8(base, address + 9664); }
  void set_option(uint8_t v) const { guest::StoreU8(base, address + 9664, v); }
  uint32_t option_address() const { return address + 9664; }
  // the buy and equip dialogs' cursor (states 9 and 11)
  uint8_t confirm_cursor() const { return guest::LoadU8(base, address + 9665); }
  void set_confirm_cursor(uint8_t v) const { guest::StoreU8(base, address + 9665, v); }
  uint32_t confirm_cursor_address() const { return address + 9665; }
  uint8_t unknown_9666() const { return guest::LoadU8(base, address + 9666); }
  void set_unknown_9666(uint8_t v) const { guest::StoreU8(base, address + 9666, v); }
  uint32_t unknown_9666_address() const { return address + 9666; }
  // state 12's cursor
  uint8_t keep_cursor() const { return guest::LoadU8(base, address + 9667); }
  void set_keep_cursor(uint8_t v) const { guest::StoreU8(base, address + 9667, v); }
  uint32_t keep_cursor_address() const { return address + 9667; }
  // state 13's cursor
  uint8_t discard_cursor() const { return guest::LoadU8(base, address + 9668); }
  void set_discard_cursor(uint8_t v) const { guest::StoreU8(base, address + 9668, v); }
  uint32_t discard_cursor_address() const { return address + 9668; }
  // state 14's cursor
  uint32_t remove_cursor() const { return guest::LoadU32(base, address + 9672); }
  void set_remove_cursor(uint32_t v) const { guest::StoreU32(base, address + 9672, v); }
  uint32_t remove_cursor_address() const { return address + 9672; }
  // the category's item count
  uint8_t item_count() const { return guest::LoadU8(base, address + 9676); }
  void set_item_count(uint8_t v) const { guest::StoreU8(base, address + 9676, v); }
  uint32_t item_count_address() const { return address + 9676; }
  // the level list's length
  uint8_t level_count() const { return guest::LoadU8(base, address + 9677); }
  void set_level_count(uint8_t v) const { guest::StoreU8(base, address + 9677, v); }
  uint32_t level_count_address() const { return address + 9677; }
  // the type list's length
  uint8_t type_count() const { return guest::LoadU8(base, address + 9678); }
  void set_type_count(uint8_t v) const { guest::StoreU8(base, address + 9678, v); }
  uint32_t type_count_address() const { return address + 9678; }
  // state 7's rows
  uint8_t color_rows() const { return guest::LoadU8(base, address + 9679); }
  void set_color_rows(uint8_t v) const { guest::StoreU8(base, address + 9679, v); }
  uint32_t color_rows_address() const { return address + 9679; }
  // state 8's options
  uint8_t option_count() const { return guest::LoadU8(base, address + 9680); }
  void set_option_count(uint8_t v) const { guest::StoreU8(base, address + 9680, v); }
  uint32_t option_count_address() const { return address + 9680; }
  // a mask of the items state 4 opens (-1 at creation)
  uint32_t open_items_mask() const { return guest::LoadU32(base, address + 9688); }
  void set_open_items_mask(uint32_t v) const { guest::StoreU32(base, address + 9688, v); }
  uint32_t open_items_mask_address() const { return address + 9688; }
  // the refresh runs while 1 (then set to whether it answered 0)
  uint32_t refresh_pending() const { return guest::LoadU32(base, address + 9708); }
  void set_refresh_pending(uint32_t v) const { guest::StoreU32(base, address + 9708, v); }
  uint32_t refresh_pending_address() const { return address + 9708; }
  // set by the bonnet's color choice
  uint32_t bonnet_color_set() const { return guest::LoadU32(base, address + 9712); }
  void set_bonnet_color_set(uint32_t v) const { guest::StoreU32(base, address + 9712, v); }
  uint32_t bonnet_color_set_address() const { return address + 9712; }
  // a command's first-run flag
  uint32_t command_first_run() const { return guest::LoadU32(base, address + 9716); }
  void set_command_first_run(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t command_first_run_address() const { return address + 9716; }
  // end the command at once after its set-up
  uint32_t command_end_after_setup() const { return guest::LoadU32(base, address + 9720); }
  void set_command_end_after_setup(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t command_end_after_setup_address() const { return address + 9720; }
  // end the command now
  uint32_t command_end_now() const { return guest::LoadU32(base, address + 9724); }
  void set_command_end_now(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t command_end_now_address() const { return address + 9724; }
  // the input pages run only while set
  uint32_t pages_enabled() const { return guest::LoadU32(base, address + 9728); }
  void set_pages_enabled(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t pages_enabled_address() const { return address + 9728; }
  // the stand and garage car rewrite request
  uint32_t rewrite_car() const { return guest::LoadU32(base, address + 9732); }
  void set_rewrite_car(uint32_t v) const { guest::StoreU32(base, address + 9732, v); }
  uint32_t rewrite_car_address() const { return address + 9732; }
  // the category copied from the menu
  uint32_t menu_category() const { return guest::LoadU32(base, address + 9740); }
  void set_menu_category(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t menu_category_address() const { return address + 9740; }
  // the level, type or option copied from the menu
  uint32_t menu_pick() const { return guest::LoadU32(base, address + 9744); }
  void set_menu_pick(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t menu_pick_address() const { return address + 9744; }
  uint32_t unknown_9748() const { return guest::LoadU32(base, address + 9748); }
  void set_unknown_9748(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t unknown_9748_address() const { return address + 9748; }
  // the states pushed before a sub-page (4 bytes each by byte +10008; no bound check)
  uint32_t state_history_address() const { return address + 9756; }
  // the state history's top
  uint8_t history_depth() const { return guest::LoadU8(base, address + 10008); }
  void set_history_depth(uint8_t v) const { guest::StoreU8(base, address + 10008, v); }
  uint32_t history_depth_address() const { return address + 10008; }
  // a pending stand action (1 to 4 ApplyAreaDecals, 5 to 8 ApplyDecals, 9 and 10 ApplyDecalWord, -1 none)
  uint32_t stand_action() const { return guest::LoadU32(base, address + 10012); }
  void set_stand_action(uint32_t v) const { guest::StoreU32(base, address + 10012, v); }
  uint32_t stand_action_address() const { return address + 10012; }
  // the busy icon shown
  uint32_t busy_shown() const { return guest::LoadU32(base, address + 10016); }
  void set_busy_shown(uint32_t v) const { guest::StoreU32(base, address + 10016, v); }
  uint32_t busy_shown_address() const { return address + 10016; }
  // the busy icon's movie
  uint32_t busy_movie() const { return guest::LoadU32(base, address + 10020); }
  void set_busy_movie(uint32_t v) const { guest::StoreU32(base, address + 10020, v); }
  uint32_t busy_movie_address() const { return address + 10020; }
  // the busy icon's named animations
  uint32_t busy_animation_0() const { return guest::LoadU32(base, address + 10024); }
  void set_busy_animation_0(uint32_t v) const { guest::StoreU32(base, address + 10024, v); }
  uint32_t busy_animation_0_address() const { return address + 10024; }
  uint32_t busy_animation_1() const { return guest::LoadU32(base, address + 10028); }
  void set_busy_animation_1(uint32_t v) const { guest::StoreU32(base, address + 10028, v); }
  uint32_t busy_animation_1_address() const { return address + 10028; }
  uint32_t busy_animation_2() const { return guest::LoadU32(base, address + 10032); }
  void set_busy_animation_2(uint32_t v) const { guest::StoreU32(base, address + 10032, v); }
  uint32_t busy_animation_2_address() const { return address + 10032; }
  // the command queue (28 bytes, VirtualAlloc'ed by TuningMachine::Create)
  template <typename T = TuningCommandQueue>
  T queue() const { return T(base, guest::LoadU32(base, address + 10036)); }
  void set_queue(const View& v) const { guest::StoreU32(base, address + 10036, v.address); }
  void set_queue(uint32_t v) const { guest::StoreU32(base, address + 10036, v); }
  uint32_t queue_address() const { return address + 10036; }
  // the category menu (68 bytes, a front-end menu)
  template <typename T = FrontEndMenu>
  T category_menu() const { return T(base, guest::LoadU32(base, address + 10040)); }
  void set_category_menu(const View& v) const { guest::StoreU32(base, address + 10040, v.address); }
  void set_category_menu(uint32_t v) const { guest::StoreU32(base, address + 10040, v); }
  uint32_t category_menu_address() const { return address + 10040; }
  // the sticker editor's menu (68 bytes, a front-end menu)
  template <typename T = FrontEndMenu>
  T sticker_menu() const { return T(base, guest::LoadU32(base, address + 10044)); }
  void set_sticker_menu(const View& v) const { guest::StoreU32(base, address + 10044, v.address); }
  void set_sticker_menu(uint32_t v) const { guest::StoreU32(base, address + 10044, v); }
  uint32_t sticker_menu_address() const { return address + 10044; }
  // the decal images (tuning-pages.toml's DecalImages: 200 36-byte entries, 200 kept textures of 8 bytes from +7236, the placeholders +8836)
  uint32_t decal_images_address() const { return address + 10048; }
  // movies 13's and 14's placeholder textures (the decal images' +8836 to +8848)
  uint32_t placeholder_texture_0() const { return guest::LoadU32(base, address + 18884); }
  void set_placeholder_texture_0(uint32_t v) const { guest::StoreU32(base, address + 18884, v); }
  uint32_t placeholder_texture_0_address() const { return address + 18884; }
  uint32_t placeholder_texture_1() const { return guest::LoadU32(base, address + 18888); }
  void set_placeholder_texture_1(uint32_t v) const { guest::StoreU32(base, address + 18888, v); }
  uint32_t placeholder_texture_1_address() const { return address + 18888; }
  uint32_t placeholder_texture_2() const { return guest::LoadU32(base, address + 18892); }
  void set_placeholder_texture_2(uint32_t v) const { guest::StoreU32(base, address + 18892, v); }
  uint32_t placeholder_texture_2_address() const { return address + 18892; }
  uint32_t placeholder_texture_3() const { return guest::LoadU32(base, address + 18896); }
  void set_placeholder_texture_3(uint32_t v) const { guest::StoreU32(base, address + 18896, v); }
  uint32_t placeholder_texture_3_address() const { return address + 18896; }
  // set when a vinyl page is kept (the decal images' +8852)
  uint32_t vinyl_kept() const { return guest::LoadU32(base, address + 18900); }
  void set_vinyl_kept(uint32_t v) const { guest::StoreU32(base, address + 18900, v); }
  uint32_t vinyl_kept_address() const { return address + 18900; }
  // player 0's save data (SaveData::Of; its +6120 the garage)
  View save_data() const { return View(base, guest::LoadU32(base, address + 18904)); }
  void set_save_data(uint32_t v) const { guest::StoreU32(base, address + 18904, v); }
  uint32_t save_data_address() const { return address + 18904; }
  // a car data (CarData::, PowerGraph::Draw; its +1268 the screen's +20180)
  uint32_t car_data_address() const { return address + 18912; }
  // SetCarStats' stats
  uint32_t stat_displacement() const { return guest::LoadU32(base, address + 20768); }
  void set_stat_displacement(uint32_t v) const { guest::StoreU32(base, address + 20768, v); }
  uint32_t stat_displacement_address() const { return address + 20768; }
  uint32_t stat_weight() const { return guest::LoadU32(base, address + 20772); }
  void set_stat_weight(uint32_t v) const { guest::StoreU32(base, address + 20772, v); }
  uint32_t stat_weight_address() const { return address + 20772; }
  uint32_t stat_peak_power() const { return guest::LoadU32(base, address + 20776); }
  void set_stat_peak_power(uint32_t v) const { guest::StoreU32(base, address + 20776, v); }
  uint32_t stat_peak_power_address() const { return address + 20776; }
  float stat_torque() const { return guest::LoadFloat(base, address + 20780); }
  void set_stat_torque(float v) const { guest::StoreFloat(base, address + 20780, v); }
  uint32_t stat_torque_address() const { return address + 20780; }
  // the car spec's byte +69
  uint32_t spec_byte_69() const { return guest::LoadU32(base, address + 20784); }
  void set_spec_byte_69(uint32_t v) const { guest::StoreU32(base, address + 20784, v); }
  uint32_t spec_byte_69_address() const { return address + 20784; }
  // the car spec's byte +130
  uint32_t spec_byte_130() const { return guest::LoadU32(base, address + 20788); }
  void set_spec_byte_130(uint32_t v) const { guest::StoreU32(base, address + 20788, v); }
  uint32_t spec_byte_130_address() const { return address + 20788; }
  // a copy of the career's pick (+20792 to +20808; a name record shown in movie 2's sub-scene 9 child 6)
  uint32_t career_pick() const { return guest::LoadU32(base, address + 20792); }
  void set_career_pick(uint32_t v) const { guest::StoreU32(base, address + 20792, v); }
  uint32_t career_pick_address() const { return address + 20792; }
  // a scene model record copied from the stand car (state 4's A)
  uint32_t stand_record_address() const { return address + 20816; }
  // the camera preset command last picked
  uint32_t camera_preset_last() const { return guest::LoadU32(base, address + 25072); }
  void set_camera_preset_last(uint32_t v) const { guest::StoreU32(base, address + 25072, v); }
  uint32_t camera_preset_last_address() const { return address + 25072; }
  // the part slot last previewed
  uint32_t part_slot_last() const { return guest::LoadU32(base, address + 25076); }
  void set_part_slot_last(uint32_t v) const { guest::StoreU32(base, address + 25076, v); }
  uint32_t part_slot_last_address() const { return address + 25076; }
  uint32_t unknown_25080() const { return guest::LoadU32(base, address + 25080); }
  void set_unknown_25080(uint32_t v) const { guest::StoreU32(base, address + 25080, v); }
  uint32_t unknown_25080_address() const { return address + 25080; }
  // the day (at most 999), the header's text
  uint32_t day() const { return guest::LoadU32(base, address + 25092); }
  void set_day(uint32_t v) const { guest::StoreU32(base, address + 25092, v); }
  uint32_t day_address() const { return address + 25092; }
  // the header's texture
  uint32_t weekday() const { return guest::LoadU32(base, address + 25096); }
  void set_weekday(uint32_t v) const { guest::StoreU32(base, address + 25096, v); }
  uint32_t weekday_address() const { return address + 25096; }
  // the list's scrolled row (category 4) or the level for long lists
  uint32_t list_scroll_row() const { return guest::LoadU32(base, address + 25100); }
  void set_list_scroll_row(uint32_t v) const { guest::StoreU32(base, address + 25100, v); }
  uint32_t list_scroll_row_address() const { return address + 25100; }
  uint32_t unknown_25104() const { return guest::LoadU32(base, address + 25104); }
  void set_unknown_25104(uint32_t v) const { guest::StoreU32(base, address + 25104, v); }
  uint32_t unknown_25104_address() const { return address + 25104; }
  // the types list's row
  uint32_t types_row() const { return guest::LoadU32(base, address + 25108); }
  void set_types_row(uint32_t v) const { guest::StoreU32(base, address + 25108, v); }
  uint32_t types_row_address() const { return address + 25108; }
  uint32_t types_row_previous() const { return guest::LoadU32(base, address + 25112); }
  void set_types_row_previous(uint32_t v) const { guest::StoreU32(base, address + 25112, v); }
  uint32_t types_row_previous_address() const { return address + 25112; }
  // the save's +6116
  uint32_t money() const { return guest::LoadU32(base, address + 25116); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 25116, v); }
  uint32_t money_address() const { return address + 25116; }
  // GetPrice's answer kept
  uint32_t price() const { return guest::LoadU32(base, address + 25120); }
  void set_price(uint32_t v) const { guest::StoreU32(base, address + 25120, v); }
  uint32_t price_address() const { return address + 25120; }
  // the aero items' availability (SetAeroAvailability)
  uint32_t aero_available_0() const { return guest::LoadU32(base, address + 25128); }
  void set_aero_available_0(uint32_t v) const { guest::StoreU32(base, address + 25128, v); }
  uint32_t aero_available_0_address() const { return address + 25128; }
  uint32_t aero_available_1() const { return guest::LoadU32(base, address + 25132); }
  void set_aero_available_1(uint32_t v) const { guest::StoreU32(base, address + 25132, v); }
  uint32_t aero_available_1_address() const { return address + 25132; }
  uint32_t aero_available_2() const { return guest::LoadU32(base, address + 25136); }
  void set_aero_available_2(uint32_t v) const { guest::StoreU32(base, address + 25136, v); }
  uint32_t aero_available_2_address() const { return address + 25136; }
  uint32_t aero_available_3() const { return guest::LoadU32(base, address + 25140); }
  void set_aero_available_3(uint32_t v) const { guest::StoreU32(base, address + 25140, v); }
  uint32_t aero_available_3_address() const { return address + 25140; }
  uint32_t aero_available_4() const { return guest::LoadU32(base, address + 25144); }
  void set_aero_available_4(uint32_t v) const { guest::StoreU32(base, address + 25144, v); }
  uint32_t aero_available_4_address() const { return address + 25144; }
  uint32_t aero_available_5() const { return guest::LoadU32(base, address + 25148); }
  void set_aero_available_5(uint32_t v) const { guest::StoreU32(base, address + 25148, v); }
  uint32_t aero_available_5_address() const { return address + 25148; }
  uint32_t aero_available_6() const { return guest::LoadU32(base, address + 25152); }
  void set_aero_available_6(uint32_t v) const { guest::StoreU32(base, address + 25152, v); }
  uint32_t aero_available_6_address() const { return address + 25152; }
  uint32_t aero_available_7() const { return guest::LoadU32(base, address + 25156); }
  void set_aero_available_7(uint32_t v) const { guest::StoreU32(base, address + 25156, v); }
  uint32_t aero_available_7_address() const { return address + 25156; }
  uint32_t aero_available_8() const { return guest::LoadU32(base, address + 25160); }
  void set_aero_available_8(uint32_t v) const { guest::StoreU32(base, address + 25160, v); }
  uint32_t aero_available_8_address() const { return address + 25160; }
  uint32_t aero_available_9() const { return guest::LoadU32(base, address + 25164); }
  void set_aero_available_9(uint32_t v) const { guest::StoreU32(base, address + 25164, v); }
  uint32_t aero_available_9_address() const { return address + 25164; }
  // aero item 9's third type
  uint32_t aero9_third_type() const { return guest::LoadU32(base, address + 25168); }
  void set_aero9_third_type(uint32_t v) const { guest::StoreU32(base, address + 25168, v); }
  uint32_t aero9_third_type_address() const { return address + 25168; }
  // aero item 3's type remap (the spec's +120 set)
  uint32_t aero3_type_remap() const { return guest::LoadU32(base, address + 25172); }
  void set_aero3_type_remap(uint32_t v) const { guest::StoreU32(base, address + 25172, v); }
  uint32_t aero3_type_remap_address() const { return address + 25172; }
  // the interior's five slots' availability
  uint32_t interior_slot_available_0() const { return guest::LoadU32(base, address + 25176); }
  void set_interior_slot_available_0(uint32_t v) const { guest::StoreU32(base, address + 25176, v); }
  uint32_t interior_slot_available_0_address() const { return address + 25176; }
  uint32_t interior_slot_available_1() const { return guest::LoadU32(base, address + 25180); }
  void set_interior_slot_available_1(uint32_t v) const { guest::StoreU32(base, address + 25180, v); }
  uint32_t interior_slot_available_1_address() const { return address + 25180; }
  uint32_t interior_slot_available_2() const { return guest::LoadU32(base, address + 25184); }
  void set_interior_slot_available_2(uint32_t v) const { guest::StoreU32(base, address + 25184, v); }
  uint32_t interior_slot_available_2_address() const { return address + 25184; }
  uint32_t interior_slot_available_3() const { return guest::LoadU32(base, address + 25188); }
  void set_interior_slot_available_3(uint32_t v) const { guest::StoreU32(base, address + 25188, v); }
  uint32_t interior_slot_available_3_address() const { return address + 25188; }
  uint32_t interior_slot_available_4() const { return guest::LoadU32(base, address + 25192); }
  void set_interior_slot_available_4(uint32_t v) const { guest::StoreU32(base, address + 25192, v); }
  uint32_t interior_slot_available_4_address() const { return address + 25192; }
  // the wheels' part cursor (0 both, 1 front, 2 rear)
  uint32_t wheel_part_cursor() const { return guest::LoadU32(base, address + 25196); }
  void set_wheel_part_cursor(uint32_t v) const { guest::StoreU32(base, address + 25196, v); }
  uint32_t wheel_part_cursor_address() const { return address + 25196; }
  // the wheels' pages' cursors and previous values (not told apart)
  uint32_t unknown_25200() const { return guest::LoadU32(base, address + 25200); }
  void set_unknown_25200(uint32_t v) const { guest::StoreU32(base, address + 25200, v); }
  uint32_t unknown_25200_address() const { return address + 25200; }
  // the part group or wheel brand (0 to 16)
  uint32_t part_group() const { return guest::LoadU32(base, address + 25204); }
  void set_part_group(uint32_t v) const { guest::StoreU32(base, address + 25204, v); }
  uint32_t part_group_address() const { return address + 25204; }
  // 11 picks animation 71 (PlayGroupAnimation)
  uint32_t unknown_25208() const { return guest::LoadU32(base, address + 25208); }
  void set_unknown_25208(uint32_t v) const { guest::StoreU32(base, address + 25208, v); }
  uint32_t unknown_25208_address() const { return address + 25208; }
  // the brand tabs' row
  uint32_t tabs_row() const { return guest::LoadU32(base, address + 25212); }
  void set_tabs_row(uint32_t v) const { guest::StoreU32(base, address + 25212, v); }
  uint32_t tabs_row_address() const { return address + 25212; }
  uint32_t unknown_25216() const { return guest::LoadU32(base, address + 25216); }
  void set_unknown_25216(uint32_t v) const { guest::StoreU32(base, address + 25216, v); }
  uint32_t unknown_25216_address() const { return address + 25216; }
  // the list's cursor (the model in the brand)
  uint32_t list_cursor() const { return guest::LoadU32(base, address + 25220); }
  void set_list_cursor(uint32_t v) const { guest::StoreU32(base, address + 25220, v); }
  uint32_t list_cursor_address() const { return address + 25220; }
  // the list's last index
  uint32_t list_last() const { return guest::LoadU32(base, address + 25224); }
  void set_list_last(uint32_t v) const { guest::StoreU32(base, address + 25224, v); }
  uint32_t list_last_address() const { return address + 25224; }
  // state 17's cursor (the size)
  uint32_t size_cursor() const { return guest::LoadU32(base, address + 25228); }
  void set_size_cursor(uint32_t v) const { guest::StoreU32(base, address + 25228, v); }
  uint32_t size_cursor_address() const { return address + 25228; }
  // state 17's last index
  uint32_t size_last() const { return guest::LoadU32(base, address + 25232); }
  void set_size_last(uint32_t v) const { guest::StoreU32(base, address + 25232, v); }
  uint32_t size_last_address() const { return address + 25232; }
  uint32_t unknown_25236() const { return guest::LoadU32(base, address + 25236); }
  void set_unknown_25236(uint32_t v) const { guest::StoreU32(base, address + 25236, v); }
  uint32_t unknown_25236_address() const { return address + 25236; }
  // set by state 16's B
  uint32_t size_back() const { return guest::LoadU32(base, address + 25240); }
  void set_size_back(uint32_t v) const { guest::StoreU32(base, address + 25240, v); }
  uint32_t size_back_address() const { return address + 25240; }
  // 1 claim them, 2 paint them, 0 done
  uint32_t second_wheels_step() const { return guest::LoadU32(base, address + 25244); }
  void set_second_wheels_step(uint32_t v) const { guest::StoreU32(base, address + 25244, v); }
  uint32_t second_wheels_step_address() const { return address + 25244; }
  // the wheels' prices, 152 x brand + 4 x model
  uint32_t wheel_prices_address() const { return address + 25248; }
  // the saved wheel words (+27836 the front wheel's match, +27840 the rear's)
  uint32_t saved_wheel_both() const { return guest::LoadU32(base, address + 27832); }
  void set_saved_wheel_both(uint32_t v) const { guest::StoreU32(base, address + 27832, v); }
  uint32_t saved_wheel_both_address() const { return address + 27832; }
  uint32_t saved_wheel_front() const { return guest::LoadU32(base, address + 27836); }
  void set_saved_wheel_front(uint32_t v) const { guest::StoreU32(base, address + 27836, v); }
  uint32_t saved_wheel_front_address() const { return address + 27836; }
  uint32_t saved_wheel_rear() const { return guest::LoadU32(base, address + 27840); }
  void set_saved_wheel_rear(uint32_t v) const { guest::StoreU32(base, address + 27840, v); }
  uint32_t saved_wheel_rear_address() const { return address + 27840; }
  uint32_t saved_wheel_3() const { return guest::LoadU32(base, address + 27844); }
  void set_saved_wheel_3(uint32_t v) const { guest::StoreU32(base, address + 27844, v); }
  uint32_t saved_wheel_3_address() const { return address + 27844; }
  uint32_t saved_wheel_4() const { return guest::LoadU32(base, address + 27848); }
  void set_saved_wheel_4(uint32_t v) const { guest::StoreU32(base, address + 27848, v); }
  uint32_t saved_wheel_4_address() const { return address + 27848; }
  uint32_t saved_wheel_5() const { return guest::LoadU32(base, address + 27852); }
  void set_saved_wheel_5(uint32_t v) const { guest::StoreU32(base, address + 27852, v); }
  uint32_t saved_wheel_5_address() const { return address + 27852; }
  // the wheels' flags (probably owned), 152 x group + 4 x index
  uint32_t wheel_flags_address() const { return address + 27856; }
  // the paint page's row: hue, saturation, value, a two-way choice, the buttons
  uint32_t paint_row() const { return guest::LoadU32(base, address + 30440); }
  void set_paint_row(uint32_t v) const { guest::StoreU32(base, address + 30440, v); }
  uint32_t paint_row_address() const { return address + 30440; }
  uint32_t paint_hue() const { return guest::LoadU32(base, address + 30444); }
  void set_paint_hue(uint32_t v) const { guest::StoreU32(base, address + 30444, v); }
  uint32_t paint_hue_address() const { return address + 30444; }
  uint32_t paint_saturation() const { return guest::LoadU32(base, address + 30448); }
  void set_paint_saturation(uint32_t v) const { guest::StoreU32(base, address + 30448, v); }
  uint32_t paint_saturation_address() const { return address + 30448; }
  uint32_t paint_value() const { return guest::LoadU32(base, address + 30452); }
  void set_paint_value(uint32_t v) const { guest::StoreU32(base, address + 30452, v); }
  uint32_t paint_value_address() const { return address + 30452; }
  // the two-way choice
  uint32_t paint_choice() const { return guest::LoadU32(base, address + 30456); }
  void set_paint_choice(uint32_t v) const { guest::StoreU32(base, address + 30456, v); }
  uint32_t paint_choice_address() const { return address + 30456; }
  uint32_t paint_buttons() const { return guest::LoadU32(base, address + 30460); }
  void set_paint_buttons(uint32_t v) const { guest::StoreU32(base, address + 30460, v); }
  uint32_t paint_buttons_address() const { return address + 30460; }
  // the paint page's previous values (probably)
  uint32_t paint_previous_0() const { return guest::LoadU32(base, address + 30464); }
  void set_paint_previous_0(uint32_t v) const { guest::StoreU32(base, address + 30464, v); }
  uint32_t paint_previous_0_address() const { return address + 30464; }
  uint32_t paint_previous_1() const { return guest::LoadU32(base, address + 30468); }
  void set_paint_previous_1(uint32_t v) const { guest::StoreU32(base, address + 30468, v); }
  uint32_t paint_previous_1_address() const { return address + 30468; }
  uint32_t paint_previous_2() const { return guest::LoadU32(base, address + 30472); }
  void set_paint_previous_2(uint32_t v) const { guest::StoreU32(base, address + 30472, v); }
  uint32_t paint_previous_2_address() const { return address + 30472; }
  // the saved paint
  uint32_t saved_paint_0() const { return guest::LoadU32(base, address + 30476); }
  void set_saved_paint_0(uint32_t v) const { guest::StoreU32(base, address + 30476, v); }
  uint32_t saved_paint_0_address() const { return address + 30476; }
  uint32_t saved_paint_1() const { return guest::LoadU32(base, address + 30480); }
  void set_saved_paint_1(uint32_t v) const { guest::StoreU32(base, address + 30480, v); }
  uint32_t saved_paint_1_address() const { return address + 30480; }
  uint32_t saved_paint_2() const { return guest::LoadU32(base, address + 30484); }
  void set_saved_paint_2(uint32_t v) const { guest::StoreU32(base, address + 30484, v); }
  uint32_t saved_paint_2_address() const { return address + 30484; }
  uint32_t saved_paint_3() const { return guest::LoadU32(base, address + 30488); }
  void set_saved_paint_3(uint32_t v) const { guest::StoreU32(base, address + 30488, v); }
  uint32_t saved_paint_3_address() const { return address + 30488; }
  // the light kits' group
  uint32_t light_kit_group() const { return guest::LoadU32(base, address + 30492); }
  void set_light_kit_group(uint32_t v) const { guest::StoreU32(base, address + 30492, v); }
  uint32_t light_kit_group_address() const { return address + 30492; }
  uint32_t unknown_30496() const { return guest::LoadU32(base, address + 30496); }
  void set_unknown_30496(uint32_t v) const { guest::StoreU32(base, address + 30496, v); }
  uint32_t unknown_30496_address() const { return address + 30496; }
  // 0, 1 or 2 remove
  uint32_t light_kit_kind() const { return guest::LoadU32(base, address + 30500); }
  void set_light_kit_kind(uint32_t v) const { guest::StoreU32(base, address + 30500, v); }
  uint32_t light_kit_kind_address() const { return address + 30500; }
  uint32_t unknown_30504() const { return guest::LoadU32(base, address + 30504); }
  void set_unknown_30504(uint32_t v) const { guest::StoreU32(base, address + 30504, v); }
  uint32_t unknown_30504_address() const { return address + 30504; }
  // kind 0's row
  uint32_t kit0_row() const { return guest::LoadU32(base, address + 30508); }
  void set_kit0_row(uint32_t v) const { guest::StoreU32(base, address + 30508, v); }
  uint32_t kit0_row_address() const { return address + 30508; }
  // kind 0's values
  uint32_t kit0_value_0() const { return guest::LoadU32(base, address + 30512); }
  void set_kit0_value_0(uint32_t v) const { guest::StoreU32(base, address + 30512, v); }
  uint32_t kit0_value_0_address() const { return address + 30512; }
  uint32_t kit0_value_1() const { return guest::LoadU32(base, address + 30516); }
  void set_kit0_value_1(uint32_t v) const { guest::StoreU32(base, address + 30516, v); }
  uint32_t kit0_value_1_address() const { return address + 30516; }
  uint32_t kit0_value_2() const { return guest::LoadU32(base, address + 30520); }
  void set_kit0_value_2(uint32_t v) const { guest::StoreU32(base, address + 30520, v); }
  uint32_t kit0_value_2_address() const { return address + 30520; }
  uint32_t kit0_value_3() const { return guest::LoadU32(base, address + 30524); }
  void set_kit0_value_3(uint32_t v) const { guest::StoreU32(base, address + 30524, v); }
  uint32_t kit0_value_3_address() const { return address + 30524; }
  // kind 0's second cursor
  uint32_t kit0_cursor_b() const { return guest::LoadU32(base, address + 30528); }
  void set_kit0_cursor_b(uint32_t v) const { guest::StoreU32(base, address + 30528, v); }
  uint32_t kit0_cursor_b_address() const { return address + 30528; }
  uint32_t unknown_30532() const { return guest::LoadU32(base, address + 30532); }
  void set_unknown_30532(uint32_t v) const { guest::StoreU32(base, address + 30532, v); }
  uint32_t unknown_30532_address() const { return address + 30532; }
  uint32_t unknown_30536() const { return guest::LoadU32(base, address + 30536); }
  void set_unknown_30536(uint32_t v) const { guest::StoreU32(base, address + 30536, v); }
  uint32_t unknown_30536_address() const { return address + 30536; }
  uint32_t unknown_30540() const { return guest::LoadU32(base, address + 30540); }
  void set_unknown_30540(uint32_t v) const { guest::StoreU32(base, address + 30540, v); }
  uint32_t unknown_30540_address() const { return address + 30540; }
  // kind 1's row
  uint32_t kit1_row() const { return guest::LoadU32(base, address + 30544); }
  void set_kit1_row(uint32_t v) const { guest::StoreU32(base, address + 30544, v); }
  uint32_t kit1_row_address() const { return address + 30544; }
  // kind 1's values
  uint32_t kit1_value_0() const { return guest::LoadU32(base, address + 30548); }
  void set_kit1_value_0(uint32_t v) const { guest::StoreU32(base, address + 30548, v); }
  uint32_t kit1_value_0_address() const { return address + 30548; }
  uint32_t kit1_value_1() const { return guest::LoadU32(base, address + 30552); }
  void set_kit1_value_1(uint32_t v) const { guest::StoreU32(base, address + 30552, v); }
  uint32_t kit1_value_1_address() const { return address + 30552; }
  uint32_t kit1_value_2() const { return guest::LoadU32(base, address + 30556); }
  void set_kit1_value_2(uint32_t v) const { guest::StoreU32(base, address + 30556, v); }
  uint32_t kit1_value_2_address() const { return address + 30556; }
  // kind 1's second cursor
  uint32_t kit1_cursor_b() const { return guest::LoadU32(base, address + 30560); }
  void set_kit1_cursor_b(uint32_t v) const { guest::StoreU32(base, address + 30560, v); }
  uint32_t kit1_cursor_b_address() const { return address + 30560; }
  uint32_t unknown_30564() const { return guest::LoadU32(base, address + 30564); }
  void set_unknown_30564(uint32_t v) const { guest::StoreU32(base, address + 30564, v); }
  uint32_t unknown_30564_address() const { return address + 30564; }
  uint32_t unknown_30568() const { return guest::LoadU32(base, address + 30568); }
  void set_unknown_30568(uint32_t v) const { guest::StoreU32(base, address + 30568, v); }
  uint32_t unknown_30568_address() const { return address + 30568; }
  uint32_t unknown_30572() const { return guest::LoadU32(base, address + 30572); }
  void set_unknown_30572(uint32_t v) const { guest::StoreU32(base, address + 30572, v); }
  uint32_t unknown_30572_address() const { return address + 30572; }
  uint32_t unknown_30576() const { return guest::LoadU32(base, address + 30576); }
  void set_unknown_30576(uint32_t v) const { guest::StoreU32(base, address + 30576, v); }
  uint32_t unknown_30576_address() const { return address + 30576; }
  // the first light kit's groups, 4g per group g (+30596 + 4g and +30612 + 4g two more values; the garage car's vinyl layers' kinds are read from here too)
  uint32_t light_kit_a_address() const { return address + 30580; }
  // the second light kit's groups, 4g per group g
  uint32_t light_kit_b_address() const { return address + 30660; }
  uint32_t unknown_30712() const { return guest::LoadU32(base, address + 30712); }
  void set_unknown_30712(uint32_t v) const { guest::StoreU32(base, address + 30712, v); }
  uint32_t unknown_30712_address() const { return address + 30712; }
  uint32_t unknown_30716() const { return guest::LoadU32(base, address + 30716); }
  void set_unknown_30716(uint32_t v) const { guest::StoreU32(base, address + 30716, v); }
  uint32_t unknown_30716_address() const { return address + 30716; }
  uint32_t unknown_30720() const { return guest::LoadU32(base, address + 30720); }
  void set_unknown_30720(uint32_t v) const { guest::StoreU32(base, address + 30720, v); }
  uint32_t unknown_30720_address() const { return address + 30720; }
  // the vinyl page (0 or 1), or the sticker area (0 to 7)
  uint32_t editor_page() const { return guest::LoadU32(base, address + 30724); }
  void set_editor_page(uint32_t v) const { guest::StoreU32(base, address + 30724, v); }
  uint32_t editor_page_address() const { return address + 30724; }
  // set when +30724's last step wrapped
  uint32_t editor_page_wrapped() const { return guest::LoadU32(base, address + 30728); }
  void set_editor_page_wrapped(uint32_t v) const { guest::StoreU32(base, address + 30728, v); }
  uint32_t editor_page_wrapped_address() const { return address + 30728; }
  // the layer cursor, or the sticker slot (0 to 4; 5 and 6 two buttons)
  uint32_t layer_cursor() const { return guest::LoadU32(base, address + 30732); }
  void set_layer_cursor(uint32_t v) const { guest::StoreU32(base, address + 30732, v); }
  uint32_t layer_cursor_address() const { return address + 30732; }
  // the layer page's remembered button (3 or 4)
  uint32_t layer_button() const { return guest::LoadU32(base, address + 30736); }
  void set_layer_button(uint32_t v) const { guest::StoreU32(base, address + 30736, v); }
  uint32_t layer_button_address() const { return address + 30736; }
  // 0 edit, 1 order, 2 image, 3 remove
  uint32_t operation_cursor() const { return guest::LoadU32(base, address + 30740); }
  void set_operation_cursor(uint32_t v) const { guest::StoreU32(base, address + 30740, v); }
  uint32_t operation_cursor_address() const { return address + 30740; }
  uint32_t operation_wrapped() const { return guest::LoadU32(base, address + 30744); }
  void set_operation_wrapped(uint32_t v) const { guest::StoreU32(base, address + 30744, v); }
  uint32_t operation_wrapped_address() const { return address + 30744; }
  // 0 move, 1 rotate, 2 scale, 3 flip, 4 the free look, 5 and 6 two buttons
  uint32_t edit_cursor() const { return guest::LoadU32(base, address + 30748); }
  void set_edit_cursor(uint32_t v) const { guest::StoreU32(base, address + 30748, v); }
  uint32_t edit_cursor_address() const { return address + 30748; }
  uint32_t edit_row_saved() const { return guest::LoadU32(base, address + 30752); }
  void set_edit_row_saved(uint32_t v) const { guest::StoreU32(base, address + 30752, v); }
  uint32_t edit_row_saved_address() const { return address + 30752; }
  // state 27's position (the slot state 42 moves)
  uint32_t order_position() const { return guest::LoadU32(base, address + 30756); }
  void set_order_position(uint32_t v) const { guest::StoreU32(base, address + 30756, v); }
  uint32_t order_position_address() const { return address + 30756; }
  // 1 up, 2 down, 0 none
  uint32_t order_last_move() const { return guest::LoadU32(base, address + 30760); }
  void set_order_last_move(uint32_t v) const { guest::StoreU32(base, address + 30760, v); }
  uint32_t order_last_move_address() const { return address + 30760; }
  // the picked image or sticker
  uint32_t picked_image() const { return guest::LoadU32(base, address + 30764); }
  void set_picked_image(uint32_t v) const { guest::StoreU32(base, address + 30764, v); }
  uint32_t picked_image_address() const { return address + 30764; }
  // page 1's image group (0 to 11), the sticker picker's (0 to 5)
  uint32_t image_group() const { return guest::LoadU32(base, address + 30768); }
  void set_image_group(uint32_t v) const { guest::StoreU32(base, address + 30768, v); }
  uint32_t image_group_address() const { return address + 30768; }
  // 0 before state 35
  uint32_t before_state_35() const { return guest::LoadU32(base, address + 30772); }
  void set_before_state_35(uint32_t v) const { guest::StoreU32(base, address + 30772, v); }
  uint32_t before_state_35_address() const { return address + 30772; }
  // 0 before state 36
  uint32_t before_state_36() const { return guest::LoadU32(base, address + 30776); }
  void set_before_state_36(uint32_t v) const { guest::StoreU32(base, address + 30776, v); }
  uint32_t before_state_36_address() const { return address + 30776; }
  // 0 before states 37 and 52
  uint32_t before_state_37() const { return guest::LoadU32(base, address + 30780); }
  void set_before_state_37(uint32_t v) const { guest::StoreU32(base, address + 30780, v); }
  uint32_t before_state_37_address() const { return address + 30780; }
  // the editor's last page
  uint32_t last_page() const { return guest::LoadU32(base, address + 30784); }
  void set_last_page(uint32_t v) const { guest::StoreU32(base, address + 30784, v); }
  uint32_t last_page_address() const { return address + 30784; }
  // the editor's last layer
  uint32_t last_layer() const { return guest::LoadU32(base, address + 30788); }
  void set_last_layer(uint32_t v) const { guest::StoreU32(base, address + 30788, v); }
  uint32_t last_layer_address() const { return address + 30788; }
  // state 27's last position
  uint32_t order_last_position() const { return guest::LoadU32(base, address + 30792); }
  void set_order_last_position(uint32_t v) const { guest::StoreU32(base, address + 30792, v); }
  uint32_t order_last_position_address() const { return address + 30792; }
  // the image list's last index (4, 193 or 198)
  uint32_t image_last_index() const { return guest::LoadU32(base, address + 30796); }
  void set_image_last_index(uint32_t v) const { guest::StoreU32(base, address + 30796, v); }
  uint32_t image_last_index_address() const { return address + 30796; }
  // the pages' layout origins and scales
  uint32_t page0_origin_x() const { return guest::LoadU32(base, address + 30800); }
  void set_page0_origin_x(uint32_t v) const { guest::StoreU32(base, address + 30800, v); }
  uint32_t page0_origin_x_address() const { return address + 30800; }
  uint32_t page0_origin_y() const { return guest::LoadU32(base, address + 30804); }
  void set_page0_origin_y(uint32_t v) const { guest::StoreU32(base, address + 30804, v); }
  uint32_t page0_origin_y_address() const { return address + 30804; }
  float page0_size_scale() const { return guest::LoadFloat(base, address + 30808); }
  void set_page0_size_scale(float v) const { guest::StoreFloat(base, address + 30808, v); }
  uint32_t page0_size_scale_address() const { return address + 30808; }
  float page0_position_scale() const { return guest::LoadFloat(base, address + 30812); }
  void set_page0_position_scale(float v) const { guest::StoreFloat(base, address + 30812, v); }
  uint32_t page0_position_scale_address() const { return address + 30812; }
  uint32_t page1_origin_x() const { return guest::LoadU32(base, address + 30816); }
  void set_page1_origin_x(uint32_t v) const { guest::StoreU32(base, address + 30816, v); }
  uint32_t page1_origin_x_address() const { return address + 30816; }
  uint32_t page1_origin_y() const { return guest::LoadU32(base, address + 30820); }
  void set_page1_origin_y(uint32_t v) const { guest::StoreU32(base, address + 30820, v); }
  uint32_t page1_origin_y_address() const { return address + 30820; }
  float page1_size_scale() const { return guest::LoadFloat(base, address + 30824); }
  void set_page1_size_scale(float v) const { guest::StoreFloat(base, address + 30824, v); }
  uint32_t page1_size_scale_address() const { return address + 30824; }
  float page1_position_scale() const { return guest::LoadFloat(base, address + 30828); }
  void set_page1_position_scale(float v) const { guest::StoreFloat(base, address + 30828, v); }
  uint32_t page1_position_scale_address() const { return address + 30828; }
  // the edited decal or sticker
  uint32_t edited_x() const { return guest::LoadU32(base, address + 30832); }
  void set_edited_x(uint32_t v) const { guest::StoreU32(base, address + 30832, v); }
  uint32_t edited_x_address() const { return address + 30832; }
  uint32_t edited_y() const { return guest::LoadU32(base, address + 30836); }
  void set_edited_y(uint32_t v) const { guest::StoreU32(base, address + 30836, v); }
  uint32_t edited_y_address() const { return address + 30836; }
  float edited_angle() const { return guest::LoadFloat(base, address + 30840); }
  void set_edited_angle(float v) const { guest::StoreFloat(base, address + 30840, v); }
  uint32_t edited_angle_address() const { return address + 30840; }
  float edited_scale() const { return guest::LoadFloat(base, address + 30844); }
  void set_edited_scale(float v) const { guest::StoreFloat(base, address + 30844, v); }
  uint32_t edited_scale_address() const { return address + 30844; }
  uint32_t edited_flip() const { return guest::LoadU32(base, address + 30848); }
  void set_edited_flip(uint32_t v) const { guest::StoreU32(base, address + 30848, v); }
  uint32_t edited_flip_address() const { return address + 30848; }
  uint32_t edited_word() const { return guest::LoadU32(base, address + 30852); }
  void set_edited_word(uint32_t v) const { guest::StoreU32(base, address + 30852, v); }
  uint32_t edited_word_address() const { return address + 30852; }
  // a copy of the edited decal (restored on B in states 29 to 31)
  uint32_t edited_backup_address() const { return address + 30856; }
  // page 0's three vinyl layers, 24 bytes each (x, y, angle, scale, flag)
  uint32_t vinyl_layers_0_address() const { return address + 30880; }
  // page 1's
  uint32_t vinyl_layers_1_address() const { return address + 30952; }
  // page 0's saved copies (24-byte decal layout records)
  uint32_t saved_vinyl_layers_0_address() const { return address + 31024; }
  // page 1's
  uint32_t saved_vinyl_layers_1_address() const { return address + 31096; }
  // page 0's layers' current ids, 4j (-1 empty)
  uint32_t layer_ids_0_address() const { return address + 31168; }
  // page 1's
  uint32_t layer_ids_1_address() const { return address + 31180; }
  // page 0's saved ids
  uint32_t saved_layer_ids_0_address() const { return address + 31192; }
  // page 1's saved ids
  uint32_t saved_layer_ids_1_address() const { return address + 31204; }
  // state 27's work list's ids
  uint32_t order_ids_address() const { return address + 31216; }
  // state 27's work list's indices
  uint32_t order_indices_address() const { return address + 31228; }
  // the saved stickers, 120 x area + 24 x slot
  uint32_t saved_stickers_address() const { return address + 31240; }
  // their ids, 20 x area + 4 x slot
  uint32_t saved_sticker_ids_address() const { return address + 32200; }
  // the placed stickers, 120 x area + 24 x slot
  uint32_t stickers_address() const { return address + 32360; }
  // their ids, 20 x area + 4 x slot
  uint32_t sticker_ids_address() const { return address + 33320; }
  // state 42's order list (slot indices)
  uint32_t sticker_order_address() const { return address + 33480; }
  // 1 while 164 is held (the other slots hidden)
  uint32_t holding_164() const { return guest::LoadU32(base, address + 33500); }
  void set_holding_164(uint32_t v) const { guest::StoreU32(base, address + 33500, v); }
  uint32_t holding_164_address() const { return address + 33500; }
  // 1 while 163 is held (the picker shows the slot's sticker)
  uint32_t holding_163() const { return guest::LoadU32(base, address + 33504); }
  void set_holding_163(uint32_t v) const { guest::StoreU32(base, address + 33504, v); }
  uint32_t holding_163_address() const { return address + 33504; }
  // 1 when the garage car's id is 0
  uint32_t no_car_id() const { return guest::LoadU32(base, address + 33508); }
  void set_no_car_id(uint32_t v) const { guest::StoreU32(base, address + 33508, v); }
  uint32_t no_car_id_address() const { return address + 33508; }
  // a backup of the edited sticker (restored by B in states 44 to 46)
  uint32_t sticker_backup_address() const { return address + 33512; }
  // the sticker picker's wait (frames)
  uint32_t picker_wait() const { return guest::LoadU32(base, address + 33536); }
  void set_picker_wait(uint32_t v) const { guest::StoreU32(base, address + 33536, v); }
  uint32_t picker_wait_address() const { return address + 33536; }
  // the interior item's mode
  uint32_t interior_mode() const { return guest::LoadU32(base, address + 33540); }
  void set_interior_mode(uint32_t v) const { guest::StoreU32(base, address + 33540, v); }
  uint32_t interior_mode_address() const { return address + 33540; }
  uint32_t unknown_33544() const { return guest::LoadU32(base, address + 33544); }
  void set_unknown_33544(uint32_t v) const { guest::StoreU32(base, address + 33544, v); }
  uint32_t unknown_33544_address() const { return address + 33544; }
  // the interior accessory's pick or kind (GetPrice's mode 0 index; RefreshInterior compares it with 1)
  uint32_t interior_pick() const { return guest::LoadU32(base, address + 33548); }
  void set_interior_pick(uint32_t v) const { guest::StoreU32(base, address + 33548, v); }
  uint32_t interior_pick_address() const { return address + 33548; }
  uint32_t unknown_33552() const { return guest::LoadU32(base, address + 33552); }
  void set_unknown_33552(uint32_t v) const { guest::StoreU32(base, address + 33552, v); }
  uint32_t unknown_33552_address() const { return address + 33552; }
  uint32_t unknown_33556() const { return guest::LoadU32(base, address + 33556); }
  void set_unknown_33556(uint32_t v) const { guest::StoreU32(base, address + 33556, v); }
  uint32_t unknown_33556_address() const { return address + 33556; }
  // the interior pages' mode (4 hides a node)
  uint32_t interior_page_mode() const { return guest::LoadU32(base, address + 33564); }
  void set_interior_page_mode(uint32_t v) const { guest::StoreU32(base, address + 33564, v); }
  uint32_t interior_page_mode_address() const { return address + 33564; }
  // the picked pair's second index
  uint32_t interior_pair_index() const { return guest::LoadU32(base, address + 33572); }
  void set_interior_pair_index(uint32_t v) const { guest::StoreU32(base, address + 33572, v); }
  uint32_t interior_pair_index_address() const { return address + 33572; }
  // the slot (0 to 4)
  uint32_t interior_slot() const { return guest::LoadU32(base, address + 33576); }
  void set_interior_slot(uint32_t v) const { guest::StoreU32(base, address + 33576, v); }
  uint32_t interior_slot_address() const { return address + 33576; }
  uint32_t unknown_33580() const { return guest::LoadU32(base, address + 33580); }
  void set_unknown_33580(uint32_t v) const { guest::StoreU32(base, address + 33580, v); }
  uint32_t unknown_33580_address() const { return address + 33580; }
  uint32_t unknown_33588() const { return guest::LoadU32(base, address + 33588); }
  void set_unknown_33588(uint32_t v) const { guest::StoreU32(base, address + 33588, v); }
  uint32_t unknown_33588_address() const { return address + 33588; }
  uint32_t unknown_33592() const { return guest::LoadU32(base, address + 33592); }
  void set_unknown_33592(uint32_t v) const { guest::StoreU32(base, address + 33592, v); }
  uint32_t unknown_33592_address() const { return address + 33592; }
  uint32_t unknown_33596() const { return guest::LoadU32(base, address + 33596); }
  void set_unknown_33596(uint32_t v) const { guest::StoreU32(base, address + 33596, v); }
  uint32_t unknown_33596_address() const { return address + 33596; }
  // the interior item 3's current choice in mode 0 (GetCurrentChoice)
  uint32_t interior_choice() const { return guest::LoadU32(base, address + 33616); }
  void set_interior_choice(uint32_t v) const { guest::StoreU32(base, address + 33616, v); }
  uint32_t interior_choice_address() const { return address + 33616; }
  // the interior item 3's word the garage car and the stand take
  uint32_t interior_choice_word() const { return guest::LoadU32(base, address + 33620); }
  void set_interior_choice_word(uint32_t v) const { guest::StoreU32(base, address + 33620, v); }
  uint32_t interior_choice_word_address() const { return address + 33620; }
  uint32_t unknown_33624() const { return guest::LoadU32(base, address + 33624); }
  void set_unknown_33624(uint32_t v) const { guest::StoreU32(base, address + 33624, v); }
  uint32_t unknown_33624_address() const { return address + 33624; }
  uint32_t unknown_33640() const { return guest::LoadU32(base, address + 33640); }
  void set_unknown_33640(uint32_t v) const { guest::StoreU32(base, address + 33640, v); }
  uint32_t unknown_33640_address() const { return address + 33640; }
  // five choice codes (tens and units into the car's +1824 + 12i; -1 empty)
  uint32_t choice_code_0() const { return guest::LoadU32(base, address + 33800); }
  void set_choice_code_0(uint32_t v) const { guest::StoreU32(base, address + 33800, v); }
  uint32_t choice_code_0_address() const { return address + 33800; }
  uint32_t choice_code_1() const { return guest::LoadU32(base, address + 33804); }
  void set_choice_code_1(uint32_t v) const { guest::StoreU32(base, address + 33804, v); }
  uint32_t choice_code_1_address() const { return address + 33804; }
  uint32_t choice_code_2() const { return guest::LoadU32(base, address + 33808); }
  void set_choice_code_2(uint32_t v) const { guest::StoreU32(base, address + 33808, v); }
  uint32_t choice_code_2_address() const { return address + 33808; }
  uint32_t choice_code_3() const { return guest::LoadU32(base, address + 33812); }
  void set_choice_code_3(uint32_t v) const { guest::StoreU32(base, address + 33812, v); }
  uint32_t choice_code_3_address() const { return address + 33812; }
  uint32_t choice_code_4() const { return guest::LoadU32(base, address + 33816); }
  void set_choice_code_4(uint32_t v) const { guest::StoreU32(base, address + 33816, v); }
  uint32_t choice_code_4_address() const { return address + 33816; }
  uint32_t unknown_33820() const { return guest::LoadU32(base, address + 33820); }
  void set_unknown_33820(uint32_t v) const { guest::StoreU32(base, address + 33820, v); }
  uint32_t unknown_33820_address() const { return address + 33820; }
  // the power and power train tables' row
  uint32_t price_class() const { return guest::LoadU32(base, address + 33900); }
  void set_price_class(uint32_t v) const { guest::StoreU32(base, address + 33900, v); }
  uint32_t price_class_address() const { return address + 33900; }
  // the part tables: per item a price table (GetPrice), the current choice (GetCurrentChoice), flags (GetChoiceFlag) and IsItemAvailable's lists; +34104 + 40 x class + 4i the parts unlocked
  uint32_t part_tables_address() const { return address + 34104; }
  // power item 2's current choice (the garage car's +1672)
  uint32_t power2_level() const { return guest::LoadU32(base, address + 35020); }
  void set_power2_level(uint32_t v) const { guest::StoreU32(base, address + 35020, v); }
  uint32_t power2_level_address() const { return address + 35020; }
  // the cooling module's type (the car's +1668)
  uint32_t power2_type() const { return guest::LoadU32(base, address + 35024); }
  void set_power2_type(uint32_t v) const { guest::StoreU32(base, address + 35024, v); }
  uint32_t power2_type_address() const { return address + 35024; }
  // power item 3's (the car's +1676)
  uint32_t power3_level() const { return guest::LoadU32(base, address + 35140); }
  void set_power3_level(uint32_t v) const { guest::StoreU32(base, address + 35140, v); }
  uint32_t power3_level_address() const { return address + 35140; }
  // the power train's current choices (the car's +1680 to +1696)
  uint32_t train0_level() const { return guest::LoadU32(base, address + 35228); }
  void set_train0_level(uint32_t v) const { guest::StoreU32(base, address + 35228, v); }
  uint32_t train0_level_address() const { return address + 35228; }
  uint32_t train1_level() const { return guest::LoadU32(base, address + 35316); }
  void set_train1_level(uint32_t v) const { guest::StoreU32(base, address + 35316, v); }
  uint32_t train1_level_address() const { return address + 35316; }
  uint32_t train2_level() const { return guest::LoadU32(base, address + 35404); }
  void set_train2_level(uint32_t v) const { guest::StoreU32(base, address + 35404, v); }
  uint32_t train2_level_address() const { return address + 35404; }
  uint32_t train3_level() const { return guest::LoadU32(base, address + 35492); }
  void set_train3_level(uint32_t v) const { guest::StoreU32(base, address + 35492, v); }
  uint32_t train3_level_address() const { return address + 35492; }
  // the caliper's choice (SetCaliper)
  uint32_t train3_type() const { return guest::LoadU32(base, address + 35496); }
  void set_train3_type(uint32_t v) const { guest::StoreU32(base, address + 35496, v); }
  uint32_t train3_type_address() const { return address + 35496; }
  // the LSD's level (the car's +1700 and +1704 split it)
  uint32_t train4_level() const { return guest::LoadU32(base, address + 35608); }
  void set_train4_level(uint32_t v) const { guest::StoreU32(base, address + 35608, v); }
  uint32_t train4_level_address() const { return address + 35608; }
  // the body's current choice (compared with 3: which stand choices interior slots 3 and 4 map to)
  uint32_t body_choice() const { return guest::LoadU32(base, address + 35708); }
  void set_body_choice(uint32_t v) const { guest::StoreU32(base, address + 35708, v); }
  uint32_t body_choice_address() const { return address + 35708; }
  // the tire's current choice (item 0)
  uint32_t tire_both_choice() const { return guest::LoadU32(base, address + 35820); }
  void set_tire_both_choice(uint32_t v) const { guest::StoreU32(base, address + 35820, v); }
  uint32_t tire_both_choice_address() const { return address + 35820; }
  // the tire's front pair
  uint32_t tire_front_choice() const { return guest::LoadU32(base, address + 35948); }
  void set_tire_front_choice(uint32_t v) const { guest::StoreU32(base, address + 35948, v); }
  uint32_t tire_front_choice_address() const { return address + 35948; }
  uint32_t tire_front_option() const { return guest::LoadU32(base, address + 35952); }
  void set_tire_front_option(uint32_t v) const { guest::StoreU32(base, address + 35952, v); }
  uint32_t tire_front_option_address() const { return address + 35952; }
  // the tire's rear pair
  uint32_t tire_rear_choice() const { return guest::LoadU32(base, address + 36076); }
  void set_tire_rear_choice(uint32_t v) const { guest::StoreU32(base, address + 36076, v); }
  uint32_t tire_rear_choice_address() const { return address + 36076; }
  uint32_t tire_rear_option() const { return guest::LoadU32(base, address + 36080); }
  void set_tire_rear_option(uint32_t v) const { guest::StoreU32(base, address + 36080, v); }
  uint32_t tire_rear_option_address() const { return address + 36080; }
  // the aero items' current types (the car's +1744 to +1784)
  uint32_t aero0_type() const { return guest::LoadU32(base, address + 36164); }
  void set_aero0_type(uint32_t v) const { guest::StoreU32(base, address + 36164, v); }
  uint32_t aero0_type_address() const { return address + 36164; }
  uint32_t aero1_type() const { return guest::LoadU32(base, address + 36208); }
  void set_aero1_type(uint32_t v) const { guest::StoreU32(base, address + 36208, v); }
  uint32_t aero1_type_address() const { return address + 36208; }
  uint32_t aero2_type() const { return guest::LoadU32(base, address + 36252); }
  void set_aero2_type(uint32_t v) const { guest::StoreU32(base, address + 36252, v); }
  uint32_t aero2_type_address() const { return address + 36252; }
  // the rear spoiler's (with +25172 set remapped)
  uint32_t aero3_type() const { return guest::LoadU32(base, address + 36300); }
  void set_aero3_type(uint32_t v) const { guest::StoreU32(base, address + 36300, v); }
  uint32_t aero3_type_address() const { return address + 36300; }
  // the bonnet's (the car's +1760)
  uint32_t aero4_type() const { return guest::LoadU32(base, address + 36368); }
  void set_aero4_type(uint32_t v) const { guest::StoreU32(base, address + 36368, v); }
  uint32_t aero4_type_address() const { return address + 36368; }
  // the bonnet's color kind 0 to 2 (the car's +1764)
  uint32_t aero4_color_kind() const { return guest::LoadU32(base, address + 36372); }
  void set_aero4_color_kind(uint32_t v) const { guest::StoreU32(base, address + 36372, v); }
  uint32_t aero4_color_kind_address() const { return address + 36372; }
  uint32_t aero4_color() const { return guest::LoadU32(base, address + 36376); }
  void set_aero4_color(uint32_t v) const { guest::StoreU32(base, address + 36376, v); }
  uint32_t aero4_color_address() const { return address + 36376; }
  uint32_t aero5_type() const { return guest::LoadU32(base, address + 36436); }
  void set_aero5_type(uint32_t v) const { guest::StoreU32(base, address + 36436, v); }
  uint32_t aero5_type_address() const { return address + 36436; }
  uint32_t aero6_type() const { return guest::LoadU32(base, address + 36472); }
  void set_aero6_type(uint32_t v) const { guest::StoreU32(base, address + 36472, v); }
  uint32_t aero6_type_address() const { return address + 36472; }
  uint32_t aero7_type() const { return guest::LoadU32(base, address + 36508); }
  void set_aero7_type(uint32_t v) const { guest::StoreU32(base, address + 36508, v); }
  uint32_t aero7_type_address() const { return address + 36508; }
  uint32_t aero8_type() const { return guest::LoadU32(base, address + 36544); }
  void set_aero8_type(uint32_t v) const { guest::StoreU32(base, address + 36544, v); }
  uint32_t aero8_type_address() const { return address + 36544; }
  // aero item 9's current choice (AeroParts::Get82 takes it for the rear spoiler's help line)
  uint32_t aero9_type() const { return guest::LoadU32(base, address + 36588); }
  void set_aero9_type(uint32_t v) const { guest::StoreU32(base, address + 36588, v); }
  uint32_t aero9_type_address() const { return address + 36588; }
  // the door type (the car's +1792)
  uint32_t exterior1_choice() const { return guest::LoadU32(base, address + 36612); }
  void set_exterior1_choice(uint32_t v) const { guest::StoreU32(base, address + 36612, v); }
  uint32_t exterior1_choice_address() const { return address + 36612; }
  // the car's byte +1568
  uint32_t exterior2_choice() const { return guest::LoadU32(base, address + 36636); }
  void set_exterior2_choice(uint32_t v) const { guest::StoreU32(base, address + 36636, v); }
  uint32_t exterior2_choice_address() const { return address + 36636; }
  // the car's +1796
  uint32_t exterior3_choice() const { return guest::LoadU32(base, address + 36668); }
  void set_exterior3_choice(uint32_t v) const { guest::StoreU32(base, address + 36668, v); }
  uint32_t exterior3_choice_address() const { return address + 36668; }
  // the interior items' current choices (the car's +1800 to +1812)
  uint32_t interior0_choice() const { return guest::LoadU32(base, address + 36708); }
  void set_interior0_choice(uint32_t v) const { guest::StoreU32(base, address + 36708, v); }
  uint32_t interior0_choice_address() const { return address + 36708; }
  uint32_t interior0_level() const { return guest::LoadU32(base, address + 36712); }
  void set_interior0_level(uint32_t v) const { guest::StoreU32(base, address + 36712, v); }
  uint32_t interior0_level_address() const { return address + 36712; }
  uint32_t interior1_choice() const { return guest::LoadU32(base, address + 36752); }
  void set_interior1_choice(uint32_t v) const { guest::StoreU32(base, address + 36752, v); }
  uint32_t interior1_choice_address() const { return address + 36752; }
  uint32_t interior2_choice() const { return guest::LoadU32(base, address + 36796); }
  void set_interior2_choice(uint32_t v) const { guest::StoreU32(base, address + 36796, v); }
  uint32_t interior2_choice_address() const { return address + 36796; }
  // set when a paint is kept, 0 when discarded; when set the garage car's halfwords +0 to +4 = -2
  uint32_t paint_kept() const { return guest::LoadU32(base, address + 36824); }
  void set_paint_kept(uint32_t v) const { guest::StoreU32(base, address + 36824, v); }
  uint32_t paint_kept_address() const { return address + 36824; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::Refresh)
  uint32_t unknown_46244() const { return guest::LoadU32(base, address + 46244); }
  void set_unknown_46244(uint32_t v) const { guest::StoreU32(base, address + 46244, v); }
  uint32_t unknown_46244_address() const { return address + 46244; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::Refresh)
  uint8_t unknown_46248() const { return guest::LoadU8(base, address + 46248); }
  void set_unknown_46248(uint8_t v) const { guest::StoreU8(base, address + 46248, v); }
  uint32_t unknown_46248_address() const { return address + 46248; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::Refresh)
  uint8_t unknown_46250() const { return guest::LoadU8(base, address + 46250); }
  void set_unknown_46250(uint8_t v) const { guest::StoreU8(base, address + 46250, v); }
  uint32_t unknown_46250_address() const { return address + 46250; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::Refresh)
  uint8_t unknown_46251() const { return guest::LoadU8(base, address + 46251); }
  void set_unknown_46251(uint8_t v) const { guest::StoreU8(base, address + 46251, v); }
  uint32_t unknown_46251_address() const { return address + 46251; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::Refresh)
  uint8_t unknown_46252() const { return guest::LoadU8(base, address + 46252); }
  void set_unknown_46252(uint8_t v) const { guest::StoreU8(base, address + 46252, v); }
  uint32_t unknown_46252_address() const { return address + 46252; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::Refresh)
  uint8_t unknown_46267() const { return guest::LoadU8(base, address + 46267); }
  void set_unknown_46267(uint8_t v) const { guest::StoreU8(base, address + 46267, v); }
  uint32_t unknown_46267_address() const { return address + 46267; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::Refresh)
  uint8_t unknown_46268() const { return guest::LoadU8(base, address + 46268); }
  void set_unknown_46268(uint8_t v) const { guest::StoreU8(base, address + 46268, v); }
  uint32_t unknown_46268_address() const { return address + 46268; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::Refresh)
  uint32_t unknown_46304() const { return guest::LoadU32(base, address + 46304); }
  void set_unknown_46304(uint32_t v) const { guest::StoreU32(base, address + 46304, v); }
  uint32_t unknown_46304_address() const { return address + 46304; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61212() const { return guest::LoadU32(base, address + 61212); }
  void set_unknown_61212(uint32_t v) const { guest::StoreU32(base, address + 61212, v); }
  uint32_t unknown_61212_address() const { return address + 61212; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61228() const { return guest::LoadU32(base, address + 61228); }
  void set_unknown_61228(uint32_t v) const { guest::StoreU32(base, address + 61228, v); }
  uint32_t unknown_61228_address() const { return address + 61228; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61244() const { return guest::LoadU32(base, address + 61244); }
  void set_unknown_61244(uint32_t v) const { guest::StoreU32(base, address + 61244, v); }
  uint32_t unknown_61244_address() const { return address + 61244; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61260() const { return guest::LoadU32(base, address + 61260); }
  void set_unknown_61260(uint32_t v) const { guest::StoreU32(base, address + 61260, v); }
  uint32_t unknown_61260_address() const { return address + 61260; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61276() const { return guest::LoadU32(base, address + 61276); }
  void set_unknown_61276(uint32_t v) const { guest::StoreU32(base, address + 61276, v); }
  uint32_t unknown_61276_address() const { return address + 61276; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61292() const { return guest::LoadU32(base, address + 61292); }
  void set_unknown_61292(uint32_t v) const { guest::StoreU32(base, address + 61292, v); }
  uint32_t unknown_61292_address() const { return address + 61292; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61308() const { return guest::LoadU32(base, address + 61308); }
  void set_unknown_61308(uint32_t v) const { guest::StoreU32(base, address + 61308, v); }
  uint32_t unknown_61308_address() const { return address + 61308; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61324() const { return guest::LoadU32(base, address + 61324); }
  void set_unknown_61324(uint32_t v) const { guest::StoreU32(base, address + 61324, v); }
  uint32_t unknown_61324_address() const { return address + 61324; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_61340() const { return guest::LoadU32(base, address + 61340); }
  void set_unknown_61340(uint32_t v) const { guest::StoreU32(base, address + 61340, v); }
  uint32_t unknown_61340_address() const { return address + 61340; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::DebugLayerOrder)
  uint32_t unknown_64700() const { return guest::LoadU32(base, address + 64700); }
  void set_unknown_64700(uint32_t v) const { guest::StoreU32(base, address + 64700, v); }
  uint32_t unknown_64700_address() const { return address + 64700; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint8_t unknown_70988() const { return guest::LoadU8(base, address + 70988); }
  void set_unknown_70988(uint8_t v) const { guest::StoreU8(base, address + 70988, v); }
  uint32_t unknown_70988_address() const { return address + 70988; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_86524() const { return guest::LoadU32(base, address + 86524); }
  void set_unknown_86524(uint32_t v) const { guest::StoreU32(base, address + 86524, v); }
  uint32_t unknown_86524_address() const { return address + 86524; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_86568() const { return guest::LoadU32(base, address + 86568); }
  void set_unknown_86568(uint32_t v) const { guest::StoreU32(base, address + 86568, v); }
  uint32_t unknown_86568_address() const { return address + 86568; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_86572() const { return guest::LoadU32(base, address + 86572); }
  void set_unknown_86572(uint32_t v) const { guest::StoreU32(base, address + 86572, v); }
  uint32_t unknown_86572_address() const { return address + 86572; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_89160() const { return guest::LoadU32(base, address + 89160); }
  void set_unknown_89160(uint32_t v) const { guest::StoreU32(base, address + 89160, v); }
  uint32_t unknown_89160_address() const { return address + 89160; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_89164() const { return guest::LoadU32(base, address + 89164); }
  void set_unknown_89164(uint32_t v) const { guest::StoreU32(base, address + 89164, v); }
  uint32_t unknown_89164_address() const { return address + 89164; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_89168() const { return guest::LoadU32(base, address + 89168); }
  void set_unknown_89168(uint32_t v) const { guest::StoreU32(base, address + 89168, v); }
  uint32_t unknown_89168_address() const { return address + 89168; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_91804() const { return guest::LoadU32(base, address + 91804); }
  void set_unknown_91804(uint32_t v) const { guest::StoreU32(base, address + 91804, v); }
  uint32_t unknown_91804_address() const { return address + 91804; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_91808() const { return guest::LoadU32(base, address + 91808); }
  void set_unknown_91808(uint32_t v) const { guest::StoreU32(base, address + 91808, v); }
  uint32_t unknown_91808_address() const { return address + 91808; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_91812() const { return guest::LoadU32(base, address + 91812); }
  void set_unknown_91812(uint32_t v) const { guest::StoreU32(base, address + 91812, v); }
  uint32_t unknown_91812_address() const { return address + 91812; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_91816() const { return guest::LoadU32(base, address + 91816); }
  void set_unknown_91816(uint32_t v) const { guest::StoreU32(base, address + 91816, v); }
  uint32_t unknown_91816_address() const { return address + 91816; }
  // probably a measure fault: a register the measure kept as the screen after it was reused (TuningScreen::RestoreStand)
  uint32_t unknown_91820() const { return guest::LoadU32(base, address + 91820); }
  void set_unknown_91820(uint32_t v) const { guest::StoreU32(base, address + 91820, v); }
  uint32_t unknown_91820_address() const { return address + 91820; }
};

// allocated; measure: TuningScreen.+10036.+16
struct TuningCommandQueue : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the state whose input page HandleInput runs (-1 while a command runs)
  uint32_t page() const { return guest::LoadU32(base, address + 0); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t page_address() const { return address + 0; }
  // the state to resume after the command
  uint32_t resume() const { return guest::LoadU32(base, address + 4); }
  void set_resume(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t resume_address() const { return address + 4; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 8); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t previous_page_address() const { return address + 8; }
  // the current command (0 to 64, -1 none)
  uint32_t current() const { return guest::LoadU32(base, address + 12); }
  void set_current(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t current_address() const { return address + 12; }
  // the next command or -1 (the measure took it for a pointer)
  uint32_t next() const { return guest::LoadU32(base, address + 16); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t next_address() const { return address + 16; }
  // 0 when a command is made current
  uint32_t step() const { return guest::LoadU32(base, address + 20); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t step_address() const { return address + 20; }
  uint32_t started() const { return guest::LoadU32(base, address + 24); }
  void set_started(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t started_address() const { return address + 24; }
};

// file; measure: UiMovie::Child.ret
struct UiMovieNode : View {
  using View::View;
  static constexpr uint32_t kSize = 140;
  // GRP, PIC, PRM, SCE, TFM, TXT
  uint32_t tag() const { return guest::LoadU32(base, address + 0); }
  void set_tag(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tag_address() const { return address + 0; }
  // tracks find nodes by it
  uint32_t id() const { return guest::LoadU32(base, address + 8); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t id_address() const { return address + 8; }
  // the position (also read as words)
  float x() const { return guest::LoadFloat(base, address + 12); }
  void set_x(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t x_address() const { return address + 12; }
  float y() const { return guest::LoadFloat(base, address + 16); }
  void set_y(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t y_address() const { return address + 16; }
  uint32_t pivot_x() const { return guest::LoadU32(base, address + 20); }
  void set_pivot_x(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t pivot_x_address() const { return address + 20; }
  uint32_t pivot_y() const { return guest::LoadU32(base, address + 24); }
  void set_pivot_y(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t pivot_y_address() const { return address + 24; }
  float scale_x() const { return guest::LoadFloat(base, address + 28); }
  void set_scale_x(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t scale_x_address() const { return address + 28; }
  float scale_y() const { return guest::LoadFloat(base, address + 32); }
  void set_scale_y(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t scale_y_address() const { return address + 32; }
  // degrees
  float rotation() const { return guest::LoadFloat(base, address + 36); }
  void set_rotation(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t rotation_address() const { return address + 36; }
  // a clip rectangle (+40 to +52)
  uint32_t clip_0() const { return guest::LoadU32(base, address + 40); }
  void set_clip_0(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t clip_0_address() const { return address + 40; }
  uint32_t clip_1() const { return guest::LoadU32(base, address + 44); }
  void set_clip_1(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t clip_1_address() const { return address + 44; }
  uint32_t clip_2() const { return guest::LoadU32(base, address + 48); }
  void set_clip_2(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t clip_2_address() const { return address + 48; }
  uint32_t clip_3() const { return guest::LoadU32(base, address + 52); }
  void set_clip_3(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t clip_3_address() const { return address + 52; }
  // the color (0-255)
  uint32_t red() const { return guest::LoadU32(base, address + 56); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t red_address() const { return address + 56; }
  uint32_t green() const { return guest::LoadU32(base, address + 60); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t green_address() const { return address + 60; }
  uint32_t blue() const { return guest::LoadU32(base, address + 64); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t blue_address() const { return address + 64; }
  float alpha() const { return guest::LoadFloat(base, address + 68); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t alpha_address() const { return address + 68; }
  uint32_t layer_offset() const { return guest::LoadU32(base, address + 72); }
  void set_layer_offset(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t layer_offset_address() const { return address + 72; }
  uint32_t unknown_76() const { return guest::LoadU32(base, address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t visible() const { return guest::LoadU32(base, address + 80); }
  void set_visible(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t visible_address() const { return address + 80; }
  // a picture's size; a group's first child index (also read as words)
  float width() const { return guest::LoadFloat(base, address + 100); }
  void set_width(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t width_address() const { return address + 100; }
  // a picture's size; a group's child count; a primitive's texture entry
  float height() const { return guest::LoadFloat(base, address + 104); }
  void set_height(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t height_address() const { return address + 104; }
  // a picture's texture entry (the frame index the screens set); a scene's child nodes
  uint32_t texture_entry() const { return guest::LoadU32(base, address + 108); }
  void set_texture_entry(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t texture_entry_address() const { return address + 108; }
  // a scene's child count; a primitive textured
  uint32_t unknown_112() const { return guest::LoadU32(base, address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  // a picture's texture pixel rectangle (+116 to +128); a scene's animations
  float texture_left() const { return guest::LoadFloat(base, address + 116); }
  void set_texture_left(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t texture_left_address() const { return address + 116; }
  float texture_top() const { return guest::LoadFloat(base, address + 120); }
  void set_texture_top(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t texture_top_address() const { return address + 120; }
  uint32_t texture_right() const { return guest::LoadU32(base, address + 124); }
  void set_texture_right(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t texture_right_address() const { return address + 124; }
  uint32_t texture_bottom() const { return guest::LoadU32(base, address + 128); }
  void set_texture_bottom(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t texture_bottom_address() const { return address + 128; }
  uint32_t flip_x() const { return guest::LoadU32(base, address + 132); }
  void set_flip_x(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t flip_x_address() const { return address + 132; }
  uint32_t flip_y() const { return guest::LoadU32(base, address + 136); }
  void set_flip_y(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t flip_y_address() const { return address + 136; }
};

// allocated; measure: new<24>@StandScene::Create
struct StandHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // a block holding the garage stage at +0
  uint32_t stage_block_0() const { return guest::LoadU32(base, address + 0); }
  void set_stage_block_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t stage_block_0_address() const { return address + 0; }
  // PanelSetBase::UpdateStand
  uint32_t stage_block_1() const { return guest::LoadU32(base, address + 4); }
  void set_stage_block_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t stage_block_1_address() const { return address + 4; }
  // PanelSetBase::UpdateStand
  uint32_t stage_block_2() const { return guest::LoadU32(base, address + 8); }
  void set_stage_block_2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t stage_block_2_address() const { return address + 8; }
  // StandScene::Create, PollTeardown
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // StandScene::Create, PollTeardown
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // a stand model was made (GarageStage::MakeModel)
  uint32_t model_made() const { return guest::LoadU32(base, address + 20); }
  void set_model_made(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t model_made_address() const { return address + 20; }
};

// embedded; measure: DayBlock
struct DayBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 104824;
  // also read as a word
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // GarageScreen::CopyGarageCar
  uint16_t unknown_204() const { return guest::LoadU16(base, address + 204); }
  void set_unknown_204(uint16_t v) const { guest::StoreU16(base, address + 204, v); }
  uint32_t unknown_204_address() const { return address + 204; }
  // the day's win streak
  uint16_t win_streak() const { return guest::LoadU16(base, address + 4828); }
  void set_win_streak(uint16_t v) const { guest::StoreU16(base, address + 4828, v); }
  uint32_t win_streak_address() const { return address + 4828; }
  // the day's best
  uint16_t best_streak() const { return guest::LoadU16(base, address + 4830); }
  void set_best_streak(uint16_t v) const { guest::StoreU16(base, address + 4830, v); }
  uint32_t best_streak_address() const { return address + 4830; }
  // DayBlock::AddWord4832
  uint32_t word_4832() const { return guest::LoadU32(base, address + 4832); }
  void set_word_4832(uint32_t v) const { guest::StoreU32(base, address + 4832, v); }
  uint32_t word_4832_address() const { return address + 4832; }
  // measure fault: the save data's money
  uint32_t money() const { return guest::LoadU32(base, address + 6116); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 6116, v); }
  uint32_t money_address() const { return address + 6116; }
  // measure fault: the save data's garage halfwords
  uint16_t unknown_6324() const { return guest::LoadU16(base, address + 6324); }
  void set_unknown_6324(uint16_t v) const { guest::StoreU16(base, address + 6324, v); }
  uint32_t unknown_6324_address() const { return address + 6324; }
  uint16_t unknown_6328() const { return guest::LoadU16(base, address + 6328); }
  void set_unknown_6328(uint16_t v) const { guest::StoreU16(base, address + 6328, v); }
  uint32_t unknown_6328_address() const { return address + 6328; }
  // measure fault: TuningScreen::StoreOwnedParts
  uint32_t unknown_104820() const { return guest::LoadU32(base, address + 104820); }
  void set_unknown_104820(uint32_t v) const { guest::StoreU32(base, address + 104820, v); }
  uint32_t unknown_104820_address() const { return address + 104820; }
};

// embedded; measure: TuningScreen::Command52.r4
struct TuningDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 messages of 12 bytes (+0 the event, +4, +8)
  uint32_t messages_address() const { return address + 204; }
  uint32_t message_count() const { return guest::LoadU32(base, address + 804); }
  void set_message_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t message_count_address() const { return address + 804; }
  uint32_t message_first() const { return guest::LoadU32(base, address + 808); }
  void set_message_first(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t message_first_address() const { return address + 808; }
};

// embedded; measure: UiMovie.+0
struct TuningMovieFirstNode : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  uint32_t tag() const { return guest::LoadU32(base, address + 0); }
  void set_tag(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tag_address() const { return address + 0; }
  uint32_t scale_y() const { return guest::LoadU32(base, address + 32); }
  void set_scale_y(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t scale_y_address() const { return address + 32; }
  uint32_t clip_0() const { return guest::LoadU32(base, address + 40); }
  void set_clip_0(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t clip_0_address() const { return address + 40; }
  uint32_t clip_1() const { return guest::LoadU32(base, address + 44); }
  void set_clip_1(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t clip_1_address() const { return address + 44; }
};

// embedded; measure: TuningScreen.+9656
struct TuningScreenCategoryWord : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // measure fault: an action mapper's vtable slot 10 (ActionMapper::Pressed) in MoveCategory, not reached through the category
  uint32_t pressed_slot() const { return guest::LoadU32(base, address + 40); }
  void set_pressed_slot(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t pressed_slot_address() const { return address + 40; }
};

// The image words docs/functions/tuning-screen.md's code uses.
struct TuningScreenGlobals {
  // the stand scene's holder (StandScene::Create): +0 the block holding the garage stage
  template <typename T = StandHolder>
  static T stand_holder(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83E0u, v.address); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
};

}  // namespace schema
